#include "cg/x64/lir-translate.h"
#include "cg/x64/asm-util.h"
#include "cg/x64/code-gen.h"

#include "lir/operand.h"
#include "lir/instruction.h"
#include "lir/opcode.h"

#include "deopt.h"
#include "quake-rt.h"
#include "qjs.h"

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

// ----------------------------------------------------------------------------
//
// RAX/XMM15 is reserved for Int/Float as scratch register
#define SCR_GPB asmjit::x86::al
#define SCR_GPW asmjit::x86::ax
#define SCR_GPD asmjit::x86::eax
#define SCR_GPQ asmjit::x86::rax
#define SCR_FP asmjit::x86::xmm15

// Reserved for stack usage
#define RSP asmjit::x86::rsp

// Reserved from frame
#define RBP asmjit::x86::rbp

// Reserved for context
#define CTX asmjit::x86::r15

// Output index is been set to -1. Recognized by AtChecked/At of
// LIR::Instruction
#define OUT (-1)

// Assembly code
#define __ (cg->as())->

namespace quake {
namespace cg {
namespace x64 {
namespace {

template <typename T>
std::uint64_t FADDR(T fptr) {
  return reinterpret_cast<std::uint64_t>(fptr);
}

bool IsX64AllowedOutput(const HOperand* out) {
  return out->IsRegister() || out->IsMemory();
}

template <int N>
struct ImmOperand {
  using asmjit_type = const asmjit::Imm&;

  static asmjit::Imm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return ImmOperandToASMJIT(operand);
  }
};

template <int N>
struct Imm8Operand {
  using asmjit_type = const asmjit::Imm&;

  static asmjit::Imm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return Imm8OperandToASMJIT(operand);
  }
};

template <typename T>
struct ImmOperandNegate {
  using asmjit_type = const asmjit::Imm&;

  static asmjit::Imm GetAsmOperand(CodeGen* codegen,
                                   const HInstruction* instr) {
    auto imm = T::GetAsmOperand(codegen, instr);
    auto v = static_cast<std::int32_t>(imm.value());
    return asmjit::Imm(-v);
  }
};

template <typename T>
struct ImmOperandInvert {
  using asmjit_type = const asmjit::Imm&;

  static asmjit::Imm GetAsmOperand(CodeGen* codegen,
                                   const HInstruction* instr) {
    auto imm = T::GetAsmOperand(codegen, instr);
    auto v = static_cast<std::uint32_t>(imm.value());
    return asmjit::Imm(~v);
  }
};

template <int N>
struct RegOperand {
  using asmjit_type = const asmjit::x86::Gp&;

  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return GPRegOperandToASMJIT(operand);
  }
};

struct RaxOperand {
  using asmjit_type = const asmjit::x86::Gp&;

  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::rax;
  }
};

struct EaxOperand {
  using asmjit_type = const asmjit::x86::Gp&;

  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::eax;
  }
};

struct RdxOperand {
  using asmjit_type = const asmjit::x86::Gp&;

  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::rdx;
  }
};

struct ScratchBOperand {
  using asmjit_type = const asmjit::x86::Gp&;
  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::al;
  }
};

struct ScratchWOperand {
  using asmjit_type = const asmjit::x86::Gp&;
  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::ax;
  }
};

struct ScratchDOperand {
  using asmjit_type = const asmjit::x86::Gp&;
  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::eax;
  }
};

struct ScratchQOperand {
  using asmjit_type = const asmjit::x86::Gp&;
  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::rax;
  }
};

struct ScratchXOperand {
  using asmjit_type = const asmjit::x86::Xmm&;
  static asmjit::x86::Xmm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::xmm15;
  }
};

struct EdxOperand {
  using asmjit_type = const asmjit::x86::Gp&;

  static asmjit::x86::Gp GetAsmOperand(CodeGen*, const HInstruction* instr) {
    return asmjit::x86::edx;
  }
};

template <int N>
struct XmmOperand {
  using asmjit_type = const asmjit::x86::Xmm&;
  static asmjit::x86::Xmm GetAsmOperand(CodeGen*, const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return FPRegOperandToASMJIT(operand);
  }
};

template <int N>
struct MemOperand {
  using asmjit_type = const asmjit::x86::Mem&;

  static asmjit::x86::Mem GetAsmOperand(CodeGen* codegen,
                                        const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    return MemoryOperandToASMJIT(operand);
  }
};

template <int N>
struct LabelOperand {
  using asmjit_type = const asmjit::Label&;

