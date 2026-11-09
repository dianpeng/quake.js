// Structural unit tests for the constraint-driven linear-scan register
// allocator (RA-01/RA-03/PCOPY-02/TEST-01, D-20..D-24, D-30/D-31).
//
// The allocator ships INERT until the plan 02-05 cutover, so these tests
// drive it the only correct way available: hand-built HAssembler streams
// (or the real VregPass where edge-move provenance matters), RALinearize
// (installs the liveness-id coordinates everything reads), LivenessAnalysis
// (classical dataflow def/use points), then LinearScanRA::Run, asserting
// structural invariants only (interval accessors + stream walks +
// RAValidator, no golden text dumps, D-13/D-14).
//
// Fixture pipeline per test:
//   sculpt LIR -> [VregPass] -> RALinearize::Run -> LivenessAnalysis::Run
//   -> LinearScanRA::Run -> [RAValidator::Run]
//
// When CoalescingPass participates (the PCOPY-02 cases), the canonical
// order is:
//   sculpt -> VregPass -> RALinearize -> LivenessAnalysis -> CoalescingPass
//   -> FRESH LivenessAnalysis (same stream, same ids; the D-19 arm's
//   pattern) -> LinearScanRA
// because the pass unlinks moves and re-points vreg ids, staling the first
// analysis' per-id point lists.
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
#include <lir/linear-scan-ra.h>
#include <lir/liveness-analysis.h>
#include <lir/opcode.h>
#include <lir/operand.h>
#include <lir/ra-linearize.h>
#include <lir/ra-validator.h>
#include <lir/register.h>
#include <lir/vreg-pass.h>

#include <unittest/ir/qjs-bytecode-test-util.h>

#include <ir/checkpoint.h>
#include <ir/node.h>
#include <ir/ssa-var.h>

#include <memory>
#include <string>
#include <vector>

namespace quake {
namespace lir {
namespace {

// ----------------------------------------------------------------------------
// Fixture (same seam and JSContext source as coalescing-test /
// liveness-test): Quake needs a live JSContext (its ctor interns JSStrings).
// Zone-backed state lives on the Quake zone.

struct LinearScanFixture {
  std::unique_ptr<ir::QJSFunctionBytecode> mock;
  std::unique_ptr<QuakeWorld> world;
  std::unique_ptr<Quake> quake;
  std::unique_ptr<HAssembler> as;

  LinearScanFixture() {
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

  // The fixture pipeline tail: number the stream (liveness ids), then run
  // the dataflow. `ctx` must already map every Temporary that carries a
  // def link (VregPass output or hand registration).
  void Analyze(VregContext* context) {
    ctx = context;
    RALinearize linearize{quake.get(), as.get()};
    linearize.Run();
    analysis = std::make_unique<LivenessAnalysis>(quake.get(), as.get(), ctx);
    analysis->Run();
    RememberOutputs();
  }

  // Snapshot every live instruction's output vreg NOW — materialization
  // rewrites the output operand itself, so Lookup(def->output()) dies
  // after Run().
  void RememberOutputs() {
    as->ForeachInstruction([&](HInstruction* ins) {
      if (ins->HasOutput() && ins->output() != nullptr &&
          ins->output()->IsTemporary()) {
        std::uint32_t v = 0;
        if (ctx->Lookup(ins->output(), &v)) output_vreg_[ins] = v;
      }
    });
  }

  // Pipeline entry (a): the real SSA destruction, then numbering +
  // dataflow. Returns the VregContext every later pass shares.
  VregContext* RunVregThenLiveness() {
    VregPass vreg_pass{quake.get(), as.get()};
    ctx = vreg_pass.Run();
    RALinearize linearize{quake.get(), as.get()};
    linearize.Run();
    analysis = std::make_unique<LivenessAnalysis>(quake.get(), as.get(), ctx);
    analysis->Run();
    RememberOutputs();
    return ctx;
  }

  // Re-run the dataflow over the CURRENT stream (same liveness ids) — the
  // D-19 pattern, required after CoalescingPass mutated the stream/ids.
  void ReAnalyze() {
    analysis = std::make_unique<LivenessAnalysis>(quake.get(), as.get(), ctx);
    analysis->Run();
  }

  void Coalesce() {
    CoalescingPass pass{quake.get(), as.get(), ctx, analysis.get()};
    pass.Run();
  }

  // The allocator under test. Returns its base::Error (empty on success) so
  // the bail case can assert the reason; the instance outlives assertions.
  base::Error Allocate() {
    ra = std::make_unique<LinearScanRA>(quake.get(), as.get(), ctx,
                                        analysis.get());
    return ra->Run();
  }

  // Independent post-RA verification (D-27) — invoked by every successful
  // allocation test below.
  base::Error Validate() {
    RAValidator validator{quake.get(), as.get()};
    return validator.Run();
  }

  const RAInterval* IntervalOf(std::uint32_t vreg) const {
    return ra->Interval(vreg);
  }

  std::uint32_t VregOf(HOperand* opr) const {
    std::uint32_t id = 0;
    REQUIRE(ctx->Lookup(opr, &id));
    return id;
  }

  // The output vreg of a defining instruction, remembered before
  // materialization rewrote the output operand.
  std::uint32_t VregOf(const HInstruction* def_ins) const {
    auto itr = output_vreg_.find(def_ins);
    REQUIRE(itr != output_vreg_.end());
    return itr->second;
  }

  VregContext* ctx = nullptr;
  std::unique_ptr<LivenessAnalysis> analysis;
  std::unique_ptr<LinearScanRA> ra;
  std::map<const HInstruction*, std::uint32_t> output_vreg_;
};

// ----------------------------------------------------------------------------
// Small structural helpers

[[maybe_unused]] bool IsInStream(HAssembler* as, HInstruction* target) {
  bool found = false;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins == target) found = true;
  });
  return found;
}

[[maybe_unused]] std::size_t CountOpcode(HAssembler* as, HOpcode opcode) {
  std::size_t count = 0;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins->opcode() == opcode) count++;
  });
  return count;
}

const Register* FirstFreeGP() {
  return Register::GetRegisterFromKind(RegisterKind::kR10);
}

const Register* SecondFreeGP() {
  return Register::GetRegisterFromKind(RegisterKind::kR11);
}

const Register* ThirdFreeGP() {
  return Register::GetRegisterFromKind(RegisterKind::kR12);
}

// -----------------------------------------------------------------------------
// Task 1 — interval construction, scan core, spilling, clean bail.
//
// Shape: straight-line, three simultaneously-live GP values. Each interval
// gets its own distinct register of the bank, in free-list order.
//
// Stream: [kStart][a][b][c][ua][ub][uc][back][kEnd]
// Liveness ids: kStart=2, a=4, b=6, c=8, ua=10, ub=12, uc=14, back=16,
// kEnd=18.
TEST_CASE("linear-scan-ra: straight-line distinct registers",
          "[linear-scan-ra]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* b = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(2)));
  auto* c = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(3)));
  fx.Append1(HOpcode::kDupI, a->output());
  fx.Append1(HOpcode::kDupI, b->output());
  fx.Append1(HOpcode::kDupI, c->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* ia = fx.IntervalOf(fx.VregOf(a));
  const auto* ib = fx.IntervalOf(fx.VregOf(b));
  const auto* ic = fx.IntervalOf(fx.VregOf(c));
  REQUIRE(ia != nullptr);
  REQUIRE(ib != nullptr);
  REQUIRE(ic != nullptr);

  // single range [first def, last live point] in liveness-id coordinates
  REQUIRE(ia->def.point() == 4);
  REQUIRE(ia->last.point() == 10);
  REQUIRE(ib->def.point() == 6);
  REQUIRE(ib->last.point() == 12);
  REQUIRE(ic->def.point() == 8);
  REQUIRE(ic->last.point() == 14);

  // three distinct bank registers, in free-list scan order, none spilled
  REQUIRE(ia->assigned == FirstFreeGP());
  REQUIRE(ib->assigned == SecondFreeGP());
  REQUIRE(ic->assigned == ThirdFreeGP());
  REQUIRE(ia->assigned != ib->assigned);
  REQUIRE(ib->assigned != ic->assigned);
  REQUIRE(!ia->in_memory);
  REQUIRE(!ib->in_memory);
  REQUIRE(!ic->in_memory);
}

