#include "ir/builder.h"
#include "ir/interp-info.h"
#include "ir/loop-exit-analysis.h"
#include "ir/optimization/fold.h"
#include "ir/optimization/dce.h"
#include "ir/pass.h"

#include "flags.h"
#include "quake-rt.h"

// Notes: About dual-number mode handling
//
// The QuickJS interpreter uses a dual-number mode to handle internal ecmascript
// number representation. Ie it allows int32 to represent a number and transfer
// to real(double) representation internally if needed. Typically the transfer
// happens at 1) certain operation will generate different result, ie div, mod,
// pow or 2) overflow/underflow. But QuickJS interpreter has some sloppy
// behavior, ie the OP_post_inc/OP_post_dec will just transfer to real number
// regardlessly. The semantic of dual-number should be always the same when
// cases that just follows in the range of int32, ie as long as the number is
// in the int32 range, regardlessly whether you are using int or double, the
// result should be just the same.
//
// Our JIT compiler tries to stick to the type as much as possible, since we
// do allow dual type for node, but have to represent exact type for now. When
// certain ops, like OP_post_inc/OP_post_dec happened, if we stick to the
// interpreter behavior, then the operand will become to be real number type
// regardlessly, which makes our PHI needs to work against both type. This is
// not gonna work for now. Therefore, we generate different behavior than
// QuickJS interpreter for certain opcode to make sure our JIT code works.

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

namespace {

template <typename T>
bool FoldIntComparison(T value, Opcode opcode) {
  switch (opcode) {
    case Opcode::kGIsIntPos:
      return value > 0;
    case Opcode::kGIsIntPosOrZero:
      return value >= 0;
    case Opcode::kGIsIntNeg:
      return value < 0;
    case Opcode::kGIsNotIntMax:
      return value != std::numeric_limits<T>::max();
    case Opcode::kGIsNotIntMin:
      return value != std::numeric_limits<T>::min();
    case Opcode::kGIsNotZero:
      return value != 0;
    default:
      UNREACHABLE();
      return false;
  }
}

}  // namespace

Builder::Builder(Quake* q, Graph* g)
    : quake_(q),
      graph_(g),

      start_bc_(nullptr),
      bc_size_(0),

      rc_queue_(),
      rc_flushed_(false),
      checkpoint_(nullptr),
      pending_checkpoint_(),

      interp_info_(nullptr),
      jump_to_(nullptr),
      call_depth_(0),

      trace_info_(),

      frame_(),

      phi_loop_candidate_(),
      dup_list_(),

      nested_loop_(),
      nested_loop_trace_state_(NestedLoopTraceState::kNone),

      end_of_bb0_(nullptr),
      end_of_bb1_(nullptr),
      is_reemit_(false) {}

Checkpoint* Builder::AddCheckpointToGraph(BytecodeInfoProvider* info,
                                          Checkpoint::Reason reason,
                                          Node* current_node, bool is_after) {
  // Should refresh current frame's bytecode pointer which is been used by the
  // checkpoint construction code.
  //
  // Notes(dpeng): this is for handling special instruction, which does not
  // map to any instruction. For example kStart/kBB1 etc ...
  auto bc_info = info->bytecode_info();
  switch (reason) {
    case Checkpoint::kStart:
    case Checkpoint::kBB1:
      // For kStart/kBB1, it does not have next pc, since it happened at a
      // pseudo-instruction, which can be pictured as this instruction does
      // not consume any bytecode, and it has length zero 0, so its current
      // and next instruction both points to the same instruction, ie next
      // real bytecode instruction.
      cur_frame().cur_bc_ = bc_info.current;
      cur_frame().next_bc_ = bc_info.current;
      break;

    default:
      cur_frame().cur_bc_ = bc_info.current;
      cur_frame().next_bc_ = bc_info.next;
      break;
  }

  if (is_after) {
    checkpoint_ = graph_->AddCheckpointAfter(info, checkpoint_, this, reason,
                                             current_node);
  } else {
    checkpoint_ = graph_->AddCheckpointBefore(info, checkpoint_, this, reason,
                                              current_node);
  }

  return checkpoint_;
}

Checkpoint* Builder::DoSnapshot(BytecodeInfoProvider* info,
                                Checkpoint::Reason reason, bool is_after) {
  auto bc_info = info->bytecode_info();
  switch (reason) {
    case Checkpoint::kStart:
    case Checkpoint::kBB1:
      // For kStart/kBB1, it does not have next pc, since it happened at a
      // pseudo-instruction, which can be pictured as this instruction does
      // not consume any bytecode, and it has length zero 0, so its current
      // and next instruction both points to the same instruction, ie next
      // real bytecode instruction.
      cur_frame().cur_bc_ = bc_info.current;
      cur_frame().next_bc_ = bc_info.current;
      break;

    default:
      cur_frame().cur_bc_ = bc_info.current;
      cur_frame().next_bc_ = bc_info.next;
      break;
  }
  if (is_after) {
    checkpoint_ = graph_->SnapshotAfter(info, this, reason);
  } else {
    checkpoint_ = graph_->SnapshotBefore(info, this, reason);
  }
  return checkpoint_;
}

Checkpoint* Builder::SnapshotAfter(BytecodeInfoProvider* info,
                                   Checkpoint::Reason reason) {
  return DoSnapshot(info, reason, true);
}

Checkpoint* Builder::SnapshotBefore(BytecodeInfoProvider* info,
                                    Checkpoint::Reason reason) {
  return DoSnapshot(info, reason, false);
}

Checkpoint* Builder::ReemitCheckpointToGraph(BytecodeInfoProvider* info,
                                             Checkpoint* prev_snap,
                                             Node* current_node,
                                             bool is_after) {
  // Should refresh current frame's bytecode pointer which is been used by the
  // checkpoint construction code.
  //
  // Notes(dpeng): this is for handling special instruction, which does not
  // map to any instruction. For example kStart/kBB1 etc ...
  auto bc_info = info->bytecode_info();
  const auto reason = prev_snap->reason();

  switch (reason) {
    case Checkpoint::kStart:
    case Checkpoint::kBB1:
      // For kStart/kBB1, it does not have next pc, since it happened at a
      // pseudo-instruction, which can be pictured as this instruction does
      // not consume any bytecode, and it has length zero 0, so its current
      // and next instruction both points to the same instruction, ie next
      // real bytecode instruction.
      cur_frame().cur_bc_ = bc_info.current;
      cur_frame().next_bc_ = bc_info.current;
      break;

    default:
      cur_frame().cur_bc_ = bc_info.current;
      cur_frame().next_bc_ = bc_info.next;
      break;
  }

  if (is_after) {
    checkpoint_ = graph_->ReemitCheckpointAfter(info, checkpoint_, this,
                                                prev_snap, current_node);
  } else {
    checkpoint_ = graph_->ReemitCheckpointBefore(info, checkpoint_, this,
                                                 prev_snap, current_node);
  }

  return checkpoint_;
}

// ----------------------------------------------------------------------------
// Cast operations
SSAVar* Builder::UpgradeToFloat64IfNeeded(Opcode opcode, Node* node,
                                          SSAVar* opr) {
  if (OpcodeIsArithmetic(opcode) && OpcodeNeedFloatOperand(opcode)) {
    if (opr->type().IsInt()) {
      auto n = opr->node_checked();
      if (n->opcode() == Opcode::kKI32) {
        return graph_->KFloat64(node, static_cast<double>(n->Int32()));
      } else {
        return graph_->AddValue(node, Opcode::kI2F, opr);
      }
    }
  }

  return opr;
}

std::optional<SSAVar*> Builder::CastToString(InterpInfo* info, SSAVar* opr) {
  switch (opr->type().tag()) {
    case Type::kInt: {
      auto n = opr->node_checked();
      if (n->opcode() == Opcode::kKI32) {
        return graph_->LoadStr(
            info, JVal::ToString(quake_->js_context(), n->Int32()));
      } else {
        return graph_->AddValue(info, Opcode::kI2S, opr);
      }
    }

    case Type::kFloat64: {
      auto n = opr->node_checked();
      if (n->opcode() == Opcode::kKFloat64) {
        return graph_->LoadStr(
            info, JVal::ToString(quake_->js_context(), n->Float64()));
      } else {
        return graph_->AddValue(info, Opcode::kF2S, opr);
      }
    }
    case Type::kBool: {
      auto n = opr->node_checked();
      if (n->opcode() == Opcode::kKBool) {
        return graph_->LoadStr(info, (n->Bool() ? "true" : "false"));
      } else {
        return graph_->AddValue(info, Opcode::kB2S, opr);
      }
    }
    case Type::kObject:
      return graph_->AddValue(info, Opcode::kO2S, opr);

    case Type::kNull:
      return graph_->LoadStr(info, "null");

    case Type::kUndefined:
      return graph_->LoadStr(info, "undefined");

    case Type::kString:
      return opr;

    default:
      return {};
  }
}

SSAVar* Builder::MustCastToString(InterpInfo* info, SSAVar* opr) {
  auto v = CastToString(info, opr);
  CHECK(v);
  return *v;
}

std::optional<SSAVar*> Builder::CastToFloat64(InterpInfo* info, SSAVar* opr) {
  switch (opr->type().tag()) {
    case Type::kInt: {
      auto n = opr->node_checked();
      if (n->opcode() == Opcode::kKI32) {
        return graph_->KFloat64(info, static_cast<double>(n->Int32()));
      } else {
        return graph_->AddValue(info, Opcode::kI2F, opr);
      }
    }
    case Type::kBool: {
      auto n = opr->node_checked();
      if (n->opcode() == Opcode::kKBool) {
        return graph_->KFloat64(info, (n->Bool() ? 1.0 : 0.0));
      } else {
        return graph_->AddValue(info, Opcode::kB2F, opr);
      }
    }

    case Type::kNull:
      return graph_->KFloat64(info, 0.0);

    case Type::kUndefined:
      return graph_->KNaN(info);

    case Type::kFloat64:
      return opr;

    case Type::kString:
      return graph_->AddValue(info, Opcode::kS2F, opr);

    default:
      return {};
  }
}

SSAVar* Builder::MustCastToFloat64(InterpInfo* info, SSAVar* opr) {
  auto maybe = CastToFloat64(info, opr);
  CHECK(maybe);
  return *maybe;
}

std::optional<SSAVar*> Builder::CastToInt32(InterpInfo* info, SSAVar* opr) {
  switch (opr->type().tag()) {
    case Type::kInt:
      return opr;
    case Type::kBool: {
      auto n = opr->node_checked();
      if (n->opcode() == Opcode::kKBool) {
        return graph_->KI32(info, (n->Bool() ? 1 : 0));
      } else {
        return graph_->AddValue(info, Opcode::kB2I, opr);
      }
    }

    default:
      return {};
  }
}

SSAVar* Builder::MustCastToInt32(InterpInfo* info, SSAVar* opr) {
  auto maybe = CastToInt32(info, opr);
  CHECK(maybe);
  return *maybe;
}

// Casting a float64 to a bit pattern, which is used for bit operation. This
// node is only used for bit operation.
std::optional<SSAVar*> Builder::CastToBit(InterpInfo* info, SSAVar* opr) {
  if (opr->type().IsReal()) {
    auto n = opr->node_checked();
    if (n->opcode() == Opcode::kKFloat64) {
      auto dvalue = n->Float64();
      return graph_->KI32(info, JVal::Float64ToBit32(dvalue));
    }
    return graph_->AddValue(info, Opcode::kF2Bit, opr);
  } else if (opr->type().IsInt()) {
    return opr;
  } else {
    return {};
  }
}

SSAVar* Builder::MustCastToBit(InterpInfo* info, SSAVar* opr) {
  auto v = CastToBit(info, opr);
  CHECK(v);
  return *v;
}

// ----------------------------------------------------------------------------
// Value range guard
std::optional<SSAVar*> Builder::GuardFold(InterpInfo* info, SSAVar* value,
                                          Opcode op) {
  auto node = value->node_checked();

  switch (node->opcode()) {
    case Opcode::kKI32:
      return FoldIntComparison(node->Int32(), op) ? value : nullptr;
    case Opcode::kKI64:
      return FoldIntComparison(node->Int64(), op) ? value : nullptr;
    default:
      break;
  }

  return {};
}

SSAVar* Builder::GuardIntPositive(InterpInfo* info, SSAVar* int_value) {
  if (auto v = GuardFold(info, int_value, Opcode::kGIsIntPos); v) {
    return *v;
  }
  return AddGuardValue(info, Opcode::kGIsIntPos, int_value);
}

SSAVar* Builder::GuardIntPositiveOrZero(InterpInfo* info, SSAVar* int_value) {
  if (auto v = GuardFold(info, int_value, Opcode::kGIsIntPosOrZero); v) {
    return *v;
  }
  return AddGuardValue(info, Opcode::kGIsIntPosOrZero, int_value);
}

SSAVar* Builder::GuardIntNegative(InterpInfo* info, SSAVar* int_value) {
  if (auto v = GuardFold(info, int_value, Opcode::kGIsIntNeg); v) {
    return *v;
  }
  return AddGuardValue(info, Opcode::kGIsIntNeg, int_value);
}

// ----------------------------------------------------------------------------
// Stack manipulation
void Builder::EnterFrame(JitFrameType t, std::size_t slot_size,
                         std::size_t param_size, std::size_t local_size,
                         const void* pc, const void* npc, SSAVar* func_obj,
                         SSAVar* this_obj, SSAVar* tar_obj,
                         std::size_t vref_size, std::size_t pop_size,
                         bool is_tail) {
  // clang-format off
  frame_.emplace_back(t          /* frame type */,
                      slot_size  /* slot size */,
                      param_size /* parameter size */,
                      local_size /* local variable size */,
                      pc         /* current PC */,
                      npc        /* next PC */,
                      func_obj   /* function object */,
                      this_obj   /* this object */,
                      tar_obj    /* target object */,
                      vref_size  /* var ref size, only matter for inline */,
                      pop_size   /* stack pop size after call */,
                      is_tail    /* whether it is tail call */);
  // clang-format on
}

void Builder::EnterEntryFrame(InterpInfo* info) {
  CHECK(frame_.empty());
  trace_info_.emplace_back();
  EnterFrame(JitFrameType::kJitEntry, info->slot_size(), info->param_size(),
             info->local_size(), info->start_pc(), info->next_pc(), nullptr,
             nullptr, nullptr, 0, 0, false);
}

void Builder::LeaveFrame(InterpInfo* info) {
  UNUSE(info);
  CHECK(!frame_.empty());
  frame_.pop_back();
  trace_info_.pop_back();
}

// The call of this function happen at the caller's OP_call/OP_method_call,
// ie we are still in the caller stack instead of the callee. The basic idea is
// to transfer all the current frame's value to the inline frame's simluated
// stack
void Builder::EnterInlineFrame(InterpInfo* info, int call_argc,
                               const JSBytecodeInternalInfo& bc_info,
                               const std::vector<SSAVar*>& parameter_list,
                               SSAVar* func_obj, SSAVar* this_object,
                               SSAVar* tar_object, std::size_t psize,
                               bool is_tail) {
  const auto actual_arg_count =
      std::max(call_argc, static_cast<int>(bc_info.arg_count));

  // 1) Enter the inline frame
  {
    const auto slot_size = actual_arg_count + bc_info.loc_count;

    EnterFrame(JitFrameType::kInline, slot_size, actual_arg_count,
               bc_info.loc_count, bc_info.bc_start, bc_info.bc_start, func_obj,
               this_object, tar_object, bc_info.var_ref_count, psize, is_tail);
  }

  // 2) Populate the inline function's frame
  //
  // 2.1) re-apply the parameter into the new stack
  for (std::uint16_t i = 0; i < call_argc; i++) {
    CHECK(param()[i] == nullptr);
    param()[i] = parameter_list[i];
  }
  for (std::uint16_t i = call_argc; i < actual_arg_count; i++) {
    CHECK(param()[i] == nullptr);
    param()[i] = graph_->KUndefined(info);
  }

  // 2.2) initialize the local to JS_UNDEFINED, this is implicitly performed by
  //      quickjs's function call prologue
  for (std::uint16_t i = 0; i < bc_info.loc_count; i++) {
    CHECK(local()[i] == nullptr);
    local()[i] = graph_->KUndefined(info);
  }
}

std::size_t Builder::IRFrame::StkIdx(int idx) const {
  idx = idx < 0 ? -idx : idx;
  CHECK(static_cast<std::size_t>(idx) <= storage_.size());

  auto real_index = storage_.size() - static_cast<std::size_t>(idx);
  CHECK(real_index >= temp_base());
  return real_index - temp_base();
}

int Builder::IRFrame::StkOffset(int idx) const {
  // The idx is relative to the simulated stack, we now just convert it into
  // the real index
  auto real_idx = static_cast<int>(StkIdx(idx));

  return real_idx - stack_cur_;
}

int Builder::IRFrame::StkOffsetFromRIndex(std::size_t ridx) const {
  return static_cast<int>(ridx) - stack_cur_;
}

std::size_t Builder::IRFrame::StkOffsetToRIndex(int idx) const {
  auto ridx = stack_cur_ + idx;
  CHECK(ridx >= 0);
  return static_cast<std::size_t>(ridx);
}

void Builder::IRFrame::Push(SSAVar* value) { storage_.push_back(value); }

void Builder::IRFrame::Pop(int n) {
  CHECK(static_cast<std::uint32_t>(n) <= temp_size());
  storage_.resize(storage_.size() - n);
}

void Builder::IRFrame::Pop() {
  CHECK(temp_size() > 0);
  storage_.pop_back();
}

void Builder::Push(SSAVar* v) {
  cur_frame().Push(v);

  if (!is_reemit_) {
    if (v) {
      graph_->AddOp(interp_info_, Opcode::kTempPush, v);
    } else {
      graph_->AddOp(interp_info_, Opcode::kTempNullify,
                    graph_->KI32(interp_info_, StkIdx(1)));
    }
  }
}

void Builder::Pop() {
  if (!is_reemit_) {
    graph_->AddOp(interp_info_, Opcode::kTempPop,
                  graph_->KI32(interp_info_, 1));
  }
  cur_frame().Pop();
}

void Builder::Pop(int x) {
  if (!is_reemit_) {
    graph_->AddOp(interp_info_, Opcode::kTempPop,
                  graph_->KI32(interp_info_, x));
  }
  cur_frame().Pop(x);
}

void Builder::StkSet(int index, SSAVar* value) {
  auto real_index = StkIdx(index);
  graph_->AddOp(interp_info_, Opcode::kTempSet,
                graph_->KI32(interp_info_, real_index), value);
  temp()[real_index] = value;
}

void Builder::TempFill(std::uint32_t real_index, SSAVar* value) {
  auto needed_size = real_index + 1;
  if (temp_size() < needed_size) {
    auto hole_size = needed_size - temp_size();
    while (hole_size > 0) {
      Push(nullptr);
      --hole_size;
    }
  }

  CHECK(temp_size() >= needed_size);
  temp()[real_index] = value;
}

SSAVar* Builder::StkGet(InterpInfo* info, int index) {
  UNUSE(info);
  auto real_index = StkIdx(index);
  if (auto value = temp()[real_index]; !value) {
    auto memory_node = graph_->AddValue(info, Opcode::kTempGet,
                                        graph_->KI32(info, StkOffset(index)));
    temp()[real_index] = memory_node;
    return memory_node;
  } else {
    return value;
  }
}

std::pair<TraceResult, SSAVar*> Builder::StkAt(InterpInfo* info, int index) {
  auto real_index = StkIdx(index);

  if (auto ssa_var = temp()[real_index]; !ssa_var) {
    // If a temporary value is not loaded, we should just load it now
    auto memory_node = graph_->AddValue(info, Opcode::kTempGet,
                                        graph_->KI32(info, StkOffset(index)));

    // Get the current memory value, used for specialization
    auto memory_val = info->GetTemp(real_index);

    // Unbox the node
    if (auto [r, ssa_var] = Unbox(info, memory_val, memory_node); !r) {
      return std::make_pair(r, nullptr);
    } else {
      StkSet(index, ssa_var);
      return std::make_pair(r, ssa_var);
    }
  } else {
    if (ssa_var->type().IsBox()) {
      // perform unbox here, notes, the stack(slot) holds a boxed value, this
      // is typically result from an external call, which we cannot safely
      // unbox it during tracing site

      auto memory_val = info->GetTemp(real_index);
      if (auto [r, svar] = Unbox(info, memory_val, ssa_var); !r) {
        return std::make_pair(r, nullptr);
      } else {
        StkSet(index, svar);
        return std::make_pair(r, svar);
      }
    } else {
      return std::make_pair(TraceResult::Done(), ssa_var);
    }
  }
}

void Builder::AddTempNullify(InterpInfo* info, int idx) {
  auto real_idx = StkIdx(idx);

  // This can use real index since TempNullify is internal IR, never emitted
  // in the JIT
  graph_->AddOp(info, Opcode::kTempNullify, graph_->KI32(info, real_idx));
}

// ----------------------------------------------------------------------------
// Checkpoint
//
// Update checkpoint after a certain node is done, this is used to indicate
// that the input node has side effect
void Builder::UpdateCheckpointAfter(InterpInfo* info, Checkpoint::Reason reason,
                                    Node* n) {
  AddCheckpointToGraph(info, reason, n, true);
}

void Builder::UpdateCheckpointBefore(InterpInfo* info,
                                     Checkpoint::Reason reason, Node* n) {
  AddCheckpointToGraph(info, reason, n, false);
}

void Builder::AddPendingCheckpointAfter(Checkpoint::Reason reason, Node* node) {
  CHECK(!pending_checkpoint_);
  if (!HasRCQueued()) {
    pending_checkpoint_.emplace(reason, node);
  }
}

void Builder::AddPendingSnapshot(Checkpoint::Reason reason, bool is_after) {
  CHECK(!pending_snapshot_);
  pending_snapshot_.emplace(reason, is_after);
}

void Builder::FlushPendingSnapshot(InterpInfo* info) {
  if (pending_snapshot_) {
    if (pending_snapshot_->is_after) {
      SnapshotAfter(info, pending_snapshot_->reason);
    } else {
      SnapshotBefore(info, pending_snapshot_->reason);
    }
    pending_snapshot_.reset();
  }
}

bool Builder::TryAddPendingCheckpointAfter(Checkpoint::Reason reason,
                                           Node* node) {
  if (pending_checkpoint_) return false;
  AddPendingCheckpointAfter(reason, node);
  return true;
}

void Builder::FlushPendingCheckpoint(InterpInfo* info) {
  if (pending_checkpoint_) {
    AddCheckpointToGraph(info, pending_checkpoint_->reason,
                         pending_checkpoint_->trigger_node, true);
    pending_checkpoint_.reset();
  }
}

