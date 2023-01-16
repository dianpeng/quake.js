#ifndef _QUAKE_IR_LOOP_EXIT_ANALYSIS_H_
#define _QUAKE_IR_LOOP_EXIT_ANALYSIS_H_
#include "base.h"
#include "bytecode.h"

namespace quake {
namespace ir {

std::pair<bool, const void*> BranchRangeHasLoopExit(const BytecodeRange& branch,
                                                    const BytecodeRange& loop);

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_LOOP_EXIT_ANALYSIS_H_
