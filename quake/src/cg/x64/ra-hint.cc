#include "cg/x64/ra-hint.h"
#include "cg/x64/register.h"

#include "lir/register.h"
#include "lir/opcode.h"
#include "lir/operand.h"
#include "lir/instruction.h"

namespace quake {
namespace cg {
namespace x64 {

namespace {

void SetUpRegisterHintABI(lir::NativeCallCallerSaveGPRegisterVector* gp,
                          lir::NativeCallCallerSaveFPRegisterVector* fp) {
  for (const auto& x : GetABICallerSavedGP()) {
    gp->push_back(lir::Register::GetRegisterFromKind(x));
  }
  for (const auto& x : GetABICallerSavedFP()) {
    fp->push_back(lir::Register::GetRegisterFromKind(x));
  }
}

}  // namespace

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
      output->requirement = RegisterAllocationRequirement::ForceRegister(
          Register::GetRegisterFromKind(RegisterKind::kRdx));
      break;

    // Increment/Decrement, the output and input are preferred to be the same
    case HOpcode::kIncF:
    case HOpcode::kInc:
    case HOpcode::kDec:
    case HOpcode::kDecF:
      output->hint_policy = RegisterHintPolicy::kInput0AsOutput;
      break;

    // Phi node, use kInput0AsOutput policy
    case HOpcode::kPhiLoopF:
    case HOpcode::kPhiLoopI:
    case HOpcode::kPhiLoopObj:
    case HOpcode::kPhiBranchF:
    case HOpcode::kPhiBranchI:
    case HOpcode::kPhiBranchBox:
      output->hint_policy = RegisterHintPolicy::kInput0AsOutput;
      break;

    // special property/element accessor's fast path
    case HOpcode::kObjectFieldGetAtomFast:
    case HOpcode::kGlobalGetVarAtomFast:
    case HOpcode::kLoadArrayValue:
    case HOpcode::kLoadArrayArguments:
    case HOpcode::kLoadArrayInt8:
    case HOpcode::kLoadArrayUInt8:
    case HOpcode::kLoadArrayUInt8C:
    case HOpcode::kLoadArrayInt16:
    case HOpcode::kLoadArrayUInt16:
    case HOpcode::kLoadArrayInt32:
    case HOpcode::kLoadArrayUInt32:
    case HOpcode::kLoadArrayF32:
    case HOpcode::kLoadArrayF64:
      output->reserve.Set(Register::GetRegisterFromKind(RegisterKind::kRdx));
      break;

    // js call, ie calling out for script function evaluation, slow
    case HOpcode::kJSCallF:
    case HOpcode::kJSCallFT:
    case HOpcode::kJSCallFN:
    case HOpcode::kJSCallFTN:
    case HOpcode::kJSCallCtor:
    case HOpcode::kArrayFrom:
    case HOpcode::kApply:
      SetUpRegisterHintABI(&output->opr_abi_reserve_gp,
                           &output->opr_abi_reserve_fp);
      break;

    case HOpcode::kRCallPtr:
    case HOpcode::kRCallGC:
    case HOpcode::kRCallB64:
    case HOpcode::kRCallF64:
    case HOpcode::kRCallI32:
    case HOpcode::kRCallVoid:
    case HOpcode::kRCallBox:

      // Try to reserve all the caller saved register if needed for any call
      // instruction. The RA will temporary save any instruction fore the
      // call instruction if needed and restore them when reference in the
      // future. Notes, RA also setup the ABI correctly. For us, currently we
      // just allow runtime call to be at most 4 registers for simplicity, ie
      // all parameters will be carried on registers on X64/ARM64 platform
      SetUpRegisterHintABI(&output->opr_abi_reserve_gp,
                           &output->opr_abi_reserve_fp);

      switch (ins->opcode()) {
        case HOpcode::kRCallF64:
          // the output of native call which returns a float64, will always be
          // XMM0 registers
          output->requirement = RegisterAllocationRequirement::ForceRegister(
              Register::GetRegisterFromKind(RegisterKind::kXmm0));
          break;

        default:
          break;
      }

    default:
      break;
  }
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
