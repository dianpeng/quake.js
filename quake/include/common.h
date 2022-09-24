#ifndef _QUAKE_COMMON_H_
#define _QUAKE_COMMON_H_
#include "base.h"

namespace quake {

#define QUAKE_SIZE_CLASS_LIST(__)                                   \
  /** GP size class **/                                             \
  __(kByte, 1 << 1, 1, "byte")                                      \
  __(kWord, 1 << 2, 2, "word")                                      \
  __(kDWord, 1 << 3, 4, "dword")                                    \
  __(kQWord, 1 << 4, 8, "qword")                                    \
  __(kCommon, kByte | kWord | kDWord | kQWord, 0, "common")         \
  /** SIMD size class, ie SSE, AVX, AVX-512 **/                     \
  __(k128Bits, 1 << 10, 128 / 8, "128bits")                         \
  __(k256Bits, 1 << 10, 256 / 8, "256bits")                         \
  __(k512Bits, 1 << 10, 512 / 8, "512bits")                         \
  /** this node does not need a size class **/                      \
  __(kIrrelevant, 1 << 14, -1, "irrlevant")                         \
  /** we have not learn the size, it is derived from some rules **/ \
  __(kUnknown, 1 << 15, -2, "unknown")

// clang-format off
enum class SizeClass : std::uint16_t {
#define __(A, B, ...) A,
  QUAKE_SIZE_CLASS_LIST(__)
#undef __  // __
  kDummy
};
// clang-format on

enum class RegisterType : std::uint8_t { kGeneral, kFloat };

inline SizeClass SizeClassPromote(SizeClass);
inline std::optional<SizeClass> ParseSizeClass(const std::string_view&);
inline std::optional<RegisterType> ParseRegisterType(const std::string_view&);
inline int SizeClassToByteSize(SizeClass sc);

}  // namespace quake

#include "common-inl.h"

#endif  // _QUAKE_COMMON_H_
