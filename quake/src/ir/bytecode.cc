#include "ir/bytecode.h"

extern "C" {
#include <cutils.h>
}

namespace quake {
namespace ir {

namespace {

#define DEF(OP, SIZE, NPOP, NPUSH, BFMT) \
  QJSBytecodeInfo kOP##OP{               \
      #OP, Bytecode::OP_##OP, BytecodeFormat::kFmt_##BFMT, SIZE, NPOP, NPUSH};
#include <quickjs-opcode.h>
#undef DEF  // DEF

QJSBytecodeInfo kNull{};

}  // namespace

// clang-format off
const QJSBytecodeInfo& GetQJSBytecodeInfo(Bytecode bc) {
  switch (bc) {
#define DEF(OP, ...)      \
  case Bytecode::OP_##OP: \
    return kOP##OP;

#include <quickjs-opcode.h>

#undef DEF  // DEF

  default:
    UNREACHABLE();
    return kNull;

  }
}
// clang-format on

std::uint32_t BytecodeIterator::GetU32(std::size_t off) const {
  return ::get_u32(static_cast<const std::uint8_t*>(NextPC(off)));
}

std::int32_t BytecodeIterator::GetI32(std::size_t off) const {
  return (std::int32_t)::get_u32(static_cast<const std::uint8_t*>(NextPC(off)));
}

std::uint16_t BytecodeIterator::GetU16(std::size_t off) const {
  return ::get_u16(static_cast<const std::uint8_t*>(NextPC(off)));
}

std::uint8_t BytecodeIterator::GetU8(std::size_t off) const {
  return ::get_u8(static_cast<const std::uint8_t*>(NextPC(off)));
}

const void* BytecodeNext(const void* current_bc) {
  auto bc = *static_cast<const Bytecode*>(current_bc);
  const auto& info = GetQJSBytecodeInfo(bc);
  return static_cast<const char*>(current_bc) + info.size;
}

}  // namespace ir
}  // namespace quake
