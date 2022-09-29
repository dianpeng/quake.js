#ifndef _QUAKE_CG_X64_CODE_GEN_H_
#define _QUAKE_CG_X64_CODE_GEN_H_

#include "base.h"
#include <asmjit/asmjit.h>

namespace quake {
namespace cg {
namespace x64 {

class CodeGen {
 public:
  CodeGen() : as_() {}

 public:
  asmjit::x86::Builder* as() const { return as_.get(); }

 private:
  std::unique_ptr<asmjit::x86::Builder> as_;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_CODE_GEN_H_
