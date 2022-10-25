#include "cg/x64/abi.h"

namespace quake {
namespace cg {
namespace x64 {

// Currently just support SystemV ABI
#ifdef OS_SYSV

std::optional<RegisterKind> GetABIGPArg(int index) {
  static RegisterKind kGP[] = {RegisterKind::kRdi, RegisterKind::kRsi,
                               RegisterKind::kRdx, RegisterKind::kRcx,
                               RegisterKind::kR8,  RegisterKind::kR9};

  if (index >= 0 && index <= 5) {
    return kGP[index];
  } else {
    return {};
  }
}

std::optional<RegisterKind> GetABIFPArg(int index) {
  static RegisterKind kFP[] = {RegisterKind::kXmm0, RegisterKind::kXmm1,
                               RegisterKind::kXmm2, RegisterKind::kXmm3,
                               RegisterKind::kXmm4, RegisterKind::kXmm5,
                               RegisterKind::kXmm6, RegisterKind::kXmm7};

  if (index >= 0 && index <= 7) {
    return kFP[index];
  } else {
    return {};
  }
}

// Notes, Rax, Rdi, Rsi, R15 and Xmm15 is been reserved, and they do not show up
// anywhere

std::vector<RegisterKind> GetABICallerSavedGP() {
  static const std::vector<RegisterKind> kSet{
      RegisterKind::kRcx, RegisterKind::kRdx, RegisterKind::kRdi,
      RegisterKind::kRsi, RegisterKind::kR8,  RegisterKind::kR9,
      RegisterKind::kR10, RegisterKind::kR11};
  return kSet;
}

std::vector<RegisterKind> GetABICalleeSavedGP() {
  static const std::vector<RegisterKind> kSet{
      RegisterKind::kRbx, RegisterKind::kR12, RegisterKind::kR13,
      RegisterKind::kR14};
  return kSet;
}

std::vector<RegisterKind> GetABICallerSavedFP() {
  static const std::vector<RegisterKind> kSet{
      RegisterKind::kXmm0,  RegisterKind::kXmm1,  RegisterKind::kXmm2,
      RegisterKind::kXmm3,  RegisterKind::kXmm4,  RegisterKind::kXmm5,
      RegisterKind::kXmm6,  RegisterKind::kXmm7,  RegisterKind::kXmm8,
      RegisterKind::kXmm9,  RegisterKind::kXmm10, RegisterKind::kXmm11,
      RegisterKind::kXmm12, RegisterKind::kXmm13, RegisterKind::kXmm14};
  return kSet;
}

std::vector<RegisterKind> GetABICalleeSavedFP() { return {}; }

#endif  // OS_SYSV

}  // namespace x64
}  // namespace cg
}  // namespace quake
