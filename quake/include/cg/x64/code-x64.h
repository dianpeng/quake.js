#ifndef _QUAKE_CG_X64_CODE_X64_H_
#define _QUAKE_CG_X64_CODE_X64_H_
#include "base.h"
#include "code.h"
#include "quake.h"
#include "cg/x64/comment.h"

#include <asmjit/asmjit.h>

namespace quake {
namespace cg {
namespace x64 {
class CodeGen;

class CodeX64 : public Code {
 private:
  enum CValType { kF64, kI64, kPtr };
  struct CVal {
    int type;

    // FIXME(dpeng): Use variant/union?
    double f64;
    std::int64_t i64;
    void* ptr;

    explicit CVal(double f) : type(kF64), f64(f) {}
    explicit CVal(std::int64_t v) : type(kI64), i64(v) {}
    explicit CVal(void* p) : type(kPtr), ptr(p) {}

   public:
    static CVal F64(double v) { return CVal{v}; }
    static CVal I64(std::int64_t v) { return CVal{v}; }
    static CVal Ptr(void* ptr) { return CVal{ptr}; }

    std::size_t ByteSize() const { return 8; }

    const void* ValueAddress() const {
      switch (type) {
        case CValType::kF64:
          return reinterpret_cast<const void*>(&f64);
        case CValType::kI64:
          return reinterpret_cast<const void*>(&i64);
        case CValType::kPtr:
          return reinterpret_cast<const void*>(&ptr);
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
  const KnownPos& object_field_get_atom_fast() const {
    CHECK(object_field_get_atom_fast_);
    return *object_field_get_atom_fast_;
  }

  const KnownPos& global_get_global_atom_fast() const {
    CHECK(global_get_global_atom_fast_);
    return *global_get_global_atom_fast_;
  }

 public:
  // Body of the code, ie no register saving etc, cannot be used for function
  // call and should be used for disassembly purpose
  const KnownPos& body() const {
    CHECK(body_);
    return *body_;
  }

 public:
  enum {
    kF64One = 0,
    kF64NegOne = 1,
    kF64PosZero = 2,
    kF64NegZero = 3,
    kStrTrue = 4,
    kStrFalse = 5,
    kI64Zero = 6,
    kI64One = 7
  };

  // Constant table
  asmjit::x86::Mem const_f64_one() const { return IndexConstVal(kF64One); }
  asmjit::x86::Mem const_f64_neg_one() const {
    return IndexConstVal(kF64NegOne);
  }
  asmjit::x86::Mem const_f64_pos_zero() const {
    return IndexConstVal(kF64PosZero);
  }

  asmjit::x86::Mem const_str_true() const { return IndexConstVal(kStrTrue); }
  asmjit::x86::Mem const_str_false() const { return IndexConstVal(kStrFalse); }
  asmjit::x86::Mem const_i64_zero() const { return IndexConstVal(kI64Zero); }
  asmjit::x86::Mem const_i32_zero() const {
    auto x = IndexConstVal(kI64Zero);
    x.setSize(4);
    return x;
  }

  asmjit::x86::Mem const_i64_one() const { return IndexConstVal(kI64One); }
  asmjit::x86::Mem const_i32_one() const {
    auto x = IndexConstVal(kI64One);
    x.setSize(4);
    return x;
  }

  // notes f64 neg zero *CAN BE* used as xmmword ptr specifically for xorps
  // instruction
  asmjit::x86::Mem const_f64_neg_zero() const {
    return IndexConstVal(kF64NegZero);
  }
  asmjit::x86::Mem const_f64_neg_zero_xmmword() const {
    return asmjit::x86::xmmword_ptr(const_pool_label_, neg_zero_xmmword_);
  }
  asmjit::x86::Mem const_box_undefined_xmmword() const {
    return asmjit::x86::xmmword_ptr(const_pool_label_, box_undefined_xmmword_);
  }
  asmjit::x86::Mem const_box_null_xmmword() const {
    return asmjit::x86::xmmword_ptr(const_pool_label_, box_null_xmmword_);
  }
  asmjit::x86::Mem const_box_uninitialized_xmmword() const {
    return asmjit::x86::xmmword_ptr(const_pool_label_,
                                    box_uninitialized_xmmword_);
  }

 public:
  asmjit::x86::Mem AddConst(void* ptr) { return AddConst(CVal::Ptr(ptr)); }
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
  std::size_t AddConstValIntoPool(CVal);
  std::size_t AddXMMWord(void*);
  std::size_t AddConstVal(CVal);
  std::size_t AddConstVal(double v);
  std::size_t AddConstVal(std::int64_t);
  std::size_t AddConstVal(void*);

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
  std::optional<KnownPos> object_field_get_atom_fast_;
  std::optional<KnownPos> global_get_global_atom_fast_;
  std::optional<KnownPos> end_;  // code end position, used to help disassembly

  // --------------------------------------------------------------------------
  // constant pool
  asmjit::Label const_pool_label_;
  std::vector<ConstVal> const_val_;

  // special value's, ie xmmword
  std::size_t neg_zero_xmmword_;
  std::size_t box_undefined_xmmword_;
  std::size_t box_null_xmmword_;
  std::size_t box_uninitialized_xmmword_;

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
