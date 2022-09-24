#include "ir/builder.h"
#include "ir/interp-info.h"

// To save us from typping here. C++ really need block expression, just like
// rust to make its code compact and easy to express
#define TOS(V, INFO, IDX)                      \
  auto V = static_cast<SSAVar*>(nullptr);      \
  do {                                         \
    if (auto [r, xx] = StkAt(INFO, IDX); !r) { \
      return r;                                \
    } else {                                   \
      V = xx;                                  \
    }                                          \
  } while (false)

namespace quake {
namespace ir {

Builder::Builder(Quake* q, Graph* g)
    : quake_(q),
      graph_(g),
      rc_queue_(q->zone()),
      rc_flushed_(false),
      checkpoint_(nullptr),
      checkpoint_done_(false),
      pending_checkpoint_(),
      interp_info_(nullptr),
      last_bc_(Bytecode::OP_invalid),
      param_size_(0),
      local_size_(0),
      storage_(q->zone()) {}

// ----------------------------------------------------------------------------
// Stack manipulation

std::size_t Builder::StkIdx(int idx) const {
  idx = idx < 0 ? -idx : idx;
  CHECK(static_cast<std::size_t>(idx) >= storage_.size());

  auto real_index = storage_.size() - static_cast<std::size_t>(idx);
  CHECK(real_index >= temp_base());
  return real_index;
}

std::pair<TraceResult, SSAVar*> Builder::StkAt(InterpInfo* info, int index) {
  auto real_index = StkIdx(index);

  if (auto ssa_var = storage_[real_index]; !ssa_var) {
    // If a temporary value is not loaded, we should just load it now
    auto memory_node =
        graph_->AddValue(info, Opcode::TempGet,
                         graph_->KI32(info, static_cast<int>(real_index)));

    // Get the current memory value, used for specialization
    auto memory_val = info->GetTemp(index);

    // Unbox the node
    if (auto [r, ssa_var] = Unbox(info, memory_val, memory_node); !r) {
      return std::make_pair(r, nullptr);
    } else {
      StkSet(index, ssa_var);
      return std::make_pair(r, ssa_var);
    }
  } else {
    return std::make_pair(TraceResult::Done(), ssa_var);
  }
}

void Builder::StkSet(int index, SSAVar* value) {
  auto real_index = StkIdx(index);
  storage_[real_index] = value;
}

void Builder::Push(SSAVar* value) { storage_.push_back(value); }

void Builder::Pop(int n) {
  CHECK(static_cast<std::uint32_t>(n) <= temp_size());
  storage_.resize(storage_.size() - n);
}

void Builder::Pop() {
  CHECK(temp_size() > 0);
  storage_.pop_back();
}

// ----------------------------------------------------------------------------
// Checkpoint
//
// Update checkpoint after a certain node is done, this is used to indicate that
// the input node has side effect
void Builder::UpdateCheckpoint(InterpInfo* info, Checkpoint::Reason reason,
                               void* bc, Node* n) {
  // A single bytecode cannot emit 2 checkpoint, unless otherwise specified
  CHECK(!checkpoint_done_);

  checkpoint_ = graph_->AddCheckpoint(info, checkpoint_, this, reason, bc, n);
  checkpoint_done_ = true;
}

void Builder::UpdateCheckpointAfter(InterpInfo* info, Checkpoint::Reason reason,
                                    Node* n) {
  UpdateCheckpoint(info, reason, info->next_pc(), n);
}

void Builder::AddCheckpointAfter(Checkpoint::Reason reason, void* bc_position,
                                 Node* node) {
  CHECK(!pending_checkpoint_);
  if (!HasRCQueued()) {
    pending_checkpoint_.emplace(reason, bc_position, node);
  }
}

void Builder::UpdatePendingCheckpoint(InterpInfo* info) {
  if (pending_checkpoint_) {
    UpdateCheckpoint(info, pending_checkpoint_->reason,
                     pending_checkpoint_->resumed_bc,
                     pending_checkpoint_->trigger_node);
    pending_checkpoint_.reset();
  }
}

// Invoked when we just emit a guard node. For guard node, we need to figure out
// where our guard node should jump to if the guard failed. A guard node will
// need to jump some previously saved checkpoint and the jumpped target is been
// saved into the exit_ field of node. One node can jump to a single checkpoint
void Builder::OnGuard(Node* node) {
  CHECK(node->ir_effect().IsGuard());
  CHECK(!rc_flushed_);
  CHECK(!pending_checkpoint_);
  CHECK(!checkpoint_done_);

  node->BailoutTo(checkpoint_);
}

void Builder::OnGuard(SSAVar* node) { OnGuard(node->node()); }

// ----------------------------------------------------------------------------
// Reference counting handling.
//
// Essentially speaking, RC is a side effect operation which effectively
// snapshot the VM into sparse states. Inside of the quickjs's interpreter,
// it sparsely spreaded RC handling into each bytecode handler. This is fine
// for the interpreter but hard for us to implement a partial evaluation JIT
// since when JIT bailout, it can only return back to a certain bytecode but
// not within each bytecode. For each bytecode handler that performs RC, we
// should aways
//   1) Generate bailout before perform any RC operations
//   2) Perform all RC operations in a batch
//   3) and once 2) is done, we cannot bailout from current bytecode but have
//      to spawn a checkpoint here
// Most operation is fine, except for certain operation that requires calling
// out to outside of the JIT frame, ie calling external C/C++ functions. If
// that's the case, we must make sure after the bytecode's RC performs, nothing
// happened inside of the bytecode generated jit code will lead to a
// deoptimization.
//
// Another approach is to reverse the RC operation, but that's super hard since
// when RC drops to 0, you cannot really reverse it, unless you do something
// like hazard-pointer style GC, which is a mess.

void Builder::RcInc(InterpInfo* info, SSAVar* var) {
  if (var->type().IsHeap()) {
    // Queue RC operation
    auto node = graph_->AddNodeInternal(info, Opcode::RcInc, var);
    rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
  }
}

void Builder::RcDec(InterpInfo* info, SSAVar* var) {
  if (var->type().IsHeap()) {
    // Queue RC operation
    auto node = graph_->AddNodeInternal(info, Opcode::RcDec, var);
    rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
  }
}

void Builder::RcTryInc(InterpInfo* info, SSAVar* var) {
  auto node = graph_->AddNodeInternal(info, Opcode::RcTryInc, var);
  rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
}

void Builder::RcTryDec(InterpInfo* info, SSAVar* var) {
  auto node = graph_->AddNodeInternal(info, Opcode::RcTryDec, var);
  rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
}

bool Builder::FlushRC(InterpInfo* info) {
  if (!rc_queue_.empty()) {
    rc_flushed_ = true;

    auto rc_op =
        graph_->AddOp(info, Opcode::RcOp, static_cast<int>(rc_queue_.size()));

    int idx = 0;
    for (auto ssa_var : rc_queue_) {
      rc_op->Set(idx, ssa_var);
      ++idx;
    }

    // If there's a pending checkpoint, then just use the pending checkpoint,
    // otherwise generate a new one
    if (pending_checkpoint_) {
      UpdatePendingCheckpoint(info);
    } else {
      UpdateCheckpointAfter(info, Checkpoint::kRefCount, rc_op);
    }
    return true;
  }
  return false;
}

// ----------------------------------------------------------------------------
// Value semantic stack handling

void Builder::PushSSA(InterpInfo* info, SSAVar* var) {
  RcInc(info, var);
  Push(var);
}

void Builder::PopAndRcDec(InterpInfo* info) {
  auto real_index = StkIdx(1);
  if (auto slot = storage_[real_index]; !slot) {
    // Here we generate a TOS value if it is not there but do not emit guard for
    // it since we just need to reduce its DEC. Notes we don't need the type
    // information, but just pops it out with polymorphic ref-count deduction
    auto memory_node =
        graph_->AddValue(info, Opcode::TempGet,
                         graph_->KI32(info, static_cast<int>(real_index)));
    RcTryDec(info, memory_node);
  } else {
    RcDec(info, slot);
  }
}

TraceResult Builder::PushJVal(InterpInfo* info, JVal jval) {
  switch (jval.type()) {
    case JVal::kInt:
      Push(graph_->KInt(info, jval.Int()));
      break;

    case JVal::kFloat64:
      Push(graph_->KFloat64(info, jval.Float64()));
      break;

    case JVal::kBool:
      Push(graph_->KBool(info, jval.Bool()));
      break;

    case JVal::kNull:
      Push(graph_->KNull(info));
      break;

    case JVal::kUndefined:
      Push(graph_->KUndefined(info));
      break;

    default:
      return TraceResult::NYI("unknown constant");
  }

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Box and Unbox

std::pair<TraceResult, SSAVar*> Builder::Unbox(InterpInfo* info, JVal jval,
                                               SSAVar* node) {
  CHECK(node->type().IsBox());
  switch (jval.type()) {
    case JVal::kInt:
    case JVal::kFloat64:
    case JVal::kBool:
    case JVal::kNull:
    case JVal::kUndefined: {
      CHECK(!rc_flushed_);
      auto guard_node = graph_->AddValue(info, Opcode::BxGuard,
                                         JValT2Type(jval.type()), node);
      OnGuard(guard_node);
      return std::make_pair(TraceResult::Done(), guard_node);
    }
    default:
      return std::make_pair(TraceResult::NYI("unknown type"), nullptr);
  }
}

std::pair<TraceResult, SSAVar*> Builder::Box(InterpInfo* info, SSAVar* node) {
  CHECK(!node->type().IsBox());
  SSAVar* output = nullptr;

  // Try to perform some optimization here for boxing, some known type can
  // directly convert to certain boxing value, just in case we can do this
  switch (node->type().tag()) {
    case Type::kInt:
      output = graph_->KI64(info, JVal::BitI32(node->node()->Int32()));
      break;

    case Type::kFloat64:
      output = graph_->KI64(info, JVal::BitF64(node->node()->Real()));
      break;

    case Type::kBool:
      output = graph_->KI64(
          info, node->node()->Bool() ? JVal::kBitTrue : JVal::kBitFalse);
      break;

    case Type::kNull:
      output = graph_->KI64(info, JVal::kBitNull);
      break;

    case Type::kUndefined:
      output = graph_->KI64(info, JVal::kBitUndefined);
      break;

    case Type::kI64:
      UNREACHABLE() << "we should not have I64 for boxing";
      break;

    default:
      return std::make_pair(TraceResult::NYI("unsupported type"), nullptr);
  }

  CHECK(output);
  return std::make_pair(TraceResult::Done(), output);
}

// ----------------------------------------------------------------------------
// Check whether the node is a int zero or not. The node must be a int already
std::pair<TraceResult, SSAVar*> Builder::CheckZeroInt(InterpInfo* info,
                                                      SSAVar* node) {
  CHECK(node->type().IsInt());
  auto guard_node = graph_->AddValue(info, Opcode::GNotZeroInt, node);

  OnGuard(guard_node);
  return std::make_pair(TraceResult::Done(), guard_node);
}

// ----------------------------------------------------------------------------
// Local Variable
std::pair<TraceResult, SSAVar*> Builder::LocAt(InterpInfo* info, int idx) {
  CHECK(static_cast<std::uint32_t>(idx) < local_size());

  // If local variable is not set, then in our tracking it will be nullptr,
  // and we create a LocalGet ndoe which load the memory into our SSAVar
  // additionally, based on the local_var to emit a type guard. Our tracking
  // variable are all typed already and unboxed
  if (auto loc = local()[idx]; !loc) {
    // get local variable from current frame
    auto local_var = info->GetLocal(idx);

    // create the local load node
    auto memory_node =
        graph_->AddValue(info, Opcode::LocGet, graph_->KI32(info, idx));

    // based on the local_var to emit type guard
    if (auto [r, ssa] = Unbox(info, local_var, memory_node); !r) {
      return std::make_pair(r, nullptr);
    } else {
      // just put the unboxed value back to local tracking
      local()[idx] = ssa;
      return std::make_pair(TraceResult::Done(), ssa);
    }
  } else {
    return std::make_pair(TraceResult::Done(), loc);
  }
}

TraceResult Builder::LocSet(InterpInfo* info, int local_idx, SSAVar* value) {
  CHECK(static_cast<std::uint32_t>(local_idx) < local_size());
  local()[local_idx] = value;
  return TraceResult::Done();
}

void Builder::RcTryDecLoc(InterpInfo* info, int loc_idx) {
  CHECK(static_cast<std::uint32_t>(loc_idx) < local_size());

  if (auto loc = local()[loc_idx]; !loc) {
    auto memory_node =
        graph_->AddValue(info, Opcode::LocGet, graph_->KI32(info, loc_idx));
    RcTryDec(info, memory_node);
  } else {
    RcDec(info, loc);
  }
}

// ----------------------------------------------------------------------------
// Argument
std::pair<TraceResult, SSAVar*> Builder::ParamAt(InterpInfo* info, int idx) {
  CHECK(static_cast<std::uint32_t>(idx) < param_size());

  if (auto p = param()[idx]; !p) {
    // create the local load node
    auto memory_node =
        graph_->AddValue(info, Opcode::ParamGet, graph_->KI32(info, idx));

    auto param_val = info->GetParam(idx);

    // based on the local_var to emit type guard
    if (auto [r, ssa] = Unbox(info, param_val, memory_node); !r) {
      return std::make_pair(r, nullptr);
    } else {
      // just put the unboxed value back to local tracking
      param()[idx] = ssa;
      return std::make_pair(TraceResult::Done(), ssa);
    }
  } else {
    return std::make_pair(TraceResult::Done(), p);
  }
}

TraceResult Builder::ParamSet(InterpInfo* info, int param_idx, SSAVar* value) {
  CHECK(static_cast<std::uint32_t>(param_idx) < param_size());
  param()[param_idx] = value;
  return TraceResult::Done();
}

void Builder::RcTryDecParam(InterpInfo* info, int idx) {
  CHECK(static_cast<std::uint32_t>(idx) < param_size());
  if (auto p = param()[idx]; !p) {
    auto memory_node =
        graph_->AddValue(info, Opcode::ParamGet, graph_->KI32(info, idx));

    RcTryDec(info, memory_node);
  } else {
    RcDec(info, p);
  }
}

// ----------------------------------------------------------------------------
// Handler for opcode/bytecode

TraceResult Builder::Trace_OP_push_i32(InterpInfo* info) {
  auto arg = info->GetU32();
  auto kint = graph_->KInt(info, arg);
  Push(kint);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_push_const(InterpInfo* info) {
  auto index = info->GetU32();
  auto cvalue = info->GetConst(index);
  return PushJVal(info, cvalue);
}

TraceResult Builder::Trace_OP_push_atom_value(InterpInfo* info) {
  auto index = info->GetU32();
  auto cvalue = info->GetAtom(index, false);
  return PushJVal(info, cvalue);
}

TraceResult Builder::Trace_OP_undefined(InterpInfo* info) {
  return PushJVal(info, JVal::Undefined(info->qjs_context()));
}

TraceResult Builder::Trace_OP_null(InterpInfo* info) {
  return PushJVal(info, JVal::Null(info->qjs_context()));
}

TraceResult Builder::Trace_OP_push_false(InterpInfo* info) {
  return PushJVal(info, JVal::True(info->qjs_context()));
}

TraceResult Builder::Trace_OP_push_true(InterpInfo* info) {
  return PushJVal(info, JVal::False(info->qjs_context()));
}

// Notes, this object is immutable for a certain frame, we just do not need to
// guard anything, but directly use it
TraceResult Builder::Trace_OP_push_this(InterpInfo* info) {
  // FIXME(dpeng): Support strict mode in future
  CHECK(!info->IsModeStrict());

  if (auto self = info->This(); self.IsObject()) {
    return PushJVal(info, self);
  } else if (self.IsNull() || self.IsUndefined()) {
    return PushJVal(info, info->Global());
  } else {
    auto js_obj =
        quake_->qjs()->api.context_to_object(info->qjs_context(), self.v());
    if (::JS_IsException(js_obj)) {
      return TraceResult::BytecodeError("'this' cannot be converted to object");
    }
    auto new_val = JVal{info->qjs_context(), js_obj};
    return PushJVal(info, new_val);
  }
}

TraceResult Builder::Trace_OP_drop(InterpInfo* info) {
  PopAndRcDec(info);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_nip(InterpInfo* info) {
  TOS(v, info, 2);

  Pop();
  StkSet(1, v);

  RcDec(info, v);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_nip1(InterpInfo* info) {
  TOS(v, info, 3);
  TOS(v2, info, 2);
  TOS(v1, info, 1);

  StkSet(3, v2);
  StkSet(2, v1);
  Pop();

  RcDec(info, v);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_dup(InterpInfo* info) {
  TOS(v, info, 1);

  PushSSA(info, v);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_dup2(InterpInfo* info) {
  TOS(v2, info, 2);
  TOS(v1, info, 1);

  PushSSA(info, v2);
  PushSSA(info, v1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_dup3(InterpInfo* info) {
  TOS(v3, info, 3);
  TOS(v2, info, 2);
  TOS(v1, info, 1);

  PushSSA(info, v3);
  PushSSA(info, v2);
  PushSSA(info, v1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_dup1(InterpInfo* info) {
  TOS(v1, info, 1);
  TOS(v2, info, 2);

  StkSet(1, v2);
  Push(v1);

  RcInc(info, v2);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_insert2(InterpInfo* info) {
  TOS(v1, info, 1);
  TOS(v2, info, 2);

  Push(v1);
  StkSet(1, v2);
  StkSet(2, v1);

  RcInc(info, v1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_insert3(InterpInfo* info) {
  TOS(v1, info, 1);
  TOS(v2, info, 2);
  TOS(v3, info, 3);

  Push(v1);
  StkSet(1, v2);
  StkSet(2, v3);
  StkSet(3, v1);

  RcInc(info, v1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_insert4(InterpInfo* info) {
  TOS(v1, info, 1);
  TOS(v2, info, 2);
  TOS(v3, info, 3);
  TOS(v4, info, 4);

  Push(v1);
  StkSet(1, v2);
  StkSet(2, v3);
  StkSet(3, v4);
  StkSet(4, v1);

  RcInc(info, v1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_rot3l(InterpInfo* info) {
  TOS(tmp, info, 3);
  TOS(a2, info, 2);
  TOS(a1, info, 1);

  StkSet(3, a2);
  StkSet(2, a1);
  StkSet(1, tmp);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_rot4l(InterpInfo* info) {
  TOS(tmp, info, 4);
  TOS(a3, info, 3);
  TOS(a2, info, 2);
  TOS(a1, info, 1);

  StkSet(4, a3);
  StkSet(3, a2);
  StkSet(2, a1);
  StkSet(1, tmp);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_rot5l(InterpInfo* info) {
  TOS(tmp, info, 5);
  TOS(a4, info, 4);
  TOS(a3, info, 3);
  TOS(a2, info, 2);
  TOS(a1, info, 1);

  StkSet(5, a4);
  StkSet(4, a3);
  StkSet(3, a2);
  StkSet(2, a1);
  StkSet(1, tmp);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_rot3r(InterpInfo* info) {
  TOS(tmp, info, 1);
  TOS(a2, info, 2);
  TOS(a3, info, 3);

  StkSet(1, a2);
  StkSet(2, a3);
  StkSet(3, tmp);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_perm3(InterpInfo* info) {
  TOS(tmp, info, 2);
  TOS(a3, info, 3);

  StkSet(2, a3);
  StkSet(3, tmp);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_perm4(InterpInfo* info) {
  TOS(tmp, info, 2);
  TOS(a3, info, 3);
  TOS(a4, info, 4);

  StkSet(2, a3);
  StkSet(3, a4);
  StkSet(4, tmp);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_perm5(InterpInfo* info) {
  TOS(tmp, info, 2);
  TOS(a3, info, 3);
  TOS(a4, info, 4);
  TOS(a5, info, 5);

  StkSet(2, a3);
  StkSet(3, a4);
  StkSet(4, a5);
  StkSet(5, tmp);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_swap(InterpInfo* info) {
  TOS(tmp1, info, 4);
  TOS(tmp2, info, 3);
  TOS(a2, info, 2);
  TOS(a1, info, 1);

  StkSet(4, a2);
  StkSet(3, a1);
  StkSet(2, tmp1);
  StkSet(1, tmp2);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_swap2(InterpInfo* info) {
  TOS(tmp, info, 2);
  TOS(a1, info, 1);

  StkSet(2, a1);
  StkSet(1, tmp);

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Local Variable Handling
TraceResult Builder::Trace_OP_get_loc(InterpInfo* info) {
  auto loc_idx = info->GetU16();
  if (auto [r, ssa] = LocAt(info, loc_idx); !r) {
    return r;
  } else {
    PushSSA(info, ssa);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_set_loc(InterpInfo* info) {
  auto loc_idx = info->GetU16();
  TOS(v, info, 1);

  if (auto [r, value] = Box(info, v); !r) {
    return r;
  } else {
    // Load local variable
    RcTryDecLoc(info, loc_idx);

    // Store the value into local variable slots
    return LocSet(info, loc_idx, value);
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_put_loc(InterpInfo* info) {
  auto loc_idx = info->GetU16();
  TOS(v, info, 1);

  if (auto [r, value] = Box(info, v); !r) {
    return r;
  } else {
    // Drop current local dec if needed
    RcTryDecLoc(info, loc_idx);

    // Bump RC of TOS
    RcInc(info, value);

    // Store TOS to local
    return LocSet(info, loc_idx, value);
  }
}

// ----------------------------------------------------------------------------
// Argument Handling
TraceResult Builder::Trace_OP_get_arg(InterpInfo* info) {
  auto param_idx = info->GetU16();

  if (auto [r, ssa] = ParamAt(info, param_idx); !r) {
    return r;
  } else {
    PushSSA(info, ssa);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_set_arg(InterpInfo* info) {
  auto param_idx = info->GetU16();
  TOS(v, info, 1);

  if (auto [r, value] = Box(info, v); !r) {
    return r;
  } else {
    if (auto [r, old_param] = ParamAt(info, param_idx); !r) {
      return r;
    } else {
      RcTryDec(info, old_param);
      return ParamSet(info, param_idx, value);
    }
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_put_arg(InterpInfo* info) {
  auto param_idx = info->GetU16();
  TOS(v, info, 1);

  if (auto [r, value] = Box(info, v); !r) {
    return r;
  } else {
    if (auto [r, old_param] = ParamAt(info, param_idx); !r) {
      return r;
    } else {
      RcTryDec(info, old_param);
      RcInc(info, value);
      return ParamSet(info, param_idx, value);
    }
  }

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Arithmetic
std::pair<TraceResult, SSAVar*> Builder::DoArith(InterpInfo* info,
                                                 Opcode opcode_int,
                                                 Opcode opcode_real,
                                                 SSAVar* op1, SSAVar* op2) {
  if (op1->type().IsInt() && op2->type().IsInt()) {
    if (opcode_int == Opcode::DivInt) {
      // Perform a zero check which should be used to generate a checkpoint
      if (auto [r, new_ssa] = CheckZeroInt(info, op2); !r) {
        return std::make_pair(r, nullptr);
      } else {
        op2 = new_ssa;
      }
    }
    return std::make_pair(TraceResult::Done(),
                          graph_->AddValue(info, opcode_int, op1, op2));
  } else if (op1->type().IsReal() && op2->type().IsReal()) {
    return std::make_pair(TraceResult::Done(),
                          graph_->AddValue(info, opcode_real, op1, op2));
  } else {
    return std::make_pair(
        TraceResult::NYI("unsupported type for arithmetic operation"), nullptr);
  }
}

TraceResult Builder::Trace_OP_add(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, output] =
          DoArith(info, Opcode::AddInt, Opcode::AddReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(output);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_sub(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, output] =
          DoArith(info, Opcode::SubInt, Opcode::SubReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(output);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_mul(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, output] =
          DoArith(info, Opcode::MulInt, Opcode::MulReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(output);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_div(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, output] =
          DoArith(info, Opcode::DivInt, Opcode::DivReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(output);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_add_loc(InterpInfo* info) {
  TOS(op2, info, 1);
  auto loc_idx = info->GetU8();

  if (auto [r, op1] = LocAt(info, loc_idx); !r) {
    return r;
  } else {
    if (auto [r, output] =
            DoArith(info, Opcode::AddInt, Opcode::AddReal, op1, op2);
        !r) {
      return r;
    } else {
      Pop(1);
      Push(output);
      return TraceResult::Done();
    }
  }
}

// ----------------------------------------------------------------------------
// Bit operation
TraceResult Builder::Trace_OP_shl(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsInt() && op2->type().IsInt()) {
    op2 = graph_->AddValue(info, Opcode::BitNorm, op2);
    auto output = graph_->AddValue(info, Opcode::ShlInt, op1, op2);
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_shr(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsInt() && op2->type().IsInt()) {
    op2 = graph_->AddValue(info, Opcode::BitNorm, op2);
    auto output = graph_->AddValue(info, Opcode::ShrInt, op1, op2);
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_sar(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsInt() && op2->type().IsInt()) {
    op2 = graph_->AddValue(info, Opcode::BitNorm, op2);
    auto output = graph_->AddValue(info, Opcode::SarInt, op1, op2);
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_and(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsInt() && op2->type().IsInt()) {
    auto output = graph_->AddValue(info, Opcode::AndInt, op1, op2);
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_or(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsInt() && op2->type().IsInt()) {
    auto output = graph_->AddValue(info, Opcode::OrInt, op1, op2);
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_xor(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsInt() && op2->type().IsInt()) {
    auto output = graph_->AddValue(info, Opcode::XorInt, op1, op2);
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("invalid bit operand type");
  }
}

// ----------------------------------------------------------------------------
// Comparison
std::pair<TraceResult, SSAVar*> Builder::DoComp(InterpInfo* info,
                                                Opcode opcode_int,
                                                Opcode opcode_real, SSAVar* op1,
                                                SSAVar* op2) {
  if (op1->type().IsInt() && op2->type().IsInt()) {
    return std::make_pair(TraceResult::Done(),
                          graph_->AddValue(info, opcode_int, op1, op2));
  }
  if (op1->type().IsReal() && op2->type().IsReal()) {
    return std::make_pair(TraceResult::Done(),
                          graph_->AddValue(info, opcode_real, op1, op2));
  }

  return std::make_pair(TraceResult::NYI("invalid comparison type"), nullptr);
}

TraceResult Builder::Trace_OP_lt(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, value] = DoComp(info, Opcode::LtInt, Opcode::LtReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(value);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_lte(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, value] = DoComp(info, Opcode::LeInt, Opcode::LeReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(value);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_gt(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, value] = DoComp(info, Opcode::GtInt, Opcode::GtReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(value);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_gte(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, value] = DoComp(info, Opcode::GeInt, Opcode::GeReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(value);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_eq(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, value] = DoComp(info, Opcode::EqInt, Opcode::EqReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(value);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_neq(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto [r, value] = DoComp(info, Opcode::NeInt, Opcode::NeReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(value);
    return TraceResult::Done();
  }
}

// ----------------------------------------------------------------------------
// Unary operation
TraceResult Builder::Trace_OP_neg(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    StkSet(1, graph_->AddValue(info, Opcode::NegInt, op1));
  } else if (op1->type().IsReal()) {
    StkSet(1, graph_->AddValue(info, Opcode::NegReal, op1));
  } else {
    return TraceResult::NYI("invalid operand type: negate");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_inc(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    StkSet(1, graph_->AddValue(info, Opcode::IncInt, op1));
  } else {
    return TraceResult::NYI("invalid operand type: inc");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_dec(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    StkSet(1, graph_->AddValue(info, Opcode::DecInt, op1));
  } else {
    return TraceResult::NYI("invalid operand type: dec");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_not(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    StkSet(1, graph_->AddValue(info, Opcode::NegInt, op1));
  } else {
    return TraceResult::NYI("invalid operand type: not");
  }

  return TraceResult::Done();
}

/*
//
----------------------------------------------------------------------------
// Control Flow: Branch
TraceResult Builder::TraceBranchIf(InterpInfo* info, bool if_true) {
  TOS(cond, info, 1);

  auto jmp_pc = info->PCNext(info->GetU32() - 4);
  auto next_pc = info->PCNext(4);

  if (auto [r, ssa] = SSAToCond(info, cond); !r) {
    return r;
  } else {
    PopAndRcDec();

    auto cond = info->GetStk(info, 1).ToBool();

    if (if_true) {
      if (cond) {
        // When the observed condition is true, we bailout when the cond is
        // false.
        if (auto r = ExitIfFalse(info, cond, next_pc); !r) return r;
      } else {
        // When the observed condition is false, we bailout when the cond is
        // true
        if (auto r = ExitIfTrue(info, cond, jmp_pc); !r) return r;
      }
    } else {
      if (cond) {
        if (auto r = ExitIfFalse(info, cond, jmp_pc); !r) return r;
      } else {
        if (auto r = ExitIfTrue(info, cond, next_pc); !r) return r;
      }
    }
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_if_true(InterpInfo* info) {
  return TraceBranchIf(info, true);
}

TraceResult Builder::Trace_OP_if_false(InterpInfo* info) {
  return TraceBranchIf(info, false);
}

*/

// ----------------------------------------------------------------------------
TraceResult Builder::BeforeTrace(InterpInfo* info) {
  interp_info_ = info;
  checkpoint_done_ = false;
  rc_flushed_ = false;
  rc_queue_.clear();
  pending_checkpoint_.reset();

  return TraceResult::Done();
}

TraceResult Builder::AfterTrace(InterpInfo* info) {
  // In case we already generate the checkpoint
  if (checkpoint_done_) return TraceResult::Done();

  // Flush the RC out, if we don't have RC, which means no checkpoint has been
  // generated, then check whether we have pending checkpoint
  if (!FlushRC(info) && pending_checkpoint_) {
    UpdatePendingCheckpoint(info);
  }

  // Record last_bc
  last_bc_ = info->op();

  return TraceResult::Done();
}

TraceResult Builder::Trace(InterpInfo* info) {
  // 1) sanity check, whether all the assumption matches with each other
  CHECK(info->local_size() == local_size());
  CHECK(info->param_size() == param_size());
  CHECK(info->temp_size() == temp_size());
  CHECK(!info->IsModeStrict());

  // 2) Start to perform BeforeTrace operation
  if (auto r = BeforeTrace(info); !r) {
    return r;
  }

  // 3) Do the trace
  switch (info->op()) {
#define __(A)                                   \
  case Bytecode::A:                             \
    if (auto r = Trace_##A(info); !r) return r; \
    break;
    QUAKE_JIT_BYTECODE_LIST(__);
#undef __  // __
    default:
      // cannot handle this bytecode
      return TraceResult::NYI("cannot handle bytecode: %s",
                              info->op_name().c_str());
  }

  // 4) After the trace is done, flush all the pending operations
  return AfterTrace(info);
}

}  // namespace ir
}  // namespace quake
