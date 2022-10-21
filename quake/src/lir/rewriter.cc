#include "lir/rewriter.h"
#include "lir/assembler.h"

namespace quake {
namespace lir {

namespace {

using RewriterMap =
    std::map<std::string, std::unique_ptr<Rewriter>, std::less<>>;

RewriterMap& GetRewriterMap() {
  static RewriterMap kMap;
  return kMap;
}

}  // namespace

Rewriter::RewriteResult Rewriter::RewriteOne(Quake* q, HAssembler* as,
                                             HInstruction* ins) {
  for (const auto& e : GetRewriterMap()) {
    switch (e.second->Run(q, as, ins)) {
      case RewriteResult::kUnchanged:
        break;  // continue to look for next rewriter

      case RewriteResult::kNext:
        return RewriteResult::kNext;

      case RewriteResult::kChanged:
        return RewriteResult::kChanged;

      default:
        UNREACHABLE();
        return RewriteResult::kNext;
    }
  }

  return RewriteResult::kUnchanged;
}

void Rewriter::Rewrite(Quake* q, HAssembler* as) {
  auto has_change = false;

  do {
    has_change = false;
    as->ForeachInstruction([&](HInstruction* ins) {
      switch (RewriteOne(q, as, ins)) {
        case RewriteResult::kChanged:
        case RewriteResult::kNext:
          has_change = true;
          break;
        default:
          break;
      }
    });
  } while (has_change);
}

void Rewriter::AddRewriter(std::unique_ptr<Rewriter>&& r) {
  auto name = r->GetName();
  GetRewriterMap().insert(std::make_pair(std::move(name), std::move(r)));
}

const Rewriter* Rewriter::GetRewriter(const std::string_view& name) {
  auto itr = GetRewriterMap().find(name);
  return itr == GetRewriterMap().end() ? nullptr : itr->second.get();
}

}  // namespace lir
}  // namespace quake
