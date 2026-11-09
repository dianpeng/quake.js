#include "lir/vreg-pass.h"

#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"

#include "ir/node.h"
#include "ir/checkpoint.h"
#include "ir/ssa-var.h"

#include "quake.h"

#include <algorithm>
#include <utility>
#include <vector>

#include <unordered_map>

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// VregContext
//
// The side table is the ONLY vreg identity: Temporary stays untouched (still
// one def per operand), the id lives here. Zone-allocated, dies with the
// compile's zone Clear — nothing escapes (Pitfall 3).

std::uint32_t VregContext::GetOrNewVreg(HOperand* opr) {
  CHECK(opr);
  if (auto itr = operand_map_.find(opr); itr != operand_map_.end()) {
    return itr->second;
  }

  auto id = vreg_count();
  operand_map_.emplace(opr, id);

  // Grow the def-record storage strictly by emplace_back so the inner
  // zone::Vector is constructed with the zone (a resize() would
  // value-initialize inner vectors carrying a null ZoneAllocator)
  defs_.emplace_back(zone_);
  CHECK(defs_.size() == static_cast<std::size_t>(id) + 1);
  return id;
}

std::uint32_t VregContext::MapAlias(HOperand* opr, std::uint32_t vreg_id) {
  CHECK(opr);
  CHECK_LT(vreg_id, vreg_count());

  auto [itr, ok] = operand_map_.emplace(opr, vreg_id);
  CHECK(ok) << "aliasing an operand that already carries a vreg";
  return vreg_id;
}

bool VregContext::Lookup(HOperand* opr, std::uint32_t* vreg_id) const {
  CHECK(opr);
  if (auto itr = operand_map_.find(opr); itr != operand_map_.end()) {
    *vreg_id = itr->second;
    return true;
  }
  return false;
}

void VregContext::AddDef(std::uint32_t vreg_id, HInstruction* def) {
  CHECK_LT(vreg_id, vreg_count());
  CHECK(def);
  defs_[vreg_id].push_back(def);
}

const zone::Vector<HInstruction*>& VregContext::GetDefs(
    std::uint32_t vreg_id) const {
  CHECK_LT(vreg_id, vreg_count());
  return defs_[vreg_id];
}

void VregContext::AddEdgeMove(HInstruction* move, EdgeMoveGroup group) {
  CHECK(move);
  DCHECK(move->IsMove());
  edge_moves_.push_back(EdgeMoveRecord{move, group});
}

void VregContext::MergeVregs(std::uint32_t from, std::uint32_t to) {
  CHECK_LT(from, vreg_count());
  CHECK_LT(to, vreg_count());
  CHECK_NE(from, to) << "merging a vreg into itself is a no-op caller error";

  // Re-point every operand_map_ entry equal to `from` at `to`.
  for (auto& entry : operand_map_) {
    if (entry.second == from) {
      entry.second = to;
    }
  }

  // Union def records: append `from`'s defs into `to`'s def vector.
  for (auto* def : defs_[from]) {
    defs_[to].push_back(def);
  }
  // `from`'s defs are left in place (holes permitted); no compaction.
}

// ----------------------------------------------------------------------------
// Helpers

namespace {

// kMov vs kMovF is picked by the phi's opcode family: only the F flavors
// live in the floating point register file, everything else (I/I64/Ptr/Box)
// moves through the general purpose one.
HOpcode MoveOpcodeForPhi(HInstruction* phi) {
  switch (phi->opcode()) {
    case HOpcode::kPhiLoopF:
    case HOpcode::kPhiBranchF:
      return HOpcode::kMovF;

    default:
      return HOpcode::kMov;
  }
}

}  // namespace

// ----------------------------------------------------------------------------
// VregPass

VregPass::VregPass(Quake* quake, HAssembler* as) : quake_(quake), as_(as) {}

zone::Zone* VregPass::zone() const { return quake_->zone(); }

