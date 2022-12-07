#include "lir/builtin.h"
#include "lir/instruction.h"
#include "lir/operand.h"

namespace quake {
namespace lir {
namespace {

#define __(A, B, _, C, ...)                                        \
  static BuiltinFunctionInfo A##Info{B, BuiltinParam{__VA_ARGS__}, \
                                     BuiltinReturn{C}};

QUAKE_BUILTIN_FUNCTION_LIST(__)

#undef __  // __

BuiltinFunctionInfo kDummyInfo{};

}  // namespace

const BuiltinFunctionInfo& GetBuiltinFunctionInfo(BuiltinFunction bf) {
  switch (bf) {
#define __(A, ...)         \
  case BuiltinFunction::A: \
    return A##Info;
    QUAKE_BUILTIN_FUNCTION_LIST(__)
#undef __  // __
    default:
      return kDummyInfo;
  }
}

bool BuiltinFunctionInstructionVerify(HInstruction* call) {
  CHECK(call->IsRCall());
  CHECK(call->opr_size() >= 1);

  auto opr0 = call->Opr(0);
  CHECK(opr0->IsImm());

  auto call_idx = static_cast<BuiltinFunction>(opr0->GetImm().Int32());
  const auto& cinfo = GetBuiltinFunctionInfo(call_idx);

  // parameter size does not match
  if (cinfo.param.ParamSize() != call->opr_size() - 1) {
    return false;
  }

  // check each type of operand
  for (int i = 0; i < cinfo.param.ParamSize(); i++) {
    auto opr = call->Opr(i + 1);
    CHECK(opr);
    if (opr->htype() != cinfo.param.ParamType(i)) {
      if (opr->HasInstruction()) {
        LOG(ERROR) << opr->instruction()->Print();
      }
      LOG(ERROR) << i << ": " << static_cast<int>(opr->htype()) << " : "
                 << static_cast<int>(cinfo.param.ParamType(i));
      return false;  // argument type mismatch
    }
  }

  return true;
}

}  // namespace lir
}  // namespace quake
