#ifndef _QUAKE_H_
#define _QUAKE_H_
#include "zone.h"
#include "quake-api.h"
#include "deopt.h"

namespace quake {

// Quake, represent a trace that can be used for Jit
class Quake {
 public:
  explicit Quake(QJS* qjs) : qjs_(qjs), zone_(1024 * 16, 1024 * 128) {}

 public:
  zone::Zone* zone() { return &zone_; }
  QJS* qjs() const { return qjs_; }

 public:
  // Deoptimization table mapping
  const Deopt& GetDeopt(DeoptId) const;

  // Add a new deoptimization entry into the quake. Notes, this is for internal
  // usage by lir::HAssembler.
  DeoptId AddDeopt(DeoptReason, DeoptNext, lir::Snapshot&&);

  // Size of total deoptimization point for this trace(quake)
  std::size_t deopt_size() const { return deopt_list_.size(); }

 public:
  // in bytes
  std::size_t cell_size() const { return cell_size_; }
  void set_cell_size(std::size_t s) { cell_size_ = s; }

  // in bytes
  std::size_t spill_size() const { return spill_size_; }
  void set_spill_size(std::size_t s) { spill_size_ = s; }

 public:
  // Quake's jit flag
  enum {
    // fmod LIR will be translated into fast inline assembly, instead of
    // generaing runtime call to call std::fmod function. The former one
    // is way faster than calling std::fmod, but may yield incompatible
    // floating point behavior in corner cases on certain platform
    kJitFModFastCall,

    // fpow LIR will be translated into fast inline assembly, instead of
    // generaing runtime call to call std::fmod function. The former one
    // is way faster than calling std::fmod, but may yield incompatible
    // floating point behavior in corner cases on certain platform
    kJitFPowFastCall
  };

  bool IsJitFlagOn(int) const;

 private:
  QJS* qjs_;
  zone::Zone zone_;

 private:
  // list of deoptimization point
  std::vector<Deopt> deopt_list_;

 private:
  // stack layout related, used to help get the spill slot etc ...
  std::size_t cell_size_;   // cell region reserved size in bytes
  std::size_t spill_size_;  // spill size in bytes
};

inline const Deopt& Quake::GetDeopt(DeoptId deopt_id) const {
  CHECK(deopt_id < deopt_list_.size());
  return deopt_list_[deopt_id];
}

}  // namespace quake

#endif  // _QUAKE_H_
