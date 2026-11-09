// Structural unit tests for the post-RA constraint/boundary validator
// (RAValidator, D-27, plan 02-03).
//
// The validator ships INERT until the plan 02-05 cutover — nothing in
// DoCompile runs it — so these tests drive it the only correct way
// available: hand-built POST-RA HAssembler streams (materialized register /
// spill operands everywhere), Run() invoked directly, asserting clean runs
// on correct streams and base::Error violations on deliberately corrupted
// ones. No golden dumps; every assertion is structural.
//
// Fixture streams follow the liveness-test contract: TestStart/TestEnd
// frame the stream and every block terminates at a jump/deopt boundary
// with its label bound. The live physical-register reconstruction is
// checked directly through RAValidator::LiveRegistersAt (test-visible
// query), not only through clobber violations.
//
// Register conventions in fixtures: rbx/rdi/rsi/r8 are "value" registers
// (never reserved); rax = JIT scratch, r15 = context, xmm15 = FP scratch
// (never RA-allocated); rcx/rdx appear as declared scratch/clobbers of the
// audited opcodes.

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <quake.h>

#include <lir/arch-constraint.h>
#include <lir/assembler.h>
#include <lir/instruction.h>
#include <lir/opcode.h>
#include <lir/operand.h>
#include <lir/parallel-copy.h>
#include <lir/ra-validator.h>
#include <lir/register.h>

#include <ir/checkpoint.h>
#include <ir/node.h>
#include <ir/ssa-var.h>

#include <unittest/ir/qjs-bytecode-test-util.h>

#include <memory>
#include <string>
#include <vector>

namespace quake {
namespace lir {
namespace {

const Register* Reg(RegisterKind kind) {
  return Register::GetRegisterFromKind(kind);
}

// ----------------------------------------------------------------------------
// Fixture — same seam as liveness-test (Quake needs a live JSContext; zone
// backs the assembler stream).

struct ValidatorFixture {
  std::unique_ptr<ir::QJSFunctionBytecode> mock;
  std::unique_ptr<QuakeWorld> world;
  std::unique_ptr<Quake> quake;
  std::unique_ptr<HAssembler> as;

  ValidatorFixture() {
    mock = std::make_unique<ir::QJSFunctionBytecode>();
    mock->SetUp();

    world = std::make_unique<QuakeWorld>(nullptr);
    quake = std::make_unique<Quake>(world.get(), mock->js_context());
    as = std::make_unique<HAssembler>(quake.get());
  }

  HInstruction* Append(HOpcode opcode) {
    return as->NewInstructionBefore(nullptr, opcode, as->end_instruction());
  }

  HInstruction* AppendN(HOpcode opcode, std::size_t operand_size) {
    return as->NewInstructionBefore(nullptr, opcode, operand_size,
                                    as->end_instruction());
  }

  // A post-RA value instruction: materialized register operands in, a
  // materialized register output out. (PopulateOutput creates the output
  // Temporary; SetRAOutput swaps in the physical location, exactly what
  // LinearScanRA's materialization does for real streams.)
  HInstruction* Def(HOpcode opcode, const Register* out,
                    std::vector<HOperand*> operands) {
    auto* ins = AppendN(opcode, operands.size());
    for (std::size_t i = 0; i < operands.size(); ++i) {
      ins->OprSet(static_cast<int>(i), operands[i]);
    }
    ins->PopulateOutput(as.get());
    ins->SetRAOutput(as->NewRegister(out));
    return ins;
  }

  HOperand* R(const Register* r) { return as->NewRegister(r); }
  HOperand* S(MemoryStack::Slot slot) { return as->NewMemStack(slot); }

  base::Error Validate() {
    RAValidator validator{quake.get(), as.get()};
    return validator.Run();
  }

  // The reconstructed live-across physical-register set at `ins`.
  RegisterSet LiveAt(const HInstruction* ins) {
    RAValidator validator{quake.get(), as.get()};
    REQUIRE_FALSE(validator.Run());
    return validator.LiveRegistersAt(ins);
  }
};

// Hand-built checkpoint with a single value entry whose SSAVar carries
// `opr` as its lir_operand — the minimal honest shape of what LIRLower's
// EmitG* helpers install on guard deopts (same construction as the
// liveness-test side-exit fixture).
ir::Checkpoint* MakeCheckpoint(ValidatorFixture* fx, HOperand* opr) {
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
  ssa->set_lir_operand(opr);
  return ck;
}

// -----------------------------------------------------------------------------
// Positive: a correct post-RA stream validates clean, and the reconstructed
// live-across register set is exactly the hand-computed one.
//
// Stream (single block + backedge):
//   [kStart] a=movi32#1->rbx  u1=dupI(a)->rdi  cast=cast_v_to_b(rdi)->rsi
//            u2=dupI(a)->r8  back=jmp a  [kEnd]
// a's value (rbx) is live across `cast` (u2 reads it after); rdi/rsi are
// the cast's own input/output; `a` redefines rbx every iteration, so rbx
// is dead at the backedge jump.
TEST_CASE("validator: clean stream passes", "[ra-validator]") {
  ValidatorFixture fx;
  auto* as = fx.as.get();

  as->TestStart();
  auto* a = fx.Def(HOpcode::kMovI32, Reg(RegisterKind::kRbx),
                   {as->NewImmediate(std::int32_t(1))});
  auto* u1 = fx.Def(HOpcode::kDupI, Reg(RegisterKind::kRdi), {a->output()});
  auto* cast = fx.Def(HOpcode::kCastVToB, Reg(RegisterKind::kRsi),
                      {fx.R(Reg(RegisterKind::kRdi))});
  auto* u2 = fx.Def(HOpcode::kDupI, Reg(RegisterKind::kR8), {a->output()});
  (void)u1;
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  REQUIRE_FALSE(fx.Validate());

  // Hand-computed live-across sets (live-in semantics: the instruction's
  // own inputs are live before it): at `cast` rbx is live across (u2 reads
  // it after the cast) and rdi is the cast's own live input; rsi is the
  // cast's fresh output — nobody reads it, so it is not live at the cast.
  // rdi dies at the cast (its last reader), so it is NOT live at u2.
  auto live_at_cast = fx.LiveAt(cast);
  REQUIRE(live_at_cast.Get(Reg(RegisterKind::kRbx)));
  REQUIRE(live_at_cast.Get(Reg(RegisterKind::kRdi)));
  REQUIRE_FALSE(live_at_cast.Get(Reg(RegisterKind::kRsi)));

  auto live_at_u2 = fx.LiveAt(u2);
  REQUIRE(live_at_u2.Get(Reg(RegisterKind::kRbx)));
  REQUIRE_FALSE(live_at_u2.Get(Reg(RegisterKind::kRdi)));

  // `a` redefines rbx at the top of every loop iteration (it is a constant
  // def dominating both uses), so nothing is live around the backedge:
  // rbx is dead at the jump.
  auto live_at_back = fx.LiveAt(back);
  REQUIRE_FALSE(live_at_back.Get(Reg(RegisterKind::kRbx)));
}

// -----------------------------------------------------------------------------
// Fixed output register: kRCallF64 must produce its result in xmm0.

TEST_CASE("validator: fixed output register", "[ra-validator]") {
  auto run_with_output = [](const Register* out_reg) -> base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    as->TestStart();
    auto* call = fx.Def(HOpcode::kRCallF64, out_reg,
                        {as->NewMemStatic(static_cast<const void*>(
                            reinterpret_cast<void*>(0x1)))});
    auto* back = fx.AppendN(HOpcode::kJmp, 1);
    as->TestEnd();
    back->OprSet(0, as->NewLabel(call));
    as->TestFinalize();
    return fx.Validate();
  };

  REQUIRE_FALSE(run_with_output(Reg(RegisterKind::kXmm0)));

  auto err = run_with_output(Reg(RegisterKind::kXmm1));
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("xmm0") != std::string::npos);
}

// -----------------------------------------------------------------------------
// Two-operand rule: kIAdd's output must share operand 0's register.

TEST_CASE("validator: same-as-operand pair", "[ra-validator]") {
  auto run_with = [](const Register* op0, const Register* out) -> base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    as->TestStart();
    auto* add = fx.Def(HOpcode::kIAdd, out,
                       {fx.R(op0), fx.R(Reg(RegisterKind::kRsi))});
    auto* back = fx.AppendN(HOpcode::kJmp, 1);
    as->TestEnd();
    back->OprSet(0, as->NewLabel(add));
    as->TestFinalize();
    return fx.Validate();
  };

  REQUIRE_FALSE(run_with(Reg(RegisterKind::kRax), Reg(RegisterKind::kRax)));

  auto err = run_with(Reg(RegisterKind::kRax), Reg(RegisterKind::kRbx));
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("same register") != std::string::npos);
}

// -----------------------------------------------------------------------------
// Any-register operand: comparisons require operand 0 in a register.

