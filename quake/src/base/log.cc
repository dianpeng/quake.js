#include <base/log.h>

#include "_/log/logger.h"

namespace base {
namespace log {

LogMessage::~LogMessage() {
  _::GetLogger()->Record(file_, line_, tag_, severity_, stream_.str());
}

VLogMessage::~VLogMessage() {
  if (verbose_ <= GetVerboseLevelCurrentThread()) {
    _::GetLogger()->Record(file_, line_, tag_, severity_, stream_.str());
  }
}

std::uint32_t GetVerboseLevelCurrentThread() {
  return _::GetVerboseLevelCurrentThread();
}

void SetVerboseLevelCurrentThread(std::uint32_t v) {
  _::SetVerboseLevelCurrentThread(v);
}

void Rotate() { _::GetLogger()->sink_list.Rotate(); }

void Flush() { _::GetLogger()->sink_list.Flush(); }

void BeforeFork() { _::GetLogger()->sink_list.BeforeFork(); }

void AfterFork() { _::GetLogger()->sink_list.AfterFork(); }

void SetGlobalLogSink(GlobalLogSinkFunction f) {
  _::GetLogger()->user_sink = f;
}

void ResetGlobalLogSink() { _::GetLogger()->user_sink = nullptr; }

}  // namespace log
}  // namespace base
