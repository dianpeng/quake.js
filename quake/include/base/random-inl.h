#ifndef _BASE_RANDOM_INL_H_
#define _BASE_RANDOM_INL_H_

namespace base {

inline std::string RandString(std::size_t size) {
  std::string r;
  RandString(&r, size);
  return r;
}

inline std::int64_t RandInt64() {
  return RandInt64(std::numeric_limits<std::int64_t>::min(),
                   std::numeric_limits<std::int64_t>::max());
}

inline std::uint64_t RandUInt64() {
  return RandUInt64(std::numeric_limits<std::uint64_t>::min(),
                    std::numeric_limits<std::uint64_t>::max());
}

inline double RandDouble() {
  double v;
  RandByte(reinterpret_cast<std::uint8_t*>(&v), sizeof(v));
  return v;
}

}  // namespace base

#endif  // _BASE_RANDOM_INL_H_
