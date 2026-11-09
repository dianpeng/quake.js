// Structural unit tests for the classical liveness pass (LIVE-01/LIVE-02,
// D-12/D-13/D-14).
//
// The pass ships INERT until the plan-06 cutover, so these tests drive it
// the only correct way available: hand-built HAssembler streams, RALinearize
// run first (it installs the liveness-id coordinates the pass reads), then
// LivenessAnalysis::Run, asserting EXACT hand-computed live-in/out set
// membership and per-vreg def/use point lists against explicit vreg-id
// lists written in the test (no golden text dumps).
//
// Fixture pipeline per test:
//   sculpt LIR -> VregPass (test 2) or hand-register a VregContext
//   -> RALinearize::Run -> LivenessAnalysis::Run -> assert.
//
// Fixtures never call Print() on hand-built instructions (nullptr ir::Node*
// is stored, only dereferenced by trace/print paths).
//
// Notes, every fixture stream ends with an unconditional kJmp whose label
// is bound: the CFG materializer keys blocks by leaders and requires every
// block to terminate at a jump/deopt boundary, exactly like real traces
// (their last real instruction is always the loop-back jump).

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <quake.h>

#include <frame-layout.h>

#include <lir/assembler.h>
#include <lir/cfg.h>
#include <lir/instruction.h>
#include <lir/liveness-analysis.h>
#include <lir/opcode.h>
#include <lir/operand.h>
#include <lir/ra-linearize.h>
#include <lir/vreg-pass.h>

#include <ir/checkpoint.h>
#include <ir/node.h>
#include <ir/ssa-var.h>

#include <unittest/ir/qjs-bytecode-test-util.h>

#include <memory>
#include <vector>

namespace quake {
namespace lir {
namespace {

// ----------------------------------------------------------------------------
// Fixture (D-13, same seam and JSContext source as vreg-pass-test): Quake
// needs a live JSContext (its ctor interns JSStrings). Zone-backed state
// (HAssembler instructions, VregContext, the pass's sets) lives on the
// Quake zone.

struct LivenessFixture {
  std::unique_ptr<ir::QJSFunctionBytecode> mock;
  std::unique_ptr<QuakeWorld> world;
  std::unique_ptr<Quake> quake;
  std::unique_ptr<HAssembler> as;

