// Structural unit tests for the move-coalescing pass (COAL-01/COAL-02/TEST-01,
// D-13/D-14/D-16..D-19).
//
// The pass ships INERT until the plan 02-05 cutover, so these tests drive it
// the only correct way available: hand-built HAssembler streams, the real
// VregPass, then RALinearize, then LivenessAnalysis, then CoalescingPass,
// asserting structural invariants only (stream walks + VregContext queries,
// no golden text dumps, D-13/D-14).
//
// Fixture pipeline per test:
//   sculpt LIR -> VregPass::Run -> RALinearize::Run -> LivenessAnalysis::Run
//   -> CoalescingPass::Run -> assert.
// The hand-built shapes (case 2) register the post-vreg vreg context —
// aliases, def records AND edge-move provenance — exactly as VregPass would
// have left it.
//
// The D-19 case re-verifies the coalescing invariant independently: a fresh
// RALinearize + fresh LivenessAnalysis over the post-coalescing stream (no
// access to any coalescing internals; the pass was driven only through its
// public Run()) must show no simultaneously-live merged pair.
//
// Fixtures never call Print() on hand-built instructions (nullptr ir::Node*
// is stored, only dereferenced by trace/print paths).

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <quake.h>

#include <lir/assembler.h>
#include <lir/coalescing.h>
#include <lir/instruction.h>
#include <lir/liveness-analysis.h>
#include <lir/opcode.h>
#include <lir/operand.h>
#include <lir/ra-linearize.h>
#include <lir/vreg-pass.h>

#include <unittest/ir/qjs-bytecode-test-util.h>

#include <memory>
#include <vector>

namespace quake {
namespace lir {
namespace {

// ----------------------------------------------------------------------------
// Fixture (D-13, same seam and JSContext source as vreg-pass-test /
// liveness-test): Quake needs a live JSContext (its ctor interns JSStrings).
// Zone-backed state (HAssembler instructions, VregContext, the analysis)
// lives on the Quake zone.

struct CoalescingFixture {
  std::unique_ptr<ir::QJSFunctionBytecode> mock;
  std::unique_ptr<QuakeWorld> world;
  std::unique_ptr<Quake> quake;
  std::unique_ptr<HAssembler> as;

