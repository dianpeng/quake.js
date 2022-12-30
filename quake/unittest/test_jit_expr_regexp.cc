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

  let result = 0;
  let rhs = "abbbc";
  for (let i = 0; i < 1000; i++) {
    let pattern = /ab+c/;
  }
	global_return = true;
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  const xxx = {
    foo : (x) => {
      return x[0];
    }
  };

	{
		const a = /ab+c/;
		let result = false;
    const rhs = "abbbbc";
		for (let i= 0 ; i < 17; i++) {
			result = rhs.match(a).length != 0;
		}
    global_return = result;
	}
)""",
      true));
}

}  // namespace quake
