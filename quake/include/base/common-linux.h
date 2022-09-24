#ifndef _BASE_COMMON_LINUX_H_
#define _BASE_COMMON_LINUX_H_

namespace base {
namespace _ {
template <typename R, typename T>
R PosixSyscallHandler(T&& h) {
  while (true) {
    errno = 0;
    auto v = h();
    if (errno == EINTR) {
      continue;
    } else {
      return v;
    }
  }
}
}  // namespace _
}  // namespace base

// This macro simplify our job for correctly invoke Posix syscall. Most of the
// syscall requires set errno to 0; then invoke it; and check errno with EINTR
// to re-enter into syscall again. This macro does this for you.
//   example :
//
//   POSIX_SYSCALL(::poll(...));
//
// If poll is interruped with EINTR, the POSIX_SYSCALL macro will automatically
// re-enter the poll function
#define POSIX_SYSCALL(...)                              \
  ::base::_::PosixSyscallHandler<decltype(__VA_ARGS__)>( \
      [&]() { return (__VA_ARGS__); })

#endif  // _BASE_COMMON_LINUX_H_
