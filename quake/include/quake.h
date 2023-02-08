#ifndef _QUAKE_H_
#define _QUAKE_H_
#include "zone.h"
#include "quake-C.h"
#include "deopt.h"
#include "code.h"
#include "qjs.h"

#include "ir/trace-result.h"
#include "ir/bytecode.h"

namespace quake {

namespace ir {
class Builder;
class Graph;
class InterpInfo;
}  // namespace ir

class QuakeWorld;

using TraceId = std::uint32_t;
static constexpr TraceId kPendingTraceId = 0;

// All various trace type. The following trace type is included
//
// 1) loop
//
//    Indicating a single loop nest inside of a loop nested forrest, different
//    loop is linked via trace link entry. This quake will generate a special
//    IR graph, via reemittion phase.
//
// 2) SideTrace
//
//    A trace for certain highly likely taken branch. This is just a linear
//    trace used to avoid deoptimization back to the runtime. It is jitted
//    when certain side branch is taken too many times. This is also linked
//    via special trace link
//
// 3) Function
//
//    A trace for the whole function. Similar like SideTrace, except it just
//    starts from the first bytecode of a trace
//
// --------------------------------------------------------------------------
enum class TraceType { kLoop, kSideTrace, kFunction };

// list of static string, which is used by backend, ie during code generation
#define QUAKE_QUAKE_STATIC_STRING_LIST(__) \
  __(true, "true")                         \
  __(false, "false")

// Quake, represent a trace that can be used for Jit
class Quake {
 public:
 public:
  Quake(QuakeWorld*, QJS*, JSContext*);
  ~Quake();

 public:
  JSContext* js_context() const { return js_context_; }
  QJS* qjs() const { return qjs_; }
  zone::Zone* zone() { return &zone_; }
  QuakeWorld* world() { return world_; }
  TraceId trace_id() const { return trace_id_; }
  bool IsPendingTrace() const { return trace_id_ != kPendingTraceId; }

  DeoptId last_deoptimization_id() const { return last_deoptimization_id_; }
  void set_last_deoptimization_id(DeoptId did) {
    ++deopt_times_;
    last_deoptimization_id_ = did;
  }
  std::size_t deopt_times() const { return deopt_times_; }

 public:
  void set_expose(bool b) { expose_ = b; }
  bool expose() const { return expose_; }

 public:
  // Deoptimization table mapping
  const Deopt& GetDeopt(DeoptId) const;

  // Add a new deoptimization entry into the quake. Notes, this is for internal
  // usage by lir::HAssembler.
  DeoptId AddDeopt(DeoptReason, DeoptNext, lir::Snapshot&&);

  std::size_t GetLoopExitSize() const;

  // Size of total deoptimization point for this trace(quake)
  std::size_t deopt_size() const { return deopt_list_.size(); }
  const std::vector<Deopt>& deopt_list() const { return deopt_list_; }

  DeoptId deopt_id_max() const { return static_cast<DeoptId>(deopt_size()); }

 public:
  // JIT needed data, used for certain builtin routine implementation
#define __(A, B) \
  void* static_string_##A() const { return A##_.Ptr(); }

  QUAKE_QUAKE_STATIC_STRING_LIST(__)

#undef __  // __

 public:
  // Return a JSString pointer for constant usage. The returned string is held
  // on JavaScript's heap. The returned pointer is not REF counted
  void* AddConstPoolString(const std::string_view&);

 public:
  // frame related book-keeping information -----------------------------------
  // in bytes
  std::size_t cell_size() const { return cell_size_; }
  void set_cell_size(std::size_t s) { cell_size_ = s; }
  std::size_t cell_offset() const { return frame_size() + misc_size(); }

  // in bytes
  std::size_t spill_size() const { return spill_size_; }
  void set_spill_size(std::size_t s) { spill_size_ = s; }
  std::size_t spill_offset() const {
    return cell_size() + frame_size() + misc_size();
  }

  // in bytes
  std::size_t frame_size() const { return frame_size_; }
  void set_frame_size(std::size_t s) { frame_size_ = s; }
  std::size_t frame_offset() const { return misc_size(); }

  // in bytes
  std::size_t misc_size() const { return misc_size_; }
  void set_misc_size(std::size_t s) { misc_size_ = s; }
  std::size_t misc_offset() const { return 0; }

  std::size_t stack_byte_size() const {
    return cell_size() + spill_size() + frame_size() + misc_size();
  }

 public:
  // loop end's bytecode address, ie the backedge jump
  void set_trigger_loop_back_bc(const void* bc) { trigger_loop_back_bc_ = bc; }
  const void* trigger_loop_back_bc() const { return trigger_loop_back_bc_; }

  // loop start's bytecode address
  void set_trigger_loop_from_bc(const void* bc) { trigger_loop_from_bc_ = bc; }
  const void* trigger_loop_from_bc() const { return trigger_loop_from_bc_; }

  ir::BytecodeRange bytecode_range() const {
    return ir::BytecodeRange{trigger_loop_from_bc(), trigger_loop_back_bc()};
  }

 public:
  void AddInnerTrace(std::shared_ptr<Quake> q) {
    inner_trace_list_.push_back(q);
  }

 public:
  enum JitState {
    kJitStart,
    kJitTrace,
    kJitStop,
    kJitCompile,
    kJitUp,
    kJitFail
  };

