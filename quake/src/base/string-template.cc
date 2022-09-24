#include <base/string-template.h>
#include <base/check.h>

namespace base {
namespace {

enum { kErrorCompile, kErrorExecution };

struct StringTemplateErrorCategory : public ErrorCategory {
  const char* GetName() const override { return "string-template"; }
  std::string GetInformation(std::uint32_t code) const override {
    switch (code) {
      case kErrorCompile:
        return "string-template[compile]";
      case kErrorExecution:
        return "string-template[execution]";
      default:
        return "string-template[none]";
    }
  }
};

const ErrorCategory* Cat() {
  static StringTemplateErrorCategory kC;
  return &kC;
}

Error Err(std::uint32_t code, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);
  return Error{Cat(), code, std::move(msg)};
}

}  // namespace

StringTemplate::StringTemplate()
    : context_(),
      program_(),
      stack_(),
      int_table_(),
      real_table_(),
      string_table_(),
      func_table_(),
      var_table_() {}

const char* StringTemplate::GetTokenName(Token t) {
  switch (t) {
    case kText:
      return "text";
    case kId:
      return "id";
    case kString:
      return "string";
    case kInt:
      return "int";
    case kReal:
      return "real";
    case kTrue:
      return "true";
    case kFalse:
      return "false";
    case kLPar:
      return "(";
    case kRPar:
      return ")";
    case kComma:
      return ",";
    case kSub:
      return "-";
    case kError:
      return "<error>";
    case kRetry:
      return "<retry>";
    case kEof:
      return "<eof>";
    default:
      UNREACHABLE();
      return nullptr;
  }
}

// StringTemplate::Lexer implementation
StringTemplate::Token StringTemplate::Lexer::LexStr(char delimiter) {
  DCHECK(source_[pos_] == delimiter);
  int c;
  lexeme_.str.clear();

  for (pos_++; (c = source_[pos_]); ++pos_) {
    if (c == '\\') {
      // handle escape character
      int nc = source_[pos_ + 1];
      switch (nc) {
        case '\\':
          lexeme_.str.push_back('\\');
          ++pos_;
          continue;
        case '"':
          lexeme_.str.push_back('"');
          ++pos_;
          continue;
        case '\'':
          lexeme_.str.push_back('\'');
          ++pos_;
          continue;
        case 'v':
          lexeme_.str.push_back('\v');
          ++pos_;
          continue;
        case 't':
          lexeme_.str.push_back('\t');
          ++pos_;
          continue;
        case 'b':
          lexeme_.str.push_back('\b');
          ++pos_;
          continue;
        case 'r':
          lexeme_.str.push_back('\r');
          ++pos_;
          continue;
        case 'n':
          lexeme_.str.push_back('\n');
          ++pos_;
          continue;
        default:
          break;
      }
    } else if (c == delimiter) {
      break;
    }

    lexeme_.str.push_back(c);
  }

  if (source_[pos_] == delimiter) {
    ++pos_;
    return (lexeme_.token = kString);
  } else {
    DCHECK(!source_[pos_]);
    return LexError("early termination of quoted string");
  }
}

StringTemplate::Token StringTemplate::Lexer::LexIntOrReal() {
  enum { kWantDigitOrEnd, kWantDigitDotOrEnd, kWantDigit };

  std::string temp{};
  bool is_float = false;
  int state = kWantDigitDotOrEnd;
  int c;

  for (; (c = source_[pos_]); ++pos_) {
    switch (state) {
      case kWantDigitDotOrEnd:
        if (c == '.') {
          state = kWantDigit;
          is_float = true;
        } else if (!std::isdigit(c)) {
          goto done;
        }
        break;
      case kWantDigitOrEnd:
        if (!std::isdigit(c)) {
          goto done;
        }
        break;

      case kWantDigit:
        if (std::isdigit(c)) {
          state = kWantDigitOrEnd;
        }
        break;
      default:
        UNREACHABLE();
        break;
    }

    temp.push_back(c);
  }

done:
  if (state == kWantDigit) {
    return LexError("expect a digit for floating number");
  }
  if (is_float) {
    if (auto v = LexicalCast<double>(temp); !v) {
      return LexError("cannot convert floating number");
    } else {
      lexeme_.rval = Pass(v);
    }
    return (lexeme_.token = kReal);
  } else {
    if (auto v = LexicalCast<std::int32_t>(temp); !v) {
      return LexError("cannot convert integer number");
    } else {
      lexeme_.ival = Pass(v);
    }
    return (lexeme_.token = kInt);
  }
}

