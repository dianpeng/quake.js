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

// Notes, this represents an allocation of RA, it can be a register or a spilled
// slot
struct RegisterAllocation {
  const Register* r;       // an allocated register
  SizeClass size_class;    // size class of RA
  MemoryStack::Slot slot;  // a spilled slot
  LivenessRange range;     // the range that the register allocation happens or
                           // be responsible for

 public:
  RegisterAllocation(const Register* reg, SizeClass sc, const LivenessRange& r)
      : r(reg), size_class(sc), slot(MemoryStack::kInvalidSlot), range(r) {}

  RegisterAllocation(MemoryStack::Slot s, const LivenessRange& r)
      : r(nullptr), slot(s), range(r) {}

  RegisterAllocation() = default;

  bool IsRegister() const { return r != nullptr; }
  bool IsSpill() const { return r == nullptr; }

 public:
  // Materialize the RegisterAllocation result into a real operand. After the
  // conversion the use-def high level information will be lost. The returned
  // operand is guaranteed to be materialized
  HOperand* ToOperand(HAssembler*) const;
  bool SameAllocation(const RegisterAllocation&) const;
};

// A list of live interval. Each live interval is DISJOINT regardlessly, ie
// no overlapping with each other and must be transferred into other live
// interval or dead node via MOVE/PUSH operation
class LiveIntervalList {
 public:
  // General live interval list
  LiveIntervalList(zone::Zone* zone, const HInstruction* ins,
                   const LivenessRange& range)
      : instruction_(ins), range_(range), allocate_(zone), register_info_() {}

  // Live interval list corresponding to a reserved register
  LiveIntervalList(zone::Zone* zone, const HInstruction* ins,
                   const LivenessPoint& start, const Register* forced_register)
      : instruction_(ins),
        range_(LivenessRange::OneInstruction(start)),
        allocate_(zone),
        register_info_() {
    // Set the register info have the forced one, which will be repsected by
    // the RA
    register_info_.force = forced_register;
  }

  const HInstruction* instruction() const { return instruction_; }
  const LivenessRange& range() const { return range_; }

  void SetUpOutput();
  void SetUpReserve();

  // Check whether current allocated interval is expired at specified point.
  bool ExpireWhen(const LivenessPoint&) const;

 public:
  void AllocateRegister(const Register*, SizeClass, const LivenessPoint&);
  void AllocateSpill(MemoryStack::Slot, const LivenessPoint&);
  bool HasAllocation() const { return !allocate_.empty(); }

  bool MatchFirstAllocation() const;
  const RegisterAllocation& FirstAllocation() const {
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
  const HInstruction* instruction_;
  LivenessRange range_;
  zone::Vector<RegisterAllocation> allocate_;
  RegisterInfo register_info_;
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
  void BuildPhiPDG();

  // This pass tries to rewrite any immediate number reference node inside of
  // a phi node into a real operation that moves the immediate number into a
  // loc This is needed because we cannot resolve PHI node if none of its
  // operand is inside of register. Notes, we promote both of its operand to
  // be MOVEs if needed
  void PromotePhi();
  HOperand* PromoteImmediate(ir::Node*, HInstruction*, const Immediate&);

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
  void RegisterShuffleMoveTo(HInstruction*, const RegisterAllocation&,
                             const RegisterAllocation&, HInstruction*);
  void PhiShuffle();

 private:
  // --------------------------------------------------------------------------
  // Phase (4)
  // Materialization/Rewrite
  void MaterializeOperandTemporary(std::size_t, HInstruction*, Temporary*);
  void MaterializeOutputTemporary(HInstruction*, Temporary*);
  void MaterializeOperand(HInstruction*);
  void Materialize(HInstruction*);
  void GenerateMove(HInstruction*);
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

    SpillSlot() = default;
    SpillSlot(MemoryStack::Slot s, bool u) : slot(s), use(u) {}
  };

  std::uint32_t LastLivenessId() const { return liveness_id_ - 2; }

 private:
  Quake* quake_;
  HAssembler* as_;

  std::size_t instruction_size_;
  std::uint32_t liveness_id_;  // maxium liveness id, set up after LivenessCall

  std::vector<HInstruction*> liveness_map_;
  std::vector<HInstruction*> phi_pdg_;
  std::deque<LiveIntervalList*> rr_list_;

  // Used by RA phase. Maintaining 2 RAStatus, one for GP and the other for FP
  struct RAStatus {
    std::vector<RegisterRecord> active_list;
    std::vector<RegisterRecord> inactive_list;
    std::vector<SpillSlot>& spill_list;

    RAStatus(std::vector<SpillSlot>&, const std::vector<RegisterKind>&);

   public:
    void LinearScan(LiveIntervalList*);
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

    MemoryStack::Slot GetSpillSlot(LiveIntervalList*);
    std::optional<MemoryStack::Slot> FindSpillSlot(LiveIntervalList*);
    int GetActiveRecordForSpill(LiveIntervalList*);
  };

  std::vector<SpillSlot> spill_list_;  // list of spilled slot on stack
  RAStatus ra_general_;
  RAStatus ra_float_;
  friend class RAStatus;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REGISTER_ALLOCATOR_H_
