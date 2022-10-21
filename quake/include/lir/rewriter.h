#ifndef _QUAKE_LIR_REWRITER_H_
#define _QUAKE_LIR_REWRITER_H_
#include "base.h"
#include "lir/opcode.h"

namespace quake {
class Quake;

namespace lir {
class HAssembler;
class HInstruction;
class HOperand;

// Rewriter happens on the fly when HAssembler emit instruction sequences from
// the HIR instruction sequences. It is mostly been used to resolve arch related
// constraints. Like operand location, certain instruction's special requirement
// etc ...
class Rewriter {
 public:
  enum class RewriteResult : std::uint8_t { kUnchanged = 0, kChanged, kNext };

  virtual ~Rewriter() {}

  // Convert a HInstruction into lower representation or letting user to specify
  // certain operations accordingly
  virtual RewriteResult Run(Quake*, HAssembler*, HInstruction*) const = 0;

  // Get a unique rewriter name
  virtual std::string GetName() const = 0;

 public:
  // Perform the rewriter linearly. Notes the following algorithm is not a data
  // flow one but due to our graph is a specialized IR graph, so it is possible
  // to just rewrite the whole thing in a linear way without needing to run a
  // normal data flow pass.
  static void Rewrite(Quake*, HAssembler*);

 private:
  static RewriteResult RewriteOne(Quake*, HAssembler*, HInstruction*);

 public:
  // --------------------------------------------------------------------------
  // Rewriter pass registery
  static void AddRewriter(std::unique_ptr<Rewriter>&&);

  // Get a rewriter by the specified name
  static const Rewriter* GetRewriter(const std::string_view&);

  // Display rewriter list
  static std::string GetRewriterDisplay();
};

}  // namespace lir
}  // namespace quake

#define QUAKE__LIR_CC(a, b) QUAKE__LIR_CCI(a, b)
#define QUAKE__LIR_CCI(a, b) a##b
#define QUAKE__LIR_UNAME(P) QUAKE__LIR_CC(P, __COUNTER__)

#define QUAKE_LIR_REWRITER_ADD(XXX)                             \
  namespace {                                                   \
  base::_::Registry QUAKE__LIR_UNAME(kReg)([]() {               \
    quake::lir::Rewriter::AddRewriter(std::make_unique<XXX>()); \
  });                                                           \
  }

#endif  // _QUAKE_LIR_REWRITER_H_
