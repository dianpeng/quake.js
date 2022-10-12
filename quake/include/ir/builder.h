#ifndef _QUAKE_IR_BUILDER_H_
#define _QUAKE_IR_BUILDER_H_
#include "base.h"
#include "zone.h"

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
    const void* resumed_bc;
    Node* trigger_node;
    PendingCheckpoint(Checkpoint::Reason r, const void* b, Node* tnode)
        : reason(r), resumed_bc(b), trigger_node(tnode) {}
  };
  enum ValType { kParam, kLocal };

  struct PhiVal {
    ValType vtype;
    int index;
    SSAVar* phi;
    PhiVal(ValType vt, int i) : vtype(vt), index(i), phi(nullptr) {}
  };

  using SSAVarMappings = std::unordered_map<SSAVar*, SSAVar*>;

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

 public:
  SSAVar** param() { return storage_.data(); }
  SSAVar** param_end() { return param() + param_size_; }

  SSAVar** local() { return param_end(); }
  SSAVar** local_end() { return local() + local_size_; }

  SSAVar** temp() { return local_end(); }
  SSAVar** temp_end() { return storage_.data() + storage_.size(); }

  SSAVar** slot() { return storage_.data(); }
  SSAVar** slot_end() { return storage_.data() + storage_.size(); }

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

 private:
  bool HasRCQueued() const { return !rc_queue_.empty(); }

  TraceResult AfterTrace(InterpInfo*);
  TraceResult BeforeTrace(InterpInfo*);

  std::size_t StkIdx(int) const;
  std::pair<TraceResult, SSAVar*> StkAt(InterpInfo*, int);
  void StkSet(int, SSAVar*);
  void Push(SSAVar*);

  void Pop();
  void Pop(int);
  void PopAndRcDec(InterpInfo*);

  std::pair<TraceResult, SSAVar*> LocAt(InterpInfo*, int);
  TraceResult LocSet(InterpInfo*, int, SSAVar*);
  void RcTryDecLoc(InterpInfo*, int);
  bool CheckLocSlotUninitialized(InterpInfo*, int);

  std::pair<TraceResult, SSAVar*> ParamAt(InterpInfo*, int);
  TraceResult ParamSet(InterpInfo*, int, SSAVar*);
  void RcTryDecParam(InterpInfo*, int);

  void UpdateCheckpoint(InterpInfo*, Checkpoint::Reason, const void*, Node*);
  void UpdateCheckpointAfter(InterpInfo*, Checkpoint::Reason, Node*);
  void AddCheckpointAfter(Checkpoint::Reason, const void*, Node*);
  void UpdatePendingCheckpoint(InterpInfo*);
  void OnGuard(Node*);
  void OnGuard(SSAVar*);

  void RcInc(InterpInfo*, SSAVar*);
  void RcDec(InterpInfo*, SSAVar*);
  bool FlushRC(InterpInfo*);

  void PushSSA(InterpInfo*, SSAVar*);

  TraceResult PushJVal(InterpInfo*, JVal);
  std::pair<TraceResult, SSAVar*> CheckZeroInt(InterpInfo*, SSAVar*);

  std::pair<TraceResult, SSAVar*> Unbox(InterpInfo*, JVal, SSAVar*);
  TraceResult Box(InterpInfo*, SSAVar*);

  std::pair<TraceResult, SSAVar*> DoArith(InterpInfo*, Opcode, Opcode, SSAVar*,
                                          SSAVar*);
  std::pair<TraceResult, SSAVar*> DoComp(InterpInfo*, Opcode, Opcode, SSAVar*,
                                         SSAVar*);

  void AddPhiLoopCandidate(ValType, int);

 private:
  // --------------------------------------------------------------------------
  // Reemit pass
  SSAVar* GetMappings(const SSAVarMappings&, SSAVar*);
  SSAVar* GetMappingsChecked(const SSAVarMappings&, SSAVar*);
  void SetMappings(SSAVarMappings*, SSAVar*, SSAVar*);

  void MarkLocSet(const SSAVarMappings&, Node*);
  void MarkParamSet(const SSAVarMappings&, Node*);

  void Reemit(InterpInfo*);
  void InsertPhiCandidate();
  void FixupPhi();
  void RewriteCheckpoint(Node*, Node*, const SSAVarMappings&);
  void RewriteExit(Node*, Node*, const SSAVarMappings&);

  SSAVar* RewriteValGet(Node*, SSAVarMappings*, SSAVar**);
  SSAVar* RewriteLocGet(Node*, SSAVarMappings*);
  SSAVar* RewriteParamGet(Node*, SSAVarMappings*);
  SSAVar* RewriteTempGet(Node*, SSAVarMappings*);
  SSAVar* RewriteBxGuard(Node*, SSAVarMappings*);
  SSAVar* RewriteCloneRc(Node*, SSAVarMappings*);
  SSAVar* RewriteClone(Node*, SSAVarMappings*);

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
  bool checkpoint_done_;    // whether a effect checkpoint has already been
                            // generated or not

  std::optional<PendingCheckpoint>
      pending_checkpoint_;  // whether need another checkpoint after the handler
                            // is done. Notes, user should NOT generate
                            // checkpoint in the middle of bytecode generation.
                            // And one bytecode can just generate one checkpoint

  // --------------------------------------------------------------------------
  // Trace info
  InterpInfo* interp_info_;  // current interp_info_, mutated for every trace
  Bytecode last_bc_;     // previous bytecode, if nothing Bytecode::OP_invalid
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

  // --------------------------------------------------------------------------
  // Storage info
  std::uint32_t param_size_;
  std::uint32_t local_size_;

  std::vector<SSAVar*> storage_;

  // --------------------------------------------------------------------------
  // PHI
  //
  // when first record, we record which slot(param/local) is been stored these
  // slot can be phi candidate

  std::vector<PhiVal> phi_loop_candidate_;

  // --------------------------------------------------------------------------
  // Graph Reemit

  SSAVar* end_of_bb0_;
  SSAVar* end_of_bb1_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_BUILDER_H_
