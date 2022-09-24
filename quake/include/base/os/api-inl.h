#ifndef _BASE_OS_API_INL_H_
#define _BASE_OS_API_INL_H_
#include <base/log.h>

#if defined(OS_POSIX)
#include <unistd.h>
#endif  // OS_POSIX

namespace base {
namespace os {

inline void CloseFd(fd_t fd) {
#if defined(OS_POSIX)
  if (fd == kInvalidFd) {
    return;
  }

  // Notes(dpeng): The close can fail even if you have no way to recover from it
  //   and a close can fail with EINTR, but the fd is actually been closed. The
  //   underlying pointed resource may not be closed cleanly, which is due to
  //   the subsystem or fs issue. There's really no otherway to know the exact
  //   issue underlying. So close we choose to not report anything. A close fail
  //   may or may not break anything, but we have no choice but crash us which
  //   is not accepetable.
  if (::close(fd)) {
    LOG(WARNING) << "close failed: " << ErrString();
  }
#else
#endif  // OS_POSIX
}

// This is the no-bullshit version, ie dont' report anything
inline void CloseFdNoBS(fd_t fd) {
#if defined(OS_POSIX)
  if (fd == kInvalidFd) {
    return;
  }
  UNUSE(::close(fd));
#else
#endif  // OS_POSIX
}

}  // namespace os
}  // namespace base

#endif  // _BASE_OS_API_INL_H_
