#include "lir/snapshot.h"
#include "lir/operand.h"
#include "lir/instruction.h"

#include "ir/ssa-var.h"
#include "ir/node.h"
#include "ir/checkpoint.h"
#include "ir/type.h"

namespace quake {
namespace lir {

DeoptReason Snapshot::MapDeoptReason(ir::Checkpoint::Reason reason) {
  switch (reason) {
    case ir::Checkpoint::kSideEffect:
      return DeoptReason::kSideEffect;
    case ir::Checkpoint::kException:
      return DeoptReason::kException;
    case ir::Checkpoint::kRefCount:
      return DeoptReason::kRefCount;
    case ir::Checkpoint::kLoopExit:
      return DeoptReason::kLoopExit;

    case ir::Checkpoint::kBranch:
    case ir::Checkpoint::kBB1:
    case ir::Checkpoint::kStart:
    case ir::Checkpoint::kEnd:
      return DeoptReason::kControlFlow;

    default:
      UNREACHABLE();
      return DeoptReason::kSideEffect;
  }
}

Deopt::ValueOrigin Snapshot::MapOrigin(ir::Checkpoint::Type t) {
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

int Snapshot::MapJValType(const ir::Type& t) {
  switch (t.tag()) {
    // ------------------------------------------------------------------------
    // Primitive types
    case ir::Type::kInt:
      return JVal::kInt;
    case ir::Type::kFloat64:
      return JVal::kFloat64;
    case ir::Type::kBool:
      return JVal::kBool;
    case ir::Type::kNull:
      return JVal::kNull;
    case ir::Type::kUndefined:
      return JVal::kUndefined;

    // ------------------------------------------------------------------------
    // Heap object, ie require RC management
    case ir::Type::kString:
      return JVal::kString;
    case ir::Type::kSymbol:
      return JVal::kSymbol;
    case ir::Type::kObject:
      return JVal::kObject;
    case ir::Type::kModule:
      return JVal::kModule;
    case ir::Type::kJSFunction:
      return JVal::kFunctionBytecode;
    case ir::Type::kUninitialized:
      return JVal::kUninitialized;
    // special flag/kind to indicate that the value is actually in boxref format
    case ir::Type::kBox:
      return Deopt::kBoxRef;

    default:
      return {};
  }
}

Deopt::MachineRef Snapshot::MapMachineRef(ir::SSAVar* ssa_var,
                                          const SSAVarMappings& mappings) {
  // Take care of special value (singleton, ie Undefined/Null) which does not
  // have a dedicated lir_operand
  switch (ssa_var->node()->opcode()) {
    case ir::Opcode::kKNull:
      return Deopt::MachineRef{Deopt::kImm, Deopt::MachineImm{Deopt::Null{}},
                               0};
    case ir::Opcode::kKUndef:
      return Deopt::MachineRef{Deopt::kImm,
                               Deopt::MachineImm{Deopt::Undefined{}}, 0};

    case ir::Opcode::kKUninit:
      return Deopt::MachineRef{Deopt::kImm, Deopt::MachineImm{Deopt::Uninit{}},
                               0};
    default:
      break;
  }

  lir::HOperand* lir_opr = nullptr;
  if (auto itr = mappings.find(ssa_var); itr != mappings.end()) {
    lir_opr = itr->second;
  }
  CHECK(lir_opr) << ssa_var->node()->Print();

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
    return Deopt::MachineRef{Deopt::kSpill, lir_opr->GetMemStack().slot()};
  } else if (lir_opr->IsRegister()) {
    return Deopt::MachineRef{
        Deopt::kRegister,
        static_cast<std::uint32_t>(lir_opr->GetRegister()->register_kind())};
  } else if (lir_opr->IsNone()) {
    switch (lir_opr->instruction_checked()->opcode()) {
      case HOpcode::kNull:
        return Deopt::MachineRef{Deopt::kImm, Deopt::MachineImm{Deopt::Null{}},
                                 0};
      case HOpcode::kUndefined:
        return Deopt::MachineRef{Deopt::kImm,
                                 Deopt::MachineImm{Deopt::Undefined{}}, 0};
      case HOpcode::kUninitialized:
        return Deopt::MachineRef{Deopt::kImm,
                                 Deopt::MachineImm{Deopt::Uninit{}}, 0};
      default:
        UNREACHABLE();
        break;
    }
  }

  UNREACHABLE();
  return Deopt::MachineRef{};
}

void Snapshot::Construct(const SSAVarMappings& mappings,
                         const ir::Checkpoint& ck) {
  happen_before_ = ck.happen_before();
  frame_.reserve(ck.frame_vector().size());

  for (const auto& e : ck.frame_vector()) {
    frame_.emplace_back(e.type, e.cur_bc, e.next_bc, e.slot_size, e.param_size,
                        e.local_size);

    auto current = &frame_.back();
    for (const auto& e : e.value_list) {
      switch (e.type()) {
        case ir::Checkpoint::EntryType::kVal: {
          auto v = Deopt::Val{MapOrigin(e.origin().type),      // origin type
                              e.origin().offset,               // offset/index
                              MapJValType(e.value()->type()),  // type
                              MapMachineRef(e.value(), mappings)};
          current->value_list.push_back(std::move(v));
        } break;

        case ir::Checkpoint::EntryType::kFrame: {
          auto v = Deopt::Val{Deopt::ValueOrigin::kFrame,
                              -1,  // irrelevent
                              -1,  // irrelevent
                              MapMachineRef(e.value(), mappings)};
          current->value_list.push_back(std::move(v));
        } break;

        default:
          UNREACHABLE();
          break;
      }
    }
  }
}

}  // namespace lir
}  // namespace quake

