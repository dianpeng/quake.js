#ifndef _QUAKE_LIR_OPCODE_H_
#define _QUAKE_LIR_OPCODE_H_
#include "base.h"
#include "common.h"

#include "cg/abi.h"

#include "lir/register.h"

// ----------------------------------------------------------------------------
// HOPCODE definition
//
// A better approach is to directly make each LIR typped, but that will just
// explode the instruction space, however that makes the LIR easier to deal
// with mismatched type etc ...
//
// (enum, string-name, flag, register-constraint, output_size)

#define QUAKE_HOPCODE_LIST(__)                                        \
  /** unused **/                                                      \
  __(kSentry, "sentry", HFlag::kNone, HRegisterInfo::None(),          \
     HOpcodeOutputInfo::None())                                       \
  /** phi node **/                                                    \
  __(kPhiLV, "phi_lv", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::Unknown())                                    \
  /** immediate **/                                                   \
  __(kImm, "imm", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::Unknown())                                    \
  /** arithmetic **/                                                  \
  __(kIAdd, "iadd", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kISub, "isub", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kIMul, "imul", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kIDiv, "idiv", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kFAdd, "fadd", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                   \
  __(kFSub, "fsub", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                   \
  __(kFMul, "fmul", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                   \
  __(kFDiv, "fdiv", HFlag::kSet, HRegisterInfo::Input0AsOutput(),     \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                   \
  /** unary **/                                                       \
  __(kInc, "inc", HFlag::kSet, HRegisterInfo::Input0AsOutput(),       \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kDec, "dec", HFlag::kSet, HRegisterInfo::Input0AsOutput(),       \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kNeg, "neg", HFlag::kSet, HRegisterInfo::Input0AsOutput(),       \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kInv, "inv", HFlag::kSet, HRegisterInfo::Input0AsOutput(),       \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  /** bit **/                                                         \
  __(kAnd, "and", HFlag::kSet, HRegisterInfo::Input0AsOutput(),       \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kOr, "or", HFlag::kSet, HRegisterInfo::Input0AsOutput(),         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kXor, "xor", HFlag::kSet, HRegisterInfo::Input0AsOutput(),       \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kLShift, "lshift", HFlag::kSet, HRegisterInfo::Input0AsOutput(), \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  __(kRShift, "rshift", HFlag::kSet, HRegisterInfo::Input0AsOutput(), \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  /** condition **/                                                   \
  __(kTest, "test", HFlag::kSet, HRegisterInfo::None(),               \
     HOpcodeOutputInfo::None())                                       \
  __(kCmp, "cmp", HFlag::kSet, HRegisterInfo::None(),                 \
     HOpcodeOutputInfo::None())                                       \
  __(kJmpNe, "jmp_ne", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  __(kJmpEq, "jmp_eq", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  /*** signed </<= ***/                                               \
  __(kJmpLt, "jmp_lt", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  __(kJmpLe, "jmp_le", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  /*** unsigned </<= ***/                                             \
  __(kJmpBt, "jmp_bt", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  __(kJmpBe, "jmp_be", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  /*** signed >/>= ***/                                               \
  __(kJmpGt, "jmp_gt", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  __(kJmpGe, "jmp_ge", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  /*** unsigned >/>= ***/                                             \
  __(kJmpAt, "jmp_at", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  __(kJmpAe, "jmp_ae", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  /*** zero ***/                                                      \
  __(kJmpNz, "jmp_nz", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::None())                                       \
  __(kJmpZ, "jmp_z", HFlag::kNone, HRegisterInfo::None(),             \
     HOpcodeOutputInfo::None())                                       \
  /** move **/                                                        \
  __(kMov, "mov", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::Unknown())                                    \
  __(kMovZX, "movzx", HFlag::kNone, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::Unknown())                                    \
  __(kMovSX, "movsx", HFlag::kNone, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::Unknown())                                    \
  __(kMov64, "mov64", HFlag::kNone, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBits))                    \
  /** stack **/                                                       \
  __(kPop, "pop", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBits))                    \
  __(kPush, "push", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::None())                                       \
  /** memory address **/                                              \
  __(kLea, "lea", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::Ptr())                                        \
  /** special variable **/                                            \
  __(kLoadLocal, "load_local", HFlag::kNone, HRegisterInfo::None(),   \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                     \
  __(kLoadParam, "load_param", HFlag::kNone, HRegisterInfo::None(),   \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                     \
  __(kLoadTemp, "load_temp", HFlag::kNone, HRegisterInfo::None(),     \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                     \
  /** boxing and unboxing **/                                         \
  __(kBox, "box", HFlag::kSet, HRegisterInfo::None(),                 \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                     \
  __(kUnbox, "unbox", HFlag::kSet, HRegisterInfo::None(),             \
     HOpcodeOutputInfo::Unknown())                                    \
  __(kGetTTag, "get_ttag", HFlag::kSet, HRegisterInfo::None(),        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                     \
  /** deoptimization **/                                              \
  __(kDeopt, "deopt", HFlag::kInvalidate, HRegisterInfo::None(),      \
     HOpcodeOutputInfo::None())                                       \
  __(kIfDeopt, "if_deopt", HFlag::kInvalidate, HRegisterInfo::None(), \
     HOpcodeOutputInfo::None())

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// HOpcode is our LIR, ie low-level IR. The HOpcode stands for High-level Opcode
// for assembler (portable).
//
// The HOpcode enable certain key features for us
//
// 1) It is a data oriente definition of our HOpcode along with its rewriting
//    rules, ie how they get mapped to the detail arch-specific machine code
//
// 2) It provides each HOpcode's platform specific constraints, ie where its
//    output should go and how its input should be setup
//
// 3) It provides level of indirection allow us to port our JIT into different
//    platform(arch) in the future
// ----------------------------------------------------------------------------

enum class HFlag : std::uint8_t { kNone, kSet, kInvalidate };

enum HRegisterConstraint : std::uint8_t {
  kNone,
  kInput0AsOutput,
  kInput0AsOutput0,
  kOutputRaxRdx  // x86/64 special one, not appicable in ARM
};

using HRegisterConstraintVec =
    base::small_vector<HRegisterConstraint, cg::kParamGPRegisterSize>;

struct HRegisterInfo {
  HRegisterConstraintVec hint, req;

  HRegisterInfo() = default;

 public:
  static HRegisterInfo None() { return HRegisterInfo{}; }

  static HRegisterInfo Input0AsOutput() {
    auto r = HRegisterInfo{};
    r.hint.push_back(HRegisterConstraint::kInput0AsOutput);
    return r;
  }

  static HRegisterInfo Input0AsOutput0() {
    auto r = HRegisterInfo{};
    r.hint.push_back(HRegisterConstraint::kInput0AsOutput0);
    return r;
  }

  static HRegisterInfo OutputRaxRdx() {
    auto r = HRegisterInfo{};
    r.hint.push_back(HRegisterConstraint::kOutputRaxRdx);
    return r;
  }

 public:
  void Convert(RegisterSet* hint, const Register** force);
};

// LIR's type. In general LIR is strictly typped but never been explicitly
// make so. The HOpcode opcode itself typically already shows you what type
// it is indeed
//
// Notes the HOpcodeType is a strict lattice, the bottom is kNone, and the top
// is kBits
enum class HOpcodeType : std::uint8_t {
  kBits,  // no type information, treated it as binary, match against all type
          // ie it is at the top of the lattice
  kInt,
  kFloat,  // floating point type
  kPtr,    // pointer value, notes, unknown to us wether it is GCed or not
  kGC,     // gc pointer value, notes it is GC pointer, so we can do ref count
  kBox,    // box value
  kNone    // no type, or irrevelant
};

struct HOpcodeOutputInfo {
  std::uint8_t size;  // how many output will be produced, mostly 1
  SizeClass sc0;      // size class of output0
  SizeClass sc1;      // size class of output1, not used
  HOpcodeType t0;     // type of #0
  HOpcodeType t1;     // type of #1

  HOpcodeOutputInfo()
      : size(0),
        sc0(SizeClass::kIrrelevant),
        sc1(SizeClass::kIrrelevant),
        t0(HOpcodeType::kNone),
        t1(HOpcodeType::kNone) {}

  HOpcodeOutputInfo(SizeClass s, HOpcodeType t)
      : size(1),
        sc0(s),
        sc1(SizeClass::kIrrelevant),
        t0(t),
        t1(HOpcodeType::kNone) {}

  bool IsValue() const { return size > 0; }
  bool IsOp() const { return size == 0; }
  bool IsSValue() const { return size == 1; }

  static HOpcodeOutputInfo None() { return HOpcodeOutputInfo{}; }
  static HOpcodeOutputInfo Byte(HOpcodeType t) {
    return HOpcodeOutputInfo{SizeClass::kByte, t};
  }
  static HOpcodeOutputInfo Word(HOpcodeType t) {
    return HOpcodeOutputInfo{SizeClass::kWord, t};
  }
  static HOpcodeOutputInfo DWord(HOpcodeType t) {
    return HOpcodeOutputInfo{SizeClass::kDWord, t};
  }
  static HOpcodeOutputInfo QWord(HOpcodeType t) {
    return HOpcodeOutputInfo{SizeClass::kQWord, t};
  }
  static HOpcodeOutputInfo Ptr() {
    return HOpcodeOutputInfo{SizeClass::kQWord, HOpcodeType::kPtr};
  }
  static HOpcodeOutputInfo Unknown() {
    return HOpcodeOutputInfo{SizeClass::kUnknown, HOpcodeType::kBits};
  }
};

// clang-format off
enum class HOpcode : std::uint16_t {
#define __(A, ...) A,
  QUAKE_HOPCODE_LIST(__)
#undef __  // __
  kSizeOfHOpcode
};
// clang-format off

// ----------------------------------------------------------------------------
struct HOpcodeInfo {
  HOpcode opcode;               // enum of opcode
  std::string name;             // name of opcode
  HFlag flag;                   // flag status
  HRegisterInfo register_info;  // register info
  HOpcodeOutputInfo output;     // output info
};

const HOpcodeInfo& GetHOpcodeInfo(HOpcode);

inline bool IsHOpcodePhi(HOpcode);
inline bool IsHOpcodeDeopt(HOpcode);
inline bool IsHOpcodeValue(HOpcode);
inline bool IsHOpcodeOp(HOpcode);

}  // namespace lir
}  // namespace quake

#include "opcode-inl.h"

#endif  // _QUAKE_LIR_OPCODE_H_
