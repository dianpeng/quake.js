#ifndef _QUAKE_CG_X64_STUB_GEN_
#define _QUAKE_CG_X64_STUB_GEN_
#include "base.h"
#include "cg/x64/code-x64.h"

// Generate various stub used for our assembly code. Notes, the STUB is per
// entry instead of per process. Per process is runtime stub.

namespace quake {
class Deopt;

namespace cg {
namespace x64 {

class CodeGen;

class Stub {
 public:
  static void Prologue(CodeGen*);
  static void Epilogue(CodeGen*);

 public:
  static void Deopt(CodeGen*);
  static void CastF64ToB32(CodeGen*);
  static void ObjectFieldGetAtomFast(CodeGen*);
  static void GlobalGetGlobalAtomFast(CodeGen*);

 public:
  // deoptimization from inner JIT trace to outter JIT trace.o
  //
  // This function does deoptimization from a inner to outter, without needing
  // to go to normal Deopt routine. The normal Deopt routine requires C++ code
  // to help to reconstruct the VM frame, which is too slow when nested loop
  // is been jitted. Instead, we generate deoptimization code directly from the
  // Checkpoint, by this the linked trace, ie side trace is much more efficient
  // to execute. In the future, we can coales the register directly into its
  // outer jit code's register/spill slot.
  static void DeoptimizeFromTrace(CodeGen*, const quake::Deopt&);
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_STUB_GEN_
