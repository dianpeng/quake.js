#ifndef _QUAKE_CG_X64_CODE_GEN_H_
#define _QUAKE_CG_X64_CODE_GEN_H_
#include "base.h"
#include <asmjit/asmjit.h>

#include "cg/x64/code-x64.h"
#include "cg/x64/comment.h"
#include "cg/x64/macro-assembler.h"

namespace quake {
class Quake;

namespace lir {
class HAssembler;
class HInstruction;
}  // namespace lir

namespace cg {
namespace x64 {

class CodeGen {
  enum { kNotGen, kGenErr, kGenDone };

 public:
  static constexpr int kStackTempSize = 8;
  static constexpr int kPadding = 8;

 public:
  CodeGen(Quake*, lir::HAssembler*);

  // get the stack reserved size during the function prologue of the generated
  // jitted function
  std::size_t stack_reserve_size() const;
  std::size_t stack_reserve_size_jit_link() const;

  std::size_t stack_temp_offset() const;

  // get the micro assembler object. Notes we extend the asmjit assembler little
  // bit for supporing some other functions
  MacroAssembler* as() { return as_.get(); }

 public:
  // All in one functions. Used to generate everything into a Code object. Notes
  // if generate returns error, then any access to CodeGen's function is
  // invalid, otherwise user can use DetachCode to release the code object which
  // has been fully populated
  base::Error Generate();

 public:
  // Code object. Notes, cannot be used after DetachCode/Generate function
  CodeX64* code() const {
    CHECK(code_);
    return code_.get();
  }

  // Get fully populated code. Notes this function must be invoked after the
  // Generate is called and returns none-error result
  std::unique_ptr<CodeX64> DetachCode() {
    CHECK(state_ == kGenDone);
    CHECK(code_);
    return std::move(code_);
  }

  Quake* quake() const { return quake_; }

 private:
  bool ShouldIgnoreInstruction(lir::HInstruction* ins) const;
  base::Error GenerateStub();
  base::Error GenerateBody();
  base::Error GenerateDeopt();

  // After the code generation, we will need to finalize everything, ie resolve
  // the label to its actual address if needed.
  base::Error Finalize();

 private:
  Quake* quake_;
  lir::HAssembler* has_;
  int state_;

  // --------------------------------------------------------------------------
  // depedent state, order matters
  std::unique_ptr<CodeX64> code_;
  std::unique_ptr<MacroAssembler> as_;

  DISALLOW_COPY_AND_ASSIGN(CodeGen);
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_CODE_GEN_H_