// -----------------------------------------------------------------------------
// Shape: non-overlapping intervals — b's def lands after a's last use, so
// the expired register is reused.
//
// Stream: [kStart][a][ua][b][ub][back][kEnd]
// Liveness ids: kStart=2, a=4, ua=6, b=8, ub=10, back=12, kEnd=14.
TEST_CASE("linear-scan-ra: expiration reuses registers", "[linear-scan-ra]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  fx.Append1(HOpcode::kDupI, a->output());
  auto* b = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(2)));
  fx.Append1(HOpcode::kDupI, b->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* ia = fx.IntervalOf(fx.VregOf(a));
  const auto* ib = fx.IntervalOf(fx.VregOf(b));
  REQUIRE(ia != nullptr);
  REQUIRE(ib != nullptr);
  REQUIRE(ia->last.point() == 6);
  REQUIRE(ib->def.point() == 8);

  // a expired before b's def: b reuses a's register
  REQUIRE(ib->assigned == ia->assigned);
  REQUIRE(ib->assigned == FirstFreeGP());
  REQUIRE(!ib->in_memory);
}

// -----------------------------------------------------------------------------
// Shape: bank exhaustion. Thirteen simultaneously-live GP values with only
// twelve free GP registers: the interval with the furthest last point is
// spilled to a FrameLayout spill slot, its later use preceded by a reload.
//
// Stream: [kStart][d0]..[d12][u0]..[u12][back][kEnd] — all defs before all
// uses, so every interval overlaps every other.
TEST_CASE("linear-scan-ra: exhaustion spills with reload", "[linear-scan-ra]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  constexpr int kValueCount = 13;  // > 12 free GP registers

  as->TestStart();
  std::vector<HInstruction*> def(kValueCount);
  for (int i = 0; i < kValueCount; i++) {
    def[i] = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(i)));
  }
  std::vector<HInstruction*> use(kValueCount);
  for (int i = 0; i < kValueCount; i++) {
    use[i] = fx.Append1(HOpcode::kDupI, def[i]->output());
  }
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(def[0]));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  // exactly one spilled interval: the furthest-last value (d12)
  int spilled = -1;
  for (int i = 0; i < kValueCount; i++) {
    const auto* iv = fx.IntervalOf(fx.VregOf(def[i]));
    REQUIRE(iv != nullptr);
    if (iv->in_memory) {
      REQUIRE(spilled == -1);
      spilled = i;
      REQUIRE(iv->spill != MemoryStack::kInvalidSlot);
    }
  }
  REQUIRE(spilled == kValueCount - 1);

  const auto* spilled_iv = fx.IntervalOf(fx.VregOf(def[spilled]));
  REQUIRE(spilled_iv->assigned == nullptr);

  // one spill slot allocated from the FrameLayout spill region
  REQUIRE(as->frame_layout()->spill_count() == 1);

  // the spilled interval's later use is preceded by a reload mov whose
  // source is exactly that spill slot
  auto* reload = use[spilled]->prev();
  REQUIRE(reload->opcode() == HOpcode::kMov);
  REQUIRE(reload->Opr(0)->IsMemStack());
  REQUIRE(reload->Opr(0)->GetMemStack().slot() == spilled_iv->spill);
  REQUIRE(reload->output()->IsRegister());
  REQUIRE(reload->output()->GetRegister()->IsGP());

  // and the register written by the reload is the first bank register free
  // AT THE RELOAD POINT: every other value expired before it, but the use
  // instruction's OWN output (a dead dup value) takes the first register
  // at this very point, so the reload lands in the second
  for (int i = 0; i < kValueCount; i++) {
    if (i == spilled) continue;
    const auto* iv = fx.IntervalOf(fx.VregOf(def[i]));
    bool live_at_reload = iv->def.point() <= 54 && 54 <= iv->last.point();
    REQUIRE(!live_at_reload);
  }
  REQUIRE(reload->output()->GetRegister()->register_kind() ==
        SecondFreeGP()->register_kind());
}

// -----------------------------------------------------------------------------
// Shape: multi-def vreg (the loop-carried induction shape, hand-registered
// exactly as VregPass would leave it): entry-edge move + backedge move
// alias ONE vreg. Exactly one interval spans the synthetic redefinition
// point at the tail — the loop stays closed.
//
// Stream + ids: kStart=2, entry=4, entry_move=6, kBB1=8, upd=10, dead=12,
// deaduse=14, backedge_move=16, back=18, kEnd=20.
TEST_CASE("linear-scan-ra: multi-def vreg keeps one interval",
          "[linear-scan-ra]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* entry_move = fx.Append1(HOpcode::kMov, entry->output());
  auto* bb1_label = as->TestBB1();
  auto* upd = fx.Append1(HOpcode::kDupI, entry_move->output());
  auto* dead = fx.Append1(HOpcode::kDupI, entry_move->output());
  auto* deaduse = fx.Append1(HOpcode::kDupI, dead->output());
  auto* backedge_move = fx.Append1(HOpcode::kMov, upd->output());
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();
  as->TestFinalize();

  VregContext ctx{fx.quake->zone()};
  auto ventry = ctx.GetOrNewVreg(entry->output());
  auto vinduction = ctx.GetOrNewVreg(entry_move->output());
  auto vupd = ctx.GetOrNewVreg(upd->output());
  auto vdead = ctx.GetOrNewVreg(dead->output());
  auto vdeaduse = ctx.GetOrNewVreg(deaduse->output());
  CHECK(ctx.MapAlias(backedge_move->output(), vinduction) == vinduction);
  REQUIRE(ctx.vreg_count() == 5);
  (void)vdeaduse;

  fx.Analyze(&ctx);

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* induction = fx.IntervalOf(vinduction);
  REQUIRE(induction != nullptr);

  // one interval, from the first def (the entry-edge move) through the
  // synthetic redef at the backedge move — not truncated at the last READ.
  // Pre-authorized correction (plan 02.1-01 Task 2, 02.1-01-HARD-STOP.md):
  // the G-01 layer-1 CFG extension legitimately lengthens the induction
  // interval to the loop block's tail (the backedge jump, id 18): the
  // value is live out of BB1 around the loop, so the extent follows the
  // dataflow solution (16 -> 18).
  REQUIRE(induction->def.point() == 6);
  REQUIRE(induction->last.point() == 18);
  REQUIRE(!induction->in_memory);
  REQUIRE(induction->assigned != nullptr);
  REQUIRE(induction->assigned->IsGP());

  // the block-local values got their own registers; the entry const
  // EXPIRES at the entry-edge move (its only use, same instruction as the
  // induction def), so the induction interval takes over its register —
  // the mov's read-then-write sharing
  const auto* ientry = fx.IntervalOf(ventry);
  const auto* iupd = fx.IntervalOf(vupd);
  const auto* idead = fx.IntervalOf(vdead);
  REQUIRE(ientry != nullptr);
  REQUIRE(iupd != nullptr);
  REQUIRE(idead != nullptr);
  REQUIRE(ientry->assigned == induction->assigned);
  REQUIRE(iupd->assigned != induction->assigned);
  REQUIRE(idead->last.point() == 14);
}

// -----------------------------------------------------------------------------
// Shape: extreme bank exhaustion at the very point a spilled value must
// reload: thirteen GP values all used at ONE instruction (the thirteenth is
// the scan's spill victim), so the spilled interval's reload finds no free
// register. The documented D-23 behavior for this shape was a base::Error
// bail; the live cutover (plan 02-05) replaced the bail with the legacy-
// tolerated slot-read fallback — the operand materializes to its spill slot
// and translate reads the memory operand directly. Run must SUCCEED and the
// thirteenth operand must read the slot (never a crash, never a silent
// wrong register).
TEST_CASE("linear-scan-ra: unhandleable shape reads the spill slot",
          "[linear-scan-ra]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  // Thirteen values defined up front; ALL are operands of the same
  // multi-operand instruction E; the thirteenth carries one extra later
  // use so it (furthest last) is the spill victim. At E every register of
  // the bank is held by a value used at E — none is free for the reload.
  as->TestStart();
  constexpr int kValueCount = 13;  // > 12 free GP registers
  std::vector<HInstruction*> def(kValueCount);
  for (int i = 0; i < kValueCount; i++) {
    def[i] = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(i)));
  }

  auto* eater = fx.AppendN(HOpcode::kEnterInlineFrame, kValueCount);
  for (int i = 0; i < kValueCount; i++) {
    eater->OprSet(i, def[i]->output());
  }

  fx.Append1(HOpcode::kDupI, def[kValueCount - 1]->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(def[0]));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err.Has());
  // The spilled thirteenth value reads its slot directly at E (the
  // slot-read fallback — translate tolerates the memory operand).
  auto* last = eater->Opr(kValueCount - 1);
  REQUIRE(last != nullptr);
  REQUIRE(last->IsMemStack());
}

