#ifndef _BASE_EVAL_INL_H_
#define _BASE_EVAL_INL_H_

namespace base {
namespace eval {
namespace _ {

inline const char* GetOpcodeName(Opcode op) {
  switch (op) {
#define __(A, B, ...) \
  case Opcode::A:     \
    return B;
    EVAL_INSTRUCTION_LIST(__)
#undef __  // __

    default:
      return "";
  }
}

inline std::size_t GetOpcodeArgument(Opcode op) {
  switch (op) {
#define __(A, B, C) \
  case Opcode::A:   \
    return (C);
    EVAL_INSTRUCTION_LIST(__)
#undef __  // __
    default:
      return 0;
  }
}

const BytecodeIterator& BytecodeIterator::operator++() const {
  DCHECK(length_);
  cursor_ += length_;

  if (cursor_ < c_.get().size()) {
    std::uint32_t a1;
    auto opcode = Decode(c_.get()[cursor_], &a1);
    auto arg = GetOpcodeArgument(opcode);
    switch (arg) {
      case 2:
        std::get<2>(value_) = c_.get()[cursor_++];
        // fallthrough
      case 0:
        std::get<1>(value_) = c_.get()[cursor_++];
        // fallthrough
      default:
        std::get<0>(value_) = opcode;
        break;
    }
    length_ = (arg == 2 ? 2 : 1);
  } else {
    length_ = 0;
  }

  return *this;
}

BytecodeIterator BytecodeIterator::operator++(int) const {
  auto t{*this};
  ++(*this);
  return t;
}

inline bool BytecodeIterator::operator==(const BytecodeIterator& that) const {
  auto this_c = &c_.get();
  auto that_c = &that.c_.get();

  return std::tie(this_c, cursor_, value_) ==
         std::tie(that_c, that.cursor_, that.value_);
}

inline bool BytecodeIterator::operator!=(const BytecodeIterator& that) const {
  return !(*this == that);
}

inline const Value& Stack::GetArgument(std::size_t nth) const {
  DCHECK(IsInCall());
  auto arg_size = GetArgumentSize();
  DCHECK(nth < arg_size);
  auto idx = (arg_size - nth);
  return stack_[stack_.size() - idx];
}

inline void DefaultContext::AddVariable(const std::string& a,
                                        VariableResolver&& r) {
  variable_map_[a] = std::move(r);
}

inline void DefaultContext::AddFunction(const std::string& a, Function&& f) {
  function_map_[a] = std::move(f);
}

inline bool DefaultContext::HasVariable(const std::string& a) const {
  return variable_map_.find(a) != variable_map_.end();
}

inline bool DefaultContext::HasFunction(const std::string& a) const {
  return function_map_.find(a) != function_map_.end();
}

inline bool DefaultContext::RemoveVariable(const std::string& a) {
  if (auto itr = variable_map_.find(a); itr != variable_map_.end()) {
    variable_map_.erase(itr);
    return true;
  }
  return false;
}

inline bool DefaultContext::RemoveFunction(const std::string& a) {
  if (auto itr = function_map_.find(a); itr != function_map_.end()) {
    function_map_.erase(itr);
    return true;
  }
  return false;
}

// ------------------------------------------------------
// Literal Builder
inline LiteralBuilder::LiteralBuilder(Program* p) : t_(&p->lit_table_) {}

inline LiteralIndex LiteralBuilder::AddInt(std::int64_t i) {
  auto& c = t_->int_table;
  if (AlreadyFull(c)) return LiteralIndex{};

  if (auto itr = std::find(c.begin(), c.end(), i); itr != c.end()) {
    return Idx(std::distance(c.begin(), itr));
  } else {
    c.push_back(i);
    return CIdx(c);
  }
}

inline LiteralIndex LiteralBuilder::AddReal(double r) {
  auto& c = t_->real_table;
  if (AlreadyFull(c)) return LiteralIndex{};

  if (auto itr = std::find(c.begin(), c.end(), r); itr != c.end()) {
    return Idx(std::distance(c.begin(), itr));
  } else {
    c.push_back(r);
    return CIdx(c);
  }
}

inline LiteralIndex LiteralBuilder::AddString(const std::string& r) {
  auto& c = t_->string_table;
  if (AlreadyFull(c)) return LiteralIndex{};

  if (auto itr = std::find(c.begin(), c.end(), r); itr != c.end()) {
    return Idx(std::distance(c.begin(), itr));
  } else {
    c.push_back(r);
    return CIdx(c);
  }
}

// ------------------------------------------------------
// Program
inline std::int64_t Program::GetInt(std::uint32_t idx) const {
  DCHECK(idx < lit_table_.int_table.size());
  return lit_table_.int_table[idx];
}

inline double Program::GetReal(std::uint32_t idx) const {
  DCHECK(idx < lit_table_.real_table.size());
  return lit_table_.real_table[idx];
}

inline const std::string& Program::GetString(std::uint32_t idx) const {
  DCHECK(idx < lit_table_.string_table.size());
  return lit_table_.string_table[idx];
}

// ------------------------------------------------------
// Lexer
inline const char* GetTokenName(Token tk) {
  switch (tk) {
#define __(A, B) \
  case A:        \
    return B;
    EVAL_TOKEN_LIST(__)
#undef __  // __

    default:
      return "";
  }
}

inline bool Lexer::Expect(Token tk) {
  if (token_ == tk) {
    Next();
    return true;
  }
  return false;
}

inline bool Lexer::ExpectNext(Token tk) { return (Next() == tk); }

inline Token Lexer::Ret(Token tk) { return (token_ = tk); }

inline Token Lexer::Ret2(std::uint32_t lookahead, Token tk) {
  if (!Eof()) {
    if (!NextChar()) return Eutf8();
    if (c0_ == lookahead) {
      return (token_ = tk);
    }
  }
  return E("missing \"%c\" ?", static_cast<char>(lookahead));
}

inline Token Lexer::Pred(std::uint32_t lookahead, Token t1, Token t2) {
  if (Eof()) {
    return (token_ = t1);
  } else {
    if (!NextChar()) return Eutf8();
    if (c0_ == lookahead) {
      return (token_ = t2);
    } else {
      PrevChar();
      return (token_ = t1);
    }
  }
}

inline Token Lexer::E(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto err = FmtV(fmt, vl);
  va_end(vl);

  auto msg =
      Fmt("around %zu: %s ]]: %s", cursor_, Highlight().c_str(), err.c_str());

  lexeme_ = std::move(msg);
  return (token_ = kTkError);
}

inline Token Lexer::Eutf8() { return E("%s", "utf-8 decoding error"); }

inline std::string Lexer::Highlight() const {
  std::size_t start, end;
  if (cursor_ >= 1) {
    start = cursor_ - 1;
  } else {
    start = 0;
  }

  end = cursor_ + 1;
  if (end >= source_.size()) {
    end = source_.size();
  }

  return HighlightString(source_, start, end, 32, "****", "****");
}

inline bool Lexer::NextChar() {
  auto current_left = GetCurrentSize();
  auto current_source = GetCurrentSource();
  DCHECK(current_left);

  if (auto v = text::Utf8Decoder::DecodeOne(current_source, current_left, &c0_);
      v) {
    c0_length_ = v.value();
    cursor_ += c0_length_;
    return true;
  } else {
    return false;
  }
}

inline void Lexer::PrevChar() {
  DCHECK(c0_length_);
  cursor_ -= c0_length_;
  c0_length_ = 0;
  c0_ = 0;
}

// ------------------------------------------------------
// BytecodeWriter
inline std::uint32_t Encode(Opcode op, std::uint32_t value) {
  DCHECK(IsOpcodePayloadValid(value));
  return (static_cast<std::uint8_t>(op) << 24) | value;
}

inline Opcode Decode(std::uint32_t value, std::uint32_t* payload) {
  *payload = value & 0x00ffffff;
  return static_cast<Opcode>(value >> 24);
}

inline BytecodeWriter::Pos BytecodeWriter::Resv() {
  auto& c = p_->code();
  c.push_back(0);
  return c.size() - 1;
}

inline BytecodeWriter::Pos BytecodeWriter::Patch(Pos pos, Opcode op) {
  auto& c = p_->code();
  c[pos] = Encode(op);
  return pos;
}

inline BytecodeWriter::Pos BytecodeWriter::Patch(Pos pos, Opcode op,
                                                 std::uint32_t v) {
  auto& c = p_->code();
  c[pos] = Encode(op, v);
  return pos;
}

inline BytecodeWriter::Pos BytecodeWriter::Emit(Opcode op) {
  auto& c = p_->code();
  c.push_back(Encode(op));
  return c.size() - 1;
}

inline BytecodeWriter::Pos BytecodeWriter::Emit(Opcode op, std::uint32_t v) {
  auto& c = p_->code();
  c.push_back(Encode(op, v));
  return c.size() - 1;
}

inline BytecodeWriter::Pos BytecodeWriter::Emit(Opcode op, std::uint32_t v,
                                                std::uint32_t v2) {
  auto& c = p_->code();
  c.push_back(Encode(op, v));
  c.push_back(v2);
  return c.size() - 2;
}

inline BytecodeWriter::Pos BytecodeWriter::Emit(Opcode op,
                                                const LiteralIndex& v) {
  DCHECK(v.IsValid());
  return Emit(op, v.index);
}

inline BytecodeWriter::Pos BytecodeWriter::Emit(Opcode op,
                                                const LiteralIndex& v,
                                                std::uint32_t v2) {
  DCHECK(v.IsValid());
  return Emit(op, v.index, v2);
}

inline BytecodeWriter::Pos BytecodeWriter::Or(Pos p) {
  auto& c = p_->code();
  auto current_pos = c.size();
  c[p] = Encode(Opcode::kOr, static_cast<std::uint32_t>(current_pos));
  return p;
}

inline BytecodeWriter::Pos BytecodeWriter::And(Pos p) {
  auto& c = p_->code();
  auto current_pos = c.size();
  c[p] = Encode(Opcode::kAnd, static_cast<std::uint32_t>(current_pos));
  return p;
}

inline BytecodeWriter::Pos BytecodeWriter::JumpFalse(Pos p) {
  auto& c = p_->code();
  auto current_pos = c.size();
  c[p] = Encode(Opcode::kJumpFalse, static_cast<std::uint32_t>(current_pos));
  return p;
}

inline BytecodeWriter::Pos BytecodeWriter::JumpTrue(Pos p) {
  auto& c = p_->code();
  auto current_pos = c.size();
  c[p] = Encode(Opcode::kJumpTrue, static_cast<std::uint32_t>(current_pos));
  return p;
}

inline BytecodeWriter::Pos BytecodeWriter::Ternary(Pos p) {
  auto& c = p_->code();
  auto current_pos = c.size();
  c[p] = Encode(Opcode::kTernary, static_cast<std::uint32_t>(current_pos));
  return p;
}

inline BytecodeWriter::Pos BytecodeWriter::Jump(Pos p) {
  auto& c = p_->code();
  auto current_pos = c.size();
  c[p] = Encode(Opcode::kJump, static_cast<std::uint32_t>(current_pos));
  return p;
}

// ------------------------------------------------------
// Compiler
inline bool Compiler::E(const char* fmt, ...) {
  if (l_->token() == kTkError) {
    error_ = Err(kErrorLexer, "%s", l_->lexeme().ToString().c_str());
  } else {
    std::string msg;
    va_list vl;
    va_start(vl, fmt);
    msg = FmtV(fmt, vl);
    va_end(vl);
    error_ = Err(kErrorParser, "around %s, have error %s",
                 l_->Highlight().c_str(), msg.c_str());
  }
  return false;
}

inline bool Compiler::Eexpect(const char* context, const char* what) {
  return E("in %s, expect %s", context, what);
}

inline bool Compiler::Eidx(const char* context) {
  return E("literal %s has too much index", context);
}

inline bool Compiler::Epair(const char* context) {
  return E("unmatched token: %s", context);
}

// ------------------------------------------------------
// Trace

std::optional<Value> Trace::Ebin(const char* context, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);

