#ifndef _QUAKE_OPCODE_H_
#define _QUAKE_OPCODE_H_
#include "quake-C.h"

#include "base.h"
#include "type.h"

// ----------------------------------------------------------------------------
// Quake's IR
//
// The design of quake's IR is based on SSA style but tries to keep it as
// compact as possible
//
// About the PHI node. The phi is been categorized into different sub-types
// to allow simpler type inference. The PHI node that contains same type or
// similar type, ie both are numbers(int/float), and can be casted implicitly
// is been orgnized into PhiS. For type that totally does not to be the same
// is orgnized otherwise.

// IR, Name, Arity, Flag

#define QUAKE_OPCODE_LIST(__)                                                  \
  __(kSentry, "sentry", 0, IREffect::Op(), Type::kNone)                        \
  /** constant loading **/                                                     \
  __(kKI32, "ki32", 0, IREffect::InternalValue(), Type::kInt)                  \
  __(kKI64, "ki64", 0, IREffect::InternalValue(), Type::kI64)                  \
  __(kKFloat64, "kreal", 0, IREffect::InternalValue(), Type::kFloat64)         \
  __(kKBool, "kbool", 0, IREffect::InternalValue(), Type::kBool)               \
  __(kKNull, "knull", 0, IREffect::InternalValue(), Type::kNull)               \
  __(kKUndef, "kundef", 0, IREffect::InternalValue(), Type::kUndefined)        \
  __(kKUninit, "kuninit", 0, IREffect::InternalValue(), Type::kUninitialized)  \
  __(kKStr, "kstr", 0, IREffect::InternalValue(), Type::kUninitialized)        \
  /** frame **/                                                                \
  __(kEnterInlineFrame, "enter_inline_frame", kNarity,                         \
     IREffect::SideEffectOp(), Type::kNone)                                    \
  __(kLeaveInlineFrame, "leave_inline_frame", 1, IREffect::SideEffectOp(),     \
     Type::kNone)                                                              \
  __(kCloseVarRef, "close_var_ref", 1, IREffect::Op(), Type::kNone)            \
  /** string loading **/                                                       \
  __(kLoadStr, "load_str", 1, IREffect::SideEffectValue(), Type::kString)      \
  /** ref counting **/                                                         \
  /* notes all the ref-counting is pinned */                                   \
  __(kRcInc, "rc_inc", 1, IREffect::InternalSideEffectOp(), Type::kNone)       \
  __(kRcDec, "rc_dec", 1, IREffect::InternalSideEffectOp(), Type::kNone)       \
  __(kRcTryInc, "rc_tryinc", 1, IREffect::InternalSideEffectOp(), Type::kNone) \
  __(kRcTryDec, "rc_trydec", 1, IREffect::InternalSideEffectOp(), Type::kNone) \
  __(kRcTryDecTemp, "rc_trydec_temp", 1, IREffect::InternalSideEffectOp(),     \
     Type::kNone)                                                              \
  __(kRcOp, "rc_op", kNarity, IREffect::SideEffectOp(), Type::kNone)           \
  /** box/unbox **/                                                            \
  __(kBxGuard, "bx_guard", 1, IREffect::GuardValue(), Type::kInfer)            \
  __(kUnboxPtr, "unbox_ptr", 1, IREffect::Value(), Type::kInfer)               \
  __(kBxToTemp, "bx_to_temp", 2, IREffect::SideEffectOp(), Type::kNone)        \
  __(kBxToLocal, "bx_to_local", 2, IREffect::SideEffectOp(), Type::kNone)      \
  __(kBxToParam, "bx_to_param", 2, IREffect::SideEffectOp(), Type::kNone)      \
  /** atom <-> box **/                                                         \
  __(kBoxToAtom, "box_to_atom", 1, IREffect::Value(), Type::kInt)              \
  __(kAtomToBox, "atom_to_box", 1, IREffect::Value(), Type::kBox)              \
  /** deopt **/                                                                \
  __(kSnapshot, "snapshot", 0, IREffect::SideEffectOp(), Type::kNone)          \
  __(kDeopt, "deopt", 0, IREffect::GuardOp(), Type::kNone)                     \
  /** return **/                                                               \
  __(kReturn, "return", 1, IREffect::SideEffectOp(), Type::kNone)              \
  /** guard **/                                                                \
  __(kGIsIntPos, "g_is_int_pos", 1, IREffect::GuardValue(), Type::kInt)        \
  __(kGIsIntPosOrZero, "g_is_int_pos_or_zero", 1, IREffect::GuardValue(),      \
     Type::kInt)                                                               \
  __(kGIsNotZero, "g_is_not_zero", 1, IREffect::GuardValue(), Type::kInt)      \
  __(kGIsIntNeg, "g_is_int_neg", 1, IREffect::GuardValue(), Type::kInt)        \
  __(kGIsNotIntMax, "g_is_not_int_max", 1, IREffect::GuardValue(), Type::kInt) \
  __(kGIsNotIntMin, "g_is_not_int_min", 1, IREffect::GuardValue(), Type::kInt) \
  __(kGIsNotException, "g_is_not_exception", 1, IREffect::SideEffectGuardOp(), \
     Type::kNone)                                                              \
  __(kGObjectIsScriptFunction, "g_object_is_script_function", 1,               \
     IREffect::SideEffectGuardOp(), Type::kNone)                               \
  __(kGIsNotUninitialized, "g_is_not_uninitialized", 1,                        \
     IREffect::SideEffectGuardOp(), Type::kNone)                               \
  __(kGIsUninitialized, "g_is_uninitialized", 1,                               \
     IREffect::SideEffectGuardOp(), Type::kNone)                               \
  __(kGIsTrue, "g_is_true", 1, IREffect::SideEffectGuardOp(), Type::kNone)     \
  __(kGIsFalse, "g_is_false", 1, IREffect::SideEffectGuardOp(), Type::kNone)   \
  __(kGIsNotUndefined, "g_is_not_undefined", 1, IREffect::GuardOp(),           \
     Type::kNone)                                                              \
  __(kGBoxToAtom, "g_box_to_atom", 1, IREffect::Value(), Type::kInt)           \
  /** string manipualtion **/                                                  \
  __(kStrCon, "str_con", 2, IREffect::SideEffectValue(), Type::kString)        \
  __(kStrLen, "str_len", 1, IREffect::SideEffectValue(), Type::kInt)           \
  __(kStrFieldGet, "str_field_get", 2, IREffect::SideEffectValue(),            \
     Type::kBox)                                                               \
  /** for of **/                                                               \
  __(kForOfStart, "for_of_start", 1, IREffect::SideEffectValue(), Type::kBox)  \
  __(kForOfNext, "for_of_next", 2, IREffect::SideEffectValue(), Type::kBox)    \
  /** for in **/                                                               \
  __(kForInStart, "for_in_start", 1, IREffect::SideEffectValue(), Type::kBox)  \
  __(kForInNext, "for_in_next", 1, IREffect::SideEffectValue(), Type::kBox)    \
  /** iterator **/                                                             \
  __(kIterClose, "iter_close", 1, IREffect::SideEffectValue(), Type::kBox)     \
  /** context **/                                                              \
  __(kContextLoad, "context_load", 0, IREffect::Value(), Type::kObject)        \
  __(kContextLoadGlobalObject, "context_load_global_object", 0,                \
     IREffect::Value(), Type::kObject)                                         \
  /** special object **/                                                       \
  __(kGThisIsNullOrUndefined, "g_this_is_null_or_undefined", 0,                \
     IREffect::GuardOp(), Type::kNone)                                         \
  __(kLoadThisObject, "load_this_object", 0, IREffect::Value(), Type::kBox)    \
  /*** FIXME(dpeng): Why this is a side effect operation ? ***/                \
  __(kLoadStackFrame, "load_stack_frame", 0, IREffect::SideEffectValue(),      \
     Type::kPtr)                                                               \
  __(kLoadCurFunc, "load_cur_func", 0, IREffect::SideEffectValue(),            \
     Type::kBox)                                                               \
  __(kLoadNewTarget, "load_new_target", 0, IREffect::SideEffectValue(),        \
     Type::kBox)                                                               \
  __(kLoadHomeObject, "load_home_object", 0, IREffect::SideEffectValue(),      \
     Type::kBox)                                                               \
  __(kLoadVarObject, "load_var_object", 0, IREffect::Value(), Type::kBox)      \
  __(kImportMeta, "import_meta", 0, IREffect::Value(), Type::kBox)             \
  /** misc **/                                                                 \
  __(kJSSetProto, "js_set_proto", 2, IREffect::SideEffectValue(), Type::kBox)  \
  __(kJSSetHomeObject, "js_set_home_object", 2, IREffect::SideEffectOp(),      \
     Type::kNone)                                                              \
  __(kJSAddBrand, "js_add_brand", 2, IREffect::SideEffectValue(), Type::kBox)  \
  __(kJSCheckBrand, "js_check_brand", 2, IREffect::SideEffectValue(),          \
     Type::kBox)                                                               \
  __(kJSRegexp, "js_regexp", 2, IREffect::Value(), Type::kBox)                 \
  __(kJSGetSuper, "js_get_super", 1, IREffect::SideEffectValue(), Type::kBox)  \
  __(kJSImport, "js_import", 1, IREffect::SideEffectValue(), Type::kBox)       \
  __(kJSGetSuperValue, "js_get_super_value", kNarity,                          \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kJSPutSuperValue, "js_put_super_value", kNarity,                          \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kSetPropertyValue, "js_set_property_value", kNarity,                      \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kDefinePropertyValueValue, "js_define_property_value_value", kNarity,     \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kCopyDataProperties, "copy_data_properties", kNarity, IREffect::Value(),  \
     Type::kBox)                                                               \
  __(kAppend, "append", kNarity, IREffect::SideEffectValue(), Type::kBox)      \
  __(kNewClosure, "new_closure", 2, IREffect::SideEffectValue(), Type::kBox)   \
  /** array property **/                                                       \
  __(kLoadArray, "load_array", 2, IREffect::SideEffectValue(), Type::kBox)     \
  /*** specialized version ***/                                                \
  __(kLoadArrayValue, "load_array_value", 2, IREffect::SideEffectValue(),      \
     Type::kBox)                                                               \
  __(kLoadArrayArguments, "load_array_arguments", 2,                           \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kLoadArrayInt8, "load_array_int8", 2, IREffect::SideEffectValue(),        \
     Type::kBox)                                                               \
  __(kLoadArrayUInt8, "load_array_uint8", 2, IREffect::SideEffectValue(),      \
     Type::kBox)                                                               \
  __(kLoadArrayUInt8C, "load_array_uint8c", 2, IREffect::SideEffectValue(),    \
     Type::kBox)                                                               \
  __(kLoadArrayInt16, "load_array_int16", 2, IREffect::SideEffectValue(),      \
     Type::kBox)                                                               \
  __(kLoadArrayUInt16, "load_array_uint16", 2, IREffect::SideEffectValue(),    \
     Type::kBox)                                                               \
  __(kLoadArrayInt32, "load_array_int32", 2, IREffect::SideEffectValue(),      \
     Type::kBox)                                                               \
  __(kLoadArrayUInt32, "load_array_uint32", 2, IREffect::SideEffectValue(),    \
     Type::kBox)                                                               \
  __(kLoadArrayF32, "load_array_f32", 2, IREffect::SideEffectValue(),          \
     Type::kBox)                                                               \
  __(kLoadArrayF64, "load_array_f64", 2, IREffect::SideEffectValue(),          \
     Type::kBox)                                                               \
  /** private field **/                                                        \
  __(kPrivateFieldPut, "private_field_put", kNarity, IREffect::SideEffectOp(), \
     Type::kBox)                                                               \
  __(kPrivateFieldDefine, "private_field_define", kNarity,                     \
     IREffect::SideEffectOp(), Type::kBox)                                     \
  /** object property **/                                                      \
  __(kLoadPrimitiveProto, "load_primitive_proto", 1, IREffect::Value(),        \
     Type::kBox)                                                               \
  __(kObjectNew, "object_new", 0, IREffect::SideEffectValue(), Type::kBox)     \
  __(kArrayLengthGet, "array_length_get", 1, IREffect::SideEffectValue(),      \
     Type::kBox)                                                               \
  __(kObjectFieldGet, "object_field_get", kNarity,                             \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kObjectFieldPut, "object_field_put", kNarity,                             \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kObjectFieldDefine, "object_field_define", kNarity,                       \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  __(kObjectNameSet, "object_name_set", 2, IREffect::SideEffectValue(),        \
     Type::kBox)                                                               \
  __(kObjectNameSetComputed, "object_name_set_computed", 2,                    \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  /** global **/                                                               \
  __(kGlobalCheck, "global_check", 2, IREffect::SideEffectValue(), Type::kBox) \
  __(kGlobalGet, "global_get", 2, IREffect::SideEffectValue(), Type::kBox)     \
  __(kGlobalGetVar, "global_get_var", 2, IREffect::SideEffectValue(),          \
     Type::kBox)                                                               \
  __(kGlobalGetGlobal, "global_get_global", 2, IREffect::SideEffectValue(),    \
     Type::kBox)                                                               \
  __(kGlobalSet, "global_set", kNarity, IREffect::SideEffectValue(),           \
     Type::kBox)                                                               \
  __(kGlobalDefine, "global_define", 2, IREffect::SideEffectValue(),           \
     Type::kBox)                                                               \
  __(kGlobalCheckDefine, "global_check_define", 2, IREffect::SideEffectOp(),   \
     Type::kBox)                                                               \
  __(kGlobalDefineFunction, "global_define_function", 2,                       \
     IREffect::SideEffectValue(), Type::kBox)                                  \
  /** nested loop **/                                                          \
  __(kEnterJitTrace, "enter_jit_trace", kNarity, IREffect::SideEffectOp(),     \
     Type::kNone)                                                              \
  /** function call **/                                                        \
  __(kApply, "apply", kNarity, IREffect::SideEffectValue(), Type::kBox)        \
  __(kArrayFrom, "array_from", 2, IREffect::SideEffectValue(), Type::kBox)     \
  __(kJSCall, "js_call", kNarity, IREffect::SideEffectValue(), Type::kBox)     \
  __(kJSCCall, "js_ccall", kNarity, IREffect::SideEffectValue(), Type::kBox)   \
  /** local variables **/                                                      \
  __(kLocGet, "loc_get", 1, IREffect::SideEffectValue(), Type::kBox)           \
  __(kLocSet, "loc_set", 2, IREffect::Op(), Type::kNone)                       \
  __(kLocNullify, "loc_nullify", 1, IREffect::Op(), Type::kNone)               \
  /** argument **/                                                             \
  __(kParamGet, "param_get", 1, IREffect::SideEffectValue(), Type::kBox)       \
  __(kParamSet, "param_set", 2, IREffect::SideEffectOp(), Type::kNone)         \
  __(kParamNullify, "param_nullify", 1, IREffect::Op(), Type::kNone)           \
  /** var ref **/                                                              \
  __(kVarRefGet, "var_ref_get", 1, IREffect::SideEffectValue(), Type::kBox)    \
  __(kVarRefSet, "var_ref_set", 2, IREffect::SideEffectOp(), Type::kNone)      \
  /** temp **/                                                                 \
  __(kTempGet, "temp_get", 1, IREffect::SideEffectValue(), Type::kBox)         \
  /*** internal temporary value, used to help us to perform reemit ***/        \
  __(kTempNullify, "temp_nullify", 1, IREffect::Op(), Type::kNone)             \
  __(kTempSet, "temp_set", 2, IREffect::Op(), Type::kNone)                     \
  __(kTempPush, "temp_push", 1, IREffect::Op(), Type::kNone)                   \
  __(kTempPop, "temp_push", 1, IREffect::Op(), Type::kNone)                    \
  /** arithmetic **/                                                           \
  __(kAddInt, "add_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kAddReal, "add_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kGAddInt, "g_add_int", 2, IREffect::GuardValue(), Type::kInt)             \
  __(kSubInt, "sub_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kSubReal, "sub_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kGSubInt, "g_sub_int", 2, IREffect::GuardValue(), Type::kInt)             \
  __(kMulInt, "mul_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kMulReal, "mul_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kGMulInt, "g_mul_int", 2, IREffect::GuardValue(), Type::kInt)             \
  __(kDivReal, "div_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kModInt, "mod_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kModReal, "mod_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kPowReal, "pow_real", 2, IREffect::Value(), Type::kFloat64)               \
  /** bit operation **/                                                        \
  __(kGShlInt, "gshl_int", 2, IREffect::GuardValue(), Type::kInt)              \
  __(kGShrInt, "gshr_int", 2, IREffect::GuardValue(), Type::kInt)              \
  __(kSarInt, "sar_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kBitNorm, "bit_norm", 1, IREffect::Value(), Type::kInt)                   \
  __(kAndInt, "and_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kOrInt, "or_int", 2, IREffect::Value(), Type::kInt)                       \
  __(kXorInt, "xor_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kInvInt, "inv_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kFlipBool, "flip_bool", 1, IREffect::Value(), Type::kBool)                \
  /** comparison **/                                                           \
  __(kIsType, "is_type", 2, IREffect::Value(), Type::kBool)                    \
  __(kIsTypeUndefinedOrNull, "is_type_undefined_or_null", 2,                   \
     IREffect::Value(), Type::kBool)                                           \
  /** FIXME(dpeng) Add string specialization **/                               \
  __(kLtInt, "lt_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kLtReal, "lt_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kLeInt, "le_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kLeReal, "le_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kGtInt, "gt_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kGtReal, "gt_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kGeInt, "ge_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kGeReal, "ge_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kEqInt, "eq_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kEqReal, "eq_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kNeInt, "ne_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kNeReal, "ne_int", 2, IREffect::Value(), Type::kBool)                     \
  __(kLtStr, "lt_str", 2, IREffect::Value(), Type::kBool)                      \
  __(kLeStr, "le_str", 2, IREffect::Value(), Type::kBool)                      \
  __(kGtStr, "gt_str", 2, IREffect::Value(), Type::kBool)                      \
  __(kGeStr, "ge_str", 2, IREffect::Value(), Type::kBool)                      \
  __(kEqStr, "eq_str", 2, IREffect::Value(), Type::kBool)                      \
  __(kNeStr, "ne_str", 2, IREffect::Value(), Type::kBool)                      \
  /*** special case, used for none comparison usage, ie ===/!== ***/           \
  __(kEqPtr, "eq_ptr", 2, IREffect::Value(), Type::kBool)                      \
  __(kNePtr, "ne_ptr", 2, IREffect::Value(), Type::kBool)                      \
  /*** general case comparsion, can bailout ***/                               \
  __(kNe, "ne", 2, IREffect::GuardValue(), Type::kBox)                         \
  __(kEq, "eq", 2, IREffect::GuardValue(), Type::kBox)                         \
  __(kLt, "lt", 2, IREffect::GuardValue(), Type::kBox)                         \
  __(kLe, "le", 2, IREffect::GuardValue(), Type::kBox)                         \
  __(kGt, "gt", 2, IREffect::GuardValue(), Type::kBox)                         \
  __(kGe, "ge", 2, IREffect::GuardValue(), Type::kBox)                         \
  /** unary **/                                                                \
  __(kGNegInt, "g_neg_int", 1, IREffect::GuardValue(), Type::kInt)             \
  __(kNegInt, "neg_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kNegReal, "neg_real", 1, IREffect::Value(), Type::kFloat64)               \
  __(kGIncInt, "g_inc_int", 1, IREffect::GuardValue(), Type::kInt)             \
  __(kIncInt, "inc_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kIncReal, "inc_real", 1, IREffect::Value(), Type::kFloat64)               \
  __(kGDecInt, "g_dec_int", 1, IREffect::GuardValue(), Type::kInt)             \
  __(kDecInt, "dec_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kDecReal, "dec_real", 1, IREffect::Value(), Type::kFloat64)               \
  __(kInstanceOf, "instanceof", 2, IREffect::Value(), Type::kBox)              \
  __(kIn, "in", 2, IREffect::Value(), Type::kBox)                              \
  __(kTypeOf, "typeof", 1, IREffect::Value(), Type::kString)                   \
  __(kDelete, "delete", 2, IREffect::Value(), Type::kBox)                      \
  __(kDeleteVar, "delete_var", 1, IREffect::Value(), Type::kBox)               \
  /** high level cast **/                                                      \
  __(kToObject, "to_object", 1, IREffect::Value(), Type::kBox)                 \
  __(kToPropKey, "to_propkey", 1, IREffect::Value(), Type::kBox)               \
  /** cast **/                                                                 \
  __(kI2F, "i2f", 1, IREffect::Value(), Type::kFloat64)                        \
  __(kF2Bit, "f2bit", 1, IREffect::GuardValue(), Type::kInt)                   \
  __(kI2B, "i2b", 1, IREffect::Value(), Type::kBool)                           \
  __(kF2B, "f2b", 1, IREffect::Value(), Type::kBool)                           \
  __(kS2B, "s2b", 1, IREffect::Value(), Type::kBool)                           \
  __(kO2B, "o2b", 1, IREffect::Value(), Type::kBool)                           \
  __(kV2B, "v2b", 1, IREffect::Value(), Type::kBool)                           \
  __(kB2F, "b2f", 1, IREffect::Value(), Type::kFloat64)                        \
  __(kB2I, "b2i", 1, IREffect::Value(), Type::kInt)                            \
  __(kB2S, "b2s", 1, IREffect::Value(), Type::kString)                         \
  __(kI2S, "i2s", 1, IREffect::Value(), Type::kString)                         \
  __(kF2S, "f2s", 1, IREffect::Value(), Type::kString)                         \
  __(kO2S, "o2s", 1, IREffect::Value(), Type::kString)                         \
  __(kS2F, "s2f", 1, IREffect::Value(), Type::kFloat64)                        \
  /** phi **/                                                                  \
  __(kPhiLoop, "phi_loop", 2, IREffect::Value(), Type::kInfer)                 \
  /** dup a value **/                                                          \
  __(kDup, "dup", 1, IREffect::Value(), Type::kInfer)                          \
  /** control flow **/                                                         \
  /** a dummy control flow node, ie does nothing but just mark **/             \
  __(kStart, "start", 0, IREffect::ControlFlow(), Type::kNone)                 \
  __(kBB1, "bb1", 0, IREffect::ControlFlow(), Type::kNone)                     \
  __(kJump, "jump", 1, IREffect::ControlFlow(), Type::kNone)                   \
  __(kEnd, "end", 0, IREffect::ControlFlow(), Type::kNone)

namespace quake {
namespace ir {

// Used to point to a SSA defined value
class SSAVar;
class Checkpoint;

class IREffect {
 public:
  enum {
    // Whether this opcode has side-effect, ie generate a checkpoint after the
    // instruction is done
    kSideEffect = base::BitOn<std::uint32_t, 1>::kValue,

    // Whether this opcode is a guard, ie try to jump to previously saved
    // checkpoint. Notes an opcode that is guard does not generate checkpoint,
    // but just reuse the previously saved checkpoint
    kGuard = base::BitOn<std::uint32_t, 2>::kValue,

    // Whether this opcode is a value opcode, ie generate some value to be
    // used
    // by others. If this bit is 0, it means this node is not a value node but
    // a OP node, ie just consume operands but not produce operands
    kValue = base::BitOn<std::uint32_t, 3>::kValue,

    // Whether this opcode is a control flow opcode
    kControlFlow = base::BitOn<std::uint32_t, 4>::kValue,

    // Whether this opcode may error out due to check error
    kException = base::BitOn<std::uint32_t, 5>::kValue,

    // Whether this instruction is a internal node, ie it is always a part of
    // some other node and should not be tracked directly inside of CFG. For
    // example, the RcDec/RcInc is both internal nodes for RcOps nodes
    kInternal = base::BitOn<std::uint32_t, 6>::kValue
  };

 public:
  explicit IREffect(std::uint32_t v) : v_(v) {}

  IREffect() : v_(0) {}

  // clang-format off
  IREffect(bool is_side_effect, 
           bool is_guard, 
           bool is_value,
           bool is_control_flow, 
           bool is_exception,
           bool is_internal)
      : v_(0) {

    if (is_side_effect) SetSideEffect();
    if (is_guard) SetGuard();

    if (is_value) {
      CHECK(!is_control_flow);
      SetValue();
    } else if (is_control_flow) {
      SetControlFlow();
    }

    if (is_exception) SetException();
    if (is_internal) SetInternal();
  }
  // clang-format on

 public:
  static IREffect SideEffectValue() { return IREffect{kSideEffect | kValue}; }
  static IREffect GuardValue() { return IREffect{kValue | kGuard}; }
  static IREffect SideEffectGuardValue() {
    return IREffect{kSideEffect | kGuard | kValue};
  }
  static IREffect Value() { return IREffect{kValue}; }
  static IREffect Op() { return IREffect{0}; }
  static IREffect GuardOp() { return IREffect{kGuard}; }
  static IREffect SideEffectOp() { return IREffect{kSideEffect}; }
  static IREffect SideEffectGuardOp() { return IREffect{kSideEffect | kGuard}; }
  static IREffect ControlFlow() { return IREffect{kControlFlow}; }

  static IREffect InternalValue() { return IREffect{kInternal | kValue}; }
  static IREffect InternalSideEffectValue() {
    return IREffect{kInternal | kSideEffect | kValue};
  }
  static IREffect InternalSideEffectOp() {
    return IREffect{kInternal | kSideEffect};
  }

 public:
  bool IsSideEffect() const { return v_ & kSideEffect; }
  void ClearSideEffect() { v_ &= ~kSideEffect; }
  void SetSideEffect() { v_ |= kSideEffect; }

  bool IsValue() const { return v_ & kValue; }
  void ClearValue() { v_ &= ~kValue; }
  void SetValue() { v_ |= kValue; }

  bool IsControlFlow() const { return v_ & kControlFlow; }
  void ClearControlFlow() { v_ &= ~kControlFlow; }
  void SetControlFlow() { v_ |= kControlFlow; }

  bool IsOp() const { return !IsValue() && !IsControlFlow(); }

  bool IsGuard() const { return v_ & kGuard; }
  void SetGuard() { v_ |= kGuard; }
  void ClearGuard() { v_ &= ~kGuard; }

  bool IsException() const { return v_ & kException; }
  void ClearException() { v_ &= ~kException; }
  void SetException() { v_ |= kException; }

  bool IsInternal() const { return v_ & kInternal; }
  void ClearInternal() { v_ &= ~kInternal; }
  void SetInternal() { v_ |= kInternal; }

 public:
  bool RequireBailout() const { return IsGuard(); }

  std::uint32_t v() const { return v_; }

 private:
  std::uint32_t v_;
};

constexpr int kNarity = -1;

struct OpcodeInfo {
  std::string name;  // name of opcode
  int arity;         // argument count
  IREffect effect;   // flags of opcode
  Type type;         // type of opcode

  // Dyanmic sized arguments
  bool IsNary() const { return arity < 0; }
};

enum class Opcode : std::uint32_t {
// clang-format off
#define __(A, ...) A,
  QUAKE_OPCODE_LIST(__)
  kSizeOfOpcode
#undef __  // __
  // clang-format on
};

constexpr std::size_t kSizeOfOpcode =
    static_cast<std::size_t>(Opcode::kSizeOfOpcode);

std::optional<Opcode> OpcodeParse(const std::string_view&);

const OpcodeInfo& GetOpcodeInfo(Opcode);

bool OpcodeNeedFloatOperand(Opcode);
bool OpcodeNeedIntOperand(Opcode);

// Categories of each OPCODE
bool OpcodeIsConst(Opcode);
bool OpcodeIsPhi(Opcode);
bool OpcodeIsJump(Opcode);
bool OpcodeIsArithmetic(Opcode);
bool OpcodeIsBinaryArithmetic(Opcode);
bool OpcodeIsUnaryArithmetic(Opcode);
bool OpcodeIsCast(Opcode);
bool OpcodeIsBookkeep(Opcode);
bool OpcodeIsFrame(Opcode);

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_OPCODE_H_