// -----------------------------------------------------------------------------
// Task 2 — PHI edge-move conversion (PCOPY-02) and constraint satisfaction.
//
// Helper: find the single kParallelCopy immediately before an anchor (or
// nullptr).
HInstruction* CopyBefore(HAssembler* as, HInstruction* anchor) {
  auto* prev = anchor->prev();
  return prev != nullptr && prev->opcode() == HOpcode::kParallelCopy ? prev
                                                                     : nullptr;
}

// Pair slot equality helper against interval locations. Physical-register
// comparison: materialized operands carry the interval's size-class VIEW
// of the register (eax vs rax), which aliases one physical register.
bool PairHasDst(HInstruction* copy, const Register* r) {
  for (int i = 0; i < ParallelCopyPairCount(copy); ++i) {
    auto* dst = ParallelCopyDst(copy, i);
    if (dst->IsRegister() &&
        dst->GetRegister()->register_kind() == r->register_kind()) {
      return true;
    }
  }
  return false;
}

// -----------------------------------------------------------------------------
// PCOPY-02 (a): two loop phis whose entry moves and backedge moves all
// SURVIVE (their endpoints interfere: every entry source is reused inside
// the body). Each shuffle anchor must end up with exactly ONE kParallelCopy
// holding both pairs, and no sequential provenance kMov may remain.
TEST_CASE("linear-scan-ra: edge moves convert to one parallel copy per anchor",
          "[linear-scan-ra][pcopy]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* src_i = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* src_j = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* bb1_label = as->TestBB1();

  // the honest induction chains, both phi values used in the body, and
  // BOTH entry sources reused in the body (the interference that keeps
  // every edge move alive through coalescing)
  auto* use_i = fx.AppendN(HOpcode::kDupI, 1);
  auto* use_j = fx.AppendN(HOpcode::kDupI, 1);
  auto* re_i = fx.Append1(HOpcode::kDupI, src_i->output());
  auto* re_j = fx.Append1(HOpcode::kDupI, src_j->output());
  auto* upd_i = fx.AppendN(HOpcode::kDupI, 1);
  auto* upd_j = fx.AppendN(HOpcode::kDupI, 1);
  fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi_i = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi_i->PopulateOutput(as);
  use_i->OprSet(0, phi_i->output());
  use_i->PopulateOutput(as);
  upd_i->OprSet(0, use_i->output());
  upd_i->PopulateOutput(as);
  phi_i->OprSet(0, src_i->output());
  phi_i->OprSet(1, upd_i->output());

  auto* phi_j = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi_j->PopulateOutput(as);
  use_j->OprSet(0, phi_j->output());
  use_j->PopulateOutput(as);
  upd_j->OprSet(0, use_j->output());
  upd_j->PopulateOutput(as);
  phi_j->OprSet(0, src_j->output());
  phi_j->OprSet(1, upd_j->output());
  re_i->PopulateOutput(as);
  re_j->PopulateOutput(as);
  as->TestFinalize();

  fx.RunVregThenLiveness();

  // sanity: four provenance edge moves exist pre-RA (2 entry + 2 backedge)
  std::size_t provenance = 0;
  fx.ctx->ForeachEdgeMove([&](HInstruction*, EdgeMoveGroup) { provenance++; });
  REQUIRE(provenance == 4);

  auto err = fx.Allocate();
  REQUIRE(!err);

  // exactly two parallel copies — one per shuffle anchor
  REQUIRE(CountOpcode(as, HOpcode::kParallelCopy) == 2);
  REQUIRE(CountOpcode(as, HOpcode::kMov) == 0);

  auto* bb0_copy = CopyBefore(as, as->bb0_register_shuffle());
  auto* bb1_copy = CopyBefore(as, as->bb1_register_shuffle());
  REQUIRE(bb0_copy != nullptr);
  REQUIRE(bb1_copy != nullptr);
  REQUIRE(ParallelCopyPairCount(bb0_copy) == 2);
  REQUIRE(ParallelCopyPairCount(bb1_copy) == 2);

  // the bb0 copy lands both entry sources into the phi vregs' locations
  const auto* ii = fx.IntervalOf(fx.VregOf(phi_i->output()));
  const auto* ij = fx.IntervalOf(fx.VregOf(phi_j->output()));
  REQUIRE(PairHasDst(bb0_copy, ii->assigned));
  REQUIRE(PairHasDst(bb0_copy, ij->assigned));

  // no provenance-tracked edge move survives as a kMov anywhere
  fx.ctx->ForeachEdgeMove([&](HInstruction* move, EdgeMoveGroup) {
    if (move->Opr(0) != nullptr) {
      // still linked with live operands — must NOT be a plain kMov anymore
      REQUIRE(move->opcode() == HOpcode::kParallelCopy);
    }
  });

  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// PCOPY-02 (b): the fully-coalesced entry group — endpoints disjoint, every
// entry move merges. The bb0 anchor emits NO kParallelCopy at all (the
// backedge group still survives: its endpoints genuinely interfere).
TEST_CASE("linear-scan-ra: fully-coalesced group emits no parallel copy",
          "[linear-scan-ra][pcopy]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* src_i = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* bb1_label = as->TestBB1();
  auto* use_i = fx.AppendN(HOpcode::kDupI, 1);
  auto* upd_i = fx.AppendN(HOpcode::kDupI, 1);
  fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi_i = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi_i->PopulateOutput(as);
  use_i->OprSet(0, phi_i->output());
  use_i->PopulateOutput(as);
  upd_i->OprSet(0, use_i->output());
  upd_i->PopulateOutput(as);
  phi_i->OprSet(0, src_i->output());
  phi_i->OprSet(1, upd_i->output());
  as->TestFinalize();

  fx.RunVregThenLiveness();
  fx.Coalesce();
  fx.ReAnalyze();

  // the entry move merged, the backedge move survived
  std::size_t survivors = 0;
  fx.ctx->ForeachEdgeMove([&](HInstruction* move, EdgeMoveGroup) {
    if (move->Opr(0) != nullptr) survivors++;
  });
  REQUIRE(survivors == 1);

  auto err = fx.Allocate();
  REQUIRE(!err);

  // nothing at the bb0 anchor at all — no copy, no mov
  REQUIRE(CopyBefore(as, as->bb0_register_shuffle()) == nullptr);
  REQUIRE(as->bb0_register_shuffle()->prev()->opcode() != HOpcode::kMov);
  // the surviving backedge group became exactly one copy at bb1
  REQUIRE(CountOpcode(as, HOpcode::kParallelCopy) == 1);
  REQUIRE(CopyBefore(as, as->bb1_register_shuffle()) != nullptr);
  REQUIRE(CountOpcode(as, HOpcode::kMov) == 0);

  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// PCOPY-02 (c): branch-phi predecessor-arm moves convert at their arm
// anchors (the fast/slow join shape — exactly what HAssembler::FastSlow
// emits, per the vreg-pass fixture).
TEST_CASE("linear-scan-ra: branch-phi arm moves convert at both arms",
          "[linear-scan-ra][pcopy]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  // the honest real-trace shape per the coalescing fixture: the branch phi
  // lives inside the mandatory loop skeleton, and the fast arm's jump
  // exits to the merge user through a BOUND label (the CFG materializer
  // requires every label operand to carry a bind)
  as->TestStart();
  auto* src_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(7)));
  auto* bb1_label = as->TestBB1();
  auto* lhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* lhs_block = fx.Append(HOpcode::kBlock);
  auto* jmp = fx.AppendN(HOpcode::kJmp, 1);  // wired below -> merge user
  auto* rhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* rhs_block = fx.Append(HOpcode::kBlock);
  auto* upd_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* user_branch = fx.AppendN(HOpcode::kDupI, 1);
  auto* user_loop = fx.AppendN(HOpcode::kDupI, 1);
  // keep the lhs arm value live PAST the arm move: the read-then-write
  // sharing at the move would otherwise make its pair a legitimate
  // identity (dropped by the converter) — here it must be a real transfer
  fx.Append1(HOpcode::kDupI, lhs_def->output());
  fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  jmp->OprSet(0, as->NewLabel(user_branch));

  auto* loop_phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  loop_phi->PopulateOutput(as);
  loop_phi->OprSet(0, src_def->output());
  loop_phi->OprSet(1, upd_def->output());
  user_loop->OprSet(0, loop_phi->output());
  user_loop->PopulateOutput(as);

  auto* phi = fx.AppendN(HOpcode::kPhiBranchI, 4);
  phi->OprSet(0, lhs_def->output());
  phi->OprSet(1, rhs_def->output());
  phi->OprSet(2, as->NewLabel(lhs_block));
  phi->OprSet(3, as->NewLabel(rhs_block));
  phi->PopulateOutput(as);
  user_branch->OprSet(0, phi->output());
  user_branch->PopulateOutput(as);
  as->TestFinalize();

  fx.RunVregThenLiveness();


    auto err = fx.Allocate();
    REQUIRE(!err);


  // both arm anchors got their single-pair parallel copy; no kMov remains
  // anywhere (the loop phi's moves converted at the shuffle anchors too)
  auto* lhs_copy = CopyBefore(as, jmp);
  auto* rhs_copy = CopyBefore(as, upd_def);
  REQUIRE(lhs_copy != nullptr);
  REQUIRE(rhs_copy != nullptr);
  REQUIRE(ParallelCopyPairCount(lhs_copy) == 1);
  REQUIRE(ParallelCopyPairCount(rhs_copy) == 1);
  REQUIRE(CountOpcode(as, HOpcode::kMov) == 0);

  // each arm copy lands its arm value into the phi vreg's location
  const auto* iphi = fx.IntervalOf(fx.VregOf(phi->output()));
  REQUIRE(PairHasDst(lhs_copy, iphi->assigned));
  REQUIRE(PairHasDst(rhs_copy, iphi->assigned));

  REQUIRE(!fx.Validate());
}
// -----------------------------------------------------------------------------
// Fixed output register + multi-scratch: kIMod declares output rdx and
// scratch {rax, rdx}. The output interval is relocated into rdx by one
// pre-instruction parallel copy and moved back to its canonical register by
// a post-instruction mov.
TEST_CASE("linear-scan-ra: fixed output register with scratch clobbers",
          "[linear-scan-ra][constraint]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(17)));
  auto* b = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(5)));
  auto* mod = fx.AppendN(HOpcode::kIMod, 2);
  mod->OprSet(0, a->output());
  mod->OprSet(1, b->output());
  mod->PopulateOutput(as);
  fx.Append1(HOpcode::kDupI, mod->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* imod = fx.IntervalOf(fx.VregOf(mod));
  const auto* rdx = Register::GetRegisterFromKind(RegisterKind::kRdx);
  REQUIRE(imod->assigned != rdx);  // the scan is constraint-oblivious

  // The output constraint is satisfied at the instruction boundary: the
  // idiv writes rdx (translate stages dividend/divisor internally — no
  // pre-instruction copy for an OUTPUT constraint), and one relocation
  // mov returns the value to the interval's canonical register.
  REQUIRE(CopyBefore(as, mod) == nullptr);
  auto* fixup = mod->next();
  REQUIRE(fixup->opcode() == HOpcode::kMov);
  REQUIRE(fixup->Opr(0)->IsRegister());
  REQUIRE(fixup->Opr(0)->GetRegister()->register_kind() ==
        rdx->register_kind());
  REQUIRE(fixup->output()->IsRegister());
  REQUIRE(fixup->output()->GetRegister()->register_kind() ==
        imod->assigned->register_kind());

  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Same-as (two-operand rule): kIAdd's output must share operand 0's
// register. The input is live PAST the add here, so the scan gives the
// output its own register and the constraint copy lands operand 0 in it.
TEST_CASE("linear-scan-ra: same-as-output lands operand 0 in the output register",
          "[linear-scan-ra][constraint]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(3)));
  auto* b = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(4)));
  auto* add = fx.AppendN(HOpcode::kIAdd, 2);
  add->OprSet(0, a->output());
  add->OprSet(1, b->output());
  add->PopulateOutput(as);
  fx.Append1(HOpcode::kDupI, add->output());
  fx.Append1(HOpcode::kDupI, a->output());  // a survives the add
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* iadd = fx.IntervalOf(fx.VregOf(add));
  const auto* ia = fx.IntervalOf(fx.VregOf(a));
  REQUIRE(iadd->assigned != ia->assigned);

  // operand 0 already sits in the register the instruction computes in —
  // no pre-instruction parallel copy at all
  REQUIRE(CopyBefore(as, add) == nullptr);

  // the anchor (live past the add, which destroys the shared register) is
  // parked around the instruction: store before, reload after
  auto* store = add->prev();
  REQUIRE(store->opcode() == HOpcode::kMov);
  REQUIRE(store->Opr(0)->IsRegister());
  REQUIRE(store->Opr(0)->GetRegister()->register_kind() ==
        ia->assigned->register_kind());
  REQUIRE(store->output()->IsMemStack());
  auto* fixup = add->next();
  REQUIRE(fixup->opcode() == HOpcode::kMov);
  REQUIRE(fixup->Opr(0)->IsRegister());
  REQUIRE(fixup->Opr(0)->GetRegister()->register_kind() ==
        ia->assigned->register_kind());  // shared register
  REQUIRE(fixup->output()->IsRegister());
  REQUIRE(fixup->output()->GetRegister()->register_kind() ==
        iadd->assigned->register_kind());  // canonical
  auto* reload = fixup->next();
  REQUIRE(reload->opcode() == HOpcode::kMov);
  REQUIRE(reload->Opr(0)->IsMemStack());
  REQUIRE(reload->Opr(0)->GetMemStack().slot() ==
          store->output()->GetMemStack().slot());
  REQUIRE(reload->output()->IsRegister());
  REQUIRE(reload->output()->GetRegister()->register_kind() ==
        ia->assigned->register_kind());

  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Call argument landing: a GP RCall's RHS operand set lands in the ABI
