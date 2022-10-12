#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <template.h>

namespace quake {

TEST_CASE("basic function", "[template1]") {
  {
    std::string data = R"""()""";  // empty input
    auto r = ParseTemplate(data);
    REQUIRE(r);
    auto result = r.PassResult();
    REQUIRE(result.content.empty());
  }

  {
    std::string data = R"""(! this is comment)""";  // empty input
    auto r = ParseTemplate(data);
    REQUIRE(r);
    auto result = r.PassResult();
    REQUIRE(result.content.empty());
  }

  {
    std::string data = R"""(%xx
%xx)""";  // empty section
    auto r = ParseTemplate(data);
    if (!r) LOG(ERROR) << r.GetError().GetDisplay();
    REQUIRE(r);
    auto result = r.PassResult();
    REQUIRE(result.content.size() == 1);
    auto maybe = result.GetSection("xx");
    REQUIRE(maybe);
    REQUIRE(maybe->empty());
  }

  {
    std::string data = R"""(
%xx
%xx

%yy
%yy

%zz
%zz)""";

    auto r = ParseTemplate(data);
    REQUIRE(r);
    auto result = r.PassResult();
    REQUIRE(result.content.size() == 3);
    {
      auto maybe = result.GetSection("xx");
      REQUIRE(maybe);
      REQUIRE(maybe->empty());
    }
    {
      auto maybe = result.GetSection("yy");
      REQUIRE(maybe);
      REQUIRE(maybe->empty());
    }
    {
      auto maybe = result.GetSection("zz");
      REQUIRE(maybe);
      REQUIRE(maybe->empty());
    }
  }

  {
    std::string data = R"""(
!--------------------------
!a
!b
!--------------------------
%xx
abcd
%xx

!--------------------------
!a
!b
!--------------------------
%zz
efgh
%zz)""";

    auto r = ParseTemplate(data);
    REQUIRE(r);
    auto result = r.PassResult();
    REQUIRE(result.content.size() == 2);
    {
      auto maybe = result.GetSection("xx");
      REQUIRE(maybe);
      REQUIRE(*maybe == "abcd");
    }
    {
      auto maybe = result.GetSection("zz");
      REQUIRE(maybe);
      REQUIRE(*maybe == "efgh");
    }
  }
}

}  // namespace quake
