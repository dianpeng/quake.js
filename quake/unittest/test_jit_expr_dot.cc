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

TEST_CASE("expr: dot operator", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Object();
    obj.a = 100;
    for (i = 0; i < 100; ++i) {
      result = obj.a == 100;
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
    obj.a = 100;
    obj.b = 100;
    obj.c = 100;
    obj.d = 100;
    obj.e = 100; // slow path

    for (i = 0; i < 100; ++i) {
      result = obj.e == 100; // bailout from slow path
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
    for (i = 0; i < 100; ++i) {
      result = obj.f == undefined; // bailout from slow path
    }
    global_return = result;
  }
)""",
      true));
}

}  // namespace quake
