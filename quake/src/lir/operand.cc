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

bool HOperand::IsRCall() const { return instr_ && instr_->IsRCall(); }
bool HOperand::IsNativeCall() const { return instr_ && instr_->IsNativeCall(); }
bool HOperand::IsPhi() const { return instr_ && instr_->IsPhi(); }
bool HOperand::IsDeopt() const { return instr_ && instr_->IsDeopt(); }
bool HOperand::IsJump() const { return instr_ && instr_->IsJump(); }
bool HOperand::IsCmp() const { return instr_ && instr_->IsCmp(); }
bool HOperand::IsMove() const { return instr_ && instr_->IsMove(); }
bool HOperand::IsBranch() const { return instr_ && instr_->IsBranch(); }
bool HOperand::IsCondition() const { return instr_ && instr_->IsCondition(); }

HType HOperand::htype() const {
  if (instr_) {
    return instr_->htype();
  } else {
    // based on its current settings to return the htype correspondingly.
    if (IsImm()) {
      const auto& imm = GetImm();
      switch (imm.type()) {
        case Immediate::kInt32:
          return HType::kInt;
        case Immediate::kInt64:
          return HType::kI64;
        case Immediate::kFloat64:
          return HType::kFloat;
        default:
          UNREACHABLE();
          break;
      }
    } else if (IsLabel()) {
      return HType::kNone;
    } else {
      UNREACHABLE() << "memory, temporary(virtual register) or register "
                    << "should have its corresponding instruction";
    }
  }
  return HType::kNone;
}

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
  } else if (IsMemStatic()) {
    fmt << GetMemStatic().Print();  // result in lowering or other stuff,
                                    // may not have a instruction related
  } else if (IsMemInd()) {
    fmt << GetMemInd().Print();     // may not have a instruction related
  } else {
    CHECK(HasInstruction());
    if (IsNone() || IsTemporary()) {
      fmt << "#" << instruction()->id();
    } else {
      // RA has been executed, so print out the actual settlement of this
      // instruction after the RA pass
      if (IsMemStack()) {
        fmt << GetMemStack().Print();
      } else if (IsRegister()) {
        fmt << "@r(" << GetRegister()->Print() << ")";
      } else {
        UNREACHABLE() << "should have already been resolved by RA";
      }
    }
  }

  return fmt.str();
}

}  // namespace lir
}  // namespace quake
