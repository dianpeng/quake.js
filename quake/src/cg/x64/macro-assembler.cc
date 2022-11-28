#include "cg/x64/macro-assembler.h"
#include "cg/x64/asm-util.h"
#include "cg/x64/code-gen.h"

#include "quake.h"
#include "quake-rt.h"

#define __ this->

namespace quake {
namespace cg {
namespace x64 {

void MacroAssembler::unreachable(int code) {
  __ mov(asmjit::x86::edi, asmjit::Imm(code));
  __ call(FADDR(&QuakeJit::Unreachable));
  __ int3();
}

static constexpr std::size_t kSafeFunctionCallStackReserve = (7 + 16) * 8;

// This function will assume anything (any registers) are unsafe to be used
void MacroAssembler::SafeFunctionCallPrologue() {
  // save all the caller saved registers, regardlessly,

  // 1) all the general purpose registers for caller saved
  __ push(asmjit::x86::rax);
  __ push(asmjit::x86::rdi);
  __ push(asmjit::x86::rsi);
  __ push(asmjit::x86::rcx);
  __ push(asmjit::x86::rdx);
  __ push(asmjit::x86::r8);
  __ push(asmjit::x86::r9);

  // 2) all the sse registers are not gonna sustain
  __ sub(asmjit::x86::rsp, 16 * 8);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 15), asmjit::x86::xmm0);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 14), asmjit::x86::xmm1);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 13), asmjit::x86::xmm2);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 12), asmjit::x86::xmm3);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 11), asmjit::x86::xmm4);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 10), asmjit::x86::xmm5);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 9), asmjit::x86::xmm6);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 8), asmjit::x86::xmm7);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 7), asmjit::x86::xmm8);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 6), asmjit::x86::xmm9);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 5), asmjit::x86::xmm10);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 4), asmjit::x86::xmm11);

  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 3), asmjit::x86::xmm12);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 2), asmjit::x86::xmm13);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 1), asmjit::x86::xmm14);
  __ movq(asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 0), asmjit::x86::xmm15);

  // note we assume rsp is correctly aligned, so nothing needs to be done
}

void MacroAssembler::SafeFunctionCallEpilogue() {
  // restore all the xmm registers
  __ movq(asmjit::x86::xmm0, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 15));
  __ movq(asmjit::x86::xmm1, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 14));
  __ movq(asmjit::x86::xmm2, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 13));
  __ movq(asmjit::x86::xmm3, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 12));

  __ movq(asmjit::x86::xmm4, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 11));
  __ movq(asmjit::x86::xmm5, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 10));
  __ movq(asmjit::x86::xmm6, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 9));
  __ movq(asmjit::x86::xmm7, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 8));

  __ movq(asmjit::x86::xmm8, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 7));
  __ movq(asmjit::x86::xmm9, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 6));
  __ movq(asmjit::x86::xmm10, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 5));
  __ movq(asmjit::x86::xmm11, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 4));

  __ movq(asmjit::x86::xmm12, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 3));
  __ movq(asmjit::x86::xmm13, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 2));
  __ movq(asmjit::x86::xmm14, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 1));
  __ movq(asmjit::x86::xmm15, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8 * 0));

  __ add(asmjit::x86::rsp, 16 * 8);

  __ pop(asmjit::x86::r9);
  __ pop(asmjit::x86::r8);
  __ pop(asmjit::x86::rdx);
  __ pop(asmjit::x86::rcx);
  __ pop(asmjit::x86::rdi);
  __ pop(asmjit::x86::rsi);
  __ pop(asmjit::x86::rax);
}

