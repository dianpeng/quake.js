#include "cg/x64/asm-util.h"
#include "common.h"
#include "lir/operand.h"

namespace quake {
namespace cg {
namespace x64 {

using namespace quake::lir;

int MapRKind2ARegGP(RegisterKind rk) {
#define MAP(FROM, TO)      \
  case RegisterKind::FROM: \
    return asmjit::x86::Gp::TO

  switch (rk) {
    MAP(kRax, kIdAx);
    MAP(kRbx, kIdBx);
    MAP(kRcx, kIdCx);
    MAP(kRdx, kIdDx);
    MAP(kRbp, kIdBp);
    MAP(kRsp, kIdSp);
    MAP(kRdi, kIdDi);
    MAP(kRsi, kIdSi);

    MAP(kR8, kIdR8);
    MAP(kR9, kIdR9);
    MAP(kR10, kIdR10);
    MAP(kR11, kIdR11);
    MAP(kR12, kIdR12);
    MAP(kR13, kIdR13);
    MAP(kR14, kIdR14);
    MAP(kR15, kIdR15);
    default:
      UNREACHABLE();
      return -1;
  }

#undef MAP  // MAP
}

int MapRKind2ARegFP(RegisterKind rk) {
#define MAP(FROM, TO)      \
  case RegisterKind::FROM: \
    return TO
  switch (rk) {
    MAP(kXmm0, 0);
    MAP(kXmm1, 1);
    MAP(kXmm2, 2);
    MAP(kXmm3, 3);
    MAP(kXmm4, 4);
    MAP(kXmm5, 5);
    MAP(kXmm6, 6);
    MAP(kXmm7, 7);
    MAP(kXmm8, 8);
    MAP(kXmm9, 9);
    MAP(kXmm10, 10);
    MAP(kXmm11, 11);
    MAP(kXmm12, 12);
    MAP(kXmm13, 13);
    MAP(kXmm14, 14);
    MAP(kXmm15, 15);

    default:
      UNREACHABLE();
      return -1;
  }
#undef MAP  // MAP
}

asmjit::Imm ImmOperandToASMJIT(const HOperand* operand) {
  CHECK(operand->IsImm());

  const auto& imm = operand->GetImm();
  switch (imm.type()) {
    case Immediate::kInt32:
      return asmjit::Imm(imm.Int32());
    case Immediate::kInt64:
    case Immediate::kFloat64:
      UNREACHABLE() << "this code path cannot materialize qword";
      break;
    default:
      UNREACHABLE();
      break;
  }
  return asmjit::Imm{};
}

asmjit::x86::Gp GPRegOperandToASMJIT(const HOperand* operand) {
  CHECK(operand->IsRegister());
  auto reg = operand->GetRegister();
  auto size_class = reg->size_class();

  auto reg_id = MapRKind2ARegGP(reg->register_kind());

  switch (size_class) {
    case SizeClass::kByte:
      return asmjit::x86::gpb(reg_id);
    case SizeClass::kWord:
      return asmjit::x86::gpw(reg_id);
    case SizeClass::kDWord:
      return asmjit::x86::gpd(reg_id);
    case SizeClass::kQWord:
      return asmjit::x86::gpq(reg_id);
    default:
      UNREACHABLE() << "invalid register storage size";
      return asmjit::x86::gpq(-1);
  }
}

asmjit::x86::Xmm FPRegOperandToASMJIT(const HOperand* operand) {
  CHECK(operand->IsRegister());
  auto reg = operand->GetRegister();
  auto reg_id = MapRKind2ARegFP(reg->register_kind());
  return asmjit::x86::xmm(reg_id);
}

asmjit::x86::Mem MemIndOperandToASMJIT(const HOperand* operand) {
  CHECK(operand->IsMemInd());
  const auto& mem_ind = operand->GetMemInd();

  auto base = mem_ind.base();
  auto offset = mem_ind.offset();
  auto scale = static_cast<std::uint32_t>(mem_ind.scale());
  auto disp = mem_ind.displacement();

  CHECK(base);

  if (offset == nullptr) {
    auto areg_base = GPRegOperandToASMJIT(base);
    return asmjit::x86::ptr(areg_base, disp);
  } else {
    auto areg_base = GPRegOperandToASMJIT(base);
    auto areg_offset = GPRegOperandToASMJIT(offset);
    return asmjit::x86::ptr(areg_base, areg_offset, scale, disp);
  }
}

asmjit::x86::Mem MemStackOperandToASMJIT(const HOperand* operand) {
  const auto& stack = operand->GetMemStack();

  // notes on x64, the rbp - N is the spilled slot, the stack is growing
  // downwards instead of upwards
  auto memptr = asmjit::x86::ptr(asmjit::x86::rbp, -stack.SlotOffset());
  memptr.setSize(operand->SizeInBits());
  return memptr;
}

asmjit::x86::Mem MemStaticOperandToASMJIT(const HOperand* operand) {
  const auto& smem = operand->GetMemStatic();
  auto memptr =
      asmjit::x86::ptr(reinterpret_cast<std::uint64_t>(smem.address()));
  memptr.setSize(operand->SizeInBits());
  return memptr;
}

asmjit::x86::Mem MemoryOperandToASMJIT(const HOperand* operand) {
  if (operand->IsMemStack()) {
    return MemStackOperandToASMJIT(operand);
  } else if (operand->IsMemStatic()) {
    return MemStaticOperandToASMJIT(operand);
  } else {
    CHECK(operand->IsMemInd());
    return MemIndOperandToASMJIT(operand);
  }
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
