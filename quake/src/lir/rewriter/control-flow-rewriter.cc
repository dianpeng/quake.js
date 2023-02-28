#include "lir/rewriter/control-flow-rewriter.h"
#include "lir/rewriter/normalize-branch.h"

#include "lir/assembler.h"
#include "lir/instruction.h"

namespace quake {
namespace lir {
namespace rewriter {

void ControlFlowRewriter::ReplaceBranchWithCmpAndJmp(
    HAssembler* as, const NormalizeBranch& normalized_branch) const {
  auto cond = normalized_branch.condition;
  HOperand* cmp_bool_cond = nullptr;
  HOpcode cmp_opcode;
  HOpcode jmp_opcode;

  auto branch_op = normalized_branch.branch_opcode;

  switch (branch_op) {
    // ------------------------------------------------------------------------
    // Integer comparison condition
    case HOpcode::kILt:
    case HOpcode::kILe:
    case HOpcode::kIGt:
    case HOpcode::kIGe:
      cmp_opcode = HOpcode::kCmpI;  // integer compare
      if (branch_op == HOpcode::kILt) {
        jmp_opcode = HOpcode::kJmpLt;
      } else if (branch_op == HOpcode::kILe) {
        jmp_opcode = HOpcode::kJmpLe;
      } else if (branch_op == HOpcode::kIGt) {
        jmp_opcode = HOpcode::kJmpGt;
      } else {
        jmp_opcode = HOpcode::kJmpGe;
      }
      break;

    case HOpcode::kIEq:
    case HOpcode::kINe:
    case HOpcode::kI64Eq:
    case HOpcode::kI64Ne: {
      // specialized against integer number
      if (auto maybe_imm = cond->Opr(0);
          maybe_imm->IsImm() &&
          ((maybe_imm->GetImm().IsInt32() &&
            maybe_imm->GetImm().Int32() == 0) ||
           (maybe_imm->GetImm().IsInt64() && maybe_imm->GetImm().Int64()))) {
        cmp_opcode = HOpcode::kCmpTest;
        cmp_bool_cond = cond->Opr(1);

        if (branch_op == HOpcode::kIEq || branch_op == HOpcode::kI64Eq) {
          jmp_opcode = HOpcode::kJmpZ;
        } else {
          jmp_opcode = HOpcode::kJmpNz;
        }
      } else {
        cmp_opcode = HOpcode::kCmpI;
        if (branch_op == HOpcode::kIEq || branch_op == HOpcode::kI64Eq) {
          jmp_opcode = HOpcode::kJmpEq;
        } else {
          jmp_opcode = HOpcode::kJmpNe;
        }
      }
    } break;

    // ------------------------------------------------------------------------
    // Box type testing, notes IsHeap and IsPrimitive is mutually exclusive
    case HOpcode::kBoxIsHeap:
      cmp_opcode = HOpcode::kCmpBoxIsPrimitive;
      jmp_opcode = HOpcode::kJmpBoxIsPrimitive;
      break;

    case HOpcode::kBoxIsPrimitive:
      cmp_opcode = HOpcode::kCmpBoxIsHeap;
      jmp_opcode = HOpcode::kJmpBoxIsHeap;
      break;

    case HOpcode::kBoxTagIsEq:
      cmp_opcode = HOpcode::kCmpBoxTagIs;
      jmp_opcode = HOpcode::kJmpEq;
      break;

    case HOpcode::kBoxTagIsNe:
      cmp_opcode = HOpcode::kCmpBoxTagIs;
      jmp_opcode = HOpcode::kJmpNe;
      break;

    case HOpcode::kVarRefListIsEmpty:
      cmp_opcode = HOpcode::kCmpVarRefListIsEmpty;
      jmp_opcode = HOpcode::kJmpZ;
      break;

    case HOpcode::kVarRefListIsNotEmpty:
      cmp_opcode = HOpcode::kCmpVarRefListIsEmpty;
      jmp_opcode = HOpcode::kJmpNz;
      break;

    default:
      UNREACHABLE() << "invalid LIR comparison condition sequences";
      break;
  }

  auto position = normalized_branch.branch;

  switch (cmp_opcode) {
    case HOpcode::kCmpTest:
      as->NewInstructionBefore(normalized_branch.condition->ir_node(),
                               cmp_opcode, cmp_bool_cond, position);

      break;

    case HOpcode::kCmpVarRefListIsEmpty:
      as->NewInstructionBefore(normalized_branch.condition->ir_node(),
                               cmp_opcode, position);
      break;

    case HOpcode::kCmpBoxIsHeap:
    case HOpcode::kCmpBoxIsPrimitive:
      as->NewInstructionBefore(normalized_branch.condition->ir_node(),
                               cmp_opcode, normalized_branch.condition->Opr(0),
                               position);
      break;

    default:
      CHECK(normalized_branch.condition->opr_size() == 2);
      as->NewInstructionBefore(normalized_branch.condition->ir_node(),
                               cmp_opcode, normalized_branch.condition->Opr(0),
                               normalized_branch.condition->Opr(1), position);
      break;
  }

  as->NewInstructionBefore(normalized_branch.branch->ir_node(), jmp_opcode,
                           normalized_branch.jump_label, position);

  // remove the branch node since it is been replaced with Cmp + Jmp
  normalized_branch.branch->Remove();
}

ControlFlowRewriter::RewriteResult ControlFlowRewriter::RewriteSingleUseBranch(
    HAssembler* as, HInstruction* ins) const {
  // Normalize the branch operation into BranchTrue like operation. Notes, the
  // opcode will be set up correctly (flip if needed when BranchFalse).
  auto normalized_branch = NormalizeBranch::Create(ins);

  ReplaceBranchWithCmpAndJmp(as, normalized_branch);

  // Remove the condition either, since we are the only user of the condition
  normalized_branch.condition->Remove();

  // node has been mutated
  return RewriteResult::kChanged;
}

/**
 * The floating point number, IEEE754, we will do a simple branch normalization,
 *
 * 1. For none equalism comparison, all normalize to > or >=
 *
 *    1.1 a < b => b > a
 *    1.2 a <= b => b >= a
 *
 * 2. For BranchTrue all normalized to BranchFalse,
 *
 *    BranchTrue(cond) => BranchFalse(!cond)
 *
 */

ControlFlowRewriter::RewriteResult ControlFlowRewriter::RewriteFloatBranch(
    HAssembler* as, HInstruction* ins) const {
  auto has_rewrite = false;
  // Check rule 2, ie convert all the BranchTrue to be BranchFalse
  auto condition = ins->Opr(0);

  // Perform rule 2, ie rewrite BranchTrue to be BranchFalse
  switch (ins->opcode()) {
    case HOpcode::kBranchTrue: {
      // perform *not* of the condition
      auto not_cond = NormalizeBranch::NotConditionBefore(
          as, condition, condition->instruction_checked());

      // insert a new branch false to be used to replace the old instruction
      auto new_ins = as->NewInstructionBefore(
          ins->ir_node(), HOpcode::kBranchFalse, not_cond, ins);

      // remove the old instruction pair, notes, order matters since we need
      // to make sure condition is removed when all its reference is gone, ie
      // the branch instruction
      ins->Remove();
      condition->instruction_checked()->Remove();

      ins = new_ins;
      condition = not_cond;

      has_rewrite = true;
    } break;

    default:
      break;
  }

  CHECK(ins->opcode() == HOpcode::kBranchFalse);

  // Perform rule 1
  switch (condition->instruction_checked()->opcode()) {
    case HOpcode::kFLt:
    case HOpcode::kFLe: {
      auto flip_cond = NormalizeBranch::FlipConditionBefore(
          as, condition, condition->instruction_checked());
      condition->Replace(flip_cond);
      condition->instruction_checked()->Remove();

      has_rewrite = true;
    } break;
    default:
      break;
  }

  return has_rewrite ? RewriteResult::kChanged : RewriteResult::kUnchanged;
}

ControlFlowRewriter::RewriteResult ControlFlowRewriter::Run(
    Quake*, HAssembler* as, HInstruction* ins) const {
  if (ins->IsBranch()) {
    auto cond = ins->Opr(0);
    if (cond->SingleUseBy(ins)) {
      if (NormalizeBranch::IsAllowedCondition(
              cond->instruction_checked()->opcode())) {
        return RewriteSingleUseBranch(as, ins);
      } else {
        // for floating point number, we do some normalization
        return RewriteFloatBranch(as, ins);
      }
    }
  }

  return RewriteResult::kUnchanged;
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::ControlFlowRewriter)
