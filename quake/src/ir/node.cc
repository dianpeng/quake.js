#include "ir/node.h"
#include "ir/ssa-var.h"
#include "ir/checkpoint.h"

namespace quake {
namespace ir {

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  operand_.opr1 = nullptr;
  operand_.opr2 = nullptr;
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, SSAVar* opr)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 1);

  operand_.opr1 = opr;
  operand_.opr2 = nullptr;

  opr->AddUseChecked(this, 0, &operand_.opr1);
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, SSAVar* opr1,
           SSAVar* opr2)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 2);

  operand_.opr1 = opr1;
  operand_.opr2 = opr2;

  opr1->AddUseChecked(this, 0, &operand_.opr1);
  opr2->AddUseChecked(this, 1, &operand_.opr2);
}

// ----------------------------------------------------------------------------
// Designated typped node

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, const Type& type)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 0);

  operand_.opr1 = nullptr;
  operand_.opr2 = nullptr;
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, const Type& type,
           SSAVar* opr1)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 1);

  operand_.opr1 = opr1;
  operand_.opr2 = nullptr;

  opr1->AddUseChecked(this, 0, &operand_.opr1);
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, const Type& type,
           SSAVar* opr1, SSAVar* opr2)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 2);

  operand_.opr1 = opr1;
  operand_.opr2 = opr2;

  opr1->AddUseChecked(this, 0, &operand_.opr1);
  opr2->AddUseChecked(this, 1, &operand_.opr2);
}

// ----------------------------------------------------------------------------
// Literal nodes

Node::Node(Quake* q, const BytecodeInfo& binfo, std::int32_t value)
    : quake_(q),
      opcode_(Opcode::KI32),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode_)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 0);
  operand_.i32_value = value;
}

Node::Node(Quake* q, const BytecodeInfo& binfo, std::int64_t value)
    : quake_(q),
      opcode_(Opcode::KI64),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode_)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 0);
  operand_.i64_value = value;
}

Node::Node(Quake* q, const BytecodeInfo& binfo, double value)
    : quake_(q),
      opcode_(Opcode::KFloat64),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode_)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 0);
  operand_.f64_value = value;
}

Node::Node(Quake* q, const BytecodeInfo& binfo, bool value)
    : quake_(q),
      opcode_(Opcode::KBool),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode_)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 0);
  operand_.b_value = value;
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Null&&)
    : quake_(q),
      opcode_(Opcode::KNull),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode_)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 0);
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Undefined&&)
    : quake_(q),
      opcode_(Opcode::KUndef),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode_)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->arity == 0);
}

// ----------------------------------------------------------------------------
// NArity type opcodes
Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, std::size_t cap)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(opcode_info_->type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->IsNary());
  operand_.opr_list = q->zone()->New<zone::Vector<SSAVar*>>(q->zone());
  operand_.opr_list->resize(cap, nullptr);
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, const Type& type,
           std::size_t cap)
    : quake_(q),
      opcode_(opcode),
      next_(nullptr),
      flag_(),
      opcode_info_(&GetOpcodeInfo(opcode)),
      type_(type),
      out_(nullptr),
      operand_(),
      exit_(nullptr),
      snapshot_(nullptr),
      bytecode_info_(binfo),
      mcode_info_() {
  CHECK(opcode_info_->IsNary());
  operand_.opr_list = q->zone()->New<zone::Vector<SSAVar*>>(q->zone());
  operand_.opr_list->resize(cap, nullptr);
}

SSAVar* Node::At(int idx) {
  CHECK(idx >= 0 && idx < size());
  if (IsNary()) {
    return operand_.opr_list->at(idx);
  } else {
    switch (idx) {
      case 0:
        return operand_.opr1;
      case 1:
        return operand_.opr2;
      default:
        UNREACHABLE();
        return nullptr;
    }
  }
}

int Node::By(SSAVar* v) {
  if (IsNary()) {
    auto itr =
        std::find_if(operand_.opr_list->begin(), operand_.opr_list->end(),
                     [&](SSAVar* n) -> bool { return n == v; });
    if (itr == operand_.opr_list->end()) {
      return -1;
    } else {
      return static_cast<int>(std::distance(itr, operand_.opr_list->begin()));
    }
  } else {
    switch (arity()) {
      case 1:
        if (operand_.opr1 == v) {
          return 0;
        }
        break;

      case 2:
        if (operand_.opr2 == v) {
          return 1;
        }
        break;

      default:
        break;
    }
    return -1;
  }
}

void Node::Set(int index, SSAVar* value) {
  CHECK(index >= 0 && index < size());
  if (IsNary()) {
    (*operand_.opr_list)[index] = value;
  } else {
    if (index == 0) operand_.opr1 = value;
    if (index == 1) operand_.opr2 = value;
  }
}

void Node::BailoutTo(Checkpoint* checkpoint) {
  CHECK(ir_effect().RequireBailout());

  RemoveExit();
  exit_ = checkpoint;
  checkpoint->JumpFrom(this);
}

void Node::RemoveExit() {
  if (exit_) {
    CHECK(exit_->RemoveJumpFrom(this));
    exit_ = nullptr;
  }
}

}  // namespace ir
}  // namespace quake
