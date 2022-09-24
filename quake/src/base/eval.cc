#include <base/eval.h>

namespace base {
namespace eval {
namespace _ {
namespace {

struct EvalCategory : public ErrorCategory {
 public:
  const char* GetName() const override final;
  std::string GetInformation(std::uint32_t) const override final;
};

const char* EvalCategory::GetName() const { return "eval"; }

std::string EvalCategory::GetInformation(std::uint32_t c) const {
  switch (c) {
    case kErrorLexer:
      return "eval[lexer]";
    case kErrorParser:
      return "eval[parser]";
    case kErrorEval:
      return "eval[eval]";
    case kErrorFunc:
      return "eval[function]";
    case kErrorFuncArgType:
      return "eval[function-argument-type]";
    case kErrorFuncArgSize:
      return "eval[function-argument-size]";
    default:
      return "eval[]";
  }
}

const ErrorCategory* Cat() {
  static EvalCategory kC;
  return &kC;
}

}  // namespace

DefaultContext::Function* DefaultContext::GetFunction(
    const std::string& a) const {
  if (auto itr = function_map_.find(a); itr != function_map_.end()) {
    return &(itr->second);
  }
  return nullptr;
}

DefaultContext::VariableResolver* DefaultContext::GetVariable(
    const std::string& a) const {
  if (auto itr = variable_map_.find(a); itr != variable_map_.end()) {
    return &(itr->second);
  }
  return nullptr;
}

Error Err(std::uint32_t code, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);
  return Error{Cat(), code, std::move(msg)};
}

const ErrorCategory* ErrCat() { return Cat(); }

/** --------------------------------------------------
 * Lexer
 ** -------------------------------------------------*/

Token Lexer::Next() {
  while (cursor_ < source_.size()) {
    if (!NextChar()) {
      return Eutf8();
    }

    switch (c0_) {
      case '+':
        return Ret(kTkAdd);
      case '-':
        return Ret(kTkSub);
      case '/':
        return Ret(kTkDiv);
      case '*':
        return Pred('*', kTkMul, kTkPow);
      case '<':
        return Pred('=', kTkLt, kTkLe);
      case '>':
        return Pred('=', kTkGt, kTkGe);
      case '=':
        return Pred('=', kTkAssign, kTkEq);
      case '!':
        return Pred('=', kTkNot, kTkNe);
      case '(':
        return Ret(kTkLPar);
      case ')':
        return Ret(kTkRPar);
      case '[':
        return Ret(kTkLSqr);
      case ']':
        return Ret(kTkRSqr);
      case '{':
        return Ret(kTkLBra);
      case '}':
        return Ret(kTkRBra);
      case '.':
        return Ret(kTkDot);
      case ',':
        return Ret(kTkComma);
      case ':':
        return Ret(kTkColon);
      case '?':
        return Ret(kTkQuestion);
      case '\'':
      case '\"':
        return LexStr();
      case '&':
        return Ret2('&', kTkAnd);
      case '|':
        return Ret2('|', kTkOr);
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return LexNum();
      case ' ':
      case '\b':
      case '\v':
      case '\r':
      case '\t':
      case '\n':
        continue;
      default:
        return LexIdOrKeyword();
    }
  }

  return Ret(kTkEof);
}

template <std::size_t N>
bool Lexer::Literal(const char (&rhs)[N]) {
  const auto len = (N - 1);
  const auto pdata = source_.data() + cursor_;
  const auto psize = source_.size() - cursor_;
  std::size_t offset = 0;
  std::size_t i = 0;

  auto done = [&]() -> bool {
    cursor_ += offset;
    return true;
  };

  for (i = 0; i < len && offset < psize; ++i) {
    std::uint32_t codepoint = 0;
    std::size_t length = 0;
    if (auto v = text::Utf8Decoder::DecodeOne(pdata + offset, psize - offset,
                                              &codepoint);
        v) {
      length = v.value();
    } else {
      // NOTES(dpeng): this can be better but it works since the later lexing
      //   will fail on codepoint decoding and I don't want to propagate error
      //   from this function up which makes handling of this function becomes
      //   not binary, ie triple-state can be returned. Sadly, in C++, it will
      //   be turned into a switch statement
      return false;
    }

    if (codepoint != static_cast<std::uint32_t>(rhs[i])) {
      return false;
    }

    offset += length;
  }

  if (i == len) {
    DCHECK(offset <= psize);

    std::uint32_t lk;
    if (offset == psize) {
      return done();
    } else {
      if (auto v =
              text::Utf8Decoder::DecodeOne(pdata + offset, psize - offset, &lk);
          v) {
        if (IsIdRest(lk)) {
          return false;
        } else {
          return done();
        }
      } else {
        return done();
      }
    }
  } else {
    // Early termination, ie maybe identifer or ther keyword
    return false;
  }
}

Token Lexer::LexIdOrKeyword() {
  switch (c0_) {
    case 't':
      if (Literal("ry")) return Ret(kTkTry);
      if (Literal("rue")) return Ret(kTkTrue);
      if (Literal("hen")) return Ret(kTkThen);
      break;
    case 'f':
      if (Literal("alse")) return Ret(kTkFalse);
      break;
    case 'n':
      if (Literal("ull")) return Ret(kTkNull);
      break;
    case 'i':
      if (Literal("f")) return Ret(kTkIf);
      break;
    case 'e':
      if (Literal("lse")) return Ret(kTkElse);
      if (Literal("lif")) return Ret(kTkElif);
      break;
    default:
      break;
  }

  // Identifier lexing
  std::string b;
  Token tk;

  if (c0_ == '$' || IsIdInit(c0_)) {
    if (c0_ == '$') {
      tk = kTkSId;
    } else {
      tk = kTkId;
      b.push_back(c0_);
    }

    while (cursor_ < source_.size()) {
      if (!NextChar()) return Eutf8();
      if (IsIdRest(c0_))
        b.push_back(c0_);
      else {
        PrevChar();
        break;
      }
    }

    lexeme_ = std::move(b);
    return (token_ = tk);
  } else {
    return E("unknown characters");
  }
}

// Notes(dpeng),
//   We use a very simple method to lexing number and this makes us not been
//   able to support the prefix or suffix. Maybe we gonna revise it in the
//   future :)
Token Lexer::LexNum() {
  std::string buf;
  buf.push_back(c0_);
  auto got_dot = false;

  while (cursor_ < source_.size()) {
    if (!NextChar()) return Eutf8();
    if (c0_ == '.') {
      if (got_dot) {
        PrevChar();
        break;
      } else {
        got_dot = true;
        buf.push_back('.');
      }
    } else if (c0_ >= '0' && c0_ <= '9') {
      buf.push_back(c0_);
    } else {
      PrevChar();
      break;
    }
  }

  if (got_dot) {
    if (auto v = LexicalCast<double>(buf); v) {
      lexeme_ = v.value();
      return (token_ = kTkReal);
    } else {
      return E("invalid number literal: %s", buf.c_str());
    }
  } else {
    if (auto v = LexicalCast<std::int64_t>(buf); v) {
      lexeme_ = v.value();
      return (token_ = kTkInt);
    } else {
      return E("invalid number literal: %s", buf.c_str());
    }
  }
}

Token Lexer::LexStr() {
  std::string buf;
  auto init = c0_;

  while (cursor_ < source_.size()) {
    if (!NextChar()) return Eutf8();
    if (c0_ == init) {
      break;
    }
    if (c0_ == '\\') {
      if (!NextChar()) return Eutf8();
      switch (c0_) {
        case 'v':
          buf.push_back('\v');
          break;
        case 't':
          buf.push_back('\t');
          break;
        case 'r':
          buf.push_back('\r');
          break;
        case 'n':
          buf.push_back('\n');
          break;
        case 'b':
          buf.push_back('\b');
          break;
        case '\\':
          buf.push_back('\\');
          break;
        case '\"':
          buf.push_back('\"');
          break;
        case '\'':
          buf.push_back('\'');
          break;
        default:
          return E("unknown escape %c", c0_);
      }
    } else {
      buf.push_back(c0_);
    }
  }

  if (c0_ != init) {
    return E("early terminate in string literal");
  }
  lexeme_ = std::move(buf);
  return (token_ = kTkStr);
}

/** --------------------------------------------------
 * Compiler
 ** -------------------------------------------------*/

// Long live strongtalk and self, the unmatched JIT compiler knowledge legacy
#define __ writer_->Emit

bool Compiler::CompileListTemplate(std::function<bool()>&& val_comp) {
  DCHECK(l_->token() == kTkLSqr);

  // reserve an empty slot for later modification, this is used to init
  // an list with an hintted size. useful for reserve memory upfront
  auto pos = writer_->Resv();
  std::uint32_t cnt = 0;
  for (l_->Next(); l_->token() != kTkRSqr;) {
    ++cnt;
    if (!val_comp()) return false;
    __(Opcode::kPushList);
    if (l_->token() == kTkComma) {
      l_->Next();
    } else if (l_->token() != kTkRSqr) {
      return Eexpect("list literal", "\"]\" or \",\"");
    }
  }

  writer_->Patch(pos, Opcode::kList, cnt);
  l_->Next();
  return true;
}

bool Compiler::CompileList() {
  return CompileListTemplate([this]() -> bool { return CompileExpression(); });
}

bool Compiler::CompileStaticList() {
  return CompileListTemplate(
      [this]() -> bool { return CompileStaticExpression(); });
}

bool Compiler::CompileDictionaryTemplate(
    std::function<bool()>&& key_expr_comp,
    std::function<bool()>&& val_expr_comp) {
  DCHECK(l_->token() == kTkLBra);

  auto pos = writer_->Resv();

  std::uint32_t cnt = 0;
  for (l_->Next(); l_->token() != kTkRBra;) {
    ++cnt;

    // Key
    if (l_->token() == kTkLSqr) {
      l_->Next();
      if (!key_expr_comp()) return false;
      if (!l_->Expect(kTkRSqr)) return Epair("]");
    } else if (l_->token() != kTkStr) {
      return Eexpect("dictionary", "<string literal> \"[expression]\"");
    } else {
      if (!CompileStringLiteral(l_->lexeme().ToString())) return false;
      l_->Next();
    }

    if (l_->token() != kTkColon && l_->token() != kTkAssign) {
      return Eexpect("dictionary", "\"=\" or \":\"");
    } else {
      l_->Next();
    }

    // Value
    if (!val_expr_comp()) return false;

    __(Opcode::kAddDict);

    // Delimiter
    if (l_->token() == kTkComma) {
      l_->Next();
    } else if (l_->token() != kTkRBra) {
      return Eexpect("dictionary", "\"}\" or \",\"");
    }
  }

  writer_->Patch(pos, Opcode::kDict, cnt);
  l_->Next();
  return true;
}

bool Compiler::CompileDictionary() {
  return CompileDictionaryTemplate(
      [this]() -> bool { return CompileExpression(); },
      [this]() -> bool { return CompileExpression(); });
}

bool Compiler::CompileStaticDictionary() {
  return CompileDictionaryTemplate(
      [this]() -> bool { return CompileStaticExpression(); },
      [this]() -> bool { return CompileStaticExpression(); });
}

bool Compiler::CompileStringLiteral(const std::string& v) {
  auto idx = builder_->AddString(v);
  if (!idx.IsValid()) return Eidx("string");
  __(Opcode::kLdStr, idx);
  return true;
}

bool Compiler::CompileIntLiteral(std::int64_t v) {
  auto idx = builder_->AddInt(v);
  if (!idx.IsValid()) return Eidx("int");
  __(Opcode::kLdInt, idx);
  return true;
}

bool Compiler::CompileRealLiteral(double v) {
  auto idx = builder_->AddReal(v);
  if (!idx.IsValid()) return Eidx("real");
  __(Opcode::kLdReal, idx);
  return true;
}

bool Compiler::CompileStaticExpression() {
  switch (l_->token()) {
    case kTkSId:
      if (!CompileSId(l_->lexeme().ToString())) {
        return false;
      }
      l_->Next();
      return true;
    case kTkLSqr:
      return CompileStaticList();
    case kTkLBra:
      return CompileStaticDictionary();

    case kTkTrue:
      __(Opcode::kLdTrue);

      l_->Next();
      return true;
    case kTkFalse:
      __(Opcode::kLdFalse);

      l_->Next();
      return true;
    case kTkNull:
      __(Opcode::kLdNull);

      l_->Next();
      return true;

    case kTkInt:
      if (!CompileIntLiteral(l_->lexeme().ToInt())) return false;
      l_->Next();
      return true;
    case kTkReal:
      if (!CompileRealLiteral(l_->lexeme().ToReal())) return false;
      l_->Next();
      return true;
    case kTkStr:
      if (!CompileStringLiteral(l_->lexeme().ToString())) return false;
      l_->Next();
      return true;

    case kTkLPar:
      l_->Next();
      if (!CompileStaticExpression()) return false;
      return l_->Expect(kTkRPar);

    default:
      return E("unknown token in static expression");
  }
}

bool Compiler::CompilePrimary() {
  switch (l_->token()) {
    case kTkSId:
      if (!CompileSId(l_->lexeme().ToString())) {
        return false;
      }
      l_->Next();
      return true;
    case kTkLSqr:
      return CompileList();
    case kTkLBra:
      return CompileDictionary();
    case kTkLPar:
      l_->Next();
      if (!CompileExpression()) return false;
      return l_->Expect(kTkRPar) ? true : Epair(")");
    /** literals **/
    case kTkTrue:
      __(Opcode::kLdTrue);

      l_->Next();
      return true;
    case kTkFalse:
      __(Opcode::kLdFalse);

      l_->Next();
      return true;
    case kTkNull:
      __(Opcode::kLdNull);

      l_->Next();
      return true;

    case kTkInt:
      if (!CompileIntLiteral(l_->lexeme().ToInt())) return false;
      l_->Next();
      return true;
    case kTkReal:
      if (!CompileRealLiteral(l_->lexeme().ToReal())) return false;
      l_->Next();
      return true;
    case kTkStr:
      if (!CompileStringLiteral(l_->lexeme().ToString())) return false;
      l_->Next();
      return true;

    default:
      return E("unknown token in primary expression");
  }
}

bool Compiler::CompileFuncCall(std::string&& id) {
  DCHECK(l_->token() == kTkLPar);

  std::uint32_t cnt = 0;
  for (l_->Next(); l_->token() != kTkRPar;) {
    ++cnt;
    if (!CompileExpression()) return false;
    if (l_->token() == kTkComma) {
      l_->Next();
    } else if (l_->token() != kTkRPar) {
      return Epair(")");
    }
  }

  l_->Next();

  auto idx = builder_->AddString(id);
  if (!idx.IsValid()) return Eidx("string");

  // Notes, this opcode will occupy 2 slots, 64 bits. The second 32 bits are
  // used to store the index points to the name of the function. Notes, the
  // function is a specialized symbol in our expression
  __(Opcode::kInvoke, cnt, idx.index);
  return true;
}

// The FunctionCall can optionally follow some postfix/suffix expression, usage
// will be like foo().a.b.c.d. If that's the case we should compile accordingly
bool Compiler::CompileFuncCallAndPostfix(std::string&& id) {
  if (!CompileFuncCall(std::move(id))) return false;
  do {
    switch (l_->token()) {
      case kTkLSqr:
        if (!CompileIndex()) return false;
        break;

      case kTkDot:
        if (!CompileDot()) return false;
        break;

      default:
        return true;
    }
  } while (true);

  UNREACHABLE();
  return false;
}

bool Compiler::CompileIndex() {
  DCHECK(l_->token() == kTkLSqr);
  l_->Next();
  if (!CompileExpression()) return false;
  __(Opcode::kIndex);
  if (!l_->Expect(kTkRSqr)) return Epair("]");
  return true;
}

bool Compiler::CompileDot() {
  DCHECK(l_->token() == kTkDot);
  if (!l_->ExpectNext(kTkId))
    return Eexpect("\".\"<expression>", "<id> after \".\"");

  auto idx = builder_->AddString(l_->lexeme().ToString());
  if (!idx.IsValid()) return Eidx("string");
  __(Opcode::kAccess, idx.index);
  l_->Next();
  return true;
}

bool Compiler::CompileId(const std::string& id) {
  auto idx = builder_->AddString(id);
  if (!idx.IsValid()) return Eidx("string");
  __(Opcode::kLdId, idx.index);
  return true;
}

bool Compiler::CompileStaticList(const Value& v) {
  DCHECK(v.IsList());
  auto l = v.ToListPtr();
  auto pos = writer_->Resv();

  std::uint32_t cnt = 0;
  for (const auto& e : *l) {
    ++cnt;
    if (!CompileValue(e)) return false;
    __(Opcode::kPushList);
  }

  writer_->Patch(pos, Opcode::kList, cnt);
  return true;
}

bool Compiler::CompileStaticDictionary(const Value& v) {
  DCHECK(v.IsDictionary());
  auto d = v.ToDictionaryPtr();
  auto pos = writer_->Resv();

  std::uint32_t cnt = 0;
  for (const auto& e : *d) {
    ++cnt;
    if (!CompileStringLiteral(e.first)) return false;
    if (!CompileValue(e.second)) return false;
    __(Opcode::kAddDict);
  }

  writer_->Patch(pos, Opcode::kDict, cnt);
  return true;
}

bool Compiler::CompileValue(const Value& v) {
  switch (v.type()) {
    case Value::kNull:
      __(Opcode::kLdNull);
      break;
    case Value::kInt:
      if (!CompileIntLiteral(v.ToInt())) return false;
      break;
    case Value::kReal:
      if (!CompileRealLiteral(v.ToReal())) return false;
      break;
    case Value::kString:
      if (!CompileStringLiteral(v.ToString())) return false;
      break;
    case Value::kBoolean:
      if (v.ToBoolean()) {
        __(Opcode::kLdTrue);
      } else {
        __(Opcode::kLdFalse);
      }
      break;
    case Value::kList:
      if (!CompileStaticList(v)) return false;
      break;
    case Value::kDictionary:
      if (!CompileStaticDictionary(v)) return false;
      break;

    default:
      return E("unsupported static variable type: %s", v.type_name());
  }
  return true;
}

bool Compiler::CompileSId(const std::string& id) {
  if (auto itr = std::find_if(
          static_map_->begin(), static_map_->end(),
          [&](const StaticEntry& e) -> bool { return e.first == id; });
      itr != static_map_->end()) {
    return CompileValue(itr->second);
  } else {
    return E("static variable %s not found", id.c_str());
  }
}

bool Compiler::CompilePrefixExpr(std::string&& id) {
  // We need to handle function call specially due to the fact we are not
  // gonna have a FO functional language.
  if (l_->token() == kTkLPar) {
    return CompileFuncCallAndPostfix(std::move(id));
  }

  // [] or . style prefix operation, they can be chained in our language
  if (!CompileId(id)) return false;

  auto done = false;
  do {
    switch (l_->token()) {
      case kTkLSqr:
        if (!CompileIndex()) return false;
        break;
      case kTkDot:
        if (!CompileDot()) return false;
        break;
      default:
        done = true;
        break;
    }

  } while (!done);
  return true;
}

bool Compiler::CompilePrefixOrSuffix() {
  if (l_->token() == kTkId) {
    // Notes(dpeng): since we cannot putback a token more than 1, so we will
    //   have to specialize this situationa ie, expression prefixed with ID
    //   token.
    auto id = l_->lexeme().ToString();
    switch (l_->Next()) {
      case kTkLPar:
      case kTkDot:
      case kTkLSqr:
        return CompilePrefixExpr(std::move(id));
      default:
        return CompileId(id);
    }
  } else {
    if (!CompilePrimary()) {
      return false;
    }

    // Handle suffix expression, things like this :
    //   [0, 1, 2][0]
    //   [[0, 1], 2][0][0]
    switch (l_->token()) {
      case kTkDot:
        return CompileDot();
      case kTkLSqr:
        return CompileIndex();
      default:
        return true;
    }
  }
}

bool Compiler::CompileUnary() {
  Token tk;
  switch ((tk = l_->token())) {
    case kTkAdd:
    case kTkSub:
    case kTkNot:
      l_->Next();
      if (!CompileUnary()) return false;
      break;
    default:
      return CompilePrefixOrSuffix();
  }

  if (tk == kTkSub) {
    __(Opcode::kNegate);
  } else if (tk == kTkNot) {
    __(Opcode::kNot);
  }
  return true;
}

static constexpr std::uint32_t kNotOp = 100;

std::uint32_t Compiler::GetBinaryOpPrecedence(Token tk) {
  switch (tk) {
    case Token::kTkAnd:
    case Token::kTkOr:
      return 1;

    case Token::kTkEq:
    case Token::kTkNe:
      return 2;

    case Token::kTkGt:
    case Token::kTkGe:
    case Token::kTkLt:
    case Token::kTkLe:
      return 3;

    case Token::kTkAdd:
    case Token::kTkSub:
      return 4;

    case Token::kTkMul:
    case Token::kTkDiv:
    case Token::kTkPow:
      return 5;

    default:
      return kNotOp;
  }
}

bool Compiler::CompileBinary(std::uint32_t p) {
  if (p == kMaxOperatorPrecedence) {
    return CompileUnary();
  }

  std::uint32_t prec;
  Token tk;

  if (!CompileUnary()) return false;

  for (tk = l_->token();
       ((prec = GetBinaryOpPrecedence(tk)) != kNotOp) && (prec > p);
       tk = l_->token()) {
    std::optional<BytecodeWriter::Pos> jump_pos;
    l_->Next();

    // Notes, for || and &&, we need to generate extra more code here to allow
    // semantic for short circuiting, ie the first part evaluation done, then
    // nothing needs to be evaluated. We gonna place a JUMP here to avoid the
    // second part of the code evaluation if needed.
    if (IsLogic(tk)) {
      jump_pos = writer_->Resv();
    }

    if (!CompileBinary(prec)) return false;

    switch (tk) {
      // arithmetic operators
      case kTkAdd:
        __(Opcode::kAdd);
        break;
      case kTkSub:
        __(Opcode::kSub);
        break;
      case kTkMul:
        __(Opcode::kMul);
        break;
      case kTkDiv:
        __(Opcode::kDiv);
        break;
      case kTkPow:
        __(Opcode::kPow);
        break;

      // comparison
      case kTkLt:
        __(Opcode::kLt);
        break;
      case kTkLe:
        __(Opcode::kLe);
        break;
      case kTkGt:
        __(Opcode::kGt);
        break;
      case kTkGe:
        __(Opcode::kGe);
        break;
      case kTkEq:
        __(Opcode::kEq);
        break;
      case kTkNe:
        __(Opcode::kNe);
        break;

      // logic operator
      case kTkAnd:
        // Notes(dpeng): For and operator, we do a explicit conversion from the
        //   second value on the TOS to boolean. This semantic is pretty much
        //   consistent and also relative useful if user want to do boolean
        //   conversion. For or, we don't since [condition] || value is useful
        //   in some case, and kOr instruction will also not convert the first
        //   argument to boolean but leave it as what it is.
        __(Opcode::kBoolean);
        writer_->And(Get(jump_pos));
        break;

      case kTkOr:
        writer_->Or(Get(jump_pos));
        break;

      default:
        UNREACHABLE();
        break;
    }
  }

  return true;
}

bool Compiler::CompileTernary() {
  if (!CompileBinary()) return false;

  while (l_->token() == kTkQuestion) {
    l_->Next();
    if (!CompileBinary()) return false;

    auto pos = writer_->Resv();

    if (!l_->Expect(kTkColon)) return Eexpect("ternary", "\":\"");

    if (!CompileBinary()) return false;

    writer_->Ternary(pos);
  }
  return true;
}

bool Compiler::CompileBranch(const char* ctx, BytecodeWriter::Pos* next_jump,
                             BytecodeWriter::Pos* out_jump) {
  if (!CompileExpression()) return false;

  *next_jump = writer_->Resv();

  if (!l_->Expect(kTkThen)) return Eexpect(ctx, "\"then\"");

  if (!CompileExpression()) return false;

  *out_jump = writer_->Resv();
  return true;
}

bool Compiler::CompileIf() {
  DCHECK(l_->token() == kTkIf);
  l_->Next();
  std::vector<BytecodeWriter::Pos> out_jump_list;
  std::optional<BytecodeWriter::Pos> next_try;

  {
    BytecodeWriter::Pos next_jump, out_jump;
    if (!CompileBranch("if", &next_jump, &out_jump)) return false;
    out_jump_list.push_back(out_jump);
    next_try = next_jump;
  }

  // list of elif **
  while (l_->token() == kTkElif) {
    l_->Next();
    BytecodeWriter::Pos next_jump, out_jump;

    // Previously generated jump instruction should jump to here, ie the next
    // possible conditional testing
    writer_->JumpFalse(Get(next_try));

    if (!CompileBranch("elif", &next_jump, &out_jump)) return false;

    out_jump_list.push_back(out_jump);
    next_try = next_jump;
  }

  writer_->JumpFalse(Get(next_try));

  // Notes(dpeng): An else is a |MUST| to have condition since we are in an
  //   expression language. A single dangling If will makes expression become
  //   nothing conditionally

  if (!l_->Expect(kTkElse)) {
    return Eexpect("if", "\"else\"");
  }
  if (!CompileExpression()) return false;

  // unconditional *jump*
  for (const auto& j : out_jump_list) {
    writer_->Jump(j);
  }
  return true;
}

bool Compiler::CompileTry() {
  BytecodeWriter::Pos jump_pos;

  DCHECK(l_->token() == kTkTry);
  l_->Next();
  Program::TryEntry exception_entry;

  {
    exception_entry.start = writer_->Pc();

    if (!CompileExpression()) return false;

    exception_entry.end = writer_->Pc();
  }

  // Unconditionally jump to jump away from exception handling code
  jump_pos = writer_->Resv();

  if (!l_->Expect(kTkElse)) return Eexpect("try", "\"then\"");

  // In our current setup, the handler_pc is always same as end, but for future
  // extension, we have this redundancy
  exception_entry.handler_pc = writer_->Pc();

  // static expression, ie those one will never throw exception at runtime
  {
    __(Opcode::kTryStart);

    if (!CompileStaticExpression()) return false;

    __(Opcode::kTryEnd);
  }

  // Jump to here, ie newly code starting point
  writer_->Jump(jump_pos);

  p_->try_handler_.push_back(exception_entry);
  return true;
}

bool Compiler::CompileStatement() {
  switch (l_->token()) {
    case kTkIf:
      return CompileIf();
    case kTkTry:
      return CompileTry();
    default:
      return CompileTernary();
  }
}

bool Compiler::CompileExpression() { return CompileStatement(); }

Result<Program> Compiler::Compile(const std::string& source,
                                  const StaticMap& static_map,
                                  std::size_t* finish_pos) {
  Program prog;
  BytecodeWriter writer(&prog);
  LiteralBuilder literal_builder(&prog);
  Lexer l(source);

  p_ = &prog;
  static_map_ = &static_map;
  writer_ = &writer;
  builder_ = &literal_builder;
  l_ = &l;

  // Get the first token
  l_->Next();

  if (!CompileExpression()) return Result<Program>{Pass(&error_)};

  if (finish_pos) *finish_pos = l_->cursor();
  return Okay(std::move(prog));
}

/** --------------------------------------------------
 * Program
 ** -------------------------------------------------*/

std::string Program::Print() const {
  std::stringstream fmt;

  fmt << "--------------------------------------------------------\n";
  fmt << "------------------- Literals ---------------------------\n";
  fmt << "--------------------------------------------------------\n";

  auto dumper = [&](const auto& v, const char* prefix) {
    fmt << "** " << prefix << '(' << v.size() << ")\n";
    std::size_t idx = 0;
    for (const auto& entry : v) {
      fmt << idx << ". " << entry << '\n';
      ++idx;
    }
    fmt << "** done\n";
  };

  dumper(lit_table_.int_table, "Int table");
  dumper(lit_table_.real_table, "Real table");
  dumper(lit_table_.string_table, "String table");

  fmt << "--------------------------------------------------------\n";
  fmt << "----------------------- Try ----------------------------\n";
  fmt << "--------------------------------------------------------\n";
  {
    std::size_t idx = 0;
    for (const auto& entry : try_handler_) {
      fmt << idx << ". [" << entry.start << ':' << entry.end
          << "]: " << entry.handler_pc << '\n';
      ++idx;
    }
  }

  fmt << "--------------------------------------------------------\n";
  fmt << "----------------------- Code ---------------------------\n";
  fmt << "--------------------------------------------------------\n";
  {
    std::size_t idx = 0;
    for (const auto& r : *this) {
      auto op = std::get<0>(r);
      auto a1 = std::get<1>(r);
      auto a2 = std::get<2>(r);
      fmt << idx << ". " << GetOpcodeName(op);
      if (a2) {
        fmt << "(" << a1.value() << ", " << a2.value() << ")";
      } else if (a1) {
        fmt << "(" << a1.value() << ")";
      }
      fmt << '\n';
      ++idx;
    }
  }
  fmt << "--------------------------------------------------------\n";
  fmt << "--------------------------------------------------------\n";
  fmt << "--------------------------------------------------------\n";

  return fmt.str();
}

/** --------------------------------------------------
 * Trace
 ** -------------------------------------------------*/
Trace::Status Trace::RunInternal() {
  DCHECK(bind_info_.IsBinded());
  const auto& c = program().code();

  auto next_payload = [&]() -> std::uint32_t {
    auto v = c[pc_];
    pc_++;
    return static_cast<std::uint32_t>(v);
  };

  while (pc_ < c.size()) {
    std::uint32_t payload;
    auto opcode = Decode(c[pc_], &payload);
    ++pc_;

    switch (opcode) {
      // Binary operators ------------------------------------------
#define IMPL(NAME)                                             \
  case Opcode::k##NAME:                                        \
    if (auto v = Run##NAME(stack_.Top(1), stack_.Top(0)); v) { \
      stack_.PopAndPush(2, Pass(v));                           \
    } else {                                                   \
      return kFail;                                            \
    }                                                          \
    break

      IMPL(Add);
      IMPL(Sub);
      IMPL(Mul);
      IMPL(Div);
      IMPL(Pow);
      IMPL(Le);
      IMPL(Lt);
      IMPL(Ge);
      IMPL(Gt);
      IMPL(Eq);
      IMPL(Ne);

#undef IMPL  // IMPL

      // unary
      case Opcode::kNegate:
      case Opcode::kNot:
      case Opcode::kBoolean:
        if (auto v = RunUnary(stack_.Top(0), opcode); v) {
          stack_.Top() = Pass(v);
        } else {
          return kFail;
        }
        break;

      // literal
      case Opcode::kLdStr:
        stack_.Push(Value{program().GetString(payload)});
        break;
      case Opcode::kLdTrue:
        stack_.Push(Value{true});
        break;
      case Opcode::kLdFalse:
        stack_.Push(Value{false});
        break;
      case Opcode::kLdInt:
        stack_.Push(Value{program().GetInt(payload)});
        break;
      case Opcode::kLdReal:
        stack_.Push(Value{program().GetReal(payload)});
        break;
      case Opcode::kLdNull:
        stack_.Push(Value{});
        break;

      case Opcode::kLdId:
        if (auto v = RunLoadId(payload); v) {
          stack_.Push(Pass(v));
        } else {
          return kFail;
        }
        break;

      // list and dictionary
      case Opcode::kList:
        stack_.Push(Value{std::make_shared<List>()});
        stack_.Top().ToListPtr()->Reserve(payload);
        break;

      case Opcode::kPushList:
        RunPushList();
        break;

      case Opcode::kDict:
        stack_.Push(Value{std::make_shared<Dictionary>()});
        stack_.Top().ToDictionaryPtr()->Reserve(payload);
        break;

      case Opcode::kAddDict:
        if (!RunAddDict()) {
          return kFail;
        }
        break;

      // Jump
      case Opcode::kTernary:
        // TOP(0) -> value
        // TOP(1) -> condition
        if (AsBoolean(stack_.Top(1))) {
          auto tos = std::move(stack_.Top());
          stack_.Pop(2);
          stack_.Push(std::move(tos));
          pc_ = payload;
        } else {
          stack_.Pop(2);
        }
        break;

      case Opcode::kOr:
        // TOP(0) -> cond
        if (AsBoolean(stack_.Top())) {
          // Notes, for kOr, we don't change the TOS value to boolean but leave
          // it as it is for certain semantics.
          pc_ = payload;
        } else {
          stack_.Pop();
        }
        break;

      case Opcode::kAnd:
        if (!AsBoolean(stack_.Top())) {
          stack_.Top().SetBoolean(false);
          pc_ = payload;
        } else {
          stack_.Pop();
        }
        break;

      case Opcode::kJumpTrue:
        if (AsBoolean(stack_.Top())) {
          pc_ = payload;
        }
        stack_.Pop();
        break;

      case Opcode::kJumpFalse:
        if (!AsBoolean(stack_.Top())) {
          pc_ = payload;
        }
        stack_.Pop();
        break;

      case Opcode::kJump:
        pc_ = payload;
        break;

      case Opcode::kAccess:
        if (auto v = RunAccess(stack_.Top(0), payload); v) {
          stack_.Replace(Pass(v));
        } else {
          return kFail;
        }
        break;

      case Opcode::kIndex:
        if (auto v = RunIndex(stack_.Top(1), stack_.Top(0)); v) {
          stack_.PopAndPush(2, Pass(v));
        } else {
          return kFail;
        }
        break;

      case Opcode::kInvoke: {
        Value ret_val;
        Status ret;
        // The invoke instruction has 2 component, ie totally 64 bits :
        //   1. 32 bits : [opcode] [ argument-number ]
        //   2. 32 bits : [ index (function name) ]
        switch ((ret = RunInvoke(payload, next_payload(), &ret_val))) {
          case kFail:
            return ret;
          case kYield:
            // Pop out the invoked arguments
            stack_.Pop(payload);
            return ret;
          default:
            break;
        }

        stack_.PopAndPush(payload, std::move(ret_val));
        break;
      }

      // TODO(dpeng): Add sanity check that we know we are in recovery mode
      case Opcode::kTryStart:
      case Opcode::kTryEnd:
        break;

      default:
        UNREACHABLE() << "unknown opcode: "
                      << static_cast<std::uint32_t>(opcode) << std::endl;
        break;
    }
  }

  return kFinish;
}

Trace::Status Trace::TryUnwind() {
  // Notes(dpeng): The pc_ always pointed to the next instruction, so the
  //   checking should take this into consideration.
  for (const auto& e : program().try_handler()) {
    if (pc_ >= e.start && pc_ <= e.end) {
      // Resume the call at e.handler_pc.
      stack_.Clear();
      pc_ = e.handler_pc;

      // This should never throw, since static expression is been used which
      // will never generate error/exception
      return RunInternal();
    }
  }
  return kFail;
}

Trace::Status Trace::Run() {
  auto r = RunInternal();
  if (r == kFail) {
    // Try handler checking
    return TryUnwind();
  } else {
    yield_ = (r == kYield);
    return r;
  }
}

}  // namespace _
}  // namespace eval
}  // namespace base
