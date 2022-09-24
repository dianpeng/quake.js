#ifndef _BASE_CONFIG_UNMARSHAL_H_
#define _BASE_CONFIG_UNMARSHAL_H_
#include "reflection.h"

namespace base {
namespace config {

template <typename T>
Error Default(T* r) {
  std::any wrapper{r};
  const auto& struct_info = GetStructInfo<T>();
  for (const auto& field : struct_info.field_list()) {
    if (auto e = field->SetDefault(&wrapper); e) {
      return e;
    }
  }
  return {};
}

template <typename T>
Error Unmarshal(const Value& v, T* r, bool ignore_unknown_field = false) {
  std::any wrapper{r};
  const auto& struct_info = GetStructInfo<T>();
  for (const auto& field : struct_info.field_list()) {
    if (auto maybe = field->search_pattern()->Search(v); !maybe) {
      if (ignore_unknown_field) continue;  // just skip the unknown field

      if (field->field_type() == FieldType::kRequire) {
        return Err(kErrorUnmarshelFieldNotFound,
                   "struct %s's required field %s is not existed or invalid",
                   struct_info.struct_name().c_str(), field->name().c_str());
      } else {
        if (auto e = field->SetDefault(&wrapper); e) {
          return e;
        }
      }
    } else {
      auto field_value = Pass(maybe);
      if (auto e = field->Set(&wrapper, field_value); e) {
        return e;
      }
    }
  }

  return {};
}

}  // namespace config
}  // namespace base

#endif  // _BASE_CONFIG_UNMARSHAL_H_
