#ifndef _QUAKE_OPCODE_H_
#define _QUAKE_OPCODE_H_
#include <quake-api.h>

#include "base.h"
#include "type.h"

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
    kSideEffect = base::BitMaskOn<std::uint32_t, 1>::kValue,

    // Whether this opcode is a guard, ie try to jump to previously saved
    // checkpoint. Notes an opcode that is guard does not generate checkpoint,
    // but just reuse the previously saved checkpoint
    kGuard = base::BitMaskOn<std::uint32_t, 2>::kValue,

    // Whether this opcode is a value opcode, ie generate some value to be used
    // by others. If this bit is 0, it means this node is not a value node but
    // a OP node, ie just consume operands but not produce operands
    kValue = base::BitMaskOn<std::uint32_t, 3>::kValue,

    // Whether this opcode is a control flow opcode
    kControlFlow = base::BitMaskOn<std::uint32_t, 4>::kValue,

    // Whether this opcode may error out due to check error
    kException = base::BitMaskOn<std::uint32_t, 5>::kValue,

    // Whether this instruction is a internal node, ie it is always a part of
    // some other node and should not be tracked directly inside of CFG. For
    // example, the RcDec/RcInc is both internal nodes for RcOps nodes
    kInternal = base::BitMaskOn<std::uint32_t, 6>::kValue
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

 public:
  bool IsSideEffect() const { return v_ & kSideEffect; }
  void ClearSideEffect() { v_ &= ~kSideEffect; }
  void SetSideEffect() { v_ |= kSideEffect; }

  bool IsValue() const { return v_ & kValue; }
  void ClearValue() { v_ &= ~kValue; }
  void SetValue() { v_ |= kValue; }
  bool IsOp() const { return !IsValue(); }

  bool IsControlFlow() const { return v_ & kControlFlow; }
  void ClearControlFlow() { v_ &= ~kControlFlow; }
  void SetControlFlow() { v_ |= kControlFlow; }

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

 private:
  std::uint32_t v_;
};

constexpr int kNarity = -1;

// ----------------------------------------------------------------------------
// Quake's IR
// The design of quake's IR is based on SSA style but tries to keep it as
// compact as possible
//
// About the PHI node. The phi is been categorized into different sub-types
// to allow simpler type inference. The PHI node that contains same type or
// similar type, ie both are numbers(int/float), and can be casted implicitly
// is been orgnized into PhiS. For type that totally does not to be the same
// is orgnized otherwise.

// IR, Name, Arity, Flag

