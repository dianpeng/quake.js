#include "lir/operand.h"
#include "lir/instruction.h"

namespace quake {
namespace lir {

SizeClass HOperand::size_class() const {
  if (instr_) {
    return instr_->output_size_class();
  }

  // NOTES(dpeng): This should never happen since we have kImm node
  if (type_.IsImm()) {
    return GetImm().size_class();
  }

  return SizeClass::kIrrelevant;
}

}  // namespace lir
}  // namespace quake
