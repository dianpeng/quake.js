#ifndef _QUAKE_LIR_TEMPORARY_H_
#define _QUAKE_LIR_TEMPORARY_H_
#include "base.h"
#include "lir/register.h"

namespace quake {
namespace lir {

class HAssembler;
class HOperand;
class HInstruction;

// A virtual-register marker bound to a specific operand. Before register
// allocation it identifies the value an operand refers to; the vreg pass
// and the classical liveness/analysis passes build the VregContext tables
// over these, and the constraint-driven linear scan (LinearScanRA)
// rewrites every Temporary reference into a materialized location.
class Temporary {
 public:
  explicit Temporary(HOperand* owner)
      : owner_(owner), output_(false), force_register_(false) {}

  Temporary() : owner_(nullptr), output_(false), force_register_(false) {}

  HOperand* owner() const { return owner_; }
  HInstruction* instruction() const;

 public:
  void BindOutput() { output_ = true; }
  bool IsOutput() const { return output_; }

 public:
  // Whether this temporary CANNOT be in stack slot whne it is been referenced
  // RA will respect this flag when the corresponding temporary (VReg) is been
  // used by any operand
  bool force_register() const { return force_register_; }
  void set_force_register(bool f) { force_register_ = f; }

 private:
  HOperand* owner_;
  bool output_;
  bool force_register_;

  friend class HOperand;
  friend class HAssembler;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_TEMPORARY_H_
