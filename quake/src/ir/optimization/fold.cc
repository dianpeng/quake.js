#include "ir/optimization/fold.h"
#include "quake-rt.h"
#include <cmath>

/**
 * Caveats of FOLDING pass. Notes, most of the IR operators are already typped,
 * which means we cannot do constant folding that transfer the type, ie like
 * type promotion due to the dual number usage. When we found situation that
 * will result in a type mutation we just report bailout to the caller since
 * anyway the jitted code will bailout on the very first run.
 *
 *
 * Example : -(-1<<31) => this will result in a floating point, similar with
 *   arithmetic operator sub/div/mul which may overflow the int32 range directly
 *   into floating point number
 */

namespace quake {
namespace ir {
namespace {

enum { kAdd, kSub, kMul, kDiv, kMod };
enum { kLt, kLe, kGt, kGe, kEq, kNe };
enum { kInc, kDec, kNeg };

template <int kOp, typename T>
bool IsIntOverflow(const T& lhs, const T& rhs) {
  auto max = std::numeric_limits<T>::max();
  auto min = std::numeric_limits<T>::min();

  switch (kOp) {
    case kAdd:
      // lhs + rhs > max => overflow (rhs > 0)
      // lhs + rhs < min => overflow (lhs < 0)
      if (rhs > 0) {
        return lhs > (max - rhs);
      } else if (lhs < 0) {
        return lhs < (min - rhs);
      } else {
        return false;
      }

    case kSub:
      // lhs - rhs < min => overflow (rhs > 0)
      // lhs - rhs > max => overflow (rhs < 0)
      if (rhs > 0) {
        return lhs < (min + rhs);
      } else if (rhs < 0) {
        return lhs > (max + rhs);
      } else {
        return false;
      }

    case kMul: {
      // lhs * rhs > max => overflow
      // lhs * rhs < min => overflow
      T value = lhs * rhs;
      if (value / rhs == lhs) {
        return false;
      } else {
        return true;
      }
    }

    case kMod:
      return false;
    default:
      UNREACHABLE();
      return false;
  }
}

template <int kOp, typename T>
double FloatBinary(const T& lhs, const T& rhs) {
  switch (kOp) {
    case kAdd:
      return static_cast<double>(lhs) + static_cast<double>(rhs);
    case kSub:
      return static_cast<double>(lhs) - static_cast<double>(rhs);
    case kMul:
      return static_cast<double>(lhs) * static_cast<double>(rhs);
    case kDiv:
      return static_cast<double>(lhs) / static_cast<double>(rhs);
    case kMod:
      return std::fmod(static_cast<double>(lhs), static_cast<double>(rhs));
    default:
      UNREACHABLE();
      return 0;
  }
}

template <int kOp, typename T>
bool DoFoldConstantComp(const T& lhs, const T& rhs) {
  switch (kOp) {
    case kLt:
      return lhs < rhs;
    case kLe:
      return lhs <= rhs;
    case kGt:
      return lhs > rhs;
    case kGe:
      return lhs >= rhs;
    case kEq:
      return lhs == rhs;
    case kNe:
      return lhs != rhs;
    default:
      UNREACHABLE();
      return false;
  }
}

template <int kOp>
bool DoFoldConstantCompStr(Quake* q, const std::string_view& lhs,
                           const std::string_view& rhs) {
  auto js_context = q->js_context();
  auto js_str_lhs = JS_NewStringLen(js_context, lhs.data(), lhs.size());
  auto js_str_rhs = JS_NewStringLen(js_context, rhs.data(), rhs.size());
  DEFER([&]() {
    JS_FreeValue(js_context, js_str_lhs);
    JS_FreeValue(js_context, js_str_rhs);
  });

  auto result = QJS_JSStringCompare(
      js_context, static_cast<JSString*>(JS_VALUE_GET_PTR(js_str_lhs)),
      static_cast<JSString*>(JS_VALUE_GET_PTR(js_str_rhs)));

  switch (kOp) {
    case kLt:
      return result < 0;
    case kLe:
      return result <= 0;
    case kGt:
      return result > 0;
    case kGe:
      return result >= 0;
    case kEq:
      return result == 0;
    case kNe:
      return result != 0;

    default:
      UNREACHABLE();
      return false;
  }
}

template <int kOp, typename T>
Node* DoFoldConstantBinaryInt(Graph* graph, BytecodeInfoProvider* info,
                              const T& lhs_value, const T& rhs_value) {
  switch (kOp) {
    case kAdd:
      if (IsIntOverflow<kOp>(lhs_value, rhs_value)) {
        return nullptr;
      } else {
        return graph->KI32(info, lhs_value + rhs_value)->node_checked();
      }

    case kSub:
      if (IsIntOverflow<kOp>(lhs_value, rhs_value)) {
        return nullptr;
      } else {
        return graph->KI32(info, lhs_value - rhs_value)->node_checked();
      }

    case kMul:
      if (IsIntOverflow<kOp>(lhs_value, rhs_value)) {
        return nullptr;
      } else {
        return graph->KI32(info, lhs_value * rhs_value)->node_checked();
      }

    case kMod:
      return graph->KI32(info, lhs_value % rhs_value)->node_checked();

    default:
      UNREACHABLE();
      return nullptr;
  }

  return nullptr;
}

template <int kOp>
Node* FoldConstantBinaryInt(Graph* graph, BytecodeInfoProvider* info, Node* lhs,
                            Node* rhs) {
  switch (lhs->opcode()) {
    case Opcode::kKI32: {
      auto lhs_value = lhs->Int32();
      auto rhs_value = rhs->Int32();
      return DoFoldConstantBinaryInt<kOp>(graph, info, lhs_value, rhs_value);
    } break;

    case Opcode::kKI64: {
      auto lhs_value = lhs->Int64();
      auto rhs_value = rhs->Int64();
      return DoFoldConstantBinaryInt<kOp>(graph, info, lhs_value, rhs_value);
    } break;

    default:
      UNREACHABLE();
      break;
  }

  return nullptr;
}

template <int kOp>
Node* FoldConstantBinaryFloat(Graph* graph, BytecodeInfoProvider* info,
                              Node* lhs, Node* rhs) {
  switch (lhs->opcode()) {
    case Opcode::kKFloat64: {
      auto lhs_value = lhs->Float64();
      auto rhs_value = rhs->Float64();
      return graph->KFloat64(info, FloatBinary<kOp>(lhs_value, rhs_value))
          ->node_checked();
    } break;

    default:
      UNREACHABLE();
      break;
  }

  return nullptr;
}

template <int kOp>
Node* FoldConstantUnaryInt(Graph* graph, BytecodeInfoProvider* info,
                           Node* opr) {
  switch (opr->opcode()) {
    case Opcode::kKI32:
      switch (kOp) {
        case kNeg: {
          auto i32 = opr->Int32();
          if (i32 == std::numeric_limits<std::int32_t>::min()) {
            return nullptr;
          } else {
            return graph->KI32(info, -i32)->node_checked();
          }
        }

        case kInc:
          if (IsIntOverflow<kAdd>(opr->Int32(), 1)) {
            return nullptr;
          } else {
            return graph->KI32(info, opr->Int32() + 1)->node_checked();
          }

        case kDec:
          if (IsIntOverflow<kSub>(opr->Int32(), 1)) {
            return nullptr;
          } else {
            return graph->KI32(info, opr->Int32() - 1)->node_checked();
          }

        default:
          UNREACHABLE();
          return nullptr;
      }

    case Opcode::kKI64:
      switch (kOp) {
        case kNeg: {
          auto i64 = opr->Int64();
          if (i64 == std::numeric_limits<std::int64_t>::min()) {
            return nullptr;
          } else {
            return graph->KI64(info, -opr->Int64())->node_checked();
          }
        }

        case kInc:
          if (IsIntOverflow<kAdd>(opr->Int64(), static_cast<std::int64_t>(1))) {
            return nullptr;
          } else {
            return graph->KI64(info, opr->Int64() + 1)->node_checked();
          }

        case kDec:
          if (IsIntOverflow<kSub>(opr->Int64(), static_cast<std::int64_t>(1))) {
            return nullptr;
          } else {
            return graph->KI64(info, opr->Int64() - 1)->node_checked();
          }

        default:
          UNREACHABLE();
          return nullptr;
      }

    default:
      UNREACHABLE();
      return nullptr;
  }
}

template <int kOp>
Node* FoldConstantUnaryReal(Graph* graph, BytecodeInfoProvider* info,
                            Node* opr) {
  switch (kOp) {
    case kNeg:
      return graph->KFloat64(info, -opr->Float64())->node_checked();

    case kInc:
      return graph->KFloat64(info, opr->Float64() + 1.0)->node_checked();

    case kDec:
      return graph->KFloat64(info, opr->Float64() - 1.0)->node_checked();

    default:
      UNREACHABLE();
      return nullptr;
  }
}

// ----------------------------------------------------------------------------
// Perform reassociation of expression
//
// (C0 OP1 X) OP2 C1
//
// ----------------------------------------------------------------------------

template <int kOp, Opcode kInt, Opcode kGInt, Opcode kReal>
SSAVar* DoReassociateBinary(Graph* graph, Node* node) {
  auto rhs = node->Rhs()->node_checked();
  auto lhs = node->Lhs()->node_checked();
  auto info = node;

  auto rhs_rhs = rhs->Rhs();
  auto rhs_lhs = rhs->Lhs();

  auto maybe_guard = [&]() -> Checkpoint* {
    if (node->IsGuard()) return node->exit_checked();
    if (rhs->IsGuard()) return rhs->exit_checked();
    return nullptr;
  };

  auto constant_node = DoFoldConstantBinaryInt<kOp>(
      graph, info, lhs->Int32(), rhs_lhs->node_checked()->Int32());

  // overflow
  if (constant_node == nullptr) {
    return nullptr;
  }

  if (constant_node->IsConstFloat()) {
    // The constant node is not a int node, but promote to float node, then we
    // will have to use real number operation
    auto to_float = graph->AddValue(info, Opcode::kI2F, rhs_rhs);
    to_float->node_checked()->InsertAfter(node);
    return graph->AddValue(info, kReal, constant_node->out(), to_float);
  } else {
    if (auto guard = maybe_guard(); guard) {
      auto new_node =
          graph->AddValue(info, kGInt, constant_node->out(), rhs_rhs);
      new_node->node_checked()->BailoutTo(guard);
      return new_node;
    } else {
      return graph->AddValue(info, kInt, constant_node->out(), rhs_rhs);
    }
  }
}

}  // namespace

// ============================================================================
//
// Constant Folding
//
// ============================================================================

Fold::Result Fold::FoldConstantBinary(Graph* graph, Node* node) {
  auto lhs = node->Lhs()->node_checked();
  auto rhs = node->Rhs()->node_checked();

  CHECK(lhs->IsConst());
  CHECK(rhs->IsConst());

  switch (node->opcode()) {
    // ------------------------------------------------------------------------
    // integer arithmetic operations
    case Opcode::kAddInt:
    case Opcode::kGAddInt:
      if (auto value = FoldConstantBinaryInt<kAdd>(graph, node, lhs, rhs);
          value) {
        return Replace(value);
      } else {
        return Bailout();
      }

    case Opcode::kSubInt:
    case Opcode::kGSubInt:
      if (auto value = FoldConstantBinaryInt<kSub>(graph, node, lhs, rhs);
          value) {
        return Replace(value);
      } else {
        return Bailout();
      }

    case Opcode::kMulInt:
    case Opcode::kGMulInt:
      if (auto value = FoldConstantBinaryInt<kMul>(graph, node, lhs, rhs);
          value) {
        return Replace(value);
      } else {
        return Bailout();
      }

    case Opcode::kModInt:
      if (auto value = FoldConstantBinaryInt<kMul>(graph, node, lhs, rhs);
          value) {
        return Replace(value);
      } else {
        return Bailout();
      }

    // ------------------------------------------------------------------------
    // float arithmetic operations
    case Opcode::kAddReal:
      return Replace(FoldConstantBinaryFloat<kAdd>(graph, node, lhs, rhs));

    case Opcode::kSubReal:
      return Replace(FoldConstantBinaryFloat<kSub>(graph, node, lhs, rhs));

    case Opcode::kMulReal:
      return Replace(FoldConstantBinaryFloat<kMul>(graph, node, lhs, rhs));

    case Opcode::kDivReal:
      return Replace(FoldConstantBinaryFloat<kDiv>(graph, node, lhs, rhs));

    case Opcode::kModReal:
      return Replace(FoldConstantBinaryFloat<kMod>(graph, node, lhs, rhs));

    case Opcode::kPowReal: {
      auto x = lhs->Float64();
      auto y = rhs->Float64();
      return Replace(graph->KFloat64(node, QuakeJit::JSPow(x, y)));
    }

    default:
      UNREACHABLE();
      return NoChange();
  }
}

Fold::Result Fold::FoldConstantBitBinary(Graph* graph, Node* node) {
  auto lhs = node->Lhs()->node_checked();
  auto rhs = node->Rhs()->node_checked();
  auto info = node;

  CHECK(lhs->IsConst());
  CHECK(rhs->IsConst());

  switch (node->opcode()) {
    case Opcode::kGShlInt: {
      auto lhs_val = static_cast<std::uint64_t>(lhs->UInt32());
      auto rhs_val = static_cast<std::uint64_t>(rhs->UInt32());
      auto const_value = lhs_val << rhs_val;
      if (const_value > static_cast<std::uint64_t>(
                            std::numeric_limits<std::int32_t>::max())) {
        return Bailout();
      }
      return Replace(graph->KI32(info, const_value));
    }

    case Opcode::kGShrInt:
      // Notes the difference, both operands are uint32 type
      return Replace(graph->KI32(info, lhs->UInt32() >> rhs->UInt32()));

    case Opcode::kSarInt:
      return Replace(graph->KI32(info, lhs->Int32() >> rhs->UInt32()));

    case Opcode::kAndInt:
      return Replace(graph->KI32(info, lhs->Int32() & rhs->Int32()));
    case Opcode::kOrInt:
      return Replace(graph->KI32(info, lhs->Int32() | rhs->Int32()));
    case Opcode::kXorInt:
      return Replace(graph->KI32(info, lhs->Int32() ^ rhs->Int32()));

    default:
      UNREACHABLE();
      return NoChange();
  }
}

Fold::Result Fold::FoldConstantBitUnary(Graph* graph, Node* node) {
  auto opr = node->Opr()->node_checked();
  auto info = node;

  CHECK(opr->IsConst());

  switch (node->opcode()) {
    case Opcode::kBitNorm:
      return Replace(
          graph->KI32(info, opr->Int32() & static_cast<std::int32_t>(0x1f)));

    case Opcode::kInvInt:
      return Replace(graph->KI32(info, ~opr->UInt32()));

    case Opcode::kFlipBool:
      return Replace(graph->KBool(info, !opr->Bool()));

    default:
      UNREACHABLE();
      return NoChange();
  }
}

Fold::Result Fold::FoldConstantBit(Graph* graph, Node* node) {
  switch (node->opcode()) {
    case Opcode::kGShlInt:
    case Opcode::kGShrInt:
    case Opcode::kSarInt:
    case Opcode::kAndInt:
    case Opcode::kOrInt:
    case Opcode::kXorInt:
      return FoldConstantBitBinary(graph, node);

    case Opcode::kBitNorm:
    case Opcode::kInvInt:
    case Opcode::kFlipBool:
      return FoldConstantBitUnary(graph, node);

    default:
      UNREACHABLE();
      return NoChange();
  }
}

Fold::Result Fold::FoldConstantComp(Graph* graph, Node* node) {
  auto lhs = node->Lhs()->node_checked();
  auto rhs = node->Rhs()->node_checked();

  CHECK(lhs->IsConst());
  CHECK(rhs->IsConst());
  auto info = node;

  switch (node->opcode()) {
    case Opcode::kLtInt:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kLt>(lhs->Int32(), rhs->Int32())));
    case Opcode::kLeInt:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kLe>(lhs->Int32(), rhs->Int32())));
    case Opcode::kGtInt:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kGt>(lhs->Int32(), rhs->Int32())));
    case Opcode::kGeInt:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kGe>(lhs->Int32(), rhs->Int32())));
    case Opcode::kEqInt:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kEq>(lhs->Int32(), rhs->Int32())));
    case Opcode::kNeInt:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kNe>(lhs->Int32(), rhs->Int32())));

    case Opcode::kEqPtr:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kEq>(lhs->Int64(), rhs->Int64())));
    case Opcode::kNePtr:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kNe>(lhs->Int64(), rhs->Int64())));

    case Opcode::kLtReal:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kLt>(lhs->Float64(), rhs->Float64())));
    case Opcode::kLeReal:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kLe>(lhs->Float64(), rhs->Float64())));
    case Opcode::kGtReal:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kGt>(lhs->Float64(), rhs->Float64())));
    case Opcode::kGeReal:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kGe>(lhs->Float64(), rhs->Float64())));
    case Opcode::kEqReal:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kEq>(lhs->Float64(), rhs->Float64())));
    case Opcode::kNeReal:
      return Replace(graph->KBool(
          info, DoFoldConstantComp<kNe>(lhs->Float64(), rhs->Float64())));

    default:
      UNREACHABLE();
      return NoChange();
  }
}

