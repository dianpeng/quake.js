#ifndef _QUAKE_CG_X64_COMMENT_H_
#define _QUAKE_CG_X64_COMMENT_H_
#include "base.h"
#include <asmjit/asmjit.h>

namespace quake {
namespace cg {
namespace x64 {

class CodeGen;
class CodeX64;

struct CodeComment {
  asmjit::Label start, end;
  std::string comment;

  // setup after the code has been linked
  std::uint64_t start_offset = 0;
  std::uint64_t end_offset = 0;

  bool IsLinked() const { return start_offset != 0 && end_offset != 0; }

  CodeComment(asmjit::Label&& s, asmjit::Label&& e, std::string&& m)
      : start(std::move(s)), end(std::move(e)), comment(std::move(m)) {}
};

class CommentHolder {
 public:
  // Do not use this function to add comment, but use Comment RAII to add it
  void Add(asmjit::Label&& start, asmjit::Label&& end, std::string&& comment) {
    code_comment_.emplace_back(std::move(start), std::move(end),
                               std::move(comment));
  }

  void Finalize();

  // Try to get the comment from the code position specified by the offset. Must
  // be called after the Finalize, otherwise behavior is undefined
  std::string GetComment(std::uint64_t offset) const;

 private:
  // The list is been sorted by start_address after been linking. Which is
  // same as how ELF store the item inside of each section
  std::vector<CodeComment> code_comment_;
  friend class CodeX64;
};

// RAII to allow user to add a comment for certain section of code
class Comment {
 public:
  Comment(CodeGen*, std::string&&);
  ~Comment();
  static Comment MkComment(CodeGen*, const char*, const char*, int, const char*,
                           ...);

 private:
  CodeGen* cg_;
  asmjit::Label start_;
  std::string information_;
};

#define __QUAKE_CG_X64_COMMENT_NAME(X) CONCATE(__x64_comment, X)

#if defined(__COUNTER__)
#define QUAKE_CG_X64_COMMENT_NAME __QUAKE_CG_X64_COMMENT_NAME(__COUNTER__)
#else
#define QUAKE_CG_X64_COMMENT_NAME __QUAKE_CG_X64_COMMENT_NAME(__LINE__)
#endif  // __COUNTER__

// Use COMMENT macro to instrument a certain range assmembly code along with
// where it is been created etc ...
//
// Notes COMMENT is an RAII macro, should be used as following
// {
//   COMMENT(cg, "THIS is for JIT code epilogue");
//   __ mov(...)
//   ...
// }
#define COMMENT(CG, ...)                                                 \
  auto QUAKE_CG_X64_COMMENT_NAME = ::quake::cg::x64::Comment::MkComment( \
      (CG), __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_COMMENT_H_
