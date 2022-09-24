#include "formatter.h"

namespace base {
namespace log {
namespace _ {
namespace {

class DefaultFormatter : public Formatter {
 public:
  std::string Format(const char*, int, const char*, Severity, std::string&&,
                     bool) override;

  std::string GetName() const override { return "default"; }
};

std::string DefaultFormatter::Format(const char* file, int line,
                                     const char* tag, Severity severity,
                                     std::string&& msg, bool stacktrace) {
  std::string buf;
  LogTime log_time;
  GetNowAsLogTime(&log_time);

  if (tag) {
    buf =
        Fmt("%d:%d %s @(%s) "        // PID and thread id
            "%d.%d.%d %d.%d.%d.%d "  // timestamp, in milliseconds
            "(%s:%d) ]: ",           // filename and line number
            GetPid(), GetTid(), GetSeverityTag(severity), tag,

            log_time.year, log_time.month, log_time.day, log_time.hour,
            log_time.minute, log_time.second, log_time.msecond, file, line);

  } else {
    buf =
        Fmt("%d:%d %s "              // PID and thread id
            "%d.%d.%d %d.%d.%d.%d "  // timestamp, in milliseconds
            "(%s:%d) ]: ",           // filename and line number
            GetPid(), GetTid(), GetSeverityTag(severity),

            log_time.year, log_time.month, log_time.day, log_time.hour,
            log_time.minute, log_time.second, log_time.msecond, file, line);
  }

  // Append the message into the buffer
  buf.append(msg);

  // If applicable, generate a stacktrace afterwards
  if (stacktrace) {
    buf.push_back('\n');
    buf.append("************ StackTrace ***********\n");
    AppendStackTrace(&buf);
    buf.append("*************************************");
  }

  return buf;
}

}  // namespace

std::unique_ptr<Formatter> Formatter::Create(const std::string& name,
                                             const Parser::Attributes& attr) {
  UNUSE(attr);
  if (name == "default") {
    return std::make_unique<DefaultFormatter>();
  }
  return std::unique_ptr<Formatter>{};
}

std::unique_ptr<Formatter> Formatter::CreateDefault() {
  return std::make_unique<DefaultFormatter>();
}

}  // namespace _
}  // namespace log
}  // namespace base
