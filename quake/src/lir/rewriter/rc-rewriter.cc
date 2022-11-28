#include "lir/rewriter/rc-rewriter.h"

#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/assembler.h"

namespace quake {
namespace lir {
namespace rewriter {

void RcRewriter::RewriteRcDecMaybeDelete(HAssembler* as,
                                         HInstruction* ins) const {
  auto pos = ins;
  auto ir_node = ins->ir_node();
  auto target = ins->Opr(0);

  // 1) RcDec instruction
  auto rc_dec = as->NewInstructionBefore(ir_node, HOpcode::kRcDec, target, pos);

  // 2) JNZ to jump cross the RCall code
  auto jmp_label = as->NewLabel();
  auto jmp =
      as->NewInstructionAfter(ir_node, HOpcode::kJmpNz, jmp_label, rc_dec);

  // 3) Perform RCallVoid to free the GCPtr, notes the input operand(target) is
  //    GC pointer
  {
    // We need to mark the current region as slow block to allow RA to work
    auto slow_start =
        as->NewInstructionAfter(ir_node, HOpcode::kSlowBlockStart, jmp);

    auto free_index = as->NewBuiltinCallIndex(BuiltinFunction::kGCFree);
    auto free_call =
        as->NewInstructionAfter(ir_node, HOpcode::kRCallVoid, 2, slow_start);

    free_call->OprSet(0, free_index);
    free_call->OprSet(1, target);

    CHECK(free_call->VerifyOperand());
    free_call->PopulateOutput(as);

    auto slow_end =
        as->NewInstructionAfter(ir_node, HOpcode::kSlowBlockEnd, free_call);

    // 4) JUMP to here, ie just new a block do nothing as placeholder. Do not
    //    jump to the next instruction after the original RcDecMaybeDelete since
    //    that node may be deleted. Currently, the label's binded instruction
    //    does not support removal and tracking, until we refactor our LIR
    //    little bit to support it
    auto block = as->NewInstructionAfter(ir_node, HOpcode::kBlock, slow_end);

    jmp_label->GetLabel()->set_bind(block);  // jump to here
  }

  // Lastly, remove the original RcDecMaybeDelete
  ins->Remove();
}

RcRewriter::RewriteResult RcRewriter::Run(Quake* q, HAssembler* as,
                                          HInstruction* ins) const {
  if (ins->opcode() == HOpcode::kRcDecMaybeDelete) {
    RewriteRcDecMaybeDelete(as, ins);
    return RewriteResult::kChanged;
  }
  return RewriteResult::kUnchanged;
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::RcRewriter)
