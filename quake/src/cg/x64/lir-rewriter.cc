#include "cg/x64/lir-rewriter.h"

#include "lir/assembler.h"
#include "lir/operand.h"
#include "lir/opcode.h"
#include "lir/instruction.h"

namespace quake {
namespace cg {
namespace x64 {

using namespace quake::lir;

X64LIRRewriter::RewriteResult X64LIRRewriter::Run(Quake* q, HAssembler* as,
                                                  HInstruction* ins) const {
  q_ = q;
  as_ = as;
  ins_ = ins;

#define TRY(X) \
  if (auto r = X; r != RewriteResult::kUnchanged) return r;

  switch (ins->opcode()) {
    case HOpcode::kIMod:
    case HOpcode::kIMul:
      TRY(ResolveIModMulOperand(ins));
      break;

    case HOpcode::kMov:
      TRY(ResolveQWordMove(ins));
      break;

    case HOpcode::kCmpF:
      TRY(ResolveCmpF(ins));
      break;

    default:
      break;
  }

  TRY(ResolveImmOrder(ins));
  TRY(ResolveQWordOperand(ins));

#undef TRY  // TRY

  return RewriteResult::kUnchanged;
}

// ----------------------------------------------------------------------------
// Floating Point Branch Rewrite
//
//   The FP is kind of complicated to handle on X64, due to the NaN. Bascially,
//   the normal ucomisd/comisd will set any comparison that involves NaN to be
//   "unordered", ie EFlags' parity bit will be set to reflect so. Therefore,
//   special care needs to be handled when dealing with floating point compare,
//   you cannot just simply perform jump afterwards.
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// rewrite
//   OP imm/mem any
// to
//   OP R any
//
// This is much more applicable on x64
// ----------------------------------------------------------------------------
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveImmOrder(
    HInstruction* ins) const {
  auto is_commute = false;
  switch (ins->opcode()) {
    case HOpcode::kIAdd:
    case HOpcode::kGIAdd:
    case HOpcode::kIMul:
    case HOpcode::kGIMul:
    case HOpcode::kAnd:
    case HOpcode::kOr:
    case HOpcode::kXor:
      is_commute = true;
      break;

    case HOpcode::kFAdd:

    case HOpcode::kISub:
    case HOpcode::kGISub:
    case HOpcode::kFSub:

    case HOpcode::kFMul:

    case HOpcode::kFDiv:
    case HOpcode::kIMod:
    case HOpcode::kFMod:

    case HOpcode::kFPow:

    case HOpcode::kShlConst:
    case HOpcode::kGShrConst:
    case HOpcode::kSarConst:

    case HOpcode::kShl:
    case HOpcode::kGShr:
    case HOpcode::kSar:

    case HOpcode::kILt:
    case HOpcode::kILe:

    case HOpcode::kIGt:
    case HOpcode::kIGe:

    case HOpcode::kIEq:
    case HOpcode::kINe:

    case HOpcode::kFLt:
    case HOpcode::kFLe:

    case HOpcode::kFGt:
    case HOpcode::kFGe:

    case HOpcode::kFEq:
    case HOpcode::kFNe:

    case HOpcode::kCmpI:
    case HOpcode::kCmpF:
    case HOpcode::kCmpTest:
      break;

    // don't bother
    default:
      return RewriteResult::kUnchanged;
  }

  CHECK(ins->opr_size() == 2);

  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);

  auto gen_move = [&](HInstruction* ins, HOperand* opr0) -> HInstruction* {
    HInstruction* mov = nullptr;
    switch (opr0->GetImm().type()) {
      case Immediate::kInt32:
        mov = as_->NewInstructionBefore(ins->ir_node(), HOpcode::kMovI32, opr0,
                                        ins);
        break;

      case Immediate::kInt64:
        mov = as_->NewInstructionBefore(ins->ir_node(), HOpcode::kMovI64, opr0,
                                        ins);
        break;

      case Immediate::kFloat64:
        mov = as_->NewInstructionBefore(ins->ir_node(), HOpcode::kMovF64, opr0,
                                        ins);
        break;

      default:
        UNREACHABLE();
        break;
    }

    return mov;
  };

  auto swap_operands = [&](HInstruction* ins) {
    ins->RemoveOperands();
    ins->OprSet(1, opr0);
    ins->OprSet(0, opr1);
  };

  if (opr0->IsImm()) {
    if (opr1->IsRegister() && is_commute) {
      swap_operands(ins);
    } else {
      ins->OprSet(0, gen_move(ins, opr0)->output());
    }
    return RewriteResult::kChanged;
  }