// argument registers simultaneously (one parallel copy), and the FP RCall
// lands its argument in xmm0 — which is also its fixed output register.
TEST_CASE("linear-scan-ra: rcall arguments land in argument registers",
          "[linear-scan-ra][constraint]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* x = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(11)));
  auto* y = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(12)));
  auto* call = fx.AppendN(HOpcode::kRCallBox, 3);
  call->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kToObject));
  call->OprSet(1, x->output());
  call->OprSet(2, y->output());
  call->PopulateOutput(as);
  fx.Append1(HOpcode::kDupI, call->output());

  auto* f = fx.Append1(HOpcode::kMovF64, as->NewImmediate(2.5));
  auto* fcall = fx.AppendN(HOpcode::kRCallF64, 2);
  fcall->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kFPow));
  fcall->OprSet(1, f->output());
  fcall->PopulateOutput(as);
  fx.Append1(HOpcode::kDupI, fcall->output());

  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(x));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* ix = fx.IntervalOf(fx.VregOf(x));
  const auto* iy = fx.IntervalOf(fx.VregOf(y));
  const auto* iff = fx.IntervalOf(fx.VregOf(f));

  // GP call: ONE parallel copy, both args at once (rsi = hidden+1, rdx)
  auto* gpcopy = CopyBefore(as, call);
  REQUIRE(gpcopy != nullptr);
  REQUIRE(ParallelCopyPairCount(gpcopy) == 2);
  const auto* rsi = Register::GetRegisterFromKind(RegisterKind::kRsi);
  const auto* rdx = Register::GetRegisterFromKind(RegisterKind::kRdx);
  REQUIRE(PairHasDst(gpcopy, rsi));
  REQUIRE(PairHasDst(gpcopy, rdx));
  (void)ix;
  (void)iy;

  // FP call: one pair into xmm0 (the arg register == the fixed output)
  auto* fpcopy = CopyBefore(as, fcall);
  REQUIRE(fpcopy != nullptr);
  REQUIRE(ParallelCopyPairCount(fpcopy) == 1);
  const auto* xmm0 = Register::GetRegisterFromKind(RegisterKind::kXmm0);
  REQUIRE(PairHasDst(fpcopy, xmm0));
  REQUIRE(ParallelCopySrc(fpcopy, 0)->GetRegister()->register_kind() ==
        iff->assigned->register_kind());

  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Clobber eviction: a value live across an ABI call (caller-saved clobber
// sets) is parked around the call — store before, reload after — through
// the generic clobber machinery (no fast/slow special casing).
TEST_CASE("linear-scan-ra: caller-saved clobber evicts live ranges",
          "[linear-scan-ra][constraint]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* v = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(42)));
  auto* call = fx.AppendN(HOpcode::kRCallVoid, 1);
  call->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kGCFree));
  fx.Append1(HOpcode::kDupI, v->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(v));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* iv = fx.IntervalOf(fx.VregOf(v));
  REQUIRE(iv->assigned != nullptr);
  REQUIRE(!iv->in_memory);
  // v's assigned register is caller-saved (first of the free list), so the
  // call kills it and the plan parks the value
  const auto* r10 = Register::GetRegisterFromKind(RegisterKind::kR10);
  REQUIRE(iv->assigned == r10);

  // store before the call: r10 -> slot
  auto* store = call->prev();
  REQUIRE(store->opcode() == HOpcode::kMov);
  REQUIRE(store->Opr(0)->IsRegister());
  REQUIRE(store->Opr(0)->GetRegister()->register_kind() ==
        r10->register_kind());
  REQUIRE(store->output()->IsMemStack());
  // reload after the call: slot -> r10
  auto* reload = call->next();
  REQUIRE(reload->opcode() == HOpcode::kMov);
  REQUIRE(reload->Opr(0)->IsMemStack());
  REQUIRE(reload->Opr(0)->GetMemStack().slot() ==
          store->output()->GetMemStack().slot());
  REQUIRE(reload->output()->IsRegister());
  REQUIRE(reload->output()->GetRegister()->register_kind() ==
        r10->register_kind());

  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Fast/slow join transfer: the exact FastSlow stream shape (kBlock fast
// tail marker, kSlowBlockStart/OpStart/RCall/OpDone/End slow arm, branch
// phi merge). The join state transfer is the generic branch-phi edge
// conversion; the slow block's RCall is satisfied by the generic call
// machinery; NO policy switch exists anywhere.
TEST_CASE("linear-scan-ra: fast/slow join transfers through parallel copies",
          "[linear-scan-ra][fastslow]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* src_def =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(7)));
  auto* bb1_label = as->TestBB1();
  auto* lhs_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* lhs_block = fx.Append(HOpcode::kBlock);
  auto* jmp = fx.AppendN(HOpcode::kJmp, 1);  // wired below -> merge user

  fx.Append(HOpcode::kSlowBlockStart);
  fx.Append(HOpcode::kSlowBlockOpStart);
  auto* rcall = fx.AppendN(HOpcode::kRCallI32, 1);
  rcall->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kInt32ToStr));
  rcall->PopulateOutput(as);
  fx.Append(HOpcode::kSlowBlockOpDone);
  auto* rhs_block = fx.Append(HOpcode::kSlowBlockEnd);

  auto* upd_def = fx.Append1(HOpcode::kDupI, src_def->output());
  auto* user = fx.AppendN(HOpcode::kDupI, 1);        // the merge user
  auto* user_loop = fx.AppendN(HOpcode::kDupI, 1);
  // keep the fast-path value live past the arm move (otherwise the
  // read-then-write sharing at the move makes its pair a dropped identity)
  fx.Append1(HOpcode::kDupI, lhs_def->output());
  fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  jmp->OprSet(0, as->NewLabel(user));

  auto* loop_phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  loop_phi->PopulateOutput(as);
  loop_phi->OprSet(0, src_def->output());
  loop_phi->OprSet(1, upd_def->output());
  user_loop->OprSet(0, loop_phi->output());
  user_loop->PopulateOutput(as);

  auto* phi = fx.AppendN(HOpcode::kPhiBranchI, 4);
  phi->OprSet(0, lhs_def->output());
  phi->OprSet(1, rcall->output());
  phi->OprSet(2, as->NewLabel(lhs_block));
  phi->OprSet(3, as->NewLabel(rhs_block));
  phi->PopulateOutput(as);
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  // both arm anchors carry their single-pair parallel copy; the slow
  // block's RCall (argless here) needed no landing copy; no kMov anywhere
  auto* lhs_copy = CopyBefore(as, jmp);
  auto* rhs_copy = CopyBefore(as, upd_def);
  REQUIRE(lhs_copy != nullptr);
  REQUIRE(rhs_copy != nullptr);
  REQUIRE(ParallelCopyPairCount(lhs_copy) == 1);
  REQUIRE(ParallelCopyPairCount(rhs_copy) == 1);
  // no sequential edge-move path remains, and the only movs are the
  // clobber-eviction parks around the slow block's RCall — bracketed
  // store/reload pairs (the generic call machinery, no policy switch)
  auto movs = CountOpcode(as, HOpcode::kMov);
  REQUIRE(movs > 0);
  REQUIRE(movs % 2 == 0);

  // the whole stream — slow block included — validates clean
  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// D-31 snapshot boundary: an exit-bearing call whose checkpoint references
