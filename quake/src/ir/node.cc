#include "ir/node.h"
#include "ir/ssa-var.h"
#include "ir/checkpoint.h"

namespace quake {
namespace ir {

using namespace base;

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode)
    : quake_(q),
      opcode_(opcode),
      chain_(nullptr),
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
      chain_(nullptr),
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

  if (opr) opr->AddUseChecked(this, 0, &operand_.opr1);
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, SSAVar* opr1,
           SSAVar* opr2)
    : quake_(q),
      opcode_(opcode),
      chain_(nullptr),
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

  if (opr1) opr1->AddUseChecked(this, 0, &operand_.opr1);
  if (opr2) opr2->AddUseChecked(this, 1, &operand_.opr2);
}

// ----------------------------------------------------------------------------
// Designated typped node

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, const Type& type)
    : quake_(q),
      opcode_(opcode),
      chain_(nullptr),
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
      chain_(nullptr),
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

  if (opr1) opr1->AddUseChecked(this, 0, &operand_.opr1);
}

Node::Node(Quake* q, const BytecodeInfo& binfo, Opcode opcode, const Type& type,
           SSAVar* opr1, SSAVar* opr2)
    : quake_(q),
      opcode_(opcode),
      chain_(nullptr),
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

  if (opr1) opr1->AddUseChecked(this, 0, &operand_.opr1);
  if (opr2) opr2->AddUseChecked(this, 1, &operand_.opr2);
}

// ----------------------------------------------------------------------------
// Literal nodes

Node::Node(Quake* q, const BytecodeInfo& binfo, std::int32_t value)
    : quake_(q),
      opcode_(Opcode::kKI32),
      chain_(nullptr),
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
      opcode_(Opcode::kKI64),
      chain_(nullptr),
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
      opcode_(Opcode::kKFloat64),
      chain_(nullptr),
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
      opcode_(Opcode::kKBool),
      chain_(nullptr),
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
      opcode_(Opcode::kKNull),
      chain_(nullptr),
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
      opcode_(Opcode::kKUndef),
      chain_(nullptr),
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

Node::Node(Quake* q, const BytecodeInfo& binfo, Uninit&&)
    : quake_(q),
      opcode_(Opcode::kKUninit),
      chain_(nullptr),
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
      chain_(nullptr),
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
      chain_(nullptr),
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

