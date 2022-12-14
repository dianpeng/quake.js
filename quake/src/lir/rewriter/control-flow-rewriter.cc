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
    // Real comparison condition
    case HOpcode::kFLt:
    case HOpcode::kFLe:
    case HOpcode::kFGt:
    case HOpcode::kFGe:
    case HOpcode::kFEq:
    case HOpcode::kFNe:
      cmp_opcode = HOpcode::kCmpF;
      if (branch_op == HOpcode::kFLt) {
        jmp_opcode = HOpcode::kJmpLt;
      } else if (branch_op == HOpcode::kFLe) {
        jmp_opcode = HOpcode::kJmpLe;
      } else if (branch_op == HOpcode::kFGt) {
        jmp_opcode = HOpcode::kJmpGt;
      } else if (branch_op == HOpcode::kFGe) {
        jmp_opcode = HOpcode::kJmpGe;
      } else if (branch_op == HOpcode::kFEq) {
        jmp_opcode = HOpcode::kJmpEq;
      } else {
        jmp_opcode = HOpcode::kJmpNe;
      }
      break;

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

ControlFlowRewriter::RewriteResult ControlFlowRewriter::Run(
    Quake*, HAssembler* as, HInstruction* ins) const {
  if (ins->IsBranch()) {
    auto cond = ins->Opr(0);
    if (cond->SingleUseBy(ins)) {
      return RewriteSingleUseBranch(as, ins);
    }
  }

  return RewriteResult::kUnchanged;
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::ControlFlowRewriter)
