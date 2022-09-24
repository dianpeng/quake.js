#include <zone/zone.h>

namespace zone {

void Zone::Clear() {
  // 1) Call all the tracked object's destructor
  dtor_list_.clear();

  // 2) Clean up all the memory
  for (Segment* p = nullptr; segment_; segment_ = p) {
    p = segment_->next;
    ::free(segment_);
  }
  size_ = 0;
  segment_count_ = 0;
  pool_ = nullptr;
}

std::size_t Zone::GetTotalSize() const {
  std::size_t r = 0;
  for (auto p = segment_; p; p = p->next) {
    r += p->size;
  }
  return r;
}

}  // namespace zone
