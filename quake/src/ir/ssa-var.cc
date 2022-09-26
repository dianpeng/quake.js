#include "ir/ssa-var.h"
#include "ir/node.h"

namespace quake {
namespace ir {

SSAVar::SSAVar(Quake* q, std::uint32_t index, Node* n)
    : SSARefList(q->zone()),
      quake_(q),
      index_(index),
      flag_(),
      type_(n->type()),
      node_(n) {}

}  // namespace ir
}  // namespace quake
