#ifndef _QUAKE_LIR_RA_LINEAR_SCAN_H_
#define _QUAKE_LIR_RA_LINEAR_SCAN_H_
#include "base.h"
#include "base/error.h"

#include "lir/liveness.h"
#include "lir/memory.h"
#include "lir/parallel-copy.h"
#include "lir/register.h"

#include "zone/zone-container.h"

namespace quake {

class Quake;

namespace lir {

class HAssembler;
class HInstruction;
class HOperand;
class VregContext;
class LivenessAnalysis;

// ----------------------------------------------------------------------------
// LinearScanRA — the constraint-driven linear-scan register allocator
//
// One single-range interval per vreg, built from the classical liveness
// def/use point lists: the interval starts at the vreg's FIRST def
// point and ends at the LAST of its use points and def points — the
// synthetic redefinition points (the backedge move of a loop-carried value)
// are def points too, so a multi-def vreg keeps ONE interval spanning its
// whole loop-carried extent instead of the legacy phi force-extension.
//
// The scan assigns each interval a physical register of its bank (GP/FP
// free lists from the arch provider), expiring intervals whose last point
// precedes the current def; on bank exhaustion the furthest-last interval
// is spilled to a FrameLayout spill slot with a recycling free list. A
// whole-range spilled interval lives in its slot: its defining
// instructions' outputs materialize to the slot directly (translate
// tolerates memory outputs — the legacy-tolerated shape the 02-03 audit
// documents), and every use point is preceded by a reload mov (slot ->
// register), with the operand at that instruction overridden to the reload
// register.
//
// Shapes the allocator cannot honor (register pressure with no free
// register for a reload, call arguments beyond the register ABI, ...) make
// Run() return a base::Error naming the reason — never a reachable
// CHECK and never silent degradation.
//
// The class was shipped INERT and became the single live allocation path
//
// Allocation: intervals and spill bookkeeping live on the quake zone;
// nothing escapes the compile.
// ----------------------------------------------------------------------------
struct RAInterval {
  std::uint32_t vreg;
  LivenessPoint def;   // first def point (liveness-id coordinates)
  LivenessPoint last;  // last live point (uses AND synthetic redefs)
  RegisterKind kind;   // the interval's register bank
  // The size-class view every materialized register operand of this
  // interval carries — the legacy RegisterAllocation::ToOperand contract
  // (ra-linear-scan.cc): translate rules mix instruction-derived widths
  // (eg u::ScrD() for guarded int arithmetic, EDX() for kIMod's readback)
  // with u::Gp(operand) views, so all sites of one interval must render
  // ONE view of the physical register. Resolved from the first linked
  // def's output size class; width-less moves inherit the moved value's
  // class down the def chain.
  SizeClass size_class = SizeClass::kQWord;
  const Register* assigned = nullptr;  // physical register when !in_memory
  MemoryStack::Slot spill = MemoryStack::kInvalidSlot;
  bool in_memory = false;
};

class LinearScanRA {
 public:
  LinearScanRA(Quake*, HAssembler*, const VregContext*,
               const LivenessAnalysis*);

  // Scan + satisfy + materialize. Returns an empty Error on success; the
  // first unhandleable shape as a base::Error otherwise.
  base::Error Run();

 public:
  // Test-visible interval access (nullptr for vreg holes — ids absorbed by
  // coalescing are never compacted).
  const RAInterval* Interval(std::uint32_t vreg) const;

 private:
  zone::Zone* zone() const;

  // ---- Phase 1: interval construction ------------------------------------
  // One RAInterval per vreg id that has any def/use activity. Extents are
  // the union of the liveness point lists and the def records' own
  // liveness ids (coalescing-merged vregs derive the union of member def
  // points through VregContext::GetDefs).
  base::Error BuildIntervals();

  // ---- Phase 1 sub-step: CFG-accurate extents The point-list extent is linear
  // [first def, last def/use]; the CFG-accurate liveness solution the RAValidator
  // reconstructs can keep a value live around a whole block (a loop-carried value
  // reaching a loop-header use around the backedge) on a path the linear range
  // considers dead. For every block the dataflow marks the vreg live in or
  // live out of, extend the interval to at least that block's numbered
  // instruction span. Dataflow-driven ONLY: no def-count tests, no
  // block-leader-position tests (reverted mitigation #3 stays reverted).
  void ExtendIntervalToCfgLiveness(std::uint32_t vreg, RAInterval* iv);

  // The one shared inclusive coverage predicate (G-01 layer 1): is the
  // interval live AT `point` in the (CFG-extended) interval model? Both
  // PlanConstraints filters consume this single notion, so the planned
  // park set is exactly the CFG-live set
  // Inclusive on the last point, exclusive on the def point.
  bool IntervalLiveAt(const RAInterval&, LivenessPoint) const;

