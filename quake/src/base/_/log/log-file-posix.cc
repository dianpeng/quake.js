#include "log-file-posix.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>

#include "bootstrap.h"

namespace base {
namespace log {
namespace _ {

#define CHECK(EXP)                      \
  do {                                  \
    if (!(EXP)) {                       \
      UFATAL("CHECK failed: %s", #EXP); \
    }                                   \
  } while (false)

int LogFilePosix::CreateLogFile() {
  // The smallest and safetest bet we can have
  auto flags = O_WRONLY | O_CREAT | O_CLOEXEC | O_APPEND;
  auto perm = 0664;
  return POSIX_SYSCALL(::open(filename_.c_str(), flags, perm));
}

bool LogFilePosix::Init() {
  CHECK(fd_ == kInvalid);
  int fd;
  if (fd = CreateLogFile(); fd < 0) {
    return false;
  }
  fd_ = fd;
  return true;
}

void LogFilePosix::Rotate() {
  Close();

  // Reopen the file, if we cannot then just crash here. Currently, this is
  // not modifable.
  if (auto new_fd = CreateLogFile(); new_fd >= 0) {
    fd_ = new_fd;
  } else {
    // Cannot create log file for rotation. What should we do ? Urgently
    // report the error out and then just fucked up from now on ?
    // Notes:
    //   We don't use std::strerror to convert a errno into a string because
    //   std::strerror is not thread safe.
    UFATAL("cannot create new log file after rotation(errno: %d)", errno);
  }
}

void LogFilePosix::WriteFlush(const std::string& buffer) {
  CHECK(fd_ >= 0);

  // TODO(dpeng): writev ?
  auto total_size = buffer.size() + buffer_.size();

  if (total_size <= kBufferSize) {
    char buf[kBufferSize];
    std::memcpy(buf, buffer_.data(), buffer_.size());
    std::memcpy(buf + buffer_.size(), buffer.data(), buffer.size());
    UNUSE(POSIX_SYSCALL(::write(fd_, buf, total_size)));
  } else {
    UNUSE(POSIX_SYSCALL(::write(fd_, buffer_.data(), buffer_.size())));
    UNUSE(POSIX_SYSCALL(::write(fd_, buffer.data(), buffer.size())));
  }

  buffer_.clear();
}

bool LogFilePosix::MaybeBuffer(const std::string& b) {
  if (buffer_.size() + b.size() <= kBufferSize) {
    buffer_.append(b);
    return true;
  }
  return false;
}

void LogFilePosix::Flush() {
  if (fd_ >= 0) {
    UNUSE(POSIX_SYSCALL(::write(fd_, buffer_.data(), buffer_.size())));
    buffer_.clear();
  }
}

void LogFilePosix::EnsureBuffer(const std::string& b) {
  auto tt_size = buffer_.size() + b.size();
  if (tt_size > kBufferSize) {
    CHECK(buffer_.size() <= kBufferSize);
    auto space = (kBufferSize - buffer_.size());
    if (space) {
      buffer_.append(b.data(), space);
    }
  } else {
    buffer_.append(b);
  }
  CHECK(buffer_.size() <= kBufferSize);
}

void LogFilePosix::Write(const std::string& buffer, bool flush) {
  if (flush) {
    WriteFlush(std::move(buffer));
  } else {
    if (!MaybeBuffer(buffer)) {
      Flush();
      EnsureBuffer(buffer);
    }
  }
}

void LogFilePosix::Close() {
  Flush();

  UNUSE(::close(fd_));
  fd_ = kInvalid;
}

}  // namespace _
}  // namespace log
}  // namespace base
