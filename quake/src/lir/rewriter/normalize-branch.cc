#include "lir/rewriter/normalize-branch.h"
#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/assembler.h"

namespace quake {
namespace lir {
namespace rewriter {

NormalizeBranch NormalizeBranch::Create(HInstruction* branch) {
  CHECK(branch->IsBranch() || branch->IsConditionalBranchDeopt());

  NormalizeBranch br;
  bool is_true_branch;

  switch (branch->opcode()) {
    case HOpcode::kBranchTrue:
    case HOpcode::kDeoptIfTrue:
      is_true_branch = true;
      break;
    default:
      is_true_branch = false;
      break;
  }

  br.condition = branch->Opr(0)->instruction_checked();
  br.branch = branch;

  br.branch_opcode = MaybeFlipOpcode(br.condition->opcode(), is_true_branch);
  if (branch->IsBranch()) {
    br.jump_label = branch->Opr(1);
    CHECK(br.jump_label->IsLabel());
  } else {
    br.jump_label = nullptr;
  }

  return br;
}

NormalizeBranch::Condition NormalizeBranch::NotCondition(HAssembler* as,
                                                         HOperand* opr) {
  CHECK(opr->IsCondition());

  auto lhs = opr->instruction_checked()->Opr(0);
  auto rhs = opr->instruction_checked()->Opr(1);

  switch (opr->instruction_checked()->opcode()) {
    case HOpcode::kILt:
      return Condition{HOpcode::kIGe, lhs, rhs};

    case HOpcode::kILe:
      return Condition{HOpcode::kIGt, lhs, rhs};

    case HOpcode::kIGt:
      return Condition{HOpcode::kILe, lhs, rhs};

    case HOpcode::kIGe:
      return Condition{HOpcode::kILt, lhs, rhs};

    case HOpcode::kIEq:
      return Condition{HOpcode::kINe, lhs, rhs};

    case HOpcode::kI64Eq:
      return Condition{HOpcode::kI64Ne, lhs, rhs};

    case HOpcode::kINe:
      return Condition{HOpcode::kIEq, lhs, rhs};

    case HOpcode::kI64Ne:
      return Condition{HOpcode::kI64Eq, lhs, rhs};

    case HOpcode::kFLt:
      return Condition{HOpcode::kFGe, lhs, rhs};

    case HOpcode::kFLe:
      return Condition{HOpcode::kFGt, lhs, rhs};

    case HOpcode::kFGt:
      return Condition{HOpcode::kFLe, lhs, rhs};

    case HOpcode::kFGe:
      return Condition{HOpcode::kFLt, lhs, rhs};

    case HOpcode::kFEq:
      return Condition{HOpcode::kFNe, lhs, rhs};

    case HOpcode::kFNe:
      return Condition{HOpcode::kFEq, lhs, rhs};

    default:
      UNREACHABLE();
      return Condition{};
  }
}

NormalizeBranch::Condition NormalizeBranch::FlipCondition(HAssembler* as,
                                                          HOperand* opr) {
  CHECK(opr->IsCondition());

  auto lhs = opr->instruction_checked()->Opr(0);
  auto rhs = opr->instruction_checked()->Opr(1);

  switch (opr->instruction_checked()->opcode()) {
    case HOpcode::kILt:
      return Condition{HOpcode::kIGe, rhs, lhs};

    case HOpcode::kILe:
      return Condition{HOpcode::kIGt, rhs, lhs};

    case HOpcode::kIGt:
      return Condition{HOpcode::kILe, rhs, lhs};

    case HOpcode::kIGe:
      return Condition{HOpcode::kILt, rhs, lhs};

    case HOpcode::kIEq:
      return Condition{HOpcode::kINe, lhs, rhs};

    case HOpcode::kI64Eq:
      return Condition{HOpcode::kI64Ne, lhs, rhs};

    case HOpcode::kINe:
      return Condition{HOpcode::kIEq, lhs, rhs};

    case HOpcode::kI64Ne:
      return Condition{HOpcode::kI64Eq, lhs, rhs};

    case HOpcode::kFLt:
      return Condition{HOpcode::kFGe, rhs, lhs};

    case HOpcode::kFLe:
      return Condition{HOpcode::kFGt, rhs, lhs};

    case HOpcode::kFGt:
      return Condition{HOpcode::kFLe, rhs, lhs};

    case HOpcode::kFGe:
      return Condition{HOpcode::kFLt, rhs, lhs};

    case HOpcode::kFEq:
      return Condition{HOpcode::kFNe, lhs, rhs};

    case HOpcode::kFNe:
      return Condition{HOpcode::kFEq, lhs, rhs};

    default:
      UNREACHABLE();
      return Condition{};
  }
}

HOperand* NormalizeBranch::NotConditionBefore(HAssembler* as, HOperand* opr,
                                              HInstruction* pos) {
  auto c = NotCondition(as, opr);
  auto ins = as->NewInstructionBefore(opr->instruction_checked()->ir_node(),
                                      c.opcode, c.lhs, c.rhs, pos);
  return ins->output();
}

HOperand* NormalizeBranch::NotConditionAfter(HAssembler* as, HOperand* opr,
                                             HInstruction* pos) {
  auto c = NotCondition(as, opr);
  auto ins = as->NewInstructionAfter(opr->instruction_checked()->ir_node(),
                                     c.opcode, c.lhs, c.rhs, pos);
  return ins->output();
}

HOperand* NormalizeBranch::FlipConditionBefore(HAssembler* as, HOperand* opr,
                                               HInstruction* pos) {
  auto c = FlipCondition(as, opr);
  auto ins = as->NewInstructionBefore(opr->instruction_checked()->ir_node(),
                                      c.opcode, c.lhs, c.rhs, pos);
  return ins->output();
}

HOperand* NormalizeBranch::FlipConditionAfter(HAssembler* as, HOperand* opr,
                                              HInstruction* pos) {
  auto c = FlipCondition(as, opr);
  auto ins = as->NewInstructionAfter(opr->instruction_checked()->ir_node(),
                                     c.opcode, c.lhs, c.rhs, pos);
  return ins->output();
}

bool NormalizeBranch::IsAllowedCondition(HOpcode op) {
  switch (op) {
    case HOpcode::kILt:
    case HOpcode::kILe:
    case HOpcode::kIGt:
    case HOpcode::kIGe:
    case HOpcode::kIEq:
    case HOpcode::kINe:
    case HOpcode::kI64Eq:
    case HOpcode::kI64Ne:
    case HOpcode::kBoxIsHeap:
    case HOpcode::kBoxIsPrimitive:
      return true;
    default:
      return false;
  }
}

HOpcode NormalizeBranch::MaybeFlipOpcode(HOpcode op, bool branch_true) {
  if (branch_true) {
    return op;
  } else {
    switch (op) {
      // integer comparison
      case HOpcode::kILt:
        return HOpcode::kIGe;
      case HOpcode::kILe:
        return HOpcode::kIGt;

      case HOpcode::kIGt:
        return HOpcode::kILe;
      case HOpcode::kIGe:
        return HOpcode::kILt;

      case HOpcode::kIEq:
        return HOpcode::kINe;

      case HOpcode::kINe:
        return HOpcode::kIEq;

      case HOpcode::kI64Eq:
        return HOpcode::kI64Ne;

      case HOpcode::kI64Ne:
        return HOpcode::kI64Eq;

      // ----------------------------------------------------------------------
      // We do not support real condition since they are very specific on
      // different platform, mostly because of the NaN. For example, on X64
      // it requires extra care to be taken when you want the NaN to be
      // correctly handled
      case HOpcode::kFLt:
        return HOpcode::kFGe;
      case HOpcode::kFLe:
        return HOpcode::kFGt;
      case HOpcode::kFGt:
        return HOpcode::kFLe;
      case HOpcode::kFGe:
        return HOpcode::kFLt;
      case HOpcode::kFEq:
        return HOpcode::kFNe;
      case HOpcode::kFNe:
        return HOpcode::kFEq;

      // is_primitive/is_heap
      case HOpcode::kBoxIsHeap:
        return HOpcode::kBoxIsPrimitive;

      case HOpcode::kBoxIsPrimitive:
        return HOpcode::kBoxIsHeap;

      default:
        UNREACHABLE();
        return HOpcode::kSentry;
    }
  }
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake
