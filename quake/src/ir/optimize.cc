#include "ir/optimize.h"
#include "ir/graph.h"

#include "flags.h"
#include "quake.h"
#include "jit.h"

#include "ir/optimization/dce.h"

namespace quake {
namespace ir {

bool IROptimize(Quake* q, ir::Graph* g) {
  auto run_pass = [&](ir::Pass* p) -> bool {
    if (!ir::Pass::Run(g, p, nullptr)) return false;

    if (FLAG_trace_compile.Get()) {
      LOG(TRACE) << "=================================================";
      LOG(TRACE) << "after (" << p->GetName() << ")";
      LOG(TRACE) << g->PrintCompact();
    }
    return true;
  };

  // Notes, we do not run folding anymore, fold is been inlined inside of graph
  // construction and will be executed before the Reemition part
  {
    ir::DeadCodeElimination dce{q};
    if (!run_pass(&dce)) return false;
  }

  return true;
}

}  // namespace ir
}  // namespace quake
