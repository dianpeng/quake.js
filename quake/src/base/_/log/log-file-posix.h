#ifndef _BASE_SRC___LOG_LOG_FILE_POSIX_H_
#define _BASE_SRC___LOG_LOG_FILE_POSIX_H_
#include <base/common.h>

// LogFilePosix for Poisx OS. This file does have some assumption :
//
//   1) On Linux, a write with less than 4KB will be gauranteed to be atomic
//      We are multiple processes maybe, so atomic write of log file is critical
//
//   2) A typical rotation is provided. The log rotation is really just a simple
//      reopen operation. The external process, ie logrotate, will first move
//      the current working log into a different name and then signal the
//      process to rotate its log, ie reopen, because the reopen will just use
//      the old name, so a new file is actually been created. The old log file,
//      under a different name, will not be used anymore.
//
// And 4kb effectively means no single log entry can be larger than this number
// In most cases, we should not generate log that is larger than 4KB

namespace base {
namespace log {
namespace _ {

static const std::size_t kBufferSize = 4096;

class LogFilePosix {
 public:
  // We a log is rotating, its fd_ field's value will be temporarily stored as
  // kBusy value. Anyother waiter will needs to enter a busy spinning loop until
  // the file has been properly rotated.
  static const int kInvalid = -1;

 public:
  explicit LogFilePosix(const std::string& f)
      : filename_(f), buffer_(), fd_(kInvalid) {}

  ~LogFilePosix() { Close(); }

 public:
  // Double initialization is needed
  bool Init();

  void Rotate();
  void Write(const std::string&, bool);
  void Flush();
  void Close();

  const std::string& filename() const { return filename_; }
  const std::string& buffer() const { return buffer_; }

 private:
  void WriteFlush(const std::string&);
  bool MaybeBuffer(const std::string&);
  void EnsureBuffer(const std::string&);

  int CreateLogFile();

  // The log filename. Cannot be mutated after initialized.
  const std::string filename_;

  // Temporary buffer, used to hold the internal data when dump out. Will never
  // be larger than kBufferSize
  std::string buffer_;

  // The file that's currently been used for serialization purpose. Using this
  // fd is actually using a atomic to guard with. No need to hold the mutex. The
  // mutex is sololy been used to guard the buffer object.
  int fd_;

  DISALLOW_COPY_AND_ASSIGN(LogFilePosix);
};

}  // namespace _
}  // namespace log
}  // namespace base

#endif  // _BASE_SRC___LOG_LOG_FILE_POSIX_H_
