#ifndef _QUAKE_OPCODE_H_
#define _QUAKE_OPCODE_H_
#include <quake-api.h>

#include "base.h"
#include "type.h"

// ----------------------------------------------------------------------------
// Quake's IR
//
// The design of quake's IR is based on SSA style but tries to keep it as
// compact as possible
//
// About the PHI node. The phi is been categorized into different sub-types
// to allow simpler type inference. The PHI node that contains same type or
// similar type, ie both are numbers(int/float), and can be casted implicitly
// is been orgnized into PhiS. For type that totally does not to be the same
// is orgnized otherwise.

// IR, Name, Arity, Flag

#define QUAKE_OPCODE_LIST(__)                                                  \
  /** constant loading **/                                                     \
  __(kKI32, "ki32", 0, IREffect::InternalValue(), Type::kInt)                  \
  __(kKI64, "ki64", 0, IREffect::InternalValue(), Type::kI64)                  \
  __(kKFloat64, "kreal", 0, IREffect::InternalValue(), Type::kFloat64)         \
  __(kKBool, "kbool", 0, IREffect::InternalValue(), Type::kBool)               \
  __(kKNull, "knull", 0, IREffect::InternalValue(), Type::kNull)               \
  __(kKUndef, "kundef", 0, IREffect::InternalValue(), Type::kUndefined)        \
  __(kKNStr, "knstr", 0, IREffect::InternalValue(), Type::kString)             \
  __(kKStr, "kstr", 0, IREffect::InternalValue(), Type::kString)               \
  __(kKUninit, "kuninit", 0, IREffect::InternalValue(), Type::kUninitialized)  \
  /** ref counting **/                                                         \
  /* notes all the ref-counting is pinned */                                   \
  __(kRcInc, "rc_inc", 1, IREffect::InternalSideEffectOp(), Type::kNone)       \
  __(kRcDec, "rc_dec", 1, IREffect::InternalSideEffectOp(), Type::kNone)       \
  __(kRcTryInc, "rc_tryinc", 1, IREffect::InternalSideEffectOp(), Type::kNone) \
  __(kRcTryDec, "rc_trydec", 1, IREffect::InternalSideEffectOp(), Type::kNone) \
  __(kRcOp, "rc_op", kNarity, IREffect::SideEffectOp(), Type::kNone)           \
  /** box/unbox **/                                                            \
  __(kBxGuard, "bx_guard", 1, IREffect::GuardValue(), Type::kInfer)            \
  __(kBx, "bx", 1, IREffect::Value(), Type::kInfer)                            \
  /** guard **/                                                                \
  __(kGType, "g_type", 1, IREffect::GuardValue(), Type::kInfer)                \
  __(kGIsTrue, "g_is_true", 1, IREffect::GuardOp(), Type::kNone)               \
  __(kGIsFalse, "g_is_false", 1, IREffect::GuardOp(), Type::kNone)             \
  __(kGIsIntPos, "g_is_int_pos", 1, IREffect::GuardValue(), Type::kInt)        \
  __(kGIsIntPosOrZero, "g_is_int_pos_or_zero", 1, IREffect::GuardValue(),      \
     Type::kInt)                                                               \
  __(kGIsIntNeg, "g_is_int_neg", 1, IREffect::GuardValue(), Type::kInt)        \
  __(kGIsNotIntMax, "g_is_not_int_max", 1, IREffect::GuardValue(), Type::kInt) \
  __(kGIsNotIntMin, "g_is_not_int_min", 1, IREffect::GuardValue(), Type::kInt) \
  /** local variables **/                                                      \
  __(kLocGet, "loc_get", 1, IREffect::Value(), Type::kBox)                     \
  __(kLocSet, "loc_set", 2, IREffect::Op(), Type::kNone)                       \
  /** argument **/                                                             \
  __(kParamGet, "param_get", 1, IREffect::Value(), Type::kBox)                 \
  __(kParamSet, "param_set", 2, IREffect::Value(), Type::kNone)                \
  /** temp **/                                                                 \
  __(kTempGet, "temp_get", 1, IREffect::Value(), Type::kBox)                   \
  /** Valuearithmetic **/                                                      \
  __(kAddInt, "add_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kAddReal, "add_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kGAddInt, "g_add_int", 2, IREffect::GuardValue(), Type::kInt)             \
  __(kSubInt, "sub_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kSubReal, "sub_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kGSubInt, "g_sub_int", 2, IREffect::GuardValue(), Type::kInt)             \
  __(kMulInt, "mul_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kMulReal, "mul_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kGMulInt, "g_mul_int", 2, IREffect::GuardValue(), Type::kInt)             \
  __(kDivInt, "div_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kDivReal, "div_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kModInt, "mod_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kModReal, "mod_real", 2, IREffect::Value(), Type::kFloat64)               \
  __(kPowInt, "pow_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kPowReal, "pow_real", 2, IREffect::Value(), Type::kFloat64)               \
  /** bit operation **/                                                        \
  __(kShlInt, "shl_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kShrInt, "shr_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kSarInt, "sar_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kBitNorm, "bit_norm", 1, IREffect::Value(), Type::kInt)                   \
  __(kAndInt, "and_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kOrInt, "or_int", 2, IREffect::Value(), Type::kInt)                       \
  __(kXorInt, "xor_int", 2, IREffect::Value(), Type::kInt)                     \
  __(kInvInt, "inv_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kInvBool, "inv_bool", 1, IREffect::Value(), Type::kBool)                  \
  /** comparison **/                                                           \
  __(kIsType, "is_type", 2, IREffect::Value(), Type::kBool)                    \
  __(kIsTypeUndefinedOrNull, "is_type_undefined_or_null", 2,                   \
     IREffect::Value(), Type::kBool)                                           \
  /** FIXME(dpeng) Add string specialization **/                               \
  __(kLtInt, "lt_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kLtReal, "lt_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kLeInt, "le_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kLeReal, "le_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kGtInt, "gt_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kGtReal, "gt_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kGeInt, "ge_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kGeReal, "ge_real", 2, IREffect::Value(), Type::kBool)                    \
  __(kEqInt, "eq_int", 2, IREffect::Value(), Type::kBool)                      \
  __(kEqReal, "eq_real", 2, IREffect::Value(), Type::kFloat64)                 \
  __(kNeInt, "ne_int", 2, IREffect::Value(), Type::kInt)                       \
  __(kNeReal, "ne_int", 2, IREffect::Value(), Type::kFloat64)                  \
  /** unary **/                                                                \
  __(kGNegInt, "g_neg_int", 1, IREffect::GuardValue(), Type::kInt)             \
  __(kNegInt, "neg_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kNegReal, "neg_real", 1, IREffect::Value(), Type::kFloat64)               \
  __(kGIncInt, "g_inc_int", 1, IREffect::GuardValue(), Type::kInt)             \
  __(kIncInt, "inc_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kIncReal, "inc_real", 1, IREffect::Value(), Type::kFloat64)               \
  __(kGDecInt, "g_dec_int", 1, IREffect::GuardValue(), Type::kInt)             \
  __(kDecInt, "dec_int", 1, IREffect::Value(), Type::kInt)                     \
  __(kDecReal, "dec_real", 1, IREffect::Value(), Type::kFloat64)               \
  __(kToBoolInt, "to_bool_int", 1, IREffect::Value(), Type::kBool)             \
  __(kToBoolReal, "to_bool_real", 1, IREffect::Value(), Type::kBool)           \
  /** cast **/                                                                 \
  __(kI2F, "i2f", 1, IREffect::Value(), Type::kFloat64)                        \
  __(kF2Bit, "f2bit", 1, IREffect::Value(), Type::kInt)                        \
  /** phi **/                                                                  \
  __(kPhiLoop, "phi_loop", 2, IREffect::Value(), Type::kInfer)                 \
  /** control flow **/                                                         \
  /** a dummy control flow node, ie does nothing but just mark **/             \
  __(kStart, "start", 0, IREffect::ControlFlow(), Type::kNone)                 \
  __(kBB0, "bb0", 0, IREffect::ControlFlow(), Type::kNone)                     \
  __(kJump, "jump", 1, IREffect::ControlFlow(), Type::kNone)                   \
  __(kEnd, "end", 0, IREffect::ControlFlow(), Type::kNone)

