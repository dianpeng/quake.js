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
  RewriteResult Run(Quake*, lir::HAssembler*,
                    lir::HInstruction*) const override;

  std::string GetName() const override { return "x64"; }

 private:
  // --------------------------------------------------------------------------
  // 1) I+* is resolved to * + I (I can only shows up at OP(1))
  // -------------------------------------------------------------------------
  RewriteResult ResolveImmOrder(lir::HInstruction*) const;

  RewriteResult ResolveIModMulOperand(lir::HInstruction*) const;
  RewriteResult ResolveQWordOperand(lir::HInstruction*) const;
  RewriteResult ResolveQWordMove(lir::HInstruction*) const;

 private:
  lir::HOperand* MaterializeQWordOperand(lir::HInstruction*,
                                         lir::HOperand*) const;

  mutable Quake* q_ = nullptr;
  mutable lir::HAssembler* as_ = nullptr;
  mutable lir::HInstruction* ins_ = nullptr;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_LIR_REWRITER_H_
