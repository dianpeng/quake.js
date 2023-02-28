#include "lir/lir-lower.h"

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

LIRLower::LIRLower(Quake* q, HAssembler* as)
    : quake_(q),
      as_(as),
      mapping_(),
      block_(),
      loop_phi_(),
      current_(nullptr),
      prev_(nullptr) {}

void LIRLower::RunOne(Node* node) {
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
    Lower_##A(node); \
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

void LIRLower::Run(Graph* graph) {
  // Pass 1) Run through all the ir::Node in its control dependency order
  graph->Foreach([&](Node* n) { RunOne(n); });

  // Pass 2) Resolve all the pending phis
  MaterializeLoopPhiList();

  // Finalize the assembler and mark them into the correct position
  as_->Finalize();
}

void LIRLower::TransSnapshot(Node* node, Checkpoint* snap) {
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
void LIRLower::MarkPendingLoopPhi(Node* phi) {
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
      } else if (phi->type().IsObject() || phi->type().IsString()) {
        lir = __ PhiLoopPtr();
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

void LIRLower::MaterializeLoopPhiList() {
  for (auto phi : loop_phi_) {
    auto node = phi->node();
    auto lir = phi->lir_operand();
    CHECK(node->size() == 2);

    auto instr = lir->instruction_checked();
    instr->OprSet(0, TransOperand(node->At(0)));
    instr->OprSet(1, TransOperand(node->At(1)));

    CHECK(instr->VerifyOperand());
  }
}

// ----------------------------------------------------------------------------
// Output
void LIRLower::Output(Node* ir, HOperand* lir) {
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
  if (!lir->instruction_checked()->IsDummy()) CHECK(lir->IsTemporary());

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

HOperand* LIRLower::TransOperand(SSAVar* ssa_var) {
  auto node = ssa_var->node();
  CHECK(node && !node->IsDead());

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
  CHECK(itr != mapping_.end()) << ssa_var->node_checked()->Print();
  return itr->second;
}

HOperand* LIRLower::TransOperandImmediate(SSAVar* ssa_var) {
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
      lir_operand = __ NewNull();
      break;

    case Opcode::kKUndef:
      lir_operand = __ NewUndefined();
      break;

    case Opcode::kKUninit:
      lir_operand = __ NewUninitialized();
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

void LIRLower::Lower_kRcOp(Node* node) {
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

    __ BlockCurrent();
  }

  // Setup a barrier to disallow optimization to above this RcOp
  __ Barrier();
}

void LIRLower::DoRcInc(Node* node) {
  CHECK(node->opcode() == Opcode::kRcInc);
  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  auto operand = TransOperand(ir_opr);

  __ RcInc(operand);
}

void LIRLower::DoRcDec(Node* node) {
  CHECK(node->opcode() == Opcode::kRcDec);
  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  auto operand = TransOperand(ir_opr);
  __ RcDecMaybeDelete(operand,
                      __ NewImmediate(T2JValTagChecked(ir_opr->type().tag())));
}

void LIRLower::DoRcTryOp(Node* node, bool inc) {
  CHECK(inc ? node->opcode() == Opcode::kRcTryInc
            : node->opcode() == Opcode::kRcTryDec);

  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  CHECK(ir_opr->type().IsBox());

  auto operand = TransOperand(ir_opr);

  if (inc) {
    __ RcIncBox(operand);
  } else {
    __ RcDecMaybeDeleteBox(operand);
  }
}

void LIRLower::DoRcTryInc(Node* node) { DoRcTryOp(node, true); }

void LIRLower::DoRcTryDec(Node* node) { DoRcTryOp(node, false); }

// ----------------------------------------------------------------------------
// Dup
void LIRLower::Lower_kDup(Node* node) {
  auto value = TransOperand(node->At(0));
  switch (node->type().tag()) {
    case Type::kInt:
    case Type::kBool:
      Output(node, __ DupI(value));
      break;

    case Type::kFloat64:
      Output(node, __ DupF(value));
      break;

    default:
      if (node->type().IsHeap()) {
        Output(node, __ DupPtr(value));
      } else {
        UNREACHABLE() << node->type().Print();
      }
      break;
  }
}

// ----------------------------------------------------------------------------
// Atom <-> Box
HOperand* LIRLower::DoBoxToAtom(Node* node) {
  CHECK(node->At(0)->type().IsBox());

  auto opr = TransOperand(node->At(0));

  auto phi = __ FastSlow(
      [&](HAssembler*, HOperand* fallback) {
        auto is_int = __ IsBoxAtomInt(opr);
        __ BranchFalse(is_int, fallback);  // if not valid atom int, fallback
        auto value = __ UnboxI(opr);       // unbox the value into integer
        return __ AtomFromI(value);        // convert integer to atom int
      },
      HOpcode::kRCallBox,             // box call
      BuiltinFunction::kValueToAtom,  // slow path function type
      opr);

  CHECK(phi->IsBranchPhi());
  return phi;
}

void LIRLower::Lower_kBoxToAtom(Node* node) { Output(node, DoBoxToAtom(node)); }

void LIRLower::Lower_kGBoxToAtom(Node* node) {
  CHECK(node->HasExit());

  auto phi = DoBoxToAtom(node);
  Output(node, __ GAtom(node->exit(), phi));
}

// Directly translate to runtime call, this opcode is used really rare
void LIRLower::Lower_kAtomToBox(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kAtomToValue);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, atom);
  Output(node, call);
}

// ----------------------------------------------------------------------------
// Box value
void LIRLower::Lower_kBxGuard(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->opcode() == Opcode::kBxGuard);  // ie unboxing a value
  HOperand* ret = nullptr;

  switch (node->type().tag()) {
    case ir::Type::kInt:
      ret = __ GUnboxI(node->exit(), TransOperand(node->At(0)));
      break;

    case ir::Type::kBool:
      ret = __ GUnboxB(node->exit(), TransOperand(node->At(0)));
      break;

    case ir::Type::kFloat64:
      ret = __ GUnboxF(node->exit(), TransOperand(node->At(0)));
      break;

    case ir::Type::kString:
    case ir::Type::kObject:
    case ir::Type::kModule:
      // pointer type, which is opaque mostly towords us until it is been used
      // in certain way
      ret = __ GUnboxPtr(node->exit(), TransOperand(node->At(0)));
      break;

    case ir::Type::kUndefined:
    case ir::Type::kNull: {
      auto t = T2JValTagChecked(node->type().tag());

      // For Null, Undefined, Unintialized, the operation will become guard
      // check, instead of unboxing value. Since all the reference for this
      // node should already been specialized, ie will not require the value
      // itself.
      auto cond = __ BoxTagIsEq(TransOperand(node->At(0)), __ NewImmediate(t));
      __ DeoptIfFalse(node->exit(), cond);

      ret = __ NewNull();
    } break;

    default:
      UNREACHABLE() << "unknown type: " << node->type().Print();
      break;
  }

  Output(node, ret);
}

void LIRLower::Lower_kUnboxPtr(Node* node) {
  CHECK(node->type().IsHeap());
  Output(node, __ UnboxPtr(TransOperand(node->At(0))));
}

void LIRLower::DoBxTo(
    Node* node, std::function<void(HOperand*, HOperand*)>&& store_op,
    std::function<void(HOperand*, HOperand*, HOperand*)>&& store_to_op) {
  CHECK(node->size() == 2);

  // Convert the ir::BxToTemp to lir::BoxToTemp, which takes an imm as type
  // tag in QJS/JVal space, the second argument is the unboxed value, and
  // the third one is an imm indicate the stack slot index
  auto index = __ NewImmediate(node->At(0)->node()->Int32());
  auto ssa_var = node->At(1);  // the value needs to be boxed

  if (ssa_var->type().IsBox()) {
    // if we try to box an already boxed value to temp, then this will be
    // specialized StoreTemp lir opcode, ie no need to perform box operation
    store_op(TransOperand(ssa_var), index);
  } else {
    // okay, this slot must be a already well typed slot, then just perform
    // extra box operation here.
    auto maybe_type = T2JValTag(ssa_var->type().tag());
    CHECK(maybe_type);
    auto jval_type = __ NewImmediate(*maybe_type);

    switch (*maybe_type) {
      case JVal::kUndefined:
      case JVal::kUninitialized:
      case JVal::kNull:
        store_to_op(__ NewImmediate(0), jval_type, index);
        break;

      default:
        auto value = TransOperand(ssa_var);  // translate it into LIR value
        store_to_op(value, jval_type, index);
        break;
    }
  }
}

void LIRLower::Lower_kBxToTemp(Node* node) {
  DoBxTo(
      node, [&](HOperand* a, HOperand* b) { __ StoreTemp(a, b); },
      [&](HOperand* a, HOperand* b, HOperand* c) { __ BoxToTemp(a, b, c); });
}

void LIRLower::Lower_kBxToLocal(Node* node) {
  DoBxTo(
      node, [&](HOperand* a, HOperand* b) { __ StoreLocal(a, b); },
      [&](HOperand* a, HOperand* b, HOperand* c) { __ BoxToLocal(a, b, c); });
}

void LIRLower::Lower_kBxToParam(Node* node) {
  DoBxTo(
      node, [&](HOperand* a, HOperand* b) { __ StoreParam(a, b); },
      [&](HOperand* a, HOperand* b, HOperand* c) { __ BoxToParam(a, b, c); });
}

// ----------------------------------------------------------------------------
// EnterJitTrace
void LIRLower::Lower_kEnterJitTrace(Node* node) {
  auto trace_id = TransOperandImmediate(node->At(0));
  auto tos = TransOperandImmediate(node->At(1));
  auto did1 = TransOperandImmediate(node->At(2));
  auto did2 = TransOperandImmediate(node->At(3));

  __ EnterJitTrace(trace_id, tos, did1, did2);
}

// ----------------------------------------------------------------------------
// Guard
void LIRLower::Lower_kDeopt(Node* node) {
  CHECK(node->HasExit());
  __ Deopt(node->exit());
}

void LIRLower::Lower_kReturn(Node* node) {
  __ Return(TransOperandBox(node->At(0)));
}

void LIRLower::Lower_kSnapshot(Node* node) { UNUSE(node); }

void LIRLower::Lower_kGIsNotZero(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsNotZero);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  auto opr = TransOperand(node->At(0));
  auto cond = __ INe(opr, __ NewInt32Zero());
  __ DeoptIfFalse(node->exit(), cond);

  Output(node, opr);
}

