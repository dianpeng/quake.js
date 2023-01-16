#ifndef _QUAKE_IR_BUILDER_H_
#define _QUAKE_IR_BUILDER_H_
#include "base.h"
#include "zone.h"
#include "frame.h"

#include "ir/ssa.h"
#include "ir/bytecode.h"
#include "ir/opcode.h"
#include "ir/node.h"
#include "ir/graph.h"
#include "ir/ssa-var.h"
#include "ir/trace-result.h"
#include "ir/allowed-bytecode-list.h"

namespace quake {
class Quake;

namespace ir {

class InterpInfo;

// Builder of Graph, ie invoked when trace is on for each bytecode seen. The
// builder finish its job when it sees a loopback instruction. Notes, builder
// is not aware of it and the caller should finish the tracing when all are
// done
class Builder {
 private:
  struct PendingCheckpoint {
    Checkpoint::Reason reason;
    Node* trigger_node;
    PendingCheckpoint(Checkpoint::Reason r, Node* tnode)
        : reason(r), trigger_node(tnode) {}
  };

  struct PendingSnapshot {
    Checkpoint::Reason reason;
    bool is_after;
    PendingSnapshot(Checkpoint::Reason r, bool is) : reason(r), is_after(is) {}
  };

  enum ValType { kParam, kLocal };

  struct PhiVal {
    ValType vtype;
    int index;
    SSAVar* phi;
    PhiVal(ValType vt, int i) : vtype(vt), index(i), phi(nullptr) {}
  };

  using SSAVarMappings = std::unordered_map<SSAVar*, SSAVar*>;

  // Used for nested loop trace
  enum class NestedLoopTraceState {
    kNone,  // no nested loop
    kSkip,  // skipping the nested loop bytecode until we hit jit enter
    kExit   // wait for nested loop exit
  };

 public:
  Builder(Quake*, Graph*);

  // Setup the trace, ie called it at very first
  TraceResult Setup(InterpInfo*);

  // Trace a single instructions
  TraceResult Trace(InterpInfo*);

  // A trace is done, we should just stop it
  TraceResult Stop(InterpInfo*);

 public:
  bool IsStopped() const { return start_bc_ == jump_to_; }

  const void* start_bc() const {
    CHECK(start_bc_);
    return start_bc_;
  }

 public:
  class IRFrame {
   public:
    SSAVar** param() { return storage_.data(); }
    SSAVar** param_end() { return param() + param_size_; }

    SSAVar** local() { return param_end(); }
    SSAVar** local_end() { return local() + local_size_; }

    SSAVar** temp() { return local_end(); }
    SSAVar** temp_end() { return storage_.data() + storage_.size(); }

    SSAVar** slot() { return storage_.data(); }
    SSAVar** slot_end() { return storage_.data() + storage_.size(); }

    JitFrameType type() const { return type_; }

    std::uint32_t param_size() const { return param_size_; }
    std::uint32_t local_size() const { return local_size_; }
    std::uint32_t temp_size() const {
      return static_cast<std::uint32_t>(storage_.size()) - param_size() -
             local_size();
    }
    std::uint32_t param_base() const { return 0; }
    std::uint32_t local_base() const { return param_size(); }
    std::uint32_t temp_base() const { return param_size() + local_size(); }
    std::uint32_t slot_size() const {
      return static_cast<std::uint32_t>(storage_.size());
    }

    const void* cur_bc() const { return cur_bc_; }
    const void* next_bc() const { return next_bc_; }

    SSAVar* frame_instruction() const { return frame_instruction_; }

    IRFrame(JitFrameType t, std::size_t ssize, std::size_t psize,
            std::size_t lsize, const void* cbc, const void* nbc)
        : type_(t),
          param_size_(psize),
          local_size_(lsize),
          storage_(),
          cur_bc_(cbc),
          next_bc_(nbc),
          frame_instruction_(nullptr) {
      storage_.resize(ssize);
    }

   private:
    std::size_t StkIdx(int) const;
    void Push(SSAVar*);
    void Pop();
    void Pop(int);

   private:
    JitFrameType type_;
    std::size_t param_size_;
    std::size_t local_size_;
    std::vector<SSAVar*> storage_;
    const void* cur_bc_;
    const void* next_bc_;

