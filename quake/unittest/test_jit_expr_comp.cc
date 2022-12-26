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

/** ========================================================================
 * Equalism Comparison
 ** ======================================================================*/

TEST_CASE("comp[=|!=]: null + undefined comparison", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     false));
}

TEST_CASE("comp[=|!=]: any X undefined comparison", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = "";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = [];
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = new Object(); 
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = "";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = [];
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = new Object(); 
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));
}

TEST_CASE("comp[=|!=]: null X undefined comparison", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = "";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = [];
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = new Object(); 
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = "";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = [];
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = new Object(); 
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));
}

// ----------------------------------------------------------------------------
// general case, ie fallback to runtime call
TEST_CASE("comp[=|!=]: any + any comparison", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = 100;
  let rhs = new Object(); 
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs == rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = 100;
  let rhs = new Object(); 
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs != rhs;
  }
  global_return = value;
}

)""",
                     true));
}

/** ========================================================================
 * None Equalism Comparison
 ** ======================================================================*/
TEST_CASE("comp[<,<=,>,>=]: undefined/null", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs >= rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs <= rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs > rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = undefined;
  let rhs = undefined;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs < rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs >= rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs <= rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs > rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = null;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs < rhs;
  }
  global_return = value;
}

)""",
                     false));
}

TEST_CASE("comp[<,<=,>,>=]: int + null", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs >= rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs > rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs <= rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = 1;
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs < rhs;
  }
  global_return = value;
}

)""",
                     true));
}

TEST_CASE("comp[<,<=,>,>=]: string + null", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = "1";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs >= rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = "1";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs > rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = "1";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs <= rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = "1";
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs < rhs;
  }
  global_return = value;
}

)""",
                     true));
}

TEST_CASE("comp[<,<=,>,>=]: object + null", "[jit]") {
  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = new Object();
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs >= rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = new Object();
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs > rhs;
  }
  global_return = value;
}

)""",
                     false));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = [];
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs <= rhs;
  }
  global_return = value;
}

)""",
                     true));

  REQUIRE(VerifyBool(R"""(
{
  let i = 0;
  let lhs = null;
  let rhs = [];
  let value = false;

  for (i = 0; i < 1000; i++) {
    value = lhs < rhs;
  }
  global_return = value;
}

)""",
                     false));
}

}  // namespace quake
