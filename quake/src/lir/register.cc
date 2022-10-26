#include <lir/register.h>

namespace quake {
namespace lir {
namespace {

struct AllRegister {
  std::vector<Register> list;
  AllRegister() {
#define __(A, ...) list.emplace_back(GetRegisterFromId(RegisterId::A));
    QUAKE_FOREACH_REGISTER_ID(__)
#undef __  // __
  }

  static const std::vector<Register>& Get() {
    static AllRegister kInstance;
    return kInstance.list;
  }

  static const Register* GetRegister(RegisterId id) {
    auto idx = static_cast<std::uint32_t>(id);
    return &(AllRegister::Get()[idx]);
  }

  static const Register* GetRegister(RegisterKind kind) {
    return GetRegister(RegisterKindToId(kind));
  }

  static const Register* GetRegister(RegisterKind kind, SizeClass size_class) {
    if (auto maybe = RegisterSizeClassToId(kind, size_class); maybe) {
      auto id = *maybe;
      return GetRegister(id);
    } else {
      return nullptr;
    }
  }
};

}  // namespace

#ifdef QUAKE_TARGET_X64

const Register* kContextRegister = AllRegister::GetRegister(kContextRegisterId);
const Register* kScratchRegister = AllRegister::GetRegister(kScratchRegisterId);

#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64

const Register* Register::GetRegisterFromKindId(std::uint32_t id) {
  return AllRegister::GetRegister(static_cast<RegisterKind>(id));
}

const Register* Register::GetRegisterFromId(RegisterId id) {
  return AllRegister::GetRegister(id);
}

const Register* Register::GetRegisterFromKind(RegisterKind kind) {
  return AllRegister::GetRegister(kind);
}

const Register* Register::GetRegisterFromKindAndSizeClass(
    RegisterKind kind, SizeClass size_class) {
  return AllRegister::GetRegister(kind, size_class);
}

void RegisterSet::Union(const RegisterSet& rs) {
  const auto len = rs.bitset_.size();
  for (std::size_t i = 0; i < len; i++) {
    if (rs.bitset_[i]) {
      bitset_.set(i);
    }
  }
}

void RegisterSet::Exclude(const RegisterSet& rs) {
  const auto len = rs.bitset_.size();
  for (std::size_t i = 0; i < len; i++) {
    if (rs.bitset_[i]) {
      bitset_.set(i, false);
    }
  }
}

namespace {

RegisterSet Convert(const std::vector<RegisterKind>& kind_set) {
  RegisterSet output;
  for (const auto& x : kind_set) {
    output.Set(static_cast<std::uint32_t>(x));
  }
  return output;
}

}  // namespace

RegisterSet RegisterSet::GetCallerSavedGPRegister() {
  return Convert(GetABICallerSavedGP());
}

RegisterSet RegisterSet::GetCalleeSavedGPRegister() {
  return Convert(GetABICalleeSavedGP());
}

RegisterSet RegisterSet::GetCallerSavedFPRegister() {
  return Convert(GetABICallerSavedFP());
}

RegisterSet RegisterSet::GetCalleeSavedFPRegister() {
  return Convert(GetABICalleeSavedFP());
}

RegisterSet RegisterSet::GetCallerSavedRegister() {
  auto o1 = GetCallerSavedGPRegister();
  o1.Union(GetCallerSavedFPRegister());
  return o1;
}

RegisterSet RegisterSet::GetCalleeSavedRegister() {
  auto o1 = GetCalleeSavedGPRegister();
  o1.Union(GetCalleeSavedFPRegister());
  return o1;
}

}  // namespace lir
}  // namespace quake
