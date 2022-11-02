#include "lir/assembler.h"
#include "lir/operand.h"
#include "lir/instruction.h"
#include "ir/checkpoint.h"

namespace quake {
namespace lir {

HAssembler::HAssembler(Quake* q)
    : quake_(q),
      stack_slot_(),
      current_(nullptr),

      start_(),
      bb1_(),
      end_(),
      pending_phi_(),
      pending_label_(),

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

ir::Checkpoint* HAssembler::GetCheckpoint(DeoptId did) const {
  auto itr = checkpoint_map_.find(did);
  if (itr == checkpoint_map_.end()) {
    return nullptr;
  }
  return itr->second;
}

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
  for (auto itr = start_instruction(); itr != end_instruction();) {
    auto next = itr->next();
    v(itr);
    itr = next;
  }
}

bool HAssembler::ForeachInstructionStoppable(StoppableVisitFunction&& v) const {
  for (auto itr = start_instruction(); itr != end_instruction();) {
    auto next = itr->next();
    if (!v(itr)) return false;
    itr = next;
  }
  return true;
}

void HAssembler::ForeachInstructionBackward(VisitFunction&& v) const {
  for (auto itr = end_instruction()->prev(); itr != end_instruction();) {
    auto prev = itr->prev();
    v(itr);
    itr = prev;
  }
}

void HAssembler::ForeachInstructionExcludePhi(VisitFunction&& v) const {
  for (auto itr = start_instruction(); itr != phi_start();) {
    auto next = itr->next();
    v(itr);
    itr = next;
  }
}

void HAssembler::ForeachInstructionExcludePhiBackward(VisitFunction&& v) const {
  for (auto itr = phi_start()->prev(); itr != end_instruction();) {
    auto prev = itr->prev();
    v(itr);
    itr = prev;
  }
}

void HAssembler::ForeachInstructionBB0(VisitFunction&& v) const {
  for (auto itr = bb0_start(); itr != bb0_end();) {
    auto next = itr->next();
    v(itr);
    itr = next;
  }
}

void HAssembler::ForeachInstructionBB1(VisitFunction&& v) const {
  for (auto itr = bb1_start(); itr != bb1_end();) {
    auto next = itr->next();
    v(itr);
    itr = next;
  }
}

void HAssembler::ForeachPhi(VisitFunction&& v) const {
  for (auto itr = phi_start(); itr != phi_end();) {
    auto next = itr->next();
    CHECK(itr->IsPhi());
    v(itr);
    itr = next;
  }
}

std::vector<HInstruction*> HAssembler::ToPhiList() const {
  std::vector<HInstruction*> x;
  ForeachPhi([&](HInstruction* ins) { x.push_back(ins); });
  return x;
}

DeoptId HAssembler::AddDeoptInfo(ir::Checkpoint* checkpoint,
                                 HInstruction* ins) {
  UNUSE(ins);

  if (checkpoint->HasDeoptId()) return checkpoint->deopt_id();

  // FIXME(dpeng): Add correct reason mapping
  auto deopt_id =
      quake_->AddDeopt(DeoptReason::kGuard, DeoptNext::kContinueInterp);

  checkpoint->set_deopt_id(deopt_id);
  checkpoint_map_.insert(std::make_pair(deopt_id, checkpoint));
  return deopt_id;
}

HInstruction* HAssembler::NewPlaceholderBefore(HInstruction* before) {
  auto ins = zone()->New<HInstruction>(next_hinstruction_id_++, zone(),
                                       before->prev());
  return ins;
}

HInstruction* HAssembler::NewPlaceholderAfter(HInstruction* after) {
  auto ins = zone()->New<HInstruction>(next_hinstruction_id_++, zone(), after);
  return ins;
}

HInstruction* HAssembler::MaterializePlaceholder(HInstruction* placeholder,
                                                 ir::Node* ir_node,
                                                 HOpcode opcode) {
  placeholder->MaterializePlaceholder(ir_node, opcode, 0);
  CHECK(placeholder->VerifyOperand());
  placeholder->PopulateOutput(this);
  return placeholder;
}

HInstruction* HAssembler::MaterializePlaceholder(HInstruction* placeholder,
                                                 ir::Node* ir_node,
                                                 HOpcode opcode,
                                                 HOperand* opr) {
  placeholder->MaterializePlaceholder(ir_node, opcode, 1);
  placeholder->OprSet(0, opr);
  CHECK(placeholder->VerifyOperand());
  placeholder->PopulateOutput(this);
  return placeholder;
}

HInstruction* HAssembler::MaterializePlaceholder(HInstruction* placeholder,
                                                 ir::Node* ir_node,
                                                 HOpcode opcode, HOperand* opr1,
                                                 HOperand* opr2) {
  placeholder->MaterializePlaceholder(ir_node, opcode, 2);
  placeholder->OprSet(0, opr1);
  placeholder->OprSet(1, opr2);
  CHECK(placeholder->VerifyOperand());
  placeholder->PopulateOutput(this);
  return placeholder;
}

HInstruction* HAssembler::MaterializePlaceholder(HInstruction* placeholder,
                                                 ir::Node* ir_node,
                                                 HOpcode opcode,
                                                 std::size_t size) {
  placeholder->MaterializePlaceholder(ir_node, opcode, size);
  CHECK(placeholder->VerifyOperand());
  placeholder->PopulateOutput(this);
  return placeholder;
}

// ----------------------------------------------------------------------------
// Instruction factory
HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HInstruction* after) {
  auto instruction = MkInstructionAfter(ir_node, after->next(), opcode);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HOperand* opr1,
                                              HInstruction* after) {
  auto instruction = MkInstructionAfter(ir_node, after->next(), opcode, opr1);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HOperand* opr1, HOperand* opr2,
                                              HInstruction* after) {
  auto instruction =
      MkInstructionAfter(ir_node, after->next(), opcode, opr1, opr2);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              std::size_t size,
                                              HInstruction* after) {
  auto instruction = MkInstructionAfter(ir_node, after->next(), opcode, size);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode,
                                               HInstruction* before) {
  auto instruction = MkInstructionBefore(ir_node, before, opcode);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode, HOperand* opr1,
                                               HInstruction* before) {
  auto instruction = MkInstructionBefore(ir_node, before, opcode, opr1);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode, HOperand* opr1,
                                               HOperand* opr2,
                                               HInstruction* before) {
  auto instruction = MkInstructionBefore(ir_node, before, opcode, opr1, opr2);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionBefore(ir::Node* ir_node,
                                               HOpcode opcode, std::size_t size,
                                               HInstruction* before) {
  auto instruction = MkInstructionBefore(ir_node, before, opcode, size);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

void HAssembler::Bind(HOperand* opr) {
  CHECK(opr->IsLabel());
  auto l = opr->GetLabel();
  CHECK(!l->HasBind());
  l->set_bind(last_instruction());
  pending_label_.push_back(l);
}

void HAssembler::BindAfter(HInstruction* ins, HOperand* opr) {
  CHECK(opr->IsLabel());
  auto l = opr->GetLabel();
  CHECK(!l->HasBind());
  l->set_bind(ins);
  bind_map_.emplace(std::make_pair(ins, l));
}

// ----------------------------------------------------------------------------
// Operand factory
HOperand* HAssembler::NewLabel() {
  auto label = zone()->New<Label>();
  return zone()->New<HOperand>(zone(), label);
}

HOperand* HAssembler::NewLabel(HInstruction* ins) {
  auto label = zone()->New<Label>();
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
    Emit1(HOpcode::k##NAME, l);        \
  }

#define IMPLBR(NAME)                                       \
  void HAssembler::NAME(HOperand* cond, HOperand* label) { \
    CHECK(label->IsLabel());                               \
    Emit2(HOpcode::k##NAME, cond, label);                  \
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
  auto instruction = MkInstruction(opcode);
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::EmitG0(ir::Checkpoint* checkpoint, HOpcode opcode) {
  auto instruction = MkInstruction(opcode);
  instruction->set_deopt_id(AddDeoptInfo(checkpoint, instruction));
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::Emit1(HOpcode opcode, HOperand* operand) {
  auto instruction = MkInstruction(opcode, operand);

  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::EmitG1(ir::Checkpoint* checkpoint, HOpcode opcode,
                             HOperand* operand) {
  auto instruction = MkInstruction(opcode, operand);
  instruction->set_deopt_id(AddDeoptInfo(checkpoint, instruction));
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::Emit2(HOpcode opcode, HOperand* op1, HOperand* op2) {
  auto instruction = MkInstruction(opcode, op1, op2);
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::EmitG2(ir::Checkpoint* checkpoint, HOpcode opcode,
                             HOperand* op1, HOperand* op2) {
  auto instruction = MkInstruction(opcode, op1, op2);
  instruction->set_deopt_id(AddDeoptInfo(checkpoint, instruction));
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

void HAssembler::Barrier() {
  auto instruction = MkInstruction(HOpcode::kBarrier);
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

IMPL2(FDiv)

IMPL2(IMod)
IMPL2(FMod)

IMPL2(FPow)

IMPL2(And)
IMPL2(Or)
IMPL2(Xor)

IMPL2(Shl)
IMPL2(Shr)
IMPL2(Sar)

IMPL2(ShlConst)
IMPL2(ShrConst)
IMPL2(SarConst)

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

IMPL1(BoxIsHeap)
IMPL1(BoxIsPrimitive)

IMPLBR(BranchTrue)
IMPLBR(BranchFalse)

// ============================================================================
// Comparison Ops
IMPLCMP2(CmpF)
IMPLCMP2(CmpI)
IMPLCMP1(CmpTest)
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
HOperand* HAssembler::LoadLocal(HOperand* op1) {
  auto out = Emit1(HOpcode::kLoadLocal, op1);
  out->instruction_checked()->set_cell_index(stack_slot()->AddCell());
  return out;
}

HOperand* HAssembler::LoadParam(HOperand* op1) {
  auto out = Emit1(HOpcode::kLoadParam, op1);
  out->instruction_checked()->set_cell_index(stack_slot()->AddCell());
  return out;
}

HOperand* HAssembler::LoadTemp(HOperand* op1) {
  auto out = Emit1(HOpcode::kLoadTemp, op1);
  out->instruction_checked()->set_cell_index(stack_slot()->AddCell());
  return out;
}

// Deoptimization
IMPLDEOPT(Deopt)
IMPLDEOPT1(DeoptIfTrue)
IMPLDEOPT1(DeoptIfFalse)

HInstruction* HAssembler::Snapshot(std::size_t size) {
  auto instruction = MkInstruction(HOpcode::kSnapshot, size);
  instruction->PopulateOutput(this);
  return instruction;
}

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
IMPLG1(GUnboxI)
IMPLG1(GUnboxF)

IMPL1(UnboxPtr)
IMPL1(GetTTag)
IMPL1(IsNullOrUndefined)

// Ref count
IMPLREF(RcInc)
IMPLREF(RcDec)

// ----------------------------------------------------------------------------
// Control Flow Blocks
HOperand* HAssembler::AddBlock(HOpcode opcode, WellKnownCFGNode* out) {
  CHECK(!out->IsValid());
  auto instruction = MkInstruction(opcode);
  instruction->PopulateOutput(this);
  out->ins = instruction;
  out->label = NewLabel(instruction);
  return out->label;
}

HOperand* HAssembler::BB1() {
  auto l = AddBlock(HOpcode::kBB1, &bb1_);
  CHECK(l->IsLabel());
  auto label = l->GetLabel();
  CHECK(label->HasBind());
  bb1_start_ = label->bind();
  return l;
}

HOperand* HAssembler::BB1Label() {
  CHECK(bb1_.IsValid());
  return bb1_.label;
}

HOperand* HAssembler::Start() { return AddBlock(HOpcode::kStart, &start_); }

HOperand* HAssembler::End() { return AddBlock(HOpcode::kEnd, &end_); }

// ----------------------------------------------------------------------------
// PHIs
HOperand* HAssembler::PhiLoopI() {
  auto instruction = MkInstruction(HOpcode::kPhiLoopI, nullptr, nullptr);
  pending_phi_.push_back(instruction);
  // do not verify operand since operand is not setup yet
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::PhiLoopF() {
  auto instruction = MkInstruction(HOpcode::kPhiLoopF, nullptr, nullptr);
  pending_phi_.push_back(instruction);
  // do not verify operand since operand is not setup yet
  return instruction->PopulateOutput(this);
}

// ----------------------------------------------------------------------------
// Begin/End opcode
void HAssembler::BeginOpcode(ir::Node* node) {
  CHECK(!end_.IsValid());
  current_ = node;
}

void HAssembler::EndOpcode() { current_ = nullptr; }

void HAssembler::Finalize() {
  // (0) all the binding is incorrect, since they are pointing to the previous
  //     instruction, now rebind them correctly and fix the bind_map_
  for (auto& label : pending_label_) {
    CHECK(label->HasBind());
    auto ins = label->bind()->next();
    label->set_bind(ins);
    bind_map_.insert(std::make_pair(ins, label));
  }

  // (1) the phi is still in the middle of the graph, shuffle them at the end
  //     of the graph for RA
  {
    for (auto& phi : pending_phi_) {
      phi->InsertBefore(end_instruction());
    }
    pending_phi_.clear();
    phi_start_ = end_.ins->next();
  }
}

// ----------------------------------------------------------------------------
// String
std::string HAssembler::Print() const {
  std::stringstream fmt;
  ForeachInstruction([&](HInstruction* ins) { fmt << ins->Print() << "\n"; });
  return fmt.str();
}

}  // namespace lir
}  // namespace quake
