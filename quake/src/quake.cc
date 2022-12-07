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

Quake::Quake(QJS* qjs, JSContext* js_context)
    : qjs_(qjs),
      js_context_(js_context),
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

}  // namespace quake
