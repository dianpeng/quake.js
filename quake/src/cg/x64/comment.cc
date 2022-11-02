#include "cg/x64/comment.h"
#include "cg/x64/code-gen.h"

namespace quake {
namespace cg {
namespace x64 {

void CommentHolder::Finalize() {
  struct Comp {
    bool operator()(const CodeComment& l, const CodeComment& r) {
      return l.start_offset < r.start_offset;
    }
  };

  std::sort(code_comment_.begin(), code_comment_.end(), Comp{});
}

std::string CommentHolder::GetComment(std::uint64_t offset) const {
  struct Comp {
    bool operator()(const CodeComment& l, std::uint64_t r) {
      return l.start_offset < r;
    }
    bool operator()(const std::uint64_t l, const CodeComment& r) {
      return l < r.start_offset;
    }
    bool operator()(const CodeComment& l, const CodeComment& r) {
      return l.start_offset < r.start_offset;
    }
  };

  auto start = std::lower_bound(code_comment_.begin(), code_comment_.end(),
                                offset, Comp{});

  auto end = std::upper_bound(code_comment_.begin(), code_comment_.end(),
                              offset, Comp{});

  std::string out;

  for (; start != end; start++) {
    out.append("-- !");
    out.append(start->comment);
    out.push_back('\n');
  }

  return out;
}

Comment::Comment(CodeGen* cg, std::string&& cmt)
    : cg_(cg), start_(cg->as()->newLabel()), information_(std::move(cmt)) {
  cg->as()->bind(start_);
}

Comment::~Comment() {
  auto end = cg_->as()->newLabel();
  cg_->as()->bind(end);
  cg_->code()->comment_holder().Add(std::move(start_), std::move(end),
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
