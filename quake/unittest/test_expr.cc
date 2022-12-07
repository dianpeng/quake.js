
#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

// ----------------------------------------------------------------------------
// basic arithmetics
const auto test0_src = R"""(
  {
    let i =  0; // index 1
    let out= 0; // index 2
    for (i = 0; i < 100; i++) {
      out += i;
    }
  }
)""";

TEST_CASE(test0_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 100.0)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 4950.0)}};

  REQUIRE(Test0(test0_src, std::move(local), {}));
}

const auto test1_src = R"""(
  {
    let i =  0; // index 1
    let out= 2; // index 2
    for (i = 0; i < 4; i++) {
      out *= 2;
    }
  }
)""";

TEST_CASE(test1_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 4.0)},
      {__JS_NewFloat64(nullptr, 2.0),
       __JS_NewFloat64(nullptr, 2.0 * 2.0 * 2.0 * 2.0 * 2.0)}};

  REQUIRE(Test0(test1_src, std::move(local), {}));
}

const auto test2_src = R"""(
  {
    let i =  0; // index 1
    let out= 32; // index 2
    for (i = 0; i < 4; i++) {
      out /= 2;
    }
  }
)""";

TEST_CASE(test2_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 4.0)},
      {__JS_NewFloat64(nullptr, 32.0), __JS_NewFloat64(nullptr, 2.0)}};

  REQUIRE(Test0(test2_src, std::move(local), {}));
}

const auto test3_src = R"""(
  {
    let i =  0; // index 1
    let out= 8; // index 2
    for (i = 0; i < 4; i++) {
      out -= 2;
    }
  }
)""";

TEST_CASE(test3_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 4.0)},
      {__JS_NewFloat64(nullptr, 8.0), __JS_NewFloat64(nullptr, 0.0)}};

  REQUIRE(Test0(test3_src, std::move(local), {}));
}

// fmod
const auto test4_src = R"""(
  {
    let i =  0; // index 1
    let out= 7; // index 2
    for (i = 0; i < 1; i++) {
      out %= 5;
    }
  }
)""";

TEST_CASE(test4_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 7.0), __JS_NewFloat64(nullptr, 2.0)}};

  REQUIRE(Test0(test4_src, std::move(local), {}));
}

// fpow
const auto test5_src = R"""(
  {
    let i =  0; // index 1
    let out= 4; // index 2
    for (i = 0; i < 1; i++) {
      out **= 4;
    }
  }
)""";

TEST_CASE(test5_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 4.0),
       __JS_NewFloat64(nullptr, 4.0 * 4.0 * 4.0 * 4.0)}};

  REQUIRE(Test0(test5_src, std::move(local), {}));
}

const auto test_x0_src = R"""(
  {
    let i =  0; // index 1
    let out= 4; // index 2
    let bit = 2; // index 3
    for (i = 0; i < 1; i++) {
      out = bit;
    }
  }
)""";

TEST_CASE(test_x0_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 4.0), __JS_NewFloat64(nullptr, 2.0)},
      {__JS_NewFloat64(nullptr, 2.0)}};

  REQUIRE(Test0(test_x0_src, std::move(local), {}));
}

const auto test_x1_src = R"""(
  {
    let out = 0;
    for (let i = 0; i < 1; i++) {
      out += 3;
      out *= 2;
      out -= 3;
      out /= 3;
    }
  }
)""";

TEST_CASE(test_x1_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 0.0)}};

  REQUIRE(Test0(test_x1_src, std::move(local), {}));
}

// ----------------------------------------------------------------------------
// Bit operations
//
// SHL
const auto test6_src = R"""(
  {
    let i =  0; // index 1
    let out= 4; // index 2
    for (i = 0; i < 1; i++) {
      out <<= 2;
    }
  }
)""";

TEST_CASE(test6_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 4.0), __JS_NewFloat64(nullptr, 4.0 * 2)}};

  REQUIRE(Test0(test6_src, std::move(local), {}));
}

const auto test7_src = R"""(
  {
    let i =  0; // index 1
    let out= 4; // index 2
    let bit = 2; // index 3
    for (i = 0; i < 1; i++) {
      out <<= bit;
    }
  }
)""";

TEST_CASE(test7_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 4.0), __JS_NewFloat64(nullptr, 4.0 * 2)},
      {__JS_NewFloat64(nullptr, 2.0)}};

  REQUIRE(Test0(test7_src, std::move(local), {}));
}

// SHR
const auto test8_src = R"""(
  {
    let i =  0; // index 1
    let out= 4; // index 2
    for (i = 0; i < 1; i++) {
      out >>= 1;
    }
  }
)""";

TEST_CASE(test8_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 4.0), __JS_NewFloat64(nullptr, 4.0 / 2)}};

  REQUIRE(Test0(test8_src, std::move(local), {}));
}