  static asmjit::Label GetAsmOperand(CodeGen* codegen,
                                     const HInstruction* instr) {
    auto operand = instr->AtChecked(N);
    CHECK(operand->IsLabel());

    auto label = operand->GetLabel();
    CHECK(!label->HasMachineLabel());

    auto asm_label = codegen->as()->newLabel();
    label->set_machine_label(asm_label);

    return asm_label;
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

  TransEntry GetRule(HOpcode, const std::string_view&);
  TransEntry MatchRule(const HInstruction*) const;

 public:
  // General routine to be used by CodeGen
  static void GenerateDeoptTrampoline(CodeGen*);

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

  // ttag (type tag)
  static void DoGetTTag(CodeGen*, const HOperand*);
  static void LowerGetTTag(CodeGen*, const HInstruction*);
  static int GetTTagFromOperand(const HOperand*);

  // box/unbox
#ifdef JS_NAN_BOXING
  static void LowerBoxNaNBoxing(CodeGen*, const HInstruction*);
  static void LowerUnboxNaNBoxing(CodeGen*, const HInstruction*);
#endif  // JS_NAN_BOXING

  // static void LowerBox(CodeGen*, const HInstruction*);
  // static void LowerUnbox(CodeGen*, const HInstruction*);

  // deoptimization
  static DeoptId GetDeoptId(const HInstruction*);

  static void Deoptimize(CodeGen*, const HInstruction*);
  static void LowerDeopt(CodeGen*, const HInstruction*);
  /*
  static void LowerDeoptIfZ(CodeGen*, const HInstruction*);
  static void LowerDeoptIfNe(CodeGen*, const HInstruction*);
  static void LowerDeoptIfEq(CodeGen*, const HInstruction*);
  */

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
  // It turns out that the most common polymorphic usage of assmebly
  // instruction is the mov. The following helper function abstract away
  // the mov variant in terms of our operand type and mov types.
  static void Mov(CodeGen*, const HOperand*, const HOperand*,
                  asmjit::x86::Gp scratch);

  static void Mov(CodeGen*, const HOperand*, asmjit::Imm);
  static void Mov(CodeGen*, const HOperand*, asmjit::x86::Gp);
  static void Mov(CodeGen*, const HOperand*, asmjit::x86::Mem,
                  asmjit::x86::Gp scratch);

  static void Mov(CodeGen*, asmjit::x86::Gp, const HOperand*);

  /*
  static void Mov(CodeGen*, asmjit::x86::Mem, const HOperand*,
                  asmjit::x86::Gp scratch);
                  */

 private:
  static std::uint32_t ImmAsIndex(const HOperand*);
  static void DoLoadBoxValueFromScratch(CodeGen*, const HInstruction*,
                                        std::uint32_t);

 private:
  TranslateRuleTable();

  void InternalInit();

  void AddRule(HOpcode, const std::string_view&, TransEntry);

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

TransEntry TranslateRuleTable::MatchRule(const HInstruction* ins) const {
  static thread_local std::string kMatchBuffer;  // use TLS to save us from
                                                 // using std::string everytime

  auto idx = static_cast<std::uint32_t>(ins->opcode());
  const auto& entry = table_[idx];

  // covnert the operand to matching character
  auto operand_to_kind = [](const HOperand* opr) -> char {
    if (opr->IsImm()) {
      return 'i';
    }
    if (opr->IsMemory()) {
      return 'm';
    }
    if (opr->IsLabel()) {
      return 'l';
    }
    if (opr->IsRegister()) {
      return opr->IsFP() ? 'x' : 'r';
    }
    if (opr->IsTemporary()) {
      UNREACHABLE() << "All temporary should be resolved by RA";
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
  }

  UNREACHABLE();
  return nullptr;
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to,
                             const HOperand* from, asmjit::x86::Gp scratch) {
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

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to, asmjit::Imm imm) {
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

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to,
                             asmjit::x86::Gp reg) {
  CHECK(!to->IsFloat());
  CHECK(IsX64AllowedOutput(to));

  if (to->IsRegister()) {
    CHECK(to->GetRegister()->IsGP());
    __ mov(GPRegOperandToASMJIT(to), reg);
  } else {
    __ mov(MemoryOperandToASMJIT(to), reg);
  }
}

void TranslateRuleTable::Mov(CodeGen* cg, const HOperand* to,
                             asmjit::x86::Mem mem, asmjit::x86::Gp scratch) {
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

void TranslateRuleTable::Mov(CodeGen* cg, asmjit::x86::Gp to,
                             const HOperand* from) {
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

/*
void TranslateRuleTable::Mov(CodeGen* cg, asmjit::x86::Mem to,
                             const HOperand* from, asmjit::x86::Gp scratch) {
  CHECK(!from->IsFloat());

  if (from->IsRegister()) {
    __ mov(to, GPRegOperandToASMJIT(from));
  } else if (from->IsMemory()) {
    __ mov(scratch, MemoryOperandToASMJIT(from));
    __ mov(to, scratch);
  } else if (from->IsImm()) {
    __ mov(to, ImmOperandToASMJIT(from));
  } else {
    UNREACHABLE();
  }
}
*/

std::uint32_t TranslateRuleTable::ImmAsIndex(const HOperand* opr) {
  CHECK(opr->IsImm());
  const auto& imm = opr->GetImm();
  auto index = imm.Int32();
  CHECK(index >= 0);
  return static_cast<std::uint32_t>(index);
}

void TranslateRuleTable::DoLoadBoxValueFromScratch(CodeGen* cg,
                                                   const HInstruction* ins,
                                                   std::uint32_t offset) {
  CHECK(ins->IsUnary());
  CHECK(ins->HasOutput());
  CHECK(!ins->IsFloat());
  CHECK(ins->output_size_class() == SizeClass::kQWord);

  auto out = ins->output();
  auto index = ImmAsIndex(ins->Opr(0));

  // mov rax, qword [ctx + offset] ; rax points to local
  __ mov(SCR_GPQ, asmjit::x86::qword_ptr(CTX, offset));

  // lea rax, ptr [rax + index * 8] ; rax points to address of target
  __ lea(SCR_GPQ, asmjit::x86::ptr(SCR_GPQ, index * 8));

  if (out->IsRegister()) {
    auto o = GPRegOperandToASMJIT(out);
    // mov output, qword [rax]
    __ mov(o, asmjit::x86::ptr(SCR_GPQ));
  } else if (out->IsMemory()) {
    auto o = MemoryOperandToASMJIT(out);
    // mov rax, qword [rax]
    __ mov(SCR_GPQ, asmjit::x86::ptr(SCR_GPQ));

    // mov qword [output], rax
    __ mov(o, SCR_GPQ);
  } else {
    UNREACHABLE();
  }
}

void TranslateRuleTable::LowerLoadLocal(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadLocal);
  DoLoadBoxValueFromScratch(cg, ins, Layout::Context::kLocal);
}

void TranslateRuleTable::LowerLoadParam(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadParam);
  DoLoadBoxValueFromScratch(cg, ins, Layout::Context::kParam);
}

void TranslateRuleTable::LowerLoadTemp(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kLoadTemp);
  DoLoadBoxValueFromScratch(cg, ins, Layout::Context::kTemp);
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
      // mov al, byte [mem1]
      __ mov(SCR_GPB, asmjit_opr1);

      // mov byte [mem0], al
      __ mov(asmjit_opr1, SCR_GPB);
      break;

    case SizeClass::kWord:
      // mov ax, word [mem1]
      __ mov(SCR_GPW, asmjit_opr1);

      // mov word [mem0], ax
      __ mov(asmjit_opr0, SCR_GPW);
      break;

    case SizeClass::kDWord:
      // mov eax, dword [mem1]
      __ mov(SCR_GPD, asmjit_opr1);

      // mov dword [mem0], eax
      __ mov(asmjit_opr0, SCR_GPD);
      break;

    case SizeClass::kQWord:
      // mov rax, dword [mem1]
      __ mov(SCR_GPQ, asmjit_opr1);

      // mov dword [mem0], rax
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
  __ movabs(asmjit::x86::rax, asmjit::Imm(raw_bits));

  // mov output, rax
  if (opr0->IsRegister()) {
    auto o = GPRegOperandToASMJIT(opr0);
    __ mov(o, asmjit::x86::rax);
  } else if (opr0->IsMemory()) {
    auto o = MemoryOperandToASMJIT(opr0);
    __ mov(o, asmjit::x86::rax);
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

  // get the raw bits and we just need to put it into the rax
  const auto raw_bits = *reinterpret_cast<const std::uint64_t*>(&float64);

  __ movabs(asmjit::x86::rax, asmjit::Imm(raw_bits));
  if (opr0->IsRegister()) {
    auto r = opr0->GetRegister();
    CHECK(r->IsFP());
    auto xmm = FPRegOperandToASMJIT(opr0);
    __ movq(xmm, asmjit::x86::rax);
  } else if (opr0->IsMemory()) {
    auto mem = MemoryOperandToASMJIT(opr0);
    __ mov(mem, asmjit::x86::rax);
  } else {
    UNREACHABLE();
  }
}

// This function does assembly translation of quickjs's JS_VALUE_GET_NORM_TAG
// and put the result into eax register.
void TranslateRuleTable::DoGetTTag(CodeGen* cg, const HOperand* target) {
  static const std::uint32_t kTagDiff =
      static_cast<std::uint32_t>(JS_TAG_FLOAT64 - JS_TAG_FIRST);

  // move box value from target operand pointed to rax, notes we should not
  // scratch original target's pointed value
  if (target->IsMemory()) {
    // mov r, ptr [memory]
    __ mov(asmjit::x86::rax, MemoryOperandToASMJIT(target));
  } else if (target->IsRegister()) {
    const auto r = target->GetRegister();
    CHECK(r->IsQWord());
    __ mov(asmjit::x86::rax, GPRegOperandToASMJIT(target));
  } else {
    UNREACHABLE();
  }

  auto below_tag_diff = __ newLabel();

  // sar rax, 32
  __ sar(asmjit::x86::rax, asmjit::Imm(32));

  // cmp eax, kTagDiff
  __ cmp(asmjit::x86::eax, asmjit::Imm(kTagDiff));

  // if eax >= kTagDiff, jmp to above_tag_diff
  __ jl(below_tag_diff);

  // mov eax, JS_TAG_FLOAT64 (normalized tag)
  __ mov(asmjit::x86::eax,
         asmjit::Imm(static_cast<std::int32_t>(JS_TAG_FLOAT64)));

  // ->: below_tag_diff
  __ bind(below_tag_diff);
}

void TranslateRuleTable::LowerGetTTag(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kGetTTag);
  CHECK(ins->IsUnary());
  CHECK(ins->HasOutput());

  auto opr1 = ins->Opr(0);
  auto output = ins->output();

  DoGetTTag(cg, opr1);  // eax contains the tag

  if (output->IsMemory()) {
    __ mov(MemoryOperandToASMJIT(output), asmjit::x86::eax);
  } else if (output->IsRegister()) {
    __ mov(GPRegOperandToASMJIT(output), asmjit::x86::eax);
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

#ifdef JS_NAN_BOXING

void TranslateRuleTable::LowerBoxNaNBoxing(CodeGen* cg,
                                           const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kBox);
  CHECK(ins->IsBinary());
  CHECK(ins->HasOutput());

  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto output = ins->output();

  // boxed output must be qword
  CHECK(output->IsQWord());
  auto ttag = GetTTagFromOperand(opr1);

  auto already_boxed = false;  // immediate number case, the value can be
                               // boxed directly, without needing to do
                               // runtime boxing

  if (JVal::TypeIsHeap(ttag)) {
    // We allow 3 types of ptr to be stored inside of the operand
    // 1) register, ie register holding a pointer
    // 2) memory, ie memory holding a pointer
    // 3) i64, the pointer's value, ie address, is been directly encoded
    if (opr0->IsRegister()) {
      __ mov(asmjit::x86::rax, GPRegOperandToASMJIT(opr0));
    } else if (opr0->IsMemory()) {
      __ mov(asmjit::x86::rax, MemoryOperandToASMJIT(opr0));
    } else if (opr0->IsImm()) {
      auto const& imm = opr0->GetImm();
      CHECK(imm.IsInt64());
      __ movabs(asmjit::x86::rax, asmjit::Imm(JVal::BitPtr(ttag, imm.Int64())));
      already_boxed = true;  // do not need to box it
    } else {
      UNREACHABLE();
    }
  } else {
    if (opr0->IsRegister()) {
      if (opr0->IsQWord()) {
        __ mov(asmjit::x86::rax, GPRegOperandToASMJIT(opr0));
      } else {
        __ movzx(asmjit::x86::rax, GPRegOperandToASMJIT(opr0));
      }

    } else if (opr0->IsMemory()) {
      if (opr0->IsQWord()) {
        __ mov(asmjit::x86::rax, GPRegOperandToASMJIT(opr0));
      } else {
        __ movzx(asmjit::x86::rax, GPRegOperandToASMJIT(opr0));
      }
    } else if (opr0->IsImm()) {
      auto const& imm = opr0->GetImm();
      CHECK(!imm.IsInt64());  // cannot be imm64, since it is used to store raw
                              // boxed value for now. We have no way to pack a
                              // int64 into NAN_BOXING for now

      already_boxed = true;

      if (imm.IsInt32()) {
        __ movabs(asmjit::x86::rax, asmjit::Imm(JVal::BitI32(imm.Int32())));
      } else {
        // Special case, for floating point, we can directly emit
        CHECK(imm.IsFloat64());
        __ movabs(asmjit::x86::rax, asmjit::Imm(JVal::BitF64(imm.Float64())));
      }
    } else {
      UNREACHABLE();
    }
  }

  if (output->IsMemory()) {
    auto o = MemoryOperandToASMJIT(output);

    if (already_boxed) {
      __ mov(o, asmjit::x86::rax);
    } else {
      __ mov(o, asmjit::x86::rax);
      __ movabs(asmjit::x86::rax,
                asmjit::Imm(static_cast<std::uint64_t>(ttag) << 32));
      __ or_(o, asmjit::x86::rax);
    }
  } else if (output->IsRegister()) {
    auto o = GPRegOperandToASMJIT(output);

    if (already_boxed) {
      __ mov(o, asmjit::x86::rax);
    } else {
      __ mov(o, asmjit::x86::rax);
      __ movabs(asmjit::x86::rax,
                asmjit::Imm(static_cast<std::uint64_t>(ttag) << 32));
      __ or_(o, asmjit::x86::rax);
    }
  } else {
    UNREACHABLE();
  }
}

void TranslateRuleTable::LowerUnboxNaNBoxing(CodeGen* cg,
                                             const HInstruction* ins) {
  CHECK(ins->opcode() == HOpcode::kGUnbox);
  CHECK(ins->IsBinary());
  CHECK(ins->HasOutput());

  auto opr0 = ins->Opr(0);
  auto opr1 = ins->Opr(1);
  auto output = ins->output();

  CHECK(opr1->IsQWord());

  auto ttag = GetTTagFromOperand(opr1);

  // Unbox's output size class is based on its type_tag
  if (JVal::TypeIs64Bits(ttag)) {
    CHECK(output->IsQWord());
  } else {
    CHECK(output->IsDWord());
  }

  if (JVal::TypeIsHeap(ttag)) {
    // simply move the pointer to the destination. Notes, in nan-boxing, the
    // poninter itself does not require unboxing at all
    Mov(cg, output, opr0, asmjit::x86::rax);

  } else if (JVal::TypeIsFloat64(ttag)) {
    CHECK(output->IsFloat());

    if (opr0->IsImm()) {
      // If the operand is an immediate, it must be I64
      const auto& imm = opr0->GetImm();
      CHECK(imm.IsInt64());
      const auto value = JVal::UnboxF64AsB64(
          imm.Int64());  // unbox the value as f64 but output bits

      __ movabs(asmjit::x86::rax, asmjit::Imm(value));

    } else {
      static const std::uint64_t kTag = 9221120288580698112;

      // movabs rax, kTag
      __ movabs(asmjit::x86::rax, asmjit::Imm(kTag));

      if (opr0->IsRegister()) {
        // add rax, r
        __ add(asmjit::x86::rax, GPRegOperandToASMJIT(opr0));
      } else {
        // add rax, ptr [mem]
        __ add(asmjit::x86::rax, MemoryOperandToASMJIT(opr0));
      }
    }

    // Now the rax contains the bit representation of floating point, here, we
    // need to store it back to the output with movq
    if (output->IsRegister()) {
      // movq xmm, rax
      __ movq(FPRegOperandToASMJIT(output), asmjit::x86::rax);
    } else if (output->IsMemory()) {
      // movd [mem], rax
      __ mov(MemoryOperandToASMJIT(output), asmjit::x86::rax);
    } else {
      UNREACHABLE();
    }

  } else {
    CHECK(output->IsDWord());

    Mov(cg, asmjit::x86::rax, opr0);
    Mov(cg, output, asmjit::x86::eax);
  }
}

#endif  // JS_NAN_BOXING

/*
void TranslateRuleTable::LowerBox(CodeGen* cg, const HInstruction* ins) {
#ifdef JS_NAN_BOXING
  LowerBoxNaNBoxing(cg, ins);
#else
#endif  // JS_NAN_BOXING
}

void TranslateRuleTable::LowerUnbox(CodeGen* cg, const HInstruction* ins) {
#ifdef JS_NAN_BOXING
  LowerUnboxNaNBoxing(cg, ins);
#else
#endif  // JS_NAN_BOXING
}

*/

// ----------------------------------------------------------------------------
// unary operation lowering
void TranslateRuleTable::LowerIncDecF(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->opr_size() == 1);

  auto data = cg->const_f64_one();  // get float64 1's label
  auto opr0 = ins->Opr(0);          // operand 0
  auto out = ins->output();         // output

  auto is_inc = ins->opcode() == HOpcode::kIncF;

  if (opr0->IsRegister()) {
    auto opr0_reg = FPRegOperandToASMJIT(out);

    if (out->IsRegister()) {
      auto out_reg = opr0_reg;
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
  auto neg_zero = cg->const_f64_neg_zero();

  if (opr0->IsRegister()) {
    auto opr0_reg = FPRegOperandToASMJIT(opr0);
    __ xorps(opr0_reg, neg_zero);

    if (out->IsRegister()) {
      if (out->GetRegister() != opr0->GetRegister()) {
        __ movq(FPRegOperandToASMJIT(out), opr0_reg);
      }
    } else {
      __ movq(MemoryOperandToASMJIT(out), opr0_reg);
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
  CHECK(ins->HasDeopt());

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
    case HOpcode::kILt:
    case HOpcode::kILe:
    case HOpcode::kIGe:
    case HOpcode::kIGt:
    case HOpcode::kIEq:
    case HOpcode::kINe:
      if (opr1->IsRegister()) {
        __ ucomisd(opr0_reg, FPRegOperandToASMJIT(opr1));
      } else {
        CHECK(opr1->IsMemory());
        __ ucomisd(opr0_reg, MemoryOperandToASMJIT(opr1));
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
  CHECK(ins->HasDeopt());

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
          __ mov(asmjit::x86::eax, MemStackOperandToASMJIT(opr1));
          __ add(out_mem, asmjit::x86::eax);
        } else {
          CHECK(opr1->IsRegister());
          __ add(out_mem, GPRegOperandToASMJIT(opr1));
        }
        break;

      case HOpcode::kGISub:
        if (opr1->IsImm()) {
          __ sub(out_mem, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ mov(asmjit::x86::eax, MemStackOperandToASMJIT(opr1));
          __ sub(out_mem, asmjit::x86::eax);
        } else {
          CHECK(opr1->IsRegister());
          __ sub(out_mem, GPRegOperandToASMJIT(opr1));
        }
        break;

      case HOpcode::kGIMul:
        // for imul, its destination cannot be a memory operand but have to
        // be a register, just use scratch for now
        if (opr1->IsImm()) {
          __ imul(asmjit::x86::eax, ImmOperandToASMJIT(opr1));
        } else if (opr1->IsMemStack()) {
          __ imul(asmjit::x86::eax, MemStackOperandToASMJIT(opr1));
        } else {
          CHECK(opr1->IsRegister());
          __ imul(asmjit::x86::eax, GPRegOperandToASMJIT(opr1));
        }
        __ mov(out_mem, asmjit::x86::eax);
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

// ----------------------------------------------------------------------------
// Deoptimization exit
//
// The deoptimization exit is orgnized as following, each exit will have a
// unique deopt_id that is stored inside of the DeoptXXX instruction. The
// emitted code is as following
//
// 1) Each DeoptXXX will emit code simple as
//    mov rax, exit#
//    jmp deopt_exit // label for shared code
//
// 2) The deopt_exit label points to a per JIT code trampoline, which does all
//    the dirty work and then calls into a C++ code and then simply return
//
//    => deopt_exit
//
//    1. Using push instruction to save *ALL* registers
//    2. The stack frame will be like this
//
//               @RBP
//         =================
//         [-0]       SPILL0
//         [-1]       SPILL2
//                ...
//         [-N]       SPILLN
//         =================
//         [-(N+1)]   kRax
//         [-(N+2)]   kRbx
//         [-(N+3)]   kRcx
//                ...
//         [-(N+16)]  kR15
//         =================
//         [-(N+17)]  kXmm0
//         [-(N+18)]  kXmm1
//                ...
//         [-(N+32)]  kXmm15
//         =================
//               @RSP
//
//      So basically, from RBP to RSP, we save all the JIT frame status onto
//      the stack and then we call C++ runtime function, DeoptPrepare.
//
// 3) Runtime call DeoptPrepare
//
//      DeoptPrepare(void* rbp, void* rsp, std::size_t spill_n);
//
//      Inside of deopt prepare, it will copy the data into Context's data
//      structure for later recovery and also the data.
//
//      Notes context = kR15, and exit# = kRax, user can infer it from the
//      structure passed in
//
//      After return, then JIT frame simply perform epilogue and return.
//
// ----------------------------------------------------------------------------
void TranslateRuleTable::GenerateDeoptTrampoline(CodeGen* cg) {
  cg->BindDeoptTrampoline();

  // (0) save all the GP registers, notes order matters
  __ push(asmjit::x86::rax);  // rax is used to save exit#, save it
  __ push(asmjit::x86::rbx);
  __ push(asmjit::x86::rcx);
  __ push(asmjit::x86::rdx);
  __ push(asmjit::x86::rsp);  // nobody care, but need it somehow
  __ push(asmjit::x86::rbp);  // start of the address, will be passed in as args

  __ push(asmjit::x86::r8);
  __ push(asmjit::x86::r9);
  __ push(asmjit::x86::r10);
  __ push(asmjit::x86::r11);
  __ push(asmjit::x86::r12);
  __ push(asmjit::x86::r13);
  __ push(asmjit::x86::r14);
  __ push(asmjit::x86::r15);  // r15 is used to save ctx(QuakeRT)

  // (1) save all the XMM registers, notes order matters, we just perform move
  //     operation on the lower 8 bytes (SD). Notes we have 16 * 8 in total,
  //     which will make sure RSP is aligned with 16 boundary for SSE
  //     requirement

  // notes rax is not used anymore, actually we really do not need it though,
  // but it is simpler for me :)
  __ mov(asmjit::x86::rax, asmjit::x86::rsp);

  // get more spaces for xmm registers since we just do not have PUSHXMM ins
  __ sub(asmjit::x86::rsp, 16 * 8);

  // notes RSP is incorrectly saved on the stack, just get it right for now
  __ sub(asmjit::x86::ptr(asmjit::x86::rax, -8 * 10), 16 * 8);

  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 0), asmjit::x86::xmm0);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 1), asmjit::x86::xmm1);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 2), asmjit::x86::xmm2);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 3), asmjit::x86::xmm3);

  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 4), asmjit::x86::xmm4);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 5), asmjit::x86::xmm5);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 6), asmjit::x86::xmm6);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 7), asmjit::x86::xmm7);

  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 8), asmjit::x86::xmm8);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 9), asmjit::x86::xmm9);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 10), asmjit::x86::xmm10);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 11), asmjit::x86::xmm11);

  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 12), asmjit::x86::xmm12);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 13), asmjit::x86::xmm13);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 14), asmjit::x86::xmm14);
  __ movsd(asmjit::x86::ptr(asmjit::x86::rax, 8 * 15), asmjit::x86::xmm15);

  // (2) Calls into the PrepareDeopt function
  //
  // now all the GP/FP register is free to be used for later on, okay, we can
  // perform the last thing, calling out to the freaking DeoptPrepare function
  //
  // Notes, on x86/64 the stack is growing downwards, so the data been stored
  // inside stack will be related to the arch and not all arch will be the
  // same. One way is the jit code does shuffling to make sure the order is
  // respected, and that is kind of headache since using C++ is far-better.
  // That's the reason each arch will have DeoptData object to help the
  // DeoptPrepare to get the data in a arch independent way. We just make sure
  // our data is been coordinated well with DeoptData implementation in x64
  // folder

  // arg0 = rbp ; ie start of the local stack
  __ mov(asmjit::x86::rdi, asmjit::x86::rbp);

  // arg0 = rsp ; ie end of the local stack
  __ mov(asmjit::x86::rsi, asmjit::x86::rsp);

  // scratch RAX, in case DeoptPrepare return something, (ie modify its
  // signature);
  __ xor_(asmjit::x86::rax, asmjit::x86::rax);

  // call the DeoptPrepare function
  __ call(FADDR(&QuakeJIT::DeoptPrepare));

  // (3) Jump to the JIT frame epilogue to exit from JIT back to interpreter
  //
  // jump the epilouge. Notes RAX saves the return value from PrepareDeopt,
  // well may not be meaningful anymore
  __ jmp(cg->jit_epilogue());
}

