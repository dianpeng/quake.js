#ifndef _QUAKE_CG_X64_LIR_REWRITER_H_
#define _QUAKE_CG_X64_LIR_REWRIETR_H_
#include "base.h"

#include "lir/rewriter.h"

namespace quake {
class Quake;

namespace cg {
namespace x64 {

// ---------------------------------------------------------------------------
// Try to resolve arch constraint when translating IR to LIR. This rewriter is
// executed on the fly when the HAssembler emit instruction into the pipeline
// Notes. NewInstructionXXX will not invoke the Rewriter.
// ---------------------------------------------------------------------------
class X64LIRRewriter : public lir::Rewriter {
 public:
  using RewriteResult = lir::Rewriter::RewriteResult;

  // Invoked on every instruction
  RewriteResult Run(Quake*, lir::HAssembler*, lir::HInstruction*) override;

 private:
  // --------------------------------------------------------------------------
  // 1) I+* is resolved to * + I (I can only shows up at OP(1))
  // -------------------------------------------------------------------------
  RewriteResult ResolveImmOrder(lir::HInstruction*);

  RewriteResult ResolveIDivModMulOperand(lir::HInstruction*);
  RewriteResult ResolveQWordOperand(lir::HInstruction*);
  RewriteResult ResolveQWordMove(lir::HInstruction*);

 private:
  lir::HOperand* MaterializeQWordOperand(lir::HInstruction*, lir::HOperand*);

  Quake* q_ = nullptr;
  lir::HAssembler* as_ = nullptr;
  lir::HInstruction* ins_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(X64LIRRewriter);
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_LIR_REWRITER_H_
