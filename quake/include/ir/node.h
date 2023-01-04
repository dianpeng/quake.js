#ifndef _QUAKE_IR_NODE_H_
#define _QUAKE_IR_NODE_H_

#include "base.h"

#include "ir/ssa.h"
#include "ir/interp-info.h"
#include "ir/opcode.h"
#include "stringify.h"

namespace quake {
namespace ir {

class Checkpoint;
class SSAVar;
class Graph;

using NodeId = std::uint32_t;

template <typename T>
class NodeLink {
 public:
  inline void InsertAfter(NodeLink*);
  inline void InsertBefore(NodeLink*);
  inline void RemoveSelf();
  void SelfWire() { prev_ = next_ = this; }
  void NullWire() { prev_ = next_ = nullptr; }
  bool IsNullWire() const { return prev_ == nullptr && next_ == nullptr; }

  T* next() const { return static_cast<T*>(next_); }
  T* prev() const { return static_cast<T*>(prev_); }

 private:
  NodeLink* prev_ = nullptr;
  NodeLink* next_ = nullptr;
};

class Node : public NodeBaseImpl<NodeBase::kNode>,
             public NodeLink<Node>,
             public BytecodeInfoProvider,
             public Stringify,
             public GraphvizNode {
 public:
  explicit Node(Quake*);

  // --------------------------------------------------------------------------
  Node(Quake*, const BytecodeInfo&, Opcode);
  Node(Quake*, const BytecodeInfo&, Opcode, SSAVar*);
  Node(Quake*, const BytecodeInfo&, Opcode, SSAVar*, SSAVar*);

  Node(Quake*, const BytecodeInfo&, Opcode, const Type&);
  Node(Quake*, const BytecodeInfo&, Opcode, const Type&, SSAVar*);
  Node(Quake*, const BytecodeInfo&, Opcode, const Type&, SSAVar*, SSAVar*);

  // --------------------------------------------------------------------------
  // Used by kNArity node, which has dynamic size until we learn it. Notes
  // after initialization user can *INDEX* specific position of node's operand
  // but will just return nullptr
  Node(Quake*, const BytecodeInfo&, Opcode, std::size_t actual_size);
  Node(Quake*, const BytecodeInfo&, Opcode, const Type&,
       std::size_t actual_size);

  // --------------------------------------------------------------------------
  // Primitive literal nodes
  Node(Quake*, const BytecodeInfo&, std::int32_t);
  Node(Quake*, const BytecodeInfo&, std::int64_t);
  Node(Quake*, const BytecodeInfo&, double);
  Node(Quake*, const BytecodeInfo&, bool);
  Node(Quake*, const BytecodeInfo&, const std::string_view&);

  struct Null {};
  struct Undefined {};
  struct Uninit {};

  Node(Quake*, const BytecodeInfo&, Null&&);
  Node(Quake*, const BytecodeInfo&, Undefined&&);
  Node(Quake*, const BytecodeInfo&, Uninit&&);

 public:
  // Primitive literal nodes
  std::int32_t Int32() const {
    CHECK(opcode_ == Opcode::kKI32);
    return operand_.i32_value;
  }
  std::uint32_t UInt32() const { return static_cast<std::uint32_t>(Int32()); }

  std::int64_t Int64() const {
    CHECK(opcode_ == Opcode::kKI64);
    return operand_.i64_value;
  }
  std::uint64_t UInt64() const { return static_cast<std::uint64_t>(Int64()); }

  std::int64_t ToInt() const {
    if (opcode() == Opcode::kKI32) {
      return static_cast<std::int64_t>(Int32());
    } else {
      return Int64();
    }
  }

  bool IsIntZero() const { return ToInt() == 0; }
  bool IsIntOne() const { return ToInt() == 1; }
  bool IsIntNegOne() const { return ToInt() == -1; }

  double Float64() const {
    CHECK(opcode_ == Opcode::kKFloat64);
    return operand_.f64_value;
  }

  double ToFloat64() const {
    if (opcode() == Opcode::kKI32) return static_cast<double>(Int32());
    if (opcode() == Opcode::kKI64) return static_cast<double>(Int64());
    return Float64();
  }

  bool IsFloatZero() const { return ToFloat64() == 0.0; }

  bool Bool() const {
    CHECK(opcode_ == Opcode::kKBool);
    return operand_.b_value;
  }

  bool IsUndefined() const { return opcode_ == Opcode::kKUndef; }
  bool IsNull() const { return opcode_ == Opcode::kKNull; }
  bool IsUninit() const { return opcode_ == Opcode::kKUninit; }

  // string literals
  bool IsStr() const { return opcode_ == Opcode::kLoadStr; }
  const zone::String& Str() const { return *operand_.str; }

  // convert the constant node to boolean
  inline bool ConstToBool() const;

 public:
  // General access of internal operands
  SSAVar* At(int) const;

  // Convert certain position's value into int32, otherwise crashed
  std::int32_t AtI32(int) const;
  std::int64_t AtI64(int) const;
  double AtF64(int) const;
  bool AtBool(int) const;

  // Reverse lookup, ie finding out the index for specified SSAVar, if not
  // used by this Node, returns -1
  int By(SSAVar*) const;

  // Set the SSAVar at specified index.
  void Set(int, SSAVar*);

  // Remove the operand. After removing it the original position will be left
  // with nullptr
  SSAVar* Remove(int);

  // Whether all the operand has been set to none-nullptr or not
  bool IsComplete() const;

  // Size of operands. Notes size() is different from arity()
  int size() const {
    return IsNary() ? static_cast<int>(operand_.opr_list->size())
                    : opcode_info().arity;
  }

  // Check whether this Node uses the SSAVar object specified
  bool Use(SSAVar* v) const { return By(v) != -1; }

  // For unary access
  SSAVar* Opr() const { return At(0); }

  // For binary access
  SSAVar* Lhs() const { return At(0); }
  SSAVar* Rhs() const { return At(1); }

  // If this node produces a value, out() returns the SSAVar it belongs to
  SSAVar* out() const { return out_; }
  bool HasOut() const { return out_ != nullptr; }

 public:
  // Replace the current node' usage with specified node. Notes, after replacing
  // all the reference to this node will be replaced with specified one
  void Replace(SSAVar*);
  void Replace(Node*);
  void ReplaceAndSetDead(SSAVar* v) {
    Replace(v);
    MarkDead();
  }
  void ReplaceAndSetDead(Node* n) {
    Replace(n);
    MarkDead();
  }
  void Remove();

 public:
  Opcode opcode() const { return opcode_; }
  const std::string& opcode_name() const { return opcode_info().name; }

  int arity() const { return opcode_info().arity; }
  bool IsNary() const { return opcode_info().IsNary(); }

  const OpcodeInfo& opcode_info() const { return *opcode_info_; }
  const IREffect& ir_effect() const { return opcode_info_->effect; }
  const Type& type() const { return type_; }
  void set_type(const Type& t) { type_ = t; }

 public:
  bool IsSideEffect() const { return opcode_info().effect.IsSideEffect(); }
  bool IsValue() const { return opcode_info().effect.IsValue(); }
  bool IsOp() const { return opcode_info().effect.IsOp(); }
  bool IsControlFlow() const { return opcode_info().effect.IsControlFlow(); }
  bool IsGuard() const { return opcode_info().effect.IsGuard(); }
  bool IsException() const { return opcode_info().effect.IsException(); }
  bool IsConst() const { return OpcodeIsConst(opcode()); }
  bool IsConstInt() const {
    return opcode() == Opcode::kKI32 || opcode() == Opcode::kKI64;
  }
  bool IsConstFloat() const { return opcode() == Opcode::kKFloat64; }

  bool IsPhi() const { return OpcodeIsPhi(opcode()); }

  // Whether both operand are the same operands. A trivial phi is placed just
  // for RA phase, semantically it is just a invariant.
  bool IsTrivialPhi() const { return IsPhi() && Lhs() == Rhs(); }
  bool IsNoneTrivialPhi() const { return IsPhi() && Lhs() != Rhs(); }

  bool IsJump() const { return OpcodeIsJump(opcode()); }

  bool IsInternal() const {
    if (internal_) {
      CHECK(opcode_info().effect.IsInternal());
      return true;
    }
    return false;
  }

  // Whether the node is not been inserted into the graph or can be inserted
  // into the graph or not.
  bool IsUnbounded() const { return !IsInternal() && IsNullWire(); }

  // Only used when the node is a phi node and all its operand is complete
  bool InferPhiType();

 public:
  const SSAFlag& flag() const { return flag_; }
  SSAFlag& flag() { return flag_; }

  void SetDead();
  bool IsDead() const;

 public:
  enum { kSnapBefore, kSnapAfter };

  Checkpoint* snapshot_before() const { return snapshot_before_; }
  Checkpoint* snapshot_after() const { return snapshot_after_; }
  bool HasSnapshotBefore() const { return snapshot_before_ != nullptr; }
  bool HasSnapshotAfter() const { return snapshot_after_ != nullptr; }
  bool HasSnapshot() const {
    return snapshot_before_ != nullptr || snapshot_after_ != nullptr;
  }

  bool RemoveSnapshot(Checkpoint*);

  // Make this node jump to this checkpoint if this node requires a bailout,
  // ie it is a GUARD node
  void BailoutTo(Checkpoint*);

  // exit returns the bailout node. BailoutTo is really a set_exit with some
  // more book-keeping information
  Checkpoint* exit() const { return exit_; }
  Checkpoint* exit_checked() const {
    CHECK(exit_);
    return exit_;
  }
  bool HasExit() const { return exit_ != nullptr; }

  // In case, you want to clear yourself from the checkpoint
  void RemoveExit();

 public:
  NodeId node_id() const { return node_id_; }

  BytecodeInfo bytecode_info() const override { return bytecode_info_; }

 public:
  // Stringify
  std::string Print() const override;
  std::string PrintRefId() const;

  // Used to print the node as constant. Make sure the node is a constant,
  // ie IsConst returns true, otherwise crash
  std::string PrintConst() const;

  // Graphviz
  std::string PrintGraphName() const override;
  std::string PrintGraphNode() const override;
  std::string PrintGraphLink() const override;

 private:
  void set_chain(Node* next) { chain_ = next; }
  Node* chain() const { return chain_; }
  SSAVar* DoSet(int, SSAVar*);
  void MarkDead();

 private:
  Quake* quake_;
  Opcode opcode_;
  Node* chain_;

  SSAFlag flag_;
  const OpcodeInfo* opcode_info_;

  Type type_;  // mostly this field is same as Opcode's type and it will be
               // overwrite if user specified the type for certain opcode, ie
               // when opcode's type is kInfer

  SSAVar* out_;

  union {
    // Unary/Binary node's storage.
    struct {
      SSAVar* opr1;
      SSAVar* opr2;
    };

    // Narity
    zone::Vector<SSAVar*>* opr_list;

    // literal types value
    std::int32_t i32_value;
    std::int64_t i64_value;
    double f64_value;
    bool b_value;
    zone::String* str;
  } operand_;

  Checkpoint* exit_;
  Checkpoint* snapshot_before_;
  Checkpoint* snapshot_after_;

  NodeId node_id_;

  BytecodeInfo bytecode_info_;
  MCodeInfo mcode_info_;

  bool internal_ = false;

  DISALLOW_COPY_AND_ASSIGN(Node);

  friend class Graph;
  friend class Checkpoint;
  friend class SSAVar;
};

template <typename T>
inline void NodeLink<T>::InsertAfter(NodeLink* after) {
  RemoveSelf();

  auto old_next = after ? after->next_ : nullptr;
  prev_ = after;
  if (after) after->next_ = this;
  next_ = old_next;
  if (old_next) old_next->prev_ = this;
}

template <typename T>
inline void NodeLink<T>::InsertBefore(NodeLink* before) {
  RemoveSelf();

  auto old_prev = before ? before->prev_ : nullptr;
  next_ = before;
  if (before) before->prev_ = this;
  prev_ = old_prev;
  if (old_prev) old_prev->next_ = this;
}

template <typename T>
inline void NodeLink<T>::RemoveSelf() {
  if (prev_) prev_->next_ = next_;
  if (next_) next_->prev_ = prev_;
  prev_ = next_ = nullptr;
}

inline bool Node::ConstToBool() const {
  switch (opcode_) {
    case Opcode::kKI32:
      return Int32() != 0;
    case Opcode::kKI64:
      return Int64() != 0;
    case Opcode::kKFloat64:
      return Float64() != 0.0;
    case Opcode::kKBool:
      return Bool();

    case Opcode::kKNull:
    case Opcode::kKUndef:
    case Opcode::kKUninit:
      return false;

    case Opcode::kLoadStr:
      return !Str().empty();

    default:
      UNREACHABLE();
      return false;
  }
}

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_NODE_H_
