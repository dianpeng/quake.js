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

void GetRAHint(HInstruction* ins, RegisterInfo* output) {
  switch (ins->opcode()) {
    // On x64, following instruction takes one of operand(input) as output
    case HOpcode::kIAdd:
    case HOpcode::kISub:
    case HOpcode::kIMul:
    case HOpcode::kAnd:
    case HOpcode::kOr:
    case HOpcode::kXor:
    case HOpcode::kLShift:
    case HOpcode::kRShift:

    case HOpcode::kFAdd:
    case HOpcode::kFSub:
    case HOpcode::kFMul:
    case HOpcode::kFDiv:
      output->hint_policy = RegisterHintPolicy::kInput0AsOutput;
      break;

    case HOpcode::kIDiv:
      // idivi will pollute 2 registers, ie rdx:rax, and store the result into
      // rdx:rax as well. Typical compiler backend will represent it as a reg
      // pair to allow it participate into the RA. Here, since we make rax as
      // scratch register, cannot be allocated by RA, we will just need to mark
      // rdx as reserved register during this instruction
      output->reserve.Set(static_cast<std::uint32_t>(RegisterKind::kRdx));
      break;

    case HOpcode::kIMod:
      // Since the quotanient is in rdx, just force output to be inside of rdx
      output->force = Register::GetRegisterFromKind(RegisterKind::kRdx);
      break;

    case HOpcode::kFCallBox:
    case HOpcode::kFCallPtr:
    case HOpcode::kFCallB64:
    case HOpcode::kFCallF64:
    case HOpcode::kFCallI32:

      // Try to reserve all the caller saved register if needed for any call
      // instruction. The RA will temporary save any instruction fore the
      // call instruction if needed and restore them when reference in the
      // future. Notes, RA also setup the ABI correctly. For us, currently we
      // just allow runtime call to be at most 4 registers for simplicity, ie
      // all parameters will be carried on registers on X64/ARM64 platform
      output->reserve.Union(RegisterSet::GetCallerSavedRegister());
      break;

    default:
      break;
  }
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
