#include <base/check.h>
#include <base/uuid.h>

#ifdef OS_POSIX
#define BOOST_UUID_RANDOM_PROVIDER_FORCE_POSIX
#define BOOST_UUID_RANDOM_PROVIDER_DISABLE_GETRANDOM
#else
#error "unsupport OS, not tested yet"
#endif  // OS_POSIX

#define BOOST_UUID_RANDOM_PROVIDER_SHOW
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace base {
namespace {

thread_local boost::uuids::random_generator kRndEngine;

}  // namespace

std::string GenerateUUID() {
  auto uuids = kRndEngine();
  std::string str;
  CHECK(boost::conversion::try_lexical_convert<std::string>(uuids, str));
  return str;
}

}  // namespace base
