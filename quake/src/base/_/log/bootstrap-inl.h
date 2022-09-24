#ifndef _BASE_SRC___BOOTSTRAP_INL_H_
#define _BASE_SRC___BOOTSTRAP_INL_H_

namespace base {
namespace log {
namespace _ {

inline Scanner::Scanner(const std::string& i)
    : token_(kError), lexeme_(), cursor_(0), input_(i) {}

inline int Scanner::Y(std::size_t offset, int token) {
  cursor_ += offset;
  return (token_ = token);
}

inline int Scanner::E(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  lexeme_.text = FmtV(fmt, vl);
  va_end(vl);
  return (token_ = kError);
}

inline bool Scanner::IsIdInitChar(char c) const {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_');
}

inline bool Scanner::IsIdRestChar(char c) const {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
         (c >= '0' && c <= '9') || (c == '_');
}

template <std::size_t kSize>
bool Scanner::MatchKeyword(const char (&t)[kSize]) const {
  for (std::size_t i = 0; i < (kSize - 1); ++i) {
    auto i_cursor = (cursor_ + i + 1);
    if (i_cursor >= input_.size()) {
      return false;
    }
    if (input_[i_cursor] != t[i]) {
      return false;
    }
  }
  if ((cursor_ + kSize) < input_.size()) {
    return !IsIdRestChar(input_[cursor_ + kSize]);
  }
  return true;
}

inline bool Scanner::Check(int t) {
  if (token_ == t) {
    Next();
    return true;
  }
  return false;
}

inline std::string Parser::Parse(Table* t) {
  if (!ParseEntryList(t)) {
    return error_;
  }
  return {};
}

inline std::string Parser::ParseItem(std::string* a, Attributes* b) {
  scanner_.Next();
  if (!ParseEntry(a, b)) {
    return error_;
  }
  return {};
}

template <typename T>
std::optional<T> Parser::Get(const Value& v) {
  return std::optional<T>{};
}

template <>
inline std::optional<std::int64_t> Parser::Get(const Value& v) {
  return v.index() == kInt ? std::get<std::int64_t>(v)
                           : std::optional<std::int64_t>{};
}

template <>
inline std::optional<double> Parser::Get(const Value& v) {
  return v.index() == kReal ? std::get<double>(v) : std::optional<double>{};
}

template <>
inline std::optional<std::string> Parser::Get(const Value& v) {
  return v.index() == kString ? std::get<std::string>(v)
                              : std::optional<std::string>{};
}

template <>
inline std::optional<bool> Parser::Get(const Value& v) {
  return v.index() == kBoolean ? std::get<bool>(v) : std::optional<bool>{};
}

template <>
inline std::optional<std::shared_ptr<Parser::List>> Parser::Get(
    const Value& v) {
  return v.index() == kList ? std::get<std::shared_ptr<Parser::List>>(v)
                            : std::optional<std::shared_ptr<List>>{};
}

template <typename T>
std::optional<T> Parser::Get(const Attributes& a, const char* name) {
  if (auto itr = a.find(name); itr != a.end()) {
    return Get<T>(itr->second);
  }
  return std::optional<T>{};
}

template <typename T>
Parser::Value Parser::Mk(const T& v) {
  if constexpr (std::is_integral<T>::value) {
    return Value{static_cast<std::int64_t>(v)};
  } else if (std::is_floating_point<T>::value) {
    return Value{static_cast<double>(v)};
  } else {
    return Value{v};
  }
}

inline const char* GetSeverityTag(Severity s) {
  switch (s) {
#define __(A, B, ...) \
  case A:             \
    return B;
    BASE_LOG_SEVERITY_LIST(__)
#undef __  // __
    default:
      return "U";
  }
}

inline const char* GetSeverityName(Severity s) {
  switch (s) {
#define __(A, B, C) \
  case A:           \
    return C;
    BASE_LOG_SEVERITY_LIST(__)
#undef __  // __
    default:
      return "<unknown>";
  }
}

}  // namespace _
}  // namespace log
}  // namespace base

#endif  // _BASE_SRC___BOOTSTRAP_INL_H_
