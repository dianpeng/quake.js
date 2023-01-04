#include "ir/pass.h"
#include "jit.h"

#include "ir/optimization/fold.h"
#include "ir/optimization/dce.h"

namespace quake {
namespace ir {

void Pass::Run(Graph* g, Pass* p, std::vector<Result>* output) {
  auto has_change = false;
  do {
    has_change = false;

    g->Foreach([&](Node* n) {
      if (auto x = p->On(g, n); !x.IsNoChange()) {
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
      }
    });
  } while (has_change);

  p->Finalize();
}

}  // namespace ir
}  // namespace quake
