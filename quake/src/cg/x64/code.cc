#include "cg/x64/code.h"
#include "cg/x64/code-gen.h"

namespace quake {
namespace cg {
namespace x64 {

CodeX64::CodeX64(CodeGen* cg)
    : const_pool_label_(cg->newLabel()), zone_(1024 * 4), const_pool_(&zone_) {
  SetUp();
}

void CodeX64::SetUp() {
  // setup the known constant stuff, order matters
  AddConstVal(1.0);
  AddConstVal(-1.0);
  AddConstVal(+0.0);
  AddConstVal(-0.0);
}

std::size_t CodeX64::AddConstVal(CVal cval) {
  auto itr = std::find_if(const_val_.begin(), const_val_.end(),
                          [&](const ConstVal& cv) -> bool {
                            if (cv.cval.type == cval.type) {
                              switch (cv.cval.type) {
                                case CValType::kI64:
                                  return cv.cval.i64 == cval.i64;
                                case CValType::kF64:
                                  return cv.cval.f64 == cval.f64;
                                default:
                                  UNREACHABLE();
                                  return false;
                              }
                            }
                            return false;
                          });

  if (itr != const_val_.end()) {
    return std::distance(const_val_.begin(), itr);
  }

  // okay, need to re-insert it
  std::size_t offset;
  const_pool_.add(cval.ValueAddress(), cval.ByteSize(), offset);
  const_val_.push_back(ConstVal{cval, offset});
  return const_val_.size() - 1;
}

std::size_t CodeX64::AddConstVal(double v) { return AddConstVal(CVal::F64(v)); }

std::size_t CodeX64::AddConstVal(std::int64_t v) {
  return AddConstVal(CVal::I64(v));
}

asmjit::x86::Mem CodeX64::IndexConstVal(std::size_t idx) const {
  const auto& xx = const_val_[idx];
  return asmjit::x86::ptr(const_pool_label_, xx.offset);
}

asmjit::x86::Mem CodeX64::AddConst(CVal cval) {
  auto idx = AddConstVal(cval);
  return IndexConstVal(idx);
}

void CodeX64::EmbedConstPool(CodeGen* cg) {
  cg->embedConstPool(const_pool_label_, const_pool_);
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
