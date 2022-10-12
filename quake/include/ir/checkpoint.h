#ifndef _QUAKE_IR_CHECKPOINT_H_
#define _QUAKE_IR_CHECKPOINT_H_

#include "base.h"
#include "zone.h"
#include "quake.h"
#include "stringify.h"

#include "ir/ssa.h"

namespace quake {
namespace ir {
class InterpInfo;

class SSAVar;
class Node;
class Graph;
class Builder;

using CheckpointId = std::uint32_t;

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

class Checkpoint : public NodeBaseImpl<NodeBase::kCheckpoint>,
                   public Stringify,
                   public GraphvizNode {
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

  static const char* GetReasonName(Reason);
  static const char* GetTypeName(Type);
  static const char* GetEntryTypeName(EntryType);

  struct Origin {
    Type type;
    int offset;  // relative offset, biased on Type

    std::string Print() const;

    static Origin FromLocal(int offset) { return Origin{kLocal, offset}; }
    static Origin FromParam(int offset) { return Origin{kParam, offset}; }
    static Origin FromTemp(int offset) { return Origin{kTemp, offset}; }
    static Origin FromBase(int offset) { return Origin{kBase, offset}; }
  };

  class Entry {
   public:
    Entry(Checkpoint* ck, std::uint32_t i) : checkpoint_(ck), index_(i) {}

    // NOTES(dpeng): This this function to correctly set the value into the
    // entry, otherwise the def-use information is not maintained correctly
    void Set(EntryType, SSAVar*, const Origin&);
    void SetValue(SSAVar*);
    void Reset();
    bool IsEmpty() const { return value_ == nullptr; }

    EntryType type() const { return type_; }
    const char* type_name() const { return GetEntryTypeName(type()); }
    SSAVar* value() const { return value_; }
    const Origin& origin() const { return origin_; }

    std::string Print() const;

   private:
    // Used to track the location etc ..
    Checkpoint* checkpoint_;
    const std::uint32_t index_;

   private:
    EntryType type_;
    SSAVar* value_ =
        nullptr;     // the value slot in JIT frame. Notes, SSAVar contains
                     // RA information, which allow us to store it back
    Origin origin_;  // origin of the value, ie how to flush it back
  };

  class EntryVector : private zone::Vector<Entry> {
    using Base = zone::Vector<Entry>;

   public:
    EntryVector(Checkpoint* ck, zone::Zone* z) : Base(z), checkpoint_(ck) {}
    ~EntryVector() { Clear(); }

    using Base::at;
    using Base::back;
    using Base::begin;
    using Base::empty;
    using Base::end;
    using Base::front;
    using Base::rbegin;
    using Base::rend;
    using Base::size;
    using Base::operator[];

   public:
    // All the mutator should be correct implemented
    Entry& PushBack();
    void PushBack(EntryType, SSAVar*, const Origin&);
    void PopBack();
    void Clear();

   private:
    Checkpoint* checkpoint_;  // belonged checkpoint
    friend class Checkpoint;
  };

 public:
  Checkpoint(Quake*, CheckpointId, Reason, Node*, const void*);

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
  const char* reason_name() const { return GetReasonName(reason_); }

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
  CheckpointId id() const { return id_; }

 public:
  std::string Print() const override;

  std::string PrintGraphName() const override;
  std::string PrintGraphNode() const override;
  std::string PrintGraphLink() const override;

 private:
  Quake* quake_;
  CheckpointId id_;  // id
  SSAFlag flag_;     // general flags
  Reason reason_;    // why this checkpoint is been generated

  Node* node_;  // which node generate such checkpoint. Notes the strict
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
