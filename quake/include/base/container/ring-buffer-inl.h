#ifndef _BASE_CONTAINER_RING_BUFFER_INL_H_
#define _BASE_CONTAINER_RING_BUFFER_INL_H_

namespace base {

template <typename T, int kSize>
void RingBuffer<T, kSize>::Push(const T& value) {
  if (count_ == kSize) {
    elements_[finger_++] = value;
    if (finger_ == kSize) finger_ = 0;
  } else {
    DCHECK(finger_ == 0);
    elements_[count_++] = value;
  }
}

template <typename T, int kSize>
void RingBuffer<T, kSize>::Push(T&& value) {
  if (count_ == kSize) {
    elements_[finger_++] = std::move(value);
    if (finger_ == kSize) finger_ = 0;
  } else {
    DCHECK(finger_ == 0);
    elements_[count_++] = std::move(value);
  }
}

template <typename T, int kSize>
template <typename Callback>
void RingBuffer<T, kSize>::Foreach(Callback&& c) const {
  int j = finger_ + count_ - 1;
  if (j >= kSize) j -= kSize;
  for (int i = 0; i < count_; i++) {
    c(elements_[j]);
    if (--j == -1) j += kSize;
  }
}

template <typename T, int kSize>
template <typename Callback>
T RingBuffer<T, kSize>::Sum(Callback&& callback, const T& initial) const {
  int j = finger_ + count_ - 1;
  if (j >= kSize) j -= kSize;
  T result = initial;
  for (int i = 0; i < count_; i++) {
    result = callback(result, elements_[j]);
    if (--j == -1) j += kSize;
  }
  return result;
}

template <typename T, int kSize>
template <typename Callback>
std::pair<T, std::size_t> RingBuffer<T, kSize>::SumUntil(
    std::size_t target, Callback&& callback, const T& initial) const {
  std::size_t idx = 0;
  auto v = Sum(
      [&, c = std::move(callback)](auto a, auto b) -> T {
        if (idx < target) {
          ++idx;
          return c(a, b);
        } else {
          return a;
        }
      },
      initial);

  return std::make_pair(v, idx);
}

}  // namespace base

#endif  // _BASE_CONTAINER_RING_BUFFER_INL_H_
