
/**
 * This is a simple automatic test driver. It allows better visibility and
 * testify. Basically we export some function to be used inside of quickjs
 * and then we write testing js file for testing various jit part
 */

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <quick-js-inc.h>
#include <base.h>
#include <flags.h>

namespace quake {
/**
 * testing frameworks, ie functions to be registered into the testing
 * environment
 */
static JSValue TestYes(JSContext* ctx, JSValueConst, int argc,
                       JSValueConst* argv) {
  if (argc != 1) {
    return JS_ThrowSyntaxError(ctx, "test.yes: expect 1 argument");
  }
  if (JS_ToBool(ctx, argv[0]) != 1) {
    return JS_ThrowInternalError(ctx, "test.yes: failed");
  }

  return JS_UNDEFINED;
}

static JSValue TestNo(JSContext* ctx, JSValueConst, int argc,
                      JSValueConst* argv) {
  if (argc != 1) {
    return JS_ThrowSyntaxError(ctx, "test.no: expect 1 argument");
  }
  if (JS_ToBool(ctx, argv[0]) == 1) {
    return JS_ThrowInternalError(ctx, "test.no: failed");
  }

  return JS_UNDEFINED;
}

static JSValue TestPrint(JSContext* ctx, JSValueConst, int argc,
                         JSValueConst* argv) {
  std::stringstream fmt;
  auto delimiter = JS_NewString(ctx, " ");
  DEFER([&]() { JS_FreeValue(ctx, delimiter); });

  // We use JSON.stringify to stringify the value and dump it out.
  for (int i = 0; i < argc; i++) {
    auto stringify = JS_JSONStringify(ctx, argv[i], JS_NULL, delimiter);

    if (JS_VALUE_GET_TAG(stringify) == JS_TAG_UNDEFINED) {
      fmt << "[undefined]";
    } else {
      auto cstr = JS_ToCString(ctx, stringify);
      fmt << cstr;
      JS_FreeCString(ctx, cstr);
    }
    JS_FreeValue(ctx, stringify);
  }

  std::cerr << fmt.str() << "\n";

  return JS_UNDEFINED;
}

static JSValue TestNow(JSContext* ctx, JSValueConst, int argc,
                       JSValueConst* argv) {
  auto now = base::NowInMilliseconds();
  return JS_NewFloat64(ctx, static_cast<double>(now));
}

static JSValue JitTraceOn(JSContext* ctx, JSValueConst, int argc,
                          JSValueConst* argv) {
  Flags::EnableTraceAll();

  return JS_UNDEFINED;
}

static JSValue JitTraceOff(JSContext* ctx, JSValueConst, int argc,
                           JSValueConst* argv) {
  Flags::DisableTraceAll();

  return JS_UNDEFINED;
}

// Used to run a single quake file and report result
struct RunResult {
  enum { kDone, kCompileError, kExecError };
  int state;
  std::string error_detail;

  explicit RunResult(int s) : state(s) {}
  RunResult(int s, std::string&& x) : state(s), error_detail(std::move(x)) {}

  RunResult(int s, std::uint64_t t) : state(s), exec_time(t) {}
  RunResult(int s, std::uint64_t t, std::string&& x)
      : state(s), error_detail(std::move(x)), exec_time(t) {}

 public:
  std::uint64_t exec_time;
};

class Runner {
 public:
  ~Runner();
  void SetUp();

  RunResult RunFile(const char*);

 private:
  JSContext* NewContext();
  JSRuntime* runtime_ = nullptr;
};

Runner::~Runner() {
  if (runtime_) JS_FreeRuntime(runtime_);
}

void Runner::SetUp() { runtime_ = JS_NewRuntime(); }

JSContext* Runner::NewContext() {
  auto context = JS_NewContext(runtime_);

  auto global = JS_GetGlobalObject(context);
  DEFER([&]() { JS_FreeValue(context, global); });

  auto register_func = [&](JSCFunction cfunc, std::string_view name) {
    auto cc = JS_NewCFunction(context, cfunc, name.data(), name.size());
    CHECK(JS_SetPropertyStr(context, global, std::to_string(name).c_str(), cc));
  };

  register_func(TestYes, "yes");
  register_func(TestNo, "no");
  register_func(TestPrint, "dump");
  register_func(TestNow, "now");
  register_func(JitTraceOn, "jit_trace_on");
  register_func(JitTraceOff, "jit_trace_off");
  return context;
}

RunResult Runner::RunFile(const char* path) {
  DEFER([]() { Flags::DisableTraceAll(); });

  auto maybe_data = base::LoadWholeFile(path);
  if (!maybe_data) {
    return RunResult{RunResult::kCompileError, "file cannot be loaded"};
  }

  auto context = NewContext();
  DEFER([&]() { JS_FreeContext(context); });

  auto ret = JS_Eval(context, maybe_data->data(), maybe_data->size(), "<TEST>",
                     JS_EVAL_FLAG_COMPILE_ONLY);
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
      return RunResult{RunResult::kCompileError, std::move(data)};
    }
    return RunResult{RunResult::kCompileError};
  }

  // run the code
  std::uint64_t exec_time = 0;
  JSValue r;
  {
    base::PerfTimerMilliseconds timer{&exec_time};
    r = JS_EvalFunction(context, JS_DupValue(context, ret));
  }

  if (JS_VALUE_GET_TAG(r) == JS_TAG_EXCEPTION) {
    auto excep = JS_GetException(context);
    auto str = JS_ToString(context, excep);
    auto cstr = JS_ToCString(context, str);
    std::string data{cstr};
    JS_FreeCString(context, cstr);
    JS_FreeValue(context, str);
    JS_FreeValue(context, excep);
    return RunResult{RunResult::kExecError, exec_time, std::move(data)};
  }

  return RunResult{RunResult::kDone, exec_time};
}

TEST_CASE("case test", "[jit-case]") {
  auto case_dir = x::TestPathAt("jit/case/");

  LOG(TRACE) << "***************************************************";
  LOG(TRACE) << "Case Dir: " << case_dir;
  LOG(TRACE) << "***************************************************";

  auto total = 0;
  auto fail = 0;

  Runner runner;
  runner.SetUp();

  for (auto& dir_entry :
       std::filesystem::recursive_directory_iterator(case_dir)) {
    if (!dir_entry.is_regular_file()) continue;
    const auto& p = dir_entry.path();

    LOG(TRACE) << "[==CASE==]: " << p.c_str() << "\n";

    auto result = runner.RunFile(p.c_str());
    ++total;

    switch (result.state) {
      case RunResult::kDone:
        LOG(TRACE) << "[PASS]: " << p.c_str() << "\n";
        break;

      case RunResult::kCompileError:
        ++fail;
        LOG(TRACE) << "[COMPILE ERROR]: " << p.c_str() << " "
                   << result.error_detail << "\n";
        break;

      case RunResult::kExecError:
        ++fail;
        LOG(TRACE) << "[EXEC ERROR]: " << p.c_str() << " "
                   << result.error_detail << "\n";
        break;

      default:
        break;
    }
  }

  LOG(TRACE) << "total(" << total << "); fail(" << fail << ")";
}

}  // namespace quake
