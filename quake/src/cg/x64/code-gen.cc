#include "cg/x64/code-gen.h"

namespace quake {
namespace cg {
namespace x64 {

CodeGen::CodeGen(Quake* q)
    : quake_(q),
      as_(),

      deopt_trampoline_(),
      jit_epilogue_() {}

asmjit::Label CodeGen::deopt_trampoline() {
  CHECK(deopt_trampoline_);
  return *deopt_trampoline_;
}

void CodeGen::BindDeoptTrampoline() {
  CHECK(!deopt_trampoline_);

  deopt_trampoline_.emplace(as_->newLabel());
  as_->bind(*deopt_trampoline_);
}

asmjit::Label CodeGen::jit_epilogue() {
  CHECK(jit_epilogue_);
  return *jit_epilogue_;
}

void CodeGen::BindJitEpilogue() {
  CHECK(!jit_epilogue_);
  jit_epilogue_.emplace(as_->newLabel());
  as_->bind(*jit_epilogue_);
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
