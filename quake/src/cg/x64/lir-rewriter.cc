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
                                                  HInstruction* ins) {
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
    HInstruction* ins) {
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
  }

  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);

  if (opr0->IsImm() && !opr1->IsImm()) {
    if (is_commute) {
      ins->RemoveOperands();
      ins->OprSet(1, opr0);
      ins->OprSet(0, opr1);
      return RewriteResult::kChanged;
    } else {
      auto new_operand = as_->NewTemporary();

      as_->NewInstructionBefore(ins_->ir_node(), HOpcode::kMov, new_operand,
                                opr0, ins);

      ins->OprSet(0, new_operand);
      return RewriteResult::kChanged;
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
    HInstruction* ins) {
  if (ins->opcode() == HOpcode::kIDiv || ins->opcode() == HOpcode::kIMod ||
      ins->opcode() == HOpcode::kIMul) {
    auto opr1 = ins->Opr(1);
    if (opr1->IsImm()) {
      CHECK(opr1->GetImm().IsInt32());

      auto new_operand = as_->NewTemporary();
      as_->NewInstructionBefore(ins->ir_node(), HOpcode::kMovI32, new_operand,
                                opr1, ins);
      ins->OprSet(1, new_operand);
      return RewriteResult::kChanged;
    }
  }
  return RewriteResult::kUnchanged;
}

HOperand* X64LIRRewriter::MaterializeQWordOperand(HInstruction* instruction,
                                                  HOperand* qword_operand) {
  HOperand* new_operand = nullptr;

  CHECK(qword_operand->IsImm());
  CHECK(qword_operand->GetImm().IsQWord());

  switch (qword_operand->GetImm().type()) {
    case Immediate::kInt64:
      new_operand = as_->NewTemporary();
      as_->NewInstructionBefore(instruction->ir_node(), HOpcode::kMovI64,
                                new_operand, qword_operand, instruction);
      break;

    case Immediate::kFloat64:
      new_operand = as_->NewTemporary();
      as_->NewInstructionBefore(instruction->ir_node(), HOpcode::kMovF64,
                                new_operand, qword_operand, instruction);
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
//   OP any, r
//
// or
//   mov R, QWORD
//   OP r, any
// ----------------------------------------------------------------------------
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveQWordOperand(
    HInstruction* ins) {
  if (ins->IsBinary()) {
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
    HInstruction* ins) {
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
