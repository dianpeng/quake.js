#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

TEST_CASE("jit1", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 100;
      for (i = 0; i < 10000000; i++) {
        out += 20.0;
        out %= 3;
        out += 11.0;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  INFO("type " << global_return.type());
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 11);
}

}  // namespace quake
