#ifndef _QUAKE_CG_X64_REGISTER_H_
#define _QUAKE_CG_X64_REGISTER_H_
#include "common.h"

// ----------------------------------------------------------------------------
// clang-format off
#define QUAKE_X64_REGISTER_ID_LIST(__) \
  /* ---------------------------------------------------------------*/ \
  /* Classical 8 GP registers */ \
  /* notes, we do not expose AH/BH/CH/DH for now */ \
  __(kRax, "rax", 8) __(kEax, "eax", 4) __(kAx, "ax", 2) __(kAl, "al", 1)   \
  __(kRbx, "rbx", 8) __(kEbx, "ebx", 4) __(kBx, "bx", 2) __(kBl, "al", 1)   \
  __(kRcx, "rcx", 8) __(kEcx, "ebx", 4) __(kCx, "cx", 2) __(kCl, "cl", 1)   \
  __(kRdx, "rdx", 8) __(kEdx, "edx", 4) __(kDx, "dx", 2) __(kDl, "dl", 1)   \
  __(kRsp, "rsp", 8) __(kEsp, "esp", 4) __(kSp, "sp", 2) __(kSpl, "spl", 1) \
  __(kRbp, "rbp", 8) __(kEbp, "ebp", 4) __(kBp, "bp", 2) __(kBpl, "bpl", 1) \
  __(kRsi, "rsi", 8) __(kEsi, "esi", 4) __(kSi, "si", 2) __(kSil, "sil", 1) \
  __(kRdi, "rdi", 8) __(kEdi, "edi", 4) __(kDi, "di", 2) __(kDil, "dil", 1) \
  \
  /* ----------------------------------------------------------------*/ \
  /* Extension 8 GP registers on 64 bits machine */ \
  __(kR8,  "r8",  8) __(kR8d , "r8d",  4) __(kR8w,  "r8w",  2) __(kR8l,  "r8l",  1) \
  __(kR9,  "r9",  8) __(kR9d , "r9d",  4) __(kR9w,  "r9w",  2) __(kR9l,  "r9l",  1) \
  __(kR10, "r10", 8) __(kR10d, "r10d", 4) __(kR10w, "r10w", 2) __(kR10l, "r10l", 1) \
  __(kR11, "r11", 8) __(kR11d, "r11d", 4) __(kR11w, "r11w", 2) __(kR11l, "r11w", 1) \
  __(kR12, "r12", 8) __(kR12d, "r12d", 4) __(kR12w, "r12w", 2) __(kR12l, "r12w", 1) \
  __(kR13, "r13", 8) __(kR13d, "r13d", 4) __(kR13w, "r13w", 2) __(kR13l, "r13l", 1) \
  __(kR14, "r14", 8) __(kR14d, "r14d", 4) __(kR14w, "r14w", 2) __(kR14l, "r14l", 1) \
  __(kR15, "r15", 8) __(kR15d, "r15d", 4) __(kR15w, "r15w", 2) __(kR15l, "r15l", 1) \
  \
  /* ----------------------------------------------------------------*/        \
  /* SSE XMM registers, we just use 15 XMM registers for now, since we just */ \
  /* need them to perform scalar floating point operations. If we ever add */  \
  /* auto vectorization optimization, we need XMM/YMM/ZMM to perform the */    \
  /* vectorization operations. We only count xmm as 8 bytes for scalar */ \
  __(kXmm0, "xmm0", 8) \
  __(kXmm1, "xmm1", 8) \
  __(kXmm2, "xmm2", 8) \
  __(kXmm3, "xmm3", 8) \
  __(kXmm4, "xmm4", 8) \
  __(kXmm5, "xmm5", 8) \
  __(kXmm6, "xmm6", 8) \
  __(kXmm7, "xmm7", 8) \
  \
  /* x64 added */        \
  __(kXmm8,  "xmm8",  8) \
  __(kXmm9,  "xmm9",  8) \
  __(kXmm10, "xmm10", 8) \
  __(kXmm11, "xmm11", 8) \
  __(kXmm12, "xmm12", 8) \
  __(kXmm13, "xmm13", 8) \
  __(kXmm14, "xmm14", 8) \
  __(kXmm15, "xmm15", 8)


