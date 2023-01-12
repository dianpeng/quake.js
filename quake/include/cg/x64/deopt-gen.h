#ifndef _QUAKE_CG_X64_DEOPT_GEN_H_
#define _QUAKE_CG_X64_DEOPT_GEN_H_

#include "base.h"
#include "deopt.h"
#include "cg/x64/code-gen.h"

#include <asmjit/asmjit.h>

namespace quake {
namespace cg {
namespace x64 {

class DeoptGen {
 public:
  explicit DeoptGen(CodeGen* cg) : cg_(cg) {}

  void Generate(const Deopt&);

 private:
  Quake* quake() const { return cg_->quake(); }
  void MoveTag(asmjit::x86::Mem, int);
  void BoxFromRegister(asmjit::x86::Gp, int, asmjit::x86::Mem);
  void BoxFromRegister(asmjit::x86::Xmm, int, asmjit::x86::Mem);
  void BoxFromSpill(int, int, asmjit::x86::Mem);
  void BoxFromInt32(std::int32_t, int, asmjit::x86::Mem);
  void BoxFromInt64(std::int64_t, int, asmjit::x86::Mem);
  void BoxFromFloat64(double, int, asmjit::x86::Mem);
  void BoxUndefined(asmjit::x86::Mem);
  void BoxNull(asmjit::x86::Mem);
  void BoxUninitialized(asmjit::x86::Mem);

  void MaterializeTo(asmjit::x86::Mem, Deopt::ValueOrigin, int);
  void MaterializeToSlot(int, asmjit::x86::Mem, std::size_t);
  void MaterializeToLocal(int, asmjit::x86::Mem);
  void MaterializeToParam(int, asmjit::x86::Mem);
  void MaterializeToTemp(int, asmjit::x86::Mem);

  void BoxValueInt32(const Deopt::Val&);
  void BoxValueInt64(const Deopt::Val&);
  void BoxValueFloat64(const Deopt::Val&);
  void BoxValueBoxRef(const Deopt::Val&);
  void BoxValuePtr(const Deopt::Val&);
  void BoxValue(const Deopt::Val&);

  asmjit::x86::Mem SpillAddress(int) const;

 private:
  CodeGen* cg_;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_DEOPT_GEN_H_

