#ifndef _QUAKE_CG_LIR_ASSEMBLER_H_
#define _QUAKE_CG_LIR_ASSEMBLER_H_
#include "base.h"
#include "quake.h"
#include "common.h"

#include "lir/define.h"
#include "lir/immediate.h"
#include "lir/memory.h"
#include "lir/label.h"
#include "lir/opcode.h"
#include "lir/operand.h"
#include "lir/instruction.h"

namespace quake {

namespace ir {
class Node;
class Checkpoint;
}  // namespace ir

namespace lir {

class HAssembler {
 public:
  explicit HAssembler(Quake*);

  Quake* quake() const { return quake_; }
  zone::Zone* zone() const { return quake()->zone(); }

 public:
  // Operand factory
  HOperand* NewLabel();
  HOperand* NewTemporary();
  HOperand* NewImmediate(std::int32_t);
  HOperand* NewImmediate(std::int64_t);
  HOperand* NewImmediate(double);
  HOperand* NewImmediate(const Immediate&);
  HOperand* NewInt32Zero() {
    return NewImmediate(static_cast<std::int32_t>(0));
  }
  HOperand* NewInt64Zero() {
    return NewImmediate(static_cast<std::int64_t>(0));
  }
  HOperand* NewFloat64Zero() { return NewImmediate(static_cast<double>(0.0)); }

  HOperand* NewMemInd(MemoryIndirect&&);
  HOperand* NewMemStatic(MemoryStatic&&);
  HOperand* NewMemStatic(void*);
  HOperand* NewMemStatic(std::intptr_t);

  HOperand* NewMemStack(MemoryStack&&);
  HOperand* NewMemStack(MemoryStack::Slot slot) {
    return NewMemStack(MemoryStack{slot});
  }
  HOperand* NewRegister(const Register*);

 public:
  // HInstruction factory
  HInstruction* NewInstructionAfter(ir::Node*, HOpcode, HInstruction* after);

  HInstruction* NewInstructionAfter(ir::Node*, HOpcode, HOperand*,
                                    HInstruction* after);

  HInstruction* NewInstructionAfter(ir::Node*, HOpcode, HOperand*, HOperand*,
                                    HInstruction* after);

  HInstruction* NewInstructionAfter(ir::Node*, HOpcode, std::size_t,
                                    HInstruction* after);

  HInstruction* NewInstructionBefore(ir::Node*, HOpcode, HInstruction* before);

  HInstruction* NewInstructionBefore(ir::Node*, HOpcode, HOperand*,
                                     HInstruction* before);

  HInstruction* NewInstructionBefore(ir::Node*, HOpcode, HOperand*, HOperand*,
                                     HInstruction* before);

  HInstruction* NewInstructionBefore(ir::Node*, HOpcode, std::size_t,
                                     HInstruction* before);

 public:
  /* =========================================================================
   * The following APIs are used for Translator
   * ========================================================================*/
  void BeginOpcode(ir::Node*);
  void EndOpcode();

  // --------------------------------------------------------------------------
  // Arithmetic operations
  // HInstruction builder APIs, used with BeginOpcode and EndOpcode
  HOperand* IAdd(HOperand*, HOperand*);
  HOperand* ISub(HOperand*, HOperand*);
  HOperand* IMul(HOperand*, HOperand*);
  HOperand* IDiv(HOperand*, HOperand*);
  HOperand* FAdd(HOperand*, HOperand*);
  HOperand* FSub(HOperand*, HOperand*);
  HOperand* FMul(HOperand*, HOperand*);
  HOperand* FDiv(HOperand*, HOperand*);

  HOperand* And(HOperand*, HOperand*);
  HOperand* Or(HOperand*, HOperand*);
  HOperand* Xor(HOperand*, HOperand*);
  HOperand* LShift(HOperand*, HOperand*);
  HOperand* RShift(HOperand*, HOperand*);

  // --------------------------------------------------------------------------
  // Unary operations
  HOperand* Inc(HOperand*);
  HOperand* Dec(HOperand*);
  HOperand* Neg(HOperand*);
  HOperand* NegF(HOperand*);
  HOperand* Inv(HOperand*);

  // --------------------------------------------------------------------------
  // Comparison operations
  HOperand* Test(HOperand*, HOperand*);
  HOperand* CmpF(HOperand*, HOperand*);
  HOperand* CmpI(HOperand*, HOperand*);
  HOperand* CmpTrue(HOperand*);
  HOperand* CmpFalse(HOperand*);

  // --------------------------------------------------------------------------
  // jump operations
  HOperand* JmpNe(HOperand*);
  HOperand* JmpEq(HOperand*);
  HOperand* JmpLt(HOperand*);
  HOperand* JmpLe(HOperand*);
  HOperand* JmpBt(HOperand*);
  HOperand* JmpBe(HOperand*);
  HOperand* JmpGt(HOperand*);
  HOperand* JmpGe(HOperand*);
  HOperand* JmpAt(HOperand*);
  HOperand* JmpAe(HOperand*);
  HOperand* JmpNz(HOperand*);
  HOperand* JmpZ(HOperand*);

  // --------------------------------------------------------------------------
  // jump operation's operand can be deferred later
  HOperand* JmpNe();
  HOperand* JmpEq();
  HOperand* JmpLt();
  HOperand* JmpLe();
  HOperand* JmpBt();
  HOperand* JmpBe();
  HOperand* JmpGt();
  HOperand* JmpGe();
  HOperand* JmpAt();
  HOperand* JmpAe();
  HOperand* JmpNz();
  HOperand* JmpZ();