// the very value the argument parallel copy moves — the snapshot must
// describe the POST-copy location (the copy's destination), never the
// pre-copy source slot. The RAValidator's boundary check proves it (its
// dedicated negative case — a pure-source reference — is pinned in the
// 02-03 suite).
TEST_CASE("linear-scan-ra: snapshot across a parallel copy is post-copy",
          "[linear-scan-ra][snapshot]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();
  auto* zone = fx.quake->zone();

  as->TestStart();
  auto* x = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(19)));
  auto* call = fx.AppendN(HOpcode::kRCallBox, 2);
  call->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kToObject));
  call->OprSet(1, x->output());
  call->PopulateOutput(as);
  fx.Append1(HOpcode::kDupI, call->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(x));
  as->TestFinalize();

  // Hand-built checkpoint referencing x's output: the exact shape of what
  // LIRLower installs on guard deopts (see liveness-test shape 3).
  auto* node = zone->New<ir::Node>(fx.quake.get());
  auto* ssa = zone->New<ir::SSAVar>(fx.quake.get(), 0, node);
  auto* ck = zone->New<ir::Checkpoint>(fx.quake.get(), 0,
                                       ir::Checkpoint::kSideEffect, node,
                                       false);
  ck->frame_vector().emplace_back(ck, zone, JitFrameType::kJitEntry, nullptr,
                                  nullptr, 1, 0, 0, nullptr);
  auto& frame = ck->frame_vector().back();
  frame.value_list.Reserve(2);
  frame.value_list.PushBack(ir::Checkpoint::kVal, ssa,
                            ir::Checkpoint::Origin::FromTemp(0));
  ssa->set_lir_operand(x->output());
  call->set_exit(ck);
  REQUIRE(call->HasExit());

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  // the argument landed in its ABI register through the parallel copy
  // immediately preceding the call, and the snapshot was constructed at
  // the instruction boundary (the deopt id is published)
  REQUIRE(call->prev()->opcode() == HOpcode::kParallelCopy);
  REQUIRE(call->HasDeoptId());

  // D-31: every snapshot MachineRef is a post-copy location — the
  // validator's boundary check fails on any pure pre-copy source slot
  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// G-01 pins (phase 02.1-01): the interval-vs-CFG liveness disagreement on
