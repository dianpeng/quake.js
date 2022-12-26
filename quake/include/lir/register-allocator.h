#ifndef _QUAKE_LIR_REGISTER_ALLOCATOR_H_
#define _QUAKE_LIR_REGISTER_ALLOCATOR_H_
#include "base.h"
#include "common.h"
#include "zone.h"

#include "lir/liveness.h"
#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/assembler.h"

// ----------------------------------------------------------------------------
//
// Linear Scan Register Allocator
//
// The algorithm is been splitted into 3 different phases
//
// 1) Linearize the graph, ie mark each node inside of the LIR graph have a
//    number which forms a total order between each of them. The number is
//    been orgnized as following, the SCC part of the graph will always have
//    number smaller than the PHI node belongs to it, ie PHI node is always
//    numbered right after the corresponding SCC is been done. Which means
//    the PHI node will always have liveness range which covers the full control
//    flow structure.
//
// 2) Liveness analysis. This part will start to mark the coaerse liveness of
//    LIR graph's each node inside. It walks through the LIR in reverse order
//    and given each node a lower-bound of its def-use range W.R.T linearized
//    number.
//
// 3) RA part. This process will start to assign register for all the live
//    interval been generated during the liveness analysis. Internally we
//    will transfer the liveness_info's structure into LiveInterval and during
//    RA part each node will finally have a list of LiveInterval plus *MOVE*
//    in between to allow register transfer. Each LiveInterval will contain a
//    register or spilled slot for that node, until the node is dead.
//
//    Notes, during RA, the RA is also responsible for following things:
//
//    3.1) calling convention resolution
//    3.2) regsiter hint/requirement constraint solving via rule based solver
//    3.3) phi elimination via simple move resolution. Additionally phi's
//         usage will be reflected as forward register hinting that feed into
//         RA to allow optimization
//
// ----------------------------------------------------------------------------
//
// FIXME(dpeng):
//
//   Currently the register allocation is kind of messy and very brutle force.
//   For example, the constraint expression is really specific and brutle force,
//   ie via primitive C++ code instead of a data oritented declarative way.  We
//   may need to rewrite it a little bit. Notes currently the constraint solving
//   for pinned register is not ideal and/will typically generate bad code,
//   since it just blindlessly reserve all the involved register, ie X64's
//   imul/idiv instruction, and this does will result in some operand to be
//   used as part of imul/idiv to be spilled on stack/ or move to other register
//   and later on pick it up, may be in the same registers. We rely on post
//   RA process to eliminate these operations
//
//   The basic point for refactoried is to introduce real virtual register
//   concept and move/load/store instruction sequences. We can perform PHI
//   elimination by inserting move/load/store instruction along with virtual
//   register, this breaks SSA invariants. Currently, the move/load/store
//   is been managed by RA implicitly during different phase and very specific
//   to the tracing graph. Current RA is also not capable of allocating inside
//   of a full CFG and have specific assumption on how the tracing graph is
//   been organized.
//
//   We need to do following refactory in the future :
//
//   1) Add virtual register, and decouple the RA from allocating to instruction
//      by allocation to virtual register
//
//   2) Add constraint solving by using move/load/store instruction into the
//      original graph. This gonna break SSA and will require ParallelMove
//      to be capable of handling more than just PHI shuffling. Ie a previous
//      move's result will be consumed by later move as input. This is not
//      possible during PHI shuffling
//
//   3) Add bin-packing algorithm as pre-processing of RA pass. This allows
//      the input for RA to be simplified and speed up the RA pass time cost
//
// For now, we don't, instead we just stick to the register hinting method to
// allow RA to consider multiple forward-looking heuristics to allow RA to
// generate relative good code.
//
// ----------------------------------------------------------------------------

namespace quake {
namespace ir {
class Node;
}  // namespace ir

namespace lir {

class LiveInterval;
class RegisterAllocator;

// Notes, this represents an allocation of RA, it can be a register or a spilled
// slot
struct RegisterAllocation {
  const Register* r;       // an allocated register
  SizeClass size_class;    // size class of RA
  MemoryStack::Slot slot;  // a spilled slot
  LivenessRange range;     // the range that the register allocation happens or
                           // be responsible for

