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
	function foo(i) {}

	{
		let sum = 0;
		for (let i = 0; i < 20; ++i) {
			foo(i);
			if (i % 2) continue; // this will not work on JIT since it will bailout
			sum += i;
		}
		global_return = sum == 90;
	}

)""",
      true));
}

}  // namespace quake

