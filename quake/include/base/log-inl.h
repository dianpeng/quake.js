#ifndef _BASE_LOG_INL_H_
#define _BASE_LOG_INL_H_

namespace base {
namespace log {

inline LogMessage::LogMessage(const char* file, int line, const char* tag,
                              Severity s)
    : file_(file), line_(line), tag_(tag), severity_(s), stream_() {}

inline VLogMessage::VLogMessage(const char* file, int line, const char* tag,
                                Severity s, std::uint32_t verbose)
    : file_(file),
      line_(line),
      verbose_(verbose),
      tag_(tag),
      severity_(s),
      stream_() {}

namespace _ {

struct LogMessageVoidify {
  void operator&(std::ostream&) {}
};

template <typename T>
::base::log::_::NullStream& operator<<(::base::log::_::NullStream& s,
                                      const T& v) {
  UNUSE(v);
  return s;
}

}  // namespace _

inline std::optional<Severity> ParseSeverity(const std::string_view& severity) {
#define __(A, B, C) \
  if (severity == (C)) return A;
  BASE_LOG_SEVERITY_LIST(__)
#undef __  // __
  return {};
}

}  // namespace log
}  // namespace base

/* ---------------------------------------------------------------------
 * ---------------------------------------------------------------------
 *   Private Macros Definitions, use interfaces Macro
 * ---------------------------------------------------------------------
 * -------------------------------------------------------------------*/

// Normal log
#define _LOG_IMPL_INFO()                                                 \
  ::base::log::LogMessage(__FILE__, __LINE__, nullptr, ::base::log::kInfo) \
      .stream()

#define _LOG_IMPL_WARNING()                                                 \
  ::base::log::LogMessage(__FILE__, __LINE__, nullptr, ::base::log::kWarning) \
      .stream()

#define _LOG_IMPL_ERROR()                                                 \
  ::base::log::LogMessage(__FILE__, __LINE__, nullptr, ::base::log::kError) \
      .stream()

#define _LOG_IMPL_FATAL()                                                 \
  ::base::log::LogMessage(__FILE__, __LINE__, nullptr, ::base::log::kFatal) \
      .stream()

#define _LOG_IMPL_TRACE()                                                 \
  ::base::log::LogMessage(__FILE__, __LINE__, nullptr, ::base::log::kTrace) \
      .stream()

#define _LOG_IMPL_BUG() \
  ::base::log::LogMessage(__FILE__, __LINE__, nullptr, ::base::log::kBug).stream()

#define _LOG_IMPL_RAW() \
  ::base::log::LogMessage(__FILE__, __LINE__, nullptr, ::base::log::kRaw).stream()

#define _TLOG_IMPL_INFO(T) \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kInfo).stream()

#define _TLOG_IMPL_WARNING(T) \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kWarning).stream()

#define _TLOG_IMPL_ERROR(T) \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kError).stream()

#define _TLOG_IMPL_FATAL(T) \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kFatal).stream()

#define _TLOG_IMPL_TRACE(T) \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kTrace).stream()

#define _TLOG_IMPL_BUG(T) \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kBug).stream()

#define _TLOG_IMPL_RAW(T) \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kRaw).stream()

// Debug log, ie can be trunned off via build switch
//   BASE_LOG_DLOG_OFF
#if defined(BASE_LOG_DLOG_OFF)
#define _DLOG_IMPL_INFO() NullMessage().stream()
#define _DLOG_IMPL_WARNING() NullMessage().stream()
#define _DLOG_IMPL_ERROR() NullMessage().stream()
#define _DLOG_IMPL_FATAL() NullMessage().stream()
#define _DLOG_IMPL_TRACE() NullMessage().stream()
#define _DLOG_IMPL_BUG() NullMessage().stream()
#define _DLOG_IMPL_RAW() NullMessage().stream()

#define _DTLOG_IMPL_INFO(...) NullMessage().stream()
#define _DTLOG_IMPL_WARNING(...) NullMessage().stream()
#define _DTLOG_IMPL_ERROR(...) NullMessage().stream()
#define _DTLOG_IMPL_FATAL(...) NullMessage().stream()
#define _DTLOG_IMPL_TRACE(...) NullMessage().stream()
#define _DTLOG_IMPL_BUG(...) NullMessage().stream()
#define _DTLOG_IMPL_RAW(...) NullMessage().stream()
#else
#define _DLOG_IMPL_INFO() _LOG_IMPL_INFO()
#define _DLOG_IMPL_WARNING() _LOG_IMPL_WARNING()
#define _DLOG_IMPL_ERROR() _LOG_IMPL_ERROR()
#define _DLOG_IMPL_FATAL() _LOG_IMPL_FATAL()
#define _DLOG_IMPL_TRACE() _LOG_IMPL_TRACE()
#define _DLOG_IMPL_BUG() _LOG_IMPL_BUG()
#define _DLOG_IMPL_RAW() _LOG_IMPL_BUG()

#define _DTLOG_IMPL_INFO(...) _TLOG_IMPL_INFO(__VA_ARGS__)
#define _DTLOG_IMPL_WARNING(...) _TLOG_IMPL_WARNING(__VA_ARGS__)
#define _DTLOG_IMPL_ERROR(...) _TLOG_IMPL_ERROR(__VA_ARGS__)
#define _DTLOG_IMPL_FATAL(...) _TLOG_IMPL_FATAL(__VA_ARGS__)
#define _DTLOG_IMPL_TRACE(...) _TLOG_IMPL_TRACE(__VA_ARGS__)
#define _DTLOG_IMPL_BUG(...) _TLOG_IMPL_BUG(__VA_ARGS__)
#define _DTLOG_IMPL_RAW(...) _TLOG_IMPL_RAW(__VA_ARGS__)
#endif  // BASE_LOG_DLOG_OFF

