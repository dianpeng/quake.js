#ifndef _QUAKE_IR_BUILDER_H_
#define _QUAKE_IR_BUILDER_H_
#include "base.h"
#include "zone.h"
#include "frame-layout.h"

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

  enum ValType { kParam, kLocal, kTemp };

  struct PhiVal {
    ValType vtype;
    int index;
    SSAVar* phi;
    PhiVal(ValType vt, int i) : vtype(vt), index(i), phi(nullptr) {}
  };

  using PhiList = std::vector<PhiVal>;

  using SSAVarMappings = std::unordered_map<SSAVar*, SSAVar*>;

  // Used for nested loop trace
  enum class NestedLoopTraceState {
    kNone,     // no nested loop
    kSkip,     // skipping the nested loop bytecode until we hit jit enter
    kRun,      // the jit loop should start running, wait for nested loop exit
    kLoopExit  // the jit enters into the loop exit jitting
  };

  enum class NestedLoopJump {
    kNoJump,     // no jump
    kInnerJump,  // jump still reside inside of the inner loop
    kLoopback,   // jump performs a loopback, ie indication of end of inner loop
    kBailout     // jump targets at outside of the loop
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

  BytecodeRange bytecode_range() const {
    return BytecodeRange{start_bc(), quake_->trigger_back_bc()};
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

    // relative stack offset used by JIT code to emit temporary loading/storing
    // operations. This operation is related to the temp_cur
    int StkOffset(int) const;
    int StkOffsetFromRIndex(std::size_t) const;
    std::size_t StkOffsetToRIndex(int) const;
    int stack_cur() const { return stack_cur_; }

    // if not the entry frame, it points to a EnterInlineFrame node, which means
    // an inline frame
    Node* frame_instruction() const { return frame_instruction_; }

    // clang-format off
    IRFrame(JitFrameType t,
            JVal&& function,
            std::size_t ssize,
            std::size_t psize,
            std::size_t lsize,
            const void* cbc,
            const void* nbc,
            Node* frame_instruction,
            SSAVar* fobj,
            SSAVar* tobj,
            SSAVar* tarobj,
            std::size_t vsize,
            std::size_t pop_size,
            std::size_t aarg_size,
            bool is_tail,
            QuakeWorld* qworld
      )
        : type_(t),
        function_(std::move(function)),
          param_size_(psize),
          local_size_(lsize),
          storage_(),
          cur_bc_(cbc),
          next_bc_(nbc),
          frame_instruction_(frame_instruction),
          stack_cur_(ssize - psize - lsize),
          function_object_(fobj),
          this_object_(tobj),
          target_object_(tarobj),
          var_ref_size_(vsize),
          pop_size_(pop_size),
          actual_arg_size_(aarg_size),
          is_tail_(is_tail),
          quake_world_(qworld) {
      storage_.resize(ssize);
    }
    // clang-format on

   public:
    SSAVar* function_object() const { return function_object_; }
    SSAVar* this_object() const { return this_object_; }
    SSAVar* target_object() const { return target_object_; }
    std::size_t pop_size() const { return pop_size_; }
    std::size_t var_ref_size() const { return var_ref_size_; }
    std::size_t actual_arg_size() const { return actual_arg_size_; }
    bool is_tail() const { return is_tail_; }
    QuakeWorld* quake_world() const { return quake_world_; }
    JVal function() const { return function_; }

   private:
    std::size_t StkIdx(int) const;

    void Push(SSAVar*);
    void Pop();
    void Pop(int);

   private:
    JitFrameType type_;
    JVal function_;  // current function (QJS)

    std::size_t param_size_;
    std::size_t local_size_;
    std::vector<SSAVar*> storage_;
    const void* cur_bc_;
    const void* next_bc_;

    Node* frame_instruction_;  // if this frame is not entry frame, then it has
                               // a frame node which can be used for later usage

    // stack offset W.R.T the VM's frame. QuickJS's bytecode's stack layout is
    // not consistent, the temporary loading/storing needs to be adjusted to
    // the offset during tracing
    int stack_cur_;

   private:
    // Mostly used by inline frame
    SSAVar* function_object_;
    SSAVar* this_object_;
    SSAVar* target_object_;
    std::size_t var_ref_size_;  // upvalue size
    std::size_t pop_size_;
    std::size_t actual_arg_size_;  // actual argument size

    // whether this frame is been entered via a tail call
    bool is_tail_;

    QuakeWorld* quake_world_;

    friend class Builder;
  };

  // Used during reemition phase to reemit the inline frame instruction
  struct InlineFrameInfo {
    JSBytecodeInternalInfo bc_info;  // the bytecode info
    JVal function;                   // function object
    int call_argc;                   // the actual calling argument
    bool is_tail_call;               // whether it is a tail call
    PhiList phi_candidate;           // current frame's phi candidate
                                     // need to fixup during reemition phase
                                     // when its corresponding *leave frame*
                                     // is generated.

    InlineFrameInfo(const JSBytecodeInternalInfo& i, JVal&& func, int cargc,
                    bool t)
        : bc_info(i),
          function(std::move(func)),
          call_argc(cargc),
          is_tail_call(t) {}
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
  IRFrame& prev_frame() {
    CHECK(frame_.size() >= 2);
    return frame_[frame_.size() - 2];
  }

  const IRFrame& cur_frame() const { return frame_.back(); }
  const std::vector<IRFrame>& frame() const { return frame_; }
  std::vector<IRFrame>& frame() { return frame_; }

  PhiList* phi_list();

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

  // clang-format off
  void EnterFrame(JitFrameType,   // frame type
                  JVal&&,         // function object
                  std::size_t,    // slot size
                  std::size_t,    // parameter size
                  std::size_t,    // local variable size
                  const void*,    // current PC
                  const void*,    // next PC
                  Node*,          // instruction generates the frame
                  SSAVar*,        // function object
                  SSAVar*,        // this object
                  SSAVar*,        // target object
                  std::size_t,    // var_ref size
                  std::size_t,    // pop stack size
                  std::size_t,    // actual argument size
                  bool,           // whether it is tail call
                  QuakeWorld*);   // the related function's QuakeWorld if it has

  void EnterInlineFrame(BytecodeInfoProvider*,
                        JVal&&,                        // function object
                        int,                           // call argument size
                        const std::vector<SSAVar*>&,   // argument SSAVar
                        Node*,                         // enter inline frame
                        SSAVar*,                       // function object
                        SSAVar*,                       // this object
                        SSAVar*,                       // target object
                        std::size_t pop_size,          // pop_size
                        bool is_tail,                  // is tail call
                        QuakeWorld*);                  // quake world
  // clang-format on

  void EnterEntryFrame(InterpInfo*);
  void LeaveFrame(BytecodeInfoProvider*);

  // Used by inline frame to correctly generate the parameter RC deref. The
  // parameter RC deref is kind of tricky due to the fact that the VM uses
  // these are memory instead of immutable object and thus making the caller
  // needs to take care of the parameter RC mutation when the callee mutates
  // the storage class of parameter. For example:
  //
  // function bar(a) {
  //  a = {};
  // }
  // function foo() {
  //  for (let i = 0; i < 100; i++) {
  //    bar(1);
  //  }
  // }
  //
  // When Jitting foo, we see the parameter of bar call to be 1, which means
  // the parameter is a none heap object. However when the bar mutate the a,
  // ie the parameter passed in by the foo function, to be an object, it is
  // the *foo*'s responsibility to cleanup the parameter in quickjs design.
  // From the tracer's perspectives, the argument is fixed to be 1, therefore
  // the jit code needs to treat the parameter passed into the function to
  // be immutable memory.
  //
  // When we are inlining the function call, this becomes obvious for us since
  // the inner function call we know what it does. Therefore, we just need to
  // based on the inner function's inline stack's parameter cases to generate
  // reference counting code. Which sort of optimized the parameter RC drop
  // cases.
  std::vector<SSAVar*> FrameParameterToList() const;

  bool IsInline() const { return frame_.size() > 1; }
  bool IsEntry() const { return !IsInline(); }

  std::size_t InlineCallDepth() const {
    return frame_.size() <= 1 ? 0 : frame_.size() - 1;
  }

  InlineFrameInfo* NewInlineFrameInfo(JVal&&, int, bool);

 private:
  TraceResult StartInlineCall(InterpInfo*, int, bool);
  TraceResult StopInlineCall(InterpInfo*, SSAVar*);
  TraceResult TryInlineCall(InterpInfo*, int, bool);
  bool DecideInlineCall(InterpInfo* info, JVal&& func);

  bool ReemitEnterInlineFrame(Node*, SSAVarMappings*);
  bool ReemitLeaveInlineFrame(Node*, SSAVarMappings*);

  // Side effect operations, ie which touches/modifies the underlying VM
  // stack in a observsable way. For example, OP_for_of_start/OP_for_in_start
  // will replace the value, which is originally a collection, on stack with
  // iterator.
  bool ReemitForOfStart(Node*, SSAVarMappings*);
  bool ReemitForInStart(Node*, SSAVarMappings*);

 private:
  bool HasRCQueued() const { return !rc_queue_.empty(); }

  TraceResult AfterTrace(InterpInfo*);
  TraceResult BeforeTrace(InterpInfo*);

  // raw get
  SSAVar* StkGet(InterpInfo*, int);
  std::pair<TraceResult, SSAVar*> StkAt(InterpInfo*, int);
  void StkSet(int, SSAVar*);

  // Different from StkSet, if the index is out of boundary, then it fills
  // nullptr for the temp
  void TempFill(std::uint32_t, SSAVar*);

  std::size_t StkIdx(int idx) const { return cur_frame().StkIdx(idx); }
  int StkOffset(int idx) const { return cur_frame().StkOffset(idx); }
  int StkOffsetFromRIndex(std::size_t idx) const {
    return cur_frame().StkOffsetFromRIndex(idx);
  }
  std::size_t StkOffsetToRIndex(int idx) const {
    return cur_frame().StkOffsetToRIndex(idx);
  }

  void Push(SSAVar* v);
  void Pop();
  void Pop(int x);

  void PopAndRcDec(InterpInfo*);
  bool IsValMutated(SSAVar* param, ValType vtype, int idx);

  std::pair<TraceResult, SSAVar*> LocAt(InterpInfo*, int);
  TraceResult LocSet(InterpInfo*, int, SSAVar*);
  void RcTryDecLoc(InterpInfo*, int);
  bool CheckLocSlotUninitialized(InterpInfo*, int);

  std::pair<TraceResult, SSAVar*> ParamAt(InterpInfo*, int);
  TraceResult ParamSet(InterpInfo*, int, SSAVar*);
  void RcTryDecParam(InterpInfo*, int);

 private:
  enum class TraceCallType { kCall, kTailCall, kIterator };

  SSAVar* DoTraceCall(InterpInfo*, int, SSAVar*, SSAVar*, SSAVar*,
                      TraceCallType);

  SSAVar* DoTraceCallConstructor(InterpInfo*, int, SSAVar*, SSAVar*);
  SSAVar* DoTraceArrayFrom(InterpInfo*, int);
  SSAVar* DoTraceApply(InterpInfo*, SSAVar*, int);
  SSAVar* DoTraceLoadArray(InterpInfo*, SSAVar*, SSAVar*);

  TraceResult DoVMCall(InterpInfo* info, int call_argc, bool is_tail_call);

  TraceResult DoFreeCall(InterpInfo*, bool is_tail_call);
  TraceResult DoMethodCall(InterpInfo*, bool is_tail_call);

  void DoFrameFinishNoFlushRC(InterpInfo*);
  void DoFrameFinish(InterpInfo*);
  void DoFrameParamCopyFinish(InterpInfo*);
  void DoFrameFinishReturnToVM(InterpInfo*);

  TraceResult DoReturn(InterpInfo*, SSAVar*);

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

  void RefreshBytecodeInfoForCheckpoint(BytecodeInfoProvider*,
                                        Checkpoint::Reason);

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

  Checkpoint* ReemitCheckpointToGraph(BytecodeInfoProvider* info, Checkpoint*,
                                      Node* current_node, bool is_after);

  template <typename... ARGS>
  SSAVar* AddGuardValue(BytecodeInfoProvider* info, Opcode opcode,
                        ARGS&&... args) {
    auto value = graph_->AddValue(info, opcode, std::forward<ARGS>(args)...);
    OnGuard(value);
    return value;
  }

  template <typename... ARGS>
  Node* AddGuardOp(BytecodeInfoProvider* info, Opcode opcode, ARGS&&... args) {
    auto op = graph_->AddOp(info, opcode, std::forward<ARGS>(args)...);
    OnGuard(op);
    return op;
  }

  Node* GuardNotExceptionNoSnapshot(BytecodeInfoProvider* info,
                                    SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsNotException, operand);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }

  Node* GuardNotException(BytecodeInfoProvider* info, SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsNotException, operand);
    AddPendingSnapshot(Checkpoint::kSideEffect, true);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }

  Node* GuardIsUninitialized(BytecodeInfoProvider* info, SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsUninitialized, operand);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }
  Node* GuardIsNotUninitialized(BytecodeInfoProvider* info, SSAVar* operand) {
    auto n = AddGuardOp(info, Opcode::kGIsNotUninitialized, operand);
    CHECK(n->exit()->reason() == Checkpoint::kException);
    return n;
  }

  void UpdatePendingGuardNotException(BytecodeInfoProvider* info,
                                      SSAVar* value) {
    UNUSE(info);
    CHECK(!pending_guard_not_exception_);
    pending_guard_not_exception_.emplace(value);
  }
  void FlushPendingGuardNotException(InterpInfo* info);

  void RcInc(InterpInfo*, SSAVar*);
  void RcDec(InterpInfo*, SSAVar*);
  void RcTryDecTemp(InterpInfo*, int);
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
  void RemovePhiLoopCandidate(ValType, int);

 private:
  void AddTempNullify(InterpInfo*, int);

 private:
  // --------------------------------------------------------------------------
  // Reemit pass
  SSAVar* GetMappings(const SSAVarMappings&, SSAVar*);
  SSAVar* GetMappingsChecked(const SSAVarMappings&, SSAVar*);
  void SetMappings(SSAVarMappings*, SSAVar*, SSAVar*);

  void MarkLocSet(const SSAVarMappings&, Node*);
  void MarkParamSet(const SSAVarMappings&, Node*);
  void MarkTempNullify(const SSAVarMappings&, Node*);
  void MarkTempPush(const SSAVarMappings&, Node*);
  void MarkTempPop(const SSAVarMappings&, Node*);
  void MarkTempSet(const SSAVarMappings&, Node*);

  // Reemit, notes, if we cannot finish reemit, ie the post loop code mutate
  // type of certain node, which is nearly to impossible, we will just bailout
  // Currently our IR is not capable of handling multiple types
  bool Reemit(InterpInfo*);
  bool FinalizeForNoneLoop(InterpInfo*);
  bool InsertPhiCandidate(Node**);
  SSAVar* TryFoldPhi(Node*, SSAVar*, SSAVar*);
  bool FixupPhi();
  bool FixupDup();

  void RewriteCheckpoint(Node*, Node*, const SSAVarMappings&);
  void RewriteExit(Node*, Node*, const SSAVarMappings&);

  RewriteNode RewriteValGet(Node*, SSAVarMappings*, SSAVar**);
  RewriteNode RewriteLocGet(Node*, SSAVarMappings*);
  RewriteNode RewriteParamGet(Node*, SSAVarMappings*);
  RewriteNode RewriteTempGet(Node*, SSAVarMappings*);
  RewriteNode RewriteBxGuard(Node*, SSAVarMappings*);
  RewriteNode RewriteCloneRc(Node*, SSAVarMappings*);
  RewriteNode ReemitNode(Node*, SSAVarMappings*);

  // Special bytecode which needs to be taken care during re-emition
  bool ReemitEnterJitTrace(Node*, SSAVarMappings*);

  SSAVar* BuildCondition(InterpInfo*, SSAVar*);
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

  Node* BoxToTemp(BytecodeInfoProvider* info, SSAVar* idx, SSAVar* value,
                  bool erase_phi);
  Node* BoxToLocal(BytecodeInfoProvider* info, SSAVar* idx, SSAVar* value,
                   bool erase_phi);
  Node* BoxToParam(BytecodeInfoProvider* info, SSAVar* idx, SSAVar* value,
                   bool erase_phi);
  Node* BoxTo(BytecodeInfoProvider* info, Opcode opcode, SSAVar* idx,
              SSAVar* value, bool erase_phi);

  TraceResult DoIncDecPrefix(InterpInfo* info, bool is_inc);
  TraceResult DoIncDecPostfix(InterpInfo* info, bool is_inc);

  TraceResult DoPushThisSloppy(InterpInfo*);
  TraceResult DoPushThisStrict(InterpInfo*);

 private:
  TraceResult DoSpecialObjectArguments(InterpInfo* info);
  TraceResult DoSpecialObjectMappedArguments(InterpInfo* info);
  TraceResult DoSpecialObjectThisFunc(InterpInfo* info);
  TraceResult DoSpecialObjectNewTarget(InterpInfo* info);
  TraceResult DoSpecialObjectHomeObject(InterpInfo* info);
  TraceResult DoSpecialObjectVarObject(InterpInfo* info);
  TraceResult DoSpecialObjectImportMeta(InterpInfo* info);

 private:
  bool Optimize();
  void LoopExitAnalyse();

 private:
  // --------------------------------------------------------------------------
  // Nested loop handling
  bool NestedLoopStartForward(InterpInfo*);
  bool NestedLoopStartBackward(InterpInfo*);

  std::optional<NestedLoopJump> FindNestedLoopJump(InterpInfo*);
  std::vector<DeoptId> FindAllLoopExit(InterpInfo*, const Quake&, DeoptId);
  TraceResult EnterNestedLoop(InterpInfo*, DeoptId);
  bool CanNestedLoopBeLinked(InterpInfo*);
  void StopNestedLoopExit();
  TraceResult LoopExitTrace(InterpInfo*);
  TraceResult TryNestedLoop(InterpInfo*);

 private:
  const void* last_pc() const {
    CHECK(!trace_info_.empty());
    return trace_info_.back().last_pc;
  }

  void set_last_pc(const void* p) {
    CHECK(!trace_info_.empty());
    trace_info_.back().last_pc = p;
  }

  Bytecode last_bc() const {
    CHECK(!trace_info_.empty());
    return trace_info_.back().last_bc;
  }

  void set_last_bc(Bytecode bc) {
    CHECK(!trace_info_.empty());
    trace_info_.back().last_bc = bc;
  }

 private:
  enum class InlineCallState {
    kNoInline,
    kStartInline,
    kSkipInlineCall,
    kResumeInlineCall
  };

  void set_inline_call_state(InlineCallState s) { inline_call_state_ = s; }

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
  // Context information for each traced bytecode
  InterpInfo* interp_info_;
  const void* jump_to_ = nullptr;
  std::size_t call_depth_ = 0;
  bool pending_tail_call_ = false;

  // --------------------------------------------------------------------------
  // Trace info
  struct TraceInfo {
    Bytecode last_bc = Bytecode::OP_invalid;
    const void* last_pc = nullptr;
  };
  std::vector<TraceInfo> trace_info_;

  // --------------------------------------------------------------------------
  // Inline Call
  InlineCallState inline_call_state_ = InlineCallState::kNoInline;

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
  std::vector<std::unique_ptr<InlineFrameInfo>> frame_info_;

  // --------------------------------------------------------------------------
  // PHI
  //
  // when first record, we record which slot(param/local) is been stored these
  // slot can be phi candidate
  std::vector<PhiVal> entry_phi_list_;

  // all the inserted duplication list, after the reemittion phase done, we will
  // perform an optimization against this duplication list
  std::vector<SSAVar*> dup_list_;

  // --------------------------------------------------------------------------
  // Nested loop
  //
  // Notes, a loop can contain any number of nested loop, but during jit tracing
  // phase, one time there will be just one at a time, the jitted loop wil be
  // enterred for the tracing iteration done of that loop.
  std::shared_ptr<Quake> nested_loop_;
  NestedLoopTraceState nested_loop_trace_state_;

  // --------------------------------------------------------------------------
  // Graph Reemit
  SSAVar* end_of_bb0_;
  SSAVar* end_of_bb1_;
  bool is_reemit_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_BUILDER_H_
