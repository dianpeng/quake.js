#ifndef _QUAKE_CG_LIR_LABEL_H_
#define _QUAKE_CG_LIR_LABEL_H_
#include "base.h"
#include <asmjit/asmjit.h>

namespace quake {
namespace lir {
class HAssembler;
class HInstruction;

// Label, represents a label inside of the HInstruction stream.
class Label {
 public:
  Label() = default;
  Label(HInstruction* l) : bind_(l) {}

  HInstruction* bind() const { return bind_; }
  bool HasBind() const { return bind_ != nullptr; }

 public:
  // Machine label related
  bool HasMachineLabel() const { return static_cast<bool>(machine_label_); }
  void set_machine_label(asmjit::Label&& l) {
    machine_label_.emplace(std::move(l));
  }
  const asmjit::Label& machine_label() const { return *machine_label_; }

 private:
  // Used by HAssembler. User cannot bind it without using HAssembler since this
  // label will be translated into the machine level label
  void set_bind(HInstruction* b) { bind_ = b; }

 private:
  // --------------------------------------------------------------------------
  // Used to indicate a certain position inside of the LIR graph. This is used
  // for PHI node to point to its control dependent. The main usage of it is
  // to help RA to promote PHI's operand back into operation result in register
  // otherwise the PHI elimination is hard to perform.

  // The label means it is pointed at position *AFTER* the bind_ instruction
  HInstruction* bind_ = nullptr;

  // --------------------------------------------------------------------------
  // Real label maps back to asmjit. Only setup during the code generation
  std::optional<asmjit::Label> machine_label_;

  friend class HAssembler;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_CG_LIR_LABEL_H_