  error_ = Err(kErrorEval, "binary operation %s failed]: %s", context,
               FmtV(fmt, vl).c_str());
  va_end(vl);
  return {};
}

std::optional<Value> Trace::Ecomp(const char* context, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);

  error_ = Err(kErrorEval, "comparison operation %s failed]: %s", context,
               FmtV(fmt, vl).c_str());
  va_end(vl);
  return {};
}

std::optional<Value> Trace::Eunary(const char* context, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);

  error_ = Err(kErrorEval, "unary operation %s failed]: %s", context,
               FmtV(fmt, vl).c_str());
  va_end(vl);
  return {};
}

std::optional<Value> Trace::Eresolver(Error&& e) {
  error_ = Err(kErrorEval, "resolver failed]: %s", e.GetDisplay().c_str());
  return {};
}

std::optional<Value> Trace::Erun(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  va_end(vl);
  error_ = Error::MkV(ErrCat(), kErrorEval, fmt, vl);
  return {};
}

bool Trace::AsBoolean(const Value& v) const {
  switch (v.type()) {
    case Value::kNull:
      return false;
    case Value::kInt:
      return v.ToInt() != 0;
    case Value::kBoolean:
      return v.ToBoolean();
    case Value::kReal:
      return v.ToReal() != 0.0;
    case Value::kString:
      return !v.ToString().empty();
    case Value::kList:
      return !v.ToListPtr()->IsEmpty();
    case Value::kDictionary:
      return !v.ToDictionaryPtr()->IsEmpty();
    default:
      UNREACHABLE();
      return false;
  }
}