    SSAVar*
        frame_instruction_;  // if this frame is not entry frame, then it has
                             // a frame node which can be used for later usage

    friend class Builder;
  };

  struct RewriteNode {
    enum { kNew, kOld, kNone, kBailout };

    Node* node;
    int type;

    RewriteNode(Node* n, int t) : node(n), type(t) {}

    bool IsNew() const { return type == kNew; }
    bool IsOld() const { return type == kOld; }
    bool IsNone() const { return type == kNone; }
    bool IsBailout() const { return type == kBailout; }

    bool Has() const { return node != nullptr; }

    static RewriteNode New(Node* n) { return RewriteNode{n, kNew}; }
    static RewriteNode Old(Node* n) { return RewriteNode{n, kOld}; }
    static RewriteNode None() { return RewriteNode{nullptr, kNone}; }
    static RewriteNode Bailout() { return RewriteNode(nullptr, kBailout); }
  };

  IRFrame& cur_frame() { return frame_.back(); }
  const IRFrame& cur_frame() const { return frame_.back(); }
  const std::vector<IRFrame>& frame() const { return frame_; }
  std::vector<IRFrame>& frame() { return frame_; }

 private:
  SSAVar** param() { return cur_frame().param(); }
  SSAVar** param_end() { return cur_frame().param_end(); }

  SSAVar** local() { return cur_frame().local(); }
  SSAVar** local_end() { return cur_frame().local_end(); }

  SSAVar** temp() { return cur_frame().temp(); }
  SSAVar** temp_end() { return cur_frame().temp_end(); }

  SSAVar** slot() { return cur_frame().slot(); }
  SSAVar** slot_end() { return cur_frame().slot_end(); }

  std::uint32_t param_size() const { return cur_frame().param_size(); }
  std::uint32_t local_size() const { return cur_frame().local_size(); }
  std::uint32_t temp_size() const { return cur_frame().temp_size(); }
  std::uint32_t slot_size() const { return cur_frame().slot_size(); }

  std::uint32_t param_base() const { return cur_frame().param_base(); }
  std::uint32_t local_base() const { return cur_frame().local_base(); }
  std::uint32_t temp_base() const { return cur_frame().temp_base(); }

  void EnterFrame(InterpInfo*);
  void LeaveFrame(InterpInfo*);

 private:
  bool HasRCQueued() const { return !rc_queue_.empty(); }

  TraceResult AfterTrace(InterpInfo*);
  TraceResult BeforeTrace(InterpInfo*);

  std::pair<TraceResult, SSAVar*> StkAt(InterpInfo*, int);
  void StkSet(int, SSAVar*);

  std::size_t StkIdx(int idx) const { return cur_frame().StkIdx(idx); }
  void Push(SSAVar* v) { return cur_frame().Push(v); }
  void Pop() { cur_frame().Pop(); }
  void Pop(int x) { cur_frame().Pop(x); }

  void PopAndRcDec(InterpInfo*);

  std::pair<TraceResult, SSAVar*> LocAt(InterpInfo*, int);
  TraceResult LocSet(InterpInfo*, int, SSAVar*);
  void RcTryDecLoc(InterpInfo*, int);
  bool CheckLocSlotUninitialized(InterpInfo*, int);

  std::pair<TraceResult, SSAVar*> ParamAt(InterpInfo*, int);
  TraceResult ParamSet(InterpInfo*, int, SSAVar*);
  void RcTryDecParam(InterpInfo*, int);

 private:
  SSAVar* DoTraceCall(InterpInfo*, int, SSAVar*, SSAVar*, SSAVar*);
  SSAVar* DoTraceCallConstructor(InterpInfo*, int, SSAVar*, SSAVar*);
  SSAVar* DoTraceArrayFrom(InterpInfo*, int);
  SSAVar* DoTraceApply(InterpInfo*, SSAVar*, int);
  SSAVar* DoTraceLoadArray(InterpInfo*, SSAVar*, SSAVar*);

  TraceResult DoFreeCall(InterpInfo*);
  TraceResult DoMethodCall(InterpInfo*);

 private:
  TraceResult DoGetGlobal(InterpInfo*, bool);
  TraceResult DoSetGlobal(InterpInfo*, int);

 private:
  std::pair<TraceResult, SSAVar*> DoFieldGet(InterpInfo*, SSAVar*,
                                             std::uint32_t);

