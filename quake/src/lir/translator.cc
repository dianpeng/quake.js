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
      loop_phi_(),
      current_(nullptr),
      prev_(nullptr) {}

void Translator::RunOne(Node* node) {
  CHECK(!node->IsDead());
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
  MaterializeLoopPhiList();

  // Finalize the assembler and mark them into the correct position
  as_->Finalize();
}

void Translator::TransSnapshot(Node* node, Checkpoint* snap) {
  std::size_t none_null_or_undefined_size = 0;

  auto is_null_or_undefined = [](SSAVar* ssa_var) -> bool {
    switch (ssa_var->node()->opcode()) {
      case Opcode::kKNull:
      case Opcode::kKUndef:
      case Opcode::kKUninit:
        return true;
      default:
        return false;
    }
  };

  snap->ForeachEntry([&](const auto& e) {
    if (auto ssa_var = e.value(); is_null_or_undefined(ssa_var)) {
      return;
    } else {
      ++none_null_or_undefined_size;
    }
  });

  // do nothing, but just emit a Snapshot dummy LIR at the graph to ensure its
  // use-ref/ref-use relationship is fine during the LIR transformation pass
  auto n = __ Snapshot(none_null_or_undefined_size);

  int idx = 0;
  snap->ForeachEntry([&](const auto& e) {
    if (auto ssa_var = e.value(); !is_null_or_undefined(ssa_var)) {
      auto lir_operand = TransOperand(ssa_var);
      CHECK(lir_operand);
      n->OprSet(idx, lir_operand);
      idx++;
    }
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
// The first ecnounter of the PHI node will just put them into the phi list
// and when the whole graph is been translated, we revisit PHI node and try
// to materialize its operands
void Translator::MarkPendingLoopPhi(Node* phi) {
  CHECK(phi->IsPhi());
  CHECK(phi->size() == 2);

  switch (phi->opcode()) {
    case Opcode::kPhiLoop: {
      HOperand* lir = nullptr;

      // mapping HOpcode::Phi to LIR::Phi. Notes LIR's PHI is typped
      if (phi->type().IsInt() || phi->type().IsBool()) {
        lir = __ PhiLoopI();
      } else if (phi->type().IsReal()) {
        lir = __ PhiLoopF();
      } else if (phi->type().IsObject()) {
        lir = __ PhiLoopObj();
      } else {
        UNREACHABLE() << "PHI can only work with type int/float";
      }

      Output(phi, lir);
      loop_phi_.push_back(phi->out());
    } break;
    default:
      UNREACHABLE();
      break;
  }
}

void Translator::MaterializeLoopPhiList() {
  for (auto phi : loop_phi_) {
    auto node = phi->node();
    auto lir = phi->lir_operand();
    CHECK(node->size() == 2);

    auto instr = lir->instruction();
    CHECK(instr);
    instr->OprSet(0, TransOperand(node->At(0)));
    instr->OprSet(1, TransOperand(node->At(1)));

    CHECK(instr->VerifyOperand());
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
  CHECK(node && !node->IsDead())
      << node->opcode_name() << " #" << node->node_id();

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
  CHECK(itr != mapping_.end()) << node->Print();
  return itr->second;
}

HOperand* Translator::TransOperandImmediate(SSAVar* ssa_var) {
  auto node = ssa_var->node();
  HOperand* lir_operand = nullptr;

  CHECK(node);
  CHECK(node->IsConst());

  switch (node->opcode()) {
    case Opcode::kKI32:
      lir_operand = __ NewImmediate(node->Int32());
      break;
    case Opcode::kKI64:
      lir_operand = __ NewImmediate(node->Int64());
      break;
    case Opcode::kKFloat64:
      lir_operand = __ NewImmediate(node->Float64());
      break;
    case Opcode::kKBool:
      lir_operand = __ NewImmediate(node->Bool() ? 1 : 0);
      break;

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

  ssa_var->set_lir_operand(lir_operand);  // make sure every SSAVar has its
                                          // lowered lir operand, notes for
                                          // constant value, we do not have
                                          // definition yet, so constant
                                          // SSAVar may not have its LIROperand
                                          // which will break later phase

  return lir_operand;
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
  __ RcDecMaybeDelete(operand,
                      __ NewImmediate(T2JValTagChecked(ir_opr->type().tag())));
}

void Translator::DoRcTryOp(Node* node, bool inc) {
  CHECK(inc ? node->opcode() == Opcode::kRcTryInc
            : node->opcode() == Opcode::kRcTryDec);

  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  CHECK(ir_opr->type().IsBox());

  auto operand = TransOperand(ir_opr);

  // get a jump label
  auto label = __ NewLabel();

  // testify whether the box is heap or not
  auto cond = __ BoxIsHeap(operand);

  // jump out of the heap rc operation
  __ BranchTrue(cond, label);

  auto ptr = __ UnboxPtr(operand);

  if (inc) {
    __ RcInc(ptr);
  } else {
    auto ttag = __ GetTTag(operand);
    __ RcDecMaybeDelete(ptr, ttag);
  }

  as_->Bind(label);
}

void Translator::DoRcTryInc(Node* node) { DoRcTryOp(node, true); }

void Translator::DoRcTryDec(Node* node) { DoRcTryOp(node, false); }

// ----------------------------------------------------------------------------
// Dup
void Translator::Trans_kDup(Node* node) {
  auto value = TransOperand(node->At(0));
  switch (node->type().tag()) {
    case Type::kInt:
    case Type::kBool:
      Output(node, __ DupI(value));
      break;

    case Type::kFloat64:
      Output(node, __ DupF(value));
      break;

    case Type::kObject:
      Output(node, __ DupObj(value));
      break;

    default:
      UNREACHABLE();
      break;
  }
}

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

    case ir::Type::kObject:
    case ir::Type::kModule:
      // pointer type, which is opaque mostly towords us until it is been used
      // in certain way
      ret = __ GUnboxPtr(node->exit(), TransOperand(node->At(0)));
      break;

    default:
      UNREACHABLE() << "unknown type";
      break;
  }

  Output(node, ret);
}

void Translator::Trans_kUnboxPtr(Node* node) {
  CHECK(node->type().IsHeap());
  Output(node, __ UnboxPtr(TransOperand(node->At(0))));
}

// box the value specified inside of ir::BxToTemp back to the temporary slot
// inside of the context
void Translator::Trans_kBxToTemp(Node* node) {
  CHECK(node->size() == 2);

  // Convert the ir::BxToTemp to lir::BoxToTemp, which takes an imm as type
  // tag in QJS/JVal space, the second argument is the unboxed value, and
  // the third one is an imm indicate the stack slot index
  auto index = __ NewImmediate(node->At(0)->node()->Int32());
  auto ssa_var = node->At(1);  // the value needs to be boxed

  if (ssa_var->type().IsBox()) {
    // if we try to box an already boxed value to temp, then this will be
    // specialized StoreTemp lir opcode, ie no need to perform box operation
    __ StoreTemp(TransOperand(ssa_var), index);
  } else {
    // okay, this slot must be a already well typed slot, then just perform
    // extra box operation here.
    auto maybe_type = T2JValTag(ssa_var->type().tag());
    CHECK(maybe_type);

    auto jval_type = __ NewImmediate(*maybe_type);

    auto value = TransOperand(ssa_var);  // translate it into LIR value

    __ BoxToTemp(value, jval_type, index);
  }
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
  __ DeoptIfFalse(node->exit(), result);
}

void Translator::Trans_kGIsZero(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsZero);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  auto opr = TransOperand(node->At(0));
  auto cond = __ IEq(opr, __ NewInt32Zero());
  __ DeoptIfFalse(node->exit(), cond);
}

void Translator::Trans_kGIsNotZero(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsNotZero);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  auto opr = TransOperand(node->At(0));
  auto cond = __ INe(opr, __ NewInt32Zero());
  __ DeoptIfFalse(node->exit(), cond);
}

void Translator::Trans_kGIsTrue(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsTrue);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ DeoptIfTrue(node->exit(), TransOperand(node->At(0)));
}