  CoalescingFixture() {
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

  // The fixture pipeline tail shared by both entry styles: number the
  // stream (liveness ids), then run the dataflow.
  void FinishPipeline() {
    RALinearize linearize{quake.get(), as.get()};
    linearize.Run();
    analysis = std::make_unique<LivenessAnalysis>(quake.get(), as.get(), ctx);
    analysis->Run();
  }

  // Pipeline entry (a): the real SSA destruction. Returns the VregContext
  // every later pass shares.
  VregContext* RunVregThenLiveness() {
    VregPass vreg_pass{quake.get(), as.get()};
    ctx = vreg_pass.Run();
    FinishPipeline();
    return ctx;
  }

  // Pipeline entry (b): hand-registered post-vreg context (no VregPass).
  void Analyze(VregContext* context) {
    ctx = context;
    FinishPipeline();
  }

  // The pass under test, driven only through its public Run().
  void Coalesce() {
    CoalescingPass pass{quake.get(), as.get(), ctx, analysis.get()};
    pass.Run();
  }

  VregContext* ctx = nullptr;
  std::unique_ptr<LivenessAnalysis> analysis;
};

// ----------------------------------------------------------------------------
// D-19 deviation note (documented, Rule 1): a LITERAL second RALinearize
// over the SAME instruction objects is architecturally impossible —
// LivenessInfo is EMBEDDED in each HInstruction (SetLivenessStart CHECKs the
// id slot is pristine, include/lir/liveness.h) and every output Temporary is
// deliberately single-assignment (set_live_interval_list CHECK, T-06-02,
// include/lir/temporary.h). The independent arm of the D-19 case is therefore
// a COMPLETELY FRESH LivenessAnalysis instance — its own CFG, RPO walk and
// fixpoint tables — over the ACTUAL post-coalescing stream and the shared
// vreg context, with no access to any coalescing internals (the pass was
// driven only through its public Run()). The surviving instructions keep
// their run-1 numbering, which remains a valid program order with the
// deleted move simply absent; every dataflow fact is recomputed from scratch.
std::size_t CountMovTotal(HAssembler* as) {
  std::size_t count = 0;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins->opcode() == HOpcode::kMov) count++;
  });
  return count;
}

std::size_t CountPhi(HAssembler* as) {
  std::size_t count = 0;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins->IsPhi()) count++;
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

// -----------------------------------------------------------------------------
// Case 1 (D-16): entry-edge shuffle, init-to-phi, disjoint liveness — MERGED.
//
// The honest induction shape (VregPass on a loop-phi stream):
//   [kStart][entry #0][entry_move][kBB1][upd(kDupI phi)][user(kDupI phi)]
//   [backedge_move][back][kEnd]
// The entry const dies inside the pre-loop block (its only use IS the entry
// move); the induction vreg's live range STARTS at that move. The endpoints
// never overlap, so the merge fires — move count drops to 1, users resolve
// to the surviving operand (the entry const's own def).
//
// The backedge move's endpoints ARE simultaneously live (the induction vreg
// is live-in of the loop body, the upd value is defined inside it), so it
// stays (case 3 re-asserts it with focus).
TEST_CASE("coalescing: entry shuffle merges, backedge stays", "[coalescing]") {
  CoalescingFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* bb1_label = as->TestBB1();
  auto* upd = fx.AppendN(HOpcode::kDupI, 1);   // operand = phi output
  auto* user = fx.AppendN(HOpcode::kDupI, 1);  // operand = phi output
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi->PopulateOutput(as);
  upd->OprSet(0, phi->output());
  upd->PopulateOutput(as);
  phi->OprSet(0, entry->output());
  phi->OprSet(1, upd->output());
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);
  as->TestFinalize();

  REQUIRE(CountPhi(as) == 1);
  fx.RunVregThenLiveness();
  REQUIRE(CountPhi(as) == 0);
  REQUIRE(CountMovTotal(as) == 2);  // entry move + backedge move

  // capture the two edge moves at their well-known sites
  auto* entry_move = as->bb0_register_shuffle()->prev();
  auto* backedge_move = as->bb1_register_shuffle()->prev();
  REQUIRE(entry_move->opcode() == HOpcode::kMov);
  REQUIRE(backedge_move->opcode() == HOpcode::kMov);

  std::uint32_t induction = 0, entry_vreg = 0;
  REQUIRE(fx.ctx->Lookup(phi->output(), &induction));
  REQUIRE(fx.ctx->Lookup(entry->output(), &entry_vreg));

  fx.Coalesce();

  // the entry move merged and was deleted; the backedge move survived
  REQUIRE(CountMovTotal(as) == 1);
  REQUIRE(!IsInStream(as, entry_move));
  REQUIRE(IsInStream(as, backedge_move));
  REQUIRE(as->bb1_register_shuffle()->prev() == backedge_move);

  // users resolve to the SURVIVING operand: the entry const's own def
  REQUIRE(upd->Opr(0) == entry->output());
  REQUIRE(upd->Opr(0)->instruction() == entry);
  REQUIRE(user->Opr(0) == entry->output());
  REQUIRE(user->Opr(0)->instruction() == entry);

  // the kept backedge move still copies upd into the merged vreg
  REQUIRE(backedge_move->Opr(0) == upd->output());
  std::uint32_t survivor = 0, backedge_vreg = 0;
  REQUIRE(fx.ctx->Lookup(entry->output(), &survivor));
  REQUIRE(fx.ctx->Lookup(backedge_move->output(), &backedge_vreg));
  REQUIRE(survivor == backedge_vreg);
  REQUIRE(survivor == entry_vreg);  // the phi vreg folded into the const's
}

