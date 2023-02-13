#include "quake.h"
#include "jit.h"

#include "ir/builder.h"
#include "ir/graph.h"
#include "ir/optimize.h"
#include "ir/interp-info.h"

#include "lir/assembler.h"
#include "lir/lir-lower.h"
#include "lir/register-allocator.h"
#include "lir/pre-code-gen.h"
#include "lir/rewriter.h"
#include "lir/post-ra-optimization.h"
#include "lir/cfg.h"

#include "cg/x64/code-gen.h"

namespace quake {

std::string Quake::Print() const {
  std::stringstream fmt;

  fmt << "TraceId: " << trace_id() << "\n";
  fmt << "LastDeoptimizationId: " << last_deoptimization_id() << "\n";
  fmt << "DeoptTimes: " << deopt_times() << "\n";
  fmt << "Range(" << trigger_loop_from_bc() << " - " << trigger_loop_back_bc()
      << ")\n";
  fmt << "TraceType: " << trace_type_name() << "\n";
  fmt << "Code:\n\n---------------------\n";
  fmt << debug();
  fmt << "\n---------------------\n\n";
  return fmt.str();
}

const char* GetTraceTypeName(TraceType tt) {
  switch (tt) {
    case TraceType::kLoop:
      return "loop";
    case TraceType::kSideTrace:
      return "side-trace";
    case TraceType::kFunction:
      return "function";
    default:
      UNREACHABLE();
      return "";
  }
}

DeoptId Quake::AddDeopt(DeoptReason reason, DeoptNext next,
                        lir::Snapshot&& snap) {
  auto deopt_id = static_cast<DeoptId>(deopt_list_.size());
  deopt_list_.emplace_back(deopt_id, reason, next);
  deopt_list_.back().From(std::move(snap));
  return deopt_id;
}

std::size_t Quake::GetLoopExitSize() const {
  std::size_t cnt = 0;
  for (const auto& d : deopt_list_) {
    if (d.reason() == DeoptReason::kLoopExit) {
      ++cnt;
    }
  }
  return cnt;
}

ir::TraceResult Quake::CompilerSetup(ir::InterpInfo* info) {
  NewIRTrace();
  return ir_builder_->Setup(info);
}

ir::TraceResult Quake::CompilerTrace(ir::InterpInfo* info) {
  if (Jit::GetInstance().option_trace_jit()) {
    LOG(ERROR) << "bytecode( " << info->op_name() << "@" << info->start_pc()
               << ")";
  }
  return ir_builder_->Trace(info);
}

ir::TraceResult Quake::CompilerStop(ir::InterpInfo* info) {
  return ir_builder_->Stop(info);
}

base::Error Quake::CompilerCompile() {
  DEFER([&]() {
    FreeIRBuilder();
    FreeIRGraph();
  });

  auto g = ir_graph();
  lir::HAssembler as{this};

  // (0) FIXME(dpeng): IR construction
  if (Jit::GetInstance().option_trace_jit()) {
    LOG(TRACE) << "====================================================";
    LOG(TRACE) << g->Print();
  }

  // (1) FIXME(dpeng): IR optimization
  //
  //     Notes this may fail, ie certain constant folding may yield result
  //     that cannot jit or always deoptimize
  if (!IROptimize(this, g)) {
    return base::Error::Mk("IROptimization failed: bytecode");
  }

  // (2) Translate the IR to LIR
  {
    lir::LIRLower trans{this, &as};
    trans.Run(g);

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (3) Rewrite the result
  {
    lir::Rewriter::Rewrite(this, &as);

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (4) Register allocation
  {
    lir::RegisterAllocator ra{this, &as};
    ra.Run();

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (5) Post RA optimization
  {
    lir::PostRAOptimization pra_opt{this};
    pra_opt.Run(&as);

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (6) Pre code generation pass
  {
    lir::PreCodeGen pcg{this, &as};
    pcg.Run();

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << as.Print();
    }
  }

  // (7) Code generation
  {
    cg::x64::CodeGen cg{this, &as};
    if (auto err = cg.Generate(); err) {
      return err;
    }

    set_code(cg.DetachCode());

    if (Jit::GetInstance().option_trace_jit()) {
      LOG(TRACE) << "====================================================";
      LOG(TRACE) << code().Disassemble();
    }
  }

  // (8) Setup all the information
  { set_trigger_loop_from_bc(ir_builder()->start_bc()); }

  return {};
}

Quake::CompilerStatus Quake::MapTraceResult(const ir::TraceResult& tr) {
  switch (tr.reason()) {
    case ir::BailoutReason::kNYI:
      return CompilerStatus::kNYI;
    case ir::BailoutReason::kBytecodeOverflow:
      return CompilerStatus::kBytecodeOverflow;
    case ir::BailoutReason::kBytecodeError:
      return CompilerStatus::kBytecodeError;
    case ir::BailoutReason::kReferenceError:
      return CompilerStatus::kReferenceError;
    case ir::BailoutReason::kDone:
      return CompilerStatus::kDone;
    case ir::BailoutReason::kStop:
      return CompilerStatus::kStop;
    case ir::BailoutReason::kLoopExit:
      return CompilerStatus::kLoopExit;
    case ir::BailoutReason::kInnerLoop:
      return CompilerStatus::kInnerLoop;

    default:
      UNREACHABLE();
      return CompilerStatus::kDone;
  }
}

Quake::CompilerStatus Quake::CompilerStep(ir::InterpInfo* info) {
  switch (jit_state()) {
    case Quake::kJitStart:
      if (Jit::GetInstance().option_trace_jit()) {
        LOG(TRACE) << "====================================================";
        LOG(TRACE) << "Start trace";
        LOG(TRACE) << info->Function().Debug();
      }

      if (auto tr = CompilerSetup(info); !tr) {
        LOG(ERROR) << "jit_fail: " << tr.Print();
        set_jit_state(Quake::kJitFail);
        return MapTraceResult(tr);
      }
      set_jit_state(Quake::kJitTrace);

      // fallthrough
    case Quake::kJitTrace: {
      auto tr = CompilerTrace(info);
      switch (tr.reason()) {
        case ir::BailoutReason::kDone:
          return CompilerStatus::kDone;

        case ir::BailoutReason::kStop:
          set_jit_state(Quake::kJitStop);
          break;

        default:
          set_jit_state(Quake::kJitFail);
          LOG(ERROR) << "jit_fail: " << tr.Print();
          return MapTraceResult(tr);
      }
    }
      // fallthrough

    case Quake::kJitStop:
      if (auto tr = CompilerStop(info); !tr) {
        LOG(ERROR) << "jit_fail: " << tr.Print();
        set_jit_state(Quake::kJitFail);
        return MapTraceResult(tr);
      }
      set_jit_state(Quake::kJitCompile);
      // fallthrough

    case Quake::kJitCompile:
      if (auto err = CompilerCompile(); err) {
        LOG(ERROR) << "jit_fail: " << err.GetDisplay();
        set_jit_state(Quake::kJitFail);
        return CompilerStatus::kCompileError;
      }

      set_jit_state(Quake::kJitUp);
      break;

    default:
      UNREACHABLE();
      break;
  }

  // when we reaching here, it means the code has been compiled and we just
  // need to insert the quake back to the queue for later searching
  CHECK(HasCode());

  if (IsEntry()) {
    world()->AddQuake(this);
  }

  return CompilerStatus::kDone;
}

void Quake::NewIRTrace() {
  ir_graph_.reset(new ir::Graph(this));
  ir_builder_.reset(new ir::Builder(this, ir_graph_.get()));
}

void Quake::FreeIRGraph() { ir_graph_.reset(); }

void Quake::FreeIRBuilder() { ir_builder_.reset(); }

void* Quake::AddConstPoolString(const std::string_view& str) {
  auto js_str = JS_NewStringLen(js_context(), str.data(), str.size());
  auto ptr = JS_VALUE_GET_PTR(js_str);
  auto qjs = JVal{js_context(), js_str};

  cpool_string_.push_back(std::move(qjs));
  return ptr;
}

void Quake::OnDeopt(DeoptId did) {
  ++deopt_times_;
  last_deoptimization_id_ = did;
  world()->OnDeopt(trace_id(), did);
}

Quake::Quake(QuakeWorld* world, QJS* qjs, JSContext* js_context, TraceType tt)
    : qjs_(qjs),
      js_context_(js_context),
      world_(world),

      trace_id_(kPendingTraceId),
      last_deoptimization_id_(kInvalidDeoptId),

      zone_(1024 * 16, 1024 * 128),
      code_(),
      trace_type_(tt),
// clang-format off

/** static string initialization **/
#define __(A, B) A##_(JVal{js_context_, JS_NewString(js_context_, B)}),
      QUAKE_QUAKE_STATIC_STRING_LIST(__)
#undef __  // __

      // clang-format on

      dummy_(false) {
}

Quake::~Quake() {
  LOG(ERROR) << "deopt_times: " << deopt_times();
}

std::shared_ptr<Quake> QuakeWorld::GetQuakeByFirstBC(const void* bc) const {
  auto itr = std::find_if(trace_list_.begin(), trace_list_.end(),
                          [&](const auto& q) -> bool {
                            return q && q->trigger_loop_from_bc() == bc;
                          });
  return itr == trace_list_.end() ? std::shared_ptr<Quake>{} : *itr;
}

std::shared_ptr<Quake> QuakeWorld::GetQuakeByLoopbackBC(const void* bc) const {
  auto itr = std::find_if(trace_list_.begin(), trace_list_.end(),
                          [&](const auto& q) -> bool {
                            return q && q->trigger_loop_back_bc() == bc;
                          });
  return itr == trace_list_.end() ? std::shared_ptr<Quake>{} : *itr;
}

}  // namespace quake