void VregPass::RemovePhiRedirectingUses(HInstruction* phi,
                                        HOperand* canonical) {
  CHECK(phi->IsPhi());
  CHECK(canonical->IsTemporary());

  // The canonical operand's def must be one of the live edge moves, never
  // the phi we are about to unlink
  auto* canonical_def = canonical->instruction_checked();
  CHECK(canonical_def->IsMove());
  CHECK_NE(canonical_def, phi);

  // Side-exit snapshot entries reference values by SSAVar*; when a phi is
  // the snapshot's value, `ssa_var->lir_operand()` is the phi output. We
  // must re-point the SSAVar at the canonical (live) move output BEFORE
  // removing the phi — otherwise after RA, MaterializeExit reads an
  // SSAVar whose lir_operand is a removed phi and CHECK-fails
  // (no-live-interval / dangling instruction).
  //
  // The backlink (opr->ir_var_) is per-operand state and last-writer-wins:
  // it may be null (loop phis are lowered through the pending list and may
  // never claim it) or may point at a different claimant of the same
  // operand. The phi's OWN defining node's out var is therefore redirected
  // explicitly — ir::Node::out() is authoritative regardless of backlink
  // history, and def-site lowering is one-to-one so the node out plus the
  // backlink cover every legitimate claimant.
  if (auto* phi_ir_var = phi->output()->ir_var()) {
    phi_ir_var->set_lir_operand(canonical);
  }
  // ir_node() is null on hand-sculpted fixture streams (nullptr ir::Node*
  // is the documented fixture convention); guard before dereferencing.
  if (auto* phi_node = phi->ir_node()) {
    if (auto* phi_node_out = phi_node->out()) {
      if (phi_node_out->lir_operand() == phi->output()) {
        phi_node_out->set_lir_operand(canonical);
      }
    }
  }

  // The backlink/node-out fixups above only cover the phi's OWN claimant
  // SSAVars. A checkpoint entry maps an interpreter-slot SSAVar onto the
  // operand holding the slot's value at the side exit, and that pointer is
  // not a registered use of the phi output: an entry whose slot's current
  // value IS the phi references phi->output() through its own SSAVar,
  // unrelated to the phi's backlink. Left alone, the entry keeps a dangling
  // reference into the removed phi — the classical liveness pass (correctly)
  // counts it as a use of the phi vreg, and the RA downstream meets a
  // def-less interval on the live compile path. Re-point every such entry.
  as_->ForeachInstruction([&](HInstruction* ins) {
    if (ins == nullptr || !ins->HasExit()) return;
    ins->exit()->ForeachEntry([&](ir::Checkpoint::Entry& e) {
      if (e.value() != nullptr && e.value()->lir_operand() == phi->output()) {
        e.value()->set_lir_operand(canonical);
      }
    });
  });

  // Replace() rewrites every registered use slot onto the canonical operand
  // (the def link of every user now points at a live move — no dangling
  // Temporary::instr_ into the removed phi, T-04-01) and re-points the phi
  // output's ir_var_, which is the other half of the snapshot deopt link
  // (T-04-02).
  phi->output()->Replace(canonical);
  CHECK(!phi->output()->HasUse());

  phi->Remove();
}

void VregPass::EliminateLoopPhi(HInstruction* phi, VregContext* ctx,
                                HOperand** out_entry_move,
                                HOperand** out_backedge_move) {
  CHECK(phi->IsLoopPhi());

  // Loop phi: EXACTLY 2 value operands — Opr(0) = entry/initial value,
  // Opr(1) = backedge value (the old "4 operands" comment in the register
  // allocator was stale; branch phis are the 4-operand form)
  CHECK_EQ(phi->opr_size(), 2);

  auto vreg = ctx->GetOrNewVreg(phi->output());
  auto mov_opcode = MoveOpcodeForPhi(phi);

  // (1) entry edge — the shuffle slot at the end of BB0, ahead of the BB1
  // marker. This move's output is the phi vreg's CANONICAL operand: every
  // user is redirected onto it below.
  auto* entry_move = as_->NewInstructionBefore(
      phi->ir_node(), mov_opcode, phi->Opr(0), as_->bb0_register_shuffle());
  ctx->MapAlias(entry_move->output(), vreg);
  ctx->AddDef(vreg, entry_move);
  ctx->AddEdgeMove(entry_move, EdgeMoveGroup::kEntry);

  // (2) backedge — the shuffle slot just before the unconditional loop back
  // jump. Its output is a second Temporary aliased onto the same vreg; on
  // the Temporary-based legacy allocator it forms the (documented, inert)
  // use-free [def,def] record that the whole-VREG single-interval model
  // (one interval spanning every def and use) treats uniformly.
  auto* backedge_move = as_->NewInstructionBefore(
      phi->ir_node(), mov_opcode, phi->Opr(1), as_->bb1_register_shuffle());
  ctx->MapAlias(backedge_move->output(), vreg);
  ctx->AddDef(vreg, backedge_move);
  ctx->AddEdgeMove(backedge_move, EdgeMoveGroup::kBackedge);

  // (3) redirect every use to the canonical (entry-edge) def and unlink the
  // phi — zero PHI instructions survive the pass (SSA-03)
  RemovePhiRedirectingUses(phi, entry_move->output());

  // Return move outputs through output params. The caller must NOT derive
  // these from anchor->prev() after elimination: once the phi is unlinked,
  // reading the phi's link fields is UB.
  if (out_entry_move) *out_entry_move = entry_move->output();
  if (out_backedge_move) *out_backedge_move = backedge_move->output();
}

