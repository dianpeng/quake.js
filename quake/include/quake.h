#ifndef _QUAKE_H_
#define _QUAKE_H_
#include "zone.h"
#include "quake-C.h"
#include "deopt.h"
#include "code.h"
#include "qjs.h"

namespace quake {

namespace ir {
class Builder;
class Graph;
}  // namespace ir

// list of static string, which is used by backend, ie during code generation
#define QUAKE_QUAKE_STATIC_STRING_LIST(__) \
  __(true, "true")                         \
  __(false, "false")

// Quake, represent a trace that can be used for Jit
class Quake {
 public:
  Quake(QJS*, JSContext*);
  ~Quake();

 public:
  JSContext* js_context() const { return js_context_; }
  QJS* qjs() const { return qjs_; }
  zone::Zone* zone() { return &zone_; }

 public:
  // Quake's jit flag
  enum {
    // fmod LIR will be translated into fast inline assembly, instead of
    // generaing runtime call to call std::fmod function. The former one
    // is way faster than calling std::fmod, but may yield incompatible
    // floating point behavior in corner cases on certain platform
    kJitFModFastCall,

    // fpow LIR will be translated into fast inline assembly, instead of
    // generaing runtime call to call std::fmod function. The former one
    // is way faster than calling std::fmod, but may yield incompatible
    // floating point behavior in corner cases on certain platform
    kJitFPowFastCall
  };

  bool IsJitFlagOn(int) const;

 public:
  // Deoptimization table mapping
  const Deopt& GetDeopt(DeoptId) const;

  // Add a new deoptimization entry into the quake. Notes, this is for internal
  // usage by lir::HAssembler.
  DeoptId AddDeopt(DeoptReason, DeoptNext, lir::Snapshot&&);

  // Size of total deoptimization point for this trace(quake)
  std::size_t deopt_size() const { return deopt_list_.size(); }

 public:
  // JIT needed data, used for certain builtin routine implementation
#define __(A, B) \
  void* static_string_##A() const { return A##_.Ptr(); }

  QUAKE_QUAKE_STATIC_STRING_LIST(__)

#undef __  // __

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

 public:
  void set_trigger_loop_back_bc(const void* bc) { trigger_loop_back_bc_ = bc; }
  const void* trigger_loop_back_bc() const { return trigger_loop_back_bc_; }

  enum JitState {
    kJitStart,
    kJitTrace,
    kJitStop,
    kJitCompile,
    kJitUp,
    kJitFail
  };

  void set_jit_state(JitState js) { jit_state_ = js; }
  JitState jit_state() const { return jit_state_; }

 private:
  QJS* qjs_;
  JSContext* js_context_;

  zone::Zone zone_;
  std::unique_ptr<Code> code_;  // jitted code, setup after the compile is
                                // done. See interface in Quake-C.h

 private:
  // list of deoptimization point
  std::vector<Deopt> deopt_list_;

 private:
  // stack layout related, used to help get the spill slot etc ...
  std::size_t cell_size_ = 0;   // cell region reserved size in bytes
  std::size_t spill_size_ = 0;  // spill size in bytes
  std::size_t frame_size_ = 0;  // frame size in bytes
  std::size_t misc_size_ = 0;   // misc data in bytes used during JIT

 private:
  // quickjs related stuff
  const void* trigger_loop_back_bc_ =
      nullptr; /* which loopback bytecode to trigger */

  JitState jit_state_ = kJitStart;

 private:
  std::unique_ptr<ir::Graph> ir_graph_;
  std::unique_ptr<ir::Builder> ir_builder_;

 private:
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

}  // namespace quake

#endif  // _QUAKE_H_
