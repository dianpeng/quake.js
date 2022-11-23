#ifndef _STACK_SLOT_H_
#define _STACK_SLOT_H_
#include "base.h"
#include "quake-rt.h"
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
// ----------------   LOW
// |              |
// |  Spill Zone  |
// |              |
// ----------------
// |              |
// |  Cell Zone   |
// |              |
// ----------------
// |              |
// |  Frame Zone  |
// |              |
// |--------------|
// |              |
// |  Misc Data   |
// |              |
// |--------------|   HIGH
//
// The stack needs to round up towards platform's requirement, ie SSE 4.2
// requires the stack(RSP) to be aligned with 16 bytes etc ...
//
// ----------------------------------------------------------------------------

namespace quake {
namespace lir {

using CellIndex = int;

// See stack format in quake/frame.h file
class StackSlot {
 public:
  static constexpr int kCellByteSize = Layout::kCellSize;
  static constexpr int kSpillByteSize =
#ifdef QUAKE_TARGET_X64
      8
#else
#error "unknown arch"
#endif  // QUAKE_TARGET_X64
      ;
  static constexpr int kSpillOverCell = kCellByteSize / kSpillByteSize;

  static_assert(kCellByteSize == 16, "JSValue must be 2 qword long");

  // FIXME(dpeng): fix it when we figure out how to layout our frame for inline
  //   function.
  static constexpr int kFrameByteSize = 0;

 public:
  // Return a cell index, notes each cell requires 16 bytes
  int AddCell() { return cell_count_++; }
  int AddSpill() { return spill_count_++; }

  // cell count, ie how many cell will be needed to keep on native stack
  int cell_count() const { return cell_count_; }

  // spill count, ie how many slot(8 bytes) should be reserved for RA spill
  int spill_count() const { return spill_count_; }

  // frame count, used for keeping inline frame
  int frame_count() const { return frame_count_; }

  std::size_t cell_size() const { return cell_count_ * kCellByteSize; }
  std::size_t spill_size() const { return spill_count_ * kSpillByteSize; }
  std::size_t frame_size() const { return frame_count_ * kFrameByteSize; }

  // Notes this is added in an ad-hoc fashion. Some platform may not needed it
  // regardlessly, we just set it accordingly
  std::size_t misc_size() const { return sizeof(QuakeJit::ScriptCall); }

  int stack_byte_size() const {
    return cell_size() + spill_size() + frame_size() + misc_size();
  }

  int frame_offset() const { return misc_size(); }
  int cell_offset() const { return misc_size() + frame_size(); }
  int spill_offset() const { return misc_size() + frame_size() + cell_size(); }

 private:
  int cell_count_ = 0;
  int spill_count_ = 0;
  int frame_count_ = 0;
};

}  // namespace lir
}  // namespace quake

#endif  // _STACK_SLOT_H_
