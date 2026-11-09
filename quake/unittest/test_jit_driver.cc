/**
 * Differential corpus driver (TEST-02, D-01/D-02/D-03).
 *
 * Every corpus file under unittest/jit/case/ runs twice in fresh QuickJS
 * contexts — first with the JIT hard-disabled (interpreter oracle), then
 * JIT-enabled. The two observation sequences (yes/no/dump arguments in
 * call order + the program completion value) must be identical, and every
 * yes()/no() assertion must pass in the JIT run. Any divergence fails this
 * TEST_CASE.
 *
 * ---------------------------------------------------------------------------
 * Design contract (locked in 01-CONTEXT.md):
 *
 *   D-01 two-run oracle    — interpreter run = oracle (JitDisableGuard:
 *                            g_quake_jit_kill_switch + threshold max, both
 *                            RAII-restored); jit run keeps yes()/no()
 *                            throwing, so any kExecError in it is a gate
 *                            failure. Automates verify/doc.txt's method.
 *   D-02 granularity       — the result channel is the observation
 *                            sequence; heap values compare via their
 *                            stringified form (pointer identity across two
 *                            fresh contexts would false-fail).
 *   D-03 corpus scope      — positive/ gen/ bailout/ memory/ must be green
 *                            (219 files); crash/ files are expected-failure
 *                            manifest debt. An unexpected manifest PASS is
 *                            a loud REQUIRE failure demanding a manifest
 *                            update, never silent success.
 *
 * The compared observation sequence per file is:
 *
 *   [yes/no/dump argument JSValues in call order] + [completion value]
 *
 * Zero corpus edits. now() is served from a deterministic run-scoped fake
 * clock (see jit-diff-util.h) so elapsed-time dumps stop being noise.
 *
 * Manifest files run inside fork()ed children with waitpid collection —
 * a genuine segfault in jitted code cannot take the corpus run down. Child
 * exit-code contract:
 *
 *   exit 0      differential PASSED  — unexpected: parent REQUIRE-fails
 *                                      ("manifest needs update")
 *   exit 3      differential failed  — expected (mismatch/exec error)
 *   signal      child died           — expected (crashed)
 *
 * The latter two are tracked debt, never gate-red.
 *
 * The per-file differential lives in unittest/jit-diff-util.h and is
 * shared with the permanent canary meta-test (unittest/lir/
 * diff-gate-canary-test.cc), which proves this gate actually fails on a
 * real miscompile (D-04).
 */

#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <flags.h>
#include <unittest/jit-diff-util.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <array>
#include <filesystem>
#include <sstream>
#include <string>

namespace quake {

// ---------------------------------------------------------------------------
// Expected-failure manifest (D-03). Explicit entries only, no globs — note
// crash_11.j's missing 's' would be missed by any *.js glob (Pitfall 8).
//
// The manifest was pre-seeded with the 11 crash/ reproducers
// (crash/crash_1.js .. crash/crash_10.js, crash/crash_11.j). On the first
// differential run (2026-11-06, glibc allocator, -O0 utest build) ALL 11
// passed the differential via the forked-child contract below — the loud
// "manifest needs update" path fired for every entry and they were removed.
// Any entry listed here is tracked debt: it runs fork()ed, a contained
// crash or differential failure counts as expected, and an unexpected pass
// fails the TEST_CASE demanding another manifest update.
// ---------------------------------------------------------------------------
constexpr std::array<const char*, 0> kExpectedFailManifest = {};

static bool IsManifestFile(const std::string& rel_path) {
  for (auto* entry : kExpectedFailManifest) {
    if (rel_path == entry) return true;
  }
  return false;
}

// Runs one manifest file inside a fork()ed child so a genuine segfault in
// the JIT cannot take the whole corpus run down with it (T-01-01).
//
// Child exit-code contract:
//   0            — differential PASSED (unexpected; parent fails loudly)
//   3            — differential failed (mismatch/exec error) as expected
//   signal death — crashed as expected
static bool RunManifestFileForked(const std::string& path) {
  auto pid = fork();
  REQUIRE(pid >= 0);

  if (pid == 0) {
    // Notes, _exit only — the child must not run Catch2/atexit machinery.
    auto verdict = test::RunFileDifferential(path.c_str());
    _exit(verdict.pass ? 0 : 3);
  }

  int status = 0;
  while (waitpid(pid, &status, 0) < 0) {
    REQUIRE(errno == EINTR);
  }

  if (WIFEXITED(status)) {
    return WEXITSTATUS(status) == 0;  // true == passed unexpectedly
  }
  return false;  // killed by signal: crashed as expected
}

TEST_CASE("case test", "[jit-case]") {
  auto case_dir = x::TestPathAt("jit/case/");

  LOG(TRACE) << "***************************************************";
  LOG(TRACE) << "Case Dir: " << case_dir;
  LOG(TRACE) << "***************************************************";

  auto total = 0;
  auto fail = 0;
  auto expected_fail = 0;
  std::stringstream report;

  for (auto& dir_entry :
       std::filesystem::recursive_directory_iterator(case_dir)) {
    if (!dir_entry.is_regular_file()) continue;
    const auto& p = dir_entry.path();

    LOG(TRACE) << "[==CASE==]: " << p.c_str() << "\n";

    auto rel = std::filesystem::relative(p, case_dir).generic_string();
    ++total;

    if (IsManifestFile(rel)) {
      if (RunManifestFileForked(p.string())) {
        ++fail;
        report << "[MANIFEST NEEDS UPDATE]: " << rel
               << " — differential passed unexpectedly; remove it from "
                  "kExpectedFailManifest\n";
      } else {
        ++expected_fail;
        LOG(TRACE) << "[EXPECTED FAIL]: " << p.c_str() << "\n";
      }
      continue;
    }

    auto verdict = test::RunFileDifferential(p.c_str());
    if (!verdict.pass) {
      ++fail;
      report << "[DIFF FAIL]: " << verdict.detail << "\n";
    } else {
      LOG(TRACE) << "[PASS]: " << p.c_str() << "\n";
    }
  }

  LOG(TRACE) << "total(" << total << "); fail(" << fail << "); "
             << "expected-fail(" << expected_fail << ")";

  // TEST-02 enforcement: the TEST_CASE fails on any non-manifest
  // differential failure (or any manifest file passing unexpectedly).
  INFO(report.str());
  REQUIRE(fail == 0);
}

}  // namespace quake
