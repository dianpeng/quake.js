#include "lir/operand.h"
#include "lir/instruction.h"

namespace quake {
namespace lir {

SizeClass HOperand::size_class() const {
  if (instr_) {
    return instr_->output_size_class();
  }

  // NOTES(dpeng): This should never happen since we have kImm node
  if (type_.IsImm()) {
    return GetImm().size_class();
  }

  return SizeClass::kIrrelevant;
}

const LivenessInfo& HOperand::liveness_info() const {
  return instr_->liveness_info();
}

LivenessInfo& HOperand::liveness_info() { return instr_->liveness_info(); }

bool HOperand::IsFloat() const {
  if (IsImm()) {
    return GetImm().IsFloat64();
  } else {
    CHECK(instr_);
    return instr_->IsFloat();
  }
}

bool HOperand::IsPhi() const { return instr_ && instr_->IsPhi(); }
bool HOperand::IsDeopt() const { return instr_ && instr_->IsDeopt(); }
bool HOperand::IsJump() const { return instr_ && instr_->IsJump(); }
bool HOperand::IsCmp() const { return instr_ && instr_->IsCmp(); }
bool HOperand::IsMove() const { return instr_ && instr_->IsMove(); }
bool HOperand::IsBranch() const { return instr_ && instr_->IsBranch(); }
bool HOperand::IsCondition() const { return instr_ && instr_->IsCondition(); }

std::string HOperand::Print() const {
  std::stringstream fmt;

  // Notes, this printting only takes care of the instruction + immediate,
  // which is before the RA. After the RA the HOperand will be morphed into
  // internal slot, memory position, etc ... which is not really about the
  // LIR instruction anymore

  if (IsImm()) {
    const auto& imm = GetImm();
    switch (imm.type()) {
      case Immediate::kInt32:
        fmt << "@i32(" << imm.Int32() << ")";
        break;

      case Immediate::kInt64:
        fmt << "@i64(" << imm.Int64() << ")";
        break;

      case Immediate::kFloat64:
        fmt << "@f64(" << imm.Float64() << ")";
        break;

      default:
        UNREACHABLE();
        break;
    }
  } else if (IsLabel()) {
    auto l = GetLabel();
    if (auto b = l->bind(); b) {
      fmt << "@label(#" << b->id() << ")";
    } else {
      fmt << "@label()";
    }
  } else {
    CHECK(HasInstruction());
    fmt << "#" << instruction()->id();
  }

  return fmt.str();
}

}  // namespace lir
}  // namespace quake
