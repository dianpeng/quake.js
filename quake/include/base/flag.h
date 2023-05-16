#ifndef _BASE_FLAG_H_
#define _BASE_FLAG_H_
#include "check.h"
#include "common.h"
#include "result.h"
#include "value.h"

namespace base {
namespace flag {
namespace _ {

class FlagInfo {
 public:
  inline FlagInfo(const char* name, const char* type, const char* cmt,
                  const char* file, int line);

  inline virtual ~FlagInfo();

  // The constructor interface used by the command line parser. The cli parser
  // will parse the option into a |Value| and passed it into the the FlagInfo
  // for futhur construction. This call is guaranteed whenever the option is
  // specified
  virtual Error From(const Value&) = 0;

  // Return the value currently been set in the flag as Value object
  virtual Value To() const = 0;

  const char* name() const { return name_; }
  const char* type() const { return type_; }
  const char* comment() const { return comment_; }
  const char* file() const { return file_; }
  int line() const { return line_; }

  // Used to get the default value representation. Mainly for debugging purpose
  virtual std::string GetDefaultRepresentation() const = 0;

 public:
  // Setter/Getter for command value, ie after cli parsing, one can expect to
  // set this value for debug purpose later on.
  void set_cmd_value(const std::string& v) { cmd_value_ = v; }
  const std::string& cmd_value() const { return cmd_value_; }

 private:
  const char* name_;
  const char* type_;
  const char* comment_;
  const char* file_;
  int line_;
  std::string cmd_value_;

  DISALLOW_COPY_AND_ASSIGN(FlagInfo);
};

// A structure represent a flag. Notes, this object is typically defined in
// global scope.
template <typename S>
class PrimitiveFlag : public FlagInfo {
 public:
  using ValueType = S;
  using VerifyFunction = std::function<bool(const S&, const char*)>;

  // Built-in type constructor, ie string/boolean/integer/real/list<...>
  PrimitiveFlag(const char* name, const char* type, const char* cmt,
                const char* file, int line, S&& default_value,
                VerifyFunction&& verify_function = {})
      : FlagInfo(name, type, cmt, file, line),
        value_(),
        default_value_(std::move(default_value)),
        verifier_(std::move(verify_function)) {
    value_ = default_value_;
  }

 public:
  const S& Get() const { return value_; }
  S& Get() { return value_; }
  const S& default_value() const { return default_value_; }

 public:
  void Set(const S& v) { value_ = v; }
  void Set(S&& v) { value_ = std::move(v); }
  void SetDefault() { value_ = default_value_; }

  std::string GetDefaultRepresentation() const override {
    return std::to_string(default_value_);
  }

 public:
  Error From(const Value&) override;
  Value To() const override { return Value{value_}; }

 protected:
  VerifyFunction& verifier() { return verifier_; }

 private:
  S value_;
  S default_value_;
  VerifyFunction verifier_;

  DISALLOW_COPY_AND_ASSIGN(PrimitiveFlag);
};

template <typename T>
class ListFlag : public FlagInfo {
 public:
  using ListType = std::vector<T>;
  using ValueType = T;
  using VerifyFunction = std::function<bool(const ListType&, const char*)>;

  // Notes(dpeng): We put the comment field at back to allow us to ignore
  //   default_value when define ListFlag in macro. ie the .../__VA_ARGS__
  //   trick
  ListFlag(const char* name, const char* type, const char* file, int line,
           const char* cmt, std::vector<T>&& default_value = {},
           VerifyFunction&& v = {})
      : FlagInfo(name, type, cmt, file, line),
        value_(),
        default_value_(std::move(default_value)),
        verifier_(std::move(v)) {
    value_ = default_value_;
  }

 public:
  const ListType& Get() const { return value_; }
  ListType& Get() { return value_; }
  const ListType& default_value() const { return default_value_; }

 public:
  void Set(const ListType& v) { value_ = v; }
  void Set(ListType&& v) { value_ = std::move(v); }
  void SetDefault() { value_ = default_value_; }

 public:
  Error From(const Value&) override;
  Value To() const override;

 protected:
  VerifyFunction& verifier() { return verifier_; }

 private:
  std::string GetDefaultRepresentation() const override final {
    return default_value_.empty()
               ? "N/A"
               : Join(default_value_.begin(), default_value_.end());
  }

  ListType value_;
  ListType default_value_;
  VerifyFunction verifier_;

