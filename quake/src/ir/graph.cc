#include <ir/graph.h>
#include <ir/interp-info.h>

namespace quake {
namespace ir {

Graph::Graph(Quake* q)
    : quake_(q),
      node_id_(0),

      ssa_var_(),
      checkpoint_(),
      internal_node_(),

      next_node_(zone()->New<Node>(quake_)),
      sentry_node_(next_node_),

      slist_(),

      end_of_bb0_(nullptr),
      end_of_bb1_(nullptr) {
  // FIXME(dpeng): Make following memory configurable
  ssa_var_.reserve(1024 * 16);
  checkpoint_.reserve(1024);
  internal_node_.reserve(1024 * 4);

  std::memset(slist_, 0,
              sizeof(void*) * static_cast<std::size_t>(Opcode::kSizeOfOpcode));
}

// We should optimize the BBType instead of calculating it everytime on the fly
// we should just try to cache it but if any node is modified, ie move to other
// position then this information should be modified on the fly
Graph::BBType Graph::GetBBType(const Node& n) const {
  CHECK(!n.IsDead());

  // Forward walking from the node position until we hit either BB0 or BB1
  // then we know the BBType
  for (auto nn = &n; nn != end_node(); nn = nn->next()) {
    if (nn == end_of_bb0()) {
      return BBType::kBB0;
    }
    if (nn == end_of_bb1()) {
      return BBType::kBB1;
    }
  }

  UNREACHABLE() << "node not belong to current graph?";
  return BBType::kBB0;
}

void Graph::SetNodeId(Node* n) { n->node_id_ = node_id_++; }

Node* Graph::AfterAlloc(Node* n) {
  auto idx = static_cast<std::size_t>(n->opcode());

  {
    CHECK(n->chain() == nullptr);
    n->set_chain(slist_[idx]);
    slist_[idx] = n;
  }

  // set the node's index
  n->internal_ = false;
  SetNodeId(n);

  // enqueue it into the node_ list for tracking
  if (next_node_) {
    n->InsertAfter(next_node_);
    next_node_ = n;
  } else {
    n->NullWire();
  }

  return n;
}

Node* Graph::AfterAllocInternal(Node* n) {
  auto idx = static_cast<std::size_t>(n->opcode());

  {
    CHECK(n->chain() == nullptr);
    n->set_chain(slist_[idx]);
    slist_[idx] = n;
  }

  n->internal_ = true;
  SetNodeId(n);
  n->NullWire();

  internal_node_.push_back(n);

  return n;
}

Checkpoint* Graph::AddCheckpointBefore(BytecodeInfoProvider* info,
                                       Checkpoint* prev, Builder* builder,
                                       Checkpoint::Reason reason, Node* node) {
  if (prev && prev->CanMerge(reason)) {
    prev->Snapshot(this, builder);
    prev->Relink(node, reason, false);
    return prev;
  } else {
    auto id = static_cast<CheckpointId>(checkpoint_.size());
    auto c = zone()->New<Checkpoint>(quake_, id, reason, node, false);
    c->Snapshot(this, builder);
    checkpoint_.emplace_back(c);
    return c;
  }
}

Checkpoint* Graph::AddCheckpointAfter(BytecodeInfoProvider* info,
                                      Checkpoint* prev, Builder* builder,
                                      Checkpoint::Reason reason, Node* node) {
  if (prev && prev->CanMerge(reason)) {
    prev->Snapshot(this, builder);
    prev->Relink(node, reason, true);
    return prev;
  } else {
    auto id = static_cast<CheckpointId>(checkpoint_.size());
    auto c = zone()->New<Checkpoint>(quake_, id, reason, node, true);
    c->Snapshot(this, builder);
    checkpoint_.emplace_back(c);
    return c;
  }
}

Checkpoint* Graph::SnapshotBefore(BytecodeInfoProvider* info, Builder* builder,
                                  Checkpoint::Reason reason) {
  auto snapshot = AddOp(info, Opcode::kSnapshot);
  return AddCheckpointBefore(info, nullptr, builder, reason, snapshot);
}

Checkpoint* Graph::SnapshotAfter(BytecodeInfoProvider* info, Builder* builder,
                                 Checkpoint::Reason reason) {
  auto snapshot = AddOp(info, Opcode::kSnapshot);
  return AddCheckpointAfter(info, nullptr, builder, reason, snapshot);
}

Checkpoint* Graph::ReemitCheckpointBefore(BytecodeInfoProvider* info,
                                          Checkpoint* prev, Builder* builder,
                                          Checkpoint* ref, Node* node) {
  auto reason = ref->reason();

  if (prev && prev->CanMerge(reason)) {
    prev->Snapshot(this, builder);
    prev->Relink(node, reason, false);
    return prev;
  } else {
    auto id = static_cast<CheckpointId>(checkpoint_.size());
    auto c = zone()->New<Checkpoint>(quake_, id, reason, node, false);
    c->Recreate(this, builder, ref);
    checkpoint_.emplace_back(c);
    return c;
  }
}

Checkpoint* Graph::ReemitCheckpointAfter(BytecodeInfoProvider* info,
                                         Checkpoint* prev, Builder* builder,
                                         Checkpoint* ref, Node* node) {
  auto reason = ref->reason();

  if (prev && prev->CanMerge(reason)) {
    prev->Snapshot(this, builder);
    prev->Relink(node, reason, true);
    return prev;
  } else {
    auto id = static_cast<CheckpointId>(checkpoint_.size());
    auto c = zone()->New<Checkpoint>(quake_, id, reason, node, true);
    c->Recreate(this, builder, ref);
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

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode, const Type& t,
                     std::size_t size) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, t, size);
  return AfterAlloc(node);
}

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode, const Type& t) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, t);
  return AfterAlloc(node);
}

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode, const Type& t,
                     SSAVar* ssa_var) {
  auto node =
      zone()->New<Node>(quake_, info->bytecode_info(), opcode, t, ssa_var);
  return AfterAlloc(node);
}

