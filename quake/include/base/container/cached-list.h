#ifndef _BASE_CONTAINER_CACHED_LIST_H_
#define _BASE_CONTAINER_CACHED_LIST_H_
#include <base/common.h>
#include <base/free-list.h>

namespace base {

// A free-list backed std::list like container. Notes this implementation is
// kind of sluggish and we should use std::list with customized container but
// the issue is free-list with std::list is kind of hard to get to work since
// std::list uses rebind to bind to another allocator, typical arena/bum pointer
// allocator will work, anyway, this is what I end up with currently and it is
// not that kind of complicated.
template <typename T, template <class> typename Alloc = FreeList>
class CachedList {
  struct NodeBase {
    NodeBase* prev;
    NodeBase* next;
    NodeBase() : prev(this), next(this) {}

   public:
    inline void Cut() {
      prev = this;
      next = this;
    }
    inline void Ln(NodeBase* b);
    inline void Rm();
    inline bool IsUnlink() const;
    inline void Swap(NodeBase*);
  };

  struct Node : NodeBase {
    T value;

    Node(T&& v) : NodeBase(), value(std::move(v)) {}
    Node(const T& v) : NodeBase(), value(v) {}

    template <typename... ARGS>
    Node(ARGS&&... args) : NodeBase(), value(std::forward<ARGS>(args)...) {}

    ~Node() { NodeBase::Rm(); }
  };

  class CachedListIterator : public IteratorTag {
   public:
    CachedListIterator() : node_(nullptr) {}

    CachedListIterator(const CachedListIterator&) = default;
    CachedListIterator& operator=(const CachedListIterator&) = default;
    CachedListIterator(CachedListIterator&&) = default;
    CachedListIterator& operator=(CachedListIterator&&) = default;

   public:
    inline bool operator==(const CachedListIterator&) const;
    inline bool operator!=(const CachedListIterator&) const;
    inline CachedListIterator& operator++() const;
    inline CachedListIterator& operator--() const;
    inline CachedListIterator operator++(int) const;
    inline CachedListIterator operator--(int) const;

   public:
    inline T& operator*();
    inline const T& operator*() const;
    inline T* operator->();
    inline const T* operator->() const;

   private:
    CachedListIterator& mutable_self() const {
      return *const_cast<CachedListIterator*>(this);
    }

    NodeBase* node() const {
      DCHECK(node_);
      return node_;
    }
    NodeBase* next() const {
      DCHECK(node_);
      return node_->next;
    }
    NodeBase* prev() const {
      DCHECK(node_);
      return node_->prev;
    }

    explicit CachedListIterator(Node* n) : node_(n) {}
    explicit CachedListIterator(const Node* n) : node_(const_cast<Node*>(n)) {}

    explicit CachedListIterator(NodeBase* n) : node_(n) {}
    explicit CachedListIterator(const NodeBase* n)
        : node_(const_cast<NodeBase*>(n)) {}

    mutable NodeBase* node_;
    friend class CachedList;
  };

 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using iterator = CachedListIterator;
  using const_iterator = const CachedListIterator;

 public:
  static constexpr std::size_t kDefaultInitReserve = 0;
  static constexpr std::size_t kDefaultMax = kKB * 16;

  CachedList();
  CachedList(std::size_t, std::size_t);

  explicit CachedList(const T& value, std::size_t count = 1);

  template <typename Itr, std::enable_if_t<IsIterator<Itr>::kValue, int> _ = 0>
  CachedList(Itr first, Itr end);

  CachedList(std::initializer_list<T>);

  CachedList(const CachedList&);
  CachedList(CachedList&&);

  CachedList& operator=(const CachedList&);
  CachedList& operator=(CachedList&&);

  ~CachedList() { Clear(); }

  bool operator==(const CachedList&) const;
  bool operator!=(const CachedList&) const;

 public:
  // Assign
  void Assign(const T&, std::size_t);

  template <typename Itr, std::enable_if_t<IsIterator<Itr>::kValue, int> _ = 0>
  void Assign(Itr first, Itr end);

  void Assign(std::initializer_list<T>);

  // Accessor
  T& Front();
  const T& Front() const;

  T& Back();
  const T& Back() const;

  // Insert
  iterator Insert(const_iterator, const T&);
  iterator Insert(const_iterator, T&&);
  iterator Insert(const_iterator, const T&, std::size_t);

  template <typename Itr, std::enable_if_t<IsIterator<Itr>::kValue, int> _ = 0>
  iterator Insert(const_iterator, Itr, Itr);
  iterator Insert(const_iterator, std::initializer_list<T>);

  // Erase
  iterator Erase(const_iterator);
  iterator Erase(const_iterator, const_iterator);

  // Emplace
  template <typename... ARGS>
  iterator Emplace(const_iterator, ARGS&&...);

  template <typename... ARGS>
  iterator EmplaceBack(ARGS&&...);

  template <typename... ARGS>
  iterator EmplaceFront(ARGS&&...);

  // Push/Pop
  iterator PushBack(T&& v) { return Insert(end(), std::move(v)); }
  iterator PushBack(const T& v) { return Insert(end(), v); }
  void PopBack() {
    DCHECK(!IsEmpty());
    Erase(last());
  }

  iterator PushFront(T&& v) { return Insert(begin(), std::move(v)); }
  iterator PushFront(const T& v) { return Insert(begin(), v); }

  void PopFront() {
    DCHECK(!IsEmpty());
    Erase(begin());
  }

  void Clear();

  void Reclaim();

  inline void Swap(CachedList*);
  void swap(CachedList& that) { Swap(&that); }

 public:
  std::size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  std::size_t GetSize() const { return size_; }
  bool IsEmpty() const { return size_ == 0; }

 public:
  iterator begin() { return iterator{begin_node()}; }
  iterator end() { return iterator{end_node()}; }

  const_iterator begin() const { return iterator{begin_node()}; }
  const_iterator end() const { return iterator{end_node()}; }

  const_iterator cbegin() { return begin(); }
  const_iterator cend() { return end(); }

 private:
  iterator last() {
    DCHECK(!IsEmpty());
    return iterator{tail_.prev};
  }
  const_iterator last() const {
    DCHECK(!IsEmpty());
    return iterator{tail_.prev};
  }

  const NodeBase* begin_node() const { return tail_.next; }
  const NodeBase* end_node() const { return &tail_; }

  NodeBase* begin_node() { return tail_.next; }
  NodeBase* end_node() { return &tail_; }

  iterator InsertNode(const_iterator p, Node* n);
  iterator EraseNode(NodeBase*);
  void EraseFrom(const_iterator);

  bool HasNode(NodeBase*) const;

  Node* Mk(const T&);
  Node* Mk(T&&);
  template <typename... ARGS>
  Node* MkT(ARGS&&...);

 protected:
  NodeBase tail_;
  std::size_t size_;
  Alloc<Node> free_list_;
};

}  // namespace base

#include "cached-list-inl.h"

#endif  // _BASE_CONATINER_CACHED_LIST_H_
