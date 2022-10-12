#include "lir/translator.h"

#if 0

#define as_ ->__

namespace quake {
namespace lir {

using namespace ir;

void Translator::RunOne(Node* node) {
  as_->BeginOPcode(node);
  current_ = node;

  TryAddBlock(node);

  switch (node->opcode()) {
#define __(A, ...)   \
  case A:            \
    Trans_##A(node); \
    break;
    QUAKE_OPCODE_LIST(__)
#undef __  // __
    default:
      UNREACHABLE();
      break;
  }

  if (node->IsValue()) {
    CHECK(node->out()->lir_operand());
  }

  prev_ = node;
  as_->EndOpcode();
}

void Translator::Run(Graph* graph) {
  // Pass 1) Run through all the ir::Node in its control dependency order
  graph->Foreach([&](Node* n) { RunOne(n); });

  // Pass 2) Resolve all the pending phis
  MaterializePhiList();

  // Pass 3) Resolve all the pending jump
  ResolvePendingJump();
}

// ----------------------------------------------------------------------------
// Jump (label/control flow)
//
// The control flow is been arranged as following. During translation, we do
// not know the jump target's HOperand yet, for example jumpping forward.
HOperand* Translator::GetJumpBlock(Node* from) {
  auto itr = block_.find(from);
  CHECK(itr != block_.end());
  CHECK(itr->IsLabel());
  CHECK(itr->HasInstruction());
  return *itr;
}

void Translator::TryAddBlock(Node* node) {
  if (node->IsLabel()) {
    // only take care of label node, ie there's some HIR instruction that will
    // jump to this node
    auto instruction = as_->LastInstruction();
    auto label = __ NewLabel(instruction);
    auto block = __ Block(label);
    auto ret = block_.insert(std::make_pair(node, block));
    CHECK(ret.second);
  }
}

void Translator::AddPendingJump(HInstruction* jump, Node* where) {
  CHECK(jump->IsJump());
  CHECK(where->IsLabel());
  pending_jump_.emplace_back(jump, where);
}

void Translator::ResolvePendingJump() {
  for (auto& pj : pending_jump_) {
    CHECK(pj.size() == 1);
    auto operand = GetJumpBlock(pj.label);
    pj.jump.OprSet(0, operand);
  }
  pending_jump_.clear();
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
      auto lir = __ PhiLoop();
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
// HIR is been emitted right after BB0 instruction. But that's fine, since we
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
  return *itr;
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
    case Opcode::kNull:
    case Opcode::kUndef:
    case Opcoed::kUninit:
      UNREACHABLE() << "should already been resolved in HIR";
      break;

    case Opcode::kNStr:
    case Opcode::kStr:
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
  auto operand = TransOperand(node, ir_opr);

  __ RcInc(operand);
}

void Translator::DoRcDec(Node* node) {
  CHECK(node->opcode() == Opcode::kRcDec);
  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  auto operand = TransOperand(node, ir_opr);
  __ RcDec(operand);
}

void Translator::DoRcTryOp(Node* node, bool inc) {
  CHECK(inc ? node->opcode() == Opcode::kRcInc
            : node->opcode() == Opcode::kRcDec);
  CHECK(node->size() == 1);

  auto ir_opr = node->At(0);
  CHECK(ir_opr->type().IsBox());

  auto operand = TransOperand(node, ir_opr);

  // testify whether the box is heap or not
  __ BoxIsHeap(operand);

  // get a jump label
  auto label = __ NewLabel();

  // jump out of the heap rc operation
  __ JumpZ(label);

  {
    // unbox the value
    auto unbox = __ Unbox(operand);

    if (inc) {
      __ RcInc(unbox);
    } else {
      __ RcDec(unbox);
    }
  }

  label->Bind(as_);
}

void Translator::DoRcTryInc(Node* node) { DoRcTryOp(node, true); }

void Translator::DoRcTryDec(Node* node) { DoRcTryOp(node, false); }

// ----------------------------------------------------------------------------
// Box value
void Translator::Trans_kBox(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kBx);  // ie boxing a value

  auto qjs_tag = static_cast<std::int32_t>(T2JValTag(node->type().Tag()));
  auto ret_opr =
      __ Box(TransOperand(node, node->At(0)), __ NewImmediate(qjs_tag));

