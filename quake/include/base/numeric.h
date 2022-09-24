#ifndef _BASE_NUMERIC_H_
#define _BASE_NUMERIC_H_
#include <cstdint>
#include <limits>
#include <utility>

#include "trait.h"

// The following code is mainly used to do some conversion, mostly numeric,
// correctly and safely. It may not demonstrate general usability since it may
// not be really relevent in most of the code base.

namespace base {

// ClampCast, which cast input type I to output O based on clamp rule. Basically
// if O's upper bound or lower bound is smaller or larger than I, then we clamp
// the value to O's upper/lower bound instead of just extracting the bits.
//
// Notes the C++ standard static_cast will just extract the corresponding bits
// to form the O without any clamping behavior. It works on unsigned integer but
// for signed integer the behavior is not specified, and typically related to
// then endiness, since it just extracts the lower bits.
//
//
//   For example , following function
//
//     std::int32_t Cast(std::int64_t v) {
//       return static_cast<std::int32_t>(v);
//     }
//
//   will simply be compiled down to following x64 assembly :
//
//     mov rax, rdi
//     ret
//
//   So, if the callee of this function tries to use the rax as its 32 bits
//   counterpart, which just means the lower 32 bits of rax register, ie eax
//   is been used. No numeric coarsion is actually been performed.

namespace _ {

template <typename O, typename I, typename MinT, typename MaxT>
O ClampCastImpl(const I& v) {
  auto max_o = std::numeric_limits<O>::max();
  O min_o;

  // We want to have numeric minimm value. The std::numeric_limits<T>::min is
  // fine with integer, for floating point the min returns smallest none
  // negative number, since floating point is not exact. The real smallest
  // number should be -DBL_MAX/-DBL_FLT if it is floating point type
  if (std::is_integral<O>::value) {
    min_o = std::numeric_limits<O>::min();
  } else {
    min_o = -std::numeric_limits<O>::max();
  }

  auto p_max_o = static_cast<MaxT>(max_o);
  auto p_min_o = static_cast<MinT>(min_o);

  // Notes(dpeng): Here comes with a subtle issue for minimum value and maximum
  //   value of a signed integer. Mostly, a minimum value is off by 1 in terms
  //   of its absolute value. example, minimum value of std::int32_t is
  //   -2147483648 and its largest value is 2147483647.
  //
  // So if we convert minimum value to its maximum value it will becomes larger
  // than the max value, the cast is simply just do a static_cast, which just
  // make compiler reinpterpret the bit pattern, ie using jmp-above or jmp-great
  // in terms of x64 assembly. Here the quick v>=0 test is rule out situation
  // this situation basically makes a minimum value been rounded to largest
  // value.
  if ((v >= 0) && (static_cast<MaxT>(v) > p_max_o)) {
    return max_o;
  }
  if (static_cast<MinT>(v) < p_min_o) {
    return min_o;
  }
  return static_cast<O>(v);
}

}  // namespace _

template <typename O, typename I>
O ClampCast(const I& v) {
  static_assert(std::is_arithmetic<O>::value, "output must be arithmetic type");
  static_assert(std::is_arithmetic<I>::value, "input must be arithmetic type");

  // The fast path will be both O and I are just unsigned integers, then do
  // a numeric static_cast
  if constexpr (IsUnsignedInteger<O>::kValue && IsUnsignedInteger<I>::kValue) {
    return static_cast<O>(v);
  }

  // Check for both integer cases, for real/floating type type, we handle in
  // a different branch
  //
  // Unfortunately, if the conversion between double and integer is happened,
  // static_cast will introduce runtime costs due to the SSE instruciton is
  // needed on x64, ie cvtxxx instruction is used at least and move between
  // x64 general register and SSE register is performed. For integer part,
  // no runtime cost is needed since it is just alias of same register W.R.T
  // different bits part.
  if constexpr (std::is_integral<O>::value && std::is_integral<I>::value) {
    return _::ClampCastImpl<O, I, std::int64_t, std::uint64_t>(v);
  } else {
    return _::ClampCastImpl<O, I, double, double>(v);
  }
}

// Check whether value U is in range of type T, notes currently only integer
// is supported.
template <typename T, typename U>
bool IsValueInRange(const U& v) {
  static_assert(std::is_integral<T>::value && std::is_integral<U>::value);
  return v >= static_cast<T>(std::numeric_limits<T>::min()) &&
         v <= static_cast<T>(std::numeric_limits<T>::max());
}

}  // namespace base

#endif  // _BASE_NUMERIC_H_
