#include "lir/register-allocator.h"
#include "lir/snapshot.h"

#include "ir/checkpoint.h"
#include "ir/node.h"
#include "ir/ssa-var.h"

// ----------------------------------------------------------------------------
// The general constraint needs to be resolved by RA and by CG.
//
// The CG part will take care of one important thing, ie that is every hopcode
// will allow to take immediate, memory or register as input, regardlessly.
// This part is handled by the CG internally via scratch register shuffling.
//
// But the output constraint is not entirely resolved by CG, instead it the CG
// will expose the requirement back to the RA. The RA needs to resolve the
// constraint exposed by RegisterInfo object.
//
// Put it this way, all the reference site does not have any requirement for
// the register usage. The output site, ie where the instruction is evaluated
// to output, can have constraints.

namespace quake {
namespace lir {

namespace {

LiveIntervalList* GetInterval(HOperand* opr) {
  CHECK(opr->IsTemporary()) << opr->Print();
  auto temp = opr->GetTemporary();
  CHECK(temp->HasLiveIntervalList());
  return temp->live_interval_list();
}

LiveIntervalList* GetInterval(HInstruction* ins) {
  auto out = ins->output();
  CHECK(out);
  return GetInterval(out);
}

LiveIntervalList* MaybeGetInterval(HOperand* opr) {
  if (opr->IsTemporary()) {
    if (auto temp = opr->GetTemporary(); temp->HasLiveIntervalList()) {
      return temp->live_interval_list();
    }
  }
  return nullptr;
}

void MaybeFlushIntervalRange(HInstruction* ins) {
  if (auto out = ins->output(); out && out->IsTemporary()) {
    auto t = out->GetTemporary();
    t->live_interval_list()->FlushRange();
  }
}

std::uint32_t UseId(HInstruction* ins, HOperand* opr) {
  if (ins->IsPhi()) {
    CHECK(ins->opr_size() == 2);
    if (ins->Opr(0) == opr) {
      return ins->liveness_info().range().start().point();
    } else {
      return ins->liveness_info().range().end().point();
    }
  } else {
    return ins->liveness_info().liveness_id();
  }
}

LivenessPoint RefPoint(HInstruction* ins, HOperand* opr) {
  if (ins->IsPhi()) {
    CHECK(ins->opr_size() == 2);
    if (ins->Opr(0) == opr) {
      return ins->liveness_info().range().start();
    } else {
      return ins->liveness_info().last_use();
    }
  } else {
    return ins->liveness_info().range().start();
  }
}

}  // namespace

// ============================================================================
// Phase(0)
void RegisterAllocator::Initialize() {
  // record instruction total size, for now HAssembler will perform a O(N)
  // complexity walk to get the total instruction size
  instruction_size_ = as_->instruction_size();

  // Reserve liveness map size
  liveness_map_.reserve(instruction_size_ * 2 + 2);
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

// Liveness number is been alloacted as following, the lineraized phase simply
// walk through all the instruction inside of the graph in a pre-defined order
// and assigna linearized id, ie liveness id, for each node. The liveness id
// is been assigned to each point uniquely and increasing by 2.

void RegisterAllocator::DoLinearize() {
  liveness_id_ = kSingleInstructionLivenessStep;  // first slot is not used

  auto add_liveness_map = [&](HInstruction* ins) {
    // Notes(dpeng): If the instruction specified is phi, we should rewrite it
    // as BB1 instruction. The reason is PHI is been placed after the whole
    // body instead of after the BB1 but during RA it is been treated as right
    // after BB1
    if (ins && ins->IsPhi()) ins = as_->bb1();

    // An instruction covers 2 consequtive liveness number [x, x+1]
    liveness_map_.push_back(ins);
    liveness_map_.push_back(ins);
  };

  add_liveness_map(nullptr);

  auto set_liveness_start = [&](HInstruction* ins) {
    // We temporarily set the liveness's endpoint to be same as start point
    // which means the register is been created and no-one is reference it
    // and drop it immediate. This is for future cases that certain instruction
    // does generate output regardlessly but no one is using the output. Like
    // calling a void function, or x64's idiv instruction which always polutes
    // 2 registers.
    auto lid = liveness_id_;
    liveness_id_ += kSingleInstructionLivenessStep;

    ins->liveness_info().SetLivenessStart(lid);
    add_liveness_map(ins);
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
void RegisterAllocator::MarkInputLiveness(HInstruction* ins, HOperand* opr) {
  if (auto ref_ins = opr->instruction(); ref_ins) {
    CHECK(ins->liveness_info().HasLivenessId());

    auto lid = UseId(ins, opr);

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
    ins->liveness_info().end().set_point(LastLivenessId());

    // Iterate through each operand to mark its liveness number
    for (auto i = 0; i < ins->opr_size(); i++) {
      auto opr = ins->Opr(i);
      MarkInputLiveness(ins, opr);
    }

    MaybeFlushIntervalRange(ins);
  });

  // Iterate all the instructions backwards to setup each instruction(none-phi)
  // live range and reference information
  as_->ForeachInstructionExcludePhiBackward([&](HInstruction* ins) {
    CHECK(!ins->IsPhi());

    // If it is a deoptXXX instruction, visit all its referenced node since it
    // is implicitly referenced here via external deoptimization function
    if (ins->HasExit()) {
      auto checkpoint = ins->exit();
      CHECK(checkpoint);
      checkpoint->ForeachEntry([&](const auto& e) {
        if (auto opr = e.value()->lir_operand(); opr) {
          MarkInputLiveness(ins, opr);
        }
      });
    }

    // Iterate through each operand to mark its liveness number
    for (auto i = 0; i < ins->opr_size(); i++) {
      auto opr = ins->Opr(i);
      MarkInputLiveness(ins, opr);
    }

    // Check whether the output is a memory indirect use, if so, treate it as
    // input operand as well
    if (auto out = ins->output(); out && out->IsMemInd()) {
      const auto& mem_ind = out->GetMemInd();
      MarkInputLiveness(ins, mem_ind.base());
      if (mem_ind.offset()) MarkInputLiveness(ins, mem_ind.offset());
    }

    if (!ins->liveness_info().end().IsValid()) {
      ins->liveness_info().SetNoUse();
    }

    MaybeFlushIntervalRange(ins);
  });

  // sanity check
  BASE_DBG({
    as_->ForeachInstruction(
        [](HInstruction* ins) { CHECK(ins->liveness_info().IsValid()); });
  });
}

// The live interval list is used for RA to
LiveIntervalList* RegisterAllocator::AllocateLiveIntervalList(HInstruction* ins,
                                                              HOperand* opr) {
  auto list =
      zone()->New<LiveIntervalList>(zone(), ins, ins->liveness_info().range());

  list->SetUpOutput();

  // Notes, since we may have reserved register list, if so we need to add
  // extra live_interval into the list for sorting purpose
  list->register_info().reserve.ForeachSet([&](std::uint32_t id) {
    auto list = zone()->New<LiveIntervalList>(
        zone(), ins, ins->liveness_info().liveness_id(),
        Register::GetRegisterFromKindId(id));
    list->SetUpReserve();
    rr_list_.emplace_back(list);
  });

  // Lastly push back the list, we gonna use stable_sort to sort rr_list_
  // which makes sure that the instruction itself's output will be
  // generated *after* all the reserved register is done
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

  // NOTES(dpeng): Must use stable_sort since we have duplicated range and
  // their relative order matters, ie reserved register
  std::stable_sort(rr_list_.begin(), rr_list_.end(), Comp{});

  for (auto interval : rr_list_) {
    DoLinearScan(interval);
  }
}

void RegisterAllocator::DoLinearScan(LiveIntervalList* interval) {
  const auto& rinfo = interval->register_info();
  ExpireSpill(interval);

  RematerializeOperand(interval);

  if (rinfo.rtype == RegisterType::kGeneral) {
    ra_general_.LinearScan(interval);
  } else {
    ra_float_.LinearScan(interval);
  }
}

void RegisterAllocator::ExpireSpill(LiveIntervalList* l) {
  auto ref_point = l->range().use_point();
  // just do a linear search, FIXME(dpeng): sort the list ?
  for (auto& spill : spill_list_) {
    if (!spill.use) continue;  // empty slot

    // check whether we have a dead slot, ie needs to be expired
    if (spill.range.end() < ref_point) {
      spill.Remove();
    }
  }
}

void RegisterAllocator::RematerializeOperand(LiveIntervalList* l) {
  // try to rematerialize operand that is spilled and check whether we can
  // move it back to register if needed. We only try to rematerialize the
  // operand back to register when following condition is met
  //
  // 1) It is not a reserve operand
  // 2) It is not part of certain instruction
  //   2.1) call
  //   2.2) phi
  // 3) The operand has at least 2 future use, counting the current use by l
  // 4) Do not spill other range, ie only lifting when it is possible to do so

  if (!l->IsMain()) return;

  if (l->instruction_checked()->IsNativeCall()) return;
  if (l->instruction_checked()->IsPhi()) return;

  // quick return, ie both float/general does not have inactive slot
  if (!ra_general_.HasInactive() && !ra_float_.HasInactive()) return;

  auto ins = l->instruction_checked();
  auto use_point = l->range().use_point();

  for (int i = 0; i < ins->opr_size(); ++i) {
    auto opr = ins->Opr(i);
    if (!opr->HasInstruction()) continue;

    if (opr->instruction_checked()->liveness_info().UseCount(use_point) < 2) {
      continue;
    } else {
      // this operand is the potential candidate for us to hoist it into
      // register

      auto live_interval = GetInterval(opr);

      // Only care about the spilled slot
      if (!live_interval->LastAllocation().IsSpill()) continue;

      auto is_fp = live_interval->register_info().IsFP();
      if (is_fp && ra_float_.HasInactive()) {
        if (auto candidate =
                ra_float_.FindBestInactiveForOperand(live_interval, i, l);
            candidate != -1) {
          ra_float_.TransferInactiveToActive(live_interval, candidate,
                                             use_point);
        }
      } else if (!is_fp && ra_general_.HasInactive()) {
        if (auto candidate =
                ra_general_.FindBestInactiveForOperand(live_interval, i, l);
            candidate != -1) {
          ra_general_.TransferInactiveToActive(live_interval, candidate,
                                               use_point);
        }
      }
    }
  }
}

// ---------------------------------------------------------------------------
// RegisterAllocator::RAStatus implementation

RegisterAllocator::RAStatus::RAStatus(
    RegisterAllocator* r, const std::vector<RegisterKind>& initial_set)
    : active_list(), inactive_list(), ra(r) {
  for (const auto& kind : initial_set) {
    inactive_list.push_back(RegisterRecord{Register::GetRegisterFromKind(kind),
                                           nullptr, LivenessRange::MaxRange()});
  }
}

void RegisterAllocator::RAStatus::LinearScan(LiveIntervalList* l) {
  SolveOperandConstraint(l);
  AllocateOutput(l);
}

void RegisterAllocator::RAStatus::SolveOperandConstraint(LiveIntervalList* l) {
  switch (l->register_info().opr_constraint) {
    case OperandConstraint::kNone:
      return;

#ifdef QUAKE_TARGET_X64
    case OperandConstraint::kX64BinaryOp:
      ra->SolveX64BinaryOpConstraint(l);
      return;

    case OperandConstraint::kX64ShiftOp:
      ra->SolveX64ShiftOpConstraint(l);
      return;
#endif  // QUAKE_TARGET_X64

    default:
      UNREACHABLE();
      return;
  }
}

void RegisterAllocator::RAStatus::AllocateOutput(LiveIntervalList* interval) {
  ExpireRegister(interval);
  AllocateOrSpill(interval);
}

void RegisterAllocator::RAStatus::ExpireRegister(LiveIntervalList* l) {
  auto ref_point = l->range().output_point();
  auto predicate = [&](const auto& r) -> bool {
    return r.allocated->ExpireWhen(ref_point);
  };

  for (auto itr = active_list.begin(); itr != active_list.end();) {
    if (predicate(*itr)) {
      auto inactive_record = RegisterRecord{
          itr->r, nullptr, LivenessRange::StartUntilLast(ref_point)};
      inactive_list.push_back(inactive_record);
      itr = active_list.erase(itr);
    } else {
      break;
    }
  }
}

void RegisterAllocator::RAStatus::AllocateOrSpill(LiveIntervalList* l) {
  if (!DoAllocateRegister(l)) {
    AllocateSpill(l);
  }
}

void RegisterAllocator::RAStatus::AllocateSpill(LiveIntervalList* l) {
  // try to find the properly spilled candidate
  if (auto active_idx = GetActiveRecordForSpill(l); active_idx >= 0) {
    const auto& active_record = active_list[active_idx];

    auto active_range = active_record.allocated;

    active_range->AllocateSpill(ra->GetSpillSlot(l->range(), active_range),
                                l->range().start(), ra);

    // Free the register from active to inactive
    auto inactive_idx = TransferActiveToInactive(l, active_idx);

    // Allocate the active register back to us here
    ForceAllocateRegister(l, inactive_idx);
  } else {
    // Should just spill on the stack directly
    auto spill_slot = ra->GetSpillSlot(l->range(), l);
    l->AllocateSpill(spill_slot, l->range().start(), ra);
  }
}

bool RegisterAllocator::RAStatus::DoAllocateRegister(LiveIntervalList* l) {
  const auto& rinfo = l->register_info();

  switch (rinfo.requirement.type) {
    case RegisterAllocationRequirement::kNone:
      break;

    case RegisterAllocationRequirement::kForceAnyRegister:
      ForceAllocateRegister(l);
      return true;

    case RegisterAllocationRequirement::kForceRegister:
      ForceAllocateRegister(l, rinfo.requirement.force_register);
      return true;

    default:
      UNREACHABLE();
      break;
  }

  return TryAllocateRegister(l);
}

// ----------------------------------------------------------------------------
// Try to find a matched register inside of the target list based on the
// RegsterInfo's given hint. Notes, it respect hint register set and also
// hint register policy. Notes, hint register set is favorred over the
// hint register policy.
int RegisterAllocator::RAStatus::FindBestCandidate(
    LiveIntervalList* l, const std::vector<RegisterRecord>& target_list) {
  if (target_list.empty()) return -1;

  if (auto x = FindRegisterByHintSet(l, target_list); x >= 0) {
    return x;
  }

  if (auto x = FindRegisterByHintPolicy(l, target_list); x >= 0) {
    return x;
  }

  return 0;
}

int RegisterAllocator::RAStatus::FindRegisterByHintSet(
    LiveIntervalList* l, const std::vector<RegisterRecord>& target_list) {
  const auto& rinfo = l->register_info();

  if (!rinfo.hint.Empty()) {
    const int size = static_cast<int>(target_list.size());
    for (int i = 0; i < size; ++i) {
      if (rinfo.hint.Get(target_list[i].r)) {
        return i;
      }
    }
  }
  return -1;
}

int RegisterAllocator::RAStatus::FindBestInactiveForOperand(
    LiveIntervalList* operand, int index, LiveIntervalList* ins) {
  if (!HasInactive()) {
    return -1;
  }

  RegisterSet skip;
  RegisterSet force;

  const auto& rinfo = ins->register_info();
  switch (rinfo.requirement.type) {
    case RegisterAllocationRequirement::kForceRegister:
      skip.Set(rinfo.requirement.force_register);  // do not allocate forced one
      if (inactive_list.size() == 1 && skip.Get(inactive_list[0].r)) return -1;
      break;

    case RegisterAllocationRequirement::kForceAnyRegister:
      if (inactive_list.size() == 1)
        return -1;  // we have one free, leave it to instruction
      break;

    default:
      break;
  }

  // try to find the needed one
  if (auto candidate = FindRegisterByHintSet(operand, inactive_list);
      candidate != -1) {
    if (!skip.Get(inactive_list[candidate].r)) {
      return candidate;
    }
  }

  // take care of opr_constraint field, certain instruction requires operand
  // to be sit inside of certain register
#ifdef QUAKE_TARGET_X64
  ra->OutputX64OpConstraint(ins, index, &force);
#endif  // QUAKE_TARGET_X64

  CHECK(force.IsNoneEmptyAndDisjoint(skip));

  for (int i = 0; i < static_cast<int>(inactive_list.size()); ++i) {
    if (force.Empty() && force.Get(inactive_list[i].r)) return i;
    if (!force.Empty() && !skip.Get(inactive_list[i].r)) return i;
  }

  return -1;
}

// Notes only finding inactive register W.R.T register hint make sense. Notes,
// if it cannot respect any hint policy, then just return -1
int RegisterAllocator::RAStatus::FindRegisterByHintPolicy(
    LiveIntervalList* l, const std::vector<RegisterRecord>& target_list) {
  const auto& rinfo = l->register_info();

  // --------------------------------------------------------------------------
  // Try to respect the hint_policy
  switch (rinfo.hint_policy) {
    case RegisterHintPolicy::kInput0AsOutput: {
      // for x64's instruction like add/sub/mul etc ... it requires the
      // one of its operand to be both output and input(0)
      auto ll = MaybeGetInterval(l->instruction()->Opr(0));
      if (auto r = ll->MaybeRegisterAllocation(l->range().start()); r) {
        // Notes, only when input(0) is a free register then we will allocate
        // it. It means the input(0)'s last reference is at the current
        // instruction so we can just use it as output which save us from one
        // register moving on x64
        return Map(r, target_list);
      }
    } break;

    default:
      break;
  }

  return -1;
}

// Try to allocate a register inside of the free register list and optionally
// does register hint constraint solving.
bool RegisterAllocator::RAStatus::TryAllocateRegister(LiveIntervalList* l) {
  int candidate = -1;

  // (0) filter out the possible candidate, take advantage of the hint
  if (!inactive_list.empty()) {
    candidate = FindBestCandidate(l, inactive_list);
  }

  // (1) Check whether we find one candidate or not. If so, transfer the list
  if (candidate != -1) {
    TransferInactiveToActive(l, candidate);
    return true;
  }
  return false;
}

// Force to allocate register inside of the register set. And spill the
// occupied slot if needed. Notes, the register set contains a list of OR
// semantic reg.
void RegisterAllocator::RAStatus::ForceAllocateRegister(
    LiveIntervalList* l, const Register* force_r) {
  int idx;
  if (idx = MapInactive(force_r); idx == -1) {
    idx = SpillActiveRegister(l, force_r);
  }
  ForceAllocateRegister(l, idx);
}

// Try to find a register to be used by L
void RegisterAllocator::RAStatus::ForceAllocateRegister(LiveIntervalList* l) {
  if (!TryAllocateRegister(l)) {
    CHECK(!active_list.empty());

    // cannot find a register to be used by us, then tries to go through
    // active list and try to find one that needs respect the hint and
    // spill it.
    auto candidate = FindBestCandidate(l, active_list);
    CHECK(candidate >= 0);
    auto inactive_idx = SpillActiveRegister(l, candidate);
    ForceAllocateRegister(l, inactive_idx);
  }
}

void RegisterAllocator::RAStatus::ForceAllocateRegister(LiveIntervalList* l,
                                                        int idx) {
  TransferInactiveToActive(l, idx);
}

void RegisterAllocator::RAStatus::RematerializeRegister(
    LiveIntervalList* l, const LivenessPoint& when) {
  CHECK(!inactive_list.empty());

  auto candidate = FindBestCandidate(l, inactive_list);
  CHECK(candidate >= 0);

  TransferInactiveToActive(l, candidate, when);
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

int RegisterAllocator::RAStatus::TransferInactiveToActive(
    LiveIntervalList* l, int idx, const LivenessPoint& when) {
  CHECK(idx >= 0 && idx < static_cast<int>(inactive_list.size()));

  auto reg = inactive_list[idx].r;
  l->AllocateRegister(reg, l->register_info().rsize, when, ra);

  base::EraseIndex(&inactive_list, static_cast<std::size_t>(idx));

  // insert the active record into the list in correct order
  auto active_record = RegisterRecord{reg, l, l->range().StartBy(when)};

  auto itr = std::upper_bound(active_list.begin(), active_list.end(),
                              active_record, RegisterRecordCompEnd{});

  active_list.insert(itr, active_record);

  auto active_itr = std::find_if(
      active_list.begin(), active_list.end(),
      [&](const RegisterRecord& rr) -> bool { return rr.r == reg; });

  return static_cast<int>(std::distance(active_list.begin(), active_itr));
}

int RegisterAllocator::RAStatus::TransferInactiveToActive(LiveIntervalList* l,
                                                          int idx) {
  return TransferInactiveToActive(l, idx, l->range().start());
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
  return SpillActiveRegister(l, idx);
}

int RegisterAllocator::RAStatus::SpillActiveRegister(LiveIntervalList* l,
                                                     int idx) {
  CHECK(idx >= 0);

  auto allocated = active_list[idx].allocated;
  auto spill_slot = ra->GetSpillSlot(l->range(), allocated);

  // now let the interval spill at the current position
  allocated->AllocateSpill(spill_slot, l->range().start(), ra);

  // now this register is free and then transfer it back to the inactive_list
  return TransferActiveToInactive(l, idx);
}

// Get spill slot. Notes spill slot is baised against the RBP, ie the starting
// point of the RSP. Notes, this function will find an spill slot, if not it
// will just create one
MemoryStack::Slot RegisterAllocator::GetSpillSlot(const LivenessRange& range,
                                                  LiveIntervalList* l) {
  for (auto& ss : spill_list_) {
    if (!ss.use) {
      ss.use = true;
      ss.range = range;
      ss.allocation = l;
      return ss.slot;
    }
  }

  auto spill_slot = static_cast<MemoryStack::Slot>(spill_list_.size());
  spill_list_.push_back(SpillSlot{spill_slot, true, range, l});
  as_->stack_slot()->AddSpill();
  return spill_slot;
}

int RegisterAllocator::RAStatus::GetActiveRecordForSpill(LiveIntervalList* l) {
  static const int kSpillCurrent = -1;

  // trying to find a possible active record, ie a register that has been
  // alloc for spilling out. The eviction algorithm tries to evict item that
  // has the latest next reference. And if the latest next reference is L
  // itself, then we do not evict it but returns -1 to indicate we shuold not
  // spill.
  struct RefPoint {
    LivenessPoint ref_point;
    int active_index;
    HInstruction* instruction;
    RefPoint(const LivenessPoint& rp, int i, HInstruction* ins)
        : ref_point(rp), active_index(i), instruction(ins) {}
  };

  struct RefPointComp {
    bool operator()(const RefPoint& l, const RefPoint& r) const {
      return l.ref_point < r.ref_point;
    }
  };

  auto ins = l->instruction_checked();

  std::vector<RefPoint> ref_point_list;

  if (auto next_use =
          l->instruction_checked()->liveness_info().NextUse(l->range().start());
      next_use) {
    ref_point_list.emplace_back(*next_use, kSpillCurrent, ins);
  } else {
    ref_point_list.emplace_back(l->range().start().Next(), kSpillCurrent, ins);
  }

  for (std::size_t i = 0; i < active_list.size(); i++) {
    const auto& entry = active_list[i];
    CHECK(entry.allocated);
    auto ins = entry.allocated->instruction_checked();
    const auto& linfo = ins->liveness_info();
    auto next_use = linfo.NextUse(l->range().start());
    CHECK(next_use) << "the record has been expired but still in active list";
    ref_point_list.emplace_back(*next_use, i, ins);
  }

  // sort the ref_point_list in descending order to decide what to evict
  std::sort(ref_point_list.begin(), ref_point_list.end(), RefPointComp{});

  if (!ins->IsPhi()) {
    return ref_point_list.back().active_index;
  } else {
    // for PHI node, we cannot spill a slot which is been allocated to another
    // PHI node since we do not have proper PHI spill code yet, ie at the
    // start of the loop, the PHI value must be in its first allocation,
    // regard whether it is a spill one or a register. So if 2 PHI node
    // allocated into same slot because we think we can spill, this will not
    // work.
    for (auto itr = ref_point_list.rbegin(); itr != ref_point_list.rend();
         itr++) {
      if (itr->instruction != ins && itr->instruction->IsPhi()) {
        continue;
      } else {
        return itr->active_index;
      }
    }

    return kSpillCurrent;  // all not work
  }
}

// ----------------------------------------------------------------------------
// Register shuffle will not mutate any live interval list. All the shuffle
// operation essentially happen at the end of the list so no harm
void RegisterAllocator::RegisterShuffleMoveTo(LiveIntervalList* ll,
                                              HInstruction* origin,
                                              const RegisterAllocation& from,
                                              const RegisterAllocation& to,
                                              HInstruction* insert_before) {
  if (to.SameAllocation(from)) {
    return;
  }

  auto mov_opcode =
      ll->register_info().IsFP() ? HOpcode::kShuffleF : HOpcode::kShuffle;

  auto to_operand = to.ToOperand(as_, ll);
  auto mov = as_->NewInstructionBefore(origin->ir_node(), mov_opcode,
                                       from.ToOperand(as_, ll), insert_before);
  mov->SetRAOutput(to_operand);
}

// ----------------------------------------------------------------------------
// This handles 2.1) situation to make sure loop carry dependency is honored
// correctly, ie by restoring register location point at the top of the loop
void RegisterAllocator::PhiSpill() {
  // 1) Get all phi node's output register set
  RegisterSet rset;
  as_->ForeachPhi([&](auto phi) {
    const auto& first_allocate = GetInterval(phi)->FirstAllocation();
    if (first_allocate.IsRegister()) {
      rset.Set(first_allocate.r);
    }
  });

  // 2) If any operand inside of the phi node uses register for phi, then
  //    spill it before the shuffle position
  auto spill_point = as_->loop_back()->liveness_info().range().use_point();
  auto spill_if_needed = [&](HOperand* out) {
    if (out->instruction_checked()->IsPhi()) return;

    if (out->IsTemporary()) {
      auto temp = out->GetTemporary();
      auto ll = temp->live_interval_list();
      const auto& allocate = ll->LastAllocation();

      // The operand is only been referenced by a single PHI node, so we don't
      // need to spill it
      if (out->UseSize() == 1) {
        return;
      }

      if (allocate.IsRegister() && rset.Get(allocate.r)) {
        ll->AllocateSpill(GetSpillSlot(ll->range(), ll), spill_point, this);
      }
    }
  };

  as_->ForeachPhi([&](auto phi) { spill_if_needed(phi->Opr(1)); });
}

void RegisterAllocator::PhiShuffle() {
  // Try to insert more instruction at the endpoint of the LIR graph
  as_->ForeachPhi([&](auto phi) {
    CHECK(phi->IsPhi());
    auto live_interval_list = GetInterval(phi);
    CHECK(live_interval_list->HasAllocation());

    const auto& first_allocate = live_interval_list->FirstAllocation();

    // Eliminate phi with register shuffling
    //
    // 1) move lhs oeprand into output register if needed
    {
      const auto& allocation =
          GetInterval(phi->Opr(0))
              ->AllocationAtOrAfter(phi->liveness_info().range().start());
      RegisterShuffleMoveTo(live_interval_list, phi, allocation, first_allocate,
                            as_->bb0_register_shuffle());
    }

    // 2) move rhs operand into output register if needed
    {
      const auto& allocation = GetInterval(phi->Opr(1))->LastAllocation();

      RegisterShuffleMoveTo(live_interval_list, phi, allocation, first_allocate,
                            as_->bb1_register_shuffle());
    }
  });
}

void RegisterAllocator::AllocateRegister() { LinearScan(); }

// ============================================================================
// Phase(4)
//
// Rewrite all the LIR graph's operand to its corresponding allocated slot
void RegisterAllocator::MaterializeNativeCallOperand(
    HInstruction* ins, int index, HOperand* opr, HOperand* new_operand,
    const RegisterAllocation& allocation, LiveIntervalList* list) {
  // Insert move instruction before the native call. Notes, all the argument
  // which should be passed by registers are already reserved during RA phase
  if (ins->IsNativeCall()) {
    int call_index;
    if (call_index = ins->MapNativeCallParamIndex(index); call_index < 0) {
      return;
    }

    RegisterKind rk;
    auto size_class = opr->size_class();
    auto is_fp = false;

    if (list->register_info().IsFP()) {
      is_fp = true;
      if (auto maybe = GetABIFPArg(call_index); !maybe) {
        UNREACHABLE() << "native call currently have limitation of parameters";
      } else {
        rk = *maybe;
      }
    } else {
      if (auto maybe = GetABIGPArg(call_index); !maybe) {
        UNREACHABLE() << "native call currently have limitation of parameters";
      } else {
        rk = *maybe;
      }
    }

    // if the register is the same, then do not emit move
    if (allocation.IsRegister() && allocation.r->register_kind() == rk) return;

    // emit mov instruction before the call to marshal parameters
    {
      HOpcode opcode = is_fp ? HOpcode::kMovF : HOpcode::kMov;
      auto mov =
          as_->NewInstructionBefore(ins->ir_node(), opcode, new_operand, ins);
      auto materialize_r =
          Register::GetRegisterFromKindAndSizeClass(rk, size_class);
      CHECK(materialize_r);
      auto to = as_->NewRegister(materialize_r);
      opr->set_instruction(mov);
      mov->SetRAOutput(to);
      ins->OprSet(index, to);  // use new operand
    }
  }
}

void RegisterAllocator::MaterializeOperandTemporary(int i, HInstruction* ins,
                                                    HOperand* opr) {
  auto temp = opr->GetTemporary();
  CHECK(temp->HasLiveIntervalList());
  auto list = temp->live_interval_list();
  auto ref_point = RefPoint(ins, opr);
  const auto& allocation = list->AllocationAtOrAfter(ref_point);
  auto new_operand = allocation.ToOperand(as_, list);
  CHECK(!new_operand->IsTemporary());
  ins->OprSet(i, new_operand);

  MaterializeNativeCallOperand(ins, i, opr, new_operand, allocation, list);
}

void RegisterAllocator::MaterializeOperand(HInstruction* ins) {
  // operand materialization,
  for (auto i = 0; i < ins->opr_size(); i++) {
    if (auto opr = ins->Opr(i); opr->IsTemporary()) {
      MaterializeOperandTemporary(i, ins, opr);
    }
  }
}

void RegisterAllocator::MaterializeOutputTemporary(HInstruction* ins,
                                                   Temporary* temp) {
  CHECK(temp->HasLiveIntervalList());
  auto list = temp->live_interval_list();
  HOperand* new_operand = nullptr;

  if (ins->IsPhi()) {
    const auto& allocation = list->FirstAllocation();
    new_operand = allocation.ToOperand(as_, list);
    CHECK(!new_operand->IsTemporary());
  } else {
    auto ref_point = ins->liveness_info().range().start();
    const auto& allocation = list->AllocationAtOrAfter(ref_point);
    new_operand = allocation.ToOperand(as_, list);
    CHECK(!new_operand->IsTemporary());
  }
  ins->SetRAOutput(new_operand);
}

void RegisterAllocator::MaterializeExit(HInstruction* ins) {
  if (ins->HasExit()) {
    auto exit = ins->exit();

    Snapshot::SSAVarMappings mappings;
    Snapshot snapshot;

    // Materialize the exit(deoptimization) point based on the current
    // allocation status and feed it into the quake's global cache
    exit->ForeachEntry([&](const auto& entry) {
      auto ssa_var = entry.value();
      auto opr = ssa_var->lir_operand();
      auto ref_point = ins->liveness_info().range().use_point();

      if (opr && opr->IsTemporary()) {
        auto temp = opr->GetTemporary();
        CHECK(temp->HasLiveIntervalList());
        auto list = temp->live_interval_list();

        const auto& allocation = list->AllocationAtOrAfter(ref_point);
        auto new_operand = allocation.ToOperand(as_, list);
        mappings.insert(std::make_pair(ssa_var, new_operand));
      }
    });

    snapshot.Construct(mappings, *exit);

    CHECK(!ins->HasDeoptId());
    ins->set_deopt_id(quake_->AddDeopt(
        DeoptReason::kGuard, DeoptNext::kContinueInterp, std::move(snapshot)));
  }
}

void RegisterAllocator::Materialize(HInstruction* ins) {
  MaterializeExit(ins);
  if (!ins->IsPhi()) MaterializeOperand(ins);
  if (auto output = ins->output(); output && output->IsTemporary()) {
    MaterializeOutputTemporary(ins, output->GetTemporary());
  }
}

void RegisterAllocator::GenerateMove(HInstruction* ins,
                                     LiveIntervalList* live_interval_list) {
  std::size_t previous = 0;

  auto rewrite_allocation = [&](const RegisterAllocation& allocation) {
    auto liveness_id = allocation.range.start();
    auto position = LivenessMap(liveness_id);
    const auto& prev_allocation = live_interval_list->AllocationIndex(previous);

    CHECK(allocation.placeholder);
    CHECK(position);

    auto from = prev_allocation.ToOperand(as_, live_interval_list);
    auto to = allocation.ToOperand(as_, live_interval_list);

    const auto is_fp = live_interval_list->register_info().IsFP();
    auto opcode = is_fp ? HOpcode::kTransF : HOpcode::kTrans;
    auto mov = as_->MaterializePlaceholder(allocation.placeholder,
                                           ins->ir_node(), opcode, from);
    mov->SetRAOutput(to);

    previous++;
  };

  live_interval_list->ForeachRestAllocation(std::move(rewrite_allocation));
}

void RegisterAllocator::Rewrite() {
  auto rewrite_instruction = [&](HInstruction* ins) {
    LiveIntervalList* list = nullptr;
    if (auto out = ins->output(); out && out->IsTemporary()) {
      list = out->GetTemporary()->live_interval_list();
    }
    Materialize(ins);
    if (list) GenerateMove(ins, list);
  };

  PhiSpill();

  as_->ForeachInstructionExcludePhi(
      [&](HInstruction* ins) { rewrite_instruction(ins); });

  PhiShuffle();

  as_->ForeachPhi([&](HInstruction* ins) { rewrite_instruction(ins); });
}

// ============================================================================
// Entry
void RegisterAllocator::Run() {
  Initialize();
  Linearize();
  LivenessAnalyse();
  AllocateRegister();
  Rewrite();

  // lastly rewrite some information back into Quake object to allow
  // deoptimization
  quake_->set_cell_size(as_->stack_slot()->cell_size());
  quake_->set_spill_size(as_->stack_slot()->spill_size());
}

void RegisterAllocator::TraceLiveness() {
  std::stringstream fmt;
  as_->ForeachInstruction([&](auto ins) {
    fmt << ins->Print() << " :{" << ins->liveness_info().Print() << "}\n";
  });
  LOG(ERROR) << fmt.str();
}

RegisterAllocator::RegisterAllocator(Quake* q, HAssembler* as)
    : quake_(q),
      as_(as),
      instruction_size_(0),
      liveness_id_(0),
      liveness_map_(),
      rr_list_(),
      spill_list_(),
      ra_general_(this, GetFreeGPRegister()),
      ra_float_(this, GetFreeFPRegister()) {}

// ----------------------------------------------------------------------------
// RegisterAllocation
HOperand* RegisterAllocation::ToOperand(HAssembler* as,
                                        LiveIntervalList* ll) const {
  if (IsSpill()) {
    auto opr = as->NewMemStack(slot);
    opr->set_instruction(ll->instruction_checked());
    return opr;
  } else if (IsRegister()) {
    auto materialize_r = Register::GetRegisterFromKindAndSizeClass(
        r->register_kind(), size_class);
    CHECK(materialize_r);
    auto opr = as->NewRegister(materialize_r);
    opr->set_instruction(ll->instruction_checked());
    return opr;
  } else {
    UNREACHABLE();
    return nullptr;
  }
}

bool RegisterAllocation::SameAllocation(const RegisterAllocation& that) const {
  if (IsSpill() && that.IsSpill()) {
    return slot == that.slot;
  }
  if (IsRegister() && that.IsRegister()) {
    return r == that.r;
  }
  return false;
}

// ----------------------------------------------------------------------------
// LiveIntervalList

void LiveIntervalList::SetUpOutput() {
  CHECK(instruction_->IsValue());

  // (0) add type and size class
  register_info_.rsize = instruction_->output_size_class();
  register_info_.rtype = instruction_->opcode_info().output.output0_rtype();

  // (1) native calling arguments hint
  if (auto param_list = instruction_->NativeCallParamRef();
      !param_list.empty()) {
    // FIXME(dpeng): Currently our hint does not carry weight information
    for (const auto& p : param_list) {
      if (instruction_->IsRegisterGP()) {
        if (auto maybe = GetABIGPArg(p); maybe) {
          register_info_.hint.Set(static_cast<std::uint16_t>(*maybe));
        }
      } else {
        if (auto maybe = GetABIFPArg(p); maybe) {
          register_info_.hint.Set(static_cast<std::uint16_t>(*maybe));
        }
      }
    }
  }

  // (3) arch hint
  GetRAHint(instruction_, &register_info_);
}

void LiveIntervalList::SetUpReserve() {
  CHECK(register_info_.requirement.type ==
        RegisterAllocationRequirement::kForceRegister);

  register_info_.rsize =
      register_info_.requirement.force_register->size_class();

  register_info_.rtype =
      register_info_.requirement.force_register->register_type();

  type_ = kReserve;
}

LivenessPoint LiveIntervalList::NextAllocateEnd() const {
  if (IsReserve()) return range().end();
  return instruction_checked()->liveness_info().last_use();
}

RegisterAllocation* LiveIntervalList::AllocateRegister(
    const Register* r, SizeClass sc, const LivenessPoint& start,
    RegisterAllocator* ra) {
  if (!allocate_.empty()) {
    allocate_.back().range.EndBefore(start);
  }

  auto range = LivenessRange{start, NextAllocateEnd()};

  allocate_.push_back(RegisterAllocation{r, sc, range});
  auto ret = &allocate_.back();

  if (HasShuffle()) {
    auto liveness_id = ret->range.start();
    auto position = ra->LivenessMap(liveness_id);
    ret->placeholder = ra->as_->NewPlaceholderBefore(position);
  }
  return ret;
}

RegisterAllocation* LiveIntervalList::AllocateSpill(MemoryStack::Slot slot,
                                                    const LivenessPoint& start,
                                                    RegisterAllocator* ra) {
  if (!allocate_.empty()) {
    allocate_.back().range.EndBefore(start);
  }

  auto range = LivenessRange{start, NextAllocateEnd()};
  allocate_.push_back(RegisterAllocation{slot, range});

  auto ret = &allocate_.back();
  if (HasShuffle()) {
    auto liveness_id = ret->range.start();
    auto position = ra->LivenessMap(liveness_id);
    ret->placeholder = ra->as_->NewPlaceholderBefore(position);
  }
  return ret;
}

bool LiveIntervalList::ExpireWhen(const LivenessPoint& when) const {
  CHECK(allocate_.size() >= 1);
  const auto& current = allocate_.back();
  return current.range.end() < when;
}

const RegisterAllocation& LiveIntervalList::AllocationAtOrAfter(
    const LivenessPoint& when) const {
  static RegisterAllocation kDummy;

  // Using lower-bound ?
  for (const auto& a : allocate_) {
    if (a.range.Contains(when)) {
      return a;
    }
  }

  UNREACHABLE() << instruction_checked()->Print();
  return kDummy;
}

// Check whether the first allocation (can be register or spill slot) matches
// the current allocation
bool LiveIntervalList::MatchFirstAllocation() const {
  CHECK(allocate_.size() >= 1);
  if (allocate_.size() > 1) {
    const auto& first_allocate = allocate_[0];
    const auto& current_allocate = allocate_.back();

    return first_allocate.SameAllocation(current_allocate);
  }

  return true;
}

const Register* LiveIntervalList::MaybeRegisterAllocation(
    const LivenessPoint& when) const {
  for (const auto& a : allocate_) {
    if (a.range.Contains(when) && a.IsRegister()) {
      CHECK(a.r);
      return a.r;
    }
  }
  return nullptr;
}

std::optional<MemoryStack::Slot> LiveIntervalList::MaybeStackAllocation(
    const LivenessPoint& when) const {
  for (const auto& a : allocate_) {
    if (a.range.Contains(when) && a.IsSpill()) {
      return a.slot;
    }
  }
  return {};
}

// ============================================================================
// Arch
// ============================================================================

void RegisterAllocator::AllocateToTempRegisterWhen(LiveIntervalList* target,
                                                   LiveIntervalList* now) {
  auto temp_range =
      LivenessRange{now->range().use_point(), target->range().end()};

  target->SetTemp(temp_range);
  DEFER([&]() { target->ClearTemp(); });

  const auto& rinfo = target->register_info();
  if (rinfo.rtype == RegisterType::kGeneral) {
    ra_general_.ForceAllocateRegister(target);
  } else {
    ra_float_.ForceAllocateRegister(target);
  }
}

void RegisterAllocator::AllocateToTempRegisterWhen(LiveIntervalList* target,
                                                   LiveIntervalList* now,
                                                   const Register* force) {
  auto temp_range =
      LivenessRange{now->range().use_point(), target->range().end()};

  target->SetTemp(temp_range);
  DEFER([&]() { target->ClearTemp(); });

  const auto& rinfo = target->register_info();
  if (rinfo.rtype == RegisterType::kGeneral) {
    ra_general_.ForceAllocateRegister(target, force);
  } else {
    ra_float_.ForceAllocateRegister(target, force);
  }
}

#ifdef QUAKE_TARGET_X64
// x64 constraint
//
// 1) cannot have 2 memory operand, ie must spill some position for first one
//    to be moved into a register
//
// 2) the memory operand cannot show up in the first operand, must be put into
//    the second operand
void RegisterAllocator::SolveX64BinaryOpConstraint(LiveIntervalList* l) {
  auto ins = l->instruction_checked();
  CHECK(ins->opr_size() == 2);
  auto opr1 = ins->Opr(0);
  auto lhs_list = GetInterval(opr1);
  const auto& lhs_alloc = lhs_list->AllocationAtOrAfter(l->range().use_point());

  if (lhs_alloc.IsSpill()) {
    // make sure lhs_list interval list is in register when l is on
    AllocateToTempRegisterWhen(lhs_list, l);
  }
}

void RegisterAllocator::SolveX64ShiftOpConstraint(LiveIntervalList* l) {
  auto ins = l->instruction_checked();
  CHECK(ins->opr_size() == 2);
  auto opr2 = ins->Opr(1);
  auto rhs_list = GetInterval(opr2);
  const auto& rhs_alloc = rhs_list->AllocationAtOrAfter(l->range().use_point());

  // The x64's shift opertion's rhs operand must be inside of rcx register,
  // since the instruction will just use cl operand
  auto rcx = Register::GetRegisterFromKind(RegisterKind::kRcx);

  if (rhs_alloc.IsSpill()) {
    AllocateToTempRegisterWhen(rhs_list, l, rcx);
  } else {
    CHECK(rhs_alloc.IsRegister());
    if (rhs_alloc.r->register_kind() != RegisterKind::kRcx) {
      // must be spilled into rcx
      AllocateToTempRegisterWhen(rhs_list, l, rcx);
    }
  }
}

void RegisterAllocator::OutputX64OpConstraint(LiveIntervalList* ins, int index,
                                              RegisterSet* output) {
  const auto& rinfo = ins->register_info();
  switch (rinfo.opr_constraint) {
    case OperandConstraint::kX64ShiftOp:
      if (index == 1) output->Set(RegisterKind::kRcx);
      break;

    default:
      break;
  }
}

#endif  // QUAKE_TARGET_X64

}  // namespace lir
}  // namespace quake
