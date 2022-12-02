#include "lir/rewriter/atom-rewriter.h"
#include "lir/operand.h"
#include "lir/instruction.h"
#include "lir/assembler.h"

namespace quake {
namespace lir {
namespace rewriter {

AtomRewriter::RewriteResult AtomRewriter::RewriteGAtom(
    HAssembler* as, HInstruction* ins) const {
  auto opr = ins->Opr(0);
  if (opr->IsImm()) {
    CHECK(opr->GetImm().IsInt32());
    auto value = opr->GetImm().UInt32();
    if (value == JS_ATOM_NULL) {
      // insert an unconditional deoptimization instruction before
      as->NewInstructionBefore(ins->ir_node(), HOpcode::kDeopt, ins);

      // dummy atom number, doesn't matter, since we will deoptimize before
      // the operation are done
      auto int_zero = as->NewInstructionBefore(ins->ir_node(), HOpcode::kMovI32,
                                               as->NewImmediate(0), ins);

      ins->Replace(int_zero);
      ins->Remove();
    } else {
      // replace the original GAtom instruction to be a immediate number
      ins->Replace(as->NewImmediate(value));
      ins->Remove();
    }

    return RewriteResult::kChanged;
  }

  return RewriteResult::kUnchanged;
}

AtomRewriter::RewriteResult AtomRewriter::Run(Quake* quake, HAssembler* as,
                                              HInstruction* ins) const {
  if (ins->opcode() == HOpcode::kGAtom) {
    return RewriteGAtom(as, ins);
  } else {
    return RewriteResult::kUnchanged;
  }
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::AtomRewriter)
