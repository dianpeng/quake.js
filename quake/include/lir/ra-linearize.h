#ifndef _QUAKE_LIR_RA_LINEARIZE_H_
#define _QUAKE_LIR_RA_LINEARIZE_H_
#include "base.h"
#include "common.h"
#include "zone.h"

#include "lir/liveness.h"

// ----------------------------------------------------------------------------
//
// RA pass (1/2): Linearize (liveness-id numbering).
//
// Phase (0) Initialize: record the instruction size and reserve the liveness
// map. Phase (1) Linearize: walk the LIR in a pre-defined order (BB0 -> BB1)
// and assign each instruction a liveness id, 2 ids per instruction, forming
// the coordinate space every later pass speaks.
//
// This module owns ONLY the numbering and the liveness map. Liveness is the
// classical backwards dataflow pass (LivenessAnalysis, phase 2); allocation
// is the constraint-driven linear scan (LinearScanRA). The legacy shell
// LiveInterval / reserve machinery that used to live here is gone with the
// legacy scan/materialize substrate (plan 02-05, D-22).
//
// The pass produces:
//   - per-instruction liveness ids (the coordinate space every later pass
//     and the dataflow point lists speak)
//   - the liveness_map_ (liveness id -> instruction)
//
namespace quake {
class Quake;

namespace lir {

class HAssembler;
class HInstruction;
class HOperand;

LivenessPoint RefPoint(HInstruction* ins, HOperand* opr);

// ----------------------------------------------------------------------------
//
// RALinearize — liveness-id numbering + liveness map only.
//
class RALinearize {
 public:
  RALinearize(Quake*, HAssembler*);

  // Run the initialize + linearize (numbering) phases
  void Run();

  zone::Zone* zone() const;

  // Map a liveness id back to its corresponding instruction
  HInstruction* LivenessMap(std::uint32_t livenessid) const {
    if (livenessid >= static_cast<std::uint32_t>(liveness_map_.size()))
      return nullptr;
    return liveness_map_[livenessid];
  }
  HInstruction* LivenessMap(const LivenessPoint& p) const {
    return LivenessMap(p.point());
  }

 private:
  // --------------------------------------------------------------------------
  // Phase (0)
  void Initialize();

 private:
  // --------------------------------------------------------------------------
  // Phase (1)
  // Linearization phase: assign each instruction its liveness ids in
  // physical order (BB0 first, then BB1).
  void Linearize();
  void DoLinearize();

 private:
  void TraceLiveness();

 private:
  Quake* quake_;
  HAssembler* as_;

  std::size_t instruction_size_;
  std::uint32_t liveness_id_;  // maxium liveness id, set up after Linearize

  std::vector<HInstruction*> liveness_map_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_RA_LINEARIZE_H_