  // Boundary refinement for the clobber-park filter: the interval's last
  // point IS `point` and that point is a genuine USE of the value — the
  // instruction consumes it (ordinary operand or checkpoint entry) before
  // any post-instruction clobber matters, so no park is needed.
  bool DiesAtUse(const RAInterval&, LivenessPoint) const;

  // The interval's bank, derived from any of its defining instructions'
  // output register type. Error when no def exists to derive it from.
  base::Error ResolveIntervalKind(std::uint32_t vreg, RAInterval*);

  // The interval's materialized register view (see RAInterval::size_class):
  // the physical register rendered in the interval's size class. Falls
  // back to the canonical register when no such view exists.
  static const Register* RegView(const Register*, SizeClass);
  const Register* ViewOf(const RAInterval&) const;

  // Resolve the interval's size class down its def chain (see
  // RAInterval::size_class). `state` memoizes per-vreg progress
  // (0 unvisited / 1 in progress / 2 done).
  SizeClass ResolveIntervalSizeClass(std::uint32_t vreg,
                                     std::vector<std::uint8_t>* state);

  // ---- Phase 2: the scan --------------------------------------------------
  // Sorted-by-def scan with an active list ordered by last point; expires
  // dead intervals (freeing registers and recycling spill slots), allocates
  // a bank register, and on exhaustion spills the furthest-last interval.
  base::Error Scan();

  // ---- Phase 3: PHI edge-move conversion (PCOPY-02, Step A) ---------------
  // Every provenance-tracked edge move surviving coalescing is replaced by
  // ONE kParallelCopy per shuffle anchor holding all the group's (src,dst)
  // pairs — the sequential kMov PHI-shuffle path is gone from the stream.
  base::Error ConvertEdgeMoves();

  // ---- Phase 4: instruction-constraint satisfaction --------
  // Walk the numbered instructions; per instruction plan (no insertion):
  // clobber/scratch eviction parks, operand/output constraint parallel
  // copies, call-argument landing. Emission happens in EmitPlans.
  base::Error PlanConstraints();

  // ---- Phase 5: spill fixups ----------------------------------------------
  // Whole-range spilled intervals: insert a reload mov (slot -> register)
  // before every use point whose operand is not otherwise satisfied (the
  // constraint plan's pairs discharge constrained operands directly), and
  // record the operand override the reload implies.
  base::Error InsertSpillReloads();

  // ---- Phase 6: emission --------------------------------------------------
  void EmitPlans();

  // ---- Phase 7: materialization -------------------------------------------
  // Rewrite every Temporary operand/output to its materialized location at
  // that instruction (interval location unless a fixup moved it), and
  // construct the deopt snapshots at instruction boundaries — MachineRefs
  // at post-parallel-copy locations when a copy directly precedes the
  // exit-bearing instruction.
  base::Error Materialize();
  base::Error MaterializeExit(HInstruction*);
  HOperand* MakeOperand(const CopySlot&, std::uint32_t vreg, HInstruction*);

  // Sequentialize a pair set per bank through the ParallelCopyResolver:
  // the result is the resolver's step order — itself a correct sequential program —
  // with a guaranteed per-site scratch (a free bank register, else a spill temp).
  std::vector<CopyPair> SequentializePairs(LivenessPoint,
                                           const std::vector<CopyPair>&,
                                           const std::vector<bool>& is_fp,
                                           const RegisterSet& extra_exclude);

  // The value's materialized location: its interval location, unless an
  // fixup (reload/park/constraint copy) moved it at this instruction.
  CopySlot LocationAt(std::uint32_t vreg, HInstruction* ins) const;
  CopySlot LocationOf(std::uint32_t vreg) const;

  // The interval's first still-linked defining instruction (operand
  // instruction links for materialized operands; nullptr for def-less
  // shapes). LinkFor falls back to the anchored instruction — vregs whose
  // defs were all converted into parallel copies have no linked def, and
  // the link only satisfies the htype()/print contract.
  HInstruction* FirstLinkedDef(std::uint32_t vreg) const;
  HInstruction* LinkFor(std::uint32_t vreg, HInstruction* fallback) const;

  // Choose a register of the bank free at `point` (not owned by any
  // register-resident interval covering the point, and not claimed by
  // `exclude`). nullptr when the bank is exhausted there.
  const Register* FindFreeRegisterAt(RegisterKind kind, LivenessPoint point,
                                     const RegisterSet& exclude) const;

