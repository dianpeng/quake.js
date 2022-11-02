#include "lir/pre-code-gen.h"
#include "lir/builtin.h"
#include "lir/assembler.h"
#include "lir/instruction.h"
#include "quake-rt.h"

namespace quake {
namespace lir {

// 1. fix all the rcall's address into correct runtime call
void PreCodeGen::Run() {
  as_->ForeachInstruction([&](HInstruction* ins) {
    // rewrite memory stack related operand or output
    RewriteMemStack(ins);

    // fix rcall's address
    FixRCallAddress(ins);
  });
}

void PreCodeGen::RewriteMemStack(HInstruction* ins) {
  const auto offset = as_->stack_slot()->spill_offset();

  for (int i = 0; i < ins->opr_size(); ++i) {
    if (auto opr = ins->Opr(i); opr->IsMemStack()) {
      opr->GetMemStack().set_offset(offset);
    }
  }

  if (auto out = ins->output(); out && out->IsMemStack()) {
    out->GetMemStack().set_offset(offset);
  }
}

void PreCodeGen::FixRCallAddress(HInstruction* ins) {
  if (ins->IsRCall()) {
    auto rcall_idx = ins->Opr(0);
    CHECK(rcall_idx->IsImm());
    auto idx = static_cast<BuiltinFunction>(rcall_idx->GetImm().Int32());
    auto addr = QuakeJit::MapRCall(idx);
    auto new_opr = as_->NewMemStatic(addr);
    ins->OprSet(0, new_opr);
  }
}

PreCodeGen::PreCodeGen(Quake* q, HAssembler* as) : quake_(q), as_(as) {}

}  // namespace lir
}  // namespace quake