void MacroAssembler::debug_i32(asmjit::x86::Gp v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_i32(std::int32_t v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_i64(asmjit::x86::Gp v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_i64(std::int64_t v) {
  SafeFunctionCallPrologue();
  __ mov(asmjit::x86::edi, v);
  __ call(FADDR(&QuakeJit::PrintI32));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_f64(asmjit::x86::Xmm v) {
  SafeFunctionCallPrologue();
  __ movq(asmjit::x86::xmm0, v);
  __ call(FADDR(&QuakeJit::PrintF64));
  SafeFunctionCallEpilogue();
}

void MacroAssembler::debug_f64(double v) {
  SafeFunctionCallPrologue();
  __ movabs(asmjit::x86::rax, *reinterpret_cast<const std::int64_t*>(&v));
  __ movq(asmjit::x86::xmm0, asmjit::x86::rax);
  __ call(FADDR(&QuakeJit::PrintF64));
  SafeFunctionCallEpilogue();
}

// ----------------------------------------------------------------------------
// cell operations

asmjit::x86::Mem MacroAssembler::CellAddress(int index) {
  auto offset = -(cg_->quake()->cell_offset() + index * Layout::kCellSize) -
                Layout::kCellSize;
  return asmjit::x86::ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::CellAddressHigher(int index) {
  static_assert(Layout::kCellSize == 16);

  auto offset = -(cg_->quake()->cell_offset() + index * Layout::kCellSize) -
                Layout::kCellSize + 8;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::CellAddressLower(int index) {
  static_assert(Layout::kCellSize == 16);

  auto offset = -(cg_->quake()->cell_offset() + index * Layout::kCellSize) -
                Layout::kCellSize;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

std::pair<asmjit::x86::Mem, asmjit::x86::Mem> MacroAssembler::GetCellAddress(
    int cell_index) const {
  auto start = -(cg_->quake()->cell_offset() + cell_index * Layout::kCellSize) -
               Layout::kCellSize;

  auto value_offset = start + Layout::Value::kValue;
  auto tag_offset = start + Layout::Value::kTag;
  auto value_addr = asmjit::x86::ptr(asmjit::x86::rbp, value_offset);
  auto tag_addr = asmjit::x86::qword_ptr(asmjit::x86::rbp, tag_offset);

  return std::make_pair(value_addr, tag_addr);
}

void MacroAssembler::store_cell(int cell_index, asmjit::x86::Gp value,
                                int tag) {
  auto [value_addr, tag_addr] = GetCellAddress(cell_index);
  __ mov(value_addr, value);
  __ mov(tag_addr, tag);
}

void MacroAssembler::store_cell(int cell_index, asmjit::x86::Xmm value,
                                int tag) {
  auto [value_addr, tag_addr] = GetCellAddress(cell_index);
  __ movsd(value_addr, value);
  __ mov(tag_addr, tag);
}

void MacroAssembler::store_cell(asmjit::x86::Mem tar, asmjit::x86::Gp value,
                                int tag) {
  auto val_addr = tar.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = tar.cloneAdjusted(Layout::Value::kTag);

  tag_addr.setSize(8);  // qword

  __ mov(val_addr, value);
  __ mov(tag_addr, tag);
}

void MacroAssembler::store_cell(asmjit::x86::Mem tar, asmjit::x86::Xmm value,
                                int tag) {
  auto val_addr = tar.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = tar.cloneAdjusted(Layout::Value::kTag);

  tag_addr.setSize(8);  // qword

  __ movsd(val_addr, value);
  __ mov(tag_addr, tag);
}

void MacroAssembler::move128(asmjit::x86::Gp dest, asmjit::x86::Gp source) {
  // use movups to quickly move the value from source to dest
  __ movups(SCR_FP, asmjit::x86::xmmword_ptr(source));
  __ movups(asmjit::x86::xmmword_ptr(dest), SCR_FP);
}

asmjit::x86::Mem MacroAssembler::ScriptCallAddress() {
  auto offset = -(Layout::MiscData::kScriptCall + Layout::kScriptCallSize);
  return asmjit::x86::ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::ScriptCallFunctionObjectAddress() {
  auto offset = -(Layout::MiscData::kScriptCall + Layout::kScriptCallSize);
  offset += Layout::ScriptCall::kFunctionObject;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::ScriptCallThisObjectAddress() {
  auto offset = -(Layout::MiscData::kScriptCall + Layout::kScriptCallSize);
  offset += Layout::ScriptCall::kThisObject;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::ScriptCallTargetObjectAddress() {
  auto offset = -(Layout::MiscData::kScriptCall + Layout::kScriptCallSize);
  offset += Layout::ScriptCall::kTargetObject;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::ScriptCallReturnAddressAddress() {
  auto offset = -(Layout::MiscData::kScriptCall + Layout::kScriptCallSize);
  offset += Layout::ScriptCall::kReturnAddress;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::ScriptCallParameterStartAddress() {
  auto offset = -(Layout::MiscData::kScriptCall + Layout::kScriptCallSize);
  offset += Layout::ScriptCall::kParameterStart;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

asmjit::x86::Mem MacroAssembler::ScriptCallParameterSizeAddress() {
  auto offset = -(Layout::MiscData::kScriptCall + Layout::kScriptCallSize);
  offset += Layout::ScriptCall::kParameterSize;

  return asmjit::x86::qword_ptr(asmjit::x86::rbp, offset);
}

MacroAssembler::MacroAssembler(CodeGen* cg)
    : asmjit::x86::Builder(&cg->code()->code_holder()), cg_(cg) {}

}  // namespace x64
}  // namespace cg
}  // namespace quake
