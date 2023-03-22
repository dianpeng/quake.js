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

void QuakeFree(QQuake* q) {
  if (auto quake = reinterpret_cast<Quake*>(q);
      quake->jit_state() != Quake::kJitUp) {
    delete quake;
  }
}

void QuakeFreeFrame(JSStackFrame* f) { ::free(f); }

int QuakeIsJitComplete(QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  return quake->jit_state() == Quake::kJitUp;
}

void QuakeEnterCall(QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  quake->IncTraceLevel();
}

int QuakeLeaveCall(QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  auto result = quake->DecTraceLevel();
  if (quake->jit_state() == Quake::kJitUp) {
    return 1;
  } else {
    if (result) {
      delete quake;
    }
    return result;
  }
}

int QuakeRun(QQuake* q, QuakeRT* ctx) {
  // setup the initial quake(trace) in the context, this field will be mutated
  // during the jit trace linking
  ctx->jit.quake = q;

  // setup the initial trace level, ie 0
  ctx->jit.trace_level = 0;

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
                        const void* target_bc, int in_jit) {
  auto world = static_cast<QuakeWorld*>(qfunc->impl);
  if (auto jit = world->GetQuakeByLoopbackBC(loop_back); jit) {
    if (jit->expose()) {
      return reinterpret_cast<QQuake*>(jit.get());
    } else {
      return nullptr;
    }
  } else if (!in_jit) {
    auto q = new Quake(world, static_cast<QJS*>(qfunc->qjs), qfunc->js_context);
    q->IncTraceLevel();

    q->set_trigger_loop_back_bc(loop_back);
    return reinterpret_cast<QQuake*>(q);
  } else {
    return nullptr;
  }
}

QQuake* QuakeOnCall(QFunction* qfunc, const void* call) {
  auto world = static_cast<QuakeWorld*>(qfunc->impl);
  if (auto jit = world->GetQuakeByFirstBC(call); jit) {
    if (jit->expose()) {
      return reinterpret_cast<QQuake*>(jit.get());
    } else {
      return nullptr;
    }
  } else {
    auto q = new Quake(world, static_cast<QJS*>(qfunc->qjs), qfunc->js_context,
                       TraceType::kFunction);
    q->IncTraceLevel();

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
