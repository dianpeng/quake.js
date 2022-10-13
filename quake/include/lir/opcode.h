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

#define QUAKE_HOPCODE_LIST(__)                                                 \
  /** unused **/                                                               \
  __(kSentry, "sentry", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::None())                                                \
  /** barrier to disallow moving instruction above, (code motion) */           \
  __(kBarrier, "barrier", HFlag::kNone, HRegisterInfo::None(),                 \
     HOpcodeOutputInfo::None())                                                \
  /** control flow block, ie translate into label **/                          \
  __(kBlock, "block", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::None())                                                \
  /** phi node **/                                                             \
  __(kPhiLoop, "phi_loop", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::Unknown())                                             \
  /** arithmetic **/                                                           \
  __(kIAdd, "iadd", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kISub, "isub", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kIMul, "imul", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kIDiv, "idiv", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kIMod, "imod", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kFAdd, "fadd", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                            \
  __(kFSub, "fsub", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                            \
  __(kFMul, "fmul", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                            \
  __(kFDiv, "fdiv", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                            \
  /** unary **/                                                                \
  __(kInc, "inc", HFlag::kSet, HRegisterInfo::None(),                          \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kDec, "dec", HFlag::kSet, HRegisterInfo::None(),                          \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kNeg, "neg", HFlag::kSet, HRegisterInfo::None(),                          \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kNegF, "negf", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kFloat))                            \
  __(kInv, "inv", HFlag::kSet, HRegisterInfo::None(),                          \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  /** bit **/                                                                  \
  __(kAnd, "and", HFlag::kSet, HRegisterInfo::None(),                          \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kOr, "or", HFlag::kSet, HRegisterInfo::None(),                            \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kXor, "xor", HFlag::kSet, HRegisterInfo::None(),                          \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kLShift, "lshift", HFlag::kSet, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kRShift, "rshift", HFlag::kSet, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kRLShift, "rlshift", HFlag::kSet, HRegisterInfo::None(),                  \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  /** comparison value **/                                                     \
  /*** -------------------- int comparison i ------------------- ***/          \
  __(kILt, "ilt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kILe, "ile", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kIGt, "igt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kIGe, "ige", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kIEq, "ieq", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kINe, "ine", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  /*** -------------------- float comparison ------------------- ***/          \
  __(kFLt, "flt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kFLe, "fle", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kFGt, "fgt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kFGe, "fge", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kFEq, "feq", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kFNe, "fne", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  /** condition **/                                                            \
  __(kCmpI, "cmpi", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::None())                                                \
  __(kCmpF, "cmpf", HFlag::kSet, HRegisterInfo::None(),                        \
     HOpcodeOutputInfo::None())                                                \
  __(kCmpFalse, "cmp_false", HFlag::kSet, HRegisterInfo::None(),               \
     HOpcodeOutputInfo::None())                                                \
  __(kCmpTrue, "cmp_true", HFlag::kSet, HRegisterInfo::None(),                 \
     HOpcodeOutputInfo::None())                                                \
  __(kBoxIsHeap, "box_is_heap", HFlag::kSet, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::None())                                                \
  __(kBoxIsPrimitive, "box_is_primitive", HFlag::kSet, HRegisterInfo::None(),  \
     HOpcodeOutputInfo::None())                                                \
  /**** jumps ****/                                                            \
  __(kJmpNe, "jmp_ne", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  __(kJmpEq, "jmp_eq", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  /*** signed </<= ***/                                                        \
  __(kJmpLt, "jmp_lt", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  __(kJmpLe, "jmp_le", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  /*** unsigned </<= ***/                                                      \
  __(kJmpBt, "jmp_bt", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  __(kJmpBe, "jmp_be", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  /*** signed >/>= ***/                                                        \
  __(kJmpGt, "jmp_gt", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  __(kJmpGe, "jmp_ge", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  /*** unsigned >/>= ***/                                                      \
  __(kJmpAt, "jmp_at", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  __(kJmpAe, "jmp_ae", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  /*** zero ***/                                                               \
  __(kJmpNz, "jmp_nz", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  __(kJmpZ, "jmp_z", HFlag::kNone, HRegisterInfo::None(),                      \
     HOpcodeOutputInfo::None())                                                \
  /** move **/                                                                 \
  __(kMov, "mov", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kMovF, "movf", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                            \
  __(kMovZX, "movzx", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kMovSX, "movsx", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kMovSXD, "movsxd", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::Unknown())                                             \
  /** preparation of constant/immediate numbers **/                            \
  __(kMovI64, "movi64", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::QWord(HOpcodeType::kInt))                              \
  __(kMovB64, "movb64", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBits))                             \
  __(kMovI32, "movi32", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kMovF64, "movf64", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                            \
  /** stack **/                                                                \
  __(kPop, "pop", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBits))                             \
  __(kPush, "push", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::None())                                                \
  /** memory address **/                                                       \
  __(kLea, "lea", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::Ptr())                                                 \
  /** special variable **/                                                     \
  __(kLoadLocal, "load_local", HFlag::kNone, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                              \
  __(kLoadParam, "load_param", HFlag::kNone, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                              \
  __(kLoadTemp, "load_temp", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                              \
  /** call **/                                                                 \
  /** all the native call must have at most 4 arguments to fit into **/        \
  /** ABI's requirement. Notes arm requires at most 4 arguments to **/         \
  /** be in registers, so this simplifies our life **/                         \
  __(kFCallBox, "fcall_box", HFlag::kInvalidate, HRegisterInfo::None(),        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                              \
  __(kFCallPtr, "fcall_ptr", HFlag::kInvalidate, HRegisterInfo::None(),        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBits))                             \
  __(kFCallB64, "fcall_b64", HFlag::kInvalidate, HRegisterInfo::None(),        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBits))                             \
  __(kFCallF64, "fcall_f64", HFlag::kInvalidate, HRegisterInfo::None(),        \
     HOpcodeOutputInfo::QWord(HOpcodeType::kFloat))                            \
  __(kFCallI32, "fcall_i32", HFlag::kInvalidate, HRegisterInfo::None(),        \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  __(kFCallVoid, "fcall_void", HFlag::kInvalidate, HRegisterInfo::None(),      \
     HOpcodeOutputInfo::None())                                                \
  /** boxing and unboxing **/                                                  \
  __(kBox, "box", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::QWord(HOpcodeType::kBox))                              \
  __(kUnbox, "unbox", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kGetTTag, "get_ttag", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::DWord(HOpcodeType::kInt))                              \
  /** deoptimization **/                                                       \
  __(kDeopt, "deopt", HFlag::kInvalidate, HRegisterInfo::None(),               \
     HOpcodeOutputInfo::None())                                                \
  /** conditional deoptimization, used for guard 0, guard type lower **/       \
  __(kDeoptIfZ, "deopt_if_z", HFlag::kInvalidate, HRegisterInfo::None(),       \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptIfEq, "deopt_if_eq", HFlag::kInvalidate, HRegisterInfo::None(),     \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptIfNe, "deopt_if)ne", HFlag::kInvalidate, HRegisterInfo::None(),     \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptIfTrue, "deopt_if_true", HFlag::kInvalidate, HRegisterInfo::None(), \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptIfFalse, "deopt_if_false", HFlag::kInvalidate,                      \
     HRegisterInfo::None(), HOpcodeOutputInfo::None())

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// HOpcode is our LIR, ie low-level IR. The HOpcode stands for
// High-level Opcode for assembler (portable).
//
// The HOpcode enable certain key features for us
//
// 1) It is a data oriente definition of our HOpcode along with its rewriting
//    rules, ie how they get mapped to the detail arch-specific machine code
//
// 2) It provides each HOpcode's platform specific constraints, ie where
// its
//    output should go and how its input should be setup
//
// 3) It provides level of indirection allow us to port our JIT into
// different
//    platform(arch) in the future
// ----------------------------------------------------------------------------

enum class HFlag : std::uint8_t { kNone, kSet, kInvalidate };

struct HRegisterInfo {
  HRegisterInfo() = default;

  explicit HRegisterInfo(bool fs) : force_spill(fs) {}

  // For simplicity, just use some flags(policy) to force RA to behave
  // in certain ways. This part of RA configuration toggle is quit
  // messy, RA requires info from LIR, also information from ABI and
  // CodeGen. If we try to express it in a flexible way, we will end up
  // designing yet another logic system which is typically a overkill.If
  // we use simple policy, we may end up in a messy implementation :(

  bool force_spill = false;  // do not use register to hold it but always spill
                             // it in the first try. This is useful for things
                             // like compressed frame used by inline function.
                             // It should never reside in register until it is
                             // first use. In most case, it is never in use

 public:
  static HRegisterInfo None() { return HRegisterInfo{}; }
  static HRegisterInfo ForceSpill() { return HRegisterInfo{true}; }
};

// LIR's type. In general LIR is strictly typped but never been
// explicitly make so. The HOpcode opcode itself typically already shows
// you what type it is indeed
//
// Notes the HOpcodeType is a strict lattice, the bottom is kNone, and
// the top is kBits
enum class HOpcodeType : std::uint8_t {
  kBits,  // no type information, treated it as binary, match against
          // all type ie it is at the top of the lattice
  kInt,
  kFloat,  // floating point type
  kPtr,    // pointer value, notes, unknown to us wether it is GCed or not
  kGC,     // gc pointer value, notes it is GC pointer, so we can do ref
           // count
  kBox,    // box value
  kNone    // no type, or irrevelant
};

inline RegisterType HOpcodeTypeToRegisterType(HOpcodeType t) {
  return t == HOpcodeType::kFloat ? RegisterType::kFloat
                                  : RegisterType::kGeneral;
}

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

  bool IsFloat() const { return t0 == HOpcodeType::kFloat; }
  bool IsBits() const { return t0 == HOpcodeType::kBits; }
  bool IsInt() const { return t0 == HOpcodeType::kInt; }
  bool IsPtr() const { return t0 == HOpcodeType::kPtr; }
  bool IsGC() const { return t0 == HOpcodeType::kGC; }
  bool IsBox() const { return t0 == HOpcodeType::kBox; }

  RegisterType output0_rtype() const { return HOpcodeTypeToRegisterType(t0); }
  RegisterType output1_rtype() const { return HOpcodeTypeToRegisterType(t1); }

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
inline bool IsHOpcodeJump(HOpcode);
inline bool IsHOpcodeComp(HOpcode);
inline bool IsHOpcodeMove(HOpcode);
inline bool IsHOpcodeValue(HOpcode);
inline bool IsHOpcodeNativeCall(HOpcode);

inline bool IsHOpcodeOp(HOpcode);

}  // namespace lir
}  // namespace quake

#include "opcode-inl.h"

#endif  // _QUAKE_LIR_OPCODE_H_
