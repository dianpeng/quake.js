// Structural-invariant unit tests for the vreg pass (SSA-01..03, D-13/D-14).
//
// The pass ships INERT — it is NOT wired into Quake::DoCompile (the single
// gate-gated cutover is plan 06 Task 1) — so these tests exercise it the
// only correct way available: by direct construction on HAND-BUILT
// HAssembler streams (D-13), asserting structural invariants by walking
// the stream and querying the returned VregContext (D-14, no golden text
// dumps, expected move counts asserted numerically per edge site).
//
// Fixtures never call Print() on hand-built instructions (nullptr ir::Node*
// is stored, only dereferenced by trace/print paths).

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <quake.h>

#include <lir/assembler.h>
#include <lir/instruction.h>
#include <lir/opcode.h>
#include <lir/operand.h>
#include <lir/vreg-pass.h>

#include <unittest/ir/qjs-bytecode-test-util.h>

#include <memory>
#include <set>
#include <vector>

namespace quake {
namespace lir {
namespace {

// ----------------------------------------------------------------------------
// Fixture (D-13): Quake needs a live JSContext (its ctor interns JSStrings),
// cheapest proven source is the qjs-bytecode test util. Zone-backed state
// (HAssembler instructions, VregContext) lives on the Quake zone.

struct VregFixture {
  std::unique_ptr<ir::QJSFunctionBytecode> mock;
  std::unique_ptr<QuakeWorld> world;
  std::unique_ptr<Quake> quake;
  std::unique_ptr<HAssembler> as;

  VregFixture() {
    mock = std::make_unique<ir::QJSFunctionBytecode>();
    mock->SetUp();  // JS runtime + context

    world = std::make_unique<QuakeWorld>(nullptr);
    quake = std::make_unique<Quake>(world.get(), mock->js_context());
    as = std::make_unique<HAssembler>(quake.get());
  }

  // Append helpers — everything goes before the sentry, ie at the stream's
  // end, mirroring how the private emitters build real streams.
  HInstruction* Append(HOpcode opcode) {
    return as->NewInstructionBefore(nullptr, opcode, as->end_instruction());
  }

  HInstruction* Append1(HOpcode opcode, HOperand* opr) {
    return as->NewInstructionBefore(nullptr, opcode, opr,
                                    as->end_instruction());
  }

  HInstruction* AppendN(HOpcode opcode, std::size_t operand_size) {
    return as->NewInstructionBefore(nullptr, opcode, operand_size,
                                    as->end_instruction());
  }
};

std::size_t CountPhi(HAssembler* as) {
  std::size_t count = 0;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins->IsPhi()) count++;
  });
  return count;
}

// Count plain integer moves whose input is exactly `input`
std::size_t CountMovOf(HAssembler* as, HOperand* input) {
  std::size_t count = 0;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins->opcode() == HOpcode::kMov && ins->Opr(0) == input) count++;
  });
  return count;
}

bool IsInStream(HAssembler* as, HInstruction* target) {
  bool found = false;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins == target) found = true;
  });
  return found;
}

