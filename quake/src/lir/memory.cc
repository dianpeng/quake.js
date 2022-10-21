#include "lir/memory.h"
#include "lir/operand.h"

namespace quake {
namespace lir {

bool MemoryIndirect::IsMaterialized() const {
  if (base_ && !base_->IsMaterialized()) return false;
  if (offset_ && !offset_->IsMaterialized()) return false;
  return true;
}

std::string MemoryIndirect::Print() const {
  return base::Fmt("@mem_ind(%s, %s, %d, %d)", base_->Print().c_str(),
                   (offset_ ? offset_->Print().c_str() : "<N/A>"),
                   ScaleNumber(), displacement());
}

}  // namespace lir
}  // namespace quake
