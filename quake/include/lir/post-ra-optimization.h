#ifndef _QUAKE_LIR_POST_RA_OPTIMIZATION_H_
#define _QUAKE_LIR_POST_RA_OPTIMIZATION_H_
#include "base.h"

namespace quake {
namespace lir {

#if 0
// Optimization performed after RA has been done, this mainly focus on the
// move-move elimination and other stuff

class PostRAOptimization {
 public:
  explicit PostRAOptimization(Quake*);

  void Run(HAssembler*);
};
#endif

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_POST_RA_OPTIMIZATION_H_
