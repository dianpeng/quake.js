#ifndef _QUAKE_IR_IR_EQUAL_UTIL_H_
#define _QUAKE_IR_IR_EQUAL_UTIL_H_
#include <base.h>

#include <ir/node.h>
#include <ir/ssa-var.h>
#include <ir/graph.h>

namespace quake {
namespace ir {
namespace _ {

// Semantic comparison of 2 graph. The comparison only consider following
// aspects of each node
//
// 1) Its opcode
// 2) Its argument list (visiting recursively)
// 3) Its node type (Internal or not)
//
// ----------------------------------------------------------------------------

bool NodeBasicEqual(const Node& lhs, const Node& rhs) {
  CHECK(lhs.IsComplete());
  CHECK(rhs.IsComplete());

  // Should have same opcode
  if (lhs.opcode() != rhs.opcode()) return false;

  // Should be of the same type
  if (lhs.IsInternal() ^ rhs.IsInternal()) return false;

  // Should have same operand
  if (lhs.size() != rhs.size()) return false;

  return true;
}

bool NodeSemanticEqual(const Node& lhs, const Node& rhs) {
  if (!NodeBasicEqual(lhs, rhs)) return false;

  for (int i = 0; i < lhs.size(); i++) {
    auto lhs_opr = lhs.At(i);
    auto rhs_opr = rhs.At(i);

    if (!NodeBasicEqual(*lhs_opr->node(), *rhs_opr->node())) return false;
  }

  return true;
}

}  // namespace _

bool GraphSemanticEqual(const Graph& lhs, const Graph& rhs) {
  auto lhs_vec = lhs.GetNodeList();
  auto rhs_vec = rhs.GetNodeList();

  if (lhs_vec.size() != rhs_vec.size()) return false;

  for (std::size_t i = 0; i < lhs_vec.size(); ++i) {
    auto lhs_node = lhs_vec[i];
    auto rhs_node = rhs_vec[i];

    if (!_::NodeSemanticEqual(*lhs_node, *rhs_node)) return false;
  }

  return true;
}

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_IR_EQUAL_UTIL_H_