std::size_t CountMovTotal(HAssembler* as) {
  std::size_t count = 0;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins->opcode() == HOpcode::kMov) count++;
  });
  return count;
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: loop phi elimination", "[vreg][ssa]") {
  VregFixture fx;
  auto* as = fx.as.get();

  // [kStart][entry=kMovI32 #0][kBB1][upd=kDupI(entry)][user][back=kJmp]
  // [kEnd][kPhiLoopI(entry, upd)][sentry]
  as->TestStart();
  auto* entry_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* bb1_label = as->TestBB1();  // kBB1 marker == bb0_register_shuffle()
  auto* upd_def = fx.Append1(HOpcode::kDupI, entry_def->output());
  auto* user = fx.AppendN(HOpcode::kDupI, 1);  // operand wired below
  auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi->OprSet(0, entry_def->output());  // entry/initial value
  phi->OprSet(1, upd_def->output());    // backedge value
  phi->PopulateOutput(as);
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);

  as->TestFinalize();  // loop_phi_start_ -> the phi (phi section wired)

  // the fixture is meaningful: exactly one loop phi pre-pass
  REQUIRE(CountPhi(as) == 1);
  REQUIRE(as->phi_size() == 1);

  VregPass pass{fx.quake.get(), as};
  VregContext* ctx = pass.Run();

  // (a) SSA-03 — zero PHI instructions survive, phi section observes empty
  REQUIRE(CountPhi(as) == 0);
  REQUIRE(as->phi_size() == 0);

  // (b) exactly one kMov per predecessor edge, at the two well-known sites:
  // entry move directly before the kBB1 marker (= bb0_register_shuffle()),
  // backedge move directly before the loop back jump (= loop_back()).
  auto* bb0_site = as->bb0_register_shuffle();
  REQUIRE(bb0_site == as->bb1());  // the kBB1 marker
  auto* entry_move = bb0_site->prev();
  REQUIRE(entry_move->opcode() == HOpcode::kMov);
  REQUIRE(entry_move->Opr(0) == entry_def->output());

  std::size_t bb0_move_count = 0;
  as->ForeachInstructionBB0([&](HInstruction* ins) {
    if (ins->opcode() == HOpcode::kMov) {
      REQUIRE(ins->Opr(0) == entry_def->output());
      bb0_move_count++;
    }
  });
  REQUIRE(bb0_move_count == 1);

  auto* backedge_site = as->bb1_register_shuffle();
  REQUIRE(backedge_site == back);  // loop_back() == end()->prev()
  auto* backedge_move = backedge_site->prev();
  REQUIRE(backedge_move->opcode() == HOpcode::kMov);
  REQUIRE(backedge_move->Opr(0) == upd_def->output());
  REQUIRE(CountMovOf(as, upd_def->output()) == 1);

  // no critical-edge splitting: nothing but the two moves was inserted
  REQUIRE(CountMovTotal(as) == 2);

  // (c) use redirection — the user now resolves to the CANONICAL (entry
  // edge) move output; the def link points at a live move, never the phi
  REQUIRE(user->Opr(0) == entry_move->output());
  REQUIRE(user->Opr(0)->instruction() == entry_move);
  REQUIRE(IsInStream(as, entry_move));
  REQUIRE(!IsInStream(as, phi));

  // (d) vreg identity — phi output and BOTH move outputs share one id
  std::uint32_t phi_vreg = 0, entry_vreg = 0, backedge_vreg = 0;
  REQUIRE(ctx->Lookup(phi->output(), &phi_vreg));
  REQUIRE(ctx->Lookup(entry_move->output(), &entry_vreg));
  REQUIRE(ctx->Lookup(backedge_move->output(), &backedge_vreg));
  REQUIRE(phi_vreg == entry_vreg);
  REQUIRE(entry_vreg == backedge_vreg);

  // def records: the phi vreg's defs are exactly the two inserted moves
  const auto& defs = ctx->GetDefs(phi_vreg);
  REQUIRE(defs.size() == 2);
  REQUIRE(((defs[0] == entry_move && defs[1] == backedge_move) ||
           (defs[0] == backedge_move && defs[1] == entry_move)));
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: branch phi elimination", "[vreg][ssa]") {
  VregFixture fx;
  auto* as = fx.as.get();

  // Fast/slow arm shape (exactly what HAssembler::FastSlow emits): fast arm
  // tail marker + jump, slow arm tail marker, then the merge user. The phi
  // labels bind to the LAST instruction of each predecessor arm.
  //
  // [kStart][src][lhs=kDupI(src)][lhs_block=kBlock][jmp merge]
  // [rhs=kDupI(src)][rhs_block=kBlock][user][kPhiBranchI][sentry]
  as->TestStart();
  auto* src_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(7)));
  auto* lhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* lhs_block = fx.Append(HOpcode::kBlock);
  auto* jmp = fx.Append1(HOpcode::kJmp, as->NewLabel());
  auto* rhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* rhs_block = fx.Append(HOpcode::kBlock);
  auto* user = fx.AppendN(HOpcode::kDupI, 1);  // operand wired below

  auto* phi = fx.AppendN(HOpcode::kPhiBranchI, 4);
  phi->OprSet(0, lhs_def->output());
  phi->OprSet(1, rhs_def->output());
  phi->OprSet(2, as->NewLabel(lhs_block));  // binds to the lhs arm tail
  phi->OprSet(3, as->NewLabel(rhs_block));  // binds to the rhs arm tail
  phi->PopulateOutput(as);
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);

  REQUIRE(CountPhi(as) == 1);

  VregPass pass{fx.quake.get(), as};
  VregContext* ctx = pass.Run();

  // SSA-03
  REQUIRE(CountPhi(as) == 0);
  REQUIRE(as->phi_size() == 0);

  // exactly one move at each predecessor-arm site: the lhs move sits at the
  // fast arm's tail (before the arm's exit jump), the rhs move at the slow
  // arm's tail (before the merge's first instruction)
  auto* lhs_move = jmp->prev();
  REQUIRE(lhs_move->opcode() == HOpcode::kMov);
  REQUIRE(lhs_move->Opr(0) == lhs_def->output());

  auto* rhs_move = user->prev();
  REQUIRE(rhs_move->opcode() == HOpcode::kMov);
  REQUIRE(rhs_move->Opr(0) == rhs_def->output());

  REQUIRE(CountMovOf(as, lhs_def->output()) == 1);
  REQUIRE(CountMovOf(as, rhs_def->output()) == 1);
  REQUIRE(CountMovTotal(as) == 2);  // no other insertion anywhere

  // use redirection onto the canonical (lhs arm) output
  REQUIRE(user->Opr(0) == lhs_move->output());
  REQUIRE(user->Opr(0)->instruction() == lhs_move);
  REQUIRE(IsInStream(as, lhs_move));
  REQUIRE(!IsInStream(as, phi));

  // one vreg across phi + both arm moves
  std::uint32_t phi_vreg = 0, lhs_vreg = 0, rhs_vreg = 0;
  REQUIRE(ctx->Lookup(phi->output(), &phi_vreg));
  REQUIRE(ctx->Lookup(lhs_move->output(), &lhs_vreg));
  REQUIRE(ctx->Lookup(rhs_move->output(), &rhs_vreg));
  REQUIRE(phi_vreg == lhs_vreg);
  REQUIRE(lhs_vreg == rhs_vreg);
  REQUIRE(ctx->GetDefs(phi_vreg).size() == 2);
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: mixed stream — loop and branch phis together", "[vreg][ssa]") {
  VregFixture fx;
  auto* as = fx.as.get();

  // Both phi families in one stream: the zero-PHI assertion runs on a
  // fixture that contained >= 1 loop phi AND >= 1 branch phi pre-pass (a
  // vacuous zero-phi fixture would make SSA-03 meaningless).
  //
  // [kStart][src][kBB1][lhs][lhs_block][jmp][rhs][rhs_block][upd]
  // [user_branch][user_loop][back=kJmp][kEnd][loop_phi][branch_phi][sentry]
  as->TestStart();
  auto* src_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(3)));
  auto* bb1_label = as->TestBB1();
  auto* lhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* lhs_block = fx.Append(HOpcode::kBlock);
  auto* jmp = fx.Append1(HOpcode::kJmp, as->NewLabel());
  auto* rhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* rhs_block = fx.Append(HOpcode::kBlock);
  auto* upd_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* user_branch = fx.AppendN(HOpcode::kDupI, 1);
  auto* user_loop = fx.AppendN(HOpcode::kDupI, 1);
  auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* loop_phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  loop_phi->OprSet(0, src_def->output());
  loop_phi->OprSet(1, upd_def->output());
  loop_phi->PopulateOutput(as);
  user_loop->OprSet(0, loop_phi->output());
  user_loop->PopulateOutput(as);

  auto* branch_phi = fx.AppendN(HOpcode::kPhiBranchI, 4);
  branch_phi->OprSet(0, lhs_def->output());
  branch_phi->OprSet(1, rhs_def->output());
  branch_phi->OprSet(2, as->NewLabel(lhs_block));
  branch_phi->OprSet(3, as->NewLabel(rhs_block));
  branch_phi->PopulateOutput(as);
  user_branch->OprSet(0, branch_phi->output());
  user_branch->PopulateOutput(as);

  as->TestFinalize();

  REQUIRE(CountPhi(as) == 2);
  REQUIRE(as->phi_size() == 1);  // loop-phi section counts loop phis only

  VregPass pass{fx.quake.get(), as};
  VregContext* ctx = pass.Run();

  // SSA-03 on the mixed stream
  REQUIRE(CountPhi(as) == 0);
  REQUIRE(as->phi_size() == 0);

  // exactly 4 moves: entry edge + backedge (loop phi) + 2 arms (branch phi)
  REQUIRE(CountMovTotal(as) == 4);
  REQUIRE(as->bb0_register_shuffle()->prev()->opcode() == HOpcode::kMov);
  REQUIRE(as->bb0_register_shuffle()->prev()->Opr(0) == src_def->output());
  REQUIRE(as->bb1_register_shuffle() == back);
  REQUIRE(back->prev()->opcode() == HOpcode::kMov);
  REQUIRE(back->prev()->Opr(0) == upd_def->output());
  REQUIRE(jmp->prev()->opcode() == HOpcode::kMov);
  REQUIRE(jmp->prev()->Opr(0) == lhs_def->output());

  // the rhs arm's tail is followed by upd_def (the loop phi's backedge
  // value) in this layout — the rhs move sits directly after rhs_block
  REQUIRE(rhs_block->next()->opcode() == HOpcode::kMov);
  REQUIRE(rhs_block->next()->Opr(0) == rhs_def->output());

  // both users redirected onto their canonical defs
  REQUIRE(user_loop->Opr(0) == as->bb0_register_shuffle()->prev()->output());
  REQUIRE(user_loop->Opr(0)->instruction() ==
          as->bb0_register_shuffle()->prev());
  REQUIRE(user_branch->Opr(0) == jmp->prev()->output());
  REQUIRE(user_branch->Opr(0)->instruction() == jmp->prev());
  REQUIRE(!IsInStream(as, loop_phi));
  REQUIRE(!IsInStream(as, branch_phi));

  // the two phis' vregs are distinct ids
  std::uint32_t loop_vreg = 0, branch_vreg = 0;
  REQUIRE(ctx->Lookup(loop_phi->output(), &loop_vreg));
  REQUIRE(ctx->Lookup(branch_phi->output(), &branch_vreg));
  REQUIRE(loop_vreg != branch_vreg);
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: fresh vreg per unseen SSA value", "[vreg][ssa]") {
  // (1) a phi-free stream of N distinct value instructions: N distinct ids,
  //     each output mapped to its own vreg
  {
    VregFixture fx;
    auto* as = fx.as.get();

    as->TestStart();
    std::vector<HInstruction*> values;
    HOperand* input = as->NewImmediate(std::int32_t(1));
    for (int i = 0; i < 4; i++) {
      auto* def = (i == 0)
                      ? fx.Append1(HOpcode::kMovI32, input)
                      : fx.Append1(HOpcode::kDupI, values.back()->output());
      values.push_back(def);
    }
    as->TestEnd();
    as->TestFinalize();

    REQUIRE(CountPhi(as) == 0);
    VregPass pass{fx.quake.get(), as};
    VregContext* ctx = pass.Run();

    REQUIRE(ctx->vreg_count() == 4);

    std::set<std::uint32_t> distinct;
    for (auto* def : values) {
      std::uint32_t vreg = 0;
      REQUIRE(ctx->Lookup(def->output(), &vreg));
      distinct.insert(vreg);
      REQUIRE(ctx->GetDefs(vreg).size() == 1);
      REQUIRE(ctx->GetDefs(vreg)[0] == def);
    }
    REQUIRE(distinct.size() == 4);
  }

  // (2) a loop-phi stream with an unrelated value: the phi vreg is distinct
  //     from every ordinary value's vreg
  {
    VregFixture fx;
    auto* as = fx.as.get();

    as->TestStart();
    auto* entry_def =
        fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
    auto* unrelated = fx.Append1(HOpcode::kDupI, entry_def->output());
    auto* bb1_label = as->TestBB1();
    auto* upd_def = fx.Append1(HOpcode::kDupI, entry_def->output());
    auto* user = fx.AppendN(HOpcode::kDupI, 1);
    fx.Append1(HOpcode::kJmp, bb1_label);
    as->TestEnd();

    auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
    phi->OprSet(0, entry_def->output());
    phi->OprSet(1, upd_def->output());
    phi->PopulateOutput(as);
    user->OprSet(0, phi->output());
    user->PopulateOutput(as);
    as->TestFinalize();

    VregPass pass{fx.quake.get(), as};
    VregContext* ctx = pass.Run();

    // entry, unrelated, upd, user + the phi's own vreg
    REQUIRE(ctx->vreg_count() == 5);

    std::uint32_t phi_vreg = 0, entry_vreg = 0, unrelated_vreg = 0,
                  upd_vreg = 0, user_vreg = 0;
    REQUIRE(ctx->Lookup(phi->output(), &phi_vreg));
    REQUIRE(ctx->Lookup(entry_def->output(), &entry_vreg));
    REQUIRE(ctx->Lookup(unrelated->output(), &unrelated_vreg));
    REQUIRE(ctx->Lookup(upd_def->output(), &upd_vreg));
    REQUIRE(ctx->Lookup(user->output(), &user_vreg));

    std::set<std::uint32_t> distinct{phi_vreg, entry_vreg, unrelated_vreg,
                                     upd_vreg, user_vreg};
    REQUIRE(distinct.size() == 5);
    REQUIRE(phi_vreg != unrelated_vreg);
    REQUIRE(phi_vreg != entry_vreg);
    REQUIRE(phi_vreg != upd_vreg);
  }
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: AddEdgeMove + ForeachEdgeMove direct", "[vreg][provenance]") {
  VregFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* mov1 = fx.Append1(HOpcode::kMov, a->output());
  auto* mov2 = fx.Append1(HOpcode::kMov, a->output());
  as->TestEnd();
  as->TestFinalize();

  VregContext ctx{fx.quake->zone()};
  auto va = ctx.GetOrNewVreg(a->output());
  ctx.AddDef(va, a);

  // empty at start
  std::size_t count = 0;
  ctx.ForeachEdgeMove([&](HInstruction*, EdgeMoveGroup) { count++; });
  REQUIRE(count == 0);

  // add two edge moves
  ctx.AddEdgeMove(mov1, EdgeMoveGroup::kEntry);
  ctx.AddEdgeMove(mov2, EdgeMoveGroup::kBackedge);

  count = 0;
  std::vector<HInstruction*> seen;
  ctx.ForeachEdgeMove([&](HInstruction* ins, EdgeMoveGroup group) {
    count++;
    seen.push_back(ins);
    (void)group;
  });
  REQUIRE(count == 2);
  REQUIRE(seen[0] == mov1);
  REQUIRE(seen[1] == mov2);
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: edge-move provenance — loop phi records entry + backedge",
          "[vreg][provenance]") {
  VregFixture fx;
  auto* as = fx.as.get();

  // [kStart][entry=kMovI32 #0][kBB1][upd=kDupI(entry)][user][back=kJmp]
  // [kEnd][kPhiLoopI(entry, upd)][sentry]
  as->TestStart();
  auto* entry_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* bb1_label = as->TestBB1();
  auto* upd_def = fx.Append1(HOpcode::kDupI, entry_def->output());
  auto* user = fx.AppendN(HOpcode::kDupI, 1);
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi->OprSet(0, entry_def->output());
  phi->OprSet(1, upd_def->output());
  phi->PopulateOutput(as);
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);
  as->TestFinalize();

  VregPass pass{fx.quake.get(), as};
  VregContext* ctx = pass.Run();

  // Exactly 2 edge moves recorded: one kEntry, one kBackedge
  std::size_t entry_count = 0, backedge_count = 0, branch_count = 0;
  std::vector<HInstruction*> recorded_moves;
  ctx->ForeachEdgeMove([&](HInstruction* ins, EdgeMoveGroup group) {
    recorded_moves.push_back(ins);
    if (group == EdgeMoveGroup::kEntry) entry_count++;
    if (group == EdgeMoveGroup::kBackedge) backedge_count++;
    if (group == EdgeMoveGroup::kBranchPhi) branch_count++;
  });
  REQUIRE(entry_count == 1);
  REQUIRE(backedge_count == 1);
  REQUIRE(branch_count == 0);
  REQUIRE(recorded_moves.size() == 2);

  // recorded instructions are actual kMov in the stream
  for (auto* m : recorded_moves) {
    REQUIRE(m->opcode() == HOpcode::kMov);
    REQUIRE(IsInStream(as, m));
  }

  // the entry move is the bb0 shuffle's prev
  auto* entry_move = as->bb0_register_shuffle()->prev();
  auto* backedge_move = as->bb1_register_shuffle()->prev();
  bool found_entry = false, found_backedge = false;
  ctx->ForeachEdgeMove([&](HInstruction* ins, EdgeMoveGroup group) {
    if (ins == entry_move && group == EdgeMoveGroup::kEntry) found_entry = true;
    if (ins == backedge_move && group == EdgeMoveGroup::kBackedge)
      found_backedge = true;
  });
  REQUIRE(found_entry);
  REQUIRE(found_backedge);
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: edge-move provenance — branch phi records lhs + rhs",
          "[vreg][provenance]") {
  VregFixture fx;
  auto* as = fx.as.get();

  // [kStart][src][lhs=kDupI(src)][lhs_block=kBlock][jmp merge]
  // [rhs=kDupI(src)][rhs_block=kBlock][user][kPhiBranchI][sentry]
  as->TestStart();
  auto* src_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(7)));
  auto* lhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* lhs_block = fx.Append(HOpcode::kBlock);
  [[maybe_unused]] auto* jmp = fx.Append1(HOpcode::kJmp, as->NewLabel());
  auto* rhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* rhs_block = fx.Append(HOpcode::kBlock);
  auto* user = fx.AppendN(HOpcode::kDupI, 1);

  auto* phi = fx.AppendN(HOpcode::kPhiBranchI, 4);
  phi->OprSet(0, lhs_def->output());
  phi->OprSet(1, rhs_def->output());
  phi->OprSet(2, as->NewLabel(lhs_block));
  phi->OprSet(3, as->NewLabel(rhs_block));
  phi->PopulateOutput(as);
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);

  REQUIRE(CountPhi(as) == 1);

  VregPass pass{fx.quake.get(), as};
  VregContext* ctx = pass.Run();
  REQUIRE(ctx != nullptr);
  REQUIRE(CountPhi(as) == 0);

  // Exactly 2 edge moves recorded, both kBranchPhi
  std::size_t entry_count = 0, backedge_count = 0, branch_count = 0;
  ctx->ForeachEdgeMove([&](HInstruction* ins, EdgeMoveGroup group) {
    (void)ins;
    if (group == EdgeMoveGroup::kEntry) entry_count++;
    if (group == EdgeMoveGroup::kBackedge) backedge_count++;
    if (group == EdgeMoveGroup::kBranchPhi) branch_count++;
  });
  REQUIRE(entry_count == 0);
  REQUIRE(backedge_count == 0);
  REQUIRE(branch_count == 2);
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: edge-move provenance — mixed stream records all four",
          "[vreg][provenance]") {
  VregFixture fx;
  auto* as = fx.as.get();

  // both phi families together
  as->TestStart();
  auto* src_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(3)));
  auto* bb1_label = as->TestBB1();
  auto* lhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* lhs_block = fx.Append(HOpcode::kBlock);
  [[maybe_unused]] auto* jmp = fx.Append1(HOpcode::kJmp, as->NewLabel());
  auto* rhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* rhs_block = fx.Append(HOpcode::kBlock);
  auto* upd_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* user_branch = fx.AppendN(HOpcode::kDupI, 1);
  auto* user_loop = fx.AppendN(HOpcode::kDupI, 1);
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* loop_phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  loop_phi->OprSet(0, src_def->output());
  loop_phi->OprSet(1, upd_def->output());
  loop_phi->PopulateOutput(as);
  user_loop->OprSet(0, loop_phi->output());
  user_loop->PopulateOutput(as);

  auto* branch_phi = fx.AppendN(HOpcode::kPhiBranchI, 4);
  branch_phi->OprSet(0, lhs_def->output());
  branch_phi->OprSet(1, rhs_def->output());
  branch_phi->OprSet(2, as->NewLabel(lhs_block));
  branch_phi->OprSet(3, as->NewLabel(rhs_block));
  branch_phi->PopulateOutput(as);
  user_branch->OprSet(0, branch_phi->output());
  user_branch->PopulateOutput(as);
  as->TestFinalize();

  VregPass pass{fx.quake.get(), as};
  VregContext* ctx = pass.Run();

  std::size_t entry_count = 0, backedge_count = 0, branch_count = 0;
  ctx->ForeachEdgeMove([&](HInstruction* ins, EdgeMoveGroup group) {
    (void)ins;
    if (group == EdgeMoveGroup::kEntry) entry_count++;
    if (group == EdgeMoveGroup::kBackedge) backedge_count++;
    if (group == EdgeMoveGroup::kBranchPhi) branch_count++;
  });
  REQUIRE(entry_count == 1);
  REQUIRE(backedge_count == 1);
  REQUIRE(branch_count == 2);
}

