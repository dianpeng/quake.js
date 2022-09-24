#ifndef _BASE_FREE_LIST_INL_H_
#define _BASE_FREE_LIST_INL_H_

namespace base {

template <typename T>
FreeList<T>::FreeList(std::size_t reserve, std::size_t max)
    : free_(nullptr),
      span_(nullptr),
      size_(0),
      capacity_(reserve),
      max_size_(max) {
  DCHECK(max_size_);
  if (reserve) {
    Grow(reserve);
  }
}

template <typename T>
FreeList<T>::~FreeList() {
  for (Span* t = nullptr; span_; span_ = t) {
    t = span_->next;
    ::free(span_);
  }
  DCHECK(!span_);
  free_ = nullptr;
  size_ = 0;
}

template <typename T>
void FreeList<T>::Swap(FreeList* that) {
  std::swap(free_, that->free_);
  std::swap(span_, that->span_);
  std::swap(size_, that->size_);
  std::swap(capacity_, that->capacity_);
  std::swap(max_size_, that->max_size_);
}

template <typename T>
template <typename... ARGS>
T* FreeList<T>::New(ARGS&&... args) {
  void* buffer = nullptr;

  if (!free_) {
    auto new_cap = capacity_ * 2;
    if (!new_cap) {
      new_cap = kMinimumInitSize;
    }

    Grow(new_cap);
    DCHECK(free_);
  }

  buffer = reinterpret_cast<void*>(free_);
  free_ = free_->next;
  size_--;
  return CallCtor<T>(buffer, std::forward<ARGS>(args)...);
}

template <typename T>
void FreeList<T>::Delete(T* ptr) {
  if (ptr) {
    CallDtor(ptr);
    auto n = reinterpret_cast<FreeNode*>(ptr);
    n->next = free_;
    free_ = n;
    size_++;
  }
}

template <typename T>
std::size_t FreeList<T>::total_bytes() const {
  std::size_t total = 0;
  for (auto s = span_; s; s = s->next) {
    total += s->size;
  }
  return total;
}

template <typename T>
std::size_t FreeList<T>::span_size() const {
  std::size_t x = 0;
  for (auto s = span_; s; s = s->next) {
    x++;
  }
  return x;
}

template <typename T>
void FreeList<T>::Grow(std::size_t new_cap) {
  if (new_cap > max_size_) {
    new_cap = max_size_;
  }
  const auto object_size = ObjectSize();
  auto byte_size = SpanSize(new_cap);
  auto ptr = ::malloc(byte_size);

  // Initialize the span part
  {
    auto new_span = reinterpret_cast<Span*>(ptr);
    new_span->size = byte_size;
    new_span->next = span_;
    span_ = new_span;
  }

  auto chunk = static_cast<char*>(ptr) + sizeof(Span);
  auto chunk_end = chunk + ObjectByteSize(new_cap);

  // Okay, chain the chunk into small object free list, notes, leave the last
  // one out to be handled manually
  for (auto p = chunk; p < chunk_end - object_size; p += object_size) {
    auto next = p + object_size;
    auto next_node = reinterpret_cast<FreeNode*>(next);
    reinterpret_cast<FreeNode*>(p)->next = next_node;
  }

  // Last object
  {
    auto last_chunk = reinterpret_cast<FreeNode*>(chunk_end - object_size);
    last_chunk->next = free_;
  }

  // Link the newly created free list back
  free_ = reinterpret_cast<FreeNode*>(chunk);
  size_ += new_cap;
  capacity_ = new_cap;
}

}  // namespace base

#endif  // _BASE_FREE_LIST_INL_H_
