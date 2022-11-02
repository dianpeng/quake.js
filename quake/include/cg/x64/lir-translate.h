#ifndef _QUAKE_CG_X64_LIR_TRANSLATE_H_
#define _QUAKE_CG_X64_LIR_TARNSLATE_H_
#include "base.h"
#include "lir/operand.h"

namespace quake {
class Quake;

namespace lir {
class HAssembler;
class HInstruction;
}  // namespace lir

namespace cg {
class CodeGen;
namespace x64 {

// Translate a single HAssembler into machine code recorded by CodeGen using
// internal translation rules.
base::Error TranslateLIR(Quake*, CodeGen*, lir::HAssembler*,
                         const lir::HInstruction*);

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_LIR_TRANSLATE_H_
