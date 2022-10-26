#ifndef _QUAKE_CG_X64_ASM_UTIL_H_
#define _QUAKE_CG_X64_ASM_UTIL_H_
#include "base.h"
#include "cg/x64/register.h"

#include <asmjit/asmjit.h>

namespace quake {
namespace lir {
class HOperand;
}  // namespace lir

namespace cg {
namespace x64 {

int MapRKInd2ARegGP(RegisterKind rk);
int MapRKind2ARegFP(RegisterKind rk);

asmjit::Imm ImmOperandToASMJIT(const quake::lir::HOperand*);
asmjit::Imm Imm8OperandToASMJIT(const quake::lir::HOperand*);

asmjit::x86::Gp GPRegOperandToASMJIT(const quake::lir::HOperand*);
asmjit::x86::Xmm FPRegOperandToASMJIT(const quake::lir::HOperand*);
asmjit::x86::Mem MemIndOperandToASMJIT(const quake::lir::HOperand*);
asmjit::x86::Mem MemStackOperandToASMJIT(const quake::lir::HOperand*);
asmjit::x86::Mem MemStaticOperandToASMJIT(const quake::lir::HOperand*);
asmjit::x86::Mem MemoryOperandToASMJIT(const quake::lir::HOperand*);

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_ASM_UTIL_H_