  HInstruction*
      placeholder;  // Used to indicate the register shuffling position
                    // We cannot really describe them correctly use the
                    // range since it is impossible to describe the
                    // dependency. We will add a placeholder
                    // instruction at the assembler graph and let user
                    // replace it with the correct mov/shuffle
 public:
  RegisterAllocation(const Register* reg, SizeClass sc, const LivenessRange& r)
      : r(reg),
        size_class(sc),
        slot(MemoryStack::kInvalidSlot),
        range(r),
        placeholder(nullptr) {}

  RegisterAllocation(MemoryStack::Slot s, const LivenessRange& r)
      : r(nullptr), slot(s), range(r), placeholder(nullptr) {}

  RegisterAllocation() = default;

  RegisterAllocation(RegisterAllocation&&) = default;
  RegisterAllocation(const RegisterAllocation&) = default;

  bool IsRegister() const { return r != nullptr; }
  bool IsSpill() const { return r == nullptr; }

 public:
  // Materialize the RegisterAllocation result into a real operand. After the
  // conversion the use-def high level information will be lost. The returned
  // operand is guaranteed to be materialized
  HOperand* ToOperand(HAssembler*, LiveInterval*) const;
  bool SameAllocation(const RegisterAllocation&) const;

  std::string Print() const;
};

// A list of live interval. Each live interval is DISJOINT regardlessly, ie
// no overlapping with each other and must be transferred into other live
// interval or dead node via MOVE/PUSH operation
class LiveInterval {
 public:
  enum {
    kMain,     // main interval, ie related to an instruction's output
    kReserve,  // reserve interval, ie related to an instruction's reseved
               // params
  };

  // General live interval list
  LiveInterval(zone::Zone* zone, HInstruction* ins, const LivenessRange& range)
      : instruction_(ins),
        range_(range),
        allocate_(zone),
        register_info_(),
        type_(kMain),
        temp_range_(),
        is_temp_(false) {}

  // Live interval list corresponding to a reserved register
  LiveInterval(zone::Zone* zone, HInstruction* ins, const LivenessPoint& start,
               const Register* forced_register)
      : instruction_(ins),
        range_(LivenessRange::OneTimestamp(start)),
        allocate_(zone),
        register_info_(),
        type_(kReserve),
        temp_range_(),
        is_temp_(false) {
    // Set the register info have the forced one, which will be repsected by
    // the RA
    register_info_.requirement =
        RegisterAllocationRequirement::ForceRegister(forced_register);
  }

  // Live interval list corresponding to a reserved register and an alias
  LiveInterval(zone::Zone* zone, HInstruction* ins, const LivenessPoint& start,
               const Register* forced_register, HInstruction* alias)
      : instruction_(ins),
        range_(LivenessRange::OneTimestamp(start)),
        allocate_(zone),
        register_info_(),
        type_(kReserve),
        temp_range_(),
        is_temp_(false),
        alias_(alias) {
    // Set the register info have the forced one, which will be repsected by
    // the RA
    register_info_.requirement =
        RegisterAllocationRequirement::ForceRegister(forced_register);
  }

  int type() const { return type_; }
  bool IsMain() const { return type_ == kMain; }
  bool IsReserve() const { return type_ == kReserve; }
  bool IsTemp() const { return is_temp_; }
  void SetTemp(const LivenessRange& range) {
    is_temp_ = true;
    temp_range_ = range;
  }
  void ClearTemp() { is_temp_ = false; }

  HInstruction* instruction() const { return instruction_; }
  HInstruction* instruction_checked() const {
    CHECK(instruction_);
    return instruction_;
  }
  const LivenessRange& range() const { return is_temp_ ? temp_range_ : range_; }

