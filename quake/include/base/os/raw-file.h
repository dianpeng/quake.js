#ifndef _BASE_OS_RAW_FILE_H_
#define _BASE_OS_RAW_FILE_H_
#include <base/check.h>
#include <base/common.h>
#include <base/error.h>

#if defined(OS_POSIX)
#include "raw-file-posix.h"
#endif  // OS_POSIX

namespace base {
namespace os {

class RawFile {
 public:
  enum Flag : int {
    kAppend = 1,
    kCreate = 2,
    kRead = 4,
    kWrite = 8,
    kSecurity = 16
  };
  enum Permission : int {
    kPermissionRead = 1,
    kPermissionWrite = 2,
    kPermissionDefault = kPermissionRead | kPermissionWrite
  };

 public:
  RawFile() : impl_() {}
  ~RawFile() {}

  inline Error Open(const std::string&, Flag,
                    Permission mode = kPermissionDefault);

  inline Result<std::size_t> Read(void*, std::size_t);
  inline Result<std::size_t> Write(const void*, std::size_t);

  void Close() { return impl_.Close(); }
  Error Fsync() { return impl_.Fsync(); }
  bool IsOpen() const { return impl_.IsOpen(); }
  const std::string& filename() const { return impl_.filename(); }
  // Release the underlying. Notes afterwards the RawFile cannot be used unless
  // call |Open| again
  unique_fd ReleaseFd() { return impl_.ReleaseFd(); }

 private:
#if defined(OS_POSIX)
  _::RawFilePosix impl_;
#else
#error "not implemented"
#endif  // OS_POSIX

  DISALLOW_COPY_AND_ASSIGN(RawFile);
};

inline Error RawFile::Open(const std::string& path, Flag flag,
                           Permission mode) {
  return impl_.Open(path, flag, mode);
}

inline Result<std::size_t> RawFile::Read(void* buffer,
                                         std::size_t buffer_size) {
  return impl_.Read(buffer, buffer_size);
}

inline Result<std::size_t> RawFile::Write(const void* buffer,
                                          std::size_t buffer_size) {
  return impl_.Write(buffer, buffer_size);
}

}  // namespace os
}  // namespace base

#endif  // _BASE_OS_RAW_FILE_H_
