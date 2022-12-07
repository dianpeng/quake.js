#include "cg/x64/lir-translate.h"
#include "cg/x64/asm-util.h"
#include "cg/x64/code-gen.h"

#include "lir/operand.h"
#include "lir/instruction.h"
#include "lir/opcode.h"

#include "deopt.h"
#include "quake-rt.h"
#include "qjs.h"

#define __ (cg->as())->

using namespace quake::lir;
using namespace base;

/* ============================================================================
 *              General Translation/Lowering Principles
 *
 * Our LIR is been designed to be architecture independent but minimize the
 * lowering gap as much as possible. However, the LIR still exposes a relative
 * obvious semantic gap between LIR and the arch-assmebly. The term used in
 * context of LIR and context of arch, ie x64, still have different semantic.
 * The following principle will make all the difference explicit and clear for
 * user to write code to perform lowering.
 *
 * The lowering principle is as following:
 *
 * 0) General rules
 *
 *    0.1 Immediate number means int32, int64/float64 is not immediate and
 *        should only be materialized via special instruction MovI64/MovB64.
 *        So they cannot participate instruction takes immediate number.
 *
 *    0.2 Floating point is been addressed with typped instruction, int
 *        instruction and floating point instruction is separated. For example
 *        mov a floating point into memory/register is via MovF, mov an imm
 *        floating point, which we only supports double precision, is via
 *        MovF64 -> MovF. Notes MovF does not support immediate number, again
 *        all the immediate float64 should be materialized via MovF64.
 *
 * 1) General purpose register or integer related operations
 *
 *    1.1 All arith/bit/mov instruction should allow I, M, and R type input
 *
 *    1.2 CMP instruction should allow I, M, R type input
 *
 *    1.3 TEST is an exception
 *
 *    1.4 All instruction's output should support R and M, except TEST
 *
 *
 *    1.5 64 bits qword literal is not an immediate, and should be only
 *        materialized with MovI64/MovB64 which has special logic
 *
 * 2) Floating point register
 *
 *   2.1 Mov instruction is via MovF, not Mov. Resolved by LIR framework
 *   2.2 Comparison and test is been carried with type as well. Resolved by
 *       LIR framework
 *   2.3 Arithmetic is supported via FXX instruction
 *   2.4 Materialization must be carried out via MovF64
 *   2.5 To turn a floating point number back to GP, must use box operation
 *
 * 3) We should not push/pop cross the boundary of LIR. The stack has been
 *    explicitly aligned agasint 16 bytes to allow SSE to work. This count
 *    is been done via the RA and hardcode into the epilogue of JIT entry.
 *    Any push/pop may break this assumption which makes SSE traps. To avoid
 *    this happen, please do not use PUSH/POP, if you really need it, make
 *    sure just use them temporarily
 *
 * ===========================================================================*/

namespace quake {
namespace cg {
namespace x64 {
namespace {

namespace a = asmjit;

bool IsX64AllowedOutput(const HOperand* out) {
  return out->IsRegister() || out->IsMemory();
}

template <int N>
struct ImmOperand {
  using asmjit_type = const a::Imm&;

  static a::Imm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return ImmOperandToASMJIT(operand);
  }
};

template <int N>
struct Imm8Operand {
  using asmjit_type = const a::Imm&;

  static a::Imm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return Imm8OperandToASMJIT(operand);
  }
};

template <std::int32_t kImm>
struct ConstIntOperand {
  using asmjit_type = const a::Imm&;
  static a::Imm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::Imm{kImm};
  }
};

template <std::uint32_t kImm>
struct ConstUintOperand {
  using asmjit_type = const a::Imm&;
  static a::Imm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::Imm{kImm};
  }
};

template <typename T>
struct ImmOperandNegate {
  using asmjit_type = const a::Imm&;

  static a::Imm GetAsmOperand(CodeGen* codegen, const HInstruction* instr) {
    auto imm = T::GetAsmOperand(codegen, instr);
    auto v = static_cast<std::int32_t>(imm.value());
    return a::Imm(-v);
  }
};

template <typename T>
struct ImmOperandInvert {
  using asmjit_type = const a::Imm&;

  static a::Imm GetAsmOperand(CodeGen* codegen, const HInstruction* instr) {
    auto imm = T::GetAsmOperand(codegen, instr);
    auto v = static_cast<std::uint32_t>(imm.value());
    return a::Imm(~v);
  }
};

template <int N>
struct RegOperand {
  using asmjit_type = const a::x86::Gp&;

  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return GPRegOperandToASMJIT(operand);
  }
};

struct RaxOperand {
  using asmjit_type = const a::x86::Gp&;

  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::rax;
  }
};

struct EaxOperand {
  using asmjit_type = const a::x86::Gp&;

  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::eax;
  }
};

struct RdxOperand {
  using asmjit_type = const a::x86::Gp&;

  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::rdx;
  }
};

struct ScratchBOperand {
  using asmjit_type = const a::x86::Gp&;
  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::al;
  }
};

struct ScratchWOperand {
  using asmjit_type = const a::x86::Gp&;
  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::ax;
  }
};

struct ScratchDOperand {
  using asmjit_type = const a::x86::Gp&;
  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::eax;
  }
};

struct ScratchQOperand {
  using asmjit_type = const a::x86::Gp&;
  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::rax;
  }
};

struct ScratchXOperand {
  using asmjit_type = const a::x86::Xmm&;
  static a::x86::Xmm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::xmm15;
  }
};

struct EdxOperand {
  using asmjit_type = const a::x86::Gp&;

  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::edx;
  }
};

template <int N>
struct XmmOperand {
  using asmjit_type = const a::x86::Xmm&;
  static a::x86::Xmm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return FPRegOperandToASMJIT(operand);
  }
};

template <int N>
struct MemOperand {
  using asmjit_type = const a::x86::Mem&;

  static a::x86::Mem GetAsmOperand(CodeGen* codegen,
                                   const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return MemoryOperandToASMJIT(operand);
  }
};

template <int N>
struct LabelOperand {
  using asmjit_type = const a::Label&;

  static a::Label GetAsmOperand(CodeGen* codegen, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    CHECK(operand->IsLabel());
    auto label = operand->GetLabel();

    CHECK(label->HasMachineLabel());
    return label->machine_label();
  }
};

}  // namespace

using TransEntry = void (*)(CodeGen*, const HInstruction*);

// ----------------------------------------------------------------------------
// Translate rule table, ie global instance holding all the builint lowering
// rules for translation purpose
class TranslateRuleTable {
 public:
  static TranslateRuleTable& GetInstance() {
    static TranslateRuleTable kInstance;
    return kInstance;
  }

  TransEntry MatchRule(const HInstruction*) const;

 private:
  static void LowerMoveMM(CodeGen*, const HInstruction*);
  static void LowerMoveFMM(CodeGen*, const HInstruction*);

  static void DoMoveB64(CodeGen*, const HInstruction*);
  static void LowerMoveI64(CodeGen*, const HInstruction*);
  static void LowerMoveB64(CodeGen*, const HInstruction*);
  static void LowerMoveF64(CodeGen*, const HInstruction*);

