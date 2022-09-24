#ifndef _BASE_CONTAINER_RING_BUFFER_H_
#define _BASE_CONTAINER_RING_BUFFER_H_
#include <base/check.h>
#include <base/common.h>

namespace base {

// A simple RingBuffer. Heavily based on V8's RingBuffer implementation
template <typename T, int kSize = 10>
class RingBuffer {
 public:
  RingBuffer() : elements_(), finger_(0), count_(0) {}

 public:
  int GetMaxSize() const { return kSize; }
  void Push(const T&);
  void Push(T&&);

  template <typename Callback>
  T Sum(Callback&&, const T&) const;

  template <typename Callback>
  void Foreach(Callback&&) const;

  template <typename Callback>
  std::pair<T, std::size_t> SumUntil(std::size_t, Callback&&, const T&) const;

  std::pair<T, std::size_t> SumUntil(std::size_t, const T& init) const {
    return SumUntil(
        index, [](auto a, auto b) { return a + b; }, init);
  }

  void Reset() { finger_ = count_ = 0; }

 private:
  T elements_[kSize];
  int finger_;
  int count_;

  DISALLOW_COPY_AND_ASSIGN(RingBuffer);
};

}  // namespace base

#include "ring-buffer-inl.h"

#endif  // _BASE_CONTAINER_RING_BUFFER_H_
