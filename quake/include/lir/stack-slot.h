#ifndef _STACK_SLOT_H_
#define _STACK_SLOT_H_
#include "base.h"
#include "quick-js-inc.h"

// ----------------------------------------------------------------------------
// Collecting all the stack spilled slot. Certain instruction will always
// spill on the stack, and we need to reserve some dedicated spaces for
// these instructions, ie boxed value must be placed on the stack for now
// since quickjs uses 16 bytes for a box value. RA's spilled slot needs to
// be placed on the stack.
//
// In our cases, the stack is been orgnized as following ----------------------
//
// +=============+
// |             |
// | Cell Region |
// |             |
// |-------------|
// |             |
// |             |
// | Spill Slot  |
// |             |
// |             |
// |-------------|
//
// The stack needs to round up towards platform's requirement, ie SSE 4.2
// requires the stack(RSP) to be aligned with 16 bytes etc ...
//
// ----------------------------------------------------------------------------

namespace quake {
namespace lir {

using CellIndex = int;

class StackSlot {
 public:
  static constexpr int kCellByteSize = sizeof(JSValue);  // 2 qword
  static constexpr int kSpillByteSize =
#ifdef QUAKE_TARGET_X64
      8
#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64
      ;
  static constexpr int kSpillOverCell = kCellByteSize / kSpillByteSize;
  static_assert(kCellByteSize == 16, "JSValue must be 2 qword long");

 public:
  // Return a cell index, notes each cell requires 16 bytes
  int AddCell() { return cell_count_++; }
  int AddSpill() { return spill_count_++; }

  int cell_count() const { return cell_count_; }
  int spill_count() const { return spill_count_; }
  int spill_offset() const { return kCellByteSize * cell_count(); }
  int stack_byte_size() const {
    return cell_count_ * kCellByteSize + spill_count_ * kSpillByteSize;
  }

  std::size_t cell_size() const { return cell_count_ * kCellByteSize; }
  std::size_t spill_size() const { return spill_count_ * kSpillByteSize; }

 private:
  int cell_count_ = 0;
  int spill_count_ = 0;
};

}  // namespace lir
}  // namespace quake

#endif  // _STACK_SLOT_H_
