#include "ir/parser.h"
#include "ir/node.h"
#include "ir/graph.h"

namespace quake {
namespace ir {

using namespace base;

Lexer::Lexer(const std::string_view& data)
    : input_(data), current_(0), lexeme_() {}

int Lexer::Next() {
  while (current_ < input_.size()) {
    auto c = input_[current_];
    switch (c) {
      case '#':
        return LexRef();
      case '(':
        return Yield(kLPar, 1);
      case ')':
        return Yield(kRPar, 1);
      case '[':
        return Yield(kLSqr, 1);
      case ']':
        return Yield(kRSqr, 1);
      case ':':
        return Yield(kColon, 1);
      case ';':
        return Yield(kSemicolon, 1);
      case '@':
        return Yield(kAt, 1);
      case ',':
        return Yield(kComma, 1);

      case '\'':
      case '"':
        return LexStr();

      case '-':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
        return LexNum();

      case '!':
        LexComment();
        continue;

      case ' ':
      case '\t':
      case '\v':
      case '\r':
      case '\n':
      case '\b':
        ++current_;
        continue;

      default:
        return LexId();
    }
  }

  return Yield(kEof, 0);
}

int Lexer::LexStr() {
  auto d = input_[current_];  // get the delimiter
  lexeme_.text_value.clear();

  for (current_++; current_ < input_.size(); current_++) {
    auto c = input_[current_];
    if (c == d) {
      current_++;
      lexeme_.token = kStr;
      return kStr;
    } else if (c == '\\') {
      // escape sequences
      if (current_ + 1 == input_.size())
        return Err("string is not properly closed by delimiter");

      auto nc = input_[current_ + 1];
      switch (nc) {
        case 'n':
          lexeme_.text_value.push_back('\n');
          break;
        case 't':
          lexeme_.text_value.push_back('\t');
          break;
        case 'r':
          lexeme_.text_value.push_back('\r');
          break;
        case 'b':
          lexeme_.text_value.push_back('\b');
          break;
        case 'v':
          lexeme_.text_value.push_back('\v');
          break;
        case '\\':
          lexeme_.text_value.push_back('\\');
          break;

        default:
          if (nc == d) {
            lexeme_.text_value.push_back(d);
            break;
          } else {
            return Err("invalid escape character");
          }
      }

      current_++;
    } else {
      lexeme_.text_value.push_back(c);
    }
  }

  return Err("string is not properly closed");
}

int Lexer::LexRef() {
  CHECK(input_[current_] == '#');
  current_++;
  std::string buf;

  for (; current_ < input_.size(); current_++) {
    auto c = input_[current_];
    if (!std::isdigit(c)) break;
    buf.push_back(c);
  }

  if (auto maybe = LexicalCast<std::uint32_t>(buf); maybe) {
    lexeme_.token = kRef;
    lexeme_.int_value = maybe.value();
    return kRef;
  } else {
    return Err("# must follow a valid number(integer) to be reference");
  }
}

void Lexer::LexComment() {
  for (current_++; current_ < input_.size(); current_++) {
    auto c = input_[current_];
    if (c == '\n') break;
  }
}

int Lexer::LexNum() {
  std::string buf;
  buf.push_back(input_[current_]);

  auto has_dot = false;

  for (current_++; current_ < input_.size(); current_++) {
    auto c = input_[current_];
    if (c == '.') {
      if (has_dot) break;
      has_dot = true;
    } else if (!std::isdigit(c)) {
      break;
    }

    buf.push_back(c);
  }

  if (has_dot) {
    if (auto maybe = LexicalCast<double>(buf); maybe) {
      lexeme_.token = kReal;
      lexeme_.real_value = maybe.value();
      return kReal;
    } else {
      return Err("invalid real number: %s", buf.c_str());
    }
  } else {
    if (auto maybe = LexicalCast<std::int64_t>(buf); maybe) {
      lexeme_.token = kInt;
      lexeme_.int_value = maybe.value();
      return kInt;
    } else {
      return Err("invalid int number: %s", buf.c_str());
    }
  }
}

int Lexer::LexId() {
  lexeme_.text_value.clear();

  for (; current_ < input_.size(); current_++) {
    auto c = input_[current_];
    if (c != '_' && !std::isalnum(c)) {
      break;
    }

    lexeme_.text_value.push_back(c);
  }

  if (lexeme_.text_value.empty()) {
    return Err("unknown token");
  } else {
    lexeme_.token = kId;
    return kId;
  }
}

int Lexer::Err(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);

