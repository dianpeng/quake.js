#ifndef _QUAKE_LIR_LIVENESS_ANALYSIS_H_
#define _QUAKE_LIR_LIVENESS_ANALYSIS_H_
#include <base.h>

#include "lir/liveness.h"
#include "lir/cfg.h"

#include "zone/zone-container.h"

namespace quake {

class Quake;

namespace lir {

class HAssembler;
class VregContext;
class RALinearize;

// ----------------------------------------------------------------------------
// LivenessAnalysis — classical backwards-dataflow liveness as a discrete pass
//
// The pass solves the textbook live-variable equations over the REAL block
// CFG (lir::CFG rebuilt from the HAssembler stream via CreateCFG, D-11):
//
//   live_out[B] = U_{S in succ(B)} live_in[S]
//   live_in[B]  = use[B] U (live_out[B] - def[B])
//
// iterated to fixpoint with a single worklist seeded in reverse postorder
// (the RPO is computed by an in-pass DFS over left_bb()/right_bb() from the
// CFG start — CFG::ForeachBasicBlock is BFS with "no order requirement" and
// is deliberately not used as an order approximation). RPO + loop-aware
// NUMBERING as a liveness model was rejected as incorrect for this use: the
// numbering keeps values alive by position convention (the old phi
// force-extension), while the dataflow keeps a value alive only while a real
// use is reachable from it — the difference is pinned by the dead-across-
// back-edge unit test shape.
//
// Use collection matches the legacy RA semantics EXACTLY (the old
// RALinearize::MarkInputLiveness rules, copied verbatim):
//   - every operand with a def link (opr->instruction() != nullptr) is a use
//     of that operand's vreg;
//   - a MemInd OUTPUT's base/offset operands count as uses;
//   - deopt side-exit checkpoint entries (ins->HasExit() ->
//     exit()->ForeachEntry -> e.value()->lir_operand()) count as uses at
//     that instruction — the conservative side-exit treatment that keeps
//     snapshots correct.
// Post-vreg LIR holds no phis; if phi-ful input is ever fed, any phi met by
// the block walk is handled defensively (operands as uses at the phi
// position, output as a def). Loop phis live outside the CFG's block ranges
// (the out-of-line phi section) and are therefore not visited.
//
// Def/use POINTS are recorded in liveness-id coordinates — the linear order
// RALinearize (the numbering pass) installs via ins->liveness_info() — so
// future consumers (the plan-06 rewire, Phase 2 coalescing) can bridge
// directly to the legacy per-instruction LivenessInfo substrate (D-10).
//
// The published result is the single source of truth for liveness (D-09):
// per-block live-in/live-out bitsets over vreg ids + per-vreg def/use point
// lists. Consumers derive what they need; no second numbering scheme may
// drift beside it.
//
// Notes, like VregPass, the pass ships INERT until the plan-06 cutover: it
// is NOT invoked from Quake::DoCompile. It is independently constructible
// with a hand-built assembler + VregContext + a run of RALinearize (which
// installs the liveness ids this pass reads).
//
// Allocation: every set and list lives in the quake zone; nothing escapes
// the compile (the zone is cleared in Quake::DoCompile's DEFER — Pitfall 3).
class LivenessAnalysis {
 public:
  LivenessAnalysis(Quake*, HAssembler*, const VregContext*);

  // Build the CFG, collect local use/def sets, run the worklist fixpoint
  // and materialize the per-vreg def/use point lists.
  void Run();

 public:
  // ---------------------------------------------------------------------
  // Per-block result sets. bid is BasicBlock::block_id(); vreg ids are
  // dense in [0, vreg_count()). Blocks unreachable from the CFG start
  // (eg the shared deopt/end pseudo blocks on deopt-free streams) report
  // empty sets.
  bool LiveIn(std::uint32_t bid, std::uint32_t vreg) const;
  bool LiveOut(std::uint32_t bid, std::uint32_t vreg) const;
  bool LiveIn(const BasicBlock* bb, std::uint32_t vreg) const;
  bool LiveOut(const BasicBlock* bb, std::uint32_t vreg) const;

