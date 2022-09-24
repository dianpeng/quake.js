#include "ir/bytecode.h"

namespace quake {
namespace ir {

namespace {

#define DEF(OP, SIZE, NPOP, NPUSH, BFMT) \
  QJSBytecodeInfo kOP##OP{               \
      #OP, Bytecode::OP_##OP, BytecodeFormat::kFmt_##BFMT, SIZE, NPOP, NPUSH};
#include <quickjs-opcode.h>
#undef DEF  // DEF

QJSBytecodeInfo kNull{};

}  // namespace

// clang-format off
const QJSBytecodeInfo& GetQJSBytecodeInfo(Bytecode bc) {
  switch (bc) {
#define DEF(OP, ...)      \
  case Bytecode::OP_##OP: \
    return kOP##OP;

#include <quickjs-opcode.h>

#undef DEF  // DEF

  default:
    UNREACHABLE();
    return kNull;

  }
}
// clang-format on

}  // namespace ir
}  // namespace quake
