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

namespace quake {
namespace ir {
class Node;
}  // namespace ir

namespace lir {

class LiveIntervalList;
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

  bool IsRegister() const { return r != nullptr; }
  bool IsSpill() const { return r == nullptr; }

 public:
  // Materialize the RegisterAllocation result into a real operand. After the
  // conversion the use-def high level information will be lost. The returned
  // operand is guaranteed to be materialized
  HOperand* ToOperand(HAssembler*, LiveIntervalList*) const;
  bool SameAllocation(const RegisterAllocation&) const;
};

// A list of live interval. Each live interval is DISJOINT regardlessly, ie
// no overlapping with each other and must be transferred into other live
// interval or dead node via MOVE/PUSH operation
class LiveIntervalList {
 public:
  enum {
    kMain,     // main interval, ie related to an instruction's output
    kReserve,  // reserve interval, ie related to an instruction's reseved
               // params
  };

  // General live interval list
  LiveIntervalList(zone::Zone* zone, HInstruction* ins,
                   const LivenessRange& range)
      : instruction_(ins),
        range_(range),
        allocate_(zone),
        register_info_(),
        type_(kMain) {}

  // Live interval list corresponding to a reserved register
  LiveIntervalList(zone::Zone* zone, HInstruction* ins,
                   const LivenessPoint& start, const Register* forced_register)
      : instruction_(ins),
        range_(LivenessRange::OneTimestamp(start)),
        allocate_(zone),
        register_info_(),
        type_(kReserve) {
    // Set the register info have the forced one, which will be repsected by
    // the RA
    register_info_.force = forced_register;
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
  const RegisterAllocation& LastAllocation() const { return allocate_.back(); }

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
  bool HasForce() const { return register_info_.force != nullptr; }
  const RegisterInfo& register_info() const { return register_info_; }
  RegisterInfo& register_info() { return register_info_; }

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

  DISALLOW_COPY_AND_ASSIGN(LiveIntervalList);
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
  LiveIntervalList* AllocateLiveIntervalList(HInstruction*, HOperand* opr);
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
  void DoLinearScan(LiveIntervalList*);
  void RegisterShuffleMoveTo(LiveIntervalList*, HInstruction*,
                             const RegisterAllocation&,
                             const RegisterAllocation&, HInstruction*);
  void PhiShuffle();
  void PhiSpill();

 private:
  // --------------------------------------------------------------------------
  // Phase (4)
  // Materialization/Rewrite
  void MaterializeOperandTemporary(int, HInstruction*, HOperand*);
  void MaterializeNativeCallOperand(HInstruction*, int, HOperand*, HOperand*,
                                    const RegisterAllocation&,
                                    LiveIntervalList*);
  void MaterializeOutputTemporary(HInstruction*, Temporary*);
  void MaterializeOperand(HInstruction*);
  void Materialize(HInstruction*);
  void GenerateMove(HInstruction*, LiveIntervalList*);
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
    LiveIntervalList* allocated;
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

    SpillSlot() = default;
    SpillSlot(MemoryStack::Slot s, bool u, const LivenessRange& r)
        : slot(s), use(u), range(r) {}
  };

  std::uint32_t LastLivenessId() const { return liveness_id_ - 2; }

 private:
  Quake* quake_;
  HAssembler* as_;

  std::size_t instruction_size_;
  std::uint32_t liveness_id_;  // maxium liveness id, set up after LivenessCall

  std::vector<HInstruction*> liveness_map_;
  std::deque<LiveIntervalList*> rr_list_;

 private:
  MemoryStack::Slot GetSpillSlot(const LivenessRange&);
  void ExpireSpill(LiveIntervalList*);
  void AllocateToTempRegisterWhen(LiveIntervalList*, LiveIntervalList*);

#ifdef QUAKE_TARGET_X64
  void SolveX64BinaryOpConstraint(LiveIntervalList*);
#endif  // QUAKE_TARGET_X64

 private:
  // Used by RA phase. Maintaining 2 RAStatus, one for GP and the other for FP
  struct RAStatus {
    std::vector<RegisterRecord> active_list;
    std::vector<RegisterRecord> inactive_list;
    RegisterAllocator* ra;

    RAStatus(RegisterAllocator*, const std::vector<RegisterKind>&);

   public:
    void LinearScan(LiveIntervalList*);
    void AllocateOutput(LiveIntervalList*);
    void SolveOperandConstraint(LiveIntervalList*);

    void ExpireRegister(LiveIntervalList*);
    void AllocateOrSpill(LiveIntervalList*);
    void AllocateSpill(LiveIntervalList*);
    bool DoAllocateRegister(LiveIntervalList*);
    bool TryAllocateRegister(LiveIntervalList*);

    void ForceAllocateRegister(LiveIntervalList*, const Register*);
    void ForceAllocateRegister(LiveIntervalList*, int);
    void ForceAllocateRegister(LiveIntervalList*);

    int FindBestCandidate(LiveIntervalList*,
                          const std::vector<RegisterRecord>&);
    int FindInactiveByRegisterHintPolicy(LiveIntervalList*);

    int Map(const Register*, const std::vector<RegisterRecord>&);
    int MapInactive(const Register*);
    int MapActive(const Register*);
    int TransferInactiveToActive(LiveIntervalList*, int);
    int TransferActiveToInactive(LiveIntervalList*, int);
    int SpillActiveRegister(LiveIntervalList*, const Register*);
    int SpillActiveRegister(LiveIntervalList*, int);

    int GetActiveRecordForSpill(LiveIntervalList*);

   private:
    HAssembler* as() const { return ra->as_; }
  };

  std::vector<SpillSlot> spill_list_;  // list of spilled slot on stack
  RAStatus ra_general_;
  RAStatus ra_float_;

 private:
  friend class RAStatus;
  friend class LiveIntervalList;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REGISTER_ALLOCATOR_H_
