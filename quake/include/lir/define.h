#ifndef _QUAKE_CG_LIR_DEFINE_H_
#define _QUAKE_CG_LIR_DEFINE_H_
#include "base.h"
#include "common.h"

namespace quake {
namespace lir {

#define HOPERAND_TYPE_LIST(__)                            \
  __(kNone, 0, "none")                                    \
  __(kMemInd, 1 << 2, "mem_ind")                          \
  __(kMemStatic, 1 << 3, "mem_static")                    \
  __(kMemStack, 1 << 4, "mem_stack")                      \
  __(kMemory, kMemInd | kMemStatic | kMemStack, "memory") \
  __(kImm, 1 << 1, "imm")                                 \
  __(kReg, 1 << 5, "reg")                                 \
  __(kRegOrImm, kReg | kImm, "reg_or_imm")                \
  __(kLabel, 1 << 6, "label")                             \
  __(kTemporary, 1 << 7, "temporary")

class HOperandType {
 public:
  // clang-format off
  enum Tag {
#define __(A, B, ...) A = B,
    HOPERAND_TYPE_LIST(__)
#undef __  // __
    kDummy
  };
  // clang-format on

  static inline const char* GetTagName(int);

  static HOperandType None() { return HOperandType{kNone}; }
  static HOperandType Imm() { return HOperandType{kImm}; }
  static HOperandType MemInd() { return HOperandType{kMemInd}; }
  static HOperandType MemStatic() { return HOperandType{kMemStatic}; }
  static HOperandType MemStack() { return HOperandType{kMemStack}; }
  static HOperandType Label() { return HOperandType{kLabel}; }
  static HOperandType Register() { return HOperandType{kReg}; }
  static HOperandType Temporary() { return HOperandType{kTemporary}; }

 public:
  explicit HOperandType(int tag) : tag_(tag) {}

 public:
  inline bool Match(const HOperandType&) const;
  inline std::string NormalizeName() const;

 public:
  bool IsNone() const { return tag_ == kNone; }
  bool IsMemInd() const { return tag_ == kMemInd; }
  bool IsMemStatic() const { return tag_ == kMemStatic; }
  bool IsMemStack() const { return tag_ == kMemStack; }
  bool IsImm() const { return tag_ == kImm; }
  bool IsReg() const { return tag_ == kReg; }
  bool IsLabel() const { return tag_ == kLabel; }
  bool IsTemporary() const { return tag_ == kTemporary; }
  bool IsMemory() const { return static_cast<bool>(tag_ & kMemory); }
  bool IsRegOrImm() const { return static_cast<bool>(tag_ & kRegOrImm); }

  bool operator==(const HOperandType& r) const { return tag_ == r.tag_; }
  bool operator!=(const HOperandType& r) const { return tag_ != r.tag_; }

 private:
  int tag_;
};

inline bool HOperandType::Match(const HOperandType& t) const {
  return static_cast<bool>(t.tag_ & tag_);
}

inline std::string HOperandType::NormalizeName() const {
  return base::Fmt("%s", GetTagName(tag_));
}

inline const char* HOperandType::GetTagName(int tag) {
  switch (tag) {
#define __(A, B, C) \
  case B:           \
    return C;
    HOPERAND_TYPE_LIST(__)
#undef __  // __
  }
}

}  // namespace lir
}  // namespace quake

#endif