std::optional<double> Trace::AsReal(const Value& v) const {
  switch (v.type()) {
    case Value::kNull:
      return 0.0;
    case Value::kInt:
      return static_cast<double>(v.ToInt());
    case Value::kReal:
      return v.ToReal();
    case Value::kString:
      return LexicalCast<double>(v.ToString());
    case Value::kBoolean:
      return v.ToBoolean() ? 1.0 : 0.0;
    case Value::kList:
    case Value::kDictionary:
      return {};
    default:
      UNREACHABLE();
      return {};
  }
}

std::optional<std::string> Trace::AsString(const Value& v) const {
  switch (v.type()) {
    case Value::kInt:
      return std::to_string(v.ToInt());
    case Value::kReal:
      return std::to_string(v.ToReal());
    case Value::kString:
      return v.ToString();
    case Value::kBoolean:
      return std::to_string(v.ToBoolean());
    case Value::kNull:
      return "null";
    case Value::kList:
    case Value::kDictionary:
      return {};
    default:
      return {};
  }
}

std::optional<Value> Trace::RunAdd(const Value& lhs, const Value& rhs) {
  if (lhs.IsInt() && rhs.IsInt()) {
    return Value{lhs.ToInt() + rhs.ToInt()};
  } else if (lhs.IsReal() && rhs.IsReal()) {
    return Value{lhs.ToReal() + rhs.ToReal()};
  } else if (lhs.IsNumber() && rhs.IsNumber()) {
    auto lhs_real = AsReal(lhs);
    if (!lhs_real) return Ebin("+", "invalid type lhs: %s", lhs.type_name());
    auto rhs_real = AsReal(rhs);
    if (!rhs_real) return Ebin("+", "invalid type rhs: %s", rhs.type_name());
    return Value{lhs_real.value() + rhs_real.value()};
  } else if (lhs.IsBoolean() && rhs.IsBoolean()) {
    auto lv = static_cast<std::int64_t>(lhs.ToBoolean());
    auto rv = static_cast<std::int64_t>(rhs.ToBoolean());
    return Value{lv + rv};
  } else if (lhs.IsString() || rhs.IsString()) {
    auto lhs_val = AsString(lhs);
    if (!lhs_val) return Ebin("+", "invalid type lhs: %s", lhs.type_name());

    auto rhs_val = AsString(rhs);
    if (!rhs_val) return Ebin("+", "invalid type rhs: %s", rhs.type_name());

    auto& lhs_str = lhs_val.value();
    lhs_str.append(rhs_val.value());
    return Value{std::move(lhs_str)};
  }

  return Ebin("+", "invalid type lhs:%s, rhs:%s", lhs.type_name(),
              rhs.type_name());
}