// Special case for string comparison.
Fold::Result Fold::FoldConstantCompStr(Graph* graph, Node* node) {
  auto lhs = node->Lhs()->node_checked();
  auto rhs = node->Rhs()->node_checked();

  CHECK(lhs->opcode() == Opcode::kLoadStr);
  CHECK(rhs->opcode() == Opcode::kLoadStr);

  auto info = node;

  switch (node->opcode()) {
    case Opcode::kLtStr:
      return Replace(graph->KBool(
          info, DoFoldConstantCompStr<kLt>(quake_, lhs->Str(), rhs->Str())));
    case Opcode::kLeStr:
      return Replace(graph->KBool(
          info, DoFoldConstantCompStr<kLe>(quake_, lhs->Str(), rhs->Str())));
    case Opcode::kGtStr:
      return Replace(graph->KBool(
          info, DoFoldConstantCompStr<kGt>(quake_, lhs->Str(), rhs->Str())));
    case Opcode::kGeStr:
      return Replace(graph->KBool(
          info, DoFoldConstantCompStr<kGe>(quake_, lhs->Str(), rhs->Str())));
    case Opcode::kEqStr:
      return Replace(graph->KBool(
          info, DoFoldConstantCompStr<kEq>(quake_, lhs->Str(), rhs->Str())));
    case Opcode::kNeStr:
      return Replace(graph->KBool(
          info, DoFoldConstantCompStr<kNe>(quake_, lhs->Str(), rhs->Str())));
    default:
      UNREACHABLE();
      return NoChange();
  }
}

