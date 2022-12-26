#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

TEST_CASE("expr: neg, inside of integer range", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1;
      for (i = 0; i < 10000; i++) {
        out = -out;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsInt());
  REQUIRE(global_return.ToInt() == 1);
}

TEST_CASE("expr: neg, floating point", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1.5;
      for (i = 0; i < 10000; i++) {
        out = -out;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 1.5);
}

TEST_CASE("expr: neg, out of range", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2147483635;
      let value = 0;
      for (i = 0; i < 15; i++) {
        out++;
        value = -out;
      }
      global_return = value;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == -2147483650.0);
}

}  // namespace quake