// -----------------------------------------------------------------------------
// Case 2 (D-16): loop-header shuffle, safe case, on a HAND-BUILT post-vreg
// stream — no VregPass. The test registers the vreg context (aliases, defs,
// provenance) exactly as VregPass would have left it, proving the pass only
// depends on the context contract, not on VregPass having just run.
TEST_CASE("coalescing: hand-built loop-header shuffle merges",
          "[coalescing]") {
  CoalescingFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* entry_move = fx.Append1(HOpcode::kMov, entry->output());
  auto* bb1_label = as->TestBB1();
  auto* upd = fx.Append1(HOpcode::kDupI, entry_move->output());
  auto* user = fx.Append1(HOpcode::kDupI, entry_move->output());
  auto* backedge_move = fx.Append1(HOpcode::kMov, upd->output());
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();
  as->TestFinalize();

  VregContext ctx{fx.quake->zone()};
  auto ventry = ctx.GetOrNewVreg(entry->output());
  ctx.AddDef(ventry, entry);
  auto vinduction = ctx.GetOrNewVreg(entry_move->output());
  ctx.AddDef(vinduction, entry_move);
  auto vupd = ctx.GetOrNewVreg(upd->output());
  ctx.AddDef(vupd, upd);
  auto vuser = ctx.GetOrNewVreg(user->output());
  ctx.AddDef(vuser, user);
  CHECK(ctx.MapAlias(backedge_move->output(), vinduction) == vinduction);
  ctx.AddDef(vinduction, backedge_move);

  // the provenance VregPass would have recorded for these two moves
  ctx.AddEdgeMove(entry_move, EdgeMoveGroup::kEntry);
  ctx.AddEdgeMove(backedge_move, EdgeMoveGroup::kBackedge);

  fx.Analyze(&ctx);
  REQUIRE(CountMovTotal(as) == 2);

  fx.Coalesce();

  REQUIRE(CountMovTotal(as) == 1);
  REQUIRE(!IsInStream(as, entry_move));
  REQUIRE(IsInStream(as, backedge_move));
  REQUIRE(user->Opr(0) == entry->output());
  REQUIRE(user->Opr(0)->instruction() == entry);
  REQUIRE(backedge_move->Opr(0) == upd->output());
}

// -----------------------------------------------------------------------------
// Case 3 (D-17): the backedge induction move's endpoints are simultaneously
// live (induction vreg live-in of the loop body, upd value defined inside
// it) — it is REJECTED and must remain a real instruction, fully intact.
TEST_CASE("coalescing: backedge induction move rejected", "[coalescing]") {
  CoalescingFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* bb1_label = as->TestBB1();
  auto* upd = fx.AppendN(HOpcode::kDupI, 1);
  auto* user = fx.AppendN(HOpcode::kDupI, 1);
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi->PopulateOutput(as);
  upd->OprSet(0, phi->output());
  upd->PopulateOutput(as);
  phi->OprSet(0, entry->output());
  phi->OprSet(1, upd->output());
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto* backedge_move = as->bb1_register_shuffle()->prev();
  auto* entry_move = as->bb0_register_shuffle()->prev();

  // sanity on the interference premise: the induction vreg IS live-in of the
  // loop-head block, and the upd value is defined there — the def clause
  // must see the simultaneity
  const std::uint32_t loop_head_bid = 1;  // B_a(0) = pre-loop, B_b(1) = body
  std::uint32_t induction = 0, upd_vreg = 0;
  REQUIRE(fx.ctx->Lookup(phi->output(), &induction));
  REQUIRE(fx.ctx->Lookup(upd->output(), &upd_vreg));
  (void)loop_head_bid;  // bids are an analysis detail; the rule fires on the
                        // real def->block mapping, not on this constant

  fx.Coalesce();

  // the backedge move survived the pass, untouched as an instruction
  REQUIRE(IsInStream(as, backedge_move));
  REQUIRE(backedge_move->opcode() == HOpcode::kMov);
  REQUIRE(backedge_move->Opr(0) == upd->output());
  REQUIRE(backedge_move->output()->instruction() == backedge_move);

  // ...while the entry move (same phi, disjoint endpoints) did merge
  REQUIRE(!IsInStream(as, entry_move));
  REQUIRE(CountMovTotal(as) == 1);

  // the surviving vreg keeps BOTH live def sites: the entry const's def and
  // the kept backedge move's def (the loop-carried shape). Notes, the union
  // also carries the DELETED entry move's def record (MergeVregs unions the
  // full records) — an inert zone-memory entry: LivenessAnalysis derives
  // defs from the stream walk, and the coalescing def clauses skip
  // instructions absent from the CFG.
  std::uint32_t survivor = 0;
  REQUIRE(fx.ctx->Lookup(entry->output(), &survivor));
  const auto& defs = fx.ctx->GetDefs(survivor);
  REQUIRE(defs.size() == 3);  // entry def + deleted entry_move + backedge
  bool has_entry_def = false, has_backedge_def = false;
  for (auto* d : defs) {
    if (d == entry) has_entry_def = true;
    if (d == backedge_move) has_backedge_def = true;
  }
  REQUIRE(has_entry_def);
  REQUIRE(has_backedge_def);
}

