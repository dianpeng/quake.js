#ifndef _BASE_CONFIG_REFLECTION_INL_H_
#define _BASE_CONFIG_REFLECTION_INL_H_

namespace base {
namespace config {

template <typename T>
Error Field::SetValue(T* ptr, const Value& value) const {
  std::any wrapper{ptr};
  return Set(&wrapper, value);
}

template <typename T, typename U>
Error Field::SetValue(T* ptr, U&& value) const {
  auto maybe_value = ToValue(std::forward<U>(value));
  if (!maybe_value) {
    return Err(kErrorSetType);
  }
  std::any wrapper{ptr};
  return Set(&wrapper, maybe_value.value());
}

template <typename T>
Result<Value> Field::GetValue(const T* ptr) const {
  std::any wrapper{ptr};
  return Get(&wrapper);
}

template <typename T>
Error Field::SetDefault(T* ptr) const {
  std::any wrapper{ptr};
  return Set(&wrapper, GetDefault());
}

inline Error Field::SetDefault(std::any* ptr) const {
  return Set(ptr, GetDefault());
}

template <typename T, typename U>
Value FieldMemberPointer<T, U>::GetDefault() const {
  auto maybe = ToValue(default_value_);
  CHECK(maybe);
  return maybe.value();
}

template <typename T, typename U>
bool FieldMemberPointer<T, U>::ModifyDefault(const Value& v) const {
  if (auto maybe = base::Get<U>(v); maybe) {
    // We need to hack around it, maybe just name the default_value_ as mutable
    const_cast<FieldMemberPointer<T, U>*>(this)->default_value_ =
        std::move(*maybe);
    return true;
  }
  return false;
}

template <typename T, typename U>
T* FieldMemberPointer<T, U>::GetPtr(std::any* r) const {
  try {
    return std::any_cast<T*>(*r);
  } catch (...) {
    return nullptr;
  }
}

template <typename T, typename U>
const T* FieldMemberPointer<T, U>::GetConstPtr(std::any* r) const {
  try {
    return std::any_cast<T*>(*r);
  } catch (...) {
    try {
      return std::any_cast<const T*>(*r);
    } catch (...) {
      return nullptr;
    }
  }
}

template <typename T, typename U>
Error FieldMemberPointer<T, U>::Set(std::any* receiver,
                                    const Value& value) const {
  auto ptr = GetPtr(receiver);
  if (!ptr) {
    return ::base::config::Err(::base::config::kErrorReceiver);
  }

  // fetcher, or value parser
  auto get_value = [&]() -> base::Result<U> {
    if (value_parser_) {
      return value_parser_(value);
    } else {
      auto maybe_value = ::base::Get<U>(value);
      if (!maybe_value) {
        return Err(kErrorSetType, "invalid type: %s", value.type_name());
      } else {
        return base::Pass(maybe_value);
      }
    }
  };

  auto maybe = get_value();
  if (!maybe) return maybe.PassError();
  auto v = maybe.PassResult();

  // predication
  if (value_verifier_) {
    if (!value_verifier_(v)) {
      return Err(kErrorVerify);
    }
  }

  // Store the value into the member field
  ptr->*member_pointer_ = std::move(v);

  // Optinoally call value_sinker_ to perform additionaly operations
  if (value_sinker_) {
    if (auto e = value_sinker_(ptr, ptr->*member_pointer_)) {
      return e;
    }
  }
  return {};
}

template <typename T, typename U>
Result<Value> FieldMemberPointer<T, U>::Get(std::any* receiver) const {
  auto ptr = GetConstPtr(receiver);
  if (!ptr) {
    return Err(kErrorReceiver);
  }

  auto maybe_value = ToValue(ptr->*member_pointer_);
  if (!maybe_value) {
    return Err(kErrorGetType);
  }

  return Pass(maybe_value);
}

inline std::string StructInfo::NormalizeStructName(const std::string& name) {
  if (name.size() < 2 || (name[0] != ':' || name[1] != ':')) {
    std::string dup;
    dup.reserve(name.size() + 2);

    dup += "::";
    dup += name;
    return dup;
  }
  return name;
}

inline const Field* StructInfo::GetField(const char* name) const {
  auto itr = std::find_if(field_list_.begin(), field_list_.end(),
                          [&](const std::unique_ptr<Field>& f) -> bool {
                            return f->name() == name;
                          });

  return (itr == field_list_.end() ? nullptr : itr->get());
}

template <typename T, typename U, typename D>
void StructInfo::Add(const char* name, const char* comment,
                     const char* config_path, FieldType field_type,
                     D&& default_value, U T::*member_pointer) {
  auto field = std::make_unique<FieldMemberPointer<T, U>>(
      this, name, comment, config_path, field_type,
      std::forward<D>(default_value), member_pointer);

  field_list_.push_back(std::move(field));
}

template <typename T, typename U, typename D>
void StructInfo::Add(const char* name, const char* comment,
                     const char* config_path, FieldType field_type,
                     D&& default_value, U T::*member_pointer,
                     std::function<bool(const U&)>&& vv) {
  auto field = std::make_unique<FieldMemberPointer<T, U>>(
      this, name, comment, config_path, field_type,
      std::forward<D>(default_value), member_pointer, std::move(vv));

  field_list_.push_back(std::move(field));
}

template <typename T, typename U, typename D>
void StructInfo::Add(const char* name, const char* comment,
                     const char* config_path, FieldType field_type,
                     D&& default_value, U T::*member_pointer,
                     std::function<Result<U>(const base::Value&)>&& vv) {
  auto field = std::make_unique<FieldMemberPointer<T, U>>(
      this, name, comment, config_path, field_type,
      std::forward<D>(default_value), member_pointer,
      std::function<bool(const U&)>{}, std::move(vv));

  field_list_.push_back(std::move(field));
}

template <typename T, typename U, typename D>
void StructInfo::Add(const char* name, const char* comment,
                     const char* config_path, FieldType field_type,
                     D&& default_value, U T::*member_pointer,
                     std::function<Error(T*, const U&)>&& vv) {
  auto field = std::make_unique<FieldMemberPointer<T, U>>(
      this, name, comment, config_path, field_type,
      std::forward<D>(default_value), member_pointer,
      std::function<bool(const U&)>{},
      std::function<Result<U>(const base::Value&)>{}, std::move(vv));

  field_list_.push_back(std::move(field));
}

template <typename T, typename U, typename D>
void StructInfo::Add(const char* name, const char* comment,
                     const char* config_path, FieldType field_type,
                     D&& default_value, U T::*member_pointer,
                     std::function<bool(const U&)>&& vv,
                     std::function<Result<U>(const base::Value&)>&& vp) {
  auto field = std::make_unique<FieldMemberPointer<T, U>>(
      this, name, comment, config_path, field_type,
      std::forward<D>(default_value), member_pointer, std::move(vv),
      std::move(vp));

  field_list_.push_back(std::move(field));
}

template <typename T, typename U, typename D>
void StructInfo::Add(const char* name, const char* comment,
                     const char* config_path, FieldType field_type,
                     D&& default_value, U T::*member_pointer,
                     std::function<bool(const U&)>&& vv,
                     std::function<Result<U>(const base::Value&)>&& vp,
                     std::function<Error(T*, const U&)>&& vc) {
  auto field = std::make_unique<FieldMemberPointer<T, U>>(
      this, name, comment, config_path, field_type,
      std::forward<D>(default_value), member_pointer, std::move(vv),
      std::move(vp), std::move(vc));

  field_list_.push_back(std::move(field));
}

template <typename T>
const StructInfo& GetStructInfo() {
  return ::__ConfigReflection<T>::GetStructInfo();
}

template <typename T>
T GetDefault() {
  const auto& struct_info = GetStructInfo<T>();
  T v;
  std::any wrapper{&v};
  for (const auto& entry : struct_info.field_list()) {
    auto err = entry->Set(&wrapper, entry->GetDefault());
    CHECK(!err) << err.GetDisplay();
  }
  return v;
}

}  // namespace config
}  // namespace base

#endif  // _BASE_CONFIG_REFLECTION_INL_H_
