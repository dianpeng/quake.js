#ifndef _ZONE_ZONE_CONTAINER_H_
#define _ZONE_ZONE_CONTAINER_H_
#include "zone/zone.h"
#include "zone/zone-allocator.h"

#include <vector>
#include <deque>

namespace zone {

template <typename T>
class Vector : public std::vector<T, ZoneAllocator<T>> {
 public:
  using Base = std::vector<T, ZoneAllocator<T>>;
  using Alloc = typename Base::allocator_type;
  using typename Base::allocator_type;

  explicit Vector(Zone* zone) : Base(Alloc{zone}) {}
  Vector(Zone* zone, std::size_t size) : Base(size, Alloc{zone}) {}
};

}  // namespace zone

#endif  // _ZONE_ZONE_CONTAINER_H_
