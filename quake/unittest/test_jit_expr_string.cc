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

TEST_CASE("expr: function call", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
	{
		let result = "";
		let array = ["a", "b", "c", "d"];

		for (let i = 0; i < 20; i++) {
      result = "";
			result += array[0];
			result += array[1];
			result += array[2];
			result += array[3];
		}
		global_return = (result === "abcd");
	}
)""",
      true));
}

}  // namespace quake
