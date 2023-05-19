#include "ir/pass.h"
#include "jit.h"

#include "ir/optimization/fold.h"
#include "ir/optimization/dce.h"

namespace quake {
namespace ir {

bool Pass::Run(Graph* g, Pass* p, std::vector<Result>* output) {
  if (!p->Setup(g)) return false;

  auto has_change = false;

  auto visitor = [&](Node* n) -> bool {
    if (auto x = p->On(g, n); x.HasChange()) {
      switch (x.state) {
        case Result::kReplacement:
          CHECK(x.node != nullptr);
          n->ReplaceAndSetDead(x.node);
          break;

        case Result::kRemove:
          if (x.node) {
            x.node->Remove();
          } else {
            n->Remove();
          }
          break;

        default:
          UNREACHABLE();
          break;
      }

      if (output) {
        x.origin = n;
        output->push_back(x);
      }

      has_change = true;
    } else if (x.IsBailout()) {
      return false;
    }
    return true;
  };

  do {
    has_change = false;
    if (!g->ForeachStoppable(visitor)) return false;
  } while (has_change);

  p->Finalize(g);
  return true;
}

}  // namespace ir
}  // namespace quake