void Builder::FlushPendingGuardNotException(InterpInfo* info) {
  if (pending_guard_not_exception_) {
    auto n = *pending_guard_not_exception_;
    UpdateCheckpointAfter(info, Checkpoint::kException, n->node_checked());
    GuardNotException(info, n);
    pending_guard_not_exception_.reset();
  }
}

// Invoked when we just emit a guard node. For guard node, we need to figure
// out where our guard node should jump to if the guard failed. A guard node
// will need to jump some previously saved checkpoint and the jumpped target
// is been saved into the exit_ field of node. One node can jump to a single
// checkpoint
void Builder::OnGuard(Node* node) {
  CHECK(node->ir_effect().IsGuard());
  node->BailoutTo(checkpoint_);
}

void Builder::OnGuard(SSAVar* node) { OnGuard(node->node_checked()); }

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
// that's the case, we must make sure after the bytecode's RC performs,
// nothing happened inside of the bytecode generated jit code will lead to a
// deoptimization.
//
// Another approach is to reverse the RC operation, but that's super hard
// since when RC drops to 0, you cannot really reverse it, unless you do
// something like hazard-pointer style GC, which is a mess.

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

    // Notes(dpeng): We flush the checkpoint request to pending checkpoint and
    //   let it finalize. The reason we have some very specific order not easy
    //   to maintain, so all the bytecode level checkpoint is happened in
    //   pending_checkpoint_ field and we run another pass in AfterTrace to
    //   decide how to materialize the pending_checkpoint_

    if (!pending_checkpoint_) {
      AddPendingCheckpointAfter(Checkpoint::kRefCount, rc_op);
    }
    rc_queue_.clear();
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
  if (auto slot = temp()[real_index]; !slot) {
    // Here we generate a TOS value if it is not there but do not emit guard
    // for it since we just need to reduce its DEC. Notes we don't need the
    // type information, but just pops it out with polymorphic ref-count
    // deduction
    auto memory_node = graph_->AddValue(info, Opcode::kTempGet,
                                        graph_->KI32(info, StkOffset(1)));
    RcDec(info, memory_node);
  } else {
    RcDec(info, slot);
  }
  Pop(1);
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

    case JVal::kString:
      Push(graph_->LoadStr(info, jval.String()));
      break;

    default:
      return TraceResult::NYI("unknown literal value");
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
    case JVal::kUndefined:
    case JVal::kString:

    // in order to support function call
    case JVal::kObject:
    case JVal::kFunctionBytecode:
    case JVal::kModule: {
      CHECK(!rc_flushed_);
      auto guard_node =
          AddGuardValue(info, Opcode::kBxGuard, JValT2Type(jval.type()), node);
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

    case Type::kObject:
    case Type::kModule:
    case Type::kString:
    case Type::kSymbol:
    case Type::kJSFunction:
      break;

    case Type::kI64:
      UNREACHABLE() << "we should not have I64 for boxing";
      break;

    default:
      return TraceResult::NYI("box: unsupported type");
  }

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Check whether the node is a int zero or not. The node must be a int already
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

void Builder::RemovePhiLoopCandidate(ValType vtype, int index) {
  auto itr =
      std::find_if(phi_loop_candidate_.begin(), phi_loop_candidate_.end(),
                   [&](const PhiVal& pv) -> bool {
                     if (pv.vtype == vtype && pv.index == index) {
                       return true;
                     }
                     return false;
                   });

  if (itr != phi_loop_candidate_.end()) {
    phi_loop_candidate_.erase(itr);
  }
}

Node* Builder::BoxToTemp(BytecodeInfoProvider* info, SSAVar* idx, SSAVar* value,
                         bool erase_phi) {
  if (erase_phi) RemovePhiLoopCandidate(kTemp, idx->node_checked()->Int32());
  return graph_->AddOp(info, Opcode::kBxToTemp, idx, value);
}

Node* Builder::BoxToLocal(BytecodeInfoProvider* info, SSAVar* idx,
                          SSAVar* value, bool erase_phi) {
  if (erase_phi) RemovePhiLoopCandidate(kLocal, idx->node_checked()->Int32());
  return graph_->AddOp(info, Opcode::kBxToLocal, idx, value);
}

Node* Builder::BoxToParam(BytecodeInfoProvider* info, SSAVar* idx,
                          SSAVar* value, bool erase_phi) {
  if (erase_phi) RemovePhiLoopCandidate(kParam, idx->node_checked()->Int32());
  return graph_->AddOp(info, Opcode::kBxToParam, idx, value);
}

Node* Builder::BoxTo(BytecodeInfoProvider* info, Opcode opcode, SSAVar* idx,
                     SSAVar* value, bool erase_phi) {
  switch (opcode) {
    case Opcode::kBxToLocal:
      return BoxToLocal(info, idx, value, erase_phi);
    case Opcode::kBxToParam:
      return BoxToParam(info, idx, value, erase_phi);
    case Opcode::kBxToTemp:
      return BoxToTemp(info, idx, value, erase_phi);
    default:
      UNREACHABLE();
      return nullptr;
  }
}

// ----------------------------------------------------------------------------
// Local Variable
std::pair<TraceResult, SSAVar*> Builder::LocAt(InterpInfo* info, int idx) {
  CHECK(static_cast<std::uint32_t>(idx) < local_size());
  AddPhiLoopCandidate(kLocal, idx);

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
  AddPhiLoopCandidate(kParam, idx);

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

bool Builder::IsValMutated(SSAVar* param, ValType vtype, int idx) {
  if (!param) return false;
  if (vtype == kTemp) return true;  // always assume to be mutated

  // If the parameter is just a unbox value which tries to unbox a LoadParam
  // of same index, then it is considered to be not mutated
  if (auto n = param->node(); n->opcode() == Opcode::kBxGuard) {
    if (auto target = n->At(0); target) {
      auto nn = target->node_checked();
      switch (vtype) {
        case kLocal:
          if (nn->opcode() == Opcode::kLocGet &&
              nn->At(0)->node_checked()->Int32() == idx) {
            return false;
          }
          break;

        case kParam:
          if (nn->opcode() == Opcode::kParamGet &&
              nn->At(0)->node_checked()->Int32() == idx) {
            return false;
          }
          break;
        default:
          break;
      }
    }
  }
  return true;
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
  return PushJVal(info, JVal::False(info->qjs_context()));
}

TraceResult Builder::Trace_OP_push_true(InterpInfo* info) {
  return PushJVal(info, JVal::True(info->qjs_context()));
}

// Notes, this object is immutable for a certain frame, we just do not need to
// guard anything, but directly use it
TraceResult Builder::Trace_OP_push_this(InterpInfo* info) {
  // FIXME(dpeng): Support strict mode in future
  CHECK(!info->IsModeStrict());

  if (auto self = info->This(); self.IsObject()) {
    auto this_object = graph_->AddValue(info, Opcode::kLoadThisObject);
    auto value = AddGuardValue(info, Opcode::kBxGuard,
                               JValT2Type(JVal::kObject), this_object);
    RcInc(info, value);
    Push(value);
    return TraceResult::Done();
  } else if (self.IsNull() || self.IsUndefined()) {
    AddGuardOp(info, Opcode::kGThisIsNullOrUndefined);
    auto value = graph_->AddValue(info, Opcode::kContextLoadGlobalObject);
    Push(value);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("push_this: unknown this type");
  }
}

TraceResult Builder::Trace_OP_object(InterpInfo* info) {
  auto value = graph_->AddValue(info, Opcode::kObjectNew);
  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);
  Push(value);
  return TraceResult::Done();
}

