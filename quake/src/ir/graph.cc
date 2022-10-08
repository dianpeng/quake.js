#include <ir/graph.h>
#include <ir/interp-info.h>

namespace quake {
namespace ir {

Graph::Graph(Quake* q)
    : quake_(q),
      ssa_var_(q->zone()),
      checkpoint_(q->zone()),
      node_(q->zone()),
      slist_() {
  // FIXME(dpeng): Make following memory configurable
  node_.reserve(1024 * 16);
  ssa_var_.reserve(1024 * 16);
  checkpoint_.reserve(1024);

  std::memset(slist_, 0,
              sizeof(void*) * static_cast<std::size_t>(Opcode::kSizeOfOpcode));
}

Node* Graph::AfterAlloc(Node* n) {
  auto idx = static_cast<std::size_t>(n->opcode());

  {
    CHECK(n->next() == nullptr);
    n->set_chain(slist_[idx]);
    slist_[idx] = n;
  }

  // set the node's index
  n->index_ = static_cast<std::uint32_t>(node_.size());

  // enqueue it into the node_ list for tracking
  node_.emplace_back(n);

  return n;
}

Checkpoint* Graph::AddCheckpoint(BytecodeInfoProvider* info, Checkpoint* prev,
                                 Builder* builder, Checkpoint::Reason reason,
                                 const void* bc, Node* node) {
  if (prev->CanMerge(reason)) {
    prev->Snapshot(this, builder);
    return prev;
  } else {
    auto c = zone()->New<Checkpoint>(quake_, reason, node, bc);
    c->Snapshot(this, builder);
    checkpoint_.emplace_back(c);
    return c;
  }
}

SSAVar* Graph::AddSSAVar(Node* n) {
  auto index = static_cast<std::uint32_t>(ssa_var_.size());
  auto ssa_var = zone()->New<SSAVar>(quake_, index, n);
  ssa_var_.emplace_back(ssa_var);
  return ssa_var;
}

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode,
                     std::size_t size) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, size);
  return AfterAlloc(node);
}

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode);
  return AfterAlloc(node);
}

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode,
                     SSAVar* ssa_var) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, ssa_var);
  return AfterAlloc(node);
}

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode, SSAVar* opr1,
                     SSAVar* opr2) {
  auto node =
      zone()->New<Node>(quake_, info->bytecode_info(), opcode, opr1, opr2);
  return AfterAlloc(node);
}

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode,
                        std::size_t size) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  CHECK(GetOpcodeInfo(opcode).IsNary());

  auto node = AddNode(info, opcode, size);
  return AddSSAVar(node);
}

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  auto node = AddNode(info, opcode);
  return AddSSAVar(node);
}

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode,
                        SSAVar* opr) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  auto node = AddNode(info, opcode, opr);
  return AddSSAVar(node);
}

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode, SSAVar* opr1,
                        SSAVar* opr2) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  auto node = AddNode(info, opcode, opr1, opr2);
  return AddSSAVar(node);
}

Node* Graph::AddOp(BytecodeInfoProvider* info, Opcode opcode,
                   std::size_t size) {
  CHECK(GetOpcodeInfo(opcode).effect.IsOp());
  CHECK(GetOpcodeInfo(opcode).IsNary());

  return AddNode(info, opcode, size);
}

Node* Graph::AddOp(BytecodeInfoProvider* info, Opcode opcode) {
  CHECK(GetOpcodeInfo(opcode).effect.IsOp());
  return AddNode(info, opcode);
}

Node* Graph::AddOp(BytecodeInfoProvider* info, Opcode opcode, SSAVar* opr) {
  CHECK(GetOpcodeInfo(opcode).effect.IsOp());
  return AddNode(info, opcode, opr);
}

Node* Graph::AddOp(BytecodeInfoProvider* info, Opcode opcode, SSAVar* opr1,
                   SSAVar* opr2) {
  CHECK(GetOpcodeInfo(opcode).effect.IsOp());
  return AddNode(info, opcode, opr1, opr2);
}

Node* Graph::AddControlFlow(BytecodeInfoProvider* info, Opcode opcode,
                            SSAVar* cond) {
  CHECK(GetOpcodeInfo(opcode).effect.IsControlFlow());
  return AddNode(info, opcode, cond);
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode,
                             std::size_t size) {
  CHECK(GetOpcodeInfo(opcode).IsNary());

  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, size);
  node->index_ = kInternalIndex;
  return node;
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode);
  node->index_ = kInternalIndex;
  return node;
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode,
                             SSAVar* ssa_var) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, ssa_var);
  node->index_ = kInternalIndex;
  return node;
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode,
                             SSAVar* opr1, SSAVar* opr2) {
  auto node =
      zone()->New<Node>(quake_, info->bytecode_info(), opcode, opr1, opr2);
  node->index_ = kInternalIndex;
  return node;
}

SSAVar* Graph::SetNodeInternalSSA(Node* node) {
  auto ssa_var = zone()->New<SSAVar>(quake_, kInternalIndex, node);
  return ssa_var;
}

template <typename F>
Node* Graph::FindNode(Opcode kind, F&& func) {
  for (auto n = slist_[static_cast<std::size_t>(kind)]; n; n = n->next()) {
    if (func(n)) return n;
  }
  return nullptr;
}

// Constant ===================================================================
SSAVar* Graph::KI32(BytecodeInfoProvider* info, std::int32_t value) {
  if (auto node = FindNode(
          Opcode::kKI32, [&](Node* n) -> bool { return n->Int32() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAlloc(new_node));
}

SSAVar* Graph::KI64(BytecodeInfoProvider* info, std::int64_t value) {
  if (auto node = FindNode(
          Opcode::kKI64, [&](Node* n) -> bool { return n->Int64() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAlloc(new_node));
}

SSAVar* Graph::KFloat64(BytecodeInfoProvider* info, double value) {
  if (auto node = FindNode(Opcode::kKFloat64,
                           [&](Node* n) -> bool { return n->Float64() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAlloc(new_node));
}

SSAVar* Graph::KBool(BytecodeInfoProvider* info, bool value) {
  if (auto node = FindNode(Opcode::kKBool,
                           [&](Node* n) -> bool { return n->Bool() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAlloc(new_node));
}

SSAVar* Graph::KNull(BytecodeInfoProvider* info) {
  if (auto n = slist_[static_cast<std::size_t>(Opcode::kKNull)]; n) {
    CHECK(n->IsNull());
    return n->out();
  }
  auto new_node =
      zone()->New<Node>(quake_, info->bytecode_info(), Node::Null{});
  return AddSSAVar(AfterAlloc(new_node));
}

SSAVar* Graph::KUndefined(BytecodeInfoProvider* info) {
  if (auto n = slist_[static_cast<std::size_t>(Opcode::kKUndef)]; n) {
    CHECK(n->IsUndefined());
    return n->out();
  }
  auto new_node =
      zone()->New<Node>(quake_, info->bytecode_info(), Node::Undefined{});
  return AddSSAVar(AfterAlloc(new_node));
}

}  // namespace ir
}  // namespace quake
