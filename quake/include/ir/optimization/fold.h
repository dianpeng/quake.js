#ifndef _QUAKE_IR_OPTIMIZATION_FOLD_H_
#define _QUAKE_IR_OPTIMIZATION_FOLD_H_
#include "base.h"
#include "quake.h"

#include "ir/pass.h"
#include "ir/graph.h"
#include "ir/ssa-var.h"
#include "ir/node.h"

namespace quake {
namespace ir {

class Fold : public Pass {
 public:
  explicit Fold(Quake* q) : quake_(q) {}

  Result On(Graph*, Node*) override;
  void Finalize() override;
  std::string GetName() const override { return "fold"; }

 private:
  // --------------------------------------------------------------------------
  // Constant folding
  Result FoldConstantBinary(Graph*, Node*);
  Result FoldConstantUnary(Graph*, Node*);
  Result FoldConstantCast(Graph*, Node*);
  Result FoldConstantStrOp(Graph*, Node*);

  Result FoldConstantBitBinary(Graph*, Node*);
  Result FoldConstantBitUnary(Graph*, Node*);
  Result FoldConstantBit(Graph*, Node*);

  Result FoldConstantComp(Graph*, Node*);
  Result FoldConstantCompStr(Graph*, Node*);

  // entry for constant folding
  Result TryConstantFold(Graph*, Node*);

  // --------------------------------------------------------------------------
  // Simplification
  Result TrySimplifyBinary(Graph*, Node*);

  Result SimplifyBinaryLHSConstant(Graph*, Node*);
  Result SimplifyReassociationLHSConst(Graph*, Node*);
  Result SimplifyBinaryRHSConstant(Graph*, Node*);
  Result SimplifyBinaryGeneral(Graph*, Node*);

 private:
  void AddCleanUp(Node* n) { pass_cleanup_.push_back(n); }

  // Used to cleanup during Finalization
  std::vector<Node*> pass_cleanup_;

  Quake* quake_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_OPTIMIZATION_FOLD_H_
