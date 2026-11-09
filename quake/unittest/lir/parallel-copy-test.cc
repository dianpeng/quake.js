// Unit tests for ParallelCopyResolver — textbook parallel-move sequentializer
// with scratch-based cycle breaking (D-28, D-30).
//
// Tests use hand-built abstract CopySlot pairs (register and spill kinds) and
// simulate the resolved sequential steps over a slot→value map.  The end-state
// is asserted equal to the requested mapping — no golden dumps, no reference
// to HAssembler or the LIR stream (pure algorithm test).

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <zone.h>

#include <lir/parallel-copy.h>
#include <lir/register.h>

#include <map>
#include <string>
#include <vector>

namespace quake {
namespace lir {
namespace {

// ----------------------------------------------------------------------------
// Test helpers — abstract value simulation
//
// We assign each slot a string "value" and simulate executing the resolved
// sequential steps.  After all steps, every destination must hold its
// requested source's original value.
// ----------------------------------------------------------------------------

using SlotValueMap = std::map<std::string, std::string>;

// Produce a stable string key for a CopySlot for use in the value map.
std::string SlotKey(const CopySlot& slot) {
  if (slot.IsRegister()) {
    return "reg:" + std::string(slot.reg->name());
  }
  return "spill:" + std::to_string(slot.spill_slot);
}

// Simulate executing a single ParallelCopyStep on the value map.
void SimulateStep(SlotValueMap& values, const ParallelCopyStep& step) {
  std::string src_key = SlotKey(step.src);
  std::string dst_key = SlotKey(step.dst);
  CATCH_REQUIRE(values.find(src_key) != values.end());
  values[dst_key] = values[src_key];
}

// Build initial value map: each slot gets a unique "initial value" string.
SlotValueMap BuildInitialValues(const zone::Vector<CopyPair>& pairs) {
  SlotValueMap values;
  for (std::size_t i = 0; i < pairs.size(); ++i) {
    const CopyPair& p = pairs[i];
    std::string src_key = SlotKey(p.src);
    std::string dst_key = SlotKey(p.dst);
    if (values.find(src_key) == values.end()) {
      values[src_key] = "val_src_" + std::to_string(i);
    }
    if (values.find(dst_key) == values.end()) {
      values[dst_key] = "val_dst_" + std::to_string(i);
    }
  }
  return values;
}

// Build expected final map: each destination must equal its source's original
// value (parallel semantics — all reads happen before all writes).
SlotValueMap BuildExpectedValues(const zone::Vector<CopyPair>& pairs,
                                 const SlotValueMap& initial) {
  SlotValueMap expected = initial;
  for (std::size_t i = 0; i < pairs.size(); ++i) {
    const CopyPair& p = pairs[i];
    if (p.IsIdentity()) continue;
    std::string src_key = SlotKey(p.src);
    std::string dst_key = SlotKey(p.dst);
    expected[dst_key] = initial.at(src_key);
  }
  return expected;
}

// Resolve and simulate — assert final state matches requested parallel mapping.
void VerifyResolve(ParallelCopyResolver& resolver,
                   const zone::Vector<CopyPair>& pairs) {
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);
  SlotValueMap values = BuildInitialValues(pairs);
  SlotValueMap expected = BuildExpectedValues(pairs, values);

  for (std::size_t i = 0; i < steps.size(); ++i) {
    SimulateStep(values, steps[i]);
  }

  // Check that every destination has the correct final value.
  for (std::size_t i = 0; i < pairs.size(); ++i) {
    const CopyPair& p = pairs[i];
    if (p.IsIdentity()) continue;
    std::string dst_key = SlotKey(p.dst);
    CATCH_REQUIRE(values[dst_key] == expected[dst_key]);
  }
}

// ----------------------------------------------------------------------------
// Test fixture — lightweight zone + register set
// ----------------------------------------------------------------------------

struct ResolverFixture {
  // Zone with 4KB reserve, 64KB max — more than enough for resolver tests.
  zone::Zone zone;
  const Register* rax;
  const Register* rbx;
  const Register* rcx;
  const Register* rdx;
  const Register* rsi;
  const Register* xmm0;
  const Register* xmm1;
  const Register* xmm2;
  CopySlot scratch_gp;
  CopySlot scratch_fp;

