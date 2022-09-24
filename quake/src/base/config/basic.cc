#include <base/config/basic.h>

namespace base {
namespace config {
namespace {

struct Category : public ErrorCategory {
  const char* GetName() const override { return "config"; }
  std::string GetInformation(std::uint32_t error_code) const override {
    switch (error_code) {
      // clang-format off
#define __(A, B) case A: return Fmt("config[%s]", B);
      CONFIG_ERROR_LIST(__)
#undef __  // __
      // clang-format on
      default:
        return "config[none]";
    }
  }
};

const ErrorCategory* Cat() {
  static Category kC;
  return &kC;
}

}  // namespace

Error Err(ConfigError ec) { return Error{Cat(), ec}; }

Error Err(ConfigError ec, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto e = ErrV(ec, fmt, vl);
  va_end(vl);

  return e;
}

Error ErrV(ConfigError ec, const char* fmt, va_list vl) {
  return Error{Cat(), ec, FmtV(fmt, vl)};
}

}  // namespace config
}  // namespace base
