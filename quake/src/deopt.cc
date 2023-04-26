#include "deopt.h"
#include "quake.h"
#include "quake-rt.h"
#include "jit.h"
#include "qjs.h"
#include "flags.h"

#include "cg/cg.h"
#include "lir/snapshot.h"
#include "lir/operand.h"
#include "lir/register.h"

namespace quake {

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
        case Deopt::kF64: {
          // bitwise copy
          auto raw = base::Get<double>(ref.imm);
          DeoptData::RawValue ret;
          std::memcpy(&ret, &raw, sizeof(ret));
          return ret;
        }
        case Deopt::kNull:
          return 0;
        case Deopt::kUndefined:
          return 0;
        case Deopt::kUninitialized:
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

JSValue BoxValueFromDeoptKind(DeoptData::RawValue raw, int type) {
  switch (type) {
    default:
      if (type < 0) {
        void* ptr;
        std::memcpy(&ptr, reinterpret_cast<void**>(&raw), sizeof(void*));
        return JS_MKPTR(type, ptr);
      }

      UNREACHABLE() << "invalid type: " << type;
      return JSValue{};

    // special handling
    case Deopt::kBoxRef: {
      JSValue* ptr;
      std::memcpy(&ptr, reinterpret_cast<JSValue**>(&raw), sizeof(ptr));
      return *ptr;
    }

    case JVal::kInt:
    case JVal::kBool:
    case JVal::kCatchOffset:
    case JVal::kException:
      return JS_MKVAL(type, static_cast<std::int32_t>(raw));

    case JVal::kFloat64: {
      double x;
      std::memcpy(&x, &raw, sizeof(x));
      return __JS_NewFloat64(nullptr, x);
    }

    case JVal::kNull:
      return JS_NULL;
    case JVal::kUndefined:
      return JS_UNDEFINED;
    case JVal::kUninitialized:
      return JS_UNINITIALIZED;
  }
}

void RematerializeValue(const DeoptData& data, const Deopt::Frame& frame,
                        JSValue* param, JSValue* local, JSValue* temp,
                        JSValue** temp_top) {
  auto temp_cur = *temp_top;

  for (const auto& v : frame.value_list) {
    switch (v.origin) {
      // normal value
      case Deopt::kLocal: {
        auto index = v.index;
        auto raw_value = GetRawValue(v.ref, data);
        local[index] = BoxValueFromDeoptKind(raw_value, v.kind);
      } break;

      case Deopt::kParam: {
        auto index = v.index;
        auto raw_value = GetRawValue(v.ref, data);
        param[index] = BoxValueFromDeoptKind(raw_value, v.kind);
      } break;

      case Deopt::kTemp: {
        auto index = v.index;
        auto raw_value = GetRawValue(v.ref, data);
        temp_cur[index] = BoxValueFromDeoptKind(raw_value, v.kind);
      } break;

      default:
        UNREACHABLE();
        break;
    }
  }

  auto max_tsize = frame.slot_size - frame.param_size - frame.local_size;
  *temp_top = temp + max_tsize;
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

  rt->jit.next_bc = recovery_address();
  rt->jit.exit_id = data.GetDeoptId();

  // entry frame, okay, no need to recreate the frame, but just do simple
  // things by reusing the existed one
  RematerializeValue(data, frame, rt->arg_buf, rt->var_buf, rt->jit.temp,
                     &rt->jit.temp_cur);
}

void MoveVarRefList(struct list_head* target, const struct list_head& source) {
  init_list_head(target);

  struct list_head* el;
  list_for_each(el, &source) {
    list_del(el);
    list_add_tail(el, target);
  }
}

std::size_t FrameMaxSlotSize(const JSStackFrame& native_frame) {
  auto function_bytecode = JS_VALUE_GET_OBJ(native_frame.cur_func);
  CHECK(function_bytecode->class_id == JS_CLASS_BYTECODE_FUNCTION);
  auto b = function_bytecode->u.func.function_bytecode;

  return b->var_count + b->stack_size + b->arg_count;
}

JSStackFrame* DuplicateInlineStackFrame(const DeoptData& deopt_data,
                                        const Deopt& deopt,
                                        const Deopt::Frame& deopt_frame,
                                        const JSStackFrame& native_frame,
                                        bool happen_before) {
  const auto slot_size = FrameMaxSlotSize(native_frame);  // get max slot size
  const auto arg_size = deopt_frame.param_size;
  const auto loc_size = deopt_frame.local_size;
  const auto tmp_size = slot_size - arg_size - loc_size;

  CHECK(arg_size == static_cast<unsigned int>(native_frame.arg_count));

  // allocate the raw frame pointer and along with its needed slot size
  auto raw_ptr = ::malloc(sizeof(JSStackFrame) + slot_size * sizeof(JSValue));
  auto slot_ptr = base::PtrForward<JSValue>(raw_ptr, sizeof(JSStackFrame));
  auto nframe = static_cast<JSStackFrame*>(raw_ptr);

  nframe->cur_func = native_frame.cur_func;  // bitwise copy
  MoveVarRefList(&nframe->var_ref_list, native_frame.var_ref_list);  // var ref

  nframe->arg_count = native_frame.arg_count;  // copy arg count
  nframe->js_mode = native_frame.js_mode;      // js mode
  nframe->cur_pc = nullptr;                    // nullify
  nframe->cur_sp = nullptr;                    // nullify
  nframe->prev_frame = nullptr;                // set later on
  nframe->jit.context = native_frame.jit.context;
  nframe->frame_type = QJS_JIT_FRAME_MALLOC;  // malloced frame

  // copy the argument(parameter)
  {
    nframe->arg_buf = slot_ptr;
    std::memcpy(nframe->arg_buf, native_frame.arg_buf,
                arg_size * sizeof(JSValue));
  }

  // copy the local
  {
    nframe->var_buf = slot_ptr + arg_size;
    std::memcpy(nframe->var_buf, native_frame.var_buf,
                loc_size * sizeof(JSValue));
  }

  // copy the temporary
  {
    std::memcpy(slot_ptr + arg_size + loc_size, native_frame.jit.temp,
                tmp_size * sizeof(JSValue));

    nframe->jit.temp = slot_ptr + arg_size + loc_size;
    nframe->jit.temp_cur = nframe->jit.temp;
  }

  // lastly rematerialize the frame with all the spilled slot
  RematerializeFrame(nframe, deopt_data, deopt_frame, happen_before);

  return nframe;
}

// Refresh all the local variable from deoptimziation data back to the original
// position and reconstruct the correct stack frame object
void RefreshSideEffect(QuakeRT* rt, const DeoptData& data, const Deopt& deopt) {
  std::vector<JSStackFrame*> raw_frame;
  auto first_inner = true;

  deopt.ForeachFrameReversely([&](const auto& frame, bool is_entry) {
    if (is_entry) {
      const auto happen_before =
          raw_frame.empty() ? deopt.happen_before() : true;

      RematerializeFrame(rt, data, frame, happen_before);
    } else {
      auto happen_before = first_inner ? deopt.happen_before() : true;

      auto dframe =
          DuplicateInlineStackFrame(data, deopt, frame, *rt, happen_before);

      dframe->jit.inner_most_frame = nullptr;
      raw_frame.push_back(dframe);
      rt = rt->prev_frame;
      first_inner = false;
    }
  });

  // Handle inner frame, which is been created on the fly during the frame
  // rematerialization
  if (!raw_frame.empty()) {
    auto current = raw_frame[0];
    auto inner_most = current;

    for (std::size_t i = 1; i < raw_frame.size(); i++) {
      auto next_frame = raw_frame[i];
      current->prev_frame = next_frame;
      current = next_frame;
    }
    current->prev_frame = rt;

    rt->jit.inner_most_frame = inner_most;
    rt->jit.context->rt->current_stack_frame = inner_most;
  } else {
    rt->jit.inner_most_frame = nullptr;
  }
}

}  // namespace