  void set_jit_state(JitState js) {
    jit_state_ = js;
    if (js == kJitFail) {
      LOG(ERROR) << "jit fail";
    }
  }
  JitState jit_state() const { return jit_state_; }

  bool IsJitUp() const { return jit_state_ == kJitUp; }
  bool IsEntry() const { return true; }

 public:
  // --------------------------------------------------------------------------
  // Compiler part
  //
  // The quake itself is been progressively built from source code, finally it
  // will turn into a fully jitted trace. Basically, quake is a compiler at
  // certain time point

  // Used during the code generation/tracing. The only state we keep around is
  // the Quake object and its C pointer is QQuake. We do not keep anything
  // around and this serves the blackboard for all the states during JIT for
  // a certain trace.
  bool HasIRBuilder() const { return static_cast<bool>(ir_builder_); }
  bool HasIRGraph() const { return static_cast<bool>(ir_graph_); }

  void NewIRTrace();
  void FreeIRGraph();
  void FreeIRBuilder();

  ir::Builder* ir_builder() const { return ir_builder_.get(); }
  ir::Graph* ir_graph() const { return ir_graph_.get(); }

  void set_code(std::unique_ptr<Code>&& c) { code_ = std::move(c); }
  bool HasCode() const { return static_cast<bool>(code_); }
  const Code& code() const { return *code_; }

  ir::TraceResult CompilerSetup(ir::InterpInfo*);
  ir::TraceResult CompilerTrace(ir::InterpInfo*);
  ir::TraceResult CompilerStop(ir::InterpInfo*);
  base::Error CompilerCompile();

  enum class CompilerStatus {
    kDone,
    kStop,
    kNYI,
    kBytecodeOverflow,
    kBytecodeError,
    kReferenceError,
    kLoopExit,
    kInnerLoop,
    kCompileError
  };
  static CompilerStatus MapTraceResult(const ir::TraceResult&);

  CompilerStatus CompilerStep(ir::InterpInfo*);

 private:
  friend class QuakeWorld;
  void set_trace_id(TraceId tid) { trace_id_ = tid; }

 private:
  QJS* qjs_;
  JSContext* js_context_;
  QuakeWorld* world_;
  TraceId trace_id_;
  DeoptId last_deoptimization_id_;

  zone::Zone zone_;
  std::unique_ptr<Code> code_;  // jitted code, setup after the compile is
                                // done. See interface in Quake-C.h

 private:
  // list of deoptimization point
  std::vector<Deopt> deopt_list_;

 private:
  // inner loop. If the current trace existed, its inner trace should be
  // existed. Notes the runtime can nullify inner trace directly.
  std::vector<std::shared_ptr<Quake>> inner_trace_list_;

 private:
  // stack layout related, used to help get the spill slot etc ...
  std::size_t cell_size_ = 0;   // cell region reserved size in bytes
  std::size_t spill_size_ = 0;  // spill size in bytes
  std::size_t frame_size_ = 0;  // frame size in bytes
  std::size_t misc_size_ = 0;   // misc data in bytes used during JIT

 private:
  // runtime statues, mostly about deoptimization counters
  std::size_t deopt_times_ = 0;

 private:
  // quickjs related stuff
  const void* trigger_loop_back_bc_ =
      nullptr; /* which loopback bytecode to trigger */

  const void* trigger_loop_from_bc_ =
      nullptr; /* first bytecode that starts the loop */

  JitState jit_state_ = kJitStart;

  bool expose_ = true; /* default to be exposed */

 private:
  std::unique_ptr<ir::Graph> ir_graph_;
  std::unique_ptr<ir::Builder> ir_builder_;

 private:
  // Constant pool ------------------------------------------------------------
  //
  // Notes, we will have to use a unique_ptr wrapper since we gonna use string's
  // memory in our JIT code. std::string does not guarantee its c_str() return
  // value's stability in terms of memory usage. Ie, if string is optimized with
  // SOO.
  std::vector<JVal> cpool_string_;

#define __(A, ...) JVal A##_;
  QUAKE_QUAKE_STATIC_STRING_LIST(__)
#undef __  // __

 private:
  bool dummy_;  // always last one
};

inline const Deopt& Quake::GetDeopt(DeoptId deopt_id) const {
  CHECK(deopt_id < deopt_list_.size());
  return deopt_list_[deopt_id];
}

class QuakeWorld {
 public:
  std::shared_ptr<Quake> GetQuakeByFirstBC(const void*) const;
  std::shared_ptr<Quake> GetQuakeByLoopbackBC(const void*) const;

 public:
  std::shared_ptr<Quake> GetQuakeByTraceId(TraceId tid) const {
    CHECK(tid != kPendingTraceId);
    CHECK(tid <= trace_list_.size());
    return trace_list_[tid - 1];
  }

 public:
  void AddQuake(Quake* q) {
    auto ptr = std::shared_ptr<Quake>{q};
    trace_list_.push_back(ptr);
    auto id = static_cast<TraceId>(trace_list_.size());
    q->set_trace_id(id);
  }

 private:
  // List of trace already generated
  std::vector<std::shared_ptr<Quake>> trace_list_;
};

}  // namespace quake

#endif  // _QUAKE_H_