namespace {

template <std::size_t N>
bool Match(const char* lhs, const char (&rhs)[N]) {
  const auto payload = (N - 1);
  for (std::size_t i = 0; i < payload; ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }

  // The off by one character cannot be a valid identifier
  return !std::isalnum(lhs[payload]) && lhs[payload] != '_';
}

}  // namespace

StringTemplate::Token StringTemplate::Lexer::LexKeywordOrId() {
  int c = source_[pos_];
  switch (c) {
    case 't':
      if (Match(source_ + pos_ + 1, "rue")) {
        pos_ += 4;
        return (lexeme_.token = kTrue);
      }
      break;
    case 'f':
      if (Match(source_ + pos_ + 1, "alse")) {
        pos_ += 5;
        return (lexeme_.token = kFalse);
      }
      break;
    default:
      break;
  }

  // Identifier
  if (std::isalpha(c) || c == '_') {
    lexeme_.str.clear();
    lexeme_.str.push_back(c);
    for (++pos_; (c = source_[pos_]); ++pos_) {
      if (std::isalnum(c) || c == '_') {
        lexeme_.str.push_back(c);
      } else {
        break;
      }
    }
    return (lexeme_.token = kId);
  }

  return LexError("unrecognized token");
}

StringTemplate::Token StringTemplate::Lexer::LexError(const char* msg) {
  lexeme_.str = Fmt("lexer error around char (%d): %s", pos_, msg);
  return (lexeme_.token = kError);
}

StringTemplate::Token StringTemplate::Lexer::Next() {
  StringTemplate::Token token;
  do {
    if (state_ == kStateExpression) {
      token = NextExpression();
    } else {
      token = NextText();
    }
  } while (token == kRetry);
  return token;
}

StringTemplate::Token StringTemplate::Lexer::NextExpression() {
  for (;;) {
    int c = source_[pos_];

    switch (c) {
      case ',':
        return Y(kComma, 1);
      case '(':
        return Y(kLPar, 1);
      case ')':
        return Y(kRPar, 1);
      case '-':
        return Y(kSub, 1);
      case 0:
        return Y(kEof, 0);

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
        return LexIntOrReal();
      case '\'':
        return LexStr('\'');
      case '"':
        return LexStr('"');
      case '}':
        pos_++;
        state_ = kStateText;
        return kRetry;

      // Skip whitespaces
      case ' ':
      case '\t':
      case '\v':
      case '\r':
      case '\b':
        ++pos_;
        continue;

      default:
        return LexKeywordOrId();
    }
  }
}

StringTemplate::Token StringTemplate::Lexer::NextText() {
  int c;

  lexeme_.str.clear();

  for (; (c = source_[pos_]); ++pos_) {
    if (c == '$') {
      int nc = source_[pos_ + 1];
      if (nc == '{') {
        state_ = kStateExpression;
        pos_ += 2;

        if (lexeme_.str.empty()) {
          // We haven't read anything into the buffer, need to retry
          return kRetry;
        } else {
          return (lexeme_.token = kText);
        }
      }
    }

    // Keep acumulating the string
    lexeme_.str.push_back(c);
  }

  if (lexeme_.str.empty()) {
    return (lexeme_.token = kEof);
  } else {
    return (lexeme_.token = kText);
  }
}

Error StringTemplate::Compile(Lexer& l) {
  switch (l.lexeme().token) {
    case kText:
      return CompileText(l);
    case kId:
      return CompileId(l);
    default:
      return Err(kErrorCompile, "invalid token: %s", l.lexeme().token_name());
  }
}

Error StringTemplate::CheckIdx(std::size_t idx) {
  // We only have 24 bits to store the idx, so maximum number is
  static const std::size_t kMax = (1 << 24);
  if (idx >= kMax) {
    return Err(kErrorCompile, "too large literal index, template too complex");
  }
  return Error{};
}

namespace {

template <typename T>
std::size_t FindOrNew(std::vector<T>* vec, T&& val) {
  auto x = std::find(vec->begin(), vec->end(), val);
  if (x == vec->end()) {
    std::size_t r = vec->size();
    vec->emplace_back(std::move(val));
    return r;
  } else {
    return std::distance(vec->begin(), x);
  }
}

}  // namespace