  // A spill slot from the recycling free list; grows the FrameLayout spill
  // region when the free list is empty. Acquire honors the slot TENURE rule:
  // a slot is acquirable only when the previous holder's last point (`free_at`,
  // recorded by ReleaseSpillSlot) is at or before the acquiring interval's DEF
  // point — the single-location materialization contract writes every def site
  // of the acquiring interval into its canonical slot, so a def-site store must
  // never land inside a previous holder's tenure.
  MemoryStack::Slot AcquireSpillSlot(std::uint32_t free_from);
  void ReleaseSpillSlot(MemoryStack::Slot, std::uint32_t free_at);

  // A transient park slot per (bank, ordinal): parks are strictly bracketed
  // around one instruction, so ordinal k's slot is reusable at every
  // instruction that parks k values of the bank.
  MemoryStack::Slot ParkSlot(bool fp, std::size_t ordinal);

  bool IsLinked(HInstruction*) const;

  // Unlink an edge move converted into a parallel copy (its output's
  // redirected uses resolve at materialization — not HInstruction::Remove).
  void UnlinkMove(HInstruction*);

  // ---- Constraint-plan record (one per numbered instruction) --------------
  struct InsPlan {
    HInstruction* ins = nullptr;
    std::uint32_t point = 0;

    // Clobber/scratch eviction parks and two-operand anchor parks: store
    // (reg -> slot) before the instruction; reload (slot -> reg) after when
    // still live. `reload == false` marks a value whose last point IS this
    // instruction (dies here: the register is parked but never reloaded).
    // `override_operand == false` marks an anchor park: the operand at this
    // instruction still reads the register (the instruction consumes it);
    // the park only preserves the value FOR the instruction's aftermath.
    struct Park {
      std::uint32_t vreg;
      MemoryStack::Slot slot;
      bool reload;
      bool override_operand = true;
    };
    std::vector<Park> parks;


    // Constraint satisfaction pairs (sequentialized per bank through the
    // ParallelCopyResolver before emission — the pair list in the stream is
    // itself a correct sequential program, the shape the RAValidator's
    // order simulation proves).
    std::vector<CopyPair> pairs;

    // Post-instruction fixups: output relocation movs (required output
    // register -> canonical location) and park reloads, as materialized
    // slot pairs emitted in plan order after the instruction.
    struct PostMov {
      CopySlot src;
      CopySlot dst;
    };
    std::vector<PostMov> post_movs;
  };
  std::vector<InsPlan> plans_;

  // Registers claimed at an instruction by the constraint plan (copy
  // destinations, any-register/scratch picks) — spill reloads must avoid
  // them.
  std::map<HInstruction*, RegisterSet> claimed_;

  // The constraint-killed register sets per instruction (clobber ∪ scratch,
  // caller-saved included for calls by the table itself).
  std::map<HInstruction*, RegisterSet> kills_;

  // The per-(bank, ordinal) park slot pool.
  std::vector<MemoryStack::Slot> park_slot_gp_;
  std::vector<MemoryStack::Slot> park_slot_fp_;

 private:
  Quake* quake_;
  HAssembler* as_;
  const VregContext* vregs_;
  const LivenessAnalysis* liveness_;

  // Per-vreg interval storage, indexed by vreg id (holes permitted — an
  // empty slot has assigned == nullptr && spill == kInvalidSlot).
  zone::Vector<RAInterval> intervals_;

  // Spill bookkeeping, legacy-shaped: a slot is recyclable once its owner's
  // interval expired (the scan releases it at expire time). `free_at` keeps
  // the releasing holder's last point — the earliest point at which the slot
  // may be handed to a new interval whose def sites write into it.
  struct SpillRecord {
    MemoryStack::Slot slot;
    bool in_use = false;
    std::uint32_t owner = 0;  // vreg id of the interval holding the slot
    std::uint32_t free_at = 0;  // last point of the previous holder
  };
  std::vector<SpillRecord> spill_records_;

  // The fixup-imposed location of a value AT a specific instruction: the
  // reload register a spill reload produced, the park slot an eviction
  // parked the value into, or the required register a constraint copy
  // landed it in. Authoritative for operand rewrites AND deopt checkpoint
  // entries at that instruction
  std::map<std::pair<HInstruction*, std::uint32_t>, CopySlot> location_fix_;

  // Liveness id -> instruction map over the whole stream (RALinearize
  // installed ids on every instruction; the def/use POINTS the analysis
  // publishes are in these coordinates).
  std::map<std::uint32_t, HInstruction*> ins_at_;

  // Registers currently held by register-resident live intervals, per bank.
  RegisterSet inuse_gp_;
  RegisterSet inuse_fp_;

  std::vector<const Register*> free_gp_;
  std::vector<const Register*> free_fp_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_RA_LINEAR_SCAN_H_
