#ifndef _BASE_SRC___LOG_SINK_H_
#define _BASE_SRC___LOG_SINK_H_
#include <base/common.h>

#include "bootstrap.h"

namespace base {
namespace log {
namespace _ {

// Sink is an object represent the destination of the log. It can be a file or
// stderr/stdout. The Sink API doesn't need to be thread safe the core API will
// obtain the lock before calling into the sink's API
class Sink {
 public:
  // Main function to flush the data out. The Write function should perform
  // operation transactionaly and thread safe.
  virtual void Write(Severity, const std::string&, bool flush) = 0;

  // Rotate the sink. It may fail silently.
  virtual void Rotate() {}

  // Flushg the sink, it may fail silently
  virtual void Flush() {}

  // Event handler for fork operation. It is mostly useful for those sink has
  // internal buffer implementation.
  virtual void BeforeFork() {}
  virtual void AfterFork() {}

  // Get the name of the sink. Used for displaying purpose.
  virtual std::string GetName() const = 0;

  virtual ~Sink() {}

 public:
  static std::shared_ptr<Sink> Create(const std::string&,
                                      const Parser::Attributes&);
  static std::shared_ptr<Sink> CreateDefault();
};

}  // namespace _
}  // namespace log
}  // namespace base

#endif  // _BASE_SRC___LOG_SINK_H_
