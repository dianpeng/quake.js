#ifndef _UNITTEST_EXEC_TEST0_H_
#define _UNITTEST_EXEC_TEST0_H_
#include "base.h"
#include "qjs.h"
#include "quake-C.h"

#include "ir/interp-info.h"
#include "ir/bytecode.h"

#include <quake.h>
#include <quake-rt.h>
#include <jit.h>
#include <ir/builder.h>
#include <ir/graph.h>

#include <lir/assembler.h>
#include <lir/lir-lower.h>
#include <lir/register-allocator.h>
#include <lir/pre-code-gen.h>
#include <lir/rewriter.h>
#include <lir/post-ra-optimization.h>

#include <lir/cfg.h>

#include <cg/x64/code-gen.h>

#include <cutils.h>

namespace quake {

// A simple mock object to enable us to testify IR construction
class QJSFunctionBytecode {
 public:
  // Mapping of JSValue to local variables and its corresponding expected
  // value

  struct ValueItem {
    JSValue init;
    std::string init_from_global;

    std::optional<JSValue> expect;
    ValueItem(JSValue&& i) : init(std::move(i)) {}

    ValueItem(const std::string_view& what)
        : init_from_global(std::to_string(what)) {}

    ValueItem(JSValue&& i, JSValue&& e)
        : init(std::move(i)), expect(std::move(e)) {}

    ValueItem(const std::string_view& i, JSValue&& e)
        : init_from_global(std::to_string(i)), expect(std::move(e)) {}

    bool IsInit() const { return init_from_global.empty(); }

    JSValue GetInit(JSContext* jsc) const {
      if (IsInit()) {
        return init;
      } else {
        auto global = JS_GetGlobalObject(jsc);
        auto v = JS_GetPropertyStr(jsc, global, init_from_global.c_str());
        return v;
      }
    }
  };

  using ValueMap = std::vector<ValueItem>;

  QJSFunctionBytecode(ValueMap&& local, ValueMap&& param)
      : js_runtime_(nullptr),
        js_context_(nullptr),
        compiled_object_(JS_UNDEFINED),
        local_(std::move(local)),
        param_(std::move(param)) {}

  ~QJSFunctionBytecode();

  // Called to initialize QUICKJS related context
  void SetUp();

  // Compile a function source code and populate internal structure
  void Compile(const std::string_view&);

  // Run the compiled bytecode via quickjs entry
  JVal Run();

  void PrepareLoop();

  // Iterate through the loop inside of the function body. This function is
  // typically used to allow us to build the IR graph for testing purpose
  void ForeachBytecodeInNaturalLoop(std::function<void(ir::InterpInfo*)>&&,
                                    std::function<void(ir::InterpInfo*)>&&);

  bool IsModeStrict() const {
    static const std::uint8_t kModeStrict = 1 << 0;
    return bc_info_.js_mode & kModeStrict;
  }

  JSContext* js_context() const { return js_context_; }

  const ValueMap& local() const { return local_; }
  const ValueMap& param() const { return param_; }

 public:
  // --------------------------------------------------------------------------
  // Mocked InterpInfo object
  class InterpInfoMock : public ir::InterpInfo {
   public:
    InterpInfoMock(QJSFunctionBytecode* b, const void* bc, std::size_t ts,
                   const ValueMap& local, const ValueMap& param)
        : b_(b),
          bc_(static_cast<const std::uint8_t*>(bc)),
          cur_(static_cast<const std::uint8_t*>(bc) + 1),
          temp_size_(ts),
          local_(local),
          param_(param) {}

    JSRuntime* qjs_runtime() const override { return b_->js_runtime_; }
    JSContext* qjs_context() const override { return b_->js_context_; }
    QJS* qjs() const override {
      UNREACHABLE();
      return nullptr;
    }

    std::int32_t GetI32() override;
    std::uint32_t GetU32() override;
    std::uint8_t GetU8() override;
    std::uint16_t GetU16() override;

    bool IsModeStrict() const { return b_->IsModeStrict(); }