template <typename T>
std::optional<Value> Trace::RunBin(const Value& lhs, const Value& rhs,
                                   const char* context) {
  if (lhs.IsInt() && rhs.IsInt()) {
    if (auto vv = T::Run(lhs.ToInt(), rhs.ToInt()); vv) {
      return Value{vv.value()};
    } else {
      return Ebin(context, "arithmetic operation not supported");
    }
  } else if (lhs.IsReal() && rhs.IsReal()) {
    if (auto vv = T::Run(lhs.ToReal(), rhs.ToReal()); vv) {
      return Value{vv.value()};
    } else {
      return Ebin(context, "arithmetic operation not supported");
    }
  } else if (lhs.IsNumber() && rhs.IsNumber()) {
    auto lhs_real = AsReal(lhs);
    if (!lhs_real)
      return Ebin(context, "invalid type lhs: %s", lhs.type_name());

    auto rhs_real = AsReal(rhs);
    if (!rhs_real)
      return Ebin(context, "invalid type rhs: %s", rhs.type_name());

    if (auto vv = T::Run(lhs_real.value(), rhs_real.value()); vv) {
      return Value{vv.value()};
    } else {
      return Ebin(context, "arithmetic operation not supported");
    }
  } else if (lhs.IsBoolean() && rhs.IsBoolean()) {
    auto lv = static_cast<std::int64_t>(lhs.ToBoolean());
    auto rv = static_cast<std::int64_t>(rhs.ToBoolean());
    if (auto vv = T::Run(lv, rv); vv) {
      return Value{vv.value()};
    } else {
      return Ebin(context, "arithmetic operation not supported");
    }
  } else if (lhs.IsString() || rhs.IsString()) {
    auto lhs_val = AsString(lhs);
    if (!lhs_val) return Ebin(context, "invalid type lhs: %s", lhs.type_name());

    auto rhs_val = AsString(rhs);
    if (!rhs_val) return Ebin(context, "invalid type rhs: %s", rhs.type_name());

    if (auto v = T::Run(base::Pass(lhs_val), base::Pass(rhs_val)); v) {
      return Value{std::move(v.value())};
    }
  }

  return Ebin(context, "invalid type lhs:%s, rhs:%s", lhs.type_name(),
              rhs.type_name());
}

