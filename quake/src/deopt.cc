#include "deopt.h"

#include "ir/type.h"
#include "ir/checkpoint.h"
#include "ir/ssa-var.h"
#include "lir/operand.h"

namespace quake {

namespace {

Deopt::ValueOrigin MapOrigin(ir::Checkpoint::Type t) {
  switch (t) {
    case ir::Checkpoint::Type::kLocal:
      return Deopt::ValueOrigin::kLocal;
    case ir::Checkpoint::Type::kParam:
      return Deopt::ValueOrigin::kParam;
    case ir::Checkpoint::Type::kTemp:
      return Deopt::ValueOrigin::kTemp;
    default:
      UNREACHABLE();
      return Deopt::ValueOrigin::kTemp;
  }
}

int MapJValType(const ir::Type& t) {
  auto v = ir::T2JValTag(t.tag());
  CHECK(v);
  return *v;
}

Deopt::MachineRef MapMachineRef(ir::SSAVar* ssa_var) {
  auto lir_opr = ssa_var->lir_operand();
  CHECK(lir_opr);

  if (lir_opr->IsImm()) {
    const auto& imm = lir_opr->GetImm();
    if (imm.IsInt32()) {
      return Deopt::MachineRef{Deopt::kImm, Deopt::MachineImm{imm.Int32()}, 0};
    }
    if (imm.IsInt64()) {
      return Deopt::MachineRef{Deopt::kImm, Deopt::MachineImm{imm.Int64()}, 0};
    }
    if (imm.IsFloat64()) {
      return Deopt::MachineRef{Deopt::kImm, Deopt::MachineImm{imm.Float64()},
                               0};
    }
    UNREACHABLE();
  } else if (lir_opr->IsMemory()) {
    CHECK(lir_opr->IsMemStack());  // can only be spilled slot for now
    return Deopt::MachineRef{Deopt::kSpill,
                             lir_opr->GetMemStack().SlotOffset()};
  } else if (lir_opr->IsRegister()) {
    return Deopt::MachineRef{
        Deopt::kRegister,
        static_cast<std::uint32_t>(lir_opr->GetRegister()->register_kind())};
  }

  UNREACHABLE();
  return Deopt::MachineRef{};
}

}  // namespace

void Deopt::From(const ir::Checkpoint& ck) {
  if (ck.entry_vector().empty())
    return;  // if the checkpoint is empty, just return

  Frame* current = nullptr;

  for (const auto& e : ck.entry_vector()) {
    switch (e.type()) {
      case ir::Checkpoint::EntryType::kVal: {
        auto v = Val{MapOrigin(e.origin().type),      // origin type
                     e.origin().offset,               // offset/index
                     MapJValType(e.value()->type()),  // type
                     MapMachineRef(e.value())};
        current->value_list.push_back(std::move(v));
      } break;

      case ir::Checkpoint::EntryType::kFrame: {
        frame_.emplace_back(e.bc_pos());
        current = &frame_.back();
      } break;

      default:
        UNREACHABLE();
        break;
    }
  }
}

}  // namespace quake
