#ifndef _QUAKE_QUICK_JS_INC_H_
#define _QUAKE_QUICK_JS_INC_H_

extern "C" {
#include <quickjs.h>

// for VM related internal data structures
#include <quickjs-internal.h>

}

#ifndef QUAKE_ENABLE
#error "quake_enable is not defined"
#endif  // QUAKE_ENABLE

#endif  // _QUAKE_QUICK_JS_INC_H_
