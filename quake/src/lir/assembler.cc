#include "lir/assembler.h"
#include "lir/operand.h"
#include "lir/instruction.h"

namespace quake {
namespace lir {

HAssembler::HAssembler(Quake* q)
    : quake_(q),
      current_(nullptr),
      next_hinstruction_id_(0),
      end_hinstruction_(
          zone()->New<HInstruction>(next_hinstruction_id_++, zone())),

      bb1_start_(end_hinstruction_),
      phi_start_(end_hinstruction_),

      temp_register_list_(),
      bind_map_() {}

// ----------------------------------------------------------------------------
// Misc
namespace {

std::size_t Difference(HInstruction* start, HInstruction* end) {
  std::size_t cnt = 0;
  for (; start != end; start = start->next()) {
    ++cnt;
  }
  return cnt;
}

}  // namespace

std::size_t HAssembler::instruction_size() const {
  return Difference(start_instruction(), end_instruction());
}

std::size_t HAssembler::bb0_size() const {
  return Difference(bb0_start(), bb0_end());
}

std::size_t HAssembler::bb1_size() const {
  return Difference(bb1_start(), bb1_end());
}

std::size_t HAssembler::phi_size() const {
  return Difference(phi_start(), phi_end());
}

void HAssembler::ForeachInstruction(VisitFunction&& v) const {
  for (auto itr = start_instruction(); itr != end_instruction();
       itr = itr->next()) {
    v(itr);
  }
}

void HAssembler::ForeachInstructionBackward(VisitFunction&& v) const {
  for (auto itr = end_instruction()->prev(); itr != end_instruction();
       itr = itr->prev()) {
    v(itr);
  }
}

void HAssembler::ForeachInstructionExcludePhi(VisitFunction&& v) const {
  for (auto itr = start_instruction(); itr != phi_start(); itr = itr->next()) {
    v(itr);
  }
}

void HAssembler::ForeachInstructionExcludePhiBackward(VisitFunction&& v) const {
  for (auto itr = phi_start()->prev(); itr != end_instruction();
       itr = itr->prev()) {
    v(itr);
  }
}

void HAssembler::ForeachInstructionBB0(VisitFunction&& v) const {
  for (auto itr = bb0_start(); itr != bb0_end(); itr = itr->next()) {
    v(itr);
  }
}

void HAssembler::ForeachInstructionBB1(VisitFunction&& v) const {
  for (auto itr = bb1_start(); itr != bb1_end(); itr = itr->next()) {
    v(itr);
  }
}

void HAssembler::ForeachPhi(VisitFunction&& v) const {
  for (auto itr = phi_start(); itr != phi_end(); itr = itr->next()) {
    v(itr);
  }
}

std::vector<HInstruction*> HAssembler::ToPhiList() const {
  std::vector<HInstruction*> x;
  ForeachPhi([&](HInstruction* ins) { x.push_back(ins); });
  return x;
}

DeoptId HAssembler::AddDeoptInfo(const ir::Checkpoint&, HInstruction*) {
  return 0;
}

// ----------------------------------------------------------------------------
// Instruction factory
HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HInstruction* after) {
  auto instruction = zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                               after->next(), current_, opcode);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HOperand* opr1,
                                              HInstruction* after) {
  auto instruction = zone()->New<HInstruction>(
      next_hinstruction_id_++, zone(), after->next(), current_, opcode, opr1);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HOperand* opr1, HOperand* opr2,
                                              HInstruction* after) {
  auto instruction =
      zone()->New<HInstruction>(next_hinstruction_id_++, zone(), after->next(),
                                current_, opcode, opr1, opr2);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              std::size_t size,
                                              HInstruction* after) {
  auto instruction = zone()->New<HInstruction>(
      next_hinstruction_id_++, zone(), after->next(), current_, opcode, size);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode,
                                               HInstruction* before) {
  auto instruction = zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                               before, current_, opcode);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode, HOperand* opr1,
                                               HInstruction* before) {
  auto instruction = zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                               before, current_, opcode, opr1);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode, HOperand* opr1,
                                               HOperand* opr2,
                                               HInstruction* before) {
  auto instruction = zone()->New<HInstruction>(
      next_hinstruction_id_++, zone(), before, current_, opcode, opr1, opr2);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode, std::size_t size,
                                               HInstruction* before) {
  auto instruction = zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                               before, current_, opcode, size);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

