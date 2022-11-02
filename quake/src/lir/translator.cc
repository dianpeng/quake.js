#include "lir/translator.h"

#include "ir/graph.h"
#include "ir/node.h"
#include "ir/ssa-var.h"
#include "ir/checkpoint.h"

#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"

#define __ as_->

namespace quake {
namespace lir {

using namespace ir;

Translator::Translator(Quake* q, HAssembler* as)
    : quake_(q),
      as_(as),
      mapping_(),
      block_(),
      phi_(),
      current_(nullptr),
      prev_(nullptr) {}

void Translator::RunOne(Node* node) {
  as_->BeginOpcode(node);
  current_ = node;

  // Pre node
  {
    if (auto snap = node->snapshot_before(); snap) {
      TransSnapshot(node, snap);
    }
  }

  // Node
  {
    switch (node->opcode()) {
#define XX(A, ...)   \
  case Opcode::A:    \
    Trans_##A(node); \
    break;
      QUAKE_OPCODE_LIST(XX)
#undef XX  // XX
      default:
        UNREACHABLE();
        break;
    }

    if (node->IsValue()) {
      CHECK(node->out()->lir_operand());
    }
  }

  // Post node
  {
    if (auto snap = node->snapshot_after(); snap) {
      TransSnapshot(node, snap);
    }
  }

  prev_ = node;
  as_->EndOpcode();
}

void Translator::Run(Graph* graph) {
  // Pass 1) Run through all the ir::Node in its control dependency order
  graph->Foreach([&](Node* n) { RunOne(n); });

  // Pass 2) Resolve all the pending phis
  MaterializePhiList();

  // Finalize the assembler and mark them into the correct position
  as_->Finalize();
}

void Translator::TransSnapshot(Node* node, Checkpoint* snap) {
  // do nothing, but just emit a Snapshot dummy LIR at the graph to ensure its
  // use-ref/ref-use relationship is fine during the LIR transformation pass
  auto n = __ Snapshot(snap->TopFrameValueSize());

  int idx = 0;
  snap->ForeachTopFrameValue([&](const auto& e) {
    auto lir_operand = TransOperand(e.value());
    CHECK(lir_operand);
    n->OprSet(idx, lir_operand);
    idx++;
  });
}

// ----------------------------------------------------------------------------
// Phi
//
// For each phi node, we should generate it after the second loop in LIR graph.
// And because in ir::Graph, phi's operand is not fully materialized, ie its
// second operand is defnied after the PHI due to the loop carry dependency.
// We should defer the PHI's operand materialization until very last.
//
// The first ecnounter of the PHI node will just put them into the phi_ list
// and when the whole graph is been translated, we revisit PHI node and try
// to materialize its operands
void Translator::MarkPendingPhi(Node* phi) {
  CHECK(phi->IsPhi());
  CHECK(phi->size() == 2);

  switch (phi->opcode()) {
    case Opcode::kPhiLoop: {
      HOperand* lir = nullptr;

      // mapping HOpcode::Phi to LIR::Phi. Notes LIR's PHI is typped
      if (phi->type().IsInt()) {
        lir = __ PhiLoopI();
      } else if (phi->type().IsReal()) {
        lir = __ PhiLoopF();
      } else {
        UNREACHABLE() << "PHI can only work with type int/float";
      }

      Output(phi, lir);
      phi_.push_back(phi->out());
    } break;
    default:
      UNREACHABLE();
      break;
  }
}

void Translator::MaterializePhiList() {
  for (auto phi : phi_) {
    auto node = phi->node();
    auto lir = phi->lir_operand();
    CHECK(node->size() == 2);

    auto instr = lir->instruction();
    CHECK(instr);
    instr->OprSet(0, TransOperand(node->At(0)));
    instr->OprSet(1, TransOperand(node->At(1)));
  }
}

// ----------------------------------------------------------------------------
// Output
void Translator::Output(Node* ir, HOperand* lir) {
  CHECK(ir);
  CHECK(lir);
  CHECK(ir == current_);

  CHECK(ir->IsValue());
  CHECK(ir->HasOut());

  auto out = ir->out();
  auto ret = mapping_.insert(std::make_pair(out, lir));

  // For each IR we should just meet them once in its def site. Iterating
  // ir::Graph means visiting each ir at where it is been defined. Therefore
  // it will never be the case that one ir will be mapped to 2 different lir
  // which is not possible at all
  CHECK(ret.second);

  // The specified lir must be temporary, ie waiting for RA
  CHECK(lir->IsTemporary());

  // the lir_operand field should not be set, since it is very first definition
  CHECK(!out->lir_operand());
  out->set_lir_operand(lir);
}

// ----------------------------------------------------------------------------
// Operand translation.
//
// The operand should be translated correctly before it is been used, ie
// when TransOperand is invoked, the input operand, SSAVar's corresponding
// value should already been generated, since it is a SSAVar, and we encounter
// its use always after its def. Notes, phi is an exception since the phi in
// HIR is been emitted right after BB1 instruction. But that's fine, since we
// will not generate it during our first encounter, but defer its operand's
// generation until very last.

HOperand* Translator::TransOperand(SSAVar* ssa_var) {
  auto node = ssa_var->node();
  CHECK(node);

  // If the node is an internal node, which means this node will never be
  // referenced outside of the scope of the node itself or it is a constant
  // which does not matter at all, then just emit it directly.
  if (node->IsInternal()) {
    if (node->IsConst()) {
      return TransOperandImmediate(ssa_var);
    } else {
      UNREACHABLE() << "trans operand does not handle internal node directly";
    }
  }

  auto itr = mapping_.find(ssa_var);
  CHECK(itr != mapping_.end());
  return itr->second;
}

HOperand* Translator::TransOperandImmediate(SSAVar* ssa_var) {
  auto node = ssa_var->node();
  CHECK(node);
  CHECK(node->IsConst());

  switch (node->opcode()) {
    case Opcode::kKI32:
      return __ NewImmediate(node->Int32());
    case Opcode::kKI64:
      return __ NewImmediate(node->Int64());
    case Opcode::kKFloat64:
      return __ NewImmediate(node->Float64());
    case Opcode::kKBool:
      return __ NewImmediate(node->Bool() ? 1 : 0);

    // Other type should not be used
    case Opcode::kKNull:
    case Opcode::kKUndef:
    case Opcode::kKUninit:
      UNREACHABLE() << "should already been resolved in HIR";
      break;

    case Opcode::kKNStr:
    case Opcode::kKStr:
      UNREACHABLE() << "not implemented";
      break;

    default:
      UNREACHABLE();
      break;
  }

  return nullptr;
}

// ----------------------------------------------------------------------------
// RefCount
//
// Ref count is been exposed out as RcOp operation. Which is a group of
// ref-count operations.

void Translator::Trans_kRcOp(Node* node) {
  const auto len = node->size();
  for (int i = 0; i < len; i++) {
    auto opr = node->At(i);
    CHECK(opr->IsInternal());
    auto opr_node = opr->node();
    switch (opr_node->opcode()) {
      case Opcode::kRcInc:
        DoRcInc(opr_node);
        break;

      case Opcode::kRcDec:
        DoRcDec(opr_node);
        break;

      case Opcode::kRcTryInc:
        DoRcTryInc(opr_node);
        break;

      case Opcode::kRcTryDec:
        DoRcTryDec(opr_node);
        break;

      default:
        UNREACHABLE();
        break;
    }
  }

  // Setup a barrier to disallow optimization to above this RcOp
  __ Barrier();
}

void Translator::DoRcInc(Node* node) {
  CHECK(node->opcode() == Opcode::kRcInc);
  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  auto operand = TransOperand(ir_opr);

  __ RcInc(operand);
}

void Translator::DoRcDec(Node* node) {
  CHECK(node->opcode() == Opcode::kRcDec);
  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  auto operand = TransOperand(ir_opr);
  __ RcDec(operand);
}

void Translator::DoRcTryOp(Node* node, bool inc) {
  CHECK(inc ? node->opcode() == Opcode::kRcTryInc
            : node->opcode() == Opcode::kRcTryDec);

  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  CHECK(ir_opr->type().IsBox()) << ir_opr->node()->Print();

  auto operand = TransOperand(ir_opr);

  // get a jump label
  auto label = __ NewLabel();

  // testify whether the box is heap or not
  auto cond = __ BoxIsHeap(operand);

  // jump out of the heap rc operation
  __ BranchTrue(cond, label);

  {
    // unbox the value into pointer
    auto unbox = __ UnboxPtr(operand);

    if (inc) {
      __ RcInc(unbox);
    } else {
      __ RcDec(unbox);
    }
  }

  as_->Bind(label);
}

void Translator::DoRcTryInc(Node* node) { DoRcTryOp(node, true); }

void Translator::DoRcTryDec(Node* node) { DoRcTryOp(node, false); }

// ----------------------------------------------------------------------------
// Box value
void Translator::Trans_kBxGuard(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->opcode() == Opcode::kBxGuard);  // ie unboxing a value