TEST_CASE("validator: any-register operand", "[ra-validator]") {
  // op0_in_spill=false: operand 0 in a register (clean);
  // true: operand 0 in a spill slot (kAnyRegister violation).
  auto run_with = [](bool op0_in_spill) -> base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    HOperand* op0 = op0_in_spill ? fx.S(0)
                                 : fx.R(Reg(RegisterKind::kRax));
    as->TestStart();
    auto* cmp = fx.Def(HOpcode::kILt, Reg(RegisterKind::kRbx),
                       {op0, as->NewImmediate(std::int32_t(7))});
    auto* back = fx.AppendN(HOpcode::kJmp, 1);
    as->TestEnd();
    back->OprSet(0, as->NewLabel(cmp));
    as->TestFinalize();
    return fx.Validate();
  };

  REQUIRE_FALSE(run_with(false));

  auto err = run_with(true);
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("register") != std::string::npos);
}

// -----------------------------------------------------------------------------
// Clobber discipline: a value live across kCastVToB must not sit in rdx
// (the stub's clobber); the same value in rbx is fine. Also pins the
// reconstructed live set directly.

TEST_CASE("validator: live value inside clobber set", "[ra-validator]") {
  auto run_with = [](RegisterKind live_kind) -> base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    const Register* live = Reg(live_kind);
    as->TestStart();
    auto* a = fx.Def(HOpcode::kMovI32, live,
                     {as->NewImmediate(std::int32_t(1))});
    auto* u1 = fx.Def(HOpcode::kDupI, Reg(RegisterKind::kRdi), {a->output()});
    auto* cast =
        fx.Def(HOpcode::kCastVToB, Reg(RegisterKind::kRsi),
               {fx.R(Reg(RegisterKind::kRdi))});
    auto* u2 = fx.Def(HOpcode::kDupI, Reg(RegisterKind::kR8), {a->output()});
    auto* back = fx.AppendN(HOpcode::kJmp, 1);
    as->TestEnd();
    back->OprSet(0, as->NewLabel(a));
    as->TestFinalize();
    (void)u1;
    (void)cast;
    (void)u2;
    return fx.Validate();
  };

  // rbx holds the live-across value -> clean, and the reconstructed set
  // at the cast carries rbx.
  REQUIRE_FALSE(run_with(RegisterKind::kRbx));

  // rdx holds the live-across value -> violation naming the register.
  auto err = run_with(RegisterKind::kRdx);
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("rdx") != std::string::npos);
}

// -----------------------------------------------------------------------------
// idiv: kIMod's output must be rdx.

TEST_CASE("validator: idiv output fixed to rdx", "[ra-validator]") {
  auto run_with = [](const Register* out) -> base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    as->TestStart();
    auto* mod = fx.Def(HOpcode::kIMod, out,
                       {fx.R(Reg(RegisterKind::kRax)),
                        fx.R(Reg(RegisterKind::kRsi))});
    auto* back = fx.AppendN(HOpcode::kJmp, 1);
    as->TestEnd();
    back->OprSet(0, as->NewLabel(mod));
    as->TestFinalize();
    return fx.Validate();
  };

  REQUIRE_FALSE(run_with(Reg(RegisterKind::kRdx)));

  auto err = run_with(Reg(RegisterKind::kRax));
  REQUIRE(err);
}

// -----------------------------------------------------------------------------
// Parallel copy discipline (D-28/D-30/D-31 structural checks):
//  * ordered chain validates clean
//  * reversed (unorderable) chain is a violation
//  * duplicate destination is a violation
//  * a kParallelCopy carrying an exit is a violation (D-31)
//  * an exit-bearing instruction preceded by a copy whose checkpoint entry
//    still references a pre-copy source location is a violation (D-31)

static HInstruction* AppendCopyN(ValidatorFixture* fx,
                                 std::vector<const Register*> slots) {
  auto* pc = fx->AppendN(HOpcode::kParallelCopy, slots.size());
  for (std::size_t i = 0; i < slots.size(); ++i) {
    pc->OprSet(static_cast<int>(i), fx->R(slots[i]));
  }
  return pc;
}

