#ifndef _QUAKE_LIR_PARALLEL_MOVE_H_
#define _QUAKE_LIR_PARALLEL_MOVE_H_
#include "base.h"
#include "quake.h"

#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/memory.h"
#include "lir/register.h"

// ParallelMove is a special process of our RA, in many different other compiler
// it has different names. Anyway, it is relative specific to our process.
// The problem is during RA, we need to respect some constraints and also
// perform some specific small move sequences around a certain instruction or
// point in the program. However the move is been inserted by the RA, it is
// not well ordered in terms of data dependency. In order to regain the correct
// order in terms of data dependency, a process needs to be done to build the
// dependency explicitly until we sort out all the needed order. In our current
// RA, the most notably issue is the phi shuffling/elimination in the end of
// the loop body.
//
// Due to the RA's invariants, the ParallelMove has some interesting properties
// to maintain which greatly simplify the algorithm. Since all the move will
// be emitted by the RA, so no 2 moves target points to the same location, since
// the move virtually happen at a single program point (position). Additionally,
// in our case, virtually no move's target will be referenced again by another
// move's input, if we are performing a PHI shuffling. The reason is PHI's
// move output will only be used in next loop iteration and it is impossible
// to reference it in current iteration, though we are in the end of the loop
// body.
//
// Example:
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
class ParallelMove;
class RegisterAllocator;

class ParallelMove {
 public:
  enum SlotType { kReg = 0, kSpill };
  using Slot = std::variant<const Register*, MemoryStack::Slot>;

  class RABroker {
   public:
    virtual ~RABroker() {}

    virtual Slot Resolve(HOperand*) = 0;
  };

  struct Move {
    HInstruction* instruction = nullptr;  // a marker for RA to regenerate the
                                          // data back, unused by ParallelMove
    HOperand* from = nullptr;
    HOperand* to = nullptr;

   public:
    std::uint32_t id() const { return instruction->id(); }
  };

  // The result from the ParallelMove pass will yield 2 sections, one is the
  // micro spill section. Which contains PHI node that requires to be spilled
  // temporary during phi elimination, ie temporarily change phi instruction's
  // output, and then after the spill instructions to recovery it back. This is
  // due to the unresolvable backedge. The other part is a ordered instruction
  // list which allows user to generate move instruction in that order.
  struct Result {
    // A list of micro spill during the move, needed to be materialized by the
    // RA to ensure correctness, ie cycle breaker
    std::vector<Move> micro_spill;

    // A list of order moves specified by the input move structure
    std::vector<Move> move_list;
  };

  ParallelMove(Quake*, HAssembler*, RABroker*, std::size_t max_spill_slot,
               std::vector<Move>&&);

  // Resolve the parallel move and build a correct move order to be materialized
  // by the RA
  Result Resolve();

 private:
  // A read node represents until now how many node tries to reference the
  // specified slot's value. This implicitly forms a barrier for the write
  // and force write to happen after
  struct ReadNode {
    std::vector<Move*> read_list;  // all the move that read from certain slot
    Move* generate_by = nullptr;   // the generate_by move must happen after
  };

  void AddRefReg(Move*, const Register*);
  void AddRefSpill(Move*, MemoryStack::Slot);
  void AddOutReg(Move*, const Register*);
  void AddOutSpill(Move*, MemoryStack::Slot);

  void BuildInput(Move*);
  void BuildOutput(Move*);

  void BuildDPG();
  void AddMicroSpill(Move*);
  ReadNode& FindReadNode(const ParallelMove::Slot&);
  void ResolveConflicts();
  Result WriteOutput();

 private:
  Quake* quake_;
  HAssembler* as_;
  RABroker* ra_;

  std::vector<Move> move_list_;
  std::vector<ReadNode> register_;  // register slot dependency
  std::vector<ReadNode> spill_;     // spill slot dependency
  std::vector<Move*> micro_spill_;  // generate in analyze
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_PARALLEL_MOVE_H_
