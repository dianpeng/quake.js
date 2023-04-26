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
    return u::Imm(operand);
  }
};

template <int N>
struct Imm8Operand {
  using asmjit_type = const a::Imm&;

  static a::Imm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return u::Imm8(operand);
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
    return u::Gp(operand);
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

struct RcxOperand {
  using asmjit_type = const a::x86::Gp&;

  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::rcx;
  }
};

struct EcxOperand {
  using asmjit_type = const a::x86::Gp&;

  static a::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return a::x86::ecx;
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
    return u::Fp(operand);
  }
};

template <int N>
struct MemOperand {
  using asmjit_type = const a::x86::Mem&;

  static a::x86::Mem GetAsmOperand(CodeGen* codegen,
                                   const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return u::Mem(operand);
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
  static void GenMoveMM(CodeGen*, const HInstruction*);
  static void GenMoveFMM(CodeGen*, const HInstruction*);

  static void DoMoveB64(CodeGen*, const HInstruction*);
  static void GenMoveI64(CodeGen*, const HInstruction*);
  static void GenMoveB64(CodeGen*, const HInstruction*);
  static void GenMoveF64(CodeGen*, const HInstruction*);

  // variable loading
  static void GenLoadLocal(CodeGen*, const HInstruction*);
  static void GenLoadParam(CodeGen*, const HInstruction*);
  static void GenLoadTemp(CodeGen*, const HInstruction*);
  static void GenLeaTemp(CodeGen*, const HInstruction*);

  static void DoBoxTo(CodeGen*, const HInstruction*, int);
  static void DoStore(CodeGen*, const HInstruction*, int);
  static void DoStore(CodeGen*, int offset, int index, const HOperand* value);

  static void GenBoxToTemp(CodeGen* cg, const HInstruction* ins) {
    DoBoxTo(cg, ins, Layout::Context::kTempCur);
  }
  static void GenStoreTemp(CodeGen* cg, const HInstruction* ins) {
    DoStore(cg, ins, Layout::Context::kTempCur);
  }

  static void GenBoxToLocal(CodeGen* cg, const HInstruction* ins) {
    DoBoxTo(cg, ins, Layout::Context::kLocal);
  }
  static void GenStoreLocal(CodeGen* cg, const HInstruction* ins) {
    DoStore(cg, ins, Layout::Context::kLocal);
  }

  static void GenBoxToParam(CodeGen* cg, const HInstruction* ins) {
    DoBoxTo(cg, ins, Layout::Context::kParam);
  }
  static void GenStoreParam(CodeGen* cg, const HInstruction* ins) {
    DoStore(cg, ins, Layout::Context::kParam);
  }

  static void GenLoadNull(CodeGen*, const HInstruction*);
  static void GenLoadUndefined(CodeGen*, const HInstruction*);
  static void GenLoadUninitialized(CodeGen*, const HInstruction*);
  static void GenLoadStringUnbox(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // VarRef (upvalue)
  static void GenLoadVarRef(CodeGen*, const HInstruction*);
  static void GenStoreVarRef(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // This
  static void GenLoadThis(CodeGen*, const HInstruction*);
  static void GenLoadThisTag(CodeGen*, const HInstruction*);

  // --------------------------------------------------------------------------
  // context
  static void GenContextLoad(CodeGen*, const HInstruction*);
  static void GenContextLoadJSContext(CodeGen*, const HInstruction*);
  static void GenContextLoadGlobalObject(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // Frame
  static void GenEnsureTempSize(CodeGen*, const HInstruction*);
  static void GenEnterInlineFrame(CodeGen*, const HInstruction*);
  static void GenLeaveInlineFrame(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // Floating point number arithmetic operation Xxx form
  //
  // The reason is to avoid cases when kFXXX operation has format of
  //   [X0][x1][x2]
  //
  // and X0 x2 points to same Xmm register, then if we perform following
  // instruction sequences as
  //
  //   1. mov X0, x1
  //   2. xxx X0, x2
  //
  // it will result in incorrect result, since X0 is x2 before the operation
  // so we end up with X1 * X1 instead of X1 *  X2
  static void GenFArithXxx(CodeGen*, const HInstruction*, HOpcode);

  static void GenFArithXxxkFAdd(CodeGen* a, const HInstruction* b) {
    GenFArithXxx(a, b, HOpcode::kFAdd);
  }
  static void GenFArithXxxkFSub(CodeGen* a, const HInstruction* b) {
    GenFArithXxx(a, b, HOpcode::kFSub);
  }
  static void GenFArithXxxkFMul(CodeGen* a, const HInstruction* b) {
    GenFArithXxx(a, b, HOpcode::kFMul);
  }
  static void GenFArithXxxkFDiv(CodeGen* a, const HInstruction* b) {
    GenFArithXxx(a, b, HOpcode::kFDiv);
  }

 private:
  // --------------------------------------------------------------------------
  // Guarded Integer
  static void GenGuardIntArithmetic(CodeGen* cg, const HInstruction* ins);

  static void GenGIAdd(CodeGen* cg, const HInstruction* ins) {
    return GenGuardIntArithmetic(cg, ins);
  }
  static void GenGISub(CodeGen* cg, const HInstruction* ins) {
    return GenGuardIntArithmetic(cg, ins);
  }
  static void GenGIMul(CodeGen* cg, const HInstruction* ins) {
    return GenGuardIntArithmetic(cg, ins);
  }

  // --------------------------------------------------------------------------
  // guarded left shift operation. The reason why we have this is because this
  // operation may overflow from the int32 range which require us to bailout
  // and pick up from the double
  static void GenGShl(CodeGen*, const HInstruction*);
  static void GenGShlConst(CodeGen*, const HInstruction*);

  static void GenGShr(CodeGen*, const HInstruction*);
  static void GenGShrConst(CodeGen*, const HInstruction*);

  static void GenGShift(CodeGen* cg, const HInstruction*, bool is_shift_left,
                        std::function<void(a::x86::Gp)>&& callback);

  static void GenGShiftConst(CodeGen* cg, const HInstruction* ins,
                             bool is_shift_left,
                             std::function<void(a::x86::Gp)>&& callback_reg,
                             std::function<void(a::x86::Mem)>&& callback_mem);

 private:
  static void GenFMod(CodeGen* cg, const HInstruction* ins);

 private:
  // --------------------------------------------------------------------------
  // Unary
  static void GenIncDecF(CodeGen* cg, const HInstruction* ins);
  static void GenIncF(CodeGen* cg, const HInstruction* ins) {
    GenIncDecF(cg, ins);
  }
  static void GenDecF(CodeGen* cg, const HInstruction* ins) {
    GenIncDecF(cg, ins);
  }

  // Guarded integer inc/dec
  static void GenGIncDec(CodeGen* cg, const HInstruction* ins);
  static void GenGInc(CodeGen* cg, const HInstruction* ins) {
    GenGIncDec(cg, ins);
  }
  static void GenGDec(CodeGen* cg, const HInstruction* ins) {
    GenGIncDec(cg, ins);
  }

  static void GenNegF(CodeGen* cg, const HInstruction* ins);
  static void GenGNeg(CodeGen* cg, const HInstruction* ins);
  static void GenInv(CodeGen* cg, const HInstruction* ins);

 private:
  // --------------------------------------------------------------------------
  // Comparison/Conditional value
  static void GenIntConditionValue(CodeGen* cg, const HInstruction* ins);

  static void GenILt(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }
  static void GenILe(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }
  static void GenIGt(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }
  static void GenIGe(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }
  static void GenINe(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }
  static void GenIEq(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }

  static void GenI64Ne(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }
  static void GenI64Eq(CodeGen* cg, const HInstruction* ins) {
    GenIntConditionValue(cg, ins);
  }

  static void GenFloatConditionValue(CodeGen* cg, const HInstruction* ins);

  static void GenFLt(CodeGen* cg, const HInstruction* ins) {
    GenFloatConditionValue(cg, ins);
  }
  static void GenFLe(CodeGen* cg, const HInstruction* ins) {
    GenFloatConditionValue(cg, ins);
  }
  static void GenFGt(CodeGen* cg, const HInstruction* ins) {
    GenFloatConditionValue(cg, ins);
  }
  static void GenFGe(CodeGen* cg, const HInstruction* ins) {
    GenFloatConditionValue(cg, ins);
  }
  static void GenFNe(CodeGen* cg, const HInstruction* ins) {
    GenFloatConditionValue(cg, ins);
  }
  static void GenFEq(CodeGen* cg, const HInstruction* ins) {
    GenFloatConditionValue(cg, ins);
  }

 private:
  static void GenCmpBoxIs(CodeGen*, const HInstruction*);

  static void GenCmpBoxIsHeap(CodeGen* cg, const HInstruction* ins) {
    GenCmpBoxIs(cg, ins);
  }
  static void GenCmpBoxIsPrimitive(CodeGen* cg, const HInstruction* ins) {
    GenCmpBoxIs(cg, ins);
  }

  static void GenCmpBoxTagIs(CodeGen*, const HInstruction*);

 private:
  static void GenVarRefListIs(CodeGen*, const HInstruction*);
  static void GenVarRefListIsEmpty(CodeGen*, const HInstruction*);
  static void GenVarRefListIsNotEmpty(CodeGen*, const HInstruction*);

  static void GenCmpVarRefListIsEmpty(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // cast
  static void GenCastF64ToB32(CodeGen*, const HInstruction*);
  static void GenCastF64ToB(CodeGen*, const HInstruction*);
  static void GenCastBToStr(CodeGen*, const HInstruction*);
  static void GenCastVToB(CodeGen*, const HInstruction*);
  static void GenCastObjToB(CodeGen*, const HInstruction*);
  static void GenCastStrToB(CodeGen*, const HInstruction*);
  static void GenCastNBToB(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // runtime call
  static void GenRCallGP(CodeGen*, const HInstruction*);
  static void GenRCallFP(CodeGen*, const HInstruction*);
  static std::uint64_t GetRCallAddress(const HInstruction* ins);

  static void GenRCallPtr(CodeGen* cg, const HInstruction* ins) {
    GenRCallGP(cg, ins);
  }
  static void GenRCallB64(CodeGen* cg, const HInstruction* ins) {
    GenRCallGP(cg, ins);
  }

  static void GenRCallI32(CodeGen* cg, const HInstruction* ins) {
    GenRCallGP(cg, ins);
  }

  static void GenRCallF64(CodeGen* cg, const HInstruction* ins) {
    GenRCallFP(cg, ins);
  }

  static void GenRCallVoid(CodeGen* cg, const HInstruction* ins);
  static void GenRCallBox(CodeGen* cg, const HInstruction* ins);

  // --------------------------------------------------------------------------
  // JS Call
  // --------------------------------------------------------------------------
  static void GenJSCallF(CodeGen*, const HInstruction*);
  static void GenJSCallFT(CodeGen*, const HInstruction*);
  static void GenJSCallFN(CodeGen*, const HInstruction*);
  static void GenJSCallFTN(CodeGen*, const HInstruction*);
  static void GenJSCallCtor(CodeGen*, const HInstruction*);
  static void GenArrayFrom(CodeGen*, const HInstruction*);
  static void GenApply(CodeGen*, const HInstruction*);

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

  template <typename T>
  static void DoScriptCall(CodeGen*, T);
  static void PerformScriptCall(CodeGen*);
  static void PerformScriptCallCtor(CodeGen*);
  static void PerformScriptCallArrayFrom(CodeGen*);
  static void PerformScriptCallApply(CodeGen*);

  // --------------------------------------------------------------------------
  // Jit Trace Link
  static void GenEnterJitTrace(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // Object Property/Element
  // --------------------------------------------------------------------------

  // This specialization of field get utilize a simple schema, which currently
  // does not use speculative method, ie via inline cache stub etc ... We will
  // add a inline cache system in the future to help boost the performance of
  // object field access, mainly because of our current method does not scale to
  // property store :(
  static void GenObjectFieldGetAtomFast(CodeGen*, const HInstruction*);
  static void GenObjectFieldSetAtomFast(CodeGen*, const HInstruction*);

  static void GenGlobalGetVarAtomFast(CodeGen*, const HInstruction*);
  static void GenGlobalGetGlobalAtomFast(CodeGen*, const HInstruction*);
  static void GenMaybeArrayLengthGet(CodeGen*, const HInstruction*);

  static void DoGenLoadArray(
      CodeGen* cg, const HInstruction* ins,
      int class_id,  // qjs class id, used to testify shape validity
      std::size_t byte_size, ArrayNativeType native_type);

  static void GenLoadArrayValue(CodeGen*, const HInstruction*);
  static void GenLoadArrayArguments(CodeGen*, const HInstruction*);
  static void GenLoadArrayInt8(CodeGen*, const HInstruction*);
  static void GenLoadArrayUInt8(CodeGen*, const HInstruction*);
  static void GenLoadArrayUInt8C(CodeGen*, const HInstruction*);
  static void GenLoadArrayInt16(CodeGen*, const HInstruction*);
  static void GenLoadArrayUInt16(CodeGen*, const HInstruction*);
  static void GenLoadArrayInt32(CodeGen*, const HInstruction*);
  static void GenLoadArrayUInt32(CodeGen*, const HInstruction*);
  static void GenLoadArrayF32(CodeGen*, const HInstruction*);
  static void GenLoadArrayF64(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // RC
  static void GenRc(CodeGen*, const HInstruction*);
  static void GenRcInc(CodeGen* cg, const HInstruction* ins) { GenRc(cg, ins); }
  static void GenRcDec(CodeGen* cg, const HInstruction* ins) { GenRc(cg, ins); }

  static void GenRcIncBox(CodeGen*, const HInstruction*);
  static void GenRcDecBox(CodeGen*, const HInstruction*);

 private:
  // --------------------------------------------------------------------------
  // Box/Unbox
  static void DoGetTTagPtr64(CodeGen*, const HOperand*, a::x86::Gp);
  static void DoGetTValPtr64(CodeGen*, const HOperand*, a::x86::Gp);

  static void DoGetTTag(CodeGen*, const HOperand*, a::x86::Gp);
  static void DoGetTVal(CodeGen*, const HOperand*, a::x86::Gp out);

  static int GetTTagFromOperand(const HOperand*);

  static void GenGUnbox32BitPtr64(CodeGen*, const HInstruction*, int);

  static void GenValueTagCmp(CodeGen* cg, const HOperand*, std::int32_t);
  static void GenGUnboxIPtr64(CodeGen*, const HInstruction*);
  static void GenGUnboxFPtr64(CodeGen*, const HInstruction*);
  static void GenGUnboxBPtr64(CodeGen*, const HInstruction*);
  static void GenGUnboxHeapPtr64(CodeGen*, const HInstruction*, std::int32_t);

  static void GenGUnboxI(CodeGen* cg, const HInstruction* ins) {
    GenGUnboxIPtr64(cg, ins);
  }
  static void GenGUnboxB(CodeGen* cg, const HInstruction* ins) {
    GenGUnboxBPtr64(cg, ins);
  }
  static void GenGUnboxF(CodeGen* cg, const HInstruction* ins) {
    GenGUnboxFPtr64(cg, ins);
  }

  static void GenGUnboxObj(CodeGen* cg, const HInstruction* ins) {
    GenGUnboxHeapPtr64(cg, ins, JVal::kObject);
  }

  static void GenGUnboxStr(CodeGen* cg, const HInstruction* ins) {
    GenGUnboxHeapPtr64(cg, ins, JVal::kString);
  }

  static void GenGUnboxMod(CodeGen* cg, const HInstruction* ins) {
    GenGUnboxHeapPtr64(cg, ins, JVal::kModule);
  }

  static void GenUnbox32BitPtr64(CodeGen*, const HInstruction*);

  static void GenUnboxPtrPtr64(CodeGen* cg, const HInstruction* ins);
  static void GenUnboxIPtr64(CodeGen* cg, const HInstruction* ins);
  static void GenUnboxFPtr64(CodeGen* cg, const HInstruction* ins);
  static void GenUnboxBPtr64(CodeGen* cg, const HInstruction* ins);

  static void GenUnboxI(CodeGen* cg, const HInstruction* ins) {
    GenUnboxIPtr64(cg, ins);
  }
  static void LOwerUnboxB(CodeGen* cg, const HInstruction* ins) {
    GenUnboxBPtr64(cg, ins);
  }
  static void GenUnboxF(CodeGen* cg, const HInstruction* ins) {
    GenUnboxFPtr64(cg, ins);
  }
  static void GenUnboxPtr(CodeGen* cg, const HInstruction* ins) {
    return GenUnboxPtrPtr64(cg, ins);
  }

  static void GenBoxPtr64(CodeGen*, const HInstruction*);
  static void GenBox(CodeGen* cg, const HInstruction* ins) {
    GenBoxPtr64(cg, ins);
  }
  static void GenGetTTag(CodeGen*, const HInstruction*);
  static void GenIsNullOrUndefined(CodeGen*, const HInstruction*);
  static void GenStrLen(CodeGen*, const HInstruction*);

  static void GenBoxTagIs(CodeGen*, const HInstruction*);
  static void GenBoxTagIsEq(CodeGen* cg, const HInstruction* ins) {
    GenBoxTagIs(cg, ins);
  }
  static void GenBoxTagIsNe(CodeGen* cg, const HInstruction* ins) {
    GenBoxTagIs(cg, ins);
  }

  // atom
  static void GenIsBoxAtomInt(CodeGen*, const HInstruction*);
  static void GenAtomFromI(CodeGen*, const HInstruction*);
  static void GenGAtom(CodeGen*, const HInstruction*);

  // misc type testify
  static void GenGObjectIsScriptFunction(CodeGen*, const HInstruction*);

 private:
  // deoptimization
  static DeoptId GetDeoptId(const HInstruction*);
  static void Deoptimize(CodeGen*, const HInstruction*);
  static void Return(CodeGen*);
  static void GenDeopt(CodeGen*, const HInstruction*);
  static void GenReturn(CodeGen*, const HInstruction*);

  static void GenDeoptIfTrueOrFalse(CodeGen*, const HInstruction*);
  static void GenDeoptIfTrue(CodeGen* cg, const HInstruction* ins) {
    GenDeoptIfTrueOrFalse(cg, ins);
  }
  static void GenDeoptIfFalse(CodeGen* cg, const HInstruction* ins) {
    GenDeoptIfTrueOrFalse(cg, ins);
  }

  static void GenDeoptCC(CodeGen*, const HInstruction*);
  static void GenDeoptZ(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptNz(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }

  static void GenDeoptGe(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptGt(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptLe(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptLt(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptEq(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptNe(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }

  static void GenDeoptAe(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptAt(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptBe(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
  }
  static void GenDeoptBt(CodeGen* cg, const HInstruction* ins) {
    GenDeoptCC(cg, ins);
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
      __ mov(u::ScrD(), u::Mem(val));
      return u::ScrD();
      break;

    case 2:
      __ mov(u::ScrW(), u::Mem(val));
      return u::ScrW();
      break;

    case 1:
      __ mov(u::ScrB(), u::Mem(val));
      return u::ScrB();
      break;

    case 8:
      __ mov(u::ScrQ(), u::Mem(val));
      return u::ScrQ();
      break;

    default:
      UNREACHABLE();
      return u::ScrQ();
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
    Mov(cg, to, u::Imm(from));
  } else if (from->IsRegister()) {
    CHECK(from->GetRegister()->IsGP());
    Mov(cg, to, u::Gp(from));
  } else if (from->IsMemory()) {
    Mov(cg, to, u::Mem(from), scratch);
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
    __ mov(u::Gp(to), imm);
  } else {
    __ mov(u::Mem(to), imm);
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to, a::x86::Gp reg) {
  CHECK(!to->IsFloat());
  CHECK(IsX64AllowedOutput(to));

  if (to->IsRegister()) {
    CHECK(to->GetRegister()->IsGP());
    __ mov(u::Gp(to), reg);
  } else {
    __ mov(u::Mem(to), reg);
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to, a::x86::Mem mem,
                             a::x86::Gp scratch) {
  CHECK(!to->IsFloat());
  CHECK(IsX64AllowedOutput(to));

  if (to->IsRegister()) {
    CHECK(to->GetRegister()->IsGP());
    __ mov(u::Gp(to), mem);
  } else {
    // mov scratch, [mem]
    __ mov(scratch, mem);

    // mov [mem], scratch
    __ mov(u::Mem(to), scratch);
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, a::x86::Gp to, const HOperand* from) {
  CHECK(!from->IsFloat());

  if (from->IsRegister()) {
    __ mov(to, u::Gp(from));
  } else if (from->IsMemory()) {
    __ mov(to, u::Mem(from));
  } else if (from->IsImm()) {
    __ mov(to, u::Imm(from));
  } else {
    UNREACHABLE();
  }
}

// ----------------------------------------------------------------------------
// Context
void TranslateRuleTable::GenContextLoad(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::Ctx());
  } else {
    __ mov(u::Mem(out), u::Ctx());
  }
}

void TranslateRuleTable::GenContextLoadJSContext(CodeGen* cg,
                                                 const HInstruction* ins) {
  auto out = ins->output();

  auto addr = a::x86::qword_ptr(u::Ctx(), Layout::Context::kContext);
  if (out->IsRegister()) {
    __ mov(u::Gp(out), addr);
  } else {
    __ mov(u::ScrQ(), addr);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenContextLoadGlobalObject(CodeGen* cg,
                                                    const HInstruction* ins) {
  auto out = ins->output();

  auto addr = a::x86::qword_ptr(u::Ctx(), Layout::Context::kContext);
  __ mov(u::ScrQ(), addr);

  auto gobj_rc_addr = a::x86::dword_ptr(
      u::ScrQ(), Layout::JSContext::kGlobalObject + Layout::Value::kValue);
  __ inc(gobj_rc_addr);

  if (out->IsRegister()) {
    __ lea(u::Gp(out),
           a::x86::qword_ptr(u::ScrQ(), Layout::JSContext::kGlobalObject));
  } else {
    __ mov(u::ScrQ(),
           a::x86::qword_ptr(u::ScrQ(), Layout::JSContext::kGlobalObject));
    __ mov(u::Mem(out), u::ScrQ());
  }
}

// ----------------------------------------------------------------------------
// This
void TranslateRuleTable::GenLoadThis(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto addr = a::x86::qword_ptr(
      u::Ctx(), Layout::Context::kThisObject + Layout::Value::kValue);

  if (out->IsRegister()) {
    __ lea(u::Gp(out), addr);
  } else {
    __ lea(u::ScrQ(), addr);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenLoadThisTag(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto rc_addr = a::x86::qword_ptr(
      u::Ctx(), Layout::Context::kThisObject + Layout::Value::kTag);

  if (out->IsRegister()) {
    __ mov(u::Gp(out), rc_addr);
  } else {
    __ mov(u::ScrQ(), rc_addr);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

// ----------------------------------------------------------------------------
// Frame
void TranslateRuleTable::GenEnsureTempSize(CodeGen* cg,
                                           const HInstruction* ins) {
  auto done = __ newLabel();
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kTempCur));
  __ sub(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kTemp));
  __ cmp(u::ScrD(), u::Imm(ins->Opr(0)));
  __ je(done);

  // FIXME(dpeng): DO NOT WRITE IT LIKE THIS
  __ mov(u::ScrD(), 0);  // first deoptimization point
  __ jmp(cg->code()->deopt_jump_entry().label());

  __ bind(done);
}

// rdx is been reserved as scratch register here
void TranslateRuleTable::GenEnterInlineFrame(CodeGen* cg,
                                             const HInstruction* ins) {
  const auto caller_depth = ins->At(0)->GetImm().Int32();
  const auto arg_size = ins->At(2)->GetImm().Int32();
  const auto loc_size = ins->At(3)->GetImm().Int32();
  const auto var_ref_size = ins->At(4)->GetImm().Int32();

  const auto func_obj = ins->At(5);  // boxed version
  const auto this_obj = ins->At(6);  // boxed version
  const auto tar_obj = ins->At(7);   // boxed version

  auto obj_ptr = __ StackTemp0();   // fill in the func_obj's object pointer
  auto temp_ptr = __ StackTemp1();  // scratch spill slot
  auto frame_addr = __ InlineFrame(caller_depth);  // get inline frame ptr

  auto frame = a::x86::rdx;  // notes, immutable during the inline frame setup

  auto frame_arg_buf = a::x86::qword_ptr(frame, Layout::Context::kParam);
  auto frame_var_buf = a::x86::qword_ptr(frame, Layout::Context::kLocal);
  auto frame_temp = a::x86::qword_ptr(frame, Layout::Context::kTemp);
  auto frame_temp_cur = a::x86::qword_ptr(frame, Layout::Context::kTempCur);
  auto frame_var_ref_list_next =
      a::x86::qword_ptr(frame, Layout::Context::kVarRefListNext);

  auto frame_var_ref_list_prev =
      a::x86::qword_ptr(frame, Layout::Context::kVarRefListPrev);
  auto frame_var_ref = a::x86::qword_ptr(frame, Layout::Context::kVarRef);
  auto frame_arg_count = a::x86::dword_ptr(frame, Layout::Context::kArgCount);
  auto frame_cur_func = a::x86::xmmword_ptr(frame, Layout::Context::kCurFunc);
  auto frame_js_mode = a::x86::dword_ptr(frame, Layout::Context::kJSMode);
  auto frame_js_context = a::x86::qword_ptr(frame, Layout::Context::kContext);
  auto frame_prev_frame = a::x86::qword_ptr(frame, Layout::Context::kPrevFrame);
  auto frame_quake = a::x86::qword_ptr(frame, Layout::Context::kQuake);
  auto frame_trace_level =
      a::x86::qword_ptr(frame, Layout::Context::kTraceLevel);
  auto frame_frame_type = a::x86::dword_ptr(frame, Layout::Context::kFrameType);

  auto frame_this_obj =
      a::x86::xmmword_ptr(frame, Layout::Context::kThisObject);
  auto frame_tar_obj =
      a::x86::xmmword_ptr(frame, Layout::Context::kTargetObject);

  auto move_value_to_mem = [&](const HOperand* opr, a::x86::Mem mem) {
    if (opr->IsRegister()) {
      __ movdqu(u::ScrX(), a::x86::xmmword_ptr(u::Gp(opr)));
    } else {
      __ mov(u::ScrQ(), u::Mem(opr));
      __ movdqu(u::ScrX(), a::x86::xmmword_ptr(u::ScrQ()));
    }
    __ movups(mem, u::ScrX());
  };

  __ lea(a::x86::rdx, frame_addr);

  // 0) setup the obj_ptr field
  {
    if (func_obj->IsRegister()) {
      __ mov(u::ScrQ(),
             a::x86::qword_ptr(u::Gp(func_obj), Layout::Value::kValue));
    } else {
      __ mov(u::ScrQ(), u::Mem(func_obj));
      __ mov(u::ScrQ(), a::x86::qword_ptr(u::ScrQ(), Layout::Value::kValue));
    }
    __ mov(obj_ptr, u::ScrQ());
  }

  // 1) allocate memory slot to fill up inline frame's local status
  //
  //    1.1 argument buffer
  //    1.2 variable buffer
  //    1.3 temporary buffer
  {
    auto slot = __ InlineFrameSlot(caller_depth);

    __ lea(u::ScrQ(), slot);

    // 1.1) set the arg_buf
    __ mov(frame_arg_buf, u::ScrQ());

    // 1.2) set the var_buf
    if (arg_size) __ add(u::ScrQ(), Layout::kCellSize * arg_size);
    __ mov(frame_var_buf, u::ScrQ());

    // 1.3) set the temp/temp_cur
    if (loc_size) __ add(u::ScrQ(), Layout::kCellSize * loc_size);
    __ mov(frame_temp, u::ScrQ());
    __ mov(frame_temp_cur, u::ScrQ());
  }

  // 2) setup the var_ref field
  {
    // 2.1) initialize the frame's var_ref_list
    __ lea(u::ScrQ(), a::x86::ptr(frame, Layout::Context::kVarRefList));
    __ mov(frame_var_ref_list_prev, u::ScrQ());
    __ mov(frame_var_ref_list_next, u::ScrQ());

    if (var_ref_size) {
      __ mov(u::ScrQ(), obj_ptr);
      __ mov(u::ScrQ(),
             a::x86::qword_ptr(u::ScrQ(), Layout::Object::kU_Func_VarRefs));

      __ mov(frame_var_ref, u::ScrQ());
    }
  }

  // 3) misc field of inline frame setup
  //   3.1) arg_count
  //   3.2) cur_func
  //   3.3) js_mode
  //   3.4) js_context (bytecode's relm)
  //   3.5) quake field
  //   3.6) trace level(used to correctly handle nested loop)
  //   3.7) frame type
  {
    // setup the arg_count
    __ mov(frame_arg_count, arg_size);

    // setup the cur_func
    move_value_to_mem(func_obj, frame_cur_func);

    // setup the js_mode and js_context
    {
      __ mov(u::ScrQ(), obj_ptr);
      __ mov(u::ScrQ(),
             a::x86::qword_ptr(u::ScrQ(),
                               Layout::Object::kU_Func_FunctionBytecode));
      __ mov(temp_ptr, u::ScrQ());

      // js_mode
      __ movzx(u::ScrD(),
               a::x86::byte_ptr(u::ScrQ(), Layout::FunctionBytecode::kJSMode));
      __ mov(frame_js_mode, u::ScrD());

      // js_context
      __ mov(u::ScrQ(), temp_ptr);
      __ mov(u::ScrQ(),
             a::x86::qword_ptr(u::ScrQ(), Layout::FunctionBytecode::kRealm));
      __ mov(frame_js_context, u::ScrQ());
    }

    __ movabs(u::ScrQ(), base::TypePuneCast<std::int64_t>(cg->quake()));
    __ mov(frame_quake, u::ScrQ());

    {
      __ mov(u::ScrD(),
             a::x86::dword_ptr(u::Ctx(), Layout::Context::kTraceLevel));
      __ mov(frame_trace_level, u::ScrD());
    }

    __ mov(frame_frame_type, QJS_JIT_FRAME_INLINE);
  }

  // 4) prev and current_stack_frame setup
  {
    __ mov(frame_prev_frame, u::Ctx());

    __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kContext));
    __ mov(u::ScrQ(), a::x86::qword_ptr(u::ScrQ(), Layout::JSContext::kRT));
    __ mov(a::x86::qword_ptr(u::ScrQ(), Layout::JSRuntime::kCurrentStackFrame),
           frame);
  }

  // 5) setup the this_obj/target_obj field
  {
    move_value_to_mem(this_obj, frame_this_obj);
    move_value_to_mem(tar_obj, frame_tar_obj);
  }

  // 6) lastly, modify r15 to be the newly created inline frame, here, to
  //    restore it just need to use the field |prev_frame|
  __ mov(u::Ctx(), frame);
}

void TranslateRuleTable::GenLeaveInlineFrame(CodeGen* cg, const HInstruction*) {
  // correctly restoring the frame
  __ mov(u::Ctx(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kPrevFrame));

  // restore JSRuntime's current_stack_frame field
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kContext));
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::ScrQ(), Layout::JSContext::kRT));
  __ mov(a::x86::qword_ptr(u::ScrQ(), Layout::JSRuntime::kCurrentStackFrame),
         u::Ctx());
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
  const auto& imm = ins->Opr(0)->GetImm();
  auto index = imm.Int32();  // this can be negative when do temp load

  // We do not need to copy the value from u::Ctx() into the local cell
  // region, but just need to hold a pointer points back to where the u::Ctx()
  // store the boxed value
  __ mov(u::ScrQ(),
         a::x86::qword_ptr(u::Ctx(), offset));  // pointer to XXX array

  // address of the final result
  auto addr = a::x86::ptr(u::ScrQ(), index * kSize);

  if (out->IsRegister()) {
    __ lea(u::Gp(out), addr);
  } else if (out->IsMemory()) {
    __ lea(u::ScrQ(), addr);
    __ mov(u::Mem(out), u::ScrQ());
  } else {
    UNREACHABLE();
  }
}

void TranslateRuleTable::GenLoadLocal(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadLocal);
  DoLoadBoxValueFromScratchPtr64(cg, ins, Layout::Context::kLocal);
}

void TranslateRuleTable::GenLoadParam(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadParam);
  DoLoadBoxValueFromScratchPtr64(cg, ins, Layout::Context::kParam);
}

// Loading temporary value is different from Local and Param since it loads
// from the temp_cur position and its offset can be positive and negative
void TranslateRuleTable::GenLoadTemp(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadTemp);
  DoLoadBoxValueFromScratchPtr64(cg, ins, Layout::Context::kTempCur);
}

void TranslateRuleTable::DoBoxTo(CodeGen* cg, const HInstruction* ins,
                                 int offset) {
  auto opr_value = ins->Opr(0);
  auto opr_tag = ins->Opr(1);
  auto opr_idx = ins->Opr(2);

  CHECK(opr_tag->IsImm());
  CHECK(opr_idx->IsImm());

  auto ttag = opr_tag->GetImm().Int32();
  auto idx = opr_idx->GetImm().Int32();

  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), offset));

  auto cell_address = a::x86::ptr(u::ScrQ(), idx * Layout::kCellSize);

  if (opr_value->IsRegister()) {
    if (auto r = opr_value->GetRegister(); r->IsFP()) {
      __ store_cell(cell_address, u::Fp(opr_value), ttag);
    } else {
      __ store_cell(cell_address, u::Gp(opr_value), ttag);
    }
  } else if (opr_value->IsMemory()) {
    // FIXME(dpeng):
    //   We can use xmm15 as scratch to simplify the process, currently this
    //   code is slow and so stupid, since it uses a memory as temporay value

    // We need to use scratch to hold the temporary value so the address
    // cannot be represented by the scratch register here.
    auto temp_cell = __ StackTempXmm0();
    auto local = __ StackTemp7();  // avoiding clubber with xmm temp
    __ mov(local, u::ScrQ());      // save scratch, ie the target cell

    // store the value and tag to the xmm temporary and then move it to the
    // cell address. By doing this, we can avoid using SCRATCH register
    {
      auto val_addr = temp_cell.cloneAdjusted(Layout::Value::kValue);
      val_addr.setSize(8);

      __ mov(u::ScrQ(), u::Mem(opr_value));
      __ mov(val_addr, u::ScrQ());
    }

    {
      auto tag_addr = temp_cell.cloneAdjusted(Layout::Value::kTag);
      tag_addr.setSize(8);

      __ mov(u::ScrD(), ttag);          // move the tag
      __ movsxd(u::ScrQ(), u::ScrD());  // sign extending tag to 64bits
      __ mov(tag_addr, u::ScrQ());      // move the tag back to the tag position
    }

    __ mov(u::ScrQ(), local);  // restore scratch
    __ movdqu(u::ScrX(), temp_cell);
    __ movups(cell_address, u::ScrX());
  } else {
    const auto& value = opr_value->GetImm();  // immediate value

    auto tag_addr = cell_address.cloneAdjusted(Layout::Value::kTag);
    auto val_addr = cell_address.cloneAdjusted(Layout::Value::kValue);
    tag_addr.setSize(4);

    switch (value.type()) {
      case Immediate::kInt32:
        val_addr.setSize(4);
        __ mov(tag_addr, ttag);
        __ mov(val_addr, value.Int32());
        break;

      case Immediate::kFloat64: {
        auto fvalue = value.Float64();
        __ mov(tag_addr, ttag);
        __ movabs(u::ScrQ(), *reinterpret_cast<const std::int64_t*>(&fvalue));

        val_addr.setSize(8);
        __ mov(val_addr, u::ScrQ());
      } break;

      default:
        UNREACHABLE();
        break;
    }
  }
}

void TranslateRuleTable::DoStore(CodeGen* cg, int offset, int idx,
                                 const HOperand* input) {
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), offset));
  auto addr = a::x86::xmmword_ptr(u::ScrQ(), idx * Layout::kCellSize);

  // basically just move input, which is a 16 bytes value to the target 16
  // bytes value position specified by output.

  if (input->IsRegister()) {
    auto from = u::Gp(input);
    __ movdqu(u::ScrX(), a::x86::xmmword_ptr(from));
    __ movups(addr, u::ScrX());
  } else {
    auto from = u::Mem(input);

    // spill scratch on stack since we need it to hold the from's result
    auto spill = __ StackTemp0();
    __ mov(spill, u::ScrQ());

    // let scratch hold the result of from, since this memory address holds
    // a pointer to a 128bits value
    __ mov(u::ScrQ(), from);

    // move the value to xmm register
    __ movdqu(u::ScrX(), a::x86::xmmword_ptr(u::ScrQ()));

    // restore the GPQ
    __ mov(u::ScrQ(), spill);

    // move xmm register's 16 bits value back to u::ScrQ() pointed address
    __ movups(addr, u::ScrX());
  }
}

void TranslateRuleTable::DoStore(CodeGen* cg, const HInstruction* ins,
                                 int offset) {
  auto input = ins->Opr(0);
  auto opr_idx = ins->Opr(1);
  CHECK(opr_idx->IsImm());
  auto idx = opr_idx->GetImm().Int32();
  DoStore(cg, offset, idx, input);
}

void TranslateRuleTable::GenLeaTemp(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto opr_offset = ins->Opr(0);
  CHECK(opr_offset->IsImm());  // must be immediate for now

  auto offset = opr_offset->GetImm().Int32();
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kTempCur));

  auto addr = a::x86::ptr(u::ScrQ(), offset * Layout::kCellSize);
  if (out->IsRegister()) {
    __ lea(u::Gp(out), addr);
  } else {
    __ lea(u::ScrQ(), addr);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenLoadNull(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto addr = cg->code()->const_box_null_xmmword();

  if (out->IsRegister()) {
    __ lea(u::Gp(out), addr);
  } else {
    __ lea(u::ScrQ(), addr);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenLoadUndefined(CodeGen* cg,
                                          const HInstruction* ins) {
  auto out = ins->output();
  auto addr = cg->code()->const_box_undefined_xmmword();

  if (out->IsRegister()) {
    __ lea(u::Gp(out), addr);
  } else {
    __ lea(u::ScrQ(), addr);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenLoadUninitialized(CodeGen* cg,
                                              const HInstruction* ins) {
  auto out = ins->output();
  auto addr = cg->code()->const_box_undefined_xmmword();

  if (out->IsRegister()) {
    __ lea(u::Gp(out), addr);
  } else {
    __ lea(u::ScrQ(), addr);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenLoadStringUnbox(CodeGen* cg,
                                            const HInstruction* ins) {
  auto quake = cg->quake();
  auto out = ins->output();
  auto str =
      static_cast<const zone::String*>(ins->Opr(0)->GetImm().AsPointer());
  auto string_lit = quake->AddConstPoolString(*str);

  __ movabs(u::ScrQ(), base::TypePuneCast<std::int64_t>(string_lit));

  // bump reference counting, otherwise failed
  auto refcount_addr = a::x86::dword_ptr(u::ScrQ(), Layout::Value::kValue);
  __ inc(refcount_addr);

  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::ScrQ());
  } else {
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenLoadVarRef(CodeGen* cg, const HInstruction* ins) {
  auto index = ins->Opr(0)->GetImm().Int32();
  auto out = ins->output();

  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kVarRef));
  __ mov(u::ScrQ(), a::x86::ptr(u::ScrQ(), Layout::kPtrSize * index));
  __ mov(u::ScrQ(), a::x86::ptr(u::ScrQ(), Layout::VarRef::kPValue));

  // Notes, the VarRef operation do not require bump rc, since it will be
  // emitted by the IR builder

  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::ScrQ());
  } else {
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenStoreVarRef(CodeGen* cg, const HInstruction* ins) {
  auto index = ins->Opr(0)->GetImm().Int32();
  auto value = ins->Opr(1);

  // the u::ScrQ() stores the pointer to the var_ref's JSValue
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kVarRef));
  __ mov(u::ScrQ(), a::x86::ptr(u::ScrQ(), Layout::kPtrSize * index));
  __ mov(u::ScrQ(), a::x86::ptr(u::ScrQ(), Layout::VarRef::kPValue));

  auto addr = a::x86::ptr(u::ScrQ());

  if (value->IsRegister()) {
    auto from = u::Gp(value);
    __ movdqu(u::ScrX(), a::x86::xmmword_ptr(from));
    __ movups(addr, u::ScrX());
  } else {
    auto from = u::Mem(value);

    // spill scratch on stack since we need it to hold the from's result
    auto spill = __ StackTemp0();
    __ mov(spill, u::ScrQ());

    // let scratch hold the result of from, since this memory address holds
    // a pointer to a 128bits value
    __ mov(u::ScrQ(), from);

    // move the value to xmm register
    __ movdqu(u::ScrX(), a::x86::xmmword_ptr(u::ScrQ()));

    // restore the GPQ
    __ mov(u::ScrQ(), spill);

    // move xmm register's 16 bits value back to u::ScrQ() pointed address
    __ movups(addr, u::ScrX());
  }
}

void TranslateRuleTable::GenMoveMM(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kMov);
  CHECK(ins->opr_size() == 1);
  CHECK(ins->HasOutput());

  auto opr0 = ins->output();
  auto opr1 = ins->Opr(0);

  CHECK(opr0->IsMemory());
  CHECK(opr1->IsMemory());

  auto asmjit_opr0 = u::Mem(opr0);
  auto asmjit_opr1 = u::Mem(opr1);

  CHECK(opr0->size_class() == opr1->size_class());

  switch (opr0->size_class()) {
    case SizeClass::kByte:
      __ mov(u::ScrB(), asmjit_opr1);
      __ mov(asmjit_opr1, u::ScrB());
      break;

    case SizeClass::kWord:
      __ mov(u::ScrW(), asmjit_opr1);
      __ mov(asmjit_opr0, u::ScrW());
      break;

    case SizeClass::kDWord:
      __ mov(u::ScrD(), asmjit_opr1);
      __ mov(asmjit_opr0, u::ScrD());
      break;

    case SizeClass::kQWord:
      __ mov(u::ScrQ(), asmjit_opr1);
      __ mov(asmjit_opr0, u::ScrQ());
      break;

    default:
      UNREACHABLE();
      break;
  }
}

void TranslateRuleTable::GenMoveFMM(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->output();
  auto opr1 = ins->Opr(0);

  CHECK(ins->opr_size() == 1);
  CHECK(ins->HasOutput());
  CHECK(opr0->IsMemory());
  CHECK(opr1->IsMemory());
  CHECK(opr0->IsFloat()) << ins->Print();
  CHECK(opr1->IsFloat());
  CHECK(opr0->size_class() == opr1->size_class());
  CHECK(opr0->IsQWord());

  auto asmjit_opr0 = u::Mem(opr0);
  auto asmjit_opr1 = u::Mem(opr1);

  __ movsd(u::ScrX(), asmjit_opr0);
  __ movsd(asmjit_opr1, u::ScrX());
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
    auto o = u::Gp(opr0);
    __ mov(o, a::x86::rax);
  } else if (opr0->IsMemory()) {
    auto o = u::Mem(opr0);
    __ mov(o, a::x86::rax);
  } else {
    UNREACHABLE();
  }
}

void TranslateRuleTable::GenMoveI64(CodeGen* cg, const HInstruction* ins) {
  return DoMoveB64(cg, ins);
}

void TranslateRuleTable::GenMoveB64(CodeGen* cg, const HInstruction* ins) {
  return DoMoveB64(cg, ins);
}

void TranslateRuleTable::GenMoveF64(CodeGen* cg, const HInstruction* ins) {
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
    auto xmm = u::Fp(opr0);
    __ movq(xmm, value_mem);
  } else if (opr0->IsMemory()) {
    auto mem = u::Mem(opr0);
    __ movq(u::ScrX(), value_mem);
    __ movq(mem, u::ScrX());
  } else {
    UNREACHABLE();
  }
}

// ----------------------------------------------------------------------------
// unary operation lowering
void TranslateRuleTable::GenIncDecF(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);

  auto data = cg->code()->const_f64_one();  // get float64 1's label
  auto opr0 = ins->Opr(0);                  // operand 0
  auto out = ins->output();                 // output

  auto is_inc = ins->opcode() == HOpcode::kIncF;

  if (opr0->IsRegister()) {
    auto opr0_reg = u::Fp(opr0);

    if (out->IsRegister()) {
      auto out_reg = u::Fp(out);

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
      __ movq(u::ScrX(), opr0_reg);

      if (is_inc) {
        __ addsd(u::ScrX(), data);
      } else {
        __ subsd(u::ScrX(), data);
      }

      __ movq(u::Mem(out), u::ScrX());
    }
  } else {
    CHECK(opr0->IsMemStack());
    auto opr0_mem = u::Mem(opr0);

    if (out->IsRegister()) {
      auto out_reg = u::Fp(out);
      __ movq(out_reg, opr0_mem);
      if (is_inc) {
        __ addsd(out_reg, data);
      } else {
        __ subsd(out_reg, data);
      }
    } else {
      CHECK(out->IsMemStack());
      __ movq(u::ScrX(), opr0_mem);

      if (is_inc) {
        __ addsd(u::ScrX(), data);
      } else {
        __ subsd(u::ScrX(), data);
      }

      __ movq(u::Mem(out), u::ScrX());
    }
  }
}

void TranslateRuleTable::GenGIncDec(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->HasDeoptId());
  CHECK(ins->opr_size() == 1);

  auto opr0 = ins->Opr(0);   // operand 0
  auto out = ins->output();  // output
  auto is_inc = ins->opcode() == HOpcode::kGInc;

  if (opr0->IsRegister()) {
    auto opr0_reg = u::Gp(opr0);
    if (out->IsRegister()) {
      auto out_r = u::Gp(out);
      if (out->GetRegister() != opr0->GetRegister()) {
        __ mov(out_r, opr0_reg);
      }
      if (is_inc) {
        __ inc(out_r);
      } else {
        __ dec(out_r);
      }
    } else {
      CHECK(out->IsMemory());
      __ mov(u::ScrD(), opr0_reg);
      if (is_inc) {
        __ inc(u::ScrD());
      } else {
        __ dec(u::ScrD());
      }
      __ mov(u::Mem(out), u::ScrD());
    }

  } else {
    auto opr0_mem = u::Mem(opr0);

    if (out->IsRegister()) {
      auto out_r = u::Gp(out);
      __ mov(out_r, opr0_mem);
      if (is_inc) {
        __ inc(out_r);
      } else {
        __ dec(out_r);
      }
    } else {
      __ mov(u::ScrD(), opr0_mem);
      if (is_inc) {
        __ inc(u::ScrD());
      } else {
        __ dec(u::ScrD());
      }
      __ mov(u::Mem(out), u::ScrD());
    }
  }

  auto done = __ newLabel();

  __ jno(done);

  Deoptimize(cg, ins);

  __ bind(done);
}

void TranslateRuleTable::GenNegF(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);

  auto opr0 = ins->Opr(0);
  auto out = ins->output();
  auto neg_zero = cg->code()->const_f64_neg_zero_xmmword();

  if (opr0->IsRegister()) {
    auto opr0_reg = u::Fp(opr0);

    if (out->IsRegister()) {
      if (out->GetRegister() == opr0->GetRegister()) {
        __ xorps(u::Fp(out), neg_zero);
      } else {
        __ movq(u::Fp(out), opr0_reg);
        __ xorps(u::Fp(out), neg_zero);
      }
    } else {
      __ movq(u::ScrX(), opr0_reg);
      __ xorps(u::ScrX(), neg_zero);
      __ movq(u::Mem(out), u::ScrX());
    }
  } else {
    auto opr0_mem = u::Mem(opr0);
    if (out->IsRegister()) {
      auto out_reg = u::Fp(out);
      __ movq(out_reg, opr0_mem);
      __ xorps(out_reg, neg_zero);
    } else {
      __ movq(u::ScrX(), opr0_mem);
      __ xorps(u::ScrX(), neg_zero);
      __ movq(u::Mem(out), u::ScrX());
    }
  }
}

// guarded negate of int32, notes if int32 is max positive value, then negate
// it will cause overflow. Notes, the neg instruction will set O flag
void TranslateRuleTable::GenGNeg(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);
  CHECK(ins->HasDeoptId());

  auto opr0 = ins->Opr(0);
  auto out = ins->output();

  if (out->IsRegister()) {
    auto opr0_reg = u::Gp(opr0);
    __ neg(opr0_reg);
    if (out->IsRegister()) {
      if (out->GetRegister() != opr0->GetRegister()) {
        __ mov(u::Gp(out), opr0_reg);
      }
    } else {
      __ mov(u::Mem(out), opr0_reg);
    }
  } else {
    auto opr0_mem = u::Mem(opr0);
    if (out->IsRegister()) {
      auto out_reg = u::Gp(out);
      __ mov(out_reg, opr0_mem);
      __ inc(out_reg);
    } else {
      __ mov(u::ScrD(), opr0_mem);
      __ inc(u::ScrD());
      __ mov(u::Mem(out), u::ScrD());
    }
  }

  // check whether overflow, otherwise deoptimize
  auto label = __ newLabel();
  __ jno(label);
  Deoptimize(cg, ins);
  __ bind(label);
}

void TranslateRuleTable::GenInv(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto out = ins->output();

  if (opr0->IsRegister()) {
    __ mov(u::ScrD(), u::Gp(opr0));
  } else {
    __ mov(u::ScrD(), u::Mem(opr0));
  }

  __ not_(u::ScrD());

  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::ScrD());
  } else {
    __ mov(u::Mem(out), u::ScrD());
  }
}

