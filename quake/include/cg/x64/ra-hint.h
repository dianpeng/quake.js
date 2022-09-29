#ifndef _QUAKE_CG_X64_RA_HINT_H_
#define _QUAKE_CG_X64_RA_HINT_H_
#include "base.h"

namespace quake {

namespace lir {
class HInstruction;
class RegisterInfo;
}  // namespace lir

namespace cg {
namespace x64 {

// Get all X64 HOpcode related hint, ie this is arch-specific information
void GetRAHint(const lir::HInstruction*, lir::RegisterInfo*);

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_RA_HINT_H_
