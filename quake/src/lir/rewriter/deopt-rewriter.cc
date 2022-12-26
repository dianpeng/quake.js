#include "lir/rewriter/deopt-rewriter.h"
#include "lir/rewriter/normalize-branch.h"

#include "lir/assembler.h"
#include "lir/operand.h"
#include "lir/instruction.h"

namespace quake {
namespace lir {
namespace rewriter {

DeoptRewriter::RewriteResult DeoptRewriter::Run(Quake* quake, HAssembler* as,
                                                HInstruction* ins) const {
  if (ins->IsDeopt()) {
    // We only try to rewrite DeoptIfTrue/DeoptIfFalse for now
    switch (ins->opcode()) {
      case HOpcode::kDeoptIfTrue:
      case HOpcode::kDeoptIfFalse: {
        auto cond = ins->Opr(0);
        if (cond->SingleUseBy(ins) &&
            NormalizeBranch::IsAllowedCondition(
                cond->instruction_checked()->opcode())) {
          CHECK(ins->HasExit());
          auto normalize_branch = NormalizeBranch::Create(ins);
          ReplaceBranchWithCmpAndJmp(as, normalize_branch, ins->exit());
          normalize_branch.condition->Remove();
          return RewriteResult::kChanged;
        }
      } break;
      default:
        break;
    }
  }
  return RewriteResult::kUnchanged;
}

void DeoptRewriter::ReplaceBranchWithCmpAndJmp(
    HAssembler* as, const NormalizeBranch& normalized_branch,
    ir::Checkpoint* ck) const {
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
        jmp_opcode = HOpcode::kDeoptLt;
      } else if (branch_op == HOpcode::kILe) {
        jmp_opcode = HOpcode::kDeoptLe;
      } else if (branch_op == HOpcode::kIGt) {
        jmp_opcode = HOpcode::kDeoptGt;
      } else {
        jmp_opcode = HOpcode::kDeoptGe;
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
          jmp_opcode = HOpcode::kDeoptZ;
        } else {
          jmp_opcode = HOpcode::kDeoptNz;
        }
      } else {
        cmp_opcode = HOpcode::kCmpI;
        if (branch_op == HOpcode::kIEq || branch_op == HOpcode::kI64Eq) {
          jmp_opcode = HOpcode::kDeoptEq;
        } else {
          jmp_opcode = HOpcode::kDeoptNe;
        }
      }
    } break;

    // ------------------------------------------------------------------------
    // Box type testing, notes IsHeap and IsPrimitive is mutually exclusive
    case HOpcode::kBoxIsHeap:
      cmp_opcode = HOpcode::kCmpBoxIsHeap;
      jmp_opcode = HOpcode::kDeoptNz;
      break;

    case HOpcode::kBoxIsPrimitive:
      cmp_opcode = HOpcode::kCmpBoxIsPrimitive;
      jmp_opcode = HOpcode::kDeoptNz;
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

  // deoptimizationXXX does not take a label
  auto jmp_ins = as->NewInstructionBefore(normalized_branch.branch->ir_node(),
                                          jmp_opcode, position);
  jmp_ins->set_exit(ck);

  // remove the branch node since it is been replaced with Cmp + Jmp
  normalized_branch.branch->Remove();
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::DeoptRewriter)
