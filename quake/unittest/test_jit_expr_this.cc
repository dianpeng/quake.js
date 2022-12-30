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

TEST_CASE("expr: this access", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  const obj = {
    'a': 2,
    'b': 40,
    'answer': function() {
      let v = 0;
      for (let i = 0; i < 100; i++) {
        v = this.a + this.b == 42;
      }
      return v === true;
    }
  };

  {
    global_return = obj.answer();
  }
)""",
      true));
  REQUIRE(VerifyBool(
      R"""(

  const obj = {
    'a': 1,
    'b': 40,
    'c': true,
    'd': null,
    'e': "hello world",
    'f': {
      'value': 42
    },
    'answer': function() {
      let v = 0;
      for (let i = 0; i < 100; i++) {
        v = this.a == 1              &&
            this.b == 40             &&
            this.c                   &&
            this.d === null          &&
            this.e === "hello world" &&
            this.f.value == 42;
      }

      return v === true;
    }
  };

  {
    global_return = obj.answer();
  }
)""",
      true));
}

}  // namespace quake
