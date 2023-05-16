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

// Notes for C header file, we need to define variable linkage of
// kQuakeJitThreshold and make it reflect the actual value usage

QuakeJitThreshold kQuakeJitThreshold = {QUAKE_JIT_THRESHOLD_BACKEDGE_DEFAULT,
                                        QUAKE_JIT_THRESHOLD_CALL_DEFAULT};

DEFINE_any(
    QuakeJitThreshold, jit_threshold,
    "jit threshold definition, expect a dictionary contains 2 fields, "
    ".backedge represents the backedge counter threshold and .call represents "
    "the call frequency counter threshold",
    [](const base::Value& input,
       const char*) -> base::Result<QuakeJitThreshold> {
      QuakeJitThreshold value{QUAKE_JIT_THRESHOLD_BACKEDGE_DEFAULT,
                              QUAKE_JIT_THRESHOLD_CALL_DEFAULT};

      auto is_valid_threshold = [](const base::Value& v) -> bool {
        if (v.IsInt()) {
          auto vv = v.ToInt();
          return vv > 0 && vv < QUAKE_JIT_THRESHOLD_MAX;
        }
        return false;
      };

      if (input.IsDictionary()) {
        auto dict = input.ToDictionaryPtr();
        if (auto t = dict->Get("backedge"); t && is_valid_threshold(*t)) {
          value.backedge_threshold = static_cast<std::uint16_t>(t->ToInt());
        }
        if (auto t = dict->Get("call"); t && is_valid_threshold(*t)) {
          value.call_threshold = static_cast<std::uint16_t>(t->ToInt());
        }
      }

      // Assign it back to the kQuakeJitThreshold since the C header file needs
      // this value
      kQuakeJitThreshold = value;
      return value;
    },
    [](const QuakeJitThreshold& x) -> base::Value {
      auto d = std::make_shared<base::Dictionary>();
      d->Add("backedge", base::Value{x.backedge_threshold});
      d->Add("call", base::Value{x.call_threshold});
      return base::Value{std::move(d)};
    },
    QuakeJitThreshold{QUAKE_JIT_THRESHOLD_BACKEDGE_DEFAULT,
                      QUAKE_JIT_THRESHOLD_CALL_DEFAULT});

DEFINE_uint(jit_penalty_nyi,
            "jit penalty threshold, for reason NYI(not yet implemented)", 3);

DEFINE_uint(jit_penalty_bytecode_overflow,
            "jit penalty threshold, for reason bytecode overflow", 3);

DEFINE_uint(jit_penalty_bytecode_error,
            "jit penalty threshold, for reason bytecode error", 1);

DEFINE_uint(jit_penalty_reference_error,
            "jit penalty threshold, for reason reference error", 1);

DEFINE_uint(jit_penalty_loop_exit,
            "jit penalty threshold, for reason loop exit unstable", 2);

DEFINE_uint(jit_penalty_inner_loop,
            "jit penalty threshold, for reason inner loop is unstable", 2);

DEFINE_uint(jit_penalty_build,
            "jit penalty threshold, any unexpected thing happened in the build",
            10);

DEFINE_uint(jit_penalty_compile,
            "jit penalty threshold, for compilation failure, this is mostly "
            "BUG caused by JIT",
            1);

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
