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

std::vector<RegisterKind> GetABICalleeSavedGP() {
  UNREACHABLE();
  return {};
}

std::vector<RegisterKind> GetABICallerSavedGP() {
  UNREACHABLE();
  return {};
}

std::vector<RegisterKind> GetABICalleeSavedFP() {
  UNREACHABLE();
  return {};
}

std::vector<RegisterKind> GetABICallerSavedFP() {
  UNREACHABLE();
  return {};
}

#endif  // OS_SYSV

}  // namespace x64
}  // namespace cg
}  // namespace quake