 private:
  void UpdateCheckpointAfter(InterpInfo*, Checkpoint::Reason, Node*);
  void UpdateCheckpointBefore(InterpInfo*, Checkpoint::Reason, Node*);

  void FlushPendingCheckpoint(InterpInfo*);
  void AddPendingCheckpointAfter(Checkpoint::Reason, Node*);

  void FlushPendingSnapshot(InterpInfo*);
  void AddPendingSnapshot(Checkpoint::Reason, bool);

  bool TryAddPendingCheckpointAfter(Checkpoint::Reason, Node*);

  // Invoked before the JSCall instruction. Used to store all the unboxed value,
  // that has been modified back to the interpreter(vm) frame. This is needed
  // since it sync the VM and JIT frame and also allow we safely calls into the
  // quickjs's runtime system
  void FrameSync(InterpInfo*);
  void OnGuard(Node*);
  void OnGuard(SSAVar*);

  Checkpoint* AddCheckpointToGraph(BytecodeInfoProvider*, Checkpoint::Reason,
                                   Node*, bool);

  Checkpoint* DoSnapshot(BytecodeInfoProvider* info, Checkpoint::Reason reason,
                         bool);

  Checkpoint* SnapshotAfter(BytecodeInfoProvider* info,
                            Checkpoint::Reason reason);
  Checkpoint* SnapshotBefore(BytecodeInfoProvider* info,
                             Checkpoint::Reason reason);

  Checkpoint* ReemitCheckpointToGraph(BytecodeInfoProvider* info,
                                      Checkpoint::Reason reason,
                                      Node* current_node,
                                      Checkpoint::ExtraTempList&& tlist,
                                      bool is_after);

  template <typename... ARGS>
  SSAVar* AddGuardValue(InterpInfo* info, Opcode opcode, ARGS&&... args) {
    auto value = graph_->AddValue(info, opcode, std::forward<ARGS>(args)...);
    OnGuard(value);
    return value;
  }

  template <typename... ARGS>
  Node* AddGuardOp(InterpInfo* info, Opcode opcode, ARGS&&... args) {
    auto op = graph_->AddOp(info, opcode, std::forward<ARGS>(args)...);
    OnGuard(op);
    return op;
  }

  Node* GuardNotExceptionNoSnapshot(InterpInfo* info, SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsNotException, operand);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }

  Node* GuardNotException(InterpInfo* info, SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsNotException, operand);
    AddPendingSnapshot(Checkpoint::kSideEffect, true);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }

