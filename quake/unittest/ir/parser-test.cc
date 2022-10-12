#define CATCH_CONFIG_MAIN
#include <unittest/catch-include.h>

#include <quake.h>
#include <ir/parser.h>
#include <ir/graph.h>

namespace quake {
namespace ir {

TEST_CASE("testing lexer basic", "[lexer-basic]") {
  {
    std::string data = R"""(
#1 ( ) [ ] ; @ : , true false
    )""";

    Lexer l{data};
    int result[] = {Lexer::kRef,  Lexer::kLPar,  Lexer::kRPar,
                    Lexer::kLSqr, Lexer::kRSqr,  Lexer::kSemicolon,
                    Lexer::kAt,   Lexer::kColon, Lexer::kComma,
                    Lexer::kId,   Lexer::kId,    Lexer::kEof};

    for (int i = 0; l.token() != Lexer::kEof; i++) {
      REQUIRE(result[i] == l.Next());
    }
  }

  {
    std::string data = R"""(
#1 #2 #0 #100
    )""";

    Lexer l{data};

    REQUIRE(l.Next() == Lexer::kRef);
    REQUIRE(l.lexeme().int_value == 1);

    REQUIRE(l.Next() == Lexer::kRef);
    REQUIRE(l.lexeme().int_value == 2);

    REQUIRE(l.Next() == Lexer::kRef);
    REQUIRE(l.lexeme().int_value == 0);

    REQUIRE(l.Next() == Lexer::kRef);
    REQUIRE(l.lexeme().int_value == 100);

    REQUIRE(l.Next() == Lexer::kEof);
  }

  {
    std::string data = R"""(
  '' 'a' '\\' ""
    )""";

    Lexer l{data};

    REQUIRE(l.Next() == Lexer::kStr);
    REQUIRE(l.lexeme().text_value == "");

    REQUIRE(l.Next() == Lexer::kStr);
    REQUIRE(l.lexeme().text_value == "a");

    REQUIRE(l.Next() == Lexer::kStr);
    REQUIRE(l.lexeme().text_value == "\\");

    REQUIRE(l.Next() == Lexer::kStr);
    REQUIRE(l.lexeme().text_value == "");

    REQUIRE(l.Next() == Lexer::kEof);
  }

  {
    std::string data = R"""(
  0 -1 1 2000 1.0 -1.0
    )""";

    Lexer l{data};

    REQUIRE(l.Next() == Lexer::kInt);
    REQUIRE(l.lexeme().int_value == 0);

    REQUIRE(l.Next() == Lexer::kInt);
    REQUIRE(l.lexeme().int_value == -1);

    REQUIRE(l.Next() == Lexer::kInt);
    REQUIRE(l.lexeme().int_value == 1);

    REQUIRE(l.Next() == Lexer::kInt);
    REQUIRE(l.lexeme().int_value == 2000);

    REQUIRE(l.Next() == Lexer::kReal);
    REQUIRE(l.lexeme().real_value == 1.0);

    REQUIRE(l.Next() == Lexer::kReal);
    REQUIRE(l.lexeme().real_value == -1.0);

    REQUIRE(l.Next() == Lexer::kEof);
  }

  {
    std::string data = R"""(
  true false yes no
    )""";

    Lexer l{data};

    REQUIRE(l.Next() == Lexer::kId);
    REQUIRE(l.lexeme().text_value == "true");

    REQUIRE(l.Next() == Lexer::kId);
    REQUIRE(l.lexeme().text_value == "false");

    REQUIRE(l.Next() == Lexer::kId);
    REQUIRE(l.lexeme().text_value == "yes");

    REQUIRE(l.Next() == Lexer::kId);
    REQUIRE(l.lexeme().text_value == "no");

    REQUIRE(l.Next() == Lexer::kEof);
  }
}

TEST_CASE("testing parser 1", "[parser1]") {
  Quake quake{nullptr};
  Graph g{&quake};

  {
    Parser parser{&quake};
    std::string data = R"""(

! ---------------------------------------
! this is a cool shit, ie IR construction
! ---------------------------------------

#0: start []
#2: loc_get [@i32(2); ]
#3: bx_guard [#2; ]
#5: add_int [#3; @i32(1); ]
#8: loc_get [@i32(0); ]
#11: rc_op [#9: rc_trydec [#8; ]; ]
#12: loc_get [@i32(1); ]
#13: bx_guard [#12; ]
#14: add_int [#13; @i32(1); ]
#17: bb0 []
#18: phi_loop [#5; #22; ]
#19: phi_loop [#14; #26; ]
#20: phi_loop [#14; #26; ]
#21: bx_guard [#18; ]
#22: add_int [#21; @i32(1); ]
#23: rc_op [#24: rc_trydec [#19; ]; ]
#25: bx_guard [#20; ]
#26: add_int [#25; @i32(1); ]
#27: jump [#17; ]
#28: end []

)""";
    CHECK(!parser.Parse(data, &g));
    LOG(ERROR) << '\n' << g.Print();
    LOG(ERROR) << "-----------------------------------------------------";
    LOG(ERROR) << data;
  }
}

}  // namespace ir
}  // namespace quake