  // Used after the liveness analysis is done
  void FlushRange() {
    if (!IsReserve()) range_ = instruction_->liveness_info().range();
  }

  void SetUpOutput();
  void SetUpReserve();

  // Check whether current allocated interval is expired at specified point.
  bool ExpireWhen(const LivenessPoint&) const;

  // Means this instruction can skip the RA output
  bool MustHaveOutput() const {
    return instruction_checked()->IsValue() ||
           register_info_.requirement.IsForceAnyRegister() ||
           register_info_.requirement.IsForceRegister();
  }

  std::string Print() const;

 public:
  // Alias instruction. Notes this alias instruction feature is only applicable
  // when the LiveInterval represents a temporary allocation, result in
  // from the reserve allocation hint
  bool HasAlias() const {
    CHECK(IsReserve());
    return alias_ != nullptr;
  }

  HInstruction* alias() const {
    CHECK(IsReserve());
    return alias_;
  }

 public:
  RegisterAllocation* AllocateRegister(const Register*, SizeClass,
                                       const LivenessPoint&,
                                       RegisterAllocator*);

  RegisterAllocation* AllocateSpill(MemoryStack::Slot, const LivenessPoint&,
                                    RegisterAllocator*);

  bool HasAllocation() const { return !allocate_.empty(); }
  bool HasShuffle() const { return allocate_.size() > 1; }

  bool MatchFirstAllocation() const;
  const RegisterAllocation& FirstAllocation() const {
    CHECK(!allocate_.empty());
    return allocate_.front();
  }
  const RegisterAllocation& LastAllocation() const {
    CHECK(!allocate_.empty());
    return allocate_.back();
  }

  // Get allocation at the point
  const RegisterAllocation& AllocationAtOrAfter(const LivenessPoint&) const;

  const RegisterAllocation& AllocationIndex(std::size_t index) const {
    return allocate_[index];
  }

  // If last allocation (current) happened to be a register allocation, then
  // tries to get its value
  const Register* MaybeRegisterAllocation(const LivenessPoint&) const;
  std::optional<MemoryStack::Slot> MaybeStackAllocation(
      const LivenessPoint&) const;

  template <typename T>
  void ForeachRestAllocation(T&& func) {
    for (std::size_t i = 1; i < allocate_.size(); i++) {
      const auto& x = allocate_[i];
      func(x);
    }
  }

 public:
  bool HasForce() const { return register_info_.requirement.IsForceRegister(); }
  const RegisterInfo& register_info() const { return register_info_; }
  RegisterInfo& register_info() { return register_info_; }
  LivenessRange AllocateRange(const LivenessPoint& lp) const {
    return LivenessRange{lp, NextAllocateEnd()};
  }

 private:
  LivenessPoint NextAllocateEnd() const;

 private:
  HInstruction* instruction_;

  LivenessRange range_;
  zone::Vector<RegisterAllocation> allocate_;
  RegisterInfo register_info_;
  int type_;

  LivenessRange temp_range_;  // Used when allocating a temporary register
  bool is_temp_;

  // alias instruction. This is mainly used to avoid adding redundancy move
  // during ABI register shuffling. For a LiveInterval, which is typically
  // temporary, can optionally have an alias instruction, when the alias
  // instruction currently have its allocation in a register(spill) which is the
  // same as the forced register, then we just do nothing
  HInstruction* alias_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(LiveInterval);
};

// ----------------------------------------------------------------------------
// *THE* linear scan register allocator
class RegisterAllocator {
 public:
  static constexpr std::size_t kRegCount =
      kGPUsrRegisterCount + kFPUsrRegisterCount;  // allocatable register

 public:
  RegisterAllocator(Quake* q, HAssembler* as);

  // --------------------------------------------------------------------------
  // Run the RA phase
  void Run();

  zone::Zone* zone() const { return quake_->zone(); }

 private:
  // --------------------------------------------------------------------------
  // Phase (0)
  void Initialize();