Fold::Result Fold::FoldConstantUnary(Graph* graph, Node* node) {
  auto opr = node->Opr()->node_checked();

  switch (node->opcode()) {
    case Opcode::kGNegInt:
    case Opcode::kNegInt:
      if (auto value = FoldConstantUnaryInt<kNeg>(graph, node, opr); value) {
        return Replace(value);
      } else {
        return Bailout();
      }

    case Opcode::kIncInt:
    case Opcode::kGIncInt:
      if (auto value = FoldConstantUnaryInt<kInc>(graph, node, opr); value) {
        return Replace(value);
      } else {
        return Bailout();
      }

    case Opcode::kDecInt:
    case Opcode::kGDecInt:
      if (auto value = FoldConstantUnaryInt<kDec>(graph, node, opr); value) {
        return Replace(value);
      } else {
        return Bailout();
      }

    case Opcode::kIncReal:
      return Replace(FoldConstantUnaryReal<kInc>(graph, node, opr));

    case Opcode::kDecReal:
      return Replace(FoldConstantUnaryReal<kDec>(graph, node, opr));

    case Opcode::kNegReal:
      return Replace(FoldConstantUnaryReal<kNeg>(graph, node, opr));

    default:
      UNREACHABLE();
      return NoChange();
  }
}

Fold::Result Fold::FoldConstantCast(Graph* graph, Node* node) {
  auto opr = node->Opr()->node_checked();
  auto info = node;

  CHECK(opr->IsConst());

  // Cast node
  switch (node->opcode()) {
    case Opcode::kI2F:
      return Replace(graph->KFloat64(info, static_cast<double>(opr->Int32())));

    case Opcode::kF2Bit:
      return Replace(graph->KI32(info, JVal::Float64ToBit32(opr->Float64())));

    case Opcode::kI2B:
      return Replace(graph->KBool(info, opr->Int32() != 0));

    case Opcode::kF2B:
      return Replace(graph->KBool(info, opr->Float64() != 0));

    case Opcode::kB2F:
      return Replace(graph->KFloat64(info, static_cast<double>(opr->Bool())));

    case Opcode::kB2I:
      return Replace(graph->KI32(info, static_cast<std::int32_t>(opr->Bool())));

    case Opcode::kB2S:
      return Replace(graph->LoadStr(info, opr->Bool() ? "true" : "false"));

    case Opcode::kI2S: {
      auto str = JVal::ToString(quake_->js_context(), opr->Int32());
      return Replace(graph->LoadStr(info, str));
    }
    case Opcode::kF2S: {
      auto str = JVal::ToString(quake_->js_context(), opr->Float64());
      return Replace(graph->LoadStr(info, str));
    }
    default:
      UNREACHABLE();
      return NoChange();
  }
}

