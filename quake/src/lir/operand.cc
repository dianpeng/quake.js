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

bool HOperand::IsPhi() const { return instr_->IsPhi(); }

bool HOperand::IsDeopt() const { return instr_->IsDeopt(); }

bool HOperand::IsJump() const { return instr_->IsJump(); }

bool HOperand::IsComp() const { return instr_->IsComp(); }

bool HOperand::IsMove() const { return instr_->IsMove(); }

}  // namespace lir
}  // namespace quake
