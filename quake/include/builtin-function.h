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
  __(kSetProto, "set_proto", HType::kBox, {HType::kBoxRef, HType::kBoxRef})    \
  __(kSetHomeObject, "set_home_object", HType::kNone,                          \
     {HType::kBoxRef, HType::kBoxRef})                                         \
  /** atom **/                                                                 \
  __(kAtomToValue, "atom_to_value", HType::kBox, {HType::kInt})                \
  __(kValueToAtom, "value_to_atom", HType::kInt, {HType::kBoxRef})             \
  /** property value **/                                                       \
  __(kSetPropertyValue, "set_property_value", HType::kBox,                     \
     {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef, HType::kInt})            \
  __(kDefinePropertyValueValue, "define_property_value_value", HType::kBox,    \
     {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef, HType::kInt})            \
  /** array **/                                                                \
  __(kLoadArray, "load_array", HType::kBox, {HType::kBoxRef, HType::kBoxRef})  \
  __(kLoadArrayInt, "load_array_int", HType::kBox, {HType::kPtr, HType::kInt}) \
  /** super value **/                                                          \
  __(kGetSuperValue, "get_super_value", HType::kBox,                           \
     {HType::kBoxRef, HType::kInt, HType::kBoxRef})                            \
  __(kPutSuperValue, "putt_super_value", HType::kBox,                          \
     {HType::kBoxRef, HType::kInt, HType::kBoxRef, HType::kBoxRef})            \
  /** global **/                                                               \
  __(kGlobalCheck, "global_check", HType::kBox, {HType::kInt})                 \
  __(kGlobalGet, "global_get", HType::kBox, {HType::kInt, HType::kInt})        \
  __(kGlobalPut, "global_put", HType::kBox,                                    \
     {HType::kInt, HType::kPtr, HType::kInt})                                  \
  __(kGlobalDefine, "global_define", HType::kBox, {HType::kInt, HType::kInt})  \
  __(kGlobalDefineFunction, "global_define_function", HType::kBox,             \
     {HType::kInt, HType::kPtr, HType::kInt})                                  \
  __(kGlobalCheckDefine, "global_check_define", HType::kBox,                   \
     {HType::kInt, HType::kInt})                                               \
  /** private field **/                                                        \
  __(kPrivateFieldPut, "private_field_put", HType::kBox,                       \
     {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef})                         \
  __(kPrivateFieldDefine, "private_field_define", HType::kBox,                 \
     {HType::kBoxRef, HType::kBoxRef, HType::kBoxRef})                         \
  /** property access **/                                                      \
  __(kObjectFieldGetAtom, "object_field_get_atom", HType::kBox,                \
     {HType::kPtr, HType::kInt})                                               \
  __(kObjectFieldPutAtom, "object_field_put_atom", HType::kBox,                \
     {HType::kPtr, HType::kInt, HType::kPtr})                                  \
  __(kObjectFieldDefineAtom, "object_field_define_atom", HType::kBox,          \
     {HType::kPtr, HType::kInt, HType::kPtr})                                  \
  __(kObjectFieldNameSet, "object_field_name_set", HType::kBox,                \
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
