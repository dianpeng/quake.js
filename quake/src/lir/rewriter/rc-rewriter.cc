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
  auto type = ins->Opr(1);

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
        as->NewInstructionAfter(ir_node, HOpcode::kRCallVoid, 3, slow_start);

    free_call->OprSet(0, free_index);
    free_call->OprSet(1, target);
    free_call->OprSet(2, type);

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

void RcRewriter::RewriteRcDecMaybeDeleteBox(HAssembler* as,
                                            HInstruction* ins) const {
  auto pos = ins;
  auto target = ins->Opr(0);
  auto ir_node = ins->ir_node();
  HInstruction* last_block = nullptr;

  auto done_label = as->NewLabel();  // label point to done

  // 1) Generate instruction to perform check whether it is a heap object or not
  //    if it is then just perform rc drop otherwise do nothing
  auto ttag = as->NewInstructionBefore(ir_node, HOpcode::kGetTTag, target, pos);

  // check whether the ttag is a heap object value, otherwise do nothing
  {
    auto cmp = as->NewInstructionAfter(ir_node, HOpcode::kCmpI, ttag->output(),
                                       as->NewImmediate(0), ttag);

    pos = as->NewInstructionAfter(ir_node, HOpcode::kJmpGe, done_label, cmp);
  }

  // perform the rc drop operation
  {
    auto rc_dec =
        as->NewInstructionAfter(ir_node, HOpcode::kRcDec, target, pos);

    // 2) JNZ to jump cross the RCall code
    auto jmp =
        as->NewInstructionAfter(ir_node, HOpcode::kJmpNz, done_label, rc_dec);

    // 3) Perform RCallVoid to free the GCPtr, notes the input operand(target)
    //    is GC pointer
    {
      // We need to mark the current region as slow block to allow RA to work
      auto slow_start =
          as->NewInstructionAfter(ir_node, HOpcode::kSlowBlockStart, jmp);

      auto free_index = as->NewBuiltinCallIndex(BuiltinFunction::kGCFree);
      auto free_call =
          as->NewInstructionAfter(ir_node, HOpcode::kRCallVoid, 3, slow_start);

      free_call->OprSet(0, free_index);
      free_call->OprSet(1, target);
      free_call->OprSet(2, ttag->output());

      CHECK(free_call->VerifyOperand());
      free_call->PopulateOutput(as);

      auto slow_end =
          as->NewInstructionAfter(ir_node, HOpcode::kSlowBlockEnd, free_call);

      // 4) JUMP to here, ie just new a block do nothing as placeholder. Do not
      //    jump to the next instruction after the original RcDecMaybeDelete
      //    since that node may be deleted. Currently, the label's binded
      //    instruction does not support removal and tracking, until we refactor
      //    our LIR little bit to support it
      last_block = as->NewInstructionAfter(ir_node, HOpcode::kBlock, slow_end);
    }
  }

  done_label->GetLabel()->set_bind(last_block);  // jump to here

  // Lastly, remove the original RcDecMaybeDelete
  ins->Remove();
}

RcRewriter::RewriteResult RcRewriter::Run(Quake* q, HAssembler* as,
                                          HInstruction* ins) const {
  switch (ins->opcode()) {
    case HOpcode::kRcDecMaybeDelete:
      RewriteRcDecMaybeDelete(as, ins);
      return RewriteResult::kChanged;
    case HOpcode::kRcDecMaybeDeleteBox:
      RewriteRcDecMaybeDeleteBox(as, ins);
      return RewriteResult::kChanged;
    default:
      break;
  }
  return RewriteResult::kUnchanged;
}

}  // namespace rewriter
}  // namespace lir
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::lir::rewriter::RcRewriter)
