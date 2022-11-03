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

 public:
  // Debug helpers. These functions can be invoked at any point and it will
  // assume any registers that is referenced by the function are supposed to
  // be saved. And it does take care of stack alignment, ie for SSE requirement
  // at any point of the machine code.
  void debug_i32(asmjit::x86::Gp);
  void debug_i32(std::int32_t);

  void debug_i64(asmjit::x86::Gp);
  void debug_i64(std::int64_t);

  void debug_f64(asmjit::x86::Xmm);
  void debug_f64(double);

 private:
  void SafeFunctionCallPrologue();
  void SafeFunctionCallEpilogue();
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_MACRO_ASSEMBLER_H_
