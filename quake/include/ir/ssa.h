#ifndef _QUAKE_IR_SSA_H_
#define _QUAKE_IR_SSA_H_
#include <quake-api.h>
#include "base.h"
#include "zone.h"
#include "qjs.h"

#include "ir/bytecode.h"
#include "ir/opcode.h"

namespace quake {
class Quake;
class InterpInfo;

namespace ir {

class Node;
class Checkpoint;
class SSAVar;
class Builder;

struct SSAFlag {
 public:
  enum { kDead = base::BitMaskOn<std::uint32_t, 1>::kValue };

 public:
  void SetDead() { value_ &= kDead; }
  bool IsDead() const { return static_cast<bool>(value_ & kDead); }

 private:
  std::uint32_t value_ = 0;
};

// Bytecode info
struct BytecodeInfo {
  Bytecode opcode = Bytecode::OP_invalid;
  const void* address = nullptr;
};

// Machine code information, used for debugging purpose
struct MCodeInfo {
  const void* start = nullptr;
  const void* end = nullptr;
};

constexpr std::uint32_t kInternalIndex =
    std::numeric_limits<std::uint32_t>::max();

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_SSA_H_
