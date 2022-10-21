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
};

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_NORMALIZE_BRANCH_H_

