#ifndef _QUAKE_CG_X64_MACRO_ASSEMBLER_H_
#define _QUAKE_CG_X64_MACRO_ASSEMBLER_H_

#include "base.h"
#include <asmjit/asmjit.h>

namespace quake {
namespace cg {
namespace x64 {

class CodeGen;

class MacroAssembler : public asmjit::x86::Builder {
 public:
  explicit MacroAssembler(CodeGen*);

 public:
  // Special function to be used to emit unreachable code. Notes this
  // instruction is destructive, and it will break any callee frame and generate
  // an error information and then quit. The final call will be inside of the
  // QuickRt
  void unreachable(int);

  void printi32(asmjit::x86::Gp);
  void printi32(std::int32_t);
  void printf64(asmjit::x86::Xmm);
  void printf64(double);
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_MACRO_ASSEMBLER_H_
