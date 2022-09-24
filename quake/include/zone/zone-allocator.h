#ifndef _ZONE_ZONE_ALLOCATOR_H_
#define _ZONE_ZONE_ALLOCATOR_H_
#include <base.h>

#include "zone/zone.h"

namespace zone {

template <typename T>
class ZoneAllocator {
 public:
  using pointer = T*;
  using const_pointer = const T*;
  using value_type = T;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using void_pointer = void*;
  using const_void_pointer = const void*;

  template <class O>
  struct rebind {
    using other = ZoneAllocator<O>;
  };

  ZoneAllocator() : zone_() {}

  explicit ZoneAllocator(Zone* zone) : zone_(zone) {}

  template <typename U>
  ZoneAllocator(const ZoneAllocator<U>& other) noexcept
      : ZoneAllocator<T>(other.zone_) {}

  template <typename U>
  friend class ZoneAllocator;

  T* allocate(size_t length);
  void deallocate(T* p, size_t length);

  size_t max_size() const {
    return std::numeric_limits<int>::max() / sizeof(T);
  }

  template <typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    void* v_p = const_cast<void*>(static_cast<const void*>(p));
    new (v_p) U(std::forward<Args>(args)...);
  }

  template <typename U>
  void destroy(U* p) {
    p->~U();
  }

  bool operator==(ZoneAllocator const& other) const {
    return zone_ == other.zone_;
  }
  bool operator!=(ZoneAllocator const& other) const {
    return zone_ != other.zone_;
  }
  Zone* zone() const { return zone_; }

 private:
  Zone* zone_;
};

template <typename T>
T* ZoneAllocator<T>::allocate(std::size_t length) {
  auto memory = zone_->Malloc(length * sizeof(T));
  CHECK(memory);
  return static_cast<T*>(memory);
}

template <typename T>
void ZoneAllocator<T>::deallocate(T* ptr, std::size_t length) {}

}  // namespace zone

#endif  // _ZONE_ZONE_ALLOCATOR_H_
