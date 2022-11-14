#ifndef _QUAKE_LIR_IMMEDIATE_H_
#define _QUAKE_LIR_IMMEDIATE_H_
#include "base.h"
#include "common.h"

namespace quake {
namespace lir {

class Immediate {
 public:
  enum {
    kInt32,
    kInt64,  // the only int64 immediate that gonna be supported by instruction
             // is x64's movabs, and register must be rax
    kFloat64
  };

 public:
  static Immediate ZeroI32() { return Immediate{static_cast<std::int32_t>(0)}; }
  static Immediate ZeroI64() { return Immediate{static_cast<std::int64_t>(0)}; }
  static Immediate ZeroF64() { return Immediate{static_cast<double>(0.0)}; }

  explicit Immediate(std::int32_t v) : v_(v) {}
  explicit Immediate(std::int64_t v) : v_(v) {}
  explicit Immediate(double v) : v_(v) {}

  Immediate(const Immediate&) = default;
  Immediate(Immediate&&) = default;
  Immediate& operator=(const Immediate&) = default;
  Immediate& operator=(Immediate&&) = default;

 public:
  int type() const { return static_cast<int>(v_.index()); }

  inline SizeClass size_class() const;
  std::size_t byte_size() const { return SizeClassToByteSize(size_class()); }

  bool IsQWord() const { return type() == kInt64 || type() == kFloat64; }
  bool IsDWord() const { return type() == kInt32; }

  bool IsInt32() const { return type() == kInt32; }
  bool IsInt64() const { return type() == kInt64; }
  bool IsFloat64() const { return type() == kFloat64; }

  std::int32_t Int32() const { return base::Get<std::int32_t>(v_); }
  std::int64_t Int64() const { return base::Get<std::int64_t>(v_); }
  double Float64() const { return base::Get<double>(v_); }

  bool operator==(const Immediate& that) const { return v_ == that.v_; }
  bool operator!=(const Immediate& that) const { return v_ != that.v_; }

 private:
  std::variant<std::int32_t, std::int64_t, double> v_;
};

inline SizeClass Immediate::size_class() const {
  switch (v_.index()) {
    case kInt32:
      return SizeClass::kDWord;
    case kInt64:
      return SizeClass::kQWord;
    default:
      return SizeClass::kQWord;
  }
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_IMMEDIATE_H_
