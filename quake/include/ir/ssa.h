#ifndef _QUAKE_IR_SSA_H_
#define _QUAKE_IR_SSA_H_
#include "quake-C.h"
#include "base.h"
#include "zone.h"
#include "qjs.h"
#include "defuse-list.h"

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
  enum {
    // Whehter the node is dead or not
    kDead = base::BitOn<std::uint16_t, 1>::kValue,

    // Whether the node is been used as jump target, ie label
    kLabel = base::BitOn<std::uint16_t, 2>::kValue,

    // Used during jit trace linking, indicate that we should reset the
    // stack slot during reemit phase of certain instructions
    kResetStackSlot = base::BitOn<std::uint16_t, 3>::kValue
  };

 public:
  void SetDead() { value_ |= kDead; }
  void ClearDead() { value_ &= ~kDead; }
  bool IsDead() const { return static_cast<bool>(value_ & kDead); }

  void SetLabel() { value_ |= kLabel; }
  void ClearLabel() { value_ &= ~kLabel; }
  bool IsLabel() const { return static_cast<bool>(value_ & kLabel); }

  void SetResetStackSlot() { value_ |= kResetStackSlot; }
  void ClearStackSlot() { value_ &= ~kResetStackSlot; }
  bool IsResetStackSlot() const {
    return static_cast<bool>(value_ & kResetStackSlot);
  }

 public:
  std::uint16_t GetExtraU16() const { return extra_.u16; }
  std::uint32_t GetExtraU32() const { return extra_.u32; }
  void* GetExtraPtr() const { return extra_.ptr; }

  void SetExtra(std::uint16_t x) { extra_.u16 = x; }
  void SetExtra(std::uint32_t x) { extra_.u32 = x; }
  void SetExtra(void* x) { extra_.ptr = x; }

 private:
  std::uint16_t value_ = 0;
  union {
    void* ptr;
    std::uint16_t u16;
    std::uint32_t u32;
  } extra_;
};

// Bytecode info
struct BytecodeInfo {
  Bytecode opcode = Bytecode::OP_invalid;
  const void* current = nullptr;
  const void* next = nullptr;
};

// Machine code information, used for debugging purpose
struct MCodeInfo {
  const void* start = nullptr;
  const void* end = nullptr;
};

// A special index to represent the node is an internal node. Internal node is
// not visiable from the normal control flow dependency, but only via node's
// data dependency (operand);
constexpr std::uint32_t kInternalIndex =
    std::numeric_limits<std::uint32_t>::max();

// A stub-like base for def-use chain indication. Used by Node and Checkpoint
// to track all its internal use of each node via SSAVar
class NodeBase {
 public:
  enum { kNode, kCheckpoint };

  virtual ~NodeBase() {}
  virtual int node_type() const = 0;
};

template <int kType>
class NodeBaseImpl : public NodeBase {
 public:
  int node_type() const override { return kType; }
  ~NodeBaseImpl() override {}
};

using SSARef = DefUseRef<NodeBase, SSAVar>;
using SSARefList = DefUseRefList<NodeBase, SSAVar>;

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_SSA_H_