TEST_CASE("validator: parallel copy order", "[ra-validator]") {
  auto run_chain = [](std::vector<const Register*> slots) -> base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    const Register* rbx = Reg(RegisterKind::kRbx);
    const Register* rsi = Reg(RegisterKind::kRsi);
    as->TestStart();
    auto* c = fx.Def(HOpcode::kMovI32, rsi,
                     {as->NewImmediate(std::int32_t(3))});
    auto* pc = AppendCopyN(&fx, slots);
    auto* use_b =
        fx.Def(HOpcode::kDupI, Reg(RegisterKind::kR8), {fx.R(rbx)});
    auto* back = fx.AppendN(HOpcode::kJmp, 1);
    (void)use_b;
    (void)pc;
    as->TestEnd();
    back->OprSet(0, as->NewLabel(c));
    as->TestFinalize();
    return fx.Validate();
  };

  const Register* rbx = Reg(RegisterKind::kRbx);
  const Register* rdi = Reg(RegisterKind::kRdi);
  const Register* rsi = Reg(RegisterKind::kRsi);

  // Ordered chain a->b, c->a: sequential execution leaves b holding a's
  // original value and a holding c's.
  REQUIRE_FALSE(run_chain({rbx, rdi, rsi, rbx}));

  // Reversed listing c->a, a->b: the first move clobbers a before the
  // second move reads it.
  auto err = run_chain({rsi, rbx, rbx, rdi});
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("parallel copy") != std::string::npos);
}

TEST_CASE("validator: parallel copy duplicate destination", "[ra-validator]") {
  ValidatorFixture fx;
  auto* as = fx.as.get();
  const Register* rbx = Reg(RegisterKind::kRbx);
  const Register* rdi = Reg(RegisterKind::kRdi);
  const Register* rsi = Reg(RegisterKind::kRsi);

  as->TestStart();
  auto* a = fx.Def(HOpcode::kMovI32, rbx,
                   {as->NewImmediate(std::int32_t(1))});
  // Hand-built (bypasses the factory's duplicate-dst CHECK): two pairs
  // both targeting rbx.
  auto* pc = AppendCopyN(&fx, {rdi, rbx, rsi, rbx});
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();
  (void)pc;

  auto err = fx.Validate();
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("duplicate") != std::string::npos);
}

TEST_CASE("validator: parallel copy must not carry an exit", "[ra-validator]") {
  ValidatorFixture fx;
  auto* as = fx.as.get();
  const Register* rbx = Reg(RegisterKind::kRbx);
  const Register* rdi = Reg(RegisterKind::kRdi);

  as->TestStart();
  auto* a = fx.Def(HOpcode::kMovI32, rbx,
                   {as->NewImmediate(std::int32_t(1))});
  auto* pc = AppendCopyN(&fx, {rdi, rbx});
  auto* back = fx.AppendN(HOpcode::kJmp, 1);
  as->TestEnd();
  back->OprSet(0, as->NewLabel(a));
  as->TestFinalize();

  // D-31: a parallel copy sequence never spans a snapshot/deopt point.
  pc->set_exit(MakeCheckpoint(&fx, a->output()));

  auto err = fx.Validate();
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("exit") != std::string::npos);
}

TEST_CASE("validator: snapshot boundary after copy", "[ra-validator]") {
  // entry_in_post_copy_location: the checkpoint entry references the
  // pair's DESTINATION (post-copy location, clean) vs its SOURCE (stale
  // pre-copy location, D-31 violation).
  auto run_with_entry = [](bool entry_in_post_copy_location) -> base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    const Register* rbx = Reg(RegisterKind::kRbx);
    const Register* rdi = Reg(RegisterKind::kRdi);

    as->TestStart();
    auto* a = fx.Def(HOpcode::kMovI32, rbx,
                     {as->NewImmediate(std::int32_t(1))});
    auto* j1 = fx.AppendN(HOpcode::kJmp, 1);
    auto* pc = AppendCopyN(&fx, {rdi, rbx});  // rdi -> rbx
    auto* dz = fx.AppendN(HOpcode::kDeoptZ, 1);
    dz->OprSet(0, fx.R(rbx));
    auto* fall = fx.AppendN(HOpcode::kJmp, 1);
    as->TestEnd();
    j1->OprSet(0, as->NewLabel(pc));
    fall->OprSet(0, as->NewLabel(a));
    as->TestFinalize();

    HOperand* entry_opr =
        entry_in_post_copy_location ? fx.R(rbx) : fx.R(rdi);
    dz->set_exit(MakeCheckpoint(&fx, entry_opr));
    return fx.Validate();
  };

  REQUIRE_FALSE(run_with_entry(true));

  auto err = run_with_entry(false);
  REQUIRE(err);
  REQUIRE(err.GetMessage().find("snapshot") != std::string::npos);
}

