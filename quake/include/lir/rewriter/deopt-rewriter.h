#ifndef _QUAKE_LIR_DEOPT_REWRITER_H_
#define _QUAKE_LIR_DEOPT_REWRITER_H_

#include "base.h"
#include "deopt.h"
#include "lir/rewriter.h"
#include "lir/opcode.h"

namespace quake {
namespace ir {
class Checkpoint;
}

namespace lir {
class HInstruction;
class HAssembler;

namespace rewriter {

class NormalizeBranch;

// Rewrite deoptimize related instruction to lower form.
class DeoptRewriter : public Rewriter {
 public:
  using Rewriter::RewriteResult;

  RewriteResult Run(Quake*, HAssembler*, HInstruction*) const override;
  std::string GetName() const override { return "deopt-rewriter"; }

 private:
  void ReplaceBranchWithCmpAndJmp(HAssembler*, const NormalizeBranch&,
                                  ir::Checkpoint*) const;
};

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_DEOPT_REWRITER_H_
