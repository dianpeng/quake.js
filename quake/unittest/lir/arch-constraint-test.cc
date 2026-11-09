// D-26 permanent completeness presence test for the pure-data arch
// constraint surface (plan 02-03 task 1).
//
// Walks EVERY HOpcode value and asserts GetInstructionConstraint returns
// either a populated constraint or the explicit no-constraints marker —
// never a default/null record. This pins the exhaustive-switch posture
// (no silent gaps, same philosophy as the rule-table wildcard hazard in
// CONCERNS.md): even though -Werror's -Wswitch already makes an
// un-audited opcode fail the BUILD, this test proves the runtime contract
// independently and permanently.
//
// Also pins the call-ABI queries against the ABI helpers the legacy path
// uses (GetABIGPArg/GetABIFPArg/GetABICallerSaved*), and spot-checks the
// line-by-line translate audit encoding for one opcode per constraint
// family so a corrupted table entry cannot drift past the presence test.

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <quake.h>

#include <lir/arch-constraint.h>
#include <lir/opcode.h>
#include <lir/register.h>

#include <optional>
#include <vector>

namespace quake {
namespace lir {
namespace {

const Register* Reg(RegisterKind kind) {
  return Register::GetRegisterFromKind(kind);
}

// Every populated constraint must carry at least one piece of real data —
// a presence test that only checks "not the marker" would let a table row
// with all-empty fields pass silently.
bool IsPopulated(const arch::InstructionConstraint& c) {
  return !c.output.IsNone() || !c.operands.empty() || c.is_call ||
         !c.clobber_gp.Empty() || !c.clobber_fp.Empty() ||
         !c.scratch_gp.Empty() || !c.scratch_fp.Empty();
}

TEST_CASE("arch-constraint: every opcode has an explicit entry (D-26)",
          "[arch-constraint]") {
  for (std::uint32_t i = 0;
       i < static_cast<std::uint32_t>(HOpcode::kSizeOfHOpcode); ++i) {
    const auto op = static_cast<HOpcode>(i);
    const auto& c = arch::GetInstructionConstraint(op);
    INFO("opcode index " << i);
    // Either the explicit marker or a populated constraint — never a
    // silent default/null record.
    REQUIRE((c.is_no_constraints_marker || IsPopulated(c)));
  }
}

TEST_CASE("arch-constraint: kParallelCopy is the explicit marker exemplar",
          "[arch-constraint]") {
  const auto& c = arch::GetInstructionConstraint(HOpcode::kParallelCopy);
  REQUIRE(c.is_no_constraints_marker);
  REQUIRE_FALSE(IsPopulated(c));
}

TEST_CASE("arch-constraint: call ABI queries match the ABI helpers",
          "[arch-constraint]") {
  // GP argument registers, indices 0..5 (GetABIGPArg's full range).
  for (int i = 0; i < 6; ++i) {
    const auto* r = arch::GetCallArgRegister(static_cast<std::size_t>(i),
                                             /*fp=*/false);
    std::optional<RegisterKind> want = cg::x64::GetABIGPArg(i);
    REQUIRE(r != nullptr);
    REQUIRE(want.has_value());
    REQUIRE(r == Reg(*want));
  }
  // FP argument registers, indices 0..7 (GetABIFPArg's full range).
  for (int i = 0; i < 8; ++i) {
    const auto* r = arch::GetCallArgRegister(static_cast<std::size_t>(i),
                                             /*fp=*/true);
    std::optional<RegisterKind> want = cg::x64::GetABIFPArg(i);
    REQUIRE(r != nullptr);
    REQUIRE(want.has_value());
    REQUIRE(r == Reg(*want));
  }
  // Beyond the ABI register range: no register (must be stack-passed).
  REQUIRE(arch::GetCallArgRegister(6, /*fp=*/false) == nullptr);
  REQUIRE(arch::GetCallArgRegister(8, /*fp=*/true) == nullptr);

  // Caller-saved clobber sets equal the ABI helper sets exactly.
  auto caller_saved_of = [](const std::vector<RegisterKind>& kinds) {
    RegisterSet set;
    for (auto k : kinds) set.Set(Reg(k));
    return set;
  };
  auto want_gp = caller_saved_of(cg::x64::GetABICallerSavedGP());
  auto want_fp = caller_saved_of(cg::x64::GetABICallerSavedFP());
  const auto have_gp = arch::GetCallClobberGP();
  const auto have_fp = arch::GetCallClobberFP();
  REQUIRE(have_gp.Count() == want_gp.Count());
  REQUIRE(have_fp.Count() == want_fp.Count());
  want_gp.ForeachSet([&](std::uint32_t kind_id) {
    REQUIRE(have_gp.Get(kind_id));
  });
  want_fp.ForeachSet([&](std::uint32_t kind_id) {
    REQUIRE(have_fp.Get(kind_id));
  });
}

// -----------------------------------------------------------------------------
// Audit spot-checks: one opcode per constraint family, pinning the exact
// encoding the D-26 line-by-line translate audit produced. A table row
// corrupted (or "simplified") later fails here even though the presence
// test above would still pass.

TEST_CASE("arch-constraint: audit encoding spot-checks", "[arch-constraint]") {
  // Two-operand rule: declarative kSameAsOperand(0), not an action.
  {
    const auto& c = arch::GetInstructionConstraint(HOpcode::kIAdd);
    REQUIRE_FALSE(c.is_no_constraints_marker);
    REQUIRE(c.output.kind == arch::ArchOperandKind::kSameAsOperand);
    REQUIRE(c.output.same_as_operand == 0);
    REQUIRE(c.operands.empty());
    REQUIRE_FALSE(c.is_call);
  }

  // Register-count shift: rcx staged as scratch, kSar also two-operand.
  {
    const auto& sar = arch::GetInstructionConstraint(HOpcode::kSar);
    REQUIRE(sar.output.kind == arch::ArchOperandKind::kSameAsOperand);
    REQUIRE(sar.scratch_gp.Get(Reg(RegisterKind::kRcx)));
    REQUIRE(sar.scratch_fp.Empty());

    const auto& gshl = arch::GetInstructionConstraint(HOpcode::kGShl);
    REQUIRE(gshl.scratch_gp.Get(Reg(RegisterKind::kRcx)));
    REQUIRE(gshl.output.IsNone());

    // Const-count variants never touch rcx.
    const auto& sar_const =
        arch::GetInstructionConstraint(HOpcode::kSarConst);
    REQUIRE(sar_const.output.kind == arch::ArchOperandKind::kSameAsOperand);
    REQUIRE(sar_const.scratch_gp.Empty());
  }

  // Comparisons: operand 0 must be a register; output independent.
  {
    const auto& c = arch::GetInstructionConstraint(HOpcode::kILt);
    REQUIRE(c.operands.size() == 1);
    REQUIRE(c.operands[0].kind == arch::ArchOperandKind::kAnyRegister);
    REQUIRE(c.output.IsNone());

    const auto& fc = arch::GetInstructionConstraint(HOpcode::kFEq);
    REQUIRE(fc.operands.size() == 1);
    REQUIRE(fc.operands[0].kind == arch::ArchOperandKind::kAnyRegister);
  }

  // idiv: dividend staged in rax, rdx zeroed + remainder output.
  {
    const auto& c = arch::GetInstructionConstraint(HOpcode::kIMod);
    REQUIRE(c.output.kind == arch::ArchOperandKind::kFixedRegister);
    REQUIRE(c.output.fixed_register == Reg(RegisterKind::kRdx));
    REQUIRE(c.scratch_gp.Get(Reg(RegisterKind::kRax)));
    REQUIRE(c.scratch_gp.Get(Reg(RegisterKind::kRdx)));
  }

  // Calls: is_call with full caller-saved clobbers; kRCallF64 returns in
  // xmm0.
  {
    const auto& call = arch::GetInstructionConstraint(HOpcode::kJSCallF);
    REQUIRE(call.is_call);
    REQUIRE(call.clobber_gp.Count() ==
            arch::GetCallClobberGP().Count());
    REQUIRE(call.clobber_fp.Count() ==
            arch::GetCallClobberFP().Count());

    const auto& f64 = arch::GetInstructionConstraint(HOpcode::kRCallF64);
    REQUIRE(f64.is_call);
    REQUIRE(f64.output.kind == arch::ArchOperandKind::kFixedRegister);
    REQUIRE(f64.output.fixed_register == Reg(RegisterKind::kXmm0));

    const auto& enter =
        arch::GetInstructionConstraint(HOpcode::kEnterJitTrace);
    REQUIRE(enter.is_call);
  }

  // Stub-clobbering internal calls (not ABI calls).
  {
    const auto& cast_v =
        arch::GetInstructionConstraint(HOpcode::kCastVToB);
    REQUIRE_FALSE(cast_v.is_call);
    REQUIRE(cast_v.clobber_gp.Count() == 1);
    REQUIRE(cast_v.clobber_gp.Get(Reg(RegisterKind::kRdx)));

    const auto& array =
        arch::GetInstructionConstraint(HOpcode::kLoadArrayInt32);
    REQUIRE(array.clobber_gp.Get(Reg(RegisterKind::kRdx)));

    const auto& cast_f32 =
        arch::GetInstructionConstraint(HOpcode::kCastF64ToB32);
    REQUIRE(cast_f32.clobber_gp.Get(Reg(RegisterKind::kRcx)));
    REQUIRE_FALSE(cast_f32.clobber_gp.Get(Reg(RegisterKind::kRdx)));
  }

  // Guarded arithmetic: free output placement, no fixed registers (the
  // JIT-reserved rax scratch does not leak into the table).
  {
    const auto& c = arch::GetInstructionConstraint(HOpcode::kGIAdd);
    REQUIRE(c.is_no_constraints_marker);
  }
}

}  // namespace
}  // namespace lir
}  // namespace quake
