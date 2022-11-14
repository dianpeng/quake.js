#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

// ----------------------------------------------------------------------------
// The following call will call external function

const auto test1_src = R"""(
  {
    let i =  0; // index 1
    let o1 = 0; // index 2
    let o2 = 0; // index 3
    let o3 = 0; // index 4
    let o4 = 0; // index 5
    let o5 = 0; // index 6
    let o6 = 0; // index 7
    let o7 = 0; // index 8
    let o8 = 0; // index 9

    for (i = 0; i < 10000000; i++) {
      o1 += 1;
      o1 -= 1;

      o2 += 2;
      o2 -= 2;

      o3 += 3;
      o3 -= 3;

      o4 += 4;
      o4 -= 4;

      o5 += 5;
      o5 -= 5;

      o6 += 6;
      o6 -= 6;

      o7 += 7;
      o7 -= 7;

      o8 += 8;
      o8 -= 8;
    }
  }
)""";

TEST_CASE(test1_src, "[exec]") {
  struct Result {
    double o1 = 0.0, o2 = 0.0, o3 = 0.0, o4 = 0.0, o5 = 0.0, o6 = 0.0, o7 = 0.0,
           o8 = 0.0;
  };

  auto native = []() -> Result {
    Result r;

    auto o1 = 0;
    auto o2 = 0;
    auto o3 = 0;
    auto o4 = 0;
    auto o5 = 0;
    auto o6 = 0;
    auto o7 = 0;
    auto o8 = 0;

    for (auto i = 0; i < 1; i++) {
      o1 += 1;
      o1 += 1;

      o2 += 2;
      o2 += 2;

      o3 += 3;
      o3 += 3;

      o4 += 4;
      o4 += 4;

      o5 += 5;
      o5 += 5;

      o6 += 6;
      o6 += 6;

      o7 += 7;
      o7 += 7;

      o8 += 8;
      o8 += 8;
    }

    return Result{static_cast<double>(o1), static_cast<double>(o2),
                  static_cast<double>(o3), static_cast<double>(o4),
                  static_cast<double>(o5), static_cast<double>(o6),
                  static_cast<double>(o7), static_cast<double>(o8)};
  };

  auto rr = native();

  QJSFunctionBytecode::ValueMap local = {
      {JS_UNDEFINED},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 100.0)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o1)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o2)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o3)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o4)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o5)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o6)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o7)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, rr.o8)}};

  REQUIRE(Test0(test1_src, std::move(local), {}, true));
}

}  // namespace quake
