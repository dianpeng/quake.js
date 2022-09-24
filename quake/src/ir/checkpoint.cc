#include "ir/checkpoint.h"
#include "ir/builder.h"

namespace quake {
namespace ir {

Checkpoint::Checkpoint(Quake* q, Reason reason, Node* node, void* bc)
    : quake_(q),
      flag_(),
      index_(0),
      reason_(reason),
      node_(node),
      bc_pos_(bc),
      exit_(),
      jump_from_(q->zone()),
      entry_vector_(q->zone()) {
  CHECK(node->snapshot_ == nullptr);
  node->snapshot_ = this;
}

// ----------------------------------------------------------------------------
// Snapshot/Checkpoint
void Checkpoint::Snapshot(Graph* graph, Builder* builder) {
  int index;

  // Notes, we should clear it since DoSnapshot maybe invoked again on an
  // existed checkpoint due to the checkpoint merge
  entry_vector_.clear();

  // 1) Parameters
  index = 0;
  for (auto start = builder->param(); start < builder->param_end(); start++) {
    if (*start != nullptr) {
      entry_vector_.emplace_back(kVal, *start, Origin::FromParam(index));
    }
    ++index;
  }

  // 2) Local variables
  index = 0;
  for (auto start = builder->local(); start < builder->local_end(); start++) {
    if (*start != nullptr) {
      entry_vector_.emplace_back(kVal, *start, Origin::FromLocal(index));
    }
    ++index;
  }

  // 3) Temporary
  index = 0;
  for (auto start = builder->temp(); start < builder->temp_end(); start++) {
    if (*start != nullptr) {
      entry_vector_.emplace_back(kVal, *start, Origin::FromTemp(index));
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

  // If a checkpoint has no jump from, then we can reuse it, ie forwarding it
  if (!HasJumpFrom()) {
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

}  // namespace ir
}  // namespace quake
