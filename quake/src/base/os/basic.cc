#include <base/os/basic.h>

#include <boost/algorithm/string.hpp>

namespace base {
namespace os {
namespace {

struct OSErrorCategory : public ErrorCategory {
  const char* GetName() const override final { return "os"; }

  std::string GetInformation(std::uint32_t code) const override final {
    switch (code) {
#define __(A, B) \
  case A:        \
    return B;
      OS_ERROR_CODE_LIST(__)
#undef __  // __

      default:
        return std::string{"os[no-reason]"};
    }
  }
};

const ErrorCategory* Cat() {
  static const OSErrorCategory kC;
  return &kC;
}

}  // namespace

void ParseNameServerFromResolveConf(std::vector<std::string>* vec,
                                    const std::string& buf) {
  std::size_t pos = 0;
  while (pos < buf.size()) {
    auto ln_brk = buf.find_first_of('\n', pos);
    auto ln = buf.substr(pos, (ln_brk - pos));
    boost::trim(ln);

    // Just take care of the nameserver field and skip any other lines
    if (boost::starts_with(ln, "nameserver")) {
      auto ip = ln.substr(10);
      boost::trim(ip);
      if (!ip.empty()) {
        vec->emplace_back(std::move(ip));
      }
    }
    pos = ln_brk + 1;
  }
  return;
}

Error Err(std::uint32_t c) { return Error{Cat(), c}; }

Error ErrV(std::uint32_t c, const char* fmt, va_list vl) {
  return Error{Cat(), c, FmtV(fmt, vl)};
}

}  // namespace os
}  // namespace base
