#include "ir/builder.h"
#include "ir/interp-info.h"

// ----------------------------------------------------------------------------
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

      start_bc_(nullptr),
      bc_size_(0),

      rc_queue_(),
      rc_flushed_(false),
      checkpoint_(nullptr),
      checkpoint_done_(false),
      pending_checkpoint_(),
      interp_info_(nullptr),
      last_bc_(Bytecode::OP_invalid),
      jump_to_(nullptr),

      pending_jump_(),

      param_size_(0),
      local_size_(0),
      storage_(),

      phi_loop_candidate_(),

      end_of_bb0_(nullptr),
      end_of_bb1_(nullptr) {}

// ----------------------------------------------------------------------------
// Stack manipulation

std::size_t Builder::StkIdx(int idx) const {
  idx = idx < 0 ? -idx : idx;
  CHECK(static_cast<std::size_t>(idx) <= storage_.size());

  auto real_index = storage_.size() - static_cast<std::size_t>(idx);
  CHECK(real_index >= temp_base());
  return real_index;
}

std::pair<TraceResult, SSAVar*> Builder::StkAt(InterpInfo* info, int index) {
  auto real_index = StkIdx(index);

  if (auto ssa_var = storage_[real_index]; !ssa_var) {
    // If a temporary value is not loaded, we should just load it now
    auto memory_node =
        graph_->AddValue(info, Opcode::kTempGet,
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
                               const void* bc, Node* n) {
  // A single bytecode cannot emit 2 checkpoint, unless otherwise specified
  CHECK(!checkpoint_done_);

  checkpoint_ = graph_->AddCheckpoint(info, checkpoint_, this, reason, bc, n);
  checkpoint_done_ = true;
}

void Builder::UpdateCheckpointAfter(InterpInfo* info, Checkpoint::Reason reason,
                                    Node* n) {
  UpdateCheckpoint(info, reason, info->next_pc(), n);
}

void Builder::AddCheckpointAfter(Checkpoint::Reason reason,
                                 const void* bc_position, Node* node) {
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
    auto node = graph_->AddNodeInternal(info, Opcode::kRcInc, var);
    rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
  } else if (var->type().IsBox()) {
    auto node = graph_->AddNodeInternal(info, Opcode::kRcTryInc, var);
    rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
  }
}

void Builder::RcDec(InterpInfo* info, SSAVar* var) {
  if (var->type().IsHeap()) {
    auto node = graph_->AddNodeInternal(info, Opcode::kRcDec, var);
    rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
  } else if (var->type().IsBox()) {
    auto node = graph_->AddNodeInternal(info, Opcode::kRcTryDec, var);
    rc_queue_.push_back(graph_->SetNodeInternalSSA(node));
  }
}

bool Builder::FlushRC(InterpInfo* info) {
  if (!rc_queue_.empty()) {
    rc_flushed_ = true;

    auto rc_op =
        graph_->AddOp(info, Opcode::kRcOp, static_cast<int>(rc_queue_.size()));

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
        graph_->AddValue(info, Opcode::kTempGet,
                         graph_->KI32(info, static_cast<int>(real_index)));
    RcDec(info, memory_node);
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
    case JVal::kUninitialized:
    case JVal::kNull:
    case JVal::kUndefined: {
      CHECK(!rc_flushed_);
      auto guard_node = graph_->AddValue(info, Opcode::kBxGuard,
                                         JValT2Type(jval.type()), node);
      OnGuard(guard_node);
      return std::make_pair(TraceResult::Done(), guard_node);
    }

    default:
      return std::make_pair(TraceResult::NYI("unknown type"), nullptr);
  }
}

// ----------------------------------------------------------------------------
// Box is essentially a NO-OP. In our IR builder, we do not have any manual
// boxing operation at all, basically, box is performed implicitly at various
// sync point, ie checkpoint, external call point etc, but they are never been
// part of our code. The JIT code never allow boxing but instead using unboxed
// value + guard. The JIT code never should deal with box value at all
//
// Our code is really a type sanity check here
TraceResult Builder::Box(InterpInfo* info, SSAVar* node) {
  CHECK(!node->type().IsBox());

  // Try to perform some optimization here for boxing, some known type can
  // directly convert to certain boxing value, just in case we can do this
  switch (node->type().tag()) {
    case Type::kInt:
    case Type::kFloat64:
    case Type::kBool:
    case Type::kNull:
    case Type::kUndefined:
    case Type::kUninitialized:
      break;

    case Type::kI64:
      UNREACHABLE() << "we should not have I64 for boxing";
      break;

    default:
      return TraceResult::NYI("unsupported type");
  }

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Check whether the node is a int zero or not. The node must be a int already
std::pair<TraceResult, SSAVar*> Builder::CheckZeroInt(InterpInfo* info,
                                                      SSAVar* node) {
  CHECK(node->type().IsInt());
  auto guard_node = graph_->AddValue(info, Opcode::kGNotZeroInt, node);

  OnGuard(guard_node);
  return std::make_pair(TraceResult::Done(), guard_node);
}

void Builder::AddPhiLoopCandidate(ValType vtype, int index) {
  auto itr =
      std::find_if(phi_loop_candidate_.begin(), phi_loop_candidate_.end(),
                   [&](const PhiVal& pv) -> bool {
                     if (pv.vtype == vtype && pv.index == index) {
                       return true;
                     }
                     return false;
                   });

  if (itr == phi_loop_candidate_.end()) {
    phi_loop_candidate_.emplace_back(vtype, index);
  }
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
        graph_->AddValue(info, Opcode::kLocGet, graph_->KI32(info, idx));

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

bool Builder::CheckLocSlotUninitialized(InterpInfo* info, int idx) {
  if (auto loc = local()[idx]; !loc) {
    // notes we do not bother generating the IR to load it and unbox it, but
    // just check its current value, whether it is a Unitialized one, if so
    // just bailout
    auto local_var = info->GetLocal(idx);
    return local_var.IsUninitialized();
  } else {
    return loc->type().IsUninitialized();
  }
}

TraceResult Builder::LocSet(InterpInfo* info, int local_idx, SSAVar* value) {
  CHECK(static_cast<std::uint32_t>(local_idx) < local_size());
  local()[local_idx] = value;

  // all the store to the local variable slot can be considered as phi loop
  // candidate
  AddPhiLoopCandidate(kLocal, local_idx);

  // add a kLocSet at current IR graph to allow future rewriting of the graph
  graph_->AddOp(info, Opcode::kLocSet, graph_->KI32(info, local_idx), value);

  return TraceResult::Done();
}

void Builder::RcTryDecLoc(InterpInfo* info, int loc_idx) {
  CHECK(static_cast<std::uint32_t>(loc_idx) < local_size());

  if (auto loc = local()[loc_idx]; !loc) {
    auto memory_node =
        graph_->AddValue(info, Opcode::kLocGet, graph_->KI32(info, loc_idx));
    RcDec(info, memory_node);
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
        graph_->AddValue(info, Opcode::kParamGet, graph_->KI32(info, idx));

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

  AddPhiLoopCandidate(kParam, param_idx);

  // add a kLocSet at current IR graph to allow future rewriting of the graph
  graph_->AddOp(info, Opcode::kParamSet, graph_->KI32(info, param_idx), value);

  return TraceResult::Done();
}

void Builder::RcTryDecParam(InterpInfo* info, int idx) {
  CHECK(static_cast<std::uint32_t>(idx) < param_size());
  if (auto p = param()[idx]; !p) {
    auto memory_node =
        graph_->AddValue(info, Opcode::kParamGet, graph_->KI32(info, idx));

    RcDec(info, memory_node);
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

// The set_loc duplicate the TOS value and set it to current local slot, leaving
// the TOS untouched
TraceResult Builder::Trace_OP_put_loc(InterpInfo* info) {
  auto loc_idx = info->GetU16();
  TOS(value, info, 1);

  if (auto r = Box(info, value); !r) {
    return r;
  } else {
    // Drop current local dec if needed
    RcTryDecLoc(info, loc_idx);

    // Pop the TOS out, notes this does not generate any reference counting
    // mutation code
    Pop(1);

    // Store the value into local variable slots
    return LocSet(info, loc_idx, value);
  }
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_set_loc(InterpInfo* info) {
  auto loc_idx = info->GetU16();
  TOS(value, info, 1);

  if (auto r = Box(info, value); !r) {
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

TraceResult Builder::Trace_OP_set_loc_uninitialized(InterpInfo* info) {
  auto loc_idx = info->GetU16();
  return LocSet(info, loc_idx, graph_->KUninit(info));
}

TraceResult Builder::Trace_OP_get_loc_check(InterpInfo* info) {
  auto loc_idx = info->GetU16();

  if (auto [r, ssa] = LocAt(info, loc_idx); !r) {
    return r;
  } else {
    // check whether the value been stored at current position is a
    if (ssa->type().IsUninitialized()) {
      return TraceResult::ReferenceError("uninitialized variable");
    }

    PushSSA(info, ssa);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_put_loc_check(InterpInfo* info) {
  auto loc_idx = info->GetU16();
  TOS(value, info, 1);

  // Check original local slot contains an uninitialized value or not
  if (CheckLocSlotUninitialized(info, loc_idx)) {
    return TraceResult::ReferenceError("uninitialized variable");
  }

  if (auto r = Box(info, value); !r) {
    return r;
  } else {
    // Drop current local dec if needed
    RcTryDecLoc(info, loc_idx);

    // Pop the TOS out, notes this does not generate any reference counting
    // mutation code
    Pop(1);

    // Store the value into local variable slots
    return LocSet(info, loc_idx, value);
  }
  return TraceResult::Done();
}

// This bytecode should never be triggered more than once. It can only happen
// when we inline the function frame ...
TraceResult Builder::Trace_OP_put_loc_check_init(InterpInfo* info) {
  auto loc_idx = info->GetU16();

  if (!CheckLocSlotUninitialized(info, loc_idx)) {
    return TraceResult::ReferenceError("`this` can be initialized once");
  }

  if (auto [r, ssa] = LocAt(info, loc_idx); !r) {
    return r;
  } else {
    PushSSA(info, ssa);
    return TraceResult::Done();
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
  TOS(value, info, 1);

  if (auto r = Box(info, value); !r) {
    return r;
  } else {
    if (auto [r, old_param] = ParamAt(info, param_idx); !r) {
      return r;
    } else {
      RcDec(info, old_param);
      return ParamSet(info, param_idx, value);
    }
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_put_arg(InterpInfo* info) {
  auto param_idx = info->GetU16();
  TOS(value, info, 1);

  if (auto r = Box(info, value); !r) {
    return r;
  } else {
    if (auto [r, old_param] = ParamAt(info, param_idx); !r) {
      return r;
    } else {
      RcDec(info, old_param);
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
    if (opcode_int == Opcode::kDivInt) {
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
          DoArith(info, Opcode::kAddInt, Opcode::kAddReal, op1, op2);
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
          DoArith(info, Opcode::kSubInt, Opcode::kSubReal, op1, op2);
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
          DoArith(info, Opcode::kMulInt, Opcode::kMulReal, op1, op2);
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
          DoArith(info, Opcode::kDivInt, Opcode::kDivReal, op1, op2);
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
            DoArith(info, Opcode::kAddInt, Opcode::kAddReal, op1, op2);
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
    op2 = graph_->AddValue(info, Opcode::kBitNorm, op2);
    auto output = graph_->AddValue(info, Opcode::kShlInt, op1, op2);
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
    op2 = graph_->AddValue(info, Opcode::kBitNorm, op2);
    auto output = graph_->AddValue(info, Opcode::kShrInt, op1, op2);
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
    op2 = graph_->AddValue(info, Opcode::kBitNorm, op2);
    auto output = graph_->AddValue(info, Opcode::kSarInt, op1, op2);
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
    auto output = graph_->AddValue(info, Opcode::kAndInt, op1, op2);
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
    auto output = graph_->AddValue(info, Opcode::kOrInt, op1, op2);
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
    auto output = graph_->AddValue(info, Opcode::kXorInt, op1, op2);
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

  if (auto [r, value] = DoComp(info, Opcode::kLtInt, Opcode::kLtReal, op1, op2);
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

  if (auto [r, value] = DoComp(info, Opcode::kLeInt, Opcode::kLeReal, op1, op2);
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

  if (auto [r, value] = DoComp(info, Opcode::kGtInt, Opcode::kGtReal, op1, op2);
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

  if (auto [r, value] = DoComp(info, Opcode::kGeInt, Opcode::kGeReal, op1, op2);
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

  if (auto [r, value] = DoComp(info, Opcode::kEqInt, Opcode::kEqReal, op1, op2);
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

  if (auto [r, value] = DoComp(info, Opcode::kNeInt, Opcode::kNeReal, op1, op2);
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
    StkSet(1, graph_->AddValue(info, Opcode::kNegInt, op1));
  } else if (op1->type().IsReal()) {
    StkSet(1, graph_->AddValue(info, Opcode::kNegReal, op1));
  } else {
    return TraceResult::NYI("invalid operand type: negate");
  }

  return TraceResult::Done();
}

// The JS allows + as unary operator, which is a no-op for any number, but will
// become a meta call when the type is not number
TraceResult Builder::Trace_OP_plus(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt() || op1->type().IsReal()) {
    // do nothing, but just return
  } else {
    return TraceResult::NYI("invalid operand type: plus");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_inc(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    StkSet(1, graph_->AddValue(info, Opcode::kIncInt, op1));
  } else {
    return TraceResult::NYI("invalid operand type: inc");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_dec(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    StkSet(1, graph_->AddValue(info, Opcode::kDecInt, op1));
  } else {
    return TraceResult::NYI("invalid operand type: dec");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_not(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    StkSet(1, graph_->AddValue(info, Opcode::kNegInt, op1));
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
  jump_to_ = nullptr;
  bc_size_ = 0;

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

  switch (info->op()) {
    case Bytecode::OP_goto:
    case Bytecode::OP_if_true:
    case Bytecode::OP_if_false:
      CHECK(jump_to_);
      break;
    default:
      break;
  }

  if (jump_to_ == start_bc_) {
    return TraceResult::Stop();
  } else {
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace(InterpInfo* info) {
  // 1) sanity check, whether all the assumption matches with each other
  CHECK(info->local_size() == local_size());
  CHECK(info->param_size() == param_size());
  CHECK(!info->IsModeStrict());

  ++bc_size_;

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

TraceResult Builder::Setup(InterpInfo* info) {
  CHECK(!start_bc_);
  start_bc_ = info->start_pc();

  if (info->IsModeStrict()) {
    return TraceResult::NYI("not implement(STRICT)");
  }

  storage_.resize(info->slot_size());
  param_size_ = info->param_size();
  local_size_ = info->local_size();

  auto start = graph_->AddControlFlow(info, Opcode::kStart);
  checkpoint_ = graph_->AddCheckpoint(
      info, checkpoint_, this, Checkpoint::kStart, info->start_pc(), start);

  return TraceResult::Done();
}

TraceResult Builder::Stop(InterpInfo* info) {
  Reemit(info);
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Reemit of graph
//
// The reemit phase is performed AFTER the whole graph is been done and form
// the whole graph into a special graph that contains 2 seconds
//
// 1) one single iteration, ie like loop peeling
// 2) rest of the loop body

void Builder::Reemit(InterpInfo* info) {
  SSAVarMappings mappings;

  // marking the graph to contain the BB0 marker
  auto start_of_body = graph_->AddControlFlow(info, Opcode::kBB0);
  end_of_bb0_ = graph_->SetNodeInternalSSA(start_of_body);
  graph_->set_end_of_bb0(start_of_body);

  // Now let's revisit each already visited node and forms the loop body
  // iteration

  // 1) Create all the phi candidate in slot that has been stored during the
  //    first peeling of the loop
  InsertPhiCandidate();

  // 2) Now start to visit the graph until we hit the BB0. The algorithm is
  //    as following.
  //
  //    Since our initial graph is built as SSA, each instruction's operand,
  //    will show up in the graph ealier than the instruction itself. It makes
  //    sure that when we visit a certain instruction, and its operand has been
  //    transformed into correct one.
  //
  //    We setup a mappnig table, maps the old graph's operand, ie SSAVar* into
  //    a new SSAVar*. This essentially makes the second interpretation of IR
  //    graph using a SSAVar* as index to its operand(value) instead of the old
  //    stack index. Which still maintains all the needed information.
  //
  //    During the revisiting, we just need to *rewrite* all the kLocGet and
  //    kParamGet and kTempGet to corresponding new value created in previous
  //    peeled loop, ie the value shows up the current storage_ field. For
  //    all the other part we can just deep clone them. Notes, since the
  //    kLocGet/kParamGet/kTempGet shows up in different value, it makes
  //    the unboxing of these value unneeded, which means we should perform a
  //    little bit optimization of Unbox operation. Notes, we can feed the
  //    instruction back to a separate folding pass to do the optimization
  //    either But we are pretty sure what gonna happen here. It just means the
  //    newly created kLocGet/kParamGet/kTempGet will need to back propagte its
  //    type information back to the Unbox/TryUnbox instruction that guards it
  //    See above LocAt/ParamAt/StkAt for detail. Pretty simple though :)
  //    Other instructions are no specail, just do a *deep* clone and then
  //    setup the map entry, that's it. We gonna have a correct graph. The
  //    graph, if the jump back edge from the loop body is from left side, can
  //    be viewed as a number *6* in glyph. We just call it six graph :)

  graph_->ForeachBB0([&](Node* node) {
    SSAVar* replacement = nullptr;
    switch (node->opcode()) {
      case Opcode::kStart:
        return;

      // ----------------------------------------------------------------------
      // Sanity check
      case Opcode::kPhiLoop:
        UNREACHABLE() << "cannot have phi in first iteration";
        break;

      // ----------------------------------------------------------------------
      // Special node requires special mapping and special buildup
      case Opcode::kLocGet:
        replacement = RewriteLocGet(node, &mappings);
        break;
      case Opcode::kParamGet:
        replacement = RewriteParamGet(node, &mappings);
        break;
      case Opcode::kTempGet:
        replacement = RewriteTempGet(node, &mappings);
        break;
      case Opcode::kBxGuard:
        replacement = RewriteBxGuard(node, &mappings);
        break;

      // ----------------------------------------------------------------------
      // The first iteration(generation) of the IR graph will emit extra
      // node kLocSet/kParamSet which is just been used to fixup PHI node
      // in second pass. These 2 instructions will NOT generate any LIR
      // and will be eliminated afterwards
      case Opcode::kLocSet:
        MarkLocSet(mappings, node);
        break;

      case Opcode::kParamSet:
        MarkParamSet(mappings, node);
        break;

      default: {
        if (auto output = RewriteClone(node, &mappings); output) {
          replacement = output;
          if (auto out = node->out(); out) {
            SetMappings(&mappings, out, output);
          }
        }
      } break;
    }

    // snapshot generation. If node has snapshot requirement, just generate it
    if (auto snap = node->snapshot(); snap) {
      CHECK(replacement);

      RewriteCheckpoint(node, replacement->node_checked(), mappings);
    }

    // if the node jumps to somewhere, it needs to recover from it as well
    if (node->HasExit()) {
      CHECK(replacement);

      RewriteExit(node, replacement->node_checked(), mappings);
    }
  });

  // After the above operation, we should have already setup the stuff we need
  // for the loop body, now we need to insert an unconditional jump points back
  // to the BB0 position.
  auto back_jump = graph_->AddControlFlow(info, Opcode::kJump, end_of_bb0_);
  end_of_bb1_ = graph_->SetNodeInternalSSA(back_jump);
  graph_->set_end_of_bb1(back_jump);

  // --------------------------------------------------------------------------
  // 3) Fixup the phi generation
  FixupPhi();

  // --------------------------------------------------------------------------
  // 4) Add kEnd
  graph_->AddControlFlow(info, Opcode::kEnd);
}

void Builder::MarkLocSet(const SSAVarMappings& mappings, Node* loc_set) {
  CHECK(loc_set->size() == 2);
  auto idx = loc_set->AtI32(0);
  auto value = loc_set->At(1);
  auto new_value = GetMappings(mappings, value);
  local()[idx] = new_value;

  loc_set->SetDead();
}

void Builder::MarkParamSet(const SSAVarMappings& mappings, Node* param_set) {
  CHECK(param_set->size() == 2);
  auto idx = param_set->AtI32(0);
  auto value = param_set->At(1);
  auto new_value = GetMappings(mappings, value);
  param()[idx] = new_value;

  param_set->SetDead();
}

void Builder::InsertPhiCandidate() {
  for (auto& entry : phi_loop_candidate_) {
    switch (entry.vtype) {
      case kParam: {
        auto old = param()[entry.index];
        auto phi = graph_->AddValue(old->node_checked(), Opcode::kPhiLoop, old,
                                    nullptr);
        entry.phi = phi;
        param()[entry.index] = phi;
      } break;

      case kLocal: {
        auto old = local()[entry.index];
        auto phi = graph_->AddValue(old->node_checked(), Opcode::kPhiLoop, old,
                                    nullptr);
        entry.phi = phi;
        local()[entry.index] = phi;
      } break;
      default:
        UNREACHABLE();
        break;
    }
  }
}

void Builder::FixupPhi() {
  auto eliminate_or_fixup_phi = [](SSAVar* phi, SSAVar* current) {
    auto node = phi->node_checked();
    CHECK(node->opcode() == Opcode::kPhiLoop);
    CHECK(node->Rhs() == nullptr);

    // If the phi is a trivial assignment, ie assigning back the original value
    // then just eliminate it, ie use its LHS to replace itself
    if (phi == current) {
      auto value = node->Remove(0);
      CHECK(value);
      node->ReplaceAndSetDead(value);
    } else {
      node->Set(1, current);
      CHECK(node->InferPhiType());
    }
  };

  for (auto& entry : phi_loop_candidate_) {
    switch (entry.vtype) {
      case kParam: {
        auto current = param()[entry.index];
        eliminate_or_fixup_phi(entry.phi, current);
      } break;

      case kLocal: {
        auto current = local()[entry.index];
        eliminate_or_fixup_phi(entry.phi, current);
      } break;
      default:
        UNREACHABLE();
        break;
    }
  }
}

void Builder::RewriteCheckpoint(Node* node, Node* replacement,
                                const SSAVarMappings& mappings) {
  auto snap = node->snapshot();
  CHECK(snap);

  auto reason = snap->reason();
  auto bc_pos = snap->bc_pos();

  // Notes, if we can take snapshot, the default behavior will use the current
  // evaluation stack to compose the object. However the re-emit phase will
  // never modify the stack, since we loose the information, so the data
  // been re-emitted is the data that is not been used anymore. This is fine,
  // since we just need to use the mapping to remap all the value inside of
  // slot
  checkpoint_ = graph_->AddCheckpoint(node, checkpoint_, this, reason, bc_pos,
                                      replacement);

  // replace all the checkpoint's entry with new value
  for (auto& e : checkpoint_->entry_vector()) {
    if (auto rvalue = GetMappings(mappings, e.value());
        rvalue && rvalue != e.value()) {
      e.SetValue(rvalue);
    }
  }
}

void Builder::RewriteExit(Node* node, Node* replacement,
                          const SSAVarMappings&) {
  CHECK(node->HasExit());
  replacement->BailoutTo(checkpoint_);
}

SSAVar* Builder::RewriteValGet(Node* node, SSAVarMappings* mappings,
                               SSAVar** table) {
  CHECK(node->size() == 1);
  CHECK(node->IsValue());
  CHECK(node->out());

  auto imm = node->Opr()->node();
  CHECK(imm->opcode() == Opcode::kKI32);
  auto val_index = imm->Int32();
  auto val = table[val_index];
  CHECK(val);

  SetMappings(mappings, node->out(), val);
  return val;
}

SSAVar* Builder::RewriteLocGet(Node* node, SSAVarMappings* mappings) {
  return RewriteValGet(node, mappings, local());
}

SSAVar* Builder::RewriteParamGet(Node* node, SSAVarMappings* mappings) {
  return RewriteValGet(node, mappings, param());
}

SSAVar* Builder::RewriteTempGet(Node* node, SSAVarMappings* mappings) {
  return RewriteValGet(node, mappings, temp());
}

SSAVar* Builder::RewriteBxGuard(Node* node, SSAVarMappings* mappings) {
  CHECK(node->opcode() == Opcode::kBxGuard);
  auto operand = node->Opr();
  SSAVar* replacement = nullptr;

  // Notes, all the loop carrier dependency will not mutate our value's type
  // in our assumption for now, but this assumption may not be correct ...
  if (!operand->type().IsBox()) {
    // If the type is not box, we don't need to unbox it but just alias the
    // pointer.
    const auto& old_type = node->out()->type();
    CHECK(old_type == operand->type());
    SetMappings(mappings, node->out(), operand);
    replacement = operand;
  } else {
    // Just do a deep copy here
    replacement = RewriteClone(node, mappings);
    SetMappings(mappings, node->out(), replacement);
  }

  return replacement;
}

SSAVar* Builder::RewriteCloneRc(Node* node, SSAVarMappings* mappings) {
  // Clone contains internal node, which is not tracked so there're no
  // mappings from mappings
  CHECK(node->opcode() == Opcode::kRcOp);

  auto rnode = graph_->AddNode(node, Opcode::kRcOp, node->size());

  for (int i = 0; i < node->size(); i++) {
    auto old = node->At(i);
    auto node = old->node_checked();
    Node* r = nullptr;

    switch (node->opcode()) {
      case Opcode::kRcInc:
      case Opcode::kRcDec:
      case Opcode::kRcTryInc:
      case Opcode::kRcTryDec: {
        CHECK(node->size() == 1);
        auto old_opr = node->Opr();
        auto new_opr = GetMappingsChecked(*mappings, old_opr);
        r = graph_->AddNodeInternal(node, node->opcode(), new_opr);
      } break;
      default:
        UNREACHABLE();
        break;
    }

    rnode->Set(i, graph_->SetNodeInternalSSA(r));
  }

  return graph_->SetNodeInternalSSA(rnode);
}

// deep clone the node
SSAVar* Builder::RewriteClone(Node* node, SSAVarMappings* mappings) {
  SSAVar *op1, *op2;

  // Special cases, RcInc/RcDec/RcTryInc/RcTryDec will use internal node
  // which does not have record in mappings and require deep copy.
  switch (node->opcode()) {
    case Opcode::kRcInc:
    case Opcode::kRcDec:
    case Opcode::kRcTryInc:
    case Opcode::kRcTryDec:
      UNREACHABLE() << "internal node opcode should never show up";
    case Opcode::kRcOp:
      return RewriteCloneRc(node, mappings);
    default:
      break;
  }

  if (node->IsValue()) {
    const auto arity = node->size();

    switch (arity) {
      case 0:
        return graph_->AddValue(node, node->opcode());

      case 1:
        op1 = GetMappingsChecked(*mappings, node->Opr());
        return graph_->AddValue(node, node->opcode(), op1);

      case 2:
        op1 = GetMappingsChecked(*mappings, node->Lhs());
        op2 = GetMappingsChecked(*mappings, node->Rhs());
        return graph_->AddValue(node, node->opcode(), op1, op2);

      default: {
        auto replacement = graph_->AddValue(node, node->opcode(), arity);
        for (int i = 0; i < arity; i++) {
          auto v = GetMappingsChecked(*mappings, node->At(i));
          replacement->node_checked()->Set(i, v);
        }
        return replacement;
      }
    }
  } else if (node->IsOp()) {
    const auto arity = node->size();

    switch (arity) {
      case 0:
        graph_->AddOp(node, node->opcode());
        return nullptr;

      case 1:
        op1 = GetMappingsChecked(*mappings, node->Opr());
        graph_->AddOp(node, node->opcode(), op1);
        return nullptr;

      case 2:
        op1 = GetMappingsChecked(*mappings, node->Lhs());
        op2 = GetMappingsChecked(*mappings, node->Rhs());
        graph_->AddOp(node, node->opcode(), op1, op2);
        return nullptr;

      default: {
        auto rnode = graph_->AddOp(node, node->opcode(), arity);
        CHECK(rnode);
        for (int i = 0; i < arity; i++) {
          auto v = GetMappingsChecked(*mappings, node->At(i));
          rnode->Set(i, v);
        }
        return nullptr;
      }
    }
  } else {
    CHECK(node->IsControlFlow());
    const auto arity = node->size();

    switch (arity) {
      case 0:
        graph_->AddControlFlow(node, node->opcode());
        return nullptr;
      case 1:
        op1 = GetMappingsChecked(*mappings, node->Opr());
        graph_->AddControlFlow(node, node->opcode());
        return nullptr;

      default:
        UNREACHABLE();
        return nullptr;
    }
  }
}

SSAVar* Builder::GetMappings(const SSAVarMappings& mappings, SSAVar* var) {
  // notes constant should not be rewritting
  if (var->node()->IsConst()) return var;

  // the only internal node can show up here is the constant node
  CHECK(!var->node()->IsInternal());

  // try to find it in the mappings
  if (auto itr = mappings.find(var); itr != mappings.end()) {
    return itr->second;
  } else {
    return nullptr;
  }
}

SSAVar* Builder::GetMappingsChecked(const SSAVarMappings& mappings,
                                    SSAVar* var) {
  auto v = GetMappings(mappings, var);
  CHECK(v);
  return v;
}

// Since our graph is SSA, we cannot redefine a same symbol
void Builder::SetMappings(SSAVarMappings* mappings, SSAVar* from, SSAVar* to) {
  auto result = mappings->insert(std::make_pair(from, to));
  CHECK(result.second);
}

}  // namespace ir
}  // namespace quake