void Translator::Trans_kGIsFalse(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsFalse);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ DeoptIfFalse(node->exit(), TransOperand(node->At(0)));
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

void Translator::Trans_kGIsNotException(Node* node) {
  CHECK(node->opcode() == Opcode::kGIsNotException);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->HasExit());
  CHECK(node->exit()->reason() == ir::Checkpoint::kException);

  auto expect_tag =
      __ NewImmediate(static_cast<std::int32_t>(JVal::kException));

  auto box_tag = __ GetTTag(TransOperand(node->At(0)));

  // check whether 2 types are identical or not
  auto result = __ IEq(box_tag, expect_tag);

  // if equal, then just perform deoptimization, ie fail from exception
  __ DeoptIfTrue(node->exit(), result);
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
  CHECK(node->size() == 1);
  Output(node, __ Inv(TransOperand(node->At(0))));
}

void Translator::Trans_kInvBool(Node* node) {
  CHECK(node->opcode() == Opcode::kInvBool);
  CHECK(node->size() == 1);

  // get the operand
  auto opr = TransOperand(node->At(0));

  // compare it against 0, if it is 0 and get its boolean output
  auto out = __ INe(opr, __ NewInt32Zero());

  // flip the output/bit and output
  Output(node, __ Inv(out));
}

// --------------------------------------------------------------------------
// ToBoolean
void Translator::Trans_kI2B(Node* node) {
  CHECK(node->opcode() == Opcode::kI2B);
  CHECK(node->size() == 1);
  Output(node, __ INe(TransOperand(node->At(0)), __ NewInt32Zero()));
}

