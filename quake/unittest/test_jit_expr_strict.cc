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

TEST_CASE("comp: strict eq/ne ptr", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = new Object();
      let rhs = new Object();
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = new Object();
      let rhs = lhs;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === rhs;
      }
      global_return = result;
    }
)""",
      true));
}

TEST_CASE("comp: strict eq/ne int", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === 1;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = 1 === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = 1 === 1;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === true;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = true === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = true === true;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== 1;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = 1 !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = 1;
      let rhs = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = 1 !== 1;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== true;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = true !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = true;
      let rhs = true;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = true !== true;
      }
      global_return = result;
    }
)""",
      false));
}

TEST_CASE("comp: strict eq null/undefined", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === undefined;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = undefined === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = undefined === undefined;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = null;
      let rhs = null;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = null;
      let rhs = null;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs === null;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = null;
      let rhs = null;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = null === rhs;
      }
      global_return = result;
    }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = null === null;
      }
      global_return = result;
    }
)""",
      true));
}

TEST_CASE("comp: strict ne null/undefined", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== undefined;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = undefined !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = undefined;
      let rhs = undefined;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = undefined !== undefined;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = null;
      let rhs = null;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = null;
      let rhs = null;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = lhs !== null;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let lhs = null;
      let rhs = null;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = null !== rhs;
      }
      global_return = result;
    }
)""",
      false));

  REQUIRE(VerifyBool(
      R"""(
    {
      let i = 0;
      let out = 1;
      let result = false;
      for (i = 0; i < 10000; i++) {
        result = null !== null;
      }
      global_return = result;
    }
)""",
      false));
}

}  // namespace quake
