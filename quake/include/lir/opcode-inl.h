#ifndef _QUAKE_LIR_OPCODE_INL_H_
#define _QUAKE_LIR_OPCODE_INL_H_

namespace quake {
namespace lir {

inline bool IsHOpcodeNativeCall(HOpcode x) {
  switch (x) {
    case HOpcode::kFCallBox:
    case HOpcode::kFCallPtr:
    case HOpcode::kFCallB64:
    case HOpcode::kFCallF64:
    case HOpcode::kFCallI32:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodePhi(HOpcode x) {
  switch (x) {
    case HOpcode::kPhiLoop:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeDeopt(HOpcode x) {
  switch (x) {
    case HOpcode::kDeopt:
    // conditional deoptimization, used to generate various guard
    case HOpcode::kDeoptIfZ:
    case HOpcode::kDeoptIfEq:
    case HOpcode::kDeoptIfNe:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeJump(HOpcode x) {
  switch (x) {
    case HOpcode::kJmpNe:
    case HOpcode::kJmpEq:
    case HOpcode::kJmpBt:
    case HOpcode::kJmpBe:
    case HOpcode::kJmpLt:
    case HOpcode::kJmpLe:
    case HOpcode::kJmpAt:
    case HOpcode::kJmpAe:
    case HOpcode::kJmpGt:
    case HOpcode::kJmpGe:
    case HOpcode::kJmpNz:
    case HOpcode::kJmpZ:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeComp(HOpcode x) {
  switch (x) {
    case HOpcode::kCmp:
    case HOpcode::kCmpF:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeMove(HOpcode x) {
  switch (x) {
    case HOpcode::kMov:
    case HOpcode::kMovF:
    case HOpcode::kMovZX:
    case HOpcode::kMovSX:

      // constant preparation, ie materialize immediate number into registers
    case HOpcode::kMovI32:
    case HOpcode::kMovI64:
    case HOpcode::kMovB64:
    case HOpcode::kMovF64:
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
