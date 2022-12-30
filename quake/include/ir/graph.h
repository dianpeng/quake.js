#ifndef _QUAKE_IR_GRAPH_H_
#define _QUAKE_IR_GRAPH_H_
#include "base.h"
#include "stringify.h"

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
class Graph : public Stringify, public Graphviz {
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
  SSAVar* KNaN(BytecodeInfoProvider* info) {
    return KFloat64(info, base::kDoubleNaN);
  }

  SSAVar* KBool(BytecodeInfoProvider*, bool);
  SSAVar* True(BytecodeInfoProvider* i) { return KBool(i, true); }
  SSAVar* False(BytecodeInfoProvider* i) { return KBool(i, false); }
  SSAVar* KNull(BytecodeInfoProvider*);
  SSAVar* KUndefined(BytecodeInfoProvider*);
  SSAVar* KUninit(BytecodeInfoProvider*);

  SSAVar* LoadStr(BytecodeInfoProvider*, const std::string_view&);

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

  Node* AddNode(BytecodeInfoProvider*, Opcode opcode, const Type&, std::size_t);
  Node* AddNode(BytecodeInfoProvider*, Opcode opcode, const Type&);
  Node* AddNode(BytecodeInfoProvider*, Opcode, const Type&, SSAVar*);
  Node* AddNode(BytecodeInfoProvider*, Opcode, const Type&, SSAVar*, SSAVar*);

  // Used to related a SSAVar into the node, notes node must be
  // value(expression)
  SSAVar* AddSSAVar(Node*);

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
  Checkpoint* AddCheckpointBefore(BytecodeInfoProvider*, Checkpoint*, Builder*,
                                  Checkpoint::Reason, Node*);

  Checkpoint* AddCheckpointAfter(BytecodeInfoProvider*, Checkpoint*, Builder*,
                                 Checkpoint::Reason, Node*);

  Checkpoint* ReemitCheckpointBefore(BytecodeInfoProvider* info,
                                     Checkpoint* prev, Builder* builder,
                                     Checkpoint::Reason reason, Node* node,
                                     Checkpoint::ExtraTempList&& tlist);

  Checkpoint* ReemitCheckpointAfter(BytecodeInfoProvider* info,
                                    Checkpoint* prev, Builder* builder,
                                    Checkpoint::Reason reason, Node* node,
                                    Checkpoint::ExtraTempList&& tlist);

 public:
  const std::vector<SSAVar*>& ssa_var() const { return ssa_var_; }
  const std::vector<Checkpoint*>& checkpoint() const { return checkpoint_; }
  const std::vector<Node*>& node() const { return node_; }
  zone::Zone* zone() const { return quake_->zone(); }

 public:
  void set_end_of_bb0(Node* bb0);
  void set_end_of_bb1(Node* bb1);

  Node* end_of_bb0() const { return end_of_bb0_; }
  Node* end_of_bb1() const { return end_of_bb1_; }

  // Visit each BB0 node
  void ForeachBB0(std::function<void(Node*)>&&) const;
  bool ForeachBB0Stoppable(std::function<bool(Node*)>&&) const;

  // Visit each BB1 node
  void ForeachBB1(std::function<void(Node*)>&&) const;
  bool ForeachBB1Stoppable(std::function<bool(Node*)>&&) const;

  // Visit all node, BB0 + BB1.
  void Foreach(std::function<void(Node*)>&&) const;
  bool ForeachStoppable(std::function<bool(Node*)>&&) const;

  // Visit all internal node
  void ForeachInternal(std::function<void(Node*)>&&) const;

  // Visit all checkpoint
  void ForeachCheckpoint(std::function<void(Checkpoint*)>&&) const;

  // Compact all the node into a vector and returns back. The returned vector
  // will not contain dead node
  std::vector<Node*> GetNodeList() const;

 public:
  std::string Print() const override;
  std::string PrintGraphviz() const override;

 private:
  Node* AfterAlloc(Node*);
  Node* AfterAllocInternal(Node*);

  template <typename F>
  Node* FindNode(Opcode kind, F&& func);
  void SetNodeId(Node*);

 private:
  Quake* quake_;
  NodeId node_id_;

  std::vector<SSAVar*> ssa_var_;         // expression/variable SSA
  std::vector<Checkpoint*> checkpoint_;  // all checkpoint
  std::vector<Node*> node_;              // all the node that is visible
  std::vector<Node*> internal_node_;     // all the internal node

  // Skip list, each category forms such list. Notes, each chained has a next
  // pointed to next node of same opcode
  Node* slist_[static_cast<std::size_t>(Opcode::kSizeOfOpcode)];

  Node* end_of_bb0_;
  Node* end_of_bb1_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_GRAPH_H_