Fold::Result Fold::FoldConstantStrOp(Graph* graph, Node* node) {
  auto info = node;

  switch (node->opcode()) {
    case Opcode::kStrCon: {
      auto lhs = node->Lhs()->node_checked();
      auto rhs = node->Rhs()->node_checked();
      if (lhs->opcode() == Opcode::kLoadStr &&
          rhs->opcode() == Opcode::kLoadStr) {
        auto lhs_str = lhs->Str();
        lhs_str += rhs->Str();
        return Replace(graph->LoadStr(info, lhs_str));
      }
    } break;

    case Opcode::kStrLen:
      if (node->Opr()->node_checked()->opcode() == Opcode::kLoadStr) {
        return Replace(
            graph->KI32(node, static_cast<std::int32_t>(
                                  node->Opr()->node_checked()->Str().size())));
      }
      break;

    case Opcode::kS2F:
      if (node->Opr()->node_checked()->opcode() == Opcode::kLoadStr) {
        auto opr = node->Opr()->node_checked();

        double value;
        // try to cast a string to float64
        auto js_str = JS_NewStringLen(quake_->js_context(), opr->Str().data(),
                                      opr->Str().size());
        DEFER([&]() { JS_FreeValue(quake_->js_context(), js_str); });
        value = QJS_JSStringToFloat64(
            quake_->js_context(),
            static_cast<JSString*>(JS_VALUE_GET_PTR(js_str)));

        return Replace(graph->KFloat64(node, value));
      }
      break;

    default:
      UNREACHABLE();
      break;
  }

  return NoChange();
}