void LIRLower::Lower_kGIsTrue(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsTrue);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ DeoptIfFalse(node->exit(), TransOperand(node->At(0)));
}

void LIRLower::Lower_kGIsFalse(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGIsFalse);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ DeoptIfTrue(node->exit(), TransOperand(node->At(0)));
}

void LIRLower::Lower_kGIsIntPos(Node* node) {
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

void LIRLower::Lower_kGIsIntPosOrZero(Node* node) {
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

void LIRLower::Lower_kGIsIntNeg(Node* node) {
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

void LIRLower::Lower_kGIsNotIntMax(Node* node) {
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

void LIRLower::Lower_kGIsNotIntMin(Node* node) {
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

void LIRLower::Lower_kGIsNotException(Node* node) {
  CHECK(node->opcode() == Opcode::kGIsNotException);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->HasExit());
  CHECK(node->exit()->reason() == ir::Checkpoint::kException);

  auto expect_tag =
      __ NewImmediate(static_cast<std::int32_t>(JVal::kException));

  auto result = __ BoxTagIsEq(TransOperand(node->At(0)), expect_tag);

  // if equal, then just perform deoptimization, ie fail from exception
  __ DeoptIfTrue(node->exit(), result);
}

void LIRLower::Lower_kGObjectIsScriptFunction(Node* node) {
  CHECK(node->opcode() == Opcode::kGObjectIsScriptFunction);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ GObjectIsScriptFunction(node->exit(), TransOperand(node->At(0)));
}

void LIRLower::Lower_kGIsUninitialized(Node* node) {
  CHECK(node->opcode() == Opcode::kGIsUninitialized);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->HasExit());
  CHECK(node->exit()->reason() == ir::Checkpoint::kException);

  auto expect_tag =
      __ NewImmediate(static_cast<std::int32_t>(JVal::kUninitialized));

  auto result = __ BoxTagIsEq(TransOperand(node->At(0)), expect_tag);

  // if equal, then just perform deoptimization, ie fail from exception
  __ DeoptIfFalse(node->exit(), result);
}

void LIRLower::Lower_kGIsNotUninitialized(Node* node) {
  CHECK(node->opcode() == Opcode::kGIsNotUninitialized);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->HasExit());
  CHECK(node->exit()->reason() == ir::Checkpoint::kException);

  auto expect_tag =
      __ NewImmediate(static_cast<std::int32_t>(JVal::kUninitialized));

  auto result = __ BoxTagIsEq(TransOperand(node->At(0)), expect_tag);

  // if equal, then just perform deoptimization, ie fail from exception
  __ DeoptIfTrue(node->exit(), result);
}

void LIRLower::Lower_kLoadStr(Node* node) {
  CHECK(node->opcode() == Opcode::kLoadStr);
  Output(node, __ LoadStringUnbox(node->Str()));
}

// ----------------------------------------------------------------------------
// VarRef
void LIRLower::Lower_kVarRefGet(Node* node) {
  CHECK(node->opcode() == Opcode::kVarRefGet);
  CHECK(node->size() == 1);
  Output(node, __ LoadVarRef(TransOperandImmediate(node->At(0))));
}

void LIRLower::Lower_kVarRefSet(Node* node) {
  CHECK(node->opcode() == Opcode::kVarRefSet);
  CHECK(node->size() == 2);

  // the second operand *must* be in box shape
  __ StoreVarRef(TransOperandImmediate(node->At(0)),
                 TransOperandBox(node->At(1)));
}

// ----------------------------------------------------------------------------
// Local
void LIRLower::Lower_kLocGet(Node* node) {
  CHECK(node->opcode() == Opcode::kLocGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadLocal(TransOperandImmediate(node->At(0))));
}

void LIRLower::Lower_kLocSet(Node* node) { UNUSE(node); }
void LIRLower::Lower_kLocNullify(Node* node) { UNUSE(node); }

// ----------------------------------------------------------------------------
// Param
void LIRLower::Lower_kParamGet(Node* node) {
  CHECK(node->opcode() == Opcode::kParamGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadParam(TransOperandImmediate(node->At(0))));
}

void LIRLower::Lower_kParamSet(Node* node) { UNUSE(node); }
void LIRLower::Lower_kParamNullify(Node* node) { UNUSE(node); }

// ----------------------------------------------------------------------------
// Param
void LIRLower::Lower_kTempGet(Node* node) {
  CHECK(node->opcode() == Opcode::kTempGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadTemp(TransOperandImmediate(node->At(0))));
}

void LIRLower::Lower_kTempNullify(Node* node) { UNUSE(node); }
void LIRLower::Lower_kTempSet(Node* node) { UNUSE(node); }
void LIRLower::Lower_kTempPush(Node* node) { UNUSE(node); }
void LIRLower::Lower_kTempPop(Node* node) { UNUSE(node); }

// ============================================================================
// Arithmetic

// ----------------------------------------------------------------------------
// Add
void LIRLower::Lower_kAddInt(Node* node) {
  CHECK(node->opcode() == Opcode::kAddInt);
  CHECK(node->size() == 2);

  Output(node, __ IAdd(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kGAddInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGAddInt);
  CHECK(node->size() == 2);
  Output(node, __ GIAdd(node->exit(), TransOperand(node->At(0)),
                        TransOperand(node->At(1))));
}

void LIRLower::Lower_kAddReal(Node* node) {
  CHECK(node->opcode() == Opcode::kAddReal);
  CHECK(node->size() == 2);

  Output(node, __ FAdd(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Sub
void LIRLower::Lower_kSubInt(Node* node) {
  CHECK(node->opcode() == Opcode::kSubInt);
  CHECK(node->size() == 2);

  Output(node, __ ISub(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kGSubInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGSubInt);
  CHECK(node->size() == 2);

  Output(node, __ GISub(node->exit(), TransOperand(node->At(0)),
                        TransOperand(node->At(1))));
}

void LIRLower::Lower_kSubReal(Node* node) {
  CHECK(node->opcode() == Opcode::kSubReal);
  CHECK(node->size() == 2);

  Output(node, __ FSub(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Mul
void LIRLower::Lower_kMulInt(Node* node) {
  CHECK(node->opcode() == Opcode::kMulInt);
  CHECK(node->size() == 2);

  Output(node, __ IMul(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kGMulInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGMulInt);
  CHECK(node->size() == 2);

  Output(node, __ GIMul(node->exit(), TransOperand(node->At(0)),
                        TransOperand(node->At(1))));
}

void LIRLower::Lower_kMulReal(Node* node) {
  CHECK(node->opcode() == Opcode::kMulReal);
  CHECK(node->size() == 2);

  Output(node, __ FMul(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Div
void LIRLower::Lower_kDivReal(Node* node) {
  CHECK(node->opcode() == Opcode::kDivReal);
  CHECK(node->size() == 2);

  Output(node, __ FDiv(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Mod
void LIRLower::Lower_kModInt(Node* node) {
  CHECK(node->opcode() == Opcode::kModInt);
  CHECK(node->size() == 2);

  Output(node, __ IMod(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kModReal(Node* node) {
  CHECK(node->opcode() == Opcode::kModReal);
  CHECK(node->size() == 2);

  Output(node, __ FMod(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ----------------------------------------------------------------------------
// Pow
void LIRLower::Lower_kPowReal(Node* node) {
  CHECK(node->opcode() == Opcode::kPowReal);
  CHECK(node->size() == 2);

  Output(node, __ FPow(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

// ============================================================================
// Bit Operaions

void LIRLower::Lower_kShlInt(Node* node) {
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

void LIRLower::Lower_kGShrInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGShrInt);
  CHECK(node->size() == 2);

  auto op0 = TransOperand(node->At(0));
  auto shift_bits = TransOperand(node->At(1));
  if (shift_bits->IsImm()) {
    Output(node, __ GShrConst(node->exit(), op0, shift_bits));
  } else {
    Output(node, __ GShr(node->exit(), op0, shift_bits));
  }
}

void LIRLower::Lower_kSarInt(Node* node) {
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

void LIRLower::Lower_kBitNorm(Node* node) {
  CHECK(node->opcode() == Opcode::kBitNorm);
  CHECK(node->size() == 1);

  // Bit-norm is basically just mask out the RHS with 0x1f. This use case is
  // for int32 bit shiftting, since we only got 32 bits, the rhs, ie the
  // # of bits to be shiftted, should be limited under to 32 bits
  Output(node, __ And(TransOperand(node->At(0)),
                      __ NewImmediate(static_cast<std::int32_t>(0x1f))));
}

void LIRLower::Lower_kAndInt(Node* node) {
  CHECK(node->opcode() == Opcode::kAndInt);
  CHECK(node->size() == 2);
  Output(node, __ And(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kOrInt(Node* node) {
  CHECK(node->opcode() == Opcode::kOrInt);
  CHECK(node->size() == 2);
  Output(node, __ Or(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kXorInt(Node* node) {
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

void LIRLower::Lower_kLtInt(Node* node) {
  CHECK(node->opcode() == Opcode::kLtInt);
  CHECK(node->size() == 2);
  Output(node, __ ILt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kLeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kLeInt);
  CHECK(node->size() == 2);
  Output(node, __ ILe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kGtInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGtInt);
  CHECK(node->size() == 2);
  Output(node, __ IGt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kGeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGeInt);
  CHECK(node->size() == 2);
  Output(node, __ IGe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kEqInt(Node* node) {
  CHECK(node->opcode() == Opcode::kEqInt);
  CHECK(node->size() == 2);
  Output(node, __ IEq(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kNeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kNeInt);
  CHECK(node->size() == 2);
  Output(node, __ INe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kLtReal(Node* node) {
  CHECK(node->opcode() == Opcode::kLtReal);
  CHECK(node->size() == 2);
  Output(node, __ FLt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kLeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kLeReal);
  CHECK(node->size() == 2);
  Output(node, __ FLe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kGtReal(Node* node) {
  CHECK(node->opcode() == Opcode::kGtReal);
  CHECK(node->size() == 2);
  Output(node, __ FGt(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kGeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kGeReal);
  CHECK(node->size() == 2);
  Output(node, __ FGe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kEqReal(Node* node) {
  CHECK(node->opcode() == Opcode::kEqReal);
  CHECK(node->size() == 2);
  Output(node, __ FEq(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kNeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kNeReal);
  CHECK(node->size() == 2);
  Output(node, __ FNe(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kEqPtr(Node* node) {
  CHECK(node->opcode() == Opcode::kEqPtr);
  CHECK(node->size() == 2);

  Output(node, __ I64Eq(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kNePtr(Node* node) {
  CHECK(node->opcode() == Opcode::kNePtr);
  CHECK(node->size() == 2);

  Output(node, __ I64Ne(TransOperand(node->At(0)), TransOperand(node->At(1))));
}

void LIRLower::Lower_kIsType(Node* node) {
  CHECK(node->opcode() == Opcode::kIsType);
  CHECK(node->size() == 0);

  auto opr = node->At(0);
  CHECK(opr->type().IsBox());

  // cmp the tagging
  auto maybe_t = T2JValTag(node->type().tag());
  CHECK(maybe_t) << "the type cannot be deduced: " << node->type().Print();

  auto expect_tag = __ NewImmediate(static_cast<std::int32_t>(*maybe_t));

  auto result = __ BoxTagIsEq(TransOperand(opr), expect_tag);

  // do the eq comparison and save the result
  Output(node, result);
}

void LIRLower::Lower_kIsTypeUndefinedOrNull(Node* node) {
  CHECK(node->opcode() == Opcode::kIsType);
  CHECK(node->size() == 0);

  auto opr = node->At(0);
  CHECK(opr->type().IsBox());

  // get the box value's tagging
  auto box_tag = __ GetTTag(TransOperand(opr));
  Output(node, __ IsNullOrUndefined(box_tag));
}

// ----------------------------------------------------------------------------
// String comparison
void LIRLower::Lower_kLtStr(Node* node) {
  auto lhs = TransOperand(node->At(0));
  auto rhs = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrLt);
  auto call = __ RCallI32(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kLeStr(Node* node) {
  auto lhs = TransOperand(node->At(0));
  auto rhs = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrLe);
  auto call = __ RCallI32(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kGtStr(Node* node) {
  auto lhs = TransOperand(node->At(0));
  auto rhs = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrGt);
  auto call = __ RCallI32(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kGeStr(Node* node) {
  auto lhs = TransOperand(node->At(0));
  auto rhs = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrGe);
  auto call = __ RCallI32(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kEqStr(Node* node) {
  auto lhs = TransOperand(node->At(0));
  auto rhs = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrEq);
  auto call = __ RCallI32(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kNeStr(Node* node) {
  auto lhs = TransOperand(node->At(0));
  auto rhs = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrNe);
  auto call = __ RCallI32(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kNe(Node* node) {
  auto lhs = TransOperandBox(node->At(0));
  auto rhs = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCmpNe);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kEq(Node* node) {
  auto lhs = TransOperandBox(node->At(0));
  auto rhs = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCmpEq);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kLt(Node* node) {
  auto lhs = TransOperandBox(node->At(0));
  auto rhs = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCmpLt);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kLe(Node* node) {
  auto lhs = TransOperandBox(node->At(0));
  auto rhs = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCmpLe);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kGt(Node* node) {
  auto lhs = TransOperandBox(node->At(0));
  auto rhs = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCmpGt);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kGe(Node* node) {
  auto lhs = TransOperandBox(node->At(0));
  auto rhs = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCmpGe);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// Unary operation
void LIRLower::Lower_kNegInt(Node* node) {
  CHECK(node->opcode() == Opcode::kNegInt);
  CHECK(node->size() == 1);
  Output(node, __ Neg(TransOperand(node->At(0))));
}

void LIRLower::Lower_kGNegInt(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->opcode() == Opcode::kGNegInt);
  CHECK(node->size() == 1);
  Output(node, __ GNeg(node->exit(), TransOperand(node->At(0))));
}

void LIRLower::Lower_kNegReal(Node* node) {
  CHECK(node->opcode() == Opcode::kNegReal);
  CHECK(node->size() == 1);
  Output(node, __ NegF(TransOperand(node->At(0))));
}

void LIRLower::Lower_kTypeOf(Node* node) {
  CHECK(node->opcode() == Opcode::kTypeOf);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());

  auto value = TransOperand(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kTypeOf);
  auto call = __ RCallPtr(call_idx, 1);

  call->instruction_checked()->OprSet(1, value);
  Output(node, call);
}

void LIRLower::Lower_kDelete(Node* node) {
  CHECK(node->opcode() == Opcode::kDelete);
  CHECK(node->size() == 2);

  auto a0 = TransOperandBox(node->At(0));
  auto a1 = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kDelete);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);

  Output(node, call);
}

void LIRLower::Lower_kDeleteVar(Node* node) {
  CHECK(node->opcode() == Opcode::kDeleteVar);
  CHECK(node->size() == 1);

  auto a0 = TransOperandImmediate(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kDeleteVar);
  auto call = __ RCallBox(call_idx, 1);

  call->instruction_checked()->OprSet(1, a0);

  Output(node, call);
}

void LIRLower::Lower_kInstanceOf(Node* node) {
  CHECK(node->opcode() == Opcode::kInstanceOf);
  CHECK(node->size() == 2);

  auto a0 = TransOperandBox(node->At(0));
  auto a1 = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kInstanceOf);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);

  Output(node, call);
}

// --------------------------------------------------------------------------
// High level cast
void LIRLower::Lower_kToObject(Node* node) {
  CHECK(node->opcode() == Opcode::kToObject);
  CHECK(node->size() == 1);

  auto a0 = TransOperandBox(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kToObject);
  auto call = __ RCallI32(call_idx, 1);

  call->instruction_checked()->OprSet(1, a0);

  Output(node, call);
}

void LIRLower::Lower_kToPropKey(Node* node) {
  CHECK(node->opcode() == Opcode::kToPropKey);
  CHECK(node->size() == 1);

  auto a0 = TransOperandBox(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kToPropKey);
  auto call = __ RCallI32(call_idx, 1);

  call->instruction_checked()->OprSet(1, a0);

  Output(node, call);
}

// --------------------------------------------------------------------------
// Inc
void LIRLower::Lower_kGIncInt(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->opcode() == Opcode::kGIncInt);
  CHECK(node->size() == 1);
  Output(node, __ GInc(node->exit(), TransOperand(node->At(0))));
}

void LIRLower::Lower_kIncInt(Node* node) {
  CHECK(node->opcode() == Opcode::kIncInt);
  CHECK(node->size() == 1);
  Output(node, __ Inc(TransOperand(node->At(0))));
}

void LIRLower::Lower_kIncReal(Node* node) {
  CHECK(node->opcode() == Opcode::kIncReal);
  CHECK(node->size() == 1);
  Output(node, __ IncF(TransOperand(node->At(0))));
}

// --------------------------------------------------------------------------
// Dec
void LIRLower::Lower_kGDecInt(Node* node) {
  CHECK(node->HasExit());
  CHECK(node->opcode() == Opcode::kGDecInt);
  CHECK(node->size() == 1);
  Output(node, __ GDec(node->exit(), TransOperand(node->At(0))));
}

void LIRLower::Lower_kDecInt(Node* node) {
  CHECK(node->opcode() == Opcode::kDecInt);
  CHECK(node->size() == 1);
  Output(node, __ Dec(TransOperand(node->At(0))));
}

void LIRLower::Lower_kDecReal(Node* node) {
  CHECK(node->opcode() == Opcode::kDecReal);
  CHECK(node->size() == 1);
  Output(node, __ DecF(TransOperand(node->At(0))));
}

// --------------------------------------------------------------------------
// Inversion(logic not)
void LIRLower::Lower_kInvInt(Node* node) {
  CHECK(node->opcode() == Opcode::kInvInt);
  CHECK(node->size() == 1);
  Output(node, __ Inv(TransOperand(node->At(0))));
}

void LIRLower::Lower_kFlipBool(Node* node) {
  CHECK(node->opcode() == Opcode::kFlipBool);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBool());

  // get the operand, this must be boolean
  auto out = TransOperand(node->At(0));

  // flip the output/bit and output
  Output(node, __ CastNBToB(out));
}

// ----------------------------------------------------------------------------
// Str
void LIRLower::Lower_kStrCon(Node* node) {
  CHECK(node->opcode() == Opcode::kStrCon);
  CHECK(node->size() == 2);

  auto lhs = TransOperand(node->At(0));
  auto rhs = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrCon);
  auto call = __ RCallPtr(call_idx, 2);

  call->instruction_checked()->OprSet(1, lhs);
  call->instruction_checked()->OprSet(2, rhs);

  Output(node, call);
}

void LIRLower::Lower_kStrLen(Node* node) {
  Output(node, __ StrLen(TransOperand(node->At(0))));
}

void LIRLower::Lower_kStrFieldGet(Node* node) {
  auto a0 = TransOperand(node->At(0));  // do not box it, the function knows it
  auto a1 = TransOperand(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStrFieldGet);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);

  Output(node, call);
}

// --------------------------------------------------------------------------
// Casting
void LIRLower::Lower_kI2F(Node* node) {
  CHECK(node->opcode() == Opcode::kI2F);
  CHECK(node->size() == 1);

  Output(node, __ CastI32ToF64(TransOperand(node->At(0))));
}

void LIRLower::Lower_kF2Bit(Node* node) {
  CHECK(node->opcode() == Opcode::kF2Bit);
  CHECK(node->size() == 1);

  Output(node, __ CastF64ToB32(TransOperand(node->At(0))));
}

void LIRLower::Lower_kI2B(Node* node) {
  CHECK(node->opcode() == Opcode::kI2B);
  CHECK(node->size() == 1);
  Output(node, __ INe(TransOperand(node->At(0)), __ NewInt32Zero()));
}

void LIRLower::Lower_kF2B(Node* node) {
  CHECK(node->opcode() == Opcode::kF2B);
  CHECK(node->size() == 1);
  Output(node, __ FNe(TransOperand(node->At(0)), __ NewFloat64Zero()));
}

void LIRLower::Lower_kB2I(Node* node) {
  CHECK(node->opcode() == Opcode::kB2I);
  CHECK(node->size() == 1);

  // Do nothing, the boolean value will be unboxed(normalized) into 0/1 in the
  // backend, hence, the output of operand already becomes 0/1
  Output(node, TransOperand(node->At(0)));
}

void LIRLower::Lower_kB2F(Node* node) {
  CHECK(node->opcode() == Opcode::kB2F);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));

  // Since the input integer is already normalized, just do a float64
  // conversion
  Output(node, __ CastI32ToF64(opr));
}

void LIRLower::Lower_kV2B(Node* node) {
  CHECK(node->opcode() == Opcode::kV2B);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));
  Output(node, __ CastVToB(opr));
}

void LIRLower::Lower_kO2B(Node* node) {
  CHECK(node->opcode() == Opcode::kO2B);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));
  Output(node, __ CastObjToB(opr));
}

void LIRLower::Lower_kS2B(Node* node) {
  CHECK(node->opcode() == Opcode::kS2B);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));
  Output(node, __ CastStrToB(opr));
}

// to_string cast
void LIRLower::Lower_kB2S(Node* node) {
  CHECK(node->opcode() == Opcode::kB2S);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));
  Output(node, __ CastBToStr(opr));
}

void LIRLower::Lower_kI2S(Node* node) {
  CHECK(node->opcode() == Opcode::kI2S);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kInt32ToStr);
  auto call = __ RCallPtr(call_idx, 1);

  call->instruction_checked()->OprSet(1, opr);

  Output(node, call);
}

void LIRLower::Lower_kF2S(Node* node) {
  CHECK(node->opcode() == Opcode::kF2S);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kFloat64ToStr);
  auto call = __ RCallPtr(call_idx, 1);

  call->instruction_checked()->OprSet(1, opr);

  Output(node, call);
}

void LIRLower::Lower_kO2S(Node* node) {
  CHECK(node->opcode() == Opcode::kO2S);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kObjectToStr);
  auto call = __ RCallPtr(call_idx, 1);

  call->instruction_checked()->OprSet(1, opr);

  Output(node, call);
}

void LIRLower::Lower_kS2F(Node* node) {
  CHECK(node->opcode() == Opcode::kS2F);
  CHECK(node->size() == 1);

  auto opr = TransOperand(node->At(0));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kStringToFloat64);
  auto call = __ RCallF64(call_idx, 1);

  call->instruction_checked()->OprSet(1, opr);

  Output(node, call);
}

// --------------------------------------------------------------------------
//
// JSCall
//
// 1. JSCallF   => free call, ie this_object, target_object is undefined
// 2. JSCallFT  => call method, ie target_object set to undefined directly
// 3. JSCallFTN => call anything, everything is set up properly
//
void LIRLower::Lower_kJSCall(Node* node) {
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

void LIRLower::Lower_kJSCCall(Node* node) {
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

void LIRLower::Lower_kArrayFrom(Node* node) {
  CHECK(node->size() == 2);

  auto func_argc = node->At(0);  // function argument size, imm
  auto func_aoff = node->At(1);  // function argument offset, imm

  auto arg_start = __ LeaTemp(TransOperandImmediate(func_aoff));
  auto argc = TransOperandImmediate(func_argc);

  Output(node, __ ArrayFrom(argc, arg_start));
}

void LIRLower::Lower_kApply(Node* node) {
  auto box_lir = [&](SSAVar* v) -> HOperand* {
    CHECK(v->type().IsUnbox());
    auto lir_v = TransOperand(v);
    return __ Box(lir_v, T2JValTagChecked(v->type().tag()));
  };

  CHECK(node->size() == 3);

  auto func_tobj = node->At(0);   // function's this object
  auto func_aoff = node->At(1);   // function argument offset, imm
  auto func_magic = node->At(2);  // function argument option(magic)

  auto tobj = box_lir(func_tobj);
  auto arg_start = __ LeaTemp(TransOperandImmediate(func_aoff));
  auto magic = TransOperandImmediate(func_magic);

  Output(node, __ Apply(tobj, arg_start, magic));
}

// ----------------------------------------------------------------------------
// Object Property
void LIRLower::Lower_kObjectNew(Node* node) {
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kObjectNew);
  auto call = __ RCallBox(call_idx, 0);
  Output(node, call);
}

void LIRLower::Lower_kLoadPrimitiveProto(Node* node) {
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
void LIRLower::Lower_kObjectFieldGet(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = TransOperandImmediate(node->At(1));
  auto phi = __ FastSlow(
      [&](HAssembler*, HOperand* done) {
        auto fast_value = __ ObjectFieldGetAtomFast(target, atom);
        auto exp_tag = __ NewImmediate(JVal::kNotAvailable);
        auto cond = __ BoxTagIsNe(fast_value, exp_tag);
        __ BranchTrue(cond, done);  // If not uninit, jump to end of the PHI

        return fast_value;
      },
      HOpcode::kRCallBox,                    // box call
      BuiltinFunction::kObjectFieldGetAtom,  // slow path function type
      target, atom);

  CHECK(phi->IsBranchPhi());
  Output(node, phi);
}

void LIRLower::Lower_kArrayLengthGet(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = __ NewImmediate(JS_ATOM_length);

  auto phi = __ FastSlow(
      [&](HAssembler*, HOperand* done) {
        auto fast_value = __ MaybeArrayLengthGet(target);
        auto exp_tag = __ NewImmediate(JVal::kNotAvailable);
        auto cond = __ BoxTagIsNe(fast_value, exp_tag);
        __ BranchTrue(cond, done);  // If not uninit, jump to end of the PHI

        return fast_value;
      },
      HOpcode::kRCallBox,                    // box call
      BuiltinFunction::kObjectFieldGetAtom,  // slow path function type
      target, atom);

  CHECK(phi->IsBranchPhi());
  Output(node, phi);
}

HOperand* LIRLower::MaybeBoxOperand(HOperand* opr, const Type& t) {
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

    // For singletone value, we just use builtin intrinsic instruction
    case Type::kNull:
      return __ LoadNull();

    case Type::kUndefined:
      return __ LoadUndefined();

    case Type::kUninitialized:
      return __ LoadUninitialized();

    default:
      UNREACHABLE();
      return nullptr;
  }
}

HOperand* LIRLower::TransOperandBox(SSAVar* v) {
  auto opr = TransOperand(v);
  return MaybeBoxOperand(opr, v->type());
}

void LIRLower::Lower_kObjectFieldPut(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = TransOperandImmediate(node->At(1));
  auto value = MaybeBoxOperand(TransOperand(node->At(2)), node->At(2)->type());

  auto phi = __ FastSlow(
      [&](HAssembler*, HOperand* done) {
        auto fast_value = __ ObjectFieldSetAtomFast(target, atom, value);
        auto exp_tag = __ NewImmediate(JVal::kNotAvailable);
        auto cond = __ BoxTagIsNe(fast_value, exp_tag);
        __ BranchTrue(cond, done);  // If not, jump to end of the PHI
        return fast_value;
      },
      HOpcode::kRCallBox,                    // box call
      BuiltinFunction::kObjectFieldPutAtom,  // slow path function type
      target, atom, value);

  CHECK(phi->IsBranchPhi());

  // make sure to delete the returned value if needed. This is because of our
  // fast path cannot directly drop the rc, since dropping rc may involve
  // calling out a C function which is not something can be done without using
  // LIR construct
  __ RcDecMaybeDeleteBox(phi);

  Output(node, phi);
}

void LIRLower::Lower_kObjectFieldDefine(Node* node) {
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

void LIRLower::Lower_kObjectNameSet(Node* node) {
  auto target = TransOperand(node->At(0));
  auto atom = TransOperandImmediate(node->At(1));

  // generate a runtime call here,
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kObjectFieldNameSet);

  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, atom);

  Output(node, call);
}

void LIRLower::Lower_kObjectNameSetComputed(Node* node) {
  auto target = TransOperand(node->At(0));
  auto name = TransOperandBox(node->At(2));

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
void LIRLower::DoSpecializeGlobalGet(Node* node, bool is_get_var) {
  auto atom = TransOperandImmediate(node->At(0));
  auto flag = TransOperandImmediate(node->At(1));

  // The global get will be specialized with FastSlow code path, the fast path
  // will do fast check against 2 object,
  //
  //   ctx->global_var_obj and
  //   ctx->global_obj
  //
  // The GlobalGetByAtomFast is been speicalized to do so

  auto phi = __ FastSlow(
      [&](HAssembler*, HOperand* done) {
        HOperand* fast_value = nullptr;
        if (is_get_var) {
          fast_value = __ GlobalGetVarAtomFast(atom, flag);
        } else {
          fast_value = __ GlobalGetGlobalAtomFast(atom, flag);
        }

        auto exp_tag = __ NewImmediate(JVal::kUninitialized);
        auto cond = __ BoxTagIsNe(fast_value, exp_tag);

        __ BranchTrue(cond, done);  // If not uninit, jump to end of the PHI
        return fast_value;
      },
      HOpcode::kRCallBox,           // box call
      BuiltinFunction::kGlobalGet,  // slow path function type
      atom, flag);

  CHECK(phi->IsBranchPhi());
  Output(node, phi);
}

void LIRLower::Lower_kGlobalGetVar(Node* node) {
  DoSpecializeGlobalGet(node, true);
}

void LIRLower::Lower_kGlobalGetGlobal(Node* node) {
  DoSpecializeGlobalGet(node, false);
}

void LIRLower::Lower_kGlobalGet(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto flag = TransOperandImmediate(node->At(1));
  auto cidx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalGet);
  auto call = __ RCallBox(cidx, 2);
  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, flag);

  Output(node, call);
}

void LIRLower::Lower_kGlobalSet(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto value = TransOperandBox(node->At(1));
  auto flag = TransOperandImmediate(node->At(2));

  // generate a runtime call here,
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalPut);
  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, value);
  call->instruction_checked()->OprSet(3, flag);

  Output(node, call);
}

void LIRLower::Lower_kGlobalCheck(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalCheck);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, atom);
  Output(node, call);
}

void LIRLower::Lower_kGlobalDefine(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto flag = TransOperandImmediate(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalDefine);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, flag);
  Output(node, call);
}

void LIRLower::Lower_kGlobalDefineFunction(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto func = TransOperand(node->At(1));  // must be object
  auto flag = TransOperandImmediate(node->At(2));

  auto call_idx =
      __ NewBuiltinCallIndex(BuiltinFunction::kGlobalDefineFunction);
  auto call = __ RCallBox(call_idx, 3);
  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, func);
  call->instruction_checked()->OprSet(3, flag);

  Output(node, call);
}

void LIRLower::Lower_kGlobalCheckDefine(Node* node) {
  auto atom = TransOperandImmediate(node->At(0));
  auto flag = TransOperandImmediate(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGlobalCheckDefine);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, atom);
  call->instruction_checked()->OprSet(2, flag);
  Output(node, call);
}

// ----------------------------------------------------------------------------
// Array
void LIRLower::Lower_kLoadArray(Node* node) {
  auto array = TransOperandBox(node->At(0));
  auto index = TransOperandBox(node->At(1));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kLoadArray);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, array);
  call->instruction_checked()->OprSet(2, index);
  Output(node, call);
}

void LIRLower::DoSpecializeLoadArray(
    Node* node, std::function<HOperand*(HOperand*, HOperand*)>&& gen) {
  auto array = TransOperand(node->At(0));
  auto index = TransOperand(node->At(1));

  auto value = __ FastSlow(
      [&](HAssembler*, HOperand* done) {
        auto fast_value = gen(array, index);
        auto exp_tag = __ NewImmediate(JVal::kNotAvailable);
        auto cond = __ BoxTagIsNe(fast_value, exp_tag);
        __ BranchTrue(cond, done);  // If not uninit, jump to end of the PHI

        return fast_value;
      },
      HOpcode::kRCallBox,              // box call
      BuiltinFunction::kLoadArrayInt,  // slow path function type
      array, index);

  Output(node, value);
}

void LIRLower::Lower_kLoadArrayValue(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayValue(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayArguments(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayArguments(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayInt8(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayInt8(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayUInt8(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayUInt8(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayUInt8C(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayUInt8C(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayInt16(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayInt16(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayUInt16(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayUInt16(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayInt32(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayInt32(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayUInt32(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayUInt32(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayF32(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayF32(array, index);
                        });
}

void LIRLower::Lower_kLoadArrayF64(Node* node) {
  DoSpecializeLoadArray(node,
                        [&](HOperand* array, HOperand* index) -> HOperand* {
                          return __ LoadArrayF64(array, index);
                        });
}

// ----------------------------------------------------------------------------
// Context
void LIRLower::Lower_kContextLoadGlobalObject(Node* node) {
  Output(node, __ ContextLoadGlobalObject());
}

void LIRLower::Lower_kContextLoad(Node* node) {
  Output(node, __ ContextLoad());
}

// ----------------------------------------------------------------------------
// This
void LIRLower::Lower_kGThisIsNullOrUndefined(Node* node) {
  auto tag = __ LoadThisTag();
  auto cond = __ IsNullOrUndefined(tag);
  __ DeoptIfFalse(node->exit(), cond);
}

void LIRLower::Lower_kLoadThisObject(Node* node) {
  Output(node, __ LoadThis());
}

// ----------------------------------------------------------------------------
// SetPropertyValue
void LIRLower::Lower_kSetPropertyValue(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto index = TransOperandBox(node->At(1));
  auto value = TransOperandBox(node->At(2));
  auto flag = TransOperandImmediate(node->At(3));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kSetPropertyValue);
  auto call = __ RCallBox(call_idx, 4);

  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, index);
  call->instruction_checked()->OprSet(3, value);
  call->instruction_checked()->OprSet(4, flag);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// SetPropertyValue
void LIRLower::Lower_kDefinePropertyValueValue(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto index = TransOperandBox(node->At(1));
  auto value = TransOperandBox(node->At(2));
  auto flag = TransOperandImmediate(node->At(3));

  auto call_idx =
      __ NewBuiltinCallIndex(BuiltinFunction::kDefinePropertyValueValue);
  auto call = __ RCallBox(call_idx, 4);

  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, index);
  call->instruction_checked()->OprSet(3, value);
  call->instruction_checked()->OprSet(4, flag);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// Misc
void LIRLower::Lower_kCopyDataProperties(Node* node) {
  auto a0 = TransOperandBox(node->At(0));
  auto a1 = TransOperandBox(node->At(1));
  auto a2 = TransOperandBox(node->At(2));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCopyDataProperties);
  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);
  call->instruction_checked()->OprSet(3, a2);

  Output(node, call);
}

void LIRLower::Lower_kAppend(Node* node) {
  auto a0 = TransOperandBox(node->At(0));
  auto a1 = TransOperandBox(node->At(1));
  auto a2 = TransOperandBox(node->At(2));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kAppend);
  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);
  call->instruction_checked()->OprSet(3, a2);

  Output(node, call);
}

void LIRLower::Lower_kJSCheckBrand(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto value = TransOperandBox(node->At(1));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kCheckBrand);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, value);

  Output(node, call);
}

void LIRLower::Lower_kJSAddBrand(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto value = TransOperandBox(node->At(1));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kAddBrand);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, value);
  Output(node, call);
}

void LIRLower::Lower_kJSRegexp(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto value = TransOperandBox(node->At(1));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kRegexp);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, value);
  Output(node, call);
}

void LIRLower::Lower_kJSGetSuper(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGetSuper);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, target);
  Output(node, call);
}

void LIRLower::Lower_kJSImport(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kImport);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, target);
  Output(node, call);
}

void LIRLower::Lower_kJSSetProto(Node* node) {
  auto target = TransOperandBox(node->At(0));
  auto proto = TransOperandBox(node->At(1));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kSetProto);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, target);
  call->instruction_checked()->OprSet(2, proto);
  Output(node, call);
}

void LIRLower::Lower_kJSSetHomeObject(Node* node) {
  auto a0 = TransOperandBox(node->At(0));
  auto a1 = TransOperandBox(node->At(1));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kSetHomeObject);
  auto call = __ RCallBox(call_idx, 2);
  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);
  Output(node, call);
}

// ----------------------------------------------------------------------------
// SuperValue
void LIRLower::Lower_kJSGetSuperValue(Node* node) {
  auto a0 = TransOperandBox(node->At(0));
  auto atom = TransOperand(node->At(1));  // this is atom, do not box
  auto a2 = TransOperandBox(node->At(2));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kGetSuperValue);

  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, atom);
  call->instruction_checked()->OprSet(3, a2);

  Output(node, call);
}

void LIRLower::Lower_kJSPutSuperValue(Node* node) {
  auto a0 = TransOperandBox(node->At(0));
  auto atom = TransOperand(node->At(1));  // this is atom, do not box
  auto a2 = TransOperandBox(node->At(2));
  auto a3 = TransOperandBox(node->At(3));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kPutSuperValue);
  auto call = __ RCallBox(call_idx, 4);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, atom);
  call->instruction_checked()->OprSet(3, a2);
  call->instruction_checked()->OprSet(4, a3);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// Private field
void LIRLower::Lower_kPrivateFieldPut(Node* node) {
  auto a0 = TransOperandBox(node->At(0));
  auto a1 = TransOperandBox(node->At(1));
  auto a2 = TransOperandBox(node->At(2));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kPrivateFieldPut);
  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);
  call->instruction_checked()->OprSet(3, a2);

  Output(node, call);
}

void LIRLower::Lower_kPrivateFieldDefine(Node* node) {
  auto a0 = TransOperandBox(node->At(0));
  auto a1 = TransOperandBox(node->At(1));
  auto a2 = TransOperandBox(node->At(2));

  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kPrivateFieldDefine);
  auto call = __ RCallBox(call_idx, 3);

  call->instruction_checked()->OprSet(1, a0);
  call->instruction_checked()->OprSet(2, a1);
  call->instruction_checked()->OprSet(3, a2);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// ForOf
void LIRLower::Lower_kForOfStart(Node* node) {
  auto idx = TransOperandImmediate(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kForOfStart);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, idx);

  Output(node, call);
}

void LIRLower::Lower_kForOfNext(Node* node) {
  auto idx = TransOperandImmediate(node->At(0));
  auto offset = TransOperandImmediate(node->At(1));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kForOfNext);
  auto call = __ RCallBox(call_idx, 2);

  call->instruction_checked()->OprSet(1, idx);
  call->instruction_checked()->OprSet(2, offset);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// ForIn
void LIRLower::Lower_kForInStart(Node* node) {
  auto idx = TransOperandImmediate(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kForInStart);
  auto call = __ RCallBox(call_idx, 1);
  call->instruction_checked()->OprSet(1, idx);

  Output(node, call);
}

void LIRLower::Lower_kForInNext(Node* node) {
  auto idx = TransOperandImmediate(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kForInNext);
  auto call = __ RCallBox(call_idx, 1);

  call->instruction_checked()->OprSet(1, idx);

  Output(node, call);
}

// ----------------------------------------------------------------------------
// Iterator
void LIRLower::Lower_kIterClose(Node* node) {
  auto iter = TransOperandBox(node->At(0));
  auto call_idx = __ NewBuiltinCallIndex(BuiltinFunction::kIterClose);
  auto call = __ RCallBox(call_idx, 1);

  call->instruction_checked()->OprSet(1, iter);
  Output(node, call);
}

// ----------------------------------------------------------------------------
// Frame
void LIRLower::Lower_kEnsureTempSize(Node* node) {
  auto value = TransOperandImmediate(node->At(0));
  __ EnsureTempSize(value);
}

void LIRLower::Lower_kEnterInlineFrame(Node* node) {
  __ EnterInlineFrame(
      TransOperandImmediate(node->At(0)),  // call depth
      TransOperandImmediate(node->At(1)),  // slot size
      TransOperandImmediate(node->At(2)),  // argument(parameter) size
      TransOperandImmediate(node->At(3)),  // local variable size
      TransOperandBox(node->At(4)),        // function object
      TransOperandBox(node->At(5)),        // this object
      TransOperandBox(node->At(6))         // target object
  );
}

void LIRLower::Lower_kLeaveInlineFrame(Node* node) { __ LeaveInlineFrame(); }

void LIRLower::Lower_kCloseVarRef(Node* node) {
  __ GuardSlow(
      [&](HAssembler*, HOperand* fallback) {
        auto is_empty = __ VarRefListIsEmpty();
        __ BranchTrue(is_empty, fallback);
      },
      HOpcode::kRCallVoid, BuiltinFunction::kCloseVarRef);
}

// ----------------------------------------------------------------------------
// ControlFlow
void LIRLower::Lower_kJump(Node* node) {
  auto bb1_label = __ BB1Label();
  __ Jmp(bb1_label);
}

void LIRLower::Lower_kStart(Node* node) { __ Start(); }

void LIRLower::Lower_kEnd(Node* node) { __ End(); }

void LIRLower::Lower_kBB1(Node* node) { __ BB1(); }

// ----------------------------------------------------------------------------
// Phi
void LIRLower::Lower_kPhiLoop(Node* node) {
  CHECK(node->opcode() == Opcode::kPhiLoop);
  CHECK(node->size() == 2);
  MarkPendingLoopPhi(node);
}

// ----------------------------------------------------------------------------
// NeverReached
#define UNREACHABLE_HANDLER(XXX) \
  void LIRLower::Lower_##XXX(Node*) { UNREACHABLE() << #XXX; }

UNREACHABLE_HANDLER(kKI32)
UNREACHABLE_HANDLER(kKI64)
UNREACHABLE_HANDLER(kKFloat64)
UNREACHABLE_HANDLER(kKBool)
UNREACHABLE_HANDLER(kKNull)
UNREACHABLE_HANDLER(kKUndef)
UNREACHABLE_HANDLER(kKUninit)
UNREACHABLE_HANDLER(kKStr)
UNREACHABLE_HANDLER(kRcInc)
UNREACHABLE_HANDLER(kRcDec)
UNREACHABLE_HANDLER(kRcTryInc)
UNREACHABLE_HANDLER(kRcTryDec)
UNREACHABLE_HANDLER(kSentry)

}  // namespace lir
}  // namespace quake
