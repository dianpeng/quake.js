#ifndef _BASE_OS_UNIQUE_FD_H_
#define _BASE_OS_UNIQUE_FD_H_
#include "basic.h"
#include "api.h"

namespace base {
namespace os {
namespace _ {

struct FdCloser {
  static void Close(fd_t f) { CloseFd(f); }
};

// RAII for file descriptor. Mostly you are not supposed to even use file
// descriptor directly but prefer using File/Socket/... wrapper object
template <typename Closer = FdCloser>
class unique_fd_impl final {
 public:
  unique_fd_impl() : value_(kInvalidFd) {}
  explicit unique_fd_impl(fd_t value) : value_(value) {}
  ~unique_fd_impl() { clear(); }

  unique_fd_impl(unique_fd_impl&& other) : value_(other.release()) {}
  unique_fd_impl& operator=(unique_fd_impl&& s) {
    reset(s.release());
    return *this;
  }
  void reset(fd_t new_fd = kInvalidFd) {
    if (value_ != kInvalidFd) {
      Closer::Close(value_);
    }
    value_ = new_fd;
  }
  void reset_no_empty(fd_t new_fd) {
    CHECK(new_fd != kInvalidFd);
    reset(new_fd);
  }
  int value() const { return value_; }
  int value_or_crash() const {
    CHECK(value_ != kInvalidFd);
    return value_;
  }

  [[nodiscard]] int release() {
    int ret = value_;
    value_ = kInvalidFd;
    return ret;
  }

  bool has() const { return value_ != kInvalidFd; }
  operator bool() const { return has(); }

  void clear() { reset(kInvalidFd); }

 private:
  fd_t value_;
  DISALLOW_COPY_AND_ASSIGN(unique_fd_impl);
};

}  // namespace _

// exported type
using unique_fd = _::unique_fd_impl<_::FdCloser>;

}  // namespace os
}  // namespace base

#endif  // _BASE_OS_UNIQUE_FD_H_
