#ifndef _QUAKE_IR_SSA_VAR_H_
#define _QUAKE_IR_SSA_VAR_H_
#include "base.h"
#include "quake.h"
#include "defuse-list.h"

#include "ir/ssa.h"

namespace quake {

namespace lir {
class HOperand;
}  // namespace lir

namespace ir {

class Node;

// Each Node will use SSAVar to indirectly reference an operand or produce a
// SSAVar as output. SSAVar is a variable corresponding to the SSA name.
// We organize SSAVar in a sperate array for fast expression level access and
// optimization.
class SSAVar : public SSARefList {
 public:
  SSAVar(Quake* q, std::uint32_t index, Node* n);

 public:
  Quake* quake() const { return quake_; }
  std::uint32_t index() const { return index_; }
  SSAFlag flag() const { return flag_; }
  void set_flag(SSAFlag f) { flag_ = f; }

  void SetDead() { flag_.SetDead(); }
  bool IsDead() const { return flag_.IsDead(); }

 public:
  Node* node() const { return node_; }
  Node* node_checked() const {
    CHECK(node_);
    return node_;
  }
  const Type& type() const;
  void set_type(const Type& t);
  bool IsInternal() const { return index_ == kInternalIndex; }

 public:
  void set_lir_operand(lir::HOperand* opr) { lir_operand_ = opr; }
  lir::HOperand* lir_operand() const { return lir_operand_; }

 private:
  Quake* quake_;

  std::uint32_t index_;

  SSAFlag flag_;

  // The node that generates this SSAVar variable
  Node* node_;

  // After lowering into LIR, this field contains a pointer points to the
  // instruction that output the IR value
  lir::HOperand* lir_operand_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_SSA_VAR_H_