void VregPass::EliminateBranchPhi(HInstruction* phi, VregContext* ctx,
                                  HOperand** out_lhs_move,
                                  HOperand** out_rhs_move) {
  CHECK(phi->IsBranchPhi());

  // Branch phi: 4 operands [lhs, rhs, label_lhs, label_rhs]; the labels bind
  // to the LAST instruction of each predecessor arm
  CHECK_EQ(phi->opr_size(), 4);
  CHECK(phi->Opr(2)->IsLabel());
  CHECK(phi->Opr(3)->IsLabel());

  auto* lhs_label_ins = phi->Opr(2)->GetLabel()->bind();
  auto* rhs_label_ins = phi->Opr(3)->GetLabel()->bind();
  CHECK(lhs_label_ins);
  CHECK(rhs_label_ins);

  auto vreg = ctx->GetOrNewVreg(phi->output());
  auto mov_opcode = MoveOpcodeForPhi(phi);

  // One move per predecessor arm, at the arm's tail — label->next() is the
  // first slot after the arm's last instruction, so inserting before it puts
  // the move on the edge itself — the same insertion sites the retired
  // branch-phi shuffle used.
  auto* lhs_anchor = lhs_label_ins->next();
  CHECK(lhs_anchor);
  auto* lhs_move = as_->NewInstructionBefore(phi->ir_node(), mov_opcode,
                                             phi->Opr(0), lhs_anchor);
  ctx->MapAlias(lhs_move->output(), vreg);
  ctx->AddDef(vreg, lhs_move);
  ctx->AddEdgeMove(lhs_move, EdgeMoveGroup::kBranchPhi);

  auto* rhs_anchor = rhs_label_ins->next();
  CHECK(rhs_anchor);
  auto* rhs_move = as_->NewInstructionBefore(phi->ir_node(), mov_opcode,
                                             phi->Opr(1), rhs_anchor);
  ctx->MapAlias(rhs_move->output(), vreg);
  ctx->AddDef(vreg, rhs_move);
  ctx->AddEdgeMove(rhs_move, EdgeMoveGroup::kBranchPhi);

  // The lhs arm's output is the canonical operand (either arm is sound; the
  // canonical pick only fixes which def users reference afterwards)
  RemovePhiRedirectingUses(phi, lhs_move->output());

  // Return move outputs through output params. When the phi is itself the
  // anchor (sits immediately after the edge label), it is unlinked by
  // RemovePhiRedirectingUses and reading anchor->prev_ is UB — callers must
  // use these returned pointers instead (root-cause of crash_6.js segfault).
  if (out_lhs_move) *out_lhs_move = lhs_move->output();
  if (out_rhs_move) *out_rhs_move = rhs_move->output();
}

