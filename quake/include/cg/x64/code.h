#ifndef _QUAKE_CG_X64_CODE_H_
#define _QUAKE_CG_X64_CODE_H_
#include "base.h"
#include <asmjit/asmjit.h>

namespace quake {
namespace cg {
namespace x64 {
class CodeGen;

class CodeX64 {
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
    asmjit::Label label;
    const void* address = nullptr;

    // this can be materialized when label is set properly
    asmjit::x86::Mem ToASMJITMem() const { return asmjit::x86::ptr(label); }
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
  // Entry of the code, ie with normal ABI convention
  const KnownPos& entry() const {
    CHECK(entry_);
    return *entry_;
  }

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

 private:
  asmjit::Label GetLabel(const std::optional<KnownPos>& pos) const {
    CHECK(pos);
    return pos->label;
  }

  const void* GetAddress(const std::optional<KnownPos>& pos) const {
    CHECK(pos);
    CHECK(pos->address);
    return pos->address;
  }

 private:
  std::size_t AddConstVal(CVal);
  std::size_t AddConstVal(double v);
  std::size_t AddConstVal(std::int64_t);
  asmjit::x86::Mem IndexConstVal(std::size_t) const;
  asmjit::x86::Mem AddConst(CVal);

  void SetUp();

 private:
  // --------------------------------------------------------------------------
  // well known position
  std::optional<KnownPos> entry_;
  std::optional<KnownPos> body_;
  std::optional<KnownPos> jit_prologue_;
  std::optional<KnownPos> jit_epilogue_;
  std::optional<KnownPos> deopt_;
  std::optional<KnownPos> cast_f64_to_b32_;

  // --------------------------------------------------------------------------
  // constant pool

  asmjit::Label const_pool_label_;
  std::vector<ConstVal> const_val_;

  // order matters
  asmjit::Zone zone_;
  asmjit::ConstPool const_pool_;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_CODE_H_
