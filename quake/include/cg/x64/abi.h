#ifndef _QUAKE_CG_X64_ABI_H_
#define _QUAKE_CG_X64_ABI_H_

#include "cg/x64/register.h"

namespace quake {
namespace cg {
namespace x64 {

// Here we define some of the X64 ABI related stuff. Currently we just support
// system V ABI.
#if defined(OS_SYSV) && defined(QUAKE_TARGET_X64)

// How many general purpose register can be used in function call to pass value
// Notes, runtime call will only use GP register for now
constexpr std::size_t kParamGPRegisterSize = 6;

// Stack alignment needed. For example on x64, SSE 4.2 require RSP to be aligned
// on 16 bytes
constexpr std::size_t kStackAlignment = 16;

// ----------------------------------------------------------------------------
// CARGS mappings

// GP usage
constexpr RegisterKind kABINativeGPArg1 = RegisterKind::kRdi;
constexpr RegisterKind kABINativeGPArg2 = RegisterKind::kRsi;
constexpr RegisterKind kABINativeGPArg3 = RegisterKind::kRdx;
constexpr RegisterKind kABINativeGPArg4 = RegisterKind::kRcx;
constexpr RegisterKind kABINativeGPArg5 = RegisterKind::kR8;
constexpr RegisterKind kABINativeGPArg6 = RegisterKind::kR9;

// FP usage
constexpr RegisterKind kABINativeFPArg1 = RegisterKind::kXmm2;
constexpr RegisterKind kABINativeFPArg2 = RegisterKind::kXmm3;
constexpr RegisterKind kABINativeFPArg3 = RegisterKind::kXmm4;
constexpr RegisterKind kABINativeFPArg4 = RegisterKind::kXmm5;
constexpr RegisterKind kABINativeFPArg5 = RegisterKind::kXmm6;
constexpr RegisterKind kABINativeFPArg6 = RegisterKind::kXmm7;

// Get the X'th argument, GP or FP, and its corresponding registers for
// passing argument. If not, then returns {}, which means the ABI resolver
// needs to spill the register on the stack. For now, we *NEVER* allow
// the native call to use more than allowed register numbers
std::optional<RegisterKind> GetABIGPArg(int);
std::optional<RegisterKind> GetABIFPArg(int);

#endif

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_ABI_H_