// Verbose log
#define _VLOG_IMPL_INFO(V)                                                     \
  ::base::log::VLogMessage(__FILE__, __LINE__, nullptr, ::base::log::kInfo, (V)) \
      .stream()

#define _VLOG_IMPL_WARNING(V)                                                \
  ::base::log::VLogMessage(__FILE__, __LINE__, nullptr, ::base::log::kWarning, \
                          (V))                                               \
      .stream()

#define _VLOG_IMPL_ERROR(V)                                                \
  ::base::log::VLogMessage(__FILE__, __LINE__, nullptr, ::base::log::kError, \
                          (V))                                             \
      .stream()

#define _VLOG_IMPL_FATAL(V)                                                \
  ::base::log::VLogMessage(__FILE__, __LINE__, nullptr, ::base::log::kFatal, \
                          (V))                                             \
      .stream()

#define _VLOG_IMPL_TRACE(V)                                                \
  ::base::log::VLogMessage(__FILE__, __LINE__, nullptr, ::base::log::kTrace, \
                          (V))                                             \
      .stream()

#define _VLOG_IMPL_BUG(V)                                                     \
  ::base::log::VLogMessage(__FILE__, __LINE__, nullptr, ::base::log::kBug, (V)) \
      .stream()

#define _VLOG_IMPL_RAW(V)                                                     \
  ::base::log::VLogMessage(__FILE__, __LINE__, nullptr, ::base::log::kRaw, (V)) \
      .stream()

#define _VTLOG_IMPL_INFO(V, T)                                            \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kInfo, (V)) \
      .stream()

#define _VTLOG_IMPL_WARNING(V, T)                                            \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kWarning, (V)) \
      .stream()

#define _VTLOG_IMPL_ERROR(V, T)                                            \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kError, (V)) \
      .stream()

#define _VTLOG_IMPL_FATAL(V, T)                                            \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kFatal, (V)) \
      .stream()

#define _VTLOG_IMPL_TRACE(V, T)                                            \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kTrace, (V)) \
      .stream()

#define _VTLOG_IMPL_BUG(V, T)                                            \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kBug, (V)) \
      .stream()

#define _VTLOG_IMPL_RAW(V, T)                                            \
  ::base::log::LogMessage(__FILE__, __LINE__, (T), ::base::log::kRaw, (V)) \
      .stream()

// ----------------------------------------------------------------
// Macro interfaces
//   The log exposes nearly all its functionality via MACROS and
//   it renders a stream like interface for user to use log.
//
// In terms of whether the log will be built into final binary, we have
// 2 types:
//
//   1. DEBUG log, which always prefix with D
//   2. NORMAL log, which will always log out
//
// For functionality, 2 types of log is also provided, which allow user to
// insert a tag, which has a dedicated place in formatter:
//
//   1. TLOG(tag), the tag you put there must be a string literal
//
// Lastly, verbose log is also provided, which prefixed with V. The verbose
// level is a runtime value which can be configured to control whether the
// log finally been sinked out or not.

// example:
//   LOG(INFO) << "your info goes here"
#define LOG(S) CONCATE(_LOG_IMPL_, S)()

// example:
//   TLOG("my tag", INFO) << "your tagged info";
#define TLOG(T, S) CONCATE(_TLOG_IMPL_, S)(T)

// example:
//   VLOG(2, INFO) << "this log shows out when verbose level is larger than 2";
#define VLOG(V, S) CONCATE(_VLOG_IMPL_, S)(V)

// example:
//   VTLOG(2, "tag", INFO) << "this tagged log shows out when verbose level "
//     << "is larger than 2";
#define VTLOG(V, T, S) CONCATE(_VTLOG_IMPL_, S)(V, T)

// example:
//   DLOG(INFO) << "log";
// if -DBASE_LOG_DLOG_OFF is on, then the above line result in no-op
#define DLOG(S) CONCATE(_DLOG_IMPL_, S)()

// example:
//   DTLOG("tag", INFO) << "...";
// similar as above, except it is a tagged log
#define DTLOG(T, S) CONCATE(_DTLOG_IMPL_, S)(T)

// Conditional logging ---------------------------------------------
//  Examples:
//   LOG_IF(INFO, condition) << "Yeah output when condition is true";

#define _COND_LOG(L, COND, ...) \
  static_cast<void>(0),         \
      !((COND)) ? (void)0       \
                : ::base::log::_::LogMessageVoidify{} & L(__VA_ARGS__)

#define LOG_IF(S, C) _COND_LOG(LOG, C, S)

#define TLOG_IF(T, S, C) _COND_LOG(TLOG, C, T, S)

#define VLOG_IF(V, S, C) _COND_LOG(VLOG, C, T, S)

#define VTLOG_IF(V, T, S, C) _COND_LOG(VTLOG, C, V, T, S)

#define DLOG_IF(S, C) _COND_LOG(DLOG, C, S)

#define DTLOG_IF(T, S, C) _COND_LOG(DTLOG, C, T, S)

#endif  // _BASE_LOG_INL_H_