void Deopt::ForeachFrameReversely(
    std::function<void(const Frame&, bool is_entry)>&& visitor) const {
  for (auto itr = frame_.rbegin(); itr != frame_.rend();) {
    const auto& frame = *itr;
    itr++;
    const auto is_entry = (itr == frame_.rend());
    visitor(frame, is_entry);
  }
}

void Deopt::From(lir::Snapshot&& snap) {
  frame_ = std::move(snap.frame_);
  happen_before_ = snap.happen_before();
}

std::string Deopt::Val::Print() const {
  std::stringstream fmt;
  switch (origin) {
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

  fmt << "(" << index << ") ";
  if (kind == Deopt::kBoxRef) {
    fmt << "[box-ref] ";
  } else {
    fmt << JVal::GetTypeName(kind) << " ";
  }

  switch (ref.type) {
    case Deopt::kRegister: {
      auto rkind = static_cast<lir::RegisterKind>(ref.data);
      fmt << " $" << lir::Register::GetRegisterFromKind(rkind)->Print() << " ";
    } break;

    case Deopt::kImm:
      switch (ref.imm.index()) {
        case Deopt::kI32:
          fmt << " c(" << base::Get<std::int32_t>(ref.imm) << ")";
          break;
        case Deopt::kI64:
          fmt << " c(" << base::Get<std::int64_t>(ref.imm) << ")";
          break;
        case Deopt::kF64: {
          // bitwise copy
          auto raw = base::Get<double>(ref.imm);
          fmt << " c(" << raw << ")";
        } break;

        case Deopt::kNull:
          fmt << " null";
          break;

        case Deopt::kUninitialized:
          fmt << " uninitialized";
          break;

        case Deopt::kUndefined:
          fmt << " undefined";
          break;

        default:
          UNREACHABLE();
          break;
      }
      break;

    case Deopt::kSpill:
      fmt << " %(" << ref.data << ")";
      break;

    default:
      UNREACHABLE();
      break;
  }

  return fmt.str();
}

std::string DeoptData::Print() const {
  std::stringstream fmt;

  const auto quake = reinterpret_cast<Quake*>(GetQuakeRT()->jit.quake);
  const auto deopt_id = GetDeoptId();
  const auto& deopt = quake->GetDeopt(deopt_id);

  fmt << "\n";
  fmt << "***********************************************************\n";
  fmt << "***************** X64 Deoptimization Info *****************\n";
  fmt << "***********************************************************\n";

  fmt << "DeoptimizationId: " << deopt_id << "\n";
  fmt << "TraceId: " << quake->trace_id() << "\n";
  fmt << "HappenBefore: " << deopt.happen_before() << "\n";
  fmt << "Range(" << quake->trigger_loop_from_bc() << " - "
      << quake->trigger_loop_back_bc() << ")\n";
  fmt << "Debug\n";
  fmt << "---------------------------------------\n\n\n";
  fmt << quake->debug();
  fmt << "\n\n\n---------------------------------------";
  fmt << "\n";

  // dump all the register value
#define __(A, B)                                                   \
  do {                                                             \
    fmt << B << ": " << GetRegister(lir::RegisterKind::A) << '\n'; \
  } while (false);

  QUAKE_FOREACH_REGISTER_KIND(__)

#undef __  // __

  for (auto itr = deopt.frame().rbegin(); itr != deopt.frame().rend(); itr++) {
    fmt << "-------------------------- Frame --------------------------\n";
    for (const auto& v : itr->value_list) {
      auto rvalue = GetRawValue(v.ref, *this);
      fmt << v.Print() << " ";
      fmt << "@(" << rvalue << ") ";
      fmt << JVal::PrintRaw(rvalue, v.kind);
      fmt << "\n";
    }

    fmt << "next_bc: " << itr->next_bytecode << "\n";
    fmt << "current_bc: " << itr->current_bytecode << "\n";
    fmt << "slot_size: " << itr->slot_size << "\n";
    fmt << "param_size: " << itr->param_size << "\n";
    fmt << "local_size: " << itr->local_size << "\n";
    fmt << "jit_type: " << static_cast<int>(itr->type) << "\n";
  }

  return fmt.str();
}

DeoptId Deopt::DeoptFromJit(const void* a0, const void* a1) {
  auto deopt_data = cg::CreateDeoptData(a0, a1);
  auto rt = deopt_data->GetQuakeRT();
  auto quake = reinterpret_cast<Quake*>(rt->jit.quake);
  const auto did = deopt_data->GetDeoptId();
  const auto& deopt = quake->GetDeopt(did);

  if (FLAG_trace_compile_deoptimization.Get()) {
    LOG(TRACE) << deopt_data->Print();
  }

  RefreshSideEffect(rt, *deopt_data, deopt);
  quake->OnDeopt(did);
  return did;
}

}  // namespace quake
