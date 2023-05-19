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

class DeadCodeElimination : public Pass {
 public:
  explicit DeadCodeElimination(Quake* q) : quake_(q) {}

  Result On(Graph*, Node*) override;
  std::string GetName() const override { return "dce"; }

 private:
  Result TryGeneral(Graph*, Node*);
  Result Try(Graph*, Node*);

  Quake* quake_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_OPTIMIZATION_DCE_H_
