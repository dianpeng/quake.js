#ifndef _QUAKE_CG_H_
#define _QUAKE_CG_H_
#include "cg/x64/deopt-data.h"

namespace quake {
namespace cg {

#ifdef QUAKE_TARGET_X64

std::unique_ptr<DeoptData> CreateDeoptData(const void* a1, const void* a2);

#endif  // QUAKE_TARGET_X64

}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_H_
