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

using NodeId = std::uint32_t;  // this is unqiue id related to each node, can
                               // be used as a tag/id/index to index into OOL
                               // storage. Notes, index_ cannot be used as this
                               // purpose since it is may not be unique, ie
                               // internal node share same index_

class Node : public NodeBaseImpl<NodeBase::kNode>,
             public BytecodeInfoProvider,
             public Stringify,
             public GraphvizNode {
 public:
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
  std::int32_t Int64() const {
    CHECK(opcode_ == Opcode::kKI64);
    return operand_.i64_value;
  }
  double Float64() const {
    CHECK(opcode_ == Opcode::kKFloat64);
    return operand_.f64_value;
  }
  bool Bool() const {
    CHECK(opcode_ == Opcode::kKBool);
    return operand_.b_value;
  }
  bool IsUndefined() const { return opcode_ == Opcode::kKUndef; }
  bool IsNull() const { return opcode_ == Opcode::kKNull; }
  bool IsUninit() const { return opcode_ == Opcode::kKUninit; }

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
  bool Use(SSAVar* v) { return By(v) != -1; }

  // For unary access
  SSAVar* Opr() { return At(0); }

  // For binary access
  SSAVar* Lhs() { return At(0); }
  SSAVar* Rhs() { return At(1); }

  // If this node produces a value, out() returns the SSAVar it belongs to
  SSAVar* out() { return out_; }
  bool HasOut() const { return out_ != nullptr; }

 public:
  // Replace the current node' usage with specified node. Notes, after replacing
  // all the reference to this node will be replaced with specified one
  void Replace(SSAVar*);
  void Replace(Node*);
  void ReplaceAndSetDead(SSAVar* v) {
    Replace(v);
    SetDead();
  }
  void ReplaceAndSetDead(Node* n) {
    Replace(n);
    SetDead();
  }

 public:
  Opcode opcode() const { return opcode_; }
  const std::string& opcode_name() const { return opcode_info().name; }

  int arity() const { return opcode_info().arity; }
  bool IsNary() const { return opcode_info().IsNary(); }

  const OpcodeInfo& opcode_info() const { return *opcode_info_; }
  const IREffect& ir_effect() const { return opcode_info_->effect; }
  const Type& type() const { return type_; }

 public:
  bool IsSideEffect() const { return opcode_info().effect.IsSideEffect(); }
  bool IsValue() const { return opcode_info().effect.IsValue(); }
  bool IsOp() const { return opcode_info().effect.IsOp(); }
  bool IsControlFlow() const { return opcode_info().effect.IsControlFlow(); }
  bool IsGuard() const { return opcode_info().effect.IsGuard(); }
  bool IsException() const { return opcode_info().effect.IsException(); }
  bool IsConst() const { return OpcodeIsConst(opcode()); }
  bool IsPhi() const { return OpcodeIsPhi(opcode()); }
  bool IsJump() const { return OpcodeIsJump(opcode()); }

  bool IsInternal() const {
    if (index() == kInternalIndex) {
      CHECK(opcode_info().effect.IsInternal());
      return true;
    }
    return false;
  }

  // Only used when the node is a phi node and all its operand is complete
  bool InferPhiType();

 public:
  const SSAFlag& flag() const { return flag_; }
  SSAFlag& flag() { return flag_; }

  void SetDead() { flag_.SetDead(); }
  bool IsDead() const { return flag_.IsDead(); }

 public:
  // Once instruction produce a snapshot, ie it is a instruction has side effect
  // 1) Visiable operation
  // 2) Control flow operation
  // this functino returns the snapshot happens *AFTER* the current node is
  // done
  Checkpoint* snapshot() const { return snapshot_; }
  bool HasSnapshot() const { return snapshot_ != nullptr; }

  // Make this node jump to this checkpoint if this node requires a bailout,
  // ie it is a GUARD node
  void BailoutTo(Checkpoint*);

  // exit returns the bailout node. BailoutTo is really a set_exit with some
  // more book-keeping information
  Checkpoint* exit() const { return exit_; }
  bool HasExit() const { return exit_ != nullptr; }

  // In case, you want to clear yourself from the checkpoint
  void RemoveExit();

 public:
  std::uint32_t index() const { return index_; }
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
  void set_chain(Node* next) { next_ = next; }
  Node* next() const { return next_; }
  SSAVar* DoSet(int, SSAVar*);

 private:
  Quake* quake_;
  Opcode opcode_;
  Node* next_;

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
  } operand_;

  Checkpoint* exit_;
  Checkpoint* snapshot_;

  std::uint32_t index_;
  NodeId node_id_;

  BytecodeInfo bytecode_info_;
  MCodeInfo mcode_info_;

  DISALLOW_COPY_AND_ASSIGN(Node);

  friend class Graph;
  friend class Checkpoint;
  friend class SSAVar;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_NODE_H_
