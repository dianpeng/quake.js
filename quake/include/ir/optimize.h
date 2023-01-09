#ifndef _QUAKE_IR_OPTIMIZE_H_
#define _QUAKE_IR_OPTIMIZE_H_

#include "base.h"

namespace quake {
class Quake;

namespace ir {
class Graph;

// ir optimization after the graph construction
bool IROptimize(Quake*, Graph*);

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_OPTIMIZE_H_
