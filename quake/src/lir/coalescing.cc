#include "lir/coalescing.h"

#include "lir/assembler.h"
#include "lir/cfg.h"
#include "lir/instruction.h"
#include "lir/liveness-analysis.h"
#include "lir/operand.h"
#include "lir/vreg-pass.h"

#include "ir/node.h"
#include "ir/checkpoint.h"
#include "ir/ssa-var.h"

#include "quake.h"

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// Construction / small helpers

CoalescingPass::CoalescingPass(Quake* quake, HAssembler* as,
                               VregContext* vregs,
                               const LivenessAnalysis* liveness)
    : quake_(quake),
      as_(as),
      vregs_(vregs),
      liveness_(liveness),
      block_of_(BlockMapAllocator{quake->zone()}),
      uf_parent_(quake->zone()),
      uf_rank_(quake->zone()) {
  CHECK(quake_);
  CHECK(as_);
  CHECK(vregs_);
  CHECK(liveness_);
}

zone::Zone* CoalescingPass::zone() const { return quake_->zone(); }

void CoalescingPass::BuildBlockMap() {
  block_of_.clear();
  liveness_->cfg()->ForeachBasicBlock([&](const BasicBlock* bb) -> bool {
    bb->ForeachInstruction([&](const HInstruction* ins) -> bool {
      block_of_.emplace(const_cast<HInstruction*>(ins), bb->block_id());
      return true;
    });
    return true;
  });
}

// ----------------------------------------------------------------------------
// Union-find over vreg ids

std::uint32_t CoalescingPass::Find(std::uint32_t v) {
  // iterative path-halving: cheap, no recursion, same asymptotics
  CHECK(v < static_cast<std::uint32_t>(uf_parent_.size()));
  auto x = uf_parent_[v];
  while (uf_parent_[x] != x) {
    uf_parent_[x] = uf_parent_[uf_parent_[x]];
    x = uf_parent_[x];
  }
  uf_parent_[v] = x;
  return static_cast<std::uint32_t>(x);
}

void CoalescingPass::Union(std::uint32_t a, std::uint32_t b) {
  auto ra = Find(a);
  auto rb = Find(b);
  CHECK_NE(ra, rb);
  if (uf_rank_[ra] < uf_rank_[rb]) {
    std::swap(ra, rb);
  }
  uf_parent_[rb] = static_cast<std::int32_t>(ra);
  if (uf_rank_[ra] == uf_rank_[rb]) {
    uf_rank_[ra]++;
  }
}

bool CoalescingPass::SameClass(std::uint32_t a, std::uint32_t b) {
  return Find(a) == Find(b);
}

// ----------------------------------------------------------------------------
// Interference oracle

bool CoalescingPass::PairInterferes(std::uint32_t a, std::uint32_t b,
                                    HInstruction* candidate) const {
  CHECK_NE(a, b);
  const auto blocks = liveness_->block_count();

  // (1)/(2): simultaneously live at any block boundary
  for (std::uint32_t bid = 0; bid < blocks; bid++) {
    if (liveness_->LiveIn(bid, a) && liveness_->LiveIn(bid, b)) return true;
    if (liveness_->LiveOut(bid, a) && liveness_->LiveOut(bid, b)) return true;
  }

  // (3): a surviving def of A inside a block B is LiveIn of (B live INTO the
  //      block means live across its entry region — the block-level
  //      approximation of "live across the def point")
  // (4): symmetric for a surviving def of B
  //
  // The candidate's own def is excluded — the merge deletes this move. Defs
  // not covered by the liveness CFG (unreachable code, which reports empty
  // sets anyway) cannot interfere and are skipped.
  for (auto* def : vregs_->GetDefs(a)) {
    if (def == candidate) continue;
    auto itr = block_of_.find(def);
    if (itr == block_of_.end()) continue;
    if (liveness_->LiveIn(itr->second, b)) return true;
  }
  for (auto* def : vregs_->GetDefs(b)) {
    if (def == candidate) continue;
    auto itr = block_of_.find(def);
    if (itr == block_of_.end()) continue;
    if (liveness_->LiveIn(itr->second, a)) return true;
  }
  return false;
}

bool CoalescingPass::ClassesInterfere(std::uint32_t a, std::uint32_t b,
                                      HInstruction* candidate) const {
  // Collect the member ids of both union-find classes, then require EVERY
  // cross pair to be clean — a candidate that is locally non-interfering but
  // would tangle two already-merged groups is rejected wholesale.
  const auto count = vregs_->vreg_count();
  const auto root_a = const_cast<CoalescingPass*>(this)->Find(a);
  const auto root_b = const_cast<CoalescingPass*>(this)->Find(b);
  CHECK_NE(root_a, root_b);

  zone::Vector<std::uint32_t> members_a(zone());
  zone::Vector<std::uint32_t> members_b(zone());
  for (std::uint32_t v = 0; v < count; v++) {
    const auto root = const_cast<CoalescingPass*>(this)->Find(v);
    if (root == root_a) {
      members_a.push_back(v);
    } else if (root == root_b) {
      members_b.push_back(v);
    }
  }
  CHECK(!members_a.empty());
  CHECK(!members_b.empty());

  for (auto ma : members_a) {
    for (auto mb : members_b) {
      if (PairInterferes(ma, mb, candidate)) return true;
    }
  }
  return false;
}