  LivenessFixture() {
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
  // def link (VregPass output or hand registration). The analysis object
  // (CFG member, non-copyable) lives on the fixture, which outlives the
  // assertions.
  LivenessAnalysis* Analyze(VregContext* ctx) {
    RALinearize linearize{quake.get(), as.get()};
    linearize.Run();

    analysis = std::make_unique<LivenessAnalysis>(quake.get(), as.get(), ctx);
    analysis->Run();
    return analysis.get();
  }

  std::unique_ptr<LivenessAnalysis> analysis;
};

// The block containing `target` (searched over the real CFG the analysis
// ran on).
const BasicBlock* BlockOf(const LivenessAnalysis* la,
                          const HInstruction* target) {
  const BasicBlock* found = nullptr;
  la->cfg()->ForeachBasicBlock([&](const BasicBlock* bb) -> bool {
    bb->ForeachInstruction([&](const HInstruction* ins) -> bool {
      if (ins == target) {
        found = bb;
        return false;
      }
      return true;
    });
    return found == nullptr;
  });
  CHECK(found);
  return found;
}

std::vector<std::uint32_t> ToIds(const zone::Vector<LivenessPoint>& points) {
  std::vector<std::uint32_t> ids;
  for (const auto& p : points) ids.push_back(p.point());
  return ids;
}

std::size_t CountPhi(HAssembler* as) {
  std::size_t count = 0;
  as->ForeachInstruction([&](HInstruction* ins) {
    if (ins->IsPhi()) count++;
  });
  return count;
}

// -----------------------------------------------------------------------------
// Shape 1: straight-line — single BB0 region (no BB1 marker, the trace's
// mandatory terminating jump only), def t0 then use, def t1 then use.
//
// Stream: [kStart][a=kMovI32 #][usea=kDupI(a)][b=kMovI32 #][useb=kDupI(b)]
//         [jmp->a][kEnd]
// Liveness ids (BB0 order, 2 per instruction): kStart=2, a=4, usea=6, b=8,
// useb=10, jmp=12, kEnd=14.
// Blocks: B1={kStart} -> B2={a,usea,b,useb,jmp} (self loop).
TEST_CASE("liveness: straight-line", "[liveness][live]") {
  LivenessFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* usea = fx.Append1(HOpcode::kDupI, a->output());
  auto* b = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(2)));
  auto* useb = fx.Append1(HOpcode::kDupI, b->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  VregContext ctx{fx.quake->zone()};
  auto va = ctx.GetOrNewVreg(a->output());
  auto vb = ctx.GetOrNewVreg(b->output());
  auto vusea = ctx.GetOrNewVreg(usea->output());
  auto vuseb = ctx.GetOrNewVreg(useb->output());

  auto* la = fx.Analyze(&ctx);

  REQUIRE(la->vreg_count() == 4);
  REQUIRE(va != vb);

  // entry block has empty live-in; last block has empty live-out
  auto* entry_bb = BlockOf(la, as->start_instruction()->next());
  auto* last_bb = BlockOf(la, useb);
  REQUIRE_FALSE(la->LiveIn(entry_bb, va));
  REQUIRE_FALSE(la->LiveIn(entry_bb, vb));
  REQUIRE_FALSE(la->LiveOut(last_bb, va));
  REQUIRE_FALSE(la->LiveOut(last_bb, vb));

  // exact def/use point lists, element by element (D-12), and def <= use
  REQUIRE(ToIds(la->DefPoints(va)) == std::vector<std::uint32_t>{4});
  REQUIRE(ToIds(la->UsePoints(va)) == std::vector<std::uint32_t>{6});
  REQUIRE(ToIds(la->DefPoints(vb)) == std::vector<std::uint32_t>{8});
  REQUIRE(ToIds(la->UsePoints(vb)) == std::vector<std::uint32_t>{10});

  // the users' own outputs have uses nowhere and defs at their ids
  REQUIRE(ToIds(la->DefPoints(vusea)) == std::vector<std::uint32_t>{6});
  REQUIRE(la->UsePoints(vusea).empty());
  REQUIRE(ToIds(la->DefPoints(vuseb)) == std::vector<std::uint32_t>{10});
  REQUIRE(la->UsePoints(vuseb).empty());
}

// -----------------------------------------------------------------------------
// Shape 2: loop-carried variable — the real post-vreg induction-variable
// shape, produced by running VregPass on a phi-ful stream (the plan-06
// pipeline order: VregPass -> RALinearize -> LivenessAnalysis).
//
// Pre-vreg: [kStart][entry=kMovI32 #0][kBB1][upd=kDupI(entry)]
//           [user(kPhiLoopI out)][back=kJmp bb1][kEnd][phi][sentry]
// Post-vreg stream + liveness ids (BB0 then BB1 order):
//   kStart=2, entry=4, entry_move=6, kBB1=8, upd=10, user=12,
//   backedge_move=14, back=16, kEnd=18
// vregs: 0 = the phi's induction vreg (entry_move + backedge_move defs),
//        1 = entry const, 2 = upd value, 3 = user value.
// Blocks: B_a={kStart, entry, entry_move} -> B_b={kBB1, upd, user,
//        backedge_move, back} -> B_b (self loop, the backedge).
TEST_CASE("liveness: loop-carried variable", "[liveness][live]") {
  LivenessFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* entry =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(0)));
  auto* bb1_label = as->TestBB1();
  // the honest induction chain: both body instructions use the PHI value
  // (wired below, once the phi exists) — i' = dup(i), use i — and the phi's
  // backedge input is i'
  auto* upd = fx.AppendN(HOpcode::kDupI, 1);    // operand = phi output
  auto* user = fx.AppendN(HOpcode::kDupI, 1);   // operand = phi output
  auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();

  auto* phi = fx.AppendN(HOpcode::kPhiLoopI, 2);
  phi->PopulateOutput(as);
  upd->OprSet(0, phi->output());  // i' = dup(i) — needs the phi output
  upd->PopulateOutput(as);        // phi's backedge operand needs THIS below
  phi->OprSet(0, entry->output());
  phi->OprSet(1, upd->output());
  user->OprSet(0, phi->output());
  user->PopulateOutput(as);
  as->TestFinalize();

  REQUIRE(CountPhi(as) == 1);

  VregPass vreg_pass{fx.quake.get(), as};
  VregContext* ctx = vreg_pass.Run();

  REQUIRE(CountPhi(as) == 0);
  REQUIRE(ctx->vreg_count() == 4);

  std::uint32_t induction = 0, entry_vreg = 0, upd_vreg = 0, user_vreg = 0;
  REQUIRE(ctx->Lookup(phi->output(), &induction));
  REQUIRE(ctx->Lookup(entry->output(), &entry_vreg));
  REQUIRE(ctx->Lookup(upd->output(), &upd_vreg));
  REQUIRE(ctx->Lookup(user->output(), &user_vreg));

  auto* la = fx.Analyze(ctx);

