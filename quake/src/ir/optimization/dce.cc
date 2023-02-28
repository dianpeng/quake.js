#include "ir/optimization/dce.h"

namespace quake {
namespace ir {

DeadCodeElimination::Result DeadCodeElimination::Try(Graph* graph, Node* node) {
  auto new_deopt = [&]() -> Node* {
    auto deopt = graph->AddOp(node, Opcode::kDeopt);
    deopt->BailoutTo(node->exit_checked());
    return deopt;
  };

  switch (node->opcode()) {
    // Deoptimization ---------------------------------------------------------
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

    // Frame ------------------------------------------------------------------
    case Opcode::kCloseVarRef:
      if (node->Opr()->node_checked()->Int32() ==
          0) {  // if the function's upvalue is zero,
                // then no need to issue close var ref
        return Remove();
      } else {
        return NoChange();
      }

    default:
      break;
  }

  return NoChange();
}

DeadCodeElimination::Result DeadCodeElimination::TryGeneral(Graph* graph,
                                                            Node* node) {
  // If a node is a none side effect node, and it is not a OP but a value, and
  // it is not referenced by any other node, it can be considered as dead node
  if (!node->IsSideEffect() && node->IsValue()) {
    // Check wether it is been referenced by any other node or not
    if (!node->out()->HasUse()) {
      return Remove();
    }
  }

  return NoChange();
}

DeadCodeElimination::Result DeadCodeElimination::On(Graph* g, Node* n) {
  if (auto x = TryGeneral(g, n); x.IsNoChange()) {
    return Try(g, n);
  } else {
    return x;
  }
}

}  // namespace ir
}  // namespace quake
