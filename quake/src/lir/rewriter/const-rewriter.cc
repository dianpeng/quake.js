#include "lir/rewriter/const-rewriter.h"
#include "lir/operand.h"
#include "lir/instruction.h"
#include "lir/assembler.h"

namespace quake {
namespace lir {
namespace rewriter {

ConstRewriter::RewriteResult ConstRewriter::Run(Quake* q, HAssembler* as,
                                                HInstruction* ins) const {
  if (ins->IsPhi()) {
    return RewritePhi(as, ins);
  }

  if (ins->IsRCall()) {
    return RewriteRCall(as, ins);
  }

  return RewriteResult::kUnchanged;
}

ConstRewriter::RewriteResult ConstRewriter::RewriteRCall(
    HAssembler* as, HInstruction* ins) const {
  CHECK(ins->IsRCall());

  auto pos = ins;
  auto changed = false;

  // rcall should skip the first parameter, which is an immediate to indicate
  // builtin function index
  for (int i = 1; i < ins->opr_size(); i++) {
    auto param = ins->Opr(i);
    if (param->IsImm()) {
      auto new_param =
          PromoteImmediate(as, ins->ir_node(), pos, param->GetImm());

      ins->OprSet(i, new_param);
      changed = true;
    }
  }

  return changed ? RewriteResult::kChanged : RewriteResult::kUnchanged;
}

ConstRewriter::RewriteResult ConstRewriter::RewritePhi(
    HAssembler* as, HInstruction* ins) const {
  auto op0 = ins->Opr(0);
  auto op1 = ins->Opr(1);
  auto changed = false;

  HInstruction* left_branch = nullptr;   // insert before this
  HInstruction* right_branch = nullptr;  // insert before this

  switch (ins->opcode()) {
    case HOpcode::kPhiLoopI:
    case HOpcode::kPhiLoopF:
      left_branch = as->bb0_end();
      right_branch = as->loop_back();  // end block
      break;

    case HOpcode::kPhiBranchI:
    case HOpcode::kPhiBranchF:
    case HOpcode::kPhiBranchBox:
      left_branch = ins->Opr(2)->GetLabel()->bind();
      right_branch = ins->Opr(3)->GetLabel()->bind();
      break;

    default:
      UNREACHABLE();
      break;
  }

  CHECK(left_branch);
  CHECK(right_branch);

  if (op0->IsImm()) {
    auto new_operand =
        PromoteImmediate(as, ins->ir_node(), left_branch, op0->GetImm());

    ins->OprSet(0, new_operand);
    changed = true;
  }

  if (op1->IsImm()) {
    auto new_operand =
        PromoteImmediate(as, ins->ir_node(), right_branch, op1->GetImm());

    ins->OprSet(1, new_operand);
    changed = true;
  }

  return changed ? RewriteResult::kChanged : RewriteResult::kUnchanged;
}

HOperand* ConstRewriter::PromoteImmediate(HAssembler* as, ir::Node* origin,
                                          HInstruction* after,
                                          const Immediate& imm) const {
  HInstruction* out = nullptr;
  HOperand* opr = as->NewImmediate(imm);

  switch (imm.type()) {
    case Immediate::kInt32:
      out = as->NewInstructionBefore(origin, HOpcode::kMovI32, opr, after);
      break;

    case Immediate::kInt64:
      // currently we just do not have INT64 representation, immediate::64 is
      // simply a B64. Notes, underlying I64 and B64 are just the same
      out = as->NewInstructionBefore(origin, HOpcode::kMovB64, opr, after);
      break;

    case Immediate::kFloat64:
      out = as->NewInstructionBefore(origin, HOpcode::kMovF64, opr, after);
      break;

    default:
      UNREACHABLE();
      return nullptr;
  }

  return out->output();
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::ConstRewriter)
