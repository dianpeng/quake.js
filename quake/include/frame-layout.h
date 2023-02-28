#ifndef _QUAKE_FRAME_LAYOUT_H_
#define _QUAKE_FRAME_LAYOUT_H_
#include "base.h"
#include "quick-js-inc.h"
#include "quake-rt.h"

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

enum class JitFrameType : std::uint8_t { kInline, kJitEntry };

using CellIndex = int;

// See stack format in quake/frame.h file
class FrameLayout {
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

  static constexpr int kFrameByteSize = Layout::kFrameSize;

 public:
  FrameLayout() = default;
  FrameLayout(const FrameLayout&) = default;
  FrameLayout(FrameLayout&&) = default;

  FrameLayout& operator=(const FrameLayout&) = default;
  FrameLayout& operator=(FrameLayout&&) = default;

 public:
  // Return a cell index, notes each cell requires 16 bytes
  int AddCell() { return cell_count_++; }
  int AddSpill() { return spill_count_++; }

  // cell count, ie how many cell will be needed to keep on native stack
  int cell_count() const { return cell_count_; }

  // spill count, ie how many slot(8 bytes) should be reserved for RA spill
  int spill_count() const { return spill_count_; }

  // frame count, used for keeping inline frame
  int frame_count() const { return static_cast<int>(frame_list_.size()); }

  std::size_t cell_size() const { return cell_count_ * kCellByteSize; }
  std::size_t spill_size() const { return spill_count_ * kSpillByteSize; }

 public:
  // Total frame size in bytes
  std::size_t frame_size() const;

  // Using frame's index to indexed into the frame offset, W.R.T high address
  std::size_t frame_offset(int) const;

  // Used by EnterInlineFrame to emit frame information
  void AddFrame(std::uint32_t ins_id, int depth, int slot_size);

 public:
  // Notes this is added in an ad-hoc fashion. Some platform may not needed it
  // regardlessly, we just set it accordingly
  std::size_t misc_size() const;

  int stack_byte_size() const {
    return cell_size() + spill_size() + frame_size() + misc_size();
  }

  int frame_offset() const { return misc_size(); }
  int cell_offset() const { return misc_size() + frame_size(); }
  int spill_offset() const { return misc_size() + frame_size() + cell_size(); }

 private:
  int cell_count_ = 0;
  int spill_count_ = 0;

 private:
  // --------------------------------------------------------------------------
  // Frame object
  //
  // For each frame that happen during the same call site, ie inline frame, we
  // record every one and for each depth finally we will learn a frame with
  // the maximum needed cell value slot.
  struct FrameObject {
    // bytesize of this FrameObject
    std::size_t byte_size() const {
      if (lir_id) {
        return kFrameByteSize + slot_size * kCellByteSize;
      } else {
        return 0;
      }
    }

    std::uint32_t lir_id = 0;  // lir instruction id
    int slot_size = 0;         // size of the frame, ie cell value size
  };

  std::vector<FrameObject> frame_list_;
};

}  // namespace quake

#endif  // _QUAKE_FRAME_LAYOUT_H_