Error StringTemplate::CompileText(Lexer& l) {
  auto v = l.lexeme().str;
  auto idx = FindOrNew(&string_table_, std::move(v));
  Error e = CheckIdx(idx);
  if (e) {
    return e;
  }

  // (3) encode everything into a bytecode
  program_.push_back(EncodeBytecode(kOpLdStr, idx));

  // (4) Consume the token
  l.Next();

  return Error{};
}

Error StringTemplate::CompileId(Lexer& l) {
  auto fn_name = l.lexeme().str;

  // We need to lookahead one token to decide whether it is a
  // function call or a variable reference.
  auto lookahead = l.Next();
  if (lookahead != kLPar) {
    return CompileVar(std::move(fn_name));
  } else {
    std::map<std::string, Function>::iterator pos;
    pos = context_.function_table.find(fn_name);

    // (1) check whether the function existed inside of the function table
    if (pos == context_.function_table.end()) {
      return Err(kErrorCompile, "function %s doesn't exist", fn_name.c_str());
    }

    auto tk = l.Next();

    // (2) Handle argument list
    std::size_t arg_count = 0;

    if (tk != kRPar) {
      Error e{};
      // Need to go through the argument list
      do {
        e = CompileExpr(l);
        if (e) {
          return e;
        }
        ++arg_count;
        tk = l.lexeme().token;
        if (tk == kComma) {
          l.Next();
        } else if (tk == kRPar) {
          break;
        } else {
          return Err(kErrorCompile, "expect , or )");
        }
      } while (true);
    }

    // Eat the last )
    l.Next();

    // (3) Put the function_name on the top of the stack
    {
      std::size_t idx;

      // Try to find out whether the function already existed or not
      auto x =
          std::find_if(func_table_.begin(), func_table_.end(),
                       [&fn_name](const Func& f) { return f.name == fn_name; });

      if (x == func_table_.end()) {
        func_table_.emplace_back(std::move(fn_name), std::move(pos->second));
        idx = func_table_.size() - 1;
      } else {
        idx = std::distance(func_table_.begin(), x);
      }

      program_.push_back(EncodeBytecode(kOpLdFn, idx));
    }

    // (4) generate the invoke
    program_.push_back(EncodeBytecode(kOpInvoke, arg_count));

    return Error{};
  }
}

Error StringTemplate::CompilePrimary(Lexer& l) {
  // No real expression support, just do simple recursion
  switch (l.lexeme().token) {
    case kId:
      return CompileId(l);
    case kInt:
      return CompileInt(l);
    case kReal:
      return CompileReal(l);
    case kTrue:
    case kFalse:
      return CompileBool(l);
    case kString:
      return CompileText(l);
    case kLPar: {
      l.Next();
      auto e = CompileExpr(l);
      if (e) {
        return e;
      }
      if (l.lexeme().token != kRPar) {
        return Err(kErrorCompile, "expect ) for sub expression");
      }
      l.Next();
      return Error{};
    }
    default:
      return Err(kErrorCompile, "unknown token for expr");
  }
}

Error StringTemplate::CompileExpr(Lexer& l) {
  if (l.lexeme().token == kSub) {
    return CompileUnary(l);
  } else {
    return CompilePrimary(l);
  }
}

Error StringTemplate::CompileInt(Lexer& l) {
  DCHECK(l.lexeme().token == kInt);
  auto v = l.lexeme().ival;
  auto idx = FindOrNew(&int_table_, std::move(v));
  Error e;
  if ((e = CheckIdx(idx))) {
    return e;
  }
  program_.push_back(EncodeBytecode(kOpLdInt, idx));
  l.Next();
  return Error{};
}

Error StringTemplate::CompileReal(Lexer& l) {
  DCHECK(l.lexeme().token == kReal);
  auto v = l.lexeme().rval;
  auto idx = FindOrNew(&real_table_, std::move(v));
  Error e;
  if ((e = CheckIdx(idx))) {
    return e;
  }
  program_.push_back(EncodeBytecode(kOpLdReal, idx));
  l.Next();
  return Error{};
}

