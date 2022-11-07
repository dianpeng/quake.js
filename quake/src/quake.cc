#include "quake.h"

namespace quake {

DeoptId Quake::AddDeopt(DeoptReason reason, DeoptNext next,
                        lir::Snapshot&& snap) {
  auto deopt_id = static_cast<DeoptId>(deopt_list_.size());
  deopt_list_.emplace_back(deopt_id, reason, next);
  deopt_list_.back().From(std::move(snap));
  return deopt_id;
}

}  // namespace quake
