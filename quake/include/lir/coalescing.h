#ifndef _QUAKE_LIR_COALESCING_H_
#define _QUAKE_LIR_COALESCING_H_
#include <base.h>

#include <unordered_map>

#include "zone/zone-allocator.h"
#include "zone/zone-container.h"

namespace quake {

class Quake;

namespace lir {

class HAssembler;
class HInstruction;
class VregContext;
class LivenessAnalysis;

// ----------------------------------------------------------------------------
// CoalescingPass — move coalescing over vreg edge moves
//
// The vreg pass (SSA destruction) leaves one explicit move per phi input on
// every predecessor edge. Many of those moves are pure copies between values
// whose live ranges never overlap — the entry-edge shuffle of a loop phi
// (initial value -> phi vreg) and the arm moves of a branch phi (arm value
// -> phi vreg) are the canonical shapes. This pass merges those vregs and
// deletes the moves; interference-rejecting moves (the typical backedge
// induction move, whose endpoints are simultaneously live) survive for the
// explicit parallel-copy path.
//
// The merge rule is the strict classical-interference rule
//
//   A candidate move `mov B <- A` is merged only when NOTHING simultaneously
//   live connects A and B anywhere in the fixpoint liveness solution:
//     (1) no CFG block has BOTH ids in LiveIn,
//     (2) no CFG block has BOTH ids in LiveOut,
//     (3) no surviving def of A sits in a block B is LiveIn of,
//     (4) no surviving def of B sits in a block A is LiveIn of,
//   where "surviving def" excludes the candidate move itself — the merge
//   deletes that move, so its def (of B) and use (of A) vanish from the
//   stream and must not count as overlap.
//
// Notes, on the def clauses (3)/(4) using LiveIn rather than LiveIn-or-
// LiveOut: a def is a POINT, and a block-level LiveOut set says the other
// vreg is live at the block EXIT — not at the def's position. The entry-edge
// shape makes the difference load-bearing: the source is defined in the
// pre-loop block, the phi vreg is LiveOut of that same block (its live range
// STARTS at the entry move, one instruction before the block exit), so a
// LiveOut def clause would reject every entry merge — contradicting the
// plan's mandated shapes (the entry/backedge pair: entry merges, backedge
// stays). LiveIn is the correct block-level approximation for VregPass-
// inserted moves: SSA use placement guarantees no phi-vreg use can sit
// between the source's def and the edge move (that would be use-before-def
// on the sibling path), so the only way the other vreg can be live ACROSS
// the def's position is by being live INTO the block containing it. The
// both-held checks (1)/(2) stay symmetric and cover boundary simultaneity.
//
// Candidates are enumerated EXCLUSIVELY through VregContext's provenance
// table ForeachEdgeMove: a kMov with no provenance may carry
// semantics this pass cannot see (snapshot setup, rewriter canonicalization,
// RA-inserted shuffles) and is never touched (T-02-01-02).
//
// Merging is transitive-safe through a union-find over vreg ids (path
// compression + union-by-rank): a candidate is only unioned when NO member
// pair of the two classes interferes — a merge that is locally clean but
// would tangle two already-merged groups is rejected wholesale.
//
// Applying a merge redirects every use of the dying move output onto the
// surviving source operand (the same DefUseRefList in-place replacement and
// ir_var_/node-out re-pointing VregPass::RemovePhiRedirectingUses uses),
// unlinks the move from the stream, and folds the vreg identities together
// via VregContext::MergeVregs (the SOURCE's id survives — the move is gone,
// so the destination's canonical Temporary is dead). Vreg ids are not
// compacted; holes are permitted.
//
// The pass is INERT: nothing in Quake::DoCompile invokes it (cutover is plan
// 02-05). It runs between LivenessAnalysis and the linear scan, and its
// invariant — a merged pair is never simultaneously live — is permanently
// pinned by an independent post-coalescing liveness re-run.
//
// Allocation: union-find state and the def->block index are zone-allocated;
// nothing escapes the compile.
class CoalescingPass {
 public:
  CoalescingPass(Quake*, HAssembler*, VregContext*, const LivenessAnalysis*);

  // Decide + apply every legal edge-move merge (see the class comment for
  // the rule). Idempotent on an already-coalesced stream (no provenance-free
  // candidates remain, merged moves are gone from the table's stream).
  void Run();

 private:
  // ---------------------------------------------------------------------------
  // Interference oracle. `candidate` is the move under consideration;
  // its own def is excluded (the merge deletes it). Returns true when any
  // rule (1)..(4) of the class comment connects the two ids.
  bool PairInterferes(std::uint32_t a, std::uint32_t b,
                      HInstruction* candidate) const;

  // Class-wide wrapper: true when ANY member pair of the two union-find
  // classes interferes. Conservative transitivity guard.
  bool ClassesInterfere(std::uint32_t a, std::uint32_t b,
                        HInstruction* candidate) const;

  // ---------------------------------------------------------------------------
  // Union-find over vreg ids (parent + rank, path compression, union-by-rank)
  std::uint32_t Find(std::uint32_t v);
  void Union(std::uint32_t a, std::uint32_t b);
  bool SameClass(std::uint32_t a, std::uint32_t b);

  // Index every instruction of the liveness CFG with its block id — the
  // def->block mapping def clauses (3)/(4) need.
  void BuildBlockMap();

  // Redirect every use of the move's output onto the source operand, unlink
  // the move, and fold the vreg identities (see the class comment).
  void ApplyMerge(HInstruction* move);

  zone::Zone* zone() const;

  Quake* quake_;
  HAssembler* as_;
  VregContext* vregs_;
  const LivenessAnalysis* liveness_;

  // def instruction -> CFG block id (the analysis's own CFG, zone-backed)
  using BlockMapAllocator =
      zone::ZoneAllocator<std::pair<HInstruction* const, std::uint32_t>>;
  std::unordered_map<HInstruction*, std::uint32_t, std::hash<HInstruction*>,
                     std::equal_to<HInstruction*>, BlockMapAllocator>
      block_of_;

  // Union-find storage, sized to vreg_count() at Run() (flat int vectors —
  // the nested-zone::Vector resize pitfall does not apply to them)
  zone::Vector<std::int32_t> uf_parent_;
  zone::Vector<std::int32_t> uf_rank_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_COALESCING_H_
