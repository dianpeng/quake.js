#ifndef _QUAKE_BUILTIN_FUNCTION_H_
#define _QUAKE_BUILTIN_FUNCTION_H_
#include "base.h"

// All the known builtin function lists, used internally for the rewriter/lower
#define QUAKE_BUILTIN_FUNCTION_LIST(__)                                        \
  __(kFMod, "fmod", HType::kFloat, {HType::kFloat, HType::kFloat})             \
  __(kFPow, "fpow", HType::kFloat, {HType::kFloat, HType::kFloat})             \
  __(kGCFree, "gcfree", HType::kNone, {HType::kGC})                            \
  /** misc **/                                                                 \
  __(kAddBrand, "add_brand", HType::kBox, {HType::kBoxRef, HType::kBoxRef})    \
  __(kCheckBrand, "check_brand", HType::kBox,                                  \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kRegexp, "regexp", HType::kBox, {HType::kBoxRef, HType::kBoxRef})         \
  __(kGetSuper, "get_super", HType::kBox, {HType::kBoxRef})                    \
  __(kImport, "import", HType::kBox, {HType::kBoxRef})                         \
  /** array **/                                                                \
  __(kLoadArray, "load_array", HType::kBox, {HType::kBoxRef, HType::kBoxRef})  \
  __(kLoadArrayInt, "load_array_int", HType::kBox, {HType::kPtr, HType::kInt}) \
  /** global **/                                                               \
  __(kGlobalCheck, "global_check", HType::kBoxRef, {HType::kInt})              \
  __(kGlobalGet, "global_get", HType::kBoxRef, {HType::kInt, HType::kInt})     \
  __(kGlobalPut, "global_put", HType::kBoxRef,                                 \
     {HType::kInt, HType::kPtr, HType::kInt})                                  \
  __(kGlobalDefine, "global_define", HType::kBoxRef,                           \
     {HType::kInt, HType::kInt})                                               \
  __(kGlobalCheckDefine, "global_check_define", HType::kBoxRef,                \
     {HType::kInt, HType::kInt})                                               \
  /** property access **/                                                      \
  __(kObjectFieldGetAtom, "object_field_get_atom", HType::kBoxRef,             \
     {HType::kPtr, HType::kInt})                                               \
  __(kObjectFieldPutAtom, "object_field_put_atom", HType::kBoxRef,             \
     {HType::kPtr, HType::kInt, HType::kPtr})                                  \
  __(kObjectFieldDefineAtom, "object_field_define_atom", HType::kBoxRef,       \
     {HType::kPtr, HType::kInt, HType::kPtr})                                  \
  __(kObjectFieldNameSet, "object_field_name_set", HType::kBoxRef,             \
     {HType::kPtr, HType::kInt})                                               \
  __(kObjectFieldNameSetComputed, "object_field_name_set_computed",            \
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
