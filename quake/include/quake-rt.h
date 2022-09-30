#ifndef _QUAKE_QUAKE_RT_H_
#define _QUAKE_QUAKE_RT_H_
#include "base.h"

#include "quick-js-inc.h"

namespace quake {

struct QuakeRT {
  Quake* quake;

  // --------------------------------------------------------------------------
  // JIT frame needed information

  JSValue* param;
  JSValue* param_end;

  JSValue* local;
  JSValue* local_end;

  JSValue* temp;
  JSValue* temp_end;
};

// --------------------------------------------------------------------------
// Runtime Functions
//
// QuakeJIT exposes all the needed function for JITted code to coordinate with
struct QuakeJIT {

  // --------------------------------------------------------------------------
  // Helping to prepare the deoptimization information for interpreter to return
  // the prepared the data is been hide inside of the QuakeRT object. And will
  // be used by interpreter to reconstruct its frame for execution recovery
  // inside of quickjs.c
  static void DeoptPrepare(void* start, void* end);
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
};

}  // namespace quake

#endif  // _QUAKE_QUAKE_RT_H_
