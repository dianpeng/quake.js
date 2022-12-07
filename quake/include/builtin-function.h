#ifndef _QUAKE_BUILTIN_FUNCTION_H_
#define _QUAKE_BUILTIN_FUNCTION_H_
#include "base.h"

// All the known builtin function lists, used internally for the rewriter/lower
#define QUAKE_BUILTIN_FUNCTION_LIST(__)                                        \
  __(kFMod, "fmod", RCallFMod, HType::kFloat, {HType::kFloat, HType::kFloat})  \
  __(kFPow, "fpow", RCallFPow, HType::kFloat, {HType::kFloat, HType::kFloat})  \
  __(kGCFree, "gcfree", RCallGCFree, HType::kNone, {HType::kGC})               \
  /** 2str **/                                                                 \
  __(kInt32ToStr, "int32_to_str", RCallInt32ToStr, HType::kPtr, {HType::kInt}) \
  __(kFloat64ToStr, "float64_to_str", RCallFloat64ToStr, HType::kPtr,          \
     {HType::kFloat})                                                          \
  __(kObjectToStr, "object_to_str", RCallObjectToStr, HType::kPtr,             \
     {HType::kPtr})                                                            \
  __(kStringToFloat64, "string_to_float64", RCallStringToFloat64,              \
     HType::kFloat, {HType::kPtr})                                             \
  /** str **/                                                                  \
  __(kStrCon, "str_con", RCallStrCon, HType::kPtr, {HType::kPtr, HType::kPtr}) \
  __(kStrLt, "str_lt", RCallStrLt, HType::kInt, {HType::kPtr, HType::kPtr})    \
  __(kStrLe, "str_le", RCallStrLe, HType::kInt, {HType::kPtr, HType::kPtr})    \
  __(kStrGt, "str_gt", RCallStrGt, HType::kInt, {HType::kPtr, HType::kPtr})    \
  __(kStrGe, "str_ge", RCallStrGe, HType::kInt, {HType::kPtr, HType::kPtr})    \
  __(kStrNe, "str_ne", RCallStrNe, HType::kInt, {HType::kPtr, HType::kPtr})    \
  __(kStrEq, "str_eq", RCallStrEq, HType::kInt, {HType::kPtr, HType::kPtr})    \
  /** cmp **/                                                                  \
  __(kCmpLt, "cmp_lt", RCallCmpLt, HType::kBox,                                \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kCmpLe, "cmp_le", RCallCmpLe, HType::kBox,                                \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kCmpGt, "cmp_gt", RCallCmpGt, HType::kBox,                                \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kCmpGe, "cmp_ge", RCallCmpGe, HType::kBox,                                \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kCmpNe, "cmp_ne", RCallCmpNe, HType::kBox,                                \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kCmpEq, "cmp_eq", RCallCmpEq, HType::kBox,                                \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  /** misc **/                                                                 \
  __(kAddBrand, "add_brand", RCallAddBrand, HType::kBox,                       \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kCheckBrand, "check_brand", RCallCheckBrand, HType::kBox,                 \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kRegexp, "regexp", RCallRegexp, HType::kBox,                              \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kGetSuper, "get_super", RCallGetSuper, HType::kBox, {HType::kBoxRef})     \
  __(kImport, "import", RCallImport, HType::kBox, {HType::kBoxRef})            \
  __(kSetProto, "set_proto", RCallSetProto, HType::kBox,                       \
     {HType::kBoxRef, HType::kBoxRef}, )                                       \
  __(kSetHomeObject, "set_home_object", RCallSetHomeObject, HType::kNone,      \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  /** atom **/                                                                 \
  __(kAtomToValue, "atom_to_value", RCallAtomToValue, HType::kInt,             \
     {HType::kInt})                                                            \
  __(kValueToAtom, "value_to_atom", RCallValueToAtom, HType::kInt,             \
     {HType::kBoxRef})                                                         \
  /** property value **/                                                       \
  __(kSetPropertyValue, "set_property_value", RCallSetPropertyValue,           \
     HType::kBox,                                                              \
     {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef, HType::kInt})            \
  __(kDefinePropertyValueValue, "define_property_value_value",                 \
     RCallDefinePropertyValueValue, HType::kBox,                               \
     {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef, HType::kInt})            \
  /** array **/                                                                \
  __(kLoadArray, "load_array", RCallLoadArray, HType::kBox,                    \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  __(kLoadArrayInt, "load_array_int", RCallLoadArrayInt, HType::kBox,          \
     {HType::kPtr, HType::kInt})                                               \
  /** super value **/                                                          \
  __(kGetSuperValue, "get_super_value", RCallGetSuperValue, HType::kBox,       \
     {HType::kBoxRef, HType::kInt, HType::kBoxRef})                            \
  __(kPutSuperValue, "putt_super_value", RCallPutSuperValue, HType::kBox,      \
     {HType::kBoxRef, HType::kInt, HType::kBoxRef, HType::kBoxRef})            \
  /** global **/                                                               \
  __(kGlobalCheck, "global_check", RCallGlobalCheck, HType::kBox,              \
     {HType::kInt})                                                            \
  __(kGlobalGet, "global_get", RCallGlobalGet, HType::kBox,                    \
     {HType::kInt, HType::kInt})                                               \
  __(kGlobalPut, "global_put", RCallGlobalPut, HType::kBox,                    \
     {HType::kInt, HType::kPtr, HType::kInt})                                  \
  __(kGlobalDefine, "global_define", RCallGlobalDefine, HType::kBox,           \
     {HType::kInt, HType::kInt})                                               \
  __(kGlobalDefineFunction, "global_define_function",                          \
     RCallGlobalDefineFunction, HType::kBox,                                   \
     {HType::kInt, HType::kPtr, HType::kInt})                                  \
  __(kGlobalCheckDefine, "global_check_define", RCallGlobalCheckDefine,        \
     HType::kBox, {HType::kInt, HType::kInt})                                  \
  /** private field **/                                                        \
  __(kPrivateFieldPut, "private_field_put", RCallPrivateFieldPut, HType::kBox, \
     {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef})                         \
  __(kPrivateFieldDefine, "private_field_define", RCallPrivateFieldDefine,     \
     HType::kBox, {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef})            \
  /** property access **/                                                      \
  __(kObjectFieldGetAtom, "object_field_get_atom", RCallObjectFieldGetAtom,    \
     HType::kBox, {HType::kPtr, HType::kInt})                                  \
  __(kObjectFieldPutAtom, "object_field_put_atom", RCallObjectFieldPutAtom,    \
     HType::kBox, {HType::kPtr, HType::kInt, HType::kPtr})                     \
  __(kObjectFieldDefineAtom, "object_field_define_atom",                       \
     RCallObjectFieldDefineAtom, HType::kBox,                                  \
     {HType::kPtr, HType::kInt, HType::kPtr})                                  \
  __(kObjectFieldNameSet, "object_field_name_set", RCallObjectFieldSetName,    \
     HType::kBox, {HType::kPtr, HType::kInt})                                  \
  __(kObjectFieldNameSetComputed, "object_field_name_set_computed",            \
     RCallObjectFieldSetNameComputed, HType::kBox, {HType::kPtr, HType::kPtr})

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
