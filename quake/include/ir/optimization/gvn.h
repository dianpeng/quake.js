#ifndef _QUAKE_IR_OPTIMIZATION_GVN_H_
#define _QUAKE_IR_OPTIMIZATION_GVN_H_
#include "base.h"
#include "quake.h"

#include "ir/type.h"
#include "ir/pass.h"
#include "ir/graph.h"
#include "ir/ssa-var.h"
#include "ir/node.h"

// ----------------------------------------------------------------------------
// Global Value Numbering
//
// Traditionaly CSE pass and try to pick up those terms(node) which has already
// been computed and dedup them. This pass is relative effective since we do
// loop peeling during IR emittion. Basically, a loop body as following :
//
//   for (let i = 0; i < 10; i++) {
//     ... loop body ...
//   }
//
// will be translated into following:
//
//  [BB0]:
//     ... loop body ...
//
//  [BB1]:
//     i++;
//     if (i >= 10) bailout();
//     ... loop body ...
//     goto #BB1;
//
// Obviously BB0 dom BB1.
//
// Therefore, if any expressions are duplicated inside of the loop body, the
// GVN will perform implicit loop invariant code motion, due to the loop peeling
//
// I believe this trick is invented by author of LuaJIT and has been used inside
// of PyPy JIT compiler.
// ----------------------------------------------------------------------------

namespace quake {
namespace ir {
namespace _ {

// ----------------------------------------------------------------------------
//
// Node equalism comparison related functions

inline bool GVNNodeEqual(const Node&, const Node&);
inline bool GVNVarEqual(const SSAVar&, const SSAVar&);

inline bool GVNNodeIsAllowed(const Node& n) {
  if (n.IsSideEffect()) return false;
  if (n.IsInternal()) return false;
  if (n.IsControlFlow()) return false;
  if (n.IsException()) return false;
  if (n.IsPhi()) return false;

  // For any type cannot be represented by PHI node, we cannot dedup it since
  // later phase's RA cannot handle it correctly
  if (!IsAllowedPhiType(n.type())) return false;

  return true;
}

inline bool GVNVarEqual(const SSAVar& lhs, const SSAVar& rhs) {
  CHECK(!lhs.IsDead());
  CHECK(!rhs.IsDead());
  if (&lhs == &rhs) return true;  // alias

  auto lhs_node = lhs.node_checked();
  auto rhs_node = rhs.node_checked();

  return GVNNodeEqual(*lhs_node, *rhs_node);
}

inline bool GVNNodeEqual(const Node& lhs, const Node& rhs) {
  if (&lhs == &rhs) return true;  // alias

  // side effect node will *never* be equaled to any other node
  if (!GVNNodeIsAllowed(lhs)) return false;
  if (!GVNNodeIsAllowed(rhs)) return false;

  if (lhs.opcode() != rhs.opcode()) return false;  // if opcode not match
  if (lhs.size() != rhs.size()) return false;  // if operands arity not match

  const auto len = lhs.size();

  // recursively check whether all the operands are the same or not.
  for (auto i = 0; i < len; i++) {
    if (!GVNVarEqual(*lhs.At(i), *rhs.At(i))) return false;
  }

  return true;
}

// ----------------------------------------------------------------------------
//
// Node hash generation related stuff

inline std::string GVNNodeToStr(const Node& that);

inline std::string GVNVarToStr(const SSAVar& that) {
  CHECK(!that.IsDead());
  auto node = that.node_checked();
  return GVNNodeToStr(*node);
}

inline std::string GVNNodeToStr(const Node& that) {
  if (!GVNNodeIsAllowed(that)) {
    // side effect node, its hash should be unqiue
    return base::Fmt("<%p>", &that);
  }

  // Do recursive hash generation here, make sure if 2 nodes are equal by
  // above equalism operation, they are guaranteed to generate same hash
  //
  // For simplicity, we just use string based hash generation. Better hasher
  // should be implemented in the future if we want to.
  std::stringstream fmt;

  fmt << static_cast<int>(that.opcode()) << ":";  // opcode are numbers
  fmt << "[" << that.size() << "]";               // arity
  const auto len = that.size();

  for (auto i = 0; i < len; i++) {
    fmt << GVNVarToStr(*that.At(i));  // each operand
  }

  // Generate hash
  return fmt.str();
}

inline std::size_t GVNNodeHash(const Node& that) {
  return std::hash<std::string>{}(GVNNodeToStr(that));
}

// ----------------------------------------------------------------------------
// To allow GVN to work, we need to use a hash map to guide the *numbering*
// process. The graph contains node, and all we need to do is use node. Notes
// for any side effect related node or internal node, we simply ignore them
// during GVN phase, ie they are never duplicated.
// ----------------------------------------------------------------------------

struct GVNNode {
  Node* node;  // actual graph node

  explicit GVNNode(Node* n) : node(n) { CHECK(GVNNodeIsAllowed(*n)); }

  bool operator==(const GVNNode& that) const {
    CHECK(GVNNodeIsAllowed(*that.node));
    return GVNNodeEqual(*node, *that.node);
  }
};

}  // namespace _
}  // namespace ir
}  // namespace quake

// ----------------------------------------------------------------------------
// Expose hash
namespace std {

template <>
struct hash<quake::ir::_::GVNNode> {
  std::size_t operator()(const quake::ir::_::GVNNode& n) const {
    return static_cast<std::size_t>(n.node->flag().GetExtraU64());
  }
};

}  // namespace std

namespace quake {
namespace ir {

// ----------------------------------------------------------------------------
// Global Value Numbering pass
//
// FIXME(dpeng): Optimize the hash thing

class GlobalValueNumbering : public Pass {
 public:
  explicit GlobalValueNumbering(Quake* q) : quake_(q){};

  // Make sure that all the ssa_flag extra field is been reset to 0
  bool Setup(Graph* g) override;
  Result On(Graph*, Node*) override;
  std::string GetName() const override { return "gvn"; }

 private:
  using GVNMap = std::unordered_set<quake::ir::_::GVNNode>;

  // Since we allow *inline*, we may encounter multiple different function frame
  // during the GVN process. Each inline frame, we may have a peeled loop BB or
  // not, that's fine since our GVN can handle both. But we *cannot* GVN the
  // expression that is not inside of the same Frame, ie do not cross function
  // boundary;
  struct Frame {
    GVNMap map;
  };

  // Try to calcuate node's GVN hash if needed. The node
  void MaybeCalculateHash(Node*);

  // Try to dedup this node, if we can find a replacement, which is not the
  // input, then it returns that node, otherwise a nullptr is returned. This
  // function also marks the node if it can be marked for future dedup.
  Node* TryDedup(Node*);

  // Try to find out the whether 2 nodes are in the same BB or not. Otherwise,
  // due to the loop peeling, the node may require extra LoopPhi to make sure
  // its LIR code generation and register allocation correct
  Node* MaybeMarkLoopPhi(Graph*, Node*, Node*);
  Node* CheckTrivialPhi(Graph*, Node*, Node*);

 private:
  Frame& current() { return frame_.back(); }
  const Frame& current() const { return frame_.back(); }

 private:
  Quake* quake_;

  // The iteration of the Graph is forward order, ie linearly go through each
  // IR until we finish. Therefore, we will encounter nested inline frame one
  // by one and then pop them one by one. Therefore, maintaining a frame list
  // and use it as a stack is good enough
  std::vector<Frame> frame_;

  // A bit vector to indicate whether each node has its hash been generated or
  // not
  std::vector<bool> has_hash_;
  std::vector<Node*> phi_map_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_OPTIMIZATION_GVN_H_
