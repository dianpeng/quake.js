#ifndef _QUAKE_QJS_INL_H_
#define _QUAKE_QJS_INL_H_

namespace quake {

inline const char* JVal::GetTypeName(int t) {
  switch (t) {
    case kBigDecimal:
      return "big-decimal";
    case kBigInt:
      return "big-int";
    case kBigFloat:
      return "big-float";
    case kSymbol:
      return "symbol";
    case kString:
      return "string";
    case kModule:
      return "module";
    case kFunctionBytecode:
      return "function-bytecode";
    case kObject:
      return "object";
    case kInt:
      return "int";
    case kBool:
      return "bool";
    case kNull:
      return "null";
    case kUndefined:
      return "undefined";
    case kUninitialized:
      return "uninitialized";
    case kCatchOffset:
      return "catch-offset";
    case kException:
      return "exception";
    case kFloat64:
      return "float64";
    default:
      UNREACHABLE();
      return "";
  }
}

inline std::string JVal::PrintRaw(std::uint64_t raw_value, int type) {
  if (TypeIsHeap(type)) {
    return base::Fmt("%p", raw_value);
  } else if (TypeIsFloat64(type)) {
    double x;
    std::memcpy(&x, &raw_value, sizeof(x));
    return base::Fmt("%f", x);
  } else if (TypeIs32Bits(raw_value)) {
    return base::Fmt("%d", static_cast<std::int32_t>(raw_value));
  } else {
    return base::Fmt("%" PRIu64, raw_value);
  }
}

inline SizeClass JVal::GetSizeClass(int t) {
  switch (t) {
    default:
      if (t < 0) return SizeClass::kQWord;
      UNREACHABLE();

    case JVal::kInt:
    case JVal::kBool:
    case JVal::kNull:
    case JVal::kUndefined:
    case JVal::kUninitialized:
    case JVal::kCatchOffset:
    case JVal::kException:
      return SizeClass::kDWord;

    case JVal::kFloat64:
      return SizeClass::kQWord;
  }
}

inline JVal::JVal(JSContext* qjs_ctx, JSValue v, bool inc_rc)
    : qjs_context_(qjs_ctx), v_(v) {
  if (inc_rc) {
    JS_DupValue(qjs_context_, v_);
  }
}

inline JVal::~JVal() { JS_FreeValue(qjs_context_, v_); }

inline JVal::JVal(const JVal& that)
    : qjs_context_(that.qjs_context_), v_(that.v_) {
  JS_DupValue(qjs_context_, v_);
}

inline JVal::JVal(JVal&& that) : qjs_context_(that.qjs_context_), v_(that.v_) {
  that.v_ = JS_UNDEFINED;
}

inline JVal& JVal::operator=(const JVal& that) {
  if (this != &that) {
    Release();
    qjs_context_ = that.qjs_context_;
    v_ = that.v_;
    JS_DupValue(qjs_context_, v_);
  }
  return *this;
}

inline JVal& JVal::operator=(JVal&& that) {
  if (this != &that) {
    Release();
    qjs_context_ = that.qjs_context_;
    v_ = that.v_;
    that.v_ = JS_UNDEFINED;
  }
  return *this;
}

inline JSValue JVal::Release() {
  auto tmp = v_;
  v_ = JS_UNDEFINED;
  return tmp;
}

inline bool JVal::IsFastArray() const {
  if (!IsObject()) return false;
  auto ptr = static_cast<JSObject*>(JS_VALUE_GET_PTR(v_));
  return ptr->fast_array == 1;
}

inline int JVal::Int() const {
  CHECK(IsInt());
  return JS_VALUE_GET_INT(v_);
}

inline bool JVal::Bool() const {
  CHECK(IsBool());
  return JS_VALUE_GET_BOOL(v_) == 1;
}

inline double JVal::Float64() const {
  CHECK(IsFloat64());
  return JS_VALUE_GET_FLOAT64(v_);
}

inline int JVal::ToInt() const {
  if (IsInt()) return Int();
  if (IsFloat64()) return static_cast<int>(Float64());
  UNREACHABLE();
  return 0;
}

inline double JVal::ToFloat64() const {
  if (IsInt()) return static_cast<double>(Int());
  if (IsFloat64()) return Float64();
  UNREACHABLE();
  return 0.0;
}

inline std::string JVal::String() const {
  return std::to_string(ToStringView());
}

inline std::string_view JVal::StringView() const { return ToStringView(); }

inline std::string JVal::Symbol() const {
  return std::to_string(ToStringView());
}

inline std::string_view JVal::SymbolView() const { return ToStringView(); }

inline std::string_view JVal::ToStringView() const {
  CHECK(IsString());
  std::size_t len;
  auto data = JS_ToCStringLen(qjs_context_, &len, v_);
  return std::string_view{data, len};
}

inline JSObject* JVal::Object() const { return JS_VALUE_GET_OBJ(v_); }

inline bool JVal::TypeIsHeap(int x) { return x < 0; }

inline bool JVal::TypeIsPrimitive(int x) { return x >= 0; }

inline bool JVal::TypeIs64Bits(int x) {
  return TypeIsHeap(x) || TypeIsFloat64(x);
}

inline bool JVal::TypeIs32Bits(int x) { return !TypeIs64Bits(x); }

inline std::optional<bool> JVal::ToBool() const {
  switch (type()) {
    case kInt:
      return Int() != 0;
    case kBool:
      return Bool();
    case kNull:
    case kUndefined:
      return 0;
    case kString:
      return StringView().size() != 0;

    case kObject:
      return {};  // cannot handle for now

    case kBigDecimal:
    case kBigInt:
    case kBigFloat:
      return {};  // cannot handle for now

    case kException:
      return true;  // FIXME(dpeng): invalid ? are we supposed to use an
                    // exception

    case kFloat64:
      return Float64() != 0.0;

    case kCatchOffset:
    case kSymbol:
    case kModule:
    case kFunctionBytecode:
      return true;
    default:
      UNREACHABLE();
      return false;
  }
}

// this logic is kept in sync with quick.c's function JS_ToInt32Free

inline std::int32_t JVal::Float64ToBit32(double b) {
  union {
    double d;
    std::uint64_t u64;
    std::uint32_t u32[2];
  } u;

  u.d = b;
  const auto e = u.u64 >> 52 & 0x7ff;

  if (e <= 1023 + 30) {
    return static_cast<std::int32_t>(b);
  } else if (e <= 1023 + 30 + 53) {
    std::uint64_t v;
    std::uint32_t ret;

    // remainder modulo 2^32
    v = (u.u64 & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);
    v = v << ((e - 1023) - 52 + 32);
    ret = static_cast<std::uint32_t>(v >> 32);
    // take the sign into account
    if (u.u64 >> 63) ret = -ret;

    return ret;
  } else {
    return 0;
  }
}

#ifdef JS_NAN_BOXING

inline std::int64_t JVal::BitPtr(int tag, std::int64_t ptr) {
  void* xx = nullptr;
  std::memcpy(&xx, &ptr, sizeof(void*));  // type pune that is allowed
  return BitPtr(tag, xx);
}

inline std::int64_t JVal::UnboxF64AsB64(std::int64_t value) {
  auto v = UnboxF64(value);
  std::int64_t xx = 0;
  std::memcpy(&xx, &v, sizeof(double));
  return xx;
}

inline std::int64_t JVal::bitptr(int tag, void* ptr) {
  auto v = js_mkptr(tag, ptr);
  std::int64_t out;
  std::memcpy(&out, &v, sizeof(v));
  return out;
}

#endif  // JS_NAN_BOXING

}  // namespace quake

#endif  // _QUAKE_QJS_INL_H_
