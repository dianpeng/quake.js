#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <jit.h>
#include "test0.h"

namespace quake {

/**
 * QuickJS's division will always work on floating point, ie its operand will
 * become floating point number regardlessly. In our implementation, we do
 * not repace the original operand's type but convert to floating point on
 * the fly when apply operation.
 */

TEST_CASE("expr: div, basic float", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2;
      for (i = 0; i < 10000; i++) {
        out /= 2;
        out *= 2;
			}
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 2.0);
}

TEST_CASE("expr: div, 0", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2;
      for (i = 0; i < 10000; i++) {
        out /= 0; // INF
        out *= 2; // NAN
			}
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == std::numeric_limits<double>::infinity());
}

TEST_CASE("expr: div undefined", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2;
      let value = undefined;
      for (i = 0; i < 15; i++) {
        out /= value; // NAN
        out *= 2;     // NAN
			}
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(std::isnan(global_return.ToFloat64()));
}

TEST_CASE("expr: div boolean(true)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2;
      let value = true;
      for (i = 0; i < 15; i++) {
        out /= value; // identity
        out /= 2;     // 1
        out *= 2;     // back to origin
			}
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 2.0);
}

TEST_CASE("expr: div boolean(true, literal)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2;
      for (i = 0; i < 15; i++) {
        out *= true; // identity
        out /= 2;    // 1
        out *= 2;    // back to origin
			}
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 2.0);
}

}  // namespace quake

