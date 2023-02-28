#include "cg/cg.h"
#include "cg/x64/deopt-data.h"

namespace quake {
namespace cg {

#ifdef QUAKE_TARGET_X64

std::unique_ptr<DeoptData> CreateDeoptData(const void* a1, const void* a2) {
  return std::make_unique<x64::DeoptDataX64>(a1, a2);
}

#endif  // QUAKE_TARGET_X64

}  // namespace cg
}  // namespace quake

