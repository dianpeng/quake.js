#ifndef _QUAKE_LIR_OPERAND_H_
#define _QUAKE_LIR_OPERAND_H_
#include "base.h"
#include "zone.h"
#include "common.h"

#include "lir/define.h"
#include "lir/liveness.h"
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
namespace lir {

class HInstruction;

class HOperand {
  struct None {};
  using V = std::variant<None, Immediate, MemoryIndirect, MemoryStatic,
                         MemoryStack, Label*, const Register*, Temporary*>;

 public:
  HOperand(zone::Zone* zone, HInstruction* ins)
      : instr_(ins),
        type_(HOperandType::None()),
        v_(None{}),
        liveness_info_(zone) {}

  HOperand(zone::Zone* zone, HInstruction* ins, Immediate&& imm)
      : instr_(ins),
        type_(HOperandType::Imm()),
        v_(std::move(imm)),
        liveness_info_(zone) {}

  HOperand(zone::Zone* zone, HInstruction* ins, MemoryIndirect&& m)
      : instr_(ins),
        type_(HOperandType::MemInd()),
        v_(std::move(m)),
        liveness_info_(zone) {}

  HOperand(zone::Zone* zone, HInstruction* ins, MemoryStatic&& m)
      : instr_(ins),
        type_(HOperandType::MemStatic()),
        v_(std::move(m)),
        liveness_info_(zone) {}

  HOperand(zone::Zone* zone, HInstruction* ins, MemoryStack&& m)
      : instr_(ins),
        type_(HOperandType::MemStack()),
        v_(std::move(m)),
        liveness_info_(zone) {}

  HOperand(zone::Zone* zone, HInstruction* ins, Label* l)
      : instr_(ins),
        type_(HOperandType::Label()),
        v_(l),
        liveness_info_(zone) {}

  HOperand(zone::Zone* zone, HInstruction* ins, const Register* r)
      : instr_(ins),
        type_(HOperandType::Register()),
        v_(r),
        liveness_info_(zone) {}

  HOperand(zone::Zone* zone, HInstruction* ins, Temporary* t)
      : instr_(ins),
        type_(HOperandType::Temporary()),
        v_(t),
        liveness_info_(zone) {
    t->owner_ = this;
  }

 public:
  bool IsNone() const { return type_.IsNone(); }
  bool IsImm() const { return type_.IsImm(); }
  bool IsMemInd() const { return type_.IsMemInd(); }
  bool IsMemStatic() const { return type_.IsMemStatic(); }
  bool IsMemStack() const { return type_.IsMemStack(); }
  bool IsLabel() const { return type_.IsLabel(); }
  bool IsReg() const { return type_.IsReg(); }
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

  const HOperandType& type() const { return type_; }

  inline bool IsMaterialized() const;
  SizeClass size_class() const;

 public:
  bool IsPhi() const;

 public:
  // Liveness information. Used during RA
  LivenessInfo* liveness_info() { return &liveness_info_; }

  // Generated instruction, if any
  HInstruction* instruction() const { return instr_; }
  bool HasInstruction() const { return instr_ != nullptr; }

  // Return liveness id or max std::uint32_t. Used by RA for linearization
  std::uint32_t LivenessIdOrMax() const;

 private:
  HInstruction* instr_;         // which instruction generate this node, if any
  HOperandType type_;           // operand type
  V v_;                         // value
  LivenessInfo liveness_info_;  // liveness information
};

inline bool HOperand::IsMaterialized() const { return !type_.IsTemporary(); }

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_OPERAND_H_