   public:
    JVal GetParam(std::uint32_t) const override;
    JVal GetLocal(std::uint32_t) const override;
    JVal GetTemp(std::uint32_t) const override;
    JVal GetVarRef(std::uint32_t) const override {
      UNREACHABLE();
      return JVal{qjs_context(), JS_UNDEFINED};
    }

    std::uint32_t local_size() const override { return b_->bc_info_.loc_count; }
    std::uint32_t param_size() const override { return b_->bc_info_.arg_count; }
    std::uint32_t temp_size() const override { return temp_size_; }
    std::uint32_t var_ref_size() const override { return 0; }

   public:
    ir::Bytecode op() const override { return static_cast<ir::Bytecode>(*bc_); }
    const void* start_pc() const override { return bc_; }
    const void* interp_pc() const override { return bc_ + 1; }
    const void* next_pc() const override {
      return bc_ + GetQJSBytecodeInfo(op()).size;
    }
    const void* PCNext(std::ptrdiff_t offset) override {
      return bc_ + 1 + offset;
    }
    ir::BytecodeInfo bytecode_info() const override {
      return ir::BytecodeInfo{op(), start_pc()};
    }

    JVal GetConst(std::uint32_t) const override;
    JVal GetAtom(std::uint32_t, bool) const override;

   public:
    // These function cannot be implemented and should not be used for now
    JVal This() const override {
      UNREACHABLE();
      return JVal::Undefined(qjs_context());
    }
    JVal Function() const override {
      UNREACHABLE();
      return JVal::Undefined(qjs_context());
    }
    JVal Target() const override {
      UNREACHABLE();
      return JVal::Undefined(qjs_context());
    }
    JVal Global() const override {
      UNREACHABLE();
      return JVal::Undefined(qjs_context());
    }

   private:
    QJSFunctionBytecode* b_;
    const std::uint8_t* bc_;   // this points to the start of the PC
    const std::uint8_t* cur_;  // current position
    std::size_t temp_size_;  // temporary size (calculated by qjs bytecode info)
    const ValueMap& local_;
    const ValueMap& param_;
  };

  void PrintBytecode();

 private:
  // Finding out the natural loop inside of the bytecode. The algorithm is
  // pretty simple, ie finding out the OP_jump which does a backwards jump
  // that's it. We do not verify whether it is a reducible graph or not. Notes
  // we always find out the inner most loop, since we perform a check in forward
  // manner, ie always encounter the inner most loop's OP_goto first
  void FindNaturalLoop();

 private:
  JSRuntime* js_runtime_;           // quickjs's runtime
  JSContext* js_context_;           // quickjs's context
  JSValue compiled_object_;         // the compiled bytecode
  JSBytecodeInternalInfo bc_info_;  // bytecode info

  const void* loop_start_ = nullptr;
  const void* loop_end_ = nullptr;

  ValueMap local_;
  ValueMap param_;

