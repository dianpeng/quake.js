#ifndef _QUAKE_LIR_REGISTER_ALLOCATOR_H_
#define _QUAKE_LIR_REGISTER_ALLOCATOR_H_
#include "base.h"
#include "common.h"
#include "zone.h"

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

  bool IsRegister() const { return r != nullptr; }
  bool IsSpill() const { return r == nullptr; }

 public:
  // Materialize the RegisterAllocation result into a real operand. After the
  // conversion the use-def high level information will be lost. The returned
  // operand is guaranteed to be materialized
  HOperand* ToOperand(HAssembler*) const;
};

// A list of live interval. Each live interval is DISJOINT regardlessly, ie
// no overlapping with each other and must be transferred into other live
// interval or dead node via MOVE/PUSH operation
class LiveIntervalList {
 public:
  LiveIntervalList(zone::Zone*, HInstruction* ins, const LivenessRange& range)
      : instruction_(ins),
        LivenessRange range_(range),
        allocate_(zone),
        rinfo() {}

  zone::Zone* zone() const { return zone_; }
  HInstruction* instruction() const { return instruction_; }
  const LivenessRange& range() const { return range_; }

  void SetUp();

 public:
  void AllocateRegister(const Register*, SizeClass, const LivenessPoint&);
  void AllocateSpill(MemoryStatic::Slot, const LivenessPoint&);
  bool HasAllocation() const { return !allocate_.empty(); }

  bool MatchFirstAllocation() const;

  const RegisterAllocation& FirstAllocation() const {
    return allocate_.front();
  }
  const RegisterAllocation& LastAllocation() const { return allocate_.back(); }

  template <typename T>
  void ForeachRestAllocation(T&& func) {
    for (std::size_t i = 1; i < allocate_.size(); i++) {
      const auto& x = allocate_[i];
      func(x);
    }
  }

 private:
  HInstruction* instruction_;
  LivenessRange range_;
  zone::Vector<RegisterAllocation> allocate_;
  RegisterInfo rinfo_;  // require setup

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

 private:
  // --------------------------------------------------------------------------
  // Phase (1)
  // Linearization phase, this is the simplest phase since the AS records the
  // PHI node separately and we can mark them at very last when everything is
  // done
  void Linearize();
  void DoLinearize();

  // This pass tries to rewrite any immediate number reference node inside of
  // a phi node into a real operation that moves the immediate number into a
  // loc This is needed because we cannot resolve PHI node if none of its
  // operand is inside of register. Notes, we promote both of its operand to
  // be MOVEs if needed
  void PromotePhi();
  HOperand* PromoteImmediate(ir::Node*, HInstruction*, const Immediate&);

 private:
  // --------------------------------------------------------------------------
  // Phase (2)
  // Liveness analysis, nothing special
  void LivenessAnalyse();
  LivenessIntervalList* AllocateLiveIntervalList(HInstruction*, HOperand* opr);
  void MarkLiveness(HInstruction*, HOperand*);

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
  void MaterializeTemporary(Temporary*);
  void MaterializeOperand(HInstruction*);
  void Materialize(HInstruction*);
  void GenerateMove(HInstruction*);
  void Rewrite();

 private:
  // --------------------------------------------------------------------------
  // Internal data structure used during the RA
  struct RegisterRecord {
    const Register* r;
    LiveIntervalList* allocated;
    LivenessRange range;
    bool operator<(const RegisterRecord& that) const {
      return range.start() < that.range.start();
    }
  };

  struct RegisterRecordCompEnd {
    bool operator()(const RegisterRecordCompStart& l,
                    const RegisterRecordCompStart& r) const {
      return l.current_range().end() < r.current_range().end();
    }
  };

  struct SpillSlot {
    MemoryStack::Slot slot = MemoryStack::kInvalidSlot;
    RegisterType rtype = RegisterType::kGeneral;
    bool use = false;
  };

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

    RAStatus(std::vector<SpillSlot>& sl) : spill_list(sl) {}

   public:
    void LinearScan(LiveIntervalList*);
    void ResolveReferenceConstraints(LiveIntervalList*);
    void ExpireRegister(LiveIntervalList*);
    void AllocateOrSpill(LiveIntervalList*);
    void AllocateSpill(LiveIntervalList*);
    bool DoAllocateRegister(LiveIntervalList*);
    bool TryAllocateRegister(LiveIntervalList*);
    void ForceAllocateRegister(LiveIntervalList*, const Register*);
    void ForceAllocateRegister(LiveIntervalList*, std::size_t);
    void Map(const Register*, const std::vector<RegisterRecord>&);
    int MapInactive(const Register*);
    int MapActive(const Register*);
    std::size_t TransferInactiveToActive(LiveIntervalList*, std::size_t);
    std::size_t TransferActiveToInactive(LiveIntervalList*, std::size_t);
    void SpillActiveRegister(LiveIntervalList*, const Register*);
    MemoryStack::Slot GetSpillSlot(LiveInternvalList*);
    std::optinoal<MemoryStack::Slot> FindSpillSlot(LiveIntervalList*);
    int GetActiveRecordForSpill(LiveIntervalList*);
  };

  std::vector<SpillSlot> spill_list_;  // list of spilled slot on stack

  RAStatus ra_general_;
  RAStatus ra_float_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REGISTER_ALLOCATOR_H_
