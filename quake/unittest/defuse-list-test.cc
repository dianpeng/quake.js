#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <base.h>
#include <zone.h>
#include <defuse-list.h>

namespace quake {

struct DefNode {};
struct UseNode;

using DRef = DefUseRef<DefNode, UseNode>;
using DList = DefUseRefList<DefNode, UseNode>;

struct UseNode : public DList {
  UseNode(zone::Zone* z) : DList(z) {}
};

TEST_CASE("basic", "[defuse-list1]") {
  zone::Zone zone(1024, 1024 * 1024);

  UseNode use{&zone};

  DefNode def1;
  UseNode* ptr_u1 = &use;

  DefNode def2;
  UseNode* ptr_u2 = &use;

  DefNode def3;
  UseNode* ptr_u3 = &use;

  UseNode new_use{&zone};

  {
    REQUIRE(use.AddUse(&def1, 0, &ptr_u1));
    REQUIRE(!use.AddUse(&def1, 0, &ptr_u1));

    REQUIRE(use.AddUse(&def2, 0, &ptr_u2));
    REQUIRE(!use.AddUse(&def2, 0, &ptr_u2));

    REQUIRE(use.AddUse(&def3, 0, &ptr_u3));
    REQUIRE(!use.AddUse(&def3, 0, &ptr_u3));

    REQUIRE(use.UseSize() == 3);

    REQUIRE(use.RemoveUse(&def1, 0));
    REQUIRE(use.UseSize() == 2);

    REQUIRE(use.RemoveUse(&def2) == 1);
    REQUIRE(use.HasUse());

    REQUIRE(use.RemoveUse(&def3) == 1);

    REQUIRE(use.UseSize() == 0);
    REQUIRE(!use.HasUse());

    REQUIRE(use.AddUse(&def1, 0, &ptr_u1));
    REQUIRE(use.AddUse(&def2, 0, &ptr_u2));
    REQUIRE(use.AddUse(&def3, 0, &ptr_u3));

    REQUIRE(use.Replace(&new_use) == 3);

    REQUIRE(ptr_u1 == &new_use);
    REQUIRE(ptr_u2 == &new_use);
    REQUIRE(ptr_u3 == &new_use);
    REQUIRE(use.UseSize() == 0);
    REQUIRE(new_use.UseSize() == 3);
  }
}

}  // namespace quake