/*

TraceResult Builder::Trace_OP_fclosure(InterpInfo* info) {
  auto atom = graph_->KI32(info, info->GetU32());
  auto func = graph_->AddValue(info, Opcode::kLoadFunction, atom);
  auto vref = graph_->AddValue(info, Opcode::kLoadVarRef);
  auto frame = graph_->AddValue(info, Opcode::kLoadStackFrame);

  auto value = graph_->AddValue(info, Opcode::kNewClosure, func, vref, frame);

  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);

  Push(value);
  return TraceResult::Done();
}

*/

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

  StkSet(1, v2);
  StkSet(2, v1);

  Push(v1);

  RcInc(info, v1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_insert3(InterpInfo* info) {
  TOS(v1, info, 1);
  TOS(v2, info, 2);
  TOS(v3, info, 3);

  StkSet(1, v2);
  StkSet(2, v3);
  StkSet(3, v1);

  Push(v1);

  RcInc(info, v1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_insert4(InterpInfo* info) {
  TOS(v1, info, 1);
  TOS(v2, info, 2);
  TOS(v3, info, 3);
  TOS(v4, info, 4);

  StkSet(1, v2);
  StkSet(2, v3);
  StkSet(3, v4);
  StkSet(4, v1);

  Push(v1);

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

TraceResult Builder::Trace_OP_swap2(InterpInfo* info) {
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

TraceResult Builder::Trace_OP_swap(InterpInfo* info) {
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

// The set_loc duplicate the TOS value and set it to current local slot,
// leaving the TOS untouched
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
  RcTryDecLoc(info, loc_idx);
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
      RcInc(info, value);
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
      Pop();
      return ParamSet(info, param_idx, value);
    }
  }

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Arithmetic
//
// Notes:
//
// The QuickJS uses a dual-number model, but ecmascript only allows one types
// of number underlying. The dual-number model is good for performance since
// if user just use small sub-type of number, ie int32, then no floating point
// operation is involved, this is especially good on risc machine. But it
// exposes some complexity for us, since when a int32 operation overflows, we
// need to "evolve" the number into flaoting point representation. However our
// JIT backend is generally typped statically, ie if a int instruction is gen
// then all the data(value) dependency will just treat that instruction always
// as int. The number type evolution, if resolved completely, needs to be
// modeled as control flow structure. For now, we simply bailout when the
// int operation overflow, and let the interpreter handles the complexity.
// In the future, this needs to be addressed when hyper-block scheduling is
// introduced
//
// Notes the LIR int operation has none overflow operation and overflow
// operation. Make sure to use the overflow one.
std::pair<TraceResult, SSAVar*> Builder::DoArithmeticBasic(InterpInfo* info,
                                                           Opcode opcode_int,
                                                           Opcode opcode_real,
                                                           SSAVar* op1,
                                                           SSAVar* op2) {
  if (op1->type().IsInt() && op2->type().IsInt()) {
    // This are bytecode we cannot handle in this DoArithmeticBasic
    CHECK(opcode_int != Opcode::kModInt);

    // for all the arithmetic of int, we need to respect the number type
    // evolution. All the opcode must be guarded one, and we should allow
    // the instruction to bailout
    auto ops = AddGuardValue(info, opcode_int, op1, op2);

    return std::make_pair(TraceResult::Done(), ops);
  } else if (op1->type().IsQJSNumber() && op2->type().IsQJSNumber()) {
    return std::make_pair(
        TraceResult::Done(),
        graph_->AddValue(info, opcode_real, MustCastToFloat64(info, op1),
                         MustCastToFloat64(info, op2)));
  } else if (op1->type().IsIntOrBool() && op2->type().IsIntOrBool()) {
    return std::make_pair(
        TraceResult::Done(),
        AddGuardValue(info, opcode_int, MustCastToInt32(info, op1),
                      MustCastToInt32(info, op2)));
  } else {
    // Okay, unknown cases, tries to cast all operand to float64 if we can,
    // otherwise we will need to bailout since the type is unknown to us and
    // it surely will cause exception thrown
    if (auto lhs = CastToFloat64(info, op1); lhs) {
      if (auto rhs = CastToFloat64(info, op2); rhs) {
        return std::make_pair(TraceResult::Done(),
                              graph_->AddValue(info, opcode_real, *lhs, *rhs));
      }
    }
  }

  return std::make_pair(
      TraceResult::NYI("unsupported type for arithmetic operation"), nullptr);
}

std::pair<TraceResult, SSAVar*> Builder::DoStringConcate(InterpInfo* info,
                                                         SSAVar* op1,
                                                         SSAVar* op2) {
  if (auto maybe_lhs = CastToString(info, op1); maybe_lhs) {
    if (auto maybe_rhs = CastToString(info, op2); maybe_rhs) {
      auto lhs = *maybe_lhs;
      auto rhs = *maybe_rhs;
      auto value = graph_->AddValue(info, Opcode::kStrCon, lhs, rhs);

      /**
       * Notes, we do not check exception here since the concat string will
       * not return exception when both operands are strings regardlessly.
       */

      return std::make_pair(TraceResult::Done(), value);
    }
  }

  return std::make_pair(TraceResult::NYI("unsupported type for string concate"),
                        nullptr);
}

TraceResult Builder::Trace_OP_add(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsString() || op2->type().IsString()) {
    // string case, ie we need to promote the type and perform concatenation
    if (auto [r, output] = DoStringConcate(info, op1, op2); !r) {
      return r;
    } else {
      Pop(2);
      Push(output);
      return TraceResult::Done();
    }
  } else if (auto [r, output] = DoArithmeticBasic(info, Opcode::kGAddInt,
                                                  Opcode::kAddReal, op1, op2);
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
          DoArithmeticBasic(info, Opcode::kGSubInt, Opcode::kSubReal, op1, op2);
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
          DoArithmeticBasic(info, Opcode::kGMulInt, Opcode::kMulReal, op1, op2);
      !r) {
    return r;
  } else {
    Pop(2);
    Push(output);
    return TraceResult::Done();
  }
}

// Division of 2 numbers are kind complicated, since the we are using
// dual-type number. The QuickJS just convert the int operand to float and
// performs the division and store the floating point number back to the seat.
// We will just replicate this behavior
TraceResult Builder::Trace_OP_div(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto maybe = CastToFloat64(info, op1); maybe) {
    op1 = *maybe;
  } else {
    return TraceResult::NYI("invalid operand type: div");
  }

  if (auto maybe = CastToFloat64(info, op2); maybe) {
    op2 = *maybe;
  } else {
    return TraceResult::NYI("invalid operand type: div");
  }

  auto result = graph_->AddValue(info, Opcode::kDivReal, op1, op2);
  Pop(2);
  Push(result);
  return TraceResult::Done();
}

// The mod operator is also kind of sad since we have to bailout if both
// operand is negative int. In this case, it requires fallback to use
// floating point which is not supported for now
TraceResult Builder::Trace_OP_mod(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsInt() && op2->type().IsInt()) {
    auto new_op1 = GuardIntPositiveOrZero(info, op1);
    if (!new_op1) return TraceResult::NYI("mod(int)");

    auto new_op2 = GuardIntPositive(info, op2);
    if (!new_op2) return TraceResult::NYI("mod(int)");

    // notes there's no point to guard, since new_op2 will never be a 0
    auto result = graph_->AddValue(info, Opcode::kModInt, new_op1, new_op2);
    Pop(2);
    Push(result);
  } else {
    if (auto maybe = CastToFloat64(info, op1); maybe) {
      op1 = *maybe;
    } else {
      return TraceResult::NYI("invalid operand type: mod");
    }

    if (auto maybe = CastToFloat64(info, op2); maybe) {
      op2 = *maybe;
    } else {
      return TraceResult::NYI("invalid operand type: mod");
    }

    // fmod
    CHECK(op1->type().IsReal());
    CHECK(op2->type().IsReal());
    auto result = graph_->AddValue(info, Opcode::kModReal, op1, op2);
    Pop(2);
    Push(result);
  }
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_pow(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (auto maybe = CastToFloat64(info, op1); maybe) {
    op1 = *maybe;
  } else {
    return TraceResult::NYI("invalid operand type: pow");
  }

  if (auto maybe = CastToFloat64(info, op2); maybe) {
    op2 = *maybe;
  } else {
    return TraceResult::NYI("invalid operand type: pow");
  }

  CHECK(op1->type().IsReal());
  CHECK(op2->type().IsReal());

  auto result = graph_->AddValue(info, Opcode::kPowReal, op1, op2);
  Pop(2);
  Push(result);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_add_loc(InterpInfo* info) {
  TOS(op2, info, 1);
  auto loc_idx = info->GetU8();

  if (auto [r, op1] = LocAt(info, loc_idx); !r) {
    return r;
  } else {
    if (auto [r, output] = DoArithmeticBasic(info, Opcode::kGAddInt,
                                             Opcode::kAddReal, op1, op2);
        !r) {
      return r;
    } else {
      Pop(1);
      Push(output);
      return TraceResult::Done();
    }
  }
}

TraceResult Builder::Trace_OP_is_undefined_or_null(InterpInfo* info) {
  auto idx = StkIdx(1);
  SSAVar* output = nullptr;

  if (auto value = temp()[idx]; !value) {
    // If a temporary value is not loaded, we should just load it now
    auto memory_node = graph_->AddValue(info, Opcode::kTempGet,
                                        graph_->KI32(info, StkOffset(1)));

    output =
        graph_->AddValue(info, Opcode::kIsTypeUndefinedOrNull, memory_node);

  } else {
    output = graph_->KBool(
        info, (value->type().IsUndefined() || value->type().IsNull()));
  }

  CHECK(output);
  Pop(1);
  Push(output);
  return TraceResult::Done();
}

// Incremental/Decremental
TraceResult Builder::Trace_OP_inc(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    auto value = AddGuardValue(info, Opcode::kGIncInt, op1);
    StkSet(1, value);
  } else if (op1->type().IsReal()) {
    StkSet(1, graph_->AddValue(info, Opcode::kIncReal, op1));
  } else {
    return TraceResult::NYI("invalid operand type: inc");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_dec(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt()) {
    auto value = AddGuardValue(info, Opcode::kGDecInt, op1);
    StkSet(1, value);
  } else if (op1->type().IsReal()) {
    StkSet(1, graph_->AddValue(info, Opcode::kDecReal, op1));
  } else {
    return TraceResult::NYI("invalid operand type: dec");
  }

  return TraceResult::Done();
}

// After post_XXX, the stack will push the new value at TOS and left the
// old value at where it is. Notes, as a side effect, the QuickJS VM will
// awlays convert the value before the post inc to be floating point, but
// the value aferwards is always int, which is kind of wired :(

TraceResult Builder::Trace_OP_post_inc(InterpInfo* info) {
  TOS(op1, info, 1);
  SSAVar* r = nullptr;

  auto maybe_fop1 = CastToFloat64(info, op1);
  if (!maybe_fop1) return TraceResult::NYI("invalid operand type: post_inc");

  if (op1->type().IsInt()) {
    r = AddGuardValue(info, Opcode::kGIncInt, op1);
  } else if (op1->type().IsReal()) {
    r = graph_->AddValue(info, Opcode::kIncReal, op1);
  } else {
    return TraceResult::NYI("invalid operand type: post_inc");
  }

  StkSet(1, *maybe_fop1);
  Push(r);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_post_dec(InterpInfo* info) {
  TOS(op1, info, 1);
  SSAVar* r = nullptr;

  auto maybe_fop1 = CastToFloat64(info, op1);
  if (!maybe_fop1) return TraceResult::NYI("invalid operand type: post_dec");

  if (op1->type().IsInt()) {
    r = AddGuardValue(info, Opcode::kGDecInt, op1);
  } else if (op1->type().IsReal()) {
    r = graph_->AddValue(info, Opcode::kDecReal, op1);
  } else {
    return TraceResult::NYI("invalid operand type: post_dec");
  }

  StkSet(1, *maybe_fop1);
  Push(r);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_inc_loc(InterpInfo* info) {
  auto loc_idx = info->GetU8();

  if (auto [r, ssa] = LocAt(info, loc_idx); !r) {
    return r;
  } else {
    if (ssa->type().IsInt()) {
      // guard if the value is a max, then bailout
      auto guard = AddGuardValue(info, Opcode::kGIsNotIntMax, ssa);

      // increase the value by one
      auto new_value = graph_->AddValue(info, Opcode::kIncInt, guard);

      // set it back
      return LocSet(info, loc_idx, new_value);
    } else if (ssa->type().IsReal()) {
      auto new_value = graph_->AddValue(info, Opcode::kIncReal, ssa);
      return LocSet(info, loc_idx, new_value);
    } else {
      return TraceResult::NYI("invalid operand type: inc_loc");
    }
  }
}

TraceResult Builder::Trace_OP_dec_loc(InterpInfo* info) {
  auto loc_idx = info->GetU8();

  if (auto [r, ssa] = LocAt(info, loc_idx); !r) {
    return r;
  } else {
    if (ssa->type().IsInt()) {
      // guard if the value is a max, then bailout
      auto guard = AddGuardValue(info, Opcode::kGIsNotIntMin, ssa);

      // decrease the value by one
      auto new_value = graph_->AddValue(info, Opcode::kDecInt, guard);

      // set it back
      return LocSet(info, loc_idx, new_value);
    } else if (ssa->type().IsReal()) {
      auto new_value = graph_->AddValue(info, Opcode::kDecReal, ssa);
      return LocSet(info, loc_idx, new_value);
    } else {
      return TraceResult::NYI("invalid operand type: dec_loc");
    }
  }
}

// ----------------------------------------------------------------------------
// Bit operation
//
// Notes for bit operation, if both operand are int, then mostly just perform
// the bit operation, since integer is 32 bits; for real(number), we need to
// normalize the value into bit pattern and then perform the bit operation.
// Notes, the bit operation's result is still integer
TraceResult Builder::Trace_OP_shl(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);
  if (op1->type().IsQJSNumber() && op1->type().IsQJSNumber()) {
    op2 = graph_->AddValue(info, Opcode::kBitNorm, MustCastToBit(info, op2));

    auto output =
        graph_->AddValue(info, Opcode::kShlInt, MustCastToBit(info, op1), op2);
    Pop(2);
    Push(output);
    return TraceResult::Done();

  } else {
    return TraceResult::NYI("shl: invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_shr(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsQJSNumber() && op1->type().IsQJSNumber()) {
    op2 = graph_->AddValue(info, Opcode::kBitNorm, MustCastToBit(info, op2));

    // ShrInt is guarded version, ie it is possible we end up with an unsigned
    // value which is larger than the int32_t to be represented, and we will
    // need to bailout from it. In the allowed range, the value should be able
    // to be represented by the int32_t
    auto output =
        AddGuardValue(info, Opcode::kGShrInt, MustCastToBit(info, op1), op2);

    Pop(2);
    Push(output);
    return TraceResult::Done();

  } else {
    return TraceResult::NYI("shr: invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_sar(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsQJSNumber() && op1->type().IsQJSNumber()) {
    op2 = graph_->AddValue(info, Opcode::kBitNorm, MustCastToBit(info, op2));

    auto output =
        graph_->AddValue(info, Opcode::kSarInt, MustCastToBit(info, op1), op2);

    Pop(2);
    Push(output);
    return TraceResult::Done();

  } else {
    return TraceResult::NYI("sar: invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_and(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);
  if (op1->type().IsQJSNumber() && op2->type().IsQJSNumber()) {
    auto output =
        graph_->AddValue(info, Opcode::kAndInt, MustCastToBit(info, op1),
                         MustCastToBit(info, op2));
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("and: invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_or(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsQJSNumber() && op2->type().IsQJSNumber()) {
    auto output =
        graph_->AddValue(info, Opcode::kOrInt, MustCastToBit(info, op1),
                         MustCastToBit(info, op2));
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("or: invalid bit operand type");
  }
}

TraceResult Builder::Trace_OP_xor(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  if (op1->type().IsQJSNumber() && op2->type().IsQJSNumber()) {
    auto output =
        graph_->AddValue(info, Opcode::kXorInt, MustCastToBit(info, op1),
                         MustCastToBit(info, op2));
    Pop(2);
    Push(output);
    return TraceResult::Done();
  } else {
    return TraceResult::NYI("xor: invalid bit operand type");
  }
}

// JavaScript's comparison rule is pretty messy and crazy. Basically, it tries
// to make the string fit into the comparison table and it will try to force
// it to be converted from string to other type.

// For equalism comparison, we only specialize on certain special type, other
// will fallback to runtime function invocation. In most case, user's normal
// usage will be fine and catch up by JIT compiler
std::pair<TraceResult, SSAVar*> Builder::DoComparisonEqualism(
    InterpInfo* info, SSAVar* op1, SSAVar* op2, bool is_eq, bool* is_fallback) {
  *is_fallback = false;

  // 0) cast the boolean to int number, since they are not used during
  //    comparison
  if (op1->type().IsBool()) {
    op1 = graph_->AddValue(info, Opcode::kB2I, op1);
  }
  if (op2->type().IsBool()) {
    op2 = graph_->AddValue(info, Opcode::kB2I, op2);
  }

  // 1) if both operand are integer, then just do integer comparison
  if (op1->type().IsInt() && op2->type().IsInt()) {
    return std::make_pair(
        TraceResult::Done(),
        graph_->AddValue(info, is_eq ? Opcode::kEqInt : Opcode::kNeInt, op1,
                         op2));
  }

  // 2) if both operand are qjs number, then try the cast
  if (op1->type().IsQJSNumber() && op2->type().IsQJSNumber()) {
    op1 = MustCastToFloat64(info, op1);
    op2 = MustCastToFloat64(info, op2);
    return std::make_pair(
        TraceResult::Done(),
        graph_->AddValue(info, is_eq ? Opcode::kEqReal : Opcode::kNeReal, op1,
                         op2));
  }

  // 3) if one of the operand is string
  if (op1->type().IsString() || op2->type().IsString()) {
    SSAVar* output = nullptr;

    // 3.1) If one operand is string and the other operand is int/float64
    if (op1->type().IsQJSNumber() || op2->type().IsQJSNumber()) {
      // Per JavaScript rules, we need to convert the string into QJS number
      // for us, we directly convert it to float64 for later comparison
      if (op1->type().IsQJSNumber()) {
        op2 = graph_->AddValue(info, Opcode::kS2F, op2);
        op1 = MustCastToFloat64(info, op1);
      } else {
        op1 = graph_->AddValue(info, Opcode::kS2F, op1);
        op2 = MustCastToFloat64(info, op2);
      }

      output = graph_->AddValue(info, is_eq ? Opcode::kEqReal : Opcode::kNeReal,
                                op1, op2);
    } else if (op1->type().IsString() && op2->type().IsString()) {
      // 3.2) If both operand is string, then just specialized to string eq
      output = graph_->AddValue(info, is_eq ? Opcode::kEqStr : Opcode::kNeStr,
                                op1, op2);
    }

    if (output) {
      return std::make_pair(TraceResult::Done(), output);
    }
  }

  // 4) undefined/null cases, which is kind of sucky :(
  if (op1->type().IsNullOrUndefined() || op2->type().IsNullOrUndefined()) {
    SSAVar* output = nullptr;

    if ((op1->type().IsNull() && op2->type().IsNull()) ||
        (op1->type().IsUndefined() && op2->type().IsUndefined())) {
      output = is_eq ? graph_->True(info) : graph_->False(info);
    } else if ((op1->type().IsNull() && op2->type().IsUndefined()) ||
               (op1->type().IsUndefined() && op2->type().IsNull())) {
      output = is_eq ? graph_->True(info) : graph_->False(info);
    } else {
      output = is_eq ? graph_->False(info) : graph_->True(info);
    }
    return std::make_pair(TraceResult::Done(), output);
  }

  *is_fallback = true;

  // 5) slow path, ie calling external functions
  auto slow_path =
      AddGuardValue(info, is_eq ? Opcode::kEq : Opcode::kNe, op1, op2);

  // 5.1) add a guard checkpoint
  UpdateCheckpointAfter(info, Checkpoint::kException,
                        slow_path->node_checked());

  // 5.2) guard the return value of slow_path is not an exception, otherwise
  //      do the deoptimization
  GuardNotException(info, slow_path);

  return std::make_pair(TraceResult::Done(), slow_path);
}

std::pair<TraceResult, SSAVar*> Builder::DoComparisonNoneEqualism(
    InterpInfo* info, SSAVar* op1, SSAVar* op2,
    Opcode int_opcode,  /* integer type comparison opcode */
    Opcode real_opcode, /* float64 type comparison opcode */
    Opcode str_opcode,  /* string type comparison opcode */
    Opcode gen_opcode,  /* general type comaprison opcode */
    bool* is_fallback) {
  *is_fallback = false;

  // 0) try to cast boolean to integer
  if (op1->type().IsBool()) {
    op1 = graph_->AddValue(info, Opcode::kB2I, op1);
  }
  if (op2->type().IsBool()) {
    op2 = graph_->AddValue(info, Opcode::kB2I, op2);
  }

  // 1) check common cases
  if (op1->type().IsInt() && op2->type().IsInt()) {
    return std::make_pair(TraceResult::Done(),
                          graph_->AddValue(info, int_opcode, op1, op2));
  }

  // 2) can be converted to number, then just convert to number and then do
  //    the comparison
  if (op1->type().IsQJSNumber() && op2->type().IsQJSNumber()) {
    op1 = MustCastToFloat64(info, op1);
    op2 = MustCastToFloat64(info, op2);
    return std::make_pair(TraceResult::Done(),
                          graph_->AddValue(info, real_opcode, op1, op2));
  }

  // 3) check whether we have string
  if (op1->type().IsString() || op2->type().IsString()) {
    // 3.1) both operand is string
    if (op1->type().IsString() && op2->type().IsString()) {
      return std::make_pair(TraceResult::Done(),
                            graph_->AddValue(info, str_opcode, op1, op2));
    }

    // 3.2) one of the operand can be converted to number, ie int/float
    //      if so, we just convert the other string to string and do the
    //      comparison here. Notes in javascript, if a string cannot convert
    //      to number in the comparison context, it does not throw exception
    //      but convert to NaN, which just fails all the testify
    //
    // Notes, we do not cast string to integer, but to float. The reason is
    // some string is not representable by integer, ie NaN. We need NaN to
    // perform the comparison, if a string cannot be converted to number,
    // it will become NaN, which satisfy the ecmascript standard for
    // comparison
    if (op1->type().IsQJSNumber()) {
      op2 = graph_->AddValue(info, Opcode::kS2F, op2);
      op1 = MustCastToFloat64(info, op1);

      return std::make_pair(TraceResult::Done(),
                            graph_->AddValue(info, real_opcode, op1, op2));
    } else if (op2->type().IsQJSNumber()) {
      op1 = graph_->AddValue(info, Opcode::kS2F, op1);
      op2 = MustCastToFloat64(info, op2);

      return std::make_pair(TraceResult::Done(),
                            graph_->AddValue(info, real_opcode, op1, op2));
    }
  }

  // 4) Null/Undefined case
  //
  //    Notes the null can be viewed as a number, we can simply just convert
  //    it to 0 to do comparison, for undefined, then everything is false
  if (op1->type().IsNullOrUndefined() || op2->type().IsNullOrUndefined()) {
    // Any of the none equalism comparison operand is undefined, then the
    // result is false.
    if (op1->type().IsUndefined() || op2->type().IsUndefined()) {
      return std::make_pair(TraceResult::Done(), graph_->False(info));
    }

    if (op1->type().IsNull() && op2->type().IsNull()) {
      switch (int_opcode) {
        case Opcode::kLeInt:
        case Opcode::kGeInt:
          return std::make_pair(TraceResult::Done(), graph_->True(info));
        default:
          return std::make_pair(TraceResult::Done(), graph_->False(info));
      }
    }
  }

  *is_fallback = true;

  // 5) Fallback case, use runtime call
  auto slow_path = AddGuardValue(info, gen_opcode, op1, op2);

  // 5.1) add a guard checkpoint
  UpdateCheckpointAfter(info, Checkpoint::kException,
                        slow_path->node_checked());

  // 5.2) guard the return value of slow_path is not an exception, otherwise
  //      do the deoptimization
  GuardNotException(info, slow_path);

  return std::make_pair(TraceResult::Done(), slow_path);
}

#define IMPL_COMP(NAME, OP0, OP1, OP2, OP3)                                    \
  TraceResult Builder::Trace_OP_##NAME(InterpInfo* info) {                     \
    auto is_fallback = false;                                                  \
    TOS(op1, info, 2);                                                         \
    TOS(op2, info, 1);                                                         \
    if (auto [r, value] =                                                      \
            DoComparisonNoneEqualism(info, op1, op2, Opcode::OP0, Opcode::OP1, \
                                     Opcode::OP2, Opcode::OP3, &is_fallback);  \
        !r) {                                                                  \
      return r;                                                                \
    } else {                                                                   \
      if (!is_fallback) {                                                      \
        RcDec(info, op1);                                                      \
        RcDec(info, op2);                                                      \
      }                                                                        \
      Pop(2);                                                                  \
      Push(value);                                                             \
      return TraceResult::Done();                                              \
    }                                                                          \
  }

IMPL_COMP(lt, kLtInt, kLtReal, kLtStr, kLt);
IMPL_COMP(lte, kLeInt, kLeReal, kLeStr, kLe);
IMPL_COMP(gt, kGtInt, kGtReal, kGtStr, kGt);
IMPL_COMP(gte, kGeInt, kGeReal, kGeStr, kGe);

#undef IMPL_COMP  // IMPL_COMP

TraceResult Builder::Trace_OP_eq(InterpInfo* info) {
  auto is_fallback = false;

  TOS(op1, info, 2);
  TOS(op2, info, 1);

  SSAVar* output = nullptr;

  if (auto [r, value] =
          DoComparisonEqualism(info, op1, op2, true, &is_fallback);
      !r) {
    return r;
  } else {
    output = value;
  }

  if (!is_fallback) {
    RcDec(info, op1);
    RcDec(info, op2);
  }

  Pop(2);
  Push(output);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_neq(InterpInfo* info) {
  auto is_fallback = false;

  TOS(op1, info, 2);
  TOS(op2, info, 1);

  SSAVar* output = nullptr;

  if (auto [r, value] =
          DoComparisonEqualism(info, op1, op2, false, &is_fallback);
      !r) {
    return r;
  } else {
    output = value;
  }

  if (!is_fallback) {
    RcDec(info, op1);
    RcDec(info, op2);
  }

  Pop(2);
  Push(output);
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// strict eq/neq
TraceResult Builder::DoTraceStrictComparison(InterpInfo* info, bool is_eq) {
  TOS(lhs, info, 2);
  TOS(rhs, info, 1);
  SSAVar* result = nullptr;

  if (lhs->type().tag() == rhs->type().tag()) {
    switch (lhs->type().tag()) {
      case Type::kNull:
      case Type::kUndefined:
        result = is_eq ? graph_->True(info) : graph_->False(info);
        break;

      case Type::kObject:
      case Type::kSymbol:
        result = graph_->AddValue(info, is_eq ? Opcode::kEqPtr : Opcode::kNePtr,
                                  lhs, rhs);
        break;

      case Type::kInt:
        result = graph_->AddValue(info, is_eq ? Opcode::kEqInt : Opcode::kNeInt,
                                  lhs, rhs);
        break;

      case Type::kBool:
        result = graph_->AddValue(info, is_eq ? Opcode::kEqInt : Opcode::kNeInt,
                                  graph_->AddValue(info, Opcode::kB2I, lhs),
                                  graph_->AddValue(info, Opcode::kB2I, rhs));
        break;

      case Type::kFloat64:
        result = graph_->AddValue(
            info, is_eq ? Opcode::kEqReal : Opcode::kNeReal,
            MustCastToFloat64(info, lhs), MustCastToFloat64(info, rhs));
        break;

      case Type::kString:
        result = graph_->AddValue(info, is_eq ? Opcode::kEqStr : Opcode::kNeStr,
                                  lhs, rhs);
        break;

      default:
        result = is_eq ? graph_->False(info) : graph_->True(info);
        break;
    }
  } else {
    result = is_eq ? graph_->False(info) : graph_->True(info);
  }

  RcDec(info, lhs);
  RcDec(info, rhs);

  Pop(2);
  Push(result);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_strict_eq(InterpInfo* info) {
  return DoTraceStrictComparison(info, true);
}

TraceResult Builder::Trace_OP_strict_neq(InterpInfo* info) {
  return DoTraceStrictComparison(info, false);
}

// ----------------------------------------------------------------------------
// Unary operation
TraceResult Builder::Trace_OP_typeof(InterpInfo* info) {
  TOS(op1, info, 1);
  SSAVar* typeof = nullptr;

  if (op1->type().IsUnbox()) {
    // directly get the typeof string from QJS api
    auto type_of_atom = info->qjs()->api.js_typeof_to_atom(
        info->qjs_context(), JSValue{info->GetTOS().v()});

    // convert the atom ref to string
    JVal to_string{info->qjs_context(),
                   JS_AtomToString(info->qjs_context(), type_of_atom)};

    typeof = graph_->LoadStr(info, to_string.String());
  } else {
    // generate runtime call
    typeof = graph_->AddValue(info, Opcode::kTypeOf, op1);
  }

  PopAndRcDec(info);
  Push(typeof);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_instanceof(InterpInfo* info) {
  TOS(val, info, 2);
  TOS(obj, info, 1);

  if (!val->type().IsObject() && !obj->type().IsObject()) {
    return TraceResult::BytecodeError(
        "is_instance_of: both operand must be object");
  }

  // just call the runtime for now, in the future we may want to add CHA to
  // resolve it into optimized code path
  auto value = graph_->AddValue(info, Opcode::kInstanceOf, val, obj);
  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);

  RcDec(info, val);
  RcDec(info, obj);
  Pop(2);
  Push(value);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_delete(InterpInfo* info) {
  TOS(op1, info, 2);
  TOS(op2, info, 1);

  auto delete_op = graph_->AddValue(info, Opcode::kDelete, op1, op2);

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        delete_op->node_checked());

  GuardNotException(info, delete_op);

  Pop(2);
  Push(delete_op);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_delete_var(InterpInfo* info) {
  auto atom = info->GetU32();
  auto delete_op =
      graph_->AddValue(info, Opcode::kDeleteVar,
                       graph_->KI32(info, static_cast<std::uint32_t>(atom)));

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        delete_op->node_checked());

  GuardNotException(info, delete_op);

  Push(delete_op);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_neg(InterpInfo* info) {
  TOS(op1, info, 1);
  if (op1->type().IsInt()) {
    // Notes(dpeng): Since -0.0 cannot be expressed as integer, we need to
    //   assert the value to be none zero, otherwise bailout
    auto guard = AddGuardValue(info, Opcode::kGIsNotZero, op1);
    StkSet(1, AddGuardValue(info, Opcode::kGNegInt, guard));
  } else if (op1->type().IsReal()) {
    StkSet(1, graph_->AddValue(info, Opcode::kNegReal, op1));
  } else if (auto val = CastToFloat64(info, op1); val) {
    StkSet(1, graph_->AddValue(info, Opcode::kNegReal, *val));
  } else {
    return TraceResult::NYI("invalid operand type: negate");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_to_object(InterpInfo* info) {
  TOS(op1, info, 1);

  // folded version based on JS_ToObject implementation. Only unknown one, ie
  // boxed version will be dispatched to runtime call
  if (op1->type().IsUnbox()) {
    switch (op1->type().tag()) {
      case Type::kNull:
      case Type::kUndefined:
        return TraceResult::BytecodeError("invalid operand: to_object");

      case Type::kObject:
        // already an object, just duplicate it, ie bump rc
        RcInc(info, op1);
        return TraceResult::Done();

      default:
        // use default one, since to_object require some dirty work for now
        break;
    }
  }

  // fallback version, ie calling out for runtime routine
  auto value = graph_->AddValue(info, Opcode::kToObject, op1);
  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);

  RcDec(info, op1);

  Pop(1);
  Push(value);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_to_propkey(InterpInfo* info) {
  TOS(op1, info, 1);

  // folded version based on JS_ToObject implementation. Only unknown one, ie
  // boxed version will be dispatched to runtime call
  switch (op1->type().tag()) {
    case Type::kInt:
    case Type::kString:
    case Type::kSymbol:
      return TraceResult::Done();

    default:
      break;
  }

  // fallback version, ie calling out for runtime routine
  auto value = graph_->AddValue(info, Opcode::kToPropKey, op1);
  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);

  PopAndRcDec(info);
  Push(value);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_to_propkey2(InterpInfo* info) {
  TOS(op2, info, 2);
  if (op2->type().IsNullOrUndefined()) {
    return TraceResult::BytecodeError("to_propkey2: value has no property");
  }

  return Trace_OP_to_propkey(info);
}

// The JS allows + as unary operator, which is a no-op for any number, but
// will become a meta call when the type is not number
TraceResult Builder::Trace_OP_plus(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsInt() || op1->type().IsReal()) {
    // do nothing, but just return
  } else if (auto val = CastToFloat64(info, op1); val) {
    StkSet(1, *val);
  } else {
    return TraceResult::NYI("invalid operand type: plus");
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_not(InterpInfo* info) {
  TOS(op1, info, 1);

  if (op1->type().IsQJSNumber()) {
    StkSet(1,
           graph_->AddValue(info, Opcode::kInvInt, MustCastToBit(info, op1)));
  } else if (auto val = CastToFloat64(info, op1); val) {
    StkSet(1,
           graph_->AddValue(info, Opcode::kInvInt, MustCastToBit(info, *val)));
  } else {
    return TraceResult::NYI("invalid operand type: not");
  }

  return TraceResult::Done();
}

SSAVar* Builder::BuildCondition(InterpInfo* info, SSAVar* cond) {
  switch (cond->type().tag()) {
    case Type::kBool:
      return cond;

    case Type::kInt:
      return graph_->AddValue(info, Opcode::kI2B, cond);

    case Type::kFloat64:
      return graph_->AddValue(info, Opcode::kF2B, cond);

    case Type::kNull:
    case Type::kUndefined:
      return graph_->KBool(info, false);

    case Type::kObject:
      return graph_->AddValue(info, Opcode::kO2B, cond);

    case Type::kString:
      return graph_->AddValue(info, Opcode::kS2B, cond);

    default:
      return nullptr;
  }
}

TraceResult Builder::Trace_OP_lnot(InterpInfo* info) {
  TOS(op1, info, 1);
  auto output = BuildCondition(info, op1);
  if (!output) return TraceResult::NYI("invalid condition type");

  PopAndRcDec(info);
  Push(graph_->AddValue(info, Opcode::kFlipBool, output));
  return TraceResult::Done();
}

void Builder::AddBranchExitSnapshot(InterpInfo* info, const void* untaken_pc) {
  auto loop = bytecode_range();

  if (!loop.Contains(untaken_pc)) {
    SnapshotBefore(info, Checkpoint::kLoopExit);
  } else {
    // Notes the root branch may already jump out of the loop, which means the
    // loop exit exists
    auto checkpoint = SnapshotBefore(info, Checkpoint::kBranch);
    trace_snapshot_list_.emplace_back(untaken_pc, nullptr, checkpoint);
  }
}

// ----------------------------------------------------------------------------
// Control Flow: Branch
//
// Long term project (dpeng): Add hyper-block detection and scheduling
TraceResult Builder::TraceBranchIf(InterpInfo* info, bool if_true) {
  TOS(tos, info, 1);

  const auto jmp_pc = info->PCNext(info->GetI32());
  const auto next_pc = info->PCNext(4);
  auto cond = BuildCondition(info, tos);
  if (!cond) {
    return TraceResult::NYI("branch: invalid condition");
  }

  // JVal's to boolean value
  if (auto maybe_bool = info->GetTemp(info->temp_size() - 1).ToBool();
      !maybe_bool) {
    return TraceResult::ReferenceError("uninitialized condition");
  } else {
    auto condition_value = *maybe_bool;

    // FIXME(dpeng): Move this branch elimination out of the builder to the
    //   optimizer. For now, we put it here to allow quicker iteration and
    //   testing, notes, we really should not do anything here.
    if (cond->node_checked()->IsConst()) {
      // Since the branch is constant, which means its evaluation is always
      // the same, ie no need to check for deoptimization cases since it will
      // never trigger deoptimization. The other branch, which is not taken
      // during trace, is dead always
      //
      // We end up with a no-op here :)
    } else {
      // Based on the jump position to calculate the untaken branch and mark
      // it inside of the trace snapshot list for later resolution
      if (if_true) {
        if (condition_value) {
          AddBranchExitSnapshot(info, next_pc);
        } else {
          AddBranchExitSnapshot(info, jmp_pc);
        }
      } else {
        if (condition_value) {
          AddBranchExitSnapshot(info, jmp_pc);
        } else {
          AddBranchExitSnapshot(info, next_pc);
        }
      }

      // The if_true/if_false jump will be specialized biased towards a
      // certain condition, ie the condition we just observed. It means we
      // will insert a deoptimization point here
      AddGuardOp(info, condition_value ? Opcode::kGIsTrue : Opcode::kGIsFalse,
                 cond);
    }

    // setup the expectation jumpping position to detect whether we have a bug
    // or mis-interpreted behaviors
    const void* target;

    // figure out the guessing jumpping position
    if (if_true && condition_value) {
      target = jmp_pc;
    } else if (if_true && !condition_value) {
      target = next_pc;
    } else if (!if_true && condition_value) {
      target = next_pc;
    } else {
      target = jmp_pc;
    }

    UpdateJumpTo(target);

    // Drop conditional's RC
    PopAndRcDec(info);

    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_if_true(InterpInfo* info) {
  return TraceBranchIf(info, true);
}

TraceResult Builder::Trace_OP_if_false(InterpInfo* info) {
  return TraceBranchIf(info, false);
}

TraceResult Builder::Trace_OP_goto(InterpInfo* info) {
  const auto jump_pc = info->PCNext(info->GetI32());
  UpdateJumpTo(jump_pc);
  return TraceResult::Done();
}

// The real question we have is that wehether we are supposed to jit a trace
// which contains the return here. The issue we have here is that we may have
// following situation that will trigger a jump
//
// for (let i = 0; i < 100; i++) {
//  for (let j = 0; j < 1; j++) {
//    ....
//  }
// }
//
// The inner loop will be triggered because of outer loop. And the issue is
// the inner loop's jit frame will be enterred and then exit continously. But
// if we do not handle OP_return, the inner loop, maybe really large may never
// be jitted anymore. I see this issue during MD5 testing, the MD5 itself is a
// 16 step per iteration loop and if we feed a 16 bytes string into MD5
// function and we try to benchmark the case for 1000 times, then inner loop
// will be triggered for jitting but it will never really be jitted due to the
// fact that it does not have enough iteration
void Builder::DoFrameFinish(InterpInfo* info) {
  auto drop_rc = [&](SSAVar** start, SSAVar** end) {
    auto idx = 0;
    for (; start != end; start++) {
      if (auto v = *start; v) {
        RcDec(info, v);
      } else {
        auto memory =
            graph_->AddValue(info, Opcode::kLocGet, graph_->KI32(info, idx));
        RcDec(info, memory);
      }
      ++idx;
    }
  };

  drop_rc(local(), local_end());
  drop_rc(temp(), temp_end());

  FlushRC(info);
}

// During the return finalization phase, the VM interpreter will try decrease
// ref count for all the value that is part of the local and temporary
// variable
TraceResult Builder::DoReturn(InterpInfo* info, SSAVar* return_value) {
  if (IsEntry()) {
    DoFrameFinish(info);
    graph_->AddOp(info, Opcode::kReturn, return_value);
    return TraceResult::Done();
  } else {
    return StopInlineCall(info, return_value);
  }
}

TraceResult Builder::Trace_OP_return(InterpInfo* info) {
  auto value = StkGet(info, 1);
  Pop(1);
  return DoReturn(info, value);
}

TraceResult Builder::Trace_OP_return_undef(InterpInfo* info) {
  return DoReturn(info, graph_->KUndefined(info));
}

// ----------------------------------------------------------------------------
// Inline Call Handling
// ----------------------------------------------------------------------------
//
// 1) Ignore, we cannot do inline, ie not a script function
// 2) Done, the inline is done
// 3) XXXX, error, just return
TraceResult Builder::StartInlineCall(InterpInfo* info, int call_argc,
                                     bool is_tail_call) {
  auto foreach_param = [&](std::function<void(int, SSAVar*)>&& visitor) {
    for (auto arg_idx = StkIdx(call_argc), arg_end = StkIdx(0);
         arg_idx != arg_end; arg_idx++) {
      auto param = temp()[arg_idx];
      CHECK(param);
      visitor(arg_idx, param);
    }
  };

  TOS(func_object, info, call_argc + 1);
  auto this_object = graph_->KUndefined(info);
  auto tar_object = graph_->KUndefined(info);

  // the current javascript callable object, notes it may not be a script func
  auto jval_func = info->GetTemp(StkIdx(call_argc + 1));

  // generate bytecode that make sure the callee is a function bytecode
  AddGuardOp(info, Opcode::kGObjectIsScriptFunction, func_object);

  // get its bytecode information
  auto bc_info = jval_func.GetBytecodeInfo();
  CHECK(bc_info);

  // setup the inline frame ir, this will translate into machine code
  {
    const auto call_depth = graph_->KInt(info, InlineCallDepth() + 1);
    const auto slot_size = graph_->KInt(info, bc_info->stack_size);
    const auto arg_count = graph_->KInt(
        info, std::max(call_argc, static_cast<int>(bc_info->arg_count)));
    const auto loc_count = graph_->KInt(info, bc_info->loc_count);

    auto op = graph_->AddOp(info, Opcode::kEnterInlineFrame, 7);

    op->Set(0, call_depth);
    op->Set(1, slot_size);
    op->Set(2, arg_count);
    op->Set(3, loc_count);
    op->Set(4, func_object);
    op->Set(5, this_object);
    op->Set(6, tar_object);
  }

  std::vector<SSAVar*> parameter_list;

  // collect all the parameter in the caller's stack and store it temporarily we
  // will re-apply all the parameters back to the callee's simulated stack
  foreach_param([&](int arg_idx, SSAVar* param) {
    UNUSE(arg_idx);
    parameter_list.push_back(param);
  });

  // enter the simulated inline stack frame
  EnterInlineFrame(info, call_argc, *bc_info, parameter_list, func_object,
                   this_object, tar_object, call_argc + 1, is_tail_call);

  // setup the runtime to allow track cross the call boundary
  SetInlineQuakeTrace(quake_->js_context(), quake_);

  return TraceResult::Done();
}

TraceResult Builder::StopInlineCall(InterpInfo* info, SSAVar* return_value) {
  SSAVar* func_object;
  SSAVar* this_object;
  SSAVar* tar_object;
  std::size_t call_argc;
  std::size_t pop_size;
  std::size_t var_ref_size;
  bool is_tail;

  auto foreach_param = [&](std::function<void(int, SSAVar*)>&& visitor) {
    for (auto arg_idx = StkIdx(call_argc), arg_end = StkIdx(0);
         arg_idx != arg_end; arg_idx++) {
      auto param = temp()[arg_idx];
      CHECK(param);
      visitor(arg_idx, param);
    }
  };

  CHECK(IsInline());

  {
    const auto& frame = cur_frame();

    func_object = frame.function_object();
    this_object = frame.this_object();
    tar_object = frame.target_object();
    call_argc = frame.param_size();
    pop_size = frame.pop_size();
    is_tail = frame.is_tail();
    var_ref_size = frame.var_ref_size();
  }

  // Generate a close_var_loop instruction, this may effectively turn into a
  // no-op
  graph_->AddOp(info, Opcode::kCloseVarRef, graph_->KInt(info, var_ref_size));

  // Free all the locals and temps as what VM interpreter does
  DoFrameFinish(info);

  // Leave the simulated stacks
  LeaveFrame(info);

  // Emit a leave frame instruction to mark we are done, this basically will
  // translate to nothing
  {
    auto leave_frame = graph_->AddOp(info, Opcode::kLeaveInlineFrame,
                                     graph_->KInt(info, var_ref_size));

    // Make sure to generate a checkpoint after the StopInlineCall is done
    UpdateCheckpointAfter(info, Checkpoint::kSideEffect, leave_frame);
  }

  if (!is_tail) {
    // We are not a tail call, and we have upper function to continue, then
    // finish the upper function's OP_call's return part job, ie cleanup the
    // reference counting and manipulate the stack

    if (func_object) RcDec(info, func_object);
    if (this_object) RcDec(info, this_object);
    if (tar_object) RcDec(info, tar_object);
    foreach_param([&](int, SSAVar* param) { RcDec(info, param); });

    Pop(pop_size);
    Push(return_value);
    return TraceResult::Done();
  } else {
    // We are in a tail call, then we may need to return to VM interpreter or
    // return to another inline call

    if (IsEntry()) {
      // We have nothing left, just directly perform an implicit return here
      return DoReturn(info, return_value);
    } else {
      // We have other inline frame below us, so we need to cleanup ourself and
      // return back
      DoFrameFinish(info);

      // Pop up the caller frame since we are an inline frame that is been
      // tail called against with
      return StopInlineCall(info, return_value);
    }
  }
}

// FIXME(dpeng): Add fancier inline decider to avoid optimistic inline operation
TraceResult Builder::TryInlineCall(InterpInfo* info, int call_argc,
                                   bool is_tail_call) {
  if (InlineCallDepth() >= FLAG_inline_call_max_depth.Get()) {
    return TraceResult::Ignore();
  }

  // The current javascript caller is not a bytecode target, but other thing,
  // just bailout here
  if (auto jval_func = info->GetTemp(StkIdx(call_argc + 1));
      !jval_func.IsScriptFunction()) {
    return TraceResult::Ignore();
  }

  return StartInlineCall(info, call_argc, is_tail_call);
}

// ----------------------------------------------------------------------------
// Script Call Handling
//
// The function handling is as following, since the VM frame requires a stack
// for passing around parameters in boxed format, we need to generate extra
// long instruction to setup the whole stack to be used by the call.
SSAVar* Builder::DoTraceCall(InterpInfo* info, int call_argc,
                             SSAVar* func_object, SSAVar* this_object,
                             SSAVar* tar_object, TraceCallType call_type) {
  auto foreach_param = [&](std::function<void(int, SSAVar*)>&& visitor) {
    for (auto arg_idx = StkIdx(call_argc), arg_end = StkIdx(0);
         arg_idx != arg_end; arg_idx++) {
      auto param = temp()[arg_idx];
      CHECK(param);
      visitor(arg_idx, param);
    }
  };

  // 1) Generate call parameter in boxed version and store them back to the
  //    stack inside of the frame.
  //
  //    Notes, here we do not perform unbox operation on the value stored on
  //    the stack, due to the introduction of call instruction, our stack now
  //    can have untypped instruction
  foreach_param([&](int arg_idx, SSAVar* param) {
    auto idx = graph_->KI32(info, StkOffsetFromRIndex(arg_idx));
    BoxToTemp(info, idx, param, false);
  });

  // 2) Generate the call node
  auto call = graph_->AddValue(info, Opcode::kJSCall, 5);
  call->node_checked()->Set(0, func_object);
  call->node_checked()->Set(1, this_object);
  call->node_checked()->Set(2, tar_object);
  call->node_checked()->Set(3, graph_->KI32(info, call_argc));
  call->node_checked()->Set(4, graph_->KI32(info, StkOffset(call_argc)));

  if (call_type == TraceCallType::kCall) {
    // 3.1) Right after the call is been generated, we need to insert a
    // checkpoint
    //      here to allow the exceptio to bailout correctly, otherwise it is
    //      invalid
    UpdateCheckpointAfter(info, Checkpoint::kException, call->node_checked());

    // 3.2) Insert a deoptimization check before the RC check, based on all
    // the
    //      quickjs VM handler
    GuardNotException(info, call);
  }

  // 4) Generate RC deference based on the type of input parameter
  if (call_type == TraceCallType::kCall) {
    RcDec(info, func_object);
    RcDec(info, this_object);
    RcDec(info, tar_object);
  }

  if (call_type == TraceCallType::kCall) {
    foreach_param([&](int, SSAVar* param) { RcDec(info, param); });
  }

  return call;
}

TraceResult Builder::DoVMCall(InterpInfo* info, int call_argc,
                              bool is_tail_call) {
  TOS(func_object, info, call_argc + 1);
  auto this_object = graph_->KUndefined(info);
  auto tar_object = graph_->KUndefined(info);

  auto call = DoTraceCall(
      info, call_argc, func_object, this_object, tar_object,
      is_tail_call ? TraceCallType::kTailCall : TraceCallType::kCall);

  if (!is_tail_call) {
    Pop(call_argc + 1);
  }
  Push(call);
  return TraceResult::Done();
}

TraceResult Builder::DoFreeCall(InterpInfo* info, bool is_tail_call) {
  auto call_argc = info->GetU16();
  if (auto x = TryInlineCall(info, call_argc, is_tail_call);
      x.reason() == BailoutReason::kIgnore) {
    return DoVMCall(info, call_argc, is_tail_call);
  } else {
    return x;
  }
}

TraceResult Builder::Trace_OP_call(InterpInfo* info) {
  return DoFreeCall(info, false);
}

TraceResult Builder::Trace_OP_tail_call(InterpInfo* info) {
  if (auto r = DoFreeCall(info, true); !r) {
    return r;
  }

  auto value = StkGet(info, 1);
  Pop(1);
  return DoReturn(info, value);
}

TraceResult Builder::DoMethodCall(InterpInfo* info, bool is_tail_call) {
  auto call_argc = info->GetU16();

  TOS(func_object, info, call_argc + 1);
  TOS(this_object, info, call_argc + 2);
  auto tar_object = graph_->KUndefined(info);

  auto call = DoTraceCall(
      info, call_argc, func_object, this_object, tar_object,
      is_tail_call ? TraceCallType::kTailCall : TraceCallType::kCall);

  // adjust stack position
  if (!is_tail_call) {
    Pop(call_argc + 2);
  }
  Push(call);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_call_method(InterpInfo* info) {
  return DoMethodCall(info, false);
}

TraceResult Builder::Trace_OP_tail_call_method(InterpInfo* info) {
  if (auto r = DoMethodCall(info, true); !r) {
    return r;
  }

  auto value = StkGet(info, 1);
  Pop(1);
  return DoReturn(info, value);
}

SSAVar* Builder::DoTraceCallConstructor(InterpInfo* info, int call_argc,
                                        SSAVar* func_object,
                                        SSAVar* this_object) {
  auto foreach_param = [&](std::function<void(int, SSAVar*)>&& visitor) {
    for (auto arg_idx = StkIdx(call_argc), arg_end = StkIdx(0);
         arg_idx != arg_end; arg_idx++) {
      auto param = temp()[arg_idx];
      CHECK(param);
      visitor(arg_idx, param);
    }
  };
  // 1) Generate call parameter in boxed version and store them back to the
  //    stack inside of the frame
  foreach_param([&](int arg_idx, SSAVar* param) {
    auto idx = graph_->KI32(info, StkOffsetFromRIndex(arg_idx));
    BoxToTemp(info, idx, param, false);
  });

  // 2) Generate the call node
  auto call = graph_->AddValue(info, Opcode::kJSCCall, 4);
  call->node_checked()->Set(0, func_object);
  call->node_checked()->Set(1, this_object);
  call->node_checked()->Set(2, graph_->KI32(info, call_argc));
  call->node_checked()->Set(3, graph_->KI32(info, StkOffset(call_argc)));

  // 3.1) Right after the call is been generated, we need to insert a
  // checkpoint
  //      here to allow the exceptio to bailout correctly, otherwise it is
  //      invalid
  UpdateCheckpointAfter(info, Checkpoint::kException, call->node_checked());

  // 3.2) Insert a deoptimization check before the RC check, based on all the
  //      quickjs VM handler
  GuardNotException(info, call);

  // 4) Generate RC deference based on the type of input parameter
  RcDec(info, func_object);
  RcDec(info, this_object);

  foreach_param([&](int, SSAVar* param) { RcDec(info, param); });

  return call;
}

TraceResult Builder::Trace_OP_call_constructor(InterpInfo* info) {
  auto call_argc = info->GetU16();
  TOS(func_object, info, call_argc + 2);
  TOS(this_object, info, call_argc + 1);

  auto call = DoTraceCallConstructor(info, call_argc, func_object, this_object);

  // adjust stack position
  Pop(call_argc + 2);
  Push(call);
  return TraceResult::Done();
}

SSAVar* Builder::DoTraceArrayFrom(InterpInfo* info, int call_argc) {
  auto foreach_param = [&](std::function<void(int, SSAVar*)>&& visitor) {
    for (auto arg_idx = StkIdx(call_argc), arg_end = StkIdx(0);
         arg_idx != arg_end; arg_idx++) {
      auto param = temp()[arg_idx];
      CHECK(param);
      visitor(arg_idx, param);
    }
  };

  // 1) Generate call parameter in boxed version and store them back to the
  //    stack inside of the frame.
  //
  //    Notes, here we do not perform unbox operation on the value stored on
  //    the stack, due to the introduction of call instruction, our stack now
  //    can have untypped instruction
  foreach_param([&](int arg_idx, SSAVar* param) {
    auto idx = graph_->KI32(info, StkOffsetFromRIndex(arg_idx));
    BoxToTemp(info, idx, param, false);
  });

  // 2) Generate the call node
  auto param_size = graph_->KI32(info, call_argc);
  auto param_idx = graph_->KI32(info, StkOffset(call_argc));
  auto call = graph_->AddValue(info, Opcode::kArrayFrom, param_size, param_idx);

  // 3.1) Right after the call is been generated, we need to insert a
  // checkpoint
  //      here to allow the exceptio to bailout correctly, otherwise it is
  //      invalid
  UpdateCheckpointAfter(info, Checkpoint::kException, call->node_checked());

  // 3.2) Insert a deoptimization check before the RC check, based on all the
  //      quickjs VM handler
  GuardNotException(info, call);

  foreach_param([&](int, SSAVar* param) { RcDec(info, param); });

  return call;
}

TraceResult Builder::Trace_OP_array_from(InterpInfo* info) {
  auto call_argc = info->GetU16();
  auto call = DoTraceArrayFrom(info, call_argc);
  Pop(call_argc);
  Push(call);
  return TraceResult::Done();
}

SSAVar* Builder::DoTraceApply(InterpInfo* info, SSAVar* this_object,
                              int option) {
  const auto call_argc = 2;  // for apply, the call_argc is fixed

  auto foreach_param = [&](std::function<void(int, SSAVar*)>&& visitor) {
    for (auto arg_idx = StkIdx(call_argc), arg_end = StkIdx(0);
         arg_idx != arg_end; arg_idx++) {
      auto param = temp()[arg_idx];
      CHECK(param);

      visitor(arg_idx, param);
    }
  };

  foreach_param([&](int arg_idx, SSAVar* param) {
    auto idx = graph_->KI32(info, StkOffsetFromRIndex(arg_idx));
    BoxToTemp(info, idx, param, false);
  });

  auto param_idx = graph_->KI32(info, StkOffset(call_argc));
  auto opt = graph_->KI32(info, option);

  auto apply = graph_->AddValue(info, Opcode::kApply, 3);
  apply->node_checked()->Set(0, this_object);
  apply->node_checked()->Set(1, param_idx);
  apply->node_checked()->Set(2, opt);

  UpdateCheckpointAfter(info, Checkpoint::kException, apply->node_checked());
  GuardNotException(info, apply);

  RcDec(info, this_object);
  foreach_param([&](int, SSAVar* param) { RcDec(info, param); });

  return apply;
}

TraceResult Builder::Trace_OP_apply(InterpInfo* info) {
  TOS(this_object, info, 3);

  auto opt = info->GetU16();
  auto val = DoTraceApply(info, this_object, opt);
  Pop(3);
  Push(val);
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// FieldGet operation
//
// The FieldGet operation is been specialized based on type during IR
// building, and in the later LIR phase, it will be futhur specialized into a
// fast path get + fallback slow path. This is totally transparent from user
// perspectives
//
// Each type will have its own specialized fast+slow path access mode.
//
// ----------------------------------------------------------------------------
// Notes, due to we do not have proper branch PHI construct for now, we cannot
// generate IR as following:
//
//  #1. fast_path
//  #2. if_done goto #4
//  #3. slow_path
//  #4. normal value
//
//  The reason is the above construct requires proper handling if branch graph
//  which is not addressed for now, since we are in a linear trace most of the
//  time.
//
//  The above case can be expressed though for now, but it requires some sort
//  of checking of done or not done, which is not easy to express for now
//
//  Until now, we generate a single IR/LIR to express such complicated
//  operation without furthur lowering into close to metal LIR code. And
//  address the complication inside of the code generation. This is not ideal
//  since CG does not have optimization, but for now it is okay

std::pair<TraceResult, SSAVar*> Builder::DoFieldGet(InterpInfo* info,
                                                    SSAVar* target,
                                                    std::uint32_t atom_ref) {
  SSAVar* object = nullptr;
  switch (target->type().tag()) {
    case Type::kNull:
    case Type::kUndefined:
      // all through exception here, no need to trace
      return std::make_pair(
          TraceResult::BytecodeError("invalid field_get on invalid type"),
          nullptr);

    case Type::kInt:
    case Type::kFloat64:
    case Type::kBool:
    case Type::kSymbol:  // symbol is handle specifically

      // require extra operation, ie loading the primitive type's proto object
      // for field access
      //
      // The following ir node generate a boxed version value, which will not
      // work for us, so we need to unbox it. Since the prototype is
      // gauranteed to be object(ptr), we just unbox the pointer regardlessly
      object = graph_->AddValue(
          info, Opcode::kLoadPrimitiveProto,
          graph_->KInt(info, T2JValTagChecked(target->type().tag())));

      // unbox the value into object pointer, this is gauranteed to work
      object = graph_->AddValue(info, Opcode::kUnboxPtr, Type{Type::kObject},
                                object);

      break;

    case Type::kString:
      // FIXME(dpeng): Specialize access of string's character into LIR for
      // performance.
      if (atom_ref == JS_ATOM_length) {
        // specialized to access its length directly without needing to
        // bailout to runtime call
        return std::make_pair(TraceResult::Done(),
                              graph_->AddValue(info, Opcode::kStrLen, target));
      } else {
        // Notes, string's field access will *NEVER THROW* exception, so no
        // need to check the return value here
        auto field_get = graph_->AddValue(
            info, Opcode::kStrFieldGet, target,
            graph_->KI32(info, static_cast<std::int32_t>(atom_ref)));

        return std::make_pair(TraceResult::Done(), field_get);
      }

    case Type::kObject:
      // All the above type can be viewed as an object when access its
      // internal field, ie property access
      object = target;
      break;

    default:
      return std::make_pair(TraceResult::NYI("field_get invalid type"),
                            nullptr);
  }
  CHECK(object);
  CHECK(object->type().IsObject());

  SSAVar* field_get = nullptr;
  auto qjs_object = info->GetTOS();  // top of the object
  CHECK(qjs_object.IsObject());

  auto atom = graph_->KI32(info, static_cast<std::int32_t>(atom_ref));

  if (qjs_object.IsFastArray() && atom_ref == JS_ATOM_length) {
    // Specialization against Array.length property access. Notes this can
    // fail and we will perform fallback.
    field_get = graph_->AddValue(info, Opcode::kArrayLengthGet, object);

    // update the checkpoint after the ObjectFieldGet since it may generate
    // exception
    UpdateCheckpointAfter(info, Checkpoint::kException,
                          field_get->node_checked());
    GuardNotException(info, field_get);

  } else {
    auto hash = graph_->KI32(
        info,
        static_cast<std::int32_t>(qjs_object.Object()->shape->prop_hash_mask));

    // generate the field get node
    field_get = graph_->AddValue(info, Opcode::kObjectFieldGet, 3);

    field_get->node_checked()->Set(0, object);
    field_get->node_checked()->Set(1, atom);
    field_get->node_checked()->Set(2, hash);

    // update the checkpoint after the ObjectFieldGet since it may generate
    // exception
    UpdateCheckpointAfter(info, Checkpoint::kException,
                          field_get->node_checked());
    GuardNotException(info, field_get);
  }

  // notes, we do not generate code for RC manipulation here, since it can be
  // used by different Opcode which may not manipulate RC
  return std::make_pair(TraceResult::Done(), field_get);
}

TraceResult Builder::Trace_OP_get_field(InterpInfo* info) {
  TOS(target, info, 1);
  auto atom = info->GetU32();

  if (auto [r, ssa] = DoFieldGet(info, target, atom); !r) {
    return r;
  } else {
    PopAndRcDec(info);
    Push(ssa);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_get_field2(InterpInfo* info) {
  TOS(target, info, 1);
  auto atom = info->GetU32();

  if (auto [r, ssa] = DoFieldGet(info, target, atom); !r) {
    return r;
  } else {
    Push(ssa);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_put_field(InterpInfo* info) {
  TOS(object, info, 2);
  TOS(value, info, 1);  // FIXME(dpeng): This will force us to unbox the
                        // value which causes more than enough stronger
                        // assumption against the value type. This will lead
                        // to bailout more than enough. For us the value
                        // setting can be whatever value, which we really
                        // don't care its type

  if (object->type().IsObject()) {
    auto atom_ref = info->GetU32();
    auto field_put = graph_->AddValue(info, Opcode::kObjectFieldPut, 3);

    field_put->node_checked()->Set(0, object);
    field_put->node_checked()->Set(
        1, graph_->KI32(info, static_cast<std::int32_t>(atom_ref)));
    field_put->node_checked()->Set(2, value);

    // generate a is not exception call, go guard it off the exception
    UpdateCheckpointAfter(info, Checkpoint::kException,
                          field_put->node_checked());
    GuardNotException(info, field_put);

    RcDec(info, object);
    Pop(2);

    return TraceResult::Done();
  } else {
    return TraceResult::NYI("put_field: have invalid type");
  }
}

TraceResult Builder::Trace_OP_define_field(InterpInfo* info) {
  TOS(object, info, 2);
  TOS(value, info, 1);  // FIXME(dpeng): This will force us to unbox the
                        // value which causes more than enough stronger
                        // assumption against the value type. This will lead
                        // to bailout more than enough. For us the value
                        // setting can be whatever value, which we really
                        // don't care its type

  if (object->type().IsObject()) {
    auto atom_ref = info->GetU32();
    auto field_define = graph_->AddValue(info, Opcode::kObjectFieldDefine, 3);

    field_define->node_checked()->Set(0, object);
    field_define->node_checked()->Set(
        1, graph_->KI32(info, static_cast<std::int32_t>(atom_ref)));
    field_define->node_checked()->Set(2, value);

    // generate a is not exception call, go guard it off the exception
    UpdateCheckpointAfter(info, Checkpoint::kException,
                          field_define->node_checked());
    GuardNotException(info, field_define);

    Pop(1);

    return TraceResult::Done();

  } else {
    return TraceResult::NYI("define_field: have invalid type");
  }
}

TraceResult Builder::Trace_OP_copy_data_properties(InterpInfo* info) {
  // FIXME(dpeng): Fix the following code to not use case, this is really
  //   convoluted
  std::int32_t mask = static_cast<std::int8_t>(info->GetU8());

  auto idx0 = -1 - (mask & 3);
  auto idx1 = -1 - ((mask >> 2) & 7);
  auto idx2 = -1 - ((mask >> 5) & 7);

  TOS(a0, info, idx0);
  TOS(a1, info, idx1);
  TOS(a2, info, idx2);

  auto value = graph_->AddValue(info, Opcode::kCopyDataProperties, 3);
  value->node_checked()->Set(0, a0);
  value->node_checked()->Set(1, a1);
  value->node_checked()->Set(2, a2);

  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_append(InterpInfo* info) {
  TOS(a0, info, 3);  // target array, ie what to append to
  TOS(a1, info, 2);  // the integer indicator
  TOS(a2, info, 1);  // source array, ie what to copy from

  auto value = graph_->AddValue(info, Opcode::kAppend, 3);
  value->node_checked()->Set(0, a0);
  value->node_checked()->Set(1, a1);
  value->node_checked()->Set(2, a2);

  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);

  PopAndRcDec(info);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_define_func(InterpInfo* info) {
  TOS(func, info, 1);
  auto atom_ref = info->GetU32();
  auto flag_ref = info->GetU8();

  if (!func->type().IsObject()) {
    return TraceResult::BytecodeError("define_func: target is not a function");
  }

  auto value = graph_->AddValue(info, Opcode::kGlobalDefineFunction, 3);

  value->node_checked()->Set(0, graph_->KI32(info, atom_ref));
  value->node_checked()->Set(1, func);
  value->node_checked()->Set(2, graph_->KI32(info, flag_ref));

  UpdateCheckpointAfter(info, Checkpoint::kException, value->node_checked());
  GuardNotException(info, value);
  PopAndRcDec(info);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_set_name(InterpInfo* info) {
  TOS(object, info, 1);

  if (object->type().IsObject()) {
    auto atom_ref = info->GetU32();
    auto name_set = graph_->AddValue(info, Opcode::kObjectNameSet, 2);
    name_set->node_checked()->Set(0, object);
    name_set->node_checked()->Set(
        1, graph_->KI32(info, static_cast<std::int32_t>(atom_ref)));

    // generate a is not exception call, go guard it off the exception
    UpdateCheckpointAfter(info, Checkpoint::kException,
                          name_set->node_checked());
    GuardNotException(info, name_set);

    return TraceResult::Done();

  } else {
    return TraceResult::NYI("name_set: have invalid type");
  }
}

TraceResult Builder::Trace_OP_set_name_computed(InterpInfo* info) {
  TOS(object, info, 1);
  TOS(value, info, 2);

  if (object->type().IsObject()) {
    auto name_set = graph_->AddValue(info, Opcode::kObjectNameSetComputed, 2);
    name_set->node_checked()->Set(0, object);
    name_set->node_checked()->Set(1, value);

    // generate a is not exception call, go guard it off the exception
    UpdateCheckpointAfter(info, Checkpoint::kException,
                          name_set->node_checked());
    GuardNotException(info, name_set);

    return TraceResult::Done();

  } else {
    return TraceResult::NYI("name_set_computed: have invalid type");
  }
}

SSAVar* Builder::DoTraceLoadArray(InterpInfo* info, SSAVar* array,
                                  SSAVar* index) {
  SSAVar* output = nullptr;

  // generate the value
  auto gen = [&](Opcode opcode) -> SSAVar* {
    return graph_->AddValue(info, opcode, array, index);
  };

  if (array->type().IsObject() && index->type().IsInt()) {
    // If the index is an constant value, ie we already know its value, then
    // check whether it is a valid index, otherwise fallback to slow path
    if (index->node_checked()->opcode() != Opcode::kKI32 ||
        index->node_checked()->Int32() >= 0) {
      auto qjs_value = info->GetTempVal(2);  // get the current qjs value
                                             // try to specialize it now
      auto qjs_obj = qjs_value.Object();

      switch (qjs_obj->class_id) {
        case JS_CLASS_ARRAY:
          output = gen(Opcode::kLoadArrayValue);
          break;

        case JS_CLASS_ARGUMENTS:
          output = gen(Opcode::kLoadArrayArguments);
          break;

        case JS_CLASS_INT8_ARRAY:
          output = gen(Opcode::kLoadArrayInt8);
          break;

        case JS_CLASS_UINT8_ARRAY:
          output = gen(Opcode::kLoadArrayUInt8);
          break;

        case JS_CLASS_UINT8C_ARRAY:
          output = gen(Opcode::kLoadArrayUInt8C);
          break;

        case JS_CLASS_INT16_ARRAY:
          output = gen(Opcode::kLoadArrayInt16);
          break;

        case JS_CLASS_UINT16_ARRAY:
          output = gen(Opcode::kLoadArrayUInt16);
          break;

        case JS_CLASS_INT32_ARRAY:
          output = gen(Opcode::kLoadArrayInt32);
          break;

        case JS_CLASS_UINT32_ARRAY:
          output = gen(Opcode::kLoadArrayUInt32);
          break;

        case JS_CLASS_FLOAT32_ARRAY:
          output = gen(Opcode::kLoadArrayF32);
          break;

        case JS_CLASS_FLOAT64_ARRAY:
          output = gen(Opcode::kLoadArrayF64);
          break;

        default:
          break;
      }
    }
  }

  // generalized version, notes LoadArray will directly translate to a
  // runtime call
  if (!output) output = gen(Opcode::kLoadArray);

  return output;
}

TraceResult Builder::Trace_OP_get_array_el(InterpInfo* info) {
  TOS(array, info, 2);
  TOS(index, info, 1);

  auto output = DoTraceLoadArray(info, array, index);

  // The fact is OP_get_array_el handler will fire exception after the RC is
  // done, which is really really wired and inconsitent along with other
  // handler
  RcDec(info, array);
  Pop(2);
  Push(output);

  // Generate a guard to guard the output, this is really wired since the
  // guard needs to be generated after the RC is done.
  UpdatePendingGuardNotException(info, output);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_get_array_el2(InterpInfo* info) {
  TOS(array, info, 2);
  TOS(index, info, 1);

  auto output = DoTraceLoadArray(info, array, index);

  UpdateCheckpointAfter(info, Checkpoint::kException, output->node_checked());
  GuardNotException(info, output);

  // The fact is OP_get_array_el handler will fire exception after the RC is
  // done, which is really really wired and inconsitent along with other
  // handler
  Pop(1);
  Push(output);

  return TraceResult::Done();
}

// TODO(dpeng): Support store specialization?
//   Currently the array store operation is the simplest to support, though
//   it may not be very practical. The cost is you always have to maintain
//   a piece of assembly code, on different platforms, for this operation
//   which may not be that ideal
TraceResult Builder::Trace_OP_put_array_el(InterpInfo* info) {
  TOS(array, info, 3);
  TOS(index, info, 2);
  TOS(value, info, 1);

  auto output = graph_->AddValue(info, Opcode::kSetPropertyValue, 4);

  output->node_checked()->Set(0, array);
  output->node_checked()->Set(1, index);
  output->node_checked()->Set(2, value);
  output->node_checked()->Set(3, graph_->KI32(info, JS_PROP_THROW_STRICT));

  RcDec(info, array);
  Pop(3);

  UpdatePendingGuardNotException(info, output);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_define_array_el(InterpInfo* info) {
  TOS(array, info, 3);
  TOS(index, info, 2);
  TOS(value, info, 1);

  auto output = graph_->AddValue(info, Opcode::kDefinePropertyValueValue, 4);

  output->node_checked()->Set(0, array);
  output->node_checked()->Set(1, index);
  output->node_checked()->Set(2, value);
  output->node_checked()->Set(
      3, graph_->KI32(info, JS_PROP_C_W_E | JS_PROP_THROW));

  UpdateCheckpointAfter(info, Checkpoint::kException, output->node_checked());
  GuardNotException(info, output);

  Pop(1);

  return TraceResult::Done();
}

// ref_value
//
// Notes ref_value is been implemented the same as array loading, for now it
// will be specialized based on the DoTraceLoadArray function. This function
// takes care of all the situations based on the traced value to perform
// specialization
TraceResult Builder::Trace_OP_get_ref_value(InterpInfo* info) {
  TOS(target, info, 2);
  TOS(index, info, 1);

  if (target->type().IsUndefined()) {
    return TraceResult::BytecodeError("get_ref_value: target is undefined");
  }

  auto output = DoTraceLoadArray(info, target, index);
  UpdateCheckpointAfter(info, Checkpoint::kException, output->node_checked());

  Pop(1);
  Push(output);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_put_ref_value(InterpInfo* info) {
  TOS(target, info, 3);
  TOS(index, info, 2);
  TOS(value, info, 1);

  auto flag = JS_PROP_THROW_STRICT;

  if (target->type().IsUndefined()) {
    if (info->IsModeStrict()) {
      return TraceResult::BytecodeError("put_ref_value: target is undefined");
    } else {
      target = graph_->AddValue(
          info,
          Opcode::kContextLoadGlobalObject  // notes the load global object
                                            // will return an object which
                                            // has already bump its rc
      );
    }
  } else {
    if (info->IsModeStrict()) flag |= JS_PROP_NO_ADD;
  }

  auto ret = graph_->AddValue(info, Opcode::kSetPropertyValue, 4);
  ret->node_checked()->Set(0, target);
  ret->node_checked()->Set(1, index);
  ret->node_checked()->Set(2, value);
  ret->node_checked()->Set(3, graph_->KI32(info, flag));

  RcDec(info, target);
  Pop(3);

  // The exception check is after the reference count drop
  UpdatePendingGuardNotException(info, ret);
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// global
TraceResult Builder::DoGetGlobal(InterpInfo* info, bool throw_when_undef) {
  auto atom_ref = info->GetU32();

  // Try to probe the global value type in order to decide what types of
  // specialization we are supposed to perform here
  auto type = info->qjs()->api.js_global_check_symbol(
      info->qjs_context(), atom_ref, static_cast<int>(throw_when_undef));

  auto gen = [&](Opcode opcode) {
    auto get_var = graph_->AddValue(
        info, opcode, graph_->KI32(info, static_cast<std::int32_t>(atom_ref)),
        graph_->KBool(info, throw_when_undef));
    UpdateCheckpointAfter(info, Checkpoint::kException,
                          get_var->node_checked());
    GuardNotException(info, get_var);
    Push(get_var);  // notes do not bump RC
  };

  switch (type) {
    case QJS_GLOBAL_VAR:
      gen(Opcode::kGlobalGetVar);
      break;

    case QJS_MUST_GLOBAL:
      gen(Opcode::kGlobalGetGlobal);
      break;

    default:
      gen(Opcode::kGlobalGet);
      break;
  }

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_get_var_undef(InterpInfo* info) {
  return DoGetGlobal(info, false);
}

TraceResult Builder::Trace_OP_get_var(InterpInfo* info) {
  return DoGetGlobal(info, true);
}

TraceResult Builder::DoSetGlobal(InterpInfo* info, int flag) {
  TOS(value, info, 1);  // FIXME(dpeng): untypped

  auto atom_ref = info->GetU32();
  auto set_var = graph_->AddValue(info, Opcode::kGlobalSet, 3);

  set_var->node_checked()->Set(
      0, graph_->KI32(info, static_cast<std::int32_t>(atom_ref)));
  set_var->node_checked()->Set(1, value);
  set_var->node_checked()->Set(2, graph_->KI32(info, flag));

  UpdateCheckpointAfter(info, Checkpoint::kException, set_var->node_checked());
  GuardNotException(info, set_var);

  Pop(1);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_put_var(InterpInfo* info) {
  return DoSetGlobal(info, 0);
}

TraceResult Builder::Trace_OP_put_var_init(InterpInfo* info) {
  return DoSetGlobal(info, 1);
}

TraceResult Builder::Trace_OP_put_var_strict(InterpInfo* info) {
  TOS(cond, info, 2);

  if (cond->type().IsInt())
    return TraceResult::BytecodeError("put_var_strict: invalid cond type");

  // Guard against the condition to be not the zero. If it is zero, then
  // bailout to VM interpreter, since it will force VM to throw an exception,
  // ie an reference error for global not defined
  AddGuardValue(info, Opcode::kGIsNotZero, cond);

  if (auto r = DoSetGlobal(info, 2); !r) {
    return r;
  } else {
    Pop(1);
    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_define_var(InterpInfo* info) {
  auto atom = info->GetU32();
  auto flag = info->GetU8();

  auto define_var =
      graph_->AddValue(info, Opcode::kGlobalDefine,
                       graph_->KI32(info, static_cast<std::int32_t>(atom)),
                       graph_->KI32(info, static_cast<std::int32_t>(flag)));

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        define_var->node_checked());
  GuardNotException(info, define_var);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_check_define_var(InterpInfo* info) {
  auto atom = info->GetU32();
  auto flag = info->GetU8();

  auto define_var =
      graph_->AddValue(info, Opcode::kGlobalCheckDefine,
                       graph_->KI32(info, static_cast<std::int32_t>(atom)),
                       graph_->KI32(info, static_cast<std::int32_t>(flag)));

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        define_var->node_checked());
  GuardNotException(info, define_var);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_check_var(InterpInfo* info) {
  auto atom_ref = info->GetU32();
  auto check_var =
      graph_->AddValue(info, Opcode::kGlobalCheck,
                       graph_->KI32(info, static_cast<std::int32_t>(atom_ref)));

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        check_var->node_checked());
  GuardNotException(info, check_var);
  Push(check_var);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_check_ctor_return(InterpInfo* info) {
  TOS(target, info, 1);

  if (target->type().IsObject()) {
    Push(graph_->True(info));
  } else {
    if (!target->type().IsUndefined()) {
      return TraceResult::BytecodeError(
          "check_ctor_return: type is not undefined");
    } else {
      Push(graph_->False(info));
    }
  }
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_check_ctor(InterpInfo* info) {
  TOS(target, info, 1);
  if (target->type().IsUndefined()) {
    return TraceResult::BytecodeError("check_ctor: type is undefined");
  }
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_check_brand(InterpInfo* info) {
  TOS(target, info, 2);
  TOS(value, info, 1);

  auto check_brand =
      graph_->AddValue(info, Opcode::kJSCheckBrand, target, value);

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        check_brand->node_checked());
  GuardNotException(info, check_brand);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_add_brand(InterpInfo* info) {
  TOS(target, info, 2);
  TOS(value, info, 1);

  auto add_brand = graph_->AddValue(info, Opcode::kJSAddBrand, target, value);

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        add_brand->node_checked());
  GuardNotException(info, add_brand);

  RcDec(info, target);
  RcDec(info, value);

  Pop(2);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_regexp(InterpInfo* info) {
  TOS(target, info, 2);
  TOS(value, info, 1);

  // Notes(dpeng): The VM's OP_regexp handler does not check return value from
  //   js_regexp_constructor_internal, which may return an exception. The only
  //   reason why it returns exception is when both input operand is not
  //   string
  //
  //   Since the VM does not check, we can assume that the input is always
  //   valid and will never be none-string. As one could guess, this bytecode
  //   is only emitted during parsing regexp literal, which is definitly valid
  //   regexp

  auto regexp = graph_->AddValue(info, Opcode::kJSRegexp, target, value);
  Pop(2);
  Push(regexp);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_get_super(InterpInfo* info) {
  TOS(target, info, 1);

  auto get_super = graph_->AddValue(info, Opcode::kJSGetSuper, target);

  UpdateCheckpointAfter(info, Checkpoint::kException,
                        get_super->node_checked());
  GuardNotException(info, get_super);

  PopAndRcDec(info);
  Push(get_super);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_import(InterpInfo* info) {
  TOS(target, info, 1);

  auto import = graph_->AddValue(info, Opcode::kJSImport, target);

  UpdateCheckpointAfter(info, Checkpoint::kException, import->node_checked());
  GuardNotException(info, import);

  PopAndRcDec(info);
  Push(import);

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// super_value (get/set)
TraceResult Builder::Trace_OP_get_super_value(InterpInfo* info) {
  TOS(value, info, 3);
  TOS(object, info, 2);
  TOS(property, info, 1);

  // Notes(dpeng):
  // If the atom is a invalid atom, we bailout to VM interpreter and it will
  // report exception. This is slightly different since we do not generate an
  // exception checkpoint and directly bailout to exception
  auto atom = AddGuardValue(info, Opcode::kGBoxToAtom, property);

  /**
   * Notes(dpeng):
   *
   *   For now, the atom is been freed inside of the specialized runtime
   *   call kGetSuperValue. This may not be ideal since if we want to
   *   optimize the get_super_value, then we will have to have our specialized
   *   the FreeAtom + GetPropertyInternal call
   */

  auto get_prop = graph_->AddValue(info, Opcode::kJSGetSuperValue, 3);
  get_prop->node_checked()->Set(0, object);
  get_prop->node_checked()->Set(1, atom);
  get_prop->node_checked()->Set(2, value);

  UpdateCheckpointAfter(info, Checkpoint::kException, get_prop->node_checked());
  GuardNotException(info, get_prop);

  RcDec(info, property);
  RcDec(info, value);
  RcDec(info, object);

  Pop(3);
  Push(get_prop);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_put_super_value(InterpInfo* info) {
  TOS(a4, info, 4);
  TOS(a3, info, 3);
  TOS(a2, info, 2);
  TOS(a1, info, 1);

  if (!a3->type().IsObject()) {
    return TraceResult::BytecodeError("put_super_value: sp[-3] is not object");
  }
  auto atom = AddGuardValue(info, Opcode::kGBoxToAtom, a2);

  /**
   * Notes(dpeng):
   *
   *   Similar as OP_get_super_value, we have dedicated runtime handler to
   *   perform specialized operation which mimics the VM handler. This is not
   *   ideal for specialization, if we need to optimize its performance.
   */

  auto set_super = graph_->AddValue(info, Opcode::kJSPutSuperValue, 4);

  set_super->node_checked()->Set(0, a3);
  set_super->node_checked()->Set(1, atom);
  set_super->node_checked()->Set(2, a1);
  set_super->node_checked()->Set(3, a4);

  RcDec(info, a4);
  RcDec(info, a3);
  RcDec(info, a2);

  Pop(4);
  Push(set_super);

  // Notes(dpeng):
  //
  //   The quickjs's VM impelmentation is not consistent cross get_super_value
  //   and put_super_value. The get_super_value check the exception before
  //   free each RC on the local stack but the put_super_value check the
  //   exception *after* RC free. Anyway, we try our best to be consistent
  //   with the VM impelementation
  UpdatePendingGuardNotException(info, set_super);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_set_proto(InterpInfo* info) {
  TOS(proto, info, 1);
  TOS(target, info, 2);

  if (proto->type().IsObject() || proto->type().IsNull()) {
    auto set_proto = graph_->AddValue(info, Opcode::kJSSetProto, target, proto);

    UpdateCheckpointAfter(info, Checkpoint::kException,
                          set_proto->node_checked());
    GuardNotException(info, set_proto);
  }

  RcDec(info, proto);
  Pop(1);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_set_home_object(InterpInfo* info) {
  TOS(a1, info, 1);
  TOS(a2, info, 2);
  graph_->AddOp(info, Opcode::kJSSetHomeObject, a1, a2);
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// VarRef
TraceResult Builder::Trace_OP_get_var_ref(InterpInfo* info) {
  auto idx = info->GetU16();
  auto node =
      graph_->AddValue(info, Opcode::kVarRefGet, graph_->KI32(info, idx));

  if (auto [r, ssa] = Unbox(info, info->GetVarRef(idx), node); !r) {
    return r;
  } else {
    RcInc(info, ssa);
    Push(ssa);
    return TraceResult::Done();
  }
}

// VarRef is upvalue store, which is a memory store, and we need to emit a
// checkpoint of VM after the operation is done

TraceResult Builder::Trace_OP_put_var_ref(InterpInfo* info) {
  TOS(value, info, 1);
  auto idx = info->GetU16();

  // Notes, we need to try to decrease the ref-count of original var_ref slot,
  // we can load the var ref into our memory and perform the ref-count dec,
  // however this is kind of complicated. The simplest way is to emit a
  // special ir/lir to perform the ref count decrease directly.
  //
  // For, now we just do a simple version
  {
    auto old_value =
        graph_->AddValue(info, Opcode::kVarRefGet, graph_->KI32(info, idx));
    RcDec(info, old_value);
  }

  // Store the new value into the var_ref slot
  auto op =
      graph_->AddOp(info, Opcode::kVarRefSet, graph_->KI32(info, idx), value);

  // Emit a checkpoint afterwards
  AddPendingCheckpointAfter(Checkpoint::kSideEffect, op);

  Pop(1);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_set_var_ref(InterpInfo* info) {
  TOS(value, info, 1);
  auto idx = info->GetU16();

  // Notes, we need to try to decrease the ref-count of original var_ref slot,
  // we can load the var ref into our memory and perform the ref-count dec,
  // however this is kind of complicated. The simplest way is to emit a
  // special ir/lir to perform the ref count decrease directly.
  //
  // For, now we just do a simple version
  {
    auto old_value =
        graph_->AddValue(info, Opcode::kVarRefGet, graph_->KI32(info, idx));
    RcDec(info, old_value);
  }

  // Try to increase the value's ref-count, notes, this operation will be
  // deferred until the VarRefSet opcode is generated
  RcInc(info, value);

  // Store the new value into the var_ref slot
  auto op =
      graph_->AddOp(info, Opcode::kVarRefSet, graph_->KI32(info, idx), value);

  // Emit a checkpoint afterwards
  AddPendingCheckpointAfter(Checkpoint::kSideEffect, op);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_get_var_ref_check(InterpInfo* info) {
  auto idx = info->GetU16();
  auto node =
      graph_->AddValue(info, Opcode::kVarRefGet, graph_->KI32(info, idx));

  if (auto [r, ssa] = Unbox(info, info->GetVarRef(idx), node); !r) {
    return r;
  } else {
    if (ssa->type().IsUninitialized()) {
      return TraceResult::BytecodeError(
          "get_var_ref_check: uninitialized value");
    }

    RcInc(info, ssa);
    Push(ssa);

    return TraceResult::Done();
  }
}

TraceResult Builder::Trace_OP_put_var_ref_check(InterpInfo* info) {
  TOS(value, info, 1);

  auto idx = info->GetU16();

  {
    auto old_value =
        graph_->AddValue(info, Opcode::kVarRefGet, graph_->KI32(info, idx));

    UpdateCheckpointAfter(info, Checkpoint::kException,
                          old_value->node_checked());

    GuardIsNotUninitialized(info, old_value);
    RcDec(info, old_value);
  }

  auto op =
      graph_->AddOp(info, Opcode::kVarRefSet, graph_->KI32(info, idx), value);

  AddPendingCheckpointAfter(Checkpoint::kSideEffect, op);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_put_var_ref_check_init(InterpInfo* info) {
  TOS(value, info, 1);

  auto idx = info->GetU16();

  {
    auto old_value =
        graph_->AddValue(info, Opcode::kVarRefGet, graph_->KI32(info, idx));

    UpdateCheckpointAfter(info, Checkpoint::kException,
                          old_value->node_checked());

    GuardIsUninitialized(info, old_value);
    RcDec(info, old_value);
  }

  auto op =
      graph_->AddOp(info, Opcode::kVarRefSet, graph_->KI32(info, idx), value);

  AddPendingCheckpointAfter(Checkpoint::kSideEffect, op);

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Private field
TraceResult Builder::Trace_OP_put_private_field(InterpInfo* info) {
  TOS(a3, info, 3);
  TOS(a2, info, 2);
  TOS(a1, info, 1);

  auto put_private = graph_->AddValue(info, Opcode::kPrivateFieldPut, 3);
  put_private->node_checked()->Set(0, a3);
  put_private->node_checked()->Set(1, a1);
  put_private->node_checked()->Set(2, a2);

  RcDec(info, a3);
  RcDec(info, a1);

  Pop(3);

  UpdatePendingGuardNotException(info, put_private);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_define_private_field(InterpInfo* info) {
  TOS(a3, info, 3);
  TOS(a2, info, 2);
  TOS(a1, info, 1);

  auto put_private = graph_->AddValue(info, Opcode::kPrivateFieldDefine, 3);
  put_private->node_checked()->Set(0, a3);
  put_private->node_checked()->Set(1, a2);
  put_private->node_checked()->Set(2, a1);

  RcDec(info, a2);

  Pop(2);

  UpdatePendingGuardNotException(info, put_private);

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Iterator
TraceResult Builder::Trace_OP_iterator_check_object(InterpInfo* info) {
  TOS(maybe_object, info, 1);
  if (!maybe_object->type().IsObject()) {
    return TraceResult::BytecodeError("iterator_check_object: type fail");
  }
  return TraceResult::Done();
}

// --------------------------------------------------------------------------
// The bytecode of iterator_get_value_done is calling out to a handler, which
// returns a 2 elements tuple. The function essentially just does following
// things:
//
//   1) read *done* property of iterator object, denoted as $1
//   2) read *value* property of iterator oject, denoted as $2
//   3) if any of 1) or 2) failed with exception, then return (false, except)
//   4) otherwise, returns (ToBool($1), $2)
//
// --------------------------------------------------------------------------
// Notes in current JS implementation, ToBool will never fail.
//
// We can split the above logic as following, we manually create 2 ir code to
// load the JS_ATOM_done and JS_ATOM_value field from the iterator object,
// corresponding to the 1) and 2) step. One thing to note here is that we
// have to add a JS_ToBoolFree bytecode decomposition here for the $1.
// --------------------------------------------------------------------------

TraceResult Builder::Trace_OP_iterator_get_value_done(InterpInfo* info) {
  TOS(iterator, info, 1);

  SSAVar* done_field = nullptr;
  SSAVar* value_field = nullptr;

  if (!iterator->type().IsObject()) {
    return TraceResult::BytecodeError(
        "iterator_get_value_done: iterator is not an object");
  }

  // 1. Start to load iterator's done property
  if (auto [tr, val] = DoFieldGet(info, iterator, JS_ATOM_done); !tr) {
    return tr;
  } else {
    done_field = val;
  }
  RcDec(info, done_field);

  // Notes(dpeng): We cannot emit a simplified to boolean code here, since the
  // original bytecode handler does get value and done field in single shot
  // which means we cannot bailout in the middle of the handler. We will have
  // to implement a specialized stub during code gen to fully convert any
  // value to boolean. In JS, this operation will never fail
  auto done_boolean_field =
      graph_->AddValue(info, Opcode::kV2B, /* any value to boolean */
                       done_field);

  // 2. Start to load the iterator's value tag
  if (auto [tr, val] = DoFieldGet(info, iterator, JS_ATOM_value); !tr) {
    return tr;
  } else {
    value_field = val;
  }

  // 3. Finalize the rest
  RcDec(info, iterator);
  Pop(1);
  Push(value_field);
  Push(done_boolean_field);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_iterator_close(InterpInfo* info) {
  Pop(1);
  TOS(maybe_object, info, 1);
  RcDec(info, maybe_object);

  Pop(1);
  TOS(iterator, info, 1);
  if (!iterator->type().IsUndefined()) {
    auto iter_close = graph_->AddValue(info, Opcode::kIterClose, iterator);

    UpdateCheckpointAfter(info, Checkpoint::kException,
                          iter_close->node_checked());

    GuardNotException(info, iter_close);
    RcDec(info, iterator);
  }

  Pop(1);
  return TraceResult::Done();
}

// Notes this bytecode will show up around return emittion. If we really see
// it is because we somehow have inlined frame which have return instruction.
TraceResult Builder::Trace_OP_iterator_close_return(InterpInfo* info) {
  TOS(ret_value, info, 1);

  const auto temp_size = static_cast<int>(info->temp_size());
  auto total = 0;

  for (int index = temp_size - 2 /* skip the return value */; index >= 0;
       index--) {
    if (auto jval = info->GetTemp(index); jval.IsCatchOffset()) {
      // check whether we are fucked up here, ie internal error
      if (index < 3) {
        return TraceResult::BytecodeError(
            "iterator_close_return: invalid stack");
      }
      break;
    }

    // Check whether our simulated stack have such value or not
    auto tindex = temp_size - index;
    auto rindex = StkIdx(tindex);
    if (auto value = temp()[rindex]; value) {
      RcDec(info, value);
    } else {
      // load the memory node and then de-ref it
      auto memory_node = graph_->AddValue(
          info, Opcode::kTempGet, graph_->KI32(info, StkOffset(tindex)));

      // no need to unbox it
      RcDec(info, memory_node);
    }
    total++;
  }
  Pop(1 + total);  // pop return value and freed value

  // Marshal the final stack result
  TOS(a1, info, 1);
  TOS(a2, info, 2);
  TOS(a3, info, 3);
  Pop(3);

  Push(ret_value);
  Push(a3);
  Push(a2);
  Push(a1);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_iterator_next(InterpInfo* info) {
  TOS(func_object, info, 3);
  TOS(this_object, info, 4);
  auto tar_object = graph_->KUndefined(info);

  auto call = DoTraceCall(info, 1, func_object, this_object, tar_object,
                          TraceCallType::kIterator);

  Pop(1);
  Push(call);
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Range for

TraceResult Builder::Trace_OP_for_of_start(InterpInfo* info) {
  auto idx = StkIdx(1);
  if (auto iter = temp()[idx]; iter) {
    BoxToTemp(info, graph_->KI32(info, StkOffset(1)), iter, false);
    temp()[idx] = nullptr;
  }

  // Call the runtime ForOfStart operation
  {
    auto tos_idx = graph_->KI32(info, StkOffset(1));
    auto for_of = graph_->AddValue(info, Opcode::kForOfStart, tos_idx);
    UpdateCheckpointAfter(info, Checkpoint::kException, for_of->node_checked());
    GuardNotException(info, for_of);
  }

  // Marshal the stack layout. After the ForOfStart, the runtime function will
  // generate return value as output inside of the temporary stack, and we are
  // expecting the later code to perform TempGet function to load the value

  Push(nullptr); /* the newly obtained method, output as side effect */
  Push(nullptr); /* the newly obtained CatchOffset, output as side effect */

  SnapshotAfter(info, Checkpoint::kSideEffect);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_for_of_next(InterpInfo* info) {
  auto arg = info->GetU8();
  int offset = -3 - arg;

  // For all the temporary lies in the following range, [start, end] will need
  // to be packed back into the temporary array to be used by the runtime
  for (auto start = StkIdx(offset), end = StkIdx(1); start <= end; start++) {
    if (auto value = temp()[start]; value) {
      auto pos = graph_->KI32(info, StkOffsetFromRIndex(start));
      BoxToTemp(info, pos, value, false);
      temp()[start] = nullptr;
    }
  }

  // Generate for_next runtime call
  {
    auto tos_idx = graph_->KI32(info, StkOffset(1));
    auto off = graph_->KI32(info, offset);
    auto for_of = graph_->AddValue(info, Opcode::kForOfNext, tos_idx, off);
    UpdateCheckpointAfter(info, Checkpoint::kException, for_of->node_checked());
    GuardNotException(info, for_of);
  }

  // As side effect, the for_of_next runtime will output 2 new value into the
  // temporary stack
  Push(nullptr);
  Push(nullptr);

  SnapshotAfter(info, Checkpoint::kSideEffect);

  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_for_in_start(InterpInfo* info) {
  auto idx = StkIdx(1);
  if (auto iter = temp()[idx]; iter) {
    BoxToTemp(info, graph_->KI32(info, StkOffset(1)), iter, false);
    temp()[idx] = nullptr;
  }

  {
    auto tos_idx = graph_->KI32(info, StkOffset(1));
    auto for_in = graph_->AddValue(info, Opcode::kForInStart, tos_idx);
    UpdateCheckpointAfter(info, Checkpoint::kException, for_in->node_checked());
    GuardNotException(info, for_in);
  }

  SnapshotAfter(info, Checkpoint::kSideEffect);
  return TraceResult::Done();
}

TraceResult Builder::Trace_OP_for_in_next(InterpInfo* info) {
  auto idx = StkIdx(1);
  if (auto iter = temp()[idx]; iter) {
    BoxToTemp(info, graph_->KI32(info, StkOffset(1)), iter, false);
    temp()[idx] = nullptr;
  }

  {
    auto tos_idx = graph_->KI32(info, StkOffset(1));
    auto for_in = graph_->AddValue(info, Opcode::kForInNext, tos_idx);
    UpdateCheckpointAfter(info, Checkpoint::kException, for_in->node_checked());
    GuardNotException(info, for_in);
  }

  Push(nullptr); /* value */
  Push(nullptr); /* done */

  SnapshotAfter(info, Checkpoint::kSideEffect);
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Bailout

#define BAILOUT_TRACER(XX)                       \
  TraceResult Builder::Trace_##XX(InterpInfo*) { \
    return TraceResult::NYI("bailout: " #XX);    \
  }

BAILOUT_TRACER(OP_throw)
BAILOUT_TRACER(OP_throw_error)

#undef BAILOUT_TRACER  // BAILOUT_TRACER

// ----------------------------------------------------------------------------
// Nested loop support
//
// The nested loop support is described as following :
//
// 1) The outer loop will enter the nested loop's jitted code during tracing
//
//    1.1) The trace will happen in the first iteration of the jitted nested
//         loop and the outer loop will learn this by checking the bytecode
//         address
//
//    1.2) The outer trace will be sure that the jit code will be executed
//         when the inner loop's last goto is executed
//
//    1.3) After the nested loop bailout, the outer loop will learn this by
//         resuming from JIT to trace. By knowing that, the outer loop's
//         tracer will learn the inner loop's last deoptimization id, which is
//         recoreded during the bailout.
//
//    1.4) The outer loop can checkout the deoptimization of that id and learn
//         that the deoptimization is actually a loop exit, if not bailout
//         from jit since the inner loop's jit is unstable and no need to
//         perform jit link.
//
//    1.5) Once the inner loop's loop exit is learned, we start to form a side
//         trace with just a trace for loop exit bytecode. The side trace is a
//         specialized trace which does not contain a loop but is used to link
//         inner loop with outer loop.
//
//
//    1.6 Actual trace link, the inner loop will be patched to use specialized
//        deoptimization exit and then tail call(jump) to the side trace and
//        lastly the side trace will return to the caller, ie the outer loop.
//
//
// 2) The outer loop recover to normal jitting. It add a jit linking at the
//    inner loop's starting bytecode. The jit linking is essentially just a
//    call to inner loop's entry with specialized patched exit trampoline that
//    do not use normal C++ deoptimization routine but directly deoptimize to
//    a jitted entry and then jump to a jitted side exit.
//
//
// 3) The whole loop linking is done.
// ----------------------------------------------------------------------------

// Try to figure out that whether the specified bytecode is starting point of
// a certain jitted trace that just is been compiled
bool Builder::NestedLoopStartForward(InterpInfo* info) {
  auto world = quake_->world();
  if (auto nested_jit = world->GetQuakeByFirstBC(info->start_pc());
      nested_jit) {
    nested_loop_ = nested_jit;
    return true;
  }
  return false;
}

bool Builder::NestedLoopStartBackward(InterpInfo* info) {
  // try backward nested loop, ie checking whether the current interp info is
  // a valid jump and does a backward jump
  const void* jump_pc = nullptr;

  switch (info->op()) {
    case Bytecode::OP_if_true:
    case Bytecode::OP_if_false: {
      if (auto maybe_bool = info->GetTemp(info->temp_size() - 1).ToBool();
          !maybe_bool) {
        return {};
      } else {
        auto condition_value = *maybe_bool;
        if (info->op() == Bytecode::OP_if_true && !condition_value) {
          jump_pc = info->PCNext(4);
          break;
        } else if (info->op() == Bytecode::OP_if_false && condition_value) {
          jump_pc = info->PCNext(4);
          break;
        }
      }
      // fallthrough
    }
    case Bytecode::OP_goto: {
      auto offset = info->PeekI32();
      jump_pc = info->PCNext(offset);
    } break;
    default:
      return false;
  }

  if (jump_pc < info->start_pc()) {
    auto world = quake_->world();
    // backwards jump, check whether it starts a trace or not
    if (auto nested_jit = world->GetQuakeByFirstBC(jump_pc); nested_jit) {
      nested_loop_ = nested_jit;
      return true;
    }
  }

  return false;
}

std::optional<Builder::NestedLoopJump> Builder::FindNestedLoopJump(
    InterpInfo* info) {
  const void* jump_pc = nullptr;

  switch (info->op()) {
    case Bytecode::OP_if_true:
    case Bytecode::OP_if_false: {
      if (auto maybe_bool = info->GetTemp(info->temp_size() - 1).ToBool();
          !maybe_bool) {
        return {};
      } else {
        auto condition_value = *maybe_bool;
        if (info->op() == Bytecode::OP_if_true && !condition_value) {
          jump_pc = info->PCNext(4);
          break;
        } else if (info->op() == Bytecode::OP_if_false && condition_value) {
          jump_pc = info->PCNext(4);
          break;
        }
      }
      // fallthrough
    }
    case Bytecode::OP_goto: {
      auto offset = info->PeekI32();
      jump_pc = info->PCNext(offset);
    } break;
    default:
      return NestedLoopJump::kNoJump;
  }

  if (jump_pc == nested_loop_->trigger_loop_from_bc()) {
    // inner loop jumpback
    return NestedLoopJump::kLoopback;
  } else if (!nested_loop_->bytecode_range().Contains(jump_pc)) {
    // inner loop's jump instruction jumps out of its loop scope, which is
    // not expected for us, ie the inner loop trace is not valid for this
    // run
    return NestedLoopJump::kBailout;
  } else {
    return NestedLoopJump::kInnerJump;
  }
}

// Finding out all the loop exit which has recovery bytecode same as current
// one. Since we peel the iteration of loop, at least we have 2 exit jumps to
// the same position and this complicates the whole process of jit nesting. If
// we just stick to the traced loop exit, we will have certain types of code
// cannot be jitted.
std::vector<DeoptId> Builder::FindAllLoopExit(InterpInfo* info,
                                              const Quake& quake, DeoptId did) {
  std::vector<DeoptId> did_set;

  // current bytecode address
  auto recovery_bytecode = info->start_pc();

  // iterate all the deoptimization point and find out deoptimization position
  // which has exactly same exit position
  for (const auto& deopt : quake.deopt_list()) {
    if (deopt.reason() == DeoptReason::kLoopExit) {
      if (deopt.top_frame_recovery_bytecode() == recovery_bytecode) {
        did_set.push_back(deopt.deopt_id());
      }
    }
  }

  // FIXME(dpeng);
  // this is kind of hacky for now, we need to refactory it allow us to learn
  // that which deopt is paired with another deoptimization site due to the
  // code re-emit. For now, we just do following swap. The deoptimization id
  // slot just has 2 and we cannot really specify more than 2 slots for now

  CHECK(did_set.size() >= 2);
  if (did_set.size() > 2) {
    // at least need to contain the last did number
    if (did_set[0] < did) did_set[1] = did;
    if (did_set[0] > did) {
      did_set[1] = did_set[0];
      did_set[0] = did;
    }
    did_set.resize(2);
  }

  return did_set;
}

// Generate instruction to enter the nested loop.
TraceResult Builder::EnterNestedLoop(InterpInfo* info, DeoptId loop_exit_id) {
  struct SnapshotValue {
    ValType type;  // value type
    int index;     // index of the value
  };

  std::vector<SnapshotValue> value_list;

  auto snapshot = [&](SSAVar** start, SSAVar** end, Opcode op,
                      Opcode nullify_op, ValType t) {
    auto idx = 0;
    for (; start != end; start++) {
      if (*start) {
        auto pos = graph_->KI32(info, idx);
        if (IsValMutated(*start, t, idx)) {
          auto ops = BoxTo(info, op, pos, *start, true);
          ops->flag().SetResetStackSlot();
          value_list.push_back(SnapshotValue{t, idx});
        } else {
          RemovePhiLoopCandidate(t, idx);
        }
        *start = nullptr;
      }
      ++idx;
    }
  };

  CHECK(nested_loop_);

  snapshot(param(), param_end(), Opcode::kBxToParam, Opcode::kParamNullify,
           kParam);

  snapshot(local(), local_end(), Opcode::kBxToLocal, Opcode::kLocNullify,
           kLocal);

  // --------------------------------------------------------------------------
  // temporary value is counting in reverse order, ie stack order
  {
    auto idx = 1;
    for (auto start = temp_end() - 1; start >= temp(); start--) {
      if (*start) {
        auto pos = graph_->KI32(info, idx);
        auto ops = BoxToTemp(info, pos, *start, true);
        ops->flag().SetResetStackSlot();
        value_list.push_back(SnapshotValue{kTemp, idx});
        *start = nullptr;
      }
      ++idx;
    }
  }

  auto did_set = FindAllLoopExit(info, *nested_loop_, loop_exit_id);
  CHECK(did_set.size() == 2);

  BASE_DBG({
    CHECK(std::find(did_set.begin(), did_set.end(), loop_exit_id) !=
          did_set.end());
  });

  auto node = graph_->AddOp(info, Opcode::kEnterJitTrace,
                            2 + 2);  // enter into the jit

  node->Set(0, graph_->KI32(info, nested_loop_->trace_id()));
  node->Set(1, graph_->KI32(info, StkOffset(0)));
  node->Set(2, graph_->KI32(info, did_set[0]));
  node->Set(3, graph_->KI32(info, did_set[1]));

  if (info->temp_size() > std::numeric_limits<std::uint16_t>::max()) {
    return TraceResult::NYI("inner loop: too many temporary value");
  }

  node->flag().set_extra(static_cast<std::uint16_t>(info->temp_size()));

  // since the inner loop just finish its jitted code and also we lost trace
  // here, because we cannot trace jit. Therefore, we need to correctly setup
  // the temp value, which is dynamically mutated.
  CHECK(temp_size() <= info->temp_size());
  {
    auto left = info->temp_size() - temp_size();
    for (std::uint32_t i = 0; i < left; i++) {
      // Indicate that the temporary is unknown to us, and we need to load it
      Push(nullptr);
    }
  }

  // Generate a snapshot/checkpoint afterwards
  UpdateCheckpointAfter(info, Checkpoint::kBranch, node);

  SnapshotBefore(info, Checkpoint::kSideEffect);

  // ---------------------------------------------------------------------------
  // Notes(dpeng):
  //
  //   We will *have to* recover every value after the loop done, the reason
  //   is because of the reference counting. The value been recoveried here
  //   may not be used by any statement after the loop, which is considered to
  //   be a dead code, and will be removed by DCE pass
  //
  // ---------------------------------------------------------------------------
  for (auto& entry : value_list) {
    switch (entry.type) {
      case ValType::kParam:
        if (auto [r, ssa] = ParamAt(info, entry.index); !r) {
          return r;
        }
        break;

      case ValType::kLocal:
        if (auto [r, ssa] = LocAt(info, entry.index); !r) {
          return r;
        }
        break;

      case ValType::kTemp:
        if (auto [r, ssa] = StkAt(info, entry.index); !r) {
          return r;
        }
        break;

      default:
        UNREACHABLE();
        break;
    }
  }

  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Notes this function will be executed at the very first bytecode just been
// recovered from the deoptimization point. It may overlap with whatever in
// the inner loop's code since the sparse deoptimization point generated.
// This is fine, we only need to verify that the last deoptimization is at an
// loop exit deoptimization point and then we just generate that range of
// code to be a side trace
bool Builder::CanNestedLoopBeLinked(InterpInfo* info) {
  if (auto last_tid = quake_->world()->last_trace_id();
      last_tid == nested_loop_->trace_id()) {
    auto last_deopt = quake_->world()->last_deoptimization_id();
    if (const auto& deopt = nested_loop_->GetDeopt(last_deopt);
        deopt.reason() == DeoptReason::kLoopExit) {
      CHECK(deopt.top_frame_recovery_bytecode() == info->start_pc());

      // notes, since we will inline the loop exit into the current trace, we
      // need to generate the loop enter instruction for the nested loop.
      // Nothing special just calls into the nested loop
      if (auto x = EnterNestedLoop(info, last_deopt); !x) {
        return false;
      }

      return true;
    }
  }

  return false;
}

void Builder::StopNestedLoopExit() {
  // link the inner trace
  quake_->AddInnerTrace(nested_loop_);

  // remove the inner trace from the trace list, since we will never enter the
  // inner trace again
  //
  // nested_loop_->set_expose(false);

  nested_loop_.reset();
  nested_loop_trace_state_ = NestedLoopTraceState::kNone;
}

TraceResult Builder::LoopExitTrace(InterpInfo* info) {
  CHECK(nested_loop_trace_state_ == NestedLoopTraceState::kLoopExit);

  // Notes, we do not need to have extra loop exit trace, but the loop exit
  // trace will be right inside of the outer loop. We don't need extra linked
  // trace, since if the inner loop is invalid at that loop exit
  // deoptimization point, the outer loop is useless as well. And we can only
  // trigger out loop tracing from inner loop.

  if (auto x = FindNestedLoopJump(info); x) {
    switch (*x) {
      case NestedLoopJump::kLoopback:
        // this means inner loop is unstable, since it jumps back to the start
        // of inner loop.
        return TraceResult::InnerLoop("inner loop deoptimization loopback");

      case NestedLoopJump::kBailout:
        // This means the jump bailout from the inner loop, ie we are ending
        // the side trace for loop exit and we can finish the inner loop trace
        // link stuff.
        StopNestedLoopExit();
        return TraceResult::Next();

      case NestedLoopJump::kInnerJump:
      case NestedLoopJump::kNoJump:
        // Still inside of the loop exit tracing, perform normal trace
        return TraceResult::Next();

      default:
        UNREACHABLE();
        return TraceResult::Next();
    }
  } else {
    return TraceResult::ReferenceError("inner loop: condition is unknown");
  }
}

TraceResult Builder::TryNestedLoop(InterpInfo* info) {
  switch (nested_loop_trace_state_) {
    case NestedLoopTraceState::kNone:
      // try to detect whether we are at the starting of an inner loop or so
      if (NestedLoopStartForward(info)) {
        nested_loop_trace_state_ = NestedLoopTraceState::kSkip;
        goto skip;
      } else if (NestedLoopStartBackward(info)) {
        nested_loop_trace_state_ = NestedLoopTraceState::kRun;
        return TraceResult::Ignore();  // directly trace this jump back
                                       // instruction. This is peeled
                                       // iteration of the jitted loop
                                       // embedded inside
      } else {
        return TraceResult::Ignore();
      }

      // fallthrough
    case NestedLoopTraceState::kSkip:
    skip:
      // the inner loop's first iteration tracing will definitly happen at the
      // VM and will be observed by us, if so we just do a simple skipping
      // here until we hit the loop jump back instruction
      if (auto x = FindNestedLoopJump(info); x) {
        switch (*x) {
          case NestedLoopJump::kNoJump:
          case NestedLoopJump::kInnerJump:
            // Still inside of the inner loop trace, keep skipping inner
            // loop's bytecode instructions
            return TraceResult::Done();

          case NestedLoopJump::kLoopback:
            // loopback jump, the VM will enter into the nested loop via this
            // loopback jump instruction and we should change our state
            // machine
            nested_loop_trace_state_ = NestedLoopTraceState::kRun;
            return TraceResult::Done();

          case NestedLoopJump::kBailout:
            // exception happened, ie the inner loop jumps out of its scope to
            // the outer scope.
            //
            // It means, the inner loop is not stable in terms to our traced
            // one and its jit may not be able to enter and our current trace
            // is invalid assumption.
            //
            // Since currently, we are not able to run VM from *JIT* frame,
            // which we may add it, we should bailout from the outer loop and
            // wait for inner loop to become stable and then trigger outer
            // loop again.
            return TraceResult::InnerLoop("inner loop unstable");

          default:
            UNREACHABLE();
            return TraceResult::Next();
        }
      } else {
        return TraceResult::ReferenceError("inner loop: condition is unknown");
      }
      break;

    case NestedLoopTraceState::kRun:
      // Okay, we recover from the inner loop's jitted frame, and we are sure
      // that the inner loop is deoptimized from jitted code and we should
      // find out the deoptimization point is our expected deoptimization
      // point or not.
      if (CanNestedLoopBeLinked(info)) {
        nested_loop_trace_state_ = NestedLoopTraceState::kLoopExit;
      } else {
        // The inner loop is not stable, and we bailout because of loop exit
        return TraceResult::InnerLoop("inner loop unstable at deoptimization");
      }

      // fallthrough
    case NestedLoopTraceState::kLoopExit:
      return LoopExitTrace(info);

    default:
      UNREACHABLE();
      return TraceResult::Next();
  }
}

// ----------------------------------------------------------------------------
// Other tracing support utilities
TraceResult Builder::BeforeTrace(InterpInfo* info) {
  interp_info_ = info;
  rc_flushed_ = false;
  rc_queue_.clear();
  pending_checkpoint_.reset();
  jump_to_ = nullptr;
  call_depth_ = frame_.size();

  bc_size_ = 0;

  // This part starts to tick the loop trace linking state machine. If nothing
  // happen, ie no inner loop is detected, it will return and continue the
  // loop tracing
  if (auto x = TryNestedLoop(info); x.IsNotInternal()) {
    return x;
  } else {
    // This part detects whether the current instruction forms a *hole* inside
    // of bytecode sequences, which we need to record it for later loop exit
    // detect
    if (x.reason() == BailoutReason::kIgnore && last_pc()) {
      auto expect_next_bytecode = BytecodeNext(last_pc());

      // checking whether the current traced bytecode is a linear fall-through
      // of the previous bytecode
      if (info->start_pc() != expect_next_bytecode) {
        // Correctly handle backwards jump, which is kind of complicated, ie
        // the backward jump can split an already existing hole that has been
        // recoreded

        if (info->start_pc() < expect_next_bytecode) {
          // Notes, here we have many possible situation due to the complexity
          // of bytecode layout been generated by the quickjs. In general we
          // do not support conditional loop during tracing, and we try our
          // best to find it out and exclude it.
          //
          // 1) For for_of/for_in loop, the quickjs will generate first
          // iteration
          //    that unconditionally jump into the middle of the loop body,
          //    which makes the first iteration of these loop different from
          //    the rest of loop iteration. The inner trace is triggered after
          //    the first trace so its trace(Quake) is different from outer
          //    loop's view, since the outer loop will observe the first
          //    iteration of inner loop. We need to detect this situation here
          //    and try nested loop here.
          //
          // 2) For small loop, which requires scalar evolution analysis or
          // trip
          //    count analysis, we can directly unroll the loop here, as long
          //    as the trip count is known to be small enough.
          //
          //
          // 3) Other types of backwards jump will be treated as unexpected
          // code,
          //    JavaScript have goto statement, it is possible for user to
          //    generate backwards jump which we do not expect and do not want
          //    to handle. The reason for this is because, we cannot really
          //    tell the difference between a user written goto and a normal
          //    loop's backwards jump with quickjs's bytecode. If it is really
          //    a loop's backwards jump then we are basically unconditionally
          //    unroll all the loop, which is not something what we want.
          return TraceResult::NYI("backwards jump other than loopback");
        } else {
          // We are sure that there's hole inside of the bytecode range
          hole_list_.emplace_back(expect_next_bytecode, info->start_pc());
        }
      }
    }
  }

  // Continue tracing in current trace object
  return TraceResult::Next();
}

TraceResult Builder::AfterTrace(InterpInfo* info) {
  // Flush the RC out, if we don't have RC, which means no checkpoint has been
  // generated, then check whether we have pending checkpoint
  FlushRC(info);

  // Flush pending exception operation, ie very specificand we need to
  // generate yet another checkpoint right after the pending checkpoint
  FlushPendingGuardNotException(info);

  // Flush pending checkpoint if needed, notes the pending checkpoint has to
  // happen at very last since it is a checkpoint of the current bytecode. The
  // pending not exception will just have another checkpoint
  FlushPendingCheckpoint(info);

  FlushPendingSnapshot(info);

  // Record last_bc
  set_last_bc(info->op());
  set_last_pc(info->start_pc());

  switch (info->op()) {
    case Bytecode::OP_goto:
    case Bytecode::OP_if_true:
    case Bytecode::OP_if_false:
      CHECK(jump_to_);
      break;
    default:
      break;
  }

  // If we just emit an inline call, emit a new trace_info layer
  if (call_depth_ < frame_.size()) {
    CHECK(call_depth_ + 1 == frame_.size());
    trace_info_.emplace_back();
  }

  switch (quake_->trace_type()) {
    case TraceType::kLoop:
      if (jump_to_ == start_bc_) {
        return TraceResult::Stop();
      } else {
        return TraceResult::Done();
      }
    default:
      if (BytecodeIsLast(last_bc())) {
        return TraceResult::Stop();
      } else {
        return TraceResult::Done();
      }
  }
}

TraceResult Builder::Trace(InterpInfo* info) {
  // 1) sanity check, whether all the assumption matches with each other
  CHECK(info->local_size() == local_size());
  CHECK(info->param_size() == param_size());
  CHECK(!info->IsModeStrict());

  ++bc_size_;

  // 2) Start to perform BeforeTrace operation
  if (auto r = BeforeTrace(info); r.reason() != BailoutReason::kNext) {
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

  EnterEntryFrame(info);

  // Setup the very first special checkpoint, used to rewind the whole JIT
  // code if needed. QuickJS's stack is not consistent cross loop iteration,
  // mainly for for_in/for_of range for, the first iteration and rest
  // iteration have different stack size. Currently this will lead to many
  // BUGS for our jitted code since our code relies on forward index instead
  // of backwards index to access temporary. This means the temporary size
  // must be the same as with tracing.
  auto start = graph_->AddControlFlow(info, Opcode::kStart);
  AddCheckpointToGraph(info, Checkpoint::kStart, start, false);

  quake_->set_debug(info->Function().Debug());
  return TraceResult::Done();
}

// Optimize the graph before entering into the Reemit phase
bool Builder::Optimize() {
  std::vector<Pass::Result> output;

  auto n = graph_->Finalize();
  DEFER([&]() { graph_->Resume(n); });

  {
    Fold fold{quake_};
    if (!Pass::Run(graph_, &fold, &output)) return false;
  }

  // fixup all the result in current storage position
  for (const auto& x : output) {
    // folding will never delete any node
    CHECK(x.state == Pass::Result::kReplacement);
    for (auto& value : cur_frame().storage_) {
      if (x.origin->IsValue() && value == x.origin->out()) {
        CHECK(x.node->IsValue());
        value = x.node->out();
      }
    }
  }
  return true;
}

void Builder::LoopExitAnalyse() {
  auto find_region = [&](const void* start) -> const void* {
    for (const auto& e : hole_list_) {
      if (e.start == start) {
        return e.end;
      }
    }

    // natural join of 2 blocks, ie the other untaken branch contains nothing
    return nullptr;
  };

  auto loop = bytecode_range();

  for (auto& trace : trace_snapshot_list_) {
    trace.end_bc = find_region(trace.start_bc);
    if (!trace.end_bc) continue;  // skip natural untaken branch

    if (auto [is_in_loop, target] = BranchRangeHasLoopExit(
            BytecodeRange{trace.start_bc, trace.end_bc}, loop);
        is_in_loop) {
      // this is a loop exit
      trace.checkpoint->set_reason(Checkpoint::kLoopExit);
    }
  }
}

TraceResult Builder::Stop(InterpInfo* info) {
  // LoopExit analysis
  LoopExitAnalyse();

  // Optimization
  if (!Optimize()) {
    return TraceResult::BytecodeError("IR(optimization): bailout");
  }

  // Post trace process the graph
  switch (quake_->trace_type()) {
    case TraceType::kLoop:
      if (!Reemit(info)) {
        return TraceResult::NYI(
            "Reemit failed, type must be alter at loop carry dependency");
      }
      break;

    default:
      if (!FinalizeForNoneLoop(info)) {
        return TraceResult::NYI("FinalizeForNoneLoop failed");
      }
      break;
  }

  LeaveFrame(info);
  graph_->Finalize();
  return TraceResult::Done();
}

// ----------------------------------------------------------------------------
// Finish graph
//
// This function is only used for jitting side-trace or function call, ie not
// a loop. It replaces the Reemit phase basically.
//
// This function does nothing special except clean up those internal
// instruction
bool Builder::FinalizeForNoneLoop(InterpInfo* info) {
  graph_->Foreach([&](auto node) {
    switch (node->opcode()) {
      case Opcode::kLocSet:
      case Opcode::kParamSet:
      case Opcode::kTempNullify:
      case Opcode::kTempSet:
      case Opcode::kTempPop:
      case Opcode::kTempPush:
        node->Remove();
        break;

      default:
        break;
    }
  });

  // Just a fake region for BB0-BB1 region. This is a linear side trace, so
  // no need to emit a loop body

  auto start_of_body = graph_->AddControlFlow(info, Opcode::kBB1);
  end_of_bb0_ = graph_->SetNodeInternalSSA(start_of_body);
  graph_->set_end_of_bb0(start_of_body);
  graph_->set_end_of_bb1(start_of_body);
  graph_->AddControlFlow(info, Opcode::kEnd);

  return true;
}

// ----------------------------------------------------------------------------
// Reemit of graph
//
// The reemit phase is performed AFTER the whole graph is been done and form
// the whole graph into a special graph that contains 2 seconds
//
// 1) one single iteration, ie like loop peeling
// 2) rest of the loop body
//
// The Reemit will be splitted into 2 phases, due to QuickJS uses dual-number
// mode, we will have to decide the type of PHI after the rewriting of the
// loop body. Our IR code for different arithmetic, which requires type
// information, will be lowered into corresponding typping one in later phase
// after the reemit.

bool Builder::Reemit(InterpInfo* info) {
  is_reemit_ = true;

  SSAVarMappings mappings;

  // marking the graph to contain the BB0 marker
  auto start_of_body = graph_->AddControlFlow(info, Opcode::kBB1);
  end_of_bb0_ = graph_->SetNodeInternalSSA(start_of_body);
  graph_->set_end_of_bb0(start_of_body);

  // Now let's revisit each already visited node and forms the loop body
  // iteration

  // 1) Create all the phi candidate in slot that has been stored during the
  //    first peeling of the loop
  Node* last_phi = nullptr;
  if (!InsertPhiCandidate(&last_phi)) {
    return false;
  }

  // After all the loop phi been inserted, we need to insert a checkpoint
  // to guarantee any deoptimization happend inside of the loop will not
  // deoptimize to the point that is outside of the loop
  CHECK(start_bc_);

  // We need to add a checkpoint *BEFORE* the first instruction in the
  // emittion, right after the kBB1 block. This flag is used to ensure we just
  // insert it at the very first instruction
  auto first_checkpoint = false;
  auto add_first_checkpoint = [&](Node* node) {
    if (!first_checkpoint) {
      first_checkpoint = true;
      AddCheckpointToGraph(node, Checkpoint::kBB1,
                           last_phi ? last_phi : start_of_body, true);
    }
  };

  // 2) Now start to visit the graph until we hit the BB0. The algorithm is
  //    as following.
  //
  //    Since our initial graph is built as SSA, each instruction's operand,
  //    will show up in the graph ealier than the instruction itself. It makes
  //    sure that when we visit a certain instruction, and its operand has
  //    been transformed into correct one.
  //
  //    We setup a mappnig table, maps the old graph's operand, ie SSAVar*
  //    into a new SSAVar*. This essentially makes the second interpretation
  //    of IR graph using a SSAVar* as index to its operand(value) instead of
  //    the old stack index. Which still maintains all the needed information.
  //
  //    During the revisiting, we just need to *rewrite* all the kLocGet and
  //    kParamGet and kTempGet to corresponding new value created in previous
  //    peeled loop, ie the value shows up the current storage_ field. For
  //    all the other part we can just deep clone them. Notes, since the
  //    kLocGet/kParamGet/kTempGet shows up in different value, it makes
  //    the unboxing of these value unneeded, which means we should perform a
  //    little bit optimization of Unbox operation. Notes, we can feed the
  //    instruction back to a separate folding pass to do the optimization
  //    either But we are pretty sure what gonna happen here. It just means
  //    the newly created kLocGet/kParamGet/kTempGet will need to back
  //    propagte its type information back to the Unbox/TryUnbox instruction
  //    that guards it See above LocAt/ParamAt/StkAt for detail. Pretty simple
  //    though :) Other instructions are no specail, just do a *deep* clone
  //    and then setup the map entry, that's it. We gonna have a correct
  //    graph. The graph, if the jump back edge from the loop body is from
  //    left side, can be viewed as a number *6* in glyph. We just call it six
  //    graph :)

  if (!graph_->ForeachBB0Stoppable([&](Node* node) -> bool {
        // try to add first checkpoiint during reemittion
        add_first_checkpoint(node);

        auto replacement = RewriteNode::None();

        switch (node->opcode()) {
          case Opcode::kStart:
            return true;

          // Sanity check
          case Opcode::kPhiLoop:
            UNREACHABLE() << "cannot have phi in first iteration";
            break;

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

          // temp internal nodes
          case Opcode::kTempNullify:
            MarkTempNullify(mappings, node);
            break;
          case Opcode::kTempSet:
            MarkTempSet(mappings, node);
            break;
          case Opcode::kTempPop:
            MarkTempPop(mappings, node);
            break;
          case Opcode::kTempPush:
            MarkTempPush(mappings, node);
            break;

          case Opcode::kBxToLocal:
          case Opcode::kBxToParam:
          case Opcode::kBxToTemp:
            if (node->flag().IsResetStackSlot()) {
              auto idx = node->At(0)->node_checked()->Int32();
              switch (node->opcode()) {
                case Opcode::kBxToLocal:
                  local()[idx] = nullptr;
                  break;

                case Opcode::kBxToParam:
                  param()[idx] = nullptr;
                  break;

                default:
                  temp()[idx] = nullptr;
                  break;
              }
            }
            goto clone_node;

          case Opcode::kLocNullify:
            local()[node->At(0)->node_checked()->Int32()] = nullptr;
            // node->Remove();
            break;

          case Opcode::kParamNullify:
            param()[node->At(0)->node_checked()->Int32()] = nullptr;
            // node->Remove();
            break;

          // Special IR code, which generate side effect
          case Opcode::kEnterJitTrace:
            // Need to specialized handling, ie nullify all the environment
            // after this instruction and we also need to generate snapshot
            // after the jit trace
            if (!ReemitEnterJitTrace(node, &mappings)) return false;
            break;

          default: {
          clone_node:
            if (auto output = ReemitNode(node, &mappings); output.IsBailout()) {
              return false;
            } else if (output.Has()) {
              replacement = output;
              if (auto out = node->out(); output.node->HasOut() && out) {
                SetMappings(&mappings, out, output.node->out());
              }
            }
          } break;
        }

        // snapshot generation. If node has snapshot requirement, just
        // generate it
        if (node->HasSnapshot() && replacement.IsNew()) {
          RewriteCheckpoint(node, replacement.node, mappings);
        }

        // if the node jumps to somewhere, it needs to recover from it as well
        if (replacement.IsNew() &&
            (node->HasExit() && replacement.node->IsGuard())) {
          RewriteExit(node, replacement.node, mappings);
        }

        return true;
      })) {
    return false;
  }

  // After the above operation, we should have already setup the stuff we need
  // for the loop body, now we need to insert an unconditional jump points
  // back to the BB0 position.
  auto back_jump = graph_->AddControlFlow(info, Opcode::kJump, end_of_bb0_);
  end_of_bb1_ = graph_->SetNodeInternalSSA(back_jump);
  graph_->set_end_of_bb1(back_jump);

  // --------------------------------------------------------------------------
  // 3) Fixup the phi generation
  if (!FixupPhi()) {
    return false;
  }
  if (!FixupDup()) {
    return false;
  }

  // --------------------------------------------------------------------------
  // 4) Add kEnd
  graph_->AddControlFlow(info, Opcode::kEnd);

  return true;
}

bool Builder::ReemitEnterJitTrace(Node* node, SSAVarMappings* mappings) {
  const auto info = node;  // alias for BytecodeInfoProvider interface

  // 1) Try to box any value that is not been boxed by the peeled iteration's
  //    jit trace link code. This is possible since the side trace may yield
  //    more value on the stack, mostly temporary value
  //
  // Notes, in certain cases that we need to box new value before enter jit
  // during reemition, the reason is because the trailing code part after the
  // nested loop may yield mutation of existed states, which is not captured
  // during the first trace

  auto snapshot = [&](SSAVar** start, SSAVar** end, Opcode op, ValType vtype) {
    auto idx = 0;
    for (; start != end; start++) {
      if (IsValMutated(*start, vtype, idx)) {
        auto pos = graph_->KI32(info, idx);
        graph_->AddOp(info, op, pos, *start);
        *start = nullptr;
      }
      ++idx;
    }
  };

  snapshot(local(), local_end(), Opcode::kBxToLocal, kLocal);
  snapshot(param(), param_end(), Opcode::kBxToParam, kParam);
  snapshot(temp(), temp_end(), Opcode::kBxToTemp, kTemp);

  // 2) Clone the jit trace enter node
  if (auto output = ReemitNode(node, mappings); output.IsBailout()) {
    return false;
  } else {
    RewriteCheckpoint(node, output.node, *mappings);
  }

  // 3) Realign the temporary stack based on the traced result
  {
    auto need_to_align = node->flag().extra() - temp_size();
    for (std::size_t i = 0; i < need_to_align; i++) {
      Push(nullptr);
    }
  }

  return true;
}

void Builder::MarkLocSet(const SSAVarMappings& mappings, Node* loc_set) {
  CHECK(loc_set->size() == 2);
  auto idx = loc_set->AtI32(0);
  auto value = loc_set->At(1);
  auto new_value = GetMappings(mappings, value);

  // Notes(dpeng): Since phi elimination will happen during RA phase, this
  // will make the phi node change its storage between loop iteration.
  // Therefore, we cannot simply let local value points to the phi node itself
  // but have to create an alias(dup) to address this issue. This issue
  // happens when the deoptimization happened cross the loop iteration
  // boundary.
  if (new_value->node_checked()->IsNoneTrivialPhi()) {
    auto ty = new_value->type();
    new_value = graph_->AddValue(loc_set, Opcode::kDup, ty, new_value);
    dup_list_.push_back(new_value);
  }

  local()[idx] = new_value;
  loc_set->Remove();
}

void Builder::MarkParamSet(const SSAVarMappings& mappings, Node* param_set) {
  CHECK(param_set->size() == 2);
  auto idx = param_set->AtI32(0);
  auto value = param_set->At(1);
  auto new_value = GetMappings(mappings, value);

  // same as MarkLocSet
  if (new_value->node_checked()->IsNoneTrivialPhi()) {
    auto ty = new_value->type();
    new_value = graph_->AddValue(param_set, Opcode::kDup, ty, new_value);
    dup_list_.push_back(new_value);
  }

  param()[idx] = new_value;
  param_set->Remove();
}

void Builder::MarkTempNullify(const SSAVarMappings& mappings,
                              Node* temp_nullify) {
  auto idx = temp_nullify->AtI32(0);
  TempFill(idx, nullptr);
  temp_nullify->Remove();
}

void Builder::MarkTempPop(const SSAVarMappings& mappings, Node* temp_pop) {
  UNUSE(mappings);
  auto idx = temp_pop->AtI32(0);
  Pop(idx);
  temp_pop->Remove();
}

void Builder::MarkTempPush(const SSAVarMappings& mappings, Node* temp_push) {
  UNUSE(mappings);
  auto value = temp_push->At(0);
  auto new_value = GetMappingsChecked(mappings, value);
  Push(new_value);
  temp_push->Remove();
}

void Builder::MarkTempSet(const SSAVarMappings& mappings, Node* temp_set) {
  UNUSE(mappings);
  auto idx = temp_set->AtI32(0);
  auto value = temp_set->At(1);
  auto new_value = GetMappingsChecked(mappings, value);
  temp()[idx] = new_value;
  temp_set->Remove();
}

bool Builder::InsertPhiCandidate(Node** end) {
  Node* last = nullptr;
  for (auto& entry : phi_loop_candidate_) {
    switch (entry.vtype) {
      case kParam: {
        auto old = param()[entry.index];
        auto ty = old->type();
        auto phi = graph_->AddValue(old->node_checked(), Opcode::kPhiLoop, ty,
                                    old, nullptr);
        entry.phi = phi;
        param()[entry.index] = phi;
        last = phi->node_checked();
      } break;

      case kLocal: {
        auto old = local()[entry.index];
        auto ty = old->type();
        auto phi = graph_->AddValue(old->node_checked(), Opcode::kPhiLoop, ty,
                                    old, nullptr);
        entry.phi = phi;
        local()[entry.index] = phi;
        last = phi->node_checked();
      } break;
      default:
        UNREACHABLE();
        break;
    }
  }

  *end = last;
  return true;
}

bool Builder::IsAllowedPhiType(const Type& t) const {
  switch (t.tag()) {
    case Type::kInt:
    case Type::kFloat64:
    case Type::kBool:
    case Type::kObject:
    case Type::kString:
      return true;

    default:
      return false;
  }
}

// ----------------------------------------------------------------------------
// FIXME(dpeng): Move the following logic into optimization pass
//
// Simplify PHI early on to ease our unittest. QuickJS will generate wired
// undefined assignment in the middle of the loop for no obvious usage. Other
// types of PHI simplficiation will be added in the future with fold engine,
// ie specific pass to handle such cases
// ----------------------------------------------------------------------------
SSAVar* Builder::TryFoldPhi(Node* node, SSAVar* lhs, SSAVar* rhs) {
  if (lhs->type().IsUndefined() && rhs->type().IsUndefined()) {
    return graph_->KUndefined(node);
  } else if (lhs->type().IsNull() && rhs->type().IsNull()) {
    return graph_->KNull(node);
  } else if (lhs->type().IsUninitialized() && rhs->type().IsUninitialized()) {
    return graph_->KUninit(node);
  } else {
    // check lhs rhs are both constant, if they have same constant value, then
    // just eliminate the phi
    auto lnode = lhs->node_checked();
    auto rnode = rhs->node_checked();
    if (lnode->IsConst() && rnode->IsConst()) {
      if (lnode->opcode() == rnode->opcode()) {
        switch (lnode->opcode()) {
          case Opcode::kKI32:
            if (lnode->Int32() == rnode->Int32()) return lhs;
            break;

          case Opcode::kKI64:
            if (lnode->Int64() == rnode->Int64()) return lhs;
            break;

          case Opcode::kKFloat64:
            if (lnode->Float64() == rnode->Float64()) return lhs;
            break;

          case Opcode::kLoadStr:
            if (lnode->Str() == rnode->Str()) return lhs;
            break;

          default:
            break;
        }
      }
    }
  }

  return nullptr;
}

bool Builder::FixupPhi() {
  SSAVar* replacement = nullptr;

  auto eliminate_or_fixup_phi = [&](SSAVar* phi, SSAVar* current,
                                    SSAVar** repl) -> bool {
    CHECK(!current->IsDead());
    auto node = phi->node_checked();
    CHECK(node->opcode() == Opcode::kPhiLoop);
    CHECK(node->Rhs() == nullptr);
    *repl = nullptr;

    /**
     * Notes, we used to introduce a RA bug since we remove trivial phi here.
     * For case that a loop invariant variable is accessed inside of the loop,
     * we just do not place phi for them. However this fails the RA part,
     * since this variable, though is invariant at IR, but is actually variant
     * for LIR. Becauase after the access of this variable inside of the loop,
     * its initial register can be spilled, however without PHI decoration the
     * RA will not reserve its consistency inside of certain slot cross loop
     * carry dependency which cause the 3rd JIT iteration to fail. Because
     * after the 2nd the loop iteration, the value can be spilled to other
     * position.
     */
    if (phi == current) {
      auto lhs = node->At(0);
      if (lhs->node_checked()->IsConst()) {
        // just do specialize when the invariant is a constant value, which
        // does not need to take care of since it can be re-created aways
        auto value = node->Remove(0);
        CHECK(value);
        *repl = value;
        node->ReplaceAndSetDead(value);
        return true;
      }
    }

    auto lhs = node->At(0);
    auto rhs = current;
    CHECK(lhs && rhs);

    // FIXME(dpeng): Move this out of the IR construction.
    if (auto fold = TryFoldPhi(node, lhs, rhs); fold) {
      *repl = fold;
      node->ReplaceAndSetDead(fold);
      return true;
    }

    // Check type of phi operand. Currently we don't have boxed phi, so we
    // cannot upgrade type evolution value into boxed version for now. And
    // we may want to add it, but if so, then the jit will generate sucky
    // code instead of performant code :(
    if (!IsAllowedPhiType(lhs->type()) || !IsAllowedPhiType(rhs->type())) {
      return false;
    }

    // Okay, finish the phi construction
    node->Set(1, rhs);

    // Infer the type, we still can fail here, ie due to the type evolution
    return node->InferPhiType();
  };

  auto replace_ref = [&](SSAVar** start, SSAVar** end, SSAVar* target,
                         SSAVar* replacement) {
    if (!replacement) return;

    for (; start != end; start++) {
      if (*start == target) {
        *start = replacement;
      }
    }
  };

  for (auto& entry : phi_loop_candidate_) {
    switch (entry.vtype) {
      case kParam: {
        auto current = param()[entry.index];
        if (!eliminate_or_fixup_phi(entry.phi, current, &replacement))
          return false;
        replace_ref(param(), param_end(), entry.phi, replacement);
      } break;

      case kLocal: {
        auto current = local()[entry.index];
        if (!eliminate_or_fixup_phi(entry.phi, current, &replacement))
          return false;
        replace_ref(local(), local_end(), entry.phi, replacement);
      } break;
      default:
        UNREACHABLE();
        break;
    }
  }

  return true;
}

bool Builder::FixupDup() {
  // FIXME(dpeng): Move this into optimization pass of IR
  // This fix the cascading effects because we perform the phi folding.

  for (auto dup : dup_list_) {
    auto dup_node = dup->node_checked();
    auto opr = dup_node->Opr();
    if (opr->node_checked()->IsPhi()) continue;  // valid phi

    dup_node->ReplaceAndSetDead(opr);
  }

  return true;
}

void Builder::RewriteCheckpoint(Node* node, Node* replacement,
                                const SSAVarMappings& mappings) {
  CHECK(node->HasSnapshot());

  auto add_checkpoint = [&](Checkpoint* snap, bool is_after) {
    ReemitCheckpointToGraph(node, snap, replacement, is_after);

    // Replace all the checkpoint's entry with new value
    checkpoint_->ForeachEntry([&](Checkpoint::Entry& e) {
      if (auto rvalue = GetMappings(mappings, e.value());
          rvalue && rvalue != e.value()) {
        e.SetValue(rvalue);
      }
    });
  };

  if (auto snap = node->snapshot_before(); snap) add_checkpoint(snap, false);
  if (auto snap = node->snapshot_after(); snap) add_checkpoint(snap, true);
}

void Builder::RewriteExit(Node* node, Node* replacement,
                          const SSAVarMappings&) {
  CHECK(node->HasExit());
  replacement->BailoutTo(checkpoint_);
}

Builder::RewriteNode Builder::RewriteValGet(Node* node,
                                            SSAVarMappings* mappings,
                                            SSAVar** table) {
  CHECK(node->size() == 1);
  CHECK(node->IsValue());
  CHECK(node->out());

  auto imm = node->Opr()->node_checked();
  CHECK(imm->opcode() == Opcode::kKI32);
  auto val_index = imm->Int32();

  // NOTES(dpeng): The temporary index is not valid, needs to convert to
  if (node->opcode() == Opcode::kTempGet) {
    val_index = StkOffsetToRIndex(val_index);
  }

  auto val = table[val_index];

  if (val) {
    SetMappings(mappings, node->out(), val);
    return RewriteNode::Old(val->node_checked());
  } else {
    // When the corresponding slot is null, it means certain instruction has
    // nullify the status, ie we do not know the actual value stored there
    // instead, we need to reemit load instruction and unbox to exactly same
    // type as old one. This typically result from a jit trace link, which
    // will always nullify the local status
    auto new_node = graph_->AddValue(node, node->opcode(), node->At(0));
    auto result = new_node;

    // Check whether the old node has any kBxGuard reference, if so generate a
    // placeholder, otherwise do nothing
    if (node->UsedByBxGuard()) {
      // A special type tag here, that allows the BxGuard after this load
      // instruction to fill in the correct type information. Notes the
      // previous load instruction will always result in boxed type
      result =
          AddGuardValue(node, Opcode::kBxGuard, Type::kInferType, new_node);
    }

    table[val_index] = result;
    SetMappings(mappings, node->out(), result);
    return RewriteNode::New(result->node_checked());
  }
}

Builder::RewriteNode Builder::RewriteLocGet(Node* node,
                                            SSAVarMappings* mappings) {
  return RewriteValGet(node, mappings, local());
}

Builder::RewriteNode Builder::RewriteParamGet(Node* node,
                                              SSAVarMappings* mappings) {
  return RewriteValGet(node, mappings, param());
}

Builder::RewriteNode Builder::RewriteTempGet(Node* node,
                                             SSAVarMappings* mappings) {
  return RewriteValGet(node, mappings, temp());
}

Builder::RewriteNode Builder::RewriteBxGuard(Node* node,
                                             SSAVarMappings* mappings) {
  CHECK(node->opcode() == Opcode::kBxGuard);

  auto operand = node->Opr();

  // Try to get its operand from mappings and check wether its type is already
  // static or not. If so, just do nothing, otherwise put a BxGuard
  auto mapped_operand = GetMappingsChecked(*mappings, operand);

  // Handle special cases that is caused by the jit trace rewriting
  if (mapped_operand->node_checked()->opcode() == Opcode::kBxGuard &&
      mapped_operand->type().IsInfer()) {
    auto old_type = node->type();
    CHECK(old_type.IsUnbox());

    // This can be caused by jit tracing link, the RewriteValGet will add
    // guard internally, if so, we just do nothing but just return the operand
    // which is already unboxed
    mapped_operand->set_type(old_type);
  }

  if (!mapped_operand->type().IsBox()) {
    SetMappings(mappings, node->out(), mapped_operand);
    return RewriteNode::Old(mapped_operand->node_checked());
  } else {
    auto old_type = node->type();
    auto guard_node =
        AddGuardValue(node, Opcode::kBxGuard, old_type, mapped_operand);
    SetMappings(mappings, node->out(), guard_node);
    return RewriteNode::New(guard_node->node_checked());
  }
}

Builder::RewriteNode Builder::RewriteCloneRc(Node* node,
                                             SSAVarMappings* mappings) {
  // Clone contains internal node, which is not tracked so there're no
  // mappings from mappings
  CHECK(node->opcode() == Opcode::kRcOp);

  std::vector<Node*> ops;
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
        auto is_dec = node->opcode() == Opcode::kRcDec ||
                      node->opcode() == Opcode::kRcTryDec;

        if (new_opr->type().IsHeap()) {
          auto opcode = is_dec ? Opcode::kRcDec : Opcode::kRcInc;
          r = graph_->AddNodeInternal(node, opcode, new_opr);
        } else if (new_opr->type().IsBox()) {
          auto opcode = is_dec ? Opcode::kRcTryDec : Opcode::kRcTryInc;
          r = graph_->AddNodeInternal(node, opcode, new_opr);
        }
      } break;
      default:
        UNREACHABLE();
        break;
    }

    if (r) ops.push_back(r);
  }

  if (ops.empty()) {
    return RewriteNode::None();
  } else {
    auto rnode = graph_->AddNode(node, Opcode::kRcOp, ops.size());
    int i = 0;
    for (auto& op : ops) {
      rnode->Set(i, graph_->SetNodeInternalSSA(op));
      i++;
    }
    return RewriteNode::New(rnode);
  }
}

// ----------------------------------------------------------------------------
//
// Cloning of old node to new node. Notes during the loop carry dependency,
// the operand of the old node may need to be fixed with new typped. Ie,
// certain operation is a integer, but then CAST to a float, afterwards, all
// the ops happened afterwards must be in FLOAT operation. This only happens
// at the artihmetic(binary/unary) operations
Opcode Builder::MaybeUpgradeOpcode(Node* old_node, SSAVar* lhs, SSAVar* rhs) {
  auto old_op = old_node->opcode();

  // The type mapping only happens for operator that has multiple types, for
  // operator that never alter types, even they are arithmetic operation,
  // for example like bit operation, we will not need to upgrade them at all.
  if (lhs->type().IsReal() || rhs->type().IsReal()) {
    switch (old_op) {
      case Opcode::kGAddInt:
      case Opcode::kAddInt:
        return Opcode::kAddReal;

      case Opcode::kGSubInt:
      case Opcode::kSubInt:
        return Opcode::kSubReal;

      case Opcode::kGMulInt:
      case Opcode::kMulInt:
        return Opcode::kSubReal;

      case Opcode::kModInt:
        return Opcode::kModReal;

      // Comparison
      case Opcode::kLtInt:
        return Opcode::kLtReal;
      case Opcode::kLeInt:
        return Opcode::kLeReal;
      case Opcode::kGtInt:
        return Opcode::kGtReal;
      case Opcode::kGeInt:
        return Opcode::kGeReal;
      case Opcode::kEqInt:
        return Opcode::kEqReal;
      case Opcode::kNeInt:
        return Opcode::kNeReal;

      default:
        break;
    }
  }

  return old_op;
}

Opcode Builder::MaybeUpgradeOpcode(Node* old_node, SSAVar* opr) {
  auto old_op = old_node->opcode();

  if (opr->type().IsReal()) {
    switch (old_op) {
      // Unary operations
      case Opcode::kGIncInt:
      case Opcode::kIncInt:
        return Opcode::kIncReal;

      case Opcode::kGDecInt:
      case Opcode::kDecInt:
        return Opcode::kDecReal;

      case Opcode::kGNegInt:
      case Opcode::kNegInt:
        return Opcode::kNegReal;

      case Opcode::kI2B:
        return Opcode::kF2B;

      default:
        break;
    }
  }

  return old_op;
}

Builder::RewriteNode Builder::ReemitNode(Node* node, SSAVarMappings* mappings) {
  // This function cannot handle infer type, since it does not perform
  // inferrence
  CHECK(!node->opcode_info().type.IsInfer());
  SSAVar *op1, *op2;

  // 1)
  // -----------------------------------------------------------------------
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

  // 2)
  // -----------------------------------------------------------------------
  // Ignore certain node, ie which requires a integer to be passed in however
  // the type has already evolved, ie g_int_pos, g_int_neg, etc ..
  switch (node->opcode()) {
    case Opcode::kGIsNotZero:
    case Opcode::kGIsIntPos:
    case Opcode::kGIsIntPosOrZero:
    case Opcode::kGIsIntNeg:
    case Opcode::kGIsNotIntMax:
    case Opcode::kGIsNotIntMin: {
      auto opr = GetMappingsChecked(*mappings, node->Opr());
      if (opr->type().IsReal()) {
        return RewriteNode::Old(opr->node_checked());
      }

      // eliminiate against constant type, ie no need to perform check, this
      // will be moved to fold engine in IR optimization pass
      switch (opr->node_checked()->opcode()) {
        case Opcode::kKI32: {
          auto i32 = opr->node_checked()->Int32();
          if (!FoldIntComparison(i32, node->opcode())) {
            return RewriteNode::Bailout();
          }
          return RewriteNode::Old(opr->node_checked());
        }

        case Opcode::kKI64: {
          auto i64 = opr->node_checked()->Int64();
          if (!FoldIntComparison(i64, node->opcode())) {
            return RewriteNode::Bailout();
          }
          return RewriteNode::Old(opr->node_checked());
        }

        default:
          break;
      }
    } break;

    case Opcode::kGIsNotException: {
      auto opr = GetMappingsChecked(*mappings, node->Opr());
      if (opr->type().IsUnbox()) {
        return RewriteNode::None();
      }
    } break;

    // cast, notes, since type may change which means the cast may be
    // redundant notes, our optimization pass will pick up the redundancy as
    // well
    case Opcode::kI2B:
    case Opcode::kF2B:
      if (auto opr = GetMappingsChecked(*mappings, node->Opr());
          opr->type().IsBool()) {
        return RewriteNode::Old(opr->node_checked());
      }
      break;

    case Opcode::kI2F:
      // if the operand is already a float64, then do nothing but just
      // get rid of it.
      if (auto opr = GetMappingsChecked(*mappings, node->Opr());
          opr->type().IsReal()) {
        return RewriteNode::Old(opr->node_checked());
      }
      break;

    case Opcode::kF2Bit:
      if (auto opr = GetMappingsChecked(*mappings, node->Opr());
          opr->type().IsInt()) {
        return RewriteNode::Old(opr->node_checked());
      }
      break;

    default:
      break;
  }

  // 3)
  // -----------------------------------------------------------------------
  // special node handling

  switch (node->opcode()) {
    case Opcode::kLoadStr:
      return RewriteNode::New(
          graph_->LoadStr(node, node->Str())->node_checked());

    default:
      break;
  }

  // 4)
  // -----------------------------------------------------------------------
  // Perform the rewriting, the new node will not be exactly the same as
  // the old node since each node takes type, we may shift the type
  // during the reemition due to the loop carry dependency may alter the
  // type internally
  Opcode op;
  if (node->IsValue()) {
    const auto arity = node->size();

    switch (arity) {
      case 0:
        return RewriteNode::New(
            graph_->AddValue(node, node->opcode())->node_checked());

      case 1:
        op1 = GetMappingsChecked(*mappings, node->Opr());
        op = MaybeUpgradeOpcode(node, op1);
        op1 = UpgradeToFloat64IfNeeded(op, node, op1);

        return RewriteNode::New(
            graph_->AddValue(node, op, op1)->node_checked());

      case 2:
        op1 = GetMappingsChecked(*mappings, node->Lhs());
        op2 = GetMappingsChecked(*mappings, node->Rhs());
        op = MaybeUpgradeOpcode(node, op1, op2);
        op1 = UpgradeToFloat64IfNeeded(op, node, op1);
        op2 = UpgradeToFloat64IfNeeded(op, node, op2);

        return RewriteNode::New(
            graph_->AddValue(node, op, op1, op2)->node_checked());

      default: {
        CHECK(GetOpcodeInfo(node->opcode()).type.IsValid());

        auto replacement = graph_->AddValue(node, node->opcode(), arity);
        for (int i = 0; i < arity; i++) {
          auto v = GetMappingsChecked(*mappings, node->At(i));
          replacement->node_checked()->Set(i, v);
        }
        return RewriteNode::New(replacement->node_checked());
      }
    }
  } else if (node->IsOp()) {
    const auto arity = node->size();

    switch (arity) {
      case 0:
        return RewriteNode::New(graph_->AddOp(node, node->opcode()));

      case 1:
        op1 = GetMappingsChecked(*mappings, node->Opr());
        op = MaybeUpgradeOpcode(node, op1);
        op1 = UpgradeToFloat64IfNeeded(op, node, op1);

        return RewriteNode::New(graph_->AddOp(node, node->opcode(), op1));

      case 2:
        op1 = GetMappingsChecked(*mappings, node->Lhs());
        op2 = GetMappingsChecked(*mappings, node->Rhs());
        op = MaybeUpgradeOpcode(node, op1, op2);
        op1 = UpgradeToFloat64IfNeeded(op, node, op1);
        op2 = UpgradeToFloat64IfNeeded(op, node, op2);

        return RewriteNode::New(graph_->AddOp(node, node->opcode(), op1, op2));

      default: {
        CHECK(GetOpcodeInfo(node->opcode()).type.IsValid());

        auto rnode = graph_->AddOp(node, node->opcode(), arity);
        for (int i = 0; i < arity; i++) {
          auto v = GetMappingsChecked(*mappings, node->At(i));
          rnode->Set(i, v);
        }
        return RewriteNode::New(rnode);
      }
    }
  } else {
    CHECK(node->IsControlFlow());
    const auto arity = node->size();

    switch (arity) {
      case 0:
        return RewriteNode::New(graph_->AddControlFlow(node, node->opcode()));
      case 1:
        op1 = GetMappingsChecked(*mappings, node->Opr());
        return RewriteNode::New(graph_->AddControlFlow(node, node->opcode()));

      default:
        UNREACHABLE();
        return RewriteNode::None();
    }
  }
}

SSAVar* Builder::GetMappings(const SSAVarMappings& mappings, SSAVar* var) {
  // notes constant should not be rewritting
  if (var->node_checked()->IsConst()) return var;

  // the only internal node can show up here is the constant node
  CHECK(!var->node_checked()->IsInternal());

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
