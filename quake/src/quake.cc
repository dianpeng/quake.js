#include "quake.h"

#include "ir/builder.h"
#include "ir/graph.h"

namespace quake {

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

bool Quake::IsJitFlagOn(int x) const {
  switch (x) {
    default:
      return true;
  }
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

Quake::Quake(QuakeWorld* world, QJS* qjs, JSContext* js_context)
    : qjs_(qjs),
      js_context_(js_context),
      world_(world),
      zone_(1024 * 16, 1024 * 128),
// clang-format off

/** static string initialization **/
#define __(A, B) A##_(JVal{js_context_, JS_NewString(js_context_, B)}),
      QUAKE_QUAKE_STATIC_STRING_LIST(__)
#undef __  // __

      // clang-format on

      dummy_(false) {
}

Quake::~Quake() {}

Quake* QuakeWorld::GetQuakeByFirstBC(const void* bc) const {
  auto itr = std::find_if(
      trace_list_.begin(), trace_list_.end(),
      [&](const auto& q) -> bool { return q->trigger_loop_from_bc() == bc; });
  return itr == trace_list_.end() ? nullptr : itr->get();
}

Quake* QuakeWorld::GetQuakeByLoopbackBC(const void* bc) const {
  auto itr = std::find_if(
      trace_list_.begin(), trace_list_.end(),
      [&](const auto& q) -> bool { return q->trigger_loop_back_bc() == bc; });
  return itr == trace_list_.end() ? nullptr : itr->get();
}

}  // namespace quake