// this function just tries to simplify the guard value
Fold::Result Fold::FoldConstantGuard(Graph* graph, Node* node) {
  auto opr = node->Opr()->node_checked();

  switch (node->opcode()) {
    case Opcode::kGIsIntPos:
      if (auto value = opr->Int32(); value <= 0) {
        return Bailout();
      } else {
        return Replace(opr->out());
      }

    case Opcode::kGIsIntPosOrZero:
      if (auto value = opr->Int32(); value < 0) {
        return Bailout();
      } else {
        return Replace(opr->out());
      }

    case Opcode::kGIsNotZero:
      if (auto value = opr->Int32(); value == 0) {
        return Bailout();
      } else {
        return Replace(opr->out());
      }

    case Opcode::kGIsIntNeg:
      if (auto value = opr->Int32(); value >= 0) {
        return Bailout();
      } else {
        return Replace(opr->out());
      }

    case Opcode::kGIsNotIntMax:
      if (auto value = opr->Int32();
          value == std::numeric_limits<std::int32_t>::max()) {
        return Bailout();
      } else {
        return Replace(opr->out());
      }

    case Opcode::kGIsNotIntMin:
      if (auto value = opr->Int32();
          value == std::numeric_limits<std::int32_t>::min()) {
        return Bailout();
      } else {
        return Replace(opr->out());
      }
    default:
      UNREACHABLE();
      break;
  }
  return NoChange();
}

Fold::Result Fold::TryConstantFold(Graph* graph, Node* node) {
  auto is_constant = [](Node* node) -> bool {
    return node->opcode() == Opcode::kLoadStr || node->IsConst();
  };

  // check whether it is a valid constant folding node, ie all its operands
  // are constant and also the opcode is allowed
  for (auto i = 0; i < node->size(); i++) {
    if (!is_constant(node->At(i)->node_checked())) return NoChange();
  }

  // dispatch to various handler for constant folding
  switch (node->opcode()) {
    default:
      break;

    // arithmetic operations
    case Opcode::kAddInt:
    case Opcode::kGAddInt:
    case Opcode::kAddReal:
    case Opcode::kSubInt:
    case Opcode::kGSubInt:
    case Opcode::kSubReal:
    case Opcode::kMulInt:
    case Opcode::kGMulInt:
    case Opcode::kMulReal:
    case Opcode::kDivReal:
    case Opcode::kModInt:
    case Opcode::kModReal:
    case Opcode::kPowReal:
      return FoldConstantBinary(graph, node);

    // bit operations
    case Opcode::kGShlInt:
    case Opcode::kGShrInt:
    case Opcode::kSarInt:
    case Opcode::kAndInt:
    case Opcode::kOrInt:
    case Opcode::kXorInt:
    case Opcode::kBitNorm:
    case Opcode::kInvInt:
    case Opcode::kFlipBool:
      return FoldConstantBit(graph, node);

    // comparison
    case Opcode::kLtInt:
    case Opcode::kLeInt:
    case Opcode::kGtInt:
    case Opcode::kGeInt:
    case Opcode::kEqInt:
    case Opcode::kNeInt:

    case Opcode::kLtReal:
    case Opcode::kLeReal:
    case Opcode::kGtReal:
    case Opcode::kGeReal:
    case Opcode::kEqReal:
    case Opcode::kNeReal:

    case Opcode::kEqPtr:
    case Opcode::kNePtr:
      return FoldConstantComp(graph, node);

    case Opcode::kLtStr:
    case Opcode::kLeStr:
    case Opcode::kGtStr:
    case Opcode::kGeStr:
    case Opcode::kEqStr:
    case Opcode::kNeStr:
      return FoldConstantCompStr(graph, node);

    // string operations
    case Opcode::kStrCon:
    case Opcode::kStrLen:
    case Opcode::kS2F:
      return FoldConstantStrOp(graph, node);

    // unary operations
    case Opcode::kGNegInt:
    case Opcode::kNegInt:
    case Opcode::kIncInt:
    case Opcode::kGIncInt:
    case Opcode::kDecInt:
    case Opcode::kGDecInt:

    case Opcode::kIncReal:
    case Opcode::kDecReal:
    case Opcode::kNegReal:
      return FoldConstantUnary(graph, node);

    // cast
    case Opcode::kI2F:
    case Opcode::kF2Bit:
    case Opcode::kI2B:
    case Opcode::kF2B:
    case Opcode::kB2F:
    case Opcode::kB2I:
    case Opcode::kB2S:
    case Opcode::kI2S:
    case Opcode::kF2S:
      return FoldConstantCast(graph, node);

    // guard
    case Opcode::kGIsIntPos:
    case Opcode::kGIsIntPosOrZero:
    case Opcode::kGIsNotZero:
    case Opcode::kGIsIntNeg:
    case Opcode::kGIsNotIntMax:
    case Opcode::kGIsNotIntMin:
      return FoldConstantGuard(graph, node);
  }

  return NoChange();
}

