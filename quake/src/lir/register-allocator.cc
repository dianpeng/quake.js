#include "lir/register-allocator.h"
#include "lir/parallel-move.h"
#include "lir/snapshot.h"

#include "ir/checkpoint.h"
#include "ir/node.h"
#include "ir/ssa-var.h"

#include "cg/abi.h"

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

LiveInterval* GetLiveInterval(HOperand* opr) {
  CHECK(opr->IsTemporary());
  auto temp = opr->GetTemporary();
  CHECK(temp->HasLiveInterval());
  return temp->live_interval_list();
}

LiveInterval* GetLiveInterval(HInstruction* ins) {
  auto out = ins->output();
  CHECK(out);
  return GetLiveInterval(out);
}

LiveInterval* MaybeGetLiveInterval(HOperand* opr) {
  if (opr->IsTemporary()) {
    if (auto temp = opr->GetTemporary(); temp->HasLiveInterval()) {
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
  if (ins->IsLoopPhi()) {
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
  if (ins->IsLoopPhi()) {
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
    if (ins && ins->IsLoopPhi()) ins = as_->bb1();

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
    // be invovled inside of RA, we should generate LiveInterval inside of
    // its Temporary to allow RA
    auto live_interval = AllocateLiveInterval(ins);
    if (auto out = ins->output(); out) {
      auto temp = out->GetTemporary();
      temp->set_live_interval_list(live_interval);
    }
  };

  // Mark BB0's node's liveness id, ie liveness start
  as_->ForeachInstructionBB0([&](HInstruction* ins) {
    CHECK(!ins->IsLoopPhi());
    // Skip phi node liveness id assignment until the very last
    set_liveness_start(ins);
  });

  // Then followed by liveness id of PHI. PHI node are just placed right after
  // the loop condition testing of first iteration
  as_->ForeachLoopPhi([&](HInstruction* ins) {
    CHECK(ins->IsLoopPhi());
    set_liveness_start(ins);
  });

  // Lastly, all the BB1 instruction will the be marked incrementally
  as_->ForeachInstructionBB1([&](HInstruction* ins) {
    CHECK(!ins->IsLoopPhi());
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
  as_->ForeachLoopPhi([&](HInstruction* ins) {
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
  as_->ForeachInstructionExcludeLoopPhiBackward([&](HInstruction* ins) {
    CHECK(!ins->IsLoopPhi());

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
LiveInterval* RegisterAllocator::AllocateLiveInterval(HInstruction* ins) {
  auto list =
      zone()->New<LiveInterval>(zone(), ins, ins->liveness_info().range());

  list->SetUpOutput();

  // --------------------------------------------------------------------------
  // Based on register hint to setup other constraint needed reservations
  //
  // Register reservation handling
  list->register_info().reserve.ForeachSet([&](std::uint32_t id) {
    auto list = zone()->New<LiveInterval>(zone(), ins,
                                          ins->liveness_info().liveness_id(),
                                          Register::GetRegisterFromKindId(id));
    list->SetUpReserve();
    rr_list_.emplace_back(list);
  });

  // Operand reservation handling
  if (ins->IsNativeCall()) {
    auto fp_index = 0;
    auto gp_index = 0;
    auto fp_size =
        static_cast<int>(list->register_info().opr_abi_reserve_fp.size());
    auto gp_size =
        static_cast<int>(list->register_info().opr_abi_reserve_gp.size());

    // 1) Foreach operand that is part of the native call, we create temp
    //    list with alias instruction pointed to its operand's instruction
#if 0
    ins->ForeachNativeCallParameter([&](HOperand* opr) {
      const Register* forced_register = nullptr;

      if (GetLiveInterval(opr)->register_info().IsFP()) {
        forced_register = list->register_info().opr_abi_reserve_fp[fp_index];
        ++fp_index;
      } else {
        forced_register = list->register_info().opr_abi_reserve_gp[gp_index];
        ++gp_index;
      }
      auto pins = opr->instruction_checked();
      auto temp_list = zone()->New<LiveInterval>(
          zone(), ins, ins->liveness_info().liveness_id(), forced_register,
          pins);

      temp_list->SetUpReserve();
      rr_list_.emplace_back(temp_list);
    });
#endif

    // 2) For rest of abi reserved register, as long as it is not been part of
    //    parameter of native function call, we just emit a normal temp list
    for (; gp_index < gp_size; gp_index++) {
      auto r = list->register_info().opr_abi_reserve_gp[gp_index];
      auto tl = zone()->New<LiveInterval>(
          zone(), ins, ins->liveness_info().liveness_id(), r);
      tl->SetUpReserve();
      rr_list_.emplace_back(tl);
    }

    for (; fp_index < fp_size; fp_index++) {
      auto r = list->register_info().opr_abi_reserve_fp[fp_index];
      auto tl = zone()->New<LiveInterval>(
          zone(), ins, ins->liveness_info().liveness_id(), r);
      tl->SetUpReserve();
      rr_list_.emplace_back(tl);
    }
  } else {
    CHECK(list->register_info().opr_abi_reserve_fp.empty());
    CHECK(list->register_info().opr_abi_reserve_gp.empty());
  }

  // Lastly push back the list, we gonna use stable_sort to sort rr_list_
  // which makes sure that the instruction itself's output will be
  // generated *after* all the reserved register is done
  rr_list_.emplace_back(list);

  return list;
}

// ============================================================================
//
// Phase(3) Register allocation
//
// ---------------------------------------------------------------------------
// Linear scan allocation. This covers the 1) and 2) parts of the work. It
// does a interval constraint solving until we reach a met condition. Notes
// ABI call is handled as on demand pops/push register. It is safe to use
// generate it at any point without needing to bother RA.
void RegisterAllocator::LinearScan() {
  struct Comp {
    bool operator()(const LiveInterval* l, const LiveInterval* r) {
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

void RegisterAllocator::DoLinearScan(LiveInterval* interval) {
  const auto& rinfo = interval->register_info();
  Expire(interval);
  RematerializeOperand(interval);

  switch (interval->instruction_checked()->opcode()) {
    case HOpcode::kSlowBlockStart:
      OnSlowBlockStart();
      break;

    case HOpcode::kSlowBlockEnd:
      OnSlowBlockEnd(interval->instruction_checked());
      break;

    default:
      break;
  }

  if (!interval->MustHaveOutput()) return;

  if (rinfo.rtype == RegisterType::kGeneral) {
    ra_general_.LinearScan(interval);
  } else {
    ra_float_.LinearScan(interval);
  }
}

void RegisterAllocator::Expire(LiveInterval* l) {
  ExpireSpill(l);
  ra_general_.ExpireRegister(l);
  ra_float_.ExpireRegister(l);
}

void RegisterAllocator::ExpireSpill(LiveInterval* l) {
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

void RegisterAllocator::RematerializeOperand(LiveInterval* l) {
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
  if (l->instruction_checked()->IsLoopPhi()) return;

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

      auto live_interval = GetLiveInterval(opr);

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

void RegisterAllocator::RAStatus::LinearScan(LiveInterval* l) {
  SolveOperandConstraint(l);
  AllocateOutput(l);
}

void RegisterAllocator::RAStatus::SolveOperandConstraint(LiveInterval* l) {
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

void RegisterAllocator::RAStatus::AllocateOutput(LiveInterval* interval) {
  AllocateOrSpill(interval);
}

void RegisterAllocator::RAStatus::ExpireRegister(LiveInterval* l) {
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
      itr++;  // the list is sorted ? we should just bailout
    }
  }
}

void RegisterAllocator::RAStatus::AllocateOrSpill(LiveInterval* l) {
  if (!DoAllocateRegister(l)) {
    AllocateSpill(l);
  }
}

void RegisterAllocator::RAStatus::AllocateSpill(LiveInterval* l) {
  // try to find the properly spilled candidate
  if (auto active_idx = GetActiveRecordForSpill(l); active_idx >= 0) {
    const auto& active_record = active_list[active_idx];

    auto active_allocation = active_record.allocated;
    auto spill_range = active_allocation->AllocateRange(l->range().start());

    active_allocation->AllocateSpill(
        ra->GetSpillSlot(spill_range, active_allocation), l->range().start(),
        ra);

    // Free the register from active to inactive
    auto inactive_idx = TransferActiveToInactive(l, active_idx);

    // Allocate the active register back to us here
    ForceAllocateRegister(l, inactive_idx);
  } else {
    auto spill_range = l->AllocateRange(l->range().start());

    // Should just spill on the stack directly
    auto spill_slot = ra->GetSpillSlot(spill_range, l);
    l->AllocateSpill(spill_slot, l->range().start(), ra);
  }
}

bool RegisterAllocator::RAStatus::DoAllocateRegister(LiveInterval* l) {
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
    LiveInterval* l, const std::vector<RegisterRecord>& target_list) {
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
    LiveInterval* l, const std::vector<RegisterRecord>& target_list) {
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
    LiveInterval* operand, int index, LiveInterval* ins) {
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
    LiveInterval* l, const std::vector<RegisterRecord>& target_list) {
  const auto& rinfo = l->register_info();

  // --------------------------------------------------------------------------
  // Try to respect the hint_policy
  switch (rinfo.hint_policy) {
    case RegisterHintPolicy::kInput0AsOutput: {
      // for x64's instruction like add/sub/mul etc ... it requires the
      // one of its operand to be both output and input(0)
      auto ll = MaybeGetLiveInterval(l->instruction()->Opr(0));
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
bool RegisterAllocator::RAStatus::TryAllocateRegister(LiveInterval* l) {
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
    LiveInterval* l, const Register* force_r) {
  // Check whether l has a alias instruction, if so if the aliased one is
  // already in the forced register, then just skip it
  if (l->IsReserve() && l->HasAlias()) {
    auto alias = l->alias();
    auto alias_list = GetLiveInterval(alias);
    const auto& allocation =
        alias_list->AllocationAtOrAfter(l->range().start());
    if (allocation.IsRegister() && allocation.r == force_r) {
      return;
    }
  }

  int idx;
  if (idx = MapInactive(force_r); idx == -1) {
    idx = SpillActiveRegister(l, force_r);
  }
  ForceAllocateRegister(l, idx);
}

// Try to find a register to be used by L
void RegisterAllocator::RAStatus::ForceAllocateRegister(LiveInterval* l) {
  // Check whether l has a alias instruction, if so if the aliased one is
  // already in the forced register, then just skip it
  if (l->IsReserve() && l->HasAlias()) {
    auto alias = l->alias();
    auto alias_list = GetLiveInterval(alias);
    const auto& allocation =
        alias_list->AllocationAtOrAfter(l->range().start());
    if (allocation.IsRegister()) {
      return;  // already inside of a register, just do nothing
    }
  }

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

std::string RegisterAllocator::RAStatus::Print() const {
  std::stringstream fmt;
  auto dump = [&](const RegisterRecord& rr, bool is_active) {
    if (is_active) {
      fmt << "A ";
      fmt << rr.r->Print() << "@("
          << rr.allocated->instruction_checked()->Print() << ") @("
          << rr.range.Print() << ")\n";
    } else {
      fmt << "D ";
      fmt << rr.r->Print() << "\n";
    }
  };

  for (const auto& r : active_list) {
    dump(r, true);
  }
  for (const auto& r : inactive_list) {
    dump(r, false);
  }

  return fmt.str();
}

void RegisterAllocator::RAStatus::MustAllocateRegister(
    LiveInterval* l, const Register* r, const LivenessPoint& start) {
  if (auto idx = MapInactive(r); idx >= 0) {
    TransferInactiveToActive(l, idx, start);
  } else {
    UNREACHABLE();
  }
}

void RegisterAllocator::RAStatus::ForceAllocateRegister(LiveInterval* l,
                                                        int idx) {
  TransferInactiveToActive(l, idx);
}

void RegisterAllocator::RAStatus::RematerializeRegister(
    LiveInterval* l, const LivenessPoint& when) {
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
    LiveInterval* l, int idx, const LivenessPoint& when) {
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

int RegisterAllocator::RAStatus::TransferInactiveToActive(LiveInterval* l,
                                                          int idx) {
  return TransferInactiveToActive(l, idx, l->range().start());
}

int RegisterAllocator::RAStatus::TransferActiveToInactive(LiveInterval* l,
                                                          int idx) {
  CHECK(idx >= 0 && idx < static_cast<int>(active_list.size()));
  auto reg = active_list[idx].r;
  auto inactive_record = RegisterRecord{
      reg, nullptr, LivenessRange::StartUntilLast(l->range().start())};

  // Notes, after removing, the sorted vector is still in order
  base::EraseIndex(&active_list, static_cast<std::size_t>(idx));

  inactive_list.push_back(inactive_record);
  return static_cast<int>(inactive_list.size() - 1);
}

int RegisterAllocator::RAStatus::SpillActiveRegister(LiveInterval* l,
                                                     const Register* r) {
  auto idx = MapActive(r);
  CHECK(idx >= 0);
  return SpillActiveRegister(l, idx);
}

int RegisterAllocator::RAStatus::SpillActiveRegister(LiveInterval* l, int idx) {
  CHECK(idx >= 0);

  auto allocated = active_list[idx].allocated;
  auto spill_range = allocated->AllocateRange(l->range().start());
  auto spill_slot = ra->GetSpillSlot(spill_range, allocated);

  // now let the interval spill at the current position
  allocated->AllocateSpill(spill_slot, l->range().start(), ra);

  // now this register is free and then transfer it back to the inactive_list
  return TransferActiveToInactive(l, idx);
}

// Get spill slot. Notes spill slot is baised against the RBP, ie the starting
// point of the RSP. Notes, this function will find an spill slot, if not it
// will just create one
MemoryStack::Slot RegisterAllocator::GetSpillSlot(const LivenessRange& range,
                                                  LiveInterval* l) {
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

void RegisterAllocator::FreeSpillSlot(LiveInterval* l, MemoryStack::Slot slot) {
  for (auto& ss : spill_list_) {
    if (ss.use && ss.slot == slot) {
      CHECK(ss.allocation == l);
      ss.Remove();
      return;
    }
  }

  UNREACHABLE();
}

int RegisterAllocator::RAStatus::GetActiveRecordForSpill(LiveInterval* l) {
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

  if (!ins->IsLoopPhi()) {
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
      if (itr->instruction != ins && itr->instruction->IsLoopPhi()) {
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
void RegisterAllocator::RegisterShuffleMoveTo(LiveInterval* ll,
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
void RegisterAllocator::LoopPhiShuffleBB0() {
  // Shuffle the pre-loop iteration part.
  as_->ForeachLoopPhi([&](auto phi) {
    CHECK(phi->IsLoopPhi());
    auto live_interval_list = GetLiveInterval(phi);
    CHECK(live_interval_list->HasAllocation());

    const auto& first_allocate = live_interval_list->FirstAllocation();

    // Eliminate phi with register shuffling
    //
    // 1) move lhs oeprand into output register if needed
    {
      const auto& allocation =
          GetLiveInterval(phi->Opr(0))
              ->AllocationAtOrAfter(phi->liveness_info().range().start());
      RegisterShuffleMoveTo(live_interval_list, phi, allocation, first_allocate,
                            as_->bb0_register_shuffle());
    }
  });
}

void RegisterAllocator::LoopPhiShuffleBB1() {
  static const auto kInvalid = std::numeric_limits<MemoryStack::Slot>::max();
  struct Broker : public ParallelMove::RABroker {
   public:
    ParallelMove::Slot Resolve(HOperand* opr) override {
      const auto& allocation = GetLiveInterval(opr)->LastAllocation();
      if (allocation.IsRegister()) {
        return ParallelMove::Slot{allocation.r};
      } else {
        return ParallelMove::Slot{allocation.slot};
      }
    }

    ParallelMove::Slot ResolvePhi(HOperand* opr) override {
      CHECK(opr->instruction_checked()->IsLoopPhi());
      const auto& allocation = GetLiveInterval(opr)->FirstAllocation();
      if (allocation.IsRegister()) {
        return ParallelMove::Slot{allocation.r};
      } else {
        return ParallelMove::Slot{allocation.slot};
      }
    }
  };

  auto build_phi_move_list = [&]() -> std::vector<ParallelMove::Move> {
    std::vector<ParallelMove::Move> move_list;
    as_->ForeachLoopPhi([&](HInstruction* phi) {
      CHECK(phi->IsLoopPhi());
      CHECK(phi->output()->instruction_checked() == phi);
      move_list.push_back(ParallelMove::Move{phi, phi->Opr(1), phi->output()});
    });
    return move_list;
  };

  Broker broker;
  ParallelMove phi_shuffle{quake_, as_, &broker, spill_list_.size(),
                           build_phi_move_list()};

  auto result = phi_shuffle.Resolve();

  std::vector<MemoryStack::Slot> micro_spill_slot;
  micro_spill_slot.resize(as_->instruction_maxid(), kInvalid);

  auto is_micro_spill = [&](HInstruction* ins) -> bool {
    return std::find_if(result.micro_spill.begin(), result.micro_spill.end(),
                        [&](const auto& move) -> bool {
                          return ins == move.instruction;
                        }) != result.micro_spill.end();
  };

  // (1) generate all the phi shuffling instruction
  for (auto& move : result.move_list) {
    auto ins = move.instruction;
    auto interval = GetLiveInterval(ins);
    const auto& allocation = GetLiveInterval(ins->Opr(1))->LastAllocation();

    if (is_micro_spill(ins)) {
      auto range = LivenessRange::OneInstruction(
          as_->loop_back()->liveness_info().range().start());

      auto spill_slot = GetSpillSlot(range, interval);

      micro_spill_slot[ins->id()] = spill_slot;

      RegisterAllocation target_allocation{spill_slot, range};

      RegisterShuffleMoveTo(interval, ins, allocation, target_allocation,
                            as_->bb1_register_shuffle());
    } else {
      RegisterShuffleMoveTo(interval, ins, allocation,
                            interval->FirstAllocation(),
                            as_->bb1_register_shuffle());
    }
  }

  // (2) Recover all the micro spill back
  for (auto& move : result.micro_spill) {
    auto spill = move.instruction;

    auto spill_slot = micro_spill_slot[spill->id()];
    CHECK(spill_slot != kInvalid);

    auto interval = GetLiveInterval(spill);

    auto range = LivenessRange::OneInstruction(
        as_->loop_back()->liveness_info().range().start());

    RegisterAllocation allocation{spill_slot, range};

    RegisterShuffleMoveTo(interval, spill, allocation,
                          interval->FirstAllocation(),
                          as_->bb1_register_shuffle());
  }
}

void RegisterAllocator::LoopPhiShuffle() {
  LoopPhiShuffleBB0();
  LoopPhiShuffleBB1();
}

void RegisterAllocator::BranchPhiElimination(HInstruction* ins) {
  // elimination of branch phi node
  CHECK(ins->IsBranchPhi());
  auto lhs = ins->Opr(0);  // lhs of phi
  auto rhs = ins->Opr(1);  // rhs of phi

  auto lhs_label = ins->Opr(2)->GetLabel()->bind();
  auto rhs_label = ins->Opr(3)->GetLabel()->bind();

  auto lhs_live = GetLiveInterval(lhs);  // lhs interval
  auto rhs_live = GetLiveInterval(rhs);  // rhs interval
  auto interval = GetLiveInterval(ins);  // ins interval

  // First allocation represents the PHI's very first move. All the branch phi
  // will always appear at the very first inside of the merge branch. Notes
  // our branch phi is only created inside of LIR, and have very specific
  // CFG pattern
  const auto& allocation = interval->FirstAllocation();

  const auto& lhs_allocation = lhs_live->AllocationAtOrAfter(
      lhs_label->liveness_info().range().output_point());

  const auto& rhs_allocation = rhs_live->AllocationAtOrAfter(
      rhs_label->liveness_info().range().output_point());

  RegisterShuffleMoveTo(interval, ins, lhs_allocation, allocation,
                        lhs_label->next());

  RegisterShuffleMoveTo(interval, ins, rhs_allocation, allocation,
                        rhs_label->next());
}

void RegisterAllocator::AllocateRegister() { LinearScan(); }

// ============================================================================
// Phase(4)
//
// Rewrite all the LIR graph's operand to its corresponding allocated slot
void RegisterAllocator::MaterializeNativeCallOperand(
    HInstruction* ins, int index, HOperand* opr, HOperand* new_operand,
    const RegisterAllocation& allocation, LiveInterval* list) {
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
    if (allocation.IsRegister() && allocation.r->register_kind() == rk) {
      return;
    }

    // emit mov instruction before the call to marshal parameters
    {
      HOpcode opcode = is_fp ? HOpcode::kMovF : HOpcode::kMov;
      auto mov =
          as_->NewInstructionBefore(ins->ir_node(), opcode, new_operand, ins);
      auto materialize_r =
          Register::GetRegisterFromKindAndSizeClass(rk, size_class);
      CHECK(materialize_r);
      auto to = as_->NewRegister(materialize_r);

      mov->SetRAOutput(to);
      ins->OprSet(index, to);  // use new operand
    }
  }
}

void RegisterAllocator::MaterializeOperandTemporary(int i, HInstruction* ins,
                                                    HOperand* opr) {
  auto temp = opr->GetTemporary();
  CHECK(temp->HasLiveInterval());
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
  CHECK(temp->HasLiveInterval());
  auto list = temp->live_interval_list();
  HOperand* new_operand = nullptr;

  if (ins->IsLoopPhi()) {
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
      if (!opr) return;

      auto ref_point = ins->liveness_info().range().start();

      if (opr->IsTemporary()) {
        auto temp = opr->GetTemporary();
        CHECK(temp->HasLiveInterval());
        auto list = temp->live_interval_list();

        const auto& allocation = list->AllocationAtOrAfter(ref_point);
        auto new_operand = allocation.ToOperand(as_, list);
        mappings.insert(std::make_pair(ssa_var, new_operand));
      } else {
        mappings.insert(std::make_pair(ssa_var, opr));
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
  if (!ins->IsLoopPhi()) MaterializeOperand(ins);
  if (auto output = ins->output(); output && output->IsTemporary()) {
    MaterializeOutputTemporary(ins, output->GetTemporary());
  }
}

void RegisterAllocator::GenerateMove(HInstruction* ins,
                                     LiveInterval* live_interval_list) {
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
    // try eliminate branch phi
    if (ins->IsBranchPhi()) BranchPhiElimination(ins);

    // generate output from temporary assigned virtual register
    LiveInterval* list = nullptr;
    if (auto out = ins->output(); out && out->IsTemporary()) {
      list = out->GetTemporary()->live_interval_list();
    }

    // materialize operands
    Materialize(ins);

    // generate shuffle
    if (list) GenerateMove(ins, list);
  };

  as_->ForeachInstructionExcludeLoopPhi(
      [&](HInstruction* ins) { rewrite_instruction(ins); });

  LoopPhiShuffle();

  as_->ForeachLoopPhi([&](HInstruction* ins) { rewrite_instruction(ins); });
}

void RegisterAllocator::OnSlowBlockStart() {
  slow_path_ = true;
  slow_path_recovery_ra_.clear();
}

void RegisterAllocator::OnSlowBlockEnd(HInstruction* ins) {
  CHECK(slow_path_);
  slow_path_ = false;
  auto point = ins->liveness_info().range().start();

  // NOTES(dpeng): This code is very specific, it assumes it will only work in
  // our FastSlow pattern. Otherwise it will not work, ie CRASH
  for (const auto& s : slow_path_recovery_ra_) {
    CHECK(s.to.IsRegister());
    CHECK(s.from.IsSpill());

    // free the from slot
    FreeSpillSlot(s.interval, s.from.slot);

    // allocate the to slot
    if (s.to.r->IsFP()) {
      ra_float_.MustAllocateRegister(s.interval, s.to.r, point);
    } else {
      ra_general_.MustAllocateRegister(s.interval, s.to.r, point);
    }
  }
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
  quake_->set_frame_size(as_->stack_slot()->frame_size());
  quake_->set_misc_size(as_->stack_slot()->misc_size());
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
                                        LiveInterval* ll) const {
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
// LiveInterval

void LiveInterval::SetUpOutput() {
  if (instruction_->IsValue()) {
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
  }

  // (3) arch hint
  GetRAHint(instruction_, &register_info_);
}

void LiveInterval::SetUpReserve() {
  CHECK(register_info_.requirement.type ==
        RegisterAllocationRequirement::kForceRegister);

  register_info_.rsize =
      register_info_.requirement.force_register->size_class();

  register_info_.rtype =
      register_info_.requirement.force_register->register_type();

  type_ = kReserve;
}

LivenessPoint LiveInterval::NextAllocateEnd() const {
  if (IsReserve()) return range().end();
  return instruction_checked()->liveness_info().last_use();
}

RegisterAllocation* LiveInterval::AllocateRegister(const Register* r,
                                                   SizeClass sc,
                                                   const LivenessPoint& start,
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

RegisterAllocation* LiveInterval::AllocateSpill(MemoryStack::Slot slot,
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

  if (ra->slow_path_ && allocate_.size() >= 2) {
    const auto& from = allocate_.back();
    const auto& to = allocate_[allocate_.size() - 2];
    ra->RecordSlowPathRecoveryRA(this, from, to);
  }

  return ret;
}

std::string RegisterAllocation::Print() const {
  if (IsSpill()) {
    return base::Fmt("#(%d)", slot);
  } else {
    return base::Fmt("$%s", r->Print().c_str());
  }
}

std::string LiveInterval::Print() const {
  std::stringstream fmt;

  fmt << instruction_checked()->Print() << "{\n";
  for (const auto& x : allocate_) {
    fmt << x.Print();
    fmt << " " << x.range.Print() << "\n";
  }

  fmt << "}\n";
  return fmt.str();
}

bool LiveInterval::ExpireWhen(const LivenessPoint& when) const {
  CHECK(allocate_.size() >= 1);
  const auto& current = allocate_.back();
  return current.range.end() < when;
}

const RegisterAllocation& LiveInterval::AllocationAtOrAfter(
    const LivenessPoint& when) const {
  static RegisterAllocation kDummy;

  for (const auto& a : allocate_) {
    if (a.range.Contains(when)) {
      return a;
    }
  }

  UNREACHABLE();
  return kDummy;
}

// Check whether the first allocation (can be register or spill slot) matches
// the current allocation
bool LiveInterval::MatchFirstAllocation() const {
  CHECK(allocate_.size() >= 1);
  if (allocate_.size() > 1) {
    const auto& first_allocate = allocate_[0];
    const auto& current_allocate = allocate_.back();

    return first_allocate.SameAllocation(current_allocate);
  }

  return true;
}

const Register* LiveInterval::MaybeRegisterAllocation(
    const LivenessPoint& when) const {
  for (const auto& a : allocate_) {
    if (a.range.Contains(when) && a.IsRegister()) {
      CHECK(a.r);
      return a.r;
    }
  }
  return nullptr;
}

std::optional<MemoryStack::Slot> LiveInterval::MaybeStackAllocation(
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

void RegisterAllocator::AllocateToTempRegisterWhen(LiveInterval* target,
                                                   LiveInterval* now) {
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

void RegisterAllocator::AllocateToTempRegisterWhen(LiveInterval* target,
                                                   LiveInterval* now,
                                                   const Register* force) {
  auto temp_range =
      LivenessRange{now->range().use_point(), target->range().end()};

  target->SetTemp(temp_range);
  DEFER([&]() { target->ClearTemp(); });

  const auto& rinfo = target->register_info();
  if (rinfo.IsGP()) {
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
void RegisterAllocator::SolveX64BinaryOpConstraint(LiveInterval* l) {
  auto ins = l->instruction_checked();
  CHECK(ins->opr_size() == 2);
  auto opr1 = ins->Opr(0);
  auto lhs_list = GetLiveInterval(opr1);
  const auto& lhs_alloc = lhs_list->AllocationAtOrAfter(l->range().use_point());

  if (lhs_alloc.IsSpill()) {
    // make sure lhs_list interval list is in register when l is on
    AllocateToTempRegisterWhen(lhs_list, l);
  }
}

void RegisterAllocator::SolveX64ShiftOpConstraint(LiveInterval* l) {
  auto ins = l->instruction_checked();
  CHECK(ins->opr_size() == 2);
  auto opr2 = ins->Opr(1);
  auto rhs_list = GetLiveInterval(opr2);
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

void RegisterAllocator::OutputX64OpConstraint(LiveInterval* ins, int index,
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