void HAssembler::Bind(HOperand* opr) {
  CHECK(opr->IsLabel());
  auto current = last_instruction();
  bind_map_.emplace(std::make_pair(current, opr->GetLabel()));
}

void HAssembler::BindAfter(HInstruction* ins, HOperand* opr) {
  CHECK(opr->IsLabel());
  bind_map_.emplace(std::make_pair(ins, opr->GetLabel()));
}

// ----------------------------------------------------------------------------
// Operand factory
HOperand* HAssembler::NewLabel() {
  auto label = zone()->New<Label>();
  return zone()->New<HOperand>(zone(), label);
}

HOperand* HAssembler::NewLabel(HInstruction* ins) {
  auto label = zone()->New<Label>(ins);
  auto opr = zone()->New<HOperand>(zone(), label);
  BindAfter(ins, opr);
  return opr;
}

HOperand* HAssembler::NewTemporary() {
  auto temp = zone()->New<Temporary>();
  temp_register_list_.push_back(temp);
  auto opr = zone()->New<HOperand>(zone(), temp);
  temp->owner_ = opr;
  return opr;
}

HOperand* HAssembler::NewImmediate(std::int32_t value) {
  return zone()->New<HOperand>(zone(), Immediate{value});
}

HOperand* HAssembler::NewImmediate(std::int64_t value) {
  return zone()->New<HOperand>(zone(), Immediate{value});
}

HOperand* HAssembler::NewImmediate(double value) {
  return zone()->New<HOperand>(zone(), Immediate{value});
}

HOperand* HAssembler::NewImmediate(const Immediate& imm) {
  return zone()->New<HOperand>(zone(), Immediate{imm});
}

HOperand* HAssembler::NewMemInd(MemoryIndirect&& m) {
  return zone()->New<HOperand>(zone(), std::move(m));
}

HOperand* HAssembler::NewMemStatic(MemoryStatic&& m) {
  return zone()->New<HOperand>(zone(), std::move(m));
}

HOperand* HAssembler::NewMemStack(MemoryStack&& m) {
  return zone()->New<HOperand>(zone(), std::move(m));
}

HOperand* HAssembler::NewRegister(const Register* r) {
  return zone()->New<HOperand>(zone(), r);
}

// ----------------------------------------------------------------------------
// HInstruction implementation and emitting