// ----------------------------------------------------------------------------
// integer comparison conditional value
void TranslateRuleTable::GenIntConditionValue(CodeGen* cg,
                                              const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();

  CHECK(opr0->IsRegister());  // guaranteed by RA

  auto opr0_reg = u::Gp(opr0);

  auto is_eq_operator = [](HOpcode opc) -> bool {
    return opc == HOpcode::kIEq || opc == HOpcode::kI64Eq ||
           opc == HOpcode::kINe || opc == HOpcode::kI64Ne;
  };

  // generate test/cmp sequences
  switch (ins->opcode()) {
    case HOpcode::kILt:
    case HOpcode::kILe:
    case HOpcode::kIGe:
    case HOpcode::kIGt:

    case HOpcode::kI64Eq:
    case HOpcode::kI64Ne:
    case HOpcode::kIEq:
    case HOpcode::kINe:
      if (opr1->IsImm()) {
        const auto& imm = opr1->GetImm();
        if (is_eq_operator(ins->opcode()) && imm.Int32() == 0) {
          __ test(opr0_reg, opr0_reg);  // if target is 0, then just use test
        } else {
          __ cmp(opr0_reg, u::Imm(opr1));
        }
      } else if (opr1->IsRegister()) {
        __ cmp(opr0_reg, u::Gp(opr1));
      } else {
        CHECK(opr1->IsMemory());
        __ cmp(opr0_reg, u::Mem(opr1));
      }
      break;

    default:
      UNREACHABLE();
      break;
  }

  // clean up the target if needed. notes the cleanup code should before the
  // cmp since it may impact flag. Notes, do not use xor which will invalidate
  // the flag register and also we cannot forward the cleanup before the value
  // comparison since this will fail the case when input and output operand
  // clubbers
  if (out->IsRegister()) {
    auto r = u::Gp(out);
    __ mov(r, 0);
  } else {
    __ mov(u::ScrD(), 0);
  }

  // now let's use setcc instruction to perform the operation
  switch (ins->opcode()) {
    case HOpcode::kILt:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ setl(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setl(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kILe:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ setle(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setle(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kIGt:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ setg(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setg(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kIGe:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ setge(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setge(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kIEq:
    case HOpcode::kI64Eq:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ sete(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ sete(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kINe:
    case HOpcode::kI64Ne:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ setne(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setne(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    default:
      UNREACHABLE();
      break;
  }
}

void TranslateRuleTable::GenFloatConditionValue(CodeGen* cg,
                                                const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();

  CHECK(opr0->IsRegister());  // guaranteed by RA

  auto opr0_reg = u::Fp(opr0);

  // clean up the target if needed. notes the cleanup code should before the
  // cmp since it may impact flag
  if (out->IsRegister()) {
    auto r = u::Gp(out);
    __ xor_(r, r);
  } else {
    __ xor_(u::ScrD(), u::ScrD());
  }

  switch (ins->opcode()) {
    case HOpcode::kFLt:
    case HOpcode::kFLe:
    case HOpcode::kFGe:
    case HOpcode::kFGt:
      /**
       * Notes, we reorder the operand for < and <=  to allow them to use seta
       * setnb which does not need to check for parity to detect unordered
       * result
       */
      if (opr1->IsRegister()) {
        if (ins->opcode() == HOpcode::kFLt || ins->opcode() == HOpcode::kFLe) {
          __ comisd(u::Fp(opr1), opr0_reg);
        } else {
          __ comisd(opr0_reg, u::Fp(opr1));
        }
      } else {
        CHECK(opr1->IsMemory());
        if (ins->opcode() == HOpcode::kFLt || ins->opcode() == HOpcode::kFLe) {
          __ movq(u::ScrX(), u::Mem(opr1));
          __ comisd(u::ScrX(), opr0_reg);
        } else {
          __ comisd(opr0_reg, u::Mem(opr1));
        }
      }
      break;

    case HOpcode::kFEq:
    case HOpcode::kFNe:
      if (opr1->IsRegister()) {
        __ ucomisd(opr0_reg, u::Fp(opr1));
      } else {
        CHECK(opr1->IsMemory());
        __ ucomisd(opr0_reg, u::Mem(opr1));
      }
      break;

    default:
      UNREACHABLE();
      break;
  }

  // comisd can reorder its both operand which will allow us to utilize the
  // special flag settings for unordered result to avoid explicitly check
  // for NaN cases

  // Now let's use setcc instruction to perform the operation
  switch (ins->opcode()) {
    case HOpcode::kFLt:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ seta(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ seta(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kFLe:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ setnb(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setnb(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kFGt:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ seta(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ seta(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kFGe:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ setnb(out_r.r8Lo());  // lower 8 bit part of the register
      } else {
        __ setnb(u::ScrB());
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    // ------------------------------------------------------------------------
    // The eq/ne should take care of the special case of parity flag, ie the
    // NaN cases, otherwise it will be invalid result
    case HOpcode::kFEq:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ mov(u::ScrD(), 0);    // set GPD to be 0, notes you cannot use xor
                                 // since it reset the EFLAGS register
        __ setnp(out_r.r8Lo());  // set if not unordered
        __ cmovne(out_r, u::ScrD());  // set 0 if not equal
      } else {
        auto zero = cg->code()->const_i32_zero();
        __ setnp(u::ScrB());
        __ cmovne(u::ScrD(), zero);
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    case HOpcode::kFNe:
      if (out->IsRegister()) {
        auto out_r = u::Gp(out);
        __ mov(u::ScrD(), 1);
        __ setp(out_r);
        __ cmovne(out_r, u::ScrD());
      } else {
        auto one = cg->code()->const_i32_one();
        __ setp(u::ScrB());
        __ cmovne(u::ScrD(), one);
        __ mov(u::Mem(out), u::ScrD());
      }
      break;

    default:
      UNREACHABLE();
      break;
  }
}

void TranslateRuleTable::GenCmpBoxIs(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);
  auto opr0 = ins->Opr(0);  // must be a kBoxRef, ie pointer to JSValue

  DoGetTTag(cg, opr0, u::ScrD());

  // Notes, we just do a simple test comparison, since we will translate
  // kJmpBoxIsXXX to corresponding jlt (<0, heap) or jge (>=0, primitive)
  __ cmp(u::ScrD(), JVal::kHeap);
}

void TranslateRuleTable::GenCmpBoxTagIs(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 2);
  auto value = ins->Opr(0);
  auto tag = ins->Opr(1);

  a::x86::Mem tag_addr;

  if (value->IsRegister()) {
    tag_addr = a::x86::dword_ptr(u::Gp(value), Layout::Value::kTag);
  } else {
    CHECK(value->IsMemory());
    __ mov(u::ScrQ(), u::Mem(value));
    tag_addr = a::x86::dword_ptr(u::ScrQ(), Layout::Value::kTag);
  }

  if (tag->IsImm()) {
    __ cmp(tag_addr, tag->GetImm().Int32());
  } else if (tag->IsRegister()) {
    __ cmp(tag_addr, u::Gp(tag));
  } else {
    __ mov(u::ScrD(), tag_addr);
    __ cmp(u::Mem(tag), u::ScrD());
  }
}

// ----------------------------------------------------------------------------
void TranslateRuleTable::GenVarRefListIs(CodeGen* cg, const HInstruction* ins) {
  auto temp = __ StackTemp2(4);
  auto out = ins->output();
  auto is_empty = ins->opcode() == HOpcode::kVarRefListIsEmpty;
  a::x86::Gp out_reg;

  // compare the ref list
  GenCmpVarRefListIsEmpty(cg, ins);

  if (out->IsRegister()) {
    out_reg = u::Gp(out);
  } else {
    out_reg = u::ScrQ();
  }

  {
    __ mov(temp, 1);
    __ mov(out_reg, 0);
    if (is_empty) {
      __ cmove(out_reg, temp);
    } else {
      __ cmovne(out_reg, temp);
    }
  }

  if (!out->IsRegister()) __ mov(u::Mem(out), out_reg);
}

void TranslateRuleTable::GenVarRefListIsEmpty(CodeGen* cg,
                                              const HInstruction* ins) {
  GenVarRefListIs(cg, ins);
}

void TranslateRuleTable::GenVarRefListIsNotEmpty(CodeGen* cg,
                                                 const HInstruction* ins) {
  GenVarRefListIs(cg, ins);
}

void TranslateRuleTable::GenCmpVarRefListIsEmpty(CodeGen* cg,
                                                 const HInstruction* ins) {
  auto next = a::x86::qword_ptr(u::Ctx(), Layout::Context::kVarRefListNext);
  auto list = a::x86::qword_ptr(u::Ctx(), Layout::Context::kVarRefList);
  auto next_value = __ StackTemp1();

  __ mov(u::ScrQ(), next);
  __ mov(next_value, u::ScrQ());
  __ lea(u::ScrQ(), list);
  __ cmp(u::ScrQ(), next_value);
}

// ----------------------------------------------------------------------------
void TranslateRuleTable::GenFArithXxx(CodeGen* cg, const HInstruction* ins,
                                      HOpcode op) {
  auto x0 = ins->output();
  auto x1 = ins->Opr(0);
  auto x2 = ins->Opr(1);

  a::x86::Xmm lhs, rhs;

  // Format [X0][x1][x2]

  // 1. if X0 != x2
  if (x2->GetRegister() != x0->GetRegister()) {
    if (x1->GetRegister() != x0->GetRegister()) {
      __ movq(u::Fp(x0), u::Fp(x1));
    }
    lhs = u::Fp(x0);
    rhs = u::Fp(x2);
  } else {
    // for operation that can be commuted, ie add/mul, we can just reduce this
    // to one single instruction otherwise it is rather complicated
    if (op == HOpcode::kFAdd || op == HOpcode::kFMul) {
      lhs = u::Fp(x2);
      rhs = u::Fp(x1);
    } else {
      // ----------------------------------------------------------------------
      // Shortcut here, ie use scratch to perform the operation, otherwise it
      // will not work
      //
      // movq scratchX, x1
      // xxxx scratchX, x2
      // movq x0, scratchX
      // ----------------------------------------------------------------------
      __ movq(u::ScrX(), u::Fp(x1));

      switch (op) {
        case HOpcode::kFSub:
          __ subsd(u::ScrX(), u::Fp(x2));
          break;

        case HOpcode::kFDiv:
          __ divsd(u::ScrX(), u::Fp(x2));
          break;

        default:
          UNREACHABLE();
          break;
      }

      __ movq(u::Fp(x0), u::ScrX());
      return;
    }
  }

  switch (op) {
    case HOpcode::kFAdd:
      __ addsd(lhs, rhs);
      break;
    case HOpcode::kFSub:
      __ subsd(lhs, rhs);
      break;
    case HOpcode::kFMul:
      __ mulsd(lhs, rhs);
      break;
    case HOpcode::kFDiv:
      __ divsd(lhs, rhs);
      break;
    default:
      UNREACHABLE();
      break;
  }
}

// ----------------------------------------------------------------------------
// arithmetic operation lowering
//
// opr0 [memory/register]
// opr1 [memory/register/imm]
// out  [memory/register]
void TranslateRuleTable::GenGuardIntArithmetic(CodeGen* cg,
                                               const HInstruction* ins) {
  CHECK(ins->HasDeoptId());

  // perform an add operation
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();

  if (opr0->IsMemory()) {
    __ mov(u::ScrD(), u::Mem(opr0));
  } else {
    CHECK(opr0->IsRegister());
    __ mov(u::ScrD(), u::Gp(opr0));
  }

  switch (ins->opcode()) {
    case HOpcode::kGIAdd:
      if (opr1->IsImm()) {
        __ add(u::ScrD(), u::Imm(opr1));
      } else if (opr1->IsMemStack()) {
        __ add(u::ScrD(), u::MemStack(opr1));
      } else {
        CHECK(opr1->IsRegister());
        __ add(u::ScrD(), u::Gp(opr1));
      }
      break;

    case HOpcode::kGISub:
      if (opr1->IsImm()) {
        __ sub(u::ScrD(), u::Imm(opr1));
      } else if (opr1->IsMemStack()) {
        __ sub(u::ScrD(), u::MemStack(opr1));
      } else {
        CHECK(opr1->IsRegister());
        __ sub(u::ScrD(), u::Gp(opr1));
      }
      break;

    case HOpcode::kGIMul:
      if (opr1->IsImm()) {
        __ imul(u::ScrD(), u::Imm(opr1));
      } else if (opr1->IsMemStack()) {
        __ imul(u::ScrD(), u::MemStack(opr1));
      } else {
        CHECK(opr1->IsRegister());
        __ imul(u::ScrD(), u::Gp(opr1));
      }
      break;

    default:
      UNREACHABLE();
      break;
  }

  // 3. perform overflow detection, if overflow, then just perform
  // deoptimzation
  auto label = __ newLabel();

  // jump when not overflow, ie skipping the deoptimization code
  __ jno(label);

  Deoptimize(cg, ins);

  // normal flow, ie recover from deoptimization
  __ bind(label);

  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::ScrD());
  } else {
    __ mov(u::Mem(out), u::ScrD());
  }
}

void TranslateRuleTable::GenGShift(CodeGen* cg, const HInstruction* ins,
                                   bool is_shift_left,
                                   std::function<void(a::x86::Gp)>&& callback) {
  auto out = ins->output();
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);

  CHECK(ins->HasDeoptId());

  if (opr1->IsRegister()) {
    __ mov(a::x86::ecx, u::Gp(opr1));
  } else {
    __ mov(a::x86::ecx, u::Mem(opr1));
  }

  a::x86::Gp dst_reg;

  if (out->IsRegister()) {
    dst_reg = u::Gp(out);
  } else {
    dst_reg = u::ScrD();
  }

  if (opr0->IsRegister()) {
    __ mov(u::Gp(out), u::Gp(opr0));
  } else {
    __ mov(u::Gp(out), u::Mem(opr0));
  }

  // perform the shl operations, notes we are in 32 bits mode
  if (is_shift_left) {
    __ shl(dst_reg, a::x86::ecx);
  } else {
    __ shr(dst_reg, a::x86::ecx);
  }

  if (out->IsMemory()) {
    __ mov(u::Mem(out), dst_reg);
  }

  callback(dst_reg);
}

void TranslateRuleTable::GenGShiftConst(
    CodeGen* cg, const HInstruction* ins, bool is_shift_left,
    std::function<void(a::x86::Gp)>&& callback_reg,
    std::function<void(a::x86::Mem)>&& callback_mem) {
  auto out = ins->output();
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1)->GetImm().Int32();

  if (out->IsRegister()) {
    auto out_r = u::Gp(out);
    if (opr0->IsRegister()) {
      __ mov(out_r, u::Gp(opr0));
    } else {
      __ mov(out_r, u::Mem(opr0));
    }

    if (is_shift_left) {
      __ shl(out_r, opr1);
    } else {
      __ shr(out_r, opr1);
    }

    callback_reg(out_r);
  } else {
    auto out_mem = u::Mem(out);

    if (opr0->IsRegister()) {
      __ mov(out_mem, u::Gp(opr0));
      if (is_shift_left) {
        __ shl(out_mem, opr1);
      } else {
        __ shr(out_mem, opr1);
      }
    } else {
      __ mov(u::ScrD(), u::Mem(opr0));
      if (is_shift_left) {
        __ shl(u::ScrD(), opr1);
      } else {
        __ shr(u::ScrD(), opr1);
      }
      __ mov(out_mem, u::ScrD());  // this does not impact flag
    }

    callback_mem(out_mem);
  }
}

void TranslateRuleTable::GenGShl(CodeGen* cg, const HInstruction* ins) {
  GenGShift(cg, ins, true, [&](a::x86::Gp dst) {
    // now check whether we are overflow or not
    auto label = __ newLabel();

    // If not overflow, skip the deoptimization
    __ jno(label);

    Deoptimize(cg, ins);

    __ bind(label);
  });
}

void TranslateRuleTable::GenGShlConst(CodeGen* cg, const HInstruction* ins) {
  GenGShiftConst(
      cg, ins, true,
      [&](a::x86::Gp) {
        auto label = __ newLabel();
        __ jno(label);
        Deoptimize(cg, ins);
        __ bind(label);
      },
      [&](a::x86::Mem) {
        auto label = __ newLabel();
        __ jno(label);
        Deoptimize(cg, ins);
        __ bind(label);
      });
}

void TranslateRuleTable::GenGShr(CodeGen* cg, const HInstruction* ins) {
  GenGShift(cg, ins, false, [&](a::x86::Gp dst) {
    auto label = __ newLabel();
    __ cmp(dst, std::numeric_limits<std::int32_t>::max());
    __ jbe(label);
    Deoptimize(cg, ins);
    __ bind(label);
  });
}

void TranslateRuleTable::GenGShrConst(CodeGen* cg, const HInstruction* ins) {
  GenGShiftConst(
      cg, ins, true,
      [&](a::x86::Gp dst) {
        auto label = __ newLabel();
        __ cmp(dst, std::numeric_limits<std::int32_t>::max());
        __ jbe(label);
        Deoptimize(cg, ins);
        __ bind(label);
      },
      [&](a::x86::Mem dst) {
        auto label = __ newLabel();
        __ cmp(dst, std::numeric_limits<std::int32_t>::max());
        __ jbe(label);
        Deoptimize(cg, ins);
        __ bind(label);
      });
}

// Fmod will be special runtime call calls into stub code generated by cg,
// similar as cast etc ..
//
// param(0) -> rsp
// param(1) -> rsp + 8
// result -> xmm15
//
void TranslateRuleTable::GenFMod(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto out = ins->output();

  auto stk0 = __ StackTemp0();
  auto stk1 = __ StackTemp1();

  if (opr0->IsRegister()) {
    __ movsd(stk0, u::Fp(opr0));
  } else {
    __ movq(u::ScrX(), u::Mem(opr0));
    __ movsd(stk0, u::ScrX());
  }

  if (opr1->IsRegister()) {
    __ movsd(stk1, u::Fp(opr1));
  } else {
    __ movsd(u::ScrX(), u::Mem(opr1));
    __ movsd(stk1, u::ScrX());
  }

  {
    auto opr1 = stk0;
    auto opr2 = stk1;

    __ fld(opr2);
    __ fld(opr1);

    auto mod_loop = __ newLabel();
    __ bind(mod_loop);

    // If we support sahf instruction, then we can save the following.
    // Refactory it until we have cpu feature flag setup
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
                                 // bank conflicts, ie using al/ah is not
                                 // ideal most of the times
    }
    __ jne(mod_loop);
    __ fstp(a::x86::st(1));

    if (out->IsRegister()) {
      __ fstp(opr1);
      __ movsd(u::Fp(out), opr1);
    } else {
      __ fstp(u::Mem(out));
    }
  }
}

// Notes, CastF64ToB32 will force RA to reserve 1 register for us to be used
// here, and we have a rbp to be used here.
// Since F64ToB32 is essentially a small function to be inlined inside of the
// generated bytecode, we will use a special call and return instruction to
// do the trick. We will let GenCastF64ToB32 calls into a special label and
// then return from it. The call is not a normal ABI call, but a special call
void TranslateRuleTable::GenCastF64ToB32(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto out = ins->output();

  // Notes(dpeng): The stack has to be 16 bytes aligned during usage of the SSE
  // instructions, in our case we are gonna use it. Additionally, we will use
  // rbp as a our scratch registers.
  //
  // In this routine, we will have 3 scratch registers,
  //   1) RAX
  //   2) RBP
  //   3) RCX (guaranteed by the RA)
  auto temp_slot = a::x86::qword_ptr(a::x86::rsp, 0);

  __ mov(temp_slot, a::x86::rbp);  // push rbp to be used as scratch register

  // put the operand (F64) into scratch register and start to
  if (opr0->IsRegister()) {
    __ movq(u::ScrX(), u::Fp(opr0));
  } else {
    __ movq(u::ScrX(), u::Mem(opr0));
  }

  __ call(cg->code()->cast_f64_to_b32().label());

  // the result is been put into u::ScrD()
  if (out->IsRegister()) {
    auto out_reg = u::Gp(out);
    __ mov(out_reg, u::ScrD());
  } else {
    auto out_mem = u::Mem(out);
    __ mov(out_mem, u::ScrD());
  }

  __ mov(a::x86::rbp, temp_slot);  // recover rbp
}

void TranslateRuleTable::GenCastBToStr(CodeGen* cg, const HInstruction* ins) {
  auto cond = ins->Opr(0);
  auto out = ins->output();

  a::x86::Gp outr{u::ScrQ()};
  if (out->IsRegister()) {
    outr = u::Gp(out);
  }

  if (cond->IsRegister()) {
    auto r = u::Gp(cond);
    __ test(r, r);
  } else {
    __ mov(u::ScrD(), u::Mem(cond));
    __ test(u::ScrD(), u::ScrD());
  }

  __ mov(outr, cg->code()->const_str_false());
  __ cmovnz(outr, cg->code()->const_str_true());

  // Bump the reference counting, since it is a heap allocated stuff.
  //
  // FIXME(dpeng): We have one option here to optimize all the const string's
  // RC away, we need another type in our backend and ignore all the RC, just
  // treat it like a I64. We can use I64 type as such use case, thus all the
  // RC are gone.
  //
  // The string is constant and will be held by the Quake, and it will never
  // be freeed.
  auto refcount_addr = a::x86::dword_ptr(outr, Layout::Value::kValue);
  __ inc(refcount_addr);

  // lastly, generate the output
  if (!out->IsRegister()) {
    __ mov(u::Mem(out), outr);
  }
}

void TranslateRuleTable::GenCastF64ToB(CodeGen* cg, const HInstruction* ins) {
  auto cond = ins->Opr(0);
  auto out = ins->output();
  a::x86::Xmm input_r;

  auto generate_output = [&](bool value) {
    if (out->IsMemory()) {
      __ mov(u::Mem(out), static_cast<int>(value));
    } else {
      __ mov(u::Gp(out), static_cast<int>(value));
    }
  };

  if (cond->IsRegister()) {
    input_r = u::Fp(cond);
  } else {
    __ movq(u::ScrX(), u::Mem(cond));
    input_r = u::ScrX();
  }

  auto false_branch = __ newLabel();
  auto done_branch = __ newLabel();

  __ ucomisd(input_r, input_r);
  __ jp(false_branch);

  auto f64_zero = cg->code()->AddConst(0.0);
  __ ucomisd(input_r, f64_zero);
  __ je(false_branch);

  // true output
  generate_output(true);
  __ jmp(done_branch);

  // false output
  __ bind(false_branch);
  generate_output(false);

  __ bind(done_branch);
}

void TranslateRuleTable::GenCastVToB(CodeGen* cg, const HInstruction* ins) {
  auto cond = ins->Opr(0);
  auto out = ins->output();

  if (cond->IsRegister()) {
    __ mov(u::ScrQ(), u::Gp(cond));
  } else {
    __ mov(u::ScrQ(), u::Mem(cond));
  }

  __ call(cg->code()->any_to_bool().label());

  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::ScrD());
  } else {
    __ mov(u::Mem(out), u::ScrD());
  }
}

void TranslateRuleTable::GenCastObjToB(CodeGen* cg, const HInstruction* ins) {
  auto cond = ins->Opr(0);
  auto out = ins->output();

  if (cond->IsRegister()) {
    __ mov(u::ScrQ(), u::Gp(cond));
  } else {
    __ mov(u::ScrQ(), u::Mem(cond));
  }

  __ movzx(u::ScrD(), a::x86::byte_ptr(u::ScrQ(), 5));
  __ not_(u::ScrD());
  __ shr(u::ScrB(), 7);
  __ movzx(u::ScrD(), u::ScrB());

  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::ScrD());
  } else {
    __ mov(u::Mem(out), u::ScrD());
  }
}

void TranslateRuleTable::GenCastNBToB(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto out = ins->output();

  if (opr0->IsRegister()) {
    __ mov(u::ScrD(), u::Gp(opr0));
  } else {
    __ mov(u::ScrD(), u::Mem(opr0));
  }

  if (out->IsRegister()) {
    auto r = u::Gp(out);
    __ xor_(r, r);  // impact flag register, must be placed before test
  }

  __ test(u::ScrD(), u::ScrD());

  if (out->IsRegister()) {
    auto r = u::Gp(out);
    __ setz(r);
  } else {
    // have to perform the branch otherwise require an extra scratch register
    auto zero_branch = __ newLabel();
    auto done_branch = __ newLabel();
    auto out_mem = u::Mem(out);

    __ jz(zero_branch);  // jump to branch that

    __ mov(out_mem, 0);  // the input is 1, invert is 0
    __ jmp(done_branch);

    __ bind(zero_branch);
    __ mov(out_mem, 1);  // the input is 0, invert is 1

    __ bind(done_branch);
  }
}

void TranslateRuleTable::GenCastStrToB(CodeGen* cg, const HInstruction* ins) {
  static const std::uint32_t kMask = (static_cast<std::uint32_t>(1) << 31) - 1;

  auto cond = ins->Opr(0);
  auto out = ins->output();

  if (cond->IsRegister()) {
    __ mov(u::ScrQ(), u::Gp(cond));
  } else {
    __ mov(u::ScrQ(), u::Mem(cond));
  }

  auto false_branch = __ newLabel();
  auto done = __ newLabel();

  // FIXME(dpeng): Use CMovCC instruction set, the issue is we do not have
  //   enough scratch here and we can reserve one extra scratch but that may
  //   not yield better performance though
  __ mov(u::ScrD(), a::x86::dword_ptr(u::ScrQ(), Layout::String::kLength));
  __ and_(u::ScrD(), kMask);
  __ test(u::ScrD(), u::ScrD());  // testing whether length is zero
  __ jz(false_branch);            // length is zero

  __ mov(u::ScrD(), 1);
  __ jmp(done);

  __ bind(false_branch);
  __ mov(u::ScrD(), 0);

  __ bind(done);

  if (out->IsRegister()) {
    __ mov(u::Gp(out), u::ScrD());
  } else {
    __ mov(u::Mem(out), u::ScrD());
  }
}

std::uint64_t TranslateRuleTable::GetRCallAddress(const HInstruction* ins) {
  auto call_address = ins->Opr(0);
  CHECK(call_address->IsMemStatic());
  return reinterpret_cast<std::uint64_t>(
      call_address->GetMemStatic().address());
}

void TranslateRuleTable::GenRCallGP(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto call_mem = GetRCallAddress(ins);

  // notes rdi is always reserved
  __ mov(a::x86::rdi, u::Ctx());
  __ call(call_mem);

  // okay, get the return value from rax and put it back to the output
  if (out->size_class() == SizeClass::kQWord) {
    if (out->IsRegister()) {
      __ mov(u::Gp(out), a::x86::rax);
    } else {
      __ mov(u::Mem(out), a::x86::rax);
    }
  } else {
    CHECK(out->size_class() == SizeClass::kDWord);
    if (out->IsRegister()) {
      __ mov(u::Gp(out), a::x86::eax);
    } else {
      __ mov(u::Mem(out), a::x86::eax);
    }
  }
}

void TranslateRuleTable::GenRCallFP(CodeGen* cg, const HInstruction* ins) {
  auto call_mem = GetRCallAddress(ins);
  CHECK(ins->output()->IsRegister());
  CHECK(ins->output()->GetRegister() ==
        Register::GetRegisterFromKind(RegisterKind::kXmm0));

  // notes rdi is always reserved
  __ mov(a::x86::rdi, u::Ctx());
  __ call(call_mem);

  if (auto out = ins->output(); out->IsRegister()) {
    if (out->GetRegister()->register_kind() != RegisterKind::kXmm0) {
      __ movq(u::Fp(out), a::x86::xmm0);
    }
  } else {
    __ movq(u::Mem(out), a::x86::xmm0);
  }
}

void TranslateRuleTable::GenRCallVoid(CodeGen* cg, const HInstruction* ins) {
  auto call_mem = GetRCallAddress(ins);

  // notes rdi is always reserved
  __ mov(a::x86::rdi, u::Ctx());
  __ call(call_mem);
}

// rax:rdx forms the JSValue, the rax mapps to lower address and rdx mapps to
// higher address.
void TranslateRuleTable::GenRCallBox(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto call_mem = GetRCallAddress(ins);

  __ mov(a::x86::rdi, u::Ctx());
  __ call(call_mem);

  auto lower = __ CellAddressLower(ins->cell_index());
  auto higher = __ CellAddressHigher(ins->cell_index());
  auto cell = __ CellAddress(ins->cell_index());

  __ mov(lower, a::x86::rax);
  __ mov(higher, a::x86::rdx);

  if (out->IsRegister()) {
    __ lea(u::Gp(out), cell);
  } else {
    __ lea(u::ScrQ(), cell);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

// ============================================================================
// JSCall lowering

void TranslateRuleTable::PrepareScriptCallFunctionObject(CodeGen* cg,
                                                         const HOperand* func) {
  auto addr = __ ScriptCallFunctionObjectAddress();

  if (func->IsRegister()) {
    __ mov(addr, u::Gp(func));
  } else {
    __ mov(u::ScrQ(), u::Mem(func));
    __ mov(addr, u::ScrQ());
  }
}

void TranslateRuleTable::PrepareScriptCallThisObject(CodeGen* cg,
                                                     const HOperand* tobj) {
  auto addr = __ ScriptCallThisObjectAddress();
  if (tobj->IsRegister()) {
    __ mov(addr, u::Gp(tobj));
  } else {
    __ mov(u::ScrQ(), u::Mem(tobj));
    __ mov(addr, u::ScrQ());
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
    __ mov(addr, u::Gp(tobj));
  } else {
    __ mov(u::ScrQ(), u::Mem(tobj));
    __ mov(addr, u::ScrQ());
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
  __ lea(u::ScrQ(), cell_address);
  __ mov(addr, u::ScrQ());
}

void TranslateRuleTable::PrepareScriptCallParameterStart(CodeGen* cg,
                                                         const HOperand* opr) {
  auto addr = __ ScriptCallParameterStartAddress();

  if (opr->IsRegister()) {
    __ mov(addr, u::Gp(opr));
  } else {
    __ mov(u::ScrQ(), u::Mem(opr));
    __ mov(addr, u::ScrQ());
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
    __ lea(u::Gp(output), addr);
  } else {
    __ lea(u::ScrQ(), addr);
    __ mov(u::Mem(output), u::ScrQ());
  }
}

template <typename T>
void TranslateRuleTable::DoScriptCall(CodeGen* cg, T func_addr) {
  auto addr = __ ScriptCallAddress();
  __ mov(asmjit::x86::rdi, u::Ctx());
  __ lea(asmjit::x86::rsi, addr);
  __ call(FADDR(func_addr));
}

void TranslateRuleTable::PerformScriptCall(CodeGen* cg) {
  DoScriptCall(cg, &QuakeJit::JSCall);
}

void TranslateRuleTable::PerformScriptCallCtor(CodeGen* cg) {
  DoScriptCall(cg, &QuakeJit::JSCallCtor);
}

void TranslateRuleTable::PerformScriptCallArrayFrom(CodeGen* cg) {
  DoScriptCall(cg, &QuakeJit::JSCallArrayFrom);
}

void TranslateRuleTable::PerformScriptCallApply(CodeGen* cg) {
  DoScriptCall(cg, &QuakeJit::JSCallApply);
}

// JSCallF input parameter
//
// 1) function object in BoxRef format
// 2) immediate number, parameter size of function
// 3) parameter starting address, parameter is passed via VM stack
void TranslateRuleTable::GenJSCallF(CodeGen* cg, const HInstruction* ins) {
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
void TranslateRuleTable::GenJSCallFT(CodeGen* cg, const HInstruction* ins) {
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
void TranslateRuleTable::GenJSCallFN(CodeGen* cg, const HInstruction* ins) {
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
void TranslateRuleTable::GenJSCallFTN(CodeGen* cg, const HInstruction* ins) {
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

void TranslateRuleTable::GenJSCallCtor(CodeGen* cg, const HInstruction* ins) {
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

void TranslateRuleTable::GenArrayFrom(CodeGen* cg, const HInstruction* ins) {
  PrepareScriptCallReturnAddress(cg, ins);
  PrepareScriptCallParameterSize(cg, ins->Opr(0));
  PrepareScriptCallParameterStart(cg, ins->Opr(1));

  PerformScriptCallArrayFrom(cg);
  MaterializeScriptCallOutput(cg, ins);
}

void TranslateRuleTable::GenApply(CodeGen* cg, const HInstruction* ins) {
  PrepareScriptCallReturnAddress(cg, ins);
  PrepareScriptCallThisObject(cg, ins->Opr(0));
  PrepareScriptCallParameterStart(cg, ins->Opr(1));
  PrepareScriptCallOption(cg, ins->Opr(2));

  PerformScriptCallApply(cg);
  MaterializeScriptCallOutput(cg, ins);
}

void TranslateRuleTable::GenEnterJitTrace(CodeGen* cg,
                                          const HInstruction* ins) {
  auto offset = ins->Opr(1)->GetImm().Int32();
  auto deopt_id1 = ins->Opr(2);
  auto deopt_id2 = ins->Opr(3);

  auto did_addr = a::x86::qword_ptr(u::Ctx(), Layout::Context::kLinkDeoptId);
  auto did1_addr = a::x86::dword_ptr(u::Ctx(), Layout::Context::kLinkDeoptId1);
  auto did2_addr = a::x86::dword_ptr(u::Ctx(), Layout::Context::kLinkDeoptId2);

  auto quake_addr = a::x86::qword_ptr(u::Ctx(), Layout::Context::kQuake);
  auto temp_cur_addr = a::x86::qword_ptr(u::Ctx(), Layout::Context::kTempCur);

  auto trace = static_cast<Quake*>(ins->Opr(4)->GetImm().AsPointer());
  auto entry = trace->code().trace_entry();  // function start address

  // save it into the slot
  __ push(did_addr);

  // store the new deoptimization id into the context to let the inner loop to
  // perform deoptimization site selection correctly
  __ mov(did1_addr, deopt_id1->GetImm().UInt32());
  __ mov(did2_addr, deopt_id2->GetImm().UInt32());

  // store the new trace's value into
  __ push(quake_addr);

  __ movabs(u::ScrQ(), FADDR(trace));
  __ mov(quake_addr, u::ScrQ());

  // store the old temp_cur and mutate temp_cur to new one to simulate the VM
  // stack

  if (offset) {
    __ push(temp_cur_addr);
    __ add(temp_cur_addr, Layout::kCellSize * offset);
    __ push(u::ScrQ());
  }

  // calls into the nested jitted code
  __ call(FADDR(entry));

  // test the inner jitted code is bailout from expected side trace or not
  // if not, directly yield to deoptimization code
  auto done = __ newLabel();

  auto restore_ctx = [&]() {
    if (offset) {
      __ pop(u::ScrQ());
      __ pop(temp_cur_addr);
    }

    __ pop(quake_addr);
    __ pop(did_addr);
  };

  __ cmp(a::x86::eax, deopt_id1->GetImm().UInt32());
  __ je(done);

  __ cmp(a::x86::eax, deopt_id2->GetImm().UInt32());
  __ je(done);

  {
    // make sure the recover the RSP register
    __ add(a::x86::rsp, 8 * 4);

    // mov an invalide deoptimization id inside of the rax to indicate any
    // upper jit trace that we just need to directly bailout, nothing needs
    // to be performed. The inner most jit trace already populate the recovery
    // frame correctly, nothing needs to be done now
    __ mov(a::x86::eax, kInvalidDeoptId);

    // since everything is already deoptimized, then just perform a
    // unconditional return, that's it, notes the deoptimization id
    // is still stored inside of the rax
    Return(cg);
  }

  __ bind(done);

  restore_ctx();
}

void TranslateRuleTable::GenRc(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto is_inc = ins->opcode() == HOpcode::kRcInc;

  a::x86::Mem refcount_addr;
  a::x86::Gp base;

  if (opr0->IsRegister()) {
    base = u::Gp(opr0);
  } else {
    base = u::ScrQ();

    __ mov(u::ScrQ(), u::Mem(opr0));
  }

  refcount_addr = a::x86::dword_ptr(base, Layout::Value::kValue);

  if (is_inc) {
    __ add(refcount_addr, 1);  // increase rc
  } else {
    __ sub(refcount_addr, 1);  // decrease rc
  }
}

void TranslateRuleTable::GenRcIncBox(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto done = __ newLabel();

  a::x86::Gp ptr;

  if (opr0->IsRegister()) {
    ptr = u::Gp(opr0);
  } else {
    ptr = u::ScrQ();

    __ mov(u::ScrQ(), u::Mem(opr0));
  }

  auto tag_addr = a::x86::dword_ptr(ptr, Layout::Value::kTag);
  auto value_addr = a::x86::qword_ptr(ptr, Layout::Value::kValue);

  __ cmp(tag_addr, JVal::kHeap);
  __ jge(done);

  __ mov(u::ScrQ(), value_addr);
  __ add(a::x86::dword_ptr(u::ScrQ()), 1);

  __ bind(done);
}

void TranslateRuleTable::GenRcDecBox(CodeGen* cg, const HInstruction* ins) {
  auto opr0 = ins->Opr(0);
  auto done = __ newLabel();

  a::x86::Gp ptr;

  if (opr0->IsRegister()) {
    ptr = u::Gp(opr0);
  } else {
    ptr = u::ScrQ();

    __ mov(u::ScrQ(), u::Mem(opr0));
  }

  auto tag_addr = a::x86::dword_ptr(ptr, Layout::Value::kTag);
  auto value_addr = a::x86::qword_ptr(ptr, Layout::Value::kValue);

  __ cmp(tag_addr, JVal::kHeap);
  __ jge(done);

  __ mov(u::ScrQ(), value_addr);
  __ sub(a::x86::dword_ptr(u::ScrQ()), 1);

  __ bind(done);
}

void TranslateRuleTable::DoGetTTagPtr64(CodeGen* cg, const HOperand* target,
                                        a::x86::Gp out) {
  if (target->IsMemory()) {
    __ mov(u::ScrQ(), u::Mem(target));
    auto addr = a::x86::dword_ptr(u::ScrQ(), Layout::Value::kTag);
    __ mov(out, addr);
  } else {
    auto base = u::Gp(target);
    auto ptr = a::x86::dword_ptr(base, Layout::Value::kTag);
    __ mov(out, ptr);
  }
}

void TranslateRuleTable::DoGetTValPtr64(CodeGen* cg, const HOperand* target,
                                        a::x86::Gp out) {
  if (target->IsMemory()) {
    __ mov(u::ScrQ(), u::Mem(target));
    auto addr = a::x86::qword_ptr(u::ScrQ(), Layout::Value::kValue);
    __ mov(out, addr);
  } else {
    auto base = u::Gp(target);
    auto ptr = a::x86::qword_ptr(base, Layout::Value::kValue);
    __ mov(out, ptr);
  }
}

void TranslateRuleTable::DoGetTTag(CodeGen* cg, const HOperand* opr,
                                   a::x86::Gp out) {
  DoGetTTagPtr64(cg, opr, out);
}

void TranslateRuleTable::DoGetTVal(CodeGen* cg, const HOperand* opr,
                                   a::x86::Gp out) {
  DoGetTValPtr64(cg, opr, out);
}

void TranslateRuleTable::GenBoxTagIs(CodeGen* cg, const HInstruction* ins) {
  const auto is_eq = ins->opcode() == HOpcode::kBoxTagIsEq;

  auto opr = ins->Opr(0);
  auto tag = ins->Opr(1);
  auto out = ins->output();

  a::x86::Mem tag_addr;

  if (opr->IsRegister()) {
    tag_addr = a::x86::dword_ptr(u::Gp(opr), Layout::Value::kTag);
  } else {
    CHECK(opr->IsMemory());
    __ mov(u::ScrQ(), u::Mem(opr));
    tag_addr = a::x86::dword_ptr(u::ScrQ(), Layout::Value::kTag);
  }

  if (tag->IsImm()) {
    __ cmp(tag_addr, tag->GetImm().Int32());
  } else if (tag->IsRegister()) {
    __ cmp(tag_addr, u::Gp(tag));
  } else {
    CHECK(tag->IsMemory());
    __ mov(u::ScrD(), tag_addr);
    __ cmp(u::Mem(tag), u::ScrD());
  }

  if (out->IsRegister()) {
    __ mov(u::Gp(out), 0);
    if (is_eq) {
      __ sete(u::Gp(out));
    } else {
      __ setne(u::Gp(out));
    }
  } else {
    __ mov(u::ScrQ(), 0);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenGetTTag(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kGetTTag);
  CHECK(ins->IsUnary());
  CHECK(ins->HasOutput());

  auto opr1 = ins->Opr(0);
  auto output = ins->output();

  if (output->IsMemory()) {
    DoGetTTag(cg, opr1, u::ScrD());  // eax contains the tag
    __ mov(u::Mem(output), u::ScrD());
  } else if (output->IsRegister()) {
    DoGetTTag(cg, opr1, u::Gp(output));
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

void TranslateRuleTable::GenStrLen(CodeGen* cg, const HInstruction* ins) {
  static const std::uint32_t kMask = (static_cast<std::uint32_t>(1) << 31) - 1;

  auto opr = ins->Opr(0);
  auto out = ins->output();

  a::x86::Gp length_field;

  if (opr->IsRegister()) {
    auto addr = a::x86::dword_ptr(u::Gp(opr), Layout::String::kLength);

    if (out->IsRegister()) {
      auto out_reg = u::Gp(out);
      __ mov(out_reg, addr);
      __ and_(out_reg, kMask);
    } else {
      __ mov(u::ScrD(), addr);
      __ and_(u::ScrD(), kMask);
      __ mov(u::Mem(out), u::ScrD());
    }

  } else {
    auto addr = a::x86::dword_ptr(u::ScrQ(), Layout::String::kLength);
    __ mov(u::ScrQ(), u::Mem(opr));

    if (out->IsRegister()) {
      auto out_r = u::Gp(out);
      __ mov(out_r, addr);
      __ and_(out_r, kMask);
    } else {
      __ mov(u::ScrD(), addr);
      __ and_(u::ScrD(), kMask);
      __ mov(u::Mem(out), u::ScrD());
    }
  }
}

void TranslateRuleTable::GenIsNullOrUndefined(CodeGen* cg,
                                              const HInstruction* ins) {
  static const int kNullTag = JS_TAG_NULL;
  static const int kUndefinedTag = JS_TAG_UNDEFINED;

  auto opr = ins->Opr(0);
  auto out = ins->output();

  if (opr->IsRegister()) {
    __ mov(u::ScrD(), u::Gp(opr));
  } else {
    __ mov(u::ScrD(), u::Mem(opr));
  }

  if (kNullTag == 2 && kUndefinedTag == 3) {
    // clang's optimizing version
    if (out->IsRegister()) {
      auto out_reg = u::Gp(out);

      __ and_(u::ScrD(), a::Imm(-2));
      __ xor_(out_reg, out_reg);
      __ cmp(u::ScrD(), 2);
      __ sete(out_reg.r8Lo());
    } else {
      auto out_mem = u::Mem(out);

      __ and_(u::ScrD(), a::Imm(-2));
      __ mov(out_mem, 0);
      __ cmp(u::ScrD(), 2);

      // notes out_mem should be in byte size of 4, we need to convert it into
      // byte
      out_mem.setSize(1);
      __ sete(out_mem);
    }
  } else {
    auto pos = __ newLabel();
    auto done = __ newLabel();

    // basic version, unoptimized, have branch etc ...
    __ cmp(u::ScrD(), a::Imm(kNullTag));
    __ je(pos);

    __ cmp(u::ScrD(), a::Imm(kUndefinedTag));
    __ je(pos);

    if (out->IsRegister()) {
      auto out_reg = u::Gp(out);
      __ xor_(out_reg, out_reg);
    } else {
      auto out_mem = u::Mem(out);
      __ mov(out_mem, 0);
    }

    __ jmp(done);

    // positive branch
    __ bind(pos);
    {
      if (out->IsRegister()) {
        auto out_reg = u::Gp(out);
        __ mov(out_reg, 1);
      } else {
        auto out_mem = u::Mem(out);
        __ mov(out_mem, 1);
      }
    }

    // done branch
    __ bind(done);
  }
}

void TranslateRuleTable::GenBoxPtr64(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();

  auto val = ins->Opr(0);
  auto ty = ins->Opr(1);

  CHECK(ty->IsImm());
  auto type_tag = ty->GetImm().Int32();
  auto cell_index = ins->cell_index();

  // Store the value into the cell slot
  if (val->IsRegister()) {
    if (auto r = val->GetRegister(); r->IsFP()) {
      __ store_cell(cell_index, u::Fp(val), type_tag);
    } else {
      __ store_cell(cell_index, u::Gp(val), type_tag);
    }
  } else if (val->IsImm()) {
    // allowing value to be just immedaite number
    const auto& imm = val->GetImm();
    switch (imm.type()) {
      case Immediate::kInt32:
        __ mov(u::ScrD(), imm.Int32());
        __ store_cell(cell_index, u::ScrD(), type_tag);
        break;

      case Immediate::kInt64:
        __ movabs(u::ScrQ(), imm.Int64());
        __ store_cell(cell_index, u::ScrQ(), type_tag);
        break;

      case Immediate::kFloat64:
        __ mov(u::ScrQ(), cg->code()->AddConst(imm.Float64()));
        __ store_cell(cell_index, u::ScrQ(), type_tag);
        break;

      default:
        UNREACHABLE();
        break;
    }

  } else {
    CHECK(val->IsMemory());
    auto out = MoveMemoryToScratch(cg, val);
    __ store_cell(cell_index, out, type_tag);
  }

  // Return the cell slot address into our output position
  auto cell_address = __ CellAddress(cell_index);
  if (out->IsRegister()) {
    __ lea(u::Gp(out), cell_address);
  } else {
    __ lea(u::ScrQ(), cell_address);
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenUnboxPtrPtr64(CodeGen* cg,
                                          const HInstruction* ins) {
  auto out = ins->output();

  if (out->IsRegister()) {
    DoGetTVal(cg, ins->Opr(0), u::Gp(out));
  } else {
    DoGetTVal(cg, ins->Opr(0), u::ScrQ());
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenValueTagCmp(CodeGen* cg, const HOperand* opr,
                                        std::int32_t tag) {
  if (opr->IsMemory()) {
    __ mov(u::ScrQ(), u::Mem(opr));
    __ cmp(a::x86::dword_ptr(u::ScrQ(), Layout::Value::kTag), tag);
  } else {
    __ cmp(a::x86::dword_ptr(u::Gp(opr), Layout::Value::kTag), tag);
  }
}

void TranslateRuleTable::GenGUnboxHeapPtr64(CodeGen* cg,
                                            const HInstruction* ins,
                                            std::int32_t tag) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);
  auto done = __ newLabel();

  GenValueTagCmp(cg, op0, tag);
  __ je(done);

  Deoptimize(cg, ins);

  __ bind(done);

  // load the value into scratch register

  if (out->IsMemory()) {
    DoGetTVal(cg, op0, u::ScrQ());
    __ mov(u::Mem(out), u::ScrQ());
  } else {
    DoGetTVal(cg, op0, u::Gp(out));
  }
}

void TranslateRuleTable::GenGUnbox32BitPtr64(CodeGen* cg,
                                             const HInstruction* ins,
                                             int jval_type) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);
  auto done = __ newLabel();

  GenValueTagCmp(cg, op0, jval_type);
  __ je(done);

  Deoptimize(cg, ins);

  __ bind(done);

  if (out->IsMemory()) {
    DoGetTVal(cg, op0, u::ScrQ());
    __ mov(u::Mem(out), u::ScrD());
  } else {
    DoGetTVal(cg, op0, u::Gp(out));
  }
}

void TranslateRuleTable::GenGUnboxIPtr64(CodeGen* cg, const HInstruction* ins) {
  GenGUnbox32BitPtr64(cg, ins, JVal::kInt);
}

void TranslateRuleTable::GenGUnboxBPtr64(CodeGen* cg, const HInstruction* ins) {
  GenGUnbox32BitPtr64(cg, ins, JVal::kBool);
}

void TranslateRuleTable::GenGUnboxFPtr64(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);
  auto done = __ newLabel();

  GenValueTagCmp(cg, op0, JVal::kFloat64);
  __ je(done);

  Deoptimize(cg, ins);

  __ bind(done);

  if (op0->IsMemory()) {
    __ mov(u::ScrQ(), u::Mem(op0));
    auto addr = a::x86::qword_ptr(u::ScrQ(), Layout::Value::kValue);

    if (out->IsMemory()) {
      __ mov(u::ScrQ(), addr);
      __ mov(u::Mem(out), u::ScrQ());
    } else {
      __ movq(u::Fp(out), addr);
    }
  } else {
    auto addr = a::x86::qword_ptr(u::Gp(op0), Layout::Value::kValue);
    if (out->IsMemory()) {
      __ mov(u::ScrQ(), addr);
      __ movq(u::Fp(out), u::ScrQ());
    } else {
      __ movq(u::Fp(out), addr);
    }
  }
}

void TranslateRuleTable::GenUnbox32BitPtr64(CodeGen* cg,
                                            const HInstruction* ins) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);

  if (out->IsMemory()) {
    DoGetTVal(cg, op0, u::ScrQ());
    __ mov(u::Mem(out), u::ScrD());
  } else {
    DoGetTVal(cg, op0, u::Gp(out));
    __ mov(u::Gp(out), u::ScrD());
  }
}

void TranslateRuleTable::GenUnboxIPtr64(CodeGen* cg, const HInstruction* ins) {
  GenUnbox32BitPtr64(cg, ins);
}

void TranslateRuleTable::GenUnboxBPtr64(CodeGen* cg, const HInstruction* ins) {
  GenUnbox32BitPtr64(cg, ins);
}

void TranslateRuleTable::GenUnboxFPtr64(CodeGen* cg, const HInstruction* ins) {
  auto out = ins->output();
  auto op0 = ins->Opr(0);

  DoGetTVal(cg, op0, u::ScrQ());
  if (out->IsMemory()) {
    __ mov(u::Mem(out), u::ScrQ());
  } else {
    __ movq(u::Fp(out), u::ScrQ());
  }
}

// ----------------------------------------------------------------------------
// Atom
void TranslateRuleTable::GenIsBoxAtomInt(CodeGen* cg, const HInstruction* ins) {
  auto opr = ins->Opr(0);
  auto out = ins->output();

  auto invalid = __ newLabel();
  auto done = __ newLabel();

  GenValueTagCmp(cg, opr, JVal::kInt);
  __ jne(invalid);

  // 2) Check wether the int value is small enough
  DoGetTVal(cg, opr, u::ScrD());

  __ cmp(u::ScrD(), JS_ATOM_MAX_INT);
  if (out->IsRegister()) {
    auto r = u::Gp(out);
    __ setl(r.r8());
  } else {
    __ setl(u::ScrB());
    __ mov(u::Mem(out), u::ScrD());
  }

  __ jmp(done);

  // 3) invalid case, ie output is false(0)
  __ bind(invalid);

  if (out->IsRegister()) {
    auto r = u::Gp(out);
    __ xor_(r, r);
  } else {
    __ mov(u::Mem(out), 0);
  }

  __ bind(done);
}

void TranslateRuleTable::GenAtomFromI(CodeGen* cg, const HInstruction* ins) {
  auto opr = ins->Opr(0);
  auto out = ins->output();

  switch (DecideMMType(out, opr)) {
    case MMType::kRr:
      __ or_(u::Gp(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kRR:
      __ mov(u::Gp(out), u::Gp(opr));
      __ or_(u::Gp(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kRM:
      __ mov(u::Gp(out), u::Mem(opr));
      __ or_(u::Gp(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kMR:
      __ mov(u::Mem(out), u::Gp(opr));
      __ or_(u::Mem(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kMM:
      __ mov(u::ScrD(), u::Mem(opr));
      __ mov(u::Mem(out), u::ScrD());
      __ or_(u::Mem(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kRI:
      __ mov(u::Gp(out), u::Imm(opr));
      __ or_(u::Gp(out), JS_ATOM_TAG_INT);
      break;

    case MMType::kMI:
      __ mov(u::Mem(out), u::Imm(opr));
      __ or_(u::Mem(out), JS_ATOM_TAG_INT);
      break;

    default:
      UNREACHABLE();
      break;
  }
}

// If atom value is JS_ATOM_NULL, then deoptimize, otherwise do nothing
void TranslateRuleTable::GenGAtom(CodeGen* cg, const HInstruction* ins) {
  auto opr = ins->Opr(0);
  auto out = ins->output();

  auto done = __ newLabel();

  // check whether it is JS_ATOM_NULL, otherwise deoptimize
  if (opr->IsRegister()) {
    __ cmp(u::Gp(opr), JS_ATOM_NULL);
  } else {
    __ mov(u::ScrD(), u::Mem(opr));
    __ cmp(u::ScrD(), JS_ATOM_NULL);
  }
  __ jnz(done);

  Deoptimize(cg, ins);

  __ bind(done);

  Mov(cg, out, opr, u::ScrQ());
}

void TranslateRuleTable::GenGObjectIsScriptFunction(CodeGen* cg,
                                                    const HInstruction* ins) {
  auto opr = ins->Opr(0);
  a::x86::Gp obj_ptr;
  auto done = __ newLabel();

  if (opr->IsRegister()) {
    obj_ptr = u::Gp(opr);
  } else {
    __ mov(u::ScrQ(), u::Mem(opr));
    obj_ptr = u::ScrQ();
  }

  __ cmp(a::x86::word_ptr(obj_ptr, Layout::Object::kClassId),
         JS_CLASS_BYTECODE_FUNCTION);

  __ je(done);

  Deoptimize(cg, ins);

  __ bind(done);
}

// ----------------------------------------------------------------------------
// Object
//
// This opcode also reserves the rdx register, which we can use it as our
// scratch register for temporary usage.
//
// The basic idea for the fast path is we check whether object's shape's 2
// field is the same with our assumption, ie hash_mask and prop_size. If so it
// means we can directly address into property array without needing to worry
// about memory invalid, and since JSAtom is dedup, we can just compare the
// atom value there to learn whether the target is allowed or not. Notes, if
// entry is invalid, deleted or so, the atom is JSAtomNull which will guard
// us.
//
// This will require way smaller amount of inline code for fast path, though
// it is highly specialized, which is not a bad thing for us until we get our
// inline cache system to cache more cases, ie polymorphic.
void TranslateRuleTable::GenObjectFieldGetAtomFast(CodeGen* cg,
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
    auto r = u::Gp(js_object);
    __ mov(u::ScrQ(), r);
  } else {
    __ mov(u::ScrQ(), u::Mem(js_object));
  }

  // do the fast call
  __ call(cg->code()->object_field_get_atom_fast().label());

  auto cell_addr = __ CellAddress(ins->cell_index());

  // generate output
  if (output->IsRegister()) {
    __ lea(u::Gp(output), cell_addr);
  } else {
    __ lea(u::ScrQ(), cell_addr);
    __ mov(u::Mem(output), u::ScrQ());
  }
}

void TranslateRuleTable::GenObjectFieldSetAtomFast(CodeGen* cg,
                                                   const HInstruction* ins) {
  static_assert(sizeof(::JSShapeProperty) == 8);
  static_assert(sizeof(::JSProperty) == 16);

  auto output = ins->output();
  auto js_object = ins->Opr(0);
  auto atom = ins->Opr(1)->GetImm().UInt32();  // atom index
  auto val = ins->Opr(2);

  auto imm = __ StackTemp0(4);    // pass immediate number
  auto value = __ StackTemp1();   // pass value pointer
  auto cidx = __ StackTemp2(4);   // pass cell index
  auto bflag = __ StackTemp7(4);  // pass the bailout flag, used by slow path

  // stack0 contains atom value
  __ mov(imm, atom);

  // stack1 contains value to be set
  if (val->IsRegister()) {
    __ mov(value, u::Gp(val));
  } else {
    __ mov(u::ScrQ(), u::Mem(val));
    __ mov(value, u::ScrQ());
  }

  // stack2 contains the cell index
  __ mov(cidx, ins->cell_index());

  // stack7 contains the bailout type
  __ mov(bflag, JVal::kNotAvailable);

  // scratch points to the pointer of input
  if (js_object->IsRegister()) {
    auto r = u::Gp(js_object);
    __ mov(u::ScrQ(), r);
  } else {
    __ mov(u::ScrQ(), u::Mem(js_object));
  }

  // do the fast call
  __ call(cg->code()->object_field_set_atom_fast().label());

  // materialize output
  auto cell_addr = __ CellAddress(ins->cell_index());

  // generate output
  if (output->IsRegister()) {
    __ lea(u::Gp(output), cell_addr);
  } else {
    __ lea(u::ScrQ(), cell_addr);
    __ mov(u::Mem(output), u::ScrQ());
  }
}

void TranslateRuleTable::GenMaybeArrayLengthGet(CodeGen* cg,
                                                const HInstruction* ins) {
  auto target = ins->Opr(0);
  auto out = ins->output();

  // --------------------------------------------------------------------------
  // We just check object->fast_array to be true or not. If it is true, we
  // know the count field is valid. Notes, if it is not a fast array, the
  // count field is not valid
  a::x86::Mem fast_array_addr;

  if (target->IsRegister()) {
    auto r = u::Gp(target);
    fast_array_addr = a::x86::byte_ptr(r, 5);
  } else {
    __ mov(u::ScrQ(), u::Mem(target));
    fast_array_addr = a::x86::byte_ptr(u::ScrQ(), 5);
  }

  auto not_found = __ newLabel();
  auto done = __ newLabel();

  __ test(fast_array_addr, 8);
  __ je(not_found);

  // Branch that matches fast array tag
  {
    if (target->IsRegister()) {
      __ mov(u::ScrD(),
             a::x86::dword_ptr(u::Gp(target), Layout::Object::kU_Array_Count));
    } else {
      __ mov(u::ScrQ(), u::Mem(target));
      __ mov(u::ScrD(),
             a::x86::dword_ptr(u::ScrQ(), Layout::Object::kU_Array_Count));
    }

    // Box the value
    __ store_cell(ins->cell_index(), u::ScrD(), JVal::kInt);
    __ jmp(done);
  }

  __ bind(not_found);
  __ invalid_cell(ins->cell_index(), u::ScrQ());

  __ bind(done);

  if (out->IsRegister()) {
    __ lea(u::Gp(out), __ CellAddress(ins->cell_index()));
  } else {
    __ lea(u::ScrQ(), __ CellAddress(ins->cell_index()));
    __ mov(u::Mem(out), u::ScrQ());
  }
}

// The JS_GetGlobalVar function has 2 path can be specialized, in a sensitive
// order,
//
// 1) lookup js_global_var's property table, if this lookup are done then
// return 2) lookup js_global's property table, if this lookup are done, then
// return
//
// For now, we just alias it to GenObjectFieldGetAtomFast call, since we do
// not have fully inlined find_own_property implementation yet. Once we have
// we can perform a fule inline one global_var_obj for lookup, and then fall
// back to a partial find on global_obj and then lastly fallback to slowest
// external runtime call
void TranslateRuleTable::GenGlobalGetVarAtomFast(CodeGen* cg,
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

  // bailout value, notes for global variable access, uninitialized will lead
  // to slow path, which will actually throw an exception afterwards
  __ mov(bflag, JVal::kUninitialized);

  // populate the global variable table, ie the global_var_obj field
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kContext));

  auto js_obj_addr = a::x86::qword_ptr(
      u::ScrQ(), Layout::JSContext::kGlobalVarObject + Layout::Value::kValue);

  __ mov(u::ScrQ(), js_obj_addr);  // object pointer

  __ call(cg->code()->object_field_get_atom_fast().label());

  // generate output
  auto cell_addr = __ CellAddress(ins->cell_index());

  if (output->IsRegister()) {
    __ lea(u::Gp(output), cell_addr);
  } else {
    __ lea(u::ScrQ(), cell_addr);
    __ mov(u::Mem(output), u::ScrQ());
  }
}

void TranslateRuleTable::GenGlobalGetGlobalAtomFast(CodeGen* cg,
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

  // bailout value, notes for global variable access, uninitialized will lead
  // to slow path, which will actually throw an exception afterwards
  __ mov(bflag, JVal::kUninitialized);

  // populate the global variable table, ie the global_var_obj field
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kContext));

  auto js_obj_addr = a::x86::qword_ptr(
      u::ScrQ(), Layout::JSContext::kGlobalVarObject + Layout::Value::kValue);

  __ mov(u::ScrQ(), js_obj_addr);  // object pointer

  __ call(cg->code()->global_get_global_atom_fast().label());

  // generate output
  auto cell_addr = __ CellAddress(ins->cell_index());

  if (output->IsRegister()) {
    __ lea(u::Gp(output), cell_addr);
  } else {
    __ lea(u::ScrQ(), cell_addr);
    __ mov(u::Mem(output), u::ScrQ());
  }
}

// ----------------------------------------------------------------------------
// Array
//
// Scratch and rdx will be reserved
//
void TranslateRuleTable::DoGenLoadArray(
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

  a::x86::Gp obj_ptr;

  if (obj->IsRegister()) {
    obj_ptr = u::Gp(obj);
  } else {
    __ mov(u::ScrQ(), u::Mem(obj));
    obj_ptr = u::ScrQ();
  }

  // 0) check whether object has expected shape
  __ cmp(a::x86::word_ptr(obj_ptr, Layout::Object::kClassId), class_id);
  __ jne(fallback);

  // 1) figure out the length
  if (idx->IsRegister()) {
    auto r = u::Gp(idx);
    __ movsxd(a::x86::rdx, r);
  } else if (idx->IsImm()) {
    auto value = idx->GetImm().Int32();
    CHECK(value >= 0);
    __ mov(a::x86::edx, value);
    // __ movsxd(a::x86::rdx, a::x86::edx);
    lower_bound_check_do_not_need = true;
  } else {
    auto m = u::Mem(idx);
    __ movsxd(a::x86::rdx, m);
  }

  // 2) check the index is in the valid range, ie >= 9 and < array_count
  if (!lower_bound_check_do_not_need) {
    __ cmp(a::x86::rdx, 0);
    __ jl(fallback);
  }

  __ cmp(a::x86::edx,
         a::x86::dword_ptr(obj_ptr, Layout::Object::kU_Array_Count));
  __ jge(fallback);

  // 3) get the inline native array's element address
  //
  // make scratch points to the address of u
  __ mov(u::ScrQ(), a::x86::qword_ptr(obj_ptr, Layout::Object::kU_Array_U));

  a::x86::Mem value_addr;
  if (byte_size <= 8) {
    auto po2 = 0;
    switch (byte_size) {
      case 1:
        po2 = 0;
        break;
      case 2:
        po2 = 1;
        break;
      case 4:
        po2 = 2;
        break;
      case 8:
        po2 = 3;
        break;
      default:
        UNREACHABLE();
        break;
    }

    // okay, just use the basic addressing mode
    value_addr = a::x86::ptr(u::ScrQ(), a::x86::rdx, po2);
  } else {
    CHECK(byte_size == 16);

    __ sal(a::x86::rdx, 4);  // multiply 16
    value_addr = a::x86::ptr(u::ScrQ(), a::x86::rdx, 0);
  }
  value_addr.setSize(byte_size);

  // 4) box the value loaded from the native array directly to the cell
  {
    auto cell_tag_addr = __ CellAddressTag(ins->cell_index());
    auto cell_val_addr = __ CellAddressValue(ins->cell_index());
    auto cell_addr = __ CellAddress(ins->cell_index());

    switch (native_type) {
      case ArrayNativeType::kInt8:
        __ movsx(u::ScrD(), value_addr);
        __ mov(cell_tag_addr, JVal::kInt);

        cell_val_addr.setSize(4);
        __ mov(cell_val_addr, u::ScrD());
        break;

      case ArrayNativeType::kUInt8:
      case ArrayNativeType::kUInt16:
        __ movzx(u::ScrD(), value_addr);
        __ mov(cell_tag_addr, JVal::kInt);

        cell_val_addr.setSize(4);
        __ mov(cell_val_addr, u::ScrD());
        break;

      case ArrayNativeType::kInt16:
        __ movsxd(u::ScrD(), value_addr);
        __ mov(cell_tag_addr, JVal::kInt);

        cell_val_addr.setSize(4);
        __ mov(cell_val_addr, u::ScrD());
        break;

      case ArrayNativeType::kInt32:
      case ArrayNativeType::kUInt32:
        __ mov(u::ScrD(), value_addr);
        __ mov(cell_tag_addr, JVal::kInt);

        cell_val_addr.setSize(4);
        __ mov(cell_val_addr, u::ScrD());
        break;

      case ArrayNativeType::kF64:
        __ mov(u::ScrQ(), value_addr);
        __ mov(cell_tag_addr, JVal::kFloat64);

        cell_val_addr.setSize(8);
        __ mov(cell_val_addr, u::ScrQ());
        break;

      case ArrayNativeType::kF32:
        __ pxor(u::ScrX(), u::ScrX());
        __ cvtss2sd(u::ScrX(), value_addr);
        __ movsd(temp, u::ScrX());
        __ mov(cell_tag_addr, JVal::kFloat64);
        __ mov(u::ScrQ(), temp);

        cell_val_addr.setSize(8);
        __ mov(cell_val_addr, u::ScrQ());
        break;

      case ArrayNativeType::kJSValue:
        __ movdqu(u::ScrX(), value_addr);

        cell_addr.setSize(16);
        __ movups(cell_addr, u::ScrX());
        __ MaybeIncreaseRefCount(ins->cell_index(), u::ScrQ());

        break;

      default:
        UNREACHABLE();
        break;
    }
  }
  __ jmp(done);  // fast path is done

  // 5) handling fallback case, ie to trigger slow path
  __ bind(fallback);
  __ invalid_cell(ins->cell_index(), u::ScrQ());

  // 6) everything is done,
  __ bind(done);

  // 7) generate valid output
  if (out->IsRegister()) {
    __ lea(u::Gp(out), __ CellAddress(ins->cell_index()));
  } else {
    __ lea(u::ScrQ(), __ CellAddress(ins->cell_index()));
    __ mov(u::Mem(out), u::ScrQ());
  }
}

void TranslateRuleTable::GenLoadArrayInt8(CodeGen* cg,
                                          const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_INT8_ARRAY, 1, ArrayNativeType::kInt8);
}

void TranslateRuleTable::GenLoadArrayUInt8(CodeGen* cg,
                                           const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_UINT8_ARRAY, 1, ArrayNativeType::kUInt8);
}

void TranslateRuleTable::GenLoadArrayUInt8C(CodeGen* cg,
                                            const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_UINT8C_ARRAY, 1, ArrayNativeType::kUInt8);
}

void TranslateRuleTable::GenLoadArrayInt16(CodeGen* cg,
                                           const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_INT16_ARRAY, 2, ArrayNativeType::kInt16);
}

void TranslateRuleTable::GenLoadArrayUInt16(CodeGen* cg,
                                            const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_UINT16_ARRAY, 2, ArrayNativeType::kUInt16);
}

void TranslateRuleTable::GenLoadArrayInt32(CodeGen* cg,
                                           const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_INT32_ARRAY, 4, ArrayNativeType::kInt32);
}

void TranslateRuleTable::GenLoadArrayUInt32(CodeGen* cg,
                                            const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_UINT32_ARRAY, 4, ArrayNativeType::kUInt32);
}

void TranslateRuleTable::GenLoadArrayF32(CodeGen* cg, const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_FLOAT32_ARRAY, 4, ArrayNativeType::kF32);
}

void TranslateRuleTable::GenLoadArrayF64(CodeGen* cg, const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_FLOAT64_ARRAY, 8, ArrayNativeType::kF64);
}

void TranslateRuleTable::GenLoadArrayValue(CodeGen* cg,
                                           const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_ARRAY, 16, ArrayNativeType::kJSValue);
}

void TranslateRuleTable::GenLoadArrayArguments(CodeGen* cg,
                                               const HInstruction* ins) {
  DoGenLoadArray(cg, ins, JS_CLASS_ARRAY, 16, ArrayNativeType::kJSValue);
}

// ----------------------------------------------------------------------------
// Deoptimization

std::uint32_t TranslateRuleTable::GetDeoptId(const HInstruction* ins) {
  CHECK(ins->HasDeoptId());
  return static_cast<std::uint32_t>(ins->deopt_id());
}

void TranslateRuleTable::Deoptimize(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->HasDeoptId());
  const auto did = GetDeoptId(ins);
  __ mov(u::ScrD(), did);
  __ jmp(cg->code()->deopt_jump_entry().label());
}

void TranslateRuleTable::Return(CodeGen* cg) {
  __ jmp(cg->code()->epilogue().label());
}

// Allow deopt id will be saved into RAX register
void TranslateRuleTable::GenDeopt(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 0);
  Deoptimize(cg, ins);
}

// The return value will be placed at the first temporary value. The VM
// interpreter will not need to reenter the loop to free the value out
void TranslateRuleTable::GenReturn(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);

  // move the value to the kTemp
  DoStore(cg, Layout::Context::kTemp, 0, ins->Opr(0));

  // --------------------------------------------------------------------------
  // Keep syncing with DeoptGen code for flush VM status out into the frame
  {
    // set the next PC to be null
    __ xor_(u::ScrQ(), u::ScrQ());
    __ mov(a::x86::qword_ptr(u::Ctx(), Layout::Context::kNextPC), u::ScrQ());
    __ mov(a::x86::qword_ptr(u::Ctx(), Layout::Context::kInnerMostFrame),
           u::ScrQ());

    // set the exit_id to be invalid deopt id, which means the function is
    // been returned from deopt id
    __ mov(a::x86::dword_ptr(u::Ctx(), Layout::Context::kExitId),
           kInvalidDeoptId);

    // make eax points to deopt id
    __ mov(a::x86::eax, kInvalidDeoptId);
  }

  // directly jump into return trampoline
  __ jmp(cg->code()->epilogue().label());
}

void TranslateRuleTable::GenDeoptIfTrueOrFalse(CodeGen* cg,
                                               const HInstruction* ins) {
  auto is_true = ins->opcode() == HOpcode::kDeoptIfTrue ? true : false;

  auto cond = ins->Opr(0);
  if (cond->IsRegister()) {
    auto r = u::Gp(cond);
    __ test(r, r);
  } else {
    auto m = u::Mem(cond);
    __ mov(u::ScrD(), m);
    __ test(u::ScrD(), u::ScrD());
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

void TranslateRuleTable::GenDeoptCC(CodeGen* cg, const HInstruction* ins) {
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
  static void Gen(CodeGen* codegen, const HInstruction* instr) {
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
      auto dreg = u::Gp(dest);

      if (source->IsRegister()) {
        __ mov(dreg, u::Gp(source));
      } else if (source->IsImm()) {
        __ mov(dreg, u::Imm(source));
      } else {
        CHECK(source->IsMemory());
        __ mov(dreg, u::Mem(source));
      }
    } else {
      CHECK(dest->IsMemory());
      auto dmem = u::Mem(dest);

      if (source->IsRegister()) {
        __ mov(dmem, u::Gp(source));
      } else if (source->IsImm()) {
        __ mov(dmem, u::Imm(source));
      } else {
        CHECK(source->IsMemory());

        // WARNING(dpeng):
        //   The M2M move will pollute scratch register

        // asmjit will figure out sub-class :)
        __ mov(u::ScrQ(), u::Mem(source));
        __ mov(u::Mem(dest), u::ScrQ());
      }
    }
  }

  static void MoveF(CodeGen* cg, const HOperand* dest, const HOperand* source,
                    const bool is_movq) {
    if (dest->IsRegister()) {
      auto dreg = u::Fp(dest);
      if (source->IsRegister()) {
        if (source->GetRegister()->IsFP()) {
          if (is_movq) {
            __ movq(dreg, u::Fp(source));
          } else {
            __ movsd(dreg, u::Fp(source));
          }
        } else {
          // movsd cannot move data from GP to SSE
          __ movq(dreg, u::Gp(source));
        }
      } else {
        CHECK(source->IsMemory());
        if (is_movq) {
          __ movq(dreg, u::Mem(source));
        } else {
          __ movsd(dreg, u::Mem(source));
        }
      }
    } else {
      CHECK(dest->IsMemory());
      auto dmem = u::Mem(dest);

      if (source->IsRegister()) {
        if (source->GetRegister()->IsFP()) {
          if (is_movq) {
            __ movq(dmem, u::Fp(source));
          } else {
            __ movsd(dmem, u::Fp(source));
          }
        } else {
          __ mov(dmem, u::Gp(source));
          UNREACHABLE()
              << "when source is GP register, the dest cannot be memory";
        }
      } else {
        UNREACHABLE() << "should not issue this";
      }
    }
  }

  static void Gen(CodeGen* cg, const HInstruction* ins) {
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
  static void Gen(CodeGen* env, const HInstruction* instr) { func(env, instr); }
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
  static void Gen(CodeGen* env, const HInstruction* ins) {
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
  static void Gen(CodeGen* env, const HInstruction* instr) {
    AAction::Gen(env, instr);
    RuleActions<Actions...>::Gen(env, instr);
  }
};

template <>
struct RuleActions<> {
  static void Gen(CodeGen*, const HInstruction*) {}
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
#define ECX() EcxOperand
#define RCX() RcxOperand
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
      RType::Gen(codegen, instr);                                \
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
  // Arithmetic Operations
  // --------------------------------------------------------------------------

#define IMPL_BINARY(name, instr)                                              \
  BEGIN(HOpcode::name) {                                                      \
    /** output into register **/                                              \
    RULE("Rrr", MOVE(-1, 0), ASM(instr, REG(-1), REG(1)));                    \
    RULE("Rri", MOVE(-1, 0), ASM(instr, REG(-1), IMM(1)));                    \
    RULE("Rrm", MOVE(-1, 0), ASM(instr, REG(-1), MEM(1)));                    \
    RULE("Rmi", MOVE(-1, 0), ASM(instr, REG(-1), IMM(1)));                    \
    RULE("Rmr", MOVE(-1, 1), ASM(instr, REG(-1), MEM(0)));                    \
    RULE("Rmm", MOVE(-1, 0), ASM(instr, REG(-1), MEM(1)));                    \
    /** output into memory **/                                                \
    RULE("Mri", MOVE(-1, 0), ASM(instr, MEM(-1), IMM(1)));                    \
    RULE("Mrr", MOVE(-1, 0), ASM(instr, MEM(-1), REG(1)));                    \
    RULE("Mrm", MOVE(-1, 0), ASM(mov, SCRATCH(D), MEM(1)),                    \
         ASM(instr, MEM(-1), SCRATCH(D)));                                    \
    RULE("Mmi", MOVE(-1, 0), ASM(instr, MEM(-1), IMM(1)));                    \
    RULE("Mmr", MOVE(-1, 0), ASM(instr, MEM(-1), REG(1)));                    \
    /** Notes, MOVE macro does not support MM **/                             \
    RULE("Mmm", ASM(mov, SCRATCH(D), MEM(0)), ASM(instr, SCRATCH(D), MEM(1)), \
         ASM(mov, MEM(-1), SCRATCH(D)));                                      \
  }                                                                           \
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

    RULE("Rmi", ASM(imul, REG(-1), REG(0), IMM(1)));
    RULE("Rmr", MOVE(-1, 0),
                ASM(imul, REG(-1), REG(1)));
    RULE("Rmm", MOVE(-1, 0),
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

    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)),
                ASM(imul, SCRATCH(D), IMM(1)),
                ASM(mov, MEM(-1), SCRATCH(D)));
    RULE("Mmr", ASM(mov, SCRATCH(D), MEM(0)),
                ASM(imul, SCRATCH(D), REG(1)),
                ASM(mov, MEM(-1), SCRATCH(D)));
    RULE("Mmm", ASM(mov, SCRATCH(D), MEM(0)),
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

    RULE("Rrm", ASM(mov, EAX(), REG(0)), 
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), MEM(1)),
                ASM(mov, REG(-1), EDX()));

    RULE("Rmr", ASM(mov, EAX(), MEM(0)),
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), REG(1)),
                ASM(mov, REG(-1), EDX()));

    RULE("Rmm", ASM(mov, EAX(), MEM(0)),
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), MEM(1)),
                ASM(mov, REG(-1), EDX()));

    RULE("Mrr", ASM(mov, EAX(), REG(0)), 
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), REG(1)),
                ASM(mov, MEM(-1), EDX()));

    RULE("Mrm", ASM(mov, EAX(), REG(0)), 
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), MEM(1)),
                ASM(mov, MEM(-1), EDX()));

    RULE("Mmm", ASM(mov, EAX(), MEM(0)), 
                ASM(xor_, EDX(), EDX()),
                ASM(idiv, EDX(), EAX(), MEM(1)),
                ASM(mov, MEM(-1), EDX()));
  }
  END();
  // clang-format on

  // --------------------------------------------------------------------------
  // Guarded int arithmetic
  // GIAdd, GISub, GIMul, if overflow, then just deoptimization
  BEGIN(HOpcode::kGIAdd) {
    RULE("Rrr", CALL(GenGIAdd));
    RULE("Rrm", CALL(GenGIAdd));
    RULE("Rri", CALL(GenGIAdd));

    RULE("Rmr", CALL(GenGIAdd));
    RULE("Rmm", CALL(GenGIAdd));
    RULE("Rmi", CALL(GenGIAdd));

    RULE("Mri", CALL(GenGIAdd));
    RULE("Mrm", CALL(GenGIAdd));
    RULE("Mrr", CALL(GenGIAdd));

    RULE("Mmi", CALL(GenGIAdd));
    RULE("Mmm", CALL(GenGIAdd));
    RULE("Mmr", CALL(GenGIAdd));
  }
  END();

  BEGIN(HOpcode::kGISub) {
    RULE("Rrr", CALL(GenGISub));
    RULE("Rrm", CALL(GenGISub));
    RULE("Rri", CALL(GenGISub));

    RULE("Rmr", CALL(GenGISub));
    RULE("Rmm", CALL(GenGISub));
    RULE("Rmi", CALL(GenGISub));

    RULE("Mri", CALL(GenGISub));
    RULE("Mrm", CALL(GenGISub));
    RULE("Mrr", CALL(GenGISub));

    RULE("Mmi", CALL(GenGISub));
    RULE("Mmm", CALL(GenGISub));
    RULE("Mmr", CALL(GenGISub));
  }
  END();

  BEGIN(HOpcode::kGIMul) {
    RULE("Rrr", CALL(GenGIMul));
    RULE("Rrm", CALL(GenGIMul));
    RULE("Rri", CALL(GenGIMul));

    RULE("Rmr", CALL(GenGIMul));
    RULE("Rmm", CALL(GenGIMul));
    RULE("Rmi", CALL(GenGIMul));

    RULE("Mrr", CALL(GenGIMul));
    RULE("Mrm", CALL(GenGIMul));
    RULE("Mri", CALL(GenGIMul));

    RULE("Mmr", CALL(GenGIMul));
    RULE("Mmm", CALL(GenGIMul));
    RULE("Mmi", CALL(GenGIMul));
  }
  END();

  // --------------------------------------------------------------------------
  // Floating point operation

