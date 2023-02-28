#include "frame-layout.h"
#include "quake-rt.h"

#include "lir/instruction.h"

namespace quake {

void FrameLayout::AddFrame(std::uint32_t ins_id, int depth, int slot_size) {
  if (static_cast<std::size_t>(depth) >= frame_list_.size()) {
    frame_list_.resize(depth + 1);
  }
  if (frame_list_[depth].slot_size < slot_size) {
    frame_list_[depth].lir_id = ins_id;
    frame_list_[depth].slot_size = slot_size;
  }
}

std::size_t FrameLayout::frame_size() const {
  std::size_t c = 0;
  for (const auto& f : frame_list_) {
    c += f.byte_size();
  }
  return c;
}

std::size_t FrameLayout::frame_offset(int depth) const {
  CHECK(static_cast<std::size_t>(depth) < frame_list_.size());
  CHECK(frame_list_[depth].lir_id != lir::kInvalidInstructionId);
  std::size_t c = 0;
  for (auto i = 0; i < depth; i++) {
    c += frame_list_[depth].byte_size();
  }
  return c;
}

std::size_t FrameLayout::misc_size() const {
  return sizeof(QuakeJit::ScriptCall);
}

}  // namespace quake
