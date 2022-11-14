#include "lir/rewriter/numeric-rewriter.h"

#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/assembler.h"

namespace quake {
namespace lir {
namespace rewriter {

NumericRewriter::RewriteResult NumericRewriter::Run(Quake* q, HAssembler* as,
                                                    HInstruction* ins) const {
  switch (ins->opcode()) {
    // Currently, we do not use this to convert fmod to ::fmod call, but we
    // directly yield during the code generation, which seems to be way
    // faster than calling out to a function
    case HOpcode::kFMod:
      if (!q->IsJitFlagOn(Quake::kJitFModFastCall)) {
        RewriteFMod(as, ins);
        return RewriteResult::kChanged;
      } else {
        break;
      }

    case HOpcode::kFPow:
      RewriteFPow(as, ins);
      return RewriteResult::kChanged;

    default:
      break;
  }

  return RewriteResult::kUnchanged;
}

// fmod a, b => rcall_f64 (:fmod) a, b
void NumericRewriter::RewriteFMod(HAssembler* as, HInstruction* ins) const {
  CHECK(ins->opr_size() == 2);

  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto cidx = as->NewBuiltinCallIndex(BuiltinFunction::kFMod);
  auto call =
      as->NewInstructionBefore(ins->ir_node(), HOpcode::kRCallF64, 3, ins);

  call->OprSet(0, cidx);
  call->OprSet(1, opr0);
  call->OprSet(2, opr1);

  ins->Replace(call);
  ins->Remove();

  CHECK(BuiltinFunctionInstructionVerify(call));
}

// fpow a, b => rcall_f64 (:fpow) a, b
void NumericRewriter::RewriteFPow(HAssembler* as, HInstruction* ins) const {
  CHECK(ins->opr_size() == 2);

  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto cidx = as->NewBuiltinCallIndex(BuiltinFunction::kFPow);
  auto call =
      as->NewInstructionBefore(ins->ir_node(), HOpcode::kRCallF64, 3, ins);

  call->OprSet(0, cidx);
  call->OprSet(1, opr0);
  call->OprSet(2, opr1);

  ins->Replace(call);
  ins->Remove();

  CHECK(BuiltinFunctionInstructionVerify(call));
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::NumericRewriter)
