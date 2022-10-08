#include "lir/liveness.h"

namespace quake {
namespace lir {

std::optional<LivenessPoint> LivenessInfo::NextUse(
    const LivenessPoint& when) const {
  if (auto itr = std::lower_bound(use_list_.begin(), use_list_.end(), when);
      itr != use_list_.end()) {
    return {};
  } else {
    return *itr;
  }
}

}  // namespace lir
}  // namespace quake
