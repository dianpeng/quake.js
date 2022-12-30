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
    for (i = 0; i < 10000; ++i) {
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

    for (i = 0; i < 10000; ++i) {
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
    for (i = 0; i < 10000; ++i) {
      result = obj.f == undefined; // bailout from slow path
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = [1, 2, 3];
    for (i = 0; i < 10000; ++i) {
      result = (obj[0] == 1) && (obj[1] == 2) && (obj[2] == 3);
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
    let obj = [1, 2, 3];
    for (i = 0; i < 10000; ++i) {
      result = (obj[4] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

// --------------------------------------------------------------------------
// different specialization of array access
// --------------------------------------------------------------------------

TEST_CASE("expr: [] operator: int8", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Int8Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4);
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
    let obj = new Int8Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator: uint8", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Uint8Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4);
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
    let obj = new Uint8Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator: int16", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Int16Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4);
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
    let obj = new Int16Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator: uint16", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Uint16Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4);
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
    let obj = new Uint16Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator: int32", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Int32Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4);
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
    let obj = new Int32Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator: uint32", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Uint32Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4);
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
    let obj = new Uint32Array(8);
    obj[5] = 4;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator: float32", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Float32Array(8);
    obj[5] = 4.0;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4.0);
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
    let obj = new Float32Array(8);
    obj[5] = 4.0;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: [] operator: float64", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Float64Array(8);
    obj[5] = 4.0;
    for (i = 0; i < 10000; ++i) {
      result = (obj[5] == 4.0);
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
    let obj = new Float64Array(8);
    obj[5] = 4.0;
    for (i = 0; i < 10000; ++i) {
      result = (obj[10] == undefined);
    }
    global_return = result;
  }
)""",
      true));
}

TEST_CASE("expr: dot operator set", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = new Object();
    for (i = 0; i < 10000; ++i) {
      obj.a = 100;
      obj.b = -100;
      obj.c = 200;
    }
    global_return = (obj.a + obj.b + obj.c) == 200;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  {
    let i = 0;
    let result = false;
    let obj = [];
    for (i = 0; i < 10000; ++i) {
      obj[0] = 100;
      obj[1] = -100;
      obj[2] = 200;
    }
    global_return = (obj[0] + obj[1] + obj[2]) == 200;
  }
)""",
      true));
}

}  // namespace quake
