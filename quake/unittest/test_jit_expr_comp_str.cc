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

TEST_CASE("expr[NaN]: string can lead to NaN", "[jit]") {
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'a';
    let rhs = 0;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs < rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0;
    let rhs = 'a';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs < rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'a';
    let rhs = 0;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs <= rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0;
    let rhs = 'a';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs <= rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'a';
    let rhs = 0;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs > rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0;
    let rhs = 'a';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs > rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'a';
    let rhs = 0;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs >= rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 0;
    let rhs = 'a';
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
    let lhs = 'a';
    let rhs = 0;
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
    let lhs = 0;
    let rhs = 'a';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs >= rhs;
    }
    global_return = value;
  }
  )""",
                     false));
}

TEST_CASE("expr[=|!=]: string", "[jit]") {
  // both operand are strings
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'a';
    let rhs = 'a';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     true));

  // one of the operand is integer and the other string can be converted to
  // string
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = '1';
    let rhs = 1;
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     true));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 1;
    let rhs = '1';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     true));

  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = '10';
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
    let lhs = 1;
    let rhs = '10';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  // one of the operand is integer and the other string can NOT be converted to
  // integer. The conversion result will be NaN
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 'aab';
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
    let lhs = 1;
    let rhs = 'aabb';
    let value = false;
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  // string comparison fallback, ie one of them is string and the other is an
  // object
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 1;
    let rhs = 'aabb';
    let value = [];
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     false));

  // string comparison fallback, ie one of them is string and the other is an
  // object
  REQUIRE(VerifyBool(R"""(
  {
    let i = 0;
    let lhs = 1;
    let rhs = [];
    let value = 'abcdef';
    for (i = 0; i < 1000; i++) {
      value = lhs == rhs;
    }
    global_return = value;
  }
  )""",
                     false));
}

}  // namespace quake
