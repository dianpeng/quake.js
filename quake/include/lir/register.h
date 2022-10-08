#ifndef _QUAKE_LIR_REGISTER_H_
#define _QUAKE_LIR_REGISTER_H_
#include "base.h"

#ifdef QUAKE_TARGET_X64

#include "cg/x64/register.h"
#include "cg/x64/abi.h"
#include "cg/x64/ra-hint.h"

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

using cg::x64::GetRAHint;
using cg::x64::RegisterIdToKind;
using cg::x64::RegisterKindIsFP;
using cg::x64::RegisterKindIsGP;
using cg::x64::RegisterKindToId;

using cg::x64::GetFreeFPRegister;
using cg::x64::GetFreeGPRegister;

// ABI related ================================================================
using cg::x64::GetABICalleeSavedFP;
using cg::x64::GetABICalleeSavedGP;
using cg::x64::GetABICallerSavedFP;
using cg::x64::GetABICallerSavedGP;
using cg::x64::GetABIFPArg;
using cg::x64::GetABIGPArg;

#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64

class Register {
 public:
  static const Register* GetRegisterFromKind(RegisterKind);
  static const Register* GetRegisterFromId(RegisterId);
  static const Register* GetRegisterFromKindId(std::uint32_t);

 public:
  explicit Register(const MachineRegister* mreg) : mreg_(mreg) {}

  RegisterKind register_kind() const { return mreg_->kind; }
  RegisterId register_id() const { return mreg_->id; }
  RegisterType register_type() const { return mreg_->type; }
  bool IsFP() const { return mreg_->type == RegisterType::kFloat; }
  bool IsGP() const { return mreg_->type == RegisterType::kGeneral; }
  SizeClass size_class() const { return mreg_->size_class; }
  const std::string& name() const { return mreg_->name; }

  bool IsQWord() const { return mreg_->size_class == SizeClass::kQWord; }
  bool IsDWord() const { return mreg_->size_class == SizeClass::kDWord; }
  bool IsWord() const { return mreg_->size_class == SizeClass::kWord; }
  bool IsByte() const { return mreg_->size_class == SizeClass::kByte; }

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
  static RegisterSet GetCallerSavedGPRegister();
  static RegisterSet GetCallerSavedFPRegister();
  static RegisterSet GetCallerSavedRegister();

  static RegisterSet GetCalleeSavedGPRegister();
  static RegisterSet GetCalleeSavedFPRegister();
  static RegisterSet GetCalleeSavedRegister();

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

 public:
  template <typename F>
  void ForeachSet(F&&);

  template <typename F>
  void ForeachUnset(F&&);

  template <typename F>
  void Foreach(F&&);

 private:
  explicit RegisterSet(RegBitset&& b) : bitset_(std::move(b)) {}

  RegBitset bitset_;
};

enum class RegisterHintPolicy : std::uint8_t { kNone, kInput0AsOutput };

// ----------------------------------------------------------------------------
// RegisterInfo is a key structure used to represent information needed by RA
// to resolve certain constraints of instruction register allocation at
// different stages. The RA information is formulated via different stages:
//
// 1) LIR stage, normally coming from the result of HRegisterInfo
//
// 2) CG stage, which is encoded by different arch and been setup during the
//    RA phase. This will provide information like idiv will pollute rdx
//    register
// ----------------------------------------------------------------------------

struct RegisterInfo {
  SizeClass rsize;     // size class of register itself
  RegisterType rtype;  // type of the register

  // Forced register, ie the current instruction must be output to the assigned
  // register
  const Register* force = nullptr;

  // Force register, ie current instruction must be output to a register, not
  // spilled. Notes it can be spilled when not used. But must materialized back
  // to where it spilled
  bool force_register = false;

  // Hintted register set, ie output should try to use the suggested register
  // set
  RegisterSet hint;

  // The instruction has side effect to pollute certain list of registers, the
  // register must be reserved before the instruction and can be used afterwards
  RegisterSet reserve;

  // Hint policy, ie used hint other types of register usage which cannot be
  // easily expressed via hint register set
  RegisterHintPolicy hint_policy = RegisterHintPolicy::kNone;

 public:
  void Convert(RegisterSet* out_hint, RegisterSet* out_reserve,
               const Register** out_force) {
    out_hint->Union(hint);
    out_reserve->Union(reserve);
    *out_force = force;
  }
};

template <typename F>
void RegisterSet::ForeachSet(F&& f) {
  for (std::size_t i = 0; i < bitset_.size(); i++) {
    if (bitset_[i]) f(static_cast<std::uint32_t>(i));
  }
}

template <typename F>
void RegisterSet::ForeachUnset(F&& f) {
  for (std::size_t i = 0; i < bitset_.size(); i++) {
    if (!bitset_[i]) f(static_cast<std::uint32_t>(i));
  }
}

template <typename F>
void RegisterSet::Foreach(F&& f) {
  for (std::size_t i = 0; i < bitset_.size(); i++) {
    f(static_cast<std::uint32_t>(i), bitset_[i]);
  }
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REGISTER_H_
