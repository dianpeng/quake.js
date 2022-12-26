#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

/**
 * Comparison
 *
 * We have 4 comparison
 *   1. Int
 *   2. Real
 *   3. Str
 *   4. General
 */

bool VerifyInt(const char* code, std::int32_t value) {
  QJSFunctionBytecode qjs{{}, {}};
  qjs.SetUp();
  qjs.Compile(code);
  auto global_return = qjs.Run();
  if (global_return.IsInt()) {
    return global_return.ToInt() == value;
  }
  return false;
}

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

TEST_CASE("expr: eq", "[jit]") {
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0;
    let rhs = 1;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0;
    let rhs = 0.7;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 1;
    let rhs = 0.7;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     false));
}

TEST_CASE("expr: eq_str", "[jit]") {
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'abcdefz';
    let rhs = 'abcdefz';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     true));
}

TEST_CASE("expr: ne", "[jit]") {
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 1;
    let rhs = 1;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs != rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0;
    let rhs = 0.7;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs != rhs;
    }
    global_return = value;
  }
  )""",
                     true));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0.6;
    let rhs = 0.7;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs != rhs;
    }
    global_return = value;
  }
  )""",
                     true));
}

TEST_CASE("expr: ne_str", "[jit]") {
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'abcdefg';
    let rhs = 'abcdefz';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs != rhs;
    }
    global_return = value;
  }
  )""",
                     true));
}

}  // namespace quake
