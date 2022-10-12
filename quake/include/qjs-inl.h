#ifndef _QUAKE_QJS_INL_H_
#define _QUAKE_QJS_INL_H_

namespace quake {

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
    qjs_context_ = that.qjs_context_;
    v_ = that.v_;
    JS_DupValue(qjs_context_, v_);
  }
  return *this;
}

inline JVal& JVal::operator=(JVal&& that) {
  if (this != &that) {
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
