#ifndef _QUAKE_FLAGS_H_
#define _QUAKE_FLAGS_H_
#include "base.h"

// ----------------------------------------------------------------------------
// The inline call maximum deption
DECLARE_uint(inline_call_max_depth);

// ----------------------------------------------------------------------------
// Tracing
DECLARE_bool(trace_compile_info);
DECLARE_bool(trace_compile_deoptimization);
DECLARE_bool(trace_compile_bytecode);
DECLARE_bool(trace_compile_ir);
DECLARE_bool(trace_compile_ir_optimization);
DECLARE_bool(trace_compile_lir_lower);
DECLARE_bool(trace_compile_lir_rewrite);
DECLARE_bool(trace_compile_lir_register_allocation);
DECLARE_bool(trace_compile_lir_post_ra_optimization);
DECLARE_bool(trace_compile_pre_code_generation);
DECLARE_bool(trace_compile_code_generation);

namespace quake {

class Flags {
 public:
  static void EnableTraceAll();
  static void DisableTraceAll();
};

}  // namespace quake

#endif  // _QUAKE_FLAGS_H_
