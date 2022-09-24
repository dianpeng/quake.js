#include <base/flag.h>

namespace base {
namespace flag {
namespace {

struct FlagError : public ErrorCategory {
  const char* GetName() const override final { return "flag"; }
  std::string GetInformation(std::uint32_t v) const override final {
    switch (v) {
      case kErrorInvalidType:
        return "type[invalid type]";
      case kErrorInvalidValue:
        return "type[invalid value]";
      default:
        return "type[]";
    }
  }
};

}  // namespace

const ErrorCategory* ErrCategory() {
  static FlagError kC;
  return &kC;
}

std::string ListFlags() {
  std::stringstream out;
  std::size_t idx = 1;
  for (const auto& entry : _::FlagRegistry::GetInstance()) {
    out << idx << ". " << entry->name() << " [" << entry->type() << ": ("
        << entry->GetDefaultRepresentation() << ")@(" << entry->To().Print()
        << ")]: " << entry->comment() << "\n";
    ++idx;
  }
  return out.str();
}

Value DumpFlags() {
  auto list = std::make_shared<List>();
  for (const auto& entry : _::FlagRegistry::GetInstance()) {
    auto dict = std::make_shared<Dictionary>();
    dict->Add("name", Value{entry->name()});
    dict->Add("type", Value{entry->type()});
    dict->Add("comment", Value{entry->comment()});
    dict->Add("file", Value{entry->file()});
    dict->Add("line", Value{entry->line()});
    dict->Add("default", Value{entry->GetDefaultRepresentation()});
    dict->Add("value", entry->To());
    list->Add(Value{std::move(dict)});
  }
  return Value{std::move(list)};
}

}  // namespace flag
}  // namespace base
