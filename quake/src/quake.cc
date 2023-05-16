#include "quake.h"
#include "jit.h"
#include "flags.h"
#include "quake-C.h"

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

#define MAYBE_TRACE(FLAG, NAME, WHAT)                 \
  do {                                                \
    if (FLAG.Get()) {                                 \
      LOG(TRACE) << "********************** " << NAME \
                 << " ******************";            \
      LOG(TRACE) << WHAT;                             \
    }                                                 \
  } while (false)

namespace quake {

void SetInlineQuakeTrace(JSContext* js_context, Quake* q) {
  if (js_context->jit_parent) {
    CHECK(js_context->jit_parent == q);
  } else {
    js_context->jit_parent = q;
  }
}

void ClearInlineQuakeTrace(JSContext* js_context) {
  if (js_context->jit_parent) {
    js_context->jit_parent = nullptr;
  }
}

std::string Quake::Print() const {
  std::stringstream fmt;

  fmt << "-------------------------------------------------------------\n";
  fmt << "TraceId: " << trace_id() << "\n";
  fmt << "LastDeoptimizationId: " << last_deoptimization_id() << "\n";
  fmt << "DeoptTimes: " << deopt_times() << "\n";
  fmt << "Range(" << trigger_loop_from_bc() << " - " << trigger_loop_back_bc()
      << ")\n";
  fmt << "TraceType: " << trace_type_name() << "\n";
  fmt << "Code:\n\n---------------------\n";
  fmt << debug();
  fmt << "\n-------------------------------------------------------------\n\n";
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

void Quake::IncTraceLevel(void* bfunc) {
  if (auto itr = trace_level_.find(bfunc); itr != trace_level_.end()) {
    itr->second++;
  } else {
    trace_level_[bfunc] = 1;
  }
}

std::pair<bool, bool> Quake::DecTraceLevel(void* bfunc) {
  auto caller_done = false;
  auto should_del = false;

  if (auto itr = trace_level_.find(bfunc); itr != trace_level_.end()) {
    itr->second--;
    if (itr->second == 0) {
      trace_level_.erase(itr);
      caller_done = true;
    }

    should_del = trace_level_.empty();
  } else {
    UNREACHABLE();
  }

  return std::make_pair(caller_done, should_del);
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

  MAYBE_TRACE(FLAG_trace_compile_info, "Trace Start", info->Function().Debug());
  auto result = ir_builder_->Setup(info);
  if (!result) {
    OnTraceError(result);
    ClearInlineQuakeTrace(js_context_);
  }
  return result;
}

ir::TraceResult Quake::CompilerTrace(ir::InterpInfo* info) {
  if (FLAG_trace_compile_bytecode.Get()) {
    LOG(TRACE) << "@" << info->start_pc() << ": "
               << ir::BytecodePrint(info->start_pc());
  }

  auto result = ir_builder_->Trace(info);
  switch (result.reason()) {
    case ir::BailoutReason::kDone:
    case ir::BailoutReason::kStop:
      break;

    default:
      OnTraceError(result);
      ClearInlineQuakeTrace(js_context_);
      break;
  }
  return result;
}

ir::TraceResult Quake::CompilerStop(ir::InterpInfo* info) {
  auto result = ir_builder_->Stop(info);
  if (!result) {
    OnTraceError(result);
    ClearInlineQuakeTrace(js_context_);
  }

  return result;
}

base::Error Quake::DoCompile() {
  DEFER([&]() {
    ir_builder_.reset();
    ir_graph_.reset();
    zone_.Clear();
  });

  auto g = ir_graph();
  lir::HAssembler as{this};

  // (0) IR consturction
  MAYBE_TRACE(FLAG_trace_compile_ir, "IR Construction", g->PrintCompact());

  // (1) IR optimization
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
    MAYBE_TRACE(FLAG_trace_compile_lir_lower, "LIR Lowering", as.Print());
  }

  // (3) Rewrite the result
  {
    lir::Rewriter::Rewrite(this, &as);
    MAYBE_TRACE(FLAG_trace_compile_lir_rewrite, "LIR Rewrite", as.Print());
  }

  // (4) Register allocation
  {
    lir::RegisterAllocator ra{this, &as};
    ra.Run();
    MAYBE_TRACE(FLAG_trace_compile_lir_register_allocation, "LIR RegAlloc",
                as.Print());
  }

  // (5) Post RA optimization
  {
    lir::PostRAOptimization pra_opt{this};
    pra_opt.Run(&as);

    MAYBE_TRACE(FLAG_trace_compile_lir_post_ra_optimization,
                "LIR Post RA Optimization", as.Print());
  }

  // (6) Pre code generation pass
  {
    lir::PreCodeGen pcg{this, &as};
    pcg.Run();
    MAYBE_TRACE(FLAG_trace_compile_pre_code_generation, "Pre Code Generation",
                as.Print());
  }

  // (7) Code generation
  {
    cg::x64::CodeGen cg{this, &as};
    if (auto err = cg.Generate(); err) {
      return err;
    }

    set_code(cg.DetachCode());
    MAYBE_TRACE(FLAG_trace_compile_code_generation, "Code Generation",
                code().Disassemble());
  }

  // (8) Clean up job
  set_trigger_loop_from_bc(ir_builder()->start_bc());
  return {};
}

base::Error Quake::CompilerCompile() {
  auto err = DoCompile();
  if (err) OnCompileError(err);

  ClearInlineQuakeTrace(js_context_);
  return err;
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

void Quake::OnTraceError(const ir::TraceResult& tr) {
  switch (tr.reason()) {
#define __(A, ...)                                          \
  case ir::BailoutReason::A:                                \
    if (world()->AddPenalty(trigger_bc(), QuakeWorld::A)) { \
      world()->DisableJit(trigger_bc());                    \
    }                                                       \
    break;

    QUAKE_IR_REAL_BAILOUT_REASON(__)
#undef __  // __

    default:
      UNREACHABLE() << "unknown trace result: " << tr.Print();
      break;
  }
}

void Quake::OnCompileError(const base::Error& e) {
  if (world()->AddPenalty(trigger_bc(), QuakeWorld::kCompilePenalty)) {
    world()->DisableJit(trigger_bc());
  }
}

const char* Quake::GetJitStateName(JitState js) {
  // clang-format off
  switch (js) {
#define __(A, B) \
  case A:        \
    return B;
    QUAKE_JIT_STATE(__)
#undef __  // __
  default:
    UNREACHABLE();
    return "";
  }
  // clang-format on
}

void Quake::set_jit_state(JitState js) {
  if (FLAG_trace_compile_info.Get()) {
    LOG(TRACE) << "----------------------------------------------------------";
    LOG(TRACE) << "jit state change (" << GetJitStateName(jit_state_) << " -> "
               << GetJitStateName(js) << ")";
  }
  jit_state_ = js;
  if (js == kJitUp) {
    world()->ClearPenalty(trigger_bc());
  }
}

Quake::CompilerStatus Quake::CompilerStep(ir::InterpInfo* info) {
  switch (jit_state()) {
    case Quake::kJitStart:
      if (auto tr = CompilerSetup(info); !tr) {
        set_jit_state(Quake::kJitFail);
        return MapTraceResult(tr);
      } else {
        set_jit_state(Quake::kJitTrace);
      }
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
          return MapTraceResult(tr);
      }
      // fallthrough
    }

    case Quake::kJitStop: {
      if (auto tr = CompilerStop(info); !tr) {
        set_jit_state(Quake::kJitFail);
        return MapTraceResult(tr);
      } else {
        set_jit_state(Quake::kJitCompile);
      }
      // fallthrough
    }

    case Quake::kJitCompile: {
      if (auto err = CompilerCompile(); err) {
        set_jit_state(Quake::kJitFail);
        return CompilerStatus::kCompileError;
      } else {
        set_jit_state(Quake::kJitUp);
      }
      break;
    }

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

const void* Quake::trigger_bc() const {
  switch (trace_type()) {
    case TraceType::kLoop:
      CHECK(trigger_loop_back_bc());

      // The loop is enterred at backedge
      return trigger_loop_back_bc();

    default:
      CHECK(trigger_loop_from_bc());

      // For function or side trace, the jitted code is been enterred via first
      // BC directly
      return trigger_loop_from_bc();
  }
}

Quake::Quake(QuakeWorld* world, JSContext* js_context, TraceType tt)
    : js_context_(js_context),
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

Quake::~Quake() {}

/** ---------------------------------------------------------------------------
 ** QuakeWorld Implementation
 ** --------------------------------------------------------------------------*/
QuakeWorld::PenaltyList::iterator QuakeWorld::FindPenalty(
    const void* bc, std::optional<PenaltyType> t) {
  auto pred = [&](const PenaltyInfo& pinfo) -> bool {
    if (pinfo.bc == bc) {
      if (t) {
        return *t == pinfo.type;
      } else {
        return true;
      }
    }
    return false;
  };

  return std::find_if(penalty_.begin(), penalty_.end(), std::move(pred));
}

QuakeWorld::BuildPenaltyList::iterator QuakeWorld::FindBuildPenalty(
    const void* bc) {
  auto pred = [&](const BuildPenaltyInfo& pinfo) -> bool {
    return pinfo.bc == bc;
  };

  return std::find_if(build_penalty_.begin(), build_penalty_.end(),
                      std::move(pred));
}

QuakeWorld::PenaltyList::const_iterator QuakeWorld::FindPenalty(
    const void* bc, std::optional<PenaltyType> t) const {
  auto pred = [&](const PenaltyInfo& pinfo) -> bool {
    if (pinfo.bc == bc) {
      if (t) {
        return *t == pinfo.type;
      } else {
        return true;
      }
    }
    return false;
  };

  return std::find_if(penalty_.begin(), penalty_.end(), std::move(pred));
}

QuakeWorld::BuildPenaltyList::const_iterator QuakeWorld::FindBuildPenalty(
    const void* bc) const {
  auto pred = [&](const BuildPenaltyInfo& pinfo) -> bool {
    return pinfo.bc == bc;
  };

  return std::find_if(build_penalty_.begin(), build_penalty_.end(),
                      std::move(pred));
}

bool QuakeWorld::CheckPenaltyThreshold(PenaltyType ptype,
                                       std::uint64_t c) const {
  switch (ptype) {
    case kNYI:
      return c >= FLAG_jit_penalty_nyi.Get();
    case kBytecodeOverflow:
      return c >= FLAG_jit_penalty_bytecode_overflow.Get();
    case kBytecodeError:
      return c >= FLAG_jit_penalty_bytecode_error.Get();
    case kReferenceError:
      return c >= FLAG_jit_penalty_reference_error.Get();
    case kLoopExit:
      return c >= FLAG_jit_penalty_loop_exit.Get();
    case kInnerLoop:
      return c >= FLAG_jit_penalty_inner_loop.Get();
    default:
      UNREACHABLE();
      return false;
  }
}

bool QuakeWorld::AddBuildPenalty(const void* bc) {
  if (auto itr = FindBuildPenalty(bc); itr != build_penalty_.end()) {
    itr->counter++;
    return itr->counter >= FLAG_jit_penalty_build.Get();
  } else {
    build_penalty_.emplace_back(bc, 1);
    return 1 >= FLAG_jit_penalty_build.Get();
  }
}

bool QuakeWorld::AddPenalty(const void* bc, PenaltyType pt) {
  const auto global_penalty = AddBuildPenalty(bc);

  if (auto itr = FindPenalty(bc, pt); itr != penalty_.end()) {
    itr->counter++;
    return CheckPenaltyThreshold(pt, itr->counter) || global_penalty;
  } else {
    penalty_.emplace_back(bc, pt, 1);
    return CheckPenaltyThreshold(pt, 1) || global_penalty;
  }
}

void QuakeWorld::ClearPenalty(const void* bc) {
  if (auto itr = FindPenalty(bc, {}); itr != penalty_.end()) {
    penalty_.erase(itr);
  }
  if (auto itr = FindBuildPenalty(bc); itr != build_penalty_.end()) {
    build_penalty_.erase(itr);
  }
}

QuakeWorld::PenaltyType QuakeWorld::GetPenalty(const void* bc) const {
  if (auto itr = FindPenalty(bc, {}); itr != penalty_.end()) {
    return itr->type;
  }
  if (auto itr = FindBuildPenalty(bc); itr != build_penalty_.end()) {
    if (itr->counter >= FLAG_jit_penalty_build.Get()) {
      return kBuildPenalty;
    }
  }

  return kNoPenalty;
}

void QuakeWorld::DisableJit(const void* bc) {
  auto idx = QuakeJitOpcodeHash(bc);                 // get the bytecode's hash
  auto qfunc = function_bytecode_qfunc();            // get the qfunc
  qfunc->jit_hc[idx] = QUAKE_JIT_THRESHOLD_DISABLE;  // disable the call
}

void QuakeWorld::EnableJit(const void* bc) {
  auto idx = QuakeJitOpcodeHash(bc);       // get the bytecode's hash
  auto qfunc = function_bytecode_qfunc();  // get the qfunc
  qfunc->jit_hc[idx] = 0;                  // resume the counter to be 0
}

bool QuakeWorld::IsJitDisable(const void* bc) const {
  auto idx = QuakeJitOpcodeHash(bc);             // get the bytecode's hash
  auto qfunc = function_bytecode_qfunc();        // get the qfunc
  return QuakeIsJitDisable(qfunc->jit_hc[idx]);  // check flag
}

bool QuakeWorld::HasLoop() const {
  for (const auto& x : trace_list_) {
    if (x->trace_type() == TraceType::kLoop) {
      return true;
    }
  }
  return false;
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
