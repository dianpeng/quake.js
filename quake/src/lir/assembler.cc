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

std::size_t difference(HInstruction* start, HInstruction* end) {
  std::size_t cnt = 0;
  for (; start != end; start = start->next()) {
    ++cnt;
  }
  return cnt;
}

}  // namespace

std::size_t HAssembler::instruction_size() const {
  return difference(start_instruction(), end_instruction());
}

std::size_t HAssembler::bb0_size() const {
  return difference(bb0_start(), bb0_end());
}

std::size_t HAssembler::bb1_size() const {
  return difference(bb1_start(), bb1_end());
}

std::size_t HAssembler::phi_size() const {
  return difference(phi_start(), phi_end());
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

// ----------------------------------------------------------------------------
// Operand factory
HOperand* HAssembler::NewLabel() {
  auto label = zone()->New<Label>();
  return zone()->New<HOperand>(zone(), label);
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

#define IMPL1(NAME)                           \
  HOperand* HAssembler::NAME(HOperand* op1) { \
    return Emit1(HOpcode::k##NAME, op1);      \
  }

#define IMPLJ(JMP)                         \
  HOperand* HAssembler::JMP(HOperand* l) { \
    CHECK(VerifyFlag());                   \
    return Emit1(HOpcode::k##JMP, l);      \
  }

HOperand* HAssembler::Emit1(HOpcode opcode, HOperand* operand) {
  auto instruction =
      zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                last_instruction(), current_, opcode, operand);

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

// Check previous instruction whether it generates a flag or not. Otherwise
// the jump is incorrect
bool HAssembler::VerifyFlag() const {
  if (auto last = last_instruction(); last != end_instruction()) {
    return last->opcode_info().flag == HFlag::kSet;
  } else {
    return true;
  }
}

IMPL2(IAdd)
IMPL2(ISub)
IMPL2(IMul)
IMPL2(IDiv)

IMPL2(FAdd)
IMPL2(FSub)
IMPL2(FMul)
IMPL2(FDiv)

IMPL2(And)
IMPL2(Or)
IMPL2(Xor)
IMPL2(LShift)
IMPL2(RShift)

IMPL2(CmpF)
IMPL2(CmpI)

IMPL1(Inc)
IMPL1(Dec)
IMPL1(Neg)
IMPL1(Inv)

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

IMPL1(LoadLocal)
IMPL1(LoadParam)
IMPL1(LoadTemp)

IMPL1(Mov)
IMPL1(MovZX)
IMPL1(MovSX)
IMPL1(MovI64)
IMPL1(MovB64)
IMPL1(MovI32)
IMPL1(MovF64)

IMPL1(Pop)
IMPL1(Push)
IMPL1(Lea)

IMPL1(Box)
IMPL1(Unbox)
IMPL1(GetTTag)

void HAssembler::Bind(HOperand* opr) {
  CHECK(opr->IsLabel());
  auto current = last_instruction();
  bind_map_.emplace(std::make_pair(current, opr->GetLabel()));
}

void HAssembler::BindAfter(HInstruction* ins, HOperand* opr) {
  CHECK(opr->IsLabel());
  bind_map_.emplace(std::make_pair(ins, opr->GetLabel()));
}

}  // namespace lir
}  // namespace quake
