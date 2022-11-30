#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

TEST_CASE("jit1", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 100;
      for (i = 0; i < 10000000; i++) {
        out += 20.0;
        out %= 3;
        out += 11.0;
      }
      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  INFO("type " << global_return.type());
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 11);
}

TEST_CASE("jit2", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 100;
      let foo = function() {
        return 11.0;
      };

      let xx = foo();

      for (i = 0; i < 10000000; i++) {
        out += 20.0;
        out %= 3;
        out += foo();
      }

      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  INFO("type " << global_return.type());
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 11);
}

TEST_CASE("jit3", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 100;
      let foo = function(a, b) {
        return a + b;
      };

      let xx = foo();

      for (i = 0; i < 10000000; i++) {
        out += 20.0;
        out %= 3;
        out += foo(10.1, 0.9);
      }

      global_return = out;
    }
  )""");
  auto global_return = qjs.Run();
  INFO("type " << global_return.type());
  REQUIRE(global_return.IsScalarNumber());
  REQUIRE(global_return.ToInt() == 11);
}

TEST_CASE("jit4", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let foo = function(a, b) {
        return a + b;
      };

      let bar = new Object();
      bar.foo = 11.0;

      for (i = 0; i < 10000000; i++) {
        out += bar.foo;
      }

      global_return = out;
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 110000000);
  }
}

TEST_CASE("jit5", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let foo = function(a, b) {
        return a + b;
      };

      let bar = new Object();
      bar.foo = function(a, b) {
        return a + b;
      };

      for (i = 0; i < 10000000; i++) {
        out += bar.foo(10.1, 0.9);
      }

      global_return = out;
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 110000000);
  }
}


TEST_CASE("jit6", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let foo = function(a, b) {
        return a + b;
      };

      let bar = new Object();
      bar.foo = function(a, b) {
        return a + b;
      };

      for (i = 0; i < 10000000; i++) {
        out = i;
      }

      global_return = out;
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 10000000-1);
  }
}



TEST_CASE("jit7", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let x = 0;

      for (i = 0; i < 1000000; i++) {
        x = new Object();
        out = i;
      }

      global_return = out;
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 1000000-1);
  }
}


TEST_CASE("jit8", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let x = 0;

      for (i = 0; i < 1000000; i++) {
        x = new Object();
        x.y = i;
        out = x.y;
      }

      global_return = out;
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 1000000-1);
  }
}

TEST_CASE("jit9", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;
      let x = new Object();

      for (i = 0; i < 10000000; i++) {
        x.y = i;
        out = x.y;
      }

      global_return = out;
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 10000000-1);
  }
}


TEST_CASE("jit10", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;

      for (i = 0; i < 10000000; i++) {
        global_return = i;
      }
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 10000000-1);
  }
}


TEST_CASE("jit11", "[jit]") {
  QJSFunctionBytecode qjs{{}, {}};

  qjs.SetUp();
  qjs.Compile(R"""(
    {
      let i = 0;
      let out = 0;

      for (i = 0; i < 1000000; i++) {
        out = [1, 2, 3];
        global_return = i;
      }
    }
  )""");
  {
    auto global_return = qjs.Run();
    INFO("type " << global_return.type());
    REQUIRE(global_return.IsScalarNumber());
    REQUIRE(global_return.ToInt() == 1000000-1);
  }
}

}  // namespace quake
