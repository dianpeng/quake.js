#include <base/os/raw-file-posix.h>
#include <base/os/raw-file.h>

#ifdef OS_POSIX

namespace base {
namespace os {
namespace _ {

Error RawFilePosix::Open(const std::string& path, Flag flag, Permission perm) {
  if (fd_) {
    return Err(kErrorResourceAlreadyExisted, "%s already opened",
               filename_.c_str());
  }

  filename_ = path;
  cursor_ = 0;

  {
    posix_flag_ = 0;
    if (flag & RawFile::kAppend) posix_flag_ |= O_APPEND;
    if (flag & RawFile::kCreate) posix_flag_ |= O_CREAT;
    if ((flag & RawFile::kRead) && (flag & RawFile::kWrite))
      posix_flag_ |= O_RDWR;
    else if ((flag & RawFile::kRead))
      posix_flag_ |= O_RDONLY;
    else if ((flag & RawFile::kWrite))
      posix_flag_ |= O_WRONLY;
    if (flag & RawFile::kSecurity) posix_flag_ |= O_CLOEXEC;
  }

  {
    posix_permission_ = 0;
    if (perm & RawFile::kPermissionRead) posix_permission_ |= S_IRUSR;
    if (perm & RawFile::kPermissionWrite) posix_permission_ |= S_IWUSR;
  }

  if (auto fd = POSIX_SYSCALL(
          open(filename_.c_str(), posix_flag_, posix_permission_));
      fd >= 0) {
    fd_.reset(fd);
    return Error{};
  } else {
    return ErrSys("open", errno);
  }
}

}  // namespace _
}  // namespace os
}  // namespace base

#endif  // OS_POSIX
