#include "lir/opcode.h"

namespace quake {
namespace lir {

namespace {

#define __(ENUM, NAME, F, RI, OI) \
  static const HOpcodeInfo ENUM##Info{HOpcode::ENUM, NAME, F, RI, OI};

QUAKE_HOPCODE_LIST(__)

#undef __  // __

static const HOpcodeInfo kNull{};

}  // namespace

const HOpcodeInfo& GetHOpcodeInfo(HOpcode opcode) {
  switch (opcode) {
#define __(ENUM, ...) \
  case HOpcode::ENUM: \
    return ENUM##Info;
    QUAKE_HOPCODE_LIST(__)
#undef __  // __
    default:
      UNREACHABLE();
      return kNull;
  }
}

}  // namespace lir
}  // namespace quake

