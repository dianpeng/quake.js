#include "ir/checkpoint.h"
#include "ir/builder.h"

namespace quake {
namespace ir {

using namespace base;

const char* Checkpoint::GetReasonName(Checkpoint::Reason reason) {
  switch (reason) {
    case Reason::kSideEffect:
      return "side-effect";
    case Reason::kRefCount:
      return "ref-count";
    case Reason::kControlFlow:
      return "control-flow";
    case Reason::kStart:
      return "start";
    case Reason::kEnd:
      return "end";
    default:
      UNREACHABLE();
      return "";
  }
}

const char* Checkpoint::GetTypeName(Type t) {
  switch (t) {
    case Type::kLocal:
      return "local";
    case Type::kParam:
      return "param";
    case Type::kTemp:
      return "temp";
    default:
      UNREACHABLE();
      return "";
  }
}

const char* Checkpoint::GetEntryTypeName(EntryType et) {
  switch (et) {
    case EntryType::kVal:
      return "value";
    case EntryType::kFrame:
      return "frame";
    default:
      UNREACHABLE();
      return "";
  }
}

std::string Checkpoint::Origin::Print() const {
  return Fmt("(%s,%d)", GetTypeName(type), offset);
}

std::string Checkpoint::Entry::Print() const {
  return Fmt("%s(%s,%s)", value_->node()->PrintRefId().c_str(), type_name(),
             origin_.Print().c_str());
}

Checkpoint::Entry& Checkpoint::EntryVector::PushBack() {
  auto idx = static_cast<std::uint32_t>(Base::size());
  Base::emplace_back(Entry{checkpoint_, idx});
  return Base::back();
}

void Checkpoint::EntryVector::PushBack(EntryType etype, SSAVar* ssa_var,
                                       const Origin& origin) {
  PushBack().Set(etype, ssa_var, origin);
}

void Checkpoint::EntryVector::PushBack(const void* bc) { PushBack().Set(bc); }

void Checkpoint::EntryVector::PopBack() {
  CHECK(!Base::empty());
  Base::back().Reset();
  Base::pop_back();
}

void Checkpoint::EntryVector::Clear() {
  for (auto& e : *this) {
    e.Reset();
  }
  Base::clear();
}

void Checkpoint::Entry::Set(EntryType t, SSAVar* ssa_var, const Origin& o) {
  Reset();

  type_ = t;
  value_ = ssa_var;
  origin_ = o;
  CHECK(ssa_var->AddUse(checkpoint_, index_, &value_));
}

void Checkpoint::Entry::SetValue(SSAVar* v) {
  auto otype = type_;
  auto oorigin = origin_;
  Reset();
  Set(otype, v, oorigin);
}

void Checkpoint::Entry::Reset() {
  if (value_) {
    CHECK(value_->RemoveUse(checkpoint_, index_));
    value_ = nullptr;
  }
}

Checkpoint::Checkpoint(Quake* q, CheckpointId id, Reason reason, Node* node,
                       bool is_after)
    : quake_(q),
      id_(id),
      deopt_id_(kInvalidDeoptId),
      flag_(),
      reason_(reason),
      node_(node),
      exit_(),
      jump_from_(q->zone()),
      entry_vector_(this, q->zone()) {
  SetSnapshot(node, is_after);
}

const void* Checkpoint::TopFrameBCPos() const {
  for (auto itr = entry_vector_.rbegin(); itr != entry_vector_.rend(); itr++) {
    if (itr->type() == kFrame) {
      return itr->bc_pos();
    }
  }
  UNREACHABLE();
  return nullptr;
}

std::size_t Checkpoint::ForeachTopFrameValue(
    std::function<void(const Entry&)>&& visitor) const {
  std::size_t sz = 0;
  for (auto itr = entry_vector_.rbegin(); itr != entry_vector_.rend(); itr++) {
    if (itr->type() == kFrame) {
      break;
    } else {
      visitor(*itr);
    }
    ++sz;
  }
  return sz;
}

std::size_t Checkpoint::ForeachTopFrame(
    std::function<void(const Entry&)>&& visitor) const {
  std::size_t sz = 0;
  for (auto itr = entry_vector_.rbegin(); itr != entry_vector_.rend(); itr++) {
    ++sz;
    if (itr->type() == kFrame) {
      visitor(*itr);
      break;
    } else {
      visitor(*itr);
    }
  }
  return sz;
}

std::size_t Checkpoint::TopFrameSize() const {
  std::size_t sz = 0;
  for (auto itr = entry_vector_.rbegin(); itr != entry_vector_.rend(); itr++) {
    ++sz;
    if (itr->type() == kFrame) {
      break;
    }
  }
  return sz;
}

std::size_t Checkpoint::TopFrameValueSize() const { return TopFrameSize() - 1; }

void Checkpoint::SetSnapshot(Node* node, bool is_after) {
  if (is_after) {
    CHECK(node->snapshot_after_ == nullptr);
    node->snapshot_after_ = this;
  } else {
    CHECK(node->snapshot_before_ == nullptr);
    node->snapshot_before_ = this;
  }
}

void Checkpoint::Relink(Node* new_node, bool is_after) {
  if (node_) {
    // it is not possible that a node's before/after checkpoint
    node_->RemoveSnapshot(this);
    node_ = new_node;
    SetSnapshot(node_, is_after);
  }
}

// ----------------------------------------------------------------------------
// Snapshot/Checkpoint
void Checkpoint::Snapshot(Graph* graph, Builder* builder, const void* bc) {
  int index;

  entry_vector_.Clear();
  entry_vector_.reserve(
      builder->slot_size());  // make sure do not reallocate memory

  entry_vector_.PushBack(bc);  // push current bytecode

  // 1) Parameters
  index = 0;
  for (auto start = builder->param(); start < builder->param_end(); start++) {
    if (*start != nullptr) {
      entry_vector_.PushBack(kVal, *start, Origin::FromParam(index));
    }
    ++index;
  }

  // 2) Local variables
  index = 0;
  for (auto start = builder->local(); start < builder->local_end(); start++) {
    if (*start != nullptr) {
      entry_vector_.PushBack(kVal, *start, Origin::FromLocal(index));
    }
    ++index;
  }

  // 3) Temporary
  index = 0;
  for (auto start = builder->temp(); start < builder->temp_end(); start++) {
    if (*start != nullptr) {
      entry_vector_.PushBack(kVal, *start, Origin::FromTemp(index));
    }
    ++index;
  }

  // --------------------------------------------------------------------------
  // 4) Adding frame link
}

// Notes, the merge essentially means the previous saved checkpoint will be
// eliminated, and will be replaced with the new checkpoint
bool Checkpoint::CanMerge(Checkpoint::Reason current_reason) const {
  UNUSE(current_reason);

  // If a checkpoint has no jump from, then we can reuse it, ie forwarding
  // it. For now, we do not try to merge checkpoint that is resulted from a
  // control flow node. Checkpoint will not result in any runtime instructions
  // after code generation. The merge is merely for simplicty of optimization
  // passes and save some memory.
  if (!HasJumpFrom()) {
    switch (reason_) {
      case Reason::kControlFlow:
      case Reason::kBB1:
      case Reason::kStart:
      case Reason::kEnd:
        return false;
      default:
        break;
    }
    CHECK(!exit().IsUsed());
    return true;
  }
  return false;
}

bool Checkpoint::RemoveJumpFrom(Node* where) {
  auto itr = std::find_if(jump_from_.begin(), jump_from_.end(),
                          [&](Node* that) -> bool { return where == that; });
  if (itr != jump_from_.end()) {
    jump_from_.erase(itr);
    return true;
  }
  return false;
}

bool Checkpoint::ReplaceJumpFrom(Node* where, Node* to) {
  auto itr = std::find_if(jump_from_.begin(), jump_from_.end(),
                          [&](Node* that) -> bool { return where == that; });
  if (itr != jump_from_.end()) {
    *itr = to;
    return true;
  }
  return false;
}

std::string Checkpoint::PrintGraphName() const {
  return Fmt("checkpoint_%u", id_);
}

std::string Checkpoint::PrintGraphNode() const {
  auto name = PrintGraphName();
  return Fmt("%s [color=purple shape=square label=\"checkpoint_%u\"]",
             name.c_str(), id_);
}

std::string Checkpoint::PrintGraphLink() const {
  std::stringstream fmt;
  auto name = PrintGraphName();

  for (const auto& entry : entry_vector_) {
    fmt << name << " -> " << entry.value()->node()->PrintGraphName()
        << " [style=bold color=pink penwidth=1.5 label=\"" << entry.type_name()
        << "," << entry.origin().Print() << "\"]\n";
  }

  for (const auto& node : jump_from_) {
    fmt << node->PrintGraphName() << " -> " << name
        << " [style=bold color=brown penwidth=2.0 label=\"B\"]\n";
  }

  fmt << node_->PrintGraphName() << " -> " << name
      << " [style=bold color=gold penwidth=4.0 label=\"G\"]\n";

  return fmt.str();
}

std::string Checkpoint::Print() const {
  std::stringstream fmt;

  fmt << "$checkpoint(" << id_ << "," << reason_name() << "): ";

  // (0) Printing out the node that generates the current checkpoint. Notes
  //     the node that generates such checkpoint must be none internal.
  CHECK(!node_->IsInternal());
  fmt << node_->PrintRefId() << "; ";

  // (1) Printing out all the snapshotted information here, in a blob
  {
    fmt << "\nsnapshot[\n";
    for (const auto& entry : entry_vector_) {
      fmt << "  " << entry.Print() << "; \n";
    }
    fmt << "]\n";
  }

  // (2) Printing out which node jumps to this checkpoint, mostly afterwards
  {
    fmt << "bailout[";
    for (const auto& node : jump_from_) {
      fmt << node->PrintRefId() << "; ";
    }
    fmt << "]";
  }

  fmt << "\n";

  return fmt.str();
}

}  // namespace ir
}  // namespace quake