  auto* loop_head_bb = BlockOf(la, upd);   // the loop-head block (BB1)
  auto* backedge_bb = BlockOf(la, back);   // the block holding the backedge
  auto* pre_loop_bb = BlockOf(la, entry);  // the entry-edge block (BB0)
  REQUIRE(loop_head_bb == backedge_bb);    // single-block loop body

  // the induction vreg is live-in at the loop head AND live-out of the
  // backedge block (it must survive the wrap-around), and it crosses the
  // loop-head boundary from the entry block
  REQUIRE(la->LiveIn(loop_head_bb, induction));
  REQUIRE(la->LiveOut(backedge_bb, induction));
  REQUIRE(la->LiveOut(pre_loop_bb, induction));
  REQUIRE_FALSE(la->LiveIn(pre_loop_bb, induction));  // defined in BB0

  // def list has BOTH def points: entry-edge move (id 6) + backedge move
  // (id 14); use list = upd (10) + user (12) — element by element
  REQUIRE(ToIds(la->DefPoints(induction)) ==
          std::vector<std::uint32_t>{6, 14});
  REQUIRE(ToIds(la->UsePoints(induction)) ==
          std::vector<std::uint32_t>{10, 12});

  // the entry const and the upd value are block-local: def, single use,
  // never live across a block boundary
  REQUIRE(ToIds(la->DefPoints(entry_vreg)) == std::vector<std::uint32_t>{4});
  REQUIRE(ToIds(la->UsePoints(entry_vreg)) == std::vector<std::uint32_t>{6});
  REQUIRE(ToIds(la->DefPoints(upd_vreg)) == std::vector<std::uint32_t>{10});
  REQUIRE(ToIds(la->UsePoints(upd_vreg)) == std::vector<std::uint32_t>{14});
  REQUIRE_FALSE(la->LiveIn(loop_head_bb, entry_vreg));
  REQUIRE_FALSE(la->LiveIn(loop_head_bb, upd_vreg));
  REQUIRE(la->UsePoints(user_vreg).empty());
}

// -----------------------------------------------------------------------------
// Shape 3: side exit keeps values live — a conditional deopt whose
// checkpoint entry references a Temporary with NO ordinary operand use
// anywhere: the only thing keeping it alive is the conservative side-exit
// rule (checkpoint entries count as uses at the deopt instruction).
//
// Stream + ids (single BB0 region): kStart=2, guard=4, x=6, j1=8,
//   dz(kDeoptZ guard, exit->ck{x})=10, f_user(kDupI guard)=12, back=14,
//   kEnd=16.
// Blocks: B1={kStart, guard, x, j1} -> B2={dz} -> {deopt pseudo block},
//   B2 -> B3={f_user, back} (self loop).
TEST_CASE("liveness: side exit keeps values live", "[liveness][live]") {
  LivenessFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* guard =
      fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(1)));
  auto* x = fx.Append1(HOpcode::kMovI32, as->NewImmediate(std::int32_t(42)));
  auto* j1 = fx.AppendN(HOpcode::kJmp, 1);
  auto* dz = fx.AppendN(HOpcode::kDeoptZ, 1);
  dz->OprSet(0, guard->output());
  auto* f_user = fx.Append1(HOpcode::kDupI, guard->output());
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  j1->OprSet(0, as->NewLabel(dz));
  back->OprSet(0, as->NewLabel(f_user));
  as->TestFinalize();

  // Hand-built checkpoint carrying exactly one value entry whose SSAVar's
  // lir_operand is x's output Temporary. This is the minimal honest shape
  // of what LIRLower's EmitG* helpers install on guard deopts.
  auto* zone = fx.quake->zone();
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
  dz->set_exit(ck);
  REQUIRE(dz->HasExit());

  VregContext ctx{fx.quake->zone()};
  auto vg = ctx.GetOrNewVreg(guard->output());
  auto vx = ctx.GetOrNewVreg(x->output());
  [[maybe_unused]] auto vf = ctx.GetOrNewVreg(f_user->output());

  auto* la = fx.Analyze(&ctx);

  auto* def_bb = BlockOf(la, x);       // block defining x
  auto* deopt_bb = BlockOf(la, dz);    // the side-exit block
  auto* fall_bb = BlockOf(la, f_user); // the deopt's fallthrough block

  // THE discriminator: x has no ordinary operand use anywhere — it is live
  // across the side exit purely because the checkpoint entry counts as a
  // use at the deopt instruction.
  REQUIRE(la->LiveIn(deopt_bb, vx));
  REQUIRE(la->LiveOut(def_bb, vx));
  REQUIRE(ToIds(la->UsePoints(vx)) == std::vector<std::uint32_t>{10});
  REQUIRE(ToIds(la->DefPoints(vx)) == std::vector<std::uint32_t>{6});

  // and nothing past the side exit reads it: not live into the fallthrough,
  // not live-out of the deopt block's continuation path
  REQUIRE_FALSE(la->LiveIn(fall_bb, vx));
  REQUIRE_FALSE(la->LiveOut(deopt_bb, vx));

  // the guard value is used by BOTH the deopt's operand (10) and the
  // fallthrough's user (12), so it is live-out of the deopt block through
  // the fallthrough edge
  REQUIRE(ToIds(la->UsePoints(vg)) == std::vector<std::uint32_t>{10, 12});
  REQUIRE(ToIds(la->DefPoints(vg)) == std::vector<std::uint32_t>{4});
  REQUIRE(la->LiveIn(deopt_bb, vg));
  REQUIRE(la->LiveOut(deopt_bb, vg));
  REQUIRE_FALSE(la->LiveIn(def_bb, vg));  // defined there, before both uses
}

