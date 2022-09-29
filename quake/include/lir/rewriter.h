#ifndef _QUAKE_LIR_REWRITER_H_
#define _QUAKE_LIR_REWRITER_H_
#include "base.h"
#include "lir/opcode.h"

namespace quake {
class Quake;

namespace lir {
class HAssembler;
class HInstruction;
class HOperand;

// Rewriter happens on the fly when HAssembler emit instruction sequences from
// the HIR instruction sequences. It is mostly been used to resolve arch related
// constraints. Like operand location, certain instruction's special requirement
// etc ...
class Rewriter {
 public:
  enum class RewriteResult : std::uint8_t { kUnchanged = 0, kChanged, kNext };

  virtual ~Rewriter() {}

  // Convert a HInstruction into lower representation or letting user to specify
  // certain operations accordingly
  virtual RewriteResult Run(Quake*, lir::HAssembler*, lir::HInstruction*) = 0;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REWRITER_H_
