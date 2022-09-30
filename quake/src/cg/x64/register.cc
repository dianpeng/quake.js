#include "cg/x64/register.h"

#ifdef QUAKE_TARGET_X64

namespace quake {
namespace cg {
namespace x64 {

namespace {

using AllRegisterList = std::vector<MachineRegister>;

struct AllRegister {
  AllRegister() {
#define __(ID, NAME, BSIZE)                                           \
  list.emplace_back(RegisterIdToKind(RegisterId::ID), RegisterId::ID, \
                    RegisterIdToType(RegisterId::ID),                 \
                    base::Unwrap(ByteSizeToSizeClass(BSIZE)), NAME);

    QUAKE_X64_REGISTER_ID_LIST(__)

#undef __  // __
  }

  AllRegisterList list;

 public:
  static AllRegisterList& Get() {
    static AllRegister kInstance;
    return kInstance.list;
  }

  static const MachineRegister* GetMachineRegister(RegisterId id) {
    auto idx = static_cast<std::size_t>(id);
    return &Get()[idx];
  }
};

// clang-format off
RegisterId Kind2Id(RegisterKind kind) {
  switch (kind) {
#define __(A, ...) case RegisterKind::A: return RegisterId::A;
    QUAKE_X64_REGISTER_KIND_LIST(__)
#undef __  // __
    default:
      UNREACHABLE();
      return RegisterId::kRax;
  }
}
// clang-format on

}  // namespace

bool MachineRegister::IsGP() const { return type == RegisterType::kGeneral; }

bool MachineRegister::IsFP() const { return type == RegisterType::kFloat; }

bool MachineRegister::Alias(const MachineRegister& mr) const {
  return RegisterKindContain(kind, mr.id);
}

bool MachineRegister::Match(SizeClass sc) const { return sc == size_class; }

bool MachineRegister::IsFullSize() const {
  return size_class == SizeClass::kQWord;
}

RegisterKind RegisterIdToKind(RegisterId id) {
#define BETWEEN(ID, L, R) \
  if ((ID) >= RegisterId::L && (ID) <= RegisterId::R) return RegisterKind::L;

#define IS(ID, X) \
  if ((ID) == RegisterId::X) return RegisterKind::X;

  BETWEEN(id, kRax, kAl);
  BETWEEN(id, kRbx, kBl);
  BETWEEN(id, kRcx, kCl);
  BETWEEN(id, kRdx, kDl);
  BETWEEN(id, kRsp, kSpl);
  BETWEEN(id, kRbp, kBpl);
  BETWEEN(id, kRsi, kSil);
  BETWEEN(id, kRdi, kDil);
  BETWEEN(id, kR8, kR8l);
  BETWEEN(id, kR9, kR9l);
  BETWEEN(id, kR10, kR10l);
  BETWEEN(id, kR11, kR11l);
  BETWEEN(id, kR12, kR12l);
  BETWEEN(id, kR13, kR13l);
  BETWEEN(id, kR14, kR14l);
  BETWEEN(id, kR15, kR15l);

  IS(id, kXmm0);
  IS(id, kXmm1);
  IS(id, kXmm2);
  IS(id, kXmm3);
  IS(id, kXmm4);
  IS(id, kXmm5);
  IS(id, kXmm6);
  IS(id, kXmm7);
  IS(id, kXmm8);
  IS(id, kXmm9);
  IS(id, kXmm10);
  IS(id, kXmm11);
  IS(id, kXmm12);
  IS(id, kXmm13);
  IS(id, kXmm14);
  IS(id, kXmm15);

  UNREACHABLE();
  return RegisterKind::kRax;

#undef BETWEEN  // BETWEEN
#undef IS       // IS
}

bool RegisterKindContain(RegisterKind kind, RegisterId id) {
  return kind == RegisterIdToKind(id);
}

RegisterType RegisterKindToType(RegisterId id) {
  if (auto kind = RegisterIdToKind(id); kind >= RegisterKind::kXmm0) {
    return RegisterType::kFloat;
  } else {
    return RegisterType::kGeneral;
  }
}

RegisterId RegisterKindToId(RegisterKind kind) { return Kind2Id(kind); }

std::optional<RegisterId> RegisterSizeClassToSizeId(RegisterKind kind,
                                                    SizeClass sc) {
  auto id = Kind2Id(kind);

  if (kind >= RegisterKind::kXmm0) {
    if (sc == SizeClass::kQWord) {
      return id;
    } else {
      return {};
    }
  } else {
    std::uint32_t offset = 0;
    switch (sc) {
      case SizeClass::kQWord:
        offset = 0;
        break;
      case SizeClass::kDWord:
        offset = 1;
        break;
      case SizeClass::kWord:
        offset = 2;
        break;
      case SizeClass::kByte:
        offset = 3;
        break;
      default:
        return {};
    }

    return static_cast<RegisterId>(static_cast<std::uint32_t>(id) + offset);
  }
}

bool RegisterKindIsFP(RegisterKind kind) { return kind >= RegisterKind::kXmm0; }

bool RegisterKindIsGP(RegisterKind kind) { return kind < RegisterKind::kXmm0; }

const MachineRegister* GetRegisterFromId(RegisterId id) {
  return AllRegister::GetMachineRegister(id);
}

const MachineRegister* GetRegisterFromKind(RegisterKind kind) {
  return AllRegister::GetMachineRegister(Kind2Id(kind));
}

const MachineRegister* GetRegisterSubClass(RegisterKind kind,
                                           SizeClass size_class) {
  if (auto maybe_id = RegisterSizeClassToId(kind, size_class); maybe_id) {
    return AllRegister::GetMachineRegister(*maybe_id);
  } else {
    return nullptr;
  }
}

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // QUAKE_TARGET_X64

