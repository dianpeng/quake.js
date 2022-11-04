#ifndef _UNITTEST_IR_IR_TEST_UTIL_H_
#define _UNITTEST_IR_IR_TEST_UTIL_H_
#include "base.h"
#include "qjs.h"
#include "quick-js-inc.h"
#include "ir/interp-info.h"
#include "ir/bytecode.h"

#include <cutils.h>

namespace quake {
namespace ir {

// A simple mock object to enable us to testify IR construction
class QJSFunctionBytecode {
 public:
  QJSFunctionBytecode()
      : js_runtime_(nullptr),
        js_context_(nullptr),
        compiled_object_(JS_UNDEFINED) {}

  ~QJSFunctionBytecode();

  // Called to initialize QUICKJS related context
  void SetUp();

  // Compile a function source code and populate internal structure
  void Compile(const std::string_view&);

  // Iterate through the loop inside of the function body. This function is
  // typically used to allow us to build the IR graph for testing purpose
  void ForeachBytecodeInNaturalLoop(std::function<void(InterpInfo*)>&&,
                                    std::function<void(InterpInfo*)>&&);

  bool IsModeStrict() const {
    static const std::uint8_t kModeStrict = 1 << 0;
    return bc_info_.js_mode & kModeStrict;
  }

  JSContext* js_context() const { return js_context_; }

 public:
  // Mocked InterpInfo object
  class InterpInfoMock : public InterpInfo {
   public:
    InterpInfoMock(QJSFunctionBytecode* b, const void* bc, std::size_t ts)
        : b_(b),
          bc_(static_cast<const std::uint8_t*>(bc)),
          cur_(static_cast<const std::uint8_t*>(bc) + 1),
          temp_size_(ts) {}

    JSRuntime* qjs_runtime() const override { return b_->js_runtime_; }
    JSContext* qjs_context() const override { return b_->js_context_; }
    QJS* qjs() const override {
      UNREACHABLE();
      return nullptr;
    }

    std::uint32_t GetU32() override;
    std::uint8_t GetU8() override;
    std::uint16_t GetU16() override;

    bool IsModeStrict() const { return b_->IsModeStrict(); }

   public:
    JVal GetParam(std::uint32_t) const override;
    JVal GetLocal(std::uint32_t) const override;
    JVal GetTemp(std::uint32_t) const override;
    JVal GetSlot(std::uint32_t) const override;

    std::uint32_t local_size() const override { return b_->bc_info_.loc_count; }
    std::uint32_t param_size() const override { return b_->bc_info_.arg_count; }
    std::uint32_t temp_size() const override { return temp_size_; }
    std::uint32_t slot_size() const override {
      return local_size() + param_size() + temp_size();
    }

   public:
    Bytecode op() const override { return static_cast<Bytecode>(*bc_); }
    const void* start_pc() const override { return bc_; }
    const void* interp_pc() const override { return bc_ + 1; }
    const void* next_pc() const override {
      return bc_ + GetQJSBytecodeInfo(op()).size;
    }
    const void* PCNext(std::ptrdiff_t offset) override {
      return bc_ + 1 + offset;
    }
    BytecodeInfo bytecode_info() const override {
      return BytecodeInfo{op(), start_pc()};
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
  };

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

JVal QJSFunctionBytecode::InterpInfoMock::GetParam(std::uint32_t) const {
  return JVal::Float64(qjs_context(), 1.0);
}

JVal QJSFunctionBytecode::InterpInfoMock::GetLocal(std::uint32_t) const {
  return JVal::Float64(qjs_context(), 1.0);
}

JVal QJSFunctionBytecode::InterpInfoMock::GetTemp(std::uint32_t) const {
  return JVal::Float64(qjs_context(), 1.0);
}

JVal QJSFunctionBytecode::InterpInfoMock::GetSlot(std::uint32_t) const {
  return JVal::Float64(qjs_context(), 1.0);
}

void QJSFunctionBytecode::FindNaturalLoop() {
  auto bc_start = static_cast<const std::uint8_t*>(bc_info_.bc_start);
  auto bc_end = bc_start + bc_info_.bc_size;

  while (bc_start != bc_end) {
    auto opcode = static_cast<Bytecode>(*bc_start);
    const auto& info = GetQJSBytecodeInfo(opcode);

    if (opcode == Bytecode::OP_goto) {
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
  FindNaturalLoop();
}

void QJSFunctionBytecode::ForeachBytecodeInNaturalLoop(
    std::function<void(InterpInfo*)>&& visitor,
    std::function<void(InterpInfo*)>&& last) {
  auto start = static_cast<const std::uint8_t*>(loop_start_);
  auto end = static_cast<const std::uint8_t*>(loop_end_);
  std::size_t temp_size = 0;

  while (start != end) {
    auto opcode = static_cast<Bytecode>(*start);
    const auto& info = GetQJSBytecodeInfo(opcode);
    temp_size += info.n_push;
    temp_size -= info.n_pop;

    InterpInfoMock ii{this, start, temp_size};
    visitor(&ii);
    start += info.size;
    if (start == end) {
      last(&ii);
    }
  }
}

}  // namespace ir
}  // namespace quake

#endif
