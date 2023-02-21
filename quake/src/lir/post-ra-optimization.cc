#include "lir/post-ra-optimization.h"

#include "lir/optimization/slow-block-schedule.h"

namespace quake {
namespace lir {

void PostRAOptimization::Run(HAssembler* as) {
  {
    SlowBlockSchedule pass{};
    pass.Run(as);
  }
}

}  // namespace lir
}  // namespace quake
