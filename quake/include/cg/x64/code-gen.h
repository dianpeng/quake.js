#ifndef _QUAKE_CG_X64_CODE_GEN_H_
#define _QUAKE_CG_X64_CODE_GEN_H_

#include "base.h"
#include <asmjit/asmjit.h>

namespace quake {
class Quake;

namespace cg {
namespace x64 {

class CodeGen {
 public:
  explicit CodeGen(Quake*);

 public:
  asmjit::x86::Builder* as() const { return as_.get(); }

 public:
  // known entry/label for certain know position

  // A label that points to the deoptimization trampoline. Assume the exit#
  // is been stored inside of the RAX register. Will never scratch RAX inside
  asmjit::Label deopt_trampoline();
  void BindDeoptTrampoline();

  // A label that points to the epilogue part of the JIT code. Inside of it,
  // it will assume RAX as return value. RAX == 0 means nothing special to
  // do. Notes RAX is been used as a hidden way to carray information between
  // epilogue and other trampoline for now. The code may not be documented
  // well.
  asmjit::Label jit_epilogue();
  void BindJitEpilogue();

 public:
  // constant table
  asmjit::x86::Mem const_f64_one() const {
    CHECK(const_f64_one_);
    return *const_f64_one_;
  }
  asmjit::x86::Mem const_f64_neg_zero() const {
    CHECK(const_f64_neg_zero_);
    return *const_f64_neg_zero_;
  }
  asmjit::x86::Mem const_f64_pos_zero() const {
    CHECK(const_f64_pos_zero_);
    return *const_f64_pos_zero_;
  }

 private:
  Quake* quake_;
  std::unique_ptr<asmjit::x86::Builder> as_;

  // --------------------------------------------------------------------------
  // known labels, generated and used at various stages

  // deoptimization trampoline label
  std::optional<asmjit::Label> deopt_trampoline_;

  // jit frame's epilogue trampoline label
  std::optional<asmjit::Label> jit_epilogue_;

  // constant
  std::optional<asmjit::x86::Mem> const_f64_one_;
  std::optional<asmjit::x86::Mem> const_f64_pos_zero_;
  std::optional<asmjit::x86::Mem> const_f64_neg_zero_;

  DISALLOW_COPY_AND_ASSIGN(CodeGen);
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_CODE_GEN_H_
