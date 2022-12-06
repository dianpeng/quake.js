#ifndef _QUAKE_JIT_H_
#define _QUAKE_JIT_H_
#include "base.h"
#include "common.h"
#include "quake-C.h"

#include <asmjit/asmjit.h>

namespace quake {

class Code;

#define QUAKE_JIT_OPTION_LIST(__) __(bool, option_trace_jit, false)

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
  // --------------------------------------------------------------------------
  // all the flags, ie global tunnable flags used for jit runtime
#define __(A, B, ...)          \
  A B() const { return B##_; } \
  void set_##B(A value) { B##_ = value; }

  QUAKE_JIT_OPTION_LIST(__)

#undef __  // __

 public:
  base::Result<JitCodeEntry> AddCode(Code*);

 private:
  Jit();

  asmjit::JitRuntime jit_runtime_;

 private:
#define __(A, B, ...) A B##_;
  QUAKE_JIT_OPTION_LIST(__)
#undef __  // __
  bool dummy_;
};

}  // namespace quake

#endif  // _QUAKE_JIT_H_