const auto test9_src = R"""(
  {
    let i =  0; // index 1
    let out= 4; // index 2
    let bit = 1; // index 3
    for (i = 0; i < 1; i++) {
      out >>= bit;
    }
  }
)""";

TEST_CASE(test9_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 4.0), __JS_NewFloat64(nullptr, 4.0 / 2)},
      {__JS_NewFloat64(nullptr, 1.0)}};

  REQUIRE(Test0(test9_src, std::move(local), {}));
}

const auto test10_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      out &= 2;
    }
  }
)""";

TEST_CASE(test10_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 2.0)},
      {__JS_NewFloat64(nullptr, 1.0)}};

  REQUIRE(Test0(test10_src, std::move(local), {}));
}

const auto test11_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      out ^= 3;
    }
  }
)""";

TEST_CASE(test11_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1.0)}};

  REQUIRE(Test0(test11_src, std::move(local), {}));
}

const auto test12_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    let bit = 2; // index3
    for (i = 0; i < 1; i++) {
      out &= bit;
    }
  }
)""";

TEST_CASE(test12_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 2.0)},
      {__JS_NewFloat64(nullptr, 2.0)}};

  REQUIRE(Test0(test12_src, std::move(local), {}));
}

const auto test13_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    let bit = 3; // index3
    for (i = 0; i < 1; i++) {
      out ^= bit;
    }
  }
)""";

TEST_CASE(test13_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 3.0)}};

  REQUIRE(Test0(test13_src, std::move(local), {}));
}

const auto test14_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      out++;
    }
  }
)""";

TEST_CASE(test14_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 3.0)}};

  REQUIRE(Test0(test14_src, std::move(local), {}));
}

const auto test15_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      out--;
    }
  }
)""";

TEST_CASE(test15_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 1.0)}};

  REQUIRE(Test0(test15_src, std::move(local), {}));
}

const auto test16_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      ++out;
    }
  }
)""";

TEST_CASE(test16_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 3.0)}};

  REQUIRE(Test0(test16_src, std::move(local), {}));
}

const auto test17_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      --out;
    }
  }
)""";

TEST_CASE(test17_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 1.0)}};

  REQUIRE(Test0(test17_src, std::move(local), {}));
}

const auto test18_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      out = -out;
    }
  }
)""";

TEST_CASE(test18_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, -2.0)}};

  REQUIRE(Test0(test18_src, std::move(local), {}));
}

const auto test19_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 1; i++) {
      out = +out;
    }
  }
)""";

TEST_CASE(test19_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 1.0)},
      {__JS_NewFloat64(nullptr, 2.0), __JS_NewFloat64(nullptr, 2.0)}};

  REQUIRE(Test0(test19_src, std::move(local), {}));
}

const auto test20_src = R"""(
  {
    let i = 0;   // index1
    let out = 2; // index2
    for (i = 0; i < 2; i++) {
      out = !out;
    }
  }
)""";

TEST_CASE(test20_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 2.0)},
      {__JS_NewFloat64(nullptr, 2.0), JS_TRUE}};

  REQUIRE(Test0(test20_src, std::move(local), {}));
}

// ----------------------------------------------------------------------------
// manual loop, comparison testify, ie whether the comparison opcode works
// as expected or not
const auto test21_src = R"""(
  {
    let i = 0;
    while (true) {
      if (i >= 10) break;
      i++;
    }
  }
)""";

TEST_CASE(test21_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 10.0)}};

  REQUIRE(Test0(test21_src, std::move(local), {}));
}

const auto test22_src = R"""(
  {
    let i = 20;
    while (true) {
      if (i < 10) break;
      i--;
    }
  }
)""";

TEST_CASE(test22_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 20.0), __JS_NewFloat64(nullptr, 9.0)}};

  REQUIRE(Test0(test22_src, std::move(local), {}));
}

const auto test23_src = R"""(
  {
    let i = 0;
    while (true) {
      if (i > 10) break;
      i++;
    }
  }
)""";

TEST_CASE(test23_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 11.0)}};

  REQUIRE(Test0(test23_src, std::move(local), {}));
}

const auto test24_src = R"""(
  {
    let i = 20;
    while (true) {
      if (i <= 10) break;
      i--;
    }
  }
)""";

TEST_CASE(test24_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 20.0), __JS_NewFloat64(nullptr, 10.0)}};

  REQUIRE(Test0(test24_src, std::move(local), {}));
}

// ----------------------------------------------------------------------------
// manual loop, notes the difference between for loop for how it is deoptimized
// from the jitted frame
const auto test34_src = R"""(
  {
    let i =  0; // index 1
    let out= 0; // index 2
    while (true) {
      out += i;
      i += 1;
      if (i >= 100) break;
    }
  }
)""";

TEST_CASE(test34_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 99.0)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 4851.0)}};

  REQUIRE(Test0(test34_src, std::move(local), {}));
}

}  // namespace quake