Node* Graph::AddNode(BytecodeInfoProvider* info, Opcode opcode, const Type& t,
                     SSAVar* opr1, SSAVar* opr2) {
  auto node =
      zone()->New<Node>(quake_, info->bytecode_info(), opcode, t, opr1, opr2);
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

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode,
                        const Type& t, std::size_t size) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  CHECK(GetOpcodeInfo(opcode).IsNary());

  auto node = AddNode(info, opcode, t, size);
  return AddSSAVar(node);
}

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode,
                        const Type& t) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  auto node = AddNode(info, opcode, t);
  return AddSSAVar(node);
}

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode,
                        const Type& t, SSAVar* opr) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  auto node = AddNode(info, opcode, t, opr);
  return AddSSAVar(node);
}

SSAVar* Graph::AddValue(BytecodeInfoProvider* info, Opcode opcode,
                        const Type& t, SSAVar* opr1, SSAVar* opr2) {
  CHECK(GetOpcodeInfo(opcode).effect.IsValue());
  auto node = AddNode(info, opcode, t, opr1, opr2);
  return AddSSAVar(node);
}

SSAVar* Graph::LoadStr(BytecodeInfoProvider* info,
                       const std::string_view& value) {
  auto kstr = KStr(info, value);
  return AddValue(info, Opcode::kLoadStr, kstr);
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

Node* Graph::AddControlFlow(BytecodeInfoProvider* info, Opcode opcode) {
  CHECK(GetOpcodeInfo(opcode).effect.IsControlFlow());
  return AddNode(info, opcode);
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode,
                             std::size_t size) {
  CHECK(GetOpcodeInfo(opcode).IsNary());

  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, size);
  return AfterAllocInternal(node);
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode);
  return AfterAllocInternal(node);
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode,
                             SSAVar* ssa_var) {
  auto node = zone()->New<Node>(quake_, info->bytecode_info(), opcode, ssa_var);
  return AfterAllocInternal(node);
}

Node* Graph::AddNodeInternal(BytecodeInfoProvider* info, Opcode opcode,
                             SSAVar* opr1, SSAVar* opr2) {
  auto node =
      zone()->New<Node>(quake_, info->bytecode_info(), opcode, opr1, opr2);
  return AfterAllocInternal(node);
}

