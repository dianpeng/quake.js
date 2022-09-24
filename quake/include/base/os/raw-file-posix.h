#ifndef _BASE_OS_RAW_FILE_POSIX_H_
#define _BASE_OS_RAW_FILE_POSIX_H_
// Until we make all the arch/platform flags available in build script, we still
// have to include this file to let the platform detection code kicks in :(
#include <base/common.h>

#if defined(OS_POSIX)

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <base/check.h>
#include <base/result.h>

#include "basic.h"
#include "unique-fd.h"

namespace base {
namespace os {
namespace _ {

class RawFilePosix {
 public:
  using Permission = int;
  using Flag = int;

  inline RawFilePosix();
  RawFilePosix(RawFilePosix&&) = default;
  RawFilePosix& operator=(RawFilePosix&&) = default;

  // Implementation depend on RawFile's internal declaration
  Error Open(const std::string&, Flag, Permission);
  inline Result<std::size_t> Read(void*, std::size_t);
  inline Result<std::size_t> Write(const void*, std::size_t);
  void Close() { fd_.clear(); }
  inline Error Fsync();
  bool IsOpen() const { return fd_.has(); }
  const std::string& filename() const { return filename_; }

  unique_fd ReleaseFd() {
    auto t = std::move(fd_);
    return t;
  }

 private:
  std::string filename_;
  unique_fd fd_;
  int posix_flag_;
  int posix_permission_;
  off_t cursor_;
  DISALLOW_COPY_AND_ASSIGN(RawFilePosix);
};

inline Error RawFilePosix::Fsync() {
  if (!fd_) {
    return Err(kErrorResourceNotExisted, "file not open");
  }
  if (auto e = POSIX_SYSCALL(fsync(fd_.value())); e) {
    return ErrSys("fsync", errno);
  }
  return Error{};
}

inline RawFilePosix::RawFilePosix()
    : filename_(), fd_(), posix_flag_(0), posix_permission_(0), cursor_(0) {}

inline Result<std::size_t> RawFilePosix::Read(void* buffer,
                                              std::size_t buffer_size) {
  if (!fd_) {
    return Err(kErrorResourceNotExisted, "file not open");
  }
  if (auto ret =
          POSIX_SYSCALL(pread(fd_.value(), buffer, buffer_size, cursor_));
      ret >= 0) {
    cursor_ += ret;
    return static_cast<std::size_t>(ret);
  } else {
    return ErrSys("pread", errno);
  }
}

inline Result<std::size_t> RawFilePosix::Write(const void* buffer,
                                               std::size_t buffer_size) {
  if (!fd_) {
    return Err(kErrorResourceNotExisted, "file not open");
  }
  if (auto ret = POSIX_SYSCALL(write(fd_.value(), buffer, buffer_size));
      ret >= 0) {
    cursor_ += ret;
    return static_cast<std::size_t>(ret);
  } else {
    return ErrSys("write", errno);
  }
}

}  // namespace _
}  // namespace os
}  // namespace base

#endif  // OS_POSIX
#endif  // _BASE_OS_RAW_FILE_POSIX_H_