SSAVar* Node::At(int idx) const {
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

std::int32_t Node::AtI32(int idx) const {
  auto v = At(idx);
  CHECK(v);
  auto node = v->node_checked();
  return node->Int32();
}

std::int64_t Node::AtI64(int idx) const {
  auto v = At(idx);
  CHECK(v);
  auto node = v->node_checked();
  return node->Int64();
}

double Node::AtF64(int idx) const {
  auto v = At(idx);
  CHECK(v);
  auto node = v->node_checked();
  return node->Float64();
}

bool Node::AtBool(int idx) const {
  auto v = At(idx);
  CHECK(v);
  auto node = v->node_checked();
  return node->Bool();
}

int Node::By(SSAVar* v) const {
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

bool Node::IsComplete() const {
  const auto len = size();
  for (int i = 0; i < len; i++) {
    if (!At(i)) return false;
  }
  return true;
}

SSAVar* Node::DoSet(int index, SSAVar* value) {
  CHECK(index >= 0 && index < size());

  SSAVar* old = nullptr;
  if (old = At(index); old) {
    CHECK(old->RemoveUse(this, index));
  }

  if (IsNary()) {
    (*operand_.opr_list)[index] = value;
    if (value) {
      value->AddUseChecked(this, index, operand_.opr_list->data() + index);
    }
  } else {
    if (index == 0) {
      operand_.opr1 = value;
      if (value) {
        value->AddUseChecked(this, index, &operand_.opr1);
      }
    } else if (index == 1) {
      operand_.opr2 = value;
      if (value) {
        value->AddUseChecked(this, index, &operand_.opr2);
      }
    }
  }

  return old;
}

void Node::Set(int index, SSAVar* value) { DoSet(index, value); }

SSAVar* Node::Remove(int index) { return DoSet(index, nullptr); }

void Node::Replace(SSAVar* ref) {
  if (out_) {
    out_->Replace(ref);
  }
}

void Node::Replace(Node* ref) {
  if (ref->out()) Replace(ref->out());
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

bool Node::InferPhiType() {
  CHECK(IsPhi());
  CHECK(IsComplete());
  Type t = At(0)->type();

  for (int i = 1; i < size(); i++) {
    auto tt = At(i)->type();
    if (tt != t) {
      return false;
    }
  }

  if (type_.IsInfer()) {
    type_ = t;
    return true;
  } else {
    return type_ == t;
  }
}

std::string Node::PrintConst() const {
  std::stringstream fmt;
  CHECK(IsConst());
  switch (opcode()) {
    case Opcode::kKI32:
      fmt << "@i32(" << Int32() << ")";
      break;

    case Opcode::kKI64:
      fmt << "@i64(" << Int64() << ")";
      break;

    case Opcode::kKFloat64:
      fmt << "@f64(" << Float64() << ")";
      break;

    case Opcode::kKBool:
      fmt << "@b(" << (Bool() ? "true" : "false") << ")";
      break;

    case Opcode::kKNull:
      fmt << "@null";
      break;

    case Opcode::kKUndef:
      fmt << "@undefined";
      break;

    case Opcode::kKUninit:
      fmt << "@uninitialized";
      break;

    case Opcode::kKNStr:
      fmt << "@nstr";
      break;

    case Opcode::kKStr:
      // FIXME(dpeng): Add string support
      fmt << "@str(...)";
      break;

    default:
      UNREACHABLE();
      break;
  }

  return fmt.str();
}

std::string Node::Print() const {
  std::stringstream fmt;

  if (IsConst()) {
    // const is been stringified in compact way
    fmt << PrintConst();
    return fmt.str();
  } else {
    fmt << "#" << node_id() << ": " << opcode_name() << " ";
  }

  // (1) all the operands
  fmt << "[";
  for (int i = 0; i < size(); i++) {
    auto opr = At(i);
    auto opr_node = opr->node();
    if (opr_node->IsInternal()) {
      fmt << opr_node->Print() << "; ";
    } else {
      fmt << '#' << opr->node()->node_id() << "; ";
    }
  }
  fmt << "]";

  // (2) print out the type
  // fmt << "{" << type().Print() << "}";

  return fmt.str();
}

std::string Node::PrintRefId() const { return Fmt("#%u", node_id()); }

// ----------------------------------------------------------------------------
// Graphviz interfaces implementation

std::string Node::PrintGraphName() const { return Fmt("node_%u", node_id()); }

std::string Node::PrintGraphNode() const {
  auto name = PrintGraphName();
  if (IsConst()) {
    return Fmt("%s [color=aqua shape=circle label=\"%s\"]\n", name.c_str(),
               PrintConst().c_str());
  } else if (IsInternal()) {
    return Fmt("%s [color=coral shape=circle label=\"%s\"]\n", name.c_str(),
               opcode_name().c_str());
  } else if (IsControlFlow()) {
    return Fmt("%s [color=red shape=diamond label=\"%s\"]\n",
               PrintGraphName().c_str(), opcode_name().c_str());
  } else {
    return Fmt("%s [color=red shape=box label=\"%s\"]\n",
               PrintGraphName().c_str(), opcode_name().c_str());
  }
}

std::string Node::PrintGraphLink() const {
  auto name = PrintGraphName();
  std::stringstream fmt;

  for (int i = 0; i < size(); i++) {
    auto opr = At(i);
    // detect link type, if current node is a control flow, then it is a
    // control flow dependency instead of a value dependency. notes, we
    // do not label link
    if (IsControlFlow()) {
      fmt << name << " -> " << opr->node()->PrintGraphName()
          << " [style=dashed penwidth=4.0 color=green]\n";
    } else {
      fmt << name << " -> " << opr->node()->PrintGraphName()
          << " [style=dotted penwidth=2.0 color=blue]\n";
    }
  }
  return fmt.str();
}

}  // namespace ir
}  // namespace quake
