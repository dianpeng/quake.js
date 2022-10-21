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
    case HOpcode::kIDiv:
    case HOpcode::kIMod:
    case HOpcode::kIMul:
      TRY(ResolveIDivModMulOperand(ins));
      break;

    case HOpcode::kMov:
      TRY(ResolveQWordMove(ins));
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
// rewrite
//   OP imm any
// to
//   OP any imm
//
// This is much more applicable on x64
// ----------------------------------------------------------------------------
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveImmOrder(
    HInstruction* ins) const {
  auto is_commute = false;
  switch (ins->opcode()) {
    case HOpcode::kIAdd:
    case HOpcode::kIMul:
    case HOpcode::kAnd:
    case HOpcode::kOr:
    case HOpcode::kXor:
      is_commute = true;
      break;

    default:
      is_commute = false;
      break;

    // don't bother
    case HOpcode::kPhiLoopI:
    case HOpcode::kPhiLoopF:
      return RewriteResult::kUnchanged;
  }

  if (ins->opr_size() == 2) {
    auto opr0 = ins->Opr(0);
    auto opr1 = ins->Opr(1);

    if (opr0->IsImm() && !opr1->IsImm()) {
      if (is_commute) {
        ins->RemoveOperands();
        ins->OprSet(1, opr0);
        ins->OprSet(0, opr1);
        return RewriteResult::kChanged;
      } else {
        auto mov = as_->NewInstructionBefore(ins_->ir_node(), HOpcode::kMov,
                                             opr0, ins);
        auto opr = mov->output();
        ins->OprSet(0, opr);
        return RewriteResult::kChanged;
      }
    }
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
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveIDivModMulOperand(
    HInstruction* ins) const {
  if (ins->opcode() == HOpcode::kIDiv || ins->opcode() == HOpcode::kIMod ||
      ins->opcode() == HOpcode::kIMul) {
    auto opr1 = ins->Opr(1);
    if (opr1->IsImm()) {
      CHECK(opr1->GetImm().IsInt32());
      as_->NewInstructionBefore(ins->ir_node(), HOpcode::kMovI32, opr1, ins);
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
  if (ins->IsBinary() && !ins->IsMove()) {
    auto opr0 = ins->Opr(0);
    auto opr1 = ins->Opr(1);
    HOperand* new_opr0 = nullptr;
    HOperand* new_opr1 = nullptr;

    if (opr0->IsImm() && opr0->GetImm().IsQWord()) {
      new_opr0 = MaterializeQWordOperand(ins, opr0);
    }
    if (opr1->IsImm() && opr1->GetImm().IsQWord()) {
      new_opr1 = MaterializeQWordOperand(ins, opr1);
    }

    if (new_opr0) ins->OprSet(0, new_opr0);
    if (new_opr1) ins->OprSet(1, new_opr1);

    if (new_opr0 || new_opr1) {
      return RewriteResult::kChanged;
    }
  }
  return RewriteResult::kUnchanged;
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

}  // namespace x64
}  // namespace cg
}  // namespace quake

QUAKE_LIR_REWRITER_ADD(quake::cg::x64::X64LIRRewriter)
