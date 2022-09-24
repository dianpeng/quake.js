#include <base/os/api.h>

// for crossplatform host
#include <boost/asio.hpp>

namespace base {
namespace os {

std::optional<std::uint32_t> GetHardwareConcurrency() {
  if (auto v = std::thread::hardware_concurrency(); v) {
    return static_cast<std::uint32_t>(v);
  } else {
    return {};
  }
}

std::optional<std::string> GetHostname() {
  boost::system::error_code ec;
  auto v = boost::asio::ip::host_name(ec);
  if (ec) {
    return {};
  } else {
    return v;
  }
}

}  // namespace os
}  // namespace base

#ifdef OS_POSIX
#include "api-posix.inl"
#else
#error "invalid OS"
#endif  // OS_POSIX
