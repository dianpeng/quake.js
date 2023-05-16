#ifndef _QUAKE_IR_TRACE_RESULT_H_
#define _QUAKE_IR_TRACE_RESULT_H_
#include "base.h"

namespace quake {
namespace ir {

#define QUAKE_IR_REAL_BAILOUT_REASON(__)       \
  __(kNYI, "[nyi]")                            \
  __(kBytecodeOverflow, "[bytecode overflow]") \
  __(kBytecodeError, "[bytecode error]")       \
  __(kReferenceError, "[reference error]")     \
  /** control flow issue **/                   \
  __(kLoopExit, "[loop exit]")                 \
  __(kInnerLoop, "[inner loop]")

enum class BailoutReason : std::uint8_t {
  kDone,  // this instruction is done

  // --------------------------------------------------------------------------
  // Internal Usage
  kNext,    // no conclusion, should continue, used internally
  kIgnore,  // nothing is done here, can be continued, used internally
  kStop,    // stop the trace

// clang-format off
  // --------------------------------------------------------------------------
#define __(A, ...) A,
  QUAKE_IR_REAL_BAILOUT_REASON(__)
#undef __  // __

  kSizeOfReason
  // clang-format on
};

const char* GetBailoutReasonName(BailoutReason);

class TraceResult {
 public:
  static TraceResult Stop() { return TraceResult{BailoutReason::kStop}; }
  static TraceResult Next() { return TraceResult{BailoutReason::kNext}; }
  static TraceResult Ignore() { return TraceResult{BailoutReason::kIgnore}; }

  static TraceResult Done() { return TraceResult{}; }
  static TraceResult NYI(const char*, ...);
  static TraceResult BytecodeOverflow(const char*, ...);
  static TraceResult BytecodeError(const char*, ...);
  static TraceResult ReferenceError(const char*, ...);

  static TraceResult LoopExit(const char*, ...);
  static TraceResult InnerLoop(const char*, ...);

 public:
  TraceResult() : reason_(BailoutReason::kDone), message_() {}
  explicit TraceResult(BailoutReason r) : reason_(r), message_() {}
  TraceResult(BailoutReason r, std::string&& msg)
      : reason_(r), message_(std::move(msg)) {}
  TraceResult(const TraceResult&) = default;
  TraceResult(TraceResult&&) = default;
  TraceResult& operator=(const TraceResult&) = default;
  TraceResult& operator=(TraceResult&&) = default;

  bool IsNotInternal() const {
    return reason_ != BailoutReason::kNext && reason_ != BailoutReason::kIgnore;
  }

  const std::string& message() const { return message_; }
  BailoutReason reason() const { return reason_; }
  const char* reason_name() const { return GetBailoutReasonName(reason()); }

  std::string Print() const {
    return base::Fmt("[%s]: %s", reason_name(), message().c_str());
  }

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
