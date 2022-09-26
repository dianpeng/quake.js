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
};

}  // namespace

#ifdef QUAKE_TARGET_X64

const Register* kContextRegister = AllRegister::GetRegister(kContextRegisterId);
const Register* kScratchRegister = AllRegister::GetRegister(kScratchRegisterId);

#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64

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

}  // namespace lir
}  // namespace quake