std::uint32_t TranslateRuleTable::GetDeoptId(const HInstruction* ins) {
  CHECK(IsHOpcodeDeopt(ins->opcode()));
  CHECK(!ins->HasOutput());
  CHECK(ins->HasDeopt());
  return static_cast<std::uint32_t>(ins->deopt_id());
}

void TranslateRuleTable::Deoptimize(CodeGen* cg, const HInstruction* ins) {
  CHECK(ins->HasDeopt());
  __ mov(asmjit::x86::eax, GetDeoptId(ins));
  __ jmp(cg->deopt_trampoline());
}

// Allow deopt id will be saved into RAX register
void TranslateRuleTable::LowerDeopt(CodeGen* cg, const HInstruction* ins) {
  Deoptimize(cg, ins);
}

/*
void TranslateRuleTable::LowerDeoptIfZ(CodeGen* cg, const HInstruction* ins) {

  __ mov(asmjit::x86::eax, GetDeoptId(ins));
  __ jz(cg->deopt_trampoline());
}

void TranslateRuleTable::LowerDeoptIfNe(CodeGen* cg, const HInstruction* ins)
{

  __ mov(asmjit::x86::eax, GetDeoptId(ins));
  __ jne(cg->deopt_trampoline());
}

void TranslateRuleTable::LowerDeoptIfEq(CodeGen* cg, const HInstruction* ins)
{

  __ mov(asmjit::x86::eax, GetDeoptId(ins));
  __ je(cg->deopt_trampoline());
}
*/

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

