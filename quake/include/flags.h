#ifndef _QUAKE_FLAGS_H_
#define _QUAKE_FLAGS_H_
#include "base.h"
#include "quake-C.h"

// ----------------------------------------------------------------------------
// The inline call maximum deption
DECLARE_uint(inline_call_max_depth);

// Whether allow jit the function, including function's entry. May not be useful
// for some cases or good for isolation of errors
DECLARE_bool(allow_jit_function);

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

// ----------------------------------------------------------------------------
// Jit threshold
DECLARE_any(QuakeJitThreshold, jit_threshold);

// ----------------------------------------------------------------------------
// Jit penalty threshold
//
// How many NYI tracing result will make engine decide to mark BC to be not
// able to be Jitted
DECLARE_uint(jit_penalty_nyi);
DECLARE_uint(jit_penalty_bytecode_overflow);
DECLARE_uint(jit_penalty_bytecode_error);
DECLARE_uint(jit_penalty_reference_error);

// Control flow related penalty item
DECLARE_uint(jit_penalty_loop_exit);
DECLARE_uint(jit_penalty_inner_loop);

DECLARE_uint(jit_penalty_compile);

DECLARE_uint(jit_penalty_build);

namespace quake {

class Flags {
 public:
  static void EnableTraceAll();
  static void DisableTraceAll();
};

}  // namespace quake

#endif  // _QUAKE_FLAGS_H_
