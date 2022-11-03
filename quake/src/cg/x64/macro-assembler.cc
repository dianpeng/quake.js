#include "cg/x64/macro-assembler.h"
#include "cg/x64/asm-util.h"
#include "cg/x64/code-gen.h"

#include "quake-rt.h"

#define __ this->

namespace quake {
namespace cg {
namespace x64 {

void MacroAssembler::unreachable(int code) {
  __ mov(asmjit::x86::edi, asmjit::Imm(code));
  __ call(FADDR(&QuakeJit::Unreachable));
  __ int3();
}

void MacroAssembler::printi32(asmjit::x86::Gp v) {
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  __ int3();
}

void MacroAssembler::printi32(std::int32_t v) {
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  __ int3();
}

void MacroAssembler::printf64(asmjit::x86::Xmm v) {
  __ movq(asmjit::x86::xmm0, v);
  __ call(FADDR(&QuakeJit::PrintF64));
  __ int3();
}

void MacroAssembler::printf64(double v) {
  __ movabs(asmjit::x86::rax, *reinterpret_cast<const std::int64_t*>(&v));
  __ movq(asmjit::x86::xmm0, asmjit::x86::rax);
  __ call(FADDR(&QuakeJit::PrintF64));
  __ int3();
}

MacroAssembler::MacroAssembler(CodeGen* cg)
    : asmjit::x86::Builder(&cg->code()->code_holder()) {}

}  // namespace x64
}  // namespace cg
}  // namespace quake