template <typename... Args>
struct ASMJITInstructionType {
  using type =
      asmjit::Error (asmjit::x86::EmitterExplicitT<asmjit::x86::Builder>::*)(
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
// ----------------------------------------------------------------------------
#define REG(x) RegOperand<x>
#define XMM(x) XmmOperand<x>
#define IMM(i) ImmOperand<i>
#define IMM8(i) Imm8Operand<i>
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
            &asmjit::x86::Builder::instr, OperandList<args>>

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
  BEGIN(HOpcode::kPhiLoopF) {
    CRASH("PHI should not show up");
    NONE();
  }
  END();

  BEGIN(HOpcode::kPhiLoopI) {
    CRASH("PHI should not show up");
    NONE();
  }
  END();

  // --------------------------------------------------------------------------
  // Integer arithmetic operations

#define IMPL_BINARY(name, instr)                                         \
  BEGIN(HOpcode::name) {                                                 \
    /** output into register **/                                         \
    RULE("Rri", ASM(mov, REG(-1), IMM(0)), ASM(instr, REG(-1), REG(1))); \
    RULE("Rrr", ASM(mov, REG(-1), REG(0)), ASM(instr, REG(-1), REG(1))); \
    RULE("Rrm", ASM(mov, REG(-1), REG(0)), ASM(instr, REG(-1), MEM(1))); \
    /** output into memory **/                                           \
    RULE("Mri", ASM(mov, MEM(-1), REG(0)), ASM(instr, MEM(-1), IMM(1))); \
    RULE("Mrr", ASM(mov, MEM(-1), REG(0)), ASM(instr, MEM(-1), REG(1))); \
    RULE("Mrm", ASM(mov, MEM(-1), REG(0)), ASM(instr, REG(-1), MEM(1))); \
  }                                                                      \
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

    RULE("Rrr", ASM(mov, REG(-1), REG(0)),
                ASM(imul, REG(-1), REG(1)));

    RULE("Rrm", ASM(mov, REG(-1), REG(0)),
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
    RULE("Rrr", ASM(xor_, EDX(), EDX()),
                ASM(mov, EAX(), REG(0)), 
                ASM(idiv, EDX(), EAX(), REG(1)),
                ASM(mov, REG(-1), EDX()));

    RULE("Mrr", ASM(xor_, EDX(), EDX()),
                ASM(mov, EAX(), REG(0)), 
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

  // --------------------------------------------------------------------------
  // Floating point operation

  BEGIN(HOpcode::kFAdd) {
    RULE("Xxx", ASM(movsd, XMM(-1), XMM(0)), ASM(addsd, XMM(-1), XMM(1)));
    RULE("Xxm", ASM(movsd, XMM(-1), XMM(0)), ASM(addsd, XMM(-1), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kFSub) {
    RULE("Xxx", ASM(movsd, XMM(-1), XMM(0)), ASM(subsd, XMM(-1), XMM(1)));
    RULE("Xxm", ASM(movsd, XMM(-1), XMM(0)), ASM(subsd, XMM(-1), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kFMul) {
    RULE("Xxx", ASM(movsd, XMM(-1), XMM(0)), ASM(mulsd, XMM(-1), XMM(1)));
    RULE("Xxm", ASM(movsd, XMM(-1), XMM(0)), ASM(mulsd, XMM(-1), MEM(1)));
  }
  END();

  BEGIN(HOpcode::kFDiv) {
    RULE("Xxx", ASM(movsd, XMM(-1), XMM(0)), ASM(divsd, XMM(-1), XMM(1)));
    RULE("Xxm", ASM(movsd, XMM(-1), XMM(0)), ASM(divsd, XMM(-1), MEM(1)));
  }
  END();

  // ----------------------------------------------------------------------------
  // Unary operations
#define IMPL_UNARY_INT(X)                                      \
  RULE("Rr", ASM(mov, REG(-1), REG(0)), ASM(X, REG(-1)));      \
  RULE("Rm", ASM(mov, REG(-1), MEM(0)), ASM(X, REG(-1)));      \
  RULE("Mr", ASM(mov, MEM(-1), REG(0)), ASM(X, MEM(-1)));      \
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
    IMPL_UNARY_INT(not_);
    RULE("Ri", ASM(mov, REG(-1), ImmOperandInvert<IMM(0)>));
  }
  END();

#undef IMPL_UNARY_INT  // IMPL_UNARY

  // --------------------------------------------------------------------------
  // Shift
  //
  // shl/shr/sar have 2 versions, one is the XXXConst version
  // which have its second operand (shift bits) in immediate number mode.
  // And the other version is register version, which requires Rcx to be
  // reserved by RA.
  // clang-format off
  BEGIN(HOpcode::kShlConst) {
    RULE("Rri", ASM(mov, REG(-1), REG(0)), ASM(shl, REG(-1), IMM8(1)));
    RULE("Rmi", ASM(mov, REG(-1), MEM(0)), ASM(shl, REG(-1), IMM8(1)));

    RULE("Mri", ASM(mov, MEM(-1), REG(0)), ASM(shl, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shl, MEM(-1), IMM8(1)));
  }
  END();

  BEGIN(HOpcode::kShrConst) {
    RULE("Rri", ASM(mov, REG(-1), REG(0)), ASM(shr, REG(-1), IMM8(1)));
    RULE("Rmi", ASM(mov, REG(-1), MEM(0)), ASM(shr, REG(-1), IMM8(1)));

    RULE("Mri", ASM(mov, MEM(-1), REG(0)), ASM(shr, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shr, MEM(-1), IMM8(1)));
  }
  END();

  BEGIN(HOpcode::kSarConst) {
    RULE("Rri", ASM(mov, REG(-1), REG(0)), ASM(sar, REG(-1), IMM8(1)));
    RULE("Rmi", ASM(mov, REG(-1), MEM(0)), ASM(sar, REG(-1), IMM8(1)));

    RULE("Mri", ASM(mov, MEM(-1), REG(0)), ASM(sar, MEM(-1), IMM8(1)));
    RULE("Mmi", ASM(mov, SCRATCH(D), MEM(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(sar, MEM(-1), IMM8(1)));
  }
  END();

  // notes for Shl/Shr/Sar the RCX register is been reserved for operand, and
  // should already been placed well
  BEGIN(HOpcode::kShl) {
    RULE("Rrr", ASM(mov, REG(-1), REG(0)), ASM(shl, REG(-1), REG(1)));
    RULE("Rmr", ASM(mov, REG(-1), MEM(0)), ASM(shl, REG(-1), REG(1)));

    RULE("Mrr", ASM(mov, MEM(-1), REG(0)), ASM(shl, MEM(-1), REG(1)));
    RULE("Mmr", ASM(mov, SCRATCH(D), REG(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shl, MEM(-1), REG(1)));
  }
  END();

  BEGIN(HOpcode::kShr) {
    RULE("Rrr", ASM(mov, REG(-1), REG(0)), ASM(shr, REG(-1), REG(1)));
    RULE("Rmr", ASM(mov, REG(-1), MEM(0)), ASM(shr, REG(-1), REG(1)));

    RULE("Mrr", ASM(mov, MEM(-1), REG(0)), ASM(shr, MEM(-1), REG(1)));
    RULE("Mmr", ASM(mov, SCRATCH(D), REG(0)), 
                ASM(mov, MEM(-1), SCRATCH(D)),
                ASM(shr, MEM(-1), REG(1)));
  }
  END();

  BEGIN(HOpcode::kSar) {
    RULE("Rrr", ASM(mov, REG(-1), REG(0)), ASM(sar, REG(-1), REG(1)));
    RULE("Rmr", ASM(mov, REG(-1), MEM(0)), ASM(sar, REG(-1), REG(1)));

    RULE("Mrr", ASM(mov, MEM(-1), REG(0)), ASM(sar, MEM(-1), REG(1)));
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
    RULE("Xxx", CALL(LowerFLt));
    RULE("Xxm", CALL(LowerFLt));

    RULE("Mxx", CALL(LowerFLt));
    RULE("Mxm", CALL(LowerFLt));
  }
  END();

  BEGIN(HOpcode::kFLe) {
    RULE("Xxx", CALL(LowerFLe));
    RULE("Xxm", CALL(LowerFLe));

    RULE("Mxx", CALL(LowerFLe));
    RULE("Mxm", CALL(LowerFLe));
  }
  END();

  BEGIN(HOpcode::kFGt) {
    RULE("Xxx", CALL(LowerFGt));
    RULE("Xxm", CALL(LowerFGt));

    RULE("Mxx", CALL(LowerFGt));
    RULE("Mxm", CALL(LowerFGt));
  }
  END();

  BEGIN(HOpcode::kFGe) {
    RULE("Xxx", CALL(LowerFGe));
    RULE("Xxm", CALL(LowerFGe));

    RULE("Mxx", CALL(LowerFGe));
    RULE("Mxm", CALL(LowerFGe));
  }
  END();

  BEGIN(HOpcode::kFEq) {
    RULE("Xxx", CALL(LowerFEq));
    RULE("Xxm", CALL(LowerFEq));

    RULE("Mxx", CALL(LowerFEq));
    RULE("Mxm", CALL(LowerFEq));
  }
  END();

  BEGIN(HOpcode::kFNe) {
    RULE("Xxx", CALL(LowerFNe));
    RULE("Xxm", CALL(LowerFNe));

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
    RULE("mx", ASM(movsd, SCRATCH(X), XMM(0)), ASM(comisd, SCRATCH(X), XMM(1)));
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

  BEGIN(HOpcode::kSpill) {
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

  BEGIN(HOpcode::kSpillF) {
    // floating point, notes, floating point mov does not support immediate
    // number, and have to carry out via MovI64
    RULE("Xx", ASM(movsd, XMM(-1), XMM(0)));
    RULE("Xm", ASM(movsd, XMM(-1), MEM(0)));

    RULE("Mx", ASM(movsd, MEM(-1), XMM(0)));
    RULE("Mm", CALL(LowerMoveFMM));
  }
  END();

  // --------------------------------------------------------------------------
  // MOVZX/MOVSX/MOVSXD
  BEGIN(HOpcode::kMovZX) {
    RULE("Rr", ASM(movzx, REG(-1), REG(0)));
    RULE("Rm", ASM(movzx, REG(-1), MEM(0)));
  }
  END();

  BEGIN(HOpcode::kMovSX) {
    RULE("Rr", ASM(movsx, REG(-1), REG(0)));
    RULE("Rm", ASM(movsx, REG(-1), MEM(0)));
  }
  END();

  BEGIN(HOpcode::kMovSXD) {
    RULE("Rr", ASM(movsxd, REG(-1), REG(0)));
    RULE("Rm", ASM(movsxd, REG(-1), MEM(0)));
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
    RULE("Ri", CALL(LowerMoveF64));
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

  // clang-format on

  // --------------------------------------------------------------------------
  // boxing operations
  /*
  BEGIN(HOpcode::kBox) {
    RULE("Rri", CALL(LowerBox));
    RULE("Mri", CALL(LowerBox));

    RULE("Rmi", CALL(LowerBox));
    RULE("Mmi", CALL(LowerBox));
  }
  END();
  */

  /*
  BEGIN(HOpcode::kGUnbox) {
    RULE("Rri", CALL(LowerUnbox));
    RULE("Mri", CALL(LowerUnbox));

    RULE("Rmi", CALL(LowerUnbox));
    RULE("Mmi", CALL(LowerUnbox));
  }
  END();
  */

  BEGIN(HOpcode::kGetTTag) {
    RULE("Rr", CALL(LowerGetTTag));
    RULE("Mr", CALL(LowerGetTTag));
    RULE("Rm", CALL(LowerGetTTag));
    RULE("Mm", CALL(LowerGetTTag));
  }
  END();

  // --------------------------------------------------------------------------
  // deoptimization operations
  BEGIN(HOpcode::kDeopt) {
    RULE("i", CALL(LowerDeopt));
    NONE();
  }
  END();

  /*
  BEGIN(HOpcode::kDeoptIfZ) {
    RULE("i", CALL(LowerDeoptIfZ));
    NONE();
  }
  END();

  BEGIN(HOpcode::kDeoptIfEq) {
    RULE("i", CALL(LowerDeoptIfEq));
    NONE();
  }
  END();

  BEGIN(HOpcode::kDeoptIfNe) {
    RULE("i", CALL(LowerDeoptIfNe));
    NONE();
  }
  END();
  */

  // --------------------------------------------------------------------------
  // Native Call (FCALLXXX) instructions
  //
  // Notes we rely on the RA to reserve all the caller saved registers, ie
  // spill them out if needed. And we RA should also insert correct moves
  // to enforce the ABI requirements
  //
  // notes for now all FCall just support 4 arguments for simplicity reasons
}

Error TransateLIR(Quake* quake, CodeGen* codegen, HAssembler* has,
                  const HInstruction* ins) {
  UNUSE(has);

  if (auto entry = TranslateRuleTable::GetInstance().MatchRule(ins); entry) {
    entry(codegen, ins);
    return {};
  }
  return Error::Mk("translation rule not found");
}

Error TranslateTrampoline(Quake* quake, CodeGen* codegen) {
  UNUSE(quake);
  TranslateRuleTable::GenerateDeoptTrampoline(codegen);
  return {};
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
