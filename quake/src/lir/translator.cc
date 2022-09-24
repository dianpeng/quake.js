#include "lir/translator.h"

/*
// Macro used to capture output(IR) from hassembler
#define OUTPUT(NODE, HOPR)                               \
  do {                                                   \
    auto opr = HOPR;                                     \
    CHECK(opr.IsTemporary());                            \
    opr.Temporary()->SetOutput((NODE)->out()->ra_ref()); \
  } while (false)

namespace quake {
namespace lir {

// ----------------------------------------------------------------------------
// RefCount
void CodeGenerator::DoRcInc(HOperand* node) {
  as_->Inc(HOperand::MemInd(node, Layout::OffsetRC()));
}

void CodeGenerator::DoRcDec(HOperand* node) {
  // try to decrement the RC
  as_->Dec(HOperand::MemInd(node, Layout::OffsetRC()));

  // if it is not zero, then skip the memory free
  auto label = as_->JmpNz(as_->NewLabel());

  // code for freeing the memory. notes the node is unboxed
  as_->CallN(HOperand::MemInd(&::__JS_FreeValue), node);

  // Binding the label here
  as_->Bind(label);
}

void CodeGenerator::EmitRcInc(Node* node) { DoRcInc(node->opr_vreg()); }

void CodeGenerator::EmitRcDec(Node* node) { DoRcDec(node->opr_vreg()); }

void CodeGenerator::EmitRcTryInc(Node* node) {
  CHECK(node->type().IsBox());

  // Get the box value's tag field
  auto opr = as_->LoadBoxValueTag(node->opr_vreg());

  // Check whether it is a valid tag gc type or not
  as_->Cmp(opr, 0);
  auto label = as_->JmpAt(as_->NewLabel());

  DoRcInc(node->opr_vreg());
  as_->Bind(label);
}

void CodeGenerator::EmitRcTryDec(Node* node) {
  CHECK(node->type().IsBox());

  // get the box value's tag field
  auto opr = as_->LoadBoxValueTag(node->out_vreg());

  // check whether it is a valid tag gc type or not
  as_->Cmp(opr, 0);
  auto label = as_->JmpAt(as_->NewLabel());

  DoRcDec(node->out_vreg());
  as_->Bind(label);
}

// ----------------------------------------------------------------------------
// Box value
void CodeGenerator::EmitBxOn(Node* node) {
  CHECK(!node->type().IsBox());

  auto qjs_tag = static_cast<std::int32_t>(T2JValTag(node->type().Tag()));
  if (node->type().IsHeap()) {
    OUTPUT(node, as_->BoxOnHeap(qjs_tag, node->out_vreg()));
  } else {
    OUTPUT(node, as_->BoxOnPrimitive(qjs_tag, node->out_vreg()));
  }
}

void CodeGenerator::EmitBxOff(Node* node) {
  CHECK(!node->type().IsBox());  // type has been inferred, ie guarded

  auto qjs_tag = static_cast<std::int32_t>(T2JValTag(node->type().Tag()));
  if (node->type().IsHeap()) {
    OUTPUT(node, as_->BoxOffHeap(qjs_tag, node->out_vreg()));
  } else {
    OUTPUT(node, as_->BoxOffPrimitive(qjs_tag, node->out_vreg()));
  }
}

void CodeGenerator::EmitBxGuard(Node* node) {
  auto qjs_tag = static_cast<std::int32_t>(T2JValTag(node->type().Tag()));

  DoTypeGuard(node->type(), node->opr_vreg(), node->exit());
  if (node->type().IsPrimitive()) {
    OUTPUT(node, as_->BoxLoadPrimitive(node->type(), node->opr_vreg()));
  } else {
    OUTPUT(node, as_->BoxLoadHeap(node->type(), node->opr_vreg()));
  }
}

// ----------------------------------------------------------------------------
// Guard
void CodeGenerator::DoTypeGuard(const Type& type, Node* opr, Checkpoint* exit) {
  auto qjs_tag = T2JValTag(type.Tag());
  auto vreg = as_->LoadBoxValueTag(opr);
  as_->Cmp(vreg, qjs_tag);
  as_->JmpNe(as_->CheckpointLabel(exit));
}

void CodeGenerator::EmitGType(Node* node) {
  CHECK(node->type().IsBox());
  DoTypeGuard(node->type(), node->opr_vreg(), node->exit());
}

void CodeGenerator::EmitGNotZeroInt(Node* node) {
  auto vreg = node->opr_vreg();
  as_->Test(vreg, vreg);
  as_->JmpZ(as_->CheckpointLabel(node->exit()));
}

// ----------------------------------------------------------------------------
// Local
void CodeGenerator::EmitLocGet(Node* node) {
  auto lit = node->Opr()->Int32();
  OUTPUT(node, as_->LoadLocal(lit));
}

void CodeGenerator::EmitLocSet(Node* node) {
  auto lit = node->Lhs()->Int32();
  auto value = node->rhs_vreg();
  as_->StoreLocal(lit, value);
}

// ----------------------------------------------------------------------------
// Param
void CodeGenerator::EmitParamGet(Node* node) {
  auto lit = node->Opr()->Int32();
  OUTPUT(node, as_->LoadParam(lit));
}

void CodeGenerator::EmitParamSet(Node* node) {
  auto lit = node->Lhs()->Int32();
  auto value = node->rhs_vreg();
  as_->StoreParam(lit, value);
}

// ----------------------------------------------------------------------------
// Temp
void CodeGenerator::EmitTempGet(Node* node) {
  auto lit = node->Opr()->Int32();
  OUTPUT(node, as_->LoadTemp(lit));
}

void CodeGenerator::EmitTempSet(Node* node) {
  auto lit = node->Lhs()->Int32();
  auto value = node->rhs_vreg();
  as_->StoreTemp(lit, value);
}

// ----------------------------------------------------------------------------
// Arithmetic
void CodeGenerator::EmitAddInt(Node* node) {
  auto lhs = node->lhs_vreg();
  auto rhs = node->rhs_vreg();
  OUTPUT(node, as_->IAdd(lhs, rhs));
}

void CodeGenerator::EmitAddReal(Node* node) {
  auto lhs = node->lhs_vreg();
  auto rhs = node->rhs_vreg();
  OUTPUT(node, as_->FAdd(lhs, rhs));
}

}  // namespace lir
}  // namespace quake

*/