// -----------------------------------------------------------------------------
// Shape 4: value dead across the back-edge — a value defined inside the
// loop body whose last use is before the backedge, with nothing at or
// after the loop head using it. It must NOT be live-in at the loop head
// and NOT live-out of the backedge block, even though control wraps the
// backedge every iteration. This is the case the legacy phi
// force-extension got wrong by over-conservatism — the proof the dataflow
// is real; a numbering hack cannot produce it.
//
// Stream + ids: kStart=2, entry=4, entry_move=6, kBB1=8, upd=10,
//   dead=12, deaduse=14, backedge_move=16, back=18, kEnd=20.
// vregs: 0=entry const, 1=induction (entry_move+backedge_move defs),
//        2=upd, 3=DEAD (def 12, single use 14), 4=deaduse out.
TEST_CASE("liveness: value dead across the back-edge", "[liveness][live]") {
  LivenessFixture fx;
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
  auto* back = fx.Append1(HOpcode::kJmp, bb1_label);
  as->TestEnd();
  as->TestFinalize();

  // hand-registered context (the hand-emitted post-vreg shape: the
  // induction vreg has BOTH move defs aliased onto one id)
  VregContext ctx{fx.quake->zone()};
  auto ventry = ctx.GetOrNewVreg(entry->output());
  auto vinduction = ctx.GetOrNewVreg(entry_move->output());
  auto vupd = ctx.GetOrNewVreg(upd->output());
  auto vdead = ctx.GetOrNewVreg(dead->output());
  auto vdeaduse = ctx.GetOrNewVreg(deaduse->output());
  CHECK(ctx.MapAlias(backedge_move->output(), vinduction) == vinduction);
  REQUIRE(ctx.vreg_count() == 5);

  auto* la = fx.Analyze(&ctx);

  auto* loop_head_bb = BlockOf(la, upd);
  auto* backedge_bb = BlockOf(la, back);
  REQUIRE(loop_head_bb == backedge_bb);

  // THE discriminator: DEAD is not live at the loop head and does not
  // survive the backedge, although control wraps it every iteration
  REQUIRE_FALSE(la->LiveIn(loop_head_bb, vdead));
  REQUIRE_FALSE(la->LiveOut(backedge_bb, vdead));

  // its life is exactly [def 12, use 14], inside the body
  REQUIRE(ToIds(la->DefPoints(vdead)) == std::vector<std::uint32_t>{12});
  REQUIRE(ToIds(la->UsePoints(vdead)) == std::vector<std::uint32_t>{14});
  REQUIRE(la->UsePoints(vdeaduse).empty());

  // positive control in the SAME fixture: the loop-carried induction vreg
  // (two defs: entry-edge 6 + backedge 16; uses at 10 and 12) IS live at
  // the loop head and across the backedge — the contrast is the proof
  REQUIRE(la->LiveIn(loop_head_bb, vinduction));
  REQUIRE(la->LiveOut(backedge_bb, vinduction));
  REQUIRE(ToIds(la->DefPoints(vinduction)) ==
          std::vector<std::uint32_t>{6, 16});
  REQUIRE(ToIds(la->UsePoints(vinduction)) ==
          std::vector<std::uint32_t>{10, 12});

  // entry const and upd stay block-local
  REQUIRE(ToIds(la->DefPoints(ventry)) == std::vector<std::uint32_t>{4});
  REQUIRE(ToIds(la->UsePoints(ventry)) == std::vector<std::uint32_t>{6});
  REQUIRE(ToIds(la->DefPoints(vupd)) == std::vector<std::uint32_t>{10});
  REQUIRE(ToIds(la->UsePoints(vupd)) == std::vector<std::uint32_t>{16});
}

}  // namespace
}  // namespace lir
}  // namespace quake
