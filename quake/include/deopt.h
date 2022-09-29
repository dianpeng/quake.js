#ifndef _QUAKE_DEOPT_H_
#define _QUAKE_DEOPT_H_
#include "base.h"

namespace quake {

namespace ir {
class Checkpoint;
}  // namespace ir

enum class DeoptReason : std::uint8_t { kGuard };
enum class DeoptNext : std::uint8_t { kUnwind };

// ----------------------------------------------------------------------------
// A deoptimization entry resulted from Checkpoint. Notes, we do not store
// Checkpoint and all the IR/LIR are on zone and will be reclaimed after
// JIT
class Deopt {
 public:
  using DeoptId = std::uint32_t;  // deoptimization id, used to look up

  enum ValOrigin : std::uint8_t { kLocal, kParam, kTemp };

  // --------------------------------------------------------------------------
  // Live value existed when exit from a certain deoptimization point
  struct Val {
    ValOrigin origin;  // origin of the value, ie how to interpret the index
    int kind;          // same as JVal::Tag, used to box the value back
    int stack_index;   // index used to store the value back
    MachineRef ref;    // reference into machine resource/slot, register/stack
  };

  // --------------------------------------------------------------------------
  // All the function frame
  struct Frame {
    std::vector<Val> value_list;

    /** added based on quickjs.c's frame logic **/
    void* bytecode_object;
    void* next_bytecode;
  };

 public:
  Deopt(DeoptId id, DeoptReason reason, DeoptNext next)
      : deopt_id_(id), reason_(reason), next_(next) {}

  DeoptId deopt_id() const { return deopt_id_; }
  DeoptReason reason() const { return reason_; }
  DeoptNext next() const { return next_; }

  const std::vector<Frame>& frame() const { return frame_; }

  void From(const ir::Checkpoint&);

 private:
  DeoptId deopt_id_;
  DeoptReason reason_;
  DeoptNext next_;

  //  list of (inlined) frame
  std::vector<Frame> frame_;
};

}  // namespace quake

#endif  // _QUAKE_DEOPT_H_
