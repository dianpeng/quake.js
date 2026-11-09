// D-04 permanent canary meta-test: proves the differential gate is
// fallible, ie it actually FAILS on a real miscompile. This file is never
// deleted and never skipped — gate trustworthiness is a permanent property
// of this suite.
//
// Mechanism: quake::test::g_inject_miscompile (ENABLE_TEST-only, defined
// in src/flags.cc) arms a +1 corruption of every integer-add result in
// Lower_kAddInt (src/lir/lir-lower.cc). Arming it around the differential
// run affects only the jit run — the oracle run never compiles anything
// (JitDisableGuard keeps it interpreter-only), so a corruption of the
// lowering cannot touch it.

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <flags.h>
#include <unittest/jit-diff-util.h>

#include <string>

namespace quake {

TEST_CASE("canary: gate passes when disarmed", "[diff-canary]") {
  auto case_path = x::TestPathAt("lir/canary-case.js");

  // The injection must never leak armed between tests.
  REQUIRE(quake::test::g_inject_miscompile == false);

  auto verdict = test::RunFileDifferential(case_path.c_str());
  INFO(verdict.detail);
  REQUIRE(verdict.pass);
}

TEST_CASE("canary: gate fails on injected miscompile", "[diff-canary]") {
  auto case_path = x::TestPathAt("lir/canary-case.js");

  REQUIRE(quake::test::g_inject_miscompile == false);
  quake::test::g_inject_miscompile = true;
  DEFER([]() { quake::test::g_inject_miscompile = false; });

  auto verdict = test::RunFileDifferential(case_path.c_str());

  REQUIRE_FALSE(verdict.pass);
  // The failure must be identified as a semantic divergence, not a
  // harness error.
  REQUIRE(verdict.detail.find("mismatch") != std::string::npos);
  INFO(verdict.detail);
}

}  // namespace quake
