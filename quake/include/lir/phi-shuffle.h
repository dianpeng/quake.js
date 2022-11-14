#ifndef _QUAKE_LIR_PHI_SHUFFLE_H_
#define _QUAKE_LIR_PHI_SHUFFLE_H_
#include "base.h"
#include "quake.h"

#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/memory.h"
#include "lir/register.h"

// Phi Shuffle
//
// Phi Shuffle is a special process of our RA, in many different other compiler
// it has different names. Anyway, it is relative specific to our process.
// The problem is during RA, we need to eliminate PHI nodes. We use a relative
// simple virtual register method, ie LiveInterval is our virtual method,
// but it still lacks explicit moves to eliminate the PHI. During RA, we do
// allocate PHI nodes register in following order:
//
// 1) LHS of phi
// 2) PHI
// 3) RHS of phi
//
// Then at the end of each CFG block, we need to insert move to eliminate the
// PHI, most notably, the loop PHI node in normal tracing jit. Then this has
// some problem. The problem is at the end of each CFG node, the dependency
// order, ie value dependency of PHI node and its lhs/rhs operand is unknown
// at that moment. The move is basically parallel move, but it has implicit
// data dependency. For example:
//
// #1: add #2, #3
// #4: PHI(#1, ...)
// #5: PHI(#4, ...)
// #6: PHI(#1, ...)
//
// The above example shows that each PHI has its own data dependency with each
// other. And the first PHI node rely on #1, if #1 is allocated to certain
// register R1, and then because #1 is dead when PHI is assigning to a register,
// and then PHI will be allocated to the same register R1. Since we do not know
// the data-dependency, #6's elimination will be incorrect. Since we will insert
// a move for #4, which makes #4 occupies the R1, and then #6 will need to use
// R1 again, since its LHS is output into R1. For this problem, we need to
// build a data dependency graph which allows us to sort the dependency order
// of each PHI elimination. The above example's optimal and correct order will
// be eliminate #6 and then #4. This is what PHI shuffle does. Another brutle
// force way is to spill ALL the registers that is been output by PHI node
// and then perform the PHI elimination. This is not appicable inside of the
// loop since it generates too many moves.

namespace quake {
namespace lir {
class PhiShuffle;
class RegisterAllocator;

class PhiShuffle {
 public:
  enum SlotType { kReg = 0, kSpill };
  using Slot = std::variant<const Register*, MemoryStack::Slot>;

  class RABroker {
   public:
    virtual ~RABroker() {}

    virtual Slot ResolvePhiOperand(HInstruction*, HOperand*) = 0;
    virtual Slot ResolvePhiOutput(HInstruction*) = 0;
  };

  // The result from the PhiShuffle pass will yield 2 sections, one is the
  // micro spill section. Which contains PHI node that requires to be spilled
  // temporary during phi elimination, ie temporarily change phi instruction's
  // output, and then after the spill instructions to recovery it back. This is
  // due to the unresolvable backedge. The other part is a ordered instruction
  // list which allows user to generate move instruction in that order.

  struct Result {
    std::vector<HInstruction*> micro_spill;
    std::vector<HInstruction*> phi_shuffle;
  };

  PhiShuffle(Quake*, HAssembler*, RABroker*, std::size_t max_spill_slot,
             std::vector<HInstruction*>&&);

  Result ResolveLHS();
  Result ResolveRHS();

 private:
  // A read node represents until now how many node tries to reference the
  // specified slot's value. This implicitly forms a barrier for the write
  // and force write to happen after
  struct ReadNode {
    std::vector<HInstruction*> read_list;
    HInstruction* generate_by = nullptr;
  };

  void AddRefReg(HInstruction*, const Register*);
  void AddRefSpill(HInstruction*, MemoryStack::Slot);
  void AddOutReg(HInstruction*, const Register*);
  void AddOutSpill(HInstruction*, MemoryStack::Slot);

  void BuildPhiInput(HInstruction*, HOperand*);
  void BuildPhiOutput(HInstruction*);
  void BuildDPG();
  void AddMicroSpill(HInstruction*);
  ReadNode& FindReadNode(const PhiShuffle::Slot&);
  void ResolveConflicts();
  Result WriteOutput();
  Result Resolve();

 private:
  Quake* quake_;
  HAssembler* as_;
  RABroker* ra_;

  std::vector<HInstruction*> phi_list_;
  std::vector<ReadNode> register_;
  std::vector<ReadNode> spill_;
  std::vector<HInstruction*> micro_spill_;  // generate in analyze
  std::size_t phi_operand_index_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_PHI_SHUFFLE_H_
