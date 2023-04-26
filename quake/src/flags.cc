#include "flags.h"

DEFINE_uint(inline_call_max_depth, "call inline maximum call depth allowed", 0);

DEFINE_bool(allow_jit_function, "whether allow jit full function", true);

// ----------------------------------------------------------------------------
// Tracing
DEFINE_bool(trace_compile_info, "trace compilation information", false);
DEFINE_bool(trace_compile_deoptimization, "trace deoptimization", false);
DEFINE_bool(trace_compile_bytecode, "trace bytecode emittion", false);
DEFINE_bool(trace_compile_ir, "trace IR construction", false);
DEFINE_bool(trace_compile_ir_optimization, "trace IR optimization", false);
DEFINE_bool(trace_compile_lir_lower, "trace LIR lower", false);
DEFINE_bool(trace_compile_lir_rewrite, "trace LIR rewrite", false);
DEFINE_bool(trace_compile_lir_register_allocation, "trace LIR RA", false);
DEFINE_bool(trace_compile_lir_post_ra_optimization,
            "trace LIR post RA optimization", false);
DEFINE_bool(trace_compile_pre_code_generation,
            "trace pre code generation optimization", false);
DEFINE_bool(trace_compile_code_generation, "trace generation optimization",
            false);

namespace quake {

void Flags::EnableTraceAll() {
  FLAG_trace_compile_info.Set(true);
  FLAG_trace_compile_deoptimization.Set(true);
  FLAG_trace_compile_bytecode.Set(true);
  FLAG_trace_compile_ir.Set(true);
  FLAG_trace_compile_ir_optimization.Set(true);
  FLAG_trace_compile_lir_lower.Set(true);
  FLAG_trace_compile_lir_rewrite.Set(true);
  FLAG_trace_compile_lir_register_allocation.Set(true);
  FLAG_trace_compile_lir_post_ra_optimization.Set(true);
  FLAG_trace_compile_pre_code_generation.Set(true);
  FLAG_trace_compile_code_generation.Set(true);
}

void Flags::DisableTraceAll() {
  FLAG_trace_compile_info.Set(false);
  FLAG_trace_compile_deoptimization.Set(false);
  FLAG_trace_compile_bytecode.Set(false);
  FLAG_trace_compile_ir.Set(false);
  FLAG_trace_compile_ir_optimization.Set(false);
  FLAG_trace_compile_lir_lower.Set(false);
  FLAG_trace_compile_lir_rewrite.Set(false);
  FLAG_trace_compile_lir_register_allocation.Set(false);
  FLAG_trace_compile_lir_post_ra_optimization.Set(false);
  FLAG_trace_compile_pre_code_generation.Set(false);
  FLAG_trace_compile_code_generation.Set(false);
}

}  // namespace quake
