#ifndef _BASE_ZONE_INL_H_
#define _BASE_ZONE_INL_H_

namespace zone {

inline Zone::Zone(std::size_t reserve, std::size_t max)
    : segment_(nullptr),
      pool_(nullptr),
      size_(0),
      segment_size_(reserve),
      segment_count_(0),
      max_size_(max) {
  DCHECK(max);

  if (reserve) {
    Grow(reserve);
  }
}

inline Zone::Zone(std::size_t max, void* ptr, std::size_t size)
    : segment_(nullptr),
      pool_(nullptr),
      size_(0),
      segment_size_(0),
      segment_count_(0),
      max_size_(max) {
  DCHECK(max);
  InitSSO(ptr, size);
}

inline void Zone::InitSSO(void* ptr, std::size_t size) {
  pool_ = ptr;
  size_ = size;
  segment_size_ = size;
}

inline void Zone::GrowAtLeast(std::size_t at_least) {
  auto new_size = segment_size_ * 2;

  if (new_size > max_size_) {
    new_size = max_size_;
  }

  if (new_size < at_least) {
    new_size = at_least;
  }

  segment_size_ = new_size;
  Grow(new_size);
}

inline void Zone::Grow(std::size_t size) {
  auto new_segment = ::malloc(size + sizeof(Segment));
  auto header = static_cast<Segment*>(new_segment);

  header->next = segment_;
  header->size = size;
  segment_ = header;

  pool_ = base::PtrForward<Segment>(new_segment, sizeof(Segment));
  size_ = size;
  ++segment_count_;
}

inline void* Zone::Malloc(std::size_t size) {
  if (!size) {
    static char kDummy;
    return &kDummy;
  } else {
    size = base::Align(size, kAlignment);
    if (size_ < size) {
      GrowAtLeast(size);
    }

    size_ -= size;
    auto new_pool = base::PtrForward<void>(pool_, size);
    auto r = pool_;
    pool_ = new_pool;
    return r;
  }
}

template <typename T, typename... ARGS>
inline T* Zone::New(ARGS&&... args) {
  auto buffer = Malloc(sizeof(T));
  auto r = base::CallCtor<T>(buffer, std::forward<ARGS>(args)...);
  return r;
}

template <typename T, typename... ARGS>
inline T* Zone::Grab(ARGS&&... args) {
  auto buffer = Malloc(sizeof(T));
  auto r = base::CallCtor<T>(buffer, std::forward<ARGS>(args)...);
  AddDestructor<T>(r);
  return r;
}

}  // namespace zone

#endif  // _BASE_ZONE_H_
