#include "lir/optimization/slow-block-schedule.h"

namespace quake {
namespace lir {

void SlowBlockSchedule::Run(HAssembler* as) {
  in_slow_block_ = false;
  as->ForeachInstruction([this](HInstruction* ins) { OnVisit(ins); });
  MoveSlowBlock(as);
}

void SlowBlockSchedule::OnVisit(HInstruction* ins) {
  switch (ins->opcode()) {
    case HOpcode::kSlowBlockStart: {
      CHECK(!in_slow_block_);
      in_slow_block_ = true;
      auto id = static_cast<std::uint32_t>(slow_block_.size());
      slow_block_.emplace_back(ins, id);
    } break;

    case HOpcode::kSlowBlockEnd: {
      CHECK(in_slow_block_);
      in_slow_block_ = false;
      CHECK(!slow_block_.empty());
      slow_block_.back().slow_block_end = ins;
    } break;

    default:
      break;
  }
}

void SlowBlockSchedule::MoveSlowBlock(HAssembler* as) {
  auto position = as->end_instruction();

  for (auto& block : slow_block_) {
    block.Check();

    // record the *next* instruction right after the slow block, ie where to
    // recover and where to return
    auto jmp_position =
        block.slow_block_end->next();  // next position to recover

    // (0) shift the bulk of the block body to the position
    auto block_start = as->MoveInstructionRangeBefore(
        block.slow_block_start, block.slow_block_end, position);

    // (1) insert the very first forward jump which jumps to the slow block
    //     been inserted at the back of the trace body
    {
      auto slow_block_label = as->NewLabel(block_start);
      as->NewInstructionBefore(block.slow_block_start->ir_node(), HOpcode::kJmp,
                               slow_block_label, jmp_position);
    }

    // (2) insert the very last backward jump from the slow block end
    {
      auto recovery_block_label = as->NewLabel(jmp_position);
      as->NewInstructionBefore(block.slow_block_end->ir_node(), HOpcode::kJmp,
                               recovery_block_label, block.slow_block_end);
    }
  }
}

}  // namespace lir
}  // namespace quake