inline std::optional<Value> Trace::RunSub(const Value& lhs, const Value& rhs) {
  struct Op {
    static std::optional<std::int64_t> Run(std::int64_t l, std::int64_t r) {
      return l - r;
    }

    static std::optional<double> Run(double l, double r) { return l - r; }

    static std::optional<std::string> Run(std::string&&, std::string&&) {
      return {};
    }
  };

  return RunBin<Op>(lhs, rhs, "-");
}

inline std::optional<Value> Trace::RunMul(const Value& lhs, const Value& rhs) {
  struct Op {
    static std::optional<std::int64_t> Run(std::int64_t l, std::int64_t r) {
      return l * r;
    }

    static std::optional<double> Run(double l, double r) { return l * r; }

    static std::optional<std::string> Run(std::string&&, std::string&&) {
      return {};
    }
  };

  return RunBin<Op>(lhs, rhs, "*");
}

inline std::optional<Value> Trace::RunDiv(const Value& lhs, const Value& rhs) {
  struct Op {
    static std::optional<std::int64_t> Run(std::int64_t l, std::int64_t r) {
      if (!r) return {};
      return l / r;
    }

    static std::optional<double> Run(double l, double r) {
      if (!r) return {};
      return l / r;
    }

    static std::optional<std::string> Run(std::string&&, std::string&&) {
      return {};
    }
  };

  return RunBin<Op>(lhs, rhs, "/");
}

inline std::optional<Value> Trace::RunPow(const Value& lhs, const Value& rhs) {
  struct Op {
    static std::optional<std::int64_t> Run(std::int64_t l, std::int64_t r) {
      return std::pow(l, r);
    }

    static std::optional<double> Run(double l, double r) {
      return std::pow(l, r);
    }

    static std::optional<std::string> Run(std::string&&, std::string&&) {
      return {};
    }
  };

  return RunBin<Op>(lhs, rhs, "/");
}

