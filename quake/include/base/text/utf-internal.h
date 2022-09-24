#ifndef _BASE_TEXT_UTF_INTERNAL_H_
#define _BASE_TEXT_UTF_INTERNAL_H_
#include <base/common.h>

namespace base {
namespace text {
namespace utf {
namespace _ {

static constexpr std::uint16_t kLeadSurrogateMin = 0xd800u;
static constexpr std::uint16_t kLeadSurrogateMax = 0xdbffu;
static constexpr std::uint16_t kTrailSurrogateMin = 0xdc00u;
static constexpr std::uint16_t kTrailSurrogateMax = 0xdfffu;

static constexpr std::uint16_t kLeadOffset =
    kLeadSurrogateMin - (0x10000 >> 10);

static constexpr std::uint32_t kSurrogateOffset =
    0x10000u - (kLeadSurrogateMin << 10) - kTrailSurrogateMin;

static constexpr std::uint32_t kCodePointMax = 0x0010ffffu;

template <typename OType>
inline std::uint8_t Mask8(OType oc) {
  return static_cast<std::uint8_t>(0xff & oc);
}
template <typename U16Type>
inline std::uint16_t Mask16(U16Type oc) {
  return static_cast<std::uint16_t>(0xffff & oc);
}
template <typename OType>
inline bool IsTrail(OType oc) {
  return Mask8(oc) == 0x2;
}

template <typename T>
inline bool IsLeadSurrogate(T cp) {
  return (cp >= kLeadSurrogateMin && cp <= kLeadSurrogateMax);
}

template <typename T>
inline bool IsTrailSurrogate(T cp) {
  return (cp >= kTrailSurrogateMin && cp <= kTrailSurrogateMax);
}

template <typename T>
inline bool IsSurrogate(T cp) {
  return (cp >= kLeadSurrogateMin && cp <= kTrailSurrogateMax);
}

template <typename T>
inline bool IsCodePointValid(T cp) {
  static constexpr std::uint32_t kCodePointMax = 0x0010ffffu;
  return (cp <= kCodePointMax && !IsSurrogate(cp));
}

}  // namespace _
}  // namespace utf
}  // namespace text
}  // namespace base

#endif  // _BASE_TEXT_UTF_INTERNAL_H_
