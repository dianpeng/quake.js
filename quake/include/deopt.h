#ifndef _QUAKE_DEOPT_H_
#define _QUAKE_DEOPT_H_
#include "base.h"

namespace quake {

namespace ir {
class Checkpoint;
}  // namespace ir

enum class DeoptimizationReason : std::uint8_t { kGuard };

enum class DeoptimizationNext : std::uint8_t { kUnwind };

// ----------------------------------------------------------------------------
// A deoptimization entry resulted from Checkpoint. Notes, we do not store
// Checkpoint and all the IR/LIR are on zone and will be reclaimed after
// JIT
class Deoptimization {
 public:
  using DeoptId = std::uint32_t;  // deoptimization id, used to look up

  enum ValKind { kLocal, kParam, kTemp };

  struct Val {
    ValKind kind;
    int stack_index;
    MachineRef ref;
  };

  struct Frame {
    std::vector<Val> val_list;
    void* bytecode_object;
    void* next_bytecode;

    /** added based on quickjs.c's frame logic **/
  };

  Deoptimization(DeoptId id, DeoptimizationReason reason,
                 DeoptimizationNext next)
      : deopt_id_(id), reason_(reason), next_(next) {}

  DeoptId deopt_id() const { return deopt_id_; }
  DeoptimizationReason reason() const { return reason_; }
  DeoptimizationNext next() const { return next_; }

  const std::vector<Frame>& frame() const { return frame_; }

  void From(const ir::Checkpoint&);

 private:
  DeoptId deopt_id_;
  DeoptimizationReason reason_;
  DeoptimizationNext next_;

  //  list of (inlined) frame
  std::vector<Frame> frame_;
};

}  // namespace quake

#endif  // _QUAKE_DEOPT_H_
