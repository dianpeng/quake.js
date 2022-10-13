#ifndef _QJS_H_
#define _QJS_H_
#include "base.h"
#include "quick-js-inc.h"

// Quake's QJS wrapper. These wrapper is not for external use, ie it is not
// yet another quickjs c++ wrapper. Instead its only goal is to be used by
// quake to make us easier to work with QuickJS's native C apis. It is not
// complete and never intend to be complete

namespace quake {

// A RAII style JSValue object, wrapping the reference counting accordingly
class JVal {
 public:
  // Type flags
  enum {
    kBigDecimal = -11,
    kBigInt = -10,
    kBigFloat = -9,
    kSymbol = -8,
    kString = -7,
    kModule = -3,
    kFunctionBytecode = -2,
    kObject = -1,

    kInt = 0,
    kBool = 1,
    kNull = 2,
    kUndefined = 3,
    kUninitialized = 4,
    kCatchOffset = 5,
    kException = 6,
    kFloat64 = 7
  };

  // JIT related stuff, notes, never used cross JIT code and should never be
  // learnt by normal QuickJS stuff. These are not tag inside of box value
  // but used inside JIT code
  enum {
    kCompressedFrame = -12  // used in inline function, for var_ref stuff
  };

 public:
  static inline bool TypeIsHeap(int);
  static inline bool TypeIsPrimitive(int);
  static inline bool TypeIs64Bits(int);
  static inline bool TypeIs32Bits(int);
  static bool TypeIsFloat64(int x) { return x == kFloat64; }

 public:
#ifdef JS_NAN_BOXING
  static constexpr std::int64_t ToBit(JSValue v) {
    return *(std::int64_t*)(&v);
  }
  // Bit pattern helper to allow IR to generate instructions
  static const std::int64_t kBitTrue;
  static const std::int64_t kBitFalse;
  static const std::int64_t kBitNull;
  static const std::int64_t kBitUndefined;

  static std::int64_t BitI32(int value) {
    return ToBit(JS_MKVAL(JS_TAG_INT, value));
  }

  static std::int64_t BitF64(double value) {
    // Notes(dpeng): None of the quickjs's implementation of boxing requires
    // the usage of context here, so just save it from typing. In the future
    // it is highly unlikely it will change and for us, we mostly just support
    // quickjs's nan-boxing.
    return ToBit(__JS_NewFloat64(nullptr, value));
  }

  static double UnboxF64(std::int64_t value) {
    return JS_VALUE_GET_FLOAT64(static_cast<std::uint64_t>(value));
  }

  static inline std::int64_t BitPtr(int tag, std::int64_t ptr);
  static inline std::int64_t bitptr(int tag, void* ptr);
  static inline std::int64_t UnboxF64AsB64(std::int64_t);
#endif  // JS_NAN_BOXING

 public:
  // *MOVE* a value into JVal, assuming the reference counting is totally
  // valid. After *MOVE* the JSValue is invalid to use anymore and user
  // should not drop its RC via QuickJS api anymore.
  inline JVal(JSContext*, JSValue v, bool inc_rc = false);
  inline ~JVal();

  inline JVal(const JVal&);
  inline JVal(JVal&&);
  inline JVal& operator=(const JVal&);
  inline JVal& operator=(JVal&&);

  static JVal Undefined(JSContext* ctx) { return JVal(ctx, JS_UNDEFINED); }
  static JVal Null(JSContext* ctx) { return JVal(ctx, JS_NULL); }
  static JVal True(JSContext* ctx) { return JVal(ctx, JS_TRUE); }
  static JVal False(JSContext* ctx) { return JVal(ctx, JS_FALSE); }
  static JVal Int32(JSContext* ctx, std::int32_t value) {
    return JVal(ctx, JS_NewInt32(ctx, value));
  }
  static JVal Float64(JSContext* ctx, double value) {
    return JVal(ctx, JS_NewFloat64(ctx, value));
  }

  static JVal ToString(JVal value) {
    auto qctx = value.qjs_context();
    return JVal{qctx, JS_ToString(qctx, value.v())};
  }

 public:
  JSContext* qjs_context() const { return qjs_context_; }
  int type() const { return JS_VALUE_GET_NORM_TAG(v_); }
  const JSValue& v() const { return v_; }

  // Release the JSValue, this makes the return JSValue owned by the caller
  inline JSValue Release();

  bool IsBigDecimal() const { return type() == kBigDecimal; }
  bool IsBigInt() const { return type() == kBigInt; }
  bool IsBigFloat() const { return type() == kBigFloat; }
  bool IsSymbol() const { return type() == kSymbol; }
  bool IsString() const { return type() == kString; }
  bool IsModule() const { return type() == kModule; }
  bool IsFunctionBytecode() const { return type() == kFunctionBytecode; }
  bool IsObject() const { return type() == kObject; }
  bool IsHeap() const { return type() < 0; }

  bool IsInt() const { return type() == kInt; }
  bool IsBool() const { return type() == kBool; }
  bool IsNull() const { return type() == kNull; }
  bool IsUndefined() const { return type() == kUndefined; }
  bool IsUninitialized() const { return type() == kUninitialized; }
  bool IsCatchOffset() const { return type() == kCatchOffset; }
  bool IsException() const { return type() == kException; }
  bool IsFloat64() const { return type() >= kFloat64; }
  bool IsPrimitive() const { return type() >= 0; }

 public:
  inline bool Bool() const;
  inline int Int() const;
  inline double Float64() const;

  inline std::string String() const;
  inline std::string_view StringView() const;

  inline std::string Symbol() const;
  inline std::string_view SymbolView() const;

  // Convert the QJS value into a boolean that is suitable for condition
  // evaluation. The algorithm used here is exact replicated from JS_ToBool
  inline std::optional<bool> ToBool() const;

  // Notes, we return a opaque pointer of that wrapped object to be used by
  // other APIs
  inline JSObject* Object() const;

 private:
  inline std::string_view ToStringView() const;

  JSContext* qjs_context_;
  JSValue v_;
};

}  // namespace quake

#include "qjs-inl.h"

#endif  // _QJS_H_