  auto maybe_t = T2JValTag(node->type().tag());
  CHECK(maybe_t) << "the type cannot be deduced: " << node->type().Print();
  HOperand* ret = nullptr;

  switch (node->type().tag()) {
    case ir::Type::kInt:
      ret = __ GUnboxI(node->exit(), TransOperand(node->At(0)));
      break;

    case ir::Type::kFloat64:
      ret = __ GUnboxF(node->exit(), TransOperand(node->At(0)));
      break;

    default:
      UNREACHABLE() << "unknown type";
      break;
  }

  Output(node, ret);
}

// ----------------------------------------------------------------------------
// Guard
void Translator::Trans_kGType(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGType);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->HasExit());

  auto maybe_t = T2JValTag(node->type().tag());
  CHECK(maybe_t) << "the type cannot be deduced: " << node->type().Print();

  auto expect_tag = __ NewImmediate(static_cast<std::int32_t>(*maybe_t));

  auto box_tag = __ GetTTag(TransOperand(node->At(0)));

  // check whether 2 types are identical or not
  auto result = __ IEq(box_tag, expect_tag);

  // if not equal, then just perform deoptimization
  __ DeoptIfTrue(node->exit(), result);
}

void Translator::Trans_kGIsTrue(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsTrue);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ DeoptIfFalse(node->exit(), TransOperand(node->At(0)));
}

