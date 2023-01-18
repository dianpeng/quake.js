#include "ir/trace-result.h"

namespace quake {
namespace ir {

const char* GetBailoutReasonName(BailoutReason br) {
  switch (br) {
    case BailoutReason::kDone:
      return "done";
    case BailoutReason::kStop:
      return "stop";
    case BailoutReason::kNYI:
      return "NYI";
    case BailoutReason::kBytecodeOverflow:
      return "bytecode-overflow";
    case BailoutReason::kBytecodeError:
      return "bytecode-error";
    case BailoutReason::kReferenceError:
      return "reference-error";
    default:
      return "<none>";
  }
}

TraceResult TraceResult::NYI(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  auto msg = base::FmtV(fmt, va);
  va_end(va);
  return TraceResult{BailoutReason::kNYI, std::move(msg)};
}

TraceResult TraceResult::BytecodeOverflow(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  auto msg = base::FmtV(fmt, va);
  va_end(va);
  return TraceResult{BailoutReason::kBytecodeOverflow, std::move(msg)};
}

TraceResult TraceResult::BytecodeError(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  auto msg = base::FmtV(fmt, va);
  va_end(va);
  return TraceResult{BailoutReason::kBytecodeError, std::move(msg)};
}

TraceResult TraceResult::ReferenceError(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  auto msg = base::FmtV(fmt, va);
  va_end(va);
  return TraceResult{BailoutReason::kReferenceError, std::move(msg)};
}

TraceResult TraceResult::LoopExit(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  auto msg = base::FmtV(fmt, va);
  va_end(va);
  return TraceResult{BailoutReason::kLoopExit, std::move(msg)};
}

TraceResult TraceResult::InnerLoop(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  auto msg = base::FmtV(fmt, va);
  va_end(va);
  return TraceResult{BailoutReason::kInnerLoop, std::move(msg)};
}

}  // namespace ir
}  // namespace quake
