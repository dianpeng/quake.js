#ifndef _QUAKE_CG_X64_ASM_UTIL_H_
#define _QUAKE_CG_X64_ASM_UTIL_H_
#include "base.h"

#include "cg/x64/register.h"

#include <asmjit/asmjit.h>

// ----------------------------------------------------------------------------
// Special assumption of x64
// ----------------------------------------------------------------------------
// RAX/XMM15 is reserved for Int/Float as scratch register

namespace quake {
namespace lir {
class HOperand;
}  // namespace lir

namespace cg {
namespace x64 {

int MapRKInd2ARegGP(RegisterKind rk);
int MapRKind2ARegFP(RegisterKind rk);

enum class ArrayNativeType {
  kInt8,
  kInt16,
  kInt32,
  kUInt8,
  kUInt16,
  kUInt32,
  kF32,
  kF64,
  kJSValue
};

namespace u {

inline asmjit::x86::Gp ScrB() { return asmjit::x86::al; }
inline asmjit::x86::Gp ScrW() { return asmjit::x86::ax; }
inline asmjit::x86::Gp ScrD() { return asmjit::x86::eax; }
inline asmjit::x86::Gp ScrQ() { return asmjit::x86::rax; }
inline asmjit::x86::Xmm ScrX() { return asmjit::x86::xmm15; }
inline asmjit::x86::Gp Rsp() { return asmjit::x86::rsp; }
inline asmjit::x86::Gp Rbp() { return asmjit::x86::rbp; }
inline asmjit::x86::Gp Ctx() { return asmjit::x86::r15; }

asmjit::Imm Imm(const quake::lir::HOperand*);
asmjit::Imm Imm8(const quake::lir::HOperand*);

asmjit::x86::Gp Gp(const quake::lir::HOperand*);
asmjit::x86::Xmm Fp(const quake::lir::HOperand*);
asmjit::x86::Mem Mem(const quake::lir::HOperand*);

asmjit::x86::Mem MemInd(const quake::lir::HOperand*);
asmjit::x86::Mem MemStack(const quake::lir::HOperand*);
asmjit::x86::Mem MemStatic(const quake::lir::HOperand*);

}  // namespace u

asmjit::x86::Gp MapRegisterGPFromRegisterKindASMJIT(RegisterKind, SizeClass);
asmjit::x86::Xmm MapRegisterFPFromRegisterKindASMJIT(RegisterKind);

template <typename T>
std::uint64_t FADDR(T fptr) {
  return reinterpret_cast<std::uint64_t>(fptr);
}

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_ASM_UTIL_H_
