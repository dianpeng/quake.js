#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

TEST_CASE(
    "expr: inc, inside of integer range, the expression should return its "
    "current value",
    "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let value = 0;
      for (i = 0; i < 10000; i++) {
				value = ++out;
      }
      global_return = value;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsInt());
  REQUIRE(global_return.ToInt() == 10000);
}

TEST_CASE(
    "expr: post_inc, inside of integer range, the expression should return its "
    "previous value",
    "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let value = 0;
      for (i = 0; i < 10000; i++) {
				value = out++;
      }
      global_return = value;
    }
  )""");
  auto global_return = qjs.Run();

  // Notes, the post_inc/post_dec will return a floating point number based on
  // current VM implementation, the JIT will do the same thing
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToInt() == 10000 - 1);
}

TEST_CASE("expr: inc, float", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0.5;
      let value = 0;
      for (i = 0; i < 10000; i++) {
				value = ++out;
      }
      global_return = value;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 10000.5);
}

TEST_CASE("expr: post_inc, float", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0.5;
      let value = 0;
      for (i = 0; i < 10000; i++) {
				value = out++;
      }
      global_return = value;
    }
  )""");
  auto global_return = qjs.Run();

  // Notes, the post_inc/post_dec will return a floating point number based on
  // current VM implementation, the JIT will do the same thing
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 9999.5);
}

TEST_CASE("expr: inc out of range, ie bailout from jit", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2147483635;
      let value = 0;
      for (i = 0; i < 15; i++) {
				value = ++out;
      }
      global_return = value;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 2147483650.0);
}

TEST_CASE("expr: post_inc out of range, ie bailout from jit", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2147483635;
      let value = 0;
      for (i = 0; i < 15; i++) {
				value = out++;
      }
      global_return = value;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 2147483650.0 - 1.0);
}

}  // namespace quake
