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
  inline int ScaleNumber() const;
  std::int32_t displacement() const { return displacement_; }

  void set_base(HOperand* opr) { base_ = opr; }
  void set_offset(HOperand* offset) { offset_ = offset; }
  void set_scale(Scale scale) { scale_ = scale; }
  void set_displacement(std::int32_t disp) { displacement_ = disp; }
  bool IsMaterialized() const;

  std::string Print() const;

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
  explicit MemoryStatic(const void* x) : address_(reinterpret_cast<Addr>(x)) {}

  Addr address() const { return address_; }

  inline std::string Print() const;

 private:
  Addr address_;
};

// Stack slot relative the RBP/RSP. Notes all the memory are aligned with 8
// bytes
class MemoryStack {
 public:
  using Slot = std::uint32_t;

  // MemoryStack slot's alignment is at least
  static constexpr std::size_t kAlignment = cg::kStackAlignment;
  static constexpr Slot kInvalidSlot = -1;

  explicit MemoryStack(Slot s) : slot_(s), offset_(0) {}

  // Slot relative to the memory stack position
  Slot slot() const { return slot_; }
  std::uint32_t SlotOffset() const { return offset_ + slot_ * kAlignment; }
  inline std::string Print() const;

 public:
  // Used by PreCodeGen
  void set_offset(std::uint32_t off) { offset_ = off; }

 private:
  Slot slot_;
  std::uint32_t offset_;  // offset is in bytes
};

inline int MemoryIndirect::ScaleNumber() const {
  switch (scale_) {
    case kScale1:
      return 1;
    case kScale2:
      return 2;
    case kScale4:
      return 4;
    case kScale8:
      return 8;
    default:
      UNREACHABLE();
      return -1;
  }
}

inline std::string MemoryStatic::Print() const {
  return base::Fmt("@mem_addr(%p)", reinterpret_cast<void*>(address_));
}

inline std::string MemoryStack::Print() const {
  return base::Fmt("@mem_stk(%d)", slot_);
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_MEMORY_H_
