#ifndef _BASE_FREE_LIST_H_
#define _BASE_FREE_LIST_H_
#include "check.h"
#include "common.h"

namespace base {

// Basic free list. Ie a chain of same size object allocated from large range
// or span. Support fast new and delete, however the object size cannot be
// adjusted dynamically.
template <typename T>
class FreeList {
 public:
  // This is the minimum init size, it is still been constrained by the max size
  // Notes this number indicates how many |T| been allocated instead of bytes
  static constexpr std::size_t kMinimumInitSize = 8;
  static constexpr std::size_t kReserveDefault = 0;
  static constexpr std::size_t kMaxSizeDefault = kKB * 128;

  FreeList() : FreeList(kReserveDefault, kMaxSizeDefault) {}
  FreeList(std::size_t, std::size_t);
  ~FreeList();

  // Get a new object from the free list, internally it may expand the segement.
  // The New will do the construction for you and the returned pointer contains
  // an constructed object.
  template <typename... ARGS>
  T* New(ARGS&&... args);

  // Free/Delete an object. This does 2 things, 1) call the dtor; 2) free the
  // memory span back to the free list for future reuse.
  void Delete(T*);

  std::size_t total_bytes() const;
  std::size_t size() const { return size_; }
  std::size_t span_size() const;
  std::size_t capacity() const { return capacity_; }
  std::size_t max_size() const { return max_size_; }

 public:
  // This is sololy used for CachedList's implementation, ie swapping two free
  // list. In most cases it is useless
  void Swap(FreeList*);

 private:
  void Grow(std::size_t);

  struct FreeNode {
    FreeNode* next{nullptr};
  };

  struct Span {
    Span* next{nullptr};
    std::size_t size{0};
  };

  std::size_t ObjectSize() const {
    return sizeof(T) < sizeof(FreeNode) ? sizeof(FreeNode) : sizeof(T);
  }

  std::size_t ObjectByteSize(std::size_t count) const {
    return ObjectSize() * count;
  }

  std::size_t SpanSize(std::size_t count) const {
    return ObjectByteSize(count) + sizeof(Span);
  }

  FreeNode* free_;
  Span* span_;
  std::size_t size_;
  std::size_t capacity_;
  std::size_t max_size_;

  DISALLOW_COPY_AND_ASSIGN(FreeList);
};

}  // namespace base

#include "free-list-inl.h"

#endif  // _BASE_FREE_LIST_H_
