#ifndef _QUAKE_LIR_REWRITER_RC_REWRITER_H_
#define _QUAKE_LIR_REWRITER_RC_REWRITER_H_
#include "base.h"
#include "lir/rewriter.h"

// RC rewriter
//
// 1) Rewrite RcDecMaybeDelete =>
//   1.1) RcDec     (which set the zero flag)
//   1.2) jnz #l.4  (if rc is not zero)
//   1.3) RCallVoid (#Delete, GCPtr)
//   1.4) -----
//
// The above sequences are better for later phase of LIR

namespace quake {
namespace lir {

class HAssembler;
class HInstruction;

namespace rewriter {

class RcRewriter : public Rewriter {
 public:
  using Rewriter::RewriteResult;

  RewriteResult Run(Quake*, HAssembler*, HInstruction*) const override;
  std::string GetName() const override { return "rc-rewriter"; }

 private:
  void RewriteRcDecMaybeDelete(HAssembler*, HInstruction*) const;
  void RewriteRcDecMaybeDeleteBox(HAssembler* as, HInstruction* ins) const;
  void DoRewriteRcDecMaybe(HAssembler*, HInstruction*, HOpcode) const;
};

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REWRITER_RC_REWRITER_H_