// loop-carried values used at loop-header state snapshots, with a
// caller-saved-clobbering call inside the loop body — the exact trace
// class of the corpus reproducer unittest/jit/case/crash/crash_6.js
// (RAValidator: r9 clobber_gp of rcall_void #1941). The linear point-list
// interval of a loop-carried vreg is [first def, last def/use]; the value
// the backedge move defines is live from that def around the loop to the
// loop-header checkpoint use — across every body call. Pre-fix the park
// filter sees the call after the interval's last point and does not park;
// the validator's CFG-accurate register liveness keeps the register live
// across the call and fails loudly.

// The direct model assertion both fixtures pin: for every vreg the
// dataflow solution marks live at a block boundary, the allocated interval
// must cover the block's whole numbered instruction span — the planner's
// coverage can never drift from the CFG-accurate model the validator
// reconstructs (D-2.1-04 direction (b), one shared coverage notion).
void RequireCfgCoverage(const LinearScanFixture& fx) {
  const auto* liveness = fx.analysis.get();
  liveness->cfg()->ForeachBasicBlock(
      [&](const BasicBlock* bb) -> bool {
        std::uint32_t lo = std::numeric_limits<std::uint32_t>::max();
        std::uint32_t hi = 0;
        for (const auto* ins = bb->start(); ins != bb->end();
             ins = ins->next()) {
          const auto id = ins->liveness_info().liveness_id();
          if (id == kInvalidLivenessId) continue;
          lo = std::min(lo, id);
          hi = std::max(hi, id);
        }
        if (lo == std::numeric_limits<std::uint32_t>::max()) return true;
        const auto bid = bb->block_id();
        for (std::uint32_t v = 0; v < fx.ctx->vreg_count(); ++v) {
          if (!liveness->LiveIn(bid, v) && !liveness->LiveOut(bid, v)) {
            continue;
          }
          const auto* iv = fx.IntervalOf(v);
          if (iv == nullptr) continue;  // absorbed-id hole: no interval
          // The two boundary requirements are asymmetric, matching the
          // dataflow semantics: LiveIn means a use reaches the block top
          // from a def outside the block, so the interval must extend to
          // the block head; LiveOut means the value flows past the block
          // bottom (possibly defined inside it), so the interval must
          // extend to the block tail. The G-01 under-extension is a
          // LiveOut-side miss: the backedge def closes the linear range
          // inside a block the value is live out of.
          if (liveness->LiveIn(bid, v)) {
            REQUIRE(iv->def.point() <= lo);
          }
          if (liveness->LiveOut(bid, v)) {
            REQUIRE(iv->last.point() >= hi);
          }
        }
        return true;
      });
}

// Hand-build a side-exit checkpoint carrying exactly one value entry whose
// SSAVar's lir_operand is `value` — the minimal honest shape of what
// LIRLower installs on guard deopts (same construction as the liveness-
// test side-exit fixture and the D-31 snapshot case above), attaching it
// to `ins`.
void AttachSnapshot(LinearScanFixture* fx, HInstruction* ins,
                    HOperand* value) {
  auto* zone = fx->quake->zone();
  auto* node = zone->New<ir::Node>(fx->quake.get());
  auto* ssa = zone->New<ir::SSAVar>(fx->quake.get(), 0, node);
  auto* ck = zone->New<ir::Checkpoint>(fx->quake.get(), 0,
                                       ir::Checkpoint::kSideEffect, node,
                                       false);
  ck->frame_vector().emplace_back(ck, zone, JitFrameType::kJitEntry, nullptr,
                                  nullptr, 1, 0, 0, nullptr);
  auto& frame = ck->frame_vector().back();
  frame.value_list.Reserve(2);
  frame.value_list.PushBack(ir::Checkpoint::kVal, ssa,
                            ir::Checkpoint::Origin::FromTemp(0));
  ssa->set_lir_operand(value);
  ins->set_exit(ck);
  REQUIRE(ins->HasExit());
}

// -----------------------------------------------------------------------------
// Fixture A — the disagreement pin (mirrors unittest/jit/case/crash/
// crash_6.js): one loop-carried value read by a loop-header state snapshot,
// its backedge move defining the next-iteration value BEFORE a body call.
//
// Stream: [kStart][entry][entry_move][kBB1][snapshot][upd][backedge_move]
//         [call][back][kEnd]
// The induction vreg is multi-def (entry move + aliased backedge move, the
// exact VregPass registration shape of the multi-def fixture above). Its
// linear interval is [entry_move, backedge_move]; the call sits after the
// backedge def, so pre-fix no park is planned — but the value the backedge
// move defines is live from there around the loop to the loop-header
// snapshot use, across the call. RED pre-fix: the coverage assertion above
// fails (interval ends at the backedge def, inside the live block), and
// the validator reports the assigned register as clobber_gp of rcall_void.
TEST_CASE("linear-scan-ra: G-01 interval-coverage disagreement - loop-carried "
          "snapshot value vs body call",
          "[linear-scan-ra][g01]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* entry_move = fx.Append1(HOpcode::kMov, entry->output());
  auto* bb1_label = as->TestBB1();
  // the loop-header state snapshot: a guard deopt whose checkpoint carries
  // the loop-carried value (the crash_6.js slow-block/loop-header shape)
  auto* snapshot = fx.Append1(HOpcode::kDeoptZ, entry_move->output());
  auto* upd = fx.Append1(HOpcode::kDupI, entry_move->output());
  auto* backedge_move = fx.Append1(HOpcode::kMov, upd->output());
  auto* call = fx.AppendN(HOpcode::kRCallVoid, 1);
  call->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kGCFree));
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();
  as->TestFinalize();

  AttachSnapshot(&fx, snapshot, entry_move->output());

  // multi-def registration, exactly as VregPass leaves it (the multi-def
  // fixture above): the backedge move aliases the induction vreg
  VregContext ctx{fx.quake->zone()};
  auto ventry = ctx.GetOrNewVreg(entry->output());
  auto vinduction = ctx.GetOrNewVreg(entry_move->output());
  auto vupd = ctx.GetOrNewVreg(upd->output());
  // Harness AddDef records, mirroring the real VregPass registration
  // (src/lir/vreg-pass.cc:215,226): park emission reads producer links via
  // LinkFor/FirstLinkedDef in materialization, so every hand-registered def
  // needs its AddDef — without it the layer-1 extension's planned parks
  // misread producer links (02.1-01-HARD-STOP.md fixture-harness defect).
  ctx.AddDef(ventry, entry);
  ctx.AddDef(vinduction, entry_move);
  ctx.AddDef(vupd, upd);
  CHECK(ctx.MapAlias(backedge_move->output(), vinduction) == vinduction);
  ctx.AddDef(vinduction, backedge_move);
  REQUIRE(ctx.vreg_count() == 3);
  (void)ventry;
  (void)vupd;

  fx.Analyze(&ctx);
  fx.Coalesce();   // canonical order (provenance no-op on this context)
  fx.ReAnalyze();  // FRESH dataflow (the D-19 arm)

  auto err = fx.Allocate();
  REQUIRE(!err);

  // direct model assertion: RED pre-fix — the induction interval ends at
  // the backedge def, inside the block the dataflow marks live
  RequireCfgCoverage(fx);

  // the validator must see no register holding a CFG-live value across
  // the body call (pre-fix: <assigned> clobber_gp of rcall_void)
  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Fixture B — the crash_6.js trace class at corpus geometry (regression
// pin for unittest/jit/case/crash/crash_6.js): two loop-carried values,
// one read by the loop-header state snapshot, both redefined by backedge
// moves before TWO caller-saved-clobbering body calls. RED pre-fix (the
// same clobber_gp validator class fires on the first unparked register);
// post-fix the validator is clean and every interval covers its live
// blocks.
TEST_CASE("linear-scan-ra: G-01 interval coverage - crash_6.js class, two "
          "inductions, two body calls",
          "[linear-scan-ra][g01]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry_i =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* entry_move_i = fx.Append1(HOpcode::kMov, entry_i->output());
  auto* entry_j =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* entry_move_j = fx.Append1(HOpcode::kMov, entry_j->output());
  auto* bb1_label = as->TestBB1();
  auto* snapshot_i = fx.Append1(HOpcode::kDeoptZ, entry_move_i->output());
  auto* use_j = fx.Append1(HOpcode::kDupI, entry_move_j->output());
  auto* upd_i = fx.Append1(HOpcode::kDupI, entry_move_i->output());
  auto* upd_j = fx.Append1(HOpcode::kDupI, entry_move_j->output());
  auto* backedge_move_i = fx.Append1(HOpcode::kMov, upd_i->output());
  auto* backedge_move_j = fx.Append1(HOpcode::kMov, upd_j->output());
  auto* call1 = fx.AppendN(HOpcode::kRCallVoid, 1);
  call1->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kGCFree));
  auto* call2 = fx.AppendN(HOpcode::kRCallVoid, 1);
  call2->OprSet(0, as->NewBuiltinCallIndex(BuiltinFunction::kGCFree));
  [[maybe_unused]] auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();
  as->TestFinalize();

  AttachSnapshot(&fx, snapshot_i, entry_move_i->output());

  VregContext ctx{fx.quake->zone()};
  auto ventry_i = ctx.GetOrNewVreg(entry_i->output());
  auto vinduction_i = ctx.GetOrNewVreg(entry_move_i->output());
  auto ventry_j = ctx.GetOrNewVreg(entry_j->output());
  auto vinduction_j = ctx.GetOrNewVreg(entry_move_j->output());
  auto vupd_i = ctx.GetOrNewVreg(upd_i->output());
  auto vupd_j = ctx.GetOrNewVreg(upd_j->output());
  auto vuse_j = ctx.GetOrNewVreg(use_j->output());
  // Harness AddDef records, mirroring the real VregPass registration
  // (src/lir/vreg-pass.cc:215,226) — same rationale as Fixture A above.
  ctx.AddDef(ventry_i, entry_i);
  ctx.AddDef(vinduction_i, entry_move_i);
  ctx.AddDef(ventry_j, entry_j);
  ctx.AddDef(vinduction_j, entry_move_j);
  ctx.AddDef(vupd_i, upd_i);
  ctx.AddDef(vupd_j, upd_j);
  ctx.AddDef(vuse_j, use_j);
  CHECK(ctx.MapAlias(backedge_move_i->output(), vinduction_i) ==
        vinduction_i);
  ctx.AddDef(vinduction_i, backedge_move_i);
  CHECK(ctx.MapAlias(backedge_move_j->output(), vinduction_j) ==
        vinduction_j);
  ctx.AddDef(vinduction_j, backedge_move_j);
  REQUIRE(ctx.vreg_count() == 7);
  (void)ventry_i;
  (void)ventry_j;
  (void)vupd_i;
  (void)vupd_j;
  (void)vuse_j;

  fx.Analyze(&ctx);
  fx.Coalesce();   // canonical order (provenance no-op on this context)
  fx.ReAnalyze();  // FRESH dataflow (the D-19 arm)

  auto err = fx.Allocate();
  REQUIRE(!err);

  // direct model assertion (both inductions are under-extended pre-fix)
  RequireCfgCoverage(fx);

  // the crash_6.js class regression gate: validator-clean allocation
  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Fixture C — layer-2 policy pin (plan 02.1-01 Task 3, G-04): a whole-range
