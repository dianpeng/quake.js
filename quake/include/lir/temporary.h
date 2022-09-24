#ifndef _QUAKE_LIR_TEMPORARY_H_
#define _QUAKE_LIR_TEMPORARY_H_
#include "base.h"
#include "lir/register.h"

namespace quake {
namespace lir {

class HAssembler;
class HOperand;
class HInstruction;
class LiveIntervalList;

// This is really a shim of LiveIntervalList. It binds to a specific Operand
// and when it gest resolved the corresponding Operand will be materialized
// as underlying value pointed from LiveIntervalList. On the IR::Node level,
// it will always hold a LiveIntervalList which always contains a up to now
// value
class Temporary {
 public:
  explicit Temporary(HOperand* owner)
      : live_interval_list_(nullptr), owner_(owner), output_(false) {}

  Temporary() : live_interval_list_(nullptr), owner_(nullptr), output_(false) {}

  HOperand* owner() const { return owner_; }
  HInstruction* instruction() const;

 public:
  // --------------------------------------------------------------------------
  // For RegisterAllocation
  //
  // Notes, during the RA phase, the LiveIntervalList will become a internal
  // storage for RA to store OOL information. And after the RA all the Temporary
  // will be just gone, and all the Temporary reference HOperand will become
  // concrect/materialized HOperand.
  bool HasLiveIntervalList() const { return live_interval_list_ != nullptr; }
  LiveIntervalList* live_interval_list() const { return live_interval_list_; }
  void set_live_interval_list(LiveIntervalList* l) { live_interval_list_ = l; }

 public:
  void BindOutput() { output_ = true; }
  bool IsOutput() const { return output_; }
  bool IsBinded() const { return live_interval_list_ != nullptr; }

 private:
  LiveIntervalList* live_interval_list_;
  HOperand* owner_;
  bool output_;

  friend class HOperand;
  friend class HAssembler;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_TEMPORARY_H_
