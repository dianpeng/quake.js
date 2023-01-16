#include "ir/loop-exit-analysis.h"

namespace quake {
namespace ir {

std::pair<bool, const void*> BranchRangeHasLoopExit(const BytecodeRange& branch,
                                                    const BytecodeRange& loop) {
  for (auto itr = BytecodeIterator{branch}; itr.Has(); itr.Next()) {
    const auto& i = itr.info();
    const void* jmp_target = nullptr;

    switch (i.opcode) {
      case Bytecode::OP_goto:
      case Bytecode::OP_if_true:
      case Bytecode::OP_if_false: {
        auto offset = itr.GetI32();
        jmp_target = itr.NextPC(offset);
      } break;

      default:
        break;
    }

    if (!jmp_target) continue;

    // (0) If the jump target of the above jump target outside of the loop,
    //     then it does a loop exit jump
    if (!loop.Contains(jmp_target)) {
      return std::make_pair(true, jmp_target);
    }

    // (1) If the jump target of the above jump target outside of the branch
    //     range, which means it is end of the branch range, ie outer control
    //     flow block is ended
    if (!branch.Contains(jmp_target)) {
      return std::make_pair(false, nullptr);
    }

    // (2) The jump resides inside of the specified branch range, okay, just
    //     keep searching until we hit the end of the control flow block
  }

  return std::make_pair(false, nullptr);
}

}  // namespace ir
}  // namespace quake
