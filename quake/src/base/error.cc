#include <base/error.h>
#include <base/value.h>

namespace base {
namespace {

class GeneralErrorCategory : public ErrorCategory {
 public:
  const char* GetName() const override final { return "general"; }

  std::string GetInformation(std::uint32_t code) const override final {
    // TODO(dpeng): Currently, we don't have too much general error reason
    //   category here. Just dump the literal out
    switch (code) {
      case Error::kReasonInternal:
        return std::string{"general[internal]"};
      case Error::kReasonUnspecified:
        return std::string{"general[unspecified]"};
      default:
        return std::string{"general[no-reason]"};
    }
  }
};

class ErrnoErrorCategory : public ErrorCategory {
 public:
  const char* GetName() const override final { return "errno"; }

  std::string GetInformation(std::uint32_t e) const override final {
    return Fmt("errno[%u|%s]", e, ErrString(e).c_str());
  }
};

}  // namespace

const ErrorCategory* Error::GeneralCategory() {
  static GeneralErrorCategory kErrorCategory;
  return &kErrorCategory;
}

const ErrorCategory* Error::ErrnoCategory() {
  static ErrnoErrorCategory kErrorCategory;
  return &kErrorCategory;
}

std::string Error::GetJSON() const {
  return ErrorToValue(*this).ToJSON(ValueToJSON::kPrettyPrint);
}

}  // namespace base