SSAVar* Graph::SetNodeInternalSSA(Node* node) {
  auto ssa_var = zone()->New<SSAVar>(quake_, kInternalIndex, node);
  return ssa_var;
}

template <typename F>
Node* Graph::FindNode(Opcode kind, F&& func) {
  for (auto n = slist_[static_cast<std::size_t>(kind)]; n; n = n->chain()) {
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
  return AddSSAVar(AfterAllocInternal(new_node));
}

SSAVar* Graph::KI64(BytecodeInfoProvider* info, std::int64_t value) {
  if (auto node = FindNode(
          Opcode::kKI64, [&](Node* n) -> bool { return n->Int64() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAllocInternal(new_node));
}

SSAVar* Graph::KPtr(BytecodeInfoProvider* info, const void* ptr) {
#ifdef QUAKE_TARGET_X64
  return KI64(info, base::TypePuneCast<std::int64_t>(ptr));
#else
#error "unknown"
#endif  // QUAKE_TRAGET_X64
}

SSAVar* Graph::KFloat64(BytecodeInfoProvider* info, double value) {
  if (auto node =
          FindNode(Opcode::kKFloat64,
                   [&](Node* n) -> bool { return n->Float64() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAllocInternal(new_node));
}

SSAVar* Graph::KBool(BytecodeInfoProvider* info, bool value) {
  if (auto node = FindNode(Opcode::kKBool,
                           [&](Node* n) -> bool { return n->Bool() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAllocInternal(new_node));
}

SSAVar* Graph::KNull(BytecodeInfoProvider* info) {
  if (auto n = slist_[static_cast<std::size_t>(Opcode::kKNull)]; n) {
    CHECK(n->IsNull());
    return n->out();
  }
  auto new_node =
      zone()->New<Node>(quake_, info->bytecode_info(), Node::Null{});
  return AddSSAVar(AfterAllocInternal(new_node));
}

SSAVar* Graph::KUndefined(BytecodeInfoProvider* info) {
  if (auto n = slist_[static_cast<std::size_t>(Opcode::kKUndef)]; n) {
    CHECK(n->IsUndefined());
    return n->out();
  }
  auto new_node =
      zone()->New<Node>(quake_, info->bytecode_info(), Node::Undefined{});
  return AddSSAVar(AfterAllocInternal(new_node));
}

SSAVar* Graph::KStr(BytecodeInfoProvider* info, const std::string_view& value) {
  if (auto node = FindNode(Opcode::kKStr,
                           [&](Node* n) -> bool { return n->Str() == value; });
      node) {
    return node->out();
  }
  auto new_node = zone()->New<Node>(quake_, info->bytecode_info(), value);
  return AddSSAVar(AfterAllocInternal(new_node));
}

SSAVar* Graph::KUninit(BytecodeInfoProvider* info) {
  if (auto n = slist_[static_cast<std::size_t>(Opcode::kKUninit)]; n) {
    CHECK(n->IsUninit());
    return n->out();
  }

  auto new_node =
      zone()->New<Node>(quake_, info->bytecode_info(), Node::Uninit{});
  return AddSSAVar(AfterAllocInternal(new_node));
}

// ============================================================================
// Structure
void Graph::set_end_of_bb0(Node* end_of_bb0) {
  CHECK(end_of_bb0->opcode() == Opcode::kBB1);
  end_of_bb0_ = end_of_bb0;
}

void Graph::set_end_of_bb1(Node* end_of_bb1) { end_of_bb1_ = end_of_bb1; }

void Graph::ForeachBB0(std::function<void(Node*)>&& visitor) const {
  Node* next;
  for (auto n = start_node(); n != end_node(); n = next) {
    next = n->next();

    if (n == end_of_bb0_) break;
    if (n->IsDead()) continue;
    visitor(n);
  }
}

bool Graph::ForeachBB0Stoppable(std::function<bool(Node*)>&& visitor) const {
  Node* next;
  for (auto n = start_node(); n != end_node(); n = next) {
    next = n->next();

    if (n == end_of_bb0_) break;
    if (n->IsDead()) continue;
    if (!visitor(n)) return false;
  }
  return true;
}

void Graph::ForeachBB1(std::function<void(Node*)>&& visitor) const {
  CHECK(end_of_bb0_);
  CHECK(end_of_bb1_);

  Node* next;
  for (auto n = end_of_bb0_->next(); n != end_node(); n = next) {
    next = n->next();

    if (n == end_of_bb1_) break;
    if (n->IsDead()) continue;
    visitor(n);
  }
}

bool Graph::ForeachBB1Stoppable(std::function<bool(Node*)>&& visitor) const {
  CHECK(end_of_bb0_);
  CHECK(end_of_bb1_);

  Node* next;
  for (auto n = end_of_bb0_->next(); n != end_node(); n = next) {
    next = n->next();

    if (n == end_of_bb1_) break;
    if (n->IsDead()) continue;
    if (!visitor(n)) return false;
  }
  return true;
}

void Graph::Foreach(std::function<void(Node*)>&& visitor) const {
  Node* next;
  for (auto n = start_node(); n != end_node(); n = next) {
    next = n->next();

    if (n->IsDead()) continue;
    visitor(n);
  }
}

void Graph::ForeachInternal(std::function<void(Node*)>&& visitor) const {
  for (auto n : internal_node_) {
    if (n->IsDead()) continue;
    visitor(n);
  }
}

bool Graph::ForeachStoppable(std::function<bool(Node*)>&& visitor) const {
  Node* next;

  for (auto n = start_node(); n != end_node(); n = next) {
    next = n->next();

    if (n->IsDead()) continue;
    if (!visitor(n)) return false;
  }
  return true;
}

void Graph::ForeachCheckpoint(
    std::function<void(Checkpoint*)>&& visitor) const {
  for (auto& n : checkpoint_) {
    if (n->IsDead()) continue;
    visitor(n);
  }
}

std::vector<Node*> Graph::GetNodeList() const {
  std::vector<Node*> o;
  Foreach([&](Node* n) { o.push_back(n); });
  return o;
}

// ============================================================================
// Stringify
std::string Graph::Print() const {
  std::stringstream fmt;
  Foreach([&](Node* n) {
    if (n->opcode() == Opcode::kBB1) {
      fmt << "! ---------------------------------------------------------\n";
      fmt << "! ------------------------ BB0 done -----------------------\n";
      fmt << "! ---------------------------------------------------------\n";
    }
    fmt << n->Print() << "\n";

    if (auto snap = n->snapshot_before(); snap) {
      fmt << snap->Print();
    }

    if (auto snap = n->snapshot_after(); snap) {
      fmt << snap->Print();
    }
  });
  return fmt.str();
}

std::string Graph::PrintCompact() const {
  std::stringstream fmt;
  Foreach([&](Node* n) {
    if (n->opcode() == Opcode::kBB1) {
      fmt << "! ---------------------------------------------------------\n";
      fmt << "! ------------------------ BB0 done -----------------------\n";
      fmt << "! ---------------------------------------------------------\n";
    }
    fmt << n->Print() << "\n";
  });
  return fmt.str();
}

std::string Graph::PrintGraphviz() const {
  std::stringstream fmt;
  fmt << "digraph IR {\n";

  {
    std::string prev;
    Foreach([&](Node* n) {
      fmt << n->PrintGraphNode();
      fmt << n->PrintGraphLink();
      auto name = n->PrintGraphName();
      if (!prev.empty()) {
        fmt << prev << " -> " << name << "[style=bold penwidth=4.0]\n";
      }
      prev = name;
    });
  }

  ForeachInternal([&](Node* n) {
    fmt << n->PrintGraphNode();
    fmt << n->PrintGraphLink();
  });

  ForeachCheckpoint([&](Checkpoint* cp) {
    fmt << cp->PrintGraphNode();
    fmt << cp->PrintGraphLink();
  });

  fmt << "}";

  return fmt.str();
}

}  // namespace ir
}  // namespace quake
