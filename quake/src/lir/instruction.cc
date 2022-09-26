#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/assembler.h"

#include "ir/node.h"
#include "ir/ssa-var.h"

#include "zone.h"

using namespace zone;

namespace quake {
namespace lir {

void HOperandList::Set(HInstruction* ins, int index, HOperand* value) {
  CHECK(index >= 0 && index < static_cast<int>(holder_.size()));
  holder_[index] = value;
  value->AddUse(ins, index, &holder_[index]);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone, 0),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode, HOperand* op1)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone, 1),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  operand_.Set(this, 0, op1);

  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode, HOperand* op1,
                           HOperand* op2)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone, 2),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  operand_.Set(this, 0, op1);
  operand_.Set(this, 1, op2);

  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode, std::size_t len)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone, len),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone)
    : id_(id),
      ir_node_(nullptr),
      opcode_(HOpcode::kSentry),
      opcode_info_(GetHOpcodeInfo(HOpcode::kSentry)),
      operand_(zone, 0),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  SelfWire();
}

HOperand* HInstruction::At(int idx) const {
  if (idx == -1) {
    return output_;
  } else {
    return Opr(idx);
  }
}

bool HInstruction::VerifyOperand() const { return true; }

void HInstruction::DecideSizeClass() {
  if (opcode_info_.output.IsOp()) {
    output_sc_ = SizeClass::kIrrelevant;
  } else {
    if (opcode_info_.output.sc0 == SizeClass::kUnknown) {
      // Fallback to architecture specific code to infer the size class of our
      // specific LIR opcode. Typically it is a result of move instruction,
      // which we just don't know how it should be interpreted. For example,
      // movzx/movsx should promote input register's size class
      switch (opcode_) {
        case HOpcode::kImm:
        case HOpcode::kMov:
          CHECK(opr_size() == 1);
          output_sc_ = Opr(0)->size_class();
          break;

        case HOpcode::kMovZX:
        case HOpcode::kMovSX:
          // ZX and SX will need to promote the size class
          CHECK(opr_size() == 1);
          output_sc_ = SizeClassPromote(Opr(0)->size_class());
          break;

        default:
          UNREACHABLE();
          break;
      }
    }
  }
}

HOperand* HInstruction::PopulateOutput(HAssembler* as) {
  DecideSizeClass();

  if (opcode_info_.output.IsOp()) {
    output_ = nullptr;
    return nullptr;
  } else {
    output_ = as->NewTemporary();
    output_->set_instruction(this);
    return output_;
  }
}

ir::Checkpoint* HInstruction::GetExit(HAssembler* as) const {
  if (IsDeopt()) {
    auto operand = Opr(0);
    CHECK(operand->IsImm());
    CHECK(operand->GetImm().IsInt32());
    auto index = operand->GetImm().Int32();
    return as->GetCheckpoint(index);
  }
  return nullptr;
}

void HInstruction::SetOutput(ir::Node* node) {
  CHECK(node == ir_node());
  ir_output_ = true;

  // notes ir::Node may or may not produce a value. If it does produce a value,
  // just mark it as output
  if (auto out = node->out(); out) {
    CHECK(output_);
    out->set_lir_operand(output_);
  }
}

// Check wether this instruction is been used by any other FCall
std::vector<int> HInstruction::NativeCallParamRef() const {
  std::vector<int> param;

  if (auto out = output(); out) {
    out->ForeachUse([&](const auto& use) -> bool {
      if (use.def->IsNativeCall()) {
        param.push_back(use.index);
      }
      return true;
    });
  }

  return param;
}

}  // namespace lir
}  // namespace quake