Error StringTemplate::CompileVar(std::string&& name) {
  // This function can emit 2 types of bytecode
  //
  // (1) kOpLdSVar which is faster one, it resolve into index defined
  //     inside of the context_ object and store it inside of the var_table_
  //
  // (2) kOpLdDVar which needs to use variable_resolver_ to lookup the variable.
  //     Internally it requires some sort of map implementation which is costy.

  // Check whether we can resolve it directly with kOpLdSVar
  std::map<std::string, VariableResolver>::iterator pos =
      context_.variable_table.find(name);
  if (pos != context_.variable_table.end()) {
    std::size_t idx = 0;

    // Fast path, can use kOpLdSVar
    auto x = std::find_if(var_table_.begin(), var_table_.end(),
                          [&name](const Vars& v) { return name == v.name; });
    if (x != var_table_.end()) {
      idx = std::distance(var_table_.begin(), x);
    } else {
      idx = var_table_.size();
      var_table_.push_back(Vars(std::move(name), std::move(pos->second)));
    }
    auto e = CheckIdx(idx);
    if (e) {
      return e;
    }

    program_.push_back(EncodeBytecode(kOpLdSVar, idx));
  } else {
    // Slow path
    auto idx = FindOrNew(&string_table_, std::move(name));
    Error e;
    if ((e = CheckIdx(idx))) {
      return e;
    }
    program_.push_back(EncodeBytecode(kOpLdDVar, idx));
  }

  return Error{};
}

Error StringTemplate::CompileBool(Lexer& l) {
  DCHECK(l.lexeme().token == kTrue || l.lexeme().token == kFalse);
  if (l.lexeme().token == kTrue) {
    program_.push_back(EncodeBytecode(kOpLdTrue));
  } else {
    program_.push_back(EncodeBytecode(kOpLdFalse));
  }
  l.Next();
  return Error{};
}

Error StringTemplate::CompileUnary(Lexer& l) {
  DCHECK(l.lexeme().token == kSub);
  l.Next();
  auto e = CompilePrimary(l);
  if (e) {
    return e;
  }
  program_.push_back(EncodeBytecode(kOpNeg));
  return Error{};
}

Error StringTemplate::Compile(const std::string& source, Context&& context,
                              bool gen_nop) {
  context_ = std::move(context);
  program_.clear();
  int_table_.clear();
  real_table_.clear();
  string_table_.clear();
  func_table_.clear();
  var_table_.clear();

  Lexer l{source.c_str()};
  Token tk = l.Next();

  while (tk != kEof) {
    if (auto e = Compile(l); e) {
      return e;
    }
    if (gen_nop) {
      program_.push_back(EncodeBytecode(kOpNop));
    }
    tk = l.lexeme().token;
  }
  return Error{};
}

Result<std::string> StringTemplate::Run() {
  stack_.clear();
  Opcode op;
  std::uint32_t arg;

  for (auto x : program_) {
    if (!DecodeBytecode(x, &op, &arg)) {
      LOG(FATAL)
          << "BUG ? The template prorgram has unrecognized instruction: \n"
          << Dump();
    }

    switch (op) {
      case kOpLdInt:
        stack_.push_back(Val(int_table_[arg]));
        break;
      case kOpLdReal:
        stack_.push_back(Val(real_table_[arg]));
        break;
      case kOpLdStr:
        stack_.push_back(Val(&string_table_[arg]));
        break;
      case kOpLdTrue:
        stack_.push_back(Val(true));
        break;
      case kOpLdFalse:
        stack_.push_back(Val(false));
        break;
      case kOpLdSVar: {
        auto& entry = var_table_[arg];
        Val result{};
        auto e = entry.entry(entry.name, &result);
        if (IsUndefined(result)) {
          return Err(kErrorExecution, "variable %s is not resolved properly",
                     entry.name.c_str());
        }
        if (e) {
          return e;
        }
        stack_.push_back(std::move(result));
      } break;
      case kOpLdDVar: {
        auto& n = string_table_[arg];

        if (!context_.variable_resolver) {
          return Err(kErrorExecution, "variable %s cannot be resolved",
                     n.c_str());
        }
        Val result{};
        auto e = context_.variable_resolver(n, &result);
        if (IsUndefined(result)) {
          return Err(kErrorExecution, "variable %s is not resolved properly",
                     n.c_str());
        }
        if (e) {
          return e;
        }
        stack_.push_back(std::move(result));
      } break;
      case kOpLdFn:
        stack_.push_back(Val(static_cast<std::int32_t>(arg)));
        break;
      case kOpNeg: {
        DCHECK(!stack_.empty());
        auto& top = stack_.back();
        if (top.index() == kValInt) {
          auto v = std::get<std::int32_t>(top);
          stack_.back() = Val(-v);
        } else if (top.index() == kValReal) {
          auto v = std::get<double>(top);
          stack_.back() = Val(-v);
        } else {
          return Err(kErrorExecution,
                     "negative operator can only work on numbers");
        }
      } break;

      case kOpInvoke: {
        // Invoke a function
        DCHECK(stack_.size() >= (arg + 1))
            << "BUG, invoke have too less value on stack!\n"
            << Dump();

        auto& fn = stack_.back();
        DCHECK(fn.index() == kValInt);
        auto idx = std::get<std::int32_t>(fn);
        auto& func = func_table_[idx];
        auto start_idx = (stack_.size() - 1 - arg);
        auto end_idx = stack_.size() - 1;
        Val result{};
        auto e = func.entry(stack_, start_idx, end_idx, &result);
        if (e) {
          return e;
        }
        if (IsUndefined(result)) {
          return Err(kErrorExecution, "function %s doesn't return anything",
                     func.name.c_str());
        }
        stack_.resize(stack_.size() - 1 - arg);
        stack_.push_back(std::move(result));
      } break;
      default:
        break;
    }
  }

  // Join everything
  std::string o;
  for (const auto& x : stack_) {
    o.append(ToString(x));
  }
  return o;
}

