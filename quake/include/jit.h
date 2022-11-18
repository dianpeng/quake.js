#ifndef _QUAKE_JIT_H_
#define _QUAKE_JIT_H_
#include "base.h"
#include "common.h"
#include "quake-C.h"

#include <asmjit/asmjit.h>

namespace quake {

class Code;

// A global state holder. Any single trace will need to access this object to
// shared internal obejct. Notes, this object should be somehow allow thread
// safe via TLS. We do not maintain lock on object since we are not sure
// asmjit is thread safe or not just by lock.
class Jit {
 public:
  // thread safe interface, will always return a Jit state per thread owned, ie
  // via TLS
  static Jit& GetInstance();

 public:
  asmjit::JitRuntime& jit_runtime() { return jit_runtime_; }
  const asmjit::JitRuntime& jit_runtime() const { return jit_runtime_; }

 public:
  base::Result<JitCodeEntry> AddCode(Code*);

 private:
  Jit();

  asmjit::JitRuntime jit_runtime_;
};

}  // namespace quake

#endif  // _QUAKE_JIT_H_
