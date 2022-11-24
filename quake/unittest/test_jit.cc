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

TEST_CASE("jit2", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 100;
      let foo = function() {
        return 11.0;
      };

      let xx = foo();

      for (i = 0; i < 10000000; i++) {
        out += 20.0;
        out %= 3;
        out += foo();
      }

      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  INFO("type " << global_return.type());
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 11);
}

TEST_CASE("jit3", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 100;
      let foo = function(a, b) {
        return a + b;
      };

      let xx = foo();

      for (i = 0; i < 10000000; i++) {
        out += 20.0;
        out %= 3;
        out += foo(10.1, 0.9);
      }

      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  INFO("type " << global_return.type());
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 11);
}

#if 0

TEST_CASE("jit4", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 100;
      let foo = function(a, b) {
        return a + b;
      };

      let bar = new Object();
      bar.foo = function(a, b) {
        return a + b;
      };

      for (i = 0; i < 12; i++) {
        out += 20.0;
        out %= 3;
        out += bar.foo(10.1, 0.9);
      }

      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  INFO("type " << global_return.type());
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 11);
}

#endif

}  // namespace quake
