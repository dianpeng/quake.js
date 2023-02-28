#include "qjs.h"

namespace quake {

#ifdef JS_NAN_BOXING

const std::int64_t JVal::kBitTrue = ToBit(JS_TRUE);
const std::int64_t JVal::kBitFalse = ToBit(JS_FALSE);
const std::int64_t JVal::kBitNull = ToBit(JS_NULL);
const std::int64_t JVal::kBitUndefined = ToBit(JS_UNDEFINED);

#endif  // JS_NAN_BOXING

std::string JVal::ToString(JSContext* jsc, std::int32_t value) {
  auto js_val = JS_ToString(jsc, JS_NewInt32(jsc, value));
  DEFER([&]() { JS_FreeValue(jsc, js_val); });

  std::size_t len;
  auto cstr = JS_ToCStringLen(jsc, &len, js_val);
  DEFER([&]() { JS_FreeCString(jsc, cstr); });

  return std::string{cstr, len};
}

std::string JVal::ToString(JSContext* jsc, double value) {
  auto js_val = JS_ToString(jsc, JS_NewFloat64(jsc, value));
  DEFER([&]() { JS_FreeValue(jsc, js_val); });

  std::size_t len;
  auto cstr = JS_ToCStringLen(jsc, &len, js_val);
  DEFER([&]() { JS_FreeCString(jsc, cstr); });

  return std::string{cstr, len};
}

std::string JVal::ToString(JSContext* jsc, bool value) {
  return value ? "true" : "false";
}

std::string JVal::Print(JSContext* jsc, JSValue value) {
  auto str = JS_JSONStringify(jsc, value, JS_NULL, JS_NewString(jsc, "  "));
  DEFER([&]() { JS_FreeValue(jsc, str); });

  if (JS_VALUE_GET_TAG(str) == JS_TAG_UNDEFINED) {
    return "[undefined]";
  } else {
    std::size_t len;
    auto cstr = JS_ToCStringLen(jsc, &len, str);
    DEFER([&]() { JS_FreeCString(jsc, cstr); });
    return std::string{cstr, len};
  }
}

std::string JVal::Debug() const {
  if (IsObject()) {
    auto x = static_cast<const JSObject*>(Ptr());
    if (x->class_id == JS_CLASS_BYTECODE_FUNCTION) {
      auto b = x->u.func.function_bytecode;
      return std::string{b->debug.source,
                         static_cast<std::size_t>(b->debug.source_len)};
    }
  }
  return "[N/A]";
}

bool JVal::IsScriptFunction() const {
  switch (type()) {
    case JVal::kObject:
      return Object()->class_id == JS_CLASS_BYTECODE_FUNCTION;
    default:
      return false;
  }
}

std::optional<JVal> JVal::GetBytecodeObject() const {
  switch (type()) {
    case JVal::kFunctionBytecode:
      return *this;
    case JVal::kObject: {
      auto obj = Object();
      auto jsv =
          JS_MKPTR(JS_TAG_FUNCTION_BYTECODE, obj->u.func.function_bytecode);

      return JVal{qjs_context(), jsv, true};
    }

    default:
      return {};
  }
}

std::optional<JSBytecodeInternalInfo> JVal::GetBytecodeInfo() const {
  if (auto maybe = GetBytecodeObject(); maybe) {
    JSBytecodeInternalInfo info;
    JS_GetFunctionBytecodeInternalInfo(maybe->v(), &info);
    return info;
  } else {
    return {};
  }
}

}  // namespace quake
