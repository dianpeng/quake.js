#include "cg/x64/comment.h"
#include "cg/x64/code-gen.h"

namespace quake {
namespace cg {
namespace x64 {

void CommentHolder::Finalize() {
  struct Comp {
    bool operator()(const CodeComment& l, const CodeComment& r) {
      CHECK(l.IsLinked());
      CHECK(r.IsLinked());
      return l.start_address < r.start_address;
    }
  };

  std::sort(code_comment_.begin(), code_comment_.end(), Comp{});
}

Comment::Comment(CodeGen* cg, std::string&& cmt)
    : cg_(cg), start_(cg->newLabel()), information_(std::move(cmt)) {}

Comment::~Comment() {
  auto end = cg_->newLabel();
  cg_->comment_holder()->Add(std::move(start_), std::move(end),
                             std::move(information_));
}

Comment Comment::MkComment(CodeGen* cg, const char* file, const char* func,
                           int line, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = base::FmtV(fmt, vl);
  va_end(vl);

  auto final_msg = base::Fmt("(%s:%s@%d): %s", file, func, line, msg.c_str());
  return Comment{cg, std::move(final_msg)};
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