// ============================================================================
//
// Simplification
//
// ============================================================================

// LHS is constant value, try to do simplification.
Fold::Result Fold::SimplifyBinaryLHSConstant(Graph* graph, Node* node) {
  auto lhs = node->Lhs()->node_checked();
  auto rhs = node->Rhs()->node_checked();
  auto info = node;

  if (lhs->IsConstInt()) {
    switch (node->opcode()) {
      case Opcode::kAddInt:
      case Opcode::kGAddInt:
        // 0 + X = X
        //
        // notes 0 + X = X will never overflow/underflow, otherwise X cannot be
        // represented inside of integer type range
        if (lhs->IsIntZero()) return Replace(rhs);
        break;

      case Opcode::kSubInt:
      case Opcode::kGSubInt:
        // 0 - X = -X
        if (lhs->IsIntZero()) {
          if (node->opcode() == Opcode::kGSubInt) {
            auto new_node = graph->AddValue(info, Opcode::kGNegInt, rhs->out());
            new_node->node_checked()->BailoutTo(node->exit_checked());
            return Replace(new_node);
          } else {
            return Replace(graph->AddValue(info, Opcode::kNegInt, rhs->out()));
          }
        }
        break;

      case Opcode::kMulInt:
      case Opcode::kGMulInt:
        // TODO(dpeng): Add strength reduction if needed, to add it our shr/shl
        // requires guarded version, ie testify overflow flag and bailout if
        // needed. We need to add a new bunch of bit operation for this
        // operation

        // 0 * X = 0
        if (lhs->IsIntZero()) {
          return Replace(graph->KI32(info, 0));
        }

        // 1 * X = X
        if (lhs->IsIntOne()) {
          return Replace(rhs);
        }

        // -1 * X = -X
        if (lhs->IsIntNegOne()) {
          if (node->opcode() == Opcode::kGMulInt) {
            auto new_node = graph->AddValue(info, Opcode::kGNegInt, rhs->out());
            new_node->node_checked()->BailoutTo(node->exit_checked());
            return Replace(new_node);
          } else {
            return Replace(graph->AddValue(info, Opcode::kNegInt, rhs->out()));
          }
        }
        break;

      case Opcode::kModInt:
        // we cannot do anything up to my knowledge
        break;

      default:
        UNREACHABLE();
        break;
    }

  } else if (lhs->opcode() == Opcode::kKFloat64) {
    switch (node->opcode()) {
      case Opcode::kAddReal:
        if (lhs->Float64() == -0.0) return Replace(rhs);
        break;

      case Opcode::kSubReal:
        // -0.0 + v can be optimized, but we do not have LIR for now, and
        // personally I heavily doubt its value on our usage
        break;

      case Opcode::kMulReal:
        if (lhs->Float64() == 1.0) return Replace(rhs);

        // Notes due to float has negative/positive zero, we need to perform the
        // multiplication even if LHS is zero
        break;

      case Opcode::kDivReal:
        break;

      case Opcode::kModReal:
        break;

      case Opcode::kPowReal:
        break;

      default:
        UNREACHABLE();
        break;
    }
  }

  return NoChange();
}

// ----------------------------------------------------------------------------
// Reassociation
//
// we look for node pattern as following
//
// [OP] ---- [RHS: OP] --- [LHS: constant]
//   |         |
//   |         +---------- [RHS: X]
//   |
//   +--------- [LHS: constant]
//
//
// For simplification, currently we only reassociate integer operations
// ----------------------------------------------------------------------------

