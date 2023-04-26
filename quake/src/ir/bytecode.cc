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

std::string QJSBytecodeInfo::Print() const {
  return base::Fmt("%s[%d,%d,%d]", name.c_str(), size, n_pop, n_push);
}

std::string BytecodePrint(const void* pc) {
  const auto parameter = static_cast<const std::uint8_t*>(pc) + 1;
  auto bytecode = *static_cast<const Bytecode*>(pc);
  const auto& info = GetQJSBytecodeInfo(bytecode);
  std::stringstream fmt;

  fmt << info.Print() << " ";

  auto label_addr = [&](std::int32_t offset) -> const void* {
    return static_cast<const void*>(parameter + offset);
  };

  switch (info.format) {
    case BytecodeFormat::kFmt_none:
      break;

    case BytecodeFormat::kFmt_loc:
      fmt << "1@loc(" << get_u16(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_arg:
      fmt << "1@arg(" << get_u16(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_var_ref:
      fmt << "1@vref(" << get_u16(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_u8:
      fmt << "1@" << get_u8(parameter);
      break;

    case BytecodeFormat::kFmt_i8:
      fmt << "1@" << get_i8(parameter);
      break;

    case BytecodeFormat::kFmt_u16:
      fmt << "1@" << get_u16(parameter);
      break;

    case BytecodeFormat::kFmt_i16:
      fmt << "1@" << get_i16(parameter);
      break;

    case BytecodeFormat::kFmt_u32:
      fmt << "1@" << get_u32(parameter);
      break;

    case BytecodeFormat::kFmt_i32:
      fmt << "1@" << get_i32(parameter);
      break;

    case BytecodeFormat::kFmt_label8:
      fmt << "1@label(" << label_addr(get_i8(parameter)) << ")";
      break;

    case BytecodeFormat::kFmt_label16:
      fmt << "1@label(" << label_addr(get_i16(parameter)) << ")";
      break;

    case BytecodeFormat::kFmt_label:
      fmt << "1@label(" << label_addr(get_i32(parameter)) << ")";
      break;

    case BytecodeFormat::kFmt_loc8:
      fmt << "1@loc(" << get_u8(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_const8:
      fmt << "1@const(" << get_u8(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_const:
      fmt << "1@const(" << get_u32(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_npop:
      fmt << "1@npop(" << get_u16(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_npop_u16:
      fmt << "1@npop(" << get_u16(parameter) << ") 2@"
          << get_u16(parameter + 2);
      break;

    case BytecodeFormat::kFmt_atom:
      fmt << "1@atom(" << get_u32(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_atom_u8:
      fmt << "1@atom_u8(" << get_u32(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_atom_u16:
      fmt << "1@atom_u16(" << get_u32(parameter) << ")";
      break;

    case BytecodeFormat::kFmt_atom_label_u8:
      fmt << "1@atom_u8(" << get_u32(parameter) << ") 2@"
          << get_u32(parameter + 4) << " 3@" << get_u8(parameter + 8);
      break;

    case BytecodeFormat::kFmt_atom_label_u16:
      fmt << "1@atom_u8(" << get_u32(parameter) << ") 2@"
          << get_u32(parameter + 4) << " 3@" << get_u16(parameter + 8);
      break;

    default:
      UNREACHABLE() << "should never showup: " << info.name;
      break;
  }

  return fmt.str();
}

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

std::string BytecodeIterator::Print() const {
  if (Has()) {
    return BytecodePrint(current());
  } else {
    return "[eof]";
  }
}

}  // namespace ir
}  // namespace quake
