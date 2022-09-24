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

}  // namespace cg
}  // namespace quake

#endif  // QUAKE_CG_ABI_H_
