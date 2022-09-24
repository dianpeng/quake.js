#ifndef _BASE_BITS_H_
#define _BASE_BITS_H_
#include "common.h"

namespace base {

// A specialized template to allow user set the bits to be on from |right| to
// |left|. Example as following :
//
//   BitMaskOn<std::uint32_t, 3>::kValue = 0000....00111
//                                                   *** ---> the 3 bits is on
template <typename T, std::size_t kAlignment>
struct BitMaskOn {
  static_assert(kAlignment <= sizeof(T) * 8);
  static constexpr T kValue = (static_cast<T>(1) << kAlignment) - 1;
};

template <std::size_t A>
using U32BitMaskOn = BitMaskOn<std::uint32_t, A>;

template <std::size_t A>
using U64BitMaskOn = BitMaskOn<std::uint64_t, A>;

template <typename T, std::size_t N>
struct BitOn {
 public:
  static const std::size_t kBitSize = sizeof(T) * 8;
  static_assert(std::is_integral<T>::value, "must be an integer");
  static_assert(kBitSize > N, "invalid offset");

  static const T kValue = static_cast<T>(1 << N);
};

template <std::size_t N>
using U32BitOn = BitOn<std::uint32_t, N>;

template <std::size_t N>
using U64BitOn = BitOn<std::uint64_t, N>;

// This function cast the input type |I| as bit pattern to the O type. This is
// useful if you have a 32bits signed but want to store it into a 32bits
// unsigned value. The typical static_cast will not do the job for you
template <typename O, typename I>
O BitCast(const I& in) {
  static_assert(sizeof(I) == sizeof(O));
  O out;
  std::memcpy(&out, &in, sizeof(I));
  return out;
}

template <typename O, typename... ARGS>
constexpr O BitOr(ARGS&&... args) {
  return static_cast<O>(((static_cast<std::uint64_t>(args)) | ...));
}

template <typename O, typename... ARGS>
constexpr O BitAnd(ARGS&&... args) {
  return static_cast<O>(((static_cast<std::uint64_t>(args)) & ...));
}

template <typename O, typename... ARGS>
constexpr O BitXOR(ARGS&&... args) {
  return static_cast<O>(((static_cast<std::uint64_t>(args)) ^ ...));
}

// Helper to deal with bitflag on different types
template <typename U>
bool BitFlagHas(U value, U testify) {
  auto bit_value = static_cast<std::uint64_t>(value);
  auto bit_testify = static_cast<std::uint64_t>(testify);
  return static_cast<bool>(bit_value & bit_testify);
}

}  // namespace base

#endif  // _BASE_BITS_H_
