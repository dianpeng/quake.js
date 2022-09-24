#ifndef _BASE_ENV_H_
#define _BASE_ENV_H_
#include <functional>
#include <optional>
#include <string>

namespace base {

// Does an environment variable lookup. Optionally convert to the type user
// specified. If environment not existed or the conversion cannot be applied,
// the function returns an empty record
template <typename T>
std::optional<T> GetEnv(const char*);

// Used for testing purpose, ie mock.
namespace _ {

using EnvHookFunction = std::optional<std::string> (*)(const char*);
extern EnvHookFunction kEnvHook;

// Set the environment hook function which will be used when doing environment
// lookup.
inline void SetEnvHook(EnvHookFunction f) { kEnvHook = f; }

inline void ResetEnvHook() { SetEnvHook(nullptr); }

}  // namespace _
}  // namespace base

#include "env-inl.h"

#endif  // _BASE_ENV_H_
