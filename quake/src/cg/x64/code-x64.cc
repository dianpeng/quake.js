#include "cg/x64/code-x64.h"
#include "cg/x64/code-gen.h"
#include "disass/disassembler.h"

#include "jit.h"

namespace quake {
namespace cg {
namespace x64 {

void CodeX64::KnownPos::Link(CodeX64* c) {
  CHECK(offset_ == 0);
  CHECK(label_);
  offset_ = c->code_holder().labelOffsetFromBase(*label_);
  label_.reset();
}

CodeX64::CodeX64(CodeGen* cg)
    : const_pool_label_(),
      zone_(1024 * 4),
      const_pool_(&zone_),
      /** must be initialized **/
      code_holder_(std::make_unique<asmjit::CodeHolder>()) {
  code_holder_->init(Jit::GetInstance().jit_runtime().environment());
}

void CodeX64::SetUp(CodeGen* cg) {
  body_.emplace(cg->as()->newLabel());
  jit_prologue_.emplace(cg->as()->newLabel());
  jit_epilogue_.emplace(cg->as()->newLabel());
  deopt_.emplace(cg->as()->newLabel());
  cast_f64_to_b32_.emplace(cg->as()->newLabel());
  end_.emplace(cg->as()->newLabel());

  const_pool_label_ = cg->as()->newLabel();

  // setup the known constant stuff, order matters
  AddConstVal(1.0);
  AddConstVal(-1.0);
  AddConstVal(+0.0);
  AddConstVal(-0.0);

  // xmmword value, this is only used by xmmword
  {
    struct {
      double f0;
      double f1;
    } x;
    x.f0 = -0.0;
    x.f1 = -0.0;
    neg_zero_xmmword_ = AddXMMWord(reinterpret_cast<void*>(&x));
  }
}

std::size_t CodeX64::AddXMMWord(void* ptr) {
  std::size_t offset;
  const_pool_.add(ptr, 16, offset);  // const pool will make sure the alignment
  return offset;
}

std::size_t CodeX64::AddConstValIntoPool(CVal cval) {
  // okay, need to re-insert it
  std::size_t offset;
  const_pool_.add(cval.ValueAddress(), cval.ByteSize(), offset);
  const_val_.push_back(ConstVal{cval, offset});
  return const_val_.size() - 1;
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

  return AddConstValIntoPool(cval);
}

std::size_t CodeX64::AddConstVal(double v) { return AddConstVal(CVal::F64(v)); }

std::size_t CodeX64::AddConstVal(std::int64_t v) {
  return AddConstVal(CVal::I64(v));
}

asmjit::x86::Mem CodeX64::IndexConstVal(std::size_t idx) const {
  const auto& xx = const_val_[idx];
  switch (xx.cval.ByteSize()) {
    case 1:
      return asmjit::x86::byte_ptr(const_pool_label_, xx.offset);
    case 2:
      return asmjit::x86::word_ptr(const_pool_label_, xx.offset);
    case 4:
      return asmjit::x86::dword_ptr(const_pool_label_, xx.offset);
    case 8:
      return asmjit::x86::qword_ptr(const_pool_label_, xx.offset);
    default:
      UNREACHABLE();
      return asmjit::x86::ptr(const_pool_label_, xx.offset);
  }
}

asmjit::x86::Mem CodeX64::AddConst(CVal cval) {
  auto idx = AddConstVal(cval);
  return IndexConstVal(idx);
}

void CodeX64::EmbedConstPool(CodeGen* cg) {
  cg->as()->embedConstPool(const_pool_label_, const_pool_);
}

base::Error CodeX64::Link(CodeGen* cg) {
  DEFER([&]() {
    code_holder_->reset();
    code_holder_.reset();
  });

  // Materialize the code into jit runtime and obtain its C++ function entry
  if (auto e = Jit::GetInstance().AddCode(this); !e) {
    return e.PassError();
  } else {
    entry_ = e.PassResult();
  }

  {
    // linking the comment, the offset will not be changed, ie relative to BASE
    for (auto& entry : comment_holder_.code_comment_) {
      CHECK(!entry.IsLinked());
      entry.start_offset = code_holder_->labelOffsetFromBase(entry.start);
      entry.end_offset = code_holder_->labelOffsetFromBase(entry.end);
    }

    comment_holder_.Finalize();
  }

  {
    // linking all the known position
    jit_prologue_->Link(this);
    body_->Link(this);
    jit_epilogue_->Link(this);
    deopt_->Link(this);
    cast_f64_to_b32_->Link(this);
    end_->Link(this);
  }

  // set the code size
  total_size_ = static_cast<std::size_t>(code_holder_->codeSize());
  code_size_ =
      static_cast<std::size_t>(end_->offset() - jit_prologue_->offset());

  return {};
}

std::string CodeX64::Disassemble() const {
  CHECK(entry_);
  std::stringstream fmt;

  auto dis = disass::CreateDisassembler(reinterpret_cast<const void*>(entry_),
                                        code_size_);

  std::size_t idx = 0;
  for (; dis->HasNext(); dis->Next()) {
    auto cursor = dis->Cursor();

    if (auto comment =
            comment_holder_.GetComment(static_cast<std::uint64_t>(cursor));
        !comment.empty()) {
      fmt << "-------------------------------------------------\n";
      fmt << comment;
      fmt << "-------------------------------------------------\n";
    }

    if (auto maybe = dis->Print(); maybe) {
      fmt << "[" << idx << "] " << *maybe << "\n";
    } else {
      fmt << "<ERROR>\n";
      break;
    }

    idx++;
  }

  return fmt.str();
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