// -----------------------------------------------------------------------------
// Dedicated fixture for the backwards-walk live physical-register
// reconstruction (D-27 must-have): hand-computed block live states checked
// DIRECTLY through LiveRegistersAt, in both directions, around kIMod — the
// multi-scratch instruction (rax dividend staging + rdx zeroing) with its
// output pinned to rdx.
//
// Stream (single block + backedge):
//   [kStart] a=movi32#1->LIVE1  b=movi32#2->LIVE2  c=movi32#3->r9
//            c_use=dupI(c)->r10
//            imod=imod(rsi, rdi)->rdx
//            use_a=dupI(a)->rsi   use_b=dupI(b)->rdi
//            back=jmp a  [kEnd]
// LIVE1/LIVE2 are live across `imod` (read after it); c (r9) is dead
// before `imod` (its only use precedes it). The backedge redefines
// everything at the top, so nothing loops around.
//
// Positive: LIVE1/LIVE2 in rbx/r8 -> Run() clean and both appear in the
// reconstructed set at `imod` (outside its clobber/scratch sets).
// Negative: LIVE2 in rax (a declared scratch of imod) -> Run() names rax.
TEST_CASE("validator: live-register reconstruction fixture", "[ra-validator]") {
  auto run_with = [](RegisterKind live2_kind, RegisterSet* live_at_mod) ->
      base::Error {
    ValidatorFixture fx;
    auto* as = fx.as.get();
    as->TestStart();
    auto* a = fx.Def(HOpcode::kMovI32, Reg(RegisterKind::kRbx),
                     {as->NewImmediate(std::int32_t(1))});
    auto* b = fx.Def(HOpcode::kMovI32, Reg(live2_kind),
                     {as->NewImmediate(std::int32_t(2))});
    auto* c = fx.Def(HOpcode::kMovI32, Reg(RegisterKind::kR9),
                     {as->NewImmediate(std::int32_t(3))});
    auto* c_use =
        fx.Def(HOpcode::kDupI, Reg(RegisterKind::kR10), {c->output()});
    auto* mod =
        fx.Def(HOpcode::kIMod, Reg(RegisterKind::kRdx),
               {fx.R(Reg(RegisterKind::kRsi)), fx.R(Reg(RegisterKind::kRdi))});
    auto* use_a =
        fx.Def(HOpcode::kDupI, Reg(RegisterKind::kRsi), {a->output()});
    auto* use_b =
        fx.Def(HOpcode::kDupI, Reg(RegisterKind::kRdi), {b->output()});
    auto* back = fx.AppendN(HOpcode::kJmp, 1);
    as->TestEnd();
    back->OprSet(0, as->NewLabel(a));
    as->TestFinalize();
    (void)c_use;
    (void)use_a;
    (void)use_b;

    RAValidator validator{fx.quake.get(), as};
    auto err = validator.Run();
    if (live_at_mod != nullptr) {
      *live_at_mod = validator.LiveRegistersAt(mod);
    }
    return err;
  };

  // Positive: known-live values in registers OUTSIDE the scratch/clobber
  // sets; reconstruction matches the hand-computed state exactly.
  RegisterSet live_at_mod;
  auto err_pos = run_with(RegisterKind::kR8, &live_at_mod);
  REQUIRE_FALSE(err_pos);
  REQUIRE(live_at_mod.Get(Reg(RegisterKind::kRbx)));  // LIVE1 known live
  REQUIRE(live_at_mod.Get(Reg(RegisterKind::kR8)));   // LIVE2 known live
  REQUIRE_FALSE(live_at_mod.Get(Reg(RegisterKind::kR9)));  // c known dead
  // imod's own input registers are live-in (excluded from the clobber
  // accountability), its own output (rdx) is exempt as well.
  REQUIRE(live_at_mod.Get(Reg(RegisterKind::kRsi)));
  REQUIRE(live_at_mod.Get(Reg(RegisterKind::kRdi)));

  // Negative: the same known-live value placed in rax — a declared
  // scratch register of imod (dividend staging) — is a violation naming
  // the register, and the reconstruction still reports it live.
  RegisterSet live_at_mod_neg;
  auto err_neg = run_with(RegisterKind::kRax, &live_at_mod_neg);
  REQUIRE(err_neg);
  REQUIRE(err_neg.GetMessage().find("rax") != std::string::npos);
  REQUIRE(live_at_mod_neg.Get(Reg(RegisterKind::kRax)));
  REQUIRE(live_at_mod_neg.Get(Reg(RegisterKind::kRbx)));
}

}  // namespace
}  // namespace lir
}  // namespace quake
