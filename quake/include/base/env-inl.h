#ifndef _BASE_ENV_INL_H_
#define _BASE_ENV_INL_H_
#include <boost/lexical_cast.hpp>
#include <cstdlib>

namespace base {

template <>
inline std::optional<std::string> GetEnv(const char* name) {
  if (_::kEnvHook) {
    return _::kEnvHook(name);
  } else {
    if (auto v = std::getenv(name); v) {
      return std::optional<std::string>{std::string{v}};
    } else {
      return std::optional<std::string>{};
    }
  }
}

template <typename T>
inline std::optional<T> GetEnv(const char* name) {
  if (auto v = GetEnv<std::string>(name); v) {
    T r;
    if (boost::conversion::try_lexical_convert(v.value(), r)) {
      return std::optional<T>{std::move(r)};
    }
  }
  return std::optional<T>{};
}

}  // namespace base

#endif  // _BASE_ENV_INL_H_
