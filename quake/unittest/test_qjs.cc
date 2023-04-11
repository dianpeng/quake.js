#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>
#include "qjs.h"

namespace quake {

TEST_CASE("f64tob32", "[runtime]") {
  {
    double value = 21474836476.0;
    const auto b32 = static_cast<std::uint32_t>(JVal::Float64ToBit32(value));
    LOG(ERROR) << b32;
    LOG(ERROR) << (b32 >> 31);
  }
}

}  // namespace quake
