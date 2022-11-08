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

  // FIXME(dpeng): Make any none materializable immediate number been hidden
  //   from the normal operand, ie do not use Immediate to represent this
  //   and this will make them participate into the normal optimization
  if (ins->IsRCall()) return RewriteResult::kUnchanged;

  auto generate_move = [&](HInstruction* ins, HOperand* opr0) -> HInstruction* {
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

  if (ins->opr_size() == 2) {
    auto opr0 = ins->Opr(0);
    auto opr1 = ins->Opr(1);

    if (opr0->IsImm()) {
      if (!opr1->IsImm()) {
        if (is_commute) {
          ins->RemoveOperands();
          ins->OprSet(1, opr0);
          ins->OprSet(0, opr1);
          return RewriteResult::kChanged;
        } else {
          auto mov = generate_move(ins, opr0);
          auto opr = mov->output();
          ins->OprSet(0, opr);
          return RewriteResult::kChanged;
        }
      } else {
        // both operand are imm, which should be reduced by fold engine, but
        // we may want to generate the code for testing/debugging purpose.
        auto mov = generate_move(ins, opr0);
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
X64LIRRewriter::RewriteResult X64LIRRewriter::ResolveIModMulOperand(
    HInstruction* ins) const {
  if (ins->opcode() == HOpcode::kIMod || ins->opcode() == HOpcode::kIMul) {
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
