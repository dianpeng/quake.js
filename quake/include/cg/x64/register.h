#ifndef _QUAKE_CG_X64_REGISTER_H_
#define _QUAKE_CG_X64_REGISTER_H_
#include "common.h"

namespace quake {
namespace cg {
namespace x64 {

constexpr std::size_t kGPRegisterCount = 16;
constexpr std::size_t kFPRegisterCount = 16;
constexpr std::size_t kRegisterCount = kGPRegisterCount + kFPRegisterCount;

enum class RegisterKind : std::uint16_t {
  // -------------------------------------------------------------------------
  // X64 general purpose registers
  kRax,
  kRbx,
  kRcx,
  kRdx,
  kRdi,
  kRsi,
  kRbp,
  kRsp,

  kR8,
  kR9,
  kR10,
  kR11,
  kR12,
  kR13,
  kR14,
  kR15,

  // -------------------------------------------------------------------------
  // X64 SSE 4.2 flaoting point registers
  kXMM0,
  kXMM1,
  kXMM2,
  kXMM3,
  kXMM4,
  kXMM5,
  kXMM6,
  kXMM7,
  kXMM8,
  kXMM9,
  kXMM10,
  kXMM11,
  kXMM12,
  kXMM13,
  kXMM14,
  kXMM15,

  kSizeOfRegisterKind
};

// clang-format off
enum class RegisterId : std::uint32_t {
  kRax, kEax, kAx, kAl,
  kRbx, kEbx, kBx, kBl,
  kRcx, kEcx, kCx, kCl,
  kRdx, kEdx, kDx, kDl,
};
// clang-format on

enum class RegisterUsage : std::uint8_t {
  kUser,     // user's code can be used
  kCtx,      // hold's our context object
  kScratch,  // hold's scratch usage

  kSizeOfRegiserUsage
};

struct MachineRegister {
  RegisterId id;  // always unique
  RegisterKind kind;
  RegisterType type;
  SizeClass size;

  std::string name;

  bool operator==(const MachineRegister& rinfo) const { return id == rinfo.id; }
  bool operator!=(const MachineRegister& rinfo) const { return id != rinfo.id; }

  inline bool Alias(const MachineRegister&) const;
};

const MachineRegister* GetRegister(RegisterId);

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_REGISTER_H_
