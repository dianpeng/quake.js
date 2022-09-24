#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/assembler.h"
#include "ir/node.h"
#include "zone.h"

using namespace zone;

namespace quake {
namespace lir {

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant) {
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode, HOperand* op1)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant) {
  operand_.reserve(1);
  operand_.push_back(op1);
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode, HOperand* op1,
                           HOperand* op2)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant) {
  operand_.reserve(2);
  operand_.push_back(op1);
  operand_.push_back(op2);
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode, std::size_t len)
    : id_(id),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(GetHOpcodeInfo(opcode)),
      operand_(zone),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant) {
  operand_.resize(len, nullptr);
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone)
    : id_(id),
      ir_node_(nullptr),
      opcode_(HOpcode::kSentry),
      opcode_info_(GetHOpcodeInfo(HOpcode::kSentry)),
      operand_(zone),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant) {
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
        case HOpcode::kMovR:
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
    output_ = as->NewTemporary(this);
    return output_;
  }
}

}  // namespace lir
}  // namespace quake
