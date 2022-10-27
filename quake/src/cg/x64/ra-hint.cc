#include "cg/x64/ra-hint.h"
#include "cg/x64/register.h"

#include "lir/register.h"
#include "lir/opcode.h"
#include "lir/operand.h"
#include "lir/instruction.h"

namespace quake {
namespace cg {
namespace x64 {

using namespace quake::lir;

void GetRAHint(const HInstruction* ins, RegisterInfo* output) {
  switch (ins->opcode()) {
      // binary operation

    case HOpcode::kIAdd:
    case HOpcode::kGIAdd:
    case HOpcode::kFAdd:

    case HOpcode::kISub:
    case HOpcode::kGISub:
    case HOpcode::kFSub:

    case HOpcode::kFMul:
    case HOpcode::kFDiv:

    case HOpcode::kAnd:
    case HOpcode::kOr:
    case HOpcode::kXor:

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

      output->hint_policy = RegisterHintPolicy::kInput0AsOutput;
      output->opr_constraint = OperandConstraint::kX64BinaryOp;
      break;

    case HOpcode::kSar:
    case HOpcode::kShl:
    case HOpcode::kShr:
      output->hint_policy = RegisterHintPolicy::kInput0AsOutput;
      output->opr_constraint = OperandConstraint::kX64ShiftOp;
      break;

    case HOpcode::kIMod:
      // Since the quotanient is in rdx, just force output to be inside of rdx
      output->force = Register::GetRegisterFromKind(RegisterKind::kRdx);
      break;

    case HOpcode::kRCallPtr:
    case HOpcode::kRCallGC:
    case HOpcode::kRCallB64:
    case HOpcode::kRCallF64:
    case HOpcode::kRCallI32:
    case HOpcode::kRCallVoid:

      // Try to reserve all the caller saved register if needed for any call
      // instruction. The RA will temporary save any instruction fore the
      // call instruction if needed and restore them when reference in the
      // future. Notes, RA also setup the ABI correctly. For us, currently we
      // just allow runtime call to be at most 4 registers for simplicity, ie
      // all parameters will be carried on registers on X64/ARM64 platform
      output->reserve.Union(RegisterSet::GetCallerSavedRegister());

      if (ins->opcode() == HOpcode::kRCallF64) {
        // the output of native call which returns a float64, will always be
        // XMM0 registers
        output->force = Register::GetRegisterFromKind(RegisterKind::kXmm0);
      }
      break;

    default:
      break;
  }
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
