#ifndef _QUAKE_IR_CHECKPOINT_H_
#define _QUAKE_IR_CHECKPOINT_H_

#include "base.h"
#include "zone.h"
#include "quake.h"

#include "ir/ssa.h"

namespace quake {
namespace ir {
class InterpInfo;

class SSAVar;
class Node;
class Graph;
class Builder;

class CheckpointExit {
 public:
  enum Link {
    kLinkNone,    // undecided yet, nothing
    kLinkRoot,    // link to another root, ie parallel loop
    kLinkTrace,   // link to another trace
    kLinkInterp,  // resume from interpreter
    kLinkReturn,  // return to interpreter
    kSizeOfLink
  };

 public:
  bool IsUsed() const { return link_ != kLinkNone; }
  Link link() const { return link_; }
  void set_link(Link l) { link_ = l; }

 private:
  Link link_ = kLinkNone;
};

class Checkpoint {
 public:
  enum Reason {
    kSideEffect,  // side effect checkpoint, checkpoint is generated after
                  // instruction is done.

    kRefCount,  // reference counting node

    // general control flow node
    kControlFlow,  // because of control flow node

    kStart,  // start of the trace
    kEnd,    // end of the trace

    kSizeOfReason
  };

  enum Type {
    kLocal,  // local variables
    kParam,  // argument
    kTemp,   // temporary
    kBase,   // general stack base, ie from interpreter view

    kSizeOfType
  };

  enum EntryType {
    kVal,    // value
    kFrame,  // frame

    kSizeOfEntryType
  };

  struct Origin {
    Type type;
    int offset;  // relative offset, biased on Type

    static Origin FromLocal(int offset) { return Origin{kLocal, offset}; }
    static Origin FromParam(int offset) { return Origin{kParam, offset}; }
    static Origin FromTemp(int offset) { return Origin{kTemp, offset}; }
    static Origin FromBase(int offset) { return Origin{kBase, offset}; }
  };

  struct Entry {
    EntryType type;

    SSAVar* value;  // the value slot in JIT frame. Notes, SSAVar contains
                    // RA information, which allow us to store it back
    Origin origin;  // origin of the value, ie how to flush it back

    Entry(EntryType t, SSAVar* v, Origin&& o)
        : type(t), value(v), origin(std::move(o)) {}
  };

  using EntryVector = zone::Vector<Entry>;

 public:
  Checkpoint(Quake*, Reason, Node*, const void*);

 public:
  // Check whether the specified Checkpoint, which happened before |this|
  // can be merged against or not.
  bool CanMerge(Checkpoint::Reason current_reason) const;

  // Take a snapshot of the current status, optionally perform a optimization
  void Snapshot(Graph*, Builder*);

 public:
  // CheckpointExit handling. Each checkpoint is generated on the fly during the
  // IR construction and it may not be used afterwards. A use of checkpoint
  // means it tries to assign an exit to the checkpoint. When merge 2
  // checkpoints we can only merge 2 checkpoint with same exit.
  const CheckpointExit& exit() const { return exit_; }

 public:
  Quake* quake() const { return quake_; }

  // reason of checkpoint
  Reason reason() const { return reason_; }

  // what node generate this checkpoint
  Node* node() const { return node_; }

  // all the snapshot happened inside of the checkpoint
  const EntryVector& entry_vector() const { return entry_vector_; }

  // used to patch the value inside of the re-emitting of checkpoint
  EntryVector& entry_vector() { return entry_vector_; }

  const void* bc_pos() const { return bc_pos_; }

 public:
  const zone::Vector<Node*>& jump_from() const { return jump_from_; }
  void JumpFrom(Node* n) { jump_from_.push_back(n); }
  bool HasJumpFrom() const { return !jump_from_.empty(); }
  bool RemoveJumpFrom(Node*);
  bool ReplaceJumpFrom(Node*, Node*);

 public:
  SSAFlag flag() const { return flag_; }
  void set_flag(SSAFlag f) { flag_ = f; }
  void SetDead() { flag_.SetDead(); }
  bool IsDead() const { return flag_.IsDead(); }

 private:
  Quake* quake_;
  SSAFlag flag_;   // general flags
  Reason reason_;  // why this checkpoint is been generated
  Node* node_;     // which node generate such checkpoint. Notes the strict
                   // order between checkpoint and node is based on reason

  const void*
      bc_pos_;  // the VM interpreter's bc marker, ie any side effect that
                // happens *BEFORE* the bc_pos_ has been snapshotted, and
                // when we bailout from JIT frame, we should recover to the
                // specified position. Notes checkpoint may not lead to a
                // return to interpreter (RTI) operation, but just bailout

  CheckpointExit exit_;  // how to exit the checkpoint ?
  zone::Vector<Node*> jump_from_;

  // Data inside of the checkpoint. Later use them to help recover from JIT
  // frame
  EntryVector entry_vector_;

  DISALLOW_COPY_AND_ASSIGN(Checkpoint);
  friend class Graph;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_CHECKPOINT_H_
