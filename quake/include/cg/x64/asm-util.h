#ifndef _QUAKE_CG_X64_ASM_UTIL_H_
#define _QUAKE_CG_X64_ASM_UTIL_H_
#include "base.h"
#include "cg/x64/register.h"

#include <asmjit/asmjit.h>

// ----------------------------------------------------------------------------
// Special assumption of x64
// ----------------------------------------------------------------------------
// RAX/XMM15 is reserved for Int/Float as scratch register
#define SCR_GPB asmjit::x86::al
#define SCR_GPW asmjit::x86::ax
#define SCR_GPD asmjit::x86::eax
#define SCR_GPQ asmjit::x86::rax
#define SCR_FP asmjit::x86::xmm15

// Reserved for stack usage
#define RSP asmjit::x86::rsp

// Reserved from frame
#define RBP asmjit::x86::rbp

// Reserved for context
#define CTX asmjit::x86::r15

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

template <typename T>
std::uint64_t FADDR(T fptr) {
  return reinterpret_cast<std::uint64_t>(fptr);
}

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_ASM_UTIL_H_
