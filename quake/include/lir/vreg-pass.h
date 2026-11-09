#ifndef _QUAKE_LIR_VREG_PASS_H_
#define _QUAKE_LIR_VREG_PASS_H_
#include <base.h>

#include <unordered_map>

#include "zone/zone-allocator.h"
#include "zone/zone-container.h"

namespace quake {

class Quake;

namespace lir {

class HAssembler;
class HInstruction;
class HOperand;

// Provenance group for edge moves inserted by VregPass. Coalescing only
// considers moves that carry provenance (T-02-01-02): any kMov not in this
// table is off-limits — it might carry semantics (snapshot setup, rewriter
// canonicalization) that coalescing would silently break.
enum class EdgeMoveGroup {
  kEntry,       // entry-edge move of a loop phi (bb0_register_shuffle slot)
  kBackedge,    // backedge move of a loop phi (bb1_register_shuffle slot)
  kBranchPhi,   // predecessor-arm move of a branch phi
};

// ----------------------------------------------------------------------------
// VregContext — the vreg side table (SSA destruction's surviving state)
//
// Today's `Temporary` is strictly SSA: one defining instruction, one live
// interval slot (include/lir/temporary.h). A vreg, after PHI elimination,
// has MULTIPLE defs (the entry-edge move + the backedge move of a loop
// induction value), so it cannot reuse the Temporary-as-interval-owner
// scheme. Instead every move output is its own fresh Temporary and all
// Temporaries of one vreg share the id stored HERE — the side table is the
// only vreg identity (adopted resolution; extending Temporary with a vreg-id
// field was rejected as touching the operand model in more places for the
// same result).
//
// The canonical operand of a vreg — the one every user references after the
// pass redirected uses — is the entry-edge (loop phi) / lhs-arm (branch phi)
// move's output.
//
// Allocation: zone memory only, nothing escapes the compile (the zone is
// cleared in Quake::DoCompile's DEFER). Later passes consume it: classical
// liveness computes its sets over these ids (plan 05) and materialization
// derives one LiveInterval per VREG with all member Temporaries aliased to
// it (plan 06).
class VregContext {
 public:
  explicit VregContext(zone::Zone* zone)
      : zone_(zone),
        operand_map_(PairAllocator{zone}),
        defs_(zone),
        edge_moves_(zone) {}

  // The SSA-01 primitive: every newly-unseen SSA value maps to a fresh vreg.
  // An operand that already carries a vreg (eg an aliased move output) just
  // returns its existing id.
  std::uint32_t GetOrNewVreg(HOperand* opr);

  // A freshly inserted move output joins an EXISTING vreg (the phi's vreg).
  // The operand must not carry a vreg yet.
  std::uint32_t MapAlias(HOperand* opr, std::uint32_t vreg_id);

  bool Lookup(HOperand* opr, std::uint32_t* vreg_id) const;

  // Number of distinct vregs, ie ids are dense in [0, vreg_count())
  std::uint32_t vreg_count() const { return static_cast<std::uint32_t>(defs_.size()); }

  // ---------------------------------------------------------------------------
  // Per-vreg def records. A phi vreg's defs are the inserted edge moves (the
  // phi instruction itself is removed); an ordinary value's def is its own
  // instruction. Plan 05/06 compute liveness and intervals over these.
  void AddDef(std::uint32_t vreg_id, HInstruction* def);

  const zone::Vector<HInstruction*>& GetDefs(std::uint32_t vreg_id) const;

  // ---------------------------------------------------------------------------
  // Edge-move provenance (D-18). VregPass records every edge move it inserts
  // with its group; coalescing enumerates ONLY through this table, never by
  // scanning instructions for kMov (T-02-01-02).
  void AddEdgeMove(HInstruction* move, EdgeMoveGroup group);

  // Enumerate every recorded edge move with its group. Order is insertion
  // order (the order VregPass inserted them).
  template <typename Fn>
  void ForeachEdgeMove(Fn&& visitor) const {
    for (const auto& rec : edge_moves_) {
      visitor(rec.instr, rec.group);
    }
  }

