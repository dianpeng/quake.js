#include "lir/liveness.h"

namespace quake {
namespace lir {

std::optional<LivenessPoint> LivenessInfo::NextUse(
    const LivenessPoint& when) const {
  if (auto itr = std::lower_bound(use_list_.begin(), use_list_.end(), when);
      itr == use_list_.end()) {
    return {};
  } else {
    return *itr;
  }
}

std::size_t LivenessInfo::UseCount(const LivenessPoint& when,
                                   const LivenessPoint& stop) const {
  auto start = std::lower_bound(use_list_.begin(), use_list_.end(), when);
  auto end = std::upper_bound(use_list_.begin(), use_list_.end(), stop);
  return std::distance(start, end);
}

std::size_t LivenessInfo::UseCount(const LivenessPoint& when) const {
  auto start = std::lower_bound(use_list_.begin(), use_list_.end(), when);
  return std::distance(start, use_list_.end());
}

std::string LivenessInfo::Print() const {
  return base::Fmt(
      "#%d: %s [%s]", liveness_id_, range_.Print().c_str(),
      base::Join(use_list_.begin(), use_list_.end(), "; ").c_str());
}

}  // namespace lir
}  // namespace quake
