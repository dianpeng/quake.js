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

template <typename CharT, typename Trait>
class BasicString
    : public std::basic_string<CharT, Trait, ZoneAllocator<CharT>> {
  using Alloc = ZoneAllocator<CharT>;
  using Super = std::basic_string<CharT, Trait, ZoneAllocator<CharT>>;

 public:
  explicit BasicString(Zone* zone) : Super(Alloc(zone)) {}

  BasicString(Zone* zone, const char* xx) : Super(xx, Alloc(zone)) {}
  BasicString(Zone* zone, const char* xx, std::size_t len)
      : Super(xx, len, Alloc(zone)) {}

  BasicString(Zone* zone, const std::string_view& that)
      : Super(that.data(), that.size(), Alloc(zone)) {}

  BasicString(Zone* zone, const std::string& that)
      : Super(that.data(), that.size(), Alloc(zone)) {}

  BasicString(Zone* zone, const BasicString& that)
      : Super(that.data(), that.size(), Alloc(zone)) {}
};

using String = BasicString<char, std::char_traits<char>>;

}  // namespace zone

#endif  // _ZONE_ZONE_CONTAINER_H_
