#ifndef _BASE_CONFIG_REFLECTION_H_
#define _BASE_CONFIG_REFLECTION_H_

#include <base/check.h>
#include <base/common.h>
#include <base/result.h>
#include <base/trait.h>
#include <base/util.h>
#include <base/value.h>

#include "basic.h"

namespace base {
namespace config {

class StructInfo;

enum class FieldType : std::uint8_t { kRequire, kOptional };

// We don't choose to use a template method but using a runtime/dynamic method
// to represent the field. We are using std::any to avoid directly using the
// raw void* to bypass the C++ type system. This is mainly used in Set and Get
// interfaces. Notes, the Set/Get is not used that frequently, since the config
// is only used when the config is been set up and most of the time, each tenant
// config is not changed that frequent.
class Field {
 public:
  Field(const StructInfo* struct_info, const std::string& name,
        const std::string& comment, const std::string& config_path,
        FieldType type)
      : struct_info_(struct_info),
        name_(name),
        comment_(comment),
        config_path_(config_path),
        search_pattern_(),
        field_type_(type) {
    auto r = Value::CompileSearchPattern(config_path_);
    CHECK(r) << r.GetError().GetDisplay();
    search_pattern_ = r.PassResult();
  }

  virtual ~Field() {}

 public:
  // Set the field by using the |Value| object. The set operation may failed
  // when the config fields internal type doesn't match the |Value|'s input.
  // If so, error returned.
  virtual Error Set(std::any*, const Value&) const = 0;

  // Get the value internally and wrapping it into the |Value|. It is mostly
  // used for printing/debugging purpose.
  virtual Result<Value> Get(std::any*) const = 0;

  // Get the default value of the field. The default value is been hardcoded
  // when the reflection is setup.
  virtual Value GetDefault() const = 0;

  // Modify the default value to be the one that is not the one set in the
  // refelection section. This is only used in testing framework
  virtual bool ModifyDefault(const Value&) const = 0;

 public:
  template <typename T, typename U>
  Error SetValue(T* ptr, U&&) const;

  template <typename T>
  Error SetValue(T*, const Value&) const;

  template <typename T>
  Result<Value> GetValue(const T*) const;

  template <typename T>
  Error SetDefault(T* ptr) const;

  inline Error SetDefault(std::any* ptr) const;

 public:
  const std::string& name() const { return name_; }
  const std::string& comment() const { return comment_; }
  const std::string& config_path() const { return config_path_; }
  Value::SearchPattern* search_pattern() { return search_pattern_.get(); }
  FieldType field_type() const { return field_type_; }
  const StructInfo* struct_info() const { return struct_info_; }

 private:
  const StructInfo* struct_info_;
  const std::string name_;
  const std::string comment_;
  const std::string config_path_;
  std::unique_ptr<Value::SearchPattern> search_pattern_;
  FieldType field_type_;

  DISALLOW_COPY_AND_ASSIGN(Field);
};

template <typename T, typename U>
class FieldMemberPointer : public Field {
 public:
  using MemberPointer = U T::*;
  using ValueParser = std::function<Result<U>(const Value&)>;
  using ValueVerifier = std::function<bool(const U&)>;
  using ValueSinker = std::function<Error(T*, const U&)>;

  template <typename D>
  FieldMemberPointer(const StructInfo* struct_info, const std::string& name,
                     const std::string& comment, const std::string& config_path,
                     FieldType field_type, D&& default_value, MemberPointer mp,
                     ValueVerifier&& vv = {}, ValueParser&& vp = {},
                     ValueSinker&& vc = {})
      : Field(struct_info, name, comment, config_path, field_type),
        default_value_(std::forward<D>(default_value)),
        member_pointer_(mp),
        value_verifier_(std::move(vv)),
        value_parser_(std::move(vp)),
        value_sinker_(std::move(vc)) {}

 public:
  Error Set(std::any*, const Value&) const override;
  Result<Value> Get(std::any*) const override;

  Value GetDefault() const override;
  bool ModifyDefault(const Value& v) const override;

 private:
  T* GetPtr(std::any*) const;
  const T* GetConstPtr(std::any*) const;

  U default_value_;
  MemberPointer member_pointer_;
  ValueVerifier value_verifier_;
  ValueParser value_parser_;
  ValueSinker value_sinker_;
};

class StructInfo {
 public:
  using FieldList = std::vector<std::unique_ptr<Field>>;

  StructInfo(const char* struct_name, const char* comment)
      : struct_name_(NormalizeStructName(struct_name)),
        comment_(comment),
        field_list_() {}

 public:
  template <typename T, typename U, typename D>
  void Add(const char* name, const char* comment, const char* config_path,
           FieldType field_type, D&& default_value, U T::*member_pointer);

  template <typename T, typename U, typename D>
  void Add(const char* name, const char* comment, const char* config_path,
           FieldType field_type, D&& default_value, U T::*member_pointer,
           std::function<bool(const U&)>&& vv);

  template <typename T, typename U, typename D>
  void Add(const char* name, const char* comment, const char* config_path,
           FieldType field_type, D&& default_value, U T::*member_pointer,
           std::function<Result<U>(const base::Value&)>&& vv);

  template <typename T, typename U, typename D>
  void Add(const char* name, const char* comment, const char* config_path,
           FieldType field_type, D&& default_value, U T::*member_pointer,
           std::function<Error(T*, const U&)>&& vv);

