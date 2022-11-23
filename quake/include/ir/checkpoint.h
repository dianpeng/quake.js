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

    kException,  // exception checkpoint, ie there will be an exception handler
                 // followed by this checkpoint and it is not valid to be used
                 // jump to any other position except to a exception handler.
                 // Notes it is not a valid snapshot of a completely executed
                 // VM bytecode handler

    kRefCount,  // reference counting node

    kControlFlow,  // because of control flow node
    kBB1,  // special control flow node, the checkpoint will be added after all
           // the loop phi been inserted
    kStart,  // start of the trace
    kEnd,    // end of the trace

    kSizeOfReason
  };

  enum Type {
    kLocal,  // local variables
    kParam,  // argument
    kTemp,   // temporary

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
  };

  class Entry {
   public:
    Entry(Checkpoint* ck, std::uint32_t i) : checkpoint_(ck), index_(i) {}

    // NOTES(dpeng): This this function to correctly set the value into the
    // entry, otherwise the def-use information is not maintained correctly
    void Set(EntryType, SSAVar*, const Origin&);

    // set a frame value entry. Notes we need it reserve its reference to make
    // frame instruction live through DCE phase
    void Set(SSAVar* var);
    void SetValue(SSAVar*);
    void Reset();
    bool IsEmpty() const { return value_ == nullptr; }
    EntryType type() const { return type_; }
    const char* type_name() const { return GetEntryTypeName(type()); }
    bool IsValue() const { return type_ == kVal; }
    bool IsFrame() const { return type_ == kFrame; }

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
    void PushBack(SSAVar*);
    void PopBack();
    void Clear();
    void Reserve(std::size_t);

   private:
    Checkpoint* checkpoint_;  // belonged checkpoint
    friend class Checkpoint;
  };

  struct Frame {
    JitFrameType type;       // type of the frame
    EntryVector value_list;  // all the value list been captured

    // all the other informations
    const void* cur_bc;
    const void* next_bc;

    // other information needed to recover the frame if needed
    std::size_t slot_size;
    std::size_t local_size;
    std::size_t param_size;

    SSAVar* frame_instruction = nullptr;  // if inline/call frame, the
                                          // corresponding instruction create
                                          // the frame

    Frame(ir::Checkpoint* ck, zone::Zone* zone, JitFrameType t, const void* cbc,
          const void* nbc, std::size_t ss, std::size_t ls, std::size_t ps,
          SSAVar* fins)
        : type(t),
          value_list(ck, zone),
          cur_bc(cbc),
          next_bc(nbc),
          slot_size(ss),
          local_size(ls),
          param_size(ps),
          frame_instruction(fins) {}

   public:
    std::string Print() const;
    std::string PrintGraphLink(const std::string&) const;
  };

  using FrameVector = zone::Vector<Frame>;

 public:
  Checkpoint(Quake*, CheckpointId, Reason, Node*, bool);

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

 public:
  const FrameVector& frame_vector() const { return frame_vector_; }
  FrameVector& frame_vector() { return frame_vector_; }
  void ForeachFrameReversely(std::function<void(const Frame&)>&&);

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
  std::size_t TotalEntrySize() const;
  std::size_t ForeachEntry(std::function<void(const Entry&)>&&) const;
  std::size_t ForeachEntry(std::function<void(Entry&)>&&);

 public:
  const void* TopFrameBCPos() const;

  std::size_t ForeachTopFrame(std::function<void(const Entry&)>&&) const;
  std::size_t TopFrameSize() const;

 public:
  std::string Print() const override;
  std::string PrintGraphName() const override;
  std::string PrintGraphNode() const override;
  std::string PrintGraphLink() const override;

 public:
  void set_deopt_id(DeoptId did) {
    CHECK(deopt_id_ == kInvalidDeoptId);
    deopt_id_ = did;
  }
  DeoptId deopt_id() const { return deopt_id_; }
  bool HasDeoptId() const { return deopt_id_ != kInvalidDeoptId; }

  bool happen_before() const { return happen_before_; }
  bool happen_after() const { return !happen_before_; }

 private:
  // relink the checkpoint to a new generated node. Notes this will need to
  // happen after the snapshot join(merge)
  void Relink(Node*, Reason, bool is_after);
  void SetSnapshot(Node*, bool);

 private:
  Quake* quake_;
  CheckpointId id_;   // id
  DeoptId deopt_id_;  // deopt_id_, set up when the deopt mapping is created
  SSAFlag flag_;      // general flags
  Reason reason_;     // why this checkpoint is been generated

  Node* node_;  // which node generate such checkpoint. Notes the strict
                // order between checkpoint and node is based on reason

  CheckpointExit exit_;  // how to exit the checkpoint ?
  zone::Vector<Node*> jump_from_;

  // All the frame captured by this checkpoint
  FrameVector frame_vector_;

  // whether the checkpoint happens before or after the specific instruction
  bool happen_before_;

  DISALLOW_COPY_AND_ASSIGN(Checkpoint);
  friend class Graph;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_CHECKPOINT_H_