#define IMPL_BINARY(name, instr)                                               \
  BEGIN(HOpcode::name) {                                                       \
    /** output into register **/                                               \
    RULE("Xxx", CALL(GenFArithXxx##name));                                     \
    RULE("Xxm", MOVESD(-1, 0), ASM(instr, XMM(-1), MEM(1)));                   \
    RULE("Xmx", MOVESD(-1, 1), ASM(instr, XMM(-1), MEM(0)));                   \
    RULE("Xmm", MOVESD(-1, 0), ASM(instr, XMM(-1), MEM(1)));                   \
    /** output into memory **/                                                 \
    RULE("Mxx", ASM(movq, SCRATCH(X), XMM(0)), ASM(instr, SCRATCH(X), XMM(1)), \
         ASM(movq, MEM(-1), SCRATCH(X)));                                      \
    RULE("Mxm", ASM(movq, SCRATCH(X), XMM(0)), ASM(instr, SCRATCH(X), MEM(1)), \
         ASM(movq, MEM(-1), SCRATCH(X)));                                      \
    RULE("Mmx", ASM(movq, SCRATCH(X), MEM(0)), ASM(instr, SCRATCH(X), XMM(1)), \
         ASM(movq, MEM(-1), SCRATCH(X)));                                      \
    RULE("Mmm", ASM(movq, SCRATCH(X), MEM(0)), ASM(instr, SCRATCH(X), MEM(1)), \
         ASM(movq, MEM(-1), SCRATCH(X)));                                      \
  }                                                                            \
  END();

  IMPL_BINARY(kFAdd, addsd);
  IMPL_BINARY(kFSub, subsd);
  IMPL_BINARY(kFMul, mulsd);
  IMPL_BINARY(kFDiv, divsd);

#undef IMPL_BINARY  // IMPL_BINARY

  BEGIN(HOpcode::kFMod) {
    RULE("Xxx", CALL(GenFMod));
    RULE("Xxm", CALL(GenFMod));
    RULE("Xmx", CALL(GenFMod));
    RULE("Xmm", CALL(GenFMod));

    RULE("Mxx", CALL(GenFMod));
    RULE("Mxm", CALL(GenFMod));
    RULE("Mmx", CALL(GenFMod));
    RULE("Mmm", CALL(GenFMod));
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
    RULE("Xx", CALL(GenIncF));
    RULE("Xm", CALL(GenIncF));
    RULE("Mx", CALL(GenIncF));
    RULE("Mm", CALL(GenIncF));
  }
  END();

  BEGIN(HOpcode::kGInc) {
    RULE("Rr", CALL(GenGInc));
    RULE("Rm", CALL(GenGInc));
    RULE("Mr", CALL(GenGInc));
    RULE("Mm", CALL(GenGInc));
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
    RULE("Xx", CALL(GenDecF));
    RULE("Xm", CALL(GenDecF));
    RULE("Mx", CALL(GenDecF));
    RULE("Mm", CALL(GenDecF));
  }
  END();

  BEGIN(HOpcode::kGDec) {
    RULE("Rr", CALL(GenGDec));
    RULE("Rm", CALL(GenGDec));
    RULE("Mr", CALL(GenGDec));
    RULE("Mm", CALL(GenGDec));
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
    RULE("Xx", CALL(GenNegF));
    RULE("Xm", CALL(GenNegF));
    RULE("Mx", CALL(GenNegF));
    RULE("Mm", CALL(GenNegF));
  }
  END();

  BEGIN(HOpcode::kGNeg) {
    RULE("Rr", CALL(GenGNeg));
    RULE("Rm", CALL(GenGNeg));
    RULE("Mr", CALL(GenGNeg));
    RULE("Mm", CALL(GenGNeg));
  }
  END();

  // --------------------------------------------------------------------------
  // Invert
  BEGIN(HOpcode::kInv) {
    RULE("Ri", ASM(mov, REG(-1), ImmOperandInvert<IMM(0)>));

    RULE("Rr", CALL(GenInv));
    RULE("Rm", CALL(GenInv));
    RULE("Mr", CALL(GenInv));
    RULE("Mm", CALL(GenInv));
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
  // notes for Shl/Shr/Sar the RCX register is been reserved for operand

#define IMPL_BBIN_INT(OP, INS)                                      \
  BEGIN(OP) {                                                       \
    RULE("Rrr", MOVE(-1, 0), ASM(mov, ECX(), REG(1)),               \
         ASM(INS, REG(-1), ECX()));                                 \
    RULE("Rmr", MOVE(-1, 0), ASM(mov, ECX(), REG(1)),               \
         ASM(INS, REG(-1), ECX()));                                 \
    RULE("Rrm", MOVE(-1, 0), ASM(mov, ECX(), MEM(1)),               \
         ASM(INS, REG(-1), ECX()));                                 \
    RULE("Rmm", ASM(mov, REG(-1), MEM(0)), ASM(mov, ECX(), MEM(1)), \
         ASM(INS, REG(-1), ECX()));                                 \
    RULE("Mrr", MOVE(-1, 0), ASM(mov, ECX(), REG(1)),               \
         ASM(INS, MEM(-1), ECX()));                                 \
    RULE("Mmr", MOVE(-1, 0), ASM(mov, ECX(), REG(1)),               \
         ASM(INS, MEM(-1), ECX()));                                 \
    RULE("Mrm", MOVE(-1, 0), ASM(mov, ECX(), MEM(1)),               \
         ASM(INS, MEM(-1), ECX()));                                 \
    RULE("Mmm", MOVE(-1, 0), ASM(mov, ECX(), MEM(1)),               \
         ASM(INS, MEM(-1), ECX()));                                 \
  }                                                                 \
  END();

  // clang-format off
  
  IMPL_BBIN_INT(HOpcode::kSar, sar)

#undef IMPL_BBIN_INT

  BEGIN(HOpcode::kGShlConst) {
    RULE("Rri", CALL(GenGShlConst));
    RULE("Rmi", CALL(GenGShlConst));
    RULE("Mri", CALL(GenGShlConst));
    RULE("Mmi", CALL(GenGShlConst));
  }
  END();

  BEGIN(HOpcode::kGShl) {
    RULE("Rrr", CALL(GenGShl));
    RULE("Rmr", CALL(GenGShl));
    RULE("Rrm", CALL(GenGShl));
    RULE("Rmm", CALL(GenGShl));
    RULE("Mrr", CALL(GenGShl));
    RULE("Mmr", CALL(GenGShl));
    RULE("Mrm", CALL(GenGShl));
    RULE("Mmm", CALL(GenGShl));
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


  BEGIN(HOpcode::kGShr) {
    RULE("Rrr", CALL(GenGShr));
    RULE("Rmr", CALL(GenGShr));
    RULE("Rrm", CALL(GenGShr));
    RULE("Rmm", CALL(GenGShr));
    RULE("Mrr", CALL(GenGShr));
    RULE("Mmr", CALL(GenGShr));
    RULE("Mrm", CALL(GenGShr));
    RULE("Mmm", CALL(GenGShr));
  }
  END();

  BEGIN(HOpcode::kGShrConst) {
    RULE("Rri", CALL(GenGShrConst));
    RULE("Rmi", CALL(GenGShrConst));
    RULE("Mri", CALL(GenGShrConst));
    RULE("Mmi", CALL(GenGShrConst));
  }
  END();

  // clang-format on

  // --------------------------------------------------------------------------
  // Comparison value
  //   On x64, we will utilize conditional mov instruction to avoid branch
  BEGIN(HOpcode::kILt) {
    RULE("Rri", CALL(GenILt));
    RULE("Rrr", CALL(GenILt));
    RULE("Rrm", CALL(GenILt));

    RULE("Mri", CALL(GenILt));
    RULE("Mrr", CALL(GenILt));
    RULE("Mrm", CALL(GenILt));
  }
  END();

  BEGIN(HOpcode::kILe) {
    RULE("Rri", CALL(GenILe));
    RULE("Rrr", CALL(GenILe));
    RULE("Rrm", CALL(GenILe));

    RULE("Mri", CALL(GenILe));
    RULE("Mrr", CALL(GenILe));
    RULE("Mrm", CALL(GenILe));
  }
  END();

  BEGIN(HOpcode::kIGt) {
    RULE("Rri", CALL(GenIGt));
    RULE("Rrr", CALL(GenIGt));
    RULE("Rrm", CALL(GenIGt));

    RULE("Mri", CALL(GenIGt));
    RULE("Mrr", CALL(GenIGt));
    RULE("Mrm", CALL(GenIGt));
  }
  END();

  BEGIN(HOpcode::kIGe) {
    RULE("Rri", CALL(GenIGe));
    RULE("Rrr", CALL(GenIGe));
    RULE("Rrm", CALL(GenIGe));

    RULE("Mri", CALL(GenIGe));
    RULE("Mrr", CALL(GenIGe));
    RULE("Mrm", CALL(GenIGe));
  }
  END();

  BEGIN(HOpcode::kIEq) {
    RULE("Rri", CALL(GenIEq));
    RULE("Rrr", CALL(GenIEq));
    RULE("Rrm", CALL(GenIEq));

    RULE("Mri", CALL(GenIEq));
    RULE("Mrr", CALL(GenIEq));
    RULE("Mrm", CALL(GenIEq));
  }
  END();

  BEGIN(HOpcode::kINe) {
    RULE("Rri", CALL(GenINe));
    RULE("Rrr", CALL(GenINe));
    RULE("Rrm", CALL(GenINe));

    RULE("Mri", CALL(GenINe));
    RULE("Mrr", CALL(GenINe));
    RULE("Mrm", CALL(GenINe));
  }
  END();

  BEGIN(HOpcode::kI64Eq) {
    RULE("Rri", CALL(GenI64Eq));
    RULE("Rrr", CALL(GenI64Eq));
    RULE("Rrm", CALL(GenI64Eq));

    RULE("Mri", CALL(GenI64Eq));
    RULE("Mrr", CALL(GenI64Eq));
    RULE("Mrm", CALL(GenI64Eq));
  }
  END();

  BEGIN(HOpcode::kI64Ne) {
    RULE("Rri", CALL(GenI64Ne));
    RULE("Rrr", CALL(GenI64Ne));
    RULE("Rrm", CALL(GenI64Ne));

    RULE("Mri", CALL(GenI64Ne));
    RULE("Mrr", CALL(GenI64Ne));
    RULE("Mrm", CALL(GenI64Ne));
  }
  END();

  BEGIN(HOpcode::kFLt) {
    RULE("Rxx", CALL(GenFLt));
    RULE("Rxm", CALL(GenFLt));

    RULE("Mxx", CALL(GenFLt));
    RULE("Mxm", CALL(GenFLt));
  }
  END();

  BEGIN(HOpcode::kFLe) {
    RULE("Rxx", CALL(GenFLe));
    RULE("Rxm", CALL(GenFLe));

    RULE("Mxx", CALL(GenFLe));
    RULE("Mxm", CALL(GenFLe));
  }
  END();

  BEGIN(HOpcode::kFGt) {
    RULE("Rxx", CALL(GenFGt));
    RULE("Rxm", CALL(GenFGt));

    RULE("Mxx", CALL(GenFGt));
    RULE("Mxm", CALL(GenFGt));
  }
  END();

  BEGIN(HOpcode::kFGe) {
    RULE("Rxx", CALL(GenFGe));
    RULE("Rxm", CALL(GenFGe));

    RULE("Mxx", CALL(GenFGe));
    RULE("Mxm", CALL(GenFGe));
  }
  END();

  BEGIN(HOpcode::kFEq) {
    RULE("Rxx", CALL(GenFEq));
    RULE("Rxm", CALL(GenFEq));

    RULE("Mxx", CALL(GenFEq));
    RULE("Mxm", CALL(GenFEq));
  }
  END();

  BEGIN(HOpcode::kFNe) {
    RULE("Rxx", CALL(GenFNe));
    RULE("Rxm", CALL(GenFNe));

    RULE("Mxx", CALL(GenFNe));
    RULE("Mxm", CALL(GenFNe));
  }
  END();

  // --------------------------------------------------------------------------
  // Condition(test/cmp)
  BEGIN(HOpcode::kCmpI) {
    RULE("ri", ASM(cmp, REG(0), IMM(1)));
    RULE("mi", ASM(cmp, MEM(0), IMM(1)));
    RULE("mr", ASM(cmp, MEM(0), REG(1)));
    RULE("rm", ASM(cmp, REG(0), MEM(1)));
    RULE("rr", ASM(cmp, REG(0), REG(1)));
    RULE("mm", ASM(mov, SCRATCH(D), MEM(0)), ASM(cmp, SCRATCH(D), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kCmpF) {
    RULE("xx", ASM(ucomisd, XMM(0), XMM(1)));
    RULE("xm", ASM(ucomisd, XMM(0), MEM(1)));

    // require rewriting
    RULE("mx", ASM(movsd, SCRATCH(X), MEM(0)),
         ASM(ucomisd, SCRATCH(X), XMM(1)));
    RULE("mm", ASM(movsd, SCRATCH(X), MEM(0)),
         ASM(ucomisd, SCRATCH(X), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kCmpTest) {
    RULE("m", ASM(mov, SCRATCH(D), MEM(0)), ASM(test, SCRATCH(D), SCRATCH(D)));
    RULE("r", ASM(test, REG(0), REG(0)));
  }
  END();

  BEGIN(HOpcode::kCmpBoxIsHeap) {
    RULE("r", CALL(GenCmpBoxIsHeap));
    RULE("m", CALL(GenCmpBoxIsHeap));
  }
  END();

  BEGIN(HOpcode::kCmpBoxIsPrimitive) {
    RULE("r", CALL(GenCmpBoxIsPrimitive));
    RULE("m", CALL(GenCmpBoxIsPrimitive));
  }
  END();

  BEGIN(HOpcode::kCmpBoxTagIs) {
    RULE("ri", CALL(GenCmpBoxTagIs));
    RULE("rr", CALL(GenCmpBoxTagIs));
    RULE("rm", CALL(GenCmpBoxTagIs));

    RULE("mi", CALL(GenCmpBoxTagIs));
    RULE("mr", CALL(GenCmpBoxTagIs));
    RULE("mm", CALL(GenCmpBoxTagIs));
  }
  END();

  BEGIN(HOpcode::kCmpVarRefListIsEmpty) {
    RULE("", CALL(GenCmpVarRefListIsEmpty));
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
    RULE("Mm", CALL(GenMoveMM));
  }
  END();

  BEGIN(HOpcode::kShuffle) {
    // general purpose
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(GenMoveMM));
  }
  END();

  BEGIN(HOpcode::kDupI) {
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(GenMoveMM));
  }
  END();

  BEGIN(HOpcode::kDupPtr) {
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(GenMoveMM));
  }
  END();

  BEGIN(HOpcode::kTrans) {
    // general purpose
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Rr", ASM(mov, REG(-1), REG(0)));
    RULE("Rm", ASM(mov, REG(-1), MEM(0)));

    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
    RULE("Mr", ASM(mov, MEM(-1), REG(0)));
    RULE("Mm", CALL(GenMoveMM));
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
    RULE("Mm", CALL(GenMoveFMM));
  }
  END();

  BEGIN(HOpcode::kShuffleF) {
    // floating point, notes, floating point mov does not support immediate
    // number, and have to carry out via MovI64
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));

    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(GenMoveFMM));
  }
  END();

  BEGIN(HOpcode::kDupF) {
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));
    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(GenMoveFMM));
  }
  END();

  BEGIN(HOpcode::kTransF) {
    // floating point, notes, floating point mov does not support immediate
    // number, and have to carry out via MovI64
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));

    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(GenMoveFMM));
  }
  END();

  // --------------------------------------------------------------------------
  // MovI64/MovB64/MovI32/MovF64
  BEGIN(HOpcode::kMovI64) {
    RULE("Ri", CALL(GenMoveI64));
    RULE("Mi", CALL(GenMoveI64));
  }
  END();

  BEGIN(HOpcode::kMovF64) {
    RULE("Xi", CALL(GenMoveF64));
    RULE("Mi", CALL(GenMoveF64));
  }
  END();

  BEGIN(HOpcode::kMovB64) {
    RULE("Ri", CALL(GenMoveB64));
    RULE("Mi", CALL(GenMoveB64));
  }
  END();

  BEGIN(HOpcode::kMovI32) {
    RULE("Ri", ASM(mov, REG(-1), IMM(0)));
    RULE("Mi", ASM(mov, MEM(-1), IMM(0)));
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
    RULE("Ri", CALL(GenLoadLocal));
    RULE("Mi", CALL(GenLoadLocal));
  }
  END();

  BEGIN(HOpcode::kLoadParam) {
    RULE("Ri", CALL(GenLoadParam));
    RULE("Mi", CALL(GenLoadParam));
  }
  END();

  BEGIN(HOpcode::kLoadTemp) {
    RULE("Ri", CALL(GenLoadTemp));
    RULE("Mi", CALL(GenLoadTemp));
  }
  END();

  BEGIN(HOpcode::kLoadVarRef) {
    RULE("Ri", CALL(GenLoadVarRef));
    RULE("Mi", CALL(GenLoadVarRef));
  }
  END();

  BEGIN(HOpcode::kBoxToTemp) {
    RULE("rii", CALL(GenBoxToTemp));
    RULE("xii", CALL(GenBoxToTemp));
    RULE("mii", CALL(GenBoxToTemp));
    RULE("iii", CALL(GenBoxToTemp));
  }
  END();

  BEGIN(HOpcode::kStoreVarRef) {
    RULE("ir", CALL(GenStoreVarRef));
    RULE("im", CALL(GenStoreVarRef));
  }
  END();

  BEGIN(HOpcode::kStoreTemp) {
    RULE("ri", CALL(GenStoreTemp));
    RULE("mi", CALL(GenStoreTemp));
  }
  END();

  BEGIN(HOpcode::kBoxToLocal) {
    RULE("rii", CALL(GenBoxToLocal));
    RULE("xii", CALL(GenBoxToLocal));
    RULE("mii", CALL(GenBoxToLocal));
    RULE("iii", CALL(GenBoxToLocal));
  }
  END();

  BEGIN(HOpcode::kStoreLocal) {
    RULE("ri", CALL(GenStoreLocal));
    RULE("mi", CALL(GenStoreLocal));
  }
  END();

  BEGIN(HOpcode::kBoxToParam) {
    RULE("rii", CALL(GenBoxToParam));
    RULE("xii", CALL(GenBoxToParam));
    RULE("mii", CALL(GenBoxToParam));
    RULE("iii", CALL(GenBoxToParam));
  }
  END();

  BEGIN(HOpcode::kStoreParam) {
    RULE("ri", CALL(GenStoreParam));
    RULE("mi", CALL(GenStoreParam));
  }
  END();

  BEGIN(HOpcode::kLeaTemp) {
    RULE("Ri", CALL(GenLeaTemp));
    RULE("Mi", CALL(GenLeaTemp));
  }
  END();

  BEGIN(HOpcode::kLoadNull) {
    RULE("R", CALL(GenLoadNull));
    RULE("M", CALL(GenLoadNull));
  }
  END();

  BEGIN(HOpcode::kLoadUndefined) {
    RULE("R", CALL(GenLoadUndefined));
    RULE("M", CALL(GenLoadUndefined));
  }
  END();

  BEGIN(HOpcode::kLoadUninitialized) {
    RULE("R", CALL(GenLoadUninitialized));
    RULE("M", CALL(GenLoadUninitialized));
  }
  END();

  BEGIN(HOpcode::kLoadStringUnbox) {
    RULE("Ri", CALL(GenLoadStringUnbox));
    RULE("Mi", CALL(GenLoadStringUnbox));
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

  BEGIN(HOpcode::kCastNBToB) {
    RULE("Rr", CALL(GenCastNBToB));
    RULE("Rm", CALL(GenCastNBToB));
    RULE("Mr", CALL(GenCastNBToB));
    RULE("Mm", CALL(GenCastNBToB));
  }
  END();

  BEGIN(HOpcode::kCastF64ToB32) {
    RULE("Rx", CALL(GenCastF64ToB32));
    RULE("Rm", CALL(GenCastF64ToB32));

    RULE("Mx", CALL(GenCastF64ToB32));
    RULE("Mm", CALL(GenCastF64ToB32));
  }
  END();

  BEGIN(HOpcode::kCastBToStr) {
    RULE("Rr", CALL(GenCastBToStr));
    RULE("Rm", CALL(GenCastBToStr));

    RULE("Mr", CALL(GenCastBToStr));
    RULE("Mm", CALL(GenCastBToStr));
  }
  END();

  BEGIN(HOpcode::kCastF64ToB) {
    RULE("Rx", CALL(GenCastF64ToB));
    RULE("Rm", CALL(GenCastF64ToB));

    RULE("Mx", CALL(GenCastF64ToB));
    RULE("Mm", CALL(GenCastF64ToB));
  }
  END();

  BEGIN(HOpcode::kCastVToB) {
    RULE("Rr", CALL(GenCastVToB));
    RULE("Rm", CALL(GenCastVToB));

    RULE("Mr", CALL(GenCastVToB));
    RULE("Mm", CALL(GenCastVToB));
  }
  END();

  BEGIN(HOpcode::kCastObjToB) {
    RULE("Rr", CALL(GenCastObjToB));
    RULE("Rm", CALL(GenCastObjToB));

    RULE("Mr", CALL(GenCastObjToB));
    RULE("Mm", CALL(GenCastObjToB));
  }
  END();

  BEGIN(HOpcode::kCastStrToB) {
    RULE("Rr", CALL(GenCastStrToB));
    RULE("Rm", CALL(GenCastStrToB));

    RULE("Mr", CALL(GenCastStrToB));
    RULE("Mm", CALL(GenCastStrToB));
  }
  END();

  // clang-format on

  // --------------------------------------------------------------------------
  // function call (native call)
  BEGIN(HOpcode::kRCallPtr) {
    RULE("R*", CALL(GenRCallPtr));
    RULE("M*", CALL(GenRCallPtr));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallB64) {
    RULE("R*", CALL(GenRCallB64));
    RULE("M*", CALL(GenRCallB64));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallF64) {
    // notes F64 call will only return to xmm0, always register for now
    RULE("X*", CALL(GenRCallF64));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallI32) {
    RULE("R*", CALL(GenRCallI32));
    RULE("M*", CALL(GenRCallI32));
  }
  END();

  BEGIN(HOpcode::kRCallVoid) {
    RULE("*", CALL(GenRCallVoid));
    NONE();
  }
  END();

  BEGIN(HOpcode::kRCallBox) {
    RULE("M*", CALL(GenRCallBox));
    RULE("R*", CALL(GenRCallBox));
    NONE();
  }
  END();

  // --------------------------------------------------------------------------
  // jscall
  BEGIN(HOpcode::kJSCallF) {
    RULE("Rrir", CALL(GenJSCallF));
    RULE("Rmir", CALL(GenJSCallF));
    RULE("Rmim", CALL(GenJSCallF));
    RULE("Rrim", CALL(GenJSCallF));

    RULE("Mrir", CALL(GenJSCallF));
    RULE("Mmir", CALL(GenJSCallF));
    RULE("Mmim", CALL(GenJSCallF));
    RULE("Mrim", CALL(GenJSCallF));
  }
  END();

  BEGIN(HOpcode::kJSCallFT) {
    RULE("R*", CALL(GenJSCallFT));
    RULE("M*", CALL(GenJSCallFT));
  }
  END();

  BEGIN(HOpcode::kJSCallFN) {
    RULE("R*", CALL(GenJSCallFN));
    RULE("M*", CALL(GenJSCallFN));
  }
  END();

  BEGIN(HOpcode::kJSCallFTN) {
    RULE("R*", CALL(GenJSCallFTN));
    RULE("M*", CALL(GenJSCallFTN));
  }
  END();

  BEGIN(HOpcode::kJSCallCtor) {
    RULE("R*", CALL(GenJSCallCtor));
    RULE("M*", CALL(GenJSCallCtor));
  }
  END();

  BEGIN(HOpcode::kArrayFrom) {
    RULE("R*", CALL(GenArrayFrom));
    RULE("M*", CALL(GenArrayFrom));
  }
  END();

  BEGIN(HOpcode::kApply) {
    RULE("R*", CALL(GenApply));
    RULE("M*", CALL(GenApply));
  }
  END();

  // --------------------------------------------------------------------------
  // jit trace
  // clang-format off
  BEGIN(HOpcode::kEnterJitTrace) { 
    RULE("iiiii", CALL(GenEnterJitTrace));
  }
  END();
  // clang-format on

  // --------------------------------------------------------------------------
  // object
  BEGIN(HOpcode::kObjectFieldGetAtomFast) {
    RULE("Rri", CALL(GenObjectFieldGetAtomFast));
    RULE("Rmi", CALL(GenObjectFieldGetAtomFast));

    RULE("Mri", CALL(GenObjectFieldGetAtomFast));
    RULE("Mmi", CALL(GenObjectFieldGetAtomFast));
  }
  END();

  BEGIN(HOpcode::kObjectFieldSetAtomFast) {
    RULE("Rrir", CALL(GenObjectFieldSetAtomFast));
    RULE("Rrim", CALL(GenObjectFieldSetAtomFast));
    RULE("Rmir", CALL(GenObjectFieldSetAtomFast));
    RULE("Rmim", CALL(GenObjectFieldSetAtomFast));

    RULE("Mrir", CALL(GenObjectFieldSetAtomFast));
    RULE("Mrim", CALL(GenObjectFieldSetAtomFast));
    RULE("Mmir", CALL(GenObjectFieldSetAtomFast));
    RULE("Mmim", CALL(GenObjectFieldSetAtomFast));
  }
  END();

  BEGIN(HOpcode::kMaybeArrayLengthGet) {
    RULE("Rr", CALL(GenMaybeArrayLengthGet));
    RULE("Rm", CALL(GenMaybeArrayLengthGet));

    RULE("Mr", CALL(GenMaybeArrayLengthGet));
    RULE("Mm", CALL(GenMaybeArrayLengthGet));
  }
  END();

  BEGIN(HOpcode::kGlobalGetVarAtomFast) {
    RULE("Rii", CALL(GenGlobalGetVarAtomFast));
    RULE("Mii", CALL(GenGlobalGetVarAtomFast));
  }
  END();

  BEGIN(HOpcode::kGlobalGetGlobalAtomFast) {
    RULE("Rii", CALL(GenGlobalGetGlobalAtomFast));
    RULE("Mii", CALL(GenGlobalGetGlobalAtomFast));
  }
  END();

  // --------------------------------------------------------------------------
  // array
#define LOWER_LOAD_ARRAY(XX)             \
  BEGIN(HOpcode::kLoadArray##XX) {       \
    RULE("Rrr", CALL(GenLoadArray##XX)); \
    RULE("Rrm", CALL(GenLoadArray##XX)); \
    RULE("Rmr", CALL(GenLoadArray##XX)); \
    RULE("Rmm", CALL(GenLoadArray##XX)); \
    RULE("Rmi", CALL(GenLoadArray##XX)); \
    RULE("Rri", CALL(GenLoadArray##XX)); \
                                         \
    RULE("Mrr", CALL(GenLoadArray##XX)); \
    RULE("Mrm", CALL(GenLoadArray##XX)); \
    RULE("Mmr", CALL(GenLoadArray##XX)); \
    RULE("Mmm", CALL(GenLoadArray##XX)); \
    RULE("Mmi", CALL(GenLoadArray##XX)); \
    RULE("Mri", CALL(GenLoadArray##XX)); \
  }                                      \
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
  // Atom
  BEGIN(HOpcode::kIsBoxAtomInt) {
    RULE("Rr", CALL(GenIsBoxAtomInt));
    RULE("Mr", CALL(GenIsBoxAtomInt));

    RULE("Rm", CALL(GenIsBoxAtomInt));
    RULE("Mm", CALL(GenIsBoxAtomInt));
  }
  END();

  BEGIN(HOpcode::kAtomFromI) {
    RULE("Rr", CALL(GenAtomFromI));
    RULE("Rm", CALL(GenAtomFromI));
    RULE("Ri", CALL(GenAtomFromI));

    RULE("Mr", CALL(GenAtomFromI));
    RULE("Mi", CALL(GenAtomFromI));
    RULE("Mm", CALL(GenAtomFromI));
  }
  END();

  BEGIN(HOpcode::kGAtom) {
    RULE("Rr", CALL(GenGAtom));
    RULE("Rm", CALL(GenGAtom));

    RULE("Mr", CALL(GenGAtom));
    RULE("Mm", CALL(GenGAtom));
  }
  END();

  // --------------------------------------------------------------------------
  // misc type test

  BEGIN(HOpcode::kGObjectIsScriptFunction) {
    RULE("r", CALL(GenGObjectIsScriptFunction));
    RULE("m", CALL(GenGObjectIsScriptFunction));
  }
  END();

  // --------------------------------------------------------------------------
  // Boxing Operations
  BEGIN(HOpcode::kGUnboxI) {
    RULE("Rr", CALL(GenGUnboxI));
    RULE("Mr", CALL(GenGUnboxI));

    RULE("Rm", CALL(GenGUnboxI));
    RULE("Mm", CALL(GenGUnboxI));
  }
  END();

  BEGIN(HOpcode::kGUnboxB) {
    RULE("Rr", CALL(GenGUnboxB));
    RULE("Mr", CALL(GenGUnboxB));

    RULE("Rm", CALL(GenGUnboxB));
    RULE("Mm", CALL(GenGUnboxB));
  }
  END();

  BEGIN(HOpcode::kGUnboxF) {
    RULE("Xr", CALL(GenGUnboxF));
    RULE("Mr", CALL(GenGUnboxF));

    RULE("Xm", CALL(GenGUnboxF));
    RULE("Mm", CALL(GenGUnboxF));
  }
  END();

  BEGIN(HOpcode::kGUnboxStr) {
    RULE("Rr", CALL(GenGUnboxStr));
    RULE("Mr", CALL(GenGUnboxStr));
    RULE("Rm", CALL(GenGUnboxStr));
    RULE("Mm", CALL(GenGUnboxStr));
  }
  END();

  BEGIN(HOpcode::kGUnboxMod) {
    RULE("Rr", CALL(GenGUnboxMod));
    RULE("Mr", CALL(GenGUnboxMod));
    RULE("Rm", CALL(GenGUnboxMod));
    RULE("Mm", CALL(GenGUnboxMod));
  }
  END();

  BEGIN(HOpcode::kGUnboxObj) {
    RULE("Rr", CALL(GenGUnboxObj));
    RULE("Mr", CALL(GenGUnboxObj));
    RULE("Rm", CALL(GenGUnboxObj));
    RULE("Mm", CALL(GenGUnboxObj));
  }
  END();

  BEGIN(HOpcode::kUnboxPtr) {
    RULE("Rr", CALL(GenUnboxPtr));
    RULE("Mr", CALL(GenUnboxPtr));
    RULE("Rm", CALL(GenUnboxPtr));
    RULE("Mm", CALL(GenUnboxPtr));
  }
  END();

  BEGIN(HOpcode::kUnboxI) {
    RULE("Rr", CALL(GenUnboxI));
    RULE("Mr", CALL(GenUnboxI));
    RULE("Rm", CALL(GenUnboxI));
    RULE("Mm", CALL(GenUnboxI));
  }
  END();

  BEGIN(HOpcode::kUnboxF) {
    RULE("Rr", CALL(GenUnboxF));
    RULE("Mr", CALL(GenUnboxF));
    RULE("Rm", CALL(GenUnboxF));
    RULE("Mm", CALL(GenUnboxF));
  }
  END();

  BEGIN(HOpcode::kGetTTag) {
    RULE("Rr", CALL(GenGetTTag));
    RULE("Mr", CALL(GenGetTTag));
    RULE("Rm", CALL(GenGetTTag));
    RULE("Mm", CALL(GenGetTTag));
  }
  END();

  BEGIN(HOpcode::kBoxTagIsEq) {
    RULE("rr", CALL(GenBoxTagIsEq));
    RULE("rm", CALL(GenBoxTagIsEq));
    RULE("ri", CALL(GenBoxTagIsEq));

    RULE("mr", CALL(GenBoxTagIsEq));
    RULE("mm", CALL(GenBoxTagIsEq));
    RULE("mi", CALL(GenBoxTagIsEq));
  }
  END();

  BEGIN(HOpcode::kBoxTagIsNe) {
    RULE("rr", CALL(GenBoxTagIsNe));
    RULE("rm", CALL(GenBoxTagIsNe));
    RULE("ri", CALL(GenBoxTagIsNe));

    RULE("mr", CALL(GenBoxTagIsNe));
    RULE("mm", CALL(GenBoxTagIsNe));
    RULE("mi", CALL(GenBoxTagIsNe));
  }
  END();

  BEGIN(HOpcode::kVarRefListIsEmpty) {
    RULE("R", CALL(GenVarRefListIsEmpty));
    RULE("M", CALL(GenVarRefListIsEmpty));
  }
  END();

  BEGIN(HOpcode::kVarRefListIsNotEmpty) {
    RULE("R", CALL(GenVarRefListIsNotEmpty));
    RULE("M", CALL(GenVarRefListIsNotEmpty));
  }
  END();

  BEGIN(HOpcode::kIsNullOrUndefined) {
    RULE("Rr", CALL(GenIsNullOrUndefined));
    RULE("Mr", CALL(GenIsNullOrUndefined));
    RULE("Rm", CALL(GenIsNullOrUndefined));
    RULE("Mm", CALL(GenIsNullOrUndefined));
  }
  END();

  BEGIN(HOpcode::kStrLen) {
    RULE("Rr", CALL(GenStrLen));
    RULE("Mr", CALL(GenStrLen));
    RULE("Rm", CALL(GenStrLen));
    RULE("Mm", CALL(GenStrLen));
  }
  END();

  BEGIN(HOpcode::kBox) {
    RULE("Rri", CALL(GenBox));
    RULE("Rxi", CALL(GenBox));
    RULE("Rmi", CALL(GenBox));

    RULE("Mri", CALL(GenBox));
    RULE("Mxi", CALL(GenBox));
    RULE("Mmi", CALL(GenBox));

    // FIXME(dpeng): We should really move the following constant case into
    // the
    //   LIR optimization/rewrite pass to avoid it to be handlded by the
    //   codegen but it is still fine to be handled here. :(
    RULE("Rii", CALL(GenBox));
    RULE("Mii", CALL(GenBox));
  }
  END();

  // --------------------------------------------------------------------------
  // RC
  BEGIN(HOpcode::kRcInc) {
    RULE("r", CALL(GenRcInc));
    RULE("m", CALL(GenRcInc));
  }
  END();

  BEGIN(HOpcode::kRcDec) {
    RULE("r", CALL(GenRcDec));
    RULE("m", CALL(GenRcDec));
  }
  END();

  BEGIN(HOpcode::kRcIncBox) {
    RULE("r", CALL(GenRcIncBox));
    RULE("m", CALL(GenRcIncBox));
  }
  END();

  BEGIN(HOpcode::kRcDecBox) {
    RULE("r", CALL(GenRcDecBox));
    RULE("m", CALL(GenRcDecBox));
  }
  END();

  // --------------------------------------------------------------------------
  // deoptimization operations
  //
  // clang-format off
  BEGIN(HOpcode::kDeopt) {
    RULE("-", CALL(GenDeopt));
    NONE();
  }
  END();

  BEGIN(HOpcode::kReturn) {
    RULE("m", CALL(GenReturn));
    RULE("r", CALL(GenReturn));
    NONE();
  }
  END();

  BEGIN(HOpcode::kDeoptIfTrue) {
    RULE("m", CALL(GenDeoptIfTrue));
    RULE("r", CALL(GenDeoptIfTrue));
  }
  END();

  BEGIN(HOpcode::kDeoptIfFalse) {
    RULE("m", CALL(GenDeoptIfFalse));
    RULE("r", CALL(GenDeoptIfFalse));
  }
  END();

  BEGIN(HOpcode::kDeoptNe) { 
    RULE("-", CALL(GenDeoptNe));
  }
  END();

  BEGIN(HOpcode::kDeoptEq) { 
    RULE("-", CALL(GenDeoptEq));
  }
  END();

  BEGIN(HOpcode::kDeoptZ) { 
    RULE("-", CALL(GenDeoptZ));
  }
  END();

  BEGIN(HOpcode::kDeoptNz) { 
    RULE("-", CALL(GenDeoptNz));
  }
  END();

  BEGIN(HOpcode::kDeoptLt) { 
    RULE("-", CALL(GenDeoptLt));
  }
  END();

  BEGIN(HOpcode::kDeoptLe) { 
    RULE("-", CALL(GenDeoptLe));
  }
  END();

  BEGIN(HOpcode::kDeoptGt) { 
    RULE("-", CALL(GenDeoptGt));
  }
  END();

  BEGIN(HOpcode::kDeoptGe) { 
    RULE("-", CALL(GenDeoptGe));
  }
  END();

  BEGIN(HOpcode::kDeoptAt) { 
    RULE("-", CALL(GenDeoptAt));
  }
  END();

  BEGIN(HOpcode::kDeoptAe) { 
    RULE("-", CALL(GenDeoptAe));
  }
  END();

  BEGIN(HOpcode::kDeoptBt) { 
    RULE("-", CALL(GenDeoptBt));
  }
  END();

  BEGIN(HOpcode::kDeoptBe) { 
    RULE("-", CALL(GenDeoptBe));
  }
  END();

  // frame
  BEGIN(HOpcode::kEnsureTempSize) { 
    RULE("i", CALL(GenEnsureTempSize));
  }
  END();

  BEGIN(HOpcode::kEnterInlineFrame) {
    RULE("iiiiirrr", CALL(GenEnterInlineFrame));

    RULE("iiiiirrm", CALL(GenEnterInlineFrame));
    RULE("iiiiirmr", CALL(GenEnterInlineFrame));
    RULE("iiiiimrr", CALL(GenEnterInlineFrame));

    RULE("iiiiirmm", CALL(GenEnterInlineFrame));
    RULE("iiiiimrm", CALL(GenEnterInlineFrame));
    RULE("iiiiimmr", CALL(GenEnterInlineFrame));

    RULE("iiiiimmm", CALL(GenEnterInlineFrame));
  }
  END();

  BEGIN(HOpcode::kLeaveInlineFrame) {
    RULE("-", CALL(GenLeaveInlineFrame));
  }
  END();

  // context
  BEGIN(HOpcode::kContextLoadGlobalObject) {
    RULE("R", CALL(GenContextLoadGlobalObject));
    RULE("M", CALL(GenContextLoadGlobalObject));
  }
  END();

  BEGIN(HOpcode::kContextLoad) {
    RULE("R", CALL(GenContextLoad));
    RULE("M", CALL(GenContextLoad));
  }
  END();

  BEGIN(HOpcode::kContextLoadJSContext) {
    RULE("R", CALL(GenContextLoadJSContext));
    RULE("M", CALL(GenContextLoadJSContext));
  }
  END();

  BEGIN(HOpcode::kLoadThis) {
    RULE("R", CALL(GenLoadThis));
    RULE("M", CALL(GenLoadThis));
  }
  END();

  BEGIN(HOpcode::kLoadThisTag) {
    RULE("R", CALL(GenLoadThisTag));
    RULE("M", CALL(GenLoadThisTag));
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
