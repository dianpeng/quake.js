#ifndef _BASE_OS_BASIC_H_
#define _BASE_OS_BASIC_H_
#include <base/check.h>
#include <base/common.h>
#include <base/error.h>
#include <base/result.h>

namespace base {
namespace os {

#if defined(OS_POSIX)
using fd_t = int;
static constexpr fd_t kInvalidFd = -1;
#else
#error "unsupported OS"
#endif  // OS_POSIX

#define OS_ERROR_CODE_LIST(__)                           \
  __(kErrorThread, "thread error")                       \
  __(kErrorInvalidInput, "input is invalid")             \
  __(kErrorNotImplemented, "the api is not implemented") \
  __(kErrorSyscall, "syscall error")                     \
  __(kErrorResourceNotExisted, "resource not available") \
  __(kErrorResourceAlreadyExisted, "resource already existed")

// clang-format off
enum {

#define __(A, B) A,
  OS_ERROR_CODE_LIST(__)
#undef __  // __

  kSizeOfOSError
};
// clang-format on

// General error reporting
Error Err(std::uint32_t);
Error ErrV(std::uint32_t, const char*, va_list);

inline Error Err(std::uint32_t c, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto e = ErrV(c, fmt, vl);
  va_end(vl);
  return e;
}

// Report an API is not available or not implemented on the current ARCHs
inline Error ErrNA(const char* api) {
  return Err(kErrorNotImplemented, "api: %s", api);
}

// Report a |syscall| error, ie a syscall with errno not 0. Notes, errno can
// be set by userspace API as well, if so either use Error::MkSys or wrap your
// own category
inline Error ErrSys(const char* api, int err) {
  return Err(kErrorSyscall, "[errno: %d, %s]: %s", err, api,
             ErrString(err).c_str());
}

// get the nameserver field from resolve.conf or similar configuration file
void ParseNameServerFromResolveConf(std::vector<std::string>*,
                                    const std::string&);

}  // namespace os
}  // namespace base

#endif  // _BASE_OS_BASIC_H_
