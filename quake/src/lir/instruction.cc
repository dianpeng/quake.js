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
  if (auto old_value = holder_[index]; old_value) {
    CHECK(old_value->RemoveUse(ins, index));
  }

  holder_[index] = value;
  if (value) value->AddUse(ins, index, &holder_[index]);
}

void HOperandList::Clear(HInstruction* ins) {
  int idx = 0;
  for (auto& e : holder_) {
    if (e) {
      CHECK(e->RemoveUse(ins, idx));
    }
    e = nullptr;
    idx++;
  }
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode)
    : id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(&GetHOpcodeInfo(opcode)),
      htype_(opcode_info_->output_type_single()),
      operand_(zone, 0),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev,
                           ir::Node* node, HOpcode opcode, HOperand* op1)
    : id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(&GetHOpcodeInfo(opcode)),
      htype_(opcode_info_->output_type_single()),
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
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(&GetHOpcodeInfo(opcode)),
      htype_(opcode_info_->output_type_single()),
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
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      ir_node_(node),
      opcode_(opcode),
      opcode_info_(&GetHOpcodeInfo(opcode)),
      htype_(opcode_info_->output_type_single()),
      operand_(zone, len),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  InsertAfter(prev);
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone)
    : id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      ir_node_(nullptr),
      opcode_(HOpcode::kSentry),
      opcode_info_(&GetHOpcodeInfo(HOpcode::kSentry)),
      htype_(opcode_info_->output_type_single()),
      operand_(zone, 0),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  SelfWire();
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev)
    : id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      ir_node_(nullptr),
      opcode_(HOpcode::kPlaceholder),
      opcode_info_(&GetHOpcodeInfo(HOpcode::kPlaceholder)),
      htype_(opcode_info_->output_type_single()),
      operand_(zone, 0),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  InsertAfter(prev);
}

void HInstruction::set_opcode(HOpcode x) {
  opcode_ = x;
  opcode_info_ = &GetHOpcodeInfo(x);
}

HOperand* HInstruction::At(int idx) const {
  if (idx == -1) {
    return output_;
  } else {
    return Opr(idx);
  }
}

HOperand* HInstruction::AtChecked(int idx) const {
  auto opr = At(idx);
  CHECK(opr);
  return opr;
}

bool HInstruction::VerifyOperand() const { return true; }

bool HInstruction::IsComplete() const {
  for (int i = 0; i < opr_size(); i++) {
    if (!Opr(i)) return false;
  }
  return true;
}

bool HInstruction::DecideTypeAndSizeClass() {
  if (opcode_info().output.IsOp()) {
    output_sc_ = SizeClass::kIrrelevant;
  } else {
    // Most of the value instruction will be decided even if the instruction is
    // not setup completely
    output_sc_ = opcode_info().output.sc0;

    if (output_sc_ == SizeClass::kUnknown) {
      if (!IsComplete()) return false;
      CHECK(htype_ == HType::kBits);

      // Fallback to architecture specific code to infer the size class of our
      // specific LIR opcode. Typically it is a result of move instruction,
      // which we just don't know how it should be interpreted. For example,
      // movzx/movsx should promote input register's size class
      switch (opcode_) {
        case HOpcode::kMov:
        case HOpcode::kMovF:
        case HOpcode::kSpill:
        case HOpcode::kSpillF:
        case HOpcode::kShuffle:
        case HOpcode::kShuffleF:
          CHECK(opr_size() == 1);
          output_sc_ = Opr(0)->size_class();
          htype_ = Opr(0)->instruction()->htype();
          break;

        case HOpcode::kMovZX:
        case HOpcode::kMovSX:
        case HOpcode::kMovSXD:
          // ZX and SX will need to promote the size class
          CHECK(opr_size() == 1);
          output_sc_ = SizeClassPromote(Opr(0)->size_class());
          htype_ = Opr(0)->instruction()->htype();
          break;

        default:
          UNREACHABLE();
          break;
      }
    }
    CHECK(output_sc_ != SizeClass::kIrrelevant);
  }

  return true;
}

HOperand* HInstruction::PopulateOutput(HAssembler* as) {
  DecideTypeAndSizeClass();
  if (opcode_info().output.IsValue()) {
    output_ = as->NewTemporary();
    output_->set_instruction(this);
    return output_;
  } else {
    output_ = nullptr;
    return nullptr;
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
  // notes ir::Node may or may not produce a value. If it does produce a value,
  // just mark it as output
  if (auto out = node->out(); out) {
    CHECK(output_);
    out->set_lir_operand(output_);
  }
}

void HInstruction::SetRAOutput(HOperand* output) {
  CHECK(!output->IsTemporary());
  CHECK(output_);
  output->set_instruction(output_->instruction());
  output_ = output;
}

int HInstruction::MapNativeCallParamIndex(int index) const {
  if (index >= opr_size()) return -1;
  if (IsRCall()) return index - 1;
  return index;
}

int HInstruction::MapNativeCallParamIndex(HOperand* arg) const {
  int i = 0;
  if (IsRCall()) {
    i = 1;  // skip the runtime index
  }

  int index = 0;
  for (; i < opr_size(); i++) {
    if (Opr(i) == arg) return index;
    index++;
  }
  return -1;
}

// Check wether this instruction is been used by any other FCall
std::vector<int> HInstruction::NativeCallParamRef() const {
  std::vector<int> param;

  if (auto out = output(); out) {
    out->ForeachUse([&](const auto& use) -> bool {
      if (use.def->IsNativeCall()) {
        param.push_back(use.def->MapNativeCallParamIndex(out));
      }
      return true;
    });
  }

  return param;
}

void HInstruction::Remove() {
  if (output_) CHECK(!output_->HasUse());
  RemoveOperands();
  RemoveSelf();
}

void HInstruction::Replace(HInstruction* replacement) {
  if (replacement->HasOutput()) {
    CHECK(HasOutput());
    output_->Replace(replacement->output());
    CHECK(!output_->HasUse());
  } else {
    CHECK(!HasOutput());
  }
}

void HInstruction::MaterializePlaceholder(ir::Node* ir_node, HOpcode opcode,
                                          std::size_t size) {
  CHECK(IsPlaceholder());

  ir_node_ = ir_node;
  opcode_ = opcode;
  opcode_info_ = &GetHOpcodeInfo(opcode);
  htype_ = opcode_info_->output_type_single();
  operand_.Resize(size);
}

// ----------------------------------------------------------------------------
// Stringify/Graphviz implementation

std::string HInstruction::Print() const {
  std::stringstream fmt;
  if (HasDeopt()) {
    fmt << "*";
  }
  fmt << "#" << id() << ": " << opcode_name() << " ";

  if (output_) {
    fmt << ":" << output_->Print() << " ";
  }

  fmt << "[";
  for (int i = 0; i < opr_size(); i++) {
    auto opr = Opr(i);
    fmt << opr->Print() << "; ";
  }
  fmt << "]";

  return fmt.str();
}

}  // namespace lir
}  // namespace quake
