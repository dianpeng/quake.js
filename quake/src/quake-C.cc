#include "base.h"
#include "quake-C.h"
#include "quake.h"
#include "jit.h"

#include "ir/interp-info.h"
#include "ir/bytecode.h"
#include "ir/graph.h"
#include "ir/builder.h"

#include "lir/assembler.h"
#include "lir/lir-lower.h"
#include "lir/register-allocator.h"
#include "lir/pre-code-gen.h"
#include "lir/rewriter.h"
#include "lir/post-ra-optimization.h"
#include "lir/cfg.h"

#include "ir/optimize.h"

#include "cg/x64/code-gen.h"

#include <cstdlib>

// C++ object
#include "quake.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

using namespace quake;

static int QMapErrorTraceResult(const ir::TraceResult& tr) {
  switch (tr.reason()) {
    case ir::BailoutReason::kDone:
      return kQDone;

    case ir::BailoutReason::kStop:
      return kQStop;

    case ir::BailoutReason::kNYI:
      return kQNYI;

    case ir::BailoutReason::kBytecodeOverflow:
      return kQBytecodeOverflow;

    case ir::BailoutReason::kBytecodeError:
      return kQBytecodeError;

    case ir::BailoutReason::kReferenceError:
      return kQReferenceError;

    default:
      UNREACHABLE();
      return kQStop;
  }
}

static int QMapErrorCompile(const base::Error& error) {
  if (error) {
    return kQCompileError;
  } else {
    return kQDone;
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

int QuakeSetup(QQuake* q, QInterpInfo* qbinfo) {
  auto quake = reinterpret_cast<Quake*>(q);
  ir::InterpInfoQJS interp_info{quake, qbinfo};
  quake->NewIRTrace();
  return QMapErrorTraceResult(quake->ir_builder()->Setup(&interp_info));
}

int QuakeTrace(QQuake* q, QInterpInfo* qbinfo) {
  auto quake = reinterpret_cast<Quake*>(q);
  CHECK(quake->HasIRBuilder());
  ir::InterpInfoQJS interp_info{quake, qbinfo};
  if (Jit::GetInstance().option_trace_jit()) {
    LOG(TRACE) << "bytecode: " << interp_info.op_name() << "@"
               << interp_info.start_pc();
  }

  return QMapErrorTraceResult(quake->ir_builder()->Trace(&interp_info));
}

int QuakeStop(QQuake* q, QInterpInfo* qbinfo) {
  auto quake = reinterpret_cast<Quake*>(q);
  CHECK(quake->HasIRBuilder());
  ir::InterpInfoQJS interp_info{quake, qbinfo};
  return QMapErrorTraceResult(quake->ir_builder()->Stop(&interp_info));
}

int QuakeCompile(QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  DEFER([&]() {
    quake->FreeIRBuilder();
    quake->FreeIRGraph();
  });

  auto g = quake->ir_graph();
  lir::HAssembler as{quake};

  // (0) FIXME(dpeng): IR construction
  if (Jit::GetInstance().option_trace_jit()) {
    LOG(TRACE) << "====================================================";
    LOG(TRACE) << g->Print();
  }

  // (1) FIXME(dpeng): IR optimization
  //
  //     Notes this may fail, ie certain constant folding may yield result that
  //     cannot jit or always deoptimize
  if (!IROptimize(quake, g)) {
    return kQBytecodeError;
  }

  // (2) Translate the IR to LIR
  {
    lir::LIRLower trans{quake, &as};
    trans.Run(g);

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (3) Rewrite the result
  {
    lir::Rewriter::Rewrite(quake, &as);

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (4) Register allocation
  {
    lir::RegisterAllocator ra{quake, &as};
    ra.Run();

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (5) Post RA optimization
  {
    lir::PostRAOptimization pra_opt{quake};
    pra_opt.Run(&as);

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (6) Pre code generation pass
  {
    lir::PreCodeGen pcg{quake, &as};
    pcg.Run();

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (7) Code generation
  {
    cg::x64::CodeGen cg{quake, &as};
    if (auto err = cg.Generate(); err) {
      return QMapErrorCompile(std::move(err));
    }

    quake->set_code(cg.DetachCode());

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << quake->code().Disassemble();
    }
  }

  // (8) Setup all the information
  { quake->set_trigger_loop_from_bc(quake->ir_builder()->start_bc()); }

  return kQDone;
}

int QuakeIsJitComplete(QQuake* q) {
  auto quake = reinterpret_cast<Quake*>(q);
  return quake->jit_state() == Quake::kJitUp;
}

int QuakeRun(QQuake* q, QuakeRT* ctx) {
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
    return reinterpret_cast<QQuake*>(jit);
  } else {
    auto q = new Quake(world, static_cast<QJS*>(qfunc->qjs), qfunc->js_context);
    q->set_trigger_loop_back_bc(loop_back);
    return reinterpret_cast<QQuake*>(q);
  }
}

int QuakeStepTraceMachine(QFunction* qfunc, QQuake* quake, QInterpInfo* info) {
  auto world = static_cast<QuakeWorld*>(qfunc->impl);
  auto q = reinterpret_cast<Quake*>(quake);
  switch (q->jit_state()) {
    case Quake::kJitStart:
      if (auto x = QuakeSetup(quake, info); x != kQDone) {
        q->set_jit_state(Quake::kJitFail);
        return x;
      }
      q->set_jit_state(Quake::kJitTrace);
      // fallthrough
    case Quake::kJitTrace: {
      switch (auto x = QuakeTrace(quake, info)) {
        case kQDone:
          return kQDone;

        case kQStop:
          q->set_jit_state(Quake::kJitStop);
          break;

        default:
          q->set_jit_state(Quake::kJitFail);
          return x;
      }
    }
      // fallthrough
    case Quake::kJitStop:
      if (auto x = QuakeStop(quake, info); x != kQDone) {
        q->set_jit_state(Quake::kJitFail);
        return x;
      }
      q->set_jit_state(Quake::kJitCompile);
      // fallthrough
    case Quake::kJitCompile:
      if (auto x = QuakeCompile(quake); x != kQDone) {
        q->set_jit_state(Quake::kJitFail);
        return x;
      }
      LOG(ERROR) << "Exit Size: " << q->GetLoopExitSize();
      q->set_jit_state(Quake::kJitUp);
      break;

    default:
      UNREACHABLE();
      break;
  }

  // when we reaching here, it means the code has been compiled and we just
  // need to insert the quake back to the queue for later searching
  CHECK(q->HasCode());
  world->AddQuake(q);

  return kQDone;
}

#ifdef __cplusplus
}
#endif  // __cplusplus
