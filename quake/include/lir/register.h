#ifndef _QUAKE_LIR_REGISTER_H_
#define _QUAKE_LIR_REGISTER_H_
#include "base.h"

#ifdef QUAKE_TARGET_X64
#include "cg/x64/register.h"
#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64

namespace quake {
namespace lir {

#ifdef QUAKE_TARGET_X64
using cg::x64::kRegisterCount;
using cg::x64::MachineRegister;
#else
#error "error"
#endif  // QUAKE_TARGET_X64

class Register {
 public:
  explicit Register(const MachineRegister* mreg) : mreg_(mreg) {}
  std::uint32_t kind_id() const {
    return static_cast<std::uint32_t>(mreg_->kind);
  }

 private:
  const MachineRegister* mreg_;
};

using RegBitset = std::bitset<kRegisterCount>;

class RegisterSet {
 public:
  static inline RegisterSet Exclude(const Register*);
  static inline RegisterSet Exclude(const Register*, const Register*);
  static inline RegisterSet Exclude(const Register*, const Register*,
                                    const Register*);

  static inline RegisterSet Include(const Register*);
  static inline RegisterSet Include(const Register*, const Register*);
  static inline RegisterSet Incldue(const Register*, const Register*,
                                    const Register*);

 public:
  // --------------------------------------------------------------------------
  // Arch related
  //
  // All the allowed register, fp and gp
  static inline RegisterSet All();
  static inline RegisterSet AllFp();
  static inline RegisterSet AllGp();

  // All the allowed register that can be allocated. fp and gp
  static inline RegisterSet AllAllocate();
  static inline RegisterSet AllFpAllocate();
  static inline RegisterSet AllGpAllocate();

 public:
  bool Get(std::uint32_t id) const { return bitset_.test(id); }
  bool Get(const Register* r) const { return Get(r->kind_id()); }

  void Set(std::uint32_t id) { bitset_.set(id, true); }
  void Set(const Register* r) { Set(r->kind_id()); }

  void Unset(std::uint32_t id) { bitset_.set(id, false); }
  void Unset(const Register* r) { Unset(r->kind_id()); }

  std::size_t Count() const { return bitset_.count(); }

 public:
  RegisterSet Union(const RegisterSet&);
  void UnionWith(const RegisterSet&);

 private:
  RegBitset bitset_;
};

struct RegisterInfo {
  // Register type, used to mandate what register shoulbe used for RA
  RegisterType rtype{};

  // forced register hint, ie set of register must be used, or polluted. In
  // most cases force set will just have one register, sometimes it will have
  // 2, example like x64 idiv will spill 2 registers (rax/rdx)
  const Register* force = nullptr;

  // hint means potentially, if applicable then allocate register inside of the
  // hint set, otherwise it is fine
  RegisterSet hint{};
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_REGISTER_H_
