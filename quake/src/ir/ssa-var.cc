#include "ir/ssa-var.h"
#include "ir/node.h"

namespace quake {
namespace ir {

SSAVar::SSAVar(Quake* q, std::uint32_t index, Node* n)
    : SSARefList(q->zone()),
      quake_(q),
      index_(index),
      flag_(),
      node_(n),
      lir_operand_(nullptr) {

  CHECK(!n->out_);
  n->out_ = this;
}

const Type& SSAVar::type() const { return node_->type(); }

}  // namespace ir
}  // namespace quake
