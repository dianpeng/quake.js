#ifndef _BASE_TEXT_UTF8_INTERNAL_H_
#define _BASE_TEXT_UTF8_INTERNAL_H_
#include <cstdint>
#include <type_traits>

/**
 * The following code is based on V8's utf-decoder which is oringated from
 * Bjoern Hehrmann <bjoern@hoehrmann.de>.
 *
 * Additionaly modification is also placed in this file for handling BOM
 * of UTF explicitly.
 *
 * See licenese below:
 *
 *--------------------------------------------------------------------------

Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 *---------------------------------------------------------------------------*/

// clang-format off

namespace base {
namespace utf8 {
namespace _ {

template <typename U>
inline std::uint8_t Mask(U v) { return static_cast<std::uint8_t>(v & 0xff); }

}  // namespace _

// Byte Order Mark (BOM) handler
static constexpr std::uint8_t kBOM[] = {
  0xef,
  0xbb,
  0xbf
};

template <typename U>
inline bool StartsWithBOM(U it, U end) {
  static_assert(sizeof(typename std::remove_pointer<U>::type) == 1);
  return (((it != end) && (_::Mask(*it++)) == kBOM[0]) &&
          ((it != end) && (_::Mask(*it++)) == kBOM[1]) &&
          ((it != end) && (_::Mask(*it))   == kBOM[2]));
}

inline bool StartsWithBOM(std::uint8_t b1, std::uint8_t b2, std::uint8_t b3) {
  return _::Mask(b1) == kBOM[0] &&
         _::Mask(b2) == kBOM[1] &&
         _::Mask(b3) == kBOM[2];
}

template <typename U>
inline bool StartsWithBOM(U it) {
  static_assert(sizeof(typename std::remove_pointer<U>::type) == 1);
  return ((_::Mask(*it++)) == kBOM[0] &&
          (_::Mask(*it++)) == kBOM[1] &&
          (_::Mask(*it))   == kBOM[2]);
}

enum State : std::uint8_t {
  kReject = 0,
  kAccept = 12,
  kTwoByte = 24,
  kThreeByte = 36,
  kThreeByteLowMid = 48,
  kFourByte = 60,
  kFourByteLow = 72,
  kThreeByteHigh = 84,
  kFourByteMidHigh = 96,
};

static inline void Decode(std::uint8_t byte, State* state,
                          std::uint32_t* buffer) {

  // This first table maps bytes to character to a transition.
  static constexpr uint8_t transitions[] = {
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 00-0F
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 10-1F
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 20-2F
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 30-3F
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 40-4F
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 50-5F
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 60-6F
      0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 70-7F
      1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 80-8F
      2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 90-9F
      3,  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // A0-AF
      3,  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // B0-BF
      9,  9, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // C0-CF
      4,  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // D0-DF
      10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5,  // E0-EF
      11, 7, 7, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // F0-FF
  };

  // This second table maps a state to a new state when adding a transition.
  //  00-7F
  //  |   80-8F
  //  |   |   90-9F
  //  |   |   |   A0-BF
  //  |   |   |   |   C2-DF
  //  |   |   |   |   |   E1-EC, EE, EF
  //  |   |   |   |   |   |   ED
  //  |   |   |   |   |   |   |   F1-F3
  //  |   |   |   |   |   |   |   |   F4
  //  |   |   |   |   |   |   |   |   |   C0, C1, F5-FF
  //  |   |   |   |   |   |   |   |   |   |  E0
  //  |   |   |   |   |   |   |   |   |   |  |   F0
  static constexpr uint8_t states[] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,   // REJECT = 0
      12, 0,  0,  0,  24, 36, 48, 60, 72, 0, 84, 96,  // ACCEPT = 12
      0,  12, 12, 12, 0,  0,  0,  0,  0,  0, 0,  0,   // 2-byte = 24
      0,  24, 24, 24, 0,  0,  0,  0,  0,  0, 0,  0,   // 3-byte = 36
      0,  24, 24, 0,  0,  0,  0,  0,  0,  0, 0,  0,   // 3-byte low/mid = 48
      0,  36, 36, 36, 0,  0,  0,  0,  0,  0, 0,  0,   // 4-byte = 60
      0,  36, 0,  0,  0,  0,  0,  0,  0,  0, 0,  0,   // 4-byte low = 72
      0,  0,  0,  24, 0,  0,  0,  0,  0,  0, 0,  0,   // 3-byte high = 84
      0,  0,  36, 36, 0,  0,  0,  0,  0,  0, 0,  0,   // 4-byte mid/high = 96
  };

  uint8_t type = transitions[byte];
  *state = static_cast<State>(states[*state + type]);
  *buffer = (*buffer << 6) | (byte & (0x7F >> (type >> 1)));
}

}  // namespace utf8
}  // namespace base

// clang-format on

#endif  // _BASE_TEXT_UTF8_INTERNAL_H_
