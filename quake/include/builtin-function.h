#ifndef _QUAKE_BUILTIN_FUNCTION_H_
#define _QUAKE_BUILTIN_FUNCTION_H_
#include "base.h"

// All the known builtin function lists, used internally for the rewriter/lower
#define QUAKE_BUILTIN_FUNCTION_LIST(__)                                 \
  __(kFMod, "fmod", HType::kFloat, {HType::kFloat, HType::kFloat})      \
  __(kFPow, "fpow", HType::kFloat, {HType::kFloat, HType::kFloat})      \
  __(kGCFree, "gcfree", HType::kNone, {HType::kGC})                     \
  /** global **/                                                        \
  __(kGlobalCheck, "global_check", HType::kBox, {HType::kInt})          \
  __(kGlobalGet, "global_get", HType::kBox, {HType::kInt, HType::kInt}) \
  __(kGlobalPut, "global_put", HType::kBox,                             \
     {HType::kInt, HType::kPtr, HType::kInt})                           \
  /** property access **/                                               \
  __(kObjectFieldGetAtom, "object_field_get_atom", HType::kBox,         \
     {HType::kPtr, HType::kInt})                                        \
  __(kObjectFieldPutAtom, "object_field_put_atom", HType::kBox,         \
     {HType::kPtr, HType::kInt, HType::kPtr})                           \
  __(kObjectFieldDefineAtom, "object_field_define_atom", HType::kBox,   \
     {HType::kPtr, HType::kInt, HType::kPtr})                           \
  __(kObjectFieldNameSet, "object_field_name_set", HType::kBox,         \
     {HType::kPtr, HType::kInt})                                        \
  __(kObjectFieldNameSetComputed, "object_field_name_set_computed",     \
     HType::kBox, {HType::kPtr, HType::kPtr})

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