// spilled interval referenced at an exit-bearing instruction ONLY by a deopt
// checkpoint entry is read directly from its canonical spill slot — NO
// reload mov, the entry's materialized operand IsMemStack() and encodes as
// Deopt::kSpill (the D-2.1-07 slot-read encoding precedent). A reload here
// creates a register def/use pair that post-insertion block boundaries can
// separate (rotated-header leaders), and the validator's register-level
// fixpoint then bridges that register across loop backedges — the
// crash_6.js r9 clobber_gp of rcall_void #1138 residual
// (02.1-01-HARD-STOP.md). Reading the entry from the slot removes the
// register liveness entirely.

// The layer-2 encoding receipt: the materialized checkpoint reference IS
// the canonical spill slot (Deopt::kSpill, produced by Snapshot::
// MapMachineRef's CHECK(lir_opr->IsMemStack()) path).
bool RefIsMemStackSlot(const Deopt::MachineRef& ref,
                       MemoryStack::Slot slot) {
  return ref.IsSpill() && ref.Slot() == slot;
}

TEST_CASE("linear-scan-ra: checkpoint-entry-only use of a whole-range "
          "spilled interval reads the canonical slot",
          "[linear-scan-ra][g01]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  // Bank-exhaustion geometry (the exhaustion-spills fixture above): 13
  // simultaneously-live GP values; the eater instruction E references the
  // first twelve, so all twelve bank registers are held at E and the
  // thirteenth — furthest last, its only reference the checkpoint entry at
  // the guard — is the spill victim. The twelve dead dup outputs after E
  // drive the scan's expire so a register IS free at the guard: pre-layer-2
  // a spill reload IS emitted there (the recorded RED), not the D-23
  // slot-read fallback (that path — no free register — is pinned by the
  // unhandleable-shape fixture above and is NOT what this fixture pins).
  as->TestStart();
  constexpr int kValueCount = 13;  // > 12 free GP registers
  std::vector<HInstruction*> def(kValueCount);
  for (int i = 0; i < kValueCount; i++) {
    def[i] = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(i)));
  }
  auto* eater = fx.AppendN(HOpcode::kEnterInlineFrame, kValueCount - 1);
  for (int i = 0; i < kValueCount - 1; i++) {
    eater->OprSet(i, def[i]->output());
  }
  // dead last uses of the twelve register-resident values: each dup's own
  // dead output drives the scan's expire at the next def, so the bank is
  // free again by the time the guard executes
  for (int i = 0; i < kValueCount - 1; i++) {
    fx.Append1(HOpcode::kDupI, def[i]->output());
  }
  // The exit-bearing guard: its ordinary operand is an immediate, so the
  // spilled thirteenth value is referenced here ONLY by the checkpoint
  // entry — no ordinary operand reference at all.
  auto* guard = fx.Append1(HOpcode::kDeoptZ, as->NewImmediate(std::int32_t(0)));
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(def[0]));
  as->TestFinalize();

  AttachSnapshot(&fx, guard, def[kValueCount - 1]->output());

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* spilled_iv = fx.IntervalOf(fx.VregOf(def[kValueCount - 1]));
  REQUIRE(spilled_iv != nullptr);
  REQUIRE(spilled_iv->in_memory);
  REQUIRE(spilled_iv->assigned == nullptr);

  // (i) the checkpoint entry encodes the canonical spill slot as a
  // Deopt::kSpill MachineRef — NOT the reload register. Pre-layer-2 the
  // entry resolves to the reload register (recorded RED: this REQUIRE).
  REQUIRE(guard->HasDeoptId());
  const auto& deopt = fx.quake->deopt_list()[guard->deopt_id()];
  REQUIRE(deopt.top_frame().value_list.size() == 1);
  const auto& ref = deopt.top_frame().value_list[0].ref;
  REQUIRE(RefIsMemStackSlot(ref, spilled_iv->spill));

  // (ii) NO reload mov immediately precedes the guard for this vreg (the
  // only mov the plan could emit at this site is that reload).
  auto* prev = guard->prev();
  REQUIRE(prev != nullptr);
  REQUIRE(prev->opcode() != HOpcode::kMov);

  // (iii) the whole stream — slot reference included — validates clean.
  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Fixture D — layer-2 policy pin at the REAL corpus encoding (plan 02.1-01
// Task 3 deviation, Rule 1 — the crash_6.js #1138 shape): at corpus
// geometry LIRLower materializes a checkpoint's entries 1:1 into a
// kSnapshot instruction's ORDINARY operand list (lir-lower.cc
// TransSnapshot — "snapshot does nothing but just hold all references"),
// so the deopt-state references arrive as operand references, not exit
// entries. The layer-2 policy must recognize them as the same use kind:
// a whole-range spilled interval referenced at a kSnapshot ONLY by such
// operands reads its canonical slot — no reload mov (a reload here is the
// exact writer the validator's register-level fixpoint bridged across the
// rotated loop — 02.1-01-HARD-STOP.md #1138).
TEST_CASE("linear-scan-ra: kSnapshot operand of a whole-range spilled "
          "interval reads the canonical slot",
          "[linear-scan-ra][g01]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  // Same bank-exhaustion geometry as Fixture C: the thirteenth value is
  // the whole-range spill victim, the twelve dead dups free the bank
  // before the tail.
  as->TestStart();
  constexpr int kValueCount = 13;  // > 12 free GP registers
  std::vector<HInstruction*> def(kValueCount);
  for (int i = 0; i < kValueCount; i++) {
    def[i] = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(i)));
  }
  auto* eater = fx.AppendN(HOpcode::kEnterInlineFrame, kValueCount - 1);
  for (int i = 0; i < kValueCount - 1; i++) {
    eater->OprSet(i, def[i]->output());
  }
  for (int i = 0; i < kValueCount - 1; i++) {
    fx.Append1(HOpcode::kDupI, def[i]->output());
  }
  // The guard carries the deopt checkpoint (exit entries); the kSnapshot
  // holds the SAME value as an ordinary operand — the exact stream shape
  // LIRLower emits at corpus geometry.
  auto* guard = fx.Append1(HOpcode::kDeoptZ, as->NewImmediate(std::int32_t(0)));
  auto* snap = fx.AppendN(HOpcode::kSnapshot, 1);
  snap->OprSet(0, def[kValueCount - 1]->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(def[0]));
  as->TestFinalize();

  AttachSnapshot(&fx, guard, def[kValueCount - 1]->output());

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* spilled_iv = fx.IntervalOf(fx.VregOf(def[kValueCount - 1]));
  REQUIRE(spilled_iv != nullptr);
  REQUIRE(spilled_iv->in_memory);
  REQUIRE(spilled_iv->assigned == nullptr);

  // (i) the kSnapshot's materialized operand for the spilled vreg reads
  // the canonical spill slot directly — pre-generalization the reload mov
  // claims a register and the operand resolves to it (recorded RED).
  auto* materialized = snap->Opr(0);
  REQUIRE(materialized != nullptr);
  REQUIRE(materialized->IsMemStack());
  REQUIRE(materialized->GetMemStack().slot() == spilled_iv->spill);

  // (ii) NO reload mov immediately precedes the kSnapshot.
  auto* prev = snap->prev();
  REQUIRE(prev != nullptr);
  REQUIRE(prev->opcode() != HOpcode::kMov);

  // (iii) the guard's deopt entry encodes the same canonical slot.
  REQUIRE(guard->HasDeoptId());
  const auto& deopt = fx.quake->deopt_list()[guard->deopt_id()];
  REQUIRE(deopt.top_frame().value_list.size() == 1);
  REQUIRE(
      RefIsMemStackSlot(deopt.top_frame().value_list[0].ref, spilled_iv->spill));

  // (iv) the whole stream validates clean.
  REQUIRE(!fx.Validate());
}

