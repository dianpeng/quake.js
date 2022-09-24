#include <ir/opcode.h>

namespace quake {
namespace ir {
namespace {

#define __(A, B, C, D, E) OpcodeInfo A##Info{B, C, D, Type{E}};
QUAKE_OPCODE_LIST(__)
#undef __  // __

OpcodeInfo kNone;

}  // namespace

const OpcodeInfo& GetOpcodeInfo(Opcode opcode) {
#define __(A, ...) \
  case Opcode::A:  \
    return A##Info;

  switch (opcode) {
    QUAKE_OPCODE_LIST(__);
    default:
      UNREACHABLE();
      return kNone;
  }

#undef __  // __
}

}  // namespace ir
}  // namespace quake
