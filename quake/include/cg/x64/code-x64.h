#ifndef _QUAKE_CG_X64_CODE_X64_H_
#define _QUAKE_CG_X64_CODE_X64_H_
#include "base.h"
#include "code.h"

#include <asmjit/asmjit.h>

#include "cg/x64/comment.h"

namespace quake {
namespace cg {
namespace x64 {
class CodeGen;

class CodeX64 : public Code {
 private:
  enum CValType { kF64, kI64 };
  struct CVal {
    int type;
    double f64;
    std::int64_t i64;

    static CVal F64(double v) { return CVal{kF64, v, 0}; }
    static CVal I64(std::int64_t v) { return CVal{kI64, 0.0, v}; }

    std::size_t ByteSize() const { return sizeof(double); }

    const void* ValueAddress() const {
      switch (type) {
        case CValType::kF64:
          return reinterpret_cast<const void*>(&f64);
        case CValType::kI64:
          return reinterpret_cast<const void*>(&i64);
        default:
          UNREACHABLE();
          return nullptr;
      }
    }
  };

  struct ConstVal {
    CVal cval;
    std::size_t offset;
  };

 public:
  CodeX64(CodeGen*);

  // --------------------------------------------------------------------------
  // Known stub(trampoline) address
  struct KnownPos {
    // this can be materialized when label is set properly
    asmjit::x86::Mem ToASMJITMem() const {
      CHECK(label_);
      return asmjit::x86::ptr(*label_);
    }

    const asmjit::Label& label() const {
      CHECK(label_);
      return *label_;
    }

    std::size_t offset() const { return offset_; }

    KnownPos(asmjit::Label&& l) : label_(std::move(l)), offset_(0) {}

   private:
    void Link(CodeX64*);

    std::optional<asmjit::Label> label_;
    std::size_t offset_;  // setup when the linking is done. This is always safe
    friend class CodeX64;
  };

  const KnownPos& jit_prologue() const {
    CHECK(jit_prologue_);
    return *jit_prologue_;
  }

  const KnownPos& jit_epilogue() const {
    CHECK(jit_epilogue_);
    return *jit_epilogue_;
  }
  const KnownPos& deopt() const {
    CHECK(deopt_);
    return *deopt_;
  }
  const KnownPos& cast_f64_to_b32() const {
    CHECK(cast_f64_to_b32_);
    return *cast_f64_to_b32_;
  }

 public:
  // Body of the code, ie no register saving etc, cannot be used for function
  // call and should be used for disassembly purpose
  const KnownPos& body() const {
    CHECK(body_);
    return *body_;
  }

 public:
  // Constant table
  asmjit::x86::Mem const_f64_one() const { return IndexConstVal(0); }
  asmjit::x86::Mem const_f64_neg_one() const { return IndexConstVal(1); }
  asmjit::x86::Mem const_f64_pos_zero() const { return IndexConstVal(2); }
  asmjit::x86::Mem const_f64_neg_zero() const { return IndexConstVal(3); }

  asmjit::x86::Mem AddConst(double d) { return AddConst(CVal::F64(d)); }
  asmjit::x86::Mem AddConst(std::int64_t d) { return AddConst(CVal::I64(d)); }

  // After the code generation, emit the const pool section
  void EmbedConstPool(CodeGen*);

 public:
  // Lastly, perform linking and obtain the entry_. After the linking the
  // entry_ is available, and can be used later on.
  base::Error Link(CodeGen*);

 public:
  asmjit::CodeHolder& code_holder() override { return *code_holder_; }
  const asmjit::CodeHolder& code_holder() const override {
    return *code_holder_;
  }

  JitCodeEntry entry() const override {
    CHECK(entry_);
    return entry_;
  }

  CommentHolder& comment_holder() { return comment_holder_; }
  const CommentHolder& comment_holder() const { return comment_holder_; }

 public:
  std::string Disassemble() const override;

 private:
  std::size_t AddConstVal(CVal);
  std::size_t AddConstVal(double v);
  std::size_t AddConstVal(std::int64_t);
  asmjit::x86::Mem IndexConstVal(std::size_t) const;
  asmjit::x86::Mem AddConst(CVal);

 private:
  // --------------------------------------------------------------------------
  // Comment
  CommentHolder comment_holder_;

  // --------------------------------------------------------------------------
  // well known position
  std::optional<KnownPos> body_;
  std::optional<KnownPos> jit_prologue_;
  std::optional<KnownPos> jit_epilogue_;
  std::optional<KnownPos> deopt_;
  std::optional<KnownPos> cast_f64_to_b32_;
  std::optional<KnownPos> end_;  // code end position, used to help disassembly

  // --------------------------------------------------------------------------
  // constant pool

  asmjit::Label const_pool_label_;
  std::vector<ConstVal> const_val_;

  // order matters
  asmjit::Zone zone_;
  asmjit::ConstPool const_pool_;

  // --------------------------------------------------------------------------
  // ASMJIT related details, maybe refactored into other position
  std::unique_ptr<asmjit::CodeHolder> code_holder_;

  // --------------------------------------------------------------------------
  // Others
  JitCodeEntry entry_ = nullptr;
  std::size_t total_size_ = 0;
  std::size_t code_size_ = 0;

 private:
  // called by CodeGen to break cycle
  void SetUp(CodeGen*);
  friend class CodeGen;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_CODE_X64_H_