// -----------------------------------------------------------------------------
// Fixture E — the spill-slot TENURE pin (plan 02.1-01 rev 6, Finding 2; the
// /tmp/021-subsite-verdict.md construct): the single-location materialization
// contract writes EVERY def site of an interval into its canonical spill
// slot, so a slot acquired at the eviction sweep point is written
// retroactively at the victim's DEF point — which may sit far before the
// sweep. When the acquired slot's previous holder is still live at the
// victim's def point, the victim's def-site store clobbers that live value
// (the crash_6.js v125/v304 slot-5 collision: the jscall output's store
// clobbered the still-live Math this-ref, and the call's kRcDec(this)
// decremented the call result instead — one stranded reference per pass).
//
// Geometry: V1 is evicted early into slot S and stays live until a late use;
// V2 is defined INSIDE V1's tenure, stays register-resident while V1's slot
// is released (V1 expires first), then is itself evicted AFTER the release —
// the sweep sees S as merely not-in-use and hands it over. The pinned RULE:
// an acquired slot's previous holder must not still be live at the
// acquirer's DEF point (free_at <= victim.def).
//
// Stream: [kStart][v1][a0..a10][x1][uses a0..a10, x1][v2][use v1(L1)]
//         [b0..b10][c][use c][uses b0..b10][use v2(U2)][back][kEnd]
// Scan walk: x1's def exhausts the bank with v1 the furthest-last active
// value -> v1 EVICTED into the first slot (S). The a/x uses then free the
// whole bank, v2 takes a register INSIDE v1's tenure (def(v2) < L1), v1's
// late use expires it (releasing S), the b fillers refill the bank, and c's
// def evicts v2 — the furthest-last active value — AFTER the release. Under
// the shipped rule v2's eviction acquires S; under the tenure rule it must
// not (S's holder is still live at v2's def), so v2 gets a FRESH slot.
TEST_CASE("linear-scan-ra: eviction acquires no spill slot inside a previous "
          "holder's tenure",
          "[linear-scan-ra]") {
  LinearScanFixture fx;
  auto* as = fx.as.get();

  constexpr int kFillerCount = 11;  // + v1 (or v2) = 12 holding the bank

  as->TestStart();
  auto* v1 = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  std::vector<HInstruction*> a(kFillerCount);
  for (int i = 0; i < kFillerCount; i++) {
    a[i] = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(10 + i)));
  }
  auto* x1 = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(20)));
  for (int i = 0; i < kFillerCount; i++) {
    fx.Append1(HOpcode::kDupI, a[i]->output());
  }
  fx.Append1(HOpcode::kDupI, x1->output());
  // v2's def sits between v1's def and v1's (late) last use — inside v1's
  // slot tenure — while the whole bank is free again.
  auto* v2 = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(30)));
  // v1's last use: L1, AFTER v2's def. The dup's dead output keeps the
  // stream's shape regular; v1 expires here, releasing its slot.
  fx.Append1(HOpcode::kDupI, v1->output());
  std::vector<HInstruction*> b(kFillerCount);
  for (int i = 0; i < kFillerCount; i++) {
    b[i] = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(40 + i)));
  }
  // c's def re-exhausts the bank; v2 (furthest-last) is the eviction victim.
  auto* c = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(50)));
  fx.Append1(HOpcode::kDupI, c->output());
  for (int i = 0; i < kFillerCount; i++) {
    fx.Append1(HOpcode::kDupI, b[i]->output());
  }
  // v2's last use: U2, the furthest last point in the whole stream — this is
  // what makes v2 the eviction victim at c's def rather than a spill-current.
  fx.Append1(HOpcode::kDupI, v2->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(v1));
  as->TestFinalize();

  fx.RunVregThenLiveness();

  auto err = fx.Allocate();
  REQUIRE(!err);

  const auto* v1_iv = fx.IntervalOf(fx.VregOf(v1));
  const auto* v2_iv = fx.IntervalOf(fx.VregOf(v2));
  REQUIRE(v1_iv != nullptr);
  REQUIRE(v2_iv != nullptr);

  // geometry: both intervals are eviction casualties living in spill slots,
  // and v2's def point sits INSIDE v1's tenure — the collision shape.
  REQUIRE(v1_iv->in_memory);
  REQUIRE(v2_iv->in_memory);
  REQUIRE(v1_iv->assigned == nullptr);
  REQUIRE(v2_iv->assigned == nullptr);
  REQUIRE(v1_iv->spill != MemoryStack::kInvalidSlot);
  REQUIRE(v2_iv->spill != MemoryStack::kInvalidSlot);
  REQUIRE(v1_iv->def.point() < v2_iv->def.point());
  REQUIRE(v2_iv->def.point() < v1_iv->last.point());

  // THE TENURE RULE: v2's canonical slot must differ from the slot whose
  // previous holder (v1) is still live at v2's def point — v2's def-site
  // store would otherwise clobber v1's live value (RED pre-fix: the
  // eviction sweep acquires the slot merely because it is not in use at the
  // sweep point, which sits after v1's expiry).
  REQUIRE(v2_iv->spill != v1_iv->spill);

  // the rule allocates a FRESH slot rather than reusing the colliding one:
  // exactly two slots in the FrameLayout spill region.
  REQUIRE(as->frame_layout()->spill_count() == 2);

  // the whole stream — distinct slots included — validates clean.
  REQUIRE(!fx.Validate());
}

}  // namespace
}  // namespace lir
}  // namespace quake
