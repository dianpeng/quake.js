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
  DeoptId AddDeopt(DeoptReason, DeoptNext);

  // Called before the codegen, after the RA finish, ie all register has been
  // settled down
  void FinishDeoptEntry(DeoptId, const ir::Checkpoint&);

  // Size of total deoptimization point for this trace(quake)
  std::size_t deopt_size() const { return deopt_list_.size(); }

 private:
  QJS* qjs_;
  zone::Zone zone_;

 private:
  // list of deoptimization point
  std::vector<Deopt> deopt_list_;
};

inline const Deopt& Quake::GetDeopt(DeoptId deopt_id) const {
  CHECK(deopt_id < deopt_list_.size());
  return deopt_list_[deopt_id];
}

}  // namespace quake

#endif  // _QUAKE_H_