void Translator::Trans_kF2B(Node* node) {
  CHECK(node->opcode() == Opcode::kF2B);
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

// --------------------------------------------------------------------------
//
// JSCall
//
// 1. JSCallF   => free call, ie this_object, target_object is undefined
// 2. JSCallFT  => call method, ie target_object set to undefined directly
// 3. JSCallFTN => call anything, everything is set up properly
//
void Translator::Trans_kJSCall(Node* node) {
  auto box_lir = [&](SSAVar* v) -> HOperand* {
    CHECK(v->type().IsUnbox());
    auto lir_v = TransOperand(v);
    return __ Box(lir_v, T2JValTagChecked(v->type().tag()));
  };

  CHECK(node->size() == 5);

  auto func_object = node->At(0);  // function object, unboxed
  auto this_object = node->At(1);  // this object, unboxed
  auto ntar_object = node->At(2);  // new target object, unboxed
  auto func_argc = node->At(3);    // function argument size, imm
  auto func_aoff = node->At(4);    // function argument offset, imm

  CHECK(func_object->type().IsUnbox());

  auto argc = TransOperandImmediate(func_argc);
  auto box_func = box_lir(func_object);
  auto arg_start = __ LeaTemp(TransOperandImmediate(func_aoff));

  if (this_object->node()->IsUndefined()) {
    if (ntar_object->node()->IsUndefined()) {
      Output(node, __ JSCallF(box_func, argc, arg_start));
    } else {
      CHECK(ntar_object->type().IsUnbox());
      auto box_ntar = box_lir(ntar_object);
      Output(node, __ JSCallFN(box_func, box_ntar, argc, arg_start));
    }
  } else {
    CHECK(this_object->type().IsUnbox());

    if (ntar_object->node()->IsUndefined()) {
      auto box_this = box_lir(this_object);
      Output(node, __ JSCallFT(box_func, box_this, argc, arg_start));
    } else {
      auto box_this = box_lir(this_object);
      auto box_ntar = box_lir(ntar_object);

      Output(node, __ JSCallFTN(box_func, box_this, box_ntar, argc, arg_start));
    }
  }
}

void Translator::Trans_kJSCCall(Node* node) {
  auto box_lir = [&](SSAVar* v) -> HOperand* {
    CHECK(v->type().IsUnbox());
    auto lir_v = TransOperand(v);
    return __ Box(lir_v, T2JValTagChecked(v->type().tag()));
  };

  CHECK(node->size() == 4);

  auto func_object = node->At(0);
  auto this_object = node->At(1);
  auto func_argc = node->At(2);
  auto func_aoff = node->At(3);

  CHECK(func_object->type().IsUnbox());
  CHECK(this_object->type().IsUnbox());

  auto func = box_lir(func_object);
  auto t = box_lir(this_object);
  auto argc = TransOperandImmediate(func_argc);
  auto arg_start = __ LeaTemp(TransOperandImmediate(func_aoff));

  Output(node, __ JSCallCtor(func, t, argc, arg_start));
}

void Translator::Trans_kArrayFrom(Node* node) {
  CHECK(node->size() == 2);

  auto func_argc = node->At(0);  // function argument size, imm
  auto func_aoff = node->At(1);  // function argument offset, imm

  auto arg_start = __ LeaTemp(TransOperandImmediate(func_aoff));
  auto argc = TransOperandImmediate(func_argc);

  Output(node, __ ArrayFrom(argc, arg_start));
}

void Translator::Trans_kApply(Node* node) {
  CHECK(node->size() == 2);

  auto func_tobj = node->At(0);   // function's this object
  auto func_aoff = node->At(1);   // function argument offset, imm
  auto func_magic = node->At(2);  // function argument option(magic)

  auto arg_start = __ LeaTemp(TransOperandImmediate(func_aoff));
  auto tobj = TransOperand(func_tobj);
  auto magic = TransOperandImmediate(func_magic);

  Output(node, __ Apply(tobj, arg_start, magic));
}

// ----------------------------------------------------------------------------
// Object Property
void Translator::Trans_kLoadPrimitiveProto(Node* node) {
  auto ttag = node->At(0)->node()->Int32();

  HOperand* result = nullptr;

  // JVal's type tag, based on this we lower the IR into typped LIR
  switch (ttag) {
    case JVal::kInt:
    case JVal::kFloat64:
      result = __ LoadNumberProto();
      break;

    case JVal::kBool:
      result = __ LoadBoolProto();
      break;

    case JVal::kString:
      result = __ LoadStringProto();
      break;

    case JVal::kSymbol:
      result = __ LoadSymbolProto();
      break;

    default:
      UNREACHABLE();
      break;
  }
  CHECK(result);
  Output(node, result);
}

// --------------------------------------------------------------------------
// Currently, the slow path will need to call out, ie via runtime call to
// resolve the property, so we have to make the slow path code explicit to
// LIR pass in order to let the RA take care of the ABI. To do this, we let
// the fast path generate a uninitialized value if it cannot find the value,
// which is internally used by QuickJS and never goes out of internal usage.
// And we emit code to check whether the fast path's result is a uninitialized
// or not, if so we go out to slow path, otherwise we continue. Notes, this
// require LIR's RA to support branch phi, since it will have to introduce
// following branch control flow structure:
//
// --------------------------------------------------------------------------
//
// v1 := fast_path();
//
// if v1 == uninitialized then
//   v2 := slow_path();
// endif
//
// v3 := phi(v1, v2); // phi is a branch phi
//
// --------------------------------------------------------------------------
// To resolve this phi in our RA is kind of trivial, just do nothing but
// at the end of the materialization, we insert one/two move into the end
// of the PHI's dependent control flow block, that's it.
// --------------------------------------------------------------------------
void Translator::Trans_kObjectFieldGet(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = TransOperandImmediate(node->At(1));
  auto hash = TransOperandImmediate(node->At(2));

  auto phi = __ FastSlow(
      [&](HAssembler*, HOperand* done) {
        auto fast_value = __ ObjectFieldGetAtomFast(target, atom, hash);
        auto box_tag = __ GetTTag(fast_value);  // get type tag
        auto exp_tag = __ NewImmediate(JVal::kUninitialized);
        auto cond = __ INe(box_tag, exp_tag);  // check uinit
        __ BranchTrue(cond, done);  // If not uninit, jump to end of the PHI

        return fast_value;
      },
      HOpcode::kRCallBox,                    // box call
      BuiltinFunction::kObjectFieldGetAtom,  // slow path function type
      target, atom);

  CHECK(phi->IsBranchPhi());
  Output(node, phi);
}

HOperand* Translator::MaybeBoxOperand(HOperand* opr, const Type& t) {
  switch (t.tag()) {
    case Type::kBox:
      return opr;

    case Type::kInt:
    case Type::kFloat64:
    case Type::kBool:
    case Type::kString:
    case Type::kSymbol:
    case Type::kObject:
    case Type::kModule:
    case Type::kJSFunction:
      return __ Box(opr, T2JValTagChecked(t.tag()));

    default:
      UNREACHABLE();
      return nullptr;
  }
}

void Translator::Trans_kObjectFieldPut(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = TransOperandImmediate(node->At(1));
  auto value = MaybeBoxOperand(TransOperand(node->At(2)), node->At(2)->type());

  // generate a runtime call here,
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kObjectFieldPutAtom);

  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, atom);
  call->instruction_checked()->OprSet(3, value);

  Output(node, call);
}

