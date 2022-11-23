#ifndef _QUAKE_LIR_OPCODE_INL_H_
#define _QUAKE_LIR_OPCODE_INL_H_

namespace quake {
namespace lir {

inline bool IsHOpcodeRCall(HOpcode x) {
  switch (x) {
    case HOpcode::kRCallPtr:
    case HOpcode::kRCallGC:
    case HOpcode::kRCallB64:
    case HOpcode::kRCallF64:
    case HOpcode::kRCallI32:
    case HOpcode::kRCallVoid:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeJSCall(HOpcode x) {
  switch (x) {
    case HOpcode::kJSCallF:
    case HOpcode::kJSCallFT:
    case HOpcode::kJSCallFN:
    case HOpcode::kJSCallFTN:
    case HOpcode::kJSCallCtor:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeNativeCall(HOpcode x) {
  return IsHOpcodeRCall(x) || IsHOpcodeJSCall(x);
}

inline bool IsHOpcodePhi(HOpcode x) {
  switch (x) {
    case HOpcode::kPhiLoopF:
    case HOpcode::kPhiLoopI:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeDeopt(HOpcode x) {
  switch (x) {
    case HOpcode::kDeopt:
      return true;
    default:
      return IsHOpcodeConditionalDeopt(x);
  }
}

inline bool IsHOpcodeConditionalBranchDeopt(HOpcode x) {
  switch (x) {
    case HOpcode::kDeoptIfTrue:
    case HOpcode::kDeoptIfFalse:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeConditionalDeopt(HOpcode x) {
  switch (x) {
    case HOpcode::kDeoptIfTrue:
    case HOpcode::kDeoptIfFalse:
    case HOpcode::kDeoptNe:
    case HOpcode::kDeoptEq:
    case HOpcode::kDeoptLt:
    case HOpcode::kDeoptLe:
    case HOpcode::kDeoptGt:
    case HOpcode::kDeoptGe:
    case HOpcode::kDeoptBt:
    case HOpcode::kDeoptBe:
    case HOpcode::kDeoptAt:
    case HOpcode::kDeoptAe:
    case HOpcode::kDeoptNz:
    case HOpcode::kDeoptZ:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeConditionalJump(HOpcode x) {
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
    case HOpcode::kJmpBoxIsHeap:
    case HOpcode::kJmpBoxIsPrimitive:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeJump(HOpcode x) {
  switch (x) {
    case HOpcode::kJmp:
      return true;
    default:
      return IsHOpcodeConditionalJump(x);
  }
}

inline bool IsHOpcodeCmp(HOpcode x) {
  switch (x) {
    case HOpcode::kCmpI:
    case HOpcode::kCmpF:
    case HOpcode::kCmpBoxIsHeap:
    case HOpcode::kCmpBoxIsPrimitive:
    case HOpcode::kCmpTest:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeBranch(HOpcode x) {
  switch (x) {
    case HOpcode::kBranchTrue:
    case HOpcode::kBranchFalse:
      return true;
    default:
      return false;
  }
}

inline bool IsHOpcodeCondition(HOpcode x) {
  switch (x) {
    case HOpcode::kILt:
    case HOpcode::kILe:
    case HOpcode::kIGt:
    case HOpcode::kIGe:
    case HOpcode::kIEq:
    case HOpcode::kINe:

    case HOpcode::kFLt:
    case HOpcode::kFLe:
    case HOpcode::kFGt:
    case HOpcode::kFGe:
    case HOpcode::kFEq:
    case HOpcode::kFNe:

    case HOpcode::kBoxIsHeap:
    case HOpcode::kBoxIsPrimitive:
      return true;

    default:
      return false;
  }
}

inline bool IsHOpcodeMove(HOpcode x) {
  switch (x) {
    case HOpcode::kMov:
    case HOpcode::kMovF:

    case HOpcode::kShuffle:
    case HOpcode::kShuffleF:

    case HOpcode::kTrans:
    case HOpcode::kTransF:

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

inline bool IsHOpcodeGuard(HOpcode x) {
  switch (x) {
    case HOpcode::kGIAdd:
    case HOpcode::kGISub:
    case HOpcode::kGIMul:

    case HOpcode::kGDec:
    case HOpcode::kGInc:
    case HOpcode::kGNeg:

    case HOpcode::kDeopt:
    case HOpcode::kDeoptIfTrue:
    case HOpcode::kDeoptIfFalse:

    case HOpcode::kGUnboxI:
    case HOpcode::kGUnboxF:
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