#define QUAKE_OPCODE_LIST(__)                                              \
  /** constant loading **/                                                 \
  __(KI32, "ki32", 0, IREffect::Value(), Type::kInt)                       \
  __(KI64, "ki64", 0, IREffect::Value(), Type::kI64)                       \
  __(KFloat64, "kreal", 0, IREffect::Value(), Type::kFloat64)              \
  __(KBool, "kbool", 0, IREffect::Value(), Type::kBool)                    \
  __(KNull, "knull", 0, IREffect::Value(), Type::kNull)                    \
  __(KUndef, "kundef", 0, IREffect::Value(), Type::kUndefined)             \
  __(KNStr, "knstr", 0, IREffect::Value(), Type::kString)                  \
  __(KStr, "kstr", 0, IREffect::Value(), Type::kString)                    \
  /** ref counting **/                                                     \
  /* notes all the ref-counting is pinned */                               \
  __(RcInc, "rc_inc", 1, IREffect::InternalSideEffectValue(), Type::kNone) \
  __(RcDec, "rc_dec", 1, IREffect::InternalSideEffectValue(), Type::kNone) \
  __(RcTryInc, "rc_tryinc", 1, IREffect::SideEffectOp(), Type::kNone)      \
  __(RcTryDec, "rc_trydec", 1, IREffect::SideEffectOp(), Type::kNone)      \
  __(RcOp, "rc_op", kNarity, IREffect::SideEffectOp(), Type::kNone)        \
  /** box/unbox **/                                                        \
  __(BxOn, "bx_on", 1, IREffect::Value(), Type::kBox)                      \
  __(BxOff, "bx_off", 1, IREffect::Value(), Type::kInfer)                  \
  __(BxGuard, "bx_guard", 1, IREffect::GuardValue(), Type::kInfer)         \
  /** guard **/                                                            \
  __(GType, "g_type", 1, IREffect::GuardValue(), Type::kInfer)             \
  __(GNotZeroInt, "g_notzeroint", 1, IREffect::GuardValue(), Type::kInt)   \
  /** local variables **/                                                  \
  __(LocGet, "loc_get", 1, IREffect::Value(), Type::kBox)                  \
  __(LocSet, "loc_set", 2, IREffect::Value(), Type::kNone)                 \
  /** argument **/                                                         \
  __(ParamGet, "param_get", 1, IREffect::Value(), Type::kBox)              \
  __(ParamSet, "param_set", 2, IREffect::Value(), Type::kNone)             \
  /** temp **/                                                             \
  __(TempGet, "temp_get", 1, IREffect::Value(), Type::kBox)                \
  __(TempSet, "temp_set", 2, IREffect::Value(), Type::kNone)               \
  /** arithmetic **/                                                       \
  __(AddInt, "add_int", 2, IREffect::Value(), Type::kInt)                  \
  __(AddReal, "add_real", 2, IREffect::Value(), Type::kFloat64)            \
  __(SubInt, "sub_int", 2, IREffect::Value(), Type::kInt)                  \
  __(SubReal, "sub_real", 2, IREffect::Value(), Type::kFloat64)            \
  __(MulInt, "mul_int", 2, IREffect::Value(), Type::kInt)                  \
  __(MulReal, "mul_real", 2, IREffect::Value(), Type::kFloat64)            \
  __(DivInt, "div_int", 2, IREffect::Value(), Type::kInt)                  \
  __(DivReal, "div_real", 2, IREffect::Value(), Type::kFloat64)            \
  /** bit operation **/                                                    \
  __(ShlInt, "shl_int", 2, IREffect::Value(), Type::kInt)                  \
  __(ShrInt, "shr_int", 2, IREffect::Value(), Type::kInt)                  \
  __(SarInt, "sar_int", 2, IREffect::Value(), Type::kInt)                  \
  __(BitNorm, "bit_norm", 1, IREffect::Value(), Type::kInt)                \
  __(AndInt, "and_int", 2, IREffect::Value(), Type::kInt)                  \
  __(OrInt, "or_int", 2, IREffect::Value(), Type::kInt)                    \
  __(XorInt, "xor_int", 2, IREffect::Value(), Type::kInt)                  \
  /** comparison **/                                                       \
  /** FIXME(dpeng) Add string specialization **/                           \
  __(LtInt, "lt_int", 2, IREffect::Value(), Type::kBool)                   \
  __(LtReal, "lt_real", 2, IREffect::Value(), Type::kBool)                 \
  __(LeInt, "le_int", 2, IREffect::Value(), Type::kBool)                   \
  __(LeReal, "le_real", 2, IREffect::Value(), Type::kBool)                 \
  __(GtInt, "gt_int", 2, IREffect::Value(), Type::kBool)                   \
  __(GtReal, "gt_real", 2, IREffect::Value(), Type::kBool)                 \
  __(GeInt, "ge_int", 2, IREffect::Value(), Type::kBool)                   \
  __(GeReal, "ge_real", 2, IREffect::Value(), Type::kBool)                 \
  __(EqInt, "eq_int", 2, IREffect::Value(), Type::kBool)                   \
  __(EqReal, "eq_real", 2, IREffect::Value(), Type::kFloat64)              \
  __(NeInt, "ne_int", 2, IREffect::Value(), Type::kInt)                    \
  __(NeReal, "ne_int", 2, IREffect::Value(), Type::kFloat64)               \
  /** unary **/                                                            \
  __(NegInt, "neg_int", 1, IREffect::Value(), Type::kInt)                  \
  __(NegReal, "neg_real", 1, IREffect::Value(), Type::kFloat64)            \
  __(IncInt, "inc_int", 1, IREffect::Value(), Type::kInt)                  \
  __(DecInt, "dec_int", 1, IREffect::Value(), Type::kInt)                  \
  /** phi **/                                                              \
  __(PhiLV, "phi_lv", 2, IREffect::Value(), Type::kInfer)                  \
  /** control flow **/                                                     \
  __(ExitIf, "exit_if", 1, IREffect::ControlFlow(), Type::kNone)

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

const OpcodeInfo& GetOpcodeInfo(Opcode);

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_OPCODE_H_

