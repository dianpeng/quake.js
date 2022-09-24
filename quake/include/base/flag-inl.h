#ifndef _BASE_FLAG_INL_H_
#define _BASE_FLAG_INL_H_

namespace base {
namespace flag {
namespace _ {

inline FlagInfo::FlagInfo(const char* name, const char* type, const char* cmt,
                          const char* file, int line)
    : name_(name), type_(type), comment_(cmt), file_(file), line_(line) {
  CHECK(FlagRegistry::GetInstance().Add(this))
      << "flag: " << name_ << " has been registered before";
}

inline FlagInfo::~FlagInfo() {
  CHECK(FlagRegistry::GetInstance().Remove(this))
      << "flag: " << name() << " is not existed";
}

template <typename S>
bool Assign(const Value& v, S* out) {
  switch (v.type()) {
    case Value::kInt:
      // Notes(dpeng): Value's integer is represented as std::int64_t and
      //   doesn't differentiate between sign and unsign. But FLAG does have
      //   difference. Here we need to do the correct conversion accordingly
      if constexpr (IsInteger<S>::kValue) {
        // Tihs does not do clamp, the clamping mode is not correct since c++
        // doesn't define such thing
        *out = ClampCast<S>(v.ToInt());
        return true;
      } else if constexpr (std::is_floating_point<S>::value) {
        *out = ClampCast<S>(v.ToInt());
        return true;
      }
      break;

    case Value::kReal:
      if constexpr (std::is_floating_point<S>::value) {
        *out = static_cast<S>(v.ToReal());
        return true;
      }
      break;

    case Value::kBoolean:
      if constexpr (IsBoolean<S>::kValue) {
        *out = v.ToBoolean();
        return true;
      }
      break;

    case Value::kString:
      if constexpr (std::is_same<S, std::string>::value) {
        *out = v.ToString();
        return true;
      }
      break;

    default:
      break;
  }
  return false;
}

template <typename S>
Error PrimitiveFlag<S>::From(const Value& v) {
  if (!Assign(v, &value_)) {
    return Err(kErrorInvalidType, "%s's type is %s, but the option type is %s",
               FlagInfo::name(), FlagInfo::type(), v.type_name());
  } else {
    if (verifier_ && !verifier_(value_, FlagInfo::name())) {
      return Err(kErrorInvalidValue, "%s's value verification failed",
                 FlagInfo::name());
    }
    return Error{};
  }
}

template <typename S>
Value ListFlag<S>::To() const {
  Value v;
  auto l = v.SetList();
  for (const auto& x : value_) {
    l->Add(Value{x});
  }
  return v;
}

template <typename T>
Error ListFlag<T>::From(const Value& v) {
  if (v.type() == Value::kList) {
    const auto& list = *v.ToListPtr();
    value_.reserve(list.GetSize());
    value_.clear();

    for (const auto& e : list) {
      T temp;
      // Notes(dpeng): We just ignore entry that cannot be used/assigned for
      //   list value for now.
      UNUSE(Assign(e, &temp));
      value_.push_back(std::move(temp));
    }
    if (verifier_ && !verifier_(value_, FlagInfo::name())) {
      return Err(kErrorInvalidValue, "%s's value verification failed",
                 FlagInfo::name());
    }
    return Error{};
  }

  return Err(kErrorInvalidType, "%s's type is %s, but option type is %s",
             FlagInfo::name(), FlagInfo::type(), v.type_name());
}

template <typename S>
Value CustomizedFlag<S>::To() const {
  return *NOT_NULL(ref_)->Dump();
}

template <std::size_t kG>
Error DurationMsFlag<kG>::From(const Value& v) {
  if (!v.IsDuration() && !v.IsInt()) {
    return Err(kErrorInvalidType,
               "%s's type is duration, but option type is %s", FlagInfo::name(),
               v.type_name());
  } else if (v.IsInt() && v.ToInt() < 0) {
    return Err(kErrorInvalidType,
               "%s's type is duration, but option has a negative integer",
               FlagInfo::name());
  }

  std::uint64_t ms;
  if (v.IsDuration()) {
    ms = v.ToDuration().AsMillisecond();
  } else {
    ms = static_cast<std::uint64_t>(v.ToInt());
  }

  Set(ms / kG);
  if (verifier() && !verifier()(UIntFlag::Get(), FlagInfo::name())) {
    return Err(kErrorInvalidValue, "%s's value verification failed",
               FlagInfo::name());
  }
  return Error{};
}

template <std::size_t kG>
Error DurationMinFlag<kG>::From(const Value& v) {
  if (!v.IsDuration()) {
    return Err(kErrorInvalidType,
               "%s's type is duration, but option type is %s", FlagInfo::name(),
               v.type_name());
  } else if (v.IsInt() && v.ToInt() < 0) {
    return Err(kErrorInvalidType,
               "%s's type is duration, but option has a negative integer",
               FlagInfo::name());
  }

  std::uint64_t min;
  if (v.IsDuration()) {
    min = v.ToDuration().AsMinute();
  } else {
    min = static_cast<std::uint64_t>(v.ToInt());
  }
  Set(min / kG);
  if (verifier() && !verifier()(UIntFlag::Get(), FlagInfo::name())) {
    return Err(kErrorInvalidValue, "%s's value verification failed",
               FlagInfo::name());
  }
  return Error{};
}

template <std::size_t kG>
Error SizeFlag<kG>::From(const Value& v) {
  if (!v.IsStorageSize() && !v.IsInt()) {
    return Err(kErrorInvalidType, "%s's type is storage, but option type is %s",
               FlagInfo::name(), v.type_name());
  } else if (v.IsInt() && v.ToInt() < 0) {
    return Err(kErrorInvalidType,
               "%s's type is duration, but option has a negative integer",
               FlagInfo::name());
  }
  std::uint64_t bs;

  if (v.IsStorageSize()) {
    bs = v.ToStorageSize().AsBSize();
  } else {
    bs = static_cast<std::uint64_t>(v.ToInt());
  }
  Set(bs / kG);
  if (verifier() && !verifier()(UIntFlag::Get(), FlagInfo::name())) {
    return Err(kErrorInvalidValue, "%s's value verification failed",
               FlagInfo::name());
  }
  return Error{};
}

template <typename S>
Error CustomizedFlag<S>::From(const Value& v) {
  DCHECK(c_);
  auto r = c_(v, FlagInfo::name());
  if (r) {
    default_value_ = std::move(r.PassValue());
    ref_ = &(default_value_.value());
    return Error{};
  } else {
    return r.PassError();
  }
}

inline bool FlagRegistry::Add(FlagInfo* i) {
  if (Get(i->name())) {
    return false;
  }
  hub_.push_back(i);
  return true;
}

inline bool FlagRegistry::Remove(FlagInfo* i) {
  auto itr = std::find(hub_.begin(), hub_.end(), i);
  if (itr != hub_.end()) {
    hub_.erase(itr);
    return true;
  }
  return false;
}

inline FlagInfo* FlagRegistry::Get(const std::string_view& n) const {
  auto itr = std::find_if(hub_.begin(), hub_.end(), [&](FlagInfo* fi) -> bool {
    return fi->name() == n;
  });
  return itr == hub_.end() ? nullptr : *itr;
}

}  // namespace _

inline Error Err(int v, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto m = ErrV(v, fmt, vl);
  va_end(vl);
  return m;
}

inline Error ErrV(int v, const char* fmt, va_list vl) {
  auto m = Error::MkV(ErrCategory(), v, fmt, vl);
  return m;
}

inline Error From(const std::string_view& name, const Value& v) {
  if (auto flag = _::FlagRegistry::GetInstance().Get(name); flag) {
    return flag->From(v);
  }
  return Err(kErrorFlagNotExisted, "%.*s is not a valid flag", name.size(),
             name.data());
}

inline Result<Value> To(const std::string_view& name) {
  if (auto flag = _::FlagRegistry::GetInstance().Get(name); flag) {
    return flag->To();
  }
  return Err(kErrorFlagNotExisted, "%.*s is not a valid flag", name.size(),
             name.data());
}

inline bool IsFlag(const std::string_view& name) {
  return _::FlagRegistry::GetInstance().Get(name) != nullptr;
}

}  // namespace flag
}  // namespace base

#endif  // _BASE_FLAG_INL_H_
