#include <base/config/reflection.h>

namespace base {
namespace config {

Value StructInfo::Dump() const {
  auto list = std::make_shared<List>();
  for (const auto& f : field_list()) {
    auto obj = std::make_shared<Dictionary>();
    obj->Add("name", Value{f->name()});
    obj->Add("comment", Value{f->comment()});
    obj->Add("path", Value{f->config_path()});
    obj->Add(
        "type",
        Value{f->field_type() == FieldType::kRequire ? "require" : "optional"});
    obj->Add("default", f->GetDefault());
    list->Add(Value{std::move(obj)});
  }
  return Value{std::move(list)};
}

std::string StructInfo::PrintString() const {
  std::stringstream fmt;
  std::size_t idx = 0;
  for (const auto& f : field_list()) {
    fmt << idx << ". " << f->name() << " ("
        << (f->field_type() == FieldType::kRequire ? "require" : "optional")
        << "| " << f->config_path() << "| " << f->GetDefault().Print()
        << "]: " << f->comment() << "\n";
    ++idx;
  }
  return fmt.str();
}

}  // namespace config
}  // namespace base