namespace quake {
namespace ir {

// Used to point to a SSA defined value
class SSAVar;
class Checkpoint;

class IREffect {
 public:
  enum {
    // Whether this opcode has side-effect, ie generate a checkpoint after the
    // instruction is done
    kSideEffect = base::BitOn<std::uint32_t, 1>::kValue,

    // Whether this opcode is a guard, ie try to jump to previously saved
    // checkpoint. Notes an opcode that is guard does not generate checkpoint,
    // but just reuse the previously saved checkpoint
    kGuard = base::BitOn<std::uint32_t, 2>::kValue,

    // Whether this opcode is a value opcode, ie generate some value to be used
    // by others. If this bit is 0, it means this node is not a value node but
    // a OP node, ie just consume operands but not produce operands
    kValue = base::BitOn<std::uint32_t, 3>::kValue,

    // Whether this opcode is a control flow opcode
    kControlFlow = base::BitOn<std::uint32_t, 4>::kValue,

    // Whether this opcode may error out due to check error
    kException = base::BitOn<std::uint32_t, 5>::kValue,

    // Whether this instruction is a internal node, ie it is always a part of
    // some other node and should not be tracked directly inside of CFG. For
    // example, the RcDec/RcInc is both internal nodes for RcOps nodes
    kInternal = base::BitOn<std::uint32_t, 6>::kValue
  };

