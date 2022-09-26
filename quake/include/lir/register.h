#ifndef _QUAKE_LIR_REGISTER_H_
#define _QUAKE_LIR_REGISTER_H_
#include "base.h"

#ifdef QUAKE_TARGET_X64
#include "cg/x64/register.h"
#include "cg/x64/abi.h"
#define QUAKE_FOREACH_REGISTER_ID QUAKE_X64_REGISTER_ID_LIST
#define QUAKE_FOREACH_REGISTER_KIND QUAKE_X64_REGISTER_KIND_LIST
#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64

namespace quake {
namespace lir {

#ifdef QUAKE_TARGET_X64

using cg::x64::kFPRegisterCount;
using cg::x64::kGPRegisterCount;
using cg::x64::kRegisterCount;
using cg::x64::MachineRegister;
using cg::x64::RegisterId;
using cg::x64::RegisterKind;

using cg::x64::GetRegisterFromId;
using cg::x64::GetRegisterFromKind;
using cg::x64::GetRegisterSubClass;

using cg::x64::GetABIGPArg;
using cg::x64::GetABIFPArg;

#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64

class Register {
 public:
  explicit Register(const MachineRegister* mreg) : mreg_(mreg) {}

  // We mostly just use kind_id instead of id since id has structure
  std::uint32_t kind_id() const {
    return static_cast<std::uint32_t>(mreg_->kind);
  }

 private:
  const MachineRegister* mreg_;
};

// ----------------------------------------------------------------------------
// For each platform, we will reserve 2 registers to be used internally for
// our own usage.
//
// #1 -> storing the context
// #2 -> scratch register, used internally by JIT

static constexpr std::size_t kReservedRegisterCount = 2;

static constexpr std::size_t kGPUsrRegisterCount =
    kGPRegisterCount - kReservedRegisterCount;

static constexpr std::size_t kFPUsrRegisterCount = kFPRegisterCount;

// ----------------------------------------------------------------------------
// Reserved register
static constexpr RegisterId kContextRegisterId = RegisterId::kR15;
static constexpr RegisterId kScratchRegisterId = RegisterId::kRax;

extern const Register* kContextRegister;
extern const Register* kScratchRegister;

using RegBitset = std::bitset<kRegisterCount>;

class RegisterSet {
 public:
  RegisterSet() = default;

  bool Get(std::uint32_t id) const { return bitset_.test(id); }
  bool Get(const Register* r) const { return Get(r->kind_id()); }

  void Set(std::uint32_t id) { bitset_.set(id, true); }
  void Set(const Register* r) { Set(r->kind_id()); }

  void Unset(std::uint32_t id) { bitset_.set(id, false); }
  void Unset(const Register* r) { Unset(r->kind_id()); }

  void Clear() { bitset_.reset(); }
  std::size_t Count() const { return bitset_.count(); }
  std::size_t Size() const { return bitset_.size(); }
  bool Empty() const { return Count() != 0; }

 public:
  void Union(const RegisterSet&);
  void Exclude(const RegisterSet&);

 private:
  explicit RegisterSet(RegBitset&& b) : bitset_(std::move(b)) {}

  RegBitset bitset_;
};

// RegisterInfo is used to indicate all level of register soft/hard requirement
// for RA usage. Typically force is only been used for certain ARCH related
// stuff.
struct RegisterInfo {
  // Size class of this register info.
  SizeClass rsize;

  // Register type, used to mandate what register shoulbe used for RA
  RegisterType rtype;

  // forced register hint, ie set of register must be used, or polluted. In
  // most cases force set will just have one register, sometimes it will have
  // 2, example like x64 idiv will spill 2 registers (rax/rdx)
  const Register* force = nullptr;

  // hint means potentially, if applicable then allocate register inside of the
  // hint set, otherwise it is fine
  RegisterSet hint;

  void Convert(RegisterSet* out_hint, const Register** out_force) {
    out_hint->Union(hint);
    *out_force = force;
  }
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REGISTER_H_