// ----------------------------------------------------------------------------
// Merge application

void CoalescingPass::ApplyMerge(HInstruction* move) {
  CHECK(move->IsMove());

  // The dying operand is the move's OUTPUT (the phi vreg's canonical
  // Temporary); the survivor is the move's SOURCE operand — its defining
  // instruction stays in the stream, the move does not.
  auto* from_op = move->output();
  auto* to_op = move->Opr(0);
  CHECK(from_op && from_op->IsTemporary());
  CHECK(to_op && to_op->IsTemporary());
  CHECK_NE(from_op, to_op);

  // Node-out re-point (mirrors VregPass::RemovePhiRedirectingUses): the
  // move's defining node's out var may still claim the dying operand, and
  // node out is authoritative regardless of backlink history. ir_node() is
  // null on hand-sculpted fixture streams — guard before dereferencing.
  if (auto* node = move->ir_node()) {
    if (auto* out = node->out()) {
      if (out->lir_operand() == from_op) {
        out->set_lir_operand(to_op);
      }
    }
  }

  // Checkpoint (deopt side-exit) entries reference value operands OUTSIDE
  // the stream def-use lists: a checkpoint entry maps its SSAVar onto the
  // lir operand holding the value at the side exit, and that pointer is
  // not a registered use of the move's output. Left alone, every snapshot
  // keeps referencing the dying temporary — the classical liveness pass
  // (correctly) counts those as uses of the absorbed vreg, and the RA
  // downstream meets a def-less interval on the live compile path. Re-point
  // them exactly like the node-out fixup above.
  as_->ForeachInstruction([&](HInstruction* ins) {
    if (ins == nullptr || !ins->HasExit()) return;
    ins->exit()->ForeachEntry([&](ir::Checkpoint::Entry& e) {
      if (e.value() != nullptr && e.value()->lir_operand() == from_op) {
        e.value()->set_lir_operand(to_op);
      }
    });
  });

  // Replace() rewrites every registered use slot onto the surviving operand,
  // CHECKs zero remaining uses, and re-points the dying operand's ir_var_
  // (the deopt side-exit link) — the exact mechanics the vreg pass relies on.
  from_op->Replace(to_op);
  CHECK(!from_op->HasUse());

  move->Remove();

  // Fold the vreg identities. Ids are resolved FRESH: an earlier merge in
  // this same apply loop may already have re-pointed the operand entries, so
  // the class ids can have shifted — MergeVregs composes correctly over
  // current ids either way. The SOURCE-side id survives (its def lives on);
  // the destination-side id becomes a hole (compaction is not done).
  std::uint32_t from_id = 0, to_id = 0;
  CHECK(vregs_->Lookup(from_op, &from_id));
  CHECK(vregs_->Lookup(to_op, &to_id));
  if (from_id != to_id) {
    vregs_->MergeVregs(from_id, to_id);
  }
}

// ----------------------------------------------------------------------------
// Run

void CoalescingPass::Run() {
  // (0) Def->block index and union-find state over the CURRENT vreg universe
  BuildBlockMap();
  const auto count = vregs_->vreg_count();
  CHECK_EQ(count, liveness_->vreg_count());

  uf_parent_.clear();
  uf_rank_.clear();
  uf_parent_.resize(count);
  uf_rank_.resize(count);
  for (std::uint32_t v = 0; v < count; v++) {
    uf_parent_[v] = static_cast<std::int32_t>(v);
    uf_rank_[v] = 0;
  }

  // (1) Candidates — provenance ONLY. The provenance table is the
  // sole enumerator; no instruction-stream scan for kMov exists anywhere in
  // this pass (T-02-01-02). Moves already unlinked by an earlier run of this
  // pass (their operands are cleared by Remove) and moves whose source
  // carries no vreg (immediate inputs, defensive) are skipped, as are pairs
  // already sharing an id.
  struct Candidate {
    HInstruction* move;
    std::uint32_t src;
    std::uint32_t dst;
    bool merged;
  };
  zone::Vector<Candidate> candidates(zone());
  vregs_->ForeachEdgeMove([&](HInstruction* move, EdgeMoveGroup group) {
    (void)group;  // provenance is the entry ticket; the group itself does
                  // not gate the decision — the interference rule does
    if (move->Opr(0) == nullptr || move->output() == nullptr) return;
    std::uint32_t src = 0, dst = 0;
    if (!vregs_->Lookup(move->Opr(0), &src)) return;
    if (!vregs_->Lookup(move->output(), &dst)) return;
    if (src == dst) return;
    candidates.push_back(Candidate{move, src, dst, false});
  });

  // (2) Decide everything against the input liveness BEFORE mutating the
  // stream: each candidate unions only when its two classes are fully clean
  // against each other.
  for (auto& candidate : candidates) {
    if (SameClass(candidate.src, candidate.dst)) continue;
    if (ClassesInterfere(candidate.src, candidate.dst, candidate.move))
      continue;
    Union(candidate.src, candidate.dst);
    candidate.merged = true;
  }

  // (3) Apply — interference-rejected moves are never unlinked; they remain
  // instructions for the explicit parallel-copy path (plan 02-03).
  for (auto& candidate : candidates) {
    if (candidate.merged) {
      ApplyMerge(candidate.move);
    }
  }
}

}  // namespace lir
}  // namespace quake
