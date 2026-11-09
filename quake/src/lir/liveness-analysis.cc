#include "lir/liveness-analysis.h"

#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/ra-linearize.h"
#include "lir/vreg-pass.h"

#include "ir/checkpoint.h"
#include "ir/ssa-var.h"

#include "quake.h"

#include <algorithm>

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// Zone bitset helpers — words over zone::Vector<std::uint64_t>. Zone memory
// only; no heap, no std::set (the exact mistake cfg.cc's RunDom FIXME
// complains about). Dense vreg ids make set union/subtraction plain word
// arithmetic.

namespace {

std::uint32_t WordsFor(std::uint32_t bits) {
  return (bits + 63) / 64;
}

bool BitTest(const zone::Vector<std::uint64_t>& words, std::uint32_t bit) {
  CHECK_LT(bit, static_cast<std::uint32_t>(words.size() * 64));
  return (words[bit / 64] >> (bit % 64)) & 1;
}

void SetBit(zone::Vector<std::uint64_t>* words, std::uint32_t bit) {
  CHECK_LT(bit, static_cast<std::uint32_t>(words->size() * 64));
  (*words)[bit / 64] |= (1ull << (bit % 64));
}

void ClearBits(zone::Vector<std::uint64_t>* words) {
  std::fill(words->begin(), words->end(), 0);
}

// dst |= src; returns whether dst changed
bool OrInto(zone::Vector<std::uint64_t>* dst,
            const zone::Vector<std::uint64_t>& src) {
  CHECK_EQ(dst->size(), src.size());
  auto changed = false;
  for (std::size_t i = 0; i < dst->size(); i++) {
    auto merged = (*dst)[i] | src[i];
    if (merged != (*dst)[i]) {
      (*dst)[i] = merged;
      changed = true;
    }
  }
  return changed;
}

// Iterate the set bit positions of `words`, ascending
void ForeachSetBit(const zone::Vector<std::uint64_t>& words,
                   const std::function<void(std::uint32_t)>& visitor) {
  for (std::size_t i = 0; i < words.size(); i++) {
    auto word = words[i];
    while (word) {
      auto bit = static_cast<std::uint32_t>(__builtin_ctzll(word));
      visitor(static_cast<std::uint32_t>(i) * 64 + bit);
      word &= word - 1;
    }
  }
}

}  // namespace

// ----------------------------------------------------------------------------
// Ctor / dtor

LivenessAnalysis::LivenessAnalysis(Quake* quake, HAssembler* as,
                                   const VregContext* vregs)
    : quake_(quake),
      as_(as),
      vregs_(vregs),
      cfg_(quake),
      zone_(quake->zone()),
      use_(zone_),
      def_(zone_),
      live_in_(zone_),
      live_out_(zone_),
      def_points_(zone_),
      use_points_(zone_),
      rpo_(zone_),
      blocks_(zone_),
      rpo_number_(zone_) {}

zone::Zone* LivenessAnalysis::zone() const { return zone_; }

// ----------------------------------------------------------------------------
// Reverse postorder — iterative DFS from the CFG start over left_bb()/
// right_bb(), collecting postorder (a block is emitted only after its DFS
// subtrees completed) and reversing. The CFG offers no RPO iterator of its
// own (rpo_next_ is never set) and its generic block walk is a BFS with no
// order guarantee, so the pass computes its own — D-11's worklist order.

namespace {

struct DFSFrame {
  BasicBlock* bb;
  bool expanded;  // false: entering, true: subtree done -> emit postorder
};

}  // namespace