  // ---------------------------------------------------------------------------
  // Merge vreg `from` into vreg `to` (to survives, from becomes an alias).
  // Every operand_map_ entry equal to `from` is re-pointed at `to`, and
  // `from`'s def records are appended into `to`'s def vector.
  //
  // vreg ids are NOT compacted — holes are permitted; consumers iterate
  // VregContext entries by walking operand users, not by id range.
  void MergeVregs(std::uint32_t from, std::uint32_t to);

 private:
  struct EdgeMoveRecord {
    HInstruction* instr;
    EdgeMoveGroup group;
  };
  using PairAllocator =
      zone::ZoneAllocator<std::pair<HOperand* const, std::uint32_t>>;

  zone::Zone* zone_;

  // HOperand* (Temporary owner) -> vreg id
  std::unordered_map<HOperand*, std::uint32_t, std::hash<HOperand*>,
                     std::equal_to<HOperand*>, PairAllocator>
      operand_map_;

  // defs_[vreg_id] — grown strictly by emplace_back in GetOrNewVreg: nested
  // zone::Vector must NEVER be resize()d (resize value-initializes inner
  // vectors with a null ZoneAllocator)
  zone::Vector<zone::Vector<HInstruction*>> defs_;

  // Provenance table: every edge move VregPass inserted, with its group.
  // Grown strictly by emplace_back in AddEdgeMove.
  zone::Vector<EdgeMoveRecord> edge_moves_;
};

// ----------------------------------------------------------------------------
// VregPass — SSA destruction (PHI elimination) as a discrete pass
//
// Run on any phi-bearing stream the pass leaves ZERO PHI instructions
// (SSA-03): each phi's vreg gets an explicit move on every predecessor edge,
// all uses are redirected to the canonical move output, the phi instruction
// is removed. Moves land ONLY on the trace's three existing insertion-point
// kinds — bb0_register_shuffle() (entry edge), bb1_register_shuffle()
// (backedge) and label->next() (branch-phi predecessor arms) — NO critical
// edge is ever split (SSA-02), no label or block is created.
//
// Use redirection covers every operand slot registered on the phi output
// (DefUseRefList::Replace rewrites the ref slots in place) and every deopt
// side-exit snapshot entry (the phi output's ir_var_ link is re-pointed at
// the canonical operand), so no dangling Temporary::instr_ into the removed
// phi survives (T-04-01/T-04-02).
//
// Notes, the pass shipped INERT until the 02-05 cutover: on the still
// Temporary-based legacy allocator the backedge move output forms a use-free
// [def,def] record, so wiring it alone would have miscompiled every
// loop-carried value. VregPass + LivenessAnalysis + LinearScanRA went live
// together in one gate-gated cutover (plan 02-05).
class VregPass {
 public:
  VregPass(Quake*, HAssembler*);

  // Creates the VregContext in the quake zone, destroys every PHI and
  // returns the side table.
  VregContext* Run();

 private:
  // Both functions return the two inserted edge-move outputs via output
  // params (entry/backedge for loop phis, lhs/rhs for branch phis). The
  // caller must not derive these from anchor->prev() after the elimination:
  // when the phi is itself the anchor (the phi sits immediately after the
  // edge label), the phi is unlinked by RemovePhiRedirectingUses and
  // reading anchor->prev_ is UB (reads a freed/unlinked instruction's
  // state — observed as null / garbage on real traces).
  void EliminateLoopPhi(HInstruction* phi, VregContext* ctx,
                        HOperand** out_entry_move,
                        HOperand** out_backedge_move);
  void EliminateBranchPhi(HInstruction* phi, VregContext* ctx,
                          HOperand** out_lhs_move, HOperand** out_rhs_move);

  // Redirect every use of the phi output to the canonical operand and unlink
  // the phi from the stream
  void RemovePhiRedirectingUses(HInstruction* phi, HOperand* canonical);

  zone::Zone* zone() const;

  Quake* quake_;
  HAssembler* as_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_VREG_PASS_H_
