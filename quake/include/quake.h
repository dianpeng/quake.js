#ifndef _QUAKE_H_
#define _QUAKE_H_
#include "zone.h"
#include "quake-api.h"
#include "deopt.h"

namespace quake {

class Quake {
 public:
  explicit Quake(QJS* qjs) : qjs_(qjs), zone_(1024 * 16, 1024 * 128) {}

 public:
  zone::Zone* zone() { return &zone_; }
  QJS* qjs() const { return qjs_; }

 private:
  QJS* qjs_;
  zone::Zone zone_;
};

}  // namespace quake

#endif  // _QUAKE_H_