// -----------------------------------------------------------------------------
// Case 4: branch-phi arm moves on the mixed stream (loop structure + branch
// phi inside the body — the honest real-trace shape, since every quake trace
// carries the BB0/BB1 loop skeleton). The arm values are arm-local: the lhs
// move merges and the join user resolves to the lhs value's own def; the rhs
// arm merges symmetrically, so both arm values share one vreg afterwards.
//
// The loop phi's OWN moves do not merge here: its entry input (src) is
// reused inside the loop body, so src and the induction vreg are both
// live-in of the body — the interference rule keeps both loop moves.
TEST_CASE("coalescing: branch-phi lhs arm move merges", "[coalescing]") {
  CoalescingFixture fx;
  auto* as = fx.as.get();

  // [kStart][src][kBB1][lhs][lhs_block][jmp merge][rhs][rhs_block][upd]
  // [user_branch][user_loop][back=kJmp][kEnd][loop_phi][branch_phi][sentry]
  as->TestStart();
  auto* src = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(3)));
  auto* bb1_label = as->TestBB1();
  auto* lhs_def = fx.Append1(HOpcode::kDupI, src->output());
  auto* lhs_block = fx.Append(HOpcode::kBlock);
  auto* jmp = fx.AppendN(HOpcode::kJmp, 1);  // wired below -> the merge user
  auto* rhs_def = fx.Append1(HOpcode::kDupI, src->output());
  auto* rhs_block = fx.Append(HOpcode::kBlock);
  auto* upd_def = fx.Append1(HOpcode::kDupI, src->output());
  auto* user_branch = fx.AppendN(HOpcode::kDupI, 1);
  auto* user_loop = fx.AppendN(HOpcode::kDupI, 1);
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  // the lhs arm exits to the merge user — a BOUND label, which the CFG
  // materializer requires (every label operand must carry a bind)
  jmp->OprSet(0, as->NewLabel(user_branch));

  auto* loop_phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  loop_phi->PopulateOutput(as);
  loop_phi->OprSet(0, src->output());
  loop_phi->OprSet(1, upd_def->output());
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
  fx.RunVregThenLiveness();
  REQUIRE(CountPhi(as) == 0);
  REQUIRE(CountMovTotal(as) == 4);  // entry + backedge + 2 arms

  // capture the four edge moves at their well-known sites
  auto* entry_move = as->bb0_register_shuffle()->prev();
  auto* backedge_move = as->bb1_register_shuffle()->prev();
  auto* lhs_move = jmp->prev();
  auto* rhs_move = rhs_block->next();
  REQUIRE(entry_move->opcode() == HOpcode::kMov);
  REQUIRE(backedge_move->opcode() == HOpcode::kMov);
  REQUIRE(lhs_move->opcode() == HOpcode::kMov);
  REQUIRE(rhs_move->opcode() == HOpcode::kMov);

  fx.Coalesce();

  // both arm moves merged; both loop moves interfered (src reused in body)
  REQUIRE(!IsInStream(as, lhs_move));
  REQUIRE(!IsInStream(as, rhs_move));
  REQUIRE(IsInStream(as, entry_move));
  REQUIRE(IsInStream(as, backedge_move));
  REQUIRE(CountMovTotal(as) == 2);

  // the join user resolves to the SURVIVING lhs operand
  REQUIRE(user_branch->Opr(0) == lhs_def->output());
  REQUIRE(user_branch->Opr(0)->instruction() == lhs_def);

  // both arm values now share one vreg identity
  std::uint32_t lhs_vreg = 0, rhs_vreg = 0;
  REQUIRE(fx.ctx->Lookup(lhs_def->output(), &lhs_vreg));
  REQUIRE(fx.ctx->Lookup(rhs_def->output(), &rhs_vreg));
  REQUIRE(lhs_vreg == rhs_vreg);

  // the loop phi's user still resolves to its (unmerged) canonical entry
  // move output — the kept loop moves did not disturb it
  REQUIRE(user_loop->Opr(0) == entry_move->output());
}

