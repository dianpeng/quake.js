#ifndef _QUAKE_IR_TRACE_RESULT_H_
#define _QUAKE_IR_TRACE_RESULT_H_
#include "base.h"

namespace quake {
namespace ir {

enum class BailoutReason : std::uint8_t {
  kDone,              // this instruction is done
  kStop,              // the current trace is done, should call stop
  kNYI,               // Not yet implement
  kBytecodeOverflow,  // the function is too large to be jitted
  kBytecodeError,     // the bytecode/VM should throw an exception
  kReferenceError,    // the bytecode contains reference error

  kSizeOfReason
};

class TraceResult {
 public:
  static TraceResult Stop() { return TraceResult{BailoutReason::kStop}; }
  static TraceResult Done() { return TraceResult{}; }
  static TraceResult NYI(const char*, ...);
  static TraceResult BytecodeOverflow(const char*, ...);
  static TraceResult BytecodeError(const char*, ...);
  static TraceResult ReferenceError(const char*, ...);

 public:
  TraceResult() : reason_(BailoutReason::kDone), message_() {}
  explicit TraceResult(BailoutReason r) : reason_(r), message_() {}
  TraceResult(BailoutReason r, std::string&& msg)
      : reason_(r), message_(std::move(msg)) {}
  TraceResult(const TraceResult&) = default;
  TraceResult(TraceResult&&) = default;
  TraceResult& operator=(const TraceResult&) = default;
  TraceResult& operator=(TraceResult&&) = default;

  const std::string& message() const { return message_; }
  BailoutReason reason() const { return reason_; }

 public:
  bool done() const { return reason_ == BailoutReason::kDone; }
  operator bool() const { return done(); }

 private:
  BailoutReason reason_;
  std::string message_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_TRACE_RESULT_H_