void Translator::Trans_kGIsFalse(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsFalse);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ DeoptIfTrue(node->exit(), TransOperand(node->At(0)));
}

void Translator::Trans_kGIsIntPos(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsIntPos);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  // Get operand
  auto opr = TransOperand(node->At(0));

  // operand > 0
  auto result = __ IGt(opr, __ NewInt32Zero());

  __ DeoptIfFalse(node->exit(), result);

  // it is a value node
  Output(node, opr);
}

void Translator::Trans_kGIsIntPosOrZero(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsIntPosOrZero);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  // Get operand
  auto opr = TransOperand(node->At(0));

  // operand >= 0
  auto result = __ IGe(opr, __ NewInt32Zero());

  __ DeoptIfFalse(node->exit(), result);

  // it is a value node
  Output(node, opr);
}

void Translator::Trans_kGIsIntNeg(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsIntNeg);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  // Get operand
  auto opr = TransOperand(node->At(0));

  // operand < 0
  auto result = __ ILt(opr, __ NewInt32Zero());

  __ DeoptIfFalse(node->exit(), result);

  // it is a value node
  Output(node, opr);
}

void Translator::Trans_kGIsNotIntMax(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsNotIntMax);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  // Get operand
  auto opr = TransOperand(node->At(0));

  // operand == int32::max()
  auto result = __ IEq(opr, __ NewInt32Max());

  __ DeoptIfTrue(node->exit(), result);

  // it is a value node
  Output(node, opr);
}

