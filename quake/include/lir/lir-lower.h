#ifndef _QUAKE_LIR_LIRLOWER_H_
#define _QUAKE_LIR_LIRLOWER_H_
#include "base.h"
#include "ir/opcode.h"

namespace quake {
class Quake;

namespace ir {
class Graph;
class Node;
class SSAVar;
class Checkpoint;
class SSAVar;
}  // namespace ir

namespace lir {
class HAssembler;
class HInstruction;
class HOperand;

// LIR lowerer. It performs the lower operation from ir::Graph into 
// lir::HAssembler's internal representation
class LIRLower {
 public:
  LIRLower(Quake*, HAssembler*);

  // Perform the lowering(translation) of ir::Graph into LIR graph
  void Run(ir::Graph*);

 private:
  // Run a single node
  void RunOne(ir::Node*);

 private:
  void MarkPendingLoopPhi(ir::Node*);
  void MaterializeLoopPhiList();
  void Output(ir::Node*, HOperand*);
  HOperand* TransOperand(ir::SSAVar*);
  HOperand* TransOperandImmediate(ir::SSAVar*);
  HOperand* MaybeBoxOperand(HOperand*, const ir::Type&);

  void TransSnapshot(ir::Node*, ir::Checkpoint*);
  void DoSpecializeLoadArray(ir::Node*,
                             std::function<HOperand*(HOperand*, HOperand*)>&&);
  void DoSpecializeGlobalGet(ir::Node*, bool);
  HOperand* DoBoxToAtom(ir::Node* node);

 private:
  // --------------------------------------------------------------------------
  // IR translation for internal node. Notes the internal node will never be
  // fired directly via Run but will be fired by each groupping node. So we
  // set all the internal node entry as NEVER REACH
  void DoRcDec(ir::Node*);
  void DoRcInc(ir::Node*);
  void DoRcTryOp(ir::Node*, bool);
  void DoRcTryInc(ir::Node*);
  void DoRcTryDec(ir::Node*);

 private:
#define __(A, ...) void Lower_##A(ir::Node*);
  QUAKE_OPCODE_LIST(__)
#undef __  // __

 private:
  // Quake object
  Quake* quake_;

  // High level assembler
  HAssembler* as_;

  // Mappings between SSAVar and HOperand. The graph is in SSA format, so the
  // mapping should be setup already. Notes, we defer the PHI node resolution
  // at very end to make sure PHI is always in the last section. To allow this
  // we place the PHI node's HOperand into the mapping but do not materialize
  // its operand.
  std::map<ir::SSAVar*, HOperand*> mapping_;

  // mapping of label(block). It is only used in JUMP isntruction translation
  std::map<ir::Node*, HOperand*> block_;

  // Pending loop phi node, will be resolved/materialized after everything is
  // done
  std::vector<ir::SSAVar*> loop_phi_;

  // Current node
  ir::Node* current_;
  ir::Node* prev_;

  DISALLOW_COPY_AND_ASSIGN(LIRLower);
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_LIRLOWER_H_
