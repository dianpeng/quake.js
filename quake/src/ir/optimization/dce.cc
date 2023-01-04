#include "ir/optimization/dce.h"

namespace quake {
namespace ir {

DCE::Result DCE::TryDeopt(Graph* graph, Node* node) {
  auto new_deopt = [&]() -> Node* {
    auto deopt = graph->AddOp(node, Opcode::kDeopt);
    deopt->BailoutTo(node->exit_checked());
    return deopt;
  };

  switch (node->opcode()) {
    case Opcode::kGIsTrue:
      if (node->Opr()->node_checked()->IsConst()) {
        if (auto cond = node->Opr()->node_checked()->ConstToBool(); cond) {
          return Remove();
        } else {
          return Replace(new_deopt());
        }
      }
      break;

    case Opcode::kGIsFalse:
      if (node->Opr()->node_checked()->IsConst()) {
        if (auto cond = node->Opr()->node_checked()->ConstToBool(); !cond) {
          return Remove();
        } else {
          return Replace(new_deopt());
        }
      }
      break;

    case Opcode::kGIsNotUninitialized:
      if (node->Opr()->node_checked()->type().IsUnbox()) {
        // if the condition for GIsNotUninitialized is unboxed one, try to
        // get rid of it here, ie remove the GIsNotUninitialized node with
        // another node that is done
        if (node->Opr()->node_checked()->type().IsUninitialized()) {
          return Replace(new_deopt());
        } else {
          return Remove();
        }
      }
      break;

    case Opcode::kGIsUninitialized:
      if (node->Opr()->node_checked()->type().IsUnbox()) {
        if (node->Opr()->node_checked()->type().IsUninitialized()) {
          return Remove();
        } else {
          return Replace(new_deopt());
        }
      }
      break;

    default:
      break;
  }

  return NoChange();
}

DCE::Result DCE::On(Graph* g, Node* n) { return TryDeopt(g, n); }

}  // namespace ir
}  // namespace quake
