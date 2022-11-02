#include "quake.h"

namespace quake {

DeoptId Quake::AddDeopt(DeoptReason reason, DeoptNext next) {
  auto deopt_id = static_cast<DeoptId>(deopt_list_.size());
  deopt_list_.emplace_back(deopt_id, reason, next);
  return deopt_id;
}

void Quake::FinishDeoptEntry(DeoptId did, const ir::Checkpoint& ck) {
  deopt_list_[did].From(ck);
}

}  // namespace quake
