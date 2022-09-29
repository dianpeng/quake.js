#ifndef _QUAKE_LIR_MEMORY_H_
#define _QUAKE_LIR_MEMORY_H_
#include "base.h"
#include "common.h"
#include "cg/abi.h"

#include "lir/define.h"
#include "lir/immediate.h"

namespace quake {
namespace lir {
class HOperand;

// Represent a indirect memory node, ie a pointer reference. Notes by itself
// it does not indicate what value been put into that memory address, the
// actual size class of address is been represented with HOperand
class MemoryIndirect {
 public:
  enum Scale { kScale1 = 1, kScale2 = 2, kScale4 = 4, kScale8 = 8 };

 public:
  explicit MemoryIndirect(HOperand* base)
      : base_(base), offset_(nullptr), scale_(kScale1), displacement_(0) {}

  MemoryIndirect(HOperand* base, std::int32_t disp)
      : base_(base), offset_(nullptr), scale_(kScale1), displacement_(disp) {}

  MemoryIndirect(HOperand* base, HOperand* off)
      : base_(base), offset_(off), scale_(kScale1), displacement_(0) {}

  MemoryIndirect(HOperand* base, HOperand* off, Scale scale)
      : base_(base), offset_(off), scale_(scale), displacement_(0) {}

  MemoryIndirect(HOperand* base, HOperand* off, Scale scale, std::int32_t disp)
      : base_(base), offset_(off), scale_(scale), displacement_(disp) {}

 public:
  HOperand* base() const { return base_; }
  HOperand* offset() const { return offset_; }
  Scale scale() const { return scale_; }
  std::int32_t displacement() const { return displacement_; }

  void set_base(HOperand* opr) { base_ = opr; }
  void set_offset(HOperand* offset) { offset_ = offset; }
  void set_scale(Scale scale) { scale_ = scale; }
  void set_displacement(std::int32_t disp) { displacement_ = disp; }

  bool IsMaterialized() const;

 private:
  HOperand* base_;
  HOperand* offset_;
  Scale scale_;
  std::int32_t displacement_;
};

class MemoryStatic {
 public:
  using Addr = std::uintptr_t;  // can hold an abitary address

  explicit MemoryStatic(Addr addr) : address_(addr) {}
  explicit MemoryStatic(void* x) : address_(reinterpret_cast<Addr>(x)) {}

  Addr address() const { return address_; }

 private:
  Addr address_;
};

// Stack slot relative the RBP/RSP. Notes all the memory are aligned with 8
// bytes
class MemoryStack {
 public:
  using Slot = std::int32_t;

  // MemoryStack slot's alignment is at least
  static constexpr std::size_t kAlignment = cg::kStackAlignment;
  static constexpr Slot kInvalidSlot = -1;

  explicit MemoryStack(Slot s) : slot_(s) {}

  // Slot relative to the memory stack position
  Slot slot() const { return slot_; }
  std::uint32_t SlotOffset() const { return slot_ * kAlignment; }

 private:
  Slot slot_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_MEMORY_H_
