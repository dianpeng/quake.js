#include "ir/interp-info.h"
#include "base.h"
#include "qjs.h"

extern "C" {
#include <cutils.h>  // QuickJS's source code
}

namespace quake {
namespace ir {

InterpInfoQJS::~InterpInfoQJS() {}

std::uint32_t InterpInfoQJS::GetU32() {
  auto v = ::get_u32(pc_);
  pc_ += 4;
  return v;
}

std::int32_t InterpInfoQJS::GetI32() {
  auto v = (int32_t)::get_u32(pc_);
  pc_ += 4;
  return v;
}

std::uint16_t InterpInfoQJS::GetU16() {
  auto v = ::get_u16(pc_);
  pc_ += 2;
  return v;
}

std::uint8_t InterpInfoQJS::GetU8() {
  auto v = ::get_u8(pc_);
  pc_ += 1;
  return v;
}

std::uint32_t InterpInfo::PeekU32() {
  return ::get_u32(static_cast<const std::uint8_t*>(interp_pc()));
}

std::int32_t InterpInfo::PeekI32() {
  return (int32_t)::get_u32(static_cast<const std::uint8_t*>(interp_pc()));
}

std::uint16_t InterpInfo::PeekU16() {
  return ::get_u16(static_cast<const std::uint8_t*>(interp_pc()));
}

std::uint8_t InterpInfo::PeekU8() {
  return ::get_u8(static_cast<const std::uint8_t*>(interp_pc()));
}

}  // namespace ir
}  // namespace quake
