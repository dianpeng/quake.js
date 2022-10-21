#ifndef _QUAKE_CG_LIR_ASSEMBLER_H_
#define _QUAKE_CG_LIR_ASSEMBLER_H_
#include "base.h"
#include "quake.h"
#include "common.h"
#include "deopt.h"
#include "stringify.h"

#include "lir/builtin.h"
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

class Translator;

class HAssembler : public Stringify {
 public:
  explicit HAssembler(Quake*);

  Quake* quake() const { return quake_; }
  zone::Zone* zone() const { return quake()->zone(); }

 public:
  // Operand factory
  HOperand* NewLabel();
  HOperand* NewLabel(HInstruction*);

  HOperand* NewTemporary();
  HOperand* NewImmediate(std::int32_t);
  HOperand* NewImmediate(std::int64_t);
  HOperand* NewImmediate(double);
  HOperand* NewImmediate(const Immediate&);

  // new a builtin function call index
  HOperand* NewBuiltinCallIndex(BuiltinFunction idx) {
    return NewImmediate(static_cast<std::int32_t>(idx));
  }

  HOperand* NewInt32Zero() {
    return NewImmediate(static_cast<std::int32_t>(0));
  }
  HOperand* NewInt32Max() {
    return NewImmediate(std::numeric_limits<std::int32_t>::max());
  }
  HOperand* NewInt32Min() {
    return NewImmediate(std::numeric_limits<std::int32_t>::min());
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

  /* ########################################################################*\
  |* ################ The following APIs are used for Translator ############*|
  \* ########################################################################*/

 private:  // the private is on purpose, do not expose them out
  void BeginOpcode(ir::Node*);

  ir::Node* current() const { return current_; }

  void EndOpcode();

  // Called when the whole graph is constructed, ie after finishing the
  // whole ir::Graph to lir::Assembler conversion via lir::Translator
  void Finalize();

  // --------------------------------------------------------------------------
  // Form a barrier to disable optimization to move cross this barrier node
  void Barrier();
  HOperand* Block(HInstruction*);
  HOperand* BlockCurrent();

  // --------------------------------------------------------------------------
  // Arithmetic operations
  // HInstruction builder APIs, used with BeginOpcode and EndOpcode
  HOperand* GIAdd(ir::Checkpoint*, HOperand*, HOperand*);
  HOperand* IAdd(HOperand*, HOperand*);
  HOperand* FAdd(HOperand*, HOperand*);

  HOperand* GISub(ir::Checkpoint*, HOperand*, HOperand*);
  HOperand* ISub(HOperand*, HOperand*);
  HOperand* FSub(HOperand*, HOperand*);

  HOperand* GIMul(ir::Checkpoint*, HOperand*, HOperand*);
  HOperand* IMul(HOperand*, HOperand*);
  HOperand* FMul(HOperand*, HOperand*);

  HOperand* IDiv(HOperand*, HOperand*);
  HOperand* FDiv(HOperand*, HOperand*);

  HOperand* IMod(HOperand*, HOperand*);
  HOperand* FMod(HOperand*, HOperand*);

  HOperand* FPow(HOperand*, HOperand*);

  // --------------------------------------------------------------------------
  // Bit operations
  HOperand* And(HOperand*, HOperand*);
  HOperand* Or(HOperand*, HOperand*);
  HOperand* Xor(HOperand*, HOperand*);
  HOperand* LShift(HOperand*, HOperand*);
  HOperand* RShift(HOperand*, HOperand*);
  HOperand* RLShift(HOperand*, HOperand*);

  // --------------------------------------------------------------------------
  // Unary operations
  HOperand* GInc(ir::Checkpoint*, HOperand*);
  HOperand* Inc(HOperand*);
  HOperand* IncF(HOperand*);

  HOperand* GDec(ir::Checkpoint*, HOperand*);
  HOperand* Dec(HOperand*);
  HOperand* DecF(HOperand*);

  HOperand* GNeg(ir::Checkpoint*, HOperand*);
  HOperand* Neg(HOperand*);
  HOperand* NegF(HOperand*);

  HOperand* Inv(HOperand*);

  // --------------------------------------------------------------------------
  // Comparison value
  HOperand* ILt(HOperand*, HOperand*);
  HOperand* ILe(HOperand*, HOperand*);
  HOperand* IGt(HOperand*, HOperand*);
  HOperand* IGe(HOperand*, HOperand*);
  HOperand* IEq(HOperand*, HOperand*);
  HOperand* I2Eq(HOperand*, HOperand*, HOperand*);
  HOperand* INe(HOperand*, HOperand*);

  HOperand* FLt(HOperand*, HOperand*);
  HOperand* FLe(HOperand*, HOperand*);
  HOperand* FGt(HOperand*, HOperand*);
  HOperand* FGe(HOperand*, HOperand*);
  HOperand* FEq(HOperand*, HOperand*);
  HOperand* FNe(HOperand*, HOperand*);

  HOperand* BoxIsHeap(HOperand*);
  HOperand* BoxIsPrimitive(HOperand*);

  void BranchTrue(HOperand*, HOperand*);
  void BranchFalse(HOperand*, HOperand*);

  // --------------------------------------------------------------------------
  // Comparison operations
  void CmpF(HOperand*, HOperand*);
  void CmpI(HOperand*, HOperand*);
  void CmpTest(HOperand*);
  void CmpBoxIsHeap(HOperand*);
  void CmpBoxIsPrimitive(HOperand*);

  // --------------------------------------------------------------------------
  // jump operations
  void JmpNe(HOperand*);
  void JmpEq(HOperand*);
  void JmpLt(HOperand*);
  void JmpLe(HOperand*);
  void JmpBt(HOperand*);
  void JmpBe(HOperand*);
  void JmpGt(HOperand*);
  void JmpGe(HOperand*);
  void JmpAt(HOperand*);
  void JmpAe(HOperand*);
  void JmpNz(HOperand*);
  void JmpZ(HOperand*);
  void Jmp(HOperand*);

  // --------------------------------------------------------------------------
  // Cast
  HOperand* CastI32ToF64(HOperand*);
  HOperand* CastF64ToB32(HOperand*);

  // --------------------------------------------------------------------------
  HOperand* LoadLocal(HOperand*);
  HOperand* LoadParam(HOperand*);
  HOperand* LoadTemp(HOperand*);

  // --------------------------------------------------------------------------
  // Deoptimzation
  void Deopt(ir::Checkpoint*);
  void DeoptIfTrue(ir::Checkpoint*, HOperand*);
  void DeoptIfFalse(ir::Checkpoint*, HOperand*);

  HInstruction* Snapshot(std::size_t);

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

  // --------------------------------------------------------------------------
  // Misc memory
  HOperand* Lea(HOperand*);

  // --------------------------------------------------------------------------
  // Box and Unbox
  HOperand* GUnbox(ir::Checkpoint*, HOperand*, HOperand*);
  HOperand* UnboxPtr(HOperand*);
  HOperand* GetTTag(HOperand*);

  // --------------------------------------------------------------------------
  // Reference counting related
  void RcInc(HOperand*);
  void RcDec(HOperand*);

  // --------------------------------------------------------------------------
  // Label
  void Bind(HOperand*);
  void BindAfter(HInstruction*, HOperand*);

  // --------------------------------------------------------------------------
  // Phi
  HOperand* PhiLoopF();
  HOperand* PhiLoopI();

  // --------------------------------------------------------------------------
  // Call

  // --------------------------------------------------------------------------
  // Blocks(CFG)
  HOperand* Start();
  HOperand* BB1();
  HOperand* BB1Label();
  HOperand* End();

  friend class Translator;

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

  // Well known control flow blocks. notes user is not supposed to modify
  // these well known control flow graph node. I mean, I don't know the
  // point of modifying them :)
  HInstruction* start() const {
    CHECK(start_.IsValid());
    return start_.ins;
  }

  HInstruction* end() const {
    CHECK(end_.IsValid());
    return end_.ins;
  }

  HInstruction* bb1() const {
    CHECK(bb1_.IsValid());
    return bb1_.ins;
  }

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

  std::string Print() const override;

 private:
  HOperand* EmitG0(ir::Checkpoint*, HOpcode);
  HOperand* Emit0(HOpcode);

  HOperand* Emit1(HOpcode, HOperand*);
  HOperand* EmitG1(ir::Checkpoint*, HOpcode, HOperand*);

  HOperand* Emit2(HOpcode, HOperand*, HOperand*);
  HOperand* EmitG2(ir::Checkpoint*, HOpcode, HOperand*, HOperand*);

  DeoptId AddDeoptInfo(const ir::Checkpoint&, HInstruction*);

 private:
  struct WellKnownCFGNode {
    HInstruction* ins = nullptr;
    HOperand* label = nullptr;
    bool IsValid() const { return ins != nullptr && label != nullptr; }
  };
  HOperand* AddBlock(HOpcode, WellKnownCFGNode*);

 private:
  // Instruction construction
  template <typename... ARGS>
  HInstruction* NewInstruction(std::uint32_t index, zone::Zone* z,
                               HInstruction* prev, ir::Node* ir_node,
                               HOpcode opcode, ARGS&&... args) {
    auto ins = zone()->New<HInstruction>(index, z, prev, ir_node, opcode,
                                         std::forward<ARGS>(args)...);
    return ins;
  }

  template <typename... ARGS>
  HInstruction* MkInstruction(HOpcode opcode, ARGS&&... args) {
    return NewInstruction(next_hinstruction_id_++, zone(), last_instruction(),
                          current_, opcode, std::forward<ARGS>(args)...);
  }

  template <typename... ARGS>
  HInstruction* MkInstructionBefore(ir::Node* ir_node, HInstruction* before,
                                    HOpcode opcode, ARGS&&... args) {
    return NewInstruction(next_hinstruction_id_++, zone(), before->prev(),
                          ir_node, opcode, std::forward<ARGS>(args)...);
  }

  template <typename... ARGS>
  HInstruction* MkInstructionAfter(ir::Node* ir_node, HInstruction* pos,
                                   HOpcode opcode, ARGS&&... args) {
    return NewInstruction(next_hinstruction_id_++, zone(), pos, ir_node, opcode,
                          std::forward<ARGS>(args)...);
  }

 private:
  Quake* quake_;

  // --------------------------------------------------------------------------
  // Current information
  // Node
  ir::Node* current_;

  // Special CFG node and its data
  WellKnownCFGNode start_;
  WellKnownCFGNode bb1_;
  WellKnownCFGNode end_;

  // record all the phi node that just been emitted during the construction,
  // and move them at the end of the graph
  std::vector<HInstruction*> pending_phi_;

  // list of invalid label needs to be patched after the graph is been generated
  std::vector<Label*> pending_label_;

  // --------------------------------------------------------------------------
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