  Node* GuardIsUninitialized(InterpInfo* info, SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsUninitialized, operand);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }
  Node* GuardIsNotUninitialized(InterpInfo* info, SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsNotUninitialized, operand);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }

  void UpdatePendingGuardNotException(InterpInfo* info, SSAVar* value) {
    UNUSE(info);
    CHECK(!pending_guard_not_exception_);
    pending_guard_not_exception_.emplace(value);
  }
  void FlushPendingGuardNotException(InterpInfo* info);

  void RcInc(InterpInfo*, SSAVar*);
  void RcDec(InterpInfo*, SSAVar*);
  bool FlushRC(InterpInfo*);
  void PushSSA(InterpInfo*, SSAVar*);
  TraceResult PushJVal(InterpInfo*, JVal);

  std::pair<TraceResult, SSAVar*> Unbox(InterpInfo*, JVal, SSAVar*);
  TraceResult Box(InterpInfo*, SSAVar*);

  std::pair<TraceResult, SSAVar*> DoStringConcate(InterpInfo*, SSAVar*,
                                                  SSAVar*);
  std::pair<TraceResult, SSAVar*> DoArithmeticBasic(InterpInfo*, Opcode, Opcode,
                                                    SSAVar*, SSAVar*);
  std::pair<TraceResult, SSAVar*> DoComparisonEqualism(InterpInfo*, SSAVar*,
                                                       SSAVar*, bool,
                                                       bool* is_fallback);

  std::pair<TraceResult, SSAVar*> DoComparisonNoneEqualism(InterpInfo*, SSAVar*,
                                                           SSAVar*, Opcode,
                                                           Opcode, Opcode,
                                                           Opcode,
                                                           bool* is_fallback);
  TraceResult DoTraceStrictComparison(InterpInfo*, bool);

  void AddPhiLoopCandidate(ValType, int);

 private:
  // --------------------------------------------------------------------------
  // Reemit pass
  SSAVar* GetMappings(const SSAVarMappings&, SSAVar*);
  SSAVar* GetMappingsChecked(const SSAVarMappings&, SSAVar*);
  void SetMappings(SSAVarMappings*, SSAVar*, SSAVar*);

  void MarkLocSet(const SSAVarMappings&, Node*);
  void MarkParamSet(const SSAVarMappings&, Node*);

  // Reemit, notes, if we cannot finish reemit, ie the post loop code mutate
  // type of certain node, which is nearly to impossible, we will just bailout
  // Currently our IR is not capable of handling multiple types
  bool Reemit(InterpInfo*);
  bool InsertPhiCandidate(Node**);
  SSAVar* TryFoldPhi(Node*, SSAVar*, SSAVar*);
  bool FixupPhi();
  bool FixupDup();
  bool IsAllowedPhiType(const Type&) const;

  void RewriteCheckpoint(Node*, Node*, const SSAVarMappings&);
  void RewriteExit(Node*, Node*, const SSAVarMappings&);

  RewriteNode RewriteValGet(Node*, SSAVarMappings*, SSAVar**);
  RewriteNode RewriteLocGet(Node*, SSAVarMappings*);
  RewriteNode RewriteParamGet(Node*, SSAVarMappings*);
  RewriteNode RewriteTempGet(Node*, SSAVarMappings*);
  RewriteNode RewriteBxGuard(Node*, SSAVarMappings*);
  RewriteNode RewriteCloneRc(Node*, SSAVarMappings*);
  RewriteNode ReemitNode(Node*, SSAVarMappings*);

  TraceResult TraceBranchIf(InterpInfo*, bool);
  void AddBranchExitSnapshot(InterpInfo* info, const void*);

  SSAVar* GuardIntPositive(InterpInfo*, SSAVar*);
  SSAVar* GuardIntPositiveOrZero(InterpInfo*, SSAVar*);
  SSAVar* GuardIntNegative(InterpInfo*, SSAVar*);

  std::optional<SSAVar*> GuardFold(InterpInfo*, SSAVar*, Opcode);

  void UpdateJumpTo(const void* jump_to) { jump_to_ = jump_to; }

  Opcode MaybeUpgradeOpcode(Node*, SSAVar*, SSAVar*);
  Opcode MaybeUpgradeOpcode(Node*, SSAVar*);

  SSAVar* UpgradeToFloat64IfNeeded(Opcode, Node*, SSAVar*);

  std::optional<SSAVar*> CastToFloat64(InterpInfo*, SSAVar*);
  SSAVar* MustCastToFloat64(InterpInfo*, SSAVar*);

  std::optional<SSAVar*> CastToInt32(InterpInfo*, SSAVar*);
  SSAVar* MustCastToInt32(InterpInfo*, SSAVar*);

  std::optional<SSAVar*> CastToString(InterpInfo*, SSAVar*);
  SSAVar* MustCastToString(InterpInfo*, SSAVar*);

  std::optional<SSAVar*> CastToBit(InterpInfo*, SSAVar*);
  SSAVar* MustCastToBit(InterpInfo*, SSAVar*);

 private:
  bool Optimize();
  void LoopExitAnalyse();

 private:
  bool IsInNestedLoop(InterpInfo*) const {
    return static_cast<bool>(nested_loop_);
  }
  bool IsNestedLoop(InterpInfo*) const;

 private:
  // --------------------------------------------------------------------------
  // All the trace sub-handlers
#define __(A) TraceResult Trace_##A(InterpInfo*);
  QUAKE_JIT_BYTECODE_LIST(__)
