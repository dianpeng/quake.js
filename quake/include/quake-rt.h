#ifndef _QUAKE_QUAKE_RT_H_
#define _QUAKE_QUAKE_RT_H_
#include "base.h"
#include "quick-js-inc.h"
#include "builtin-function.h"
#include "cg/cg.h"

namespace quake {
class Quake;

struct RecoveryFrame;

struct QuakeRT {
  Quake* quake;
  int next;               // next option, ie DeoptNext enum
  RecoveryFrame* rframe;  // inner most

  // --------------------------------------------------------------------------
  // entry frame's param/local/temp, notes owned by the VM interpreter
  JSValue* param = nullptr;
  JSValue* param_end = nullptr;

  JSValue* local = nullptr;
  JSValue* local_end = nullptr;

  JSValue* temp = nullptr;
  JSValue* temp_end = nullptr;

  // --------------------------------------------------------------------------
  // Context infromation
  JSContext* context = nullptr;
};

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
  static double RCallFMod(QuakeRT*, double, double);
  static double RCallFPow(QuakeRT*, double, double);
  static void RCallGCFree(QuakeRT*, JSValue*);
};

// ----------------------------------------------------------------------------
// Knonw layout constant, used by codegen in machine code
struct Layout {
  struct Context {
    static constexpr std::size_t kParam = offsetof(QuakeRT, param);
    static constexpr std::size_t kParamEnd = offsetof(QuakeRT, param_end);

    static constexpr std::size_t kLocal = offsetof(QuakeRT, local);
    static constexpr std::size_t kLocalEnd = offsetof(QuakeRT, local_end);

    static constexpr std::size_t kTemp = offsetof(QuakeRT, temp);
    static constexpr std::size_t kTempEnd = offsetof(QuakeRT, temp_end);
  };

  struct Value {
    static constexpr std::size_t kValue = offsetof(JSValue, u);
    static constexpr std::size_t kTag = offsetof(JSValue, tag);
  };
};

}  // namespace quake

#endif  // _QUAKE_QUAKE_RT_H_
