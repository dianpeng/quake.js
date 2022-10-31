#ifndef _QUAKE_CG_X64_CODE_GEN_H_
#define _QUAKE_CG_X64_CODE_GEN_H_
#include "base.h"
#include <asmjit/asmjit.h>

#include "cg/x64/code.h"
#include "cg/x64/comment.h"

namespace quake {
class Quake;

namespace cg {
namespace x64 {

class CodeGen : public asmjit::x86::Builder {
 public:
  explicit CodeGen(Quake*);

  std::size_t stack_reserve_size() const { return 0; }

 public:
  CommentHolder* comment_holder() const { return comment_holder_.get(); }
  CodeX64* code() const { return code_.get(); }

 private:
  Quake* quake_;
  std::unique_ptr<CommentHolder> comment_holder_;
  std::unique_ptr<CodeX64> code_;
  DISALLOW_COPY_AND_ASSIGN(CodeGen);
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_CODE_GEN_H_
