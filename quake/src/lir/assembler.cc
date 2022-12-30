#include "lir/assembler.h"
#include "lir/operand.h"
#include "lir/instruction.h"
#include "ir/checkpoint.h"

namespace quake {
namespace lir {

HAssembler::HAssembler(Quake* q)
    : quake_(q),
      stack_slot_(),

      null_(nullptr),
      undefined_(nullptr),
      uninitialized_(nullptr),

      current_(nullptr),

      start_(),
      bb1_(),
      end_(),
      pending_loop_phi_(),
      pending_label_(),

      next_hinstruction_id_(0),
      end_hinstruction_(
          zone()->New<HInstruction>(next_hinstruction_id_++, zone())),

      bb1_start_(end_hinstruction_),
      loop_phi_start_(end_hinstruction_),

      temp_register_list_(),
      bind_map_() {
  auto mk_dummy = [&](HOpcode opcode) -> HOperand* {
    auto ins =
        zone()->New<HInstruction>(next_hinstruction_id_++, zone(), opcode);
    return ins->PopulateOutput(this);
  };

  null_ = mk_dummy(HOpcode::kNull);
  undefined_ = mk_dummy(HOpcode::kUndefined);
  uninitialized_ = mk_dummy(HOpcode::kUninitialized);
}

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
  return Difference(loop_phi_start(), loop_phi_end());
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

void HAssembler::ForeachInstructionExcludeLoopPhi(VisitFunction&& v) const {
  for (auto itr = start_instruction(); itr != loop_phi_start();) {
    auto next = itr->next();
    v(itr);
    itr = next;
  }
}

void HAssembler::ForeachInstructionExcludeLoopPhiBackward(
    VisitFunction&& v) const {
  for (auto itr = loop_phi_start()->prev(); itr != end_instruction();) {
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

void HAssembler::ForeachLoopPhi(VisitFunction&& v) const {
  for (auto itr = loop_phi_start(); itr != loop_phi_end();) {
    auto next = itr->next();
    CHECK(itr->IsLoopPhi());
    v(itr);
    itr = next;
  }
}

void HAssembler::ForeachBranchPhi(VisitFunction&& v) const {
  for (auto x : branch_phi_list_) {
    v(x);
  }
}

std::vector<HInstruction*> HAssembler::ToLoopPhiList() const {
  std::vector<HInstruction*> x;
  ForeachLoopPhi([&](HInstruction* ins) { x.push_back(ins); });
  return x;
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
  auto instruction = MkInstructionAfter(ir_node, after, opcode);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HOperand* opr1,
                                              HInstruction* after) {
  auto instruction = MkInstructionAfter(ir_node, after, opcode, opr1);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              HOperand* opr1, HOperand* opr2,
                                              HInstruction* after) {
  auto instruction = MkInstructionAfter(ir_node, after, opcode, opr1, opr2);

  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

HInstruction* HAssembler::NewInstructionAfter(ir::Node* ir_node, HOpcode opcode,
                                              std::size_t size,
                                              HInstruction* after) {
  auto instruction = MkInstructionAfter(ir_node, after, opcode, size);
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

HOperand* HAssembler::NewNone() {
  auto opr = zone()->New<HOperand>(zone());
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

HOperand* HAssembler::NewImmediate(std::uint32_t value) {
  return zone()->New<HOperand>(zone(),
                               Immediate{static_cast<std::int32_t>(value)});
}

HOperand* HAssembler::NewImmediate(std::int64_t value) {
  return zone()->New<HOperand>(zone(), Immediate{value});
}

HOperand* HAssembler::NewImmediate(void* ptr) {
  return zone()->New<HOperand>(zone(), Immediate{ptr});
}

HOperand* HAssembler::NewImmediate(double value) {
  return zone()->New<HOperand>(zone(), Immediate{value});
}

HOperand* HAssembler::NewImmediate(const Immediate& imm) {
  return zone()->New<HOperand>(zone(), Immediate{imm});
}

HOperand* HAssembler::NewString(const zone::String& str) {
  auto new_str =
      zone()->New<zone::String>(zone(), str);  // duplicate this string
  auto imm =
      NewImmediate(static_cast<void*>(new_str));  // store the raw pointer
  return imm;
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
#define IMPL0(NAME) \
  HOperand* HAssembler::NAME() { return Emit0(HOpcode::k##NAME); }

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

#define IMPLREF1(NAME) \
  void HAssembler::NAME(HOperand* l) { Emit1(HOpcode::k##NAME, l); }

#define IMPLREF2(NAME)                              \
  void HAssembler::NAME(HOperand* l, HOperand* r) { \
    Emit2(HOpcode::k##NAME, l, r);                  \
  }

HOperand* HAssembler::Emit0(HOpcode opcode) {
  auto instruction = MkInstruction(opcode);
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::EmitG0(ir::Checkpoint* checkpoint, HOpcode opcode) {
  auto instruction = MkInstruction(opcode);
  instruction->set_exit(checkpoint);
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
  instruction->set_exit(checkpoint);
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
  instruction->set_exit(checkpoint);
  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HInstruction* HAssembler::EmitS(HOpcode opcode, std::size_t size) {
  return MkInstruction(opcode, size);
}

HInstruction* HAssembler::EmitGS(ir::Checkpoint* checkpoint, HOpcode opcode,
                                 std::size_t size) {
  auto instruction = MkInstruction(opcode, size);
  instruction->set_exit(checkpoint);
  return instruction;
}

void HAssembler::Barrier() {
  auto instruction = MkInstruction(HOpcode::kBarrier);
  instruction->PopulateOutput(this);
}

HInstruction* HAssembler::BlockCurrent() {
  auto instruction = MkInstruction(HOpcode::kBlock);
  CHECK(instruction->VerifyOperand());
  instruction->PopulateOutput(this);
  return instruction;
}

// FastSlow path generation. The order of 2 BB is really important for RA
// assumption. We do not run everything on a well-ordered CFG for now, we
// will finally do that for hyper-block scheduling, ie allow none-linear
// branch to form inside of trace.

HOperand* HAssembler::FastSlow(
    std::function<HOperand*(HAssembler*, HOperand*)>&& fast_path,
    HOpcode rcall_type, BuiltinFunction builtin_idx, HOperand* arg0,
    HOperand* arg1, HOperand* arg2) {
  HOperand *lhs = nullptr, *rhs = nullptr;
  HInstruction *lhs_block = nullptr, *rhs_block = nullptr;

  auto done = NewLabel();

  // 0) generate fast path code, make sure the last instruction is a jmp
  lhs = fast_path(this, done);

  CHECK(last_instruction()->IsJump() || last_instruction()->IsBranch());

  for (auto none_jump = last_instruction()->prev();
       none_jump != end_instruction(); none_jump = none_jump->prev()) {
    if (!none_jump->IsJump() && !none_jump->IsBranch()) {
      lhs_block = NewInstructionAfter(current_, HOpcode::kBlock, none_jump);
      break;
    }
  }
  CHECK(lhs_block);

  // 1) generate slow path, ie the RCall code
  {
    Emit0(HOpcode::kSlowBlockStart);  // mark for RA, a slow block starts

    auto param_size = 1;
    if (arg0) param_size++;
    if (arg1) param_size++;
    if (arg2) param_size++;

    auto call_idx = NewBuiltinCallIndex(builtin_idx);
    auto rcall = EmitS(rcall_type, param_size);
    CHECK(rcall->IsRCall());

    rcall->OprSet(0, call_idx);

    if (arg0) rcall->OprSet(1, arg0);
    if (arg1) rcall->OprSet(2, arg1);
    if (arg2) rcall->OprSet(3, arg2);

    CHECK(rcall->VerifyOperand());
    rhs = rcall->PopulateOutput(this);

    Emit0(HOpcode::kSlowBlockEnd);  // mark the end of the slow block

    // lastly generate a rhs_block for branch PHI node
    rhs_block = BlockCurrent();
  }
  CHECK(rhs_block);

  if (!lhs) {
    // If user does not generate any value, just return nullptr. Notes both
    // operand should be the same
    CHECK(!rhs);
    return nullptr;
  } else {
    CHECK(rhs);

    // 2) generate branch phi
    Bind(done);

    // 3) generate the final lhs/rhs value based on the type
    switch (lhs->htype()) {
      case HType::kBoxRef:
        return PhiBranchBox(lhs, rhs, lhs_block, rhs_block);

      case HType::kInt:
        return PhiBranchI(lhs, rhs, lhs_block, rhs_block);

      case HType::kFloat:
        return PhiBranchF(lhs, rhs, lhs_block, rhs_block);

      default:
        UNREACHABLE();
        return nullptr;
    }
  }
}

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

IMPL2(I64Eq)
IMPL2(I64Ne)

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
IMPL1(CastBToStr)

// Special operation
HOperand* HAssembler::LoadLocal(HOperand* op1) {
  auto out = Emit1(HOpcode::kLoadLocal, op1);
  return out;
}

HOperand* HAssembler::LoadParam(HOperand* op1) {
  auto out = Emit1(HOpcode::kLoadParam, op1);
  return out;
}

HOperand* HAssembler::LoadTemp(HOperand* op1) {
  auto out = Emit1(HOpcode::kLoadTemp, op1);
  return out;
}

HOperand* HAssembler::LoadVarRef(HOperand* op1) {
  auto out = Emit1(HOpcode::kLoadVarRef, op1);
  return out;
}

HOperand* HAssembler::StoreVarRef(HOperand* index, HOperand* value) {
  auto out = Emit2(HOpcode::kStoreVarRef, index, value);
  return out;
}

void HAssembler::StoreTemp(HOperand* v, HOperand* i) {
  CHECK(i->IsImm());
  Emit2(HOpcode::kStoreTemp, v, i);
}

void HAssembler::BoxToTemp(HOperand* v, HOperand* type, HOperand* idx) {
  CHECK(type->IsImm());
  CHECK(idx->IsImm());

  auto ins = EmitS(HOpcode::kBoxToTemp, 3);
  ins->OprSet(0, v);
  ins->OprSet(1, type);
  ins->OprSet(2, idx);

  CHECK(ins->VerifyOperand());
  ins->PopulateOutput(this);
}

HOperand* HAssembler::LeaTemp(HOperand* op1) {
  CHECK(op1->IsImm());  // must be immediate for now
  auto out = Emit1(HOpcode::kLeaTemp, op1);
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

IMPL0(LoadNull)
IMPL0(LoadUndefined)
IMPL0(LoadUninitialized)

HOperand* HAssembler::LoadStringUnbox(const zone::String& str) {
  return Emit1(HOpcode::kLoadStringUnbox, NewString(str));
}

// Movs
IMPL1(Mov)
IMPL1(MovF)

IMPL1(MovI64)
IMPL1(MovB64)
IMPL1(MovI32)
IMPL1(MovF64)

IMPL1(DupI)
IMPL1(DupF)
IMPL1(DupPtr)

// Memory
IMPL1(Lea)

// Box and Unbox
IMPLG1(GUnboxI)
IMPLG1(GUnboxB)
IMPLG1(GUnboxF)
IMPLG1(GUnboxPtr)

IMPL1(UnboxPtr)
IMPL1(UnboxF)
IMPL1(UnboxI)
IMPL1(UnboxB)
IMPL1(GetTTag)

IMPL1(IsNullOrUndefined)
IMPL1(StrLen);

// Atom
IMPL1(IsBoxAtomInt)
IMPL1(AtomFromI)
IMPLG1(GAtom)

HOperand* HAssembler::Box(HOperand* arg, int type_tag) {
  auto imm = NewImmediate(static_cast<std::int32_t>(type_tag));
  return Emit2(HOpcode::kBox, arg, imm);
}

// Ref count
IMPLREF1(RcInc)
IMPLREF2(RcDecMaybeDelete)

// ----------------------------------------------------------------------------
// JSCall
HOperand* HAssembler::JSCallF(HOperand* func, HOperand* argc, HOperand* argv) {
  CHECK(argc->IsImm());

  auto ins = EmitS(HOpcode::kJSCallF, 3);

  ins->OprSet(0, func);
  ins->OprSet(1, argc);
  ins->OprSet(2, argv);

  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::JSCallFN(HOperand* func, HOperand* tar, HOperand* argc,
                               HOperand* argv) {
  CHECK(argc->IsImm());

  auto ins = EmitS(HOpcode::kJSCallFN, 4);

  ins->OprSet(0, func);
  ins->OprSet(1, tar);
  ins->OprSet(2, argc);
  ins->OprSet(3, argv);

  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::JSCallFT(HOperand* func, HOperand* this_object,
                               HOperand* argc, HOperand* argv) {
  CHECK(argc->IsImm());

  auto ins = EmitS(HOpcode::kJSCallFT, 4);

  ins->OprSet(0, func);
  ins->OprSet(1, this_object);
  ins->OprSet(2, argc);
  ins->OprSet(3, argv);

  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::JSCallFTN(HOperand* func, HOperand* this_object,
                                HOperand* tar, HOperand* argc, HOperand* argv) {
  CHECK(argc->IsImm());

  auto ins = EmitS(HOpcode::kJSCallFTN, 5);

  ins->OprSet(0, func);
  ins->OprSet(1, this_object);
  ins->OprSet(2, tar);
  ins->OprSet(3, argc);
  ins->OprSet(4, argv);

  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::JSCallCtor(HOperand* func, HOperand* this_object,
                                 HOperand* argc, HOperand* argv) {
  CHECK(argc->IsImm());

  auto ins = EmitS(HOpcode::kJSCallCtor, 4);

  ins->OprSet(0, func);
  ins->OprSet(1, this_object);
  ins->OprSet(2, argc);
  ins->OprSet(3, argv);

  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::ArrayFrom(HOperand* param_size, HOperand* param_start) {
  CHECK(param_size->IsImm());
  return Emit2(HOpcode::kArrayFrom, param_size, param_start);
}

HOperand* HAssembler::Apply(HOperand* this_object, HOperand* param_start,
                            HOperand* magic) {
  auto ins = EmitS(HOpcode::kApply, 3);

  ins->OprSet(0, this_object);
  ins->OprSet(1, param_start);
  ins->OprSet(2, magic);

  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

// ----------------------------------------------------------------------------
// RCall
HOperand* HAssembler::RCallBox(HOperand* call_index, std::size_t param_size) {
  auto ins = EmitS(HOpcode::kRCallBox, param_size + 1);
  ins->OprSet(0, call_index);
  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::RCallPtr(HOperand* call_index, std::size_t param_size) {
  auto ins = EmitS(HOpcode::kRCallPtr, param_size + 1);
  ins->OprSet(0, call_index);
  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::RCallF64(HOperand* call_index, std::size_t param_size) {
  auto ins = EmitS(HOpcode::kRCallF64, param_size + 1);
  ins->OprSet(0, call_index);
  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

HOperand* HAssembler::RCallI32(HOperand* call_index, std::size_t param_size) {
  auto ins = EmitS(HOpcode::kRCallI32, param_size + 1);
  ins->OprSet(0, call_index);
  CHECK(ins->VerifyOperand());
  return ins->PopulateOutput(this);
}

// ----------------------------------------------------------------------------
HOperand* HAssembler::LoadNumberProto() {
  return Emit0(HOpcode::kLoadNumberProto);
}

HOperand* HAssembler::LoadBoolProto() { return Emit0(HOpcode::kLoadBoolProto); }

HOperand* HAssembler::LoadStringProto() {
  return Emit0(HOpcode::kLoadStringProto);
}

HOperand* HAssembler::LoadSymbolProto() {
  return Emit0(HOpcode::kLoadSymbolProto);
}

HOperand* HAssembler::ObjectFieldGetAtomFast(HOperand* target, HOperand* atom) {
  CHECK(atom->IsImm());
  return Emit2(HOpcode::kObjectFieldGetAtomFast, target, atom);
}

HOperand* HAssembler::MaybeArrayLengthGet(HOperand* target) {
  return Emit1(HOpcode::kMaybeArrayLengthGet, target);
}

HOperand* HAssembler::GlobalGetVarAtomFast(HOperand* atom, HOperand* flag) {
  CHECK(atom->IsImm());
  CHECK(flag->IsImm());
  return Emit2(HOpcode::kGlobalGetVarAtomFast, atom, flag);
}

HOperand* HAssembler::GlobalGetGlobalAtomFast(HOperand* atom, HOperand* flag) {
  CHECK(atom->IsImm());
  CHECK(flag->IsImm());
  return Emit2(HOpcode::kGlobalGetGlobalAtomFast, atom, flag);
}

HOperand* HAssembler::LoadArrayValue(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayValue, array, index);
}

HOperand* HAssembler::LoadArrayArguments(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayArguments, array, index);
}

HOperand* HAssembler::LoadArrayInt8(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayInt8, array, index);
}

HOperand* HAssembler::LoadArrayUInt8(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayUInt8, array, index);
}

HOperand* HAssembler::LoadArrayUInt8C(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayUInt8C, array, index);
}

HOperand* HAssembler::LoadArrayInt16(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayInt16, array, index);
}

HOperand* HAssembler::LoadArrayUInt16(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayUInt16, array, index);
}

HOperand* HAssembler::LoadArrayInt32(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayInt32, array, index);
}

HOperand* HAssembler::LoadArrayUInt32(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayUInt32, array, index);
}

HOperand* HAssembler::LoadArrayF32(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayF32, array, index);
}

HOperand* HAssembler::LoadArrayF64(HOperand* array, HOperand* index) {
  return Emit2(HOpcode::kLoadArrayF64, array, index);
}

HOperand* HAssembler::ContextLoad() { return Emit0(HOpcode::kContextLoad); }

HOperand* HAssembler::ContextLoadGlobalObject() {
  return Emit0(HOpcode::kContextLoadGlobalObject);
}

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
  pending_loop_phi_.push_back(instruction);
  // do not verify operand since operand is not setup yet
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::PhiLoopF() {
  auto instruction = MkInstruction(HOpcode::kPhiLoopF, nullptr, nullptr);
  pending_loop_phi_.push_back(instruction);
  // do not verify operand since operand is not setup yet
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::PhiLoopPtr() {
  auto instruction = MkInstruction(HOpcode::kPhiLoopPtr, nullptr, nullptr);
  pending_loop_phi_.push_back(instruction);
  // do not verify operand since operand is not setup yet
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::PhiBranchF(HOperand* lhs, HOperand* rhs,
                                 HInstruction* lhs_block,
                                 HInstruction* rhs_block) {
  auto instruction = EmitS(HOpcode::kPhiBranchF, 4);
  instruction->OprSet(0, lhs);
  instruction->OprSet(1, rhs);
  instruction->OprSet(2, NewLabel(lhs_block));
  instruction->OprSet(3, NewLabel(rhs_block));
  branch_phi_list_.push_back(instruction);

  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::PhiBranchI(HOperand* lhs, HOperand* rhs,
                                 HInstruction* lhs_block,
                                 HInstruction* rhs_block) {
  auto instruction = EmitS(HOpcode::kPhiBranchI, 4);
  instruction->OprSet(0, lhs);
  instruction->OprSet(1, rhs);
  instruction->OprSet(2, NewLabel(lhs_block));
  instruction->OprSet(3, NewLabel(rhs_block));
  branch_phi_list_.push_back(instruction);

  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::PhiBranchPtr(HOperand* lhs, HOperand* rhs,
                                   HInstruction* lhs_block,
                                   HInstruction* rhs_block) {
  auto instruction = EmitS(HOpcode::kPhiBranchPtr, 4);
  instruction->OprSet(0, lhs);
  instruction->OprSet(1, rhs);
  instruction->OprSet(2, NewLabel(lhs_block));
  instruction->OprSet(3, NewLabel(rhs_block));
  branch_phi_list_.push_back(instruction);

  CHECK(instruction->VerifyOperand());
  return instruction->PopulateOutput(this);
}

HOperand* HAssembler::PhiBranchBox(HOperand* lhs, HOperand* rhs,
                                   HInstruction* lhs_block,
                                   HInstruction* rhs_block) {
  auto instruction = EmitS(HOpcode::kPhiBranchBox, 4);
  instruction->OprSet(0, lhs);
  instruction->OprSet(1, rhs);
  instruction->OprSet(2, NewLabel(lhs_block));
  instruction->OprSet(3, NewLabel(rhs_block));
  branch_phi_list_.push_back(instruction);

  CHECK(instruction->VerifyOperand());
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
    for (auto& phi : pending_loop_phi_) {
      phi->InsertBefore(end_instruction());
    }
    pending_loop_phi_.clear();
    loop_phi_start_ = end_.ins->next();
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