void LivenessAnalysis::ComputeRPO() {
  blocks_.resize(block_count_, nullptr);
  rpo_number_.resize(block_count_, kInvalidRpoNumber);

  zone::Vector<DFSFrame> stack{zone()};
  zone::Vector<BasicBlock*> post{zone()};

  stack.push_back(DFSFrame{cfg_.start(), false});

  while (!stack.empty()) {
    auto frame = stack.back();
    stack.pop_back();

    if (frame.expanded) {
      post.push_back(frame.bb);
      continue;
    }

    auto bid = frame.bb->block_id();
    if (blocks_[bid]) continue;  // already visited
    blocks_[bid] = frame.bb;

    // Re-push for the postorder emission once every successor subtree below
    // this frame has been emitted.
    stack.push_back(DFSFrame{frame.bb, true});

    // Push right before left so the left successor subtree is expanded first
    // (left-to-right DFS).
    if (auto* r = frame.bb->right_bb()) {
      if (!blocks_[r->block_id()]) stack.push_back(DFSFrame{r, false});
    }
    if (auto* l = frame.bb->left_bb()) {
      if (!blocks_[l->block_id()]) stack.push_back(DFSFrame{l, false});
    }
  }

  // RPO = reversed postorder
  rpo_.reserve(post.size());
  for (std::size_t i = post.size(); i-- > 0;) {
    rpo_.push_back(post[i]);
  }
  for (std::size_t i = 0; i < rpo_.size(); i++) {
    rpo_number_[rpo_[i]->block_id()] = static_cast<std::uint32_t>(i);
  }
}

// ----------------------------------------------------------------------------
// Use visitation — the legacy MarkInputLiveness rules, copied exactly.
// Every operand carrying a def link is a use of that operand's vreg; a
// MemInd output's base/offset count as uses; deopt side-exit checkpoint
// entries count as uses at the deopt instruction (the conservative
// treatment that keeps snapshots correct). Phi operands, should phi-ful
// input ever be fed, are handled defensively as ordinary uses at the phi
// position.

void LivenessAnalysis::VisitUses(
    HInstruction* ins, const std::function<void(std::uint32_t)>& sink) {
  auto visit_operand = [&](HOperand* opr) {
    // Only TEMPORARY operands with a def link are uses (the legacy rule).
    // Labels, immediates, physical registers and def-link-less slots never
    // keep a vreg alive. The def-link test alone is not enough: interned
    // constant pseudo-opcodes (kNull/kUndefined et al.) live OUT of the
    // instruction stream, are never numbered, and their dummy outputs are
    // None-type operands that still carry the def link (they flow into
    // snapshots as referenced values). They have no vreg and no interval;
    // the legacy walk silently dropped their use points onto the
    // pseudo-instruction's own never-flushed LivenessInfo, so skipping them
    // here is behavior-identical.
    if (opr && opr->IsTemporary() && opr->instruction()) {
      sink(ResolveVreg(opr));
    }
  };

  // (1) deopt side-exit snapshot entries
  if (ins->HasExit()) {
    auto* checkpoint = ins->exit();
    CHECK(checkpoint);
    checkpoint->ForeachEntry([&](const auto& e) {
      if (auto* opr = e.value()->lir_operand(); opr) {
        visit_operand(opr);
      }
    });
  }

  // (2) explicit operands
  for (auto i = 0; i < ins->opr_size(); i++) {
    visit_operand(ins->Opr(i));
  }

  // (3) a memory-indirect OUTPUT reads its base/offset inputs — the subtle
  // legacy rule (the output address is computed from them at this position)
  if (auto* out = ins->output(); out && out->IsMemInd()) {
    const auto& mem_ind = out->GetMemInd();
    visit_operand(mem_ind.base());
    if (mem_ind.offset()) visit_operand(mem_ind.offset());
  }
}

std::uint32_t LivenessAnalysis::ResolveVreg(HOperand* opr) const {
  CHECK(opr->IsTemporary());

  std::uint32_t vreg = 0;
  CHECK(vregs_->Lookup(opr, &vreg))
      << "operand with a def link is not registered in the VregContext — "
      << "run VregPass (or register the fixture's outputs) first";
  CHECK_LT(vreg, vreg_count_);
  return vreg;
}

// ----------------------------------------------------------------------------
// Local use/def collection — one walk per block. use[B] holds the
// upward-exposed vregs (used before any local definition), def[B] the
// locally-defined ones. Uses at an instruction are recorded BEFORE the
// instruction's own output def, so a value used and redefined by the same
// instruction is correctly treated as used-there.

