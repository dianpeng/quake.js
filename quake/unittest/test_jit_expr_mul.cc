#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

TEST_CASE("expr: basic mul, inside of the integer range", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1;
      for (i = 0; i < 10000; i++) {
        out *= 2;
				out -= 1;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsInt());
  REQUIRE(global_return.ToInt() == 1);
}

TEST_CASE("expr: basic mul, inside of the integer range (2)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1;
      for (i = 0; i < 10000; i++) {
				out *= 2;
				out *= 3;
				out *= 4;
				out -= 2*3*4-1;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsInt());
  REQUIRE(global_return.ToInt() == 1);
}

TEST_CASE("expr: mul casting, bool(true)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1.5;
      let value = true;
      for (i = 0; i < 10000; i++) {
        out *= value;
        out *= 2.0;
				out -= 1.5;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 1.5);
}

TEST_CASE("expr: mul casting, bool(false)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1.5;
      let value = false;
      for (i = 0; i < 10000; i++) {
        out *= value;
        out += 1.5;
        out *= 2.0;
				out -= 1.5;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 1.5);
}

TEST_CASE("expr: mul casting, null", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1.5;
      let value = null;;
      for (i = 0; i < 10000; i++) {
        out *= value;
        out += 1.5;
        out *= 2.0;
				out -= 1.5;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 1.5);
}

TEST_CASE("expr: mul casting, undefined", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1.5;
      let value = undefined;;
      for (i = 0; i < 10000; i++) {
        out *= value;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == NAN);  // must be a NaN
}

TEST_CASE("expr: basic mul, float64", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1.5;
      for (i = 0; i < 10000; i++) {
        out *= 2.0;
				out -= 1.5;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 1.5);
}

TEST_CASE("expr: mul overflow (int)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2;
      for (i = 0; i < 31; i++) {
				out *= 2;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 4294967296.0);
}

TEST_CASE("expr: mul overflow (int, negative)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = -2;
      for (i = 0; i < 31; i++) {
				out *= 2;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == -4294967296.0);
}

}  // namespace quake
