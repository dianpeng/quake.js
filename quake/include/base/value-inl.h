#ifndef _BASE_VALUE_INL_H_
#define _BASE_VALUE_INL_H_

namespace base {

inline Value::Value() : storage_() {}

inline Value::Value(std::uint8_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(std::uint16_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(std::uint32_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(std::int8_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(std::int16_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(std::int32_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(std::int64_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(std::size_t v) : storage_(static_cast<std::int64_t>(v)) {}

inline Value::Value(double v) : storage_(v) {}

inline Value::Value(float v) : storage_(static_cast<double>(v)) {}

inline Value::Value(bool b) : storage_(b) {}

inline Value::Value(const char* v) : storage_(std::string{v}) {}

inline Value::Value(std::string&& v) : storage_(std::move(v)) {}

inline Value::Value(const std::string_view& v)
    : storage_(std::string{v.data(), v.size()}) {}

inline Value::Value(const std::string& v) : storage_(v) {}

inline Value::Value(std::shared_ptr<List>&& l) : storage_(std::move(l)) {}

inline Value::Value(const std::shared_ptr<List>& l) : storage_(l) {}

inline Value::Value(std::shared_ptr<Dictionary>&& d) : storage_(std::move(d)) {}

inline Value::Value(const std::shared_ptr<Dictionary>& d) : storage_(d) {}

inline Value::Value(Duration&& d) : storage_(std::move(d)) {}

inline Value::Value(const Duration& d) : storage_(d) {}

inline Value::Value(StorageSize&& ss) : storage_(std::move(ss)) {}

inline Value::Value(const StorageSize& ss) : storage_(ss) {}

template <typename R, typename P>
inline Value::Value(const std::chrono::duration<R, P>& d)
    : storage_(static_cast<std::int64_t>(d.count())) {}

inline Value& Value::operator=(std::uint8_t v) {
  SetInt(static_cast<std::int64_t>(v));
  return *this;
}

inline Value& Value::operator=(std::uint16_t v) {
  SetInt(static_cast<std::int64_t>(v));
  return *this;
}

inline Value& Value::operator=(std::uint32_t v) {
  SetInt(static_cast<std::int64_t>(v));
  return *this;
}

inline Value& Value::operator=(std::int8_t v) {
  SetInt(static_cast<std::int64_t>(v));
  return *this;
}

inline Value& Value::operator=(std::int16_t v) {
  SetInt(static_cast<std::int64_t>(v));
  return *this;
}

inline Value& Value::operator=(std::int32_t v) {
  SetInt(static_cast<std::int64_t>(v));
  return *this;
}

inline Value& Value::operator=(std::int64_t v) {
  SetInt(v);
  return *this;
}

inline Value& Value::operator=(std::size_t v) {
  SetInt(static_cast<std::size_t>(v));
  return *this;
}

inline Value& Value::operator=(double v) {
  SetReal(v);
  return *this;
}

inline Value& Value::operator=(float v) {
  SetReal(static_cast<double>(v));
  return *this;
}

inline Value& Value::operator=(bool v) {
  SetBoolean(v);
  return *this;
}

inline Value& Value::operator=(const char* v) {
  SetString(v);
  return *this;
}

inline Value& Value::operator=(const std::string& v) {
  SetString(v);
  return *this;
}

inline Value& Value::operator=(const std::string_view& v) {
  SetString(v);
  return *this;
}

inline Value& Value::operator=(std::string&& v) {
  SetString(std::move(v));
  return *this;
}

inline Value& Value::operator=(std::shared_ptr<List>&& l) {
  SetList(std::move(l));
  return *this;
}

inline Value& Value::operator=(const std::shared_ptr<List>& l) {
  SetList(std::shared_ptr<List>{l});
  return *this;
}

inline Value& Value::operator=(std::shared_ptr<Dictionary>&& d) {
  SetDictionary(std::move(d));
  return *this;
}

inline Value& Value::operator=(const std::shared_ptr<Dictionary>& d) {
  SetDictionary(std::shared_ptr<Dictionary>{d});
  return *this;
}

inline Value& Value::operator=(Duration&& d) {
  SetDuration(std::move(d));
  return *this;
}

inline Value& Value::operator=(const Duration& d) {
  SetDuration(d);
  return *this;
}

inline Value& Value::operator=(StorageSize&& ss) {
  SetStorageSize(std::move(ss));
  return *this;
}

inline Value& Value::operator=(const StorageSize& ss) {
  SetStorageSize(ss);
  return *this;
}

template <typename R, typename P>
inline Value& Value::operator=(const std::chrono::duration<R, P>& d) {
  SetInt(static_cast<std::int64_t>(d.count()));
  return *this;
}

inline Value::Type Value::type() const {
  auto i = storage_.index();
  DCHECK(i >= 0);
  return static_cast<Value::Type>(i);
}

inline bool Value::IsNull() const { return storage_.index() == kNull; }

inline bool Value::IsInt() const { return storage_.index() == kInt; }

inline bool Value::IsReal() const { return storage_.index() == kReal; }

inline bool Value::IsNumber() const { return IsReal() || IsInt(); }

inline bool Value::IsString() const { return storage_.index() == kString; }

inline bool Value::IsBoolean() const { return storage_.index() == kBoolean; }

inline bool Value::IsList() const { return storage_.index() == kList; }

inline bool Value::IsDictionary() const {
  return storage_.index() == kDictionary;
}

inline bool Value::IsDuration() const { return storage_.index() == kDuration; }

inline bool Value::IsStorageSize() const {
  return storage_.index() == kStorageSize;
}

inline bool Value::IsScalar() const {
  return IsNull() || IsInt() || IsReal() || IsString() || IsBoolean();
}

inline std::int64_t Value::ToInt() const {
  DCHECK(IsInt());
  return Get<std::int64_t>(storage_);
}

inline double Value::ToReal() const {
  DCHECK(IsReal());
  return Get<double>(storage_);
}

inline bool Value::ToBoolean() const {
  DCHECK(IsBoolean());
  return Get<bool>(storage_);
}

inline const std::string& Value::ToString() const {
  DCHECK(IsString());
  return Get<std::string>(storage_);
}

inline std::shared_ptr<const List> Value::ToList() const {
  DCHECK(IsList());
  return Get<std::shared_ptr<List>>(storage_);
}

inline std::shared_ptr<const Dictionary> Value::ToDictionary() const {
  DCHECK(IsDictionary());
  return Get<std::shared_ptr<Dictionary>>(storage_);
}

inline std::shared_ptr<List> Value::ToList() {
  DCHECK(IsList());
  return Get<std::shared_ptr<List>>(storage_);
}

inline std::shared_ptr<Dictionary> Value::ToDictionary() {
  DCHECK(IsDictionary());
  return Get<std::shared_ptr<Dictionary>>(storage_);
}

inline const List* Value::ToListPtr() const {
  DCHECK(IsList());
  return Get<std::shared_ptr<List>>(storage_).get();
}

inline const Dictionary* Value::ToDictionaryPtr() const {
  DCHECK(IsDictionary());
  return Get<std::shared_ptr<Dictionary>>(storage_).get();
}

inline List* Value::ToListPtr() {
  DCHECK(IsList());
  return Get<std::shared_ptr<List>>(storage_).get();
}

inline Dictionary* Value::ToDictionaryPtr() {
  DCHECK(IsDictionary());
  return Get<std::shared_ptr<Dictionary>>(storage_).get();
}

inline const Duration& Value::ToDuration() const {
  DCHECK(IsDuration());
  return Get<Duration>(storage_);
}

inline const StorageSize& Value::ToStorageSize() const {
  DCHECK(IsStorageSize());
  return Get<StorageSize>(storage_);
}

inline void Value::SetNull() { storage_ = Null{}; }

inline void Value::SetInt(std::int64_t v) { storage_ = v; }

inline void Value::SetReal(double v) { storage_ = v; }

inline void Value::SetBoolean(bool v) { storage_ = v; }

inline void Value::SetString(const std::string_view& v) {
  storage_ = std::string{v.data(), v.size()};
}

inline void Value::SetString(const char* v) { storage_ = std::string{v}; }

inline void Value::SetString(const std::string& v) { storage_ = v; }

inline void Value::SetString(std::string&& v) { storage_ = std::move(v); }

inline void Value::SetList(std::shared_ptr<List>&& l) {
  storage_ = std::move(l);
}

inline void Value::SetDictionary(std::shared_ptr<Dictionary>&& d) {
  storage_ = std::move(d);
}

inline void Value::SetDuration(Duration&& d) { storage_ = std::move(d); }

inline void Value::SetDuration(const Duration& d) { storage_ = d; }

inline void Value::SetStorageSize(StorageSize&& ss) {
  storage_ = std::move(ss);
}

inline void Value::SetStorageSize(const StorageSize& ss) { storage_ = ss; }

inline std::shared_ptr<List> Value::SetList() {
  auto l = std::make_shared<List>();
  storage_ = l;
  return l;
}

inline std::shared_ptr<Dictionary> Value::SetDictionary() {
  auto d = std::make_shared<Dictionary>();
  storage_ = d;
  return d;
}

inline bool Dictionary::AddIfNotExist(const std::string_view& k,
                                      const Value& v) {
  auto itr = storage_.insert(std::make_pair(std::to_string(k), v));
  return itr.second;
}

inline bool Dictionary::AddIfNotExist(const std::string_view& k, Value&& v) {
  auto itr = storage_.insert(std::make_pair(std::to_string(k), v));
  return itr.second;
}

// TODO(dpeng): Use operator [] ?
inline void Dictionary::Add(const std::string_view& k, const Value& v) {
  auto itr = storage_.insert(std::make_pair(std::to_string(k), v));
  if (!itr.second) itr.first->second = v;
}

inline void Dictionary::Add(const std::string_view& k, Value&& v) {
  auto itr = storage_.insert(std::make_pair(std::to_string(k), Value{}));
  itr.first->second = std::move(v);
}

inline const Value* Dictionary::Get(const std::string_view& k) const {
  auto itr = storage_.find(k);
  return itr == storage_.end() ? nullptr : &(itr->second);
}

inline Value* Dictionary::Get(const std::string_view& k) {
  auto itr = storage_.find(k);
  return itr == storage_.end() ? nullptr : &(itr->second);
}

inline bool Dictionary::Has(const std::string_view& k) const {
  return storage_.end() != storage_.find(k);
}

inline bool Dictionary::Remove(const std::string_view& k) {
  if (auto itr = storage_.find(k); itr != storage_.end()) {
    storage_.erase(itr);
    return true;
  }
  return false;
}

inline std::ostream& operator<<(std::ostream& os, const Value& v) {
  os << v.Print();
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const List& v) {
  os << v.Print();
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const Dictionary& v) {
  os << v.Print();
  return os;
}

inline const char* Value::GetTypeName(Value::Type t) {
  switch (t) {
    case kNull:
      return "null";
    case kInt:
      return "int";
    case kReal:
      return "real";
    case kBoolean:
      return "boolean";
    case kString:
      return "string";
    case kList:
      return "list";
    case kDictionary:
      return "dictionary";
    case kDuration:
      return "duration";
    case kStorageSize:
      return "storage-size";
    default:
      return "";
  }
}

inline std::string Value::ToJSON(ValueToJSON opt) const {
  std::stringstream out;
  ToJSON(out, opt);
  return out.str();
}

inline std::string List::ToJSON(ValueToJSON opt) const {
  std::stringstream out;
  ToJSON(out, opt);
  return out.str();
}

inline std::string Dictionary::ToJSON(ValueToJSON opt) const {
  std::stringstream out;
  ToJSON(out, opt);
  return out.str();
}

// Magic Get/From implementation
namespace _ {

template <typename T>
struct ValueConvertType {
  static constexpr bool kIsBooleanType = IsBoolean<T>::kValue;

  static constexpr bool kIsIntType = IsInteger<T>::kValue;

  static constexpr bool kIsStringType = std::is_same<T, std::string>::value;

  static constexpr bool kIsCStringType = std::is_same<T, const char*>::value;

  static constexpr bool kIsCStringListType =
      std::is_same<T, std::vector<const char*>>::value;

  static constexpr bool kIsRealType =
      std::is_same<T, double>::value || std::is_same<T, float>::value;

  static constexpr bool kIsArrayTypeButNotBitVector =
      std::is_same<T, StringList>::value ||
      std::is_same<T, DoubleList>::value || std::is_same<T, FloatList>::value ||
      std::is_same<T, Int8List>::value || std::is_same<T, Int16List>::value ||
      std::is_same<T, Int32List>::value || std::is_same<T, Int64List>::value ||
      std::is_same<T, UInt8List>::value || std::is_same<T, UInt16List>::value ||
      std::is_same<T, UInt32List>::value || std::is_same<T, UInt64List>::value;

  static constexpr bool kIsArrayType =
      kIsArrayTypeButNotBitVector || std::is_same<T, BoolList>::value;

  static constexpr bool kIsValue = std::is_same<T, Value>::value;
  static constexpr bool kIsList = std::is_same<T, List>::value;
  static constexpr bool kIsDictionary = std::is_same<T, Dictionary>::value;

  static constexpr bool kIsListPtr =
      std::is_same<T, std::shared_ptr<List>>::value;

  static constexpr bool kIsDictionaryPtr =
      std::is_same<T, std::shared_ptr<Dictionary>>::value;

  static constexpr bool kIsPrimitiveType =
      kIsIntType || kIsStringType || kIsRealType || kIsBooleanType;

  static constexpr bool kIsValueType =
      kIsValue || kIsDictionary || kIsList || kIsListPtr || kIsDictionaryPtr;

  static constexpr bool kValue = kIsValueType || kIsPrimitiveType ||
                                 kIsArrayType || kIsCStringType ||
                                 kIsCStringListType;
};

// ----------------------------------------------------------------
// ValueGet
// ----------------------------------------------------------------

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsBooleanType, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsBoolean()) {
    return v.ToBoolean();
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsIntType, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsInt()) {
    if (IsSignedInteger<T>::kValue) {
      return static_cast<T>(v.ToInt());
    } else {
      // For unsigned integer, we do an extra step check against the value,
      // ie whether they are negative number or not.
      auto original_value = v.ToInt();
      if (original_value < 0) {
        return {};
      } else {
        return static_cast<T>(original_value);
      }
    }
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsStringType, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsString()) {
    return std::string{v.ToString()};
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsCStringType, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsString()) {
    return v.ToString().c_str();
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsRealType, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsReal()) {
    return static_cast<T>(v.ToReal());
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsValue, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  return Value{v};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsList, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsList()) {
    return List{*v.ToListPtr()};
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsDictionary, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsDictionary()) {
    return Dictionary{*v.ToDictionaryPtr()};
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsListPtr, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsList()) {
    return v.ToList();
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsDictionaryPtr, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsDictionary()) {
    return v.ToDictionary();
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<IsConst, char> _1 = 0,
          std::enable_if_t<ValueConvertType<T>::kIsArrayType, int> _2 = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  using ValueType =
      typename ValueGetResult<typename T::value_type, IsConst>::ValueType;

  if (v.IsList()) {
    std::vector<ValueType> return_value;
    auto l = v.ToListPtr();
    return_value.reserve(l->GetSize());
    for (const auto& e : *l) {
      if (auto v = ValueGet<ValueType>(e); v) {
        return_value.push_back(base::Pass(v));
      }
    }
    return std::move(return_value);
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<!IsConst, char> _1 = 0,
          std::enable_if_t<ValueConvertType<T>::kIsArrayType, int> _2 = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  using ValueType =
      typename ValueGetResult<typename T::value_type, IsConst>::ValueType;

  if (v.IsList()) {
    std::vector<ValueType> return_value;
    auto l = v.ToListPtr();
    return_value.reserve(l->GetSize());
    for (auto& e : *l) {
      if (auto v = ValueGet<ValueType>(&e); v) {
        return_value.push_back(base::Pass(v));
      }
    }
    return std::move(return_value);
  }
  return {};
}

template <typename T, typename U,
          bool IsConst = std::is_const<std::remove_reference_t<U>>::value,
          std::enable_if_t<ValueConvertType<T>::kIsCStringListType, int> _ = 0>
ValueGetReturn<T, IsConst> ValueGet(U&& v) {
  if (v.IsList()) {
    auto l = v.ToListPtr();
    std::vector<const char*> return_value;
    return_value.reserve(l->GetSize());

    for (auto& e : *l) {
      if (e.IsString()) {
        return_value.push_back(e.ToString().c_str());
      }
    }
    return std::move(return_value);
  }
  return {};
}

// ----------------------------------------------------------------
// ToValue
// ----------------------------------------------------------------
template <typename T, typename _T = std::decay_t<T>,
          std::enable_if_t<ValueConvertType<_T>::kIsPrimitiveType, int> _ = 0>
std::optional<Value> ValueTo(T&& b) {
  return Value{std::forward<T>(b)};
}

inline std::optional<Value> ValueTo(const char* b) { return Value(b); }

template <typename T, typename _T = std::decay_t<T>,
          std::enable_if_t<ValueConvertType<_T>::kIsValue, int> _ = 0>
std::optional<Value> ValueTo(T&& v) {
  return Value{std::forward<T>(v)};
}

template <typename T, typename _T = std::decay_t<T>,
          std::enable_if_t<ValueConvertType<_T>::kIsList ||
                               ValueConvertType<_T>::kIsDictionary,
                           int>
              _ = 0>
std::optional<Value> ValueTo(T&& v) {
  return Value{std::make_shared<_T>(std::forward<T>(v))};
}

template <typename T, typename _T = std::decay_t<T>,
          std::enable_if_t<ValueConvertType<_T>::kIsListPtr ||
                               ValueConvertType<_T>::kIsDictionaryPtr,
                           int>
              _ = 0>
std::optional<Value> ValueTo(const T& v) {
  return Value{_T{std::forward<T>(v)}};
}

template <
    typename T, typename _T = std::decay_t<T>,
    std::enable_if_t<ValueConvertType<_T>::kIsArrayTypeButNotBitVector, int> _ =
        0>
std::optional<Value> ValueTo(T&& b) {
  auto l = std::make_shared<List>();
  for (const auto& entry : b) {
    if (auto maybe_value = ValueTo(entry); maybe_value) {
      l->Add(base::Pass(maybe_value));
    }
  }
  return Value{std::move(l)};
}

// Specialization of std::vector<bool> which is essentially a bit vector here
inline std::optional<Value> ValueTo(const std::vector<bool>& bitvector) {
  auto l = std::make_shared<List>();
  for (auto entry : bitvector) {
    l->Add(Value{static_cast<bool>(entry)});
  }
  return Value{std::move(l)};
}

template <typename T, typename _T = std::decay_t<T>,
          std::enable_if_t<ValueConvertType<_T>::kIsCStringListType, int> _ = 0>
std::optional<Value> ValueTo(T&& b) {
  auto l = std::make_shared<List>();
  for (const auto& entry : b) {
    l->Add(Value{entry});
  }
  return Value{std::move(l)};
}

}  // namespace _

template <typename T>
_::ValueGetReturn<T, true> Get(const Value& v) {
  static_assert(_::ValueConvertType<T>::kValue, "conversion is not supported");
  return _::ValueGet<T>(v);
}

template <typename T>
_::ValueGetReturn<T, false> Get(Value* v) {
  static_assert(_::ValueConvertType<T>::kValue, "conversion is not supported");
  return _::ValueGet<T>(*v);
}

template <typename T>
std::optional<Value> ToValue(T&& v) {
  using Type = std::decay_t<T>;
  static_assert(_::ValueConvertType<Type>::kValue,
                "conversion is not supported");
  return _::ValueTo(std::forward<T>(v));
}

// Notes, not sure why the template one doesn't work with the const c string.
// Just use the overload here to simplify our life.
inline std::optional<Value> ToValue(const char* data) { return Value{data}; }

template <std::size_t N>
inline std::optional<Value> ToValue(const char* (&array)[N]) {
  auto l = std::make_shared<List>();
  for (std::size_t i = 0; i < N; ++i) {
    l->Add(Value{array[i]});
  }
  return Value{std::move(l)};
}

template <std::size_t N>
inline std::optional<Value> ToValue(const std::array<const char*, N>& arr) {
  auto l = std::make_shared<List>();
  for (const auto& e : arr) {
    l->Add(Value{e});
  }
  return Value{std::move(l)};
}

inline Value ErrorToValue(const Error& e) {
  auto root = std::make_shared<Dictionary>();
  root->Add("category_name", Value{e.category()->GetName()});
  root->Add("reason_code", Value{e.reason()});
  root->Add("information", Value{e.category()->GetInformation(e.reason())});
  root->Add("message", Value{e.GetMessage()});
  return Value{std::move(root)};
}

}  // namespace base

#endif  // _BASE_VALUE_INL_H_