  ResolverFixture() : zone(4096, 65536) {
    rax = Register::GetRegisterFromKind(RegisterKind::kRax);
    rbx = Register::GetRegisterFromKind(RegisterKind::kRbx);
    rcx = Register::GetRegisterFromKind(RegisterKind::kRcx);
    rdx = Register::GetRegisterFromKind(RegisterKind::kRdx);
    rsi = Register::GetRegisterFromKind(RegisterKind::kRsi);
    xmm0 = Register::GetRegisterFromKind(RegisterKind::kXmm0);
    xmm1 = Register::GetRegisterFromKind(RegisterKind::kXmm1);
    xmm2 = Register::GetRegisterFromKind(RegisterKind::kXmm2);
    // Per D-30 the scratch slot is GUARANTEED to not participate in the pair
    // set (the RA reserves it for the resolver at that site).  Fixtures must
    // respect the same contract: rsi/xmm0 never appear in a pair below.
    scratch_gp = CopySlot::FromReg(rsi);
    scratch_fp = CopySlot::FromReg(xmm0);
  }

  CopySlot Reg(const Register* r) { return CopySlot::FromReg(r); }
  CopySlot Spill(MemoryStack::Slot s) { return CopySlot::FromSpill(s); }
};

// ----------------------------------------------------------------------------
// Test cases
// ----------------------------------------------------------------------------

TEST_CASE("ParallelCopyResolver: acyclic single move",
          "[parallel-copy][acyclic]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  CopyPair p;
  p.src = f.Reg(f.rbx);
  p.dst = f.Reg(f.rax);
  p.size_class = SizeClass::kQWord;
  pairs.push_back(p);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  // Exactly one move step.
  CATCH_REQUIRE(steps.size() == 1);
  CATCH_REQUIRE(steps[0].kind == ParallelCopyStep::kMove);

  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: acyclic ordered emission",
          "[parallel-copy][acyclic]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  // c→a, a→b: a is a pending dst so c→a can't emit until a→b has emitted.
  CopyPair p1;
  p1.src = f.Reg(f.rcx);
  p1.dst = f.Reg(f.rax);
  p1.size_class = SizeClass::kQWord;
  pairs.push_back(p1);

  CopyPair p2;
  p2.src = f.Reg(f.rax);
  p2.dst = f.Reg(f.rbx);
  p2.size_class = SizeClass::kQWord;
  pairs.push_back(p2);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  // No scratch needed for acyclic chain.
  bool has_scratch_save = false;
  for (std::size_t i = 0; i < steps.size(); ++i) {
    if (steps[i].kind == ParallelCopyStep::kScratchSave) has_scratch_save = true;
  }
  CATCH_REQUIRE_FALSE(has_scratch_save);

  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: swap (2-cycle) uses scratch",
          "[parallel-copy][cycle]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  // rax → rbx, rbx → rax: classic swap, must use scratch.
  CopyPair p1;
  p1.src = f.Reg(f.rax);
  p1.dst = f.Reg(f.rbx);
  p1.size_class = SizeClass::kQWord;
  pairs.push_back(p1);

  CopyPair p2;
  p2.src = f.Reg(f.rbx);
  p2.dst = f.Reg(f.rax);
  p2.size_class = SizeClass::kQWord;
  pairs.push_back(p2);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  // Must involve scratch — at least one scratch save.
  bool has_scratch_save = false;
  for (std::size_t i = 0; i < steps.size(); ++i) {
    if (steps[i].kind == ParallelCopyStep::kScratchSave) {
      has_scratch_save = true;
      break;
    }
  }
  CATCH_REQUIRE(has_scratch_save);

  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: 3-node cycle",
          "[parallel-copy][cycle]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  // a→b, b→c, c→a: 3-cycle, needs exactly one scratch break.
  CopyPair p1;
  p1.src = f.Reg(f.rax);
  p1.dst = f.Reg(f.rbx);
  p1.size_class = SizeClass::kQWord;
  pairs.push_back(p1);

  CopyPair p2;
  p2.src = f.Reg(f.rbx);
  p2.dst = f.Reg(f.rcx);
  p2.size_class = SizeClass::kQWord;
  pairs.push_back(p2);

  CopyPair p3;
  p3.src = f.Reg(f.rcx);
  p3.dst = f.Reg(f.rax);
  p3.size_class = SizeClass::kQWord;
  pairs.push_back(p3);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  // Exactly one scratch save per cycle.
  int scratch_save_count = 0;
  for (std::size_t i = 0; i < steps.size(); ++i) {
    if (steps[i].kind == ParallelCopyStep::kScratchSave) scratch_save_count++;
  }
  CATCH_REQUIRE(scratch_save_count == 1);

  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: spill-to-spill goes through scratch",
          "[parallel-copy][spill]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  // Spill slot 0 → spill slot 1: must go through scratch register.
  CopyPair p;
  p.src = f.Spill(0);
  p.dst = f.Spill(1);
  p.size_class = SizeClass::kQWord;
  pairs.push_back(p);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  // A single spill-to-spill pair produces 2 moves through scratch (no cycle).
  CATCH_REQUIRE(steps.size() == 2);
  // Step 0: spill(0) → scratch
  CATCH_REQUIRE(steps[0].src.IsSpill());
  CATCH_REQUIRE(steps[0].dst == f.scratch_gp);
  // Step 1: scratch → spill(1)
  CATCH_REQUIRE(steps[1].src == f.scratch_gp);
  CATCH_REQUIRE(steps[1].dst.IsSpill());
  CATCH_REQUIRE(steps[1].dst.spill_slot == 1);

  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: identity pairs are dropped",
          "[parallel-copy][identity]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  CopyPair p1;
  p1.src = f.Reg(f.rax);
  p1.dst = f.Reg(f.rax);  // identity
  p1.size_class = SizeClass::kQWord;
  pairs.push_back(p1);

  CopyPair p2;
  p2.src = f.Reg(f.rbx);
  p2.dst = f.Reg(f.rcx);
  p2.size_class = SizeClass::kQWord;
  pairs.push_back(p2);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  // Only one real move; identity pair is dropped.
  CATCH_REQUIRE(steps.size() == 1);

  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: mixed register/spill acyclic",
          "[parallel-copy][mixed]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  CopyPair p1;
  p1.src = f.Reg(f.rax);
  p1.dst = f.Spill(0);
  p1.size_class = SizeClass::kQWord;
  pairs.push_back(p1);

  CopyPair p2;
  p2.src = f.Reg(f.rbx);
  p2.dst = f.Reg(f.rcx);
  p2.size_class = SizeClass::kQWord;
  pairs.push_back(p2);

  CopyPair p3;
  p3.src = f.Spill(1);
  p3.dst = f.Reg(f.rdx);
  p3.size_class = SizeClass::kQWord;
  pairs.push_back(p3);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: cycle involving spill slot",
          "[parallel-copy][cycle][spill]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  // rax → spill(0), spill(0) → rbx, rbx → rax: 3-cycle with spill
  CopyPair p1;
  p1.src = f.Reg(f.rax);
  p1.dst = f.Spill(0);
  p1.size_class = SizeClass::kQWord;
  pairs.push_back(p1);

  CopyPair p2;
  p2.src = f.Spill(0);
  p2.dst = f.Reg(f.rbx);
  p2.size_class = SizeClass::kQWord;
  pairs.push_back(p2);

  CopyPair p3;
  p3.src = f.Reg(f.rbx);
  p3.dst = f.Reg(f.rax);
  p3.size_class = SizeClass::kQWord;
  pairs.push_back(p3);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  VerifyResolve(resolver, pairs);
}

TEST_CASE("ParallelCopyResolver: empty pair set",
          "[parallel-copy][edge]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  ParallelCopyResolver resolver(&f.zone, f.scratch_gp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  CATCH_REQUIRE(steps.size() == 0);
}

TEST_CASE("ParallelCopyResolver: FP register swap",
          "[parallel-copy][fp]") {
  ResolverFixture f;
  zone::Vector<CopyPair> pairs(&f.zone);

  // FP register swap uses FP scratch.
  CopyPair p1;
  p1.src = f.Reg(f.xmm1);
  p1.dst = f.Reg(f.xmm2);
  p1.size_class = SizeClass::kQWord;
  pairs.push_back(p1);

  CopyPair p2;
  p2.src = f.Reg(f.xmm2);
  p2.dst = f.Reg(f.xmm1);
  p2.size_class = SizeClass::kQWord;
  pairs.push_back(p2);

  ParallelCopyResolver resolver(&f.zone, f.scratch_fp);
  zone::Vector<ParallelCopyStep> steps = resolver.Resolve(pairs);

  bool has_scratch_save = false;
  for (std::size_t i = 0; i < steps.size(); ++i) {
    if (steps[i].kind == ParallelCopyStep::kScratchSave) {
      has_scratch_save = true;
      break;
    }
  }
  CATCH_REQUIRE(has_scratch_save);

  VerifyResolve(resolver, pairs);
}

}  // namespace
}  // namespace lir
}  // namespace quake
