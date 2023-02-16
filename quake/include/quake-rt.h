#ifndef _QUAKE_QUAKE_RT_H_
#define _QUAKE_QUAKE_RT_H_
#include "base.h"
#include "quick-js-inc.h"
#include "quake-C.h"
#include "builtin-function.h"
#include "cg/cg.h"

namespace quake {
class Quake;

// --------------------------------------------------------------------------
// Runtime Functions
//
// QuakeJit exposes all the needed function for JITted code to coordinate with
struct QuakeJit {
  // --------------------------------------------------------------------------
  // Helping to prepare the deoptimization information for interpreter to return
  // the prepared the data is been hide inside of the QuakeRT object. And will
  // be used by interpreter to reconstruct its frame for execution recovery
  // inside of quickjs.c
  static std::uint32_t DeoptPrepare(void* start, void* end);

  enum UnreachableCode {
    kUnreachable,
    kUnreachableEndOfBody,
    kUnreachableEndOfPrologue,
    kUnreachableEndOfEpilogue,
    kUnreachableEndOfDeoptimization,
    kUnreachableEndOfCastF64ToB32,
    kUnreachableEndOfFMod,

    kSizeOfUnreachable
  };

 public:
  // Utility functions, used by various parts of the code, ie optimization,
  // runtime, etc ...
  static double JSPow(double, double);

 public:
  static const char* GetUnreachableCodeName(int);
  static void Unreachable(int);

 public:
  // debugging helpers
  static void PrintI32(std::int32_t value);
  static void PrintI64(std::int64_t value);
  static void PrintF64(double value);

 public:
  static const void* MapRCall(BuiltinFunction);

  static double RCallFMod(::QuakeRT*, double, double);
  static double RCallFPow(::QuakeRT*, double, double);
  static void RCallGCFree(::QuakeRT*, void*, int);

