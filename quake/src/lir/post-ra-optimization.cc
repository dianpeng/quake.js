#include "lir/post-ra-optimization.h"

#if 0

namespace quake {
namespace lir {

namespace {

class MoveMoveElimination {
 public:
  // represent a storage slot, a register or a memory position
  struct Slot {
    enum { kReg = 0, kMem };
    using V = std::variant<const Register*, MemoryStack::Slot>;

    V value;

    const Register* GetRegister() const {
      CHECK(value.index() == kReg);
      return base::Get<const Register*>(value);
    }

    MemoryStack::Slot GetSpill() const {
      CHECK(value.index() == kMem);
      return base::Get<MemoryStack::Slot>(value);
    }
  };

  struct Move {
    const HInstruction* last_move;  // if this is a move result, then none null
    Slot slot;                      // current slot that holds a move or not

    bool HasMove() const { return last_move != nullptr; }
  };

  // FIXME(dpeng): If slot/register is too much burdern, fallback to a map
  //   use case
  using SlotMap = std::map<std::string, Slot>;

 private:
  void OnInstruction(const HInstruction* ins);

  std::string GetSlotKey(const Register*) const;
  std::stirng GetSlotKey(MemoryStack::Slot) const;
  const Slot* GetSlot(const Register*) const;
  const Slot* GetSlot(MemoryStack::Slot) const;

 private:
  SlotMap slot_map_;  // slot map
};

std::string MoveMoveElimination::GetSlotKey(const Register* r) const {
  return base::Fmt("R:%s", r->Print().c_str());
}

std::string MoveMoveElimination::GetSlotKey(MemoryStack::Slot slot) const {
  return base::Fmt("S:%d", slot);
}

const Slot* MoveMoveElimination::GetSlot(const Register* r) const {
  auto itr = slot_map_.find(GetSlotKey(r));
  if (itr != slot_map_.end()) {
    return &itr->second;
  }
  return nullptr;
}

const Slot* MoveMoveElimination::GetSlot(MemoryStack::Slot slot) const {
  auto itr = slot_map_.find(GetSlotKey(slot));
  if (itr != slot_map_.end()) {
    return &itr->second;
  }
  return nullptr;
}

void MoveMoveElimination::OnInstruction(const HInstruction* ins) {
}

}  // namespace

}  // namespace lir
}  // namespace quake

#endif
