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
	  let result = 0;
		for (let i = 0; i < 100; i++) {
			result = {'x': 42}.x;
		}
    global_return = result == 42;
  }
)""",
      true));
}

}  // namespace quake
