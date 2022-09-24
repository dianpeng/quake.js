#include "ir/trace-result.h"

namespace quake {
namespace ir {

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

}  // namespace ir
}  // namespace quake