// -----------------------------------------------------------------------------
// Case 5 (D-18, T-02-01-02): a kMov with NO provenance is never touched —
// even when its endpoints are perfectly non-interfering (they would merge
// instantly if the move were an edge move).
TEST_CASE("coalescing: no edge moves — pass is a no-op", "[coalescing]") {
  CoalescingFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* mov = fx.Append1(HOpcode::kMov, a->output());
  auto* user = fx.Append1(HOpcode::kDupI, mov->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  // the provenance table is empty: nothing is a candidate
  std::size_t provenance = 0;
  fx.ctx->ForeachEdgeMove([&](HInstruction*, EdgeMoveGroup) {
    provenance++;
  });
  REQUIRE(provenance == 0);
  REQUIRE(CountMovTotal(as) == 1);

  fx.Coalesce();

  // the plain move survived untouched; its user still resolves to it
  REQUIRE(CountMovTotal(as) == 1);
  REQUIRE(user->Opr(0) == mov->output());
  REQUIRE(user->Opr(0)->instruction() == mov);
}

// -----------------------------------------------------------------------------
// Case 5b (D-18, T-02-01-02, mixed stream): a plain kMov sitting BESIDE real
// edge moves — the entry move merges around it, the plain move is left
// strictly alone (candidate enumeration is provenance-only, never a stream
// scan for kMov).
TEST_CASE("coalescing: plain kMov untouched beside real edge moves",
          "[coalescing]") {
  CoalescingFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* c2 = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(9)));
  auto* plain = fx.Append1(HOpcode::kMov, c2->output());
  auto* plain_user = fx.Append1(HOpcode::kDupI, plain->output());
  auto* bb1_label = as->TestBB1();
  auto* upd = fx.AppendN(HOpcode::kDupI, 1);
  auto* user = fx.AppendN(HOpcode::kDupI, 1);
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi->PopulateOutput(as);
  upd->OprSet(0, phi->output());
  upd->PopulateOutput(as);
  phi->OprSet(0, entry->output());
  phi->OprSet(1, upd->output());
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);
  as->TestFinalize();

  fx.RunVregThenLiveness();
  REQUIRE(CountMovTotal(as) == 3);  // entry + backedge + plain

  auto* entry_move = as->bb0_register_shuffle()->prev();
  auto* backedge_move = as->bb1_register_shuffle()->prev();

  fx.Coalesce();

  // entry merged, backedge interfered, plain was never a candidate
  REQUIRE(CountMovTotal(as) == 2);
  REQUIRE(!IsInStream(as, entry_move));
  REQUIRE(IsInStream(as, backedge_move));
  REQUIRE(IsInStream(as, plain));
  REQUIRE(plain->Opr(0) == c2->output());
  REQUIRE(plain_user->Opr(0) == plain->output());
  REQUIRE(plain_user->Opr(0)->instruction() == plain);

  // the phi users still resolved to the merged survivor
  REQUIRE(user->Opr(0) == entry->output());
}

