#ifndef _BASE_CONFIG_MARSHAL_H_
#define _BASE_CONFIG_MARSHAL_H_
#include "reflection.h"

namespace base {
namespace config {

template <typename T>
Result<Value> Marshal(const T& value) {
  std::any wrapper{&value};
  auto root = std::make_shared<Dictionary>();
  const auto& struct_info = GetStructInfo<T>();
  for (const auto& field : struct_info.field_list()) {
    if (auto r = field->Get(&wrapper); !r) {
      return r.PassError();
    } else {
      root->Add(field->name(), r.PassResult());
    }
  }

  return Value{std::move(root)};
}

}  // namespace config
}  // namespace base

#endif  // _BASE_CONFIG_MARSHAL_H_
