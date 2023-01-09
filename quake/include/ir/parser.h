#ifndef _QUAKE_IR_PARSER_H_
#define _QUAKE_IR_PARSER_H_
#include "base.h"
#include "ir/opcode.h"
#include "ir/interp-info.h"  // BytecodeInfoProvider

namespace quake {
class Quake;

namespace ir {
class Graph;
class Node;

// ----------------------------------------------------------------------------
// A simple IR textual representation parser. This allows user to construct a
// node(graph) representation from the textual file. This is needed for us to
// perform verification, ie testing etc ...

class Lexer {
 public:
  enum {
    kRef,
    kLPar,
    kRPar,
    kLSqr,
    kRSqr,
    kLBra,
    kRBra,
    kSemicolon,
    kAt,
    kColon,
    kComma,
    kStarStar,

    kStr,
    kInt,
    kReal,
    kId,

    kEof,
    kErr
  };

  struct Lexeme {
    int token;
    std::int64_t int_value;
    double real_value;
    std::string text_value;

    std::uint32_t ref_id() const {
      CHECK(token == Lexer::kRef);
      return static_cast<std::uint32_t>(int_value);
    }
  };

 public:
  explicit Lexer(const std::string_view&);

  int Next();

 public:
  const Lexeme& lexeme() const { return lexeme_; }
  int token() const { return lexeme_.token; }
  std::size_t current() const { return current_; }
  void GetPosition(int*, int*);

 private:
  int LexRef();
  int Yield(int, std::size_t);
  int LexNum();
  int LexId();
  int LexStr();

  void LexComment();
  int Err(const char*, ...);

 private:
  std::string_view input_;
  std::size_t current_;
  Lexeme lexeme_;
};

class Parser : public BytecodeInfoProvider {
 private:
  // NOTES(dpeng): order matters
  enum ParamType {
    kPRef = 0,  // ref id
    kPInt,      // internal node
    kPBool,     // boolean node
    kPI32,      // int32 literal
    kPI64,      // int64 literal
    kPF64,      // float64 literal
    kStr,       // str literal
    kUninit,    // uninitialized literal
    kNull,      // null literal
    kUndefined
  };

  struct Str {
    std::string str;
  };
  struct Uninit {};
  struct Null {};
  struct Undefined {};
  struct Ref {
    std::uint32_t ref;
  };
  struct InternalNode {
    std::uint32_t index;
  };

  using Param =
      std::variant<Ref, InternalNode, bool, std::int32_t, std::int64_t, double,
                   Str, Uninit, Null, Undefined>;

  struct Stmt {
    std::uint32_t ref;         // ref id
    Opcode opcode;             // opcode
    std::vector<Param> param;  // param list
    bool has_exit;             // whether instruction has exit or not
  };

  struct NodeRef {
    const Stmt* stmt;
    Node* node;
  };

 public:
  explicit Parser(Quake* q) : quake_(q) {}

 public:
  base::Error Parse(const std::string_view&, Graph*);

 private:
  // This constructor does not relate any BytecodeInfo to actual running thing,
  // we just need to construct a graph for semantic comparison purpose
  BytecodeInfo bytecode_info() const override { return BytecodeInfo{}; }

 private:
  base::Error Err(const char*, ...);

  base::Result<Param> ParseParam();
  base::Result<Param> ParseRefPrefixParam();
  base::Result<Param> ParseLiteralParam();
  base::Error ParseParamList(Stmt*);
  base::Error ParseStmtRest(Stmt*);
  base::Result<Stmt> ParseOne();
  base::Error ParseAll();

  base::Error GenerateAll();
  base::Error GenerateStmtNode(const Stmt&, bool);
  Node* GetNodeRefNodeChecked(std::uint32_t) const;
  base::Error FillOperand(const NodeRef&);
  std::uint32_t AddInternalStmt(Stmt&&);

 private:
  Quake* quake_ = nullptr;
  Graph* graph_ = nullptr;
  Lexer* lexer_ = nullptr;

  // First pass does parsing, ie putting everything into a stmt_list_. Second
  // pass will based on stmt_list to resolve node and generate the graph
  std::vector<Stmt> stmt_list_;
  std::vector<Stmt> internal_stmt_list_;

  std::set<std::uint32_t> ref_set_;  // used to detect duplicated ref id, notes
                                     // each ref id must be unique

  // --------------------------------------------------------------------------
  // Node generation part
  std::map<std::uint32_t, NodeRef> node_ref_map_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_PARSER_H_
