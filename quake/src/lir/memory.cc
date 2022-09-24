#include "lir/memory.h"
#include "lir/operand.h"

namespace quake {
namespace lir {

bool MemoryIndirect::IsMaterialized() const {
  if (base_ && !base_->IsMaterialized()) return false;
  if (offset_ && !offset_->IsMaterialized()) return false;
  return true;
}

}  // namespace lir
}  // namespace quake
