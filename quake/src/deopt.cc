#include "deopt.h"
#include "quake.h"
#include "quake-rt.h"

#include "qjs.h"

#include "cg/cg.h"

#include "lir/snapshot.h"
#include "lir/operand.h"
#include "lir/register.h"

namespace quake {

void Deopt::ForeachFrameReversely(
    std::function<void(const Frame&)>&& visitor) const {
  for (auto itr = frame_.rbegin(); itr != frame_.rend(); itr++) {
    visitor(*itr);
  }
}

void Deopt::From(lir::Snapshot&& snap) {
  frame_ = std::move(snap.frame_);
  happen_before_ = snap.happen_before();
}

std::string DeoptData::Print() const {
  std::stringstream fmt;

  const auto quake = reinterpret_cast<Quake*>(GetQuakeRT()->quake);
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
            case Deopt::kI32:
              rvalue = base::Get<std::int32_t>(v.ref.imm);
              break;
            case Deopt::kI64:
              rvalue = base::Get<std::int64_t>(v.ref.imm);
              break;
            case Deopt::kF64:
              rvalue = base::Get<double>(v.ref.imm);
              break;
            case Deopt::kNull:
              rvalue = 0;
              break;
            case Deopt::kUndefined:
              rvalue = 0;
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
        case Deopt::kI32:
          return base::Get<std::int32_t>(ref.imm);
        case Deopt::kI64:
          return base::Get<std::int64_t>(ref.imm);
        case Deopt::kF64:
          return base::Get<double>(ref.imm);
        case Deopt::kNull:
          return 0;
        case Deopt::kUndefined:
          return 0;
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

void RematerializeValue(const DeoptData& data, const Deopt::Frame& frame,
                        QRecoveryFrame* rframe, JSValue* param, JSValue* local,
                        JSValue* temp, std::size_t param_size,
                        std::size_t local_size, std::size_t temp_size) {
  auto js_context = data.GetQuakeRT()->context;

  for (const auto& v : frame.value_list) {
    switch (v.origin) {
      // normal value
      case Deopt::kLocal: {
        auto index = v.index;
        auto raw_value = GetRawValue(v.ref, data);
        CHECK(index < static_cast<int>(local_size));
        local[index] = BoxValue(raw_value, v.kind);
      } break;

      case Deopt::kParam: {
        auto index = v.index;
        auto raw_value = GetRawValue(v.ref, data);
        CHECK(index < static_cast<int>(param_size));
        param[index] = BoxValue(raw_value, v.kind);
      } break;

      case Deopt::kTemp: {
        auto index = v.index;
        auto raw_value = GetRawValue(v.ref, data);
        CHECK(index < static_cast<int>(temp_size));
        temp[index] = BoxValue(raw_value, v.kind);
      } break;

      case Deopt::kFrame: {
        // frame information, ie compressed frame generated by the JIT code
        auto raw_value = GetRawValue(v.ref, data);
        auto jframe = *reinterpret_cast<JitFrame**>(&raw_value);
        CHECK(rframe);
        rframe->func_object = JS_DupValue(js_context, jframe->func_object);
        rframe->this_object = JS_DupValue(js_context, jframe->this_object);
        rframe->target_object = JS_DupValue(js_context, jframe->target_object);
      } break;

      default:
        UNREACHABLE();
        break;
    }
  }
}

void RematerializeFrame(QuakeRT* rt, const DeoptData& data,
                        const Deopt::Frame& frame, const bool happen_before) {
  auto recovery_address = [&]() -> const void* {
    if (happen_before) {
      return frame.current_bytecode;
    } else {
      return frame.next_bytecode;
    }
  };

  if (frame.type != JitFrameType::kJitEntry) {
    // create a recovery_frame
    auto recovery_frame = QuakeNewQRecoveryFrame(
        frame.slot_size, frame.param_size, frame.local_size);

    recovery_frame->prev = rt->rframe;  // linking back
    rt->rframe = recovery_frame;
    recovery_frame->next_bc = recovery_address();

    const auto param_size = recovery_frame->param - recovery_frame->slot;
    const auto local_size = recovery_frame->local - recovery_frame->param;
    const auto temp_size = recovery_frame->temp - recovery_frame->local;

    RematerializeValue(data, frame, recovery_frame, recovery_frame->param,
                       recovery_frame->local, recovery_frame->temp, param_size,
                       local_size, temp_size);
  } else {
    rt->next_bc = recovery_address();
    rt->exit_id = data.GetDeoptId();
    rt->rframe = nullptr;

    // entry frame, okay, no need to recreate the frame, but just do simple
    // things by reusing the existed one
    RematerializeValue(data, frame, nullptr, rt->param, rt->local, rt->temp,
                       rt->param_end - rt->param, rt->local_end - rt->local,
                       rt->temp_end - rt->temp);
  }
}

// Refresh all the local variable from deoptimziation data back to the original
// position and reconstruct the correct stack frame object
void RefreshSideEffect(QuakeRT* rt, const DeoptData& data, const Deopt& deopt) {
  deopt.ForeachFrameReversely([&](const auto& frame) {
    RematerializeFrame(rt, data, frame, deopt.happen_before());
  });
}

}  // namespace

DeoptNext Deopt::DeoptFromJit(const void* a0, const void* a1) {
  auto deopt_data = cg::CreateDeoptData(a0, a1);
  auto rt = deopt_data->GetQuakeRT();
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  const auto& deopt = quake->GetDeopt(deopt_data->GetDeoptId());

  LOG(ERROR) << deopt_data->Print();
  RefreshSideEffect(rt, *deopt_data, deopt);
  rt->next = static_cast<int>(deopt.next());

  return deopt.next();
}

}  // namespace quake