Fold::Result Fold::SimplifyReassociationLHSConst(Graph* graph, Node* node) {
  auto rhs = node->Rhs()->node_checked();

  auto allowed_op = [](Opcode op) -> bool {
    switch (op) {
      case Opcode::kAddInt:
      case Opcode::kGAddInt:
      case Opcode::kSubInt:
      case Opcode::kGSubInt:
      case Opcode::kMulInt:
      case Opcode::kGMulInt:
        return true;
      default:
        return false;
    }
  };

  auto maybe_rhs_constant = [&]() -> std::optional<std::int32_t> {
    if (allowed_op(rhs->opcode())) {
      if (rhs->Lhs()->node_checked()->opcode() == Opcode::kKI32) {
        return std::optional<std::int32_t>{rhs->Lhs()->node_checked()->Int32()};
      }
    }
    return {};
  };

  switch (node->opcode()) {
    case Opcode::kAddInt:
    case Opcode::kGAddInt:
      if (auto rhs_lhs = maybe_rhs_constant(); rhs_lhs) {
        switch (rhs->opcode()) {
          // C1 + (C2 + X) = (C1 + C2) + X
          case Opcode::kAddInt:
          case Opcode::kGAddInt: {
            AddCleanUp(rhs);
            if (auto value =
                    DoReassociateBinary<kAdd, Opcode::kAddInt, Opcode::kGAddInt,
                                        Opcode::kAddReal>(graph, node);
                value) {
              return Replace(value);
            } else {
              return Bailout();
            }
          }

          // C1 + (C2 - X) = (C1 + C2) - X
          case Opcode::kSubInt:
          case Opcode::kGSubInt: {
            AddCleanUp(rhs);
            if (auto value =
                    DoReassociateBinary<kSub, Opcode::kSubInt, Opcode::kGSubInt,
                                        Opcode::kSubReal>(graph, node);
                value) {
              return Replace(value);
            } else {
              return Bailout();
            }
          }

          default:
            break;
        }
      }
      break;

    case Opcode::kSubInt:
    case Opcode::kGSubInt:
      if (auto rhs_lhs = maybe_rhs_constant(); rhs_lhs) {
        switch (rhs->opcode()) {
          // (C0 + X) - C1 == (C0 - C1) + X
          case Opcode::kAddInt:
          case Opcode::kGAddInt: {
            AddCleanUp(rhs);
            if (auto value =
                    DoReassociateBinary<kSub, Opcode::kAddInt, Opcode::kGAddInt,
                                        Opcode::kAddReal>(graph, node);
                value) {
              return Replace(value);
            } else {
              return Bailout();
            }
          }

          // (C0 - X) - C1 == (C0 + C1) + X
          case Opcode::kSubInt:
          case Opcode::kGSubInt: {
            AddCleanUp(rhs);
            if (auto value =
                    DoReassociateBinary<kAdd, Opcode::kAddInt, Opcode::kGAddInt,
                                        Opcode::kAddReal>(graph, node);
                value) {
              return Replace(value);
            } else {
              return Bailout();
            }
          }

          default:
            break;
        }
      }
      break;

    case Opcode::kMulInt:
    case Opcode::kGMulInt:
      if (auto rhs_lhs = maybe_rhs_constant(); rhs_lhs) {
        switch (rhs->opcode()) {
          // (C0 * X) * C1 = ((C0 * C1) * X)
          case Opcode::kMulInt:
          case Opcode::kGMulInt: {
            AddCleanUp(rhs);
            if (auto value =
                    DoReassociateBinary<kMul, Opcode::kMulInt, Opcode::kGMulInt,
                                        Opcode::kMulReal>(graph, node);
                value) {
              return Replace(value);
            } else {
              return Bailout();
            }

          } break;

          default:
            break;
        }
      }
      break;

    default:
      break;
  }

  return NoChange();
}

Fold::Result Fold::SimplifyBinaryRHSConstant(Graph* graph, Node* node) {
  auto lhs = node->Lhs()->node_checked();
  auto rhs = node->Rhs()->node_checked();
  auto info = node;

  if (rhs->IsConstInt()) {
    switch (node->opcode()) {
      case Opcode::kAddInt:
      case Opcode::kGAddInt:
        // X + 0 = X
        if (rhs->IsIntZero()) {
          return Replace(lhs);
        }
        break;

      case Opcode::kSubInt:
      case Opcode::kGSubInt:
        // X - 0
        if (rhs->IsIntZero()) return Replace(lhs);
        break;

      case Opcode::kMulInt:
      case Opcode::kGMulInt:
        // TODO(dpeng): Add strength reduction if needed, to add it our
        // shr/shl requires guarded version, ie testify overflow flag and
        // bailout if needed. We need to add a new bunch of bit operation for
        // this operation

        // X * 0 = 0
        if (rhs->IsIntZero()) {
          return Replace(graph->KI32(info, 0));
        }

        // X * 1 = X
        if (rhs->IsIntOne()) {
          return Replace(lhs);
        }

        // X * -1 = -X
        if (rhs->IsIntNegOne()) {
          if (node->opcode() == Opcode::kGMulInt) {
            auto new_node = graph->AddValue(info, Opcode::kGNegInt, lhs->out());
            new_node->node_checked()->BailoutTo(node->exit_checked());
            return Replace(new_node);
          } else {
            return Replace(graph->AddValue(info, Opcode::kNegInt, lhs->out()));
          }
        }
        break;

      case Opcode::kModInt:
        // we cannot do anything up to my knowledge
        if (rhs->IsIntOne()) {
          return Replace(lhs);
        }
        break;

      default:
        UNREACHABLE();
        break;
    }

    switch (node->opcode()) {
      case Opcode::kAddInt:
      case Opcode::kGAddInt:
      case Opcode::kMulInt:
      case Opcode::kGMulInt: {
        auto new_node =
            graph->AddValue(info, node->opcode(), node->At(1), node->At(0));

        if (node->HasExit()) {
          new_node->node_checked()->BailoutTo(node->exit_checked());
        }

        // Swap the operand, which makes the lhs to be constant. This is
        // useful for our later reassociation simplification
        return Replace(new_node);
      }
      default:
        break;
    }

  } else if (rhs->opcode() == Opcode::kKFloat64) {
    switch (node->opcode()) {
      case Opcode::kAddReal:
        if (rhs->Float64() == -0.0) return Replace(lhs);
        break;

      case Opcode::kSubReal:
        // -0.0 + v can be optimized, but we do not have LIR for now, and
        // personally I heavily doubt its value on our usage
        if (rhs->Float64() == 0.0) return Replace(lhs);
        break;

      case Opcode::kMulReal:
        if (rhs->Float64() == 1.0) return Replace(lhs);

        // Notes due to float has negative/positive zero, we need to perform
        // the multiplication even if LHS is zero
        break;

      case Opcode::kDivReal:
        if (rhs->Float64() == 1.0) return Replace(lhs);
        break;

      case Opcode::kModReal:
        break;

      case Opcode::kPowReal:
        break;

      default:
        UNREACHABLE();
        break;
    }
  }

  return NoChange();
}