void LivenessAnalysis::CollectLocalSets() {
  auto* defined_scratch = zone()->New<zone::Vector<std::uint64_t>>(zone());
  defined_scratch->resize(words_per_block_, 0);

  for (auto* bb : rpo_) {
    auto bid = bb->block_id();
    ClearBits(defined_scratch);

    bb->ForeachInstruction([&](HInstruction* ins) -> bool {
      VisitUses(ins, [&](std::uint32_t vreg) {
        // Use-before-def rule: a use covered by an earlier definition in
        // the same block does not propagate liveness into the block.
        if (!BitTest(*defined_scratch, vreg)) {
          SetBit(&use_[bid], vreg);
        }
      });

      if (auto* out = ins->output(); out && out->IsTemporary()) {
        auto vreg = ResolveVreg(out);
        SetBit(&def_[bid], vreg);
        SetBit(defined_scratch, vreg);
      }
      return true;
    });
  }
}

// ----------------------------------------------------------------------------
// Worklist fixpoint — a single worklist (no scattered per-block changed
// flags), seeded with every block in RPO. Processing B recomputes live_out
// from the successors' live_in and live_in from the local equations; any
// live_in change re-enqueues every predecessor (BasicBlock::ForeachEdge),
// which is what guarantees convergence to the true fixpoint rather than a
// declared-stable-early under-approximation. LIFO popping of the RPO seed
// visits successor-heavy blocks first — the right direction for the
// backward equations — and re-enqueued predecessors land on top.

void LivenessAnalysis::SolveFixpoint() {
  auto* queued = zone()->New<zone::Vector<std::uint8_t>>(zone());
  queued->resize(block_count_, 0);

  auto* worklist = zone()->New<zone::Vector<std::uint32_t>>(zone());

  for (auto* bb : rpo_) {
    auto bid = bb->block_id();
    (*queued)[bid] = 1;
    worklist->push_back(bid);
  }

  auto* new_out = zone()->New<zone::Vector<std::uint64_t>>(zone());
  new_out->resize(words_per_block_, 0);

  while (!worklist->empty()) {
    auto bid = worklist->back();
    worklist->pop_back();
    (*queued)[bid] = 0;

    auto* bb = blocks_[bid];

    // live_out[B] = U live_in[S] for S in succ(B); null successors skipped
    ClearBits(new_out);
    if (auto* l = bb->left_bb()) {
      OrInto(new_out, live_in_[l->block_id()]);
    }
    if (auto* r = bb->right_bb()) {
      OrInto(new_out, live_in_[r->block_id()]);
    }
    OrInto(&live_out_[bid], *new_out);

    // live_in[B] = use[B] U (live_out[B] - def[B]), word-wise. The operands
    // of the equation only grow monotonically, so live_in grows monotonically
    // too and a word mismatch against the stored solution means it changed.
    auto live_in_changed = false;
    for (std::size_t w = 0; w < words_per_block_; w++) {
      auto want = use_[bid][w] | (live_out_[bid][w] & ~def_[bid][w]);
      if (live_in_[bid][w] != want) {
        live_in_[bid][w] = want;
        live_in_changed = true;
      }
    }

    if (live_in_changed) {
      bb->ForeachEdge([&](const BasicBlock::Edge& e) {
        auto pbid = e.prev->block_id();
        // Only blocks inside the RPO participate in the dataflow (an
        // unreachable block's sets can never matter).
        if (rpo_number_[pbid] == kInvalidRpoNumber) return true;
        if (!(*queued)[pbid]) {
          (*queued)[pbid] = 1;
          worklist->push_back(pbid);
        }
        return true;
      });
    }
  }
}

// ----------------------------------------------------------------------------
// Per-vreg def/use point materialization — second pass in RPO. Unlike the
// local sets (which stop at use-before-def), the point lists record EVERY
// site: the def liveness id of each defining instruction (multiple defs per
// vreg are the norm post-PHI-elimination — entry-edge move + backedge move)
// and the liveness id of every use site, deopt-exit sites included. Lists
// are then sorted ascending, matching LivenessInfo::AddUseBack's
// upper_bound insert ordering.
//
// Notes, instructions outside the CFG's block ranges (the out-of-line phi
// section and the kEnd marker) are not visited: post-vreg the phi section
// is empty and the marker holds no values.

