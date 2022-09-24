#ifndef _SRC_BASE___LOG_LOGGER_H_
#define _SRC_BASE___LOG_LOGGER_H_
#include <base/log.h>

#include <mutex>
#include <shared_mutex>

#include "bootstrap.h"
#include "formatter.h"
#include "sink.h"

namespace base {
namespace log {
namespace _ {

// SinkList is per |log| object globally. Currently we don't have plan to let
// each severity has its own sink. The SinkList is a thread safe object. Before
// entering into API of each sink the SinkList will obtain the lock.
class SinkList {
 public:
  SinkList() = default;

  void Init(const Parser::Attributes&);
  void InitDefault();

 public:
  void Rotate();
  void Flush();
  void Write(Severity, std::string&&, bool);
  void BeforeFork();
  void AfterFork();

 public:
  std::size_t GetSinkListSize() const { return sink_list_.size(); }
  bool IsEmpty() const { return sink_list_.empty(); }
  std::shared_ptr<Sink> IndexSink(std::size_t i) { return sink_list_[i]; }

 private:
  std::mutex mutex_;
  std::vector<std::shared_ptr<Sink>> sink_list_;

  DISALLOW_COPY_AND_ASSIGN(SinkList);
};

struct LogEntry {
  std::unique_ptr<Formatter> formatter;
  bool stacktrace;
  bool flush;
  bool terminate;

  struct Option {
    bool stacktrace, flush, terminate;
    Option(bool a, bool b, bool c) : stacktrace(a), flush(b), terminate(c) {}
  };

  void Set(const Option& o) {
    stacktrace = o.stacktrace;
    flush = o.flush;
    terminate = o.terminate;
  }

 public:
  LogEntry() : formatter(), stacktrace(false), flush(false), terminate(false) {}

  LogEntry(const Option& o)
      : formatter(Formatter::CreateDefault()),
        stacktrace(o.stacktrace),
        flush(o.flush),
        terminate(o.terminate) {}
};

struct Logger {
  static const std::size_t kEntrySize =
      static_cast<std::size_t>(kSizeOfSeverity);

  LogEntry entry[kEntrySize];
  SinkList sink_list;
  GlobalLogSinkFunction user_sink{nullptr};
  std::uint32_t verbose{0};

  // The initialization of the whole process log starts here
  Logger();

 public:
  // Main method to record on log information into global log strore.
  void Record(const char*, int, const char*, Severity, std::string&&);

 public:
  static int MapSeverityFromName(const std::string&);
  static void Populate(const Parser::Attributes&, LogEntry*, int);

 private:
  void Terminate();

  DISALLOW_COPY_AND_ASSIGN(Logger);
};

void SetVerboseLevelCurrentThread(std::uint32_t);
std::uint32_t GetVerboseLevelCurrentThread();

// Get the default global logger object. This is the main log interfaces used
// for logging throughout the code base. Notes, log itself is thread safe, so
// no need to have multiple logs in a process unless otherwise needed.
Logger* GetLogger();

}  // namespace _
}  // namespace log
}  // namespace base

#endif  // _SRC_BASE___LOG_LOGGER_H_