  // variable loading
  static void LowerLoadLocal(CodeGen*, const HInstruction*);
  static void LowerLoadParam(CodeGen*, const HInstruction*);
  static void LowerLoadTemp(CodeGen*, const HInstruction*);
  static void LowerBoxToTemp(CodeGen*, const HInstruction*);
  static void LowerStoreTemp(CodeGen*, const HInstruction*);
  static void LowerLeaTemp(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // Guarded Integer
  static void LowerGuardIntArithmetic(CodeGen* cg, const HInstruction* ins);

  static void LowerGIAdd(CodeGen* cg, const HInstruction* ins) {
    return LowerGuardIntArithmetic(cg, ins);
  }
  static void LowerGISub(CodeGen* cg, const HInstruction* ins) {
    return LowerGuardIntArithmetic(cg, ins);
  }
  static void LowerGIMul(CodeGen* cg, const HInstruction* ins) {
    return LowerGuardIntArithmetic(cg, ins);
  }

 private:
  static void LowerFMod(CodeGen* cg, const HInstruction* ins);

 private:
  // --------------------------------------------------------------------------
  // Unary
  static void LowerIncDecF(CodeGen* cg, const HInstruction* ins);
  static void LowerIncF(CodeGen* cg, const HInstruction* ins) {
    LowerIncDecF(cg, ins);
  }
  static void LowerDecF(CodeGen* cg, const HInstruction* ins) {
    LowerIncDecF(cg, ins);
  }

  // Guarded integer inc/dec
  static void LowerGIncDec(CodeGen* cg, const HInstruction* ins);
  static void LowerGInc(CodeGen* cg, const HInstruction* ins) {
    LowerGIncDec(cg, ins);
  }
  static void LowerGDec(CodeGen* cg, const HInstruction* ins) {
    LowerGIncDec(cg, ins);
  }

  static void LowerNegF(CodeGen* cg, const HInstruction* ins);
  static void LowerGNeg(CodeGen* cg, const HInstruction* ins);

 private:
  // --------------------------------------------------------------------------
  // Comparison/Conditional value
  static void LowerIntConditionValue(CodeGen* cg, const HInstruction* ins);

  static void LowerILt(CodeGen* cg, const HInstruction* ins) {
    LowerIntConditionValue(cg, ins);
  }
  static void LowerILe(CodeGen* cg, const HInstruction* ins) {
    LowerIntConditionValue(cg, ins);
  }
  static void LowerIGt(CodeGen* cg, const HInstruction* ins) {
    LowerIntConditionValue(cg, ins);
  }
  static void LowerIGe(CodeGen* cg, const HInstruction* ins) {
    LowerIntConditionValue(cg, ins);
  }
  static void LowerINe(CodeGen* cg, const HInstruction* ins) {
    LowerIntConditionValue(cg, ins);
  }
  static void LowerIEq(CodeGen* cg, const HInstruction* ins) {
    LowerIntConditionValue(cg, ins);
  }

  static void LowerFloatConditionValue(CodeGen* cg, const HInstruction* ins);

  static void LowerFLt(CodeGen* cg, const HInstruction* ins) {
    LowerFloatConditionValue(cg, ins);
  }
  static void LowerFLe(CodeGen* cg, const HInstruction* ins) {
    LowerFloatConditionValue(cg, ins);
  }
  static void LowerFGt(CodeGen* cg, const HInstruction* ins) {
    LowerFloatConditionValue(cg, ins);
  }
  static void LowerFGe(CodeGen* cg, const HInstruction* ins) {
    LowerFloatConditionValue(cg, ins);
  }
  static void LowerFNe(CodeGen* cg, const HInstruction* ins) {
    LowerFloatConditionValue(cg, ins);
  }
  static void LowerFEq(CodeGen* cg, const HInstruction* ins) {
    LowerFloatConditionValue(cg, ins);
  }

 private:
  static void LowerCmpBoxIs(CodeGen*, const HInstruction*);
  static void LowerCmpBoxIsHeap(CodeGen* cg, const HInstruction* ins) {
    LowerCmpBoxIs(cg, ins);
  }
  static void LowerCmpBoxIsPrimitive(CodeGen* cg, const HInstruction* ins) {
    LowerCmpBoxIs(cg, ins);
  }

 private:
  // --------------------------------------------------------------------------
  // cast
  static void LowerCastF64ToB32(CodeGen*, const HInstruction*);
  static void LowerCastBToStr(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // runtime call
  static void LowerRCallGP(CodeGen*, const HInstruction*);
  static void LowerRCallFP(CodeGen*, const HInstruction*);
  static std::uint64_t GetRCallAddress(const HInstruction* ins);

  static void LowerRCallPtr(CodeGen* cg, const HInstruction* ins) {
    LowerRCallGP(cg, ins);
  }
  static void LowerRCallB64(CodeGen* cg, const HInstruction* ins) {
    LowerRCallGP(cg, ins);
  }

  static void LowerRCallI32(CodeGen* cg, const HInstruction* ins) {
    LowerRCallGP(cg, ins);
  }

  static void LowerRCallF64(CodeGen* cg, const HInstruction* ins) {
    LowerRCallFP(cg, ins);
  }

  static void LowerRCallVoid(CodeGen* cg, const HInstruction* ins);
  static void LowerRCallBox(CodeGen* cg, const HInstruction* ins);

  // --------------------------------------------------------------------------
  // JS Call
  // --------------------------------------------------------------------------
  static void LowerJSCallF(CodeGen*, const HInstruction*);
  static void LowerJSCallFT(CodeGen*, const HInstruction*);
  static void LowerJSCallFN(CodeGen*, const HInstruction*);
  static void LowerJSCallFTN(CodeGen*, const HInstruction*);
  static void LowerJSCallCtor(CodeGen*, const HInstruction*);
  static void LowerArrayFrom(CodeGen*, const HInstruction*);
  static void LowerApply(CodeGen*, const HInstruction*);

  static void PrepareScriptCallFunctionObject(CodeGen*, const HOperand*);
  static void PrepareScriptCallThisObject(CodeGen*, const HOperand*);
  static void PrepareScriptCallThisObjectUndefined(CodeGen*);
  static void PrepareScriptCallTargetObject(CodeGen*, const HOperand*);
  static void PrepareScriptCallTargetObjectUndefined(CodeGen*);
  static void PrepareScriptCallReturnAddress(CodeGen*, const HInstruction*);
  static void PrepareScriptCallParameterStart(CodeGen*, const HOperand*);
  static void PrepareScriptCallParameterSize(CodeGen*, const HOperand*);
  static void PrepareScriptCallOption(CodeGen*, const HOperand*);
  static void MaterializeScriptCallOutput(CodeGen*, const HInstruction*);

  static void PerformScriptCall(CodeGen*);
  static void PerformScriptCallCtor(CodeGen*);
  static void PerformScriptCallArrayFrom(CodeGen*);
  static void PerformScriptCallApply(CodeGen*);

 private:
  // --------------------------------------------------------------------------
  // Object Property/Element
  // --------------------------------------------------------------------------

  // This specialization of field get utilize a simple schema, which currently
  // does not use speculative method, ie via inline cache stub etc ... We will
  // add a inline cache system in the future to help boost the performance of
  // object field access, mainly because of our current method does not scale to
  // property store :(
  static void LowerObjectFieldGetAtomFast(CodeGen*, const HInstruction*);
  static void LowerGlobalGetVarAtomFast(CodeGen*, const HInstruction*);
  static void LowerGlobalGetGlobalAtomFast(CodeGen*, const HInstruction*);

  enum class ArrayNativeType {
    kInt8,
    kInt16,
    kInt32,
    kUInt8,
    kUInt16,
    kUInt32,
    kF32,
    kF64,
    kJSValue
  };

  static void DoLowerLoadArray(CodeGen* cg, const HInstruction* ins,
                               int class_id, std::size_t byte_size,
                               ArrayNativeType native_type);

  static void LowerLoadArrayValue(CodeGen*, const HInstruction*);
  static void LowerLoadArrayArguments(CodeGen*, const HInstruction*);
  static void LowerLoadArrayInt8(CodeGen*, const HInstruction*);
  static void LowerLoadArrayUInt8(CodeGen*, const HInstruction*);
  static void LowerLoadArrayUInt8C(CodeGen*, const HInstruction*);
  static void LowerLoadArrayInt16(CodeGen*, const HInstruction*);
  static void LowerLoadArrayUInt16(CodeGen*, const HInstruction*);
  static void LowerLoadArrayInt32(CodeGen*, const HInstruction*);
  static void LowerLoadArrayUInt32(CodeGen*, const HInstruction*);
  static void LowerLoadArrayF32(CodeGen*, const HInstruction*);
  static void LowerLoadArrayF64(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // Context
  static void LowerContextLoad(CodeGen*, const HInstruction*);
  static void LowerContextLoadGlobalObject(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // RC
  static void LowerRc(CodeGen*, const HInstruction*);
  static void LowerRcInc(CodeGen* cg, const HInstruction* ins) {
    LowerRc(cg, ins);
  }
  static void LowerRcDec(CodeGen* cg, const HInstruction* ins) {
    LowerRc(cg, ins);
  }

 private:
  // --------------------------------------------------------------------------
  // Box/Unbox
  static void DoGetTTagPtr64(CodeGen*, const HOperand*);
  static void DoGetTValPtr64(CodeGen*, const HOperand*);
  static void DoGetTTag(CodeGen*, const HOperand*);
  static void DoGetTVal(CodeGen*, const HOperand*);

  static int GetTTagFromOperand(const HOperand*);

  static void LowerGUnbox32BitPtr64(CodeGen*, const HInstruction*, int);

  static void LowerGUnboxIPtr64(CodeGen*, const HInstruction*);
  static void LowerGUnboxFPtr64(CodeGen*, const HInstruction*);
  static void LowerGUnboxBPtr64(CodeGen*, const HInstruction*);
  static void LowerGUnboxPtrPtr64(CodeGen*, const HInstruction*);

  static void LowerGUnboxI(CodeGen* cg, const HInstruction* ins) {
    LowerGUnboxIPtr64(cg, ins);
  }
  static void LowerGUnboxB(CodeGen* cg, const HInstruction* ins) {
    LowerGUnboxBPtr64(cg, ins);
  }
  static void LowerGUnboxF(CodeGen* cg, const HInstruction* ins) {
    LowerGUnboxFPtr64(cg, ins);
  }
  static void LowerGUnboxPtr(CodeGen* cg, const HInstruction* ins) {
    LowerGUnboxPtrPtr64(cg, ins);
  }

  static void LowerUnbox32BitPtr64(CodeGen*, const HInstruction*);

  static void LowerUnboxPtrPtr64(CodeGen* cg, const HInstruction* ins);
  static void LowerUnboxIPtr64(CodeGen* cg, const HInstruction* ins);
  static void LowerUnboxFPtr64(CodeGen* cg, const HInstruction* ins);
  static void LowerUnboxBPtr64(CodeGen* cg, const HInstruction* ins);

  static void LowerUnboxI(CodeGen* cg, const HInstruction* ins) {
    LowerUnboxIPtr64(cg, ins);
  }
  static void LOwerUnboxB(CodeGen* cg, const HInstruction* ins) {
    LowerUnboxBPtr64(cg, ins);
  }
  static void LowerUnboxF(CodeGen* cg, const HInstruction* ins) {
    LowerUnboxFPtr64(cg, ins);
  }
  static void LowerUnboxPtr(CodeGen* cg, const HInstruction* ins) {
    return LowerUnboxPtrPtr64(cg, ins);
  }

  static void LowerBoxPtr64(CodeGen*, const HInstruction*);
  static void LowerBox(CodeGen* cg, const HInstruction* ins) {
    LowerBoxPtr64(cg, ins);
  }
  static void LowerGetTTag(CodeGen*, const HInstruction*);
  static void LowerIsNullOrUndefined(CodeGen*, const HInstruction*);

  // atom
  static void LowerIsBoxAtomInt(CodeGen*, const HInstruction*);
  static void LowerAtomFromI(CodeGen*, const HInstruction*);
  static void LowerGAtom(CodeGen*, const HInstruction*);

 private:
  // deoptimization
  static DeoptId GetDeoptId(const HInstruction*);
  static void Deoptimize(CodeGen*, const HInstruction*);
  static void LowerDeopt(CodeGen*, const HInstruction*);

  static void LowerDeoptIfTrueOrFalse(CodeGen*, const HInstruction*);
  static void LowerDeoptIfTrue(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptIfTrueOrFalse(cg, ins);
  }
  static void LowerDeoptIfFalse(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptIfTrueOrFalse(cg, ins);
  }

  static void LowerDeoptCC(CodeGen*, const HInstruction*);
  static void LowerDeoptZ(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptNz(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }

  static void LowerDeoptGe(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptGt(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptLe(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptLt(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptEq(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptNe(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }

  static void LowerDeoptAe(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptAt(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptBe(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }
  static void LowerDeoptBt(CodeGen* cg, const HInstruction* ins) {
    LowerDeoptCC(cg, ins);
  }

 private:
  // It turns out that the most common polymorphic usage of assmebly
  // instruction is the mov. The following helper function abstract away
  // the mov variant in terms of our operand type and mov types.
  static void Mov(CodeGen*, const HOperand*, const HOperand*,
                  a::x86::Gp scratch);
  static void Mov(CodeGen*, const HOperand*, a::Imm);
  static void Mov(CodeGen*, const HOperand*, a::x86::Gp);
  static void Mov(CodeGen*, const HOperand*, a::x86::Mem, a::x86::Gp scratch);
  static void Mov(CodeGen*, a::x86::Gp, const HOperand*);

  enum class MMType { kRr, kRR, kRM, kRI, kMR, kMI, kMM };

  static MMType DecideMMType(const HOperand*, const HOperand*);

  // make sure input is in memory, otherwise crash
  static a::x86::Gp MoveMemoryToScratch(CodeGen*, const HOperand*);

 private:
  static std::uint32_t ImmAsIndex(const HOperand*);
  static void DoLoadBoxValueFromScratchPtr64(CodeGen*, const HInstruction*,
                                             std::uint32_t);

  static_assert(sizeof(JSValue) == 16,
                "assumption is each JSValue uses 2 qword");

 private:
  TranslateRuleTable();
  void InternalInit();
  void AddRule(HOpcode, const std::string_view&, TransEntry);

  bool TryWildcardPattern(const std::string_view&,
                          const std::string_view&) const;
  bool TryZeroParamPattern(const std::string_view&,
                           const std::string_view&) const;

 private:
  struct OpcodeTransEntry {
    std::string pattern;
    TransEntry entry;
    OpcodeTransEntry(std::string&& p, TransEntry e)
        : pattern(std::move(p)), entry(e) {}
  };

  using OpcodeTable = std::vector<OpcodeTransEntry>;
  using TransTable = std::vector<OpcodeTable>;

  TransTable table_;
};

void TranslateRuleTable::InternalInit() {
  table_.resize(static_cast<std::size_t>(HOpcode::kSizeOfHOpcode));
}

// ----------------------------------------------------------------------------
// Rule formation

void TranslateRuleTable::AddRule(HOpcode opcode,
                                 const std::string_view& pattern,
                                 TransEntry converter) {
  auto idx = static_cast<std::uint32_t>(opcode);
  CHECK(table_.size() > idx);

  // sanity check for duplicated rule entry
  BASE_DBG({
    for (auto& x : table_[idx]) {
      CHECK(x.pattern != pattern);
    }
  });

  table_[idx].emplace_back(std::to_string(pattern), converter);
}

bool TranslateRuleTable::TryZeroParamPattern(
    const std::string_view& current, const std::string_view& pattern) const {
  if (pattern.empty() || pattern == "-") {
    return current.empty();
  }
  if (pattern.size() >= 1 && std::isupper(pattern[0])) {
    auto p = std::to_string(pattern);
    if (p.back() == '-') p.pop_back();
    return p == current;
  }
  return false;
}

// Notes, the wildcard pattern only works on the operand, return value of
// instruction cannot be marked as wildcard for now
bool TranslateRuleTable::TryWildcardPattern(
    const std::string_view& current, const std::string_view& pattern) const {
  if (pattern == "*") {
    if (current.size()) {
      // special cases, that the instruction can have any types of operand
      // but no return
      return std::islower(current[0]);  // cannot have return
    }
    return true;
  }

  if (pattern.size() && pattern.back() == '*') {
    // the wildcard pattern currently supports partial matching until we hit
    // a unknown position
    std::size_t pos = 0;
    for (; pos < current.size(); pos++) {
      if (pattern[pos] == '*') break;
      if (pattern[pos] != current[pos]) return false;
    }
    return true;  // rest should match, since we have wildcard
  }

  return false;
}

TransEntry TranslateRuleTable::MatchRule(const HInstruction* ins) const {
  static thread_local std::string kMatchBuffer;  // use TLS to save us from
                                                 // using std::string everytime

  auto idx = static_cast<std::uint32_t>(ins->opcode());
  const auto& entry = table_[idx];

  // covnert the operand to matching character
  auto operand_to_kind = [&](const HOperand* opr) -> char {
    if (opr->IsImm()) {
      return 'i';
    }
    if (opr->IsMemory()) {
      return 'm';
    }
    if (opr->IsRegister()) {
      return opr->IsFP() ? 'x' : 'r';
    }
    if (opr->IsLabel()) {
      return 'b';
    }
    if (opr->IsTemporary()) {
      UNREACHABLE() << "All temporary should be resolved by RA: "
                    << ins->Print();
    }
    if (opr->IsNone()) {
      UNREACHABLE() << "None should be replaced with nullptr";
    }

    return 'X';
  };

  // Build the pattern, ie pattern formation
  kMatchBuffer.clear();

  // (0) output
  if (auto out = ins->output(); out) {
    auto norm_chars = operand_to_kind(out);
    kMatchBuffer.push_back(std::toupper(norm_chars));
  }

  // (1) rest of the parameters
  {
    const auto len = ins->opr_size();
    for (auto i = 0; i < len; i++) {
      auto opr = ins->Opr(i);
      CHECK(opr);
      kMatchBuffer.push_back(operand_to_kind(opr));
    }
  }

  for (const auto& e : entry) {
    if (kMatchBuffer == e.pattern) {
      return e.entry;
    }
    if (TryZeroParamPattern(kMatchBuffer, e.pattern)) return e.entry;
  }

  // (2) okay, we cannot find the specific pattern, now let's try wildcard
  //     pattern. Notes wildcard pattern always fallback
  for (const auto& e : entry) {
    if (TryWildcardPattern(kMatchBuffer, e.pattern)) return e.entry;
  }

  return nullptr;
}

a::x86::Gp TranslateRuleTable::MoveMemoryToScratch(CodeGen* cg,
                                                   const HOperand* val) {
  CHECK(val->IsMemory());

  switch (val->SizeInBytes()) {
    case 4:
      __ mov(SCR_GPD, MemoryOperandToASMJIT(val));
      return SCR_GPD;
      break;

    case 2:
      __ mov(SCR_GPW, MemoryOperandToASMJIT(val));
      return SCR_GPW;
      break;

    case 1:
      __ mov(SCR_GPB, MemoryOperandToASMJIT(val));
      return SCR_GPB;
      break;

    case 8:
      __ mov(SCR_GPQ, MemoryOperandToASMJIT(val));
      return SCR_GPQ;
      break;

    default:
      UNREACHABLE();
      return SCR_GPQ;
  }
}

TranslateRuleTable::MMType TranslateRuleTable::DecideMMType(
    const HOperand* dest, const HOperand* src) {
  if (dest->IsRegister()) {
    if (src->IsRegister()) {
      return dest->GetRegister() == src->GetRegister() ? MMType::kRr
                                                       : MMType::kRR;
    } else if (src->IsMemory()) {
      return MMType::kRM;
    } else {
      return MMType::kRI;
    }
  } else {
    CHECK(dest->IsMemory());
    if (src->IsRegister()) {
      return MMType::kMR;
    } else if (src->IsMemory()) {
      return MMType::kMM;
    } else {
      return MMType::kMI;
    }
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to,
                             const HOperand* from, a::x86::Gp scratch) {
  CHECK(!to->IsFloat());
  CHECK(!from->IsFloat());

  if (from->IsImm()) {
    Mov(cg, to, ImmOperandToASMJIT(from));
  } else if (from->IsRegister()) {
    CHECK(from->GetRegister()->IsGP());
    Mov(cg, to, GPRegOperandToASMJIT(from));
  } else if (from->IsMemory()) {
    Mov(cg, to, MemoryOperandToASMJIT(from), scratch);
  } else {
    UNREACHABLE();
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to, a::Imm imm) {
  CHECK(!to->IsFloat());
  CHECK(IsX64AllowedOutput(to));

  if (to->IsRegister()) {
    // Notes, if user want to move a IMM to XMM, either the IMM must be a double
    // which cannot be represented for now in our framework. Or it requires a
    // conversion, which needs to use cvtXXX instruction which is not a mov
    CHECK(to->GetRegister()->IsGP());
    __ mov(GPRegOperandToASMJIT(to), imm);
  } else {
    __ mov(MemoryOperandToASMJIT(to), imm);
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to, a::x86::Gp reg) {
  CHECK(!to->IsFloat());
  CHECK(IsX64AllowedOutput(to));

  if (to->IsRegister()) {
    CHECK(to->GetRegister()->IsGP());
    __ mov(GPRegOperandToASMJIT(to), reg);
  } else {
    __ mov(MemoryOperandToASMJIT(to), reg);
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to, a::x86::Mem mem,
                             a::x86::Gp scratch) {
  CHECK(!to->IsFloat());
  CHECK(IsX64AllowedOutput(to));

  if (to->IsRegister()) {
    CHECK(to->GetRegister()->IsGP());
    __ mov(GPRegOperandToASMJIT(to), mem);
  } else {
    // mov scratch, [mem]
    __ mov(scratch, mem);

    // mov [mem], scratch
    __ mov(MemoryOperandToASMJIT(to), scratch);
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, a::x86::Gp to, const HOperand* from) {
  CHECK(!from->IsFloat());

  if (from->IsRegister()) {
    __ mov(to, GPRegOperandToASMJIT(from));
  } else if (from->IsMemory()) {
    __ mov(to, MemoryOperandToASMJIT(from));
  } else if (from->IsImm()) {
    __ mov(to, ImmOperandToASMJIT(from));
  } else {
    UNREACHABLE();
  }
}

std::uint32_t TranslateRuleTable::ImmAsIndex(const HOperand* opr) {
  CHECK(opr->IsImm());
  const auto& imm = opr->GetImm();
  auto index = imm.Int32();
  CHECK(index >= 0);
  return static_cast<std::uint32_t>(index);
}

// The actual boxed value will be stored on a cell region that lives on the
// stack and the LoadXXX instruction will just return an address that points
// to that cell region. The cell region is been indicated by cell_index of
// each instruction

void TranslateRuleTable::DoLoadBoxValueFromScratchPtr64(CodeGen* cg,
                                                        const HInstruction* ins,
                                                        std::uint32_t offset) {
  CHECK(ins->IsUnary());
  CHECK(ins->HasOutput());
  CHECK(!ins->IsFloat());
  CHECK(ins->output_size_class() == SizeClass::kQWord);

  static const std::size_t kSize = Layout::kCellSize;
  static_assert(kSize == 16);

  auto out = ins->output();
  auto index = ImmAsIndex(ins->Opr(0));

  // We do not need to copy the value from CTX into the local cell region, but
  // just need to hold a pointer points back to where the CTX store the boxed
  // value
  {
    __ mov(SCR_GPQ, a::x86::qword_ptr(CTX, offset));  // pointer to XXX array
    __ lea(SCR_GPQ, a::x86::ptr(SCR_GPQ, index * kSize));  // get the address
  }

  if (out->IsRegister()) {
    auto o = GPRegOperandToASMJIT(out);
    __ mov(o, SCR_GPQ);
  } else if (out->IsMemory()) {
    auto o = MemoryOperandToASMJIT(out);
    __ mov(o, SCR_GPQ);
  } else {
    UNREACHABLE();
  }
}

void TranslateRuleTable::LowerLoadLocal(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadLocal);
  DoLoadBoxValueFromScratchPtr64(cg, ins, Layout::Context::kLocal);
}

void TranslateRuleTable::LowerLoadParam(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadParam);
  DoLoadBoxValueFromScratchPtr64(cg, ins, Layout::Context::kParam);
}

void TranslateRuleTable::LowerLoadTemp(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadTemp);
  DoLoadBoxValueFromScratchPtr64(cg, ins, Layout::Context::kTemp);
}

void TranslateRuleTable::LowerBoxToTemp(CodeGen* cg, const HInstruction* ins) {
  auto opr_value = ins->Opr(0);
  auto opr_tag = ins->Opr(1);
  CHECK(opr_tag->IsImm());
  auto opr_idx = ins->Opr(2);
  CHECK(opr_idx->IsImm());

  auto ttag = opr_tag->GetImm().Int32();
  auto idx = opr_idx->GetImm().Int32();

  __ mov(SCR_GPQ, a::x86::qword_ptr(CTX, Layout::Context::kTemp));
  __ lea(SCR_GPQ, a::x86::ptr(SCR_GPQ, idx * Layout::kCellSize));

  auto cell_address = a::x86::ptr(SCR_GPQ);

  // now rax has starting address of memory
  if (opr_value->IsRegister()) {
    if (auto r = opr_value->GetRegister(); r->IsFP()) {
      __ store_cell(cell_address, FPRegOperandToASMJIT(opr_value), ttag);
    } else {
      __ store_cell(cell_address, GPRegOperandToASMJIT(opr_value), ttag);
    }
  } else if (opr_value->IsMemory()) {
    __ store_cell(cell_address, MoveMemoryToScratch(cg, opr_value), ttag);
  } else {
    const auto& imm = opr_value->GetImm();

    auto tag_addr = a::x86::qword_ptr(SCR_GPQ, Layout::Value::kTag);

    switch (imm.type()) {
      case Immediate::kInt32: {
        auto val_addr = a::x86::dword_ptr(SCR_GPQ, Layout::Value::kValue);
        __ mov(val_addr, imm.Int32());
        __ mov(tag_addr, JVal::kInt);
      } break;

      default:
        UNREACHABLE() << "never reach here";
        break;
    }
  }
}

void TranslateRuleTable::LowerStoreTemp(CodeGen* cg, const HInstruction* ins) {
  auto input = ins->Opr(0);
  auto opr_idx = ins->Opr(1);
  CHECK(opr_idx->IsImm());

  auto idx = opr_idx->GetImm().Int32();

  __ mov(SCR_GPQ, a::x86::qword_ptr(CTX, Layout::Context::kTemp));
  __ mov(SCR_GPQ, a::x86::ptr(SCR_GPQ, idx * Layout::kCellSize));

  // basically just move input, which is a 16 bytes value to the target 16
  // bytes value position specified by output.

  if (input->IsRegister()) {
    auto from = GPRegOperandToASMJIT(input);
    __ move128(SCR_GPQ, from);
  } else {
    auto from = MemoryOperandToASMJIT(input);

    // spill scratch on stack since we need it to hold the from's result
    auto spill = __ StackTemp0();
    __ mov(spill, SCR_GPQ);

    // let scratch hold the result of from, since this memory address holds
    // a pointer to a 128bits value
    __ mov(SCR_GPQ, from);

    // move the value to xmm register
    __ movups(SCR_FP, a::x86::xmmword_ptr(SCR_GPQ));

    // restore the GPQ
    __ mov(SCR_GPQ, spill);

    // move xmm register's 16 bits value back to SCR_GPQ pointed address
    __ movups(a::x86::xmmword_ptr(SCR_GPQ), SCR_FP);
  }
}

void TranslateRuleTable::LowerLeaTemp(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto opr_offset = ins->Opr(0);
  CHECK(opr_offset->IsImm());  // must be immediate for now

  auto offset = opr_offset->GetImm().Int32();

  __ mov(SCR_GPQ, a::x86::qword_ptr(CTX, Layout::Context::kTemp));

  auto addr = a::x86::ptr(SCR_GPQ, offset * Layout::kCellSize);
  if (out->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(out), addr);
  } else {
    __ lea(SCR_GPQ, addr);
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  }
}

void TranslateRuleTable::LowerMoveMM(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kMov);
  CHECK(ins->opr_size() == 1);
  CHECK(ins->HasOutput());

  auto opr0 = ins->output();
  auto opr1 = ins->Opr(0);

  CHECK(opr0->IsMemory());
  CHECK(opr1->IsMemory());

  auto asmjit_opr0 = MemoryOperandToASMJIT(opr0);
  auto asmjit_opr1 = MemoryOperandToASMJIT(opr1);

  CHECK(opr0->size_class() == opr1->size_class());

  switch (opr0->size_class()) {
    case SizeClass::kByte:
      __ mov(SCR_GPB, asmjit_opr1);
      __ mov(asmjit_opr1, SCR_GPB);
      break;

    case SizeClass::kWord:
      __ mov(SCR_GPW, asmjit_opr1);
      __ mov(asmjit_opr0, SCR_GPW);
      break;

    case SizeClass::kDWord:
      __ mov(SCR_GPD, asmjit_opr1);
      __ mov(asmjit_opr0, SCR_GPD);
      break;

    case SizeClass::kQWord:
      __ mov(SCR_GPQ, asmjit_opr1);
      __ mov(asmjit_opr0, SCR_GPQ);
      break;

    default:
      UNREACHABLE();
      break;
  }
}

void TranslateRuleTable::LowerMoveFMM(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->output();
  auto opr1 = ins->Opr(0);

  CHECK(ins->opcode() == HOpcode::kMovF);
  CHECK(ins->opr_size() == 1);
  CHECK(ins->HasOutput());
  CHECK(opr0->IsMemory());
  CHECK(opr1->IsMemory());
  CHECK(opr0->IsFloat());
  CHECK(opr1->IsFloat());
  CHECK(opr0->size_class() == opr1->size_class());
  CHECK(opr0->IsQWord());

  auto asmjit_opr0 = MemoryOperandToASMJIT(opr0);
  auto asmjit_opr1 = MemoryOperandToASMJIT(opr1);

  __ movsd(SCR_FP, asmjit_opr0);
  __ movsd(asmjit_opr1, SCR_FP);
}

void TranslateRuleTable::DoMoveB64(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);
  CHECK(ins->HasOutput());

  auto opr0 = ins->output();
  auto opr1 = ins->Opr(0);
  CHECK(opr1->IsImm() && opr1->GetImm().IsInt64());
  auto raw_bits = opr1->GetImm().Int64();

  // mov rax, imm[64]
  __ movabs(a::x86::rax, a::Imm(raw_bits));

  // mov output, rax
  if (opr0->IsRegister()) {
    auto o = GPRegOperandToASMJIT(opr0);
    __ mov(o, a::x86::rax);
  } else if (opr0->IsMemory()) {
    auto o = MemoryOperandToASMJIT(opr0);
    __ mov(o, a::x86::rax);
  } else {
    UNREACHABLE();
  }
}

void TranslateRuleTable::LowerMoveI64(CodeGen* cg, const HInstruction* ins) {
  return DoMoveB64(cg, ins);
}

void TranslateRuleTable::LowerMoveB64(CodeGen* cg, const HInstruction* ins) {
  return DoMoveB64(cg, ins);
}

void TranslateRuleTable::LowerMoveF64(CodeGen* cg, const HInstruction* ins) {
  // to translate materialization of immediate number of float64 into a XMM
  // register is kind of tricky. One way is to store the literal in memory
  // and materialize it via a movsd, the otherway will be mov immediate into
  // rax and then mov rax into xmm via movq.
  // For now we just mov the imm into rax and then mov rax into xmm via movq
  CHECK(ins->IsFloat());

  auto opr0 = ins->output();
  auto opr1 = ins->Opr(0);
  CHECK(opr1->IsImm() && opr1->GetImm().IsFloat64());

  const auto float64 = opr1->GetImm().Float64();

  auto value_mem = cg->code()->AddConst(float64);

  if (opr0->IsRegister()) {
    auto r = opr0->GetRegister();
    CHECK(r->IsFP());
    auto xmm = FPRegOperandToASMJIT(opr0);
    __ movq(xmm, value_mem);
  } else if (opr0->IsMemory()) {
    auto mem = MemoryOperandToASMJIT(opr0);
    __ movq(SCR_FP, value_mem);
    __ movq(mem, SCR_FP);
  } else {
    UNREACHABLE();
  }
}

// ----------------------------------------------------------------------------
// unary operation lowering
void TranslateRuleTable::LowerIncDecF(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);

  auto data = cg->code()->const_f64_one();  // get float64 1's label
  auto opr0 = ins->Opr(0);                  // operand 0
  auto out = ins->output();                 // output

  auto is_inc = ins->opcode() == HOpcode::kIncF;

  if (opr0->IsRegister()) {
    auto opr0_reg = FPRegOperandToASMJIT(opr0);

    if (out->IsRegister()) {
      auto out_reg = FPRegOperandToASMJIT(out);

      if (out->GetRegister() != opr0->GetRegister()) {
        __ movq(out_reg, opr0_reg);
      }

      if (is_inc) {
        __ addsd(out_reg, data);
      } else {
        __ subsd(out_reg, data);
      }
    } else {
      CHECK(out->IsMemStack());
      __ movq(SCR_FP, opr0_reg);

      if (is_inc) {
        __ addsd(SCR_FP, data);
      } else {
        __ subsd(SCR_FP, data);
      }

      __ movq(MemoryOperandToASMJIT(out), SCR_FP);
    }
  } else {
    CHECK(opr0->IsMemStack());
    auto opr0_mem = MemoryOperandToASMJIT(opr0);

    if (out->IsRegister()) {
      auto out_reg = FPRegOperandToASMJIT(out);
      __ movq(out_reg, opr0_mem);
      if (is_inc) {
        __ addsd(out_reg, data);
      } else {
        __ subsd(out_reg, data);
      }
    } else {
      CHECK(out->IsMemStack());
      __ movq(SCR_FP, opr0_mem);

      if (is_inc) {
        __ addsd(SCR_FP, data);
      } else {
        __ subsd(SCR_FP, data);
      }

      __ movq(MemoryOperandToASMJIT(out), SCR_FP);
    }
  }
}

void TranslateRuleTable::LowerGIncDec(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);

  auto opr0 = ins->Opr(0);   // operand 0
  auto out = ins->output();  // output
  auto is_inc = ins->opcode() == HOpcode::kGInc;

  if (opr0->IsRegister()) {
    auto opr0_reg = GPRegOperandToASMJIT(opr0);
    if (is_inc) {
      __ inc(opr0_reg);
    } else {
      __ dec(opr0_reg);
    }
    if (out->IsRegister()) {
      if (out->GetRegister() != opr0->GetRegister()) {
        __ mov(GPRegOperandToASMJIT(out), opr0_reg);
      }
    } else {
      CHECK(out->IsMemory());
      __ mov(MemoryOperandToASMJIT(out), opr0_reg);
    }
  } else {
    auto opr0_mem = MemoryOperandToASMJIT(opr0);
    if (is_inc) {
      __ inc(opr0_mem);
    } else {
      __ dec(opr0_mem);
    }
    if (out->IsRegister()) {
      __ mov(GPRegOperandToASMJIT(out), opr0_mem);
    } else {
      __ mov(SCR_GPD, opr0_mem);
      __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
    }
  }
}

void TranslateRuleTable::LowerNegF(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);

  auto opr0 = ins->Opr(0);
  auto out = ins->output();
  auto neg_zero = cg->code()->const_f64_neg_zero_xmmword();

  if (opr0->IsRegister()) {
    auto opr0_reg = FPRegOperandToASMJIT(opr0);

    if (out->IsRegister()) {
      if (out->GetRegister() == opr0->GetRegister()) {
        __ xorps(FPRegOperandToASMJIT(out), neg_zero);
      } else {
        __ movq(FPRegOperandToASMJIT(out), opr0_reg);
        __ xorps(FPRegOperandToASMJIT(out), neg_zero);
      }
    } else {
      __ movq(SCR_FP, opr0_reg);
      __ xorps(SCR_FP, neg_zero);
      __ movq(MemoryOperandToASMJIT(out), SCR_FP);
    }
  } else {
    auto opr0_mem = MemoryOperandToASMJIT(opr0);
    if (out->IsRegister()) {
      auto out_reg = FPRegOperandToASMJIT(out);
      __ movq(out_reg, opr0_mem);
      __ xorps(out_reg, neg_zero);
    } else {
      __ movq(SCR_FP, opr0_mem);
      __ xorps(SCR_FP, neg_zero);
      __ movq(MemoryOperandToASMJIT(out), SCR_FP);
    }
  }
}

// guarded negate of int32, notes if int32 is max positive value, then negate
// it will cause overflow. Notes, the neg instruction will set O flag
void TranslateRuleTable::LowerGNeg(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);
  CHECK(ins->HasDeoptId());

  auto opr0 = ins->Opr(0);
  auto out = ins->output();

  if (out->IsRegister()) {
    auto opr0_reg = GPRegOperandToASMJIT(opr0);
    __ neg(opr0_reg);
    if (out->IsRegister()) {
      if (out->GetRegister() != opr0->GetRegister()) {
        __ mov(GPRegOperandToASMJIT(out), opr0_reg);
      }
    } else {
      __ mov(MemoryOperandToASMJIT(out), opr0_reg);
    }
  } else {
    auto opr0_mem = MemoryOperandToASMJIT(opr0);
    if (out->IsRegister()) {
      auto out_reg = GPRegOperandToASMJIT(out);
      __ mov(out_reg, opr0_mem);
      __ inc(out_reg);
    } else {
      __ mov(SCR_GPD, opr0_mem);
      __ inc(SCR_GPD);
      __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
    }
  }

  // check whether overflow, otherwise deoptimize
  auto label = __ newLabel();
  __ jno(label);
  Deoptimize(cg, ins);
  __ bind(label);
}

// ----------------------------------------------------------------------------
// integer comparison conditional value
void TranslateRuleTable::LowerIntConditionValue(CodeGen* cg,
                                                const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();

  CHECK(opr0->IsRegister());  // guaranteed by RA

  auto opr0_reg = GPRegOperandToASMJIT(opr0);

  // clean up the target if needed. notes the cleanup code should before the
  // cmp since it may impact flag
  if (out->IsRegister()) {
    auto r = GPRegOperandToASMJIT(out);
    __ xor_(r, r);
  } else {
    __ xor_(SCR_GPD, SCR_GPD);
  }

  // generate test/cmp sequences
  switch (ins->opcode()) {
    case HOpcode::kILt:
    case HOpcode::kILe:
    case HOpcode::kIGe:
    case HOpcode::kIGt:
    case HOpcode::kIEq:
    case HOpcode::kINe:
      if (opr1->IsImm()) {
        const auto& imm = opr1->GetImm();
        if (imm.Int32() == 0) {
          __ test(opr0_reg, opr0_reg);  // if target is 0, then just use test
        } else {
          __ cmp(opr0_reg, ImmOperandToASMJIT(opr1));
        }
      } else if (opr1->IsRegister()) {
        __ cmp(opr0_reg, GPRegOperandToASMJIT(opr1));
      } else {
        CHECK(opr1->IsMemory());
        __ cmp(opr0_reg, MemoryOperandToASMJIT(opr1));
      }
      break;

    default:
      UNREACHABLE();
      break;
  }

  // now let's use setcc instruction to perform the operation
  switch (ins->opcode()) {
    case HOpcode::kILt:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setl(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setl(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kILe:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setle(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setle(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kIGt:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setg(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setg(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kIGe:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setge(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setge(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kIEq:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ sete(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ sete(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kINe:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setne(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setne(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    default:
      UNREACHABLE();
      break;
  }
}

void TranslateRuleTable::LowerFloatConditionValue(CodeGen* cg,
                                                  const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();

  CHECK(opr0->IsRegister());  // guaranteed by RA

  auto opr0_reg = FPRegOperandToASMJIT(opr0);

  // clean up the target if needed. notes the cleanup code should before the
  // cmp since it may impact flag
  if (out->IsRegister()) {
    auto r = GPRegOperandToASMJIT(out);
    __ xor_(r, r);
  } else {
    __ xor_(SCR_GPD, SCR_GPD);
  }

  // generate test/cmp sequences
  switch (ins->opcode()) {
    case HOpcode::kFLt:
    case HOpcode::kFLe:
    case HOpcode::kFGe:
    case HOpcode::kFGt:
    case HOpcode::kFEq:
    case HOpcode::kFNe:
      if (opr1->IsRegister()) {
        __ comisd(opr0_reg, FPRegOperandToASMJIT(opr1));
      } else {
        CHECK(opr1->IsMemory());
        __ comisd(opr0_reg, MemoryOperandToASMJIT(opr1));
      }
      break;

    default:
      UNREACHABLE();
      break;
  }

  // now let's use setcc instruction to perform the operation
  switch (ins->opcode()) {
    case HOpcode::kFLt:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setl(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setl(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kFLe:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setle(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setle(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kFGt:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setg(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setg(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kFGe:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setge(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setge(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kFEq:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ sete(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ sete(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    case HOpcode::kFNe:
      if (out->IsRegister()) {
        auto out_r = GPRegOperandToASMJIT(out);
        __ setne(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setne(SCR_GPB);
        __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      }
      break;

    default:
      UNREACHABLE();
      break;
  }
}

void TranslateRuleTable::LowerCmpBoxIs(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);
  auto opr0 = ins->Opr(0);  // must be a kBoxRef, ie pointer to JSValue

  DoGetTTag(cg, opr0);  // get the tag of the operand, and its value is in eax

  // Notes, we just do a simple test comparison, since we will translate
  // kJmpBoxIsXXX to corresponding jlt (<0, heap) or jge (>=0, primitive)
  __ test(SCR_GPD, SCR_GPD);
}

// ----------------------------------------------------------------------------
// arithmetic operation lowering
void TranslateRuleTable::LowerGuardIntArithmetic(CodeGen* cg,
                                                 const HInstruction* ins) {
  CHECK(ins->HasDeoptId());

  // perform an add operation
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();

  // opr0 can only be R for now and cannot be any other stuff
  CHECK(opr0->IsRegister());

  auto opr0_reg = GPRegOperandToASMJIT(opr0);

  // 1. shuffle first operand if needed, the first operand and result will
  // always be register
  if (out->IsRegister()) {
    auto tar = GPRegOperandToASMJIT(out);
    auto out_reg = tar;

    if (out->GetRegister() != opr0->GetRegister()) {
      __ mov(tar, opr0_reg);
    }

    switch (ins->opcode()) {
      case HOpcode::kGIAdd:
        if (opr1->IsImm()) {
          __ add(out_reg, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ add(out_reg, MemStackOperandToASMJIT(opr1));
        } else {
          CHECK(opr1->IsRegister());
          __ add(out_reg, GPRegOperandToASMJIT(opr1));
        }
        break;

      case HOpcode::kGISub:
        if (opr1->IsImm()) {
          __ sub(out_reg, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ sub(out_reg, MemStackOperandToASMJIT(opr1));
        } else {
          CHECK(opr1->IsRegister());
          __ sub(out_reg, GPRegOperandToASMJIT(opr1));
        }
        break;

      case HOpcode::kGIMul:
        if (opr1->IsImm()) {
          __ imul(out_reg, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ imul(out_reg, MemStackOperandToASMJIT(opr1));
        } else {
          CHECK(opr1->IsRegister());
          __ imul(out_reg, GPRegOperandToASMJIT(opr1));
        }
        break;

      default:
        UNREACHABLE();
        break;
    }
  } else {
    CHECK(out->IsMemStack());
    auto out_mem = MemStackOperandToASMJIT(out);

    // if output is a memory, then move the first operand into memory location
    // and perform the addition.
    __ mov(MemStackOperandToASMJIT(out), opr0_reg);

    switch (ins->opcode()) {
      case HOpcode::kGIAdd:
        if (opr1->IsImm()) {
          __ add(out_mem, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ mov(a::x86::eax, MemStackOperandToASMJIT(opr1));
          __ add(out_mem, a::x86::eax);
        } else {
          CHECK(opr1->IsRegister());
          __ add(out_mem, GPRegOperandToASMJIT(opr1));
        }
        break;

      case HOpcode::kGISub:
        if (opr1->IsImm()) {
          __ sub(out_mem, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ mov(a::x86::eax, MemStackOperandToASMJIT(opr1));
          __ sub(out_mem, a::x86::eax);
        } else {
          CHECK(opr1->IsRegister());
          __ sub(out_mem, GPRegOperandToASMJIT(opr1));
        }
        break;

      case HOpcode::kGIMul:
        // for imul, its destination cannot be a memory operand but have to
        // be a register, just use scratch for now
        if (opr1->IsImm()) {
          __ imul(a::x86::eax, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ imul(a::x86::eax, MemStackOperandToASMJIT(opr1));
        } else {
          CHECK(opr1->IsRegister());
          __ imul(a::x86::eax, GPRegOperandToASMJIT(opr1));
        }
        __ mov(out_mem, a::x86::eax);
        break;

      default:
        UNREACHABLE();
        break;
    }
  }

  // 3. perform overflow detection, if overflow, then just perform
  // deoptimzation
  auto label = __ newLabel();

  // jump when not overflow, ie skipping the deoptimization code
  __ jno(label);

  Deoptimize(cg, ins);

  // normal flow, ie recover from deoptimization
  __ bind(label);
}

// Fmod will be special runtime call calls into stub code generated by cg,
// similar as cast etc ..
//
// param(0) -> rsp
// param(1) -> rsp + 8
// result -> xmm15
//
void TranslateRuleTable::LowerFMod(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();
  CHECK(opr0->IsRegister());

  auto stk0 = __ StackTemp0();
  auto stk1 = __ StackTemp1();

  __ movsd(stk0, FPRegOperandToASMJIT(opr0));

  if (opr1->IsRegister()) {
    __ movsd(stk1, FPRegOperandToASMJIT(opr1));
  } else {
    __ movsd(SCR_FP, MemoryOperandToASMJIT(opr1));
    __ movsd(stk1, SCR_FP);
  }

  {
    auto opr1 = __ StackTemp0();
    auto opr2 = __ StackTemp1();

    __ fld(opr2);
    __ fld(opr1);

    auto mod_loop = __ newLabel();
    __ bind(mod_loop);

    // If we support sahf instruction, then we can save the following. Refactory
    // it until we have cpu feature flag setup
    {
      __ fprem();
      __ fnstsw(a::x86::ax);

      /**
       * V8 uses a rather convoluted way to work with partiy check, ie using
       * sahf instruction to load it back to eflags registers, or use a 4
       * instructions bit manipulation to expose the parity flag into the
       * eflags. However the GCC emittion of code uses a test for parity flag,
       * we prefer just to generate a test + jcc which can be fusioned inside
       * of modern arch
       */

      __ test(a::x86::ah, 0x4);  // this may not be performant due to the
                                 // bank conflicts, ie using al/ah is not ideal
                                 // most of the times
    }
    __ jne(mod_loop);
    __ fstp(a::x86::st(1));

    if (out->IsRegister()) {
      __ fstp(opr1);
      __ movsd(FPRegOperandToASMJIT(out), opr1);
    } else {
      __ fstp(MemoryOperandToASMJIT(out));
    }
  }
}

// Notes, CastF64ToB32 will force RA to reserve 1 register for us to be used
// here, and we have a rbp to be used here.
// Since F64ToB32 is essentially a small function to be inlined inside of the
// generated bytecode, we will use a special call and return instruction to
// do the trick. We will let LowerCastF64ToB32 calls into a special label and
// then return from it. The call is not a normal ABI call, but a special call
void TranslateRuleTable::LowerCastF64ToB32(CodeGen* cg,
                                           const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto out = ins->output();

  // notes(dpeng): The stack has to be 16 bytes aligned during usage of the SSE
  //   instructions, in our case we are gonna use it. Additionally, we will use
  //   rbp as a our scratch registers.
  //
  //   In this routine, we will have 3 scratch registers,
  //   1) RAX
  //   2) RBP
  //   3) RCX (guaranteed by the RA)
  auto temp_slot = a::x86::qword_ptr(a::x86::rsp, 0);

  __ mov(temp_slot, a::x86::rbp);  // push rbp to be used as scratch register

  // put the operand (F64) into scratch register and start to
  if (opr0->IsRegister()) {
    __ movq(SCR_FP, FPRegOperandToASMJIT(opr0));
  } else {
    __ movq(SCR_FP, MemoryOperandToASMJIT(opr0));
  }

  // perform the call, notes the ToASMJITMem will make the call target as
  // address we need to perform one extra round of modification, ie put the
  // address into rax and then do the call
  // __ lea(SCR_GPQ, cg->code()->cast_f64_to_b32().ToASMJITMem());
  __ call(cg->code()->cast_f64_to_b32().label());

  // the result is been put into SCR_GPD
  if (out->IsRegister()) {
    auto out_reg = GPRegOperandToASMJIT(out);
    __ mov(out_reg, SCR_GPD);
  } else {
    auto out_mem = MemoryOperandToASMJIT(out);
    __ mov(out_mem, SCR_GPD);
  }

  __ mov(a::x86::rbp, temp_slot);  // recover rbp
}

void TranslateRuleTable::LowerCastBToStr(CodeGen* cg, const HInstruction* ins) {
  auto cond = ins->Opr(0);
  auto out = ins->output();

  a::x86::Gp outr{SCR_GPQ};
  if (out->IsRegister()) {
    outr = GPRegOperandToASMJIT(out);
  }

  if (cond->IsRegister()) {
    auto r = GPRegOperandToASMJIT(cond);
    __ test(r, r);
  } else {
    __ mov(SCR_GPD, MemoryOperandToASMJIT(cond));
    __ test(SCR_GPD, SCR_GPD);
  }

  __ mov(outr, cg->code()->const_str_false());
  __ cmove(outr, cg->code()->const_str_true());

  // Bump the reference counting, since it is a heap allocated stuff.
  //
  // FIXME(dpeng): We have one option here to optimize all the const string's RC
  // away, we need another type in our backend and ignore all the RC, just treat
  // it like a I64. We can use I64 type as such use case, thus all the RC are
  // gone.
  //
  // The string is constant and will be held by the Quake, and it will never be
  // freeed.
  auto refcount_addr = a::x86::dword_ptr(outr, Layout::Value::kValue);
  __ inc(refcount_addr);

  // lastly, generate the output
  if (!out->IsRegister()) {
    __ mov(MemoryOperandToASMJIT(out), outr);
  }
}

std::uint64_t TranslateRuleTable::GetRCallAddress(const HInstruction* ins) {
  auto call_address = ins->Opr(0);
  CHECK(call_address->IsMemStatic());
  return reinterpret_cast<std::uint64_t>(
      call_address->GetMemStatic().address());
}

void TranslateRuleTable::LowerRCallGP(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto call_mem = GetRCallAddress(ins);

  // notes rdi is always reserved
  __ mov(a::x86::rdi, CTX);
  __ call(call_mem);

  // okay, get the return value from rax and put it back to the output
  if (out->size_class() == SizeClass::kQWord) {
    if (out->IsRegister()) {
      __ mov(GPRegOperandToASMJIT(out), a::x86::rax);
    } else {
      __ mov(MemoryOperandToASMJIT(out), a::x86::rax);
    }
  } else {
    CHECK(out->size_class() == SizeClass::kDWord);
    if (out->IsRegister()) {
      __ mov(GPRegOperandToASMJIT(out), a::x86::eax);
    } else {
      __ mov(MemoryOperandToASMJIT(out), a::x86::eax);
    }
  }
}

void TranslateRuleTable::LowerRCallFP(CodeGen* cg, const HInstruction* ins) {
  auto call_mem = GetRCallAddress(ins);
  CHECK(ins->output()->IsRegister());
  CHECK(ins->output()->GetRegister() ==
        Register::GetRegisterFromKind(RegisterKind::kXmm0));

  // notes rdi is always reserved
  __ mov(a::x86::rdi, CTX);
  __ call(call_mem);
}

void TranslateRuleTable::LowerRCallVoid(CodeGen* cg, const HInstruction* ins) {
  auto call_mem = GetRCallAddress(ins);

  // notes rdi is always reserved
  __ mov(a::x86::rdi, CTX);
  __ call(call_mem);
}

// rax:rdx forms the JSValue, the rax mapps to lower address and rdx mapps to
// higher address.
void TranslateRuleTable::LowerRCallBox(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto call_mem = GetRCallAddress(ins);

  __ mov(a::x86::rdi, CTX);
  __ call(call_mem);

  auto lower = __ CellAddressLower(ins->cell_index());
  auto higher = __ CellAddressHigher(ins->cell_index());
  auto cell = __ CellAddress(ins->cell_index());

  __ mov(lower, a::x86::rax);
  __ mov(higher, a::x86::rdx);

  if (out->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(out), cell);
  } else {
    __ lea(SCR_GPQ, cell);
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  }
}

// ============================================================================
// JSCall lowering

void TranslateRuleTable::PrepareScriptCallFunctionObject(CodeGen* cg,
                                                         const HOperand* func) {
  auto addr = __ ScriptCallFunctionObjectAddress();

  if (func->IsRegister()) {
    __ mov(addr, GPRegOperandToASMJIT(func));
  } else {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(func));
    __ mov(addr, SCR_GPQ);
  }
}

void TranslateRuleTable::PrepareScriptCallThisObject(CodeGen* cg,
                                                     const HOperand* tobj) {
  auto addr = __ ScriptCallThisObjectAddress();
  if (tobj->IsRegister()) {
    __ mov(addr, GPRegOperandToASMJIT(tobj));
  } else {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(tobj));
    __ mov(addr, SCR_GPQ);
  }
}

void TranslateRuleTable::PrepareScriptCallThisObjectUndefined(CodeGen* cg) {
  auto addr = __ ScriptCallThisObjectAddress();
  __ mov(addr, 0);
}

void TranslateRuleTable::PrepareScriptCallTargetObject(CodeGen* cg,
                                                       const HOperand* tobj) {
  auto addr = __ ScriptCallTargetObjectAddress();
  if (tobj->IsRegister()) {
    __ mov(addr, GPRegOperandToASMJIT(tobj));
  } else {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(tobj));
    __ mov(addr, SCR_GPQ);
  }
}

void TranslateRuleTable::PrepareScriptCallTargetObjectUndefined(CodeGen* cg) {
  auto addr = __ ScriptCallTargetObjectAddress();
  __ mov(addr, 0);
}

void TranslateRuleTable::PrepareScriptCallReturnAddress(
    CodeGen* cg, const HInstruction* ins) {
  auto cell_address = __ CellAddress(ins->cell_index());
  auto addr = __ ScriptCallReturnAddressAddress();
  __ lea(SCR_GPQ, cell_address);
  __ mov(addr, SCR_GPQ);
}

void TranslateRuleTable::PrepareScriptCallParameterStart(CodeGen* cg,
                                                         const HOperand* opr) {
  auto addr = __ ScriptCallParameterStartAddress();

  if (opr->IsRegister()) {
    __ mov(addr, GPRegOperandToASMJIT(opr));
  } else {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(opr));
    __ mov(addr, SCR_GPQ);
  }
}

void TranslateRuleTable::PrepareScriptCallParameterSize(CodeGen* cg,
                                                        const HOperand* size) {
  CHECK(size->IsImm());
  auto value = size->GetImm().Int32();
  CHECK(value >= 0);

  auto addr = __ ScriptCallParameterSizeAddress();
  __ mov(addr, value);
}

void TranslateRuleTable::PrepareScriptCallOption(CodeGen* cg,
                                                 const HOperand* opt) {
  CHECK(opt->IsImm());
  auto value = opt->GetImm().Int32();

  auto addr = __ ScriptCallOptionAddress();
  __ mov(addr, value);
}

void TranslateRuleTable::MaterializeScriptCallOutput(CodeGen* cg,
                                                     const HInstruction* call) {
  auto output = call->output();
  auto addr = __ CellAddress(call->cell_index());

  if (output->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(output), addr);
  } else {
    __ lea(SCR_GPQ, addr);
    __ mov(MemoryOperandToASMJIT(output), SCR_GPQ);
  }
}

void TranslateRuleTable::PerformScriptCall(CodeGen* cg) {
  auto addr = __ ScriptCallAddress();
  __ mov(asmjit::x86::rdi, CTX);
  __ lea(asmjit::x86::rsi, addr);
  __ call(FADDR(&QuakeJit::JSCall));
}

void TranslateRuleTable::PerformScriptCallCtor(CodeGen* cg) {
  auto addr = __ ScriptCallAddress();
  __ mov(asmjit::x86::rdi, CTX);
  __ lea(asmjit::x86::rsi, addr);
  __ call(FADDR(&QuakeJit::JSCallCtor));
}

void TranslateRuleTable::PerformScriptCallArrayFrom(CodeGen* cg) {
  auto addr = __ ScriptCallAddress();
  __ mov(asmjit::x86::rdi, CTX);
  __ lea(asmjit::x86::rsi, addr);
  __ call(FADDR(&QuakeJit::JSCallArrayFrom));
}

void TranslateRuleTable::PerformScriptCallApply(CodeGen* cg) {
  auto addr = __ ScriptCallAddress();
  __ mov(asmjit::x86::rdi, CTX);
  __ lea(asmjit::x86::rsi, addr);
  __ call(FADDR(&QuakeJit::JSCallApply));
}

// JSCallF input parameter
//
// 1) function object in BoxRef format
// 2) immediate number, parameter size of function
// 3) parameter starting address, parameter is passed via VM stack
void TranslateRuleTable::LowerJSCallF(CodeGen* cg, const HInstruction* ins) {
  // 0) Prepare script call parameter marshalling
  PrepareScriptCallFunctionObject(cg, ins->Opr(0));
  PrepareScriptCallThisObjectUndefined(cg);
  PrepareScriptCallTargetObjectUndefined(cg);
  PrepareScriptCallReturnAddress(cg, ins);

  PrepareScriptCallParameterSize(cg, ins->Opr(1));
  PrepareScriptCallParameterStart(cg, ins->Opr(2));

  // 1) Perform the script call, the result ins put into the return_address
  //    of the ScriptCall object
  PerformScriptCall(cg);

  // 2) Move the ScriptCall object's result back to the instruction's output
  MaterializeScriptCallOutput(cg, ins);
}

// JSCallFT input parameter
//
// 1) function object in BoxRef format
// 2) this object in BoxRef format
// 3) immediate number, parameter size of function
// 4) parameter starting address, parameter is passed via VM stack
void TranslateRuleTable::LowerJSCallFT(CodeGen* cg, const HInstruction* ins) {
  // 0) Prepare script call parameter marshalling
  PrepareScriptCallFunctionObject(cg, ins->Opr(0));
  PrepareScriptCallThisObject(cg, ins->Opr(1));
  PrepareScriptCallTargetObjectUndefined(cg);
  PrepareScriptCallReturnAddress(cg, ins);

  PrepareScriptCallParameterSize(cg, ins->Opr(2));
  PrepareScriptCallParameterStart(cg, ins->Opr(3));

  // 1) Perform the script call, the result ins put into the return_address
  //    of the ScriptCall object
  PerformScriptCall(cg);

  // 2) Move the ScriptCall object's result back to the instruction's output
  MaterializeScriptCallOutput(cg, ins);
}

// JSCallFN input parameter
//
// 1) function object in BoxRef format
// 2) new target in BoxRef format
// 3) immediate number, parameter size of function
// 4) parameter starting address, parameter is passed via VM stack
void TranslateRuleTable::LowerJSCallFN(CodeGen* cg, const HInstruction* ins) {
  // 0) Prepare script call parameter marshalling
  PrepareScriptCallFunctionObject(cg, ins->Opr(0));
  PrepareScriptCallThisObjectUndefined(cg);
  PrepareScriptCallTargetObject(cg, ins->Opr(1));
  PrepareScriptCallReturnAddress(cg, ins);

  PrepareScriptCallParameterSize(cg, ins->Opr(2));
  PrepareScriptCallParameterStart(cg, ins->Opr(3));

  // 1) Perform the script call, the result ins put into the return_address
  //    of the ScriptCall object
  PerformScriptCall(cg);

  // 2) Move the ScriptCall object's result back to the instruction's output
  MaterializeScriptCallOutput(cg, ins);
}

// JSCallFN input parameter
//
// 1) function object in BoxRef format
// 2) this object in BoxRef format
// 3) new target in BoxRef format
// 4) immediate number, parameter size of function
// 5) parameter starting address, parameter is passed via VM stack
void TranslateRuleTable::LowerJSCallFTN(CodeGen* cg, const HInstruction* ins) {
  // 0) Prepare script call parameter marshalling
  PrepareScriptCallFunctionObject(cg, ins->Opr(0));
  PrepareScriptCallThisObject(cg, ins->Opr(1));
  PrepareScriptCallTargetObject(cg, ins->Opr(2));

  PrepareScriptCallReturnAddress(cg, ins);

  PrepareScriptCallParameterSize(cg, ins->Opr(3));
  PrepareScriptCallParameterStart(cg, ins->Opr(4));

  // 1) Perform the script call, the result ins put into the return_address
  //    of the ScriptCall object
  PerformScriptCall(cg);

  // 2) Move the ScriptCall object's result back to the instruction's output
  MaterializeScriptCallOutput(cg, ins);
}

void TranslateRuleTable::LowerJSCallCtor(CodeGen* cg, const HInstruction* ins) {
  // 0) Prepare script call parameter marshalling
  PrepareScriptCallFunctionObject(cg, ins->Opr(0));
  PrepareScriptCallTargetObject(cg, ins->Opr(1));

  PrepareScriptCallReturnAddress(cg, ins);
  PrepareScriptCallParameterSize(cg, ins->Opr(2));
  PrepareScriptCallParameterStart(cg, ins->Opr(3));

  // 1) Perform the script call, the result ins put into the return_address
  //    of the ScriptCall object
  PerformScriptCallCtor(cg);

  // 2) Move the ScriptCall object's result back to the instruction's output
  MaterializeScriptCallOutput(cg, ins);
}

void TranslateRuleTable::LowerArrayFrom(CodeGen* cg, const HInstruction* ins) {
  PrepareScriptCallReturnAddress(cg, ins);
  PrepareScriptCallParameterSize(cg, ins->Opr(0));
  PrepareScriptCallParameterStart(cg, ins->Opr(1));

  PerformScriptCallArrayFrom(cg);
  MaterializeScriptCallOutput(cg, ins);
}

void TranslateRuleTable::LowerApply(CodeGen* cg, const HInstruction* ins) {
  PrepareScriptCallReturnAddress(cg, ins);

  PrepareScriptCallThisObject(cg, ins->Opr(0));
  PrepareScriptCallParameterStart(cg, ins->Opr(1));
  PrepareScriptCallOption(cg, ins->Opr(2));

  PerformScriptCallApply(cg);
  MaterializeScriptCallOutput(cg, ins);
}

void TranslateRuleTable::LowerRc(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto is_inc = ins->opcode() == HOpcode::kRcInc;

  a::x86::Mem refcount_addr;
  a::x86::Gp base;

  if (opr0->IsRegister()) {
    base = GPRegOperandToASMJIT(opr0);
  } else {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(opr0));
    base = SCR_GPQ;
  }

  refcount_addr = a::x86::dword_ptr(base, Layout::Value::kValue);

  if (is_inc) {
    __ add(refcount_addr, 1);  // increase rc
  } else {
    __ sub(refcount_addr, 1);  // decrease rc
  }
}

void TranslateRuleTable::DoGetTTagPtr64(CodeGen* cg, const HOperand* target) {
  if (target->IsMemory()) {
    auto base = MemoryOperandToASMJIT(target);
    __ mov(SCR_GPQ, base);

    auto addr = a::x86::qword_ptr(SCR_GPQ, Layout::Value::kTag);
    __ mov(SCR_GPQ, addr);
  } else {
    auto base = GPRegOperandToASMJIT(target);
    auto ptr = a::x86::qword_ptr(base, Layout::Value::kTag);
    __ mov(SCR_GPQ, ptr);
  }
}

void TranslateRuleTable::DoGetTValPtr64(CodeGen* cg, const HOperand* target) {
  if (target->IsMemory()) {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(target));
    auto addr = a::x86::qword_ptr(SCR_GPQ, Layout::Value::kValue);
    __ mov(SCR_GPQ, addr);
  } else {
    auto base = GPRegOperandToASMJIT(target);
    auto ptr = a::x86::qword_ptr(base, Layout::Value::kValue);
    __ mov(SCR_GPQ, ptr);
  }
}

void TranslateRuleTable::DoGetTTag(CodeGen* cg, const HOperand* opr) {
  DoGetTTagPtr64(cg, opr);
}

void TranslateRuleTable::DoGetTVal(CodeGen* cg, const HOperand* opr) {
  DoGetTValPtr64(cg, opr);
}

void TranslateRuleTable::LowerGetTTag(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kGetTTag);
  CHECK(ins->IsUnary());
  CHECK(ins->HasOutput());

  auto opr1 = ins->Opr(0);
  auto output = ins->output();

  DoGetTTag(cg, opr1);  // eax contains the tag

  if (output->IsMemory()) {
    __ mov(MemoryOperandToASMJIT(output), a::x86::eax);
  } else if (output->IsRegister()) {
    __ mov(GPRegOperandToASMJIT(output), a::x86::eax);
  } else {
    UNREACHABLE();
  }
}

int TranslateRuleTable::GetTTagFromOperand(const HOperand* opr) {
  CHECK(opr->IsImm());
  const auto& imm = opr->GetImm();
  CHECK(imm.IsInt32());
  return imm.Int32();
}

void TranslateRuleTable::LowerIsNullOrUndefined(CodeGen* cg,
                                                const HInstruction* ins) {
  static const int kNullTag = JS_TAG_NULL;
  static const int kUndefinedTag = JS_TAG_UNDEFINED;

  auto opr = ins->Opr(0);
  auto out = ins->output();

  if (opr->IsRegister()) {
    __ mov(SCR_GPD, GPRegOperandToASMJIT(opr));
  } else {
    __ mov(SCR_GPD, MemoryOperandToASMJIT(opr));
  }

  if (kNullTag == 2 && kUndefinedTag == 3) {
    // clang's optimizing version
    if (out->IsRegister()) {
      auto out_reg = GPRegOperandToASMJIT(out);

      __ and_(SCR_GPD, a::Imm(-2));
      __ xor_(out_reg, out_reg);
      __ cmp(SCR_GPD, 2);
      __ sete(out_reg.r8Lo());
    } else {
      auto out_mem = MemoryOperandToASMJIT(out);

      __ and_(SCR_GPD, a::Imm(-2));
      __ mov(out_mem, 0);
      __ cmp(SCR_GPD, 2);

      // notes out_mem should be in byte size of 4, we need to convert it into
      // byte
      out_mem.setSize(1);
      __ sete(out_mem);
    }
  } else {
    auto pos = __ newLabel();
    auto done = __ newLabel();

    // basic version, unoptimized, have branch etc ...
    __ cmp(SCR_GPD, a::Imm(kNullTag));
    __ je(pos);

    __ cmp(SCR_GPD, a::Imm(kUndefinedTag));
    __ je(pos);

    if (out->IsRegister()) {
      auto out_reg = GPRegOperandToASMJIT(out);
      __ xor_(out_reg, out_reg);
    } else {
      auto out_mem = MemoryOperandToASMJIT(out);
      __ mov(out_mem, 0);
    }

    __ jmp(done);

    // positive branch
    __ bind(pos);
    {
      if (out->IsRegister()) {
        auto out_reg = GPRegOperandToASMJIT(out);
        __ mov(out_reg, 1);
      } else {
        auto out_mem = MemoryOperandToASMJIT(out);
        __ mov(out_mem, 1);
      }
    }

    // done branch
    __ bind(done);
  }
}

void TranslateRuleTable::LowerBoxPtr64(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto val = ins->Opr(0);
  auto ty = ins->Opr(1);
  CHECK(ty->IsImm());
  auto type_tag = ty->GetImm().Int32();

  auto cell_index = ins->cell_index();

  // Store the value into the cell slot
  if (val->IsRegister()) {
    if (auto r = val->GetRegister(); r->IsFP()) {
      __ store_cell(cell_index, FPRegOperandToASMJIT(val), type_tag);
    } else {
      __ store_cell(cell_index, GPRegOperandToASMJIT(val), type_tag);
    }
  } else {
    CHECK(val->IsMemory());
    auto out = MoveMemoryToScratch(cg, val);
    __ store_cell(cell_index, out, type_tag);
  }

  // Return the cell slot address into our output position
  auto cell_address = __ CellAddress(cell_index);
  if (out->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(out), cell_address);
  } else {
    __ lea(SCR_GPQ, cell_address);
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  }
}

void TranslateRuleTable::LowerUnboxPtrPtr64(CodeGen* cg,
                                            const HInstruction* ins) {
  auto out = ins->output();
  DoGetTVal(cg, ins->Opr(0));

  if (out->IsRegister()) {
    __ mov(GPRegOperandToASMJIT(out), SCR_GPQ);
  } else {
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  }
}

void TranslateRuleTable::LowerGUnboxPtrPtr64(CodeGen* cg,
                                             const HInstruction* ins) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);

  DoGetTTag(cg, op0);

  auto done = __ newLabel();

  // NOTES(dpeng): all the quickjs's heap type is less than 0
  __ cmp(SCR_GPD, 0);
  __ jl(done);

  Deoptimize(cg, ins);

  __ bind(done);

  // load the value into scratch register
  DoGetTVal(cg, op0);

  if (out->IsMemory()) {
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  } else {
    __ mov(GPRegOperandToASMJIT(out), SCR_GPQ);
  }
}

void TranslateRuleTable::LowerGUnbox32BitPtr64(CodeGen* cg,
                                               const HInstruction* ins,
                                               int jval_type) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);

  DoGetTTag(cg, op0);

  auto done = __ newLabel();

  __ cmp(SCR_GPD, a::Imm(jval_type));
  __ je(done);

  Deoptimize(cg, ins);

  __ bind(done);
  DoGetTVal(cg, op0);

  if (out->IsMemory()) {
    __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
  } else {
    __ mov(GPRegOperandToASMJIT(out), SCR_GPD);
  }
}

void TranslateRuleTable::LowerGUnboxIPtr64(CodeGen* cg,
                                           const HInstruction* ins) {
  LowerGUnbox32BitPtr64(cg, ins, JVal::kInt);
}

void TranslateRuleTable::LowerGUnboxBPtr64(CodeGen* cg,
                                           const HInstruction* ins) {
  LowerGUnbox32BitPtr64(cg, ins, JVal::kBool);
}

void TranslateRuleTable::LowerGUnboxFPtr64(CodeGen* cg,
                                           const HInstruction* ins) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);

  DoGetTTag(cg, op0);

  auto done = __ newLabel();

  __ cmp(SCR_GPD, a::Imm(JVal::kFloat64));
  __ je(done);

  Deoptimize(cg, ins);

  __ bind(done);
  DoGetTVal(cg, op0);

  if (out->IsMemory()) {
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  } else {
    __ movq(FPRegOperandToASMJIT(out), SCR_GPQ);
  }
}

void TranslateRuleTable::LowerUnbox32BitPtr64(CodeGen* cg,
                                              const HInstruction* ins) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);
  DoGetTVal(cg, op0);
  if (out->IsMemory()) {
    __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
  } else {
    __ mov(GPRegOperandToASMJIT(out), SCR_GPD);
  }
}

void TranslateRuleTable::LowerUnboxIPtr64(CodeGen* cg,
                                          const HInstruction* ins) {
  LowerUnbox32BitPtr64(cg, ins);
}

void TranslateRuleTable::LowerUnboxBPtr64(CodeGen* cg,
                                          const HInstruction* ins) {
  LowerUnbox32BitPtr64(cg, ins);
}

void TranslateRuleTable::LowerUnboxFPtr64(CodeGen* cg,
                                          const HInstruction* ins) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);

  DoGetTVal(cg, op0);

  if (out->IsMemory()) {
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  } else {
    __ movq(FPRegOperandToASMJIT(out), SCR_GPQ);
  }
}

// ----------------------------------------------------------------------------
// Atom
void TranslateRuleTable::LowerIsBoxAtomInt(CodeGen* cg,
                                           const HInstruction* ins) {
  auto opr = ins->Opr(0);
  auto out = ins->output();

  auto invalid = __ newLabel();
  auto done = __ newLabel();

  // 0) Get its type tag
  DoGetTTag(cg, opr);

  // 1) Check type tag is Int or not
  __ cmp(SCR_GPD, JVal::kInt);
  __ jne(invalid);

  // 2) Check wether the int value is small enough
  DoGetTVal(cg, opr);

  __ cmp(SCR_GPD, JS_ATOM_MAX_INT);
  if (out->IsRegister()) {
    auto r = GPRegOperandToASMJIT(out);
    __ setl(r.r8());
  } else {
    __ setl(SCR_GPB);
    __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
  }

  __ jmp(done);

  // 3) invalid case, ie output is false(0)
  __ bind(invalid);

  if (out->IsRegister()) {
    auto r = GPRegOperandToASMJIT(out);
    __ xor_(r, r);
  } else {
    __ mov(MemoryOperandToASMJIT(out), 0);
  }

  __ bind(done);
}

void TranslateRuleTable::LowerAtomFromI(CodeGen* cg, const HInstruction* ins) {
  auto opr = ins->Opr(0);
  auto out = ins->output();

  switch (DecideMMType(out, opr)) {
    case MMType::kRr:
      __ or_(GPRegOperandToASMJIT(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kRR:
      __ mov(GPRegOperandToASMJIT(out), GPRegOperandToASMJIT(opr));
      __ or_(GPRegOperandToASMJIT(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kRM:
      __ mov(GPRegOperandToASMJIT(out), MemoryOperandToASMJIT(opr));
      __ or_(GPRegOperandToASMJIT(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kMR:
      __ mov(MemoryOperandToASMJIT(out), GPRegOperandToASMJIT(opr));
      __ or_(MemoryOperandToASMJIT(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kMM:
      __ mov(SCR_GPD, MemoryOperandToASMJIT(opr));
      __ mov(MemoryOperandToASMJIT(out), SCR_GPD);
      __ or_(MemoryOperandToASMJIT(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kRI:
      __ mov(GPRegOperandToASMJIT(out), ImmOperandToASMJIT(opr));
      __ or_(GPRegOperandToASMJIT(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kMI:
      __ mov(MemoryOperandToASMJIT(out), ImmOperandToASMJIT(opr));
      __ or_(MemoryOperandToASMJIT(out), JS_ATOM_TAG_INT);
      break;

    default:
      UNREACHABLE();
      break;
  }
}

// If atom value is JS_ATOM_NULL, then deoptimize, otherwise do nothing
void TranslateRuleTable::LowerGAtom(CodeGen* cg, const HInstruction* ins) {
  auto opr = ins->Opr(0);
  auto out = ins->output();

  auto done = __ newLabel();

  // check whether it is JS_ATOM_NULL, otherwise deoptimize
  if (opr->IsRegister()) {
    __ cmp(GPRegOperandToASMJIT(opr), JS_ATOM_NULL);
  } else {
    __ mov(SCR_GPD, MemoryOperandToASMJIT(opr));
    __ cmp(SCR_GPD, JS_ATOM_NULL);
  }
  __ jnz(done);

  Deoptimize(cg, ins);

  __ bind(done);

  Mov(cg, out, opr, SCR_GPQ);
}

// ----------------------------------------------------------------------------
// Object
//
// This opcode also reserves the rdx register, which we can use it as our
// scratch register for temporary usage.
//
// The basic idea for the fast path is we check whether object's shape's 2 field
// is the same with our assumption, ie hash_mask and prop_size. If so it means
// we can directly address into property array without needing to worry about
// memory invalid, and since JSAtom is dedup, we can just compare the atom value
// there to learn whether the target is allowed or not. Notes, if entry is
// invalid, deleted or so, the atom is JSAtomNull which will guard us.
//
// This will require way smaller amount of inline code for fast path, though
// it is highly specialized, which is not a bad thing for us until we get our
// inline cache system to cache more cases, ie polymorphic.
void TranslateRuleTable::LowerObjectFieldGetAtomFast(CodeGen* cg,
                                                     const HInstruction* ins) {
  static_assert(sizeof(::JSShapeProperty) == 8);
  static_assert(sizeof(::JSProperty) == 16);

  auto output = ins->output();
  auto js_object = ins->Opr(0);
  auto atom = ins->Opr(1)->GetImm().UInt32();  // atom index

  auto imm = __ StackTemp0(4);    // pass immediate number
  auto cidx = __ StackTemp1(4);   // pass cell index
  auto bflag = __ StackTemp7(4);  // pass the bailout flag, used by slow path

  // stack0 contains atom value
  __ mov(imm, atom);

  // stack1 contains the cell index
  __ mov(cidx, ins->cell_index());

  // stack7 contains the bailout type
  __ mov(bflag, JVal::kNotAvailable);

  // scratch points to the pointer of input
  if (js_object->IsRegister()) {
    auto r = GPRegOperandToASMJIT(js_object);
    __ mov(SCR_GPQ, r);
  } else {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(js_object));
  }

  // do the fast call
  __ call(cg->code()->object_field_get_atom_fast().label());

  auto cell_addr = __ CellAddress(ins->cell_index());

  // generate output
  if (output->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(output), cell_addr);
  } else {
    __ lea(SCR_GPQ, cell_addr);
    __ mov(MemoryOperandToASMJIT(output), SCR_GPQ);
  }
}

// The JS_GetGlobalVar function has 2 path can be specialized, in a sensitive
// order,
//
// 1) lookup js_global_var's property table, if this lookup are done then return
// 2) lookup js_global's property table, if this lookup are done, then return
//
// For now, we just alias it to LowerObjectFieldGetAtomFast call, since we do
// not have fully inlined find_own_property implementation yet. Once we have
// we can perform a fule inline one global_var_obj for lookup, and then fall
// back to a partial find on global_obj and then lastly fallback to slowest
// external runtime call
void TranslateRuleTable::LowerGlobalGetVarAtomFast(CodeGen* cg,
                                                   const HInstruction* ins) {
  auto output = ins->output();
  auto atom = ins->Opr(0)->GetImm().UInt32();  // atom index

  auto imm = __ StackTemp0(4);   // pass immediate number
  auto cidx = __ StackTemp1(4);  // pass cell index
  auto bflag = __ StackTemp7(4);

  // stack0 contains atom value
  __ mov(imm, atom);

  // stack1 contains the cell index
  __ mov(cidx, ins->cell_index());

  // bailout value, notes for global variable access, uninitialized will lead to
  // slow path, which will actually throw an exception afterwards
  __ mov(bflag, JVal::kUninitialized);

  // populate the global variable table, ie the global_var_obj field
  __ mov(SCR_GPQ, a::x86::qword_ptr(CTX, Layout::Context::kContext));

  auto js_obj_addr = a::x86::qword_ptr(
      SCR_GPQ, Layout::JSContext::kGlobalVarObject + Layout::Value::kValue);

  __ mov(SCR_GPQ, js_obj_addr);  // object pointer

  __ call(cg->code()->object_field_get_atom_fast().label());

  // generate output
  auto cell_addr = __ CellAddress(ins->cell_index());

  if (output->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(output), cell_addr);
  } else {
    __ lea(SCR_GPQ, cell_addr);
    __ mov(MemoryOperandToASMJIT(output), SCR_GPQ);
  }
}

void TranslateRuleTable::LowerGlobalGetGlobalAtomFast(CodeGen* cg,
                                                      const HInstruction* ins) {
  auto output = ins->output();
  auto atom = ins->Opr(0)->GetImm().UInt32();  // atom index

  auto imm = __ StackTemp0(4);   // pass immediate number
  auto cidx = __ StackTemp1(4);  // pass cell index
  auto bflag = __ StackTemp7(4);

  // stack0 contains atom value
  __ mov(imm, atom);

  // stack1 contains the cell index
  __ mov(cidx, ins->cell_index());

  // bailout value, notes for global variable access, uninitialized will lead to
  // slow path, which will actually throw an exception afterwards
  __ mov(bflag, JVal::kUninitialized);

  // populate the global variable table, ie the global_var_obj field
  __ mov(SCR_GPQ, a::x86::qword_ptr(CTX, Layout::Context::kContext));

  auto js_obj_addr = a::x86::qword_ptr(
      SCR_GPQ, Layout::JSContext::kGlobalVarObject + Layout::Value::kValue);

  __ mov(SCR_GPQ, js_obj_addr);  // object pointer

  __ call(cg->code()->global_get_global_atom_fast().label());

  // generate output
  auto cell_addr = __ CellAddress(ins->cell_index());

  if (output->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(output), cell_addr);
  } else {
    __ lea(SCR_GPQ, cell_addr);
    __ mov(MemoryOperandToASMJIT(output), SCR_GPQ);
  }
}

// ----------------------------------------------------------------------------
// Array
//
// Scratch and rdx will be reserved

void TranslateRuleTable::DoLowerLoadArray(
    CodeGen* cg, const HInstruction* ins,
    int class_id,  // qjs class id, used to testify shape validity
    std::size_t byte_size, ArrayNativeType native_type) {
  auto out = ins->output();
  auto obj = ins->Opr(0);
  auto idx = ins->Opr(1);

  auto fallback = __ newLabel();
  auto done = __ newLabel();
  auto temp = __ StackTemp4();

  auto lower_bound_check_do_not_need = false;

  if (obj->IsRegister()) {
    __ mov(SCR_GPQ, GPRegOperandToASMJIT(obj));
  } else {
    __ mov(SCR_GPQ, MemoryOperandToASMJIT(obj));
  }

  // 0) check whether object has expected shape
  __ cmp(a::x86::word_ptr(SCR_GPQ, Layout::Object::kClassId), class_id);
  __ jne(fallback);

  // 1) figure out the length
  if (idx->IsRegister()) {
    auto r = GPRegOperandToASMJIT(idx);
    __ movsx(a::x86::rdx, r);
  } else if (idx->IsImm()) {
    auto value = idx->GetImm().Int32();
    CHECK(value >= 0);
    __ mov(a::x86::edx, value);
    __ movsx(a::x86::rdx, a::x86::edx);  // redundant ?
    lower_bound_check_do_not_need = true;
  } else {
    auto m = MemoryOperandToASMJIT(idx);
    __ movsx(a::x86::rdx, m);
  }

  // 2) check the index is in the valid range, ie >= 9 and < array_count
  if (!lower_bound_check_do_not_need) {
    __ cmp(a::x86::rdx, 0);
    __ jl(fallback);
  }

  __ cmp(a::x86::edx,
         a::x86::dword_ptr(SCR_GPQ, Layout::Object::kU_Array_Count));
  __ jge(fallback);

  // 3) get the inline native array's element address
  //
  // make scratch points to the address of u
  __ mov(SCR_GPQ, a::x86::qword_ptr(SCR_GPQ, Layout::Object::kU_Array_U));

  a::x86::Mem value_addr;
  if (byte_size <= 8) {
    // okay, just use the basic addressing mode
    value_addr = a::x86::ptr(SCR_GPQ, a::x86::rdx, byte_size);
  } else {
    CHECK(byte_size == 16);

    __ sal(a::x86::rdx, 4);  // multiply 16
    value_addr = a::x86::ptr(SCR_GPQ, a::x86::rdx, 0);
  }
  value_addr.setSize(byte_size);

  // 4) box the value loaded from the native array directly to the cell
  {
    auto cell_tag_addr = __ CellAddressTag(ins->cell_index());
    auto cell_val_addr = __ CellAddressValue(ins->cell_index());
    auto cell_addr = __ CellAddress(ins->cell_index());

    switch (native_type) {
      case ArrayNativeType::kInt8:
      case ArrayNativeType::kInt16:
        __ movsx(SCR_GPD, value_addr);
        __ mov(cell_tag_addr, JVal::kInt);

        cell_val_addr.setSize(4);
        __ mov(cell_val_addr, SCR_GPD);
        break;

      case ArrayNativeType::kUInt8:
      case ArrayNativeType::kUInt16:
        __ movzx(SCR_GPD, value_addr);
        __ mov(cell_tag_addr, JVal::kInt);

        cell_val_addr.setSize(4);
        __ mov(cell_val_addr, SCR_GPD);
        break;

      case ArrayNativeType::kInt32:
      case ArrayNativeType::kUInt32:
        __ mov(SCR_GPD, value_addr);
        __ mov(cell_tag_addr, JVal::kInt);

        cell_val_addr.setSize(4);
        __ mov(cell_val_addr, SCR_GPD);
        break;

      case ArrayNativeType::kF64:
        __ mov(SCR_GPQ, value_addr);
        __ mov(cell_tag_addr, JVal::kFloat64);

        cell_val_addr.setSize(8);
        __ mov(cell_val_addr, SCR_GPQ);
        break;

      case ArrayNativeType::kF32:
        __ pxor(SCR_FP, SCR_FP);
        __ cvtss2sd(SCR_FP, value_addr);
        __ movsd(temp, SCR_FP);
        __ mov(cell_tag_addr, JVal::kFloat64);
        __ mov(SCR_GPQ, temp);

        cell_val_addr.setSize(8);
        __ mov(cell_val_addr, SCR_GPQ);
        break;

      case ArrayNativeType::kJSValue:
        __ movdqu(SCR_FP, value_addr);

        cell_addr.setSize(16);
        __ movups(cell_addr, SCR_FP);
        break;

      default:
        UNREACHABLE();
        break;
    }
  }
  __ jmp(done);  // fast path is done

  // 5) handling fallback case, ie to trigger slow path
  __ bind(fallback);
  {
    auto cell_tag_addr = __ CellAddressTag(ins->cell_index());
    auto cell_val_addr = __ CellAddressValue(ins->cell_index());

    __ mov(SCR_GPD, JVal::kNotAvailable);
    __ mov(cell_tag_addr, SCR_GPQ);

    __ xor_(SCR_GPQ, SCR_GPQ);
    __ mov(cell_val_addr, SCR_GPQ);
  }

  // 6) everything is done,
  __ bind(done);

  // 7) generate valid output
  if (out->IsRegister()) {
    __ lea(GPRegOperandToASMJIT(out), __ CellAddress(ins->cell_index()));
  } else {
    __ lea(SCR_GPQ, __ CellAddress(ins->cell_index()));
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  }
}

void TranslateRuleTable::LowerLoadArrayInt8(CodeGen* cg,
                                            const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_INT8_ARRAY, 1, ArrayNativeType::kInt8);
}

void TranslateRuleTable::LowerLoadArrayUInt8(CodeGen* cg,
                                             const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_UINT8_ARRAY, 1, ArrayNativeType::kUInt8);
}

void TranslateRuleTable::LowerLoadArrayUInt8C(CodeGen* cg,
                                              const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_UINT8C_ARRAY, 1, ArrayNativeType::kUInt8);
}

void TranslateRuleTable::LowerLoadArrayInt16(CodeGen* cg,
                                             const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_INT16_ARRAY, 2, ArrayNativeType::kInt16);
}

void TranslateRuleTable::LowerLoadArrayUInt16(CodeGen* cg,
                                              const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_UINT16_ARRAY, 2, ArrayNativeType::kUInt16);
}

void TranslateRuleTable::LowerLoadArrayInt32(CodeGen* cg,
                                             const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_INT32_ARRAY, 4, ArrayNativeType::kInt32);
}

void TranslateRuleTable::LowerLoadArrayUInt32(CodeGen* cg,
                                              const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_UINT32_ARRAY, 4, ArrayNativeType::kUInt32);
}

void TranslateRuleTable::LowerLoadArrayF32(CodeGen* cg,
                                           const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_FLOAT32_ARRAY, 4, ArrayNativeType::kF32);
}

void TranslateRuleTable::LowerLoadArrayF64(CodeGen* cg,
                                           const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_FLOAT64_ARRAY, 8, ArrayNativeType::kF64);
}

void TranslateRuleTable::LowerLoadArrayValue(CodeGen* cg,
                                             const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_ARRAY, 16, ArrayNativeType::kJSValue);
}

void TranslateRuleTable::LowerLoadArrayArguments(CodeGen* cg,
                                                 const HInstruction* ins) {
  DoLowerLoadArray(cg, ins, JS_CLASS_ARGUMENTS, 16, ArrayNativeType::kJSValue);
}

// ----------------------------------------------------------------------------
// Context
void TranslateRuleTable::LowerContextLoad(CodeGen* cg,
                                          const HInstruction* ins) {
  auto out = ins->output();
  if (out->IsRegister()) {
    __ mov(GPRegOperandToASMJIT(out), CTX);
  } else {
    __ mov(MemoryOperandToASMJIT(out), CTX);
  }
}

void TranslateRuleTable::LowerContextLoadGlobalObject(CodeGen* cg,
                                                      const HInstruction* ins) {
  auto out = ins->output();
  auto addr = a::x86::qword_ptr(
      CTX, Layout::JSContext::kGlobalObject + Layout::Value::kValue);

  // okay, we should bump its RC count
  __ mov(SCR_GPQ, addr);
  __ inc(SCR_GPQ);  // heap object's address space starts with the GC header

  if (out->IsRegister()) {
    __ mov(GPRegOperandToASMJIT(out), SCR_GPQ);
  } else {
    __ mov(MemoryOperandToASMJIT(out), SCR_GPQ);
  }
}

// ----------------------------------------------------------------------------
// Deoptimization

std::uint32_t TranslateRuleTable::GetDeoptId(const HInstruction* ins) {
  CHECK(ins->HasDeoptId());
  return static_cast<std::uint32_t>(ins->deopt_id());
}

void TranslateRuleTable::Deoptimize(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->HasDeoptId());
  __ mov(a::x86::eax, GetDeoptId(ins));
  __ jmp(cg->code()->deopt().label());
}

// Allow deopt id will be saved into RAX register
void TranslateRuleTable::LowerDeopt(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 0);
  Deoptimize(cg, ins);
}

void TranslateRuleTable::LowerDeoptIfTrueOrFalse(CodeGen* cg,
                                                 const HInstruction* ins) {
  auto is_true = ins->opcode() == HOpcode::kDeoptIfTrue ? true : false;

  auto cond = ins->Opr(0);
  if (cond->IsRegister()) {
    auto r = GPRegOperandToASMJIT(cond);
    __ test(r, r);
  } else {
    auto m = MemoryOperandToASMJIT(cond);
    __ mov(SCR_GPD, m);
    __ test(SCR_GPD, SCR_GPD);
  }

  auto done = __ newLabel();
  if (is_true) {
    __ jz(done);
  } else {
    __ jnz(done);
  }
  Deoptimize(cg, ins);
  __ bind(done);
}

void TranslateRuleTable::LowerDeoptCC(CodeGen* cg, const HInstruction* ins) {
  auto done = __ newLabel();
  switch (ins->opcode()) {
    case HOpcode::kDeoptZ:
      __ jnz(done);
      break;

    case HOpcode::kDeoptNz:
      __ jz(done);
      break;

    case HOpcode::kDeoptGt:
      __ jle(done);
      break;

    case HOpcode::kDeoptGe:
      __ jl(done);
      break;

    case HOpcode::kDeoptLt:
      __ jge(done);
      break;

    case HOpcode::kDeoptLe:
      __ jg(done);
      break;

    case HOpcode::kDeoptAt:
      __ jbe(done);
      break;

    case HOpcode::kDeoptAe:
      __ jb(done);
      break;

    case HOpcode::kDeoptBt:
      __ jae(done);
      break;

    case HOpcode::kDeoptBe:
      __ ja(done);
      break;

    case HOpcode::kDeoptEq:
      __ jne(done);
      break;

    case HOpcode::kDeoptNe:
      __ je(done);
      break;

    default:
      UNREACHABLE();
      break;
  }

  Deoptimize(cg, ins);
  __ bind(done);
}

// ----------------------------------------------------------------------------
// Translation rule building
template <typename... Args>
struct OperandList;

template <typename FuncType, FuncType func, typename OpndList>
struct AsmAction;

template <typename FuncType, FuncType func, typename... OpndTypes>
struct AsmAction<FuncType, func, OperandList<OpndTypes...>> {
  static void Lower(CodeGen* codegen, const HInstruction* instr) {
    (codegen->as()->*func)(OpndTypes::GetAsmOperand(codegen, instr)...);
  }
};

// Special Mov/Movsd operation for optimizing the lower. Ie it tries to skip
// the mov/movsd/movq when the source and destination is the same register
// or memory

enum class MoveAction : std::uint8_t { kMove, kMoveSD, kMoveQ };

template <MoveAction kType, int kLhs, int kRhs>
struct AsmMove {
  static bool IsSameOperand(const HOperand* lhs, const HOperand* rhs) {
    if (lhs->IsRegister() && rhs->IsRegister()) {
      return lhs->GetRegister() == rhs->GetRegister();
    }

    if (lhs->IsMemStack() && rhs->IsMemStack()) {
      return lhs->GetMemStack().slot() == rhs->GetMemStack().slot();
    }

    return false;
  }

  static void Move(CodeGen* cg, const HOperand* dest, const HOperand* source) {
    if (dest->IsRegister()) {
      auto dreg = GPRegOperandToASMJIT(dest);

      if (source->IsRegister()) {
        __ mov(dreg, GPRegOperandToASMJIT(source));
      } else if (source->IsImm()) {
        __ mov(dreg, ImmOperandToASMJIT(source));
      } else {
        CHECK(source->IsMemory());
        __ mov(dreg, MemoryOperandToASMJIT(source));
      }
    } else {
      CHECK(dest->IsMemory());
      auto dmem = MemoryOperandToASMJIT(dest);

      if (source->IsRegister()) {
        __ mov(dmem, GPRegOperandToASMJIT(source));
      } else if (source->IsImm()) {
        __ mov(dmem, ImmOperandToASMJIT(source));
      } else {
        UNREACHABLE() << "should not issue this";
      }
    }
  }

  static void MoveF(CodeGen* cg, const HOperand* dest, const HOperand* source,
                    const bool is_movq) {
    if (dest->IsRegister()) {
      auto dreg = FPRegOperandToASMJIT(dest);
      if (source->IsRegister()) {
        if (source->GetRegister()->IsFP()) {
          if (is_movq) {
            __ movq(dreg, FPRegOperandToASMJIT(source));
          } else {
            __ movsd(dreg, FPRegOperandToASMJIT(source));
          }
        } else {
          // movsd cannot move data from GP to SSE
          __ movq(dreg, GPRegOperandToASMJIT(source));
        }
      } else {
        CHECK(source->IsMemory());
        if (is_movq) {
          __ movq(dreg, MemoryOperandToASMJIT(source));
        } else {
          __ movsd(dreg, MemoryOperandToASMJIT(source));
        }
      }
    } else {
      CHECK(dest->IsMemory());
      auto dmem = MemoryOperandToASMJIT(dest);

      if (source->IsRegister()) {
        if (source->GetRegister()->IsFP()) {
          if (is_movq) {
            __ movq(dmem, FPRegOperandToASMJIT(source));
          } else {
            __ movsd(dmem, FPRegOperandToASMJIT(source));
          }
        } else {
          __ mov(dmem, GPRegOperandToASMJIT(source));
          UNREACHABLE()
              << "when source is GP register, the dest cannot be memory";
        }
      } else {
        UNREACHABLE() << "should not issue this";
      }
    }
  }

  static void Lower(CodeGen* cg, const HInstruction* ins) {
    CHECK(ins->size() >= 2);

    auto dest = ins->At(kLhs);
    auto source = ins->At(kRhs);

    if (IsSameOperand(dest, source)) return;

    switch (kType) {
      case MoveAction::kMove:
        Move(cg, dest, source);
        break;

      case MoveAction::kMoveSD:
      case MoveAction::kMoveQ: {
        const auto is_movq = kType == MoveAction::kMoveQ;
        MoveF(cg, dest, source, is_movq);
      } break;
      default:
        UNREACHABLE();
        break;
    };
  }
};  // namespace x64

template <typename... Args>
struct ASMJITInstructionType {
  using type = a::Error (a::x86::EmitterExplicitT<a::x86::Builder>::*)(
      typename Args::asmjit_type...);
};

template <void (*func)(CodeGen*, const HInstruction*)>
struct CallAction {
  static void Lower(CodeGen* env, const HInstruction* instr) {
    func(env, instr);
  }
};

// Assertion rules, used for certain condition checking during ASM rules
// building this is only used for debugging purpose
//
// Just use a policy based stuff to simplify it or just use verify
enum class AssertionType : std::uint8_t {
  kInput0AsOutput,
  kOutputRdx,  // imod
  kOutputReg,
  kOutputMemory,
  kOutputValid
};

template <AssertionType aType>
struct AssertionAction {
  static void Lower(CodeGen* env, const HInstruction* ins) {
    switch (aType) {
      case AssertionType::kInput0AsOutput:
        CHECK(ins->opr_size() >= 1);
        CHECK(ins->HasOutput());
        CHECK(ins->output() == ins->Opr(0));
        return;

      case AssertionType::kOutputRdx:
        CHECK(ins->HasOutput());
        CHECK(ins->output()->IsRegister());
        CHECK(ins->output()->GetRegister()->register_kind() ==
              RegisterKind::kRdx);
        return;

      case AssertionType::kOutputReg:
        CHECK(ins->HasOutput());
        CHECK(ins->output()->IsRegister());
        return;

      case AssertionType::kOutputMemory:
        CHECK(ins->HasOutput());
        CHECK(ins->output()->IsMemory());
        return;

      case AssertionType::kOutputValid:
        CHECK(ins->HasOutput());
        CHECK(ins->output()->IsRegister() || ins->output()->IsMemory());
        return;

      default:
        return;
    }
  }
};

template <typename... Actions>
struct RuleActions;

template <typename AAction, typename... Actions>
struct RuleActions<AAction, Actions...> {
  static void Lower(CodeGen* env, const HInstruction* instr) {
    AAction::Lower(env, instr);
    RuleActions<Actions...>::Lower(env, instr);
  }
};

template <>
struct RuleActions<> {
  static void Lower(CodeGen*, const HInstruction*) {}
};

// ============================================================================
//
// Mechanical Translation Rule Definition
//
// ============================================================================

#define REG(x) RegOperand<x>
#define XMM(x) XmmOperand<x>
#define IMM(i) ImmOperand<i>
#define IMM8(i) Imm8Operand<i>
#define CONST(i) ConstIntOperand<i>
#define CONSTU(i) ConstUintOperand<i>
#define MEM(m) MemOperand<m>
#define STK(v) MemOperand<v>
#define LBL(v) LabelOperand<v>

// ----------------------------------------------------------------------------
// Used by translation rules
#define EAX() EaxOperand
#define EDX() EdxOperand
#define RAX() RaxOperand
#define RDX() RdxOperand
#define SCRATCH(X) Scratch##X##Operand

#define ASM(instr, args...)                             \
  AsmAction<typename ASMJITInstructionType<args>::type, \
            &a::x86::Builder::instr, OperandList<args>>

// Specialized mov/movq/movsd instruction which will detect whether moving to
// the same operand and do if really needs to move. It is only useful for
// certain instruction lowering, ie for Add/Sub/Mul etc which x64 has a
// operand_and_output argument. User can still use normal mov if they are sure
// there're no benefits to use the following version
#define MOVE(LHS, RHS) AsmMove<MoveAction::kMove, LHS, RHS>
#define MOVEQ(LHS, RHS) AsmMove<MoveAction::kMoveQ, LHS, RHS>
#define MOVESD(LHS, RHS) AsmMove<MoveAction::kMoveSD, LHS, RHS>

#define CALL(func) CallAction<func>
#define ASSERT(T) AssertionAction<AssertionType::T>

#define BEGIN(X) \
  do {           \
    auto opcode = X;

#define END() \
  }           \
  while (false)
#define RULE(s, actions...)                                      \
  do {                                                           \
    constexpr char pattern[] = s;                                \
    UNUSE(pattern);                                              \
    using RType = RuleActions<actions>;                          \
    auto gen = [](CodeGen* codegen, const HInstruction* instr) { \
      RType::Lower(codegen, instr);                              \
    };                                                           \
    AddRule(opcode, pattern, std::move(gen));                    \
  } while (false)

// Purely used for formatting with clang-format
#define NONE() \
  do {         \
  } while (false)

#define CRASH(MSG)                                                          \
  do {                                                                      \
    auto gen = [](CodeGen*, const HInstruction*) { UNREACHABLE() << MSG; }; \
    AddRule(opcode, "", std::move(gen));                                    \
  } while (false)

TranslateRuleTable::TranslateRuleTable() {
  InternalInit();

  // --------------------------------------------------------------------------
  // Start of the rewriting rules here

  // --------------------------------------------------------------------------
  // Integer arithmetic operations

#define IMPL_BINARY(name, instr)                           \
  BEGIN(HOpcode::name) {                                   \
    /** output into register **/                           \
    RULE("Rri", MOVE(-1, 0), ASM(instr, REG(-1), IMM(1))); \
    RULE("Rrr", MOVE(1, 0), ASM(instr, REG(-1), REG(1)));  \
    RULE("Rrm", MOVE(-1, 0), ASM(instr, REG(-1), MEM(1))); \
    /** output into memory **/                             \
    RULE("Mri", MOVE(-1, 0), ASM(instr, MEM(-1), IMM(1))); \
    RULE("Mrr", MOVE(-1, 0), ASM(instr, MEM(-1), REG(1))); \
    RULE("Mrm", MOVE(-1, 0), ASM(mov, SCRATCH(D), MEM(1)), \
         ASM(instr, MEM(-1), SCRATCH(D)));                 \
  }                                                        \
  END();

  IMPL_BINARY(kIAdd, add);
  IMPL_BINARY(kISub, sub);
  IMPL_BINARY(kAnd, and_);
  IMPL_BINARY(kOr, or_);
  IMPL_BINARY(kXor, xor_);

#undef IMPL_BINARY  // IMPL_BINARY

  // clang-format off
  // --------------------------------------------------------------------------
  // integer multiplication
  BEGIN(HOpcode::kIMul) {
    RULE("Rri", ASM(imul, REG(-1), REG(0), IMM(1)));

    RULE("Rrr", MOVE(-1, 0),
                ASM(imul, REG(-1), REG(1)));

    RULE("Rrm", MOVE(-1, 0),
                ASM(imul, REG(-1), MEM(1)));

    RULE("Mri", ASM(mov, SCRATCH(D), REG(0)),
                ASM(imul, SCRATCH(D), IMM(1)),
                ASM(mov, MEM(-1), SCRATCH(D)));

    RULE("Mrr", ASM(mov, SCRATCH(D), REG(0)),
                ASM(imul, SCRATCH(D), REG(1)),
                ASM(mov, MEM(-1), SCRATCH(D)));

    RULE("Mrm", ASM(mov, SCRATCH(D), REG(0)),
                ASM(imul, SCRATCH(D), MEM(1)),
                ASM(mov, MEM(-1), SCRATCH(D)));
  }
  END();

  // --------------------------------------------------------------------------
  // integer mod, notes we do not have division since we never generate it
  // scratch = rax
  // clang-format off
  BEGIN(HOpcode::kIMod) {
    RULE("Rrr", ASM(mov, EAX(), REG(0)), 
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), REG(1)),
                ASM(mov, REG(-1), EDX()));

    RULE("Mrr", ASM(mov, EAX(), REG(0)), 
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), REG(1)),
                ASM(mov, MEM(-1), EDX()));
  }
  END();
  // clang-format on

  // --------------------------------------------------------------------------
  // Guarded int arithmetic
  // GIAdd, GISub, GIMul, if overflow, then just deoptimization
  BEGIN(HOpcode::kGIAdd) {
    RULE("Rrr", CALL(LowerGIAdd));
    RULE("Rrm", CALL(LowerGIAdd));
    RULE("Rri", CALL(LowerGIAdd));

    RULE("Mri", CALL(LowerGIAdd));
    RULE("Mrm", CALL(LowerGIAdd));
    RULE("Mrr", CALL(LowerGIAdd));
  }
  END();

  BEGIN(HOpcode::kGISub) {
    RULE("Rrr", CALL(LowerGISub));
    RULE("Rrm", CALL(LowerGISub));
    RULE("Rri", CALL(LowerGISub));

    RULE("Mri", CALL(LowerGISub));
    RULE("Mrm", CALL(LowerGISub));
    RULE("Mrr", CALL(LowerGISub));
  }
  END();

  BEGIN(HOpcode::kGIMul) {
    RULE("Rrr", CALL(LowerGIMul));
    RULE("Rrm", CALL(LowerGIMul));
    RULE("Rri", CALL(LowerGIMul));

    RULE("Mrr", CALL(LowerGIMul));
    RULE("Mrm", CALL(LowerGIMul));
    RULE("Mri", CALL(LowerGIMul));
  }
  END();

  BEGIN(HOpcode::kFMod) {
    RULE("Xxx", CALL(LowerFMod));
    RULE("Xxm", CALL(LowerFMod));

    RULE("Mxx", CALL(LowerFMod));
    RULE("Mxm", CALL(LowerFMod));
  }
  END();

  // --------------------------------------------------------------------------
  // Floating point operation

  BEGIN(HOpcode::kFAdd) {
    RULE("Xxx", MOVESD(-1, 0), ASM(addsd, XMM(-1), XMM(1)));
    RULE("Xxm", MOVESD(-1, 0), ASM(addsd, XMM(-1), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kFSub) {
    RULE("Xxx", MOVESD(-1, 0), ASM(subsd, XMM(-1), XMM(1)));
    RULE("Xxm", MOVESD(-1, 0), ASM(subsd, XMM(-1), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kFMul) {
    RULE("Xxx", MOVESD(-1, 0), ASM(mulsd, XMM(-1), XMM(1)));
    RULE("Xxm", MOVESD(-1, 0), ASM(mulsd, XMM(-1), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kFDiv) {
    RULE("Xxx", MOVESD(-1, 0), ASM(divsd, XMM(-1), XMM(1)));
    RULE("Xxm", MOVESD(-1, 0), ASM(divsd, XMM(-1), MEM(1)));
  }
  END();

  // ----------------------------------------------------------------------------
  // Unary operations
#define IMPL_UNARY_INT(X)                                      \
  RULE("Rr", MOVE(-1, 0), ASM(X, REG(-1)));                    \
  RULE("Rm", MOVE(-1, 0), ASM(X, REG(-1)));                    \
  RULE("Mr", MOVE(-1, 0), ASM(X, MEM(-1)));                    \
  RULE("Mm", ASM(mov, SCRATCH(D), MEM(0)), ASM(X, SCRATCH(D)), \
       ASM(mov, MEM(-1), SCRATCH(D)))

  // --------------------------------------------------------------------------
  // Inc
  BEGIN(HOpcode::kInc) {
    IMPL_UNARY_INT(inc);
    NONE();
  }
  END();

  BEGIN(HOpcode::kIncF) {
    RULE("Xx", CALL(LowerIncF));
    RULE("Xm", CALL(LowerIncF));
    RULE("Mx", CALL(LowerIncF));
    RULE("Mm", CALL(LowerIncF));
  }
  END();

  BEGIN(HOpcode::kGInc) {
    RULE("Rr", CALL(LowerGInc));
    RULE("Rm", CALL(LowerGInc));
    RULE("Mr", CALL(LowerGInc));
    RULE("Mm", CALL(LowerGInc));
  }
  END();

  // --------------------------------------------------------------------------
  // Inc
  BEGIN(HOpcode::kDec) {
    IMPL_UNARY_INT(dec);
    NONE();
  }
  END();

  BEGIN(HOpcode::kDecF) {
    RULE("Xx", CALL(LowerDecF));
    RULE("Xm", CALL(LowerDecF));
    RULE("Mx", CALL(LowerDecF));
    RULE("Mm", CALL(LowerDecF));
  }
  END();

  BEGIN(HOpcode::kGDec) {
    RULE("Rr", CALL(LowerGDec));
    RULE("Rm", CALL(LowerGDec));
    RULE("Mr", CALL(LowerGDec));
    RULE("Mm", CALL(LowerGDec));
  }
  END();

  // --------------------------------------------------------------------------
  // Neg
  BEGIN(HOpcode::kNeg) {
    IMPL_UNARY_INT(neg);
    RULE("Ri", ASM(mov, REG(-1), ImmOperandNegate<IMM(0)>));
  }
  END();

  BEGIN(HOpcode::kNegF) {
    RULE("Xx", CALL(LowerNegF));
    RULE("Xm", CALL(LowerNegF));
    RULE("Mx", CALL(LowerNegF));
    RULE("Mm", CALL(LowerNegF));
  }
  END();

  BEGIN(HOpcode::kGNeg) {
    RULE("Rr", CALL(LowerGNeg));
    RULE("Rm", CALL(LowerGNeg));
    RULE("Mr", CALL(LowerGNeg));
    RULE("Mm", CALL(LowerGNeg));
  }
  END();

  // --------------------------------------------------------------------------
  // Invert
  BEGIN(HOpcode::kInv) {
    RULE("Ri", ASM(mov, REG(-1), ImmOperandInvert<IMM(0)>));
    RULE("Rr", MOVE(-1, 0), ASM(xor_, REG(-1), CONST(1)));
    RULE("Rm", MOVE(-1, 0), ASM(xor_, REG(-1), CONST(1)));
    RULE("Mr", MOVE(-1, 0), ASM(xor_, MEM(-1), CONST(1)));
    RULE("Mm", ASM(mov, SCRATCH(D), MEM(1)), ASM(xor_, MEM(-1), CONST(1)));
  }
  END();

#undef IMPL_UNARY_INT  // IMPL_UNARY

  // --------------------------------------------------------------------------
  // Shift
  //
  // shl/shr/sar have 2 versions, one is the XXXConst version
  // which have its second operand (shift bits) in immediate number mode.
  // And the other version is register version, which requires Rcx to be
  // reserved by RA. One thing to be noted, is that the SHIFT's first operand
  // can be memory and we do not force the operand to be none memory
  //
  // clang-format off
  BEGIN(HOpcode::kShlConst) {
    RULE("Rri", MOVE(-1, 0), ASM(shl, REG(-1), IMM8(1)));
    RULE("Rmi", MOVE(-1, 0), ASM(shl, REG(-1), IMM8(1)));

    RULE("Mri", MOVE(-1, 0), ASM(shl, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shl, MEM(-1), IMM8(1)));
  }
  END();

  BEGIN(HOpcode::kShrConst) {
    RULE("Rri", MOVE(-1, 0), ASM(shr, REG(-1), IMM8(1)));
    RULE("Rmi", MOVE(-1, 0), ASM(shr, REG(-1), IMM8(1)));

    RULE("Mri", MOVE(-1, 0), ASM(shr, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shr, MEM(-1), IMM8(1)));
  }
  END();

  BEGIN(HOpcode::kSarConst) {
    RULE("Rri", MOVE(-1, 0), ASM(sar, REG(-1), IMM8(1)));
    RULE("Rmi", MOVE(-1, 0), ASM(sar, REG(-1), IMM8(1)));

    RULE("Mri", MOVE(-1, 0), ASM(sar, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(sar, MEM(-1), IMM8(1)));
  }
  END();

  // notes for Shl/Shr/Sar the RCX register is been reserved for operand, and
  // should already been placed well
  BEGIN(HOpcode::kShl) {
    RULE("Rri", MOVE(-1, 0), ASM(shl, REG(-1), IMM8(1)));
    RULE("Rmi", MOVE(-1, 0), ASM(shl, REG(-1), IMM8(1)));

    RULE("Mri", MOVE(-1, 0), ASM(shl, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shl, MEM(-1), IMM8(1)));

    RULE("Rrr", MOVE(-1, 0), ASM(shl, REG(-1), REG(1)));
    RULE("Rmr", MOVE(-1, 0), ASM(shl, REG(-1), REG(1)));

    RULE("Mrr", MOVE(-1, 0), ASM(shl, MEM(-1), REG(1)));
    RULE("Mmr", ASM(mov, SCRATCH(D), REG(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shl, MEM(-1), REG(1)));
  }
  END();

  BEGIN(HOpcode::kShr) {
    RULE("Rri", MOVE(-1, 0), ASM(shr, REG(-1), IMM8(1)));
    RULE("Rmi", MOVE(-1, 0), ASM(shr, REG(-1), IMM8(1)));

    RULE("Mri", MOVE(-1, 0), ASM(shr, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shr, MEM(-1), IMM8(1)));

    RULE("Rrr", MOVE(-1, 0), ASM(shr, REG(-1), REG(1)));
    RULE("Rmr", MOVE(-1, 0), ASM(shr, REG(-1), REG(1)));

    RULE("Mrr", MOVE(-1, 0), ASM(shr, MEM(-1), REG(1)));
    RULE("Mmr", ASM(mov, SCRATCH(D), REG(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shr, MEM(-1), REG(1)));
  }
  END();

  BEGIN(HOpcode::kSar) {
    RULE("Rri", MOVE(-1, 0), ASM(sar, REG(-1), IMM8(1)));
    RULE("Rmi", MOVE(-1, 0), ASM(sar, REG(-1), IMM8(1)));

    RULE("Mri", MOVE(-1, 0), ASM(sar, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(sar, MEM(-1), IMM8(1)));

    RULE("Rrr", MOVE(-1, 0), ASM(sar, REG(-1), REG(1)));
    RULE("Rmr", MOVE(-1, 0), ASM(sar, REG(-1), REG(1)));

    RULE("Mrr", MOVE(-1, 0), ASM(sar, MEM(-1), REG(1)));
    RULE("Mmr", ASM(mov, SCRATCH(D), REG(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(sar, MEM(-1), REG(1)));
  }
  END();

  // clang-format on

  // --------------------------------------------------------------------------
  // Comparison value
  //   On x64, we will utilize conditional mov instruction to avoid branch
  BEGIN(HOpcode::kILt) {
    RULE("Rri", CALL(LowerILt));
    RULE("Rrr", CALL(LowerILt));
    RULE("Rrm", CALL(LowerILt));

    RULE("Mri", CALL(LowerILt));
    RULE("Mrr", CALL(LowerILt));
    RULE("Mrm", CALL(LowerILt));
  }
  END();

  BEGIN(HOpcode::kILe) {
    RULE("Rri", CALL(LowerILe));
    RULE("Rrr", CALL(LowerILe));
    RULE("Rrm", CALL(LowerILe));

    RULE("Mri", CALL(LowerILe));
    RULE("Mrr", CALL(LowerILe));
    RULE("Mrm", CALL(LowerILe));
  }
  END();

  BEGIN(HOpcode::kIGt) {
    RULE("Rri", CALL(LowerIGt));
    RULE("Rrr", CALL(LowerIGt));
    RULE("Rrm", CALL(LowerIGt));

    RULE("Mri", CALL(LowerIGt));
    RULE("Mrr", CALL(LowerIGt));
    RULE("Mrm", CALL(LowerIGt));
  }
  END();

  BEGIN(HOpcode::kIGe) {
    RULE("Rri", CALL(LowerIGe));
    RULE("Rrr", CALL(LowerIGe));
    RULE("Rrm", CALL(LowerIGe));

    RULE("Mri", CALL(LowerIGe));
    RULE("Mrr", CALL(LowerIGe));
    RULE("Mrm", CALL(LowerIGe));
  }
  END();

  BEGIN(HOpcode::kIEq) {
    RULE("Rri", CALL(LowerIEq));
    RULE("Rrr", CALL(LowerIEq));
    RULE("Rrm", CALL(LowerIEq));

    RULE("Mri", CALL(LowerIEq));
    RULE("Mrr", CALL(LowerIEq));
    RULE("Mrm", CALL(LowerIEq));
  }
  END();

  BEGIN(HOpcode::kINe) {
    RULE("Rri", CALL(LowerINe));
    RULE("Rrr", CALL(LowerINe));
    RULE("Rrm", CALL(LowerINe));

    RULE("Mri", CALL(LowerINe));
    RULE("Mrr", CALL(LowerINe));
    RULE("Mrm", CALL(LowerINe));
  }
  END();

  BEGIN(HOpcode::kFLt) {
    RULE("Rxx", CALL(LowerFLt));
    RULE("Rxm", CALL(LowerFLt));

    RULE("Mxx", CALL(LowerFLt));
    RULE("Mxm", CALL(LowerFLt));
  }
  END();

  BEGIN(HOpcode::kFLe) {
    RULE("Rxx", CALL(LowerFLe));
    RULE("Rxm", CALL(LowerFLe));

    RULE("Mxx", CALL(LowerFLe));
    RULE("Mxm", CALL(LowerFLe));
  }
  END();

  BEGIN(HOpcode::kFGt) {
    RULE("Rxx", CALL(LowerFGt));
    RULE("Rxm", CALL(LowerFGt));

    RULE("Mxx", CALL(LowerFGt));
    RULE("Mxm", CALL(LowerFGt));
  }
  END();

  BEGIN(HOpcode::kFGe) {
    RULE("Rxx", CALL(LowerFGe));
    RULE("Rxm", CALL(LowerFGe));

    RULE("Mxx", CALL(LowerFGe));
    RULE("Mxm", CALL(LowerFGe));
  }
  END();

  BEGIN(HOpcode::kFEq) {
    RULE("Rxx", CALL(LowerFEq));
    RULE("Rxm", CALL(LowerFEq));

    RULE("Mxx", CALL(LowerFEq));
    RULE("Mxm", CALL(LowerFEq));
  }
  END();

  BEGIN(HOpcode::kFNe) {
    RULE("Rxx", CALL(LowerFNe));
    RULE("Rxm", CALL(LowerFNe));

    RULE("Mxx", CALL(LowerFNe));
    RULE("Mxm", CALL(LowerFNe));
  }
  END();

  // --------------------------------------------------------------------------
  // Condition(test/cmp)
  BEGIN(HOpcode::kCmpI) {
    RULE("ri", ASM(cmp, REG(0), IMM(1)));
    RULE("mi", ASM(cmp, MEM(0), IMM(1)));
    RULE("mr", ASM(cmp, MEM(0), REG(1)));
    RULE("rm", ASM(cmp, REG(0), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kCmpF) {
    RULE("xx", ASM(comisd, XMM(0), XMM(1)));
    RULE("xm", ASM(comisd, XMM(0), MEM(1)));

    // require rewriting
    RULE("mx", ASM(movsd, SCRATCH(X), MEM(0)), ASM(comisd, SCRATCH(X), XMM(1)));
    RULE("mm", ASM(movsd, SCRATCH(X), MEM(0)), ASM(comisd, SCRATCH(X), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kCmpTest) {
    RULE("m", ASM(mov, SCRATCH(D), MEM(0)), ASM(test, SCRATCH(D), SCRATCH(D)));
    RULE("r", ASM(test, REG(0), REG(0)));
  }
  END();

  BEGIN(HOpcode::kCmpBoxIsHeap) {
    RULE("r", CALL(LowerCmpBoxIsHeap));
    RULE("m", CALL(LowerCmpBoxIsHeap));
  }
  END();

  BEGIN(HOpcode::kCmpBoxIsPrimitive) {
    RULE("r", CALL(LowerCmpBoxIsPrimitive));
    RULE("m", CALL(LowerCmpBoxIsPrimitive));
  }
  END();

  // --------------------------------------------------------------------------
  // JCC
  // clang-format off
  BEGIN(HOpcode::kJmpNe) { 
    RULE("b", ASM(jne, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpEq) { 
    RULE("b", ASM(je, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpZ) { 
    RULE("b", ASM(jz, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpNz) { 
    RULE("b", ASM(jnz, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpLt) { 
    RULE("b", ASM(jl, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpLe) { 
    RULE("b", ASM(jle, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpBt) { 
    RULE("b", ASM(jb, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpBe) { 
    RULE("b", ASM(jbe, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpGt) { 
    RULE("b", ASM(jg, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpGe) { 
    RULE("b", ASM(jge, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpAt) { 
    RULE("b", ASM(ja, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpAe) { 
    RULE("b", ASM(jae, LBL(0))); 
  }
  END();

  BEGIN(HOpcode::kJmpBoxIsHeap) {
    RULE("b", ASM(jl, LBL(0)));
  }
  END();

  BEGIN(HOpcode::kJmpBoxIsPrimitive) {
    RULE("b", ASM(jge, LBL(0)));
  }
  END();


  BEGIN(HOpcode::kJmp) {
    RULE("b", ASM(jmp, LBL(0)));
  }
  END();
  // clang-format on

  // --------------------------------------------------------------------------
  // MOV (integer/GP) move
  BEGIN(HOpcode::kMov) {
    // general purpose
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(LowerMoveMM));
  }
  END();

  BEGIN(HOpcode::kShuffle) {
    // general purpose
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(LowerMoveMM));
  }
  END();

  BEGIN(HOpcode::kDupI) {
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(LowerMoveMM));
  }
  END();

  BEGIN(HOpcode::kDupObj) {
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(LowerMoveMM));
  }
  END();

  BEGIN(HOpcode::kTrans) {
    // general purpose
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(LowerMoveMM));
  }
  END();

  // --------------------------------------------------------------------------
  // MOVF (float/FP) move
  BEGIN(HOpcode::kMovF) {
    // floating point, notes, floating point mov does not support immediate
    // number, and have to carry out via MovI64
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));

    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(LowerMoveFMM));
  }
  END();

  BEGIN(HOpcode::kShuffleF) {
    // floating point, notes, floating point mov does not support immediate
    // number, and have to carry out via MovI64
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));

    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(LowerMoveFMM));
  }
  END();

  BEGIN(HOpcode::kDupF) {
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));
    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(LowerMoveFMM));
  }
  END();

  BEGIN(HOpcode::kTransF) {
    // floating point, notes, floating point mov does not support immediate
    // number, and have to carry out via MovI64
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));

    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(LowerMoveFMM));
  }
  END();

  // --------------------------------------------------------------------------
  // MovI64/MovB64/MovI32/MovF64
  BEGIN(HOpcode::kMovI64) {
    RULE("Ri", CALL(LowerMoveI64));
    RULE("Mi", CALL(LowerMoveI64));
  }
  END();

  BEGIN(HOpcode::kMovF64) {
    RULE("Xi", CALL(LowerMoveF64));
    RULE("Mi", CALL(LowerMoveF64));
  }
  END();

  BEGIN(HOpcode::kMovB64) {
    RULE("Ri", CALL(LowerMoveB64));
    RULE("Mi", CALL(LowerMoveB64));
  }
  END();

  BEGIN(HOpcode::kMovI32) {
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));
  }
  END();

  // --------------------------------------------------------------------------
  // Pop/Push
  BEGIN(HOpcode::kPop) {
    RULE("R", ASM(pop, REG(-1)));
    RULE("M", ASM(pop, MEM(-1)));
  }
  END();

  BEGIN(HOpcode::kPush) {
    RULE("R", ASM(push, REG(-1)));
    RULE("M", ASM(push, MEM(-1)));
  }
  END();

  // --------------------------------------------------------------------------
  // Lea
  BEGIN(HOpcode::kLea) {
    RULE("Rm", ASM(lea, REG(-1), MEM(0)));
    NONE();
  }
  END();

  // --------------------------------------------------------------------------
  // interpreter value loading
  BEGIN(HOpcode::kLoadLocal) {
    RULE("Ri", CALL(LowerLoadLocal));
    RULE("Mi", CALL(LowerLoadLocal));
  }
  END();

  BEGIN(HOpcode::kLoadParam) {
    RULE("Ri", CALL(LowerLoadParam));
    RULE("Mi", CALL(LowerLoadParam));
  }
  END();

  BEGIN(HOpcode::kLoadTemp) {
    RULE("Ri", CALL(LowerLoadTemp));
    RULE("Mi", CALL(LowerLoadTemp));
  }
  END();

  BEGIN(HOpcode::kBoxToTemp) {
    RULE("rii", CALL(LowerBoxToTemp));
    RULE("xii", CALL(LowerBoxToTemp));
    RULE("mii", CALL(LowerBoxToTemp));
    RULE("iii", CALL(LowerBoxToTemp));
  }
  END();

  BEGIN(HOpcode::kStoreTemp) {
    RULE("ri", CALL(LowerStoreTemp));
    RULE("mi", CALL(LowerStoreTemp));
  }
  END();

  BEGIN(HOpcode::kLeaTemp) {
    RULE("Ri", CALL(LowerLeaTemp));
    RULE("Mi", CALL(LowerLeaTemp));
  }
  END();

  // --------------------------------------------------------------------------
  // cast
  // clang-format off
  BEGIN(HOpcode::kCastI32ToF64) {
    RULE("Xr", ASM(cvtsi2sd, XMM(-1), REG(0)));
    RULE("Xm", ASM(cvtsi2sd, XMM(-1), MEM(0)));

    RULE("Mr", ASM(cvtsi2sd, SCRATCH(X), REG(0)), 
               ASM(movq, MEM(-1), SCRATCH(X)));

    RULE("Mm", ASM(cvtsi2sd, SCRATCH(X), MEM(0)),
               ASM(movq, MEM(-1), SCRATCH(X)));
  }
  END();

  BEGIN(HOpcode::kCastF64ToB32) {
    RULE("Rx", CALL(LowerCastF64ToB32));
    RULE("Rm", CALL(LowerCastF64ToB32));

    RULE("Mx", CALL(LowerCastF64ToB32));
    RULE("Mm", CALL(LowerCastF64ToB32));
  }
  END();

  BEGIN(HOpcode::kCastBToStr) {
    RULE("Rx", CALL(LowerCastBToStr));
    RULE("Rm", CALL(LowerCastBToStr));

    RULE("Mx", CALL(LowerCastBToStr));
    RULE("Mm", CALL(LowerCastBToStr));
  }
  END();

  // clang-format on

  // --------------------------------------------------------------------------
  // function call (native call)
  BEGIN(HOpcode::kRCallPtr) {
    RULE("R*", CALL(LowerRCallPtr));
    RULE("M*", CALL(LowerRCallPtr));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallB64) {
    RULE("R*", CALL(LowerRCallB64));
    RULE("M*", CALL(LowerRCallB64));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallF64) {
    // notes F64 call will only return to xmm0, always register for now
    RULE("X*", CALL(LowerRCallF64));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallI32) {
    RULE("R*", CALL(LowerRCallI32));
    RULE("M*", CALL(LowerRCallI32));
  }
  END();

  BEGIN(HOpcode::kRCallVoid) {
    RULE("*", CALL(LowerRCallVoid));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallBox) {
    RULE("M*", CALL(LowerRCallBox));
    RULE("R*", CALL(LowerRCallBox));
    NONE();
  }
  END();

  // --------------------------------------------------------------------------
  // jscall
  BEGIN(HOpcode::kJSCallF) {
    RULE("Rrir", CALL(LowerJSCallF));
    RULE("Rmir", CALL(LowerJSCallF));
    RULE("Rmim", CALL(LowerJSCallF));
    RULE("Rrim", CALL(LowerJSCallF));

    RULE("Mrir", CALL(LowerJSCallF));
    RULE("Mmir", CALL(LowerJSCallF));
    RULE("Mmim", CALL(LowerJSCallF));
    RULE("Mrim", CALL(LowerJSCallF));
  }
  END();

  BEGIN(HOpcode::kJSCallFT) {
    RULE("R*", CALL(LowerJSCallFT));
    RULE("M*", CALL(LowerJSCallFT));
  }
  END();

  BEGIN(HOpcode::kJSCallFN) {
    RULE("R*", CALL(LowerJSCallFN));
    RULE("M*", CALL(LowerJSCallFN));
  }
  END();

  BEGIN(HOpcode::kJSCallFTN) {
    RULE("R*", CALL(LowerJSCallFTN));
    RULE("M*", CALL(LowerJSCallFTN));
  }
  END();

  BEGIN(HOpcode::kJSCallCtor) {
    RULE("R*", CALL(LowerJSCallCtor));
    RULE("M*", CALL(LowerJSCallCtor));
  }
  END();

  BEGIN(HOpcode::kArrayFrom) {
    RULE("R*", CALL(LowerArrayFrom));
    RULE("M*", CALL(LowerArrayFrom));
  }
  END();

  BEGIN(HOpcode::kApply) {
    RULE("R*", CALL(LowerApply));
    RULE("M*", CALL(LowerApply));
  }
  END();

  // --------------------------------------------------------------------------
  // object
  BEGIN(HOpcode::kObjectFieldGetAtomFast) {
    RULE("Rri", CALL(LowerObjectFieldGetAtomFast));
    RULE("Rmi", CALL(LowerObjectFieldGetAtomFast));

    RULE("Mri", CALL(LowerObjectFieldGetAtomFast));
    RULE("Mmi", CALL(LowerObjectFieldGetAtomFast));
  }
  END();

  BEGIN(HOpcode::kGlobalGetVarAtomFast) {
    RULE("Rii", CALL(LowerGlobalGetVarAtomFast));
    RULE("Mii", CALL(LowerGlobalGetVarAtomFast));
  }
  END();

  BEGIN(HOpcode::kGlobalGetGlobalAtomFast) {
    RULE("Rii", CALL(LowerGlobalGetGlobalAtomFast));
    RULE("Mii", CALL(LowerGlobalGetGlobalAtomFast));
  }
  END();

  // --------------------------------------------------------------------------
  // array
#define LOWER_LOAD_ARRAY(XX)               \
  BEGIN(HOpcode::kLoadArray##XX) {         \
    RULE("Rrr", CALL(LowerLoadArray##XX)); \
    RULE("Rrm", CALL(LowerLoadArray##XX)); \
    RULE("Rmr", CALL(LowerLoadArray##XX)); \
    RULE("Rmm", CALL(LowerLoadArray##XX)); \
    RULE("Rmi", CALL(LowerLoadArray##XX)); \
    RULE("Rri", CALL(LowerLoadArray##XX)); \
                                           \
    RULE("Mrr", CALL(LowerLoadArray##XX)); \
    RULE("Mrm", CALL(LowerLoadArray##XX)); \
    RULE("Mmr", CALL(LowerLoadArray##XX)); \
    RULE("Mmm", CALL(LowerLoadArray##XX)); \
    RULE("Mmi", CALL(LowerLoadArray##XX)); \
    RULE("Mri", CALL(LowerLoadArray##XX)); \
  }                                        \
  END()

  LOWER_LOAD_ARRAY(Value);
  LOWER_LOAD_ARRAY(Arguments);
  LOWER_LOAD_ARRAY(Int8);
  LOWER_LOAD_ARRAY(UInt8);
  LOWER_LOAD_ARRAY(UInt8C);
  LOWER_LOAD_ARRAY(Int16);
  LOWER_LOAD_ARRAY(UInt16);
  LOWER_LOAD_ARRAY(Int32);
  LOWER_LOAD_ARRAY(UInt32);
  LOWER_LOAD_ARRAY(F32);
  LOWER_LOAD_ARRAY(F64);

#undef LOWER_LOAD_ARRAY

  // --------------------------------------------------------------------------
  // Context
  BEGIN(HOpcode::kContextLoad) {
    RULE("R", CALL(LowerContextLoad));
    RULE("M", CALL(LowerContextLoad));
  }
  END();

  BEGIN(HOpcode::kContextLoadGlobalObject) {
    RULE("R", CALL(LowerContextLoadGlobalObject));
    RULE("M", CALL(LowerContextLoadGlobalObject));
  }
  END();

  // --------------------------------------------------------------------------
  // Atom
  BEGIN(HOpcode::kIsBoxAtomInt) {
    RULE("Rr", CALL(LowerIsBoxAtomInt));
    RULE("Mr", CALL(LowerIsBoxAtomInt));

    RULE("Rm", CALL(LowerIsBoxAtomInt));
    RULE("Mm", CALL(LowerIsBoxAtomInt));
  }
  END();

  BEGIN(HOpcode::kAtomFromI) {
    RULE("Rr", CALL(LowerAtomFromI));
    RULE("Rm", CALL(LowerAtomFromI));
    RULE("Ri", CALL(LowerAtomFromI));

    RULE("Mr", CALL(LowerAtomFromI));
    RULE("Mi", CALL(LowerAtomFromI));
    RULE("Mm", CALL(LowerAtomFromI));
  }
  END();

  BEGIN(HOpcode::kGAtom) {
    RULE("Rr", CALL(LowerGAtom));
    RULE("Rm", CALL(LowerGAtom));

    RULE("Mr", CALL(LowerGAtom));
    RULE("Mm", CALL(LowerGAtom));
  }
  END();

  // --------------------------------------------------------------------------
  // Boxing Operations
  BEGIN(HOpcode::kGUnboxI) {
    RULE("Rr", CALL(LowerGUnboxI));
    RULE("Mr", CALL(LowerGUnboxI));

    RULE("Rm", CALL(LowerGUnboxI));
    RULE("Mm", CALL(LowerGUnboxI));
  }
  END();

  BEGIN(HOpcode::kGUnboxB) {
    RULE("Rr", CALL(LowerGUnboxB));
    RULE("Mr", CALL(LowerGUnboxB));

    RULE("Rm", CALL(LowerGUnboxB));
    RULE("Mm", CALL(LowerGUnboxB));
  }
  END();

  BEGIN(HOpcode::kGUnboxF) {
    RULE("Xr", CALL(LowerGUnboxF));
    RULE("Mr", CALL(LowerGUnboxF));

    RULE("Xm", CALL(LowerGUnboxF));
    RULE("Mm", CALL(LowerGUnboxF));
  }
  END();

  BEGIN(HOpcode::kGUnboxPtr) {
    RULE("Rr", CALL(LowerGUnboxPtr));
    RULE("Mr", CALL(LowerGUnboxPtr));

    RULE("Rm", CALL(LowerGUnboxPtr));
    RULE("Mm", CALL(LowerGUnboxPtr));
  }
  END();

  BEGIN(HOpcode::kUnboxPtr) {
    RULE("Rr", CALL(LowerUnboxPtr));
    RULE("Mr", CALL(LowerUnboxPtr));
    RULE("Rm", CALL(LowerUnboxPtr));
    RULE("Mm", CALL(LowerUnboxPtr));
  }
  END();

  BEGIN(HOpcode::kUnboxI) {
    RULE("Rr", CALL(LowerUnboxI));
    RULE("Mr", CALL(LowerUnboxI));
    RULE("Rm", CALL(LowerUnboxI));
    RULE("Mm", CALL(LowerUnboxI));
  }
  END();

  BEGIN(HOpcode::kUnboxF) {
    RULE("Rr", CALL(LowerUnboxF));
    RULE("Mr", CALL(LowerUnboxF));
    RULE("Rm", CALL(LowerUnboxF));
    RULE("Mm", CALL(LowerUnboxF));
  }
  END();

  BEGIN(HOpcode::kGetTTag) {
    RULE("Rr", CALL(LowerGetTTag));
    RULE("Mr", CALL(LowerGetTTag));
    RULE("Rm", CALL(LowerGetTTag));
    RULE("Mm", CALL(LowerGetTTag));
  }
  END();

  BEGIN(HOpcode::kIsNullOrUndefined) {
    RULE("Rr", CALL(LowerIsNullOrUndefined));
    RULE("Mr", CALL(LowerIsNullOrUndefined));
    RULE("Rm", CALL(LowerIsNullOrUndefined));
    RULE("Mm", CALL(LowerIsNullOrUndefined));
  }
  END();

  BEGIN(HOpcode::kBox) {
    RULE("Rri", CALL(LowerBox));
    RULE("Rxi", CALL(LowerBox));
    RULE("Rmi", CALL(LowerBox));

    RULE("Mri", CALL(LowerBox));
    RULE("Mxi", CALL(LowerBox));
    RULE("Mmi", CALL(LowerBox));
  }
  END();

  // --------------------------------------------------------------------------
  // RC
  BEGIN(HOpcode::kRcInc) {
    RULE("r", CALL(LowerRcInc));
    RULE("m", CALL(LowerRcInc));
  }
  END();

  BEGIN(HOpcode::kRcDec) {
    RULE("r", CALL(LowerRcDec));
    RULE("m", CALL(LowerRcDec));
  }
  END();

  // --------------------------------------------------------------------------
  // deoptimization operations
  //
  // clang-format off
  BEGIN(HOpcode::kDeopt) {
    RULE("-", CALL(LowerDeopt));
    NONE();
  }
  END();

  BEGIN(HOpcode::kDeoptIfTrue) {
    RULE("m", CALL(LowerDeoptIfTrue));
    RULE("r", CALL(LowerDeoptIfTrue));
  }
  END();

  BEGIN(HOpcode::kDeoptIfFalse) {
    RULE("m", CALL(LowerDeoptIfFalse));
    RULE("r", CALL(LowerDeoptIfFalse));
  }
  END();

  BEGIN(HOpcode::kDeoptNe) { 
    RULE("-", CALL(LowerDeoptNe));
  }
  END();

  BEGIN(HOpcode::kDeoptEq) { 
    RULE("-", CALL(LowerDeoptEq));
  }
  END();

  BEGIN(HOpcode::kDeoptZ) { 
    RULE("-", CALL(LowerDeoptZ));
  }
  END();

  BEGIN(HOpcode::kDeoptNz) { 
    RULE("-", CALL(LowerDeoptNz));
  }
  END();

  BEGIN(HOpcode::kDeoptLt) { 
    RULE("-", CALL(LowerDeoptLt));
  }
  END();

  BEGIN(HOpcode::kDeoptLe) { 
    RULE("-", CALL(LowerDeoptLe));
  }
  END();

  BEGIN(HOpcode::kDeoptGt) { 
    RULE("-", CALL(LowerDeoptGt));
  }
  END();

  BEGIN(HOpcode::kDeoptGe) { 
    RULE("-", CALL(LowerDeoptGe));
  }
  END();

  BEGIN(HOpcode::kDeoptAt) { 
    RULE("-", CALL(LowerDeoptAt));
  }
  END();

  BEGIN(HOpcode::kDeoptAe) { 
    RULE("-", CALL(LowerDeoptAe));
  }
  END();

  BEGIN(HOpcode::kDeoptBt) { 
    RULE("-", CALL(LowerDeoptBt));
  }
  END();

  BEGIN(HOpcode::kDeoptBe) { 
    RULE("-", CALL(LowerDeoptBe));
  }
  END();
  // clang-format on
}

Error TranslateLIR(Quake* quake, CodeGen* codegen, HAssembler* has,
                   const HInstruction* ins) {
  UNUSE(has);

  if (auto entry = TranslateRuleTable::GetInstance().MatchRule(ins); entry) {
    entry(codegen, ins);
    return {};
  }
  return Error::Mk("translation rule not found: %s", ins->Print().c_str());
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