void LivenessAnalysis::CollectDefUsePoints() {
  for (auto* bb : rpo_) {
    bb->ForeachInstruction([&](HInstruction* ins) -> bool {
      CHECK(ins->liveness_info().HasLivenessId())
          << "LivenessAnalysis requires RALinearize to have run first "
          << "(no liveness id on the instruction)";
      auto lid = ins->liveness_info().liveness_id();

      VisitUses(ins, [&](std::uint32_t vreg) {
        use_points_[vreg].push_back(LivenessPoint{lid});
      });

      if (auto* out = ins->output(); out && out->IsTemporary()) {
        def_points_[ResolveVreg(out)].push_back(LivenessPoint{lid});
      }
      return true;
    });
  }

  for (std::uint32_t vreg = 0; vreg < vreg_count_; vreg++) {
    std::sort(def_points_[vreg].begin(), def_points_[vreg].end());
    std::sort(use_points_[vreg].begin(), use_points_[vreg].end());
  }
}

// ----------------------------------------------------------------------------
// Entry

void LivenessAnalysis::Run() {
  // (0) the real block CFG with pred/succ edges
  cfg_.CreateCFG(as_);

  block_count_ = cfg_.block_id_max();
  vreg_count_ = vregs_->vreg_count();
  words_per_block_ = WordsFor(vreg_count_);

  // Nested zone::Vector storage, grown strictly by emplace_back so the
  // inner vectors are constructed with the zone (a resize() would
  // value-initialize them with a null ZoneAllocator).
  auto alloc_block_sets = [&](zone::Vector<zone::Vector<std::uint64_t>>* v) {
    for (std::uint32_t bid = 0; bid < block_count_; bid++) {
      v->emplace_back(zone());
      v->back().resize(words_per_block_, 0);
    }
  };
  alloc_block_sets(&use_);
  alloc_block_sets(&def_);
  alloc_block_sets(&live_in_);
  alloc_block_sets(&live_out_);

  for (std::uint32_t vreg = 0; vreg < vreg_count_; vreg++) {
    def_points_.emplace_back(zone());
    use_points_.emplace_back(zone());
  }

  // (1) reverse postorder over the CFG
  ComputeRPO();

  // (2) local use/def per block
  CollectLocalSets();

  // (3) worklist fixpoint to the true solution
  SolveFixpoint();

  // (4) per-vreg def/use point lists
  CollectDefUsePoints();
}

// ----------------------------------------------------------------------------
// Result accessors

bool LivenessAnalysis::LiveIn(std::uint32_t bid, std::uint32_t vreg) const {
  CHECK_LT(bid, block_count_);
  CHECK_LT(vreg, vreg_count_);
  return BitTest(live_in_[bid], vreg);
}

bool LivenessAnalysis::LiveOut(std::uint32_t bid, std::uint32_t vreg) const {
  CHECK_LT(bid, block_count_);
  CHECK_LT(vreg, vreg_count_);
  return BitTest(live_out_[bid], vreg);
}

bool LivenessAnalysis::LiveIn(const BasicBlock* bb,
                              std::uint32_t vreg) const {
  return LiveIn(bb->block_id(), vreg);
}

bool LivenessAnalysis::LiveOut(const BasicBlock* bb,
                               std::uint32_t vreg) const {
  return LiveOut(bb->block_id(), vreg);
}

void LivenessAnalysis::ForeachLiveIn(
    std::uint32_t bid, std::function<void(std::uint32_t)>&& visitor) const {
  CHECK_LT(bid, block_count_);
  ForeachSetBit(live_in_[bid], visitor);
}

void LivenessAnalysis::ForeachLiveOut(
    std::uint32_t bid, std::function<void(std::uint32_t)>&& visitor) const {
  CHECK_LT(bid, block_count_);
  ForeachSetBit(live_out_[bid], visitor);
}

const zone::Vector<LivenessPoint>& LivenessAnalysis::DefPoints(
    std::uint32_t vreg) const {
  CHECK_LT(vreg, vreg_count_);
  return def_points_[vreg];
}

const zone::Vector<LivenessPoint>& LivenessAnalysis::UsePoints(
    std::uint32_t vreg) const {
  CHECK_LT(vreg, vreg_count_);
  return use_points_[vreg];
}


}  // namespace lir
}  // namespace quake
