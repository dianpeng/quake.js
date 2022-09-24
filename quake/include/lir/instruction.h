#ifndef _QUAKE_LIR_INSTRUCTION_H_
#define _QUAKE_LIR_INSTRUCTION_H_

#include "base.h"
#include "zone.h"
#include "common.h"
#include "lir/opcode.h"

namespace quake {
namespace ir {
class Node;
}  // namespace ir

namespace lir {
class HAssembler;
class HOperand;

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

// HInstruction is the instruction lives inside of the LIR graph. Essentially
// each node is linked inside of a list with side-exit since it is designed
// for trace. An instruction can bailout via a deoptimization insturction with
// condition guard inside of it. A deoptimization node will bailout when its
// deoptimization condition evaluation fails
class HInstruction : public HInstructionLink<HInstruction> {
 public:
  HInstruction(std::uint32_t, zone::Zone*);  // sentry

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
  bool IsPhi() const { return IsHOpcodePhi(opcode_); }
  bool IsValue() const { return opcode_info_.output.IsValue(); }
  bool IsOp() const { return opcode_info_.output.IsOp(); }

 public:
  HOpcode opcode() const { return opcode_; }
  const HOpcodeInfo& opcode_info() const { return opcode_info_; }
  ir::Node* ir_node() const { return ir_node_; }
  HOperand* output() { return output_; }
  bool has_output() const { return output_ != nullptr; }
  SizeClass output_size_class() const { return output_sc_; }
  std::uint32_t id() const { return id_; }

 public:
  HOperand* Opr(int idx) const {
    CHECK(idx >= 0 && static_cast<std::size_t>(idx) < opr_size());
    return operand_[idx];
  }

  void OprSet(int idx, HOperand* v) {
    CHECK(idx >= 0 && static_cast<std::size_t>(idx) < opr_size());
    operand_[idx] = v;
  }

  HOperand* At(int idx) const;

  std::size_t opr_size() const { return operand_.size(); }
  std::size_t size() const { return opr_size() + (output_ ? 1 : 0); }

 private:
  void DecideSizeClass();

 public:
  std::uint32_t id_;  // a unique id related to HInstruction

  ir::Node* ir_node_;  // which ir node result in this instruction. Notes
                       // a single ir_node can result in multiple HInstrs

  HOpcode opcode_;                   // opcode of the HInstruction
  const HOpcodeInfo& opcode_info_;   // opcode's corresponding info, (cached)
  zone::Vector<HOperand*> operand_;  // operands list

  HOperand* output_;     // operand output list
                         // FIXME(dpeng): Allowed paired register
  SizeClass output_sc_;  // the instruction itself's size class
                         // w.r.t its output. It is the same as
                         // SizeClass stored inside of the HOperand
                         // in output_;
  friend class HAssembler;
};

template <typename T>
inline void HInstructionLink<T>::InsertAfter(HInstructionLink* after) {
  auto old_next = after ? after->next_ : nullptr;
  prev_ = after;
  if (after) after->next_ = this;
  next_ = old_next;
  if (old_next) old_next->prev_ = this;
}

template <typename T>
inline void HInstructionLink<T>::InsertBefore(HInstructionLink* before) {
  auto new_next = before ? before->next_ : nullptr;
  if (before) before->next_ = this;
  prev_ = before;
  next_ = new_next;
  if (new_next) new_next->prev_ = this;
}

template <typename T>
inline void HInstructionLink<T>::RemoveSelf() {
  if (prev_) prev_->next = next_;
  if (next_) next_->prev = prev_;
  prev_ = next_ = nullptr;
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_INSTRUCTION_H_
