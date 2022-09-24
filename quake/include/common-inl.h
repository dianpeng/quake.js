#ifndef _QUAKE_COMMON_INL_H_
#define _QUAKE_COMMON_INL_H_
#include "base.h"

namespace quake {

inline SizeClass SizeClassPromote(SizeClass sc) {
  switch (sc) {
    case SizeClass::kByte:
      return SizeClass::kWord;
    case SizeClass::kWord:
      return SizeClass::kDWord;
    case SizeClass::kDWord:
      return SizeClass::kQWord;
    case SizeClass::kQWord:
      return SizeClass::kQWord;
    default:
      UNREACHABLE() << "other size class does not support promotion";
      return SizeClass::kUnknown;
  }
}

inline std::optional<SizeClass> ParseSizeClass(const std::string_view& v) {
#define __(A, B, C, D)               \
  do {                               \
    if (v == D) return SizeClass::A; \
  } while (false);
  QUAKE_SIZE_CLASS_LIST(__)
#undef __  // __

  return {};
}

inline std::optional<RegisterType> ParseRegisterType(
    const std::string_view& v) {
  if (v == "int" || v == "general" || v == "gp") return RegisterType::kGeneral;
  if (v == "float" || v == "fp") return RegisterType::kFloat;
  if (v == "unknown") return RegisterType::kUnknown;
  return {};
}

// clang-format off
inline int SizeClassToByteSize(SizeClass sc) {
  switch (sc) {
#define __(A, B, C, ...) case SizeClass::A: return C;
    QUAKE_SIZE_CLASS_LIST(__)
#undef __  // __
    default:
      UNREACHABLE();
      return -100;
  }
}
// clang-format on

}  // namespace quake

#endif  // _QUAKE_COMMON_INL_H_