  DISALLOW_COPY_AND_ASSIGN(ListFlag);
};

using BoolFlag = PrimitiveFlag<bool>;
using IntFlag = PrimitiveFlag<std::int32_t>;
using UIntFlag = PrimitiveFlag<std::uint32_t>;
using RealFlag = PrimitiveFlag<double>;
using StringFlag = PrimitiveFlag<std::string>;

using BooleanListFlag = ListFlag<bool>;
using IntListFlag = ListFlag<std::int32_t>;
using UIntListFlag = ListFlag<std::uint32_t>;
using RealListFlag = ListFlag<double>;
using StringListFlag = ListFlag<std::string>;

template <std::size_t kGranularity>
class DurationMsFlag : public UIntFlag {
 public:
  using UIntFlag::ValueType;
  using UIntFlag::VerifyFunction;

  DurationMsFlag(const char* name, const char* type, const char* cmt,
                 const char* file, int line, ValueType&& dv,
                 VerifyFunction&& v = {})
      :

        UIntFlag(name, type, cmt, file, line, std::move(dv), std::move(v)) {}

  Error From(const Value&) override;
};

template <std::size_t kGranularity>
class DurationMinFlag : public UIntFlag {
 public:
  using UIntFlag::ValueType;
  using UIntFlag::VerifyFunction;

  DurationMinFlag(const char* name, const char* type, const char* cmt,
                  const char* file, int line, ValueType&& dv,
                  VerifyFunction&& v = {})

      : UIntFlag(name, type, cmt, file, line, std::move(dv), std::move(v)) {}

  Error From(const Value&) override;
};

template <std::size_t kGranularity>
class SizeFlag : public UIntFlag {
 public:
  using UIntFlag::ValueType;
  using UIntFlag::VerifyFunction;

  SizeFlag(const char* name, const char* type, const char* cmt,
           const char* flag, int line, ValueType&& dv, VerifyFunction&& v = {})
      : UIntFlag(name, type, cmt, flag, line, std::move(dv), std::move(v)) {}

  Error From(const Value&) override;
};

using MsFlag = DurationMsFlag<1>;
using SecFlag = DurationMsFlag<1000>;
using MinFlag = DurationMinFlag<1>;
using HourFlag = DurationMinFlag<60>;
using DayFlag = DurationMinFlag<60 * 24>;
using ByteFlag = SizeFlag<1>;
using KByteFlag = SizeFlag<1024>;
using MByteFlag = SizeFlag<1024 * 1024>;
using GByteFlag = SizeFlag<1024 * 1024 * 1024>;

template <typename S>
class CustomizedFlag : public FlagInfo {
 public:
  using FromFunction = std::function<Result<S>(const Value&, const char*)>;
  using ToFunction = std::function<Value(const S&)>;

  // Any type constructor. User needs to register ConvertFunction to convert
  // the parsed value, ie |Value| into a customized |object|.
  CustomizedFlag(const char* name, const char* type, const char* cmt,
                 const char* file, int line, FromFunction&& from_function,
                 ToFunction&& to_function, std::optional<S> default_value = {})
      : FlagInfo(name, type, cmt, file, line),
        value_(),
        default_value_(std::move(default_value)),
        ref_(nullptr),
        c_(std::move(from_function)),
        t_(std::move(to_function)) {
    // Notes(dpeng): The default_value is always been used by assignment.
    if (default_value_) {
      value_ = default_value_.value();
    }
  }

 public:
  const S* MaybeGet() const { return ref_; }
  S* MaybeGet() { return ref_; }

  const S& Get() const { return *NOT_NULL(ref_); }
  S& Get() { return *NOT_NULL(ref_); }

  const std::optional<S>& default_value() const { return default_value_; }

 public:
  std::string GetDefaultRepresentation() const override {
    return default_value_ ? "<object>" : "N/A";
  }

  Error From(const Value&) override;
  Value To() const override;

 private:
  std::optional<S> value_;
  std::optional<S> default_value_;
  S* ref_;
  FromFunction c_;
  ToFunction t_;

  DISALLOW_COPY_AND_ASSIGN(CustomizedFlag);
};

// Central hub for flag registration
class FlagRegistry {
 public:
  using Hub = std::vector<FlagInfo*>;
  using iterator = Hub::iterator;
  using const_iterator = Hub::const_iterator;

  inline bool Add(FlagInfo*);
  inline bool Remove(FlagInfo*);
  inline FlagInfo* Get(const std::string_view&) const;

  iterator begin() { return hub_.begin(); }
  iterator end() { return hub_.end(); }
  const_iterator begin() const { return hub_.begin(); }
  const_iterator end() const { return hub_.end(); }

  std::size_t GetSize() const { return hub_.size(); }
  bool IsEmpty() const { return hub_.empty(); }