void Translator::Trans_kObjectFieldDefine(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = TransOperandImmediate(node->At(1));
  auto value = MaybeBoxOperand(TransOperand(node->At(2)), node->At(2)->type());

  // generate a runtime call here,
  auto call_idx =
      __ NewBuiltinCallIndex(BuiltinFunction::kObjectFieldDefineAtom);

  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, atom);
  call->instruction_checked()->OprSet(3, value);

  Output(node, call);
}

void Translator::Trans_kObjectNameSet(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = TransOperandImmediate(node->At(1));

  // generate a runtime call here,
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kObjectFieldNameSet);

  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, atom);

  Output(node, call);
}

void Translator::Trans_kObjectNameSetComputed(Node* node) {
  auto target = TransOperand(node->At(0));
  auto name = MaybeBoxOperand(TransOperand(node->At(2)), node->At(2)->type());

  // generate a runtime call here,
  auto call_idx =
      __ NewBuiltinCallIndex(BuiltinFunction::kObjectFieldNameSetComputed);

  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, name);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// Global
void Translator::Trans_kGlobalGet(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto flag = TransOperandImmediate(node->At(1));

  // generate a runtime call here,
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalGet);

  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, flag);

  Output(node, call);
}

void Translator::Trans_kGlobalSet(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto value = MaybeBoxOperand(TransOperand(node->At(1)), node->At(1)->type());
  auto flag = TransOperandImmediate(node->At(2));

  // generate a runtime call here,
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalPut);

  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, value);
  call->instruction_checked()->OprSet(3, flag);

  Output(node, call);
}