template <typename T>
std::optional<Value> Trace::RunComp(const Value& lhs, const Value& rhs,
                                    const char* context) {
  if (lhs.IsInt() && rhs.IsInt()) {
    if (auto vv = T::Run(lhs.ToInt(), rhs.ToInt()); vv) {
      return Value{vv.value()};
    } else {
      return Ecomp(context, "comparison operation not supported");
    }
  } else if (lhs.IsReal() && rhs.IsReal()) {
    if (auto vv = T::Run(lhs.ToReal(), rhs.ToReal()); vv) {
      return Value{vv.value()};
    } else {
      return Ecomp(context, "comparison operation not supported");
    }
  } else if (lhs.IsNumber() && rhs.IsNumber()) {
    auto lhs_real = AsReal(lhs);
    if (!lhs_real)
      return Ebin(context, "invalid type lhs: %s", lhs.type_name());
    auto rhs_real = AsReal(rhs);
    if (!rhs_real)
      return Ebin(context, "invalid type rhs: %s", rhs.type_name());

    if (auto vv = T::Run(lhs_real.value(), rhs_real.value()); vv) {
      return Value{vv.value()};
    } else {
      return Ecomp(context, "comparison operation not supported");
    }
  } else if (lhs.IsBoolean() && rhs.IsBoolean()) {
    auto lv = static_cast<std::int64_t>(lhs.ToBoolean());
    auto rv = static_cast<std::int64_t>(rhs.ToBoolean());
    if (auto vv = T::Run(lv, rv); vv) {
      return Value{vv.value()};
    } else {
      return Ecomp(context, "comparison operation not supported");
    }
  } else if (lhs.IsString() || rhs.IsString()) {
    auto lhs_val = AsString(lhs);
    if (!lhs_val) return Ebin(context, "invalid type lhs: %s", lhs.type_name());

    auto rhs_val = AsString(rhs);
    if (!rhs_val) return Ebin(context, "invalid type rhs: %s", rhs.type_name());

    if (auto vv = T::Run(lhs_val.value(), rhs_val.value()); vv) {
      return Value{vv.value()};
    } else {
      return Ecomp(context, "comparison operation not supported");
    }
  }

  return Ebin(context, "invalid type lhs:%s, rhs:%s", lhs.type_name(),
              rhs.type_name());
}

