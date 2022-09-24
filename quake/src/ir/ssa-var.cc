#include "ir/ssa-var.h"
#include "ir/node.h"

namespace quake {
namespace ir {

SSAVar::SSAVar(Quake* q, std::uint32_t index, Node* n)
    : SSARefList(q->zone()),
      quake_(q),
      index_(index),
      flag_(),
      type_(n->type()),
      node_(n) {}

std::optional<SSARef> SSARefList::FindUse(Node* n, int idx) const {
  auto itr = std::find_if(use_list_.begin(), use_list_.end(),
                          [&](const SSARef& ref) -> bool {
                            return ref.who == n && ref.index == idx;
                          });

  if (itr == use_list_.end()) {
    return {};
  } else {
    return *itr;
  }
}

std::size_t SSARefList::HasUse(Node* n) const {
  return std::count_if(use_list_.begin(), use_list_.end(),
                       [&](const SSARef& ref) -> bool { return ref.who == n; });
}

bool SSARefList::RemoveUse(Node* n, int idx) {
  auto itr = std::find_if(use_list_.begin(), use_list_.end(),
                          [&](const SSARef& ref) -> bool {
                            return ref.who == n && ref.index == idx;
                          });

  if (itr != use_list_.end()) {
    use_list_.erase(itr);
    return true;
  } else {
    return false;
  }
}

std::size_t SSARefList::RemoveUse(Node* n) {
  auto itr =
      std::remove_if(use_list_.begin(), use_list_.end(),
                     [&](const SSARef& ref) -> bool { return ref.who == n; });
  auto cnt = std::distance(itr, use_list_.end());
  use_list_.erase(itr, use_list_.end());
  return cnt;
}

bool SSARefList::AddUse(const SSARef& ref) {
  if (HasUse(ref.who, ref.index)) {
    return false;
  } else {
    use_list_.emplace_back(ref);
    return true;
  }
}

}  // namespace ir
}  // namespace quake