  HOperand* LoadLocal(HOperand*);
  HOperand* LoadParam(HOperand*);
  HOperand* LoadTemp(HOperand*);

  // Label
  void Bind(HOperand*);
  void BindAfter(HInstruction*, HOperand*);

 public:
  // --------------------------------------------------------------------------
  // Memory/moving, RTL etc ...
  // Move
  HOperand* Mov(HOperand*);
  HOperand* MovZX(HOperand*);
  HOperand* MovSX(HOperand*);

  HOperand* MovI64(HOperand*);
  HOperand* MovB64(HOperand*);
  HOperand* MovI32(HOperand*);
  HOperand* MovF64(HOperand*);

 public:
  // Stack
  HOperand* Pop(HOperand*);
  HOperand* Push(HOperand*);

  HOperand* Lea(HOperand*);

 public:
  // Box and Unbox
  HOperand* Box(HOperand*);
  HOperand* Unbox(HOperand*);
  HOperand* GetTTag(HOperand*);

 public:
  // Reference counting related

 public:
  // --------------------------------------------------------------------------
  // Deoptimization
  //
  // ie, we want to deoptimize into the given checkpoint
  void Deopt(ir::Checkpoint*);
  void DeoptIfZ(ir::Checkpoint*);
  void DeoptIfEq(ir::Checkpoint*);
  void DEoptIfNe(ir::Checkpoint*);

 public:
  HInstruction* last_instruction() const { return end_hinstruction_->prev(); }
  inline HInstruction* end_instruction() const { return end_hinstruction_; }
  inline HInstruction* start_instruction() const {
    return end_hinstruction_->next();
  }

  // Basic block level iterator, notes due to our tracing jit method, the
  // graph is static with 2 basic block and splitted by a loop back jump
  HInstruction* bb0_start() const { return start_instruction(); }
  HInstruction* bb0_end() const { return bb1_start(); }

  HInstruction* bb1_start() const { return bb1_start_; }
  HInstruction* bb1_end() const { return phi_start(); }

  HInstruction* phi_start() const { return phi_start_; }
  HInstruction* phi_end() const { return end_instruction(); }

  // Used for inserting register shuffle code for RA
  HInstruction* bb0_register_shuffle() const { return bb0_end(); }
  HInstruction* bb1_register_shuffle() const { return bb1_end()->prev(); }

 public:
  ir::Checkpoint* GetCheckpoint(int index) const {
    CHECK(index >= 0 && index <= static_cast<int>(checkpoint_list_.size()));
    return checkpoint_list_[index];
  }

 public:
  using VisitFunction = std::function<void(HInstruction*)>;

  void ForeachInstruction(VisitFunction&&) const;
  void ForeachInstructionBackward(VisitFunction&&) const;

  void ForeachInstructionExcludePhi(VisitFunction&&) const;
  void ForeachInstructionExcludePhiBackward(VisitFunction&&) const;

  void ForeachInstructionBB0(VisitFunction&&) const;
  void ForeachInstructionBB1(VisitFunction&&) const;
  void ForeachPhi(VisitFunction&&) const;
  std::vector<HInstruction*> ToPhiList() const;

  std::uint32_t instruction_maxid() const { return next_hinstruction_id_ - 1; }

  // O(n) complexity
  std::size_t instruction_size() const;
  std::size_t bb0_size() const;
  std::size_t bb1_size() const;
  std::size_t phi_size() const;

 private:
  HOperand* Emit1(HOpcode, HOperand*);
  HOperand* Emit2(HOpcode, HOperand*, HOperand*);
  bool VerifyFlag() const;

 private:
  Quake* quake_;

  // --------------------------------------------------------------------------
  // Current information
  // Node
  ir::Node* current_;

  // Next available instruction id
  std::uint32_t next_hinstruction_id_;

  // --------------------------------------------------------------------------
  // The instruction is been orgnized as following ----------------------------
  //
  // +-------------------+  <-- start
  // |  Basic Block #0   |  <-- BB#0
  // +-------------------+
  //  INS: Loop Test        <-- Whether first iteration is done or not
  // +-------------------+
  // |INS: XXX           |  <-- bb0_end_/bb1_start_
  // |  Basic Block #1   |  <-- BB#1
  // +-------------------+
  //  INS: Loop Test
  //  INS: Loop back
  // =====================  <-- bb1_end_ (None-phi instructions are done)
  //  INS: PHI0
  //  INS: PHI1
  //     ...
  //  INS: PHIN             <-- Phi sections
  // =====================
  //
  // Notes the bb0_register_shuffle() returns instruction of bb0_end_, ie
  // the shuffle must be inserted *BEFORE* bb0_register_shuffle(). Same, the
  // bb1_register_shuffle() returns instruction of position of loop test,
  // all the register shuffle code should be inserted before the loop back
  // instruction, which is an unconditional jump.

  // sentry instruction
  HInstruction* end_hinstruction_;

  // instruction points
  HInstruction* bb1_start_;
  HInstruction* phi_start_;

  // All the recoreded checkpoint
  std::vector<ir::Checkpoint*> checkpoint_list_;

  // Only used during the pending phase, after the flush is been called, all
  // TempRegister is been materialized into real register
  std::vector<Temporary*> temp_register_list_;

  // Maps HInstruction* along with its Label happend *after* itself. This map
  // is been setup after the bind call
  std::multimap<HInstruction*, Label*> bind_map_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_CG_LIR_ASSEMBLER_H_
