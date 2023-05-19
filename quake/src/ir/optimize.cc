#include "ir/optimize.h"
#include "ir/graph.h"

#include "flags.h"
#include "quake.h"
#include "jit.h"

// ----------------------------------------------------------------------------
// All the IR optimization pass

#include "ir/optimization/fold.h"
#include "ir/optimization/dce.h"
#include "ir/optimization/gvn.h"

namespace quake {
namespace ir {

bool IROptimize(Quake* q, ir::Graph* g) {
  auto run_pass = [&](ir::Pass* p) -> bool {
    if (!ir::Pass::Run(g, p, nullptr)) return false;
    if (FLAG_trace_compile_ir_optimization.Get()) {
      LOG(TRACE) << "************ IR Optimization(" << p->GetName()
                 << ") **************";
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

  // GVN, pass tries to dedup all the unrelated stuff and simplify the IR graph
  {
    ir::GlobalValueNumbering gvn{q};
    if (!run_pass(&gvn)) return false;
  }

  return true;
}

}  // namespace ir
}  // namespace quake
