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

TEST_CASE("global: get", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  global_1 = 100;
  {
    let i = 0;
    let result = false;
    for (i = 0; i < 10000; ++i) {
      result = global_1 == 100;
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("global: set", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  global_1 = 100;
  {
    let i = 0;
    let result = false;
    for (i = 0; i < 10000; ++i) {
      global_1 = i;
    }
    global_return = global_1 == 9999;
  }
)""",
      true));
}

TEST_CASE("global: bug", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

	global_1 = 20;
	global_2 = 2;
	global_3 = 20;
	global_d = "hello world: ";


	{
		let result = "";
		for (let i = 0; i < 15; i++) {
			result = global_d + (global_1 + global_2 + global_3);
		}
		global_return = (result == "hello world: 42");
	}

)""",
      true));
}

}  // namespace quake
