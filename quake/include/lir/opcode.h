#ifndef _QUAKE_LIR_OPCODE_H_
#define _QUAKE_LIR_OPCODE_H_
#include "base.h"
#include "common.h"
#include "lir/htype.h"
#include "lir/register.h"

#include "cg/abi.h"

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
  __(kPlaceholder, "placeholder", HFlag::kNone, HRegisterInfo::None(),         \
     HOpcodeOutputInfo::None())                                                \
  /** barrier to disallow moving instruction above, (code motion) */           \
  __(kBarrier, "barrier", HFlag::kNone, HRegisterInfo::None(),                 \
     HOpcodeOutputInfo::None())                                                \
  /** control flow block, ie translate into label **/                          \
  __(kBlock, "block", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::None())                                                \
  __(kStart, "start", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::None())                                                \
  __(kEnd, "end", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::None())                                                \
  __(kBB1, "bb1", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::None())                                                \
  /** phi node **/                                                             \
  /*** loop phi, ie phi node been used for the looping during reemittion ***/  \
  __(kPhiLoopF, "phi_loop_f64", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  __(kPhiLoopI, "phi_loop_i", HFlag::kNone, HRegisterInfo::None(),             \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  /** arithmetic **/                                                           \
  /*** addd ***/                                                               \
  __(kIAdd, "iadd", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kGIAdd, "giadd", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFAdd, "fadd", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** addd ***/                                                               \
  __(kISub, "isub", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kGISub, "gisub", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFSub, "fsub", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** mul ***/                                                                \
  __(kIMul, "imul", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kGIMul, "gimul", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFMul, "fmul", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** div ***/                                                                \
  __(kFDiv, "fdiv", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** mod ***/                                                                \
  __(kIMod, "imod", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFMod, "fmod", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** pow ***/                                                                \
  __(kFPow, "fpow", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /** unary **/                                                                \
  /*** inc ***/                                                                \
  __(kGInc, "ginc", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kInc, "inc", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kIncF, "incf", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** dec ***/                                                                \
  __(kGDec, "gdec", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kDec, "dec", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kDecF, "decf", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** neg ***/                                                                \
  __(kGNeg, "gneg", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kNeg, "neg", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kNegF, "negf", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /*** inv ***/                                                                \
  __(kInv, "inv", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  /** bit **/                                                                  \
  __(kAnd, "and", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kOr, "or", HFlag::kNone, HRegisterInfo::None(),                           \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kXor, "xor", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kShlConst, "shl_const", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kShrConst, "shr_const", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kSarConst, "sar_const", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kShl, "shl", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kShr, "shr", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kSar, "sar", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  /** comparison value **/                                                     \
  /*** -------------------- int comparison i ------------------- ***/          \
  __(kILt, "ilt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kILe, "ile", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kIGt, "igt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kIGe, "ige", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kIEq, "ieq", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kINe, "ine", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  /*** -------------------- float comparison ------------------- ***/          \
  __(kFLt, "flt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFLe, "fle", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFGt, "fgt", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFGe, "fge", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFEq, "feq", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kFNe, "fne", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  /*** -------------------- int comparison i ------------------- ***/          \
  __(kBoxIsHeap, "box_is_heap", HFlag::kNone, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kBoxIsPrimitive, "box_is_primitive", HFlag::kNone, HRegisterInfo::None(), \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  /** branch operations, high level jump takes 2 operand, will be lowered **/  \
  __(kBranchTrue, "branch_true", HFlag::kNone, HRegisterInfo::None(),          \
     HOpcodeOutputInfo::None())                                                \
  __(kBranchFalse, "branch_false", HFlag::kNone, HRegisterInfo::None(),        \
     HOpcodeOutputInfo::None())                                                \
  /* low level comparison, will not be generated by translator but lowered */  \
  /* optimizer */                                                              \
  /** branch/cmp **/                                                           \
  __(kCmpI, "cmpi", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::None())                                                \
  __(kCmpF, "cmpf", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::None())                                                \
  __(kCmpTest, "cmp_test", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  __(kCmpBoxIsHeap, "cmp_box_is_heap", HFlag::kNone, HRegisterInfo::None(),    \
     HOpcodeOutputInfo::None())                                                \
  __(kCmpBoxIsPrimitive, "cmp_box_is_primitive", HFlag::kNone,                 \
     HRegisterInfo::None(), HOpcodeOutputInfo::None())                         \
  /**** conditional jumps ****/                                                \
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
  /** work with kCmpBoxIsHeap/kCmpBoxIsPrimitive */                            \
  __(kJmpBoxIsHeap, "jmp_box_is_heap", HFlag::kNone, HRegisterInfo::None(),    \
     HOpcodeOutputInfo::None())                                                \
  __(kJmpBoxIsPrimitive, "jmp_box_is_primitive", HFlag::kNone,                 \
     HRegisterInfo::None(), HOpcodeOutputInfo::None())                         \
  /** unconditional jump **/                                                   \
  __(kJmp, "jmp", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::None())                                                \
  /** move **/                                                                 \
  __(kMov, "mov", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kMovF, "mov_f64", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /** speical moves **/                                                        \
  __(kTrans, "trans", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kTransF, "trans_f64", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kShuffle, "shuffle", HFlag::kNone, HRegisterInfo::None(),                 \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kShuffleF, "shuffle_f64", HFlag::kNone, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::Unknown())                                             \
  /** bits widening etc ... **/                                                \
  __(kMovZX, "movzx", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kMovSX, "movsx", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::Unknown())                                             \
  __(kMovSXD, "movsxd", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::Unknown())                                             \
  /** preparation of constant/immediate numbers **/                            \
  __(kMovB64, "movb64", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::QWord(HType::kBits))                                   \
  __(kMovI64, "movi64", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::QWord(HType::kInt))                                    \
  __(kMovI32, "movi32", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kMovF64, "movf64", HFlag::kNone, HRegisterInfo::None(),                   \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  /** memory address **/                                                       \
  __(kLea, "lea", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::Ptr())                                                 \
  /** stack operation, push/pop etc, used by RA to shuffle register **/        \
  __(kPush, "push", HFlag::kNone, HRegisterInfo::None(),                       \
     HOpcodeOutputInfo::None())                                                \
  /*** notes, pop will resume a register back, but we do not treat pop as ***/ \
  /*** an expression, instead it is just a op that recover a register ***/     \
  __(kPop, "pop", HFlag::kNone, HRegisterInfo::None(),                         \
     HOpcodeOutputInfo::None())                                                \
  /** special variable **/                                                     \
  __(kLoadLocal, "load_local", HFlag::kUseCell, HRegisterInfo::None(),         \
     HOpcodeOutputInfo::QWord(HType::kBoxRef))                                 \
  __(kLoadParam, "load_param", HFlag::kUseCell, HRegisterInfo::None(),         \
     HOpcodeOutputInfo::QWord(HType::kBoxRef))                                 \
  __(kLoadTemp, "load_temp", HFlag::kUseCell, HRegisterInfo::None(),           \
     HOpcodeOutputInfo::QWord(HType::kBoxRef))                                 \
  /** cast **/                                                                 \
  __(kCastI32ToF64, "cast_i32_to_f64", HFlag::kNone, HRegisterInfo::None(),    \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  __(kCastF64ToB32, "cast_f64_to_b32", HFlag::kNone, HRegisterInfo::None(),    \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  /** call **/                                                                 \
  /** all the native call must have at most 4 arguments to fit into **/        \
  /** ABI's requirement. Notes arm requires at most 4 arguments to **/         \
  /** be in registers, so this simplifies our life **/                         \
  __(kRCallPtr, "rcall_ptr", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::QWord(HType::kPtr))                                    \
  __(kRCallGC, "rcall_ptr", HFlag::kNone, HRegisterInfo::None(),               \
     HOpcodeOutputInfo::QWord(HType::kGC))                                     \
  __(kRCallB64, "rcall_b64", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::QWord(HType::kBits))                                   \
  __(kRCallF64, "rcall_f64", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  __(kRCallI32, "rcall_i32", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kRCallVoid, "rcall_void", HFlag::kNone, HRegisterInfo::None(),            \
     HOpcodeOutputInfo::None())                                                \
  /** ref-count **/                                                            \
  __(kRcInc, "rc_inc", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  __(kRcDec, "rc_dec", HFlag::kNone, HRegisterInfo::None(),                    \
     HOpcodeOutputInfo::None())                                                \
  /** boxing and unboxing **/                                                  \
  __(kGUnboxI, "g_unbox_i", HFlag::kNone, HRegisterInfo::None(),               \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kGUnboxF, "g_unbox_f64", HFlag::kNone, HRegisterInfo::None(),             \
     HOpcodeOutputInfo::QWord(HType::kFloat))                                  \
  __(kUnboxPtr, "unbox_ptr", HFlag::kNone, HRegisterInfo::None(),              \
     HOpcodeOutputInfo::Ptr())                                                 \
  __(kGetTTag, "get_ttag", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::DWord(HType::kInt))                                    \
  __(kIsNullOrUndefined, "is_null_or_undefined", HFlag::kNone,                 \
     HRegisterInfo::None(), HOpcodeOutputInfo::DWord(HType::kInt))             \
  /** snapshot **/                                                             \
  /*** snapshot does nothing but just hold all references to node ***/         \
  __(kSnapshot, "snapshot", HFlag::kNone, HRegisterInfo::None(),               \
     HOpcodeOutputInfo::None())                                                \
  /** deoptimization **/                                                       \
  __(kDeopt, "deopt", HFlag::kNone, HRegisterInfo::None(),                     \
     HOpcodeOutputInfo::None())                                                \
  /** conditional deoptimization, used for guard 0, guard type lower **/       \
  __(kDeoptIfTrue, "deopt_if_true", HFlag::kNone, HRegisterInfo::None(),       \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptIfFalse, "deopt_if_false", HFlag::kNone, HRegisterInfo::None(),     \
     HOpcodeOutputInfo::None())                                                \
  /**** conditional deoptIf ****/                                              \
  __(kDeoptNe, "deopt_ne", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptEq, "deopt_eq", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  /*** signed </<= ***/                                                        \
  __(kDeoptLt, "deopt_lt", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptLe, "deopt_le", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  /*** signed >/>= ***/                                                        \
  __(kDeoptGt, "deopt_gt", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptGe, "deopt_ge", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  /*** zero ***/                                                               \
  __(kDeoptNz, "deopt_nz", HFlag::kNone, HRegisterInfo::None(),                \
     HOpcodeOutputInfo::None())                                                \
  __(kDeoptZ, "deopt_z", HFlag::kNone, HRegisterInfo::None(),                  \
     HOpcodeOutputInfo::None())

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

enum class HFlag : std::uint8_t {
  kNone = 0,
  kUseCell = 1  // the instruction will use cell
};

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

struct HOpcodeOutputInfo {
  std::uint8_t size;  // how many output will be produced, mostly 1
  SizeClass sc0;      // size class of output0
  SizeClass sc1;      // size class of output1, not used
  HType t0;           // type of #0
  HType t1;           // type of #1

  HOpcodeOutputInfo()
      : size(0),
        sc0(SizeClass::kIrrelevant),
        sc1(SizeClass::kIrrelevant),
        t0(HType::kNone),
        t1(HType::kNone) {}

  HOpcodeOutputInfo(SizeClass s, HType t)
      : size(1), sc0(s), sc1(SizeClass::kIrrelevant), t0(t), t1(HType::kNone) {}

  bool IsValue() const { return size > 0; }
  bool IsOp() const { return size == 0; }
  bool IsSValue() const { return size == 1; }

  bool IsFloat() const { return t0 == HType::kFloat; }
  bool IsBits() const { return t0 == HType::kBits; }
  bool IsInt() const { return t0 == HType::kInt; }
  bool IsPtr() const { return t0 == HType::kPtr; }
  bool IsGC() const { return t0 == HType::kGC; }
  bool IsBox() const { return t0 == HType::kBox; }

  RegisterType output0_rtype() const { return HTypeToRegisterType(t0); }
  RegisterType output1_rtype() const { return HTypeToRegisterType(t1); }

  static HOpcodeOutputInfo None() { return HOpcodeOutputInfo{}; }
  static HOpcodeOutputInfo Byte(HType t) {
    CHECK(t != HType::kFloat);
    return HOpcodeOutputInfo{SizeClass::kByte, t};
  }
  static HOpcodeOutputInfo Word(HType t) {
    CHECK(t != HType::kFloat);
    return HOpcodeOutputInfo{SizeClass::kWord, t};
  }
  static HOpcodeOutputInfo DWord(HType t) {
    CHECK(t != HType::kFloat);
    return HOpcodeOutputInfo{SizeClass::kDWord, t};
  }
  static HOpcodeOutputInfo QWord(HType t) {
    return HOpcodeOutputInfo{SizeClass::kQWord, t};
  }
  static HOpcodeOutputInfo Ptr() {
    return HOpcodeOutputInfo{SizeClass::kQWord, HType::kPtr};
  }
  static HOpcodeOutputInfo Unknown() {
    return HOpcodeOutputInfo{SizeClass::kUnknown, HType::kBits};
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

  HType output_type_single() const {
    return output.t0;
  }
};

const HOpcodeInfo& GetHOpcodeInfo(HOpcode);

inline bool IsHOpcodePhi(HOpcode);

// deoptimization
inline bool IsHOpcodeDeopt(HOpcode);
inline bool IsHOpcodeDeoptBranch(HOpcode);

// moves
inline bool IsHOpcodeMove(HOpcode);

// native call
inline bool IsHOpcodeRCall(HOpcode);
inline bool IsHOpcodeNativeCall(HOpcode);

// conditions/jumps
inline bool IsHOpcodeBranch(HOpcode);
inline bool IsHOpcodeCondition(HOpcode);
inline bool IsHOpcodeCmp(HOpcode);
inline bool IsHOpcodeJump(HOpcode);

// HOpcode node that is also a guard, ie requires deoptimization code
inline bool IsHOpcodeGuard(HOpcode);

inline bool IsHOpcodeValue(HOpcode);
inline bool IsHOpcodeOp(HOpcode);

}  // namespace lir
}  // namespace quake

#include "opcode-inl.h"

#endif  // _QUAKE_LIR_OPCODE_H_
