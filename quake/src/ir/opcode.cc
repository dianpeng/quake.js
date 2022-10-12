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

bool OpcodeIsConst(Opcode op) {
  switch (op) {
    case Opcode::kKI32:
    case Opcode::kKI64:
    case Opcode::kKFloat64:
    case Opcode::kKBool:
    case Opcode::kKNull:
    case Opcode::kKUndef:
    case Opcode::kKNStr:
    case Opcode::kKStr:
    case Opcode::kKUninit:
      return true;
    default:
      return false;
  }
}

bool OpcodeIsPhi(Opcode op) {
  switch (op) {
    case Opcode::kPhiLoop:
      return true;
    default:
      return false;
  }
}

bool OpcodeIsJump(Opcode op) {
  switch (op) {
    case Opcode::kJump:
    case Opcode::kJumpTrue:
    case Opcode::kJumpFalse:
      return true;
    default:
      return false;
  }
}

std::optional<Opcode> OpcodeParse(const std::string_view& x) {
#define __(A, B, ...)             \
  do {                            \
    if (B == x) return Opcode::A; \
  } while (false);

  QUAKE_OPCODE_LIST(__)
#undef __  // __

  return {};
}

}  // namespace ir
}  // namespace quake
