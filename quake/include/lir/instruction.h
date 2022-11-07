#ifndef _QUAKE_LIR_INSTRUCTION_H_
#define _QUAKE_LIR_INSTRUCTION_H_

#include "base.h"
#include "zone.h"
#include "common.h"
#include "deopt.h"
#include "stringify.h"

#include "defuse-list.h"
#include "lir/liveness.h"
#include "lir/opcode.h"
#include "lir/stack-slot.h"

namespace quake {
namespace ir {
class Node;
class Checkpoint;
}  // namespace ir

namespace lir {
class HAssembler;
class HOperand;
class HInstruction;

struct HInstructionStats {
  std::size_t chained_size;  // the total size of hinstruction in chain list
  std::size_t phi_size;      // number of PHI node size in chain
};

// A intrusive double-linked list
template <typename T>
class HInstructionLink {
 public:
  inline void InsertAfter(HInstructionLink*);
  inline void InsertBefore(HInstructionLink*);
  inline void RemoveSelf();
  T* next() const { return static_cast<T*>(next_); }
  T* prev() const { return static_cast<T*>(prev_); }

 protected:
  void SelfWire() { prev_ = next_ = this; }

  HInstructionLink() = default;

 private:
  HInstructionLink* prev_ = nullptr;
  HInstructionLink* next_ = nullptr;
};

// Operand list
class HOperandList {
 public:
  HOperandList(zone::Zone* z, std::size_t size) : holder_(z) {
    holder_.resize(size, nullptr);
  }

  HOperand* Get(int i) const {
    CHECK(i >= 0 && static_cast<std::size_t>(i) < holder_.size());
    return holder_[i];
  }
  void Set(HInstruction*, int, HOperand*);
  inline int Size() const { return static_cast<int>(holder_.size()); }
  void Clear(HInstruction*);

 private:
  void Resize(std::size_t size) { holder_.resize(size, nullptr); }

 private:
  zone::Vector<HOperand*> holder_;
  friend class HInstruction;
};

// HInstruction is the instruction lives inside of the LIR graph. Essentially
// each node is linked inside of a list with side-exit since it is designed
// for trace. An instruction can bailout via a deoptimization insturction with
// condition guard inside of it. A deoptimization node will bailout when its
// deoptimization condition evaluation fails
class HInstruction : public HInstructionLink<HInstruction>, public Stringify {
 public:
  HInstruction(std::uint32_t, zone::Zone*);                      // sentry
  HInstruction(std::uint32_t, zone::Zone*, HInstruction* prev);  // placeholder

 public:
  HInstruction(std::uint32_t, zone::Zone*, HInstruction* prev, ir::Node*,
               HOpcode opcode);

  HInstruction(std::uint32_t, zone::Zone*, HInstruction* prev, ir::Node*,
               HOpcode opcode, HOperand*);

  HInstruction(std::uint32_t, zone::Zone*, HInstruction* prev, ir::Node*,
               HOpcode opcode, HOperand*, HOperand*);

  HInstruction(std::uint32_t, zone::Zone*, HInstruction* prev, ir::Node*,
               HOpcode opcode, std::size_t);

 public:
  // Verify input operand's constraint against the opcode
  bool VerifyOperand() const;

  // Populate the HOperand output. Notes this function can return nullptr, which
  // means this instruction does not generate any output. Otherwise, the
  // HOperand MUST be a temporary node which is not initialized. Later on, RA
  // will kick in and use it to do register allocation.
  HOperand* PopulateOutput(HAssembler*);

 public:
  bool IsRCall() const { return IsHOpcodeRCall(opcode_); }
  bool IsNativeCall() const { return IsHOpcodeNativeCall(opcode_); }
  bool IsPhi() const { return IsHOpcodePhi(opcode_); }
  bool IsDeopt() const { return IsHOpcodeDeopt(opcode_); }
  bool IsDeoptBranch() const { return IsHOpcodeDeoptBranch(opcode_); }
  bool IsCmp() const { return IsHOpcodeCmp(opcode_); }
  bool IsJump() const { return IsHOpcodeJump(opcode_); }
  bool IsBranch() const { return IsHOpcodeBranch(opcode_); }
  bool IsCondition() const { return IsHOpcodeCondition(opcode_); }
  bool IsMove() const { return IsHOpcodeMove(opcode_); }
  bool IsPlaceholder() const { return opcode() == HOpcode::kPlaceholder; }

  bool IsValue() const { return opcode_info_->output.IsValue(); }
  bool IsOp() const { return opcode_info_->output.IsOp(); }

  // Whether it is a general purpose register user
  bool IsRegisterGP() const {
    return opcode_info().output.output0_rtype() == RegisterType::kGeneral;
  }

  // Whether it is a floating point register user
  bool IsRegisterFP() const {
    return opcode_info().output.output0_rtype() == RegisterType::kFloat;
  }

 public:
  // Check whether the INSTRUCTION is been used as certain argument of native
  // call or not. If so, it returns its index in terms of its native call
  // argument size.
  std::vector<int> NativeCallParamRef() const;
  int MapNativeCallParamIndex(HOperand*) const;
  int MapNativeCallParamIndex(int) const;

 public:
  HOpcode opcode() const { return opcode_; }
  void set_opcode(HOpcode);

