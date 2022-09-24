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

// A list of allowed bytecode, ie can be jitted. If function contains bytecode
// it is not showed up inside of our bytecode list, the JIT will bailout back
// to interpreter and always mark the function not able to be JITTed
#define QUAKE_JIT_BYTECODE_LIST(__) \
  __(OP_push_i32)                   \
  __(OP_push_const)                 \
  __(OP_push_atom_value)            \
  __(OP_undefined)                  \
  __(OP_null)                       \
  __(OP_push_false)                 \
  __(OP_push_true)                  \
  __(OP_push_this)                  \
  __(OP_drop)                       \
  __(OP_nip)                        \
  __(OP_nip1)                       \
  __(OP_dup)                        \
  __(OP_dup1)                       \
  __(OP_dup2)                       \
  __(OP_dup3)                       \
  __(OP_insert2)                    \
  __(OP_insert3)                    \
  __(OP_insert4)                    \
  __(OP_rot3l)                      \
  __(OP_rot4l)                      \
  __(OP_rot5l)                      \
  __(OP_rot3r)                      \
  __(OP_perm3)                      \
  __(OP_perm4)                      \
  __(OP_perm5)                      \
  __(OP_swap)                       \
  __(OP_swap2)                      \
  __(OP_get_loc)                    \
  __(OP_set_loc)                    \
  __(OP_put_loc)                    \
  __(OP_get_arg)                    \
  __(OP_set_arg)                    \
  __(OP_put_arg)                    \
  __(OP_add)                        \
  __(OP_sub)                        \
  __(OP_mul)                        \
  __(OP_div)                        \
  __(OP_add_loc)                    \
  __(OP_shl)                        \
  __(OP_shr)                        \
  __(OP_sar)                        \
  __(OP_and)                        \
  __(OP_or)                         \
  __(OP_xor)                        \
  __(OP_lt)                         \
  __(OP_lte)                        \
  __(OP_gte)                        \
  __(OP_gt)                         \
  __(OP_eq)                         \
  __(OP_neq)                        \
  __(OP_plus)                       \
  __(OP_neg)                        \
  __(OP_inc)                        \
  __(OP_dec)                        \
  __(OP_not)

/*
  __(OP_if_true)                        \
  __(OP_if_false)                       \
  __(OP_goto)
*/

namespace quake {
class Quake;

namespace ir {

class InterpInfo;

// Builder of Graph, ie invoked when trace is on for each bytecode seen. The
// builder finish its job when it sees a loopback instruction. Notes, builder
// is not aware of it and the caller should finish the tracing when all are
// done
class Builder {
 public:
  Builder(Quake*, Graph*);

 public:
  // Setup the trace, ie called it at very first
  TraceResult Setup(InterpInfo*);

  // Trace a single instructions
  TraceResult Trace(InterpInfo*);

  // A trace is done, we should just stop it
  TraceResult Stop(InterpInfo*);

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

  std::pair<TraceResult, SSAVar*> ParamAt(InterpInfo*, int);
  TraceResult ParamSet(InterpInfo*, int, SSAVar*);
  void RcTryDecParam(InterpInfo*, int);

  void UpdateCheckpoint(InterpInfo*, Checkpoint::Reason, void*, Node*);
  void UpdateCheckpointAfter(InterpInfo*, Checkpoint::Reason, Node*);
  void AddCheckpointAfter(Checkpoint::Reason, void*, Node*);
  void UpdatePendingCheckpoint(InterpInfo*);
  void OnGuard(Node*);
  void OnGuard(SSAVar*);

  void RcInc(InterpInfo*, SSAVar*);
  void RcDec(InterpInfo*, SSAVar*);
  void RcTryInc(InterpInfo*, SSAVar*);
  void RcTryDec(InterpInfo*, SSAVar*);
  bool FlushRC(InterpInfo*);

  void PushSSA(InterpInfo*, SSAVar*);

  TraceResult PushJVal(InterpInfo*, JVal);
  std::pair<TraceResult, SSAVar*> CheckZeroInt(InterpInfo*, SSAVar*);

  std::pair<TraceResult, SSAVar*> Unbox(InterpInfo*, JVal, SSAVar*);
  std::pair<TraceResult, SSAVar*> Box(InterpInfo*, SSAVar*);

  std::pair<TraceResult, SSAVar*> DoArith(InterpInfo*, Opcode, Opcode, SSAVar*,
                                          SSAVar*);
  std::pair<TraceResult, SSAVar*> DoComp(InterpInfo*, Opcode, Opcode, SSAVar*,
                                         SSAVar*);

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

  // RC handling
  zone::Vector<SSAVar*> rc_queue_;

  bool rc_flushed_;  // will be reset for each bytecode handling, used to
                     // perform sanity check during bytecode trace. The
                     // assumption is you cannot perform GUARD(which jumps out
                     // to a checkpoint) after rc flush in the middle of a
                     // bytecode handler.

  // Checkpoint
  Checkpoint* checkpoint_;  // current checkpoint, always none null

  bool checkpoint_done_;  // whether a effect checkpoint has already been
                          // generated or not

  struct PendingCheckpoint {
    Checkpoint::Reason reason;
    void* resumed_bc;
    Node* trigger_node;
    PendingCheckpoint(Checkpoint::Reason r, void* b, Node* tnode)
        : reason(r), resumed_bc(b), trigger_node(tnode) {}
  };

  std::optional<PendingCheckpoint>
      pending_checkpoint_;  // whether need another checkpoint after the handler
                            // is done. Notes, user should NOT generate
                            // checkpoint in the middle of bytecode generation.
                            // And one bytecode can just generate one checkpoint

  // Trace info
  InterpInfo* interp_info_;  // current interp_info_, mutated for every trace
  Bytecode last_bc_;  // previous bytecode, if nothing Bytecode::OP_invalid

  // Other information
  std::uint32_t param_size_;
  std::uint32_t local_size_;

  zone::Vector<SSAVar*> storage_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_BUILDER_H_
