#ifndef _QUAKE_DEFUSE_LIST_H_
#define _QUAKE_DEFUSE_LIST_H_
#include "base.h"
#include "zone.h"

namespace quake {

template <typename D, typename R>
struct DefUseRef {
  D* def;
  int index;
  R** ref;

  DefUseRef(D* d, int i, R** r) : def(d), index(i), ref(r) {}

  static DefUseRef Make(D* d, int i, R** r) { return DefUseRef{d, i, r}; }
};

// To correctly use this DefUseRefList, one must *inherit* it. The Replace
// function assume the R is inherited from the DefUseRefList, otherwise you
// will get compilation error when using Replace function
template <typename D, typename R>
class DefUseRefList {
 public:
  using Ref = DefUseRef<D, R>;
  using Self = DefUseRefList<D, R>;

  explicit DefUseRefList(zone::Zone* zone) : use_list_(zone) {}

  // Find the ref record of (D*, int)
  std::optional<Ref> FindUse(D*, int) const;

  // Check whether (D*, int) uses the R or not
  bool HasUse(D* n, int i) const { return FindUse(n, i).has_value(); }
  std::size_t HasUse(D*) const;

  bool RemoveUse(D*, int);
  std::size_t RemoveUse(D*);

  bool AddUse(const Ref&);
  bool AddUse(D* n, int i, R** ref) { return AddUse(Ref::Make(n, i, ref)); }
  void AddUseChecked(D* n, int i, R** ref) { CHECK(AddUse(n, i, ref)); }

  std::size_t UseSize() const { return use_list_.size(); }
  bool HasUse() const { return !use_list_.empty(); }

  bool SingleUseBy(D* x) const { return HasUse(x) == 1; }

  // Replace all UseDef with another specified. Returns the replaced operand
  // size internally
  std::size_t Replace(R* new_r);

  template <typename F>
  void ForeachUse(F&& f) const {
    for (const auto& entry : use_list_) {
      if (!f(entry)) break;
    }
  }

 private:
  zone::Vector<Ref> use_list_;
};

template <typename D, typename R>
std::optional<typename DefUseRefList<D, R>::Ref> DefUseRefList<D, R>::FindUse(
    D* n, int idx) const {
  auto itr = std::find_if(
      use_list_.begin(), use_list_.end(),
      [&](const Ref& ref) -> bool { return ref.def == n && ref.index == idx; });

  if (itr == use_list_.end()) {
    return {};
  } else {
    return *itr;
  }
}

template <typename D, typename R>
std::size_t DefUseRefList<D, R>::HasUse(D* n) const {
  return std::count_if(use_list_.begin(), use_list_.end(),
                       [&](const Ref& ref) -> bool { return ref.def == n; });
}

template <typename D, typename R>
bool DefUseRefList<D, R>::RemoveUse(D* n, int idx) {
  auto itr = std::find_if(
      use_list_.begin(), use_list_.end(),
      [&](const Ref& ref) -> bool { return ref.def == n && ref.index == idx; });

  if (itr != use_list_.end()) {
    use_list_.erase(itr);
    return true;
  } else {
    return false;
  }
}

template <typename D, typename R>
std::size_t DefUseRefList<D, R>::RemoveUse(D* n) {
  auto itr =
      std::remove_if(use_list_.begin(), use_list_.end(),
                     [&](const Ref& ref) -> bool { return ref.def == n; });
  auto cnt = std::distance(itr, use_list_.end());
  use_list_.erase(itr, use_list_.end());
  return cnt;
}

template <typename D, typename R>
bool DefUseRefList<D, R>::AddUse(const Ref& ref) {
  if (HasUse(ref.def, ref.index)) {
    return false;
  } else {
    use_list_.emplace_back(ref);
    return true;
  }
}

template <typename D, typename R>
std::size_t DefUseRefList<D, R>::Replace(R* new_r) {
  auto cnt = use_list_.size();
  for (auto& u : use_list_) {
    *u.ref = new_r;
    new_r->AddUseChecked(u.def, u.index, u.ref);
  }
  use_list_.clear();
  return cnt;
}

}  // namespace quake

#endif  // _QUAKE_DEFUSE_LIST_H_
