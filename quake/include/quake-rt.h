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
  static int DeoptPrepare(void* start, void* end);

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
  static const char* GetUnreachableCodeName(int);
  static void Unreachable(int);

 public:
  // debugging helpers
  static void PrintI32(std::int32_t value);
  static void PrintI64(std::int64_t value);
  static void PrintF64(double value);

 public:
  static const void* MapRCall(BuiltinFunction);

  // --------------------------------------------------------------------------
  // keep-sync along with the lir/builtin.h
  // RCall, or Builtins
  static double RCallFMod(::QuakeRT*, double, double);
  static double RCallFPow(::QuakeRT*, double, double);
  static void RCallGCFree(::QuakeRT*, JSValue*);

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
  };

  static void JSCall(::QuakeRT*, ScriptCall*);
  static void JSCallCtor(::QuakeRT*, ScriptCall*);
};

// ----------------------------------------------------------------------------
// Knonw layout constant, used by codegen in machine code
struct Layout {
  static constexpr std::size_t kCellSize = sizeof(JSValue);
  static constexpr std::size_t kScriptCallSize = sizeof(QuakeJit::ScriptCall);

  struct Context {
    static constexpr std::size_t kParam = offsetof(::QuakeRT, param);
    static constexpr std::size_t kParamEnd = offsetof(::QuakeRT, param_end);

    static constexpr std::size_t kLocal = offsetof(::QuakeRT, local);
    static constexpr std::size_t kLocalEnd = offsetof(::QuakeRT, local_end);

    static constexpr std::size_t kTemp = offsetof(::QuakeRT, temp);
    static constexpr std::size_t kTempEnd = offsetof(::QuakeRT, temp_end);
  };


  struct Value {
    static constexpr std::size_t kValue = offsetof(JSValue, u);
    static constexpr std::size_t kTag = offsetof(JSValue, tag);
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
  };

  struct MiscData {
    static constexpr std::size_t kScriptCall = 0;
  };
};

}  // namespace quake

#endif  // _QUAKE_QUAKE_RT_H_