  static void* RCallTypeOf(::QuakeRT*, JSValue*);
  static JSValue RCallDelete(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallDeleteVar(::QuakeRT*, JSAtom);
  static JSValue RCallInstanceOf(::QuakeRT*, JSValue*, JSValue*);

  // --------------------------------------------------------------------------
  // high level cast
  static JSValue RCallToObject(::QuakeRT*, JSValue*);
  static JSValue RCallToPropKey(::QuakeRT*, JSValue*);

  // --------------------------------------------------------------------------
  // string
  static void* RCallStrCon(::QuakeRT*, void*, void*);
  static JSValue RCallStrFieldGet(::QuakeRT*, void*, JSAtom);

  static int RCallStrLt(::QuakeRT*, void*, void*);
  static int RCallStrLe(::QuakeRT*, void*, void*);
  static int RCallStrGt(::QuakeRT*, void*, void*);
  static int RCallStrGe(::QuakeRT*, void*, void*);
  static int RCallStrNe(::QuakeRT*, void*, void*);
  static int RCallStrEq(::QuakeRT*, void*, void*);

  // --------------------------------------------------------------------------
  // comparison
  static JSValue RCallCmpLt(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallCmpLe(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallCmpGt(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallCmpGe(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallCmpEq(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallCmpNe(::QuakeRT*, JSValue*, JSValue*);

  // --------------------------------------------------------------------------
  // to_string
  static void* RCallInt32ToStr(::QuakeRT*, std::int32_t);
  static void* RCallFloat64ToStr(::QuakeRT*, double);
  static void* RCallObjectToStr(::QuakeRT*, void*);
  static double RCallStringToFloat64(::QuakeRT*, void*);

  // --------------------------------------------------------------------------
  // global
  static JSValue RCallGlobalCheck(::QuakeRT*, std::uint32_t);
  static JSValue RCallGlobalGet(::QuakeRT*, std::uint32_t, int);
  static JSValue RCallGlobalPut(::QuakeRT*, std::uint32_t, JSValue*, int);
  static JSValue RCallGlobalDefine(::QuakeRT*, std::uint32_t, int);
  static JSValue RCallGlobalDefineFunction(::QuakeRT*, std::uint32_t, void*,
                                           int);
  static JSValue RCallGlobalCheckDefine(::QuakeRT*, std::uint32_t, int);

  // --------------------------------------------------------------------------
  // atom
  static JSAtom RCallValueToAtom(::QuakeRT*, JSValue*);
  static JSValue RCallAtomToValue(::QuakeRT*, JSAtom);

  // --------------------------------------------------------------------------
  // array
  static JSValue RCallLoadArray(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallLoadArrayInt(::QuakeRT*, void*, int);

  // --------------------------------------------------------------------------
  // super value
  static JSValue RCallGetSuperValue(::QuakeRT*, JSValue*, JSAtom, JSValue*);
  static JSValue RCallPutSuperValue(::QuakeRT*, JSValue*, JSAtom, JSValue*,
                                    JSValue*);

  // --------------------------------------------------------------------------
  // keep-sync along with the lir/builtin.h
  // RCall, or Builtins

  static JSValue RCallObjectNew(::QuakeRT*);

  static JSValue RCallObjectFieldGetAtom(::QuakeRT*, void*, std::uint32_t);

  static JSValue RCallObjectFieldPutAtom(::QuakeRT*, void*, std::uint32_t,
                                         JSValue*);
  static JSValue RCallObjectFieldDefineAtom(::QuakeRT*, void*, std::uint32_t,
                                            JSValue*);
  static JSValue RCallObjectFieldSetName(::QuakeRT*, void*, std::uint32_t);
  static JSValue RCallObjectFieldSetNameComputed(::QuakeRT*, void*, JSValue*);

  static JSValue RCallSetPropertyValue(::QuakeRT*, JSValue*, JSValue*, JSValue*,
                                       int);

  static JSValue RCallDefinePropertyValueValue(::QuakeRT*, JSValue*, JSValue*,
                                               JSValue*, int);

  // --------------------------------------------------------------------------
  // misc
  static JSValue RCallAddBrand(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallCheckBrand(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallRegexp(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallGetSuper(::QuakeRT*, JSValue*);
  static JSValue RCallImport(::QuakeRT*, JSValue*);
  static JSValue RCallSetProto(::QuakeRT*, JSValue*, JSValue*);
  static void RCallSetHomeObject(::QuakeRT*, JSValue*, JSValue*);
  static JSValue RCallCopyDataProperties(::QuakeRT*, JSValue*, JSValue*,
                                         JSValue*);
  static JSValue RCallAppend(::QuakeRT*, JSValue*, JSValue*, JSValue*);

  // --------------------------------------------------------------------------
  // private field
  static JSValue RCallPrivateFieldPut(::QuakeRT*, JSValue*, JSValue*, JSValue*);
  static JSValue RCallPrivateFieldDefine(::QuakeRT*, JSValue*, JSValue*,
                                         JSValue*);

  // --------------------------------------------------------------------------
  // iterator
  static JSValue RCallIterClose(::QuakeRT*, JSValue*);

  // --------------------------------------------------------------------------
  // for of
  static JSValue RCallForOfStart(::QuakeRT*, int);
  static JSValue RCallForOfNext(::QuakeRT*, int, int);

  // --------------------------------------------------------------------------
  // for in
  static JSValue RCallForInStart(::QuakeRT*, int);
  static JSValue RCallForInNext(::QuakeRT*, int);

 public:
  // --------------------------------------------------------------------------
  // Calling into the script call
  struct alignas(cg::kStackAlignment) ScriptCall {
    JSValue* function_object;  // function object
    JSValue* this_object;      // this object
    JSValue* target_object;    // target object
    JSValue* return_address;   // return address
    JSValue* parameter_start;  // starting point of parameter position
    int parameter_size;        // parameter size
    int option;                // other option, ie apply's magic
  };

  static void JSCall(::QuakeRT*, ScriptCall*);
  static void JSCallCtor(::QuakeRT*, ScriptCall*);
  static void JSCallArrayFrom(::QuakeRT*, ScriptCall*);
  static void JSCallApply(::QuakeRT*, ScriptCall*);
};

struct Layout {
  static constexpr std::size_t kCellSize = sizeof(JSValue);
  static constexpr std::size_t kScriptCallSize = sizeof(QuakeJit::ScriptCall);
  static constexpr std::size_t kGCObjectHeader = sizeof(JSGCObjectHeader);

  struct Context {
    static constexpr std::size_t kQuake = offsetof(::QuakeRT, quake);

    static constexpr std::size_t kParam = offsetof(::QuakeRT, param);
    static constexpr std::size_t kLocal = offsetof(::QuakeRT, local);
    static constexpr std::size_t kTemp = offsetof(::QuakeRT, temp);
    static constexpr std::size_t kTempCur = offsetof(::QuakeRT, temp_cur);
    static constexpr std::size_t kVarRef = offsetof(::QuakeRT, var_ref);

    static constexpr std::size_t kContext = offsetof(::QuakeRT, context);
    static constexpr std::size_t kStackFrame =
        offsetof(::QuakeRT, current_stack_frame);

    static constexpr std::size_t kThisObject = offsetof(::QuakeRT, this_object);
    static constexpr std::size_t kTargetObject =
        offsetof(::QuakeRT, target_object);

    static constexpr std::size_t kNext = offsetof(::QuakeRT, next);
    static constexpr std::size_t kExitId = offsetof(::QuakeRT, exit_id);
    static constexpr std::size_t kNextPC = offsetof(::QuakeRT, next_bc);

    static constexpr std::size_t kTraceLevel = offsetof(::QuakeRT, trace_level);

    static constexpr std::size_t kLinkDeoptId1 =
        offsetof(::QuakeRT, link_deopt_id1);

    static constexpr std::size_t kLinkDeoptId2 =
        offsetof(::QuakeRT, link_deopt_id2);
  };

  struct JSContext {
    static constexpr std::size_t kGlobalVarObject =
        offsetof(::JSContext, global_var_obj);

    static constexpr std::size_t kGlobalObject =
        offsetof(::JSContext, global_obj);
  };

  struct Object {
    static constexpr std::size_t kShape = offsetof(::JSObject, shape);
    static constexpr std::size_t kProp = offsetof(::JSObject, prop);
    static constexpr std::size_t kClassId = offsetof(::JSObject, class_id);
    static constexpr std::size_t kU_Array_U = offsetof(::JSObject, u.array.u);
    static constexpr std::size_t kU_Array_Count =
        offsetof(::JSObject, u.array.count);
  };

  struct Shape {
    static constexpr std::size_t kPropHashMask =
        offsetof(::JSShape, prop_hash_mask);

    static constexpr std::size_t kPropCount = offsetof(::JSShape, prop_count);

    // prop is been defined as dynamic array, ie zero length array in C
    static constexpr std::size_t kProp = sizeof(JSShape);

    static const std::size_t kProto = offsetof(::JSShape, proto);
  };

  struct String {
    // The string's length field is a 31bits field right after the gc header
    static constexpr std::size_t kLength = sizeof(JSRefCountHeader);
  };

  struct ShapeProperty {
    static constexpr std::size_t kData = 0;  // first dword
    static constexpr std::size_t kAtom = offsetof(JSShapeProperty, atom);
  };

  struct Value {
    static constexpr std::size_t kValue = offsetof(JSValue, u);
    static constexpr std::size_t kTag = offsetof(JSValue, tag);
  };

  struct VarRef {
    static constexpr std::size_t kPValue = offsetof(JSVarRef, pvalue);
  };

  struct ScriptCall {
    static constexpr std::size_t kFunctionObject =
        offsetof(QuakeJit::ScriptCall, function_object);

    static constexpr std::size_t kThisObject =
        offsetof(QuakeJit::ScriptCall, this_object);

    static constexpr std::size_t kTargetObject =
        offsetof(QuakeJit::ScriptCall, target_object);

    static constexpr std::size_t kReturnAddress =
        offsetof(QuakeJit::ScriptCall, return_address);

    static constexpr std::size_t kParameterStart =
        offsetof(QuakeJit::ScriptCall, parameter_start);

    static constexpr std::size_t kParameterSize =
        offsetof(QuakeJit::ScriptCall, parameter_size);

    static constexpr std::size_t kOption =
        offsetof(QuakeJit::ScriptCall, option);
  };

  struct MiscData {
    static constexpr std::size_t kScriptCall = 0;
  };
};

}  // namespace quake

#endif  // _QUAKE_QUAKE_RT_H_
