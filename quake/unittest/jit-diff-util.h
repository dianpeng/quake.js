#ifndef _QUAKE_UNITTEST_JIT_DIFF_UTIL_H_
#define _QUAKE_UNITTEST_JIT_DIFF_UTIL_H_

#include <base.h>
#include <flags.h>
#include <quick-js-inc.h>

#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// Reusable two-run differential helper
//
//   1. oracle run — fresh runtime+context with JitDisableGuard active, ie
//      interpreter-only execution (hard kill switch at both trace-entry
//      points + threshold max; either alone is insufficient since hot
//      counters saturate past QUAKE_JIT_THRESHOLD_MAX).
//   2. jit run — fresh runtime+context, JIT enabled; the yes()/no()
//      assertion natives keep throwing on failure, so any kExecError in
//      this run is a gate failure.
//
// The compared result channel is the recorded observation sequence: every
// yes()/no()/dump() argument in call order plus the JS_EvalFunction
// completion value. Zero corpus edits required. Heap values are compared
// through their stringified form — pointer identity across two fresh
// contexts would false-fail.
// ----------------------------------------------------------------------------

namespace quake {
namespace test {

// One recorded JSValue (dup'd then snapshotted): JS tag plus payload.
struct Observation {
  int tag = 0;
  std::int32_t i32 = 0;
  double f64 = 0;
  void* ptr = nullptr;
  std::string str;
};

// Single-pair comparison. Never UNREACHABLEs on any tag (unlike JSVEq in
// test0.h): unknown/exotic tags compare tag-only.
inline bool ObservationEq(const Observation& lhs, const Observation& rhs) {
  if (lhs.tag != rhs.tag) return false;
  switch (lhs.tag) {
    case JS_TAG_INT:
    case JS_TAG_BOOL:
      return lhs.i32 == rhs.i32;

    case JS_TAG_FLOAT64:
      // Exact payload equality; all NaNs compare equal to each other.
      return lhs.f64 == rhs.f64 ||
             (std::isnan(lhs.f64) && std::isnan(rhs.f64));

    // Heap types: the stringified form is the cross-context identity.
    case JS_TAG_STRING:
    case JS_TAG_SYMBOL:
    case JS_TAG_OBJECT:
    case JS_TAG_FUNCTION_BYTECODE:
    case JS_TAG_MODULE:
      return lhs.str == rhs.str;

    default:
      // JS_TAG_NULL/UNDEFINED/UNINITIALIZED/... carry no payload.
      return true;
  }
}

inline bool ObservationsEq(const std::vector<Observation>& lhs,
                           const std::vector<Observation>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  for (std::size_t i = 0; i < lhs.size(); ++i) {
    if (!ObservationEq(lhs[i], rhs[i])) return false;
  }
  return true;
}

inline std::size_t FirstDivergence(const std::vector<Observation>& lhs,
                                   const std::vector<Observation>& rhs) {
  auto n = lhs.size() < rhs.size() ? lhs.size() : rhs.size();
  for (std::size_t i = 0; i < n; ++i) {
    if (!ObservationEq(lhs[i], rhs[i])) return i;
  }
  return n;  // length mismatch (or full equality up to the common prefix)
}

inline std::string DescribeObservation(const Observation& obs) {
  std::stringstream ss;
  ss << "tag(" << obs.tag << ")";
  switch (obs.tag) {
    case JS_TAG_INT:
    case JS_TAG_BOOL:
      ss << " i32=" << obs.i32;
      break;
    case JS_TAG_FLOAT64:
      ss << " f64=" << obs.f64;
      break;
    case JS_TAG_STRING:
    case JS_TAG_SYMBOL:
    case JS_TAG_OBJECT:
    case JS_TAG_FUNCTION_BYTECODE:
    case JS_TAG_MODULE:
      ss << " str=\"" << obs.str << "\"";
      break;
    default:
      break;
  }
  return ss.str();
}

// ----------------------------------------------------------------------------
// Active observation recorder. Tests are single-threaded; the assertion
// natives and the eval-completion capture write into whatever recorder is
// currently installed.
// ----------------------------------------------------------------------------

inline std::vector<Observation>*& ActiveRecorder() {
  static std::vector<Observation>* recorder = nullptr;
  return recorder;
}

// Deterministic stand-in for wall-clock time (see DiffNow below); reset at
// the start of every single run so the oracle and jit runs stay identical.
inline double& FakeClock() {
  static double clock = 0;
  return clock;
}

// Snapshot one JSValue into the active recorder (if any). The value is
// dup'd for the snapshot's lifetime so stringification cannot observe a
// freed value.
inline void RecordObservation(JSContext* ctx, JSValueConst value) {
  auto* recorder = ActiveRecorder();
  if (recorder == nullptr) return;

  auto dup = JS_DupValue(ctx, value);
  DEFER([&]() { JS_FreeValue(ctx, dup); });

  Observation obs;
  obs.tag = JS_VALUE_GET_TAG(dup);
  switch (obs.tag) {
    case JS_TAG_INT:
      obs.i32 = JS_VALUE_GET_INT(dup);
      break;

    case JS_TAG_BOOL:
      obs.i32 = JS_VALUE_GET_INT(dup);
      break;

    case JS_TAG_FLOAT64:
      obs.f64 = JS_VALUE_GET_FLOAT64(dup);
      break;

    case JS_TAG_STRING:
    case JS_TAG_SYMBOL:
    case JS_TAG_OBJECT:
    case JS_TAG_FUNCTION_BYTECODE:
    case JS_TAG_MODULE:
      obs.ptr = JS_VALUE_GET_PTR(dup);
      if (auto* cstr = JS_ToCString(ctx, dup); cstr != nullptr) {
        obs.str = cstr;
        JS_FreeCString(ctx, cstr);
      } else {
        // Notes, JS_ToCString raised an exception; clear it so we do not
        // leak a pending exception into the native call site.
        JSValue excep = JS_GetException(ctx);
        JS_FreeValue(ctx, excep);
        obs.str = "<unstringifiable>";
      }
      break;

    default:
      break;
  }

  recorder->push_back(std::move(obs));
}

// ----------------------------------------------------------------------------
// RAII oracle enforcement. Both kQuakeJitThreshold and the kill switch are
// process-global — a missing restore would poison the subsequent JIT run.
// ----------------------------------------------------------------------------

class JitDisableGuard {
 public:
  JitDisableGuard()
      : saved_kill_(g_quake_jit_kill_switch),
        saved_threshold_(kQuakeJitThreshold) {
    g_quake_jit_kill_switch = 1;
    kQuakeJitThreshold = QuakeJitThreshold{QUAKE_JIT_THRESHOLD_MAX,
                                           QUAKE_JIT_THRESHOLD_MAX};
  }

