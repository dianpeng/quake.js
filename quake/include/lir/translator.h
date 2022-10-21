#ifndef _QUAKE_LIR_TRANSLATOR_H_
#define _QUAKE_LIR_TRANSLATOR_H_
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

// Trans_klator. It tries to lower the ir::Graph into lir::HAssembler's internal
// representation.
class Translator {
 public:
  Translator(Quake*, HAssembler*);

  // Perform the lowering(translation) of ir::Graph into LIR graph
  void Run(ir::Graph*);

 private:
  // Run a single node
  void RunOne(ir::Node*);

 private:
  void MarkPendingPhi(ir::Node*);
  void MaterializePhiList();
  void Output(ir::Node*, HOperand*);
  HOperand* TransOperand(ir::SSAVar*);
  HOperand* TransOperandImmediate(ir::SSAVar*);
  void TransSnapshot(ir::Node*, ir::Checkpoint*);

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
#define __(A, ...) void Trans_##A(ir::Node*);
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

  // Pending phi node, will be resolved/materialized after everything is done
  std::vector<ir::SSAVar*> phi_;

  // Current node
  ir::Node* current_;
  ir::Node* prev_;

  DISALLOW_COPY_AND_ASSIGN(Translator);
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_TRANSLATOR_H_
