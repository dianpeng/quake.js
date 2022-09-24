#ifndef _BASE_LOG_H_
#define _BASE_LOG_H_
#include <sstream>

#include "common.h"

namespace base {
extern std::string CONFIG_base_log;

namespace log {
namespace _ {
struct NullStream {};
}  // namespace _

#define BASE_LOG_SEVERITY_LIST(__) \
  __(kInfo, "I", "info")          \
  __(kWarning, "W", "warning")    \
  __(kError, "E", "error")        \
  __(kFatal, "F", "fatal")        \
  __(kBug, "B", "bug")            \
  __(kTrace, "T", "trace")        \
  __(kRaw, "-", "raw")

// clang-format off
// Severity of the log system.
enum Severity {
#define __(A, ...) A,
  BASE_LOG_SEVERITY_LIST(__)
#undef __  // __
  kSizeOfSeverity
};

inline std::optional<Severity> ParseSeverity(const std::string_view&);
// clang-format on

// Log rotation
void Rotate();

// Flush
void Flush();

// Fork notification
void BeforeFork();
void AfterFork();

// These 2 functions are mostly used for testing purpose.
//   SetGlobalLogSink temporarily hook the log sink with user provided function
//   and all the formatted information will be handleded into the sink |along
//   with| other existed sink. Notes, the function doesn't have to be thread
//   safe, the function will only be invoked when global log lock is held.
//
//   ResetGlobalLogSink will nullify the user's provided sink
using GlobalLogSinkFunction =
    std::function<void(Severity, const std::string&, bool, bool)>;
void SetGlobalLogSink(GlobalLogSinkFunction);
void ResetGlobalLogSink();

// Get the verbose level of the |this thread|
std::uint32_t GetVerboseLevelCurrentThread();
void SetVerboseLevelCurrentThread(std::uint32_t);

// Used to temporarily change the verbose level of current thread. The verbose
// log inside of the scope will be impacted.
class VerboseScope {
 public:
  explicit VerboseScope(std::uint32_t v)
      : prev_(GetVerboseLevelCurrentThread()) {
    SetVerboseLevelCurrentThread(v);
  }

  ~VerboseScope() { SetVerboseLevelCurrentThread(prev_); }

 private:
  std::uint32_t prev_;
  DISALLOW_COPY_AND_ASSIGN(VerboseScope);
};

// LogMessage object. Pretty much represent a single message been used to dump
// the log message out. A log |macro| will genrate a temporary LogMessage object
// and its destructor will try to flush the log message out.
class LogMessage {
 public:
  inline LogMessage(const char*, int, const char*, Severity);
  ~LogMessage();

  std::ostringstream& stream() { return stream_; }

 private:
  const char* file_;
  int line_;
  const char* tag_;
  Severity severity_;
  std::ostringstream stream_;

  DISALLOW_COPY_AND_ASSIGN(LogMessage);
};

class VLogMessage {
 public:
  inline VLogMessage(const char*, int, const char*, Severity, std::uint32_t v);
  ~VLogMessage();
  std::ostringstream& stream() { return stream_; }

 private:
  const char* file_;
  int line_;
  std::uint32_t verbose_;
  const char* tag_;
  Severity severity_;
  std::ostringstream stream_;

  DISALLOW_COPY_AND_ASSIGN(VLogMessage);
};

// NullMessage. This object is used to nullify log message in case user use
// DLOG interface.
class NullMessage {
 public:
  NullMessage() = default;
  _::NullStream& stream() { return stream_; }

 private:
  _::NullStream stream_;

  DISALLOW_COPY_AND_ASSIGN(NullMessage);
};

}  // namespace log
}  // namespace base

#include "log-inl.h"

#endif  // _BASE_LOG_H_
