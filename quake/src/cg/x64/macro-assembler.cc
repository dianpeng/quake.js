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

static constexpr std::size_t kSafeFunctionCallStackReserve = (7 + 16) * 8;

// This function will assume anything (any registers) are unsafe to be used
void MacroAssembler::SafeFunctionCallPrologue() {
  // save all the caller saved registers, regardlessly,

  // 1) all the general purpose registers for caller saved
  __ push(asmjit::x86::rax);
  __ push(asmjit::x86::rdi);
  __ push(asmjit::x86::rsi);
  __ push(asmjit::x86::rcx);
  __ push(asmjit::x86::rdx);
  __ push(asmjit::x86::r8);
  __ push(asmjit::x86::r9);

  // 2) all the sse registers are not gonna sustain
  __ sub(asmjit::x86::rsp, 16 * 8);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 15), asmjit::x86::xmm0);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 14), asmjit::x86::xmm1);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 13), asmjit::x86::xmm2);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 12), asmjit::x86::xmm3);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 11), asmjit::x86::xmm4);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 10), asmjit::x86::xmm5);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 9), asmjit::x86::xmm6);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 8), asmjit::x86::xmm7);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 7), asmjit::x86::xmm8);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 6), asmjit::x86::xmm9);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 5), asmjit::x86::xmm10);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 4), asmjit::x86::xmm11);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 3), asmjit::x86::xmm12);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 2), asmjit::x86::xmm13);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 1), asmjit::x86::xmm14);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 0), asmjit::x86::xmm15);

  // note we assume rsp is correctly aligned, so nothing needs to be done
}

void MacroAssembler::SafeFunctionCallEpilogue() {
  // restore all the xmm registers
  __ movq(asmjit::x86::xmm0, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 15));
  __ movq(asmjit::x86::xmm1, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 14));
  __ movq(asmjit::x86::xmm2, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 13));
  __ movq(asmjit::x86::xmm3, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 12));

  __ movq(asmjit::x86::xmm4, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 11));
  __ movq(asmjit::x86::xmm5, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 10));
  __ movq(asmjit::x86::xmm6, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 9));
  __ movq(asmjit::x86::xmm7, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 8));

  __ movq(asmjit::x86::xmm8, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 7));
  __ movq(asmjit::x86::xmm9, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 6));
  __ movq(asmjit::x86::xmm10, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 5));
  __ movq(asmjit::x86::xmm11, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 4));

  __ movq(asmjit::x86::xmm12, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 3));
  __ movq(asmjit::x86::xmm13, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 2));
  __ movq(asmjit::x86::xmm14, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 1));
  __ movq(asmjit::x86::xmm15, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 0));

  __ add(asmjit::x86::rsp, 16 * 8);

  __ pop(asmjit::x86::r9);
  __ pop(asmjit::x86::r8);
  __ pop(asmjit::x86::rdx);
  __ pop(asmjit::x86::rcx);
  __ pop(asmjit::x86::rdi);
  __ pop(asmjit::x86::rsi);
  __ pop(asmjit::x86::rax);
}

void MacroAssembler::debug_i32(asmjit::x86::Gp v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_i32(std::int32_t v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_i64(asmjit::x86::Gp v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_i64(std::int64_t v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_f64(asmjit::x86::Xmm v) {
  SafeFunctionCallPrologue();
  __ movq(asmjit::x86::xmm0, v);
  __ call(FADDR(&QuakeJit::PrintF64));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_f64(double v) {
  SafeFunctionCallPrologue();
  __ movabs(asmjit::x86::rax, *reinterpret_cast<const std::int64_t*>(&v));
  __ movq(asmjit::x86::xmm0, asmjit::x86::rax);
  __ call(FADDR(&QuakeJit::PrintF64));
  SafeFunctionCallEpilogue();
}

MacroAssembler::MacroAssembler(CodeGen* cg)
    : asmjit::x86::Builder(&cg->code()->code_holder()) {}

}  // namespace x64
}  // namespace cg
}  // namespace quake
