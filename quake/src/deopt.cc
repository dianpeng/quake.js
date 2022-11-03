#include "deopt.h"
#include "quake.h"
#include "quake-rt.h"

#include "qjs.h"

#include "cg/cg.h"
#include "ir/type.h"
#include "ir/checkpoint.h"
#include "ir/ssa-var.h"
#include "lir/operand.h"
#include "lir/register.h"

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
    return Deopt::MachineRef{Deopt::kSpill, lir_opr->GetMemStack().slot()};
  } else if (lir_opr->IsRegister()) {
    return Deopt::MachineRef{
        Deopt::kRegister,
        static_cast<std::uint32_t>(lir_opr->GetRegister()->register_kind())};
  }

  UNREACHABLE();
  return Deopt::MachineRef{};
}

}  // namespace

void Deopt::ForeachFrameReversely(
    std::function<void(const Frame&)>&& visitor) const {
  for (auto itr = frame_.rbegin(); itr != frame_.rend(); itr++) {
    visitor(*itr);
  }
}

void Deopt::From(const ir::Checkpoint& ck) {
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

std::string DeoptData::Print() const {
  std::stringstream fmt;

  const auto quake = GetQuakeRT()->quake;
  const auto deopt_id = GetDeoptId();
  const auto& deopt = quake->GetDeopt(deopt_id);

  fmt << "***************** X64 Deoptimization Info *****************\n";
  fmt << "DeoptimizationId: " << deopt_id << "\n";

  for (auto itr = deopt.frame().rbegin(); itr != deopt.frame().rend(); itr++) {
    fmt << "-------------------------- Frame --------------------------\n";
    for (const auto& v : itr->value_list) {
      switch (v.origin) {
        case Deopt::kLocal:
          fmt << "[local]";
          break;

        case Deopt::kParam:
          fmt << "[param]";
          break;

        case Deopt::kTemp:
          fmt << "[temp]";
          break;

        default:
          UNREACHABLE();
          break;
      }

      fmt << "(" << v.index << ") ";
      fmt << JVal::GetTypeName(v.kind) << " ";

      RawValue rvalue;

      switch (v.ref.type) {
        case Deopt::kRegister:
          rvalue = GetRegister(static_cast<lir::RegisterKind>(v.ref.data));
          break;

        case Deopt::kImm:
          switch (v.ref.imm.index()) {
            case 0:
              rvalue = base::Get<std::int32_t>(v.ref.imm);
              break;
            case 1:
              rvalue = base::Get<std::int64_t>(v.ref.imm);
              break;
            case 2:
              rvalue = base::Get<double>(v.ref.imm);
              break;

            default:
              UNREACHABLE();
              break;
          }
          break;

        case Deopt::kSpill:
          rvalue = GetSpill(v.ref.data);
          break;

        default:
          UNREACHABLE();
          break;
      }

      fmt << "@(" << rvalue << ")";

      // based on type to print it the correct format
      fmt << JVal::PrintRaw(rvalue, v.kind) << '\n';
    }

    fmt << "bc_pointer: " << itr->next_bytecode << "\n";
  }

  return fmt.str();
}

namespace {

DeoptData::RawValue GetRawValue(const Deopt::MachineRef& ref,
                                const DeoptData& data) {
  switch (ref.type) {
    case Deopt::kRegister:
      return data.GetRegister(static_cast<lir::RegisterKind>(ref.data));

    case Deopt::kImm:
      switch (ref.imm.index()) {
        case 0:
          return base::Get<std::int32_t>(ref.imm);
        case 1:
          return base::Get<std::int64_t>(ref.imm);
        case 2:
          return base::Get<double>(ref.imm);
        default:
          UNREACHABLE();
          return 0;
      }
      break;

    case Deopt::kSpill:
      return data.GetSpill(ref.data);

    default:
      UNREACHABLE();
      return 0;
  }
}

JSValue BoxValue(DeoptData::RawValue raw, int type) {
  switch (type) {
    default:
      if (type < 0) {
        return JS_MKPTR(type, *reinterpret_cast<void**>(&raw));
      }

      UNREACHABLE() << "invalid type: " << type;
      return JSValue{};

    case JVal::kInt:
    case JVal::kBool:
    case JVal::kCatchOffset:
    case JVal::kException:
      return JS_MKVAL(type, static_cast<std::int32_t>(raw));

    case JVal::kFloat64:
      return __JS_NewFloat64(nullptr, *reinterpret_cast<const double*>(&raw));

    case JVal::kNull:
      return JS_NULL;
    case JVal::kUndefined:
      return JS_UNDEFINED;
    case JVal::kUninitialized:
      return JS_UNINITIALIZED;
  }
}

// Refresh all the local variable from deoptimziation data back to the original
// position and reconstruct the correct stack frame object
void RefreshSideEffect(QuakeRT* rt, const DeoptData& data, const Deopt& deopt) {
  deopt.ForeachFrameReversely([&](const auto& frame) {
    // 1) Recreate each variables from frame's value list. This gonna impact
    //    all the local variable, parameter and temporary (mostly not)
    for (const auto& v : frame.value_list) {
      switch (v.origin) {
        case Deopt::kLocal: {
          auto index = v.index;
          auto raw_value = GetRawValue(v.ref, data);
          rt->local[index] = BoxValue(raw_value, v.kind);
        } break;

        case Deopt::kParam: {
          auto index = v.index;
          auto raw_value = GetRawValue(v.ref, data);
          rt->param[index] = BoxValue(raw_value, v.kind);
        } break;

        case Deopt::kTemp: {
          auto index = v.index;
          auto raw_value = GetRawValue(v.ref, data);
          rt->temp[index] = BoxValue(raw_value, v.kind);
        } break;

        default:
          UNREACHABLE();
          break;
      }
    }

    // ------------------------------------------------------------------------
    // Recreate other frame information
    //
    // 1) resume bytecode pointer
    // 2) frame information
    // 3) other stuff ....
  });
}

}  // namespace

DeoptNext Deopt::DeoptFromJit(const void* a0, const void* a1) {
  auto deopt_data = cg::CreateDeoptData(a0, a1);
  auto rt = deopt_data->GetQuakeRT();
  const auto& deopt = rt->quake->GetDeopt(deopt_data->GetDeoptId());

  LOG(ERROR) << '\n' << deopt_data->Print();
  RefreshSideEffect(rt, *deopt_data, deopt);
  rt->next = static_cast<int>(deopt.next());

  return deopt.next();
}

}  // namespace quake
