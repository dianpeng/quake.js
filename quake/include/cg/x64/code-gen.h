#ifndef _QUAKE_CG_X64_CODE_GEN_H_
#define _QUAKE_CG_X64_CODE_GEN_H_

#include "base.h"
#include <asmjit/asmjit.h>

namespace quake {
namespace cg {
namespace x64 {

class CodeGen {
 public:
  CodeGen() : as_() {}

 public:
  asmjit::x86::Builder* as() const { return as_.get(); }

 public:
  // known entry/label for certain know position
  
  // A label that points to the deoptimization trampoline. Assume the exit#
  // is been stored inside of the RAX register. Will never scratch RAX inside
  asmjit::Label deopt_trampoline();


  // A label that points to the epilogue part of the JIT code. Inside of it,
  // it will assume RAX as return value. RAX == 0 means nothing special to
  // do. Notes RAX is been used as a hidden way to carray information between
  // epilogue and other trampoline for now. The code may not be documented
  // well.
  asmjit::Label jit_epilogue();

 private:
  std::unique_ptr<asmjit::x86::Builder> as_;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_CODE_GEN_H_
