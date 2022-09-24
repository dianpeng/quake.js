#ifndef _QUAKE_CG_X64_ABI_H_
#define _QUAKE_CG_X64_ABI_H_

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

#endif

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_ABI_H_
