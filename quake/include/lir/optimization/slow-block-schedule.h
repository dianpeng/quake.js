#ifndef _QUAKE_LIR_OPTIMIZATION_SLOW_BLOCK_SCHEDULING_H_
#define _QUAKE_LIR_OPTIMIZATION_SLOW_BLOCK_SCHEDULING_H_
#include "base.h"

#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"

// ---------------------------------------------------------------------------
// The following pass simply move the slow block to the backyard. Ie it performs
// following transformation
//
// Before:
//
// 1. op1 a, b
//    ...
// 2. slow_block_start{
//   ...
// 3. slow_block_end}
//
// ---------------------------------------------------------------------------
//
// 1. op1 a, b
//
// 2. jmp #slow_block_start
//
//    ....
//
// N. slow_block_start{
//     ...
//    jmp #2
//    slow_block_end}
//
// ---------------------------------------------------------------------------

namespace quake {
namespace lir {

class SlowBlockSchedule {
 public:
  void Run(HAssembler*);

 private:
  void OnVisit(HInstruction*);
  void MoveSlowBlock(HAssembler*);

 private:
  struct SlowBlock {
    HInstruction* slow_block_start;
    HInstruction* slow_block_end;
    std::uint32_t slow_block_id;  // populate while traversaling
    void Check() {
      CHECK(slow_block_start);
      CHECK(slow_block_end);
    }

    SlowBlock(HInstruction* start, std::uint32_t id)
        : slow_block_start(start), slow_block_id(id) {}
  };

  std::vector<SlowBlock> slow_block_;
  bool in_slow_block_ = false;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_OPTIMIZATION_SLOW_BLOCK_SCHEDULING_H_

