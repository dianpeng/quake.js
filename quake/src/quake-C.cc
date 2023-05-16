#include "base.h"
#include "quake-C.h"
#include "quake.h"
#include "jit.h"
#include "flags.h"

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

void QuakeEnterCall(JSFunctionBytecode* bfunc, QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  quake->IncTraceLevel(bfunc);
}

int QuakeLeaveCall(JSFunctionBytecode* bfunc, QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  auto [done, del] = quake->DecTraceLevel(bfunc);
  DEFER([&]() {
    if (del) delete quake;
  });

  if (quake->jit_state() == Quake::kJitUp) {
    return 1;
  } else {
    return done;
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

void QuakeNewQFunction(void* bptr, JSContext* jsc) {
  auto b = static_cast<JSFunctionBytecode*>(bptr);
  auto qfunc = &b->jit_world;

  std::memset(qfunc->jit_hc, 0, sizeof(qfunc->jit_hc));
  qfunc->js_context = jsc;
  qfunc->impl = new QuakeWorld(b);
}

void QuakeFreeQFunction(void* bptr) {
  auto b = static_cast<JSFunctionBytecode*>(bptr);
  auto qfunc = &b->jit_world;
  if (qfunc->impl) {
    auto world = static_cast<QuakeWorld*>(qfunc->impl);
    CHECK(world->function_bytecode() == b);
    delete world;
    qfunc->impl = nullptr;
  }
}

QQuake* QuakeOnLoopBack(JSFunctionBytecode* bfunc, QFunction* qfunc,
                        const void* loop_back, const void* target_bc,
                        int in_jit) {
  auto world = static_cast<QuakeWorld*>(qfunc->impl);
  if (auto jit = world->GetQuakeByLoopbackBC(loop_back); jit) {
    if (jit->expose()) {
      return reinterpret_cast<QQuake*>(jit.get());
    } else {
      return nullptr;
    }
  } else if (!in_jit) {
    auto q = new Quake(world, qfunc->js_context);
    q->IncTraceLevel(bfunc);

    q->set_trigger_loop_back_bc(loop_back);
    return reinterpret_cast<QQuake*>(q);
  } else {
    return nullptr;
  }
}

QQuake* QuakeOnCall(JSFunctionBytecode* bfunc, QFunction* qfunc,
                    const void* call) {
  if (FLAG_allow_jit_function.Get()) {
    auto world = static_cast<QuakeWorld*>(qfunc->impl);

    if (auto jit = world->GetQuakeByFirstBC(call); jit) {
      if (jit->expose()) {
        return reinterpret_cast<QQuake*>(jit.get());
      } else {
        return nullptr;
      }
    } else {
      auto q = new Quake(world, qfunc->js_context, TraceType::kFunction);
      q->IncTraceLevel(bfunc);

      q->set_trigger_loop_from_bc(call);
      return reinterpret_cast<QQuake*>(q);
    }
  } else {
    return nullptr;
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
