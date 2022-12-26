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

TEST_CASE("unary: delete operator", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Object();
    for (i = 0; i < 14; ++i) {
      obj.a = 100;
      delete obj.a;
      result = obj.a == undefined;
    }
    global_return = result;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Object();
    let has_delete = false;
    global_xxx = 1;

    for (i = 0; i < 15; ++i) {
      if (!has_delete) {
        has_delete = true;
        delete global_xxx;
      }
    }

    try {
      let u = global_xxx;
    } catch (e) {
      global_return = true;
    }
  }
)""",
      true));
}

TEST_CASE("unary: instanceof operator", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    class A {}
    let a = new A();
    let i = 0;
    let result = false;
    for (i = 0; i < 1000; ++i) {
      result = a instanceof A;
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("unary: typeof operator", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let a = 1;
    let i = 0;
    let result = false;
    for (i = 0; i < 1000; ++i) {
      result = (typeof a) == "number";
    }
    global_return = result;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  {
    let a = '';
    let i = 0;
    let result = false;
    for (i = 0; i < 1000; ++i) {
      result = (typeof a) == "string";
    }
    global_return = result;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  {
    let a = true;
    let i = 0;
    let result = false;
    for (i = 0; i < 1000; ++i) {
      result = (typeof a) == "boolean";
    }
    global_return = result;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  {
    let a = undefined;
    let i = 0;
    let result = false;
    for (i = 0; i < 1000; ++i) {
      result = (typeof a) == "undefined";
    }
    global_return = result;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  {
    let a = null;
    let i = 0;
    let result = false;
    for (i = 0; i < 1000; ++i) {
      result = (typeof a) == "object";
    }
    global_return = result;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  {
    let a = new Object();
    let i = 0;
    let result = false;
    for (i = 0; i < 1000; ++i) {
      result = (typeof a) == "object";
    }
    global_return = result;
  }
)""",
      true));
}

}  // namespace quake