void Translator::Trans_kGlobalCheck(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalCheck);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, atom);
  Output(node, call);
}

void Translator::Trans_kGlobalDefine(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto flag = TransOperandImmediate(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalDefine);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, flag);
  Output(node, call);
}

void Translator::Trans_kGlobalCheckDefine(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto flag = TransOperandImmediate(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalCheckDefine);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, flag);
  Output(node, call);
}

// ----------------------------------------------------------------------------
// Misc
void Translator::Trans_kJSCheckBrand(Node* node) {
  auto target = MaybeBoxOperand(TransOperand(node->At(0)), node->At(0)->type());
  auto value = MaybeBoxOperand(TransOperand(node->At(1)), node->At(1)->type());
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCheckBrand);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, value);

  Output(node, call);
}

void Translator::Trans_kJSAddBrand(Node* node) {
  auto target = MaybeBoxOperand(TransOperand(node->At(0)), node->At(0)->type());
  auto value = MaybeBoxOperand(TransOperand(node->At(1)), node->At(1)->type());
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kAddBrand);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, value);
  Output(node, call);
}

void Translator::Trans_kJSRegexp(Node* node) {
  auto target = MaybeBoxOperand(TransOperand(node->At(0)), node->At(0)->type());
  auto value = MaybeBoxOperand(TransOperand(node->At(1)), node->At(1)->type());
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kRegexp);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, value);
  Output(node, call);
}

void Translator::Trans_kJSGetSuper(Node* node) {
  auto target = MaybeBoxOperand(TransOperand(node->At(0)), node->At(0)->type());
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGetSuper);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, target);
  Output(node, call);
}

void Translator::Trans_kJSImport(Node* node) {
  auto target = MaybeBoxOperand(TransOperand(node->At(0)), node->At(0)->type());
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kImport);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, target);
  Output(node, call);
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
  MarkPendingLoopPhi(node);
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
