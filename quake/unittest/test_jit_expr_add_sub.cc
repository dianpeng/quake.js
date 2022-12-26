#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

// The assumption of JIT triggering is 10, and we should set it up before we
// start to run the JIT compiler test suits. Since 10 is nearly impossible to
// be as a valid default for our jit compiler

#include "test0.h"

namespace quake {

TEST_CASE("expr: basic add and sub, inside of the integer range", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      for (i = 0; i < 10000; i++) {
        out += i;
				out -= i;
				out = i;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 10000 - 1);
}

TEST_CASE("expr: basic add and sub, floating point", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1.5;
      let vv = 1; // this is an integer

      for (i = 0; i < 10000; i++) {
        out += i;
				out -= i;
				out = 1.5;  // floating point
        out += 0.5; // add float
        out += 1;   // cast 1 to floating point number and add it
        out += vv;  // cast vv to floating point number and add it
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 4.0);
}

TEST_CASE("expr: basic add and sub, floating x int", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 1; // this is an integer
      let vv = 1;  // this is an integer

      for (i = 0; i < 10000; i++) {
        out += i;
				out -= i;
        out += 0.5; // add float
        out += 1;   // add another int
        out += 0.5; // add another float
        out -= 2;   // out is 1.0 float
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 1.0);
}

// misc
TEST_CASE("expr: bool conversion (bool + int)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let boolvalue = true;

      for (i = 0; i < 10000; i++) {
        out += i;
				out -= i;
        out += boolvalue;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 10000.0);
}

TEST_CASE("expr: bool conversion (bool + float)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0.5;
      let boolvalue = true;

      for (i = 0; i < 10000; i++) {
        out += i;
				out -= i;
        out += boolvalue;
        out -= boolvalue;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 0.5);
}

TEST_CASE("expr: null conversion (bool + float)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0.5;
      let boolvalue = true;

      for (i = 0; i < 10000; i++) {
        out += i;
				out -= i;
        out += boolvalue;
        out -= boolvalue;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 0.5);
}

TEST_CASE("expr: add_overflow (bailout in the peel)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2147483634;
      for (i = 0; i < 15; i++) {
        out += 1;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 2147483649.0);
}

TEST_CASE("expr: add_overflow (bailout in the loop)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 2147483633;
      for (i = 0; i < 15; i++) {
        out += 1;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == 2147483648.0);
}

TEST_CASE("expr: sub_overflow (bailout in the peel)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = -2147483635;
      for (i = 0; i < 15; i++) {
        out -= 1;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == -2147483650.0);
}

TEST_CASE("expr: sub_overflow (bailout in the loop)", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = -2147483634;
      for (i = 0; i < 15; i++) {
        out -= 1;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  REQUIRE(global_return.IsFloat64());
  REQUIRE(global_return.ToFloat64() == -2147483649.0);
}

}  // namespace quake
