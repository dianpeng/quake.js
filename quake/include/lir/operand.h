#ifndef _QUAKE_LIR_OPERAND_H_
#define _QUAKE_LIR_OPERAND_H_
#include "base.h"
#include "zone.h"
#include "common.h"
#include "defuse-list.h"
#include "stringify.h"

#include "lir/htype.h"
#include "lir/define.h"
#include "lir/label.h"
#include "lir/memory.h"
#include "lir/immediate.h"
#include "lir/temporary.h"
#include "lir/register.h"

// HOperand stands for LIR's operand. It is essentially yet another SSA as well
// but operand is been directly referenced via its pointer. Each HOperand will
// finally be materialized into a machine representation.
//
// Currently HOperand can be as following during the lowering process of LIR
//
// 0) None, not in used
// 1) Temporary, ie virtual register, not have been materialized into slot
// 2) Register, ie inside of a physical registers
// 3) MemoryStack, ie spilled slot during RA
// 4) MemoryStatic, static memory address
// 5) MemoryInd, ie [base+offset*scaler+displacement] on x86/64
// 6) Immediate, ie static number
// 7) Label, ie a label, used for jumping/branching
//
// Different phase in LIR pass will finaly materialized the HOperand into
// corresponding machine representation. Notes, HOperand is considered to be
// materialized if it is not temporary

namespace quake {

namespace ir {
class SSAVar;  // used for checkpoint tracing
}  // namespace ir

namespace lir {

class HInstruction;
class LivenessInfo;
class HOperand;

// ----------------------------------------------------------------------------
// Use-def/def-use relationship maintaining
//
// Each HOperand will be used by HInstruction via holding a HOperandList; and
// each HInstruction that is a *Value* will produce a HOperand which directly
// embedded inside of HInstruction as pointer. A HInstruction replacement will
// always replace its output HOperand.

using HOperandUse = DefUseRef<HInstruction, HOperand>;
using HOperandUseList = DefUseRefList<HInstruction, HOperand>;

class HOperand : public HOperandUseList, public Stringify {
  struct None {};

  using V = std::variant<None, Immediate, MemoryIndirect, MemoryStatic,
                         MemoryStack, Label*, const Register*, Temporary*>;

 public:
  HOperand(zone::Zone* zone)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::None()),
        v_(None{}) {}

  HOperand(zone::Zone* zone, MemoryIndirect&& m)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::MemInd()),
        v_(std::move(m)) {}

  HOperand(zone::Zone* zone, MemoryStatic&& m)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::MemStatic()),
        v_(std::move(m)) {}

  HOperand(zone::Zone* zone, MemoryStack&& m)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::MemStack()),
        v_(std::move(m)) {}

  HOperand(zone::Zone* zone, const Register* r)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::Register()),
        v_(r) {}

  HOperand(zone::Zone* zone, Temporary* t)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::Temporary()),
        v_(t) {
    t->owner_ = this;
  }

  HOperand(zone::Zone* zone, Label* l)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::Label()),
        v_(l) {}

  HOperand(zone::Zone* zone, Immediate&& imm)
      : HOperandUseList(zone),
        instr_(nullptr),
        type_(HOperandType::Imm()),
        v_(std::move(imm)) {}

 public:
  bool IsNone() const { return type_.IsNone(); }
  bool IsImm() const { return type_.IsImm(); }
  bool IsMemInd() const { return type_.IsMemInd(); }
  bool IsMemStatic() const { return type_.IsMemStatic(); }
  bool IsMemStack() const { return type_.IsMemStack(); }
  bool IsMemory() const { return IsMemStatic() || IsMemStack() || IsMemInd(); }
  bool IsLabel() const { return type_.IsLabel(); }
  bool IsRegister() const { return type_.IsReg(); }
  bool IsTemporary() const { return type_.IsTemporary(); }

  const Immediate& GetImm() const { return base::Get<Immediate>(v_); }
  const MemoryIndirect& GetMemInd() const {
    return base::Get<MemoryIndirect>(v_);
  }
  const MemoryStatic& GetMemStatic() const {
    return base::Get<MemoryStatic>(v_);
  }
  const MemoryStack& GetMemStack() const { return base::Get<MemoryStack>(v_); }
  const Label* GetLabel() const { return base::Get<Label*>(v_); }
  const Register* GetRegister() const { return base::Get<const Register*>(v_); }
  const Temporary* GetTemporary() const { return base::Get<Temporary*>(v_); }

  MemoryIndirect& GetMemInd() { return base::Get<MemoryIndirect>(v_); }
  MemoryStatic& GetMemStatic() { return base::Get<MemoryStatic>(v_); }
  MemoryStack& GetMemStack() { return base::Get<MemoryStack>(v_); }
  Label* GetLabel() { return base::Get<Label*>(v_); }
  Temporary* GetTemporary() { return base::Get<Temporary*>(v_); }

  // Is FP register ?
  bool IsFP() const { return IsRegister() && GetRegister()->IsFP(); }
  // Is General purpose register ?
  bool IsGP() const { return IsRegister() && GetRegister()->IsGP(); }

  // Whether the instruction that generates this output is floating point number
  // type. Notes, it does not need to be inside of the a register, it can be
  // in memory or in immediate number
  bool IsFloat() const;

  const HOperandType& type() const { return type_; }

  inline bool IsMaterialized() const;
  SizeClass size_class() const;
  bool IsQWord() const { return size_class() == SizeClass::kQWord; }
  bool IsDWord() const { return size_class() == SizeClass::kDWord; }
  bool IsWord() const { return size_class() == SizeClass::kWord; }
  bool IsByte() const { return size_class() == SizeClass::kByte; }

  int SizeInBytes() const { return SizeClassToByteSize(size_class()); }
  int SizeInBits() const { return SizeInBytes() * 8; }

 public:
  bool IsRCall() const;
  bool IsNativeCall() const;

  bool IsPhi() const;
  bool IsDeopt() const;
  bool IsJump() const;
  bool IsCmp() const;
  bool IsMove() const;
  bool IsBranch() const;
  bool IsCondition() const;

 public:
  // Liveness information. Used during RA
  LivenessInfo& liveness_info();
  const LivenessInfo& liveness_info() const;

  // Return liveness id or max std::uint32_t. Used by RA for linearization
  std::uint32_t LivenessIdOrMax() const;

  // Generated instruction, if any
  inline HInstruction* instruction() const { return instr_; }
  inline HInstruction* instruction_checked() const;
  bool HasInstruction() const { return instr_ != nullptr; }
  void set_instruction(HInstruction* ins) { instr_ = ins; }

  // return the htype related.
  HType htype() const;

  std::string Print() const override;

 public:
  ir::SSAVar* ir_var() const { return ir_var_; }
  void set_ir_var(ir::SSAVar* v) { ir_var_ = v; }

 private:
  HInstruction* instr_;  // which instruction generate this node, if any
  HOperandType type_;    // operand type
  V v_;                  // value
  ir::SSAVar* ir_var_ = nullptr;  // set by during the RA and maybe mutated

  friend class HInstruction;
};

inline bool HOperand::IsMaterialized() const { return !type_.IsTemporary(); }

inline HInstruction* HOperand::instruction_checked() const {
  CHECK(instr_);
  return instr_;
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_OPERAND_H_
