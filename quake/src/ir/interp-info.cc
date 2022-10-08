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

}  // namespace ir
}  // namespace quake