// -----------------------------------------------------------------------------
TEST_CASE("vreg: MergeVregs — redirects lookups and unions defs",
          "[vreg][merge]") {
  VregFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* b = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(2)));
  [[maybe_unused]] auto* use_a = fx.Append1(HOpcode::kDupI, a->output());
  [[maybe_unused]] auto* use_b = fx.Append1(HOpcode::kDupI, b->output());
  as->TestEnd();
  as->TestFinalize();

  VregContext ctx{fx.quake->zone()};
  auto va = ctx.GetOrNewVreg(a->output());
  auto vb = ctx.GetOrNewVreg(b->output());
  ctx.AddDef(va, a);
  ctx.AddDef(vb, b);
  REQUIRE(va != vb);

  // pre-merge: each operand resolves to its own vreg
  std::uint32_t tmp = 0;
  REQUIRE(ctx.Lookup(a->output(), &tmp));
  REQUIRE(tmp == va);
  REQUIRE(ctx.Lookup(b->output(), &tmp));
  REQUIRE(tmp == vb);

  // merge va INTO vb (vb survives, va disappears)
  ctx.MergeVregs(va, vb);

  // post-merge: both operands resolve to the survivor (vb)
  REQUIRE(ctx.Lookup(a->output(), &tmp));
  REQUIRE(tmp == vb);
  REQUIRE(ctx.Lookup(b->output(), &tmp));
  REQUIRE(tmp == vb);

  // defs: survivor's def vector contains both original defs
  const auto& survivor_defs = ctx.GetDefs(vb);
  REQUIRE(survivor_defs.size() == 2);
  bool has_a = false, has_b = false;
  for (auto* d : survivor_defs) {
    if (d == a) has_a = true;
    if (d == b) has_b = true;
  }
  REQUIRE(has_a);
  REQUIRE(has_b);

  // vreg_count stays the same (holes are permitted)
  REQUIRE(ctx.vreg_count() == 2);
}

}  // namespace
}  // namespace lir
}  // namespace quake
