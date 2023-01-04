#ifndef _QUAKE_IR_OPTIMIZATION_DCE_H_
#define _QUAKE_IR_OPTIMIZATION_DCE_H_
#include "base.h"
#include "quake.h"

#include "ir/pass.h"
#include "ir/graph.h"
#include "ir/ssa-var.h"
#include "ir/node.h"

namespace quake {
namespace ir {

class DCE : public Pass {
 public:
  explicit DCE(Quake* q) : quake_(q) {}

  Result On(Graph*, Node*) override;
  std::string GetName() const { return "dce"; }

 private:
  Result TryDeopt(Graph*, Node*);

  Quake* quake_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_OPTIMIZATION_DCE_H_
