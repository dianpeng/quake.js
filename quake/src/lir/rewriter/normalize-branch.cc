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

  br.condition = branch->Opr(0)->instruction();
  CHECK(br.condition);

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

      // real comparison
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
