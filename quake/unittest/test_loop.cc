#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include "test0.h"

namespace quake {

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
			o1 %= 1.0;
			o2 %= 2.0;
			o3 %= 3.0;
			o4 %= 4.0;
			o5 %= 5.0;
			o6 %= 6.0;
			o7 %= 7.0;
			o8 %= 8.0;

			o1 += 7.0;
			o2 += o1 + 11.0;
			o3 += o2 + 13.0;
			o4 += o3 + 14.0;
			o5 += o4 + 5.0;
			o6 += o5 + 3.0;
			o7 += o6 + 2.0;
			o8 += o7 + 2.0;
    }
  }
)""";

TEST_CASE(test1_src, "[exec]") {
  QJSFunctionBytecode::ValueMap local = {
      {__JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 0.0), __JS_NewFloat64(nullptr, 10000000.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)},
      {__JS_NewFloat64(nullptr, 1668418333272.0), __JS_NewFloat64(nullptr, 0.0)}};

  REQUIRE(Test0(test1_src, std::move(local), {}, true));
}

}  // namespace quake