  return RewriteResult::kUnchanged;
}

// ----------------------------------------------------------------------------
// rewrite
//   div any imm
// to
//   mov r0, imm
//   div any, r0
//
// Notes on x64, div does not support immediate number as divsor
// ----------------------------------------------------------------------------
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveIModMulOperand(
    HInstruction* ins) const {
  if (ins->opcode() == HOpcode::kIMod || ins->opcode() == HOpcode::kIMul) {
    auto opr1 = ins->Opr(1);
    if (opr1->IsImm()) {
      CHECK(opr1->GetImm().IsInt32());
      auto mov = as_->NewInstructionBefore(ins->ir_node(), HOpcode::kMovI32,
                                           opr1, ins);
      ins->OprSet(1, mov->output());
      return RewriteResult::kChanged;
    }
  }
  return RewriteResult::kUnchanged;
}

HOperand* X64LIRRewriter::MaterializeQWordOperand(
    HInstruction* instruction, HOperand* qword_operand) const {
  HOperand* new_operand = nullptr;

  CHECK(qword_operand->IsImm());
  CHECK(qword_operand->GetImm().IsQWord());

  switch (qword_operand->GetImm().type()) {
    case Immediate::kInt64:
      new_operand =
          as_->NewInstructionBefore(instruction->ir_node(), HOpcode::kMovI64,
                                    qword_operand, instruction)
              ->output();
      break;

    case Immediate::kFloat64:
      new_operand =
          as_->NewInstructionBefore(instruction->ir_node(), HOpcode::kMovF64,
                                    qword_operand, instruction)
              ->output();
      break;

    default:
      UNREACHABLE();
      return nullptr;
  }

  CHECK(new_operand);
  return new_operand;
}

// ----------------------------------------------------------------------------
// rewrite
//   OP any, QWORD
// or
//   OP QWORD, any
//
// to
//   mov R, QWORD
//   OP any, R
//
// or
//   mov R, QWORD
//   OP R, any
// ----------------------------------------------------------------------------
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveQWordOperand(
    HInstruction* ins) const {
  switch (ins->opcode()) {
    case HOpcode::kLoadStringUnbox:
    case HOpcode::kEnterJitTrace:
      return RewriteResult::kUnchanged;
    default:
      if (ins->IsMove()) return RewriteResult::kUnchanged;
      break;
  }

  auto has_change = false;
  for (auto i = 0; i < ins->opr_size(); i++) {
    auto opr = ins->Opr(i);
    CHECK(opr) << ins->Print();
    if (opr->IsImm() && opr->GetImm().IsQWord()) {
      if (auto new_opr = MaterializeQWordOperand(ins, opr); new_opr) {
        has_change = true;
        ins->OprSet(i, new_opr);
      }
    }
  }

  return has_change ? RewriteResult::kChanged : RewriteResult::kUnchanged;
}

// ----------------------------------------------------------------------------
// rewrite
//   mov [mem], QWORD
// to
//   movI64/F64/B64 [mem], QWORD # this will internally rewrite by the RAX
// ----------------------------------------------------------------------------
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveQWordMove(
    HInstruction* ins) const {
  if (ins->opcode() == HOpcode::kMov) {
    auto opr0 = ins->Opr(0);
    auto opr1 = ins->Opr(1);

    if (opr0->IsMemory() && (opr1->IsImm() && opr1->GetImm().IsQWord())) {
      switch (opr1->GetImm().type()) {
        case Immediate::kInt64:
          ins_->set_opcode(HOpcode::kMovI64);
          return RewriteResult::kChanged;

        case Immediate::kFloat64:
          ins_->set_opcode(HOpcode::kMovF64);
          return RewriteResult::kChanged;

        default:
          UNREACHABLE();
          break;
      }
    }
  }
  return RewriteResult::kUnchanged;
}

// ----------------------------------------------------------------------------
// rewrite
//   cmpf ANY, ANY
//   jl/jle/jg/jge LBL
// to
//   cmp ANY, ANY
//   jb/jbe/ja/jae LBL
// ----------------------------------------------------------------------------
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveCmpF(
    HInstruction* ins) const {
  CHECK(ins->opcode() == HOpcode::kCmpF);
  auto next_instruction = ins->next();

  switch (next_instruction->opcode()) {
    case HOpcode::kJmpLe:
      next_instruction->set_opcode(HOpcode::kJmpBe);
      break;
    case HOpcode::kJmpLt:
      next_instruction->set_opcode(HOpcode::kJmpBt);
      break;

    case HOpcode::kJmpGe:
      next_instruction->set_opcode(HOpcode::kJmpAe);
      break;
    case HOpcode::kJmpGt:
      next_instruction->set_opcode(HOpcode::kJmpAt);
      break;

    case HOpcode::kDeoptLe:
      next_instruction->set_opcode(HOpcode::kDeoptBe);
      break;
    case HOpcode::kDeoptLt:
      next_instruction->set_opcode(HOpcode::kDeoptBt);
      break;

    case HOpcode::kDeoptGe:
      next_instruction->set_opcode(HOpcode::kDeoptAe);
      break;
    case HOpcode::kDeoptGt:
      next_instruction->set_opcode(HOpcode::kDeoptAt);
      break;

    default:
      return RewriteResult::kUnchanged;
  }

  return RewriteResult::kChanged;
}

}  // namespace x64
}  // namespace cg
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::cg::x64::X64LIRRewriter)
