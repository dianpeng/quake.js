#ifndef _QUAKE_FRAME_H_
#define _QUAKE_FRAME_H_
#include "base.h"
#include "quick-js-inc.h"
#include "quake-C.h"

// ----------------------------------------------------------------------------
// This file is mostly just a documentation and it serves the foundation of our
// JIT code's stack layout and also deoptimization rematerialization.
//
// The JIT code will be compiled into a normal native function, and its stack
// will be layout in a relative different way, show as following
//
//
// ----------------   LOW
// |              |
// |  Spill Zone  |
// |              |
// ----------------
// |              |
// |  Cell Zone   |
// |              |
// ----------------
// |              |
// |  Frame Zone  |
// |              |
// |--------------|   HIGH
//
//
// 1) The spill zone is been responsible for holding all the spilled slot for
//    RA.
//
// 2) The Cell Zone is been responsible for holding the boxed value if needed,
//    we store the box value onto the stack if needed, in most case, we do not
//    need it though.
//
// 3) The Frame Zone is been responsible for holding all the inline frame, if
//    we do inline. It is used when inner inline frame start to deoptimize and
//    we can reconstruct interpreter frame based on the information stored there
// ----------------------------------------------------------------------------
//
//
// The frame object stored on the FrameZone is been directly modeled after the
// quick js object.

namespace quake {

enum class JitFrameType : std::uint8_t { kInline, kJitEntry };

struct JitFrame {
  JitFrameType type;
  JSValue func_object;    // GC pointer
  JSValue this_object;    // this pointer
  JSValue target_object;  // target pointer
};

}  // namespace quake

#endif  // _QUAKE_FRAME_H_