  template <typename T, typename U, typename D>
  void Add(const char* name, const char* comment, const char* config_path,
           FieldType field_type, D&& default_value, U T::*member_pointer,
           std::function<bool(const U&)>&& vv,
           std::function<Result<U>(const Value&)>&& vp);

  template <typename T, typename U, typename D>
  void Add(const char* name, const char* comment, const char* config_path,
           FieldType field_type, D&& default_value, U T::*member_pointer,
           std::function<bool(const U&)>&& vv,
           std::function<Result<U>(const Value&)>&& vp,
           std::function<Error(T*, const U&)>&& vs);

 public:
  const std::string& struct_name() const { return struct_name_; }
  const std::string& comment() const { return comment_; }

  const FieldList& field_list() const { return field_list_; }
  inline const Field* GetField(const char* name) const;

  base::Value Dump() const;
  std::string PrintString() const;

 public:
  // Normalize struct name. All the struct name will have leading :: as its
  // qualify name to indicate it starts from global scope.
  static inline std::string NormalizeStructName(const std::string&);

 private:
  std::string struct_name_;
  std::string comment_;
  FieldList field_list_;
};

// Get the struct info of the specified type or compile-time error if the type
// |T| is not related to a reflection/config type, ie haven't use the macro,
// CONFIG_REFLECTION to link relationship.
template <typename T>
const StructInfo& GetStructInfo();

// Get a config struct T and set all its reflection attribute to be default
// value
template <typename T>
T GetDefault();

}  // namespace config
}  // namespace base

// Macros to use ---------------------------------------------------------
//
// Notes the following stuff is purposely put into the filescope, ie global
// namespace instead of any other namespaces. The name has prefix __ to indicate
// its private usage.
//
// Relation to map between a type, user defined config, to a reflection object.
// This mapping is automatically defined by using the macro trick.
template <typename T>
struct __ConfigReflection : ::base::FalseType {};

#define CONFIG_REFLECTION_DECLARE(NAME, COMMENT, TYPE)                       \
  template <>                                                                \
  struct __ConfigReflection<TYPE> : ::base::TrueType {                        \
   private:                                                                  \
    static ::base::config::StructInfo* GetStructInfoMutable() {               \
      static ::base::config::StructInfo kInfo{#TYPE, COMMENT};                \
      return &kInfo;                                                         \
    }                                                                        \
                                                                             \
   public:                                                                   \
    static const ::base::config::StructInfo& GetStructInfo() {                \
      return *GetStructInfoMutable();                                        \
    }                                                                        \
    static void Ctor();                                                      \
    template <typename T, typename U, typename D>                            \
    static void Attr(const char* name, const char* comment,                  \
                     const char* config_path,                                \
                     ::base::config::FieldType field_type, D&& default_value, \
                     U T::*member_pointer) {                                 \
      static_assert(::base::_::ValueConvertType<U>::kValue);                  \
      GetStructInfoMutable()->Add(name, comment, config_path, field_type,    \
                                  std::forward<D>(default_value),            \
                                  member_pointer);                           \
    }                                                                        \
    template <typename T, typename U, typename D, typename H1>               \
    static void Attr(const char* name, const char* comment,                  \
                     const char* config_path,                                \
                     ::base::config::FieldType field_type, D&& default_value, \
                     U T::*member_pointer, H1&& vv) {                        \
      static_assert(::base::_::ValueConvertType<U>::kValue);                  \
      GetStructInfoMutable()->Add(name, comment, config_path, field_type,    \
                                  std::forward<D>(default_value),            \
                                  member_pointer,                            \
                                  base::BindFunction(std::forward<H1>(vv)));  \
    }                                                                        \
    template <typename T, typename U, typename D, typename H1, typename H2>  \
    static void Attr(const char* name, const char* comment,                  \
                     const char* config_path,                                \
                     ::base::config::FieldType field_type, D&& default_value, \
                     U T::*member_pointer, H1&& vv, H2&& vc) {               \
      static_assert(::base::_::ValueConvertType<U>::kValue);                  \
      GetStructInfoMutable()->Add(name, comment, config_path, field_type,    \
                                  std::forward<D>(default_value),            \
                                  member_pointer,                            \
                                  base::BindFunction(std::forward<H1>(vv)),   \
                                  base::BindFunction(std::forward<H2>(vc)));  \
    }                                                                        \
    template <typename T, typename U, typename D, typename H1, typename H2,  \
              typename H3>                                                   \
    static void Attr(const char* name, const char* comment,                  \
                     const char* config_path,                                \
                     ::base::config::FieldType field_type, D&& default_value, \
                     U T::*member_pointer, H1&& vv, H2&& vp, H3&& vc) {      \
      static_assert(::base::_::ValueConvertType<U>::kValue);                  \
      GetStructInfoMutable()->Add(name, comment, config_path, field_type,    \
                                  std::forward<D>(default_value),            \
                                  member_pointer,                            \
                                  base::BindFunction(std::forward<H1>(vv)),   \
                                  base::BindFunction(std::forward<H2>(vp)),   \
                                  base::BindFunction(std::forward<H3>(vc)));  \
    }                                                                        \
  }

#define CONFIG_REFLECTION(NAME, TYPE)                                         \
  static ::base::_::Registry CONCATE(                                          \
      kConfigRTTIRegister, NAME)([]() { __ConfigReflection<TYPE>::Ctor(); }); \
  void __ConfigReflection<TYPE>::Ctor()

#include "reflection-inl.h"

#endif  // _BASE_CONFIG_REFLECTION_H_
