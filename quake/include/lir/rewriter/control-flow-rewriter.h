#ifndef _QUAKE_LIR_CONTROL_FLOW_REWRITER_H_
#define _QUAKE_LIR_CONTROL_FLOW_REWRITER_H_
#include "base.h"

#include "lir/rewriter.h"
#include "lir/opcode.h"

namespace quake {
namespace lir {
class HAssembler;
class HInstruction;

namespace rewriter {

class NormalizeBranch;

// ----------------------------------------------------------------------------
// This pass will rewrite several jump(control-flow) node into its lower or
// faster representation.
//
// 1) BranchTrue/BranchFalse + Condition (single use) will be turned into
//    Cmp + Jump combination.
//
// 2) BranchTrue/BranchFalse + Condition (multiple use) will be turned into
//    CmpTrue/CmpFalse + Jmp. Notes since the condition is been used as
//    value for multiple positions, we cannot eliminate it really.
class ControlFlowRewriter : public Rewriter {
 public:
  using Rewriter::RewriteResult;

  RewriteResult Run(Quake*, HAssembler*, HInstruction*) const override;
  std::string GetName() const override { return "control-flow-rewriter"; }

 private:
  RewriteResult RewriteSingleUseBranch(HAssembler*, HInstruction*) const;

 private:
  void ReplaceBranchWithCmpAndJmp(HAssembler*, const NormalizeBranch&) const;
  RewriteResult RewriteFloatBranch(HAssembler* as, HInstruction* ins) const;
};

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_CONTROL_FLOW_REWRITER_H_
