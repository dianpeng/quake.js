#include "lir/register-allocator.h"

namespace quake {
namespace lir {

// ============================================================================
// Phase(0)
void RegisterAllocator::Initialize() {
  // record instruction total size, for now HAssembler will perform a O(N)
  // complexity walk to get the total instruction size
  instruction_size_ = as_->instruction_size();

  // Promote the phi's operand to insert fake immediate number creation
  PromotePhi();

  // Build the phi node's PDG
  BuildPhiPDG();

  // Reserve liveness map size
  liveness_map_.reserve(instruction_size_ + 1 + 1);
}

// ============================================================================
// Phase(1)
// Linearization. Mostly we just rely on HAssembler to give us correct
// orgnization of the LIR graph

// ----------------------------------------------------------------------------
// Notes each PHI node will have 4 operands, ie 2 pair of operands.
// [0] -> v0
// [1] -> v1
// [2] -> label0
// [3] -> label1

void RegisterAllocator::BuildPhiPDG() {
  // ie, just sorting PHI node with its value/data dependency relationship
  // node shows up earlier in the sequences may become dependent of node
  // shows later. simply a dfs number. Notes, this does a simple dataflow
  // algorithm which is kind of sucky
  std::vector<bool> visited;

  std::vector<HInstruction*> pqueue = as_->ToPhiList();
  visited.resize(as_->instruction_maxid(), false);

  auto can_be_removed = [&](HInstruction* ins) -> bool {
    if (visited[ins->id()]) return true;

    auto op0 = ins->Opr(0);
    auto op1 = ins->Opr(1);
    if (!op0->IsPhi() && !op1->IsPhi()) {
      visited[ins->id()] = true;
      phi_pdg_.push_back(ins);
      return true;
    } else {
      if (op0->IsPhi() && !visited[op0->instruction()->id()]) return false;
      if (op1->IsPhi() && !visited[op1->instruction()->id()]) return false;

      visited[ins->id()] = true;
      phi_pdg_.push_back(ins);
      return true;
    }
  };

  while (!pqueue.empty()) {
    auto itr =
        std::remove_if(pqueue.begin(), pqueue.end(), std::move(can_be_removed));

    BASE_DBG({
      auto count = std::distance(itr, pqueue.end());
      CHECK(count > 0);  // cannot be 0
    });

    pqueue.erase(itr, pqueue.end());
  }

  CHECK(phi_pdg_.size() == as_->phi_size());
}

HOperand* RegisterAllocator::PromoteImmediate(ir::Node* origin,
                                              HInstruction* prev,
                                              const Immediate& imm) {
  HInstruction* out = nullptr;
  HOperand* opr = as_->NewImmediate(imm);

  switch (imm.type()) {
    case Immediate::kInt32:
      out = as_->NewInstructionAfter(origin, HOpcode::kMovI32, opr, prev);
      break;

    case Immediate::kInt64:
      // currently we just do not have INT64 representation, immediate::64 is
      // simply a B64. Notes, underlying I64 and B64 are just the same
      out = as_->NewInstructionAfter(origin, HOpcode::kMovB64, opr, prev);
      break;

    case Immediate::kFloat64:
      out = as_->NewInstructionAfter(origin, HOpcode::kMovF64, opr, prev);
      break;

    default:
      UNREACHABLE();
      return nullptr;
  }

  return out->output();
}

void RegisterAllocator::PromotePhi() {
  // All phi are inside of this list, regardlessly of the visiting order it will
  // convergence in just one shot
  as_->ForeachPhi([&](HInstruction* phi) {
    CHECK(phi->IsPhi());

    auto op0 = phi->Opr(0);
    auto op1 = phi->Opr(1);

    if (op0->IsImm()) {
      auto label0 = phi->Opr(2);
      CHECK(label0->IsLabel());
      auto new_operand = PromoteImmediate(
          phi->ir_node(), label0->GetLabel()->bind(), op0->GetImm());

      phi->OprSet(0, new_operand);
    }

    if (op1->IsImm()) {
      auto label1 = phi->Opr(3);
      CHECK(label1->IsLabel());
      auto new_operand = PromoteImmediate(
          phi->ir_node(), label1->GetLabel()->bind(), op1->GetImm());
      phi->OprSet(1, new_operand);
    }
  });

  // Perform a sanity check if needed
  BASE_DBG({
    as_->ForeachPhi([&](HInstruction* phi) {
      CHECK(!phi->Opr(0)->IsImm());
      CHECK(!phi->Opr(1)->IsImm());
    });
  });
}

void RegisterAllocator::DoLinearize() {
  liveness_id_ = 1;
  liveness_map_.push_back(nullptr);  // liveness 0 is not in used

  auto set_liveness_start = [&](HInstruction* ins) {
    // We temporarily set the liveness's endpoint to be same as start point
    // which means the register is been created and no-one is reference it
    // and drop it immediate. This is for future cases that certain instruction
    // does generate output regardlessly but no one is using the output. Like
    // calling a void function, or x64's idiv instruction which always polutes
    // 2 registers.
    auto lid = liveness_id_++;
    ins->liveness_info().SetLivenessStart(lid);
    ins->liveness_info().last_use().set_point(lid);
    liveness_map_.push_back(ins);
    CHECK(liveness_map_.size() == static_cast<std::size_t>(liveness_id_));

    // If the instruction is a Value type, ie generating some output and should
    // be invovled inside of RA, we should generate LiveIntervalList inside of
    // its Temporary to allow RA
    if (ins->IsValue()) {
      auto output = ins->output();
      CHECK(output && output->IsTemporary());
      auto temp = output->GetTemporary();
      CHECK(!temp->HasLiveIntervalList());
      temp->set_live_interval_list(AllocateLiveIntervalList(ins, output));
    }
  };

  // Mark BB0's node's liveness id, ie liveness start
  as_->ForeachInstructionBB0([&](HInstruction* ins) {
    CHECK(!ins->IsPhi());
    // Skip phi node liveness id assignment until the very last
    set_liveness_start(ins);
  });

  // Then followed by liveness id of PHI. PHI node are just placed right after
  // the loop condition testing of first iteration
  as_->ForeachPhi([&](HInstruction* ins) {
    CHECK(ins->IsPhi());
    set_liveness_start(ins);
  });

  // Lastly, all the BB1 instruction will the be marked incrementally
  as_->ForeachInstructionBB1([&](HInstruction* ins) {
    CHECK(!ins->IsPhi());
    // Skip phi node liveness id assignment until the very last
    set_liveness_start(ins);
  });
}

void RegisterAllocator::Linearize() {
  DoLinearize();

  // Sanity check, now every instruction that is inside of the node should have
  // all the liveness id setup correctly
  BASE_DBG({
    as_->ForeachInstruction(
        [](HInstruction* ins) { CHECK(ins->liveness_info().HasLivenessId()); });
  });
}

// ============================================================================
// Phase(2)
//
// Liveness analysis, it starts to iterate the instruction from back until we
// hit the top.
//
// One thing we should make sure is when we hit a DeoptXXX instruction to make
// sure that each deoptimization point's referenced node is been properly
// touched
void RegisterAllocator::MarkLiveness(HInstruction* ins, HOperand* opr) {
  if (auto ref_ins = opr->instruction(); ref_ins) {
    CHECK(ins->liveness_info().HasLivenessId());
    auto lid = ins->liveness_info().liveness_id();

    // enqueue the liveness id into use_list
    ref_ins->liveness_info().AddUseBack(LivenessPoint{lid});
  }
}

void RegisterAllocator::LivenessAnalyse() {
  // PHI instruction is not inside of the instruction list but stored separately
  // so just foreach phi instruction
  as_->ForeachPhi([&](HInstruction* ins) {
    // The phi instruction MUST cover the whole loop SCC, we can just set it to
    // be largest used liveness_id_
    ins->liveness_info().last_use().set_point(liveness_id_ - 1);
  });

  // Iterate all the instructions backwards to setup each instruction(none-phi)
  // live range and reference information
  as_->ForeachInstructionExcludePhiBackward([&](HInstruction* ins) {
    CHECK(!ins->IsPhi());

    // If it is a deoptXXX instruction, visit all its referenced node since it
    // is implicitly referenced here via external deoptimization function
    if (auto checkpoint = ins->GetCheckpoint(); checkpoint) {
      for (const auto& e : checkpoint->entry_vector()) {
        auto opr = e->value->lir_operand();
        CHECK(opr != nullptr);
        MarkLiveness(ins, opr);
      }
    }

    for (auto i = 0; i < ins->opr_size(); i++) {
      auto opr = ins->Opr(i);
      MarkLiveness(ins, opr);
    }
  });

  // sanity check
  BASE_DBG({
    as_->ForeachInstruction(
        [](HInstruction* ins) { CHECK(ins->liveness_info().IsValid()); });
  });
}

// the live interval list is used for RA to
LiveIntervalList* RegisterAllocator::AllocateLiveIntervalList(HInstruction* ins,
                                                              HOperand* opr) {
  CHECK(ins->liveness_info().range().IsValid());
  auto list =
      zone()->New<LiveIntervalList>(zone(), ins, ins->liveness_info().range());

  list->SetUp();
  rr_list_.emplace_back(list);
  return list;
}

// ============================================================================
//
// Phase(3)
//
// Register allocation
//
// Our tracing output of LIR graph is essentially as following
//
// [ BB #0 ] <== iteration peeled
// [ BB #1 ] <== loop body
//
// Basically we just have 2 BB, first BB is the first iteration peeled by the
// ir::Builder, and the second BB is the loop body. The first and second BB
// are splitted by a Loop instruction. The loop body is essentially just the
// BB #1.
//
// The PHI's liveness range will cover the whole BB #1.
//
// The basic algorithm for allocation of register is as following
//
// 1) It tries to allocate register inside of BB #0, which does not have PHI
//
// 2) It tries to allocate regsiter inside of BB #1, which has PHI. And we
//    allocate the PHI right at the top of the BB #1. Notes this behavior is
//    totally fine since PHI is been pinned at the top of the PHI and it has
//    a liveness start point at the top of the BB #1. Then each other ins
//    starts to kick in.
//
// 2.1) Notes, during the loop body generation, PHI's allocation can be spilled
//      in lots of position, and this avoid loop carry dependency at the end
//      of the loop body. Example as following
//
//      1) PHI <--- R#1
//      2) PHI <--- R#2
//      3) PHI <--- S#1
//      4) <=========== loop done
//
//      First iteration PHI will be transferred in allocation chain as
//
//      R#1 -> R#2 -> S#1
//
//      At the end of the loop the PHI's placement becomes S#1. But when
//      loop jumps back to the start, the PHI's liveness point related
//      allocation is R#1, which breaks the second loop of iteration.
//
//      Therefore, we should restore PHI's original location at the end
//      of the loop chain. Notes, any other variable which does not become
//      a PHI, ie loop induction variable, will not have loop carry effect
//      and their allocation should already be dead. Loop invariant's
//      liveness range is also cover correctly, since they are immutable
//      during loop body, their liveness range ends at last reference inside
//      of the loop body.
//
// 3) Now all the node's output operand is allocated. Except PHI's is not
//    eliminated, so we do a separate pass to eliminate PHI. For each phi's
//    predecessor BB, ie BB#0 and BB#1, we place register shuffle right at
//    the end of each BB. Notes, for BB#0, since we reserve allocation for
//    PHI node at very start of BB#1, it means output allocation for PHI is
//    not been bound to the PHI during end of BB#0 and start of BB#1. It
//    is safe to use this allocation in between. We can move first operand
//    into its output and place operation in that range. Additionally, we
//    do the same thing, since at the end of BB#1 til the top of BB#1, output
//    allocation of PHI is safe as well, notes PHI's output allocation is safe
//    throughout the whole loop body.
//
// Notes, we use an external helper object called register shuffle graph to
// help us decide how to perform the move instruction sequences accordingly

// ----------------------------------------------------------------------------
void LiveIntervalList::SetUp() {
  CHECK(instruction_->IsValue());

  // (0) add type and size class
  rinfo_.rsize = instruction_->output_size_class();
  rinfo_.rtype = instruction_->opcode_info().output.output0_rtype();

  // (1) check hopcode's hint, if it has mandatory hint, then we start to
  //     respect it. Mostly it will not
  instruction_->opcode_info().register_info.Convert(&rinfo_.hint,
                                                    &rinfo_.force);

  // (2) add other hint, ie ABI hint
  if (auto narg = instruction_->RefByNCall(); narg != -1) {
    // setup ABI calling register usage
    if (const auto r = Register::CallParamMapRegister(narg); r) {
      rinfo_.hint.Set(r);
    }
  }
}

void LiveIntervalList::AllocateRegister(const Register* r, SizeClass sc,
                                        const LivenessPoint& start) {
  BASE_DBG({
    if (!allocate_.empty()) {
      CHECK(allocate_.back().IsSpill());
    }
  });

  auto range = LivenessRange{start, range_.end()};
  allocate_.push_back(RegisterAllocation{r, sc, range});
}

void LiveIntervalList::AllocateSpill(MemoryStack::Slot slot,
                                     const LivenessPoint& start) {
  BASE_DBG({
    if (!allocate_.empty()) {
      CHECK(allocate_.back().IsRegister());  // notes MUST be a r=>s, we do not
                                             // support R=R for now
    }
  });

  auto range = LivenessRange{start, range_.end()};
  allocate_.push_back(RegisterAllocation{slot, range});
}

// ---------------------------------------------------------------------------
// Linear scan allocation. This covers the 1) and 2) parts of the work. It
// does a interval constraint solving until we reach a met condition. Notes
// ABI call is handled as on demand pops/push register. It is safe to use
// generate it at any point without needing to bother RA.
void RegisterAllocator::LinearScan() {
  struct Comp {
    bool operator()(const LiveIntervalList* l, const LiveIntervalList* r) {
      return l->range().start() < r->range().start();
    }
  };
  std::sort(rr_list_.begin(), rr_list_.end(), Comp{});

  for (auto interval : rr_list_) {
    const auto& rinfo = interval->GetRegisterInfo();
    if (rinfo.rtype == RegisterType::kGeneral) {
      ra_general_.LinearScan(interval);
    } else {
      ra_float_.LinearScan(interval);
    }
  }
}

// ---------------------------------------------------------------------------
// RegisterAllocator::RAStatus implementation

void RegisterAllocator::RAStatus::LinearScan(LiveIntervalList* interval) {
  ExpireRegister(interval);
  ResolveReferenceConstraints(interval);
  AllocateOrSpill(interval);
}

void RegisterAllocator::RAStatus::ExpireRegister(LiveIntervalList* l) {
  auto itr = std::remove_if(
      active_list.begin(), active_list.end(), [&](const auto& r) -> bool {
        return r.allocated->ExpireWhen(l->range().start());
      });

  active_list.erase(itr, active_list.end());
}

void RegisterAllocator::RAStatus::ResolveReferenceConstraints(
    LiveIntervalList*) {}

void RegisterAllocator::RAStatus::AllocateOrSpill(LiveIntervalList* l) {
  if (!DoAllocateRegister(l)) {
    AllocateSpill(l);
  }
}

void RegisterAllocator::RAStatus::AllocateSpill(LiveIntervalList* l) {
  // try to find the properly spilled candidate
  if (auto active_idx = GetActiveRecordForSpill(l); active_idx >= 0) {
    const auto& active_record = active_list[active_idx];

    active_record.allocated->AllocateSpill(GetSpillSlot(l), l->range().start());

    // Free the register from active to inactive
    auto inactive_idx = TransferActiveToInactive(l, active_idx);

    // Allocate the active register back to us here
    ForceAllocateRegister(l, inactive_idx);
  } else {
    // Should just spill on the stack directly
    auto spill_slot = GetSpillSlot(l);
    l->AllocateSpill(spill_slot, l->range().start());
  }
}

bool RegisterAllocator::RAStatus::DoAllocateRegister(LiveIntervalList* l) {
  if (const auto& rinfo = l->GetRegisterInfo(); !rinfo.force) {
    return TryAllocateRegister(l);
  } else {
    ForceAllocateRegister(l, rinfo.force);
    return true;
  }
}

// Try to allocate a register inside of the free register list and optionally
// does register hint constraint solving.
bool RegisterAllocator::RAStatus::TryAllocateRegister(LiveIntervalList* l) {
  int candidate = -1;

  // (0) filter out the possible candidate, take advantage of the hint
  if (!inactive_list.empty()) {
    const auto& h = l->GetRegisterInfo().hint;
    candidate = 0;

    // yeah, it is just a hint :)
    if (!h.Empty()) {
      for (std::size_t i = 0; i < inactive_list.size(); i++) {
        if (auto& rr = inactive_list[i]; h.Get(rr.r)) {
          candidate = static_cast<int>(i);
        } else {
          break;
        }
      }
    }
  }

  // (1) Check whether we find one candidate or not. If so, transfer the list
  if (candidate != -1) {
    TransferInactiveToActive(l, candidate);
    return true;
  }
  return false;
}

// Force to allocate register inside of the register set. And spill the occupied
// slot if needed. Notes, the register set contains a list of OR semantic reg.
void RegisterAllocator::RAStatus::ForceAllocateRegister(
    LiveIntervalList* l, const Register* force_r) {
  int idx;
  if (idx = MapInactive(force_r); idx == -1) {
    idx = SpillActiveRegister(l, force_r);
  }
  ForceAllocateRegister(l, idx);
}

void RegisterAllocator::RAStatus::ForceAllocateRegister(LiveIntervalList* l,
                                                        int idx) {
  TransferInactiveToActive(l, idx);
}

int RegisterAllocator::RAStatus::Map(const Register* r,
                                     const std::vector<RegisterRecord>& rl) {
  auto len = static_cast<int>(rl.size());
  for (int i = 0; i < len; i++) {
    if (rl[i].r == r) {
      return i;
    }
  }
  return -1;
}

int RegisterAllocator::RAStatus::MapInactive(const Register* r) {
  return Map(r, inactive_list);
}

int RegisterAllocator::RAStatus::MapActive(const Register* r) {
  return Map(r, active_list);
}

int RegisterAllocator::RAStatus::TransferInactiveToActive(LiveIntervalList* l,
                                                          int idx) {
  CHECK(idx >= 0 && idx < static_cast<int>(inactive_list.size()));
  auto reg = inactive_list[idx].r;
  l->AllocateRegister(reg, l->GetRegisterInfo().rsize, l->range().start());

  base::EraseIndex(&inactive_list, static_cast<std::size_t>(idx));

  // insert the active record into the list in correct order
  auto active_record = RegisterRecord{reg, l, l->range()};

  auto itr = std::upper_bound(active_list.begin(), active_list.end(),
                              active_record, RegisterRecordCompEnd{});

  active_list.insert(itr, active_record);

  auto active_itr = std::find_if(
      active_list.begin(), active_list.end(),
      [&](const RegisterRecord& rr) -> bool { return rr.r == reg; });

  return static_cast<int>(std::distance(active_list.begin(), active_itr));
}

int RegisterAllocator::RAStatus::TransferActiveToInactive(LiveIntervalList* l,
                                                          int idx) {
  CHECK(idx >= 0 && idx < static_cast<int>(active_list.size()));
  auto reg = active_list[idx].r;
  auto inactive_record = RegisterRecord{
      reg, nullptr, LivenessRange::StartUntilLast(l->range().start())};

  // Notes, after removing, the sorted vector is also in order
  base::EraseIndex(&active_list, static_cast<std::size_t>(idx));

  inactive_list.push_back(inactive_record);
  return static_cast<int>(inactive_list.size() - 1);
}

int RegisterAllocator::RAStatus::SpillActiveRegister(LiveIntervalList* l,
                                                     const Register* r) {
  auto idx = MapActive(r);
  CHECK(idx >= 0);

  auto allocated = active_list[idx].allocated;
  auto spill_slot = GetSpillSlot(l);

  // now let the interval spill at the current position
  allocated->AllocateSpill(spill_slot, l->range().start());

  // now this register is free and then transfer it back to the inactive_list
  return TransferActiveToInactive(l, idx);
}

// Get spill slot. Notes spill slot is baised against the RBP, ie the starting
// point of the RSP. Notes, this function will find an spill slot, if not it
// will just create one
MemoryStack::Slot RegisterAllocator::RAStatus::GetSpillSlot(
    LiveIntervalList* l) {
  for (auto& ss : spill_list) {
    if (!ss.use) {
      ss.use = true;
      return ss.slot;
    }
  }
  auto spill_slot = static_cast<MemoryStack::Slot>(spill_list.size());
  spill_list.push_back(SpillSlot{spill_slot, true});
  return spill_slot;
}

std::optional<MemoryStack::Slot> RegisterAllocator::RAStatus::FindSpillSlot(
    LiveIntervalList* l) {
  for (auto& ss : spill_list) {
    if (!ss.use) {
      ss.use = true;
      return ss.slot;
    }
  }
  return {};
}

int RegisterAllocator::RAStatus::GetActiveRecordForSpill(LiveIntervalList* l) {
  // trying to find a possible active record, ie a register that has been alloc
  // for spilling out. The eviction algorithm tries to evict item that has
  // the latest next reference. And if the latest next reference is L itself,
  // then we do not evict it but returns -1 to indicate we shuold not spill.
  struct RefPoint {
    LivenessPoint ref_point;
    int active_index;
    RefPoint(const LivenessPoint& rp, int i) : ref_point(rp), active_index(i) {}
  };

  struct RefPointComp {
    bool operator()(const RefPoint& l, const RefPoint& r) const {
      return l.ref_point < r.ref_point;
    }
  };

  std::vector<RefPoint> ref_point_list;

  if (auto next_use =
          l->instruction()->liveness_info().NextUse(l->range().start());
      next_use) {
    ref_point_list.emplace_back(
        *next_use,
        -1 /* indicate no eviction happened, ie pointed current interval */
    );
  } else {
    UNREACHABLE() << "the record is been expired but still in active list";
  }

  for (std::size_t i = 0; i < active_list.size(); i++) {
    const auto& entry = active_list[i];
    CHECK(entry.allocated);
    auto ins = entry.allocated->instruction();
    const auto& linfo = ins->liveness_info();

    auto next_use = linfo.NextUse(l->range().start());
    CHECK(next_use) << "the record is been expired but still in active list";

    ref_point_list.emplace_back(*next_use, i);
  }

  // sort the ref_point_list in descending order to decide what to evict
  std::sort(ref_point_list.begin(), ref_point_list.end(), RefPointComp{});

  // return back the active index needs to be evicted
  return ref_point_list.back().active_index;
}

namespace {

LiveIntervalList* GetInterval(HOperand* opr) {
  CHECK(opr->IsTemporary());
  auto temp = opr->GetTemporary();
  CHECK(temp->HasLiveIntervalList());
  return temp->live_interval_list();
}

LiveIntervalList* GetInterval(HInstruction* ins) {
  auto out = ins->output();
  CHECK(out);
  return GetInterval(out);
}

}  // namespace

// ----------------------------------------------------------------------------
// Register shuffle will not mutate any live interval list. All the shuffle
// operation essentially happen at the end of the list so no harm
void RegisterAllocator::RegisterShuffleMoveTo(HInstruction* origin,
                                              const RegisterAllocation& to,
                                              const RegisterAllocation& from,
                                              HInstruction* insert_before) {
  if (to.SameAllocation(from)) {
    return;
  }

  as_->NewInstructionBefore(origin->ir_node(), HOpcode::kMov, to.ToOperand(as_),
                            from.ToOperand(as_), insert_before);
}

// ----------------------------------------------------------------------------
// This handles 2.1) situation to make sure loop carry dependency is honored
// correctly, ie by restoring register location point at the top of the loop

void RegisterAllocator::PhiShuffle() {
  // Try to insert more instruction at the endpoint of the LIR graph
  for (auto phi : phi_pdg_) {
    CHECK(phi->IsPhi());
    auto live_interval_list = GetInterval(phi);
    CHECK(live_interval_list->HasAllocation());

    const auto& first_allocate = live_interval_list->FirstAllocation();

    if (!live_interval_list->MatchFirstAllocation()) {
      // restore the last one to the first allocation's register
      const auto& last_allocate = live_interval_list->LastAllocation();

      RegisterShuffleMoveTo(
          phi,                        /* original instruction */
          first_allocate,             /* to allocation */
          last_allocate,              /* from allocation */
          as_->bb1_register_shuffle() /* bb1's register shuffle position */
      );
    }

    // Now, let's eliminate all the phi node here
    RegisterShuffleMoveTo(phi, GetInterval(phi->Opr(0))->LastAllocation(),
                          first_allocate, as_->bb0_register_shuffle());

    RegisterShuffleMoveTo(phi, GetInterval(phi->Opr(1))->LastAllocation(),
                          first_allocate, as_->bb0_register_shuffle());
  }
}

void RegisterAllocator::AllocateRegister() {
  LinearScan();
  PhiShuffle();
}

// ============================================================================
// Phase(4)
//
// Rewrite all the LIR graph's operand to its corresponding allocated slot
void RegisterAllocator::MaterializeOperandTemporary(std::size_t i,
                                                    HInstruction* ins,
                                                    Temporary* temp) {
  CHECK(temp->HasLiveIntervalList());

  auto list = temp->live_interval_list();
  const auto& allocation =
      list->AllocationAtOrAfter(ins->liveness_info().range().start());
  auto new_operand = allocation.ToOperand(as_);
  CHECK(!new_operand->IsTemporary());
  ins->OprSet(i, new_operand);
}

void RegisterAllocator::MaterializeOperand(HInstruction* ins) {
  // operand materialization,
  for (auto i = 0; i < ins->opr_size(); i++) {
    if (auto opr = ins->Opr(i); opr->IsTemporary()) {
      MaterializeOperandTemporary(i, ins, opr->GetTemporary());
    }
  }
}

void RegisterAllocator::MaterializeOutputTemporary(HInstruction* ins,
                                                   Temporary* temp) {
  CHECK(temp->HasLiveIntervalList());
  auto list = temp->live_interval_list();
  const auto& allocation =
      list->AllocationAtOrAfter(ins->liveness_info().range().start());
  auto new_operand = allocation.ToOperand(as_);
  CHECK(!new_operand->IsTemporary());
  ins->set_output(new_operand);
}

void RegisterAllocator::Materialize(HInstruction* ins) {
  MaterializeOperand(ins);
  if (auto output = ins->output(); output && output->IsTemporary()) {
    MaterializeOutputTemporary(ins, output->GetTemporary());
  }
}

void RegisterAllocator::GenerateMove(HInstruction* ins) {
  if (auto output = ins->output(); output && output->IsTemporary()) {
    auto live_interval_list = GetInterval(ins);
    // Notes the first live interval contains the very first output generated

    // requires register transfer
    std::size_t previous = 0;
    live_interval_list->ForeachRestAllocation(
        [&](const RegisterAllocation& allocation) {
          auto position = LivenessMap(allocation.range.start());
          const auto& prev_allocation =
              live_interval_list->AllocationIndex(previous);

          CHECK(position);
          as_->NewInstructionBefore(ins->ir_node(), HOpcode::kMov,
                                    allocation.ToOperand(as_),
                                    prev_allocation.ToOperand(as_), position);
          previous++;
        });
  }
}

void RegisterAllocator::Rewrite() {
  as_->ForeachInstruction([&](HInstruction* ins) {
    Materialize(ins);
    GenerateMove(ins);
  });
}

// ============================================================================
// Entry
void RegisterAllocator::Run() {
  Initialize();
  Linearize();
  LivenessAnalyse();
  LinearScan();
  Rewrite();
}

}  // namespace lir
}  // namespace quake