#define IMPL2(NAME)                                          \
  HOperand* HAssembler::NAME(HOperand* op1, HOperand* op2) { \
    return Emit2(HOpcode::k##NAME, op1, op2);                \
  }

#define IMPLG2(NAME)                                                    \
  HOperand* HAssembler::NAME(ir::Checkpoint* checkpoint, HOperand* op1, \
                             HOperand* op2) {                           \
    return EmitG2(checkpoint, HOpcode::k##NAME, op1, op2);              \
  }

#define IMPL1(NAME)                           \
  HOperand* HAssembler::NAME(HOperand* op1) { \
    return Emit1(HOpcode::k##NAME, op1);      \
  }

#define IMPLG1(NAME)                                                      \
  HOperand* HAssembler::NAME(ir::Checkpoint* checkpoint, HOperand* op1) { \
    return EmitG1(checkpoint, HOpcode::k##NAME, op1);                     \
  }

#define IMPLDEOPT(NAME)                               \
  void HAssembler::NAME(ir::Checkpoint* checkpoint) { \
    EmitG0(checkpoint, HOpcode::k##NAME);             \
  }

#define IMPLDEOPT1(NAME)                                              \
  void HAssembler::NAME(ir::Checkpoint* checkpoint, HOperand* cond) { \
    EmitG1(checkpoint, HOpcode::k##NAME, cond);                       \
  }

#define IMPLJ(NAME)                    \
  void HAssembler::NAME(HOperand* l) { \
    CHECK(l->IsLabel());               \
    CHECK(VerifyFlag());               \
    Emit1(HOpcode::k##NAME, l);        \
  }

#define IMPLCMP2(NAME)                              \
  void HAssembler::NAME(HOperand* l, HOperand* r) { \
    Emit2(HOpcode::k##NAME, l, r);                  \
  }

#define IMPLCMP1(NAME) \
  void HAssembler::NAME(HOperand* cond) { Emit1(HOpcode::k##NAME, cond); }

#define IMPLREF(NAME) \
  void HAssembler::NAME(HOperand* l) { Emit1(HOpcode::k##NAME, l); }

HOperand* HAssembler::Emit0(HOpcode opcode) {
  auto instruction = zone()->New<HInstruction>(
      next_hinstruction_id_++, zone(), last_instruction(), current_, opcode);
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::EmitG0(ir::Checkpoint* checkpoint, HOpcode opcode) {
  auto instruction = zone()->New<HInstruction>(
      next_hinstruction_id_++, zone(), last_instruction(), current_, opcode);
  instruction->set_deopt_id(AddDeoptInfo(*checkpoint, instruction));
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::Emit1(HOpcode opcode, HOperand* operand) {
  auto instruction =
      zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                last_instruction(), current_, opcode, operand);

  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::EmitG1(ir::Checkpoint* checkpoint, HOpcode opcode,
                             HOperand* operand) {
  auto instruction =
      zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                last_instruction(), current_, opcode, operand);
  instruction->set_deopt_id(AddDeoptInfo(*checkpoint, instruction));
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::Emit2(HOpcode opcode, HOperand* op1, HOperand* op2) {
  auto instruction =
      zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                last_instruction(), current_, opcode, op1, op2);

  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::EmitG2(ir::Checkpoint* checkpoint, HOpcode opcode,
                             HOperand* op1, HOperand* op2) {
  auto instruction =
      zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                last_instruction(), current_, opcode, op1, op2);
  instruction->set_deopt_id(AddDeoptInfo(*checkpoint, instruction));
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

// Check previous instruction whether it generates a flag or not. Otherwise
// the jump is incorrect
bool HAssembler::VerifyFlag() const {
  if (auto last = last_instruction(); last != end_instruction()) {
    return last->opcode_info().flag == HFlag::kSet;
  } else {
    return true;
  }
}

void HAssembler::Barrier() {
  auto instruction = zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                               last_instruction(), current_,
                                               HOpcode::kBarrier);
  instruction->PopulateOutput(this);
}

HOperand* HAssembler::Block(HInstruction* ins) {
  auto label = NewLabel(ins);
  return Emit1(HOpcode::kBlock, label);
}

HOperand* HAssembler::BlockCurrent() { return Block(last_instruction()); }

IMPLG2(GIAdd)
IMPL2(IAdd)
IMPL2(FAdd)

IMPLG2(GISub)
IMPL2(ISub)
IMPL2(FSub)

IMPLG2(GIMul)
IMPL2(IMul)
IMPL2(FMul)

IMPL2(IDiv)
IMPL2(FDiv)

IMPL2(IMod)
IMPL2(FMod)

IMPL2(IPow)
IMPL2(FPow)

IMPL2(And)
IMPL2(Or)
IMPL2(Xor)
IMPL2(LShift)
IMPL2(RShift)
IMPL2(RLShift)

// unary
IMPLG1(GInc)
IMPL1(Inc)
IMPL1(IncF)

IMPLG1(GDec)
IMPL1(Dec)
IMPL1(DecF)

IMPLG1(GNeg)
IMPL1(Neg)
IMPL1(NegF)

IMPL1(Inv)

// Comparison Value
IMPL2(ILt)
IMPL2(ILe)
IMPL2(IGt)
IMPL2(IGe)
IMPL2(IEq)
IMPL2(INe)

IMPL2(FLt)
IMPL2(FLe)
IMPL2(FGt)
IMPL2(FGe)
IMPL2(FEq)
IMPL2(FNe)

// ============================================================================
// Special ir, ie compare target aganist another 2 operands(c1/c2)
HOperand* HAssembler::I2Eq(HOperand* target, HOperand* c1, HOperand* c2) {
  auto instruction = zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                               last_instruction(), current_,
                                               HOpcode::kI2Eq, 3);

  instruction->OprSet(0, target);
  instruction->OprSet(1, c1);
  instruction->OprSet(2, c2);

  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

// Comparison Ops
IMPLCMP2(CmpF)
IMPLCMP2(CmpI)
IMPLCMP1(CmpTrue)
IMPLCMP1(CmpFalse)
IMPLCMP1(CmpBoxIsHeap)
IMPLCMP1(CmpBoxIsPrimitive)

// Conditional Jump
IMPLJ(JmpNe)
IMPLJ(JmpEq)
IMPLJ(JmpLt)
IMPLJ(JmpLe)
IMPLJ(JmpBt)
IMPLJ(JmpBe)
IMPLJ(JmpGt)
IMPLJ(JmpGe)
IMPLJ(JmpAt)
IMPLJ(JmpAe)
IMPLJ(JmpNz)
IMPLJ(JmpZ)
IMPLJ(Jmp)

// Cast
IMPL1(CastI32ToF64)
IMPL1(CastF64ToB32)

// Special operation
IMPL1(LoadLocal)
IMPL1(LoadParam)
IMPL1(LoadTemp)

// Deoptimization
IMPLDEOPT(Deopt)
IMPLDEOPT1(DeoptIfTrue)
IMPLDEOPT1(DeoptIfFalse)

// Movs
IMPL1(Mov)
IMPL1(MovZX)
IMPL1(MovSX)
IMPL1(MovI64)
IMPL1(MovB64)
IMPL1(MovI32)
IMPL1(MovF64)

// Memory
IMPL1(Lea)

// Box and Unbox
IMPL2(Box)
IMPLG2(UnboxMaybe)
IMPL1(UnboxPtr)
IMPL1(GetTTag)

// Ref count
IMPLREF(RcInc)
IMPLREF(RcDec)

// ----------------------------------------------------------------------------
// Control Flow Blocks
HOperand* HAssembler::AddBlock(WellKnownCFGNode* out) {
  CHECK(!out->IsValid());
  auto instruction =
      zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                last_instruction(), current_, HOpcode::kBlock);
  instruction->PopulateOutput(this);
  out->ins = instruction;
  out->label = NewLabel(instruction);
  return out->label;
}

HOperand* HAssembler::BB0() { return AddBlock(&bb0_); }

HOperand* HAssembler::BB0Label() {
  CHECK(bb0_.IsValid());
  return bb0_.label;
}

HOperand* HAssembler::Start() { return AddBlock(&start_); }

HOperand* HAssembler::End() { return AddBlock(&end_); }

// ----------------------------------------------------------------------------
// PHIs
HOperand* HAssembler::PhiLoop() {
  auto instruction = zone()->New<HInstruction>(
      next_hinstruction_id_++, zone(), last_instruction(), current_,
      HOpcode::kPhiLoop, nullptr, nullptr);

  return instruction->PopulateOutput(this);
}

// ----------------------------------------------------------------------------
// Begin/End opcode
void HAssembler::BeginOpcode(ir::Node* node) { current_ = node; }

void HAssembler::EndOpcode() { current_ = nullptr; }

}  // namespace lir
}  // namespace quake
