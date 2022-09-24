#ifndef _BASE_CONTAINER_CACHED_LIST_INL_H_
#define _BASE_CONTAINER_CACHED_LIST_INL_H_

#include "cached-list.h"

namespace base {

/* -----------------------------------------------------
 *  Node
 * ---------------------------------------------------*/
template <typename T, template <class> typename Alloc>
inline bool CachedList<T, Alloc>::NodeBase::IsUnlink() const {
  return prev == this && next == this;
}

template <typename T, template <class> typename Alloc>
inline void CachedList<T, Alloc>::NodeBase::Ln(NodeBase* b) {
  DCHECK(b);
  DCHECK(b->prev);
  DCHECK(IsUnlink());

  auto prev = b->prev;
  prev->next = this;
  this->prev = prev;
  b->prev = this;
  next = b;
}

template <typename T, template <class> typename Alloc>
inline void CachedList<T, Alloc>::NodeBase::Rm() {
  prev->next = next;
  next->prev = prev;

  prev = this;
  next = this;
}

template <typename T, template <class> typename Alloc>
inline void CachedList<T, Alloc>::NodeBase::Swap(NodeBase* that) {
  // Swapping a node with another node is none trivial, the issue is the node
  // may point itself, ie the tail node serves as senetry. Since the node can
  // be embedded as part of the |LIST| so a simple swap just wont' work here.
  //
  // The following code is based on the std::list's implementation, it is known
  // to be bug free since handling self pointing node is a pain in the ass :)
  NodeBase& this_node = *this;
  NodeBase& that_node = *that;

  if (this_node.next != &this_node) {
    if (that_node.next != &that_node) {
      // Both this_node and that_node are not empty.
      std::swap(this_node.next, that_node.next);
      std::swap(this_node.prev, that_node.prev);
      this_node.next->prev = this_node.prev->next = &this_node;
      that_node.next->prev = that_node.prev->next = &that_node;
    } else {
      // this_node is not empty, that_node is empty.
      that_node.next = this_node.next;
      that_node.prev = this_node.prev;
      that_node.next->prev = that_node.prev->next = &that_node;
      this_node.next = this_node.prev = &this_node;
    }
  } else if (that_node.next != &that_node) {
    // this_node is empty, that_node is not empty.
    this_node.next = that_node.next;
    this_node.prev = that_node.prev;
    this_node.next->prev = this_node.prev->next = &this_node;
    that_node.next = that_node.prev = &that_node;
  } else {
    // Notes, if both node are empty, then no need to do the swap, since
    // self pointing node means empty
  }
}

/* -----------------------------------------------------
 *  CachedListIterator
 * ---------------------------------------------------*/
template <typename T, template <class> typename Alloc>
inline bool CachedList<T, Alloc>::CachedListIterator::operator==(
    const CachedListIterator& itr) const {
  return node_ == itr.node_;
}

template <typename T, template <class> typename Alloc>
inline bool CachedList<T, Alloc>::CachedListIterator::operator!=(
    const CachedListIterator& itr) const {
  return node_ != itr.node_;
}

template <typename T, template <class> typename Alloc>
inline typename CachedList<T, Alloc>::CachedListIterator&
CachedList<T, Alloc>::CachedListIterator::operator++() const {
  DCHECK(node_);
  DCHECK(node_->next);

  node_ = node_->next;
  return mutable_self();
}

template <typename T, template <class> typename Alloc>
inline typename CachedList<T, Alloc>::CachedListIterator
CachedList<T, Alloc>::CachedListIterator::operator++(int) const {
  DCHECK(node_);
  DCHECK(node_->next);

  auto r{*this};
  node_ = node_->next;
  return r;
}

template <typename T, template <class> typename Alloc>
inline typename CachedList<T, Alloc>::CachedListIterator&
CachedList<T, Alloc>::CachedListIterator::operator--() const {
  DCHECK(node_);
  DCHECK(node_->prev);

  node_ = node_->prev;
  return mutable_self();
}

template <typename T, template <class> typename Alloc>
inline typename CachedList<T, Alloc>::CachedListIterator
CachedList<T, Alloc>::CachedListIterator::operator--(int) const {
  DCHECK(node_);
  DCHECK(node_->prev);

  auto r{*this};
  node_ = node_->prev;
  return r;
}

template <typename T, template <class> typename Alloc>
inline T& CachedList<T, Alloc>::CachedListIterator::operator*() {
  DCHECK(node_);
  return static_cast<Node*>(node_)->value;
}

template <typename T, template <class> typename Alloc>
inline const T& CachedList<T, Alloc>::CachedListIterator::operator*() const {
  DCHECK(node_);
  return static_cast<Node*>(node_)->value;
}

template <typename T, template <class> typename Alloc>
inline T* CachedList<T, Alloc>::CachedListIterator::operator->() {
  DCHECK(node_);
  return &(static_cast<Node*>(node_)->value);
}

template <typename T, template <class> typename Alloc>
inline const T* CachedList<T, Alloc>::CachedListIterator::operator->() const {
  DCHECK(node_);
  return &(static_cast<Node*>(node_)->value);
}

/* -----------------------------------------------------
 *  CachedList
 * ---------------------------------------------------*/
template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>::CachedList()
    : tail_(), size_(0), free_list_(0, kDefaultMax) {}

template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>::CachedList(std::size_t init, std::size_t max)
    : tail_(), size_(0), free_list_(init, max) {}

template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>::CachedList(const T& v, std::size_t count)
    : tail_(), size_(0), free_list_(0, kDefaultMax) {
  Insert(end(), v, count);
}

template <typename T, template <class> typename Alloc>
template <typename Itr, std::enable_if_t<IsIterator<Itr>::kValue, int>>
CachedList<T, Alloc>::CachedList(Itr first, Itr end)
    : tail_(), size_(0), free_list_(0, kDefaultMax) {
  Insert(CachedList<T, Alloc>::end(), first, end);
}

template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>::CachedList(std::initializer_list<T> init)
    : tail_(), size_(0), free_list_(0, kDefaultMax) {
  Insert(end(), init);
}

template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>::CachedList(const CachedList& that)
    : tail_(), size_(0), free_list_(0, kDefaultMax) {
  Insert(end(), that.begin(), that.end());
}

template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>::CachedList(CachedList&& that)
    : tail_(), size_(0), free_list_(0, kDefaultMax) {
  // Notes, essentially we initialize |this| as empty container and then do the
  // swap, which makes the std::move semantic reasonable. TBH, the std::move
  // doesn't say anything about the memory or usage, ie it is reasonable that
  // after the std::move it has more overhead than before. But in practise we
  // don't do that.
  Swap(&that);
}

template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>& CachedList<T, Alloc>::operator=(const CachedList& that) {
  if (this != &that) {
    CachedList<T, Alloc> tmp{that};
    Swap(&tmp);
  }
  return *this;
}

template <typename T, template <class> typename Alloc>
CachedList<T, Alloc>& CachedList<T, Alloc>::operator=(CachedList&& that) {
  if (this != &that) {
    CachedList<T, Alloc> tmp{std::move(that)};
    Swap(&tmp);
  }
  return *this;
}

template <typename T, template <class> typename Alloc>
bool CachedList<T, Alloc>::operator==(const CachedList& that) const {
  if (size_ != that.size_) {
    return false;
  }

  // Notes(dpeng): We may need to extend it to support iterator_traits to work
  //   with all the std algorithms, ie using std::equal instead of having the
  //   following loops.
  auto titr = that.begin();
  for (const auto& entry : *this) {
    if (entry != *(titr++)) return false;
  }
  return true;
}

template <typename T, template <class> typename Alloc>
bool CachedList<T, Alloc>::operator!=(const CachedList& that) const {
  return !(*this == that);
}

template <typename T, template <class> typename Alloc>
void CachedList<T, Alloc>::Assign(const T& v, std::size_t cnt) {
  std::size_t idx = 0;
  for (auto node = begin_node(); node != end_node(); node = node->next) {
    if (idx == cnt) {
      EraseFrom(const_iterator{node});
      return;
    }
    static_cast<Node*>(node)->value = v;
    ++idx;
  }

  for (; idx < cnt; ++idx) {
    PushBack(v);
  }
}

template <typename T, template <class> typename Alloc>
template <typename Itr, std::enable_if_t<IsIterator<Itr>::kValue, int>>
void CachedList<T, Alloc>::Assign(Itr first, Itr end) {
  for (auto node = begin_node(); node != end_node(); node = node->next) {
    if (first == end) {
      EraseFrom(const_iterator{node});
      return;
    }
    static_cast<Node*>(node)->value = *(first++);
  }
  for (; first != end; ++first) PushBack(*first);
}

template <typename T, template <class> typename Alloc>
void CachedList<T, Alloc>::Assign(std::initializer_list<T> init) {
  auto init_beg = std::begin(init);
  auto init_end = std::end(init);
  for (auto node = begin_node(); node != end_node(); node = node->next) {
    if (init_beg == init_end) {
      EraseFrom(const_iterator{node});
      return;
    }
    static_cast<Node*>(node)->value = *(init_beg++);
  }

  for (; init_beg != init_end; ++init_beg) {
    PushBack(*init_beg);
  }
}

template <typename T, template <class> typename Alloc>
T& CachedList<T, Alloc>::Front() {
  DCHECK(!empty());
  return *begin();
}

template <typename T, template <class> typename Alloc>
const T& CachedList<T, Alloc>::Front() const {
  DCHECK(!empty());
  return *begin();
}

template <typename T, template <class> typename Alloc>
T& CachedList<T, Alloc>::Back() {
  DCHECK(!empty());
  return *last();
}

template <typename T, template <class> typename Alloc>
const T& CachedList<T, Alloc>::Back() const {
  DCHECK(!empty());
  return *last();
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::InsertNode(
    typename CachedList<T, Alloc>::const_iterator p, Node* n) {
  n->Ln(p.node());
  ++size_;
  return iterator{n};
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Insert(
    typename CachedList<T, Alloc>::const_iterator p, const T& v) {
  return InsertNode(p, Mk(v));
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Insert(
    typename CachedList<T, Alloc>::const_iterator p, T&& v) {
  return InsertNode(p, Mk(std::move(v)));
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Insert(
    typename CachedList<T, Alloc>::const_iterator pos, const T& v,
    std::size_t length) {
  std::optional<iterator> ret;
  iterator p = pos;
  while (length--) {
    p = Insert(p, v);
    if (!ret) ret = p;

    ++p;
  }
  return ret ? ret.value() : pos;
}

template <typename T, template <class> typename Alloc>
template <typename Itr, std::enable_if_t<IsIterator<Itr>::kValue, int>>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Insert(
    typename CachedList<T, Alloc>::const_iterator pos, Itr start, Itr end) {
  std::optional<iterator> ret;
  iterator p = pos;

  for (; start != end; ++start) {
    p = Insert(p, *start);
    if (!ret) ret = p;
    ++p;
  }
  return ret ? ret.value() : pos;
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Insert(
    const_iterator pos, std::initializer_list<T> init) {
  return Insert(pos, std::begin(init), std::end(init));
}

template <typename T, template <class> typename Alloc>
bool CachedList<T, Alloc>::HasNode(NodeBase* node) const {
  for (auto n = begin_node(); n != end_node(); n = n->next) {
    if (n == node) {
      return true;
    }
  }
  return false;
}

template <typename T, template <class> typename Alloc>
void CachedList<T, Alloc>::EraseFrom(
    typename CachedList<T, Alloc>::const_iterator p) {
  iterator itr = p;
  while (itr != end()) itr = Erase(itr);
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::EraseNode(
    NodeBase* n) {
  BASE_DBG(CHECK(HasNode(n)););

  auto next = n->next;
  n->Rm();
  free_list_.Delete(static_cast<Node*>(n));
  --size_;
  return iterator{next};
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Erase(
    typename CachedList<T, Alloc>::const_iterator pos) {
  return EraseNode(pos.node());
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Erase(
    typename CachedList<T, Alloc>::const_iterator start, const_iterator end) {
  iterator p = start;
  while (p != end) p = Erase(p);
  return p;
}

template <typename T, template <class> typename Alloc>
template <typename... ARGS>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::Emplace(
    typename CachedList<T, Alloc>::const_iterator pos, ARGS&&... args) {
  auto n = MkT(std::forward<ARGS>(args)...);
  return InsertNode(pos, n);
}

template <typename T, template <class> typename Alloc>
template <typename... ARGS>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::EmplaceBack(
    ARGS&&... args) {
  return Emplace(end(), std::forward<ARGS>(args)...);
}

template <typename T, template <class> typename Alloc>
template <typename... ARGS>
typename CachedList<T, Alloc>::iterator CachedList<T, Alloc>::EmplaceFront(
    ARGS&&... args) {
  return Emplace(begin(), std::forward<ARGS>(args)...);
}

template <typename T, template <class> typename Alloc>
void CachedList<T, Alloc>::Clear() {
  for (auto n = begin_node(); n != end_node();) {
    auto temp = n;
    n = n->next;
    free_list_.Delete(static_cast<Node*>(temp));
  }
  size_ = 0;
  tail_.Cut();
}

template <typename T, template <class> typename Alloc>
void CachedList<T, Alloc>::Reclaim() {
  CachedList<T, Alloc> tmp{};
  tmp.Swap(this);
}

template <typename T, template <class> typename Alloc>
void CachedList<T, Alloc>::Swap(CachedList* that) {
  tail_.Swap(&that->tail_);
  std::swap(size_, that->size_);
  free_list_.Swap(&(that->free_list_));
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::Node* CachedList<T, Alloc>::Mk(const T& v) {
  return free_list_.New(v);
}

template <typename T, template <class> typename Alloc>
typename CachedList<T, Alloc>::Node* CachedList<T, Alloc>::Mk(T&& v) {
  return free_list_.New(std::move(v));
}

template <typename T, template <class> typename Alloc>
template <typename... ARGS>
typename CachedList<T, Alloc>::Node* CachedList<T, Alloc>::MkT(ARGS&&... args) {
  return free_list_.New(std::forward<ARGS>(args)...);
}

}  // namespace base

#endif  // _BASE_CONTAINER_CACHED_LIST_INL_H_