  const std::string& opcode_name() const { return opcode_info().name; }
  const HOpcodeInfo& opcode_info() const { return *opcode_info_; }

  // IsFloat returns the type is a floating pointer number or not. On platform
  // like x64, it means we should allocate XMM for their register usage
  bool IsFloat() const { return opcode_info().output.IsFloat(); }
  bool IsBits() const { return opcode_info().output.IsBits(); }
  bool IsInt() const { return opcode_info().output.IsInt(); }
  bool IsPtr() const { return opcode_info().output.IsPtr(); }
  bool IsGC() const { return opcode_info().output.IsGC(); }
  bool IsBox() const { return opcode_info().output.IsBox(); }

  ir::Node* ir_node() const { return ir_node_; }
  HOperand* output() const { return output_; }

  // Used by RA
  void SetRAOutput(HOperand* o);
  bool HasOutput() const { return output_ != nullptr; }

  SizeClass output_size_class() const { return output_sc_; }
  bool IsQWord() const { return output_size_class() == SizeClass::kQWord; }
  bool IsDWord() const { return output_size_class() == SizeClass::kDWord; }
  bool IsWord() const { return output_size_class() == SizeClass::kWord; }
  bool IsByte() const { return output_size_class() == SizeClass::kByte; }

  std::uint32_t id() const { return id_; }

 public:
  HOperand* Opr(int idx) const { return operand_.Get(idx); }
  void OprSet(int idx, HOperand* v) { operand_.Set(this, idx, v); }

  HOperand* At(int idx) const;
  HOperand* AtChecked(int idx) const;

  // Testify all the operand is not nullptr. Certain node requires setup
  // its operand later on. Will need to do this later on
  bool IsComplete() const;

  int opr_size() const { return static_cast<int>(operand_.Size()); }
  int size() const { return opr_size() + (output_ ? 1 : 0); }

  const HType& htype() const { return htype_; }

  LivenessInfo& liveness_info() { return liveness_info_; }
  const LivenessInfo& liveness_info() const { return liveness_info_; }

  bool IsBinary() const { return opr_size() == 2; }
  bool IsUnary() const { return opr_size() == 1; }

  // Try to setup the size class
  bool DecideTypeAndSizeClass();

 public:
  void RemoveOperands() { operand_.Clear(this); }

  // Remove the HInstruction from the linked list. This operation will do
  // following:
  //   1) Remove all the operands out
  //   2) Remove it from the instruction linked list
  void Remove();
  void Replace(HInstruction*);

 public:
  void SetOutput(ir::Node*);

 public:
  // DeoptId, set it up by the PreCodeGen pass
  bool HasDeoptId() const { return deopt_id_ != kInvalidDeoptId; }
  DeoptId deopt_id() const { return deopt_id_; }
  void set_deopt_id(DeoptId id) { deopt_id_ = id; }

  // ir::Checkpoint, related during the construction part. Used to indicate
  // where the jump happened.
  bool HasExit() const { return exit_ != nullptr; }
  ir::Checkpoint* exit() const { return exit_; }
  void set_exit(ir::Checkpoint* ck) { exit_ = ck; }

  CellIndex cell_index() const { return cell_index_; }
  void set_cell_index(CellIndex ci) { cell_index_ = ci; }

  // ToString
  std::string Print() const override;

 private:
  void MaterializePlaceholder(ir::Node*, HOpcode, std::size_t);

 private:
  std::uint32_t id_;      // a unique id in HInstruction space
  DeoptId deopt_id_;      // a unqiue id in Deoptimization space
  CellIndex cell_index_;  // used only when instruction require cell slot
  ir::Checkpoint* exit_;  // exit node, if jumps to the exit
  ir::Node* ir_node_;     // which ir node result in this instruction. Notes
                          // a single ir_node can result in multiple HInstrs
  HOpcode opcode_;        // opcode of the HInstruction
  const HOpcodeInfo* opcode_info_;  // opcode's corresponding info, (cached)

  HType htype_;           // instruction's htype. Notes after the type inference
  HOperandList operand_;  // list of operands
  HOperand* output_;      // operand output list
                          // FIXME(dpeng): Allowed paired register
  SizeClass output_sc_;   // the instruction itself's size class
                          // w.r.t its output. It is the same as
                          // SizeClass stored inside of the HOperand
                          // in output_;
  LivenessInfo liveness_info_;  // liveness
  friend class HAssembler;
};

template <typename T>
inline void HInstructionLink<T>::InsertAfter(HInstructionLink* after) {
  RemoveSelf();

  auto old_next = after ? after->next_ : nullptr;
  prev_ = after;
  if (after) after->next_ = this;
  next_ = old_next;
  if (old_next) old_next->prev_ = this;
}

template <typename T>
inline void HInstructionLink<T>::InsertBefore(HInstructionLink* before) {
  RemoveSelf();

  auto old_prev = before ? before->prev_ : nullptr;
  next_ = before;
  if (before) before->prev_ = this;
  prev_ = old_prev;
  if (old_prev) old_prev->next_ = this;
}

template <typename T>
inline void HInstructionLink<T>::RemoveSelf() {
  if (prev_) prev_->next_ = next_;
  if (next_) next_->prev_ = prev_;
  prev_ = next_ = nullptr;
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_INSTRUCTION_H_