  Output(node, ret_opr);
}

void Translator::Trans_kUnbox(Node* node) {
  CHECK(node->type().IsBox());
  CHECK(node->size() == 1);
  CHECK(node->opcode() == Opcode::kBxGuard);  // ie unboxing a value

  auto qjs_tag = static_cast<std::int32_t>(node->type().Tag());
  auto ret_opr =
      __ Unbox(TransOperand(node, node->At(0)), __ NewImmediate(qjs_tag));

  Output(node, ret_opr);
}

// ----------------------------------------------------------------------------
// Guard
void Translator::Trans_kGType(Node* node) {
  CHECK(!node->type().IsBox());
  CHECK(node->opcode() == Opcode::kGType);
  CHECK(node->size() == 1);
  CHECK(node->At(0)->type().IsBox());
  CHECK(node->HasExit());

  auto expect_tag =
      __ NewImmediate(static_cast<std::int32_t>(T2JValTag(node->type().Tag())));

  auto box_tag = __ GetTTag(TransOperand(node, node->At(0)));

  // check whether 2 types are identical or not
  __ Cmp(box_tag, expect_tag);

  // if not equal, then just perform deoptimization
  __ DeoptIfEq(node->exit());
}

void Translator::Trans_kGNotZero(Node* node) {
  CHECK(node->type().IsInt());  // must be int
  CHECK(node->opcode() == Opcode::kGNotZero);
  CHECK(node->size() == 1);
  CHECK(node->HasExit());

  __ Cmp(TransOperand(node, node->At(0)), __ NewInt32Zero());
  __ DeoptIfEq(node->exit());
}

// ----------------------------------------------------------------------------
// Local
void Translator::Trans_kLocGet(Node* node) {
  CHECK(node->opcode() == Opcode::kLocGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadLocal(TransOperandImmediate(node, node->At(0))));
}

void Translator::Trans_kLocSet(Node* node) { UNUSE(node); }

// ----------------------------------------------------------------------------
// Param
void Translator::Trans_kParamGet(Node* node) {
  CHECK(node->opcode() == Opcode::kParamGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadParam(TransOperandImmediate(node, node->At(0))));
}

void Translator::Trans_kParamSet(Node* node) { UNUSE(node); }

// ----------------------------------------------------------------------------
// Param
void Translator::Trans_kTempGet(Node* node) {
  CHECK(node->opcode() == Opcode::kTempGet);
  CHECK(node->size() == 1);

  Output(node, __ LoadTemp(TransOperandImmediate(node, node->At(0))));
}

// ----------------------------------------------------------------------------
// Arithmetic
void Translator::Trans_kAddInt(Node* node) {
  CHECK(node->opcode() == Opcode::kAddInt);
  CHECK(node->size() == 2);

  Output(node, __ IAdd(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kSubInt(Node* node) {
  CHECK(node->opcode() == Opcode::kSubInt);
  CHECK(node->size() == 2);

  Output(node, __ ISub(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kMulInt(Node* node) {
  CHECK(node->opcode() == Opcode::kMulInt);
  CHECK(node->size() == 2);

  Output(node, __ IMul(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kDivInt(Node* node) {
  CHECK(node->opcode() == Opcode::kDivInt);
  CHECK(node->size() == 2);

  Output(node, __ IDiv(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kAddReal(Node* node) {
  CHECK(node->opcode() == Opcode::kAddReal);
  CHECK(node->size() == 2);

  Output(node, __ FAdd(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kSubReal(Node* node) {
  CHECK(node->opcode() == Opcode::kSubReal);
  CHECK(node->size() == 2);

  Output(node, __ FSub(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kMulReal(Node* node) {
  CHECK(node->opcode() == Opcode::kMulReal);
  CHECK(node->size() == 2);

  Output(node, __ FMul(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kDivReal(Node* node) {
  CHECK(node->opcode() == Opcode::kDivReal);
  CHECK(node->size() == 2);

  Output(node, __ FDiv(TransOperand(node, node->At(0)),
                       TransOperand(node, node->At(1))));
}

void Translator::Trans_kShlInt(Node* node) {
  CHECK(node->opcode() == Opcode::kShlInt);
  CHECK(node->size() == 2);
  Output(node, __ LShift(TransOperand(node, node->At(0)),
                         TransOperand(node, node->At(1))));
}

void Translator::Trans_kShrInt(Node* node) {
  CHECK(node->opcode() == Opcode::kShrInt);
  CHECK(node->size() == 2);
  Output(node, __ RShift(TransOperand(node, node->At(0)),
                         TransOperand(node, node->At(1))));
}

void Translator::Trans_kSarInt(Node* node) {
  CHECK(node->opcode() == Opcode::kShrInt);
  CHECK(node->size() == 2);
  Output(node, __ RLShift(TransOperand(node, node->At(0)),
                          TransOperand(node, node->At(1))));
}

void Translator::Trans_kBitNorm(Node* node) {
  CHECK(node->opcode() == Opcode::kBitNorm);
  CHECK(node->size() == 1);

  // nothing is translated, just output whatever as input for now
  Output(node, TransOperand(node, node->At(0)));
}

void Translator::Trans_kAndInt(Ndoe* node) {
  CHECK(node->opcode() == Opcode::kAndInt);
  CHECK(node->size() == 2);
  Output(node, __ And(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kOrInt(Ndoe* node) {
  CHECK(node->opcode() == Opcode::kAndInt);
  CHECK(node->size() == 2);
  Output(node, __ Or(TransOperand(node, node->At(0)),
                     TransOperand(node, node->At(1))));
}

void Translator::Trans_kXorInt(Node* node) {
  CHECK(node->opcode() == Opcode::kAndInt);
  CHECK(node->size() == 2);
  Output(node, __ Xor(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

// ----------------------------------------------------------------------------
// Comparison
//
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
//   #4: kJumpTrue #3 (#3 always return boolean)
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
  Output(node, __ ILt(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kLeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kLeInt);
  CHECK(node->size() == 2);
  Output(node, __ ILe(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kGtInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGtInt);
  CHECK(node->size() == 2);
  Output(node, __ IGt(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kGeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kGeInt);
  CHECK(node->size() == 2);
  Output(node, __ IGe(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kEqInt(Node* node) {
  CHECK(node->opcode() == Opcode::kEqInt);
  CHECK(node->size() == 2);
  Output(node, __ IEq(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kNeInt(Node* node) {
  CHECK(node->opcode() == Opcode::kNeInt);
  CHECK(node->size() == 2);
  Output(node, __ INe(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kLtReal(Node* node) {
  CHECK(node->opcode() == Opcode::kLtReal);
  CHECK(node->size() == 2);
  Output(node, __ FLt(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kLeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kLeReal);
  CHECK(node->size() == 2);
  Output(node, __ FLe(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kGtReal(Node* node) {
  CHECK(node->opcode() == Opcode::kGtReal);
  CHECK(node->size() == 2);
  Output(node, __ FGt(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kGeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kGeReal);
  CHECK(node->size() == 2);
  Output(node, __ FGe(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kEqReal(Node* node) {
  CHECK(node->opcode() == Opcode::kEqReal);
  CHECK(node->size() == 2);
  Output(node, __ FEq(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

void Translator::Trans_kNeReal(Node* node) {
  CHECK(node->opcode() == Opcode::kNeReal);
  CHECK(node->size() == 2);
  Output(node, __ FNe(TransOperand(node, node->At(0)),
                      TransOperand(node, node->At(1))));
}

// ----------------------------------------------------------------------------
// Unary operation
void Translator::Trans_kNegInt(Node* node) {
  CHECK(node->opcode() == Opcode::kNegInt);
  CHECK(node->size() == 1);
  Output(node, __ Neg(TransOperand(node, node->At(0))));
}

void Translator::Trans_kNegReal(Node* node) {
  CHECK(node->opcode() == Opcode::kNegReal);
  CHECK(node->size() == 1);
  Output(node, __ NegF(TransOperand(node, node->At(0))));
}

void Translator::Trans_kInvInt(Node* node) {
  CHECK(node->opcode() == Opcode::kInvInt);
  CHECK(node->size() == 1);
  Output(node, __ Inv(TransOperand(node, node->At(0))));
}

void Translator::Trans_kIncInt(Node* node) {
  CHECK(node->opcode() == Opcode::kIncInt);
  CHECK(node->size() == 1);
  Output(node, __ Inc(TransOperand(node, node->At(0))));
}

void Translator::Trans_kDecInt(Node* node) {
  CHECK(node->opcode() == Opcode::kDecInt);
  CHECK(node->size() == 1);
  Output(node, __ Dec(TransOperand(node, node->At(0))));
}

void Translator::Trans_kToBoolInt(Node* node) {
  CHECK(node->opcode() == Opcoed::kToBoolInt);
  CHECK(node->size() == 1);
  Output(node, __ INe(TransOperand(node, node->At(0)), __ NewInt32Zero()));
}

void Translator::Trans_kToBoolReal(Node* node) {
  CHECK(node->opcode() == Opcoed::kToBoolReal);
  CHECK(node->size() == 1);
  Output(node, __ FNe(TransOperand(node, node->At(0)), __ NewFloat64Zero()));
}

// ----------------------------------------------------------------------------
// Phi
void Translator::Trans_kPhiLoop(Node* node) {
  CHECK(node->opcode() == Opcode::kPhiLoop);
  CHECK(node->size() == 2);
  MarkPendingPhi(node);
}

// ----------------------------------------------------------------------------
// ControlFlow
//
// The Start/BB0/End instruction are just marker, so directly translate to
// kBlock which has an output operand as label.
void Translator::Trans_kJumpTrue(Node* node) {
  CHECK(node->opcode() == Opcode::kJumpTrue);
  CHECK(node->size() == 2);

  // translate condition into CmpTrue
  auto condition = node->At(0);
  __ CmpTrue(TransOperand(node, condition));

  // now jump to the corresponding label if true
  AddPendingJump(__ JmpEq(), node->At(1));
}

void Translator::Trans_kJumpFalse(Node* node) {
  CHECK(node->opcode() == Opcode::kJumpFalse);
  CHECK(node->size() == 2);

  // translate condition into CmpTrue
  auto condition = node->At(0);
  __ CmpFalse(TransOperand(node, condition));

  // now jump to the corresponding label if true
  AddPendingJump(__ JmpEq(), node->At(1));
}

void Translator::Trans_kStart(Node* node) { UNUSE(node); }

void Translator::Trans_kEnd(Node* node) { UNUSE(node); }

void Translator::Trans_kBB0(Node* node) { UNUSE(node); }

// ----------------------------------------------------------------------------

}  // namespace lir
}  // namespace quake

#endif
