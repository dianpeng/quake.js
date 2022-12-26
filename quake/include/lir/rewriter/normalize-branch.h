#ifndef _QUAKE_LIR_NORMALIZE_BRANCH_H_
#define _QUAKE_LIR_NORMALIZE_BRANCH_H_
#include "base.h"
#include "lir/opcode.h"

namespace quake {
namespace lir {
class HInstruction;
class HAssembler;
class HOperand;

namespace rewriter {

// Used for control-flow/deopt rewriter as an aux helper object
struct NormalizeBranch {
  HInstruction* condition;
  HInstruction* branch;
  HOpcode branch_opcode;
  HOperand* jump_label;

 public:
  static NormalizeBranch Create(HInstruction*);
  static HOpcode MaybeFlipOpcode(HOpcode, bool);
  static bool IsAllowedCondition(HOpcode);

  // NotCondition operation will return an condition that is like perform the
  // not logic operation on the original specifid operation
  static HOperand* NotConditionBefore(HAssembler*, HOperand*, HInstruction*);
  static HOperand* NotConditionAfter(HAssembler*, HOperand*, HInstruction*);

  // FlipOperation operation will return an flipped that is like perform the
  // swap its operands position. ie a > b => b <= a etc ... Notes, both int
  // and float support such operation.
  static HOperand* FlipConditionBefore(HAssembler*, HOperand*, HInstruction*);
  static HOperand* FlipConditionAfter(HAssembler*, HOperand*, HInstruction*);

 private:
  struct Condition {
    HOpcode opcode;
    HOperand* lhs;
    HOperand* rhs;
  };

  static Condition NotCondition(HAssembler*, HOperand*);
  static Condition FlipCondition(HAssembler*, HOperand*);
};

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_NORMALIZE_BRANCH_H_

