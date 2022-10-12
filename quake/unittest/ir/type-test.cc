#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <ir/type.h>

namespace quake {
namespace ir {

TEST_CASE("basic type testing", "[type]") {
  {
    Type t;
    REQUIRE(t.IsNone());
  }
  {
    for (int i = Type::kNone; i < Type::kSizeOfTag; i++) {
      Type t{static_cast<Type::Tag>(i)};
      REQUIRE(t.tag() == i);
    }
  }
  {
    REQUIRE(Type{Type::kNone}.IsNone());
    REQUIRE(Type{Type::kInfer}.IsInfer());
    REQUIRE(Type{Type::kUnknown}.IsUnknown());
    REQUIRE(Type{Type::kBox}.IsBox());
    REQUIRE(Type{Type::kInt}.IsInt());
    REQUIRE(Type{Type::kFloat64}.IsReal());
    REQUIRE(Type{Type::kBool}.IsBool());
    REQUIRE(Type{Type::kNull}.IsNull());
    REQUIRE(Type{Type::kUndefined}.IsUndefined());
    REQUIRE(Type{Type::kI64}.IsI64());
    REQUIRE(Type{Type::kString}.IsString());
    REQUIRE(Type{Type::kSymbol}.IsSymbol());
    REQUIRE(Type{Type::kObject}.IsObject());
    REQUIRE(Type{Type::kModule}.IsModule());
    REQUIRE(Type{Type::kJSFunction}.IsJSFunction());
    REQUIRE(Type{Type::kUninitialized}.IsUninitialized());
  }
}

}  // namespace ir
}  // namespace quake