  ~JitDisableGuard() {
    kQuakeJitThreshold = saved_threshold_;
    g_quake_jit_kill_switch = saved_kill_;
  }

  JitDisableGuard(const JitDisableGuard&) = delete;
  JitDisableGuard& operator=(const JitDisableGuard&) = delete;

 private:
  int saved_kill_;
  QuakeJitThreshold saved_threshold_;
};

// ----------------------------------------------------------------------------
// Single-run substrate shared by the oracle and the jit run
// ----------------------------------------------------------------------------

namespace _ {

struct SingleRunResult {
  bool ok = false;
  std::string error;
  std::vector<Observation> obs;
};

// Notes, arguments are recorded BEFORE being processed so a miscompiled
// value flowing into a failing assertion is still observable. yes()/no()
// keep throwing on failure — in the jit run that surfaces as kExecError,
// which the differential treats as a gate failure.

static JSValue DiffYes(JSContext* ctx, JSValueConst, int argc,
                       JSValueConst* argv) {
  if (argc != 1) {
    return JS_ThrowSyntaxError(ctx, "test.yes: expect 1 argument");
  }
  RecordObservation(ctx, argv[0]);
  if (JS_ToBool(ctx, argv[0]) != 1) {
    return JS_ThrowInternalError(ctx, "test.yes: failed");
  }
  return JS_UNDEFINED;
}

static JSValue DiffNo(JSContext* ctx, JSValueConst, int argc,
                      JSValueConst* argv) {
  if (argc != 1) {
    return JS_ThrowSyntaxError(ctx, "test.no: expect 1 argument");
  }
  RecordObservation(ctx, argv[0]);
  if (JS_ToBool(ctx, argv[0]) == 1) {
    return JS_ThrowInternalError(ctx, "test.no: failed");
  }
  return JS_UNDEFINED;
}

static JSValue DiffDump(JSContext* ctx, JSValueConst, int argc,
                        JSValueConst* argv) {
  std::stringstream fmt;
  for (int i = 0; i < argc; i++) {
    RecordObservation(ctx, argv[i]);
    auto stringify = JS_ToCString(ctx, argv[i]);
    DEFER([&]() { JS_FreeCString(ctx, stringify); });
    fmt << stringify << "; ";
  }

  std::cerr << fmt.str() << "\n";
  return JS_UNDEFINED;
}

static JSValue DiffNow(JSContext* ctx, JSValueConst, int argc,
                       JSValueConst* argv) {
  // Notes, the differential compares two runs of the same program, so
  // wall-clock values from now() are pure nondeterminism (10 corpus files
  // dump elapsed-time deltas and would false-fail). A run-scoped fake
  // clock keeps now() deterministic: identical programs observe identical
  // value sequences, and a differing now() call count still surfaces as
  // an observation mismatch.
  FakeClock() += 1000.0;
  return JS_NewFloat64(ctx, FakeClock());
}

static JSValue DiffTraceOn(JSContext* ctx, JSValueConst, int argc,
                           JSValueConst* argv) {
  Flags::EnableTraceAll();
  return JS_UNDEFINED;
}

static JSValue DiffTraceOff(JSContext* ctx, JSValueConst, int argc,
                            JSValueConst* argv) {
  Flags::DisableTraceAll();
  return JS_UNDEFINED;
}

inline void RegisterNatives(JSContext* context) {
  auto global = JS_GetGlobalObject(context);
  DEFER([&]() { JS_FreeValue(context, global); });

  auto register_func = [&](JSCFunction cfunc, std::string_view name) {
    auto cc = JS_NewCFunction(context, cfunc, name.data(), name.size());
    CHECK(JS_SetPropertyStr(context, global, std::to_string(name).c_str(), cc));
  };

  register_func(DiffYes, "yes");
  register_func(DiffNo, "no");
  register_func(DiffDump, "dump");
  register_func(DiffNow, "now");
  register_func(DiffTraceOn, "jit_trace_on");
  register_func(DiffTraceOff, "jit_trace_off");
}

// Runs one file in a fresh runtime+context, recording observations into
// result.obs. The completion value of the program is recorded as the final
// observation.
inline SingleRunResult RunFileOnce(const char* path) {
  SingleRunResult result;
  DEFER([]() { Flags::DisableTraceAll(); });

  // Deterministic clock: both runs of the differential must start at the
  // same fake time (see DiffNow).
  FakeClock() = 0;

  auto* saved_recorder = ActiveRecorder();
  ActiveRecorder() = &result.obs;
  DEFER([&]() { ActiveRecorder() = saved_recorder; });

  auto maybe_data = base::LoadWholeFile(path);
  if (!maybe_data) {
    result.error = "file cannot be loaded";
    return result;
  }

  auto runtime = JS_NewRuntime();
  auto context = JS_NewContext(runtime);
  DEFER([&]() { JS_FreeRuntime(runtime); });
  DEFER([&]() { JS_FreeContext(context); });

  RegisterNatives(context);

  auto ret = JS_Eval(context, maybe_data->data(), maybe_data->size(),
                     "<TEST>", JS_EVAL_FLAG_COMPILE_ONLY);
  DEFER([&]() { JS_FreeValue(context, ret); });

  if (JS_VALUE_GET_TAG(ret) != JS_TAG_FUNCTION_BYTECODE) {
    if (JS_VALUE_GET_TAG(ret) == JS_TAG_EXCEPTION) {
      auto excep = JS_GetException(context);
      auto str = JS_ToString(context, excep);
      auto cstr = JS_ToCString(context, str);

      std::string data{cstr};

      JS_FreeCString(context, cstr);
      JS_FreeValue(context, str);
      JS_FreeValue(context, excep);
      result.error = "compile error: " + data;
    } else {
      result.error = "compile error: not a function bytecode";
    }
    return result;
  }

  auto r = JS_EvalFunction(context, JS_DupValue(context, ret));

  // The completion value is the final observation of the sequence.
  RecordObservation(context, r);

  if (JS_VALUE_GET_TAG(r) == JS_TAG_EXCEPTION) {
    auto excep = JS_GetException(context);
    auto str = JS_ToString(context, excep);
    auto cstr = JS_ToCString(context, str);
    std::string data{cstr};
    JS_FreeCString(context, cstr);
    JS_FreeValue(context, str);
    JS_FreeValue(context, excep);
    JS_FreeValue(context, r);
    result.error = "exec error: " + data;
    return result;
  }

  JS_FreeValue(context, r);
  result.ok = true;
  return result;
}

}  // namespace _

// ----------------------------------------------------------------------------
// Per-file differential entry point — used by the corpus driver and the
// permanent canary meta-test alike.
// ----------------------------------------------------------------------------

struct DiffVerdict {
  bool pass = false;
  std::string detail;
};

inline DiffVerdict RunFileDifferential(const char* path) {
  DiffVerdict verdict;
  std::string file{path};

  // ---- oracle run: interpreter only ---------------------------------------
  std::vector<Observation> oracle;
  {
    JitDisableGuard guard{};
    auto run = _::RunFileOnce(path);
    if (!run.ok) {
      verdict.detail = file + ": oracle-failed: " + run.error;
      return verdict;
    }
    oracle = std::move(run.obs);
  }

  // ---- jit run: fresh runtime+context, jit enabled -------------------------
  auto jit = _::RunFileOnce(path);
  if (!jit.ok && ObservationsEq(oracle, jit.obs)) {
    // The jit run failed without any recorded value divergence — a
    // throwing assertion on an unrecorded value or a runtime error.
    verdict.detail = file + ": jit-error: " + jit.error;
    return verdict;
  }

  // ---- compare the observation sequences -----------------------------------
  if (!ObservationsEq(oracle, jit.obs)) {
    auto idx = FirstDivergence(oracle, jit.obs);
    verdict.detail = file + ": mismatch at observation[" +
                     std::to_string(idx) + "]: oracle(" +
                     DescribeObservation(
                         idx < oracle.size() ? oracle[idx] : Observation{}) +
                     ") vs jit(" +
                     DescribeObservation(
                         idx < jit.obs.size() ? jit.obs[idx] : Observation{}) +
                     ")";
    if (!jit.ok) {
      verdict.detail += " (jit run also errored: " + jit.error + ")";
    }
    return verdict;
  }

  verdict.pass = true;
  verdict.detail =
      file + ": ok (" + std::to_string(oracle.size()) + " observations)";
  return verdict;
}

}  // namespace test
}  // namespace quake

#endif  // _QUAKE_UNITTEST_JIT_DIFF_UTIL_H_