  int line, col;
  GetPosition(&line, &col);

  lexeme_.token = kErr;
  lexeme_.text_value = Fmt("(%d:%d): %s", line, col, msg.c_str());
  return kErr;
}

int Lexer::Yield(int tk, std::size_t size) {
  lexeme_.token = tk;
  current_ += size;
  return tk;
}

void Lexer::GetPosition(int* line, int* col) {
  *line = *col = 1;
  const auto min = std::min(current_, input_.size());

  for (std::size_t i = 0; i < min; i++) {
    auto c = input_[current_];
    if (c == '\n') {
      ++*line;
      *col = 1;
    } else {
      ++*col;
    }
  }
}

// ----------------------------------------------------------------------------
// Parsing a single parameter from the current position.
Result<Parser::Param> Parser::ParseParam() {
  switch (lexer_->token()) {
    case Lexer::kAt:
      return ParseLiteralParam();
    case Lexer::kRef:
      return ParseRefPrefixParam();
    default:
      return Err(
          "unknown token, expect @(literal) or #Ref(...) as parameter for "
          "instruction");
  }
}

std::uint32_t Parser::AddInternalStmt(Stmt&& stmt) {
  internal_stmt_list_.push_back(std::move(stmt));
  return static_cast<std::uint32_t>(internal_stmt_list_.size() - 1);
}

Result<Parser::Param> Parser::ParseRefPrefixParam() {
  CHECK(lexer_->token() == Lexer::kRef);
  auto ref_id = lexer_->lexeme().ref_id();
  lexer_->Next();

  if (lexer_->token() == Lexer::kColon) {
    // internal node representation, ie #ref: {...}
    Stmt stmt;  // internal stmt
    stmt.ref = ref_id;

    if (auto e = ParseStmtRest(&stmt); e) {
      return e;
    }

    auto index = AddInternalStmt(std::move(stmt));
    return Param{InternalNode{index}};
  } else {
    return Param{Ref{ref_id}};
  }
}

Result<Parser::Param> Parser::ParseLiteralParam() {
  CHECK(lexer_->token() == Lexer::kAt);
  if (lexer_->Next() != Lexer::kId) {
    return Err("expect literal prefix");
  }

  Param p;
  auto pref = lexer_->lexeme().text_value;

  if (pref == "i32") {
    if (lexer_->Next() != Lexer::kLPar) return Err("expect '('");
    if (lexer_->Next() != Lexer::kInt) return Err("expect int");
    p = static_cast<std::int32_t>(lexer_->lexeme().int_value);
    if (lexer_->Next() != Lexer::kRPar) return Err("expect ')'");
    lexer_->Next();
  } else if (pref == "i64") {
    if (lexer_->Next() != Lexer::kLPar) return Err("expect '('");
    if (lexer_->Next() != Lexer::kInt) return Err("expect int");
    p = static_cast<std::int64_t>(lexer_->lexeme().int_value);
    if (lexer_->Next() != Lexer::kRPar) return Err("expect ')'");
    lexer_->Next();
  } else if (pref == "f64") {
    if (lexer_->Next() != Lexer::kLPar) return Err("expect '('");
    if (lexer_->Next() != Lexer::kInt) return Err("expect real");
    p = lexer_->lexeme().real_value;
    if (lexer_->Next() != Lexer::kRPar) return Err("expect ')'");
    lexer_->Next();
  } else if (pref == "b") {
    if (lexer_->Next() != Lexer::kLPar) return Err("expect '('");
    if (lexer_->Next() != Lexer::kId) return Err("expect true/false");
    if (lexer_->lexeme().text_value == "true") {
      p = true;
    } else if (lexer_->lexeme().text_value == "false") {
      p = false;
    } else {
      return Err("expect true/false");
    }
    if (lexer_->Next() != Lexer::kRPar) return Err("expect ')'");
    lexer_->Next();
  } else if (pref == "null") {
    p = Null{};
    lexer_->Next();
  } else if (pref == "undefined") {
    p = Undefined{};
    lexer_->Next();
  } else if (pref == "uninitialized") {
    p = Uninit{};
    lexer_->Next();
  } else if (pref == "nstr") {
    p = NStr{};
    lexer_->Next();
  } else if (pref == "str") {
    if (lexer_->Next() != Lexer::kLPar) return Err("expect '('");
    if (lexer_->Next() != Lexer::kStr) return Err("expect string literal");
    p = Str{lexer_->lexeme().text_value};
    if (lexer_->Next() != Lexer::kRPar) return Err("expect ')'");
    lexer_->Next();
  } else {
    return Err("unknown literal: %s", pref.c_str());
  }

  return p;
}

Error Parser::ParseParamList(Stmt* stmt) {
  if (lexer_->token() != Lexer::kLSqr) {
    return Err("expect [ to start opcode parameter list");
  }
  lexer_->Next();

  if (lexer_->token() == Lexer::kRSqr) {
    lexer_->Next();
  } else {
    while (true) {
      if (auto r = ParseParam(); !r) {
        return r.PassError();
      } else {
        stmt->param.push_back(r.PassResult());
      }
      if (lexer_->token() == Lexer::kSemicolon) {
        lexer_->Next();
      }
      if (lexer_->token() == Lexer::kRSqr) {
        lexer_->Next();
        break;
      }
    }
  }

  return {};
}

Error Parser::ParseStmtRest(Stmt* stmt) {
  CHECK(lexer_->token() == Lexer::kColon);

  if (lexer_->Next() != Lexer::kId) {
    return Err("expect an identifier to represent the opcode name");
  }
  if (auto maybe = OpcodeParse(lexer_->lexeme().text_value); maybe) {
    stmt->opcode = *maybe;
  } else {
    return Err("%s is unknown opcode", lexer_->lexeme().text_value.c_str());
  }
  lexer_->Next();

  // (3) parsing argument list
  if (auto e = ParseParamList(stmt); e) {
    return e;
  }

  // (4) sanity check
  {
    const auto& info = GetOpcodeInfo(stmt->opcode);
    if (info.arity != static_cast<int>(stmt->param.size())) {
      if (!info.IsNary()) {
        return Err("invalid arity of argument");
      }
    }
  }

  return {};
}

Result<Parser::Stmt> Parser::ParseOne() {
  Stmt stmt;

  // (0) expect a ref id
  if (lexer_->token() != Lexer::kRef) {
    return Err("expect #ref here");
  }
  stmt.ref = static_cast<std::uint32_t>(lexer_->lexeme().int_value);

  // (1) check an colon
  if (lexer_->Next() != Lexer::kColon) {
    return Err("expect a ':'");
  }

  if (auto e = ParseStmtRest(&stmt); e) {
    return e;
  }

  return stmt;
}

// Parse every statement until we hit the end of the text. This pass will
// generate the internal_stmt_list_ and also the stmt_list_
Error Parser::ParseAll() {
  lexer_->Next();  // kickstart

  while (lexer_->token() != Lexer::kEof) {
    if (auto r = ParseOne(); !r) {
      return r.PassError();
    } else {
      stmt_list_.push_back(r.PassResult());
    }
  }

  return {};
}

// After every node is been parsed into the stmt_list_ and internal_stmt_list_
// this passes walks through the stmt_list_ to figure out all the needed
// stmt and then generate the graph respectively
Error Parser::GenerateAll() {
  // Pass 1)
  //   generate all the node without setting up its operand, and also fills up
  //   the node_ref_map_ structure.
  for (const auto& stmt : stmt_list_) {
    if (auto e = GenerateStmtNode(stmt, false); e) return e;
  }
  for (const auto& stmt : internal_stmt_list_) {
    if (auto e = GenerateStmtNode(stmt, true); e) return e;
  }

  // Pass 2)
  //   Set all node's operand accordingly
  for (const auto& entry : node_ref_map_) {
    if (auto e = FillOperand(entry.second); e) return e;
  }
  return {};
}

Error Parser::GenerateStmtNode(const Stmt& stmt, bool internal) {
  const auto& info = GetOpcodeInfo(stmt.opcode);
  Node* node = nullptr;

  switch (info.arity) {
    default:
      CHECK(info.IsNary());

      if (internal) {
        node = graph_->AddNodeInternal(this, stmt.opcode, stmt.param.size());
      } else {
        node = graph_->AddNode(this, stmt.opcode, stmt.param.size());
      }
      break;

    // ------------------------------------------------------------------------
    // known arity
    case 0:
      if (internal) {
        node = graph_->AddNodeInternal(this, stmt.opcode);
      } else {
        node = graph_->AddNode(this, stmt.opcode);
      }
      break;

    case 1:
      if (internal) {
        node = graph_->AddNodeInternal(this, stmt.opcode, nullptr);
      } else {
        node = graph_->AddNode(this, stmt.opcode, nullptr);
      }
      break;

    case 2:
      if (internal) {
        node = graph_->AddNodeInternal(this, stmt.opcode, nullptr, nullptr);
      } else {
        node = graph_->AddNode(this, stmt.opcode, nullptr, nullptr);
      }
      break;
  }

  auto r = node_ref_map_.insert(std::make_pair(stmt.ref, NodeRef{&stmt, node}));
  CHECK(r.second);
  return {};
}

Node* Parser::GetNodeRefNodeChecked(std::uint32_t ref_id) const {
  auto itr = node_ref_map_.find(ref_id);
  CHECK(itr != node_ref_map_.end());
  return itr->second.node;
}

Error Parser::FillOperand(const NodeRef& nref) {
  const auto& stmt = *nref.stmt;
  auto node = nref.node;

  auto add_ssa_var = [&](Node* n) -> SSAVar* {
    if (n->HasOut()) return n->out();
    return graph_->AddSSAVar(n);
  };

  auto add_ssa_var_int = [&](Node* n) -> SSAVar* {
    CHECK(n->IsInternal());
    if (n->HasOut()) {
      CHECK(n->out()->IsInternal());
      return n->out();
    }

    return graph_->SetNodeInternalSSA(n);
  };

  for (int idx = 0; idx < static_cast<int>(stmt.param.size()); idx++) {
    const auto& p = stmt.param[idx];

    // materialize each node accordingly, notes except node/internal node, the
    // other is not materialized yet
    switch (p.index()) {
      case kPRef: {
        auto ref_node = GetNodeRefNodeChecked(Get<Ref>(p).ref);
        auto ssa_ref = add_ssa_var(ref_node);
        node->Set(idx, ssa_ref);
      } break;

      case kPInt: {
        const auto& int_stmt = internal_stmt_list_[Get<InternalNode>(p).index];
        auto ref_node = GetNodeRefNodeChecked(int_stmt.ref);
        auto ssa_ref = add_ssa_var_int(ref_node);
        node->Set(idx, ssa_ref);
      } break;

      case kPBool:
        node->Set(idx, graph_->KBool(this, Get<bool>(p)));
        break;
      case kPI32:
        node->Set(idx, graph_->KI32(this, Get<std::int32_t>(p)));
        break;
      case kPI64:
        node->Set(idx, graph_->KI64(this, Get<std::int64_t>(p)));
        break;
      case kPF64:
        node->Set(idx, graph_->KFloat64(this, Get<double>(p)));
        break;
      case kUninit:
        node->Set(idx, graph_->KUninit(this));
        break;
      case kNull:
        node->Set(idx, graph_->KNull(this));
        break;
      case kUndefined:
        node->Set(idx, graph_->KUndefined(this));
        break;

      case kStr:
      case kNStr:
        UNREACHABLE() << "nstr/str is not supported yet";
        break;

      default:
        UNREACHABLE()
            << "should never reach here, nstr/str is not supported yet";
        break;
    }
  }
  return {};
}

Error Parser::Err(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);

  if (lexer_) {
    if (lexer_->token() == Lexer::kErr) {
      return Error::Mk("tokenizer error: %s",
                       lexer_->lexeme().text_value.c_str());
    } else {
      int line, col;
      lexer_->GetPosition(&line, &col);
      return Error::Mk("around(%d, %d): %s", line, col, msg.c_str());
    }
  } else {
    return Error::Mk(std::move(msg));
  }
}

Error Parser::Parse(const std::string_view& input, Graph* g) {
  Lexer l{input};
  lexer_ = &l;
  graph_ = g;

  DEFER([&]() {
    lexer_ = nullptr;
    graph_ = nullptr;
  });

  // Pass 1) generate all the stmt list
  if (auto e = ParseAll(); e) return e;
  lexer_ = nullptr;

  // Pass 2) generate the node in the graph
  if (auto e = GenerateAll(); e) return e;

  return {};
}

}  // namespace ir
}  // namespace quake