void Translator::Trans_kGIsNotIntMin(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsNotIntMin);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  // Get operand
  auto opr = TransOperand(node->At(0));

  // operand == int32::min()
  auto result = __ IEq(opr, __ NewInt32Min());

  __ DeoptIfTrue(node->exit(), result);

  // it is a value node
  Output(node, opr);
}

// ----------------------------------------------------------------------------
// Local
void Translator::Trans_kLocGet(Node* node) {
  CHECK(node->opcode() == Opcode::kLocGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadLocal(TransOperandImmediate(node->At(0))));
}

void Translator::Trans_kLocSet(Node* node) { UNUSE(node); }

// ----------------------------------------------------------------------------
// Param
void Translator::Trans_kParamGet(Node* node) {
  CHECK(node->opcode() == Opcode::kParamGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadParam(TransOperandImmediate(node->At(0))));
}

void Translator::Trans_kParamSet(Node* node) { UNUSE(node); }

// ----------------------------------------------------------------------------
// Param
void Translator::Trans_kTempGet(Node* node) {
  CHECK(node->opcode() == Opcode::kTempGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadTemp(TransOperandImmediate(node->At(0))));
}

// ============================================================================
// Arithmetic

// ----------------------------------------------------------------------------
// Add
void Translator::Trans_kAddInt(Node* node) {
  CHECK(node->opcode() == Opcode::kAddInt);
  CHECK(node->size() == 2);

  Output(node, __ IAdd(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kGAddInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGAddInt);
  CHECK(node->size() == 2);
  Output(node, __ GIAdd(node->exit(), TransOperand(node->At(0)),
                        TransOperand(node->At(1))));
}

void Translator::Trans_kAddReal(Node* node) {
  CHECK(node->opcode() == Opcode::kAddReal);
  CHECK(node->size() == 2);

  Output(node, __ FAdd(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Sub
void Translator::Trans_kSubInt(Node* node) {
  CHECK(node->opcode() == Opcode::kSubInt);
  CHECK(node->size() == 2);

  Output(node, __ ISub(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kGSubInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGSubInt);
  CHECK(node->size() == 2);

  Output(node, __ GISub(node->exit(), TransOperand(node->At(0)),
                        TransOperand(node->At(1))));
}

void Translator::Trans_kSubReal(Node* node) {
  CHECK(node->opcode() == Opcode::kSubReal);
  CHECK(node->size() == 2);

  Output(node, __ FSub(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Mul
void Translator::Trans_kMulInt(Node* node) {
  CHECK(node->opcode() == Opcode::kMulInt);
  CHECK(node->size() == 2);

  Output(node, __ IMul(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kGMulInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGMulInt);
  CHECK(node->size() == 2);

  Output(node, __ GIMul(node->exit(), TransOperand(node->At(0)),
                        TransOperand(node->At(1))));
}

void Translator::Trans_kMulReal(Node* node) {
  CHECK(node->opcode() == Opcode::kMulReal);
  CHECK(node->size() == 2);

  Output(node, __ FMul(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Div
void Translator::Trans_kDivReal(Node* node) {
  CHECK(node->opcode() == Opcode::kDivReal);
  CHECK(node->size() == 2);

  Output(node, __ FDiv(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Mod
void Translator::Trans_kModInt(Node* node) {
  CHECK(node->opcode() == Opcode::kModInt);
  CHECK(node->size() == 2);

  Output(node, __ IMod(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kModReal(Node* node) {
  CHECK(node->opcode() == Opcode::kModReal);
  CHECK(node->size() == 2);

  Output(node, __ FMod(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Pow
void Translator::Trans_kPowReal(Node* node) {
  CHECK(node->opcode() == Opcode::kPowReal);
  CHECK(node->size() == 2);

  Output(node, __ FPow(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ============================================================================
// Bit Operaions

void Translator::Trans_kShlInt(Node* node) {
  CHECK(node->opcode() == Opcode::kShlInt);
  CHECK(node->size() == 2);

  auto op0 = TransOperand(node->At(0));
  auto shift_bits = TransOperand(node->At(1));

  if (shift_bits->IsImm()) {
    Output(node, __ ShlConst(op0, shift_bits));
  } else {
    Output(node, __ Shl(op0, shift_bits));
  }
}

void Translator::Trans_kShrInt(Node* node) {
  CHECK(node->opcode() == Opcode::kShrInt);
  CHECK(node->size() == 2);

  auto op0 = TransOperand(node->At(0));
  auto shift_bits = TransOperand(node->At(1));
  if (shift_bits->IsImm()) {
    Output(node, __ ShrConst(op0, shift_bits));
  } else {
    Output(node, __ Shr(op0, shift_bits));
  }
}

void Translator::Trans_kSarInt(Node* node) {
  CHECK(node->opcode() == Opcode::kSarInt);
  CHECK(node->size() == 2);

  auto op0 = TransOperand(node->At(0));
  auto shift_bits = TransOperand(node->At(1));
  if (shift_bits->IsImm()) {
    Output(node, __ SarConst(op0, shift_bits));
  } else {
    Output(node, __ Sar(op0, shift_bits));
  }
}

void Translator::Trans_kBitNorm(Node* node) {
  CHECK(node->opcode() == Opcode::kBitNorm);
  CHECK(node->size() == 1);

  // Bit-norm is basically just mask out the RHS with 0x1f. This use case is
  // for int32 bit shiftting, since we only got 32 bits, the rhs, ie the
  // # of bits to be shiftted, should be limited under to 32 bits
  Output(node, __ And(TransOperand(node->At(0)),
                      __ NewImmediate(static_cast<std::int32_t>(0x1f))));
}

void Translator::Trans_kAndInt(Node* node) {
  CHECK(node->opcode() == Opcode::kAndInt);
  CHECK(node->size() == 2);
  Output(node, __ And(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kOrInt(Node* node) {
  CHECK(node->opcode() == Opcode::kOrInt);
  CHECK(node->size() == 2);
  Output(node, __ Or(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kXorInt(Node* node) {
  CHECK(node->opcode() == Opcode::kXorInt);
  CHECK(node->size() == 2);
  Output(node, __ Xor(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Comparison
//
// notes Lt/Le/Gt/Ge/Eq/Ne are value operation, ie it does return something
// back, but for branching we just do not need value.
//
// The bytecode sequences:
//
//   OP_lt
//   OP_if_true
//
// will be translated into following HIR
//
//   #3: kLtInt #1, #2
//   #4: kBranchTrue #3 (#3 always return boolean)
//
// which will be directly translated into LIR
//
//   #3 kILt  #1, #2 (notes, this is a value operand)
//   #4 kCmpI #3, 1  (similar as above, notes boolean is been mapped into 1/0)
//   #5 kJE   @LBL
//
// ----------------------------------------------------------------------------
// The above sequence is definitly not performant, we just need to cmp the
// #1 and #2, then performs the jump. So effectively, the above HIR should
// be translated into following LIR preferrable:
//
//   kComp @a, @b
//   kJmpLt
//
// The pass to lower the branch into kBranchXXX is been performed inside of
// later LIR optimization passes.

void Translator::Trans_kLtInt(Node* node) {
  CHECK(node->opcode() == Opcode::kLtInt);
  CHECK(node->size() == 2);
  Output(node, __ ILt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kLeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kLeInt);
  CHECK(node->size() == 2);
  Output(node, __ ILe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kGtInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGtInt);
  CHECK(node->size() == 2);
  Output(node, __ IGt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kGeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGeInt);
  CHECK(node->size() == 2);
  Output(node, __ IGe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kEqInt(Node* node) {
  CHECK(node->opcode() == Opcode::kEqInt);
  CHECK(node->size() == 2);
  Output(node, __ IEq(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kNeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kNeInt);
  CHECK(node->size() == 2);
  Output(node, __ INe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kLtReal(Node* node) {
  CHECK(node->opcode() == Opcode::kLtReal);
  CHECK(node->size() == 2);
  Output(node, __ FLt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kLeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kLeReal);
  CHECK(node->size() == 2);
  Output(node, __ FLe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kGtReal(Node* node) {
  CHECK(node->opcode() == Opcode::kGtReal);
  CHECK(node->size() == 2);
  Output(node, __ FGt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kGeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kGeReal);
  CHECK(node->size() == 2);
  Output(node, __ FGe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kEqReal(Node* node) {
  CHECK(node->opcode() == Opcode::kEqReal);
  CHECK(node->size() == 2);
  Output(node, __ FEq(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kNeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kNeReal);
  CHECK(node->size() == 2);
  Output(node, __ FNe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void Translator::Trans_kIsType(Node* node) {
  CHECK(node->opcode() == Opcode::kIsType);
  CHECK(node->size() == 0);

  auto opr = node->At(0);
  CHECK(opr->type().IsBox());

  // get the box value's tagging
  auto box_tag = __ GetTTag(TransOperand(opr));

  // cmp the tagging
  auto maybe_t = T2JValTag(node->type().tag());
  CHECK(maybe_t) << "the type cannot be deduced: " << node->type().Print();

  auto expect_tag = __ NewImmediate(static_cast<std::int32_t>(*maybe_t));

  // do the eq comparison and save the result
  Output(node, __ IEq(box_tag, expect_tag));
}

void Translator::Trans_kIsTypeUndefinedOrNull(Node* node) {
  CHECK(node->opcode() == Opcode::kIsType);
  CHECK(node->size() == 0);

  auto opr = node->At(0);
  CHECK(opr->type().IsBox());

  // get the box value's tagging
  auto box_tag = __ GetTTag(TransOperand(opr));

  Output(node, __ IsNullOrUndefined(box_tag));
}

// ----------------------------------------------------------------------------
// Unary operation
void Translator::Trans_kNegInt(Node* node) {
  CHECK(node->opcode() == Opcode::kNegInt);
  CHECK(node->size() == 1);
  Output(node, __ Neg(TransOperand(node->At(0))));
}

void Translator::Trans_kGNegInt(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->opcode() == Opcode::kGNegInt);
  CHECK(node->size() == 1);
  Output(node, __ GNeg(node->exit(), TransOperand(node->At(0))));
}

void Translator::Trans_kNegReal(Node* node) {
  CHECK(node->opcode() == Opcode::kNegReal);
  CHECK(node->size() == 1);
  Output(node, __ NegF(TransOperand(node->At(0))));
}

// --------------------------------------------------------------------------
// Inc
void Translator::Trans_kGIncInt(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->opcode() == Opcode::kGIncInt);
  CHECK(node->size() == 1);
  Output(node, __ GInc(node->exit(), TransOperand(node->At(0))));
}

void Translator::Trans_kIncInt(Node* node) {
  CHECK(node->opcode() == Opcode::kIncInt);
  CHECK(node->size() == 1);
  Output(node, __ Inc(TransOperand(node->At(0))));
}

void Translator::Trans_kIncReal(Node* node) {
  CHECK(node->opcode() == Opcode::kIncReal);
  CHECK(node->size() == 1);
  Output(node, __ IncF(TransOperand(node->At(0))));
}

// --------------------------------------------------------------------------
// Dec
void Translator::Trans_kGDecInt(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->opcode() == Opcode::kGDecInt);
  CHECK(node->size() == 1);
  Output(node, __ GDec(node->exit(), TransOperand(node->At(0))));
}

void Translator::Trans_kDecInt(Node* node) {
  CHECK(node->opcode() == Opcode::kDecInt);
  CHECK(node->size() == 1);
  Output(node, __ Dec(TransOperand(node->At(0))));
}

void Translator::Trans_kDecReal(Node* node) {
  CHECK(node->opcode() == Opcode::kDecReal);
  CHECK(node->size() == 1);
  Output(node, __ DecF(TransOperand(node->At(0))));
}

// --------------------------------------------------------------------------
// Inversion(logic not)
void Translator::Trans_kInvInt(Node* node) {
  CHECK(node->opcode() == Opcode::kInvInt);
  CHECK(node->size() == 2);
  Output(node, __ Inv(TransOperand(node->At(0))));
}

void Translator::Trans_kInvBool(Node* node) {
  CHECK(node->opcode() == Opcode::kInvBool);
  CHECK(node->size() == 2);

  // get the operand
  auto opr = TransOperand(node->At(0));

  // compare it against 0, if it is 0 and get its boolean output
  auto out = __ INe(opr, __ NewInt32Zero());

  // flip the output/bit and output
  Output(node, __ Inv(out));
}

// --------------------------------------------------------------------------
// ToBoolean
void Translator::Trans_kToBoolInt(Node* node) {
  CHECK(node->opcode() == Opcode::kToBoolInt);
  CHECK(node->size() == 1);
  Output(node, __ INe(TransOperand(node->At(0)), __ NewInt32Zero()));
}

void Translator::Trans_kToBoolReal(Node* node) {
  CHECK(node->opcode() == Opcode::kToBoolReal);
  CHECK(node->size() == 1);
  Output(node, __ FNe(TransOperand(node->At(0)), __ NewFloat64Zero()));
}

// --------------------------------------------------------------------------
// Casting
void Translator::Trans_kI2F(Node* node) {
  CHECK(node->opcode() == Opcode::kI2F);
  CHECK(node->size() == 1);

  Output(node, __ CastI32ToF64(TransOperand(node->At(0))));
}

void Translator::Trans_kF2Bit(Node* node) {
  CHECK(node->opcode() == Opcode::kF2Bit);
  CHECK(node->size() == 1);

  Output(node, __ CastF64ToB32(TransOperand(node->At(0))));
}

// ----------------------------------------------------------------------------
// ControlFlow
void Translator::Trans_kJump(Node* node) {
  auto bb1_label = __ BB1Label();
  __ Jmp(bb1_label);
}

void Translator::Trans_kStart(Node* node) { __ Start(); }

void Translator::Trans_kEnd(Node* node) { __ End(); }

void Translator::Trans_kBB1(Node* node) { __ BB1(); }

// ----------------------------------------------------------------------------
// Phi
void Translator::Trans_kPhiLoop(Node* node) {
  CHECK(node->opcode() == Opcode::kPhiLoop);
  CHECK(node->size() == 2);
  MarkPendingPhi(node);
}

// ----------------------------------------------------------------------------
// NeverReached
#define UNREACH_HANDLER(XXX) \
  void Translator::Trans_##XXX(Node*) { UNREACHABLE() << #XXX; }

UNREACH_HANDLER(kKI32)
UNREACH_HANDLER(kKI64)
UNREACH_HANDLER(kKFloat64)
UNREACH_HANDLER(kKBool)
UNREACH_HANDLER(kKNull)
UNREACH_HANDLER(kKUndef)
UNREACH_HANDLER(kKNStr)
UNREACH_HANDLER(kKStr)
UNREACH_HANDLER(kKUninit)
UNREACH_HANDLER(kRcInc)
UNREACH_HANDLER(kRcDec)
UNREACH_HANDLER(kRcTryInc)
UNREACH_HANDLER(kRcTryDec)

}  // namespace lir
}  // namespace quake
