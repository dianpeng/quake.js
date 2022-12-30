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

  function foo0() {
    return 42;
  }
  function foo1(a) {
    return a * 2;
  }
  function foo2(a, b) {
    return a + b;
  }

  {
    let i = 0;
    let result = false;
    let obj = new Object();
    obj.a = 100;
    for (i = 0; i < 10000; ++i) {
      result = foo0() + foo1(1) + foo2(2, 3);
    }
    global_return = result == (42+1*2+2+3);
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
  function foo(a, b) {
    return a + b;
  }

  {

		let i = 0;
		let result = 0;
    let arg = [1, 2];
    let y, z;

		for (i = 0; i < 14; i++) {
			result = foo(arg[0], arg[1]);
		}

    global_return = result == 3;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  function foo(a) {
    return a.a + a.b;
  }

  {
    let i = 0;
    let result = false;
    let obj = new Object();
    obj.a = 1;
    obj.b = 2;

    for (i = 0; i < 10000; ++i) {
      result = foo(obj);
    }
    global_return = result == 3;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  function foo(a, b) {
    return a.a + b.b;
  }

  {
    let i = 0;
    let result = false;

    let a = new Object();
    a.a = 1;

    let b = new Object();
    b.b = 2;

    for (i = 0; i < 15; ++i) {
      result = foo(a, b);
    }
    global_return = result == 3;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(

  function foo(a, b) {
    return a + b;
  }

  {
    let i = 0;
    let result = false;

    let a = new Object();
    a.a = 1;

    let b = new Object();
    b.b = 2;

    for (i = 0; i < 15; ++i) {
      result = foo(a.a, b.b);
    }

    global_return = result == 3;
  }
)""",
      true));
}

TEST_CASE("expr: method call", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  class A {
    foo0() {
      return 42;
    }
    foo1(a) {
      return a * 2;
    }
    foo2(a, b) {
      return a + b;
    }
  }


  {
    let i = 0;
    let result = false;
    let obj = new A();
    for (i = 0; i < 10000; ++i) {
      result = obj.foo0() + obj.foo1(1) + obj.foo2(2, 3);
    }
    global_return = result == (42+1*2+2+3);
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
  {
		const person = {
			fullName: {
        bar : function(a, b) {
          return a + b;
        }
      }
		}

		const person1 = {
			firstName: 1,
			lastName: 2
		}

		let i = 0;
		let result = 0;

		for (i = 0; i < 10000; i++) {
			result = person.fullName.bar(1, 2);
		}

    global_return = result == 3;
  }
)""",
      true));
}

TEST_CASE("expr: method call with argument as object", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  class A {
    foo(a) {
      return a.a + a.b;
    }
  }


  {
    let i = 0;
    let result = false;
    let a = new A();
    let obj = new Object();
    obj.a = 1;
    obj.b = 2;
    for (i = 0; i < 10000; ++i) {
      result = a.foo(obj);
    }
    global_return = result == 3;
  }
)""",
      true));

  REQUIRE(VerifyBool(
      R"""(
  {
		const person = {
			fullName: function() {
        return this.firstName + this.lastName;
      }
		}

		const person1 = {
			firstName: 1,
			lastName: 2
		}

		let i = 0;
		let result = 0;

		for (i = 0; i < 15; i++) {
			result = person.fullName.apply(person1);
		}

    global_return = result == 3;
  }
)""",
      true));
}

TEST_CASE("expr: ctor", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(

  class A {
    constructor(a, b, c) {
      this.a = a;
      this.b = b;
      this.c = c;
    }
  }

  {
    let i = 0;
    let result = false;
    let obj = new A();
    for (i = 0; i < 10000; ++i) {
      result = new A(1, 2, 3);
    }
    global_return = result.a == 1 && result.b == 2 && result.c == 3;
  }
)""",
      true));
}

TEST_CASE("expr: apply", "[jit]") {
  REQUIRE(VerifyBool(
      R"""(
  function foo(a, b) {
    return a+b;
  }

  {
		let i = 0;
		let result = 0;
    let arg = [1, 2];
		for (i = 0; i < 100; i++) {
			result = foo(...arg);
		}
    global_return = result == 3;
  }
)""",
      true));
}

}  // namespace quake
