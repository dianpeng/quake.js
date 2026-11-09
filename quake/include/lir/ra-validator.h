#ifndef _QUAKE_LIR_RA_VALIDATOR_H_
#define _QUAKE_LIR_RA_VALIDATOR_H_
#include "base.h"
#include "base/error.h"

#include "lir/cfg.h"
#include "lir/register.h"

#include <map>

namespace quake {

class Quake;

namespace lir {

class HAssembler;
class HInstruction;

// ----------------------------------------------------------------------------
// RAValidator — independent post-RA constraint and boundary verification
// (D-27, plan 02-03).
//
// Walks the post-RA LIR stream and proves, against the SAME pure-data
// lir::arch constraint tables the RA consumes (plan 02-03 task 1):
//
//  1. Operand/output locations honor their declared constraints:
//     kFixedRegister sits in exactly that register, kAnyRegister sits in
//     some register, kSameAsOperand pairs share a register. Register-only
//     checks: when a checked location is a non-register (spill/immediate)
//     the translate-time materialization move discharges the relation, so
//     the validator does not flag legacy-tolerated shapes.
//  2. Clobber/scratch discipline: the live physical-register set at each
//     instruction (reconstructed with a backwards dataflow over the CFG,
//     modeled over RegisterSet exactly like classical liveness but on
//     materialized locations) is disjoint from the instruction's declared
//     clobber/scratch sets — except the instruction's own operand
//     registers (inputs are staged/consumed by the instruction) and its
//     own output register.
//  3. Parallel-copy discipline (D-28/D-30/D-31): no duplicate
//     destinations; the pair list, interpreted as a sequential move
//     program, reaches the requested end state (order-correctness); a
//     kParallelCopy never carries an exit; and an exit-bearing
//     instruction immediately preceded by a parallel copy has all its
//     snapshot MachineRef locations at post-copy (destination or
//     untouched) slots — never at a pre-copy source slot.
//
// The reconstructed live-across set per instruction is exposed through
// LiveRegistersAt (test-visible query) so the backwards computation is
// checked directly against hand-computed fixtures, not only indirectly
// through clobber violations.
//
// Notes, the validator ships INERT until the plan 02-05 cutover: nothing
// in DoCompile runs it. It runs in Catch2 tests today and will ride the
// differential-gate path after the cutover (every corpus compile
// self-checks, D-27). Run() returns base::Error (first violation) so
// normal traces never hit a reachable CHECK (T-02-03-04); RunChecked()
// is the CHECK-failing variant reserved for the gated compile path.
//
// Allocation: the CFG lives on the quake zone like every other pass; the
// per-instruction live sets live in a plain std::map (validator lifetime
// only, dies with the validator — the zone-clear discipline does not
// apply to a verification pass that outlives no compile).
class RAValidator {
 public:
  RAValidator(Quake*, HAssembler*);

  // Run all checks. Returns the first violation as a base::Error, or an
  // empty Error when the stream satisfies every constraint.
  base::Error Run();

  // CHECK-failing variant for the gated compile path (T-02-03-04: the
  // CHECK must never be reachable on normal traces — Run is the
  // fault-tolerant entry).
  void RunChecked();

 public:
  // The reconstructed live-across physical-register set at `ins`
  // (registers whose values are needed before or at this instruction).
  // Only meaningful after a successful Run() on the same stream.
  const RegisterSet& LiveRegistersAt(const HInstruction*) const;

 private:
  // Per-instruction visitors. Each returns the first violation found.
  base::Error CheckOperandConstraints(const HInstruction*);
  base::Error CheckClobbers(const HInstruction*, const RegisterSet& across,
                            const RegisterSet& uses);
  base::Error CheckParallelCopy(HInstruction*);
  base::Error CheckExitBoundary(HInstruction*);

  // Def/use collection over materialized locations. kParallelCopy is
  // special-cased: even indices (sources) are uses, odd indices
  // (destinations) are defs — the destinations overwrite their slots.
  // Deopt side-exit checkpoint entries count as uses at the instruction
  // (the conservative side-exit treatment, same policy as
  // LivenessAnalysis).
  void CollectUsesDefs(const HInstruction*, RegisterSet* uses,
                       RegisterSet* defs);

  zone::Zone* zone() const;

  Quake* quake_;
  HAssembler* as_;
  CFG cfg_;

  // live-across set per instruction, filled during Run().
  std::map<const HInstruction*, RegisterSet> live_at_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_RA_VALIDATOR_H_
