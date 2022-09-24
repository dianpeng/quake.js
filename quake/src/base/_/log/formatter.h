#ifndef _BASE_SRC___LOG_FORMATTER_H_
#define _BASE_SRC___LOG_FORMATTER_H_
#include <base/common.h>
#include <base/log.h>

#include "bootstrap.h"

namespace base {
namespace log {
namespace _ {

// Formatter is a interface used for do message formatting. Notes, the formatter
// should be thread safe and mostly it should be stateless.
class Formatter {
 public:
  struct Option {
    std::string tag;
    bool stack_trace;
    Option(std::string&& t = {}, bool st = false)
        : tag(std::move(t)), stack_trace(st) {}
  };

  virtual std::string Format(const char* file, int line, const char* tag,
                             Severity severity, std::string&& msg,
                             bool stacktrace) = 0;

  virtual std::string GetName() const = 0;

  virtual ~Formatter() {}

 public:
  static std::unique_ptr<Formatter> Create(const std::string&,
                                           const Parser::Attributes&);
  static std::unique_ptr<Formatter> CreateDefault();
};

}  // namespace _
}  // namespace log
}  // namespace base

#endif  // _BASE_SRC___LOG_FORMATTER_H_
