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

MacroAssembler::MacroAssembler(CodeGen* cg)
    : asmjit::x86::Builder(&cg->code()->code_holder()) {}

}  // namespace x64
}  // namespace cg
}  // namespace quake