 public:
  static FlagRegistry& GetInstance() {
    static FlagRegistry kInstance;
    return kInstance;
  }

 private:
  Hub hub_;
  FlagRegistry() = default;

  DISALLOW_COPY_AND_ASSIGN(FlagRegistry);
};

}  // namespace _

enum { kErrorInvalidType = 1, kErrorInvalidValue, kErrorFlagNotExisted };

const ErrorCategory* ErrCategory();

inline Error Err(int, const char*, ...);
inline Error ErrV(int, const char*, va_list);

// This is the main interface for setting up a flag with parsed |Value| object
// It is supposed to be called by external cli/config parser for setup the
// global Flag operations. Notes, the flag itself is not thread safe.
inline Error From(const std::string_view&, const Value&);

// Convert a flag's name into its current configured value
inline Result<Value> To(const std::string_view&);

// Check whether a name is related to a global flag
inline bool IsFlag(const std::string_view&);

// List all flags help information for huamn digestion
std::string ListFlags();
Value DumpFlags();

inline bool IsErrorFlagNotExisted(const Error& e) {
  return e.category() == ErrCategory() && e.reason() == kErrorFlagNotExisted;
}

}  // namespace flag
}  // namespace base

// ---------------------------------------------------------------
// Interfaces(Macro)
//
//   The following macro naming convention is still similar as
//   gflags, type is in lower case. Obviously, gflags doesn't
//   support compound type and we extend the macro accordingly
//
//   Notes, we don't use FLAGS_ but FLAG_ to access the variable.
//   Unlike gflags, the flag itself is declared as its related c++
//   types, we wrap the whole variable into a object, user needs to
//   use following code to access flags :
//
//       FLAGS_my_flag.Get()
//
// Usage: the macro is in following definition. Notes, we use a
//   trick to mimic overload macro for verify function. The
//   default value is been ignored intentionally
//
//
// For any none |ANY| type macro, the prototype is as following
//
//   DEFINE_{type}(variable_name, comment, default_value, [verify])
//
// DEFINE_string(a_string, "this is a string", "hello world");
// DEFINE_string(a_string, "this is a string", "hello",
//              [](const std::string&, const char*) -> bool { ... });
//
//
// For list type, the default_value is also optional, when not specified,
// then the default is empty list.
//
// DEFINE_string_list(A, "this is a string list");
// DEFINE_string_list(B, "...", {"a", "b"});
// DEFINE_string_list(C, "...", {},
//                    [](const std::vector<std::string>& v, const char*) {
//                       ....
//                    });
//
// DECLARE_string(...) will just introduce the flag/global variable.
// ie external xxxx
//
// ---------------------------------------------------------------