 public:
  explicit IREffect(std::uint32_t v) : v_(v) {}

  IREffect() : v_(0) {}

  // clang-format off
  IREffect(bool is_side_effect, 
           bool is_guard, 
           bool is_value,
           bool is_control_flow, 
           bool is_exception,
           bool is_internal)
      : v_(0) {

    if (is_side_effect) SetSideEffect();
    if (is_guard) SetGuard();

    if (is_value) {
      CHECK(!is_control_flow);
      SetValue();
    } else if (is_control_flow) {
      SetControlFlow();
    }

    if (is_exception) SetException();
    if (is_internal) SetInternal();
  }
  // clang-format on

 public:
  static IREffect SideEffectValue() { return IREffect{kSideEffect | kValue}; }
  static IREffect GuardValue() { return IREffect{kValue | kGuard}; }
  static IREffect Value() { return IREffect{kValue}; }
  static IREffect Op() { return IREffect{0}; }
  static IREffect GuardOp() { return IREffect{kGuard}; }
  static IREffect SideEffectOp() { return IREffect{kSideEffect}; }
  static IREffect ControlFlow() { return IREffect{kControlFlow}; }

  static IREffect InternalValue() { return IREffect{kInternal | kValue}; }
  static IREffect InternalSideEffectValue() {
    return IREffect{kInternal | kSideEffect | kValue};
  }
  static IREffect InternalSideEffectOp() {
    return IREffect{kInternal | kSideEffect};
  }

 public:
  bool IsSideEffect() const { return v_ & kSideEffect; }
  void ClearSideEffect() { v_ &= ~kSideEffect; }
  void SetSideEffect() { v_ |= kSideEffect; }

  bool IsValue() const { return v_ & kValue; }
  void ClearValue() { v_ &= ~kValue; }
  void SetValue() { v_ |= kValue; }

  bool IsControlFlow() const { return v_ & kControlFlow; }
  void ClearControlFlow() { v_ &= ~kControlFlow; }
  void SetControlFlow() { v_ |= kControlFlow; }

  bool IsOp() const { return !IsValue() && !IsControlFlow(); }

  bool IsGuard() const { return v_ & kGuard; }
  void SetGuard() { v_ |= kGuard; }
  void ClearGuard() { v_ &= ~kGuard; }

  bool IsException() const { return v_ & kException; }
  void ClearException() { v_ &= ~kException; }
  void SetException() { v_ |= kException; }

  bool IsInternal() const { return v_ & kInternal; }
  void ClearInternal() { v_ &= ~kInternal; }
  void SetInternal() { v_ |= kInternal; }

 public:
  bool RequireBailout() const { return IsGuard(); }

  std::uint32_t v() const { return v_; }

 private:
  std::uint32_t v_;
};

constexpr int kNarity = -1;

struct OpcodeInfo {
  std::string name;  // name of opcode
  int arity;         // argument count
  IREffect effect;   // flags of opcode
  Type type;         // type of opcode

  // Dyanmic sized arguments
  bool IsNary() const { return arity < 0; }
};

enum class Opcode : std::uint32_t {
// clang-format off
#define __(A, ...) A,
  QUAKE_OPCODE_LIST(__)
  kSizeOfOpcode
#undef __  // __
  // clang-format on
};

constexpr std::size_t kSizeOfOpcode =
    static_cast<std::size_t>(Opcode::kSizeOfOpcode);

std::optional<Opcode> OpcodeParse(const std::string_view&);

const OpcodeInfo& GetOpcodeInfo(Opcode);

// Categories of each OPCODE
bool OpcodeIsConst(Opcode);
bool OpcodeIsPhi(Opcode);
bool OpcodeIsJump(Opcode);

bool OpcodeIsArithmetic(Opcode);
bool OpcodeIsBinaryArithmetic(Opcode);
bool OpcodeIsUnaryArithmetic(Opcode);

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_OPCODE_H_

