#include "base.h"
#include "quake-C.h"
#include "quake.h"
#include "jit.h"

#include "ir/interp-info.h"

#include <cstdlib>

// C++ object
#include "quake.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

using namespace quake;

static int MapCompilerStatus(const Quake::CompilerStatus st) {
  switch (st) {
    case Quake::CompilerStatus::kDone:
      return kQDone;
    case Quake::CompilerStatus::kStop:
      return kQStop;
    case Quake::CompilerStatus::kNYI:
      return kQNYI;
    case Quake::CompilerStatus::kBytecodeOverflow:
      return kQBytecodeOverflow;
    case Quake::CompilerStatus::kBytecodeError:
      return kQBytecodeError;
    case Quake::CompilerStatus::kReferenceError:
      return kQReferenceError;

    default:
      UNREACHABLE();
      return kQStop;
  }
}

QRecoveryFrame* QuakeNewQRecoveryFrame(std::size_t slot_size,
                                       std::size_t param_size,
                                       std::size_t local_size) {
  auto buf = ::malloc(sizeof(QRecoveryFrame) + sizeof(JSValue) * slot_size);
  auto frame = static_cast<QRecoveryFrame*>(buf);
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

void QuakeFree(QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  delete quake;
}

int QuakeIsJitComplete(QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  return quake->jit_state() == Quake::kJitUp;
}

int QuakeRun(QQuake* q, QuakeRT* ctx) {
  // setup the initial quake(trace) in the context, this field will be mutated
  // during the jit trace linking
  ctx->quake = q;

  // setup the initial trace level, ie 0
  ctx->trace_level = 0;

  auto quake = reinterpret_cast<Quake*>(q);
  CHECK(quake->HasCode());
  return quake->code().entry()(ctx);
}

void QuakeNewQFunction(QFunction* qfunc, QJS* qjs, JSContext* jsc) {
  std::memset(qfunc->jit_hc, 0, sizeof(qfunc->jit_hc));
  qfunc->qjs = qjs;
  qfunc->js_context = jsc;
  qfunc->impl = new QuakeWorld();
}

void QuakeFreeQFunction(QFunction* qfunc) {
  if (qfunc->impl) {
    delete static_cast<QuakeWorld*>(qfunc->impl);
    qfunc->impl = nullptr;
  }
}

QQuake* QuakeOnLoopBack(QFunction* qfunc, const void* loop_back,
                        const void* target_bc) {
  auto world = static_cast<QuakeWorld*>(qfunc->impl);
  if (auto jit = world->GetQuakeByLoopbackBC(loop_back); jit) {
    if (jit->expose()) {
      return reinterpret_cast<QQuake*>(jit.get());
    } else {
      return nullptr;
    }
  } else {
    auto q = new Quake(world, static_cast<QJS*>(qfunc->qjs), qfunc->js_context);
    q->set_trigger_loop_back_bc(loop_back);
    return reinterpret_cast<QQuake*>(q);
  }
}

int QuakeStepTraceMachine(QFunction* qfunc, QQuake* quake, QInterpInfo* info) {
  auto q = reinterpret_cast<Quake*>(quake);
  ir::InterpInfoQJS interp_info{q, info};
  return MapCompilerStatus(q->CompilerStep(&interp_info));
}

#ifdef __cplusplus
}
#endif  // __cplusplus