#define QUAKE_X64_REGISTER_KIND_LIST(__) \
  __(kRax,   "rax")   \
  __(kRbx,   "rbx")   \
  __(kRcx,   "rcx")   \
  __(kRdx,   "rdx")   \
  __(kRsp,   "rsp")   \
  __(kRbp,   "rbp")   \
  __(kRsi,   "rsi")   \
  __(kRdi,   "rdi")   \
  __(kR8,    "r8")    \
  __(kR9,    "r9")    \
  __(kR10,   "r10")   \
  __(kR11,   "r11")   \
  __(kR12,   "r12")   \
  __(kR13,   "r13")   \
  __(kR14,   "r14")   \
  __(kR15,   "r15")   \
  __(kXmm0,  "xmm0")  \
  __(kXmm1,  "xmm1")  \
  __(kXmm2,  "xmm2")  \
  __(kXmm3,  "xmm3")  \
  __(kXmm4,  "xmm4")  \
  __(kXmm5,  "xmm5")  \
  __(kXmm6,  "xmm6")  \
  __(kXmm7,  "xmm7")  \
  __(kXmm8,  "xmm8")  \
  __(kXmm9,  "xmm9")  \
  __(kXmm10, "xmm10") \
  __(kXmm11, "xmm11") \
  __(kXmm12, "xmm12") \
  __(kXmm13, "xmm13") \
  __(kXmm14, "xmm14") \
  __(kXmm15, "xmm15")

// clang-format on

namespace quake {
namespace cg {
namespace x64 {

constexpr std::size_t kGPRegisterCount = 16;
constexpr std::size_t kFPRegisterCount = 16;
constexpr std::size_t kRegisterCount = kGPRegisterCount + kFPRegisterCount;

// clang-format off
enum class RegisterKind : std::uint16_t {
#define __(A, ...) A,
  QUAKE_X64_REGISTER_KIND_LIST(__)
#undef __  // __
  kSizeOfRegisterKind
};
// clang-format on

// clang-format off
//
// The RegisterId is the unique id for every possible register encoding inside
// of X64 arch, ie each sub-type of register will become a unqiue id. In most
// cases, we do not use RegisterId, but uses RegisterKind. Only when perform
// materialization, we use RegisterId to dispatch the exact register type.
enum class RegisterId : std::uint32_t {
#define __(A, ...) A,
  QUAKE_X64_REGISTER_ID_LIST(__)
#undef __  // __
  kSizeOfRegister
};
// clang-format on

struct MachineRegister {
  RegisterKind kind;
  RegisterId id;
  RegisterType type;
  SizeClass size_class;
  std::string name;

  MachineRegister(RegisterKind k, RegisterId i, RegisterType t, SizeClass s,
                  std::string&& n)
      : kind(k), id(i), type(t), size_class(s), name(std::move(n)) {}

  bool operator==(const MachineRegister& rinfo) const { return id == rinfo.id; }
  bool operator!=(const MachineRegister& rinfo) const { return id != rinfo.id; }

 public:
  bool IsGP() const;
  bool IsFP() const;
  bool Alias(const MachineRegister&) const;
  bool Match(SizeClass) const;
  bool IsFullSize() const;
};

RegisterId RegisterKindToId(RegisterKind);
RegisterKind RegisterIdToKind(RegisterId);

RegisterType RegisterKindToType(RegisterKind);
RegisterType RegisterIdToType(RegisterId);

bool RegisterKindContain(RegisterKind, RegisterId);
bool RegisterKindIsFP(RegisterKind);
bool RegisterKindIsGP(RegisterKind);
std::optional<RegisterId> RegisterSizeClassToId(RegisterKind, SizeClass);

const MachineRegister* GetRegisterFromId(RegisterId);
const MachineRegister* GetRegisterFromKind(RegisterKind);
const MachineRegister* GetRegisterSubClass(RegisterKind, SizeClass);

// Used to initialize RA for free slot (ie inactive register)
std::vector<RegisterKind> GetFreeGPRegister();
std::vector<RegisterKind> GetFreeFPRegister();

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_REGISTER_H_
