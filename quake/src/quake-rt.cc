#include "quake-rt.h"
#include "quake.h"
#include "deopt.h"

#include <cmath>

namespace quake {

const char* QuakeJit::GetUnreachableCodeName(int x) {
  switch (x) {
    case QuakeJit::kUnreachable:
      return "unreachable";
    case QuakeJit::kUnreachableEndOfBody:
      return "end(body)";
    case QuakeJit::kUnreachableEndOfPrologue:
      return "end(prologue)";
    case QuakeJit::kUnreachableEndOfEpilogue:
      return "end(epilogue)";
    case QuakeJit::kUnreachableEndOfDeoptimization:
      return "end(deoptimization)";
    case QuakeJit::kUnreachableEndOfCastF64ToB32:
      return "end(f64_to_b32)";
    default:
      return "unknown";
  }
}

void QuakeJit::Unreachable(int x) {
  const char* msg = GetUnreachableCodeName(x);

  // there may not be any frame since the function is been invoked from JIT
  // code, we may need to generate some DWARF/EFI to allow stack unwind to
  // work.
  LOG(FATAL) << "(JIT-unreachable): " << msg;
}

const void* QuakeJit::MapRCall(BuiltinFunction bf) {
  switch (bf) {
    case BuiltinFunction::kFMod:
      return reinterpret_cast<const void*>(&RCallFMod);

    case BuiltinFunction::kFPow:
      return reinterpret_cast<const void*>(&RCallFPow);

    default:
      UNREACHABLE();
      return nullptr;
  }
}

void QuakeJit::PrintI32(std::int32_t value) {
  LOG(TRACE) << "(JIT): " << value;
}

void QuakeJit::PrintI64(std::int64_t value) {
  LOG(TRACE) << "(JIT): " << value;
}

void QuakeJit::PrintF64(double value) { LOG(TRACE) << "(JIT): " << value; }

double QuakeJit::RCallFMod(QuakeRT*, double x, double y) {
  return std::fmod(x, y);
}

double QuakeJit::RCallFPow(QuakeRT*, double x, double y) {
  return std::pow(x, y);
}

int QuakeJit::DeoptPrepare(void* a1, void* a2) {
  return static_cast<int>(Deopt::DeoptFromJit(a1, a2));
}

}  // namespace quake
