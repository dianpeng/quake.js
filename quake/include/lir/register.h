#ifndef _QUAKE_LIR_REGISTER_H_
#define _QUAKE_LIR_REGISTER_H_
#include "base.h"

#ifdef QUAKE_TARGET_X64

#include "cg/abi.h"
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
  static const Register* GetRegisterFromKindAndSizeClass(RegisterKind,
                                                         SizeClass);

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

  std::string Print() const { return mreg_->Print(); }

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
  explicit RegisterSet(RegisterKind rk) : bitset_() {
    Set(static_cast<std::uint32_t>(rk));
  }

  bool Get(std::uint32_t id) const { return bitset_.test(id); }
  bool Get(const Register* r) const { return Get(r->kind_id()); }
  bool Get(RegisterKind kid) const {
    return Get(static_cast<std::uint32_t>(kid));
  }

  void Set(std::uint32_t id) { bitset_.set(id, true); }
  void Set(const Register* r) { Set(r->kind_id()); }
  void Set(RegisterKind kid) { Set(static_cast<std::uint32_t>(kid)); }

  void Unset(std::uint32_t id) { bitset_.set(id, false); }
  void Unset(const Register* r) { Unset(r->kind_id()); }
  void Unset(RegisterKind kid) { Unset(static_cast<std::uint32_t>(kid)); }

  void Clear() { bitset_.reset(); }
  std::size_t Count() const { return bitset_.count(); }
  std::size_t Size() const { return bitset_.size(); }
  bool Empty() const { return bitset_.none(); }

  bool IsDisjoint(const RegisterSet& rset) const {
    return (bitset_ & rset.bitset_).none();
  }
  bool IsNoneEmptyAndDisjoint(const RegisterSet& rset) const {
    if (Empty() && rset.Empty()) return true;
    return IsDisjoint(rset);
  }

 public:
  void Union(const RegisterSet&);
  void Exclude(const RegisterSet&);

  std::string Print() const;

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

using NativeCallCallerSaveGPRegisterVector =
    base::static_vector<const Register*, cg::kNativeCallCallerSaveGPSize>;

using NativeCallCallerSaveFPRegisterVector =
    base::static_vector<const Register*, cg::kNativeCallCallerSaveFPSize>;

enum class RegisterHintPolicy : std::uint8_t { kNone, kInput0AsOutput };

// Really should not be here, used to represent instruction's operand
// constraints for example, x64's require's both operand cannot be memory, and
// memory must be at second operand etc ...
//
// The way is not that flexible, we just hardcode some policy or strategy here
// and let the RA to resolve them
enum class OperandConstraint : std::uint8_t {
  kNone

#ifdef QUAKE_TARGET_X64
  ,

  /**
   * x64 binary operation general requirement(constraint)
   * 1) memory operand can only show up in second position
   * 2) cannot have 2 memory operands
   *    notes, this constraint is overlapped with *1*
   */
  kX64BinaryOp

#endif  // QUAKE_TARGET_X64
};

// Used to indicate ABI constraint. Notes we the ABI constraint is been used to
// help the RA to decide how to arrange ABI

struct RegisterAllocationRequirement {
  enum {
    kNone,  // no requirement
    kForceAnyRegister,
    kForceRegister,
    kForceSpill
  };

  int type = kNone;

  const Register* force_register = nullptr;

  RegisterAllocationRequirement() = default;

  RegisterAllocationRequirement(int ft, const Register* r)
      : type(ft), force_register(r) {}

  static RegisterAllocationRequirement None() {
    return RegisterAllocationRequirement{};
  }
  static RegisterAllocationRequirement ForceRegister(const Register* r) {
    return RegisterAllocationRequirement{kForceRegister, r};
  }
  static RegisterAllocationRequirement ForceAnyRegister() {
    return RegisterAllocationRequirement{kForceAnyRegister, nullptr};
  }
  static RegisterAllocationRequirement ForceSpill() {
    return RegisterAllocationRequirement{kForceSpill, nullptr};
  }
  bool IsNone() const { return type == kNone; }
  bool IsForceAnyRegister() const { return type == kForceAnyRegister; }
  bool IsForceRegister() const { return type == kForceRegister; }
  bool IsForceSpill() const { return type == kForceSpill; }
};

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

  // --------------------------------------------------------------------------
  // Operand requirement
  OperandConstraint opr_constraint = OperandConstraint::kNone;

  NativeCallCallerSaveGPRegisterVector opr_abi_reserve_gp;

  NativeCallCallerSaveFPRegisterVector opr_abi_reserve_fp;

  // --------------------------------------------------------------------------
  // instruction output RA requirement
  // Forced register, ie the current instruction must be output to the assigned
  // register
  RegisterAllocationRequirement requirement =
      RegisterAllocationRequirement::None();

  // Hintted register set, ie output should try to use the suggested register
  // set
  RegisterSet hint;

  // Hint policy, ie used hint other types of register usage which cannot be
  // easily expressed via hint register set
  RegisterHintPolicy hint_policy = RegisterHintPolicy::kNone;

  // --------------------------------------------------------------------------
  // Additional requirement
  //
  // The instruction has side effect to pollute certain list of registers, the
  // register must be reserved before the instruction and can be used afterwards
  RegisterSet reserve;

  bool IsFP() const { return rtype == RegisterType::kFloat; }
  bool IsGP() const { return rtype == RegisterType::kGeneral; }
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