VregContext* VregPass::Run() {
  auto* ctx = zone()->New<VregContext>(zone());

  // (0) Collect every PHI via ForeachInstruction + IsPhi(). Do NOT use the
  // phi-section iterator: the phi-section pointers are only wired by
  // LIRLower+Finalize, so section-based walks see zero phis on hand-built
  // fixtures — the predicate walk works on real and fixture streams alike
  // (Pitfall 7). Collecting first also keeps iteration safe while the
  // stream is mutated afterwards.
  std::vector<HInstruction*> phis;
  as_->ForeachInstruction([&](HInstruction* ins) {
    if (ins->IsPhi()) phis.push_back(ins);
  });

  // Debug guard (T-04-03): Phase 1 emits each edge's move group
  // SEQUENTIALLY, which is only sound while no group holds two moves
  // targeting the same vreg (a real swap cycle would silently corrupt two
  // induction variables — the retired entry-edge shuffle's known bug class).
  // Genuine
  // parallel-copy cycles are Phase 2's problem (PCOPY-01); this CHECK flags
  // them instead of silently accepting.
  [[maybe_unused]] std::vector<std::pair<HInstruction*, std::uint32_t>>
      edge_move_records;
  auto record_edge_move = [&](HInstruction* anchor, HOperand* target) {
    BASE_DBG({
      std::uint32_t vreg = 0;
      CHECK(ctx->Lookup(target, &vreg));
      for (const auto& record : edge_move_records) {
        CHECK(record.first != anchor || record.second != vreg)
            << "edge move group holds two moves targeting the same vreg — "
            << "sequential emission would corrupt it (parallel copy is "
            << "Phase 2 PCOPY-01)";
      }
      edge_move_records.emplace_back(anchor, vreg);
    });
  };

  std::vector<HInstruction*> removed;
  // Live successor of each removed phi, captured while the phi is still
  // linked — HInstruction::Remove NULLs the removed node's own prev_/next_,
  // so OnPhiRemoved cannot recover the successor from the stream afterwards
  // (it rebinds phi-bound labels through this map).
  std::unordered_map<HInstruction*, HInstruction*> successor_of_removed;

  // (1) loop phis — entry-edge + backedge moves. The insertion points are
  // the two dedicated shuffle slots sitting on the only two predecessor
  // edges of the loop head; no critical edge is ever split, no label or
  // block is created (SSA-02).
  for (auto* phi : phis) {
    if (!phi->IsLoopPhi()) continue;

    successor_of_removed.emplace(phi, phi->next());
    HOperand *entry_move = nullptr, *backedge_move = nullptr;
    EliminateLoopPhi(phi, ctx, &entry_move, &backedge_move);
    removed.push_back(phi);

    record_edge_move(as_->bb0_register_shuffle(), entry_move);
    record_edge_move(as_->bb1_register_shuffle(), backedge_move);
  }

  // (2) branch phis — one move at each predecessor arm's tail. The arm
  // anchors are captured BEFORE the elimination: Remove() clears the phi's
  // operand slots, so Opr(2)/Opr(3) are gone once it ran.
  //
  // NOTE: the move outputs are returned through output params, not derived
  // from anchor->prev(). When the phi is itself the anchor (sits
  // immediately after the edge label — common on real traces with a
  // phi-as-first-instruction-of-join-block), the phi is unlinked by
  // RemovePhiRedirectingUses and reading anchor->prev_ after elimination is
  // UB (root-cause of crash_6.js SIGSEGV: phi->prev_ read as null/garbage).
  for (auto* phi : phis) {
    if (!phi->IsBranchPhi()) continue;

    auto* lhs_anchor = phi->Opr(2)->GetLabel()->bind()->next();
    auto* rhs_anchor = phi->Opr(3)->GetLabel()->bind()->next();

    successor_of_removed.emplace(phi, phi->next());
    HOperand *lhs_move = nullptr, *rhs_move = nullptr;
    EliminateBranchPhi(phi, ctx, &lhs_move, &rhs_move);
    removed.push_back(phi);

    record_edge_move(lhs_anchor, lhs_move);
    record_edge_move(rhs_anchor, rhs_move);
  }

  // (3) the assembler caches phi-section state (loop_phi_start_,
  // branch_phi_list_) that would dangle after the removals; let it
  // re-derive a consistent (empty or remaining) phi section
  as_->OnPhiRemoved(removed, successor_of_removed);

  // (4) fresh vreg per unseen SSA value: every surviving value
  // instruction's output Temporary. The inserted moves' outputs are already
  // aliased onto their phi vregs — GetOrNewVreg is idempotent for them, and
  // each ordinary value records its single def here
  as_->ForeachInstruction([&](HInstruction* ins) {
    CHECK(!ins->IsPhi()) << "PHI survived SSA destruction";

    if (auto* out = ins->output(); out && out->IsTemporary()) {
      std::uint32_t vreg = 0;
      if (!ctx->Lookup(out, &vreg)) {
        vreg = ctx->GetOrNewVreg(out);
        ctx->AddDef(vreg, ins);
      }
    }
  });

  return ctx;
}

}  // namespace lir
}  // namespace quake
