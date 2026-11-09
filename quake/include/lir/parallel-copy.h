#ifndef _QUAKE_LIR_PARALLEL_COPY_H_
#define _QUAKE_LIR_PARALLEL_COPY_H_
#include "base.h"
#include "common.h"
#include "zone.h"

#include "lir/opcode.h"
#include "lir/operand.h"
#include "lir/instruction.h"
#include "lir/assembler.h"
#include "lir/memory.h"
#include "lir/register.h"

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// Parallel Copy — first-class explicit parallel move per D-28 / D-30.
//
// A kParallelCopy instruction carries an explicit list of (src, dst) slot
// pairs that conceptually all happen at the same program point.  The
// ParallelCopyResolver turns the pair set into an order-correct sequential
// move program, breaking cycles and spill-to-spill transfers through a
// per-site scratch slot.
//
// Pairs are stored as 2N consecutive HOperand* entries in the instruction's
// operand list: even indices are sources, odd indices are destinations.
// Each slot is either a physical Register* or a MemoryStack::Slot.
// ----------------------------------------------------------------------------

// A single abstract slot — either a register or a spill slot.
struct CopySlot {
  enum Kind { kRegister, kSpill };

  Kind kind;
  const Register* reg = nullptr;        // valid when kind == kRegister
  MemoryStack::Slot spill_slot = 0;     // valid when kind == kSpill

  // Register constructor
  static CopySlot FromReg(const Register* r) {
    CopySlot s;
    s.kind = kRegister;
    s.reg = r;
    return s;
  }

  // Spill constructor
  static CopySlot FromSpill(MemoryStack::Slot slot) {
    CopySlot s;
    s.kind = kSpill;
    s.spill_slot = slot;
    return s;
  }

  bool IsRegister() const { return kind == kRegister; }
  bool IsSpill() const { return kind == kSpill; }

  bool IsGP() const { return kind == kRegister && reg && reg->IsGP(); }
  bool IsFP() const { return kind == kRegister && reg && reg->IsFP(); }

  // Equality — two slots are the same if they refer to the same storage
  // Physical-slot identity: two register slots are the SAME slot when they
  // name the same physical register — size-class views (eax vs rax, r13d
  // vs r13) alias one physical register, and parallel copies move physical
  // registers. Width lives on the pair, not the slot.
  bool operator==(const CopySlot& that) const {
    if (kind != that.kind) return false;
    if (kind == kRegister) {
      return reg == that.reg ||
             (reg != nullptr && that.reg != nullptr &&
              reg->register_kind() == that.reg->register_kind());
    }
    return spill_slot == that.spill_slot;
  }
  bool operator!=(const CopySlot& that) const { return !(*this == that); }
};

// A single copy pair: src → dst, both at the same SizeClass.
struct CopyPair {
  CopySlot src;
  CopySlot dst;
  SizeClass size_class;

  bool IsIdentity() const { return src == dst; }
};

// ----------------------------------------------------------------------------
// Accessor helpers over a kParallelCopy HInstruction
// ----------------------------------------------------------------------------

// Number of copy pairs = operand_count / 2
inline int ParallelCopyPairCount(const HInstruction* ins) {
  DCHECK(ins->opcode() == HOpcode::kParallelCopy);
  DCHECK(ins->opr_size() % 2 == 0);
  return ins->opr_size() / 2;
}

// Source operand of pair #i
inline HOperand* ParallelCopySrc(const HInstruction* ins, int i) {
  DCHECK(ins->opcode() == HOpcode::kParallelCopy);
  DCHECK(i >= 0 && i < ParallelCopyPairCount(ins));
  return ins->Opr(2 * i);
}

// Destination operand of pair #i
inline HOperand* ParallelCopyDst(const HInstruction* ins, int i) {
  DCHECK(ins->opcode() == HOpcode::kParallelCopy);
  DCHECK(i >= 0 && i < ParallelCopyPairCount(ins));
  return ins->Opr(2 * i + 1);
}

