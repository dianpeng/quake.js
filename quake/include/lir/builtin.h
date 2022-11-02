#ifndef _QUAKE_LIR_BUILTIN_H_
#define _QUAKE_LIR_BUILTIN_H_
#include "base.h"
#include "builtin-function.h"
#include "lir/htype.h"

namespace quake {
namespace lir {
class HInstruction;

// Used to verify the builtin function's parameter along with its type
struct BuiltinParam {
  std::vector<HType> type_list;

  explicit BuiltinParam(std::initializer_list<HType> input)
      : type_list(input) {}

  BuiltinParam() = default;

 public:
  int ParamSize() const { return static_cast<int>(type_list.size()); }
  HType ParamType(int i) const { return type_list[i]; }
};

struct BuiltinReturn {
  HType out_type;
};

struct BuiltinFunctionInfo {
  std::string name;
  BuiltinParam param;
  BuiltinReturn ret;
};

const BuiltinFunctionInfo& GetBuiltinFunctionInfo(BuiltinFunction);
bool BuiltinFunctionInstructionVerify(HInstruction*);

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_BUILTIN_H_
