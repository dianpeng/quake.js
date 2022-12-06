#include "jit.h"
#include "code.h"

namespace quake {

namespace {
thread_local std::unique_ptr<Jit> kJit;
}

Jit& Jit::GetInstance() {
  if (!kJit) {
    kJit.reset(new Jit());
  }
  return *kJit;
}

Jit::Jit()
    :
/* clant-format off */
#define __(A, B, C) B##_(C),
      QUAKE_JIT_OPTION_LIST(__)
#undef __  // __
           /* clant-format on */

      dummy_(false) {
}

base::Result<JitCodeEntry> Jit::AddCode(Code* c) {
  JitCodeEntry fn;
  if (auto e = jit_runtime_.add(&fn, &c->code_holder()); e) {
    return base::Error::Mk("ASMJIT(error): %s",
                           asmjit::DebugUtils::errorAsString(e));
  }

  return fn;
}

}  // namespace quake
