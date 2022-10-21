#ifndef _QUAKE_LIR_REWRITER_NUMERIC_REWRITER_H_
#define _QUAKE_LIR_REWRITER_NUMERIC_REWRITER_H_
#include "base.h"
#include "lir/rewriter.h"

namespace quake {
namespace lir {

class HAssembler;
class HInstruction;

namespace rewriter {

class NumericRewriter : public Rewriter {
 public:
  using Rewriter::RewriteResult;

  RewriteResult Run(Quake*, HAssembler*, HInstruction*) const override;
  std::string GetName() const override { return "float-rewriter"; }

 private:
  void RewriteFMod(HAssembler*, HInstruction*) const;
  void RewriteFPow(HAssembler*, HInstruction*) const;
};

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

#endif
