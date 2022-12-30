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

TEST_CASE("loop condition: using array's length, this will casue bug",
          "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
	{
		let sum = 0;

		for (let i = 0; i < 100; ++i) {
			sum += 2;
		}
		global_return = (sum == 200);
	}
  )""",
      true));

  REQUIRE(VerifyBool(
      R"""(
	function gen_array(cnt) {
		let arr = [];
		for (let i = 0; i < cnt; i++) {
			arr[i] = 2;
		}
		return arr;
	}

	{
		const arr = gen_array(100);
		let sum = 0;

		for (let i = 0; i < arr.length; ++i) {
			sum += arr[i];
		}
		global_return = (sum == 200);
	}
  )""",
      true));
}

}  // namespace quake
