#include "lir/ra-linearize.h"

#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"

#include "quake.h"

namespace quake {
namespace lir {

LivenessPoint RefPoint(HInstruction* ins, HOperand* opr) {
  // Notes, zero-PHI input by construction (the vreg pass removes every phi
  // before allocation), so the operand's reference point is simply the
  // instruction's own position
  return ins->liveness_info().range().start();
}

// ============================================================================
// Phase(0)
void RALinearize::Initialize() {
  // record instruction total size, for now HAssembler will perform a O(N)
  // complexity walk to get the total instruction size
  instruction_size_ = as_->instruction_size();

  // Reserve liveness map size
  liveness_map_.reserve(instruction_size_ * 2 + 2);
}

// ============================================================================
// Phase(1)
// Linearization. Mostly we just rely on HAssembler to give us correct
// orgnization of the LIR graph

// ----------------------------------------------------------------------------
// Liveness number is been alloacted as following, the lineraized phase simply
// walk through all the instruction inside of the graph in a pre-defined order
// and assigna linearized id, ie liveness id, for each node. The liveness id
// is been assigned to each point uniquely and increasing by 2.

void RALinearize::DoLinearize() {
  liveness_id_ = kSingleInstructionLivenessStep;  // first slot is not used

  auto add_liveness_map = [&](HInstruction* ins) {
    // An instruction covers 2 consequtive liveness number [x, x+1]
    liveness_map_.push_back(ins);
    liveness_map_.push_back(ins);
  };

  add_liveness_map(nullptr);

  auto set_liveness_start = [&](HInstruction* ins) {
    auto lid = liveness_id_;
    liveness_id_ += kSingleInstructionLivenessStep;

    ins->liveness_info().SetLivenessStart(lid);
    add_liveness_map(ins);
    CHECK(liveness_map_.size() == static_cast<std::size_t>(liveness_id_));
  };

  // Mark BB0's nodes' liveness ids, then BB1's
  as_->ForeachInstructionBB0(
      [&](HInstruction* ins) { set_liveness_start(ins); });

  as_->ForeachInstructionBB1(
      [&](HInstruction* ins) { set_liveness_start(ins); });
}

void RALinearize::Linearize() {
  DoLinearize();

  // Sanity check, now every instruction that is inside of the node should have
  // all the liveness id setup correctly
  BASE_DBG({
    as_->ForeachInstruction(
        [](HInstruction* ins) { CHECK(ins->liveness_info().HasLivenessId()); });
  });
}

void RALinearize::TraceLiveness() {
  std::stringstream fmt;
  as_->ForeachInstruction([&](auto ins) {
    fmt << ins->Print() << " :{" << ins->liveness_info().Print() << "}\n";
  });
  LOG(ERROR) << fmt.str();
}

// ============================================================================
// Entry
void RALinearize::Run() {
  Initialize();
  Linearize();
}

zone::Zone* RALinearize::zone() const { return quake_->zone(); }

RALinearize::RALinearize(Quake* q, HAssembler* as)
    : quake_(q),
      as_(as),
      instruction_size_(0),
      liveness_id_(0),
      liveness_map_() {}

}  // namespace lir
}  // namespace quake