#define IMPLEMENTATION(OP, Name)                                            \
  inline std::optional<Value> Trace::Run##Name(const Value& lhs,            \
                                               const Value& rhs) {          \
    struct Op {                                                             \
      static std::optional<bool> Run(std::int64_t l, std::int64_t r) {      \
        return l OP r;                                                      \
      }                                                                     \
      static std::optional<bool> Run(double l, double r) { return l OP r; } \
      static std::optional<bool> Run(const std::string& l,                  \
                                     const std::string& r) {                \
        return l OP r;                                                      \
      }                                                                     \
    };                                                                      \
    return RunComp<Op>(lhs, rhs, #OP);                                      \
  }

IMPLEMENTATION(==, Eq)
IMPLEMENTATION(!=, Ne)
IMPLEMENTATION(<, Lt)
IMPLEMENTATION(<=, Le)
IMPLEMENTATION(>, Gt)
IMPLEMENTATION(>=, Ge)

#undef IMPLEMENTATION  // IMPLEMENTATION

inline std::optional<Value> Trace::RunUnary(const Value& v, Opcode op) {
  if (op == Opcode::kNegate) {
    if (v.IsInt()) return Value{-v.ToInt()};
    if (v.IsReal()) return Value{-v.ToReal()};
    if (v.IsBoolean()) return Value{-static_cast<std::int64_t>(v.ToBoolean())};
    return Eunary("<negate>", "type %s cannot be negated", v.type_name());
  } else if (op == Opcode::kBoolean) {
    return Value{AsBoolean(v)};
  } else {
    DCHECK(op == Opcode::kNot);
    return Value{!AsBoolean(v)};
  }
}

inline std::optional<Value> Trace::RunLoadId(std::uint32_t index) {
  const auto& id = program().GetString(index);
  if (auto resolver = context().GetVariable(id); resolver) {
    if (auto vv = (*resolver)(this, id); vv) {
      auto v = vv.PassResult();
      // Notes(dpeng): The user is |NOT| supposde to return value as
      //   duration or storage size
      DCHECK(!v.IsDuration() && !v.IsStorageSize());
      return v;
    } else {
      return Eresolver(vv.PassError());
    }
  } else {
    return Erun("cannot load variable with name: %s", id.c_str());
  }
}

inline void Trace::RunPushList() {
  auto& vlist = stack_.Top(1);
  DCHECK(vlist.IsList());
  vlist.ToListPtr()->Add(std::move(stack_.Top()));
  stack_.Pop();
}

inline bool Trace::RunAddDict() {
  auto& dict = stack_.Top(2);
  auto& maybe_key = stack_.Top(1);

  DCHECK(dict.IsDictionary());

  if (auto key = AsString(maybe_key); key) {
    dict.ToDictionaryPtr()->Add(key.value(), std::move(stack_.Top()));
    stack_.Pop(2);
    return true;
  } else {
    error_ = Err(kErrorEval,
                 "dictionary's key must be able to convert to string, type %s "
                 "cannot be converted to string",
                 maybe_key.type_name());
    return false;
  }
}

inline std::optional<Value> Trace::RunAccess(const Value& object,
                                             std::uint32_t payload) {
  const auto& name = program().GetString(payload);
  if (object.IsDictionary()) {
    if (auto v = object.ToDictionaryPtr()->Get(name); v) {
      return *v;
    }
  }
  return Erun("unknown field name \"%s\"", name.c_str());
}

inline std::optional<Value> Trace::RunIndex(const Value& object,
                                            const Value& index) {
  if (object.IsDictionary()) {
    if (index.IsString()) {
      if (auto v = object.ToDictionaryPtr()->Get(index.ToString()); v) {
        return *v;
      }
    }
  } else if (object.IsString()) {
    const auto& str = object.ToString();
    if (index.IsInt()) {
      auto idx = index.ToInt();
      if (idx < 0 || static_cast<std::size_t>(idx) >= str.size()) {
        return Erun("string index is out of boundary");
      }
      return Value{std::to_string(str[idx])};
    }
  } else if (object.IsList()) {
    auto list_ptr = object.ToListPtr();
    if (index.IsInt()) {
      auto idx = index.ToInt();
      if (idx < 0 || static_cast<std::size_t>(idx) >= list_ptr->GetSize()) {
        return Erun("list index is out of boundary");
      }
      return list_ptr->Get(idx);
    }
  }

  return Erun("invalid type, lhs:%s and rhs:%s on \".\" operator",
              object.type_name(), index.type_name());
}

inline Trace::Status Trace::RunInvoke(std::uint32_t argument_count,
                                      std::uint32_t function_name,
                                      Value* output) {
  const auto& fname = program().GetString(function_name);
  if (auto resolver = context().GetFunction(fname); resolver) {
    stack_.InCall(argument_count);
    DEFER([&]() { stack_.OutCall(); });
    if (auto err = (*resolver)(this, &stack_); err) {
      Eresolver(std::move(err));
      return kFail;
    } else {
      if (stack_.yield_) {
        // Okay, user yield the current context, just do yielding here
        return kYield;
      } else {
        *output = stack_.PassCallReturn();
        return kFinish;
      }
    }
  }

  Erun("function %s not fonud", fname.c_str());
  return kFail;
}

inline Result<std::pair<bool, Value>> Trace::Run(const Program& program,
                                                 const Context& context) {
  // Always assume a fresh start
  stack_.Clear();
  pc_ = 0;
  yield_ = false;
  bind_info_.Bind(&program, &context);
  switch (Run()) {
    case kYield:
      return Ryield();
    case kFail:
      return Rfail();
    case kFinish:
      return Rokay(stack_.EnsureAndPassFinalValue());
    default:
      UNREACHABLE();
      return {};
  }
}

inline Result<std::pair<bool, Value>> Trace::Resume(Value&& v) {
  DCHECK(yield());
  yield_ = false;

  // The blocking instruction MUST be kInvoke, the pc_ currently points to
  // the next instruction |after| the kInvoke, but the previous kInvoke
  // haven't returned a valid value due to the yield. Now, user's provided
  // value should be pushed onto the top of the stack
  stack_.Push(std::move(v));

  // Run from where we stop
  switch (Run()) {
    case kYield:
      return Ryield();
    case kFail:
      return Rfail();
    case kFinish:
      return Rokay(stack_.EnsureAndPassFinalValue());
    default:
      UNREACHABLE();
      return {};
  }
}

}  // namespace _

inline Result<Value> Eval(const std::string& code, const StaticMap& static_map,
                          const Context& context) {
  using namespace eval::_;
  Compiler compiler;

  auto maybe_program = compiler.Compile(code, static_map, nullptr);
  if (!maybe_program) {
    return Result<Value>{maybe_program.PassError()};
  }

  auto program = maybe_program.PassResult();
  Trace trace;
  auto result = trace.Run(program, context);
  if (result) {
    auto value = result.PassResult();
    if (!value.first) {
      return std::move(value.second);
    } else {
      return Err(kErrorParser, "execution yielded");
    }
  } else {
    return Result<Value>{result.PassError()};
  }
}

inline Result<Program> Compile(const std::string& source,
                               const StaticMap& static_map,
                               std::size_t* finish_cursor) {
  using namespace eval::_;
  Compiler compiler;
  return compiler.Compile(source, static_map, finish_cursor);
}

}  // namespace eval
}  // namespace base

#endif  // _BASE_EVAL_INL_H_
