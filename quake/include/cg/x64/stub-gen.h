#ifndef _QUAKE_CG_X64_STUB_GEN_
#define _QUAKE_CG_X64_STUB_GEN_
#include "base.h"

// Generate various stub used for our assembly code. Notes, the STUB is per
// entry instead of per process. Per process is runtime stub.

namespace quake {
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
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_STUB_GEN_
