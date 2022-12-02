#ifndef _QUAKE_CG_ABI_H_
#define _QUAKE_CG_ABI_H_
#include "base.h"

#ifdef QUAKE_TARGET_X64
#include "cg/x64/abi.h"
#else
#error "error"
#endif  // QUAKE_TARGET_X64

namespace quake {
namespace cg {

#ifdef QUAKE_TARGET_X64
using x64::kParamGPRegisterSize;
using x64::kStackAlignment;
#endif

// The 6 is chosen because following reason:
//   1) ARM allows 4 registers to be passed in register
//   2) X64 allows 6 registers to be passed in register
//
//   3) In most cases, 6 registers is more than enough for us, except for FFI,
//      which we can use other LIR code to support it
constexpr std::size_t kNativeCallMaxParameterSize= 6;
constexpr std::size_t kNativeCallCallerSaveFPSize = 15;
constexpr std::size_t kNativeCallCallerSaveGPSize = 8;

}  // namespace cg
}  // namespace quake

#endif  // QUAKE_CG_ABI_H_
