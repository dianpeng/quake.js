#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

bool VerifyBool(const char* code, bool value) {
  QJSFunctionBytecode qjs{{}, {}};
  qjs.SetUp();
  qjs.Compile(code);
  auto global_return = qjs.Run();
  if (global_return.IsBool()) {
    return global_return.ToBool() == value;
  }
  return false;
}

TEST_CASE("expr: and", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
  {
    let i = 0;
    let result = false;
		let value = 1024;
    for (i = 0; i < 10000; ++i) {
      result = (value & (value-1));
    }
    global_return = result == 0;
  }
)""",
      true));
}

TEST_CASE("expr: or", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
  {
    let i = 0;
    let result = false;
		let value = 1024;
    for (i = 0; i < 10000; ++i) {
      result = (value | (value-1));
    }
    global_return = ((1024*2-1) == result);
  }
)""",
      true));
}

TEST_CASE("expr: xor", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
  {
    let i = 0;
    let result = false;
		let value = 1024;
    for (i = 0; i < 10000; ++i) {
      result = (value ^ (value-1));
    }
    global_return = ((1024*2-1) == result);
  }
)""",
      true));
}

TEST_CASE("expr: shift", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
  {
    let i = 0;
    let result = 1;
    for (i = 0; i < 15; ++i) {
      result = result << 1;
    }
    global_return = result == 32768;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
  {
    let i = 0;
    let result = 1;
    let shift = 1;
    for (i = 0; i < 15; ++i) {
      result = result << shift;
    }
    global_return = result == 32768;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
  {
    let i = 0;
    let result = 32768;
    for (i = 0; i < 15; ++i) {
      result = result >> 1;
    }
    global_return = result == 1;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
  {
    let i = 0;
    let result = 32768;
    let shift = 1;
    for (i = 0; i < 15; ++i) {
      result = result >> shift;
    }
    global_return = result == 1;
  }
)""",
      true));
}

}  // namespace quake
