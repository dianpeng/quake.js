#ifndef _QUAKE_LIR_PRE_CODE_GEN_H_
#define _QUAKE_LIR_PRE_CODE_GEN_H_
#include "base.h"

namespace quake {
class Quake;

namespace lir {
class HAssembler;
class HInstruction;

// Before codegen, we have on extra pass to fix something, ie materialize RCall
// with its corresponding runtime call's address, instead of using a enum index.
// Materialize all the operand/output which generates a memory stack to its
// actual memory stack offset
class PreCodeGen {
 public:
  PreCodeGen(Quake*, HAssembler*);

  void Run();

 private:
  void RewriteMemStack(HInstruction*);
  void FixRCallAddress(HInstruction*);

 private:
  Quake* quake_;
  HAssembler* as_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_PRE_CODE_GEN_H_