Fold::Result Fold::SimplifyBinaryGeneral(Graph* graph, Node* node) {
  auto lhs = node->Lhs()->node_checked();
  auto rhs = node->Rhs()->node_checked();
  auto info = node;

  // whehter lhs == -rhs or rhs == -lhs
  auto negative_of_other_int = [](Node* lhs, Node* rhs) {
    if (lhs->opcode() == Opcode::kNegInt || lhs->opcode() == Opcode::kGNegInt) {
      return lhs->Opr()->node_checked() == rhs;
    }
    if (rhs->opcode() == Opcode::kNegInt || rhs->opcode() == Opcode::kGNegInt) {
      return rhs->Opr()->node_checked() == lhs;
    }
    return false;
  };

  // Simplification
  switch (node->opcode()) {
    case Opcode::kAddInt:
    case Opcode::kGAddInt:
      if (negative_of_other_int(lhs, rhs)) {
        return Replace(graph->KI32(info, 0));
      }
      break;

    case Opcode::kAddReal:
      if (lhs->opcode() == Opcode::kNegReal &&
          lhs->Opr()->node_checked() == rhs) {
        return Replace(
            graph->AddValue(info, Opcode::kSubReal, rhs->out(), rhs->out()));
      }
      if (rhs->opcode() == Opcode::kNegReal &&
          rhs->Opr()->node_checked() == lhs) {
        return Replace(
            graph->AddValue(info, Opcode::kSubReal, lhs->out(), lhs->out()));
      }
      break;

    case Opcode::kSubInt:
    case Opcode::kGSubInt:
      if (lhs == rhs) {
        return Replace(graph->KI32(info, 0));
      }
      break;
    case Opcode::kSubReal:
      break;

    case Opcode::kMulInt:
    case Opcode::kGMulInt:
    case Opcode::kMulReal:
      break;

    case Opcode::kDivReal:
      break;

    case Opcode::kModInt:
    case Opcode::kModReal:
    case Opcode::kPowReal:
      break;

    default:
      UNREACHABLE();
      break;
  }

  return NoChange();
}

Fold::Result Fold::TrySimplifyBinary(Graph* graph, Node* node) {
  auto is_lhs_const = [](Node* node) -> bool {
    return node->Lhs()->node_checked()->IsConst();
  };

  auto is_rhs_const = [](Node* node) -> bool {
    return node->Rhs()->node_checked()->IsConst();
  };

  switch (node->opcode()) {
    default:
      break;

    case Opcode::kGAddInt:
    case Opcode::kGSubInt:
    case Opcode::kGMulInt:
    case Opcode::kAddInt:
    case Opcode::kSubInt:
    case Opcode::kMulInt:
    case Opcode::kModInt:
    case Opcode::kAddReal:
    case Opcode::kSubReal:
    case Opcode::kMulReal:
    case Opcode::kDivReal:
    case Opcode::kModReal:
    case Opcode::kPowReal:
      // LHS is constant
      if (is_lhs_const(node)) {
        CHECK(!is_rhs_const(node));
        if (auto x = SimplifyBinaryLHSConstant(graph, node); x.IsNoChange()) {
          return SimplifyReassociationLHSConst(graph, node);
        } else {
          return x;
        }
      }

      // RHS is consatnt
      if (is_rhs_const(node)) {
        CHECK(!is_lhs_const(node));
        return SimplifyBinaryRHSConstant(graph, node);
      }

      // General rewriting cases
      return SimplifyBinaryGeneral(graph, node);
  }

  return NoChange();
}

Fold::Result Fold::On(Graph* g, Node* n) {
  if (n->IsBookkeep()) return NoChange();

  if (auto x = TryConstantFold(g, n); !x.IsNoChange()) {
    return x;
  }
  if (auto x = TrySimplifyBinary(g, n); !x.IsNoChange()) {
    return x;
  }

  return NoChange();
}

void Fold::Finalize() {
  for (auto x : pass_cleanup_) {
    x->TryRemove();
  }
  pass_cleanup_.clear();
}

}  // namespace ir
}  // namespace quake

