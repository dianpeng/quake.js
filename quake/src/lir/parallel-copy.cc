#include "lir/parallel-copy.h"

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// ParallelCopyResolver — textbook sequentialization with scratch cycle breaking
//
// Algorithm
//   1. Drop identity pairs (src == dst) and validate the pair set:
//      destinations are unique (one-program-point invariant) and the scratch
//      slot does not participate in any pair
//   2. Repeatedly emit every pending move whose DESTINATION is not the source
//      of any still-pending move — i.e. nobody still needs that destination's
//      old value, so clobbering it now is safe.
//   3. When a full pass emits nothing, a cycle exists:
//        - Save one cycle node's source into the scratch slot.
//        - Rewrite that node's source to read from scratch.
//        - Continue — the cycle now drains through the scratch.
//      Each cycle breaks with exactly one scratch save; the rewritten move
//      (which reads the scratch back) is emitted with kScratchRestore.
//
// Spill-to-spill pairs: when both source and destination are spill
// slots, the pair is emitted as two moves through the scratch register
// (x64 has no mem-to-memory move).
// ----------------------------------------------------------------------------

ParallelCopyResolver::ParallelCopyResolver(zone::Zone* zone,
                                           const CopySlot& scratch)
    : zone_(zone), scratch_(scratch) {
  CHECK(zone_ != nullptr);
}

zone::Vector<ParallelCopyStep> ParallelCopyResolver::Resolve(
    const zone::Vector<CopyPair>& input_pairs) {
  zone::Vector<ParallelCopyStep> result(zone_);

  int n = static_cast<int>(input_pairs.size());

  // Validate the pair set (defensive — the NewParallelCopyBefore factory
  // CHECKs the same invariants, but the resolver is a public API usable
  // without it).
  for (int i = 0; i < n; ++i) {
    CHECK(input_pairs[i].src != scratch_ && input_pairs[i].dst != scratch_)
        << "parallel copy scratch slot must not participate in the pair set";
    for (int j = i + 1; j < n; ++j) {
      CHECK(input_pairs[i].dst != input_pairs[j].dst)
          << "duplicate destination in parallel copy pair set";
    }
  }

  // Working set of pending pairs (drop identity moves up front).
  zone::Vector<CopyPair> pending(zone_);
  pending.reserve(n);
  for (int i = 0; i < n; ++i) {
    if (!input_pairs[i].IsIdentity()) {
      pending.push_back(input_pairs[i]);
    }
  }

  // Outer loop: keep emitting until nothing is pending.
  while (!pending.empty()) {
    bool emitted_any = false;

    // Inner pass: emit every move whose destination is not still needed as a
    // source by another pending move.
    int i = 0;
    while (i < static_cast<int>(pending.size())) {
      const CopyPair& pair = pending[i];

      // Ready iff no OTHER pending move reads pair.dst's old value.
      bool dst_is_pending_src = false;
      for (int j = 0; j < static_cast<int>(pending.size()); ++j) {
        if (i == j) continue;
        if (pair.dst == pending[j].src) {
          dst_is_pending_src = true;
          break;
        }
      }

      if (!dst_is_pending_src) {
        // Emit this move — clobbering dst is safe now.
        if (pair.src.IsSpill() && pair.dst.IsSpill()) {
          // x64 has no mem-to-mem move — bridge through the scratch register.
          CHECK(scratch_.IsRegister())
              << "spill-to-spill parallel copy needs a register scratch";

          ParallelCopyStep load;
          load.kind = ParallelCopyStep::kMove;
          load.src = pair.src;
          load.dst = scratch_;
          load.size_class = pair.size_class;
          result.push_back(load);

          ParallelCopyStep store;
          store.kind = ParallelCopyStep::kMove;
          store.src = scratch_;
          store.dst = pair.dst;
          store.size_class = pair.size_class;
          result.push_back(store);
        } else {
          // A pair reading the scratch is the rewritten cycle breaker — the
          // move that reads the saved value back out of the scratch.
          ParallelCopyStep step;
          step.kind = (pair.src == scratch_)
                          ? ParallelCopyStep::kScratchRestore
                          : ParallelCopyStep::kMove;
          step.src = pair.src;
          step.dst = pair.dst;
          step.size_class = pair.size_class;
          result.push_back(step);
        }

        // Remove from pending (swap-with-back for O(1) removal; the element
        // swapped in is re-checked at the same index).
        if (i != static_cast<int>(pending.size()) - 1) {
          pending[i] = pending.back();
        }
        pending.pop_back();
        emitted_any = true;
      } else {
        ++i;
      }
    }

    if (!emitted_any) {
      // A cycle (or cycles) remain.  Save one cycle node's source into the
      // scratch slot and rewrite that node to read from scratch — exactly one
      // scratch save per cycle.
      CHECK(!pending.empty());

      CopyPair& first = pending[0];

      ParallelCopyStep save_step;
      save_step.kind = ParallelCopyStep::kScratchSave;
      save_step.src = first.src;
      save_step.dst = scratch_;
      save_step.size_class = first.size_class;
      result.push_back(save_step);

      // Rewrite the pair's source to be the scratch slot.  The scratch is not
      // any pending move's destination (validated above), and no pending move
      // reads it, so the saved value survives until the rewritten move emits
      // as a kScratchRestore.
      first.src = scratch_;
    }
  }

  return result;
}

}  // namespace lir
}  // namespace quake