#undef __  // __
  // --------------------------------------------------------------------------

 private:
  Quake* quake_;
  Graph* graph_;

  // --------------------------------------------------------------------------
  // Trace formation testing
  const void*
      start_bc_;         // the starting point of BC, used to testify a OP_goto
                         // to jump to the start_bc_ or not. If so, it means the
                         // end of the trace, and we can perform a trace stop
  std::size_t bc_size_;  // size of bc been recoreded

  // --------------------------------------------------------------------------
  // RC handling
  std::vector<SSAVar*> rc_queue_;

  bool rc_flushed_;  // will be reset for each bytecode handling, used to
                     // perform sanity check during bytecode trace. The
                     // assumption is you cannot perform GUARD(which jumps out
                     // to a checkpoint) after rc flush in the middle of a
                     // bytecode handler.

  // --------------------------------------------------------------------------
  // Checkpoint
  Checkpoint* checkpoint_;  // current checkpoint, always none null

  std::optional<PendingCheckpoint>
      pending_checkpoint_;  // whether need another checkpoint after the handler
                            // is done. Notes, user should NOT generate
                            // checkpoint in the middle of bytecode generation.
                            // And one bytecode can just generate one checkpoint

  std::optional<PendingSnapshot> pending_snapshot_;

  // Very specialized usage, ie happend after the RC. Currently only used for
  // certain specialized bytecode, ie OP_get_array_el which requires the
  // RCFree and then checks whether exception happened or not
  std::optional<SSAVar*> pending_guard_not_exception_;

  // --------------------------------------------------------------------------
  // Trace info
  InterpInfo* interp_info_;  // current interp_info_, mutated for every trace
  Bytecode last_bc_;     // previous bytecode, if nothing Bytecode::OP_invalid
  const void* last_pc_;  // last bytecode's PC
  const void* jump_to_;  // if the current bc is a branch instruction, then
                         // this field will be set to the pointed position

  // --------------------------------------------------------------------------
  // Pending Jump
  //
  // Each jump instruction will be resolved, ie set its corresponding operand
  // after the trace is done. The issue is we may not be able to get operand
  // of a jump if the jump is a forward jump.
  struct PendingJump {
    const void* jump_to;  // where the jump points to, in terms of bytecode
    Node* jump;           // the jump node
  };
  std::vector<PendingJump> pending_jump_;

  struct PendingTarget {
    const void* from_bc;
    const void* to_bc;
    PendingTarget(const void* f, const void* t) : from_bc(f), to_bc(t) {}
  };
  std::vector<PendingTarget> pending_target_;

  // --------------------------------------------------------------------------
  // We need a way to detect any untaken branch to learn whether those branches
  // leads to a goto that is jump outside of the loop range. One way is to
  // construct full CFG but that can only perform a approximate detection due to
  // the fact that we don't know which BB is been taken for the trace. To solve
  // this we uses a deferred analysis way. Basically, during the tracing we
  // record all the branch that is not been taken. After the full trace
  // formation, we can find out those *hole* inside of the loop' bytecode range
  // and we can know the range of bytecode which we are not taken to contain
  // a jump that leads to somewhere outside of the loop and we are for sure
  // acertain that these jumps are outside of the loop
  struct TraceSnapshot {
    const void* start_bc;
    const void* end_bc;  // this can be null, which means we do not know where
                         // to stop
    Checkpoint* checkpoint;  // corresponding checkpoint

    TraceSnapshot(const void* bc, Checkpoint* ck)
        : start_bc(bc), end_bc(nullptr), checkpoint(ck) {}

    TraceSnapshot(const void* start, const void* end, Checkpoint* ck)
        : start_bc(start), end_bc(end), checkpoint(ck) {}
  };
  std::vector<TraceSnapshot> trace_snapshot_list_;
  std::vector<BytecodeRange> hole_list_;

  // --------------------------------------------------------------------------
  // Frame information
  std::vector<IRFrame> frame_;

  // --------------------------------------------------------------------------
  // PHI
  //
  // when first record, we record which slot(param/local) is been stored these
  // slot can be phi candidate
  std::vector<PhiVal> phi_loop_candidate_;

  // all the inserted duplication list, after the reemittion phase done, we will
  // perform an optimization against this duplication list
  std::vector<SSAVar*> dup_list_;

  // --------------------------------------------------------------------------
  // Nested loop
  //
  // Notes, a loop can contain any number of nested loop, but during jit tracing
  // phase, one time there will be just one at a time, the jitted loop wil be
  // enterred for the tracing iteration done of that loop.
  std::shared_ptr<Quake*> nested_loop_;
  NestedLoopTraceState nested_loop_trace_state_;

  // --------------------------------------------------------------------------
  // Graph Reemit

  SSAVar* end_of_bb0_;
  SSAVar* end_of_bb1_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_BUILDER_H_
