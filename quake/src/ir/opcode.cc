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

// FIXME(dpeng): Put this into opcode definition
bool OpcodeNeedFloatOperand(Opcode op) {
  switch (op) {
    case Opcode::kAddReal:
    case Opcode::kSubReal:
    case Opcode::kMulReal:
    case Opcode::kDivReal:
    case Opcode::kModReal:
    case Opcode::kPowReal:
    case Opcode::kLtReal:
    case Opcode::kLeReal:
    case Opcode::kGtReal:
    case Opcode::kGeReal:
    case Opcode::kEqReal:
    case Opcode::kNeReal:
    case Opcode::kNegReal:
    case Opcode::kIncReal:
    case Opcode::kDecReal:
    case Opcode::kF2Bit:
    case Opcode::kF2B:
      return true;
    default:
      return false;
  }
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
      return true;
    default:
      return false;
  }
}

bool OpcodeIsBinaryArithmetic(Opcode op) {
  switch (op) {
    case Opcode::kGAddInt:
    case Opcode::kGSubInt:
    case Opcode::kGMulInt:

    case Opcode::kSubInt:
    case Opcode::kAddInt:
    case Opcode::kMulInt:
    case Opcode::kModInt:

    case Opcode::kSubReal:
    case Opcode::kAddReal:
    case Opcode::kMulReal:
    case Opcode::kDivReal:
    case Opcode::kModReal:
    case Opcode::kPowReal:

    // int(bit) operations
    case Opcode::kShlInt:
    case Opcode::kShrInt:
    case Opcode::kBitNorm:
    case Opcode::kAndInt:
    case Opcode::kOrInt:
    case Opcode::kXorInt:

    // Comparison
    case Opcode::kLtInt:
    case Opcode::kLeInt:
    case Opcode::kGtInt:
    case Opcode::kGeInt:
    case Opcode::kEqInt:
    case Opcode::kNeInt:

    case Opcode::kLtReal:
    case Opcode::kLeReal:
    case Opcode::kGtReal:
    case Opcode::kGeReal:
    case Opcode::kEqReal:
    case Opcode::kNeReal:
      return true;

    default:
      return false;
  }
}

bool OpcodeIsUnaryArithmetic(Opcode op) {
  switch (op) {
    // Unary operations
    case Opcode::kGIncInt:
    case Opcode::kGDecInt:
    case Opcode::kGNegInt:

    case Opcode::kIncInt:
    case Opcode::kDecInt:
    case Opcode::kNegInt:

    case Opcode::kIncReal:
    case Opcode::kDecReal:
    case Opcode::kNegReal:

    case Opcode::kI2B:
    case Opcode::kF2B:
    case Opcode::kI2F:
    case Opcode::kF2Bit:
      return true;

    default:
      return false;
  }
}

bool OpcodeIsArithmetic(Opcode op) {
  return OpcodeIsBinaryArithmetic(op) || OpcodeIsUnaryArithmetic(op);
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