// primitive types
#define DEFINE_string(NAME, COMMENT, ...)                         \
  ::base::flag::_::StringFlag CONCATE(FLAG_, NAME) {              \
    STR(NAME), "string", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_bool(NAME, COMMENT, ...)                         \
  ::base::flag::_::BoolFlag CONCATE(FLAG_, NAME) {              \
    STR(NAME), "bool", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_int(NAME, COMMENT, ...)                         \
  ::base::flag::_::IntFlag CONCATE(FLAG_, NAME) {              \
    STR(NAME), "int", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_uint(NAME, COMMENT, ...)                         \
  ::base::flag::_::UIntFlag CONCATE(FLAG_, NAME) {              \
    STR(NAME), "uint", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_real(NAME, COMMENT, ...)                         \
  ::base::flag::_::RealFlag CONCATE(FLAG_, NAME) {              \
    STR(NAME), "real", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

// list types
#define DEFINE_string_list(NAME, ...)                          \
  ::base::flag::_::StringListFlag CONCATE(FLAG_, NAME) {       \
    STR(NAME), "list<string>", __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_bool_list(NAME, ...)                          \
  ::base::flag::_::BoolListFlag CONCATE(FLAG_, NAME) {       \
    STR(NAME), "list<bool>", __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_int_list(NAME, ...)                          \
  ::base::flag::_::IntListFlag CONCATE(FLAG_, NAME) {       \
    STR(NAME), "list<int>", __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_uint_list(NAME, ...)                          \
  ::base::flag::_::UIntListFlag CONCATE(FLAG_, NAME) {       \
    STR(NAME), "list<uint>", __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_real_list(NAME, ...)                          \
  ::base::flag::_::RealListFlag CONCATE(FLAG_, NAME) {       \
    STR(NAME), "list<real>", __FILE__, __LINE__, __VA_ARGS__ \
  }

// Size
#define DEFINE_byte(NAME, COMMENT, ...)                            \
  ::base::flag::_::ByteFlag CONCATE(FLAG_, NAME) {                 \
    STR(NAME), "size<b>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_kb(NAME, COMMENT, ...)                               \
  ::base::flag::_::KByteFlag CONCATE(FLAG_, NAME) {                 \
    STR(NAME), "size<kb>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_mb(NAME, COMMENT, ...)                               \
  ::base::flag::_::MByteFlag CONCATE(FLAG_, NAME) {                 \
    STR(NAME), "size<mb>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_gb(NAME, COMMENT, ...)                               \
  ::base::flag::_::GByteFlag CONCATE(FLAG_, NAME) {                 \
    STR(NAME), "size<gb>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

// Duration
#define DEFINE_msec(NAME, COMMENT, ...)                                   \
  ::base::flag::_::MsFlag CONCATE(FLAG_, NAME) {                          \
    STR(NAME), "duration<msec>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_sec(NAME, COMMENT, ...)                                   \
  ::base::flag::_::SecFlag CONCATE(FLAG_, NAME) {                        \
    STR(NAME), "duration<sec>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_min(NAME, COMMENT, ...)                                   \
  ::base::flag::_::MinFlag CONCATE(FLAG_, NAME) {                        \
    STR(NAME), "duration<min>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DEFINE_hour(NAME, COMMENT, ...)                                   \
  ::base::flag::_::HourFlag CONCATE(FLAG_, NAME) {                        \
    STR(NAME), "duration<hour>", COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

// Any macro, is a specialized way to define customize structure/object. The
// usage is to provide |type| as part of the macro and also provide a converter
// function to convert Value into a structure corresponding to the |type|.
//
//  DEFINE_any(MyType, my_type, "A specialized structure",
//             [](const Value& argument, const char* name) -> Result<MyType> {
//             ...
//             });
//
#define DEFINE_any(TYPE, NAME, COMMENT, ...)                       \
  ::base::flag::_::CustomizedFlag<TYPE> CONCATE(FLAG_, NAME) {     \
    STR(NAME), STR(TYPE), COMMENT, __FILE__, __LINE__, __VA_ARGS__ \
  }

#define DECLARE_any(TYPE, NAME) \
  extern ::base::flag::_::CustomizedFlag<TYPE> CONCATE(FLAG_, NAME);

#define DECLARE_string(NAME) \
  extern ::base::flag::_::StringFlag CONCATE(FLAG_, NAME)
#define DECLARE_int(NAME) extern ::base::flag::_::IntFlag CONCATE(FLAG_, NAME)
#define DECLARE_uint(NAME) extern ::base::flag::_::UIntFlag CONCATE(FLAG_, NAME)
#define DECLARE_bool(NAME) extern ::base::flag::_::BoolFlag CONCATE(FLAG_, NAME)
#define DECLARE_real(NAME) extern ::base::flag::_::RealFlag CONCATE(FLAG_, NAME)

#define DECLARE_string_list(NAME) \
  extern ::base::flag::_::StringListFlag CONCATE(FLAG_, NAME)

#define DECLARE_int_list(NAME) \
  extern ::base::flag::_::IntListFlag CONCATE(FLAG_, NAME)

#define DECLARE_uint_list(NAME) \
  extern ::base::flag::_::UIntListFlag CONCATE(FLAG_, NAME)

#define DECLARE_bool_list(NAME) \
  extern ::base::flag::_::BoolListFlag CONCATE(FLAG_, NAME)

#define DECLARE_real_list(NAME) \
  extern ::base::flag::_::RealListFlag CONCATE(FLAG_, NAME)

#define DECLARE_byte(NAME) extern ::base::flag::_::ByteFlag CONCATE(FLAG_, NAME)
#define DECLARE_kb(NAME) extern ::base::flag::_::KByteFlag CONCATE(FLAG_, NAME)
#define DECLARE_mb(NAME) extern ::base::flag::_::MByteFlag CONCATE(FLAG_, NAME)
#define DECLARE_gb(NAME) extern ::base::flag::_::GByteFlag CONCATE(FLAG_, NAME)

#define DECLARE_msec(NAME) extern ::base::flag::_::MsFlag CONCATE(FLAG_, NAME)
#define DECLARE_sec(NAME) extern ::base::flag::_::SecFlag CONCATE(FLAG_, NAME)
#define DECLARE_min(NAME) extern ::base::flag::_::MinFlag CONCATE(FLAG_, NAME)
#define DECLARE_hour(NAME) extern ::base::flag::_::HourFlag CONCATE(FLAG_, NAME)

#include "flag-inl.h"

#endif  // _BASE_FLAG_H_