  friend class InterpInfoMocked;
};

// ============================================================================
// InterpInfoMock implementation
// ============================================================================
JVal QJSFunctionBytecode::InterpInfoMock::GetConst(std::uint32_t idx) const {
  return JVal{qjs_context(),
              JS_DupValue(qjs_context(), b_->bc_info_.constant_pool[idx])};
}

JVal QJSFunctionBytecode::InterpInfoMock::GetAtom(std::uint32_t idx,
                                                  bool force_string) const {
  return JVal{qjs_context(),
              __JS_AtomToValue(qjs_context(), idx, force_string ? 1 : 0)};
}

std::uint32_t QJSFunctionBytecode::InterpInfoMock::GetU32() {
  auto v = get_u32(cur_);
  cur_ += 4;
  return v;
}

std::int32_t QJSFunctionBytecode::InterpInfoMock::GetI32() {
  auto v = (int32_t)get_u32(cur_);
  cur_ += 4;
  return v;
}

std::uint8_t QJSFunctionBytecode::InterpInfoMock::GetU8() {
  auto v = get_u8(cur_);
  cur_ += 1;
  return v;
}

std::uint16_t QJSFunctionBytecode::InterpInfoMock::GetU16() {
  auto v = get_u16(cur_);
  cur_ += 2;
  return v;
}

JVal QJSFunctionBytecode::InterpInfoMock::GetParam(std::uint32_t idx) const {
  CHECK(idx < param_.size());
  return JVal{qjs_context(),
              JS_DupValue(qjs_context(), param_[idx].GetInit(qjs_context()))};
}

JVal QJSFunctionBytecode::InterpInfoMock::GetLocal(std::uint32_t idx) const {
  CHECK(idx < local_.size());
  return JVal{qjs_context(),
              JS_DupValue(qjs_context(), local_[idx].GetInit(qjs_context()))};
}

JVal QJSFunctionBytecode::InterpInfoMock::GetTemp(std::uint32_t idx) const {
  // TODO(dpeng): Fixme
  return JVal::Float64(qjs_context(), 1.0);
}

void QJSFunctionBytecode::FindNaturalLoop() {
  auto bc_start = static_cast<const std::uint8_t*>(bc_info_.bc_start);
  auto bc_end = bc_start + bc_info_.bc_size;

  while (bc_start != bc_end) {
    auto opcode = static_cast<ir::Bytecode>(*bc_start);
    const auto& info = GetQJSBytecodeInfo(opcode);

    if (opcode == ir::Bytecode::OP_goto) {
      auto arg_pos = bc_start + 1;

      // check this operation
      auto jump_offset = static_cast<std::int32_t>(::get_u32(arg_pos));

      if (jump_offset < 0) {
        // should be a loop
        loop_start_ = arg_pos + jump_offset;
        loop_end_ = bc_start;
        break;
      }
    }

    bc_start += info.size;  // move to next bytecode
  }

  CHECK(loop_start_) << "cannot find natural loop";
}

void QJSFunctionBytecode::PrintBytecode() {
  auto bc_start = static_cast<const std::uint8_t*>(bc_info_.bc_start);
  auto bc_end = bc_start + bc_info_.bc_size;

  while (bc_start != bc_end) {
    auto opcode = static_cast<ir::Bytecode>(*bc_start);
    const auto& info = GetQJSBytecodeInfo(opcode);
    LOG(ERROR) << "OPCODE: " << info.name << "@"
               << reinterpret_cast<const void*>(bc_start);
    bc_start += info.size;  // move to next bytecode
  }
}

void QJSFunctionBytecode::SetUp() {
  js_runtime_ = JS_NewRuntime();
  js_context_ = JS_NewContext(js_runtime_);
}

QJSFunctionBytecode::~QJSFunctionBytecode() {
  if (js_context_) {
    JS_FreeValue(js_context_, compiled_object_);
  }
  if (js_context_) JS_FreeContext(js_context_);
  if (js_runtime_) JS_FreeRuntime(js_runtime_);
}

void QJSFunctionBytecode::Compile(const std::string_view& source) {
  auto ret = ::JS_Eval(js_context_, source.data(), source.size(),
                       "<QJSFunctionBytecode>", JS_EVAL_FLAG_COMPILE_ONLY);
  CHECK(JS_VALUE_GET_TAG(ret) == JS_TAG_FUNCTION_BYTECODE);
  CHECK(JS_GetFunctionBytecodeInternalInfo(ret, &bc_info_) == 0);
  compiled_object_ = ret;
}

void QJSFunctionBytecode::PrepareLoop() {
  FindNaturalLoop();
  PrintBytecode();
}

JVal QJSFunctionBytecode::Run() {
  CHECK(JS_VALUE_GET_TAG(compiled_object_) == JS_TAG_FUNCTION_BYTECODE);

  std::uint64_t time;
  JSValue r = JS_UNDEFINED;
  DEFER([&]() { JS_FreeValue(nullptr, r); });

  {
    base::PerfTimerMilliseconds timer{&time};
    r = JS_EvalFunction(js_context_,
                        JS_DupValue(js_context_, compiled_object_));
  }
  LOG(ERROR) << "EXEC TIME: " << time;
  if (JS_VALUE_GET_TAG(r) == JS_TAG_EXCEPTION) {
    auto excep = JS_GetException(js_context_);
    auto js_str = JS_ToString(js_context_, excep);
    JS_FreeValue(js_context_, excep);

    auto data = JS_ToCString(js_context_, js_str);
    LOG(ERROR) << "Exception: " << data;

    JS_FreeValue(js_context_, js_str);
    JS_FreeCString(js_context_, data);

    return JVal{js_context_, r};
  }
  auto global = JS_GetGlobalObject(js_context_);
  auto x = JVal{js_context_,
                JS_GetPropertyStr(js_context_, global, "global_return")};
  JS_FreeValue(js_context_, global);
  return x;
}

void QJSFunctionBytecode::ForeachBytecodeInNaturalLoop(
    std::function<void(ir::InterpInfo*)>&& visitor,
    std::function<void(ir::InterpInfo*)>&& last) {
  auto start = static_cast<const std::uint8_t*>(loop_start_);
  auto end = static_cast<const std::uint8_t*>(loop_end_);
  std::size_t temp_size = 0;

  while (start != end) {
    auto opcode = static_cast<ir::Bytecode>(*start);
    const auto& info = GetQJSBytecodeInfo(opcode);
    temp_size += info.n_push;
    temp_size -= info.n_pop;

    InterpInfoMock ii{this, start, temp_size, local_, param_};
    visitor(&ii);
    start += info.size;
    if (start == end) {
      last(&ii);
    }
  }
}

bool JSVEq(const JSValue& lhs, const JSValue& rhs) {
  if (JS_VALUE_GET_TAG(lhs) != JS_VALUE_GET_TAG(rhs)) return false;
  auto t = JS_VALUE_GET_TAG(lhs);
  switch (t) {
    default:
      UNREACHABLE();
      return false;

    case JS_TAG_INT:
    case JS_TAG_BOOL:
      return JS_VALUE_GET_INT(lhs) == JS_VALUE_GET_INT(rhs);

    case JS_TAG_NULL:
    case JS_TAG_UNDEFINED:
    case JS_TAG_UNINITIALIZED:
      return true;

    case JS_TAG_FLOAT64:
      return JS_VALUE_GET_FLOAT64(lhs) == JS_VALUE_GET_FLOAT64(rhs);
  }
}

// ----------------------------------------------------------------------------
// Special testify function, used to verify values

bool Test0(const char* source_code, QJSFunctionBytecode::ValueMap&& local,
           QJSFunctionBytecode::ValueMap&& param, bool trace = false) {
  QJSFunctionBytecode qjs_func{std::move(local), std::move(param)};
  qjs_func.SetUp();
  qjs_func.Compile(source_code);
  qjs_func.PrepareLoop();
  auto qjs = static_cast<QJS*>(JS_QuakeGetQJS());

  Quake quake{qjs, qjs_func.js_context()};
  ir::Graph g{&quake};
  ir::Builder builder{&quake, &g};
  lir::HAssembler as{&quake};

  auto has_start = false;
  qjs_func.ForeachBytecodeInNaturalLoop(
      [&](ir::InterpInfo* info) {
        if (!has_start) {
          auto r = builder.Setup(info);
          if (!r) {
            LOG(FATAL) << r.message();
          }
          has_start = true;
        }
        if (auto r = builder.Trace(info); !r) {
          LOG(FATAL) << r.message();
        }
      },
      [&](ir::InterpInfo* info) {
        auto r = builder.Stop(info);
        if (!r) {
          LOG(FATAL) << r.message();
        }
      });
  if (trace) {
    LOG(TRACE) << "--------------------------------------------------------\n";
    LOG(TRACE) << "HIR-----------------------------------------------------\n";
    LOG(TRACE) << '\n' << g.Print();
  }

  {
    lir::LIRLower trans{&quake, &as};
    trans.Run(&g);
  }

  if (trace) {
    LOG(TRACE) << "--------------------------------------------------------\n";
    LOG(TRACE) << "LIR-----------------------------------------------------\n";
    LOG(TRACE) << '\n' << as.Print();
  }

  { lir::Rewriter::Rewrite(&quake, &as); }

  if (trace) {
    LOG(TRACE) << "--------------------------------------------------------\n";
    LOG(TRACE) << "LIR (after rewrite) ------------------------------------\n";
    LOG(TRACE) << '\n' << as.Print();
  }

  {
    lir::RegisterAllocator ra{&quake, &as};
    ra.Run();
  }

  if (trace) {
    LOG(TRACE) << "--------------------------------------------------------\n";
    LOG(TRACE) << "LIR (after RA) -----------------------------------------\n";
    LOG(TRACE) << '\n' << as.Print();
  }

  // Construct a CFG to show the LIR into CFG graph
  if (trace) {
    lir::CFG cfg{&quake};
    cfg.CreateCFG(&as);
    cfg.CreateDom();

    LOG(TRACE) << cfg.PrintGraph();
    LOG(TRACE) << cfg.PrintDom();
  }

  // post ra optimization
  {
    lir::PostRAOptimization opt{&quake};
    opt.Run(&as);
  }
  if (trace) {
    LOG(TRACE) << "--------------------------------------------------------\n";
    LOG(TRACE) << "LIR (after post ra optimization) -----------------------\n";
    LOG(TRACE) << '\n' << as.Print();
  }

  {
    lir::PreCodeGen pcg{&quake, &as};
    pcg.Run();
  }

  if (trace) {
    LOG(TRACE) << "--------------------------------------------------------\n";
    LOG(TRACE) << "LIR (after PCG) ----------------------------------------\n";
    LOG(TRACE) << '\n' << as.Print();
  }

  {
    if (trace) {
      LOG(TRACE)
          << "--------------------------------------------------------\n";
      LOG(TRACE)
          << "Code Generation ----------------------------------------\n";
    }

    cg::x64::CodeGen cg{&quake, &as};
    if (auto err = cg.Generate(); err) {
      LOG(FATAL) << err.GetDisplay();
    }

    auto code = cg.DetachCode();
    if (trace) {
      LOG(ERROR) << code->Disassemble();
    }

    QuakeRT rt;
    rt.context = qjs_func.js_context();
    rt.quake = reinterpret_cast<QQuake*>(&quake);

    std::vector<JSValue> value_list;
    DEFER([&]() {
      for (auto& e : value_list) {
        JS_FreeValue(qjs_func.js_context(), e);
      }
    });

    value_list.reserve(qjs_func.param().size() + qjs_func.local().size());

    for (const auto& e : qjs_func.param()) {
      value_list.push_back(
          JS_DupValue(qjs_func.js_context(), e.GetInit(qjs_func.js_context())));
    }

    rt.param = value_list.data();
    rt.param_end = value_list.data() + value_list.size();

    for (const auto& e : qjs_func.local()) {
      value_list.push_back(
          JS_DupValue(qjs_func.js_context(), e.GetInit(qjs_func.js_context())));
    }

    rt.local = rt.param_end;
    rt.local_end = value_list.data() + value_list.size();

    if (trace) {
      LOG(TRACE)
          << "--------------------------------------------------------\n";
      LOG(TRACE)
          << "Code Execution -----------------------------------------\n";
    }

    std::uint64_t time = 0;
    {
      base::PerfTimerMilliseconds timer{&time};
      auto entry = code->entry();
      entry(&rt);
    }

    if (trace) {
      LOG(TRACE) << "Execution time: " << time;
    }

    // Lastly, verify the result inside of the value_list
    {
      std::size_t index = 0;
      for (const auto& e : qjs_func.param()) {
        if (e.expect) {
          return JSVEq(*e.expect, rt.param[index]);
        }
        index++;
      }
    }

    {
      std::size_t index = 0;
      for (const auto& e : qjs_func.local()) {
        if (e.expect) {
          return JSVEq(*e.expect, rt.local[index]);
        }
        index++;
      }
    }
  }

  return true;
}

}  // namespace quake

#endif