// ----------------------------------------------------------------------------
// HAssembler factory
// ----------------------------------------------------------------------------

// Create a kParallelCopy instruction before |anchor| with the given pairs.
// Each pair's src and dst are wrapped into the corresponding HOperand kind
// (Register* or MemoryStack).  Duplicate destinations are rejected via
// CHECK (one-program-point invariant).
inline HInstruction* NewParallelCopyBefore(HAssembler* as,
                                           HInstruction* anchor,
                                           const zone::Vector<CopyPair>& pairs) {
  CHECK(as != nullptr);
  CHECK(anchor != nullptr);

  int n = static_cast<int>(pairs.size());

  // Check for duplicate destinations — the one-program-point invariant:
  // no two pairs may write to the same slot.
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      CHECK(pairs[i].dst != pairs[j].dst)
          << "Duplicate destination in parallel copy pair set";
    }
  }

  // Create instruction with 2*N operands.
  HInstruction* ins = as->NewInstructionBefore(
      nullptr, HOpcode::kParallelCopy,
      static_cast<std::size_t>(2 * n), anchor);

  // Populate each pair's source and destination operands.
  for (int i = 0; i < n; ++i) {
    const CopyPair& pair = pairs[i];

    // Source operand
    HOperand* src_opr = nullptr;
    if (pair.src.IsRegister()) {
      src_opr = as->NewRegister(pair.src.reg);
    } else {
      src_opr = as->NewMemStack(pair.src.spill_slot);
    }
    ins->OprSet(2 * i, src_opr);
    // The materialized-operand contract (operand.cc): a register/memory
    // operand on a post-RA stream must carry an instruction link for
    // Print()/htype() — the live codegen COMMENT path prints every
    // instruction. The copy instruction itself is the link (pair slots
    // are physical locations, not vreg values with a defining instruction).
    src_opr->set_instruction(ins);

    // Destination operand
    HOperand* dst_opr = nullptr;
    if (pair.dst.IsRegister()) {
      dst_opr = as->NewRegister(pair.dst.reg);
    } else {
      dst_opr = as->NewMemStack(pair.dst.spill_slot);
    }
    ins->OprSet(2 * i + 1, dst_opr);
    dst_opr->set_instruction(ins);
  }

  return ins;
}

// ----------------------------------------------------------------------------
// ParallelCopyResolver — textbook sequentialization with scratch-based
// cycle breaking per D-28 / D-30.
//
// Algorithm:
//   1. Drop identity pairs (src == dst).
//   2. Repeatedly emit every pending move whose source is not the
//      destination of any still-unemitted move.
//   3. When a pass emits nothing, a cycle exists:
//        - Pick one node in the cycle.
//        - Save its source into the scratch slot.
//        - Rewrite that node to read from scratch.
//        - Continue the outer loop.
//
// Spill-to-spill pairs are handled uniformly: when both sides are spill
// slots and the scratch is a register, the resolver emits spill→scratch
// then scratch→spill (two sequential moves).
// ----------------------------------------------------------------------------

// A single sequential step produced by the resolver.
struct ParallelCopyStep {
  enum Kind {
    kMove,            // ordinary move: src → dst
    kScratchSave,     // save cycle node into scratch: src → scratch
    kScratchRestore   // restore from scratch into dst: scratch → dst
  };

  Kind kind;
  CopySlot src;
  CopySlot dst;
  SizeClass size_class;
};

class ParallelCopyResolver {
 public:
  // Construct with the guaranteed scratch slot for this site.
  // The scratch must match the register bank (GP/FP) used for
  // cycle breaking and spill-to-spill bridging.
  ParallelCopyResolver(zone::Zone* zone, const CopySlot& scratch);

  // Resolve a set of parallel copy pairs into an order-correct sequential
  // move program.  The result is returned as a zone-allocated vector of
  // steps.
  zone::Vector<ParallelCopyStep> Resolve(const zone::Vector<CopyPair>& pairs);

 private:
  zone::Zone* zone_;
  CopySlot scratch_;
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_PARALLEL_COPY_H_
