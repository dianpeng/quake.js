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
    : HInstructionList(zone),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      exit_(nullptr),
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
    : HInstructionList(zone),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      exit_(nullptr),
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
    : HInstructionList(zone),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      exit_(nullptr),
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
    : HInstructionList(zone),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      exit_(nullptr),
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
    : HInstructionList(zone),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      exit_(nullptr),
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

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HOpcode opcode)
    : HInstructionList(zone),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      exit_(nullptr),
      ir_node_(nullptr),
      opcode_(opcode),
      opcode_info_(&GetHOpcodeInfo(opcode)),
      htype_(opcode_info_->output_type_single()),
      operand_(zone, 0),
      output_(nullptr),
      output_sc_(SizeClass::kIrrelevant),
      liveness_info_(zone) {
  CHECK(opcode_info_->output.IsDummy());
  SelfWire();
}

HInstruction::HInstruction(std::uint32_t id, Zone* zone, HInstruction* prev)
    : HInstructionList(zone),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      cell_index_(0),
      exit_(nullptr),
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
  if (opcode_info().output.IsDummy()) {
    output_sc_ = SizeClass::kIrrelevant;
  } else if (opcode_info().output.IsOp()) {
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
        case HOpcode::kTrans:
        case HOpcode::kTransF:
        case HOpcode::kShuffle:
        case HOpcode::kShuffleF:
          CHECK(opr_size() == 1);
          output_sc_ = Opr(0)->size_class();
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
    if (opcode_info().output.IsDummy()) {
      output_ = as->NewNone();
    } else {
      output_ = as->NewTemporary();
    }
    output_->set_instruction(this);
    return output_;
  } else {
    output_ = nullptr;
    return nullptr;
  }
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

  // Notes, do not modify corresponding ir value, but let the ir value always
  // hold the original virtual operand. This is needed for snapshot to work

  output->set_instruction(output_->instruction());
  output_ = output;
}

int HInstruction::MapNativeCallParamIndex(int index) const {
  if (index >= opr_size()) return -1;
  auto opr = Opr(index);
  return MapNativeCallParamIndex(opr);
}

// Return each index corresponding to FP/GP register. Notes, the returned
// index combined with operand's type (FP/GP) can be used to access parameter
// register information while RA is performed.
//
// rcall(#F, #G, #G, #F)
//
// 1. #F => 0
// 2. #G => 0
// 3. #G => 1
// 4. #F => 2
//
// This can be used to work with ABI parameter fetching internally
//
// For RCall, a hidden parameter is been passed by first argument in GP, so
// GP will always have one more

int HInstruction::MapNativeCallParamIndexRCall(HOperand* arg) const {
#ifdef QUAKE_TARGET_X64
  int i = 0;
  if (IsRCall()) {
    i = 1;  // skip the runtime index
  }

  int gp = 0;
  int fp = 0;

  for (; i < opr_size(); i++) {
    auto current = Opr(i);
    if (current == arg) break;
    if (current->htype() == HType::kFloat) {
      ++fp;
    } else {
      ++gp;
    }
  }

  if (arg->htype() == HType::kFloat) {
    return fp;
  } else {
    // Notes, for RCall, one hidden argument is been carried out
    return gp + (IsRCall() ? 1 : 0);
  }

#else
  UNREACHABLE();
  return -1;
#endif  // QUAKE_TARGET_X64
}

int HInstruction::MapNativeCallParamIndexJSCall(HOperand* arg) const {
  UNUSE(arg);
  return -1;
}

int HInstruction::MapNativeCallParamIndex(HOperand* arg) const {
  if (IsRCall()) return MapNativeCallParamIndexRCall(arg);
  if (IsJSCall()) return MapNativeCallParamIndexJSCall(arg);
  return -1;
}

int HInstruction::NativeCallParameterSize() const {
  if (IsNativeCall()) {
    if (IsRCall()) {
      return opr_size() - 1;
    } else if (IsJSCall()) {
      return 0;  // every parameter is hidden from the view of LIR
    } else {
      UNREACHABLE();
      return 0;
    }
  } else {
    return -1;
  }
}

void HInstruction::ForeachNativeCallParameter(
    std::function<void(HOperand*)>&& visitor) const {
  if (IsNativeCall()) {
    if (IsJSCall()) {
      // js call does not have exposed parameters, all require marshaling
      return;
    } else if (IsRCall()) {
      auto idx = 1;
      for (; idx < opr_size(); idx++) {
        visitor(Opr(idx));
      }
    } else {
      UNREACHABLE();
    }
  }
}

// Check wether this instruction is been used by any other FCall
std::vector<int> HInstruction::NativeCallParamRef() const {
  std::vector<int> param;

  if (auto out = output(); out) {
    out->ForeachUse([&](const auto& use) -> bool {
      if (use.def->IsNativeCall()) {
        if (auto idx = use.def->MapNativeCallParamIndex(out); idx >= 0) {
          param.push_back(idx);
        }
      }
      return true;
    });
  }

  return param;
}

HOperand* HInstruction::RemoveOutput() {
  if (output_) {
    output_->set_instruction(nullptr);  // unbounded instruction
    if (auto ir_var = output_->ir_var(); ir_var) {
      ir_var->set_lir_operand(nullptr);  // clear the ir_var's binding now
    }
  }
  auto o = output_;
  output_ = nullptr;
  return o;
}

void HInstruction::Remove() {
  if (output_) CHECK(!output_->HasUse());
  RemoveOperands();
  RemoveSelf();
}

bool HInstruction::TryRemove() {
  if (output_ && output_->HasUse()) return false;
  RemoveOperands();
  RemoveSelf();
  return true;
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

void HInstruction::Replace(HOperand* replacement) {
  CHECK(HasOutput());
  output_->Replace(replacement);
  CHECK(!output_->HasUse());
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
  if (HasExit()) {
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
