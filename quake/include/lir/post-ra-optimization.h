#ifndef _QUAKE_LIR_POST_RA_OPTIMIZATION_H_
#define _QUAKE_LIR_POST_RA_OPTIMIZATION_H_
#include "base.h"

namespace quake {
class Quake;

namespace lir {
class HAssembler;

// Optimization performed after RA has been done, this mainly focus on the
// move-move elimination and other stuff

class PostRAOptimization {
 public:
  explicit PostRAOptimization(Quake* q) : quake_(q) {}

  void Run(HAssembler*);

 private:
  Quake* quake_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_POST_RA_OPTIMIZATION_H_