  // Iterate the vreg ids in the block's live-in/live-out set, ascending.
  void ForeachLiveIn(std::uint32_t bid,
                     std::function<void(std::uint32_t)>&&) const;
  void ForeachLiveOut(std::uint32_t bid,
                      std::function<void(std::uint32_t)>&&) const;

  // ---------------------------------------------------------------------
  // Per-vreg def/use point lists in liveness-id coordinates, ascending
  // (upper_bound insert semantics, matching LivenessInfo::AddUseBack
  // ordering). Multiple defs per vreg are the norm post-PHI-elimination
  // (entry-edge move + backedge move of a loop-carried value).
  const zone::Vector<LivenessPoint>& DefPoints(std::uint32_t vreg) const;
  const zone::Vector<LivenessPoint>& UsePoints(std::uint32_t vreg) const;

  // ---------------------------------------------------------------------
  std::uint32_t block_count() const { return block_count_; }
  std::uint32_t vreg_count() const { return vreg_count_; }

  // The CFG the dataflow ran over (zone-allocated blocks — hold only during
  // compile).
  const CFG* cfg() const { return &cfg_; }

  // Position of the block in the computed reverse postorder, or
  // kInvalidRpoNumber when the block is unreachable from the CFG start.
  static constexpr std::uint32_t kInvalidRpoNumber =
      std::numeric_limits<std::uint32_t>::max();
  std::uint32_t rpo_number(std::uint32_t bid) const {
    return bid < block_count_ ? rpo_number_[bid] : kInvalidRpoNumber;
  }

 private:
  void ComputeRPO();
  void CollectLocalSets();
  void SolveFixpoint();
  void CollectDefUsePoints();

  // Legacy-faithful use visitation: operands with def links, MemInd output
  // base/offset and deopt-exit checkpoint entries. Points are appended to
  // the per-vreg use lists (liveness id of `ins`); local set filling is
  // delegated to `sink`.
  void VisitUses(HInstruction* ins,
                 const std::function<void(std::uint32_t)>& sink);

  // Resolve an operand's vreg through the side table. The input contract
  // (post-vreg LIR from VregPass, or a fixture-registered context) is that
  // every Temporary carrying a def link is known — failing loud here is
  // strictly better than silently under-approximating liveness.
  std::uint32_t ResolveVreg(HOperand* opr) const;

  zone::Zone* zone() const;

  Quake* quake_;
  HAssembler* as_;
  const VregContext* vregs_;
  CFG cfg_;

  zone::Zone* zone_;

  std::uint32_t block_count_ = 0;
  std::uint32_t vreg_count_ = 0;
  std::uint32_t words_per_block_ = 0;

  // use_[bid] / def_[bid] — the local (upward-exposed / killed) sets.
  // live_in_[bid] / live_out_[bid] — the fixpoint solution.
  // Words over vreg ids, one zone::Vector per block, grown strictly by
  // emplace_back (a resize() would value-initialize inner vectors with a
  // null ZoneAllocator).
  zone::Vector<zone::Vector<std::uint64_t>> use_;
  zone::Vector<zone::Vector<std::uint64_t>> def_;
  zone::Vector<zone::Vector<std::uint64_t>> live_in_;
  zone::Vector<zone::Vector<std::uint64_t>> live_out_;

  // Per-vreg def/use point lists (liveness-id coordinates).
  zone::Vector<zone::Vector<LivenessPoint>> def_points_;
  zone::Vector<zone::Vector<LivenessPoint>> use_points_;

  // The computed reverse postorder and per-block lookup (by block id).
  zone::Vector<BasicBlock*> rpo_;
  zone::Vector<BasicBlock*> blocks_;
  zone::Vector<std::uint32_t> rpo_number_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_LIVENESS_ANALYSIS_H_
