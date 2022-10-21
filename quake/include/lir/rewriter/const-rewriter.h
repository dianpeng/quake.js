#ifndef _QUAKE_LIR_REWRITER_CONST_REWRITER_H_
#define _QUAKE_LIR_REWRITER_CONST_REWRITER_H_
#include "base.h"
#include "lir/rewriter.h"

namespace quake {
namespace ir {
class Node;
}  // namespace ir

namespace lir {

class HAssembler;
class HInstruction;
class Immediate;
class HOperand;

namespace rewriter {

class ConstRewriter : public Rewriter {
 public:
  using Rewriter::RewriteResult;

  RewriteResult Run(Quake*, HAssembler*, HInstruction*) const override;
  std::string GetName() const override { return "phi-rewriter"; }

 private:
  HOperand* PromoteImmediate(HAssembler*, ir::Node*, HInstruction*,
                             const Immediate&) const;
  RewriteResult RewriteRCall(HAssembler*, HInstruction*) const;
  RewriteResult RewritePhi(HAssembler*, HInstruction*) const;
};

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REWRITER_CONST_REWRITER_H_
