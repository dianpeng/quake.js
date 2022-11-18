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
    case Reason::kBB1:
      return "bb1";
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

void Checkpoint::EntryVector::Reserve(std::size_t size) { Base::reserve(size); }

Checkpoint::Entry& Checkpoint::EntryVector::PushBack() {
  // Make sure the PushBack will never allocate new memory, otherwise we are
  // buggy at recording all the address into use-def chain
  CHECK(Base::capacity() > Base::size());

  auto idx = static_cast<std::uint32_t>(Base::size());
  Base::emplace_back(Entry{checkpoint_, idx});
  return Base::back();
}

void Checkpoint::EntryVector::PushBack(EntryType etype, SSAVar* ssa_var,
                                       const Origin& origin) {
  PushBack().Set(etype, ssa_var, origin);
}

void Checkpoint::EntryVector::PushBack(SSAVar* frame_ins) {
  PushBack().Set(frame_ins);
}

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

void Checkpoint::Entry::Set(SSAVar* ssa_var) {
  Reset();

  type_ = kFrame;
  value_ = ssa_var;
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
      frame_vector_(q->zone()) {
  SetSnapshot(node, is_after);
}

const void* Checkpoint::TopFrameBCPos() const {
  const auto& last_frame = frame_vector_.back();
  return last_frame.cur_bc;
}

std::size_t Checkpoint::ForeachTopFrame(
    std::function<void(const Entry&)>&& visitor) const {
  const auto& frame = frame_vector_.back();

  std::size_t sz = 0;
  for (auto itr = frame.value_list.rbegin(); itr != frame.value_list.rend();
       itr++) {
    visitor(*itr);
    ++sz;
  }
  return sz;
}

std::size_t Checkpoint::TopFrameSize() const {
  return frame_vector_.back().value_list.size();
}

std::size_t Checkpoint::TotalEntrySize() const {
  std::size_t cnt = 0;
  for (const auto& frame : frame_vector_) {
    cnt += frame.value_list.size();
  }
  return cnt;
}

std::size_t Checkpoint::ForeachEntry(
    std::function<void(const Entry&)>&& visitor) const {
  std::size_t cnt = 0;
  for (const auto& frame : frame_vector_) {
    for (const auto& v : frame.value_list) {
      visitor(v);
      ++cnt;
    }
  }
  return cnt;
}

std::size_t Checkpoint::ForeachEntry(std::function<void(Entry&)>&& visitor) {
  std::size_t cnt = 0;
  for (auto& frame : frame_vector_) {
    for (auto& v : frame.value_list) {
      visitor(v);
      ++cnt;
    }
  }
  return cnt;
}

void Checkpoint::SetSnapshot(Node* node, bool is_after) {
  if (is_after) {
    CHECK(node->snapshot_after_ == nullptr);
    node->snapshot_after_ = this;
  } else {
    CHECK(node->snapshot_before_ == nullptr);
    node->snapshot_before_ = this;
  }
  happen_before_ = !is_after;
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
void Checkpoint::Snapshot(Graph* graph, Builder* builder) {
  frame_vector_.clear();

  auto materialize_one_frame = [&](Builder::IRFrame& frame) {
    frame_vector_.emplace_back(this, quake_->zone(), frame.type(),
                               frame.cur_bc(), frame.next_bc(),
                               frame.slot_size(), frame.param_size(),
                               frame.local_size(), frame.frame_instruction());

    auto& f = frame_vector_.back();
    f.frame_instruction = frame.frame_instruction();

    // NOTES(dpeng): Until we use a memory persistent data structure, like
    //   std::list, we will have to make sure the value vector is been
    //   allocated at first then start to do the push. The reaosn is because
    //   we gonna record the position in use-def chain to allow O(1) replace
    f.value_list.Reserve(frame.slot_size());

    // 1) Parameters
    auto index = 0;
    for (auto start = frame.param(); start < frame.param_end(); start++) {
      if (*start != nullptr) {
        f.value_list.PushBack(kVal, *start, Origin::FromParam(index));
      }
      ++index;
    }

    // 2) Local variables
    index = 0;
    for (auto start = frame.local(); start < frame.local_end(); start++) {
      if (*start != nullptr) {
        f.value_list.PushBack(kVal, *start, Origin::FromLocal(index));
      }
      ++index;
    }

    // 3) Temporary
    index = 0;
    for (auto start = frame.temp(); start < frame.temp_end(); start++) {
      if (*start != nullptr) {
        f.value_list.PushBack(kVal, *start, Origin::FromTemp(index));
      }
      ++index;
    }
  };

  for (auto& frame : builder->frame()) {
    materialize_one_frame(frame);
  }
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

std::string Checkpoint::Frame::PrintGraphLink(const std::string& name) const {
  std::stringstream fmt;
  for (const auto& entry : value_list) {
    fmt << name << " -> " << entry.value()->node()->PrintGraphName()
        << " [style=bold color=pink penwidth=1.5 label=\"" << entry.type_name()
        << "," << entry.origin().Print() << "\"]\n";
  }

  return fmt.str();
}

std::string Checkpoint::Frame::Print() const {
  std::stringstream fmt;

  // (1) Printing out all the snapshotted information here, in a blob
  {
    fmt << "\nframe[\n";
    for (const auto& entry : value_list) {
      fmt << "  " << entry.Print() << "; \n";
    }
    fmt << "]\n";
  }

  return fmt.str();
}

std::string Checkpoint::PrintGraphLink() const {
  std::stringstream fmt;
  auto name = PrintGraphName();

  for (const auto& frame : frame_vector_) {
    fmt << frame.PrintGraphLink(name);
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
    for (const auto& frame : frame_vector_) {
      fmt << frame.Print();
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
