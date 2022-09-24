#ifndef _QUAKE_LIR_OPCODE_INL_H_
#define _QUAKE_LIR_OPCODE_INL_H_

namespace quake {
namespace lir {

inline bool IsHOpcodePhi(HOpcode x) {
  switch (x) {
    case HOpcode::kPhiLV:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeDeopt(HOpcode x) {
  switch (x) {
    case HOpcode::kDeopt:
    case HOpcode::kDeoptIf:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeValue(HOpcode x) {
  return GetHOpcodeInfo(x).output.IsValue();
}

inline bool IsHOpcodeOp(HOpcode x) { return GetHOpcodeInfo(x).output.IsOp(); }

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_OPCODE_INL_H_