 private:
  // --------------------------------------------------------------------------
  // Phase (1)
  // Linearization phase, this is the simplest phase since the AS records the
  // PHI node separately and we can mark them at very last when everything is
  // done
  void Linearize();
  void DoLinearize();

 private:
  // --------------------------------------------------------------------------
  // Phase (2)
  // Liveness analysis, nothing special
  void LivenessAnalyse();
  LiveInterval* AllocateLiveInterval(HInstruction*);
  void MarkInputLiveness(HInstruction*, HOperand*);

 private:
  // --------------------------------------------------------------------------
  // Phase (3)
  // During this phase, all the Temporary HOperand will be replaced with
  // correct spilled slot --> MemStack or register --> const Register* in its
  // usage. And LIR will not maintain any SSA style thing, ie HOperand will
  // not become unique anymore.
  void AllocateRegister();
  void LinearScan();
  void DoLinearScan(LiveInterval*);
  void RegisterShuffleMoveTo(LiveInterval*, HInstruction*,
                             const RegisterAllocation&,
                             const RegisterAllocation&, HInstruction*);

  // phi elimination operations
  void LoopPhiShuffle();
  void LoopPhiShuffleBB0();
  void LoopPhiShuffleBB1();

  void BranchPhiElimination(HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // Phase (4)
  // Materialization/Rewrite
  void MaterializeOperandTemporary(int, HInstruction*, HOperand*);
  void MaterializeNativeCallOperand(HInstruction*, int, HOperand*, HOperand*,
                                    const RegisterAllocation&, LiveInterval*);
  void MaterializeOutputTemporary(HInstruction*, Temporary*);
  void MaterializeOperand(HInstruction*);
  void MaterializeExit(HInstruction*);
  void Materialize(HInstruction*);
  void GenerateMove(HInstruction*, LiveInterval*);
  void Rewrite();

  // Map a liveness id back to its corresponding instruction
  HInstruction* LivenessMap(std::uint32_t livenessid) const {
    if (livenessid >= static_cast<std::uint32_t>(liveness_map_.size()))
      return nullptr;
    return liveness_map_[livenessid];
  }
  HInstruction* LivenessMap(const LivenessPoint& p) const {
    return LivenessMap(p.point());
  }

 private:
  void TraceLiveness();

 private:
  // --------------------------------------------------------------------------
  // Internal data structure used by tracking active/inactive list. Notes the
  // active list should be ordered by its ending in ASC order
  struct RegisterRecord {
    const Register* r;
    LiveInterval* allocated;
    LivenessRange range;
  };

  struct RegisterRecordCompEnd {
    bool operator()(const RegisterRecord& l, const RegisterRecord& r) const {
      return l.range.end() < r.range.end();
    }
  };

  struct SpillSlot {
    MemoryStack::Slot slot = MemoryStack::kInvalidSlot;
    bool use = false;
    LivenessRange
        range;  // range of the current spilled slot, valid when use is true

    LiveInterval* allocation =
        nullptr;  // the allocated instruction if it is allocated

    SpillSlot() = default;
    SpillSlot(MemoryStack::Slot s, bool u, const LivenessRange& r,
              LiveInterval* l)
        : slot(s), use(u), range(r), allocation(l) {}

    void Remove() {
      use = false;
      allocation = nullptr;
    }
  };

  std::uint32_t LastLivenessId() const { return liveness_id_ - 2; }

 private:
  Quake* quake_;
  HAssembler* as_;

  std::size_t instruction_size_;
  std::uint32_t liveness_id_;  // maxium liveness id, set up after LivenessCall

  std::vector<HInstruction*> liveness_map_;
  std::deque<LiveInterval*> rr_list_;

 private:
  MemoryStack::Slot GetSpillSlot(const LivenessRange&, LiveInterval*);
  void FreeSpillSlot(LiveInterval*, MemoryStack::Slot);

  void ExpireSpill(LiveInterval*);
  void Expire(LiveInterval*);
  void RematerializeOperand(LiveInterval*);
  void AllocateToTempRegisterWhen(LiveInterval*, LiveInterval*);
  void AllocateToTempRegisterWhen(LiveInterval*, LiveInterval*,
                                  const Register*);

#ifdef QUAKE_TARGET_X64
  void SolveX64BinaryOpConstraint(LiveInterval*);
  void SolveX64ShiftOpConstraint(LiveInterval*);
  void OutputX64OpConstraint(LiveInterval*, int, RegisterSet*);
#endif  // QUAKE_TARGET_X64

  void OnSlowBlockStart();
  void OnSlowBlockEnd(HInstruction*);

 private:
  // Used by RA phase. Maintaining 2 RAStatus, one for GP and the other for FP
  struct RAStatus {
    std::vector<RegisterRecord> active_list;
    std::vector<RegisterRecord> inactive_list;
    RegisterAllocator* ra;

    RAStatus(RegisterAllocator*, const std::vector<RegisterKind>&);

   public:
    bool HasInactive() const { return !inactive_list.empty(); }

   public:
    void LinearScan(LiveInterval*);
    void AllocateOutput(LiveInterval*);
    void SolveOperandConstraint(LiveInterval*);

    void ExpireRegister(LiveInterval*);
    void AllocateOrSpill(LiveInterval*);
    void AllocateSpill(LiveInterval*);
    bool DoAllocateRegister(LiveInterval*);
    bool DoAllocateRegisterSlowBlock(LiveInterval*);
    bool TryAllocateRegister(LiveInterval*);

    void ForceAllocateRegister(LiveInterval*, const Register*);
    void ForceAllocateRegister(LiveInterval*, int);
    void ForceAllocateRegister(LiveInterval*);
    void MustAllocateRegister(LiveInterval*, const Register*,
                              const LivenessPoint&);

    void RematerializeRegister(LiveInterval*, const LivenessPoint&);

    int FindBestCandidate(LiveInterval*, const std::vector<RegisterRecord>&);
    int FindBestInactiveForOperand(LiveInterval*, int, LiveInterval*);
    int FindRegisterByHintPolicy(LiveInterval*,
                                 const std::vector<RegisterRecord>&);
    int FindRegisterByHintSet(LiveInterval*,
                              const std::vector<RegisterRecord>&);

    int Map(const Register*, const std::vector<RegisterRecord>&);
    int MapInactive(const Register*);
    int MapActive(const Register*);

    int TransferInactiveToActive(LiveInterval*, int, const LivenessPoint&);
    int TransferInactiveToActive(LiveInterval*, int);
    int TransferActiveToInactive(LiveInterval*, int);

    int SpillActiveRegister(LiveInterval*, const Register*);
    int SpillActiveRegister(LiveInterval*, int);

    int GetActiveRecordForSpill(LiveInterval*);

    std::string Print() const;

   private:
    HAssembler* as() const { return ra->as_; }
  };

  std::vector<SpillSlot> spill_list_;  // list of spilled slot on stack
  RAStatus ra_general_;
  RAStatus ra_float_;

  // Used to recover register transfer inside of the slow path, will recover
  // when the slow block done
  struct SlowPathRecoveryRA {
    LiveInterval* interval;
    RegisterAllocation from;
    RegisterAllocation to;

    SlowPathRecoveryRA(LiveInterval* a, const RegisterAllocation& b,
                       const RegisterAllocation& c)
        : interval(a), from(b), to(c) {}
  };

  std::vector<SlowPathRecoveryRA> slow_path_recovery_ra_;
  bool slow_path_ = false;

  void RecordSlowPathRecoveryRA(LiveInterval* interval,
                                const RegisterAllocation& from,
                                const RegisterAllocation& to) {
    CHECK(slow_path_);
    CHECK(from.IsSpill());
    CHECK(to.IsRegister());
    slow_path_recovery_ra_.emplace_back(interval, from, to);
  }

 private:
  friend class RAStatus;
  friend class LiveInterval;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REGISTER_ALLOCATOR_H_
