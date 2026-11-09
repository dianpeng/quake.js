#include "lir/ra-linear-scan.h"
#include "lir/arch-constraint.h"
#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/liveness-analysis.h"
#include "lir/operand.h"
#include "lir/parallel-copy.h"
#include "lir/snapshot.h"
#include "lir/vreg-pass.h"

#include "ir/checkpoint.h"
#include "ir/ssa-var.h"

#include "frame-layout.h"
#include "quake.h"

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

namespace quake {
namespace lir {

namespace {

// Move opcode for a spill transfer of the given bank (the legacy
// RegisterShuffleMoveTo pairing).
HOpcode SpillMoveOpcode(bool is_fp) { return is_fp ? HOpcode::kMovF : HOpcode::kMov; }

HOpcode SpillMoveOpcode(RegisterKind kind) {
  return RegisterKindIsFP(kind) ? HOpcode::kMovF : HOpcode::kMov;
}

}  // namespace

LinearScanRA::LinearScanRA(Quake* quake, HAssembler* as,
                           const VregContext* vregs,
                           const LivenessAnalysis* liveness)
    : quake_(quake),
      as_(as),
      vregs_(vregs),
      liveness_(liveness),
      intervals_(zone()) {
  // The bank free lists, as Register* (order matters: the scan allocates
  // the first free register of the bank — the reverse-ABI order the x64
  // provider uses, so ABI argument registers are allocated last).
  for (auto kind : GetFreeGPRegister()) {
    free_gp_.push_back(Register::GetRegisterFromKind(kind));
  }
  for (auto kind : GetFreeFPRegister()) {
    free_fp_.push_back(Register::GetRegisterFromKind(kind));
  }

  // The liveness-id -> instruction map every point lookup goes through.
  as_->ForeachInstruction([&](HInstruction* ins) {
    auto id = ins->liveness_info().liveness_id();
    if (id != kInvalidLivenessId) ins_at_.emplace(id, ins);
  });
}

zone::Zone* LinearScanRA::zone() const { return quake_->zone(); }

const RAInterval* LinearScanRA::Interval(std::uint32_t vreg) const {
  if (vreg >= intervals_.size()) return nullptr;
  const auto& iv = intervals_[vreg];
  if (iv.assigned == nullptr && iv.spill == MemoryStack::kInvalidSlot) {
    return nullptr;
  }
  return &iv;
}

bool LinearScanRA::IsLinked(HInstruction* ins) const {
  // HInstruction::Remove unlinks the node (prev_/next_ nulled) — every
  // linked instruction has both links since the stream is circular.
  return ins != nullptr && ins->next() != nullptr && ins->prev() != nullptr;
}

// Unlink an edge move converted into a parallel copy. NOT HInstruction::
// Remove: the move's output still carries the vreg's redirected uses —
// they resolve to the copy's destination location at materialization
// (task 3), so the use-count CHECK does not apply yet.
void LinearScanRA::UnlinkMove(HInstruction* move) {
  move->RemoveOperands();
  move->RemoveSelf();
}

// ============================================================================
// Phase 1 — interval construction

base::Error LinearScanRA::ResolveIntervalKind(std::uint32_t vreg,
                                              RAInterval* out) {
  // The interval's own def point's instruction is the primary source
  // (hand-registered fixture contexts carry no VregContext::AddDef
  // records); the def records are the fallback for merged vregs whose
  // canonical def moved.
  auto derive = [&](HInstruction* def) -> const Register* {
    auto rtype = def->opcode_info().output.output0_rtype();
    if (rtype == RegisterType::kGeneral) return free_gp_.front();
    if (rtype == RegisterType::kFloat) return free_fp_.front();
    return nullptr;
  };

  if (auto itr = ins_at_.find(out->def.point()); itr != ins_at_.end()) {
    if (auto* r = derive(itr->second)) {
      out->kind = r->register_kind();
      return {};
    }
  }

  for (auto* def : vregs_->GetDefs(vreg)) {
    if (!IsLinked(def)) continue;  // coalescing-unlinked move: inert record
    if (auto* r = derive(def)) {
      out->kind = r->register_kind();
      return {};
    }
  }
  return base::Error::Mk(
      "linear-scan-ra: vreg %u has no live def to derive its register bank "
      "from",
      vreg);
}

// ----------------------------------------------------------------------------
// Materialized register views (the legacy ToOperand contract, see
// RAInterval::size_class in the header): every register operand the RA
// emits for an interval renders the interval's size-class view of the
// physical register. Translate freely mixes instruction-derived widths
// with u::Gp(operand) views, so a qword view of an int32 value yields
// unencodable mixes like `mov eax, r13`.

const Register* LinearScanRA::RegView(const Register* r, SizeClass sc) {
  if (r == nullptr) return nullptr;
  if (r->size_class() == sc) return r;
  if (auto* view = Register::GetRegisterFromKindAndSizeClass(
          r->register_kind(), sc)) {
    return view;
  }
  return r;  // no such view (eg FP sizes): the canonical register
}

const Register* LinearScanRA::ViewOf(const RAInterval& iv) const {
  return RegView(iv.assigned, iv.size_class);
}

SizeClass LinearScanRA::ResolveIntervalSizeClass(
    std::uint32_t vreg, std::vector<std::uint8_t>* state) {
  auto& st = (*state)[vreg];
  if (st == 2) return intervals_[vreg].size_class;
  if (st == 1) return SizeClass::kQWord;  // def chain cycle: safe default

  st = 1;
  auto sc = SizeClass::kQWord;
  for (auto* def : vregs_->GetDefs(vreg)) {
    if (!IsLinked(def)) continue;  // coalescing-unlinked move: inert record

    auto d = def->output_size_class();
    if (d == SizeClass::kUnknown || d == SizeClass::kIrrelevant) {
      // Width-less moves/shuffles carry no output width — inherit the
      // moved value's class down the def chain.
      if (def->opr_size() >= 1 && def->Opr(0) != nullptr &&
          def->Opr(0)->IsTemporary()) {
        std::uint32_t src = 0;
        if (vregs_->Lookup(def->Opr(0), &src) && src != vreg) {
          d = ResolveIntervalSizeClass(src, state);
        } else {
          continue;
        }
      } else {
        continue;
      }
    }
    sc = d;
    break;
  }

  st = 2;
  intervals_[vreg].size_class = sc;
  return sc;
}

// ----------------------------------------------------------------------------
// G-01 layer 1 (plan 02.1-01): the one shared inclusive coverage predicate
// and the DiesAtUse boundary refinement. Both PlanConstraints filters
// (clobber parks, occupant eviction) consume IntervalLiveAt, so the planned
// park set is exactly the CFG-live set — the planner's coverage can never
// drift from the dataflow model the RAValidator reconstructs.

bool LinearScanRA::IntervalLiveAt(const RAInterval& iv,
                                  LivenessPoint point) const {
  // Inclusive on the last point (the occupant-eviction filter needs the
  // boundary: a pre-instruction landing mov would otherwise clobber a
  // value the instruction itself references; the clobber-park filter
  // excludes the boundary via DiesAtUse below). Exclusive on the def
  // point: a value is never live AT its own def for park purposes — the
  // shipped raw tests were strict on def.
  return iv.def.point() < point.point() && point.point() <= iv.last.point();
}

bool LinearScanRA::DiesAtUse(const RAInterval& iv, LivenessPoint point) const {
  // The boundary refinement: last == point only counts as "live at" when
  // the point is NOT a genuine use of the value. A value whose last point
  // IS the instruction is consumed before the clobber matters (an operand
  // or a checkpoint entry read by the instruction itself — uses the
  // validator excludes from clobber accountability). The inverse shape —
  // last == point reached by the CFG extension only (a block tail the
  // value is live out of, no use at the tail instruction) — IS live past
  // the instruction and must be parked.
  if (!IntervalLiveAt(iv, point)) return false;
  if (iv.last.point() != point.point()) return false;
  for (auto p : liveness_->UsePoints(iv.vreg)) {
    if (p.point() == point.point()) return true;
  }
  return false;
}

// The CFG-driven extent extension (G-01 layer 1): for every block the
// dataflow solution marks the vreg live in or live out of, the interval
// must cover the block's whole numbered instruction span. The G-01
// under-extension is a LiveOut-side miss: the backedge def closes the
// linear range inside a block the value is live out of. Dataflow-driven
// ONLY — no def-count tests, no block-leader-position tests (reverted
// mitigation #3 stays reverted).
void LinearScanRA::ExtendIntervalToCfgLiveness(std::uint32_t vreg,
                                               RAInterval* iv) {
  liveness_->cfg()->ForeachBasicBlock([&](const BasicBlock* bb) -> bool {
    const auto bid = bb->block_id();
    if (!liveness_->LiveIn(bid, vreg) && !liveness_->LiveOut(bid, vreg)) {
      return true;
    }
    std::uint32_t hi = 0;
    bool any = false;
    for (const auto* ins = bb->start(); ins != bb->end(); ins = ins->next()) {
      const auto id = ins->liveness_info().liveness_id();
      if (id == kInvalidLivenessId) continue;  // inserted movs/markers
      hi = std::max(hi, id);
      any = true;
    }
    if (any && iv->last.point() < hi) iv->last = LivenessPoint{hi};
    return true;
  });
}

base::Error LinearScanRA::BuildIntervals() {  intervals_.clear();

  const auto count = vregs_->vreg_count();
  intervals_.resize(count);  // holes stay default (empty) intervals

  // The numbered stream's end — the conservative bound for loop-carried
  // wrap ranges (see below).
  std::uint32_t max_liveness_id_ = 0;
  for (const auto& [id, ins] : ins_at_) {
    (void)ins;
    max_liveness_id_ = std::max(max_liveness_id_, id);
  }

  std::vector<std::uint8_t> size_state(count, 0);
  for (std::uint32_t vreg = 0; vreg < count; ++vreg) {
    ResolveIntervalSizeClass(vreg, &size_state);
  }

  for (std::uint32_t vreg = 0; vreg < count; ++vreg) {
    const auto& defs = liveness_->DefPoints(vreg);
    const auto& uses = liveness_->UsePoints(vreg);

    // The union of the analysis' point lists and every linked def record's
    // own liveness id (coalescing-merged vregs keep absorbed def records in
    // VregContext; the deleted moves are unlinked and filtered, the live
    // member defs contribute their redefinition points).
    std::vector<std::uint32_t> member_ids;
    for (auto* def : vregs_->GetDefs(vreg)) {
      if (!IsLinked(def)) continue;
      auto id = def->liveness_info().liveness_id();
      if (id != kInvalidLivenessId) member_ids.push_back(id);
    }

    bool any = !defs.empty() || !uses.empty() || !member_ids.empty();
    if (!any) continue;  // hole (absorbed id): no interval

    // An id with NO live def anywhere is a coalescing-absorbed id: the
    // merge redirected every stream use AND every checkpoint reference
    // onto the survivor operand, but the pre-coalescing LivenessAnalysis
    // (which by design runs before the merge pass) keeps stale def/use
    // points for it. Nothing post-coalescing resolves to this id — no
    // materialized operand will ever query its location — so treat it like
    // a hole instead of deriving a bank from an unrelated instruction that
    // happens to occupy a stale def point's liveness id.
    auto has_live_def = [&]() -> bool {
      if (FirstLinkedDef(vreg) != nullptr) return true;
      for (auto p : defs) {
        auto itr = ins_at_.find(p.point());
        if (itr == ins_at_.end()) continue;
        auto* out = itr->second->output();
        std::uint32_t ov = 0;
        if (out != nullptr && out->IsTemporary() && vregs_->Lookup(out, &ov) &&
            ov == vreg) {
          return true;
        }
      }
      return false;
    };
    if (!has_live_def()) continue;  // absorbed id: stale points only

    auto& iv = intervals_[vreg];
    iv.vreg = vreg;
    if (!defs.empty()) {
      iv.def = defs.front();
    } else if (!uses.empty()) {
      iv.def = uses.front();
    } else {
      // No analysis points at all (a value the block walk never visited,
      // known only through its def record): anchor the interval at the
      // earliest member def.
      iv.def = LivenessPoint{*std::min_element(member_ids.begin(),
                                               member_ids.end())};
    }
    iv.last = iv.def;
    for (auto p : defs) {
      if (iv.last.point() < p.point()) iv.last = p;
    }
    for (auto p : uses) {
      if (iv.last.point() < p.point()) iv.last = p;
    }

    // member def records (linked) extend through their own points too
    for (auto id : member_ids) {
      if (iv.last.point() < id) iv.last = LivenessPoint{id};
    }

    // G-01 layer 1 (plan 02.1-01): CFG-accurate extent extension. Applied
    // BEFORE the wrap-extension check so wrap stays the last resort. The
    // block-leader-position extension that lived here was reverted in 02-05
    // (mitigation #3: syntactic, regressed runtime correctness); the
    // dataflow-driven extension below replaces it with the CFG-accurate
    // notion (ExtendIntervalToCfgLiveness documents the mechanism).
    ExtendIntervalToCfgLiveness(vreg, &iv);

    // Loop-carried wrap: a multi-def vreg is loop-phi-shaped by
    // construction (the vreg pass gives a phi vreg one def per edge move:
    // the entry arm AND the backedge arm). The linear [def, last] range of
    // such a vreg cannot express that the value is live around the whole
    // loop — a use at the loop header is reached from the backedge without
    // the entry def re-executing, so the register-level liveness (which the
    // validator checks) keeps the value live across every intermediate
    // clobber. Extend every multi-def range to the end of the numbered
    // stream — the conservative classical-loop bound, the same shape
    // legacy achieved with its loop-PHI force-extension. Single-def vregs
    // keep their precise ranges.
    if (iv.last.point() < iv.def.point()) {
      iv.last = LivenessPoint{max_liveness_id_};
    }

    if (auto err = ResolveIntervalKind(vreg, &iv); err) return err;
  }

  return {};
}

// ============================================================================
// Phase 2 — the scan

MemoryStack::Slot LinearScanRA::AcquireSpillSlot(std::uint32_t free_from) {
  for (auto& record : spill_records_) {
    // The tenure rule (plan 02.1-01, Finding 2): a released slot is
    // acquirable only when its previous holder's last point is at or before
    // the acquiring interval's DEF point. The single-location
    // materialization contract writes every def site of the acquiring
    // interval into this slot retroactively, so acquiring a slot whose
    // holder is still live at the acquirer's def would clobber that live
    // value (the crash_6.js v125/v304 slot collision class).
    if (!record.in_use && record.free_at <= free_from) {
      record.in_use = true;
      return record.slot;
    }
  }
  auto slot = static_cast<MemoryStack::Slot>(spill_records_.size());
  spill_records_.push_back(SpillRecord{slot, true, 0, 0});
  as_->frame_layout()->AddSpill();
  return slot;
}

void LinearScanRA::ReleaseSpillSlot(MemoryStack::Slot slot,
                                    std::uint32_t free_at) {
  for (auto& record : spill_records_) {
    if (record.in_use && record.slot == slot) {
      record.in_use = false;
      record.free_at = free_at;
      return;
    }
  }
}

const Register* LinearScanRA::FindFreeRegisterAt(
    RegisterKind kind, LivenessPoint point, const RegisterSet& exclude) const {
  const auto& list = RegisterKindIsFP(kind) ? free_fp_ : free_gp_;
  const auto& inuse = RegisterKindIsFP(kind) ? inuse_fp_ : inuse_gp_;
  for (auto* r : list) {
    if (inuse.Get(r) || exclude.Get(r)) continue;
    // Owned by a register-resident interval covering this point? (The
    // inuse set only tracks the scan's CURRENT frontier; reload/fixup
    // queries ask about arbitrary later points.)
    bool covered = false;
    for (const auto& iv : intervals_) {
      if (iv.assigned == r && iv.def.point() <= point.point() &&
          point.point() <= iv.last.point()) {
        covered = true;
        break;
      }
    }
    if (!covered) return r;
  }
  return nullptr;
}

base::Error LinearScanRA::Scan() {
  // Intervals in def order (stable: equal defs keep id order).
  std::vector<std::uint32_t> ids;
  ids.reserve(intervals_.size());
  for (const auto& iv : intervals_) ids.push_back(iv.vreg);
  std::stable_sort(ids.begin(), ids.end(), [&](std::uint32_t a, std::uint32_t b) {
    return intervals_[a].def.point() < intervals_[b].def.point();
  });

  // The active list, ascending by last point.
  std::vector<std::uint32_t> active;

  auto expire = [&](LivenessPoint point) {
    for (auto itr = active.begin(); itr != active.end();) {
      const auto& iv = intervals_[*itr];
      // Expire when the last live point does not reach PAST this def — an
      // interval whose last use sits AT this instruction shares the
      // register with the value being defined here: within one
      // instruction operands are read before the output is written (the
      // x64 two-operand shape), and def/use points are per-instruction
      // ids, so equal points mean the same instruction.
      if (iv.last.point() <= point.point()) {
        if (iv.assigned != nullptr) {
          (iv.assigned->IsFP() ? inuse_fp_ : inuse_gp_).Unset(iv.assigned);
        } else {
          // Record the releasing holder's last point: the slot's true
          // free-from point for the tenure rule below.
          ReleaseSpillSlot(iv.spill, iv.last.point());
        }
        itr = active.erase(itr);
      } else {
        ++itr;
      }
    }
  };

  for (auto vreg : ids) {
    auto& cur = intervals_[vreg];
    expire(cur.def);

    auto& inuse = RegisterKindIsFP(cur.kind) ? inuse_fp_ : inuse_gp_;
    const auto& list = RegisterKindIsFP(cur.kind) ? free_fp_ : free_gp_;

    const Register* free = nullptr;
    for (auto* r : list) {
      if (!inuse.Get(r)) {
        free = r;
        break;
      }
    }

    if (free != nullptr) {
      cur.assigned = free;
      inuse.Set(free);
    } else {
      // Bank exhausted: spill the furthest-last interval of the bank —
      // the current one when nothing live reaches further (the classic
      // spill-current-then policy)
      std::uint32_t victim = 0;
      bool have_victim = false;
      for (auto a : active) {
        const auto& iv = intervals_[a];
        if (iv.assigned == nullptr) continue;  // spilled: holds no register
        if (RegisterKindIsFP(iv.kind) != RegisterKindIsFP(cur.kind)) continue;
        if (!have_victim || intervals_[victim].last.point() < iv.last.point()) {
          victim = a;
          have_victim = true;
        }
      }

      if (!have_victim || cur.last.point() > intervals_[victim].last.point()) {
        cur.in_memory = true;
        // Both acquisition sites pass the ACQUIRER's def point: the
        // canonical slot is written at every def site of the acquiring
        // interval, so it must be free (previous holder expired) at that
        // def point, not merely at this later sweep point.
        cur.spill = AcquireSpillSlot(cur.def.point());
      } else {
        auto& vv = intervals_[victim];
        cur.assigned = vv.assigned;
        inuse.Set(cur.assigned);
        vv.assigned = nullptr;
        vv.in_memory = true;
        vv.spill = AcquireSpillSlot(vv.def.point());
      }
    }

    // Insert into the active list, ascending by last point.
    auto pos = std::find_if(active.begin(), active.end(), [&](std::uint32_t a) {
      return intervals_[a].last.point() > cur.last.point();
    });
    active.insert(pos, vreg);
  }

  return {};
}

// ============================================================================
// Location queries

CopySlot LinearScanRA::LocationOf(std::uint32_t vreg) const {
  const auto& iv = intervals_[vreg];
  if (iv.in_memory) return CopySlot::FromSpill(iv.spill);
  CHECK(iv.assigned != nullptr);
  return CopySlot::FromReg(ViewOf(iv));
}

CopySlot LinearScanRA::LocationAt(std::uint32_t vreg, HInstruction* ins) const {
  auto itr = location_fix_.find(std::make_pair(ins, vreg));
  if (itr != location_fix_.end()) return itr->second;
  return LocationOf(vreg);
}

// The producer link materialized operands carry: the interval's first
// still-linked def — or the first def record of any kind (a def whose edge
// move was converted into a parallel copy is unlinked but still carries the
// value's concrete output size class, which spill-mov creation reads via
// Opr(0)->size_class()) — or the anchored instruction for def-less shapes.
// The link only satisfies the htype()/print contract.
HInstruction* LinearScanRA::LinkFor(std::uint32_t vreg,
                                    HInstruction* fallback) const {
  if (auto* def = FirstLinkedDef(vreg)) return def;
  for (auto* def : vregs_->GetDefs(vreg)) {
    if (def != nullptr) return def;
  }
  return fallback;
}

HInstruction* LinearScanRA::FirstLinkedDef(std::uint32_t vreg) const {
  for (auto* def : vregs_->GetDefs(vreg)) {
    if (IsLinked(def)) return def;
  }
  return nullptr;
}

MemoryStack::Slot LinearScanRA::ParkSlot(bool fp, std::size_t ordinal) {
  auto& pool = fp ? park_slot_fp_ : park_slot_gp_;
  while (pool.size() <= ordinal) {
    pool.push_back(static_cast<MemoryStack::Slot>(spill_records_.size()));
    spill_records_.push_back(SpillRecord{pool.back(), true, 0});
    as_->frame_layout()->AddSpill();
  }
  return pool[ordinal];
}

// ============================================================================
// Phase 3 — PHI edge-move conversion (PCOPY-02, Step A)
//
// Every provenance-tracked edge move that survived coalescing is replaced
// by ONE kParallelCopy per shuffle anchor. The group's pairs are each
// move's source vreg location -> destination vreg location (the
// destination interval's canonical allocation — in this single-location
// model every member def writes the interval's own location, which is
// exactly the legacy edge-write contract of ra-materialize.cc:244-260).
// The pair list is sequentialized through the ParallelCopyResolver per
// bank before emission: the stream copy is then itself a correct
// sequential program (cycles broken through the per-site scratch), the
// shape the RAValidator's order simulation proves.

base::Error LinearScanRA::ConvertEdgeMoves() {
  std::set<HInstruction*> survivors;
  vregs_->ForeachEdgeMove([&](HInstruction* move, EdgeMoveGroup) {
    if (!IsLinked(move)) return;         // coalescing unlinked it
    if (move->Opr(0) == nullptr) return; // defensive: cleared operands
    survivors.insert(move);
  });
  if (survivors.empty()) return {};

  // Group the survivors by their shuffle anchor: the instruction
  // immediately after the contiguous survivor run (the moves of one group
  // were inserted one after another directly before the anchor — bb0
  // entry shuffle, bb1 backedge shuffle, or a branch-phi arm's
  // label->next() slot).
  std::map<HInstruction*, std::vector<HInstruction*>> groups;
  for (auto* move : survivors) {
    auto* anchor = move->next();
    while (survivors.count(anchor)) anchor = anchor->next();
    groups[anchor].push_back(move);
  }

  for (auto& [anchor, moves] : groups) {
    std::vector<CopyPair> pairs;
    std::vector<bool> pair_is_fp;

    for (auto* move : moves) {
      std::uint32_t sv = 0, dv = 0;
      if (!vregs_->Lookup(move->Opr(0), &sv)) {
        return base::Error::Mk(
            "linear-scan-ra: edge move #%u source carries no vreg",
            move->id());
      }
      if (!vregs_->Lookup(move->output(), &dv)) {
        return base::Error::Mk(
            "linear-scan-ra: edge move #%u output carries no vreg",
            move->id());
      }
      auto src = LocationOf(sv);
      auto dst = LocationOf(dv);
      if (src == dst) continue;  // identity: nothing to transfer

      auto is_fp = RegisterKindIsFP(intervals_[sv].kind);
      CopyPair pair;
      pair.src = src;
      pair.dst = dst;
      // Width: the register side's size class when both sides are
      // registers; the full 8-byte slot otherwise (MemoryStack slots are
      // 8-byte aligned — bit-exact, per the 02-02 lowering decision).
      pair.size_class = (src.IsRegister() && dst.IsRegister())
                            ? src.reg->size_class()
                            : SizeClass::kQWord;
      pairs.push_back(pair);
      pair_is_fp.push_back(is_fp);
    }

    if (pairs.empty()) {
      // Every move was an identity — the group vanishes entirely (the
      // fully-coalesced group produces no instruction at all).
      for (auto* move : moves) UnlinkMove(move);
      continue;
    }

    RegisterSet no_claims;
    auto ordered = SequentializePairs(
        LivenessPoint{anchor->liveness_info().liveness_id()}, pairs,
        pair_is_fp, no_claims);
    zone::Vector<CopyPair> zone_ordered{zone()};
    for (const auto& p : ordered) zone_ordered.push_back(p);
    NewParallelCopyBefore(as_, anchor, zone_ordered);

    for (auto* move : moves) UnlinkMove(move);
  }

  return {};
}

// ----------------------------------------------------------------------------
// Per-bank sequentialization through the ParallelCopyResolver: each
// bank's pair set resolves with a guaranteed scratch — a register free at
// the site, or a spill temp — and the emitted pair list is the resolver's
// step sequence, itself a correct sequential program.

std::vector<CopyPair> LinearScanRA::SequentializePairs(
    LivenessPoint point, const std::vector<CopyPair>& pairs,
    const std::vector<bool>& pair_is_fp, const RegisterSet& extra_exclude) {
  std::vector<CopyPair> result;

  for (bool fp : {false, true}) {
    zone::Vector<CopyPair> bank_pairs{zone()};
    for (std::size_t i = 0; i < pairs.size(); ++i) {
      if (pair_is_fp[i] == fp) bank_pairs.push_back(pairs[i]);
    }
    if (bank_pairs.empty()) continue;

    // The scratch must not participate in any pair: exclude every pair
    // location and the instruction's claimed registers from the free
    // search (a free register can be another pair's destination only in
    // theory — destinations are live vreg locations — but the exclusion
    // makes the guarantee structural).
    RegisterSet participants;
    for (const auto& p : bank_pairs) {
      if (p.src.IsRegister()) participants.Set(p.src.reg);
      if (p.dst.IsRegister()) participants.Set(p.dst.reg);
    }
    participants.Union(extra_exclude);

    CopySlot scratch;
    auto kind = fp ? free_fp_.front()->register_kind()
                   : free_gp_.front()->register_kind();
    if (auto* r = FindFreeRegisterAt(kind, point, participants); r != nullptr) {
      scratch = CopySlot::FromReg(r);
    } else {
      // No bank register free at the site: park through a spill temp
      // (dedicated per-bank slot; the resolver bridges spill-to-spill
      // through it in two steps).
      scratch = CopySlot::FromSpill(ParkSlot(fp, 0));
    }

    ParallelCopyResolver resolver{zone(), scratch};
    auto steps = resolver.Resolve(bank_pairs);
    for (const auto& step : steps) {
      CopyPair pair;
      pair.src = step.src;
      pair.dst = step.dst;
      pair.size_class = step.size_class;
      result.push_back(pair);
    }
  }

  return result;
}

// ============================================================================
// Phase 4 — instruction-constraint satisfaction
//
// Per numbered instruction: (a) clobber/scratch eviction parks every
// register-resident interval covering the instruction whose register the
// instruction kills; (b) operand/output constraints of kind
// kFixedRegister / kAnyRegister / kSameAsOperand and RCall argument
// landing become (current location, required location) pairs of ONE
// kParallelCopy inserted immediately before the instruction; (c)
// fixed-output relocations become post-instruction movs. Fast/slow
// boundaries and joins carry no constraints of their own (the five
// marker opcodes are explicit no-constraints entries) — the join's
// register-state transfer IS the branch-phi edge conversion of Step A,
// and the slow block's RCall is satisfied by this generic machinery.

base::Error LinearScanRA::PlanConstraints() {
  plans_.clear();
  claimed_.clear();

  for (auto& [pid, ins] : ins_at_) {
    if (!IsLinked(ins)) continue;  // unlinked by Step A (deleted moves)
    const auto& c = arch::GetInstructionConstraint(ins);
    if (c.is_no_constraints_marker) continue;

    InsPlan plan;
    plan.ins = ins;
    plan.point = pid;
    bool pushed = false;
    auto ensure = [&]() -> InsPlan* {
      if (!pushed) {
        plans_.push_back(std::move(plan));
        pushed = true;
        return &plans_.back();
      }
      return &plans_.back();
    };

    const auto P = pid;
    auto& claim = claimed_[ins];

    // Register sets the instruction kills beyond its own operands/output
    // (for calls the table folds the caller-saved sets into clobber_*).
    RegisterSet kill;
    kill.Union(c.clobber_gp);
    kill.Union(c.clobber_fp);
    kill.Union(c.scratch_gp);
    kill.Union(c.scratch_fp);

    std::size_t park_ord_gp = 0, park_ord_fp = 0;
    auto parked = std::make_unique<std::set<std::uint32_t>>();

    // (a) clobber/scratch eviction parks. Only intervals LIVE PAST the
    // instruction need a park: a value whose last point IS this
    // instruction is consumed before the clobber matters (an operand or a
    // checkpoint entry read by the instruction itself — both are uses
    // the validator excludes from clobber accountability).
    if (!kill.Empty()) {
      for (auto& iv : intervals_) {
        if (iv.assigned == nullptr) continue;  // spilled: holds no register
        if (!kill.Get(iv.assigned)) continue;
        // Only intervals LIVE PAST the instruction are parked: a value
        // whose last point IS this instruction is consumed before the
        // clobber matters (an operand or a checkpoint entry read by the
        // instruction itself — both are uses the validator excludes from
        // clobber accountability). This is the deopt-consistent policy:
        // unconditional parks around exit-bearing instructions move values
        // the baked deopt snapshots still reference at their old
        // locations (empirically: interpreter crashes in RefreshSideEffect).
        // G-01 layer 1: the coverage test routes through the one shared
        // predicate (the CFG-extended interval model); the DiesAtUse
        // refinement keeps the boundary: a last==P point reached by the
        // CFG extension only (the value is live OUT of the block, no use
        // at this instruction) IS live past it and must be parked.
        if (!IntervalLiveAt(iv, LivenessPoint{P})) continue;
        if (DiesAtUse(iv, LivenessPoint{P})) continue;
        if (parked->count(iv.vreg)) continue;
        // A constraint fix (argument landing, required-register operand)
        // owns the value's read AT the instruction, but a register-resident
        // interval LIVE PAST the instruction still must be preserved: the
        // instruction or its landing copy destroys the canonical register.
        // (Spilled intervals never reach this loop — assigned == nullptr
        // above — so the fix genuinely discharges only them.)

        bool fp = iv.assigned->IsFP();
        auto ordinal = fp ? park_ord_fp++ : park_ord_gp++;
        auto slot = ParkSlot(fp, ordinal);
        ensure()->parks.push_back(
            InsPlan::Park{iv.vreg, slot, true, true});
        // The parked register is CLAIMED at this instruction: phase-5
        // spill reloads must not target it (their mov would be clobbered
        // by the park store emitted immediately before the instruction).
        claim.Set(iv.assigned);
        location_fix_.emplace(std::make_pair(ins, iv.vreg),
                              CopySlot::FromSpill(slot));
        parked->insert(iv.vreg);
      }
    }

    // Occupant eviction: the required register r currently holds another
    // interval's live value — park it around this instruction (store
    // before, reload after; uses at this instruction read the park slot).
    // G-01 layer 1: same shared coverage predicate as the clobber filter —
    // inclusive at the last point (a value referenced AT the instruction
    // must survive the pre-instruction landing mov; the reload flag below
    // stays the anchor-park test: reload only when it outlives the point).
    auto evict_occupant = [&](const Register* r) -> base::Error {
      for (auto& iv : intervals_) {
        if (iv.assigned != r) continue;
        if (!IntervalLiveAt(iv, LivenessPoint{P})) continue;
        if (parked->count(iv.vreg)) continue;
        if (location_fix_.count(std::make_pair(ins, iv.vreg))) continue;

        bool fp = r->IsFP();
        auto ordinal = fp ? park_ord_fp++ : park_ord_gp++;
        auto slot = ParkSlot(fp, ordinal);
        ensure()->parks.push_back(
            InsPlan::Park{iv.vreg, slot, iv.last.point() > P, true});
        claim.Set(r);
        location_fix_.emplace(std::make_pair(ins, iv.vreg),
                              CopySlot::FromSpill(slot));
        parked->insert(iv.vreg);
      }
      return {};
    };

    // Two-operand anchor preservation: the instruction reads AND writes
    // the shared register, destroying the anchor value. When the anchor
    // outlives the instruction (and is not the output itself), park it
    // around the instruction WITHOUT an operand override — the operand
    // still reads the register at the instruction; the reload restores
    // the value for everything after it.
    auto park_anchor = [&](std::uint32_t v) {
      if (parked->count(v)) return;
      auto& iv = intervals_[v];
      bool fp = iv.assigned->IsFP();
      auto ordinal = fp ? park_ord_fp++ : park_ord_gp++;
      auto slot = ParkSlot(fp, ordinal);
      ensure()->parks.push_back(InsPlan::Park{v, slot, true, false});
      claim.Set(iv.assigned);
      parked->insert(v);
    };

    // Resolved location of an operand's value at this instruction,
    // hoisting it into a fresh register of the bank when it sits in a
    // slot (the kAnyRegister materialization move).
    auto hoist_to_register = [&](std::uint32_t v, bool fp, const char* what,
                                 CopySlot* out) -> base::Error {
      auto loc = LocationAt(v, ins);
      if (loc.IsRegister()) {
        *out = loc;
        return {};
      }
      auto kind = fp ? free_fp_.front()->register_kind()
                     : free_gp_.front()->register_kind();
      auto* r = FindFreeRegisterAt(kind, LivenessPoint{P}, claim);
      if (r == nullptr) {
        return base::Error::Mk(
            "linear-scan-ra: vreg %u (%s) needs a register at liveness "
            "point %u but the bank is exhausted (register pressure)",
            v, what, P);
      }
      claim.Set(r);
      auto* rv = RegView(r, intervals_[v].size_class);
      ensure()->pairs.push_back(
          CopyPair{loc, CopySlot::FromReg(rv), rv->size_class()});
      location_fix_.emplace(std::make_pair(ins, v), CopySlot::FromReg(rv));
      *out = CopySlot::FromReg(rv);
      return {};
    };

    // (b) operand constraints.
    const int opr_constrained = static_cast<int>(std::min(
        c.operands.size(), static_cast<std::size_t>(ins->opr_size())));
    for (int i = 0; i < opr_constrained; ++i) {
      const auto& oc = c.operands[static_cast<std::size_t>(i)];
      auto* opr = ins->Opr(i);
      if (opr == nullptr || !opr->IsTemporary()) continue;
      std::uint32_t v = 0;
      if (!vregs_->Lookup(opr, &v)) continue;

      switch (oc.kind) {
        case arch::ArchOperandKind::kNone:
          break;

        case arch::ArchOperandKind::kFixedRegister: {
          // The required register renders in the VALUE's size-class view
          // (legacy MaterializeNativeCallOperand parity): a dword int32
          // value lands in the dword view of the argument register, so
          // the landing pair never mixes widths.
          auto* rv = RegView(oc.fixed_register, intervals_[v].size_class);
          auto required = CopySlot::FromReg(rv);
          auto loc = LocationAt(v, ins);
          if (loc != required) {
            if (auto err = evict_occupant(oc.fixed_register); err) return err;
            ensure()->pairs.push_back(
                CopyPair{loc, required, rv->size_class()});
            location_fix_.emplace(std::make_pair(ins, v), required);
            claim.Set(oc.fixed_register);
          }
          break;
        }

        case arch::ArchOperandKind::kAnyRegister: {
          CopySlot hoisted;
          if (auto err = hoist_to_register(v, opr->htype() == HType::kFloat,
                                           "any-register operand", &hoisted);
              err) {
            return err;
          }
          break;
        }

        case arch::ArchOperandKind::kSameAsOperand: {
          const int j = static_cast<int>(oc.same_as_operand);
          if (j < 0 || j >= ins->opr_size()) break;
          auto* other = ins->Opr(j);
          if (other == nullptr || !other->IsTemporary()) break;
          std::uint32_t vj = 0;
          if (!vregs_->Lookup(other, &vj)) break;

          CopySlot required;
          if (auto err = hoist_to_register(vj, opr->htype() == HType::kFloat,
                                           "same-as anchor", &required);
              err) {
            return err;
          }

          auto loc = LocationAt(v, ins);
          if (loc != required) {
            if (required.IsRegister()) {
              if (auto err = evict_occupant(required.reg); err) return err;
            }
            ensure()->pairs.push_back(CopyPair{
                loc, required,
                required.IsRegister() ? required.reg->size_class()
                                      : SizeClass::kQWord});
            location_fix_.emplace(std::make_pair(ins, v), required);
          }
          break;
        }
      }
    }

    // (c) output constraints.
    if (ins->HasOutput() && ins->output() != nullptr &&
        ins->output()->IsTemporary()) {
      std::uint32_t vout = 0;
      if (vregs_->Lookup(ins->output(), &vout)) {
        auto canonical = LocationOf(vout);
        auto note_output_fixup = [&](CopySlot required) {
          // The instruction writes `required`; the interval's canonical
          // location receives it afterwards.
          if (canonical != required) {
            ensure()->post_movs.push_back(InsPlan::PostMov{required, canonical});
          }
        };

        switch (c.output.kind) {
          case arch::ArchOperandKind::kNone:
          case arch::ArchOperandKind::kAnyRegister:
            break;

          case arch::ArchOperandKind::kFixedRegister: {
            // Viewed in the interval's size class like every other
            // materialized register: the relocation mov then carries one
            // width on both sides.
            auto* rv = RegView(c.output.fixed_register,
                               intervals_[vout].size_class);
            auto required = CopySlot::FromReg(rv);
            if (canonical != required) {
              if (auto err = evict_occupant(c.output.fixed_register);
                  err) {
                return err;
              }
              location_fix_.emplace(std::make_pair(ins, vout), required);
              claim.Set(c.output.fixed_register);
              note_output_fixup(required);
            }
            break;
          }

          case arch::ArchOperandKind::kSameAsOperand: {
            const int j = static_cast<int>(c.output.same_as_operand);
            if (j < 0 || j >= ins->opr_size()) break;
            auto* other = ins->Opr(j);
            if (other == nullptr || !other->IsTemporary()) break;
            std::uint32_t vj = 0;
            if (!vregs_->Lookup(other, &vj)) break;

            CopySlot required;
            if (auto err = hoist_to_register(
                    vj, ins->output()->htype() == HType::kFloat,
                    "same-as-output anchor", &required);
                err) {
              return err;
            }

            if (vj != vout) {
              // Two-operand rule: operand j must PHYSICALLY sit in the
              // output's register at the instruction — land it there with
              // a pre-instruction pair when it lives elsewhere (the value
              // the instruction reads+writes is the SHARED register; the
              // anchor value must not survive there, so an occupant is
              // parked around the instruction).
              auto loc_j = LocationAt(vj, ins);
              if (loc_j != required) {
                if (required.IsRegister()) {
                  if (auto err = evict_occupant(required.reg); err) return err;
                }
                ensure()->pairs.push_back(CopyPair{
                    loc_j, required,
                    required.IsRegister() ? required.reg->size_class()
                                          : SizeClass::kQWord});
                if (required.IsRegister()) claim.Set(required.reg);
              }
              location_fix_.emplace(std::make_pair(ins, vj), required);
              // The instruction destroys the SHARED register. When the
              // anchor's canonical home IS that register and the value
              // outlives the instruction, preserve it around the
              // instruction (the anchor park reads no override; a hoisted
              // or relocated anchor keeps its own home untouched).
              if (intervals_[vj].assigned != nullptr &&
                  intervals_[vj].last.point() > P &&
                  required == CopySlot::FromReg(intervals_[vj].assigned)) {
                park_anchor(vj);
              }
            }
            if (canonical != required) {
              location_fix_.emplace(std::make_pair(ins, vout), required);
              note_output_fixup(required);
            }
            break;
          }
        }
      }
    }

    // (d) RCall argument landing (the RHS operand set lands in the ABI
    // argument registers simultaneously — the explicit parallel copy this
    // whole design exists for).
    if (c.is_call && ins->IsRCall()) {
      int gp = 0, fp = 0;
      for (int i = 1; i < ins->opr_size(); ++i) {
        auto* opr = ins->Opr(i);
        if (opr == nullptr || !opr->IsTemporary()) continue;
        std::uint32_t v = 0;
        if (!vregs_->Lookup(opr, &v)) continue;

        const bool is_fp = opr->htype() == HType::kFloat;
        // The hidden context argument shifts GP indices by one (the
        // legacy MapNativeCallParamIndexRCall contract).
        auto* arg = is_fp ? arch::GetCallArgRegister(fp++, true)
                          : arch::GetCallArgRegister(gp++ + 1, false);
        if (arg == nullptr) {
          return base::Error::Mk(
              "linear-scan-ra: vreg %u (rcall argument #%d) has no argument "
              "register (beyond the register ABI)",
              v, i);
        }
        // The argument register renders in the VALUE's size-class view
        // (legacy MaterializeNativeCallOperand parity).
        auto* rv = RegView(arg, intervals_[v].size_class);
        auto required = CopySlot::FromReg(rv);
        auto loc = LocationAt(v, ins);
        if (loc != required) {
          if (auto err = evict_occupant(arg); err) return err;
          ensure()->pairs.push_back(
              CopyPair{loc, required, rv->size_class()});
          location_fix_.emplace(std::make_pair(ins, v), required);
          claim.Set(arg);
        }
      }
    }

  }

  return {};
}

// ============================================================================
// Phase 5 — spill fixups: reloads before the use points of whole-range
// spilled intervals. The interval lives in its slot; each ordinary use is
// preceded by a reload mov (slot -> register), and the value at that
// instruction resolves through the reload register. Uses already
// discharged by the constraint plan (constraint pairs read the slot or the
// required register directly — the location fix is in place) are skipped.
// Layer 2 (plan 02.1-01, G-01 residual): a use referenced ONLY by a deopt
// checkpoint entry reads the canonical slot directly — no reload mov, no
// location fix — because a reload's register def/use pair can be split by
// a post-insertion block boundary and bridged across a loop by the
// validator's register-level fixpoint (crash_6.js #1138; the rationale is
// documented at the policy site in InsertSpillReloads below).

base::Error LinearScanRA::InsertSpillReloads() {
  for (const auto& iv : intervals_) {
    if (!iv.in_memory) continue;

    // Distinct use points only (the analysis records one point per use;
    // multiple operands of one instruction can repeat it).
    std::vector<std::uint32_t> points;
    for (auto p : liveness_->UsePoints(iv.vreg)) {
      if (points.empty() || points.back() != p.point()) {
        points.push_back(p.point());
      }
    }

    for (auto pid : points) {
      auto itr = ins_at_.find(pid);
      if (itr == ins_at_.end()) continue;  // defensive: stale point
      auto* ins = itr->second;
      if (!IsLinked(ins)) continue;        // deleted edge move

      // Is vreg referenced at this instruction at all — and HOW? An
      // ordinary operand use needs the value in a register (the reload
      // below). A deopt checkpoint entry reference does NOT: the entry
      // materializes through LocationAt -> the canonical spill slot, and
      // Snapshot::MapMachineRef encodes a MemStack operand as
      // Deopt::kSpill. Layer 2: when the reference is deopt-state-ONLY,
      // emit NO reload and record NO location_fix_. A reload here creates
      // a register def/use pair that post-insertion block boundaries can
      // separate (an insertion can turn the post-backedge instruction
      // into a rotated-header block leader, per the RAValidator's own
      // comment), and the validator's register-level CFG fixpoint then
      // bridges that register across the whole loop — the crash_6.js r9
      // clobber_gp of rcall_void #1138 residual.
      // No sound single-range interval extent from pre-RA analysis can
      // anticipate that bridged range; reading the entry from the slot
      // removes the register liveness entirely (monotonic shrink: deleting
      // a def together with its only bridged use cannot make any register
      // live anywhere it was not already live). The single-location model
      // guarantees the slot holds the value across the whole range:
      // member defs write the canonical location, park slots are distinct
      // slots, and constraint-owned uses are already skipped by the
      // location_fix_ check below.
      //
      // Deopt-state references arrive through TWO encodings (Rule-1
      // deviation, plan 02.1-01 Task 3 — the corpus shape found at the
      // #1138 site): (1) an exit-bearing instruction's checkpoint entries
      // (ins->exit()->ForeachEntry), and (2) a kSnapshot instruction's
      // ORDINARY operands — LIRLower materializes the checkpoint's entry
      // list 1:1 into the snapshot's operand list (lir-lower.cc
      // TransSnapshot; the opcode emits no code and exists only to hold
      // the references). Both count as the same use kind here.
      bool referenced_as_operand = false;
      bool referenced_as_deopt_state = false;
      for (int i = 0; i < ins->opr_size(); ++i) {
        auto* opr = ins->Opr(i);
        if (opr == nullptr || !opr->IsTemporary()) continue;
        std::uint32_t ov = 0;
        if (vregs_->Lookup(opr, &ov) && ov == iv.vreg) {
          if (ins->opcode() == HOpcode::kSnapshot) {
            referenced_as_deopt_state = true;
          } else {
            referenced_as_operand = true;
          }
        }
      }
      if (ins->HasExit()) {
        ins->exit()->ForeachEntry([&](const ir::Checkpoint::Entry& e) {
          if (e.value() == nullptr) return;
          auto* opr = e.value()->lir_operand();
          std::uint32_t ov = 0;
          if (opr != nullptr && opr->IsTemporary() &&
              vregs_->Lookup(opr, &ov) && ov == iv.vreg) {
            referenced_as_deopt_state = true;
          }
        });
      }

      // The constraint plan already owns this use (its pair reads the
      // slot, or lands the value in a required register).
      if (location_fix_.count(std::make_pair(ins, iv.vreg))) continue;

      // Layer 2: deopt-state-ONLY references read the canonical slot
      // directly — no reload mov, no location fix (LocationAt falls back
      // to LocationOf). Use-kind-driven, NOT position-driven: no rotated-
      // header or block-leader detection (pre-RA position analysis is
      // unsound for that shape — 02.1-01-HARD-STOP.md — and position
      // detection is what made the naive variant of this family
      // indistinguishable from reverted mitigation #2). The existing
      // machinery already covers the interaction with a preceding
      // parallel copy (MaterializeExit rewrites a pure-source slot
      // reference to the pair destination; CheckExitBoundary then sees a
      // destination, not a pure source) — no sane logic is added here.
      if (!referenced_as_operand && referenced_as_deopt_state) continue;

      if (!referenced_as_operand && !referenced_as_deopt_state) continue;

      // The reload register is CLAIMED at this instruction: several
      // spilled intervals can share one use instruction (a checkpoint
      // references every spilled slot value at its side exit), and without
      // the claim every reload picks the SAME first free register —
      // overwriting the previous reload and aliasing distinct values.
      auto& claim = claimed_[ins];
      auto* r = FindFreeRegisterAt(iv.kind, LivenessPoint{pid}, claim);
      if (r == nullptr) {
        continue;
      }
      claim.Set(r);

      auto* slot_opr = as_->NewMemStack(iv.spill);
      slot_opr->set_instruction(LinkFor(iv.vreg, ins));

      auto* reload = as_->NewInstructionBefore(
          nullptr, SpillMoveOpcode(iv.kind), slot_opr, ins);
      auto* rv = RegView(r, iv.size_class);
      reload->SetRAOutput(as_->NewRegister(rv));

      location_fix_.emplace(std::make_pair(ins, iv.vreg),
                            CopySlot::FromReg(rv));

    }
  }

  return {};
}

// ============================================================================
// Phase 6 — emission: park stores, the constraint parallel copy, and the
// post-instruction movs (output relocations, park reloads), in that order
// around the planned instruction.

void LinearScanRA::EmitPlans() {
  for (auto& plan : plans_) {
    auto* ins = plan.ins;

    // (1) park stores: reg -> slot, before the instruction.
    for (const auto& park : plan.parks) {
      const auto& iv = intervals_[park.vreg];
      auto* src_opr = as_->NewRegister(ViewOf(iv));
      src_opr->set_instruction(LinkFor(iv.vreg, ins));
      auto* dst_opr = as_->NewMemStack(park.slot);
      dst_opr->set_instruction(LinkFor(iv.vreg, ins));

      auto* store = as_->NewInstructionBefore(
          nullptr, SpillMoveOpcode(iv.kind), src_opr, ins);
      store->SetRAOutput(dst_opr);
    }

    // (2) the constraint parallel copy: pair list sequentialized per bank
    // (the resolver's step order — itself a correct sequential program).
    if (!plan.pairs.empty()) {
      std::vector<bool> pair_is_fp;
      for (const auto& p : plan.pairs) {
        pair_is_fp.push_back(p.dst.IsFP());
      }
      auto ordered = SequentializePairs(LivenessPoint{plan.point}, plan.pairs,
                                        pair_is_fp, claimed_[ins]);
      zone::Vector<CopyPair> zone_ordered{zone()};
      for (const auto& p : ordered) zone_ordered.push_back(p);
      NewParallelCopyBefore(as_, ins, zone_ordered);
    }

    // (3) post-instruction movs, in plan order.
    auto* after = ins;
    auto emit_mov = [&](CopySlot src, CopySlot dst, bool fp,
                        HInstruction* link) {
      HOperand* src_opr = nullptr;
      HOperand* dst_opr = nullptr;
      if (src.IsRegister()) {
        src_opr = as_->NewRegister(src.reg);
      } else {
        src_opr = as_->NewMemStack(src.spill_slot);
      }
      if (dst.IsRegister()) {
        dst_opr = as_->NewRegister(dst.reg);
      } else {
        dst_opr = as_->NewMemStack(dst.spill_slot);
      }
      // Materialized operands carry a producer link (the htype()/print
      // contract) — the anchored instruction is the value's site here.
      src_opr->set_instruction(link);
      dst_opr->set_instruction(link);
      auto* mov = as_->NewInstructionAfter(nullptr, SpillMoveOpcode(fp),
                                           src_opr, after);
      mov->SetRAOutput(dst_opr);
      after = mov;
    };

    for (const auto& mov : plan.post_movs) {
      emit_mov(mov.src, mov.dst, mov.src.IsFP() || mov.dst.IsFP(), ins);
    }
    for (const auto& park : plan.parks) {
      if (!park.reload) continue;
      const auto& iv = intervals_[park.vreg];
      emit_mov(CopySlot::FromSpill(park.slot),
               CopySlot::FromReg(ViewOf(iv)), iv.assigned->IsFP(),
               LinkFor(park.vreg, ins));
    }

  }
}

// ============================================================================
// Phase 7 — materialization

namespace {

// Abstract-slot view of a materialized operand for the boundary fix
// (non-location operands map to a distinguished null-register slot the
// comparisons skip).
CopySlot SlotOfOperand(const HOperand* o) {
  if (o->IsRegister()) return CopySlot::FromReg(o->GetRegister());
  if (o->IsMemStack()) return CopySlot::FromSpill(o->GetMemStack().slot());
  CopySlot none;
  none.kind = CopySlot::kRegister;
  none.reg = nullptr;
  return none;
}

}  // namespace

HOperand* LinearScanRA::MakeOperand(const CopySlot& slot, std::uint32_t vreg,
                                  HInstruction* ins) {
  HOperand* opr;
  if (slot.IsRegister()) {
    opr = as_->NewRegister(slot.reg);
  } else {
    opr = as_->NewMemStack(slot.spill_slot);
  }
  // The legacy RegisterAllocation::ToOperand contract: materialized
  // operands carry their producer link.
  opr->set_instruction(LinkFor(vreg, ins));
  return opr;
}

base::Error LinearScanRA::MaterializeExit(HInstruction* ins) {
  auto* exit = ins->exit();

  Snapshot::SSAVarMappings mappings;
  exit->ForeachEntry([&](const ir::Checkpoint::Entry& entry) {
    auto* ssa_var = entry.value();
    if (ssa_var == nullptr) return;
    auto* opr = ssa_var->lir_operand();
    if (opr == nullptr) return;  // undefined/null/uninitialized/immediate

    HOperand* new_opr;
    std::uint32_t v = 0;
    if (opr->IsTemporary() && vregs_->Lookup(opr, &v)) {
      new_opr = MakeOperand(LocationAt(v, ins), v, ins);
    } else {
      new_opr = opr;
    }

    // a parallel copy directly preceding this instruction means the
    // snapshot must describe the POST-copy boundary — a location the copy
    // reads but never writes (a pure source slot) is stale by the time the
    // exit can fire, so rewrite it to the pair's destination.
    if (auto* prev = ins->prev();
        prev != nullptr && prev->opcode() == HOpcode::kParallelCopy) {
      auto loc = SlotOfOperand(new_opr);
      if (loc.reg != nullptr || new_opr->IsMemStack()) {
        for (int i = 0; i < ParallelCopyPairCount(prev); ++i) {
          auto src_loc = SlotOfOperand(ParallelCopySrc(prev, i));
          auto dst_loc = SlotOfOperand(ParallelCopyDst(prev, i));
          if (src_loc == loc && dst_loc != loc) {
            new_opr = ParallelCopyDst(prev, i);
            loc = dst_loc;
          }
        }
      }
    }

    mappings.insert(std::make_pair(ssa_var, new_opr));
  });

  Snapshot snapshot;
  snapshot.Construct(mappings, *exit);
  auto did = quake_->AddDeopt(Snapshot::MapDeoptReason(exit->reason()),
                              DeoptNext::kContinueInterp, std::move(snapshot));
  ins->set_deopt_id(did);
  return {};
}

base::Error LinearScanRA::Materialize() {
  base::Error err;
  as_->ForeachInstruction([&](HInstruction* ins) {
    if (err) return;

    for (int i = 0; i < ins->opr_size(); ++i) {
      auto* opr = ins->Opr(i);
      if (opr == nullptr || !opr->IsTemporary()) continue;
      std::uint32_t v = 0;
      if (!vregs_->Lookup(opr, &v)) {
        err = base::Error::Mk(
            "linear-scan-ra: operand #%d of instruction #%u carries no "
            "vreg at materialization",
            i, ins->id());
        return;
      }
      ins->OprSet(i, MakeOperand(LocationAt(v, ins), v, ins));
    }

    if (ins->HasOutput() && ins->output() != nullptr &&
        ins->output()->IsTemporary()) {
      std::uint32_t v = 0;
      if (!vregs_->Lookup(ins->output(), &v)) {
        err = base::Error::Mk(
            "linear-scan-ra: output of instruction #%u carries no vreg at "
            "materialization",
            ins->id());
        return;
      }
      // Every member def of a multi-def vreg writes the interval's own
      // (canonical) location — in this single-location model LocationAt
      // answers that for every def site, which is exactly the legacy
      // edge-write contract (ra-materialize.cc:244-260).
      ins->SetRAOutput(MakeOperand(LocationAt(v, ins), v, ins));
    }

    if (ins->HasExit()) {
      err = MaterializeExit(ins);
    }
  });
  if (err) return err;

  // Publish the frame layout exactly like the legacy materializer.
  quake_->set_frame_layout(*as_->frame_layout());
  return {};
}

// ============================================================================
// Entry

base::Error LinearScanRA::Run() {
  if (auto err = BuildIntervals(); err) return err;
  if (auto err = Scan(); err) return err;
  if (auto err = ConvertEdgeMoves(); err) return err;
  if (auto err = PlanConstraints(); err) return err;
  if (auto err = InsertSpillReloads(); err) return err;
  EmitPlans();
  if (auto err = Materialize(); err) return err;
  return {};
}

}  // namespace lir
}  // namespace quake