std::string StringTemplate::ToString(const Val& v) {
  switch (v.index()) {
    case kValUndefined:
      return std::string{};
    case kValStringRef:
      return *std::get<const std::string*>(v);
    case kValString:
      return std::get<std::string>(v);
    case kValInt:
      return std::to_string(std::get<std::int32_t>(v));
    case kValReal:
      return std::to_string(std::get<double>(v));
    case kValBool:
      return std::get<bool>(v) ? "true" : "false";
    default:
      UNREACHABLE();
      return std::string{};
  }
}

const char* StringTemplate::GetOpcodeName(Opcode c) {
  switch (c) {
    case kOpLdInt:
      return "ld_int";
    case kOpLdReal:
      return "ld_real";
    case kOpLdStr:
      return "ld_str";
    case kOpLdTrue:
      return "ld_true";
    case kOpLdFalse:
      return "ld_false";
    case kOpLdSVar:
      return "ld_svar";
    case kOpLdDVar:
      return "ld_dvar";
    case kOpLdFn:
      return "ld_fn";
    case kOpNeg:
      return "neg";
    case kOpInvoke:
      return "invoke";
    case kOpNop:
      return "nop";
    default:
      return nullptr;
  }
}

void StringTemplate::Disassemble(std::ostream& out) const {
  Opcode opcode;
  std::uint32_t arg;
  for (const auto& x : program_) {
    if (DecodeBytecode(x, &opcode, &arg)) {
      out << GetOpcodeName(opcode) << '(' << arg << ")\n";
    } else {
      out << "UNKNOWN(" << static_cast<int>(opcode) << ") (" << arg << ")\n";
    }
  }
}

namespace {

template <typename T>
void PrintTable(const std::vector<T>& table, std::ostream& out) {
  std::size_t count = 0;
  for (const auto& x : table) {
    out << count << ". " << x << '\n';
    ++count;
  }
}

}  // namespace

std::string StringTemplate::Dump() const {
  std::stringstream out{};
  out << '\n';
  out << "---------------------------\n";
  PrintTable(int_table_, out);
  out << "---------------------------\n";
  PrintTable(real_table_, out);
  out << "---------------------------\n";
  PrintTable(string_table_, out);
  out << "---------------------------\n";
  {
    std::size_t count = 0;
    for (const auto& x : var_table_) {
      out << count << ". " << x.name << '\n';
      ++count;
    }
  }
  out << "---------------------------\n";
  {
    std::size_t count = 0;
    for (const auto& x : func_table_) {
      out << count << ". " << x.name << '\n';
      ++count;
    }
  }
  out << "---------------------------\n";
  Disassemble(out);
  return out.str();
}

Result<std::string> TemplateRender(
    const std::string& content,
    std::function<std::string(const std::string&)>&& resolver) {
  // Use StringTemplate object. This is not performant at all. User should
  // only use it for none performance cirticial code path. For performance
  // critical code, consider using StringTemplate directly since it will do
  // compilation and average run of each render is pretty low
  StringTemplate t{};
  StringTemplate::Context ctx{};

  ctx.variable_resolver = [&resolver](const std::string& key,
                                      StringTemplate::Val* result) -> Error {
    *result = resolver(key);
    return Error{};
  };

  if (auto e = t.Compile(content, std::move(ctx)); e) {
    return e;
  }

  return t.Run();
}

}  // namespace base
