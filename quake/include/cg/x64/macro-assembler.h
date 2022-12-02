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
  // temporary address, the following address only related to rbp and can be
  // safely used cross *fast call(subroutine)* boundary.
  asmjit::x86::Mem StackTemp0(int size = 8);
  asmjit::x86::Mem StackTemp1(int size = 8);
  asmjit::x86::Mem StackTemp2(int size = 8);
  asmjit::x86::Mem StackTemp3(int size = 8);
  asmjit::x86::Mem StackTemp4(int size = 8);
  asmjit::x86::Mem StackTemp5(int size = 8);
  asmjit::x86::Mem StackTemp6(int size = 8);
  asmjit::x86::Mem StackTemp7(int size = 8);

 public:
  // cell/box operations
  //
  // turn a cell index into a usable address
  asmjit::x86::Mem CellAddress(int index);
  asmjit::x86::Mem CellAddressLower(int index);
  asmjit::x86::Mem CellAddressHigher(int index);
  asmjit::x86::Mem CellAddressTag(int index);
  asmjit::x86::Mem CellAddressValue(int index);

  void store_cell(int cell_index, asmjit::x86::Gp, int tag);
  void store_cell(int cell_index, asmjit::x86::Xmm, int tag);

  void store_cell(asmjit::x86::Mem, asmjit::x86::Gp, int tag);
  void store_cell(asmjit::x86::Mem, asmjit::x86::Xmm, int tag);

  // Move a 128 bits value.
  //
  // The first GP register represent the target address to hold the 128 bits
  // value, and the second Gp/Mem register represent the from address to hold
  // the source 128 bits value
  void move128(asmjit::x86::Gp, asmjit::x86::Gp);

 public:
  // JSCall helper
  asmjit::x86::Mem ScriptCallAddress();
  asmjit::x86::Mem ScriptCallFunctionObjectAddress();
  asmjit::x86::Mem ScriptCallThisObjectAddress();
  asmjit::x86::Mem ScriptCallTargetObjectAddress();
  asmjit::x86::Mem ScriptCallReturnAddressAddress();
  asmjit::x86::Mem ScriptCallParameterStartAddress();
  asmjit::x86::Mem ScriptCallParameterSizeAddress();
  asmjit::x86::Mem ScriptCallOptionAddress();

 private:
  std::pair<asmjit::x86::Mem, asmjit::x86::Mem> GetCellAddress(
      int cell_index) const;

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

 private:
  CodeGen* cg_;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_MACRO_ASSEMBLER_H_
