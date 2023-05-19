#include "ir/optimization/gvn.h"
#include "quake-rt.h"

namespace quake {
namespace ir {

using namespace quake::ir::_;

void GlobalValueNumbering::MaybeCalculateHash(Node* n) {
  if (!has_hash_[n->node_id()]) {
    n->flag().SetExtra(static_cast<std::uint64_t>(_::GVNNodeHash(*n)));
    has_hash_[n->node_id()] = true;
  }
}

Node* GlobalValueNumbering::CheckTrivialPhi(Graph* g, Node* from,
                                            Node* replacement) {
  if (auto n = phi_map_[replacement->node_id()]; n) {
    return n;
  } else {
    // ---------------------------------------------------------------------
    // create PHI that self reference to itself. Notes this phi is still a
    // cyclic reference but it just does not have any mutation internally.
    // For example:
    //
    //   #1: PHI(value, #1)
    //
    //   [PHI] --------> [Value]
    //    | ^
    //    | |
    //    | |
    //    +-+
    //
    // ---------------------------------------------------------------------
    auto phi_ssa = g->AddValue(from, Opcode::kPhiLoop, replacement->type(),
                               replacement->out(), nullptr);
    auto phi = phi_ssa->node_checked();
    phi->Set(1, phi_ssa);

    CHECK(phi->InferPhiType());
    phi_map_[replacement->node_id()] = phi;
    return phi;
  }
}

bool GlobalValueNumbering::Setup(Graph* g) {
  has_hash_.resize(g->max_node_id(), false);
  phi_map_.resize(g->max_node_id(), nullptr);
  frame_.emplace_back();
  return true;
}

// As long as we figure out that original node and replacement node are not
// in the same BB, then we need to add loop phi decoration for the replacement
Node* GlobalValueNumbering::MaybeMarkLoopPhi(Graph* g, Node* original,
                                             Node* replacement) {
  // Phi node will *never* be part of GVN
  CHECK(!original->IsPhi());
  CHECK(!replacement->IsPhi());

  // For none loop JIT, it does not have more than one BB
  if (quake_->trace_type() != TraceType::kLoop) {
    return replacement;
  }

  auto original_bb = g->GetBBType(*original);
  auto replacement_bb = g->GetBBType(*replacement);

  if (original_bb != replacement_bb) {
    // Therefore the replacement node requires a loop phi, the situation is
    // as following :
    //
    // [BB0]:
    //    ...
    //    #replacement->: instruction opr1, opr2, ...
    //    ...
    //
    // [BB1]:
    //    ...
    //
    //    #original->: instruction opr1, opr2, ...
    //
    //
    // Since the inside of BB1 means it will have loop carry dependency of its
    // reference, unless we re-generate LIR considering such thing, then all
    // the reference for variable that is generated outside of the loop body
    // will require a phi node decoration. For each PHI node, we can/should
    // just generate one decoration

    return CheckTrivialPhi(g, original, replacement);
  }

  return replacement;
}

Node* GlobalValueNumbering::TryDedup(Node* n) {
  if (!GVNNodeIsAllowed(*n)) {
    return nullptr;  // for node that has side effect, just do nothing
  }

  // Check whether we already have a node that is duplicated from this
  if (auto itr = current().map.find(GVNNode{n}); itr != current().map.end()) {
    return (itr->node == n ? nullptr : itr->node);
  }

  // Okay, we do not have such node, just record it
  current().map.insert(GVNNode{n});
  return nullptr;
}

GlobalValueNumbering::Result GlobalValueNumbering::On(Graph* g, Node* n) {
  // Check whether we are starting another inline frame or not. An inline
  // frame section is guarded by EnterInlineFrame and LeaveInlineFrame
  switch (n->opcode()) {
    case Opcode::kEnterInlineFrame:
      frame_.emplace_back();
      break;

    case Opcode::kLeaveInlineFrame:
      CHECK(frame_.size() > 1);
      frame_.pop_back();
      break;

    default:
      if (auto dn = TryDedup(n); dn) {
        dn = MaybeMarkLoopPhi(g, n, dn);
        return Replace(dn);
      }
  }

  return NoChange();
}

}  // namespace ir
}  // namespace quake
