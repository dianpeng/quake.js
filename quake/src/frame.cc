#include "frame.h"

#include <cstdlib>

namespace quake {

RecoveryFrame* NewRecoveryFrame(std::size_t slot_size, std::size_t param_size,
                                std::size_t local_size) {
  auto buf = ::malloc(sizeof(RecoveryFrame) + sizeof(JSValue) * slot_size);

  auto frame = static_cast<RecoveryFrame*>(buf);

  frame->slot =
      reinterpret_cast<JSValue*>(reinterpret_cast<char*>(buf) + sizeof(frame));
  frame->slot_size = slot_size;
  frame->param = frame->slot;
  frame->local = frame->slot + param_size;
  frame->temp = frame->local + local_size;

  frame->func_object = JS_UNINITIALIZED;
  frame->this_object = JS_UNINITIALIZED;
  frame->target_object = JS_UNINITIALIZED;

  return frame;
}

}  // namespace quake
