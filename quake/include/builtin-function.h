#ifndef _QUAKE_BUILTIN_FUNCTION_H_
#define _QUAKE_BUILTIN_FUNCTION_H_
#include "base.h"

// All the known builtin function lists, used internally for the rewriter/lower
#define QUAKE_BUILTIN_FUNCTION_LIST(__)                            \
  __(kFMod, "fmod", HType::kFloat, {HType::kFloat, HType::kFloat}) \
  __(kFPow, "fpow", HType::kFloat, {HType::kFloat, HType::kFloat}) \
  __(kGCFree, "gcfree", HType::kNone, {HType::kGC})

namespace quake {

// clang-format off
enum BuiltinFunction {
#define __(A, ...) A,
  QUAKE_BUILTIN_FUNCTION_LIST(__)
#undef __  // __
  kSizeOfBuiltinFunction
};
// clang-format on

}  // namespace quake

#endif  // _QUAKE_BUILTIN_FUNCTION_H_
