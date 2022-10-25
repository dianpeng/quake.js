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

std::string LivenessInfo::Print() const {
  return base::Fmt(
      "#%d: %s [%s]", liveness_id_, range_.Print().c_str(),
      base::Join(use_list_.begin(), use_list_.end(), "; ").c_str());
}

}  // namespace lir
}  // namespace quake
