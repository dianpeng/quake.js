// This include is needed when you find the catch's own CHECK collides with our
// CHECK macro during unittests compilation. If so, just use this header. In
// general cases, we never use catch2's CHECK

#define CATCH_CONFIG_PREFIX_ALL
#include <catch.hpp>

#define REQUIRE CATCH_REQUIRE
#define REQUIRE_FALSE CATCH_REQUIRE_FALSE
#define TEST_CASE CATCH_TEST_CASE
#define INFO CATCH_INFO

// ---------------------------------------------------------------------
// Following MACRO definition is used to adapt catch2.hpp as gtest

#ifdef CATCH_DISABLE_GTEST
#else

#define ASSERT_TRUE REQUIRE
#define ASSERT_FALSE REQUIRE_FALSE
#define ASSERT_EQ(A, B) REQUIRE((A) == (B))
#define ASSERT_NE(A, B) REQUIRE((A) != (B))
#define ASSERT_GT(A, B) REQUIRE((A) > (B))
#define ASSERT_GE(A, B) REQUIRE((A) >= (B))
#define ASSERT_LT(A, B) REQUIRE((A) < (B))
#define ASSERT_LE(A, B) REQUIRE((A) <= (B))

/** Bunch of helpers to deal with Sparrow related natives. Notes, we learn the
 ** MUST while we are writing the unit test, so the code may be inconsistent
 ** for asserting things like error or result
 **/

#endif  // CATCH_DISABLE_GTEST
