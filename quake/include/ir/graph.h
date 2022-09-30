#ifndef _QUAKE_IR_GRAPH_H_
#define _QUAKE_IR_GRAPH_H_
#include "base.h"

#include "ir/ssa.h"
#include "ir/opcode.h"
#include "ir/checkpoint.h"
#include "ir/node.h"
#include "ir/ssa-var.h"

namespace quake {
class Quake;

namespace ir {

// Graph object. Factory for the whole Node and also keeps internal reference
// for the generated IR graph. Essentially, the trace is been kept as a linear
// type with various external index.
class Graph {
 public:
  explicit Graph(Quake*);

 public:
  // Constant
  SSAVar* KInt(BytecodeInfoProvider* info, int value) {
    if constexpr (sizeof(int) <= 4) {
      return KI32(info, static_cast<std::int32_t>(value));
    } else {
      return KI64(info, static_cast<std::int64_t>(value));
    }
  }

  SSAVar* KI32(BytecodeInfoProvider*, std::int32_t);
  SSAVar* KI64(BytecodeInfoProvider*, std::int64_t);
  SSAVar* KFloat64(BytecodeInfoProvider*, double);
  SSAVar* KBool(BytecodeInfoProvider*, bool);
  SSAVar* KNull(BytecodeInfoProvider*);
  SSAVar* KUndefined(BytecodeInfoProvider*);

 public:
  // Special internal node
  //
  // The internal node is been allocated without letting it been exposed inside
  // of node list for later traversal
  Node* AddNodeInternal(BytecodeInfoProvider*, Opcode opcode, std::size_t);
  Node* AddNodeInternal(BytecodeInfoProvider*, Opcode opcode);
  Node* AddNodeInternal(BytecodeInfoProvider*, Opcode opcode, SSAVar*);
  Node* AddNodeInternal(BytecodeInfoProvider*, Opcode opcode, SSAVar*, SSAVar*);

  // Set internal node into a ssa slot. Notes, this ssa slot is been hidden as
  // well
  SSAVar* SetNodeInternalSSA(Node*);

 public:
  // Raw operation. Create node accordingly -----------------------------------
  Node* AddNode(BytecodeInfoProvider*, Opcode opcode, std::size_t);
  Node* AddNode(BytecodeInfoProvider*, Opcode opcode);
  Node* AddNode(BytecodeInfoProvider*, Opcode, SSAVar*);
  Node* AddNode(BytecodeInfoProvider*, Opcode, SSAVar*, SSAVar*);

  // Value operation ----------------------------------------------------------
  // the node will generate a value and allocated inside of the SSAVar
  SSAVar* AddValue(BytecodeInfoProvider*, Opcode, std::size_t);
  SSAVar* AddValue(BytecodeInfoProvider*, Opcode);
  SSAVar* AddValue(BytecodeInfoProvider*, Opcode, SSAVar*);
  SSAVar* AddValue(BytecodeInfoProvider*, Opcode, SSAVar*, SSAVar*);

  SSAVar* AddValue(BytecodeInfoProvider*, Opcode, const Type&, std::size_t);
  SSAVar* AddValue(BytecodeInfoProvider*, Opcode, const Type&);
  SSAVar* AddValue(BytecodeInfoProvider*, Opcode, const Type&, SSAVar*);
  SSAVar* AddValue(BytecodeInfoProvider*, Opcode, const Type&, SSAVar*,
                   SSAVar*);

  // Op operation -------------------------------------------------------------
  // Node just been statically placed into the tracking list
  Node* AddOp(BytecodeInfoProvider*, Opcode, std::size_t);
  Node* AddOp(BytecodeInfoProvider*, Opcode);
  Node* AddOp(BytecodeInfoProvider*, Opcode, SSAVar*);
  Node* AddOp(BytecodeInfoProvider*, Opcode, SSAVar*, SSAVar*);

  // Control Flow operation ---------------------------------------------------
  // Node must be control flow
  Node* AddControlFlow(BytecodeInfoProvider*, Opcode, SSAVar*);
  Node* AddControlFlow(BytecodeInfoProvider*, Opcode);

  // Checkpoint operation -----------------------------------------------------
  // Node must be control flow
  Checkpoint* AddCheckpoint(BytecodeInfoProvider*, Checkpoint*, Builder*,
                            Checkpoint::Reason, const void*, Node*);

 public:
  const zone::Vector<SSAVar*>& ssa_var() const { return ssa_var_; }
  const zone::Vector<Checkpoint*>& checkpoint() const { return checkpoint_; }
  const zone::Vector<Node*>& node() const { return node_; }
  zone::Zone* zone() const { return quake_->zone(); }

 private:
  SSAVar* AddSSAVar(Node*);
  Node* AfterAlloc(Node*);

  template <typename F>
  Node* FindNode(Opcode kind, F&& func);

 private:
  Quake* quake_;

  zone::Vector<SSAVar*> ssa_var_;  // expression/variable SSA
  zone::Vector<Checkpoint*> checkpoint_;
  zone::Vector<Node*> node_;

  // Skip list, each category forms such list. Notes, each chained has a next
  // pointed to next node of same opcode
  Node* slist_[static_cast<std::size_t>(Opcode::kSizeOfOpcode)];
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_GRAPH_H_
