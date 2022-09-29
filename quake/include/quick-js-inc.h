#ifndef _QUAKE_QUICK_JS_INC_H_
#define _QUAKE_QUICK_JS_INC_H_

#define JS_NAN_BOXING

#ifndef JS_NAN_BOXING
#error "QUAKE(jit) can only work with NAN boxing"
#endif  // JS_NAN_BOXING

extern "C" {
#include <quickjs.h>
}

#endif  // _QUAKE_QUICK_JS_INC_H_
