#include "lir/arch-constraint.h"
#include "lir/ra-linearize.h"
#include "lir/instruction.h"

#include "cg/x64/abi.h"

#include <array>

// ----------------------------------------------------------------------------
// The x64 provider of the lir::arch declarative constraint interface
// (include/lir/arch-constraint.h). All arch-specific register-constraint
// knowledge the generic RA needs is served from here as pure per-opcode
// data (the legacy action/hint queries were removed with the legacy
// allocation substrate, plan 02-05 D-22).
namespace quake {
namespace lir {
namespace arch {

// ----------------------------------------------------------------------------
// Pure-data constraint tables (D-25/D-26, plan 02-03).
//
// Every HOpcode has an explicit entry in the table below, audited line by
// line against the register behavior its translate rule exhibits in
// src/cg/x64/lir-translate.cc (plus the stub bodies it calls into in
// src/cg/x64/stub-gen.cc). The switch that fills the table is EXHAUSTIVE
// over HOpcode and deliberately has NO default case: -Werror's -Wswitch
// turns any opcode left un-audited (or any newly added opcode) into a
// build error — the sparse-switch `default: break` wildcard anti-pattern
// called out in 02-PATTERNS.md is never reintroduced (D-26).
//
// Audit summary (translate rule -> encoded data):
//
//  * Two-operand arithmetic/move-shaped ops (int IMPL_BINARY, FP
//    IMPL_BINARY/GenFArithXxx, kIMul reg/mem forms, kSar IMPL_BBIN_INT,
//    kSarConst, IMPL_UNARY_INT kInc/kDec/kNeg/kInv): the machine
//    instruction reads its dst as src1 (MOVE(-1, 0) + op REG(-1), ...), ie
//    the output must share operand 0's register -> output =
//    kSameAsOperand(0). The legacy translate TOLERATES other placements via
//    the materialization move (eg IMPL_BINARY "Rmr"/FP "Xmx" MOVE(-1, 1));
//    the declarative relation is what the constraint-driven RA (plan 02-04)
//    guarantees and what the D-27 validator enforces going forward.
//  * Register-count shifts (kSar IMPL_BBIN_INT, kGShl/kGShr GenGShift):
//    the shift count is staged into rcx at translate time
//    (ASM(mov, ECX(), ...)) -> scratch_gp = {rcx}. kSar additionally keeps
//    the two-operand shape; the const-count variants (kSarConst "Rri",
//    kGShlConst/kGShrConst GenGShiftConst) shift by an immediate and never
//    touch rcx.
//  * Comparisons (int GenIntConditionValue CHECKs opr0->IsRegister(); FP
//    GenFloatConditionValue builds u::Fp(opr0)): operand 0 must sit in a
//    register -> operands[0] = kAnyRegister. The setcc output is written
//    independently of both inputs.
//  * kIMod: translate stages the dividend into rax, zeroes rdx, idivs, and
//    reads the remainder back from rdx -> scratch_gp = {rax, rdx}, output =
//    kFixedRegister(rdx). (rax is the JIT-reserved scratch; it is listed so
//    the table documents the full translate behavior. rdx is exempt from
//    the scratch liveness check as the instruction's own output.)
//  * ABI calls (kRCall*, kJSCall*/kArrayFrom/kApply — DoScriptCall targets
//    QuakeJit::JSCall family, kEnterJitTrace tail-calls a nested trace
//    entry): is_call = true with the full caller-saved clobber sets.
//    kRCallF64's translate CHECKs its output register is xmm0 -> output =
//    kFixedRegister(xmm0).
//  * Stub-clobbering internal calls: kCastVToB (Stub::AnyToBool documents
//    rax + rdx as its free registers), the atom-fast property/global
//    accessors (Stub::ObjectFieldGetAtomFast / ObjectFieldSetAtomFast /
//    GlobalGetGlobalAtomFast chase shapes through rdx) and the LoadArray
//    family (DoGenLoadArray computes the element address through rdx), and
//    kEnterInlineFrame (the inline frame pointer is built in rdx) ->
//    clobber_gp = {rdx}. kCastF64ToB32's stub clobbers rax + rcx + rbp, and
//    the translate wrapper borrows/restores rbp around the call ->
//    clobber_gp = {rcx}. These are NOT ABI calls (is_call = false): only
//    the listed register escapes destruction.
//  * Everything else — moves, loads/stores, constants, pure casts, cmp/jump
//    /deopt/snapshot, rc, unbox/box, guarded arithmetic (GenGuardIntArith-
//    metic/GenGIncDec compute through the JIT-reserved rax scratch with
//    free output placement), x87 kFMod, etc — touches no allocatable
//    register outside its declared operands and the JIT-reserved rax/xmm15
//    scratch: explicit no-constraints marker.
//
// The JIT-reserved registers (rax = kScratchRegister, r15 = context,
// xmm15 = FP scratch) are never RA-allocated, so they never appear in
// clobber/scratch sets except rax above for kIMod, where the dividend
// staging is integral to the rule.
namespace {

using arch::ArchOperandConstraint;
using lir::HOpcode;
using arch::InstructionConstraint;
using lir::Register;
using lir::RegisterKind;
using lir::RegisterSet;

constexpr std::size_t kOpcodeCount =
    static_cast<std::size_t>(HOpcode::kSizeOfHOpcode);

InstructionConstraint NoConstraints() {
  InstructionConstraint c;
  c.is_no_constraints_marker = true;
  return c;
}

RegisterSet SetOf(std::initializer_list<RegisterKind> kinds) {
  RegisterSet set;
  for (auto k : kinds) {
    set.Set(Register::GetRegisterFromKind(k));
  }
  return set;
}

// Builds the whole table: every slot starts as the explicit no-constraints
// marker; the exhaustive switch below then overwrites the audited opcodes.
// The switch runs inside a loop over every HOpcode value so each case group
// can apply its setting through a single per-opcode helper (the group
// helpers CHECK the entry is still the untouched marker, so a opcode can
// never silently land in two groups).
std::array<InstructionConstraint, kOpcodeCount> BuildConstraintTable() {
  std::array<InstructionConstraint, kOpcodeCount> table;
  for (auto& entry : table) entry = NoConstraints();

  const Register* rcx = Register::GetRegisterFromKind(RegisterKind::kRcx);
  const Register* rdx = Register::GetRegisterFromKind(RegisterKind::kRdx);
  const Register* rax = Register::GetRegisterFromKind(RegisterKind::kRax);
  const Register* xmm0 = Register::GetRegisterFromKind(RegisterKind::kXmm0);

  // The exhaustive switch below is the single-assignment guarantee (one
  // case group per opcode); kSar deliberately passes through two setters
  // (two-operand rule + shift-count scratch), so the setters only clear
  // the marker, never assert it.
  auto SetTwoOperandRule = [&](HOpcode op) {
    auto& c = table[static_cast<std::size_t>(op)];
    c.is_no_constraints_marker = false;
    c.output = ArchOperandConstraint::SameAs(0);
  };

  auto SetShiftCountScratch = [&](HOpcode op) {
    auto& c = table[static_cast<std::size_t>(op)];
    c.is_no_constraints_marker = false;
    c.scratch_gp.Set(rcx);
  };

  auto SetOperand0AnyRegister = [&](HOpcode op) {
    auto& c = table[static_cast<std::size_t>(op)];
    c.is_no_constraints_marker = false;
    c.operands.push_back(ArchOperandConstraint::AnyRegister());
  };

  auto SetRdxClobber = [&](HOpcode op) {
    auto& c = table[static_cast<std::size_t>(op)];
    c.is_no_constraints_marker = false;
    c.clobber_gp.Set(rdx);
  };

  auto SetCall = [&](HOpcode op) {
    auto& c = table[static_cast<std::size_t>(op)];
    c.is_no_constraints_marker = false;
    c.is_call = true;
    c.clobber_gp = GetCallClobberGP();
    c.clobber_fp = GetCallClobberFP();
  };

  // Notes, this switch is intentionally default-free: -Wswitch under
  // -Werror compiles only while every HOpcode enumerator has an explicit
  // case, so a newly added opcode fails the build until it is audited here.
  // The loop stops before kSizeOfHOpcode (not a real opcode — the enum's
  // bound sentinel, excluded from the table's domain).
  for (std::size_t i = 0; i < kOpcodeCount - 1; ++i) {
    const HOpcode op = static_cast<HOpcode>(i);
    switch (op) {
      // -------------------------------------------------------------
      // Two-operand rule: output must share operand 0's register
      // (int/FP binary arith, int mul, unary inc/dec/neg/inv, sar).
      case HOpcode::kIAdd:
      case HOpcode::kISub:
      case HOpcode::kAnd:
      case HOpcode::kOr:
      case HOpcode::kXor:
      case HOpcode::kIMul:
      case HOpcode::kFAdd:
      case HOpcode::kFSub:
      case HOpcode::kFMul:
      case HOpcode::kFDiv:
      case HOpcode::kInc:
      case HOpcode::kDec:
      case HOpcode::kNeg:
      case HOpcode::kInv:
      case HOpcode::kSarConst:
        SetTwoOperandRule(op);
        break;

      // -------------------------------------------------------------
      // Register-count shifts: count staged into rcx (scratch).
      // kSar additionally carries the two-operand shape.
      case HOpcode::kSar:
        SetTwoOperandRule(op);
        SetShiftCountScratch(op);
        break;
      case HOpcode::kGShl:
      case HOpcode::kGShr:
        SetShiftCountScratch(op);
        break;

      // -------------------------------------------------------------
      // Comparisons: operand 0 must sit in a register (cmp/comisd
      // lhs); the setcc output is independent.
      case HOpcode::kILt:
      case HOpcode::kILe:
      case HOpcode::kIGt:
      case HOpcode::kIGe:
      case HOpcode::kIEq:
      case HOpcode::kINe:
      case HOpcode::kI64Eq:
      case HOpcode::kI64Ne:
      case HOpcode::kFLt:
      case HOpcode::kFLe:
      case HOpcode::kFGt:
      case HOpcode::kFGe:
      case HOpcode::kFEq:
      case HOpcode::kFNe:
        SetOperand0AnyRegister(op);
        break;

      // -------------------------------------------------------------
      // idiv: dividend staged in rax, rdx zeroed + remainder output.
      case HOpcode::kIMod:
        {
          auto& c = table[static_cast<std::size_t>(op)];
          c.is_no_constraints_marker = false;
          c.output = ArchOperandConstraint::Fixed(rdx);
          c.scratch_gp.Set(rax);
          c.scratch_gp.Set(rdx);
        }
        break;

      // -------------------------------------------------------------
      // ABI calls: full caller-saved clobbers. kRCallF64 returns in
      // xmm0 (translate CHECK).
      case HOpcode::kRCallPtr:
      case HOpcode::kRCallB64:
      case HOpcode::kRCallI32:
      case HOpcode::kRCallVoid:
      case HOpcode::kRCallBox:
      case HOpcode::kJSCallF:
      case HOpcode::kJSCallFT:
      case HOpcode::kJSCallFN:
      case HOpcode::kJSCallFTN:
      case HOpcode::kJSCallCtor:
      case HOpcode::kArrayFrom:
      case HOpcode::kApply:
      case HOpcode::kEnterJitTrace:
        SetCall(op);
        break;
      case HOpcode::kRCallF64:
        SetCall(op);
        table[static_cast<std::size_t>(op)].output =
            ArchOperandConstraint::Fixed(xmm0);
        break;

      // -------------------------------------------------------------
      // Stub-clobbering internal calls (not ABI calls).
      case HOpcode::kCastVToB:
      case HOpcode::kObjectFieldGetAtomFast:
      case HOpcode::kObjectFieldSetAtomFast:
      case HOpcode::kGlobalGetVarAtomFast:
      case HOpcode::kGlobalGetGlobalAtomFast:
      case HOpcode::kLoadArrayValue:
      case HOpcode::kLoadArrayArguments:
      case HOpcode::kLoadArrayInt8:
      case HOpcode::kLoadArrayUInt8:
      case HOpcode::kLoadArrayUInt8C:
      case HOpcode::kLoadArrayInt16:
      case HOpcode::kLoadArrayUInt16:
      case HOpcode::kLoadArrayInt32:
      case HOpcode::kLoadArrayUInt32:
      case HOpcode::kLoadArrayF32:
      case HOpcode::kLoadArrayF64:
      case HOpcode::kEnterInlineFrame:
        SetRdxClobber(op);
        break;

      // -------------------------------------------------------------
      // CastF64ToB32 stub: rax + rcx + rbp clobbered; the translate
      // wrapper borrows/restores rbp, rax is the reserved scratch.
      case HOpcode::kCastF64ToB32:
        {
          auto& c = table[static_cast<std::size_t>(op)];
          c.is_no_constraints_marker = false;
          c.clobber_gp.Set(rcx);
        }
        break;

      // -------------------------------------------------------------
      // Explicit no-constraints marker (D-26): audited to exhibit no
      // allocatable-register behavior beyond the declared operands
      // and the JIT-reserved rax/xmm15 scratch.
      case HOpcode::kSentry:
      case HOpcode::kPlaceholder:
      case HOpcode::kLeaveInlineFrame:
      case HOpcode::kBarrier:
      case HOpcode::kUndefined:
      case HOpcode::kNull:
      case HOpcode::kUninitialized:
      case HOpcode::kBlock:
      case HOpcode::kSlowBlockStart:
      case HOpcode::kSlowBlockOpStart:
      case HOpcode::kSlowBlockOpDone:
      case HOpcode::kSlowBlockEnd:
      case HOpcode::kStart:
      case HOpcode::kEnd:
      case HOpcode::kBB1:
      case HOpcode::kStrLen:
      case HOpcode::kPhiLoopF:
      case HOpcode::kPhiLoopI:
      case HOpcode::kPhiLoopI64:
      case HOpcode::kPhiLoopPtr:
      case HOpcode::kPhiBranchF:
      case HOpcode::kPhiBranchI:
      case HOpcode::kPhiBranchPtr:
      case HOpcode::kPhiBranchBox:
      case HOpcode::kContextLoadGlobalObject:
      case HOpcode::kContextLoad:
      case HOpcode::kContextLoadJSContext:
      case HOpcode::kLoadThis:
      case HOpcode::kLoadThisTag:
      case HOpcode::kLoadStackFrame:
      case HOpcode::kLoadCurFunc:
      case HOpcode::kLoadNewTarget:
      case HOpcode::kLoadHomeObject:
      case HOpcode::kLoadNumberProto:
      case HOpcode::kLoadBoolProto:
      case HOpcode::kLoadSymbolProto:
      case HOpcode::kLoadStringProto:
      case HOpcode::kMaybeArrayLengthGet:
      case HOpcode::kGIAdd:
      case HOpcode::kGISub:
      case HOpcode::kGIMul:
      case HOpcode::kFMod:      // inline x87 fprem; only rax(ax) touched
      case HOpcode::kFPow:      // no translate rule today; never emitted
      case HOpcode::kGInc:
      case HOpcode::kIncF:
      case HOpcode::kGDec:
      case HOpcode::kDecF:
      case HOpcode::kGNeg:
      case HOpcode::kNegF:
      case HOpcode::kGShlConst:
      case HOpcode::kGShrConst:
      case HOpcode::kBoxIsHeap:            // no translate rule; never emitted
      case HOpcode::kBoxIsPrimitive:       // no translate rule; never emitted
      case HOpcode::kBoxTagIsEq:           // scratch-only tag compare
      case HOpcode::kBoxTagIsNe:           // scratch-only tag compare
      case HOpcode::kVarRefListIsEmpty:    // scratch-only cmov
      case HOpcode::kVarRefListIsNotEmpty: // scratch-only cmov
      case HOpcode::kCmpI:
      case HOpcode::kCmpF:
      case HOpcode::kCmpTest:
      case HOpcode::kCmpBoxIsHeap:
      case HOpcode::kCmpBoxIsPrimitive:
      case HOpcode::kCmpBoxTagIs:
      case HOpcode::kCmpVarRefListIsEmpty:
      case HOpcode::kBranchTrue:
      case HOpcode::kBranchFalse:
      case HOpcode::kJmpNe:
      case HOpcode::kJmpEq:
      case HOpcode::kJmpLt:
      case HOpcode::kJmpLe:
      case HOpcode::kJmpBt:
      case HOpcode::kJmpBe:
      case HOpcode::kJmpGt:
      case HOpcode::kJmpGe:
      case HOpcode::kJmpAt:
      case HOpcode::kJmpAe:
      case HOpcode::kJmpNz:
      case HOpcode::kJmpZ:
      case HOpcode::kJmpBoxIsHeap:
      case HOpcode::kJmpBoxIsPrimitive:
      case HOpcode::kJmp:
      case HOpcode::kReturn:
      case HOpcode::kDupI:
      case HOpcode::kDupF:
      case HOpcode::kDupPtr:
      case HOpcode::kMov:
      case HOpcode::kMovF:
      case HOpcode::kTrans:
      case HOpcode::kTransF:
      case HOpcode::kShuffle:
      case HOpcode::kShuffleF:
      case HOpcode::kParallelCopy:  // D-29 opcode: the marker exemplar
      case HOpcode::kMovB64:
      case HOpcode::kMovI64:
      case HOpcode::kMovI32:
      case HOpcode::kMovF64:
      case HOpcode::kLoadUndefined:
      case HOpcode::kLoadNull:
      case HOpcode::kLoadUninitialized:
      case HOpcode::kLoadStringUnbox:
      case HOpcode::kLea:
      case HOpcode::kPush:
      case HOpcode::kPop:
      case HOpcode::kLoadLocal:
      case HOpcode::kLoadParam:
      case HOpcode::kLoadTemp:
      case HOpcode::kLoadVarRef:
      case HOpcode::kStoreVarRef:
      case HOpcode::kBoxToTemp:
      case HOpcode::kStoreTemp:
      case HOpcode::kBoxToLocal:
      case HOpcode::kStoreLocal:
      case HOpcode::kBoxToParam:
      case HOpcode::kStoreParam:
      case HOpcode::kLeaTemp:
      case HOpcode::kCastNBToB:
      case HOpcode::kCastI32ToF64:
      case HOpcode::kCastF64ToB:
      case HOpcode::kCastBToStr:
      case HOpcode::kCastObjToB:
      case HOpcode::kCastStrToB:
      case HOpcode::kRcInc:
      case HOpcode::kRcIncBox:
      case HOpcode::kRcDecMaybeDelete:
      case HOpcode::kRcDecMaybeDeleteBox:
      case HOpcode::kRcDec:
      case HOpcode::kRcDecBox:
      case HOpcode::kGUnboxI:
      case HOpcode::kGUnboxF:
      case HOpcode::kGUnboxB:
      case HOpcode::kGUnboxObj:
      case HOpcode::kGUnboxStr:
      case HOpcode::kGUnboxMod:
      case HOpcode::kGObjectIsScriptFunction:
      case HOpcode::kUnboxPtr:
      case HOpcode::kUnboxI:
      case HOpcode::kUnboxB:
      case HOpcode::kUnboxF:
      case HOpcode::kGetTTag:
      case HOpcode::kIsNullOrUndefined:  // scratch-only tag test
      case HOpcode::kBox:
      case HOpcode::kIsBoxAtomInt:
      case HOpcode::kAtomFromI:
      case HOpcode::kGAtom:
      case HOpcode::kSnapshot:
      case HOpcode::kDeopt:
      case HOpcode::kDeoptIfTrue:
      case HOpcode::kDeoptIfFalse:
      case HOpcode::kDeoptNe:
      case HOpcode::kDeoptEq:
      case HOpcode::kDeoptLt:
      case HOpcode::kDeoptLe:
      case HOpcode::kDeoptGt:
      case HOpcode::kDeoptGe:
      case HOpcode::kDeoptAt:
      case HOpcode::kDeoptAe:
      case HOpcode::kDeoptBt:
      case HOpcode::kDeoptBe:
      case HOpcode::kDeoptNz:
      case HOpcode::kDeoptZ:
        // Explicit marker — the initializer already installed it; nothing
        // to encode for these opcodes.
        break;

      case HOpcode::kSizeOfHOpcode:
        // Not a real opcode — the enum bound sentinel. The fill loop stops
        // before it; the label exists because -Wswitch requires every
        // enumerator of the switched type to be named.
        UNREACHABLE();
        break;
    }
  }

  return table;
}

}  // namespace

const InstructionConstraint& GetInstructionConstraint(HOpcode op) {
  static const auto kTable = BuildConstraintTable();
  return kTable[static_cast<std::size_t>(op)];
}

const InstructionConstraint& GetInstructionConstraint(const HInstruction* ins) {
  return GetInstructionConstraint(ins->opcode());
}

const Register* GetCallArgRegister(std::size_t arg_index, bool fp) {
  if (fp) {
    auto kind = cg::x64::GetABIFPArg(static_cast<int>(arg_index));
    return kind ? Register::GetRegisterFromKind(*kind) : nullptr;
  }
  auto kind = cg::x64::GetABIGPArg(static_cast<int>(arg_index));
  return kind ? Register::GetRegisterFromKind(*kind) : nullptr;
}

RegisterSet GetCallClobberGP() {
  static const RegisterSet kSet = SetOf(
      {RegisterKind::kRdi, RegisterKind::kRsi, RegisterKind::kRdx,
       RegisterKind::kRcx, RegisterKind::kR8,  RegisterKind::kR9,
       RegisterKind::kR10, RegisterKind::kR11});
  return kSet;
}

RegisterSet GetCallClobberFP() {
  static const RegisterSet kSet =
      SetOf({RegisterKind::kXmm0,  RegisterKind::kXmm1,  RegisterKind::kXmm2,
             RegisterKind::kXmm3,  RegisterKind::kXmm4,  RegisterKind::kXmm5,
             RegisterKind::kXmm6,  RegisterKind::kXmm7,  RegisterKind::kXmm8,
             RegisterKind::kXmm9,  RegisterKind::kXmm10, RegisterKind::kXmm11,
             RegisterKind::kXmm12, RegisterKind::kXmm13, RegisterKind::kXmm14});
  return kSet;
}

}  // namespace arch
}  // namespace lir
}  // namespace quake