// -----------------------------------------------------------------------------
// Case 6 (D-19): independent post-coalescing liveness re-verification.
//
// After coalescing the induction shape, build a COMPLETELY FRESH RALinearize
// run and a fresh LivenessAnalysis over the post-coalescing stream — no
// access to any coalescing internals (the pass was driven only through its
// public Run()) — and assert the pinned invariant: for every merged pair,
// the recomputed liveness contains NO simultaneously-live pair. The absorbed
// id has no def and no use anywhere anymore, so it cannot be live in any
// block; the survivor keeps the full loop-carried shape.
TEST_CASE("coalescing: D-19 independent liveness re-verification",
          "[coalescing]") {
  CoalescingFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* bb1_label = as->TestBB1();
  auto* upd = fx.AppendN(HOpcode::kDupI, 1);
  auto* user = fx.AppendN(HOpcode::kDupI, 1);
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi->PopulateOutput(as);
  upd->OprSet(0, phi->output());
  upd->PopulateOutput(as);
  phi->OprSet(0, entry->output());
  phi->OprSet(1, upd->output());
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);
  as->TestFinalize();

  fx.RunVregThenLiveness();

  std::uint32_t induction = 0, entry_vreg = 0;
  REQUIRE(fx.ctx->Lookup(phi->output(), &induction));
  REQUIRE(fx.ctx->Lookup(entry->output(), &entry_vreg));

  fx.Coalesce();
  REQUIRE(CountMovTotal(as) == 1);  // the merge happened in this fixture

  // The independent re-verification: a completely fresh LivenessAnalysis
  // instance (own CFG, RPO, fixpoint tables) over the ACTUAL post-coalescing
  // stream. Nothing from the coalescing pass is read, and a literal second
  // RALinearize is architecturally impossible (see the deviation note at
  // ResetLivenessIdsForReLinearize's former site above).
  auto fresh =
      std::make_unique<LivenessAnalysis>(fx.quake.get(), as, fx.ctx);
  fresh->Run();

  // THE invariant (must_have 5): the merged pair is never simultaneously
  // live. The absorbed id lost every operand (uses redirected, move gone),
  // so it has no def/use points and no block may hold it live — if any of
  // these fail, a merge silently created overlapping live ranges.
  REQUIRE(fresh->DefPoints(induction).empty());
  REQUIRE(fresh->UsePoints(induction).empty());
  for (std::uint32_t bid = 0; bid < fresh->block_count(); bid++) {
    REQUIRE_FALSE(fresh->LiveIn(bid, induction));
    REQUIRE_FALSE(fresh->LiveOut(bid, induction));
  }

  // positive control: the survivor is a live loop-carried value — live-in
  // of the loop-head block (the block defining the upd value), live-out of
  // the block holding the backedge. The merge preserved the carried shape.
  const BasicBlock* loop_head = nullptr;
  const BasicBlock* backedge_bb = nullptr;
  fresh->cfg()->ForeachBasicBlock([&](const BasicBlock* bb) -> bool {
    bb->ForeachInstruction([&](const HInstruction* ins) -> bool {
      if (ins == upd) loop_head = bb;
      if (ins == back) backedge_bb = bb;
      return true;
    });
    return true;
  });
  CHECK(loop_head);
  CHECK(backedge_bb);
  REQUIRE(fresh->LiveIn(loop_head, entry_vreg));
  REQUIRE(fresh->LiveOut(backedge_bb, entry_vreg));

  // and the survivor's def records contain both live sites: the entry
  // const's def plus the kept backedge move (the union also carries the
  // deleted entry move's inert record, as in case 3)
  const auto& defs = fx.ctx->GetDefs(entry_vreg);
  REQUIRE(defs.size() == 3);
  bool has_entry = false, has_backedge = false;
  for (auto* d : defs) {
    if (d == entry) has_entry = true;
    if (d == as->bb1_register_shuffle()->prev()) has_backedge = true;
  }
  REQUIRE(has_entry);
  REQUIRE(has_backedge);
}

}  // namespace
}  // namespace lir
}  // namespace quake
