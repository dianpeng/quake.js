#ifndef _BASE_EVAL_H_
#define _BASE_EVAL_H_
#include "check.h"
#include "common.h"
#include "result.h"
#include "string-util.h"
#include "text/utf-codec.h"
#include "value.h"

namespace base {
namespace eval {
namespace _ {

// This library provides a simple yet powerful interfaces for doing C like
// language expression evaluation at reasonable performance. It supports
// nearly full C language expression and has already fully integrated with the
// Value object interface. It can be used cross multiple facilities builtin
// as part of Base
//
//  The grammar is briefly described as following : ------------------------
//
//  expression := stmt | ternary-or
//
//  stmt       := if | try
//  try        := 'try' '(' expression ')' 'else' '(' static-expression ')'
//  if         := 'if' cval-list else-val | ternary-or
//
//  cval       := '(' expression ')' 'then' '(' expression ')'
//  cval-list  := ['elif' cond-val]*
//  else-val   := 'else' '(' expression ')'
//
//  ternary-or := equal-or ('?' expression : expression)
//  equal-or   := compare-or [('==' | '!=') compare-or]*
//  compare-or := factor-or  [('>' | '>=' | '<' | '<=') factor-or]*
//  factor-or  := term-or [('*' | '/' | '^^') term-or]*
//  term-or    := unary [('+' | '-') unary]*
//  unary      := ('-' | '+' | '!') unary | prefix-or-suffix
//
//  arg-list   := expression [',' expression]*
//
//  prefix-or-suffix :=
//                ID '(' func-list ')' |
//                ID ('.' ID)+ |
//                ID ('[' expression ']') |
//                primary (index-suffix | access-suffix)
//
//  index-suffix := EMPTY | ('[' expression ']') *
//  access-suffix := EMPTY | ('.' ID)*
//
//  static-expression := STRING |
//                       NUMBERS |
//                       BOOLEAN |
//                       NULL |
//                       static-dict |
//                       static-list
//
//  primary    := ID |
//                STRING |
//                NUMBERS |
//                BOOLEAN |
//                NULL |
//                dict |
//                list |
//                subexpr
//
// dict := '{' dict-entry-list '}'
// dict-entry := key '=' value
// key := '[ expression ']' | STRING
// value := expression
// dict-entry-list := dict-entry*
//
// static-dict := '{' static-dict-entry-list '}'
// static-dict-entry-list := STRING '=' static-expression
//
// list := '[' list-entry-list ']'
// list-entry := expression
// list-entry-list = list-entry*
// subexpr := '('expression')'
//
// static-list := '[' static-list-entry-list ']'
// static-list-entry-list := static-expression [',' static-expression]*
//
// Notes, we write a simple recursive descent parser and generate bytecode
// on the fly. No AST is emitted what's so ever

#define EVAL_INSTRUCTION_LIST(__) \
  /** arithmetic **/              \
  __(kAdd, "add", 0)              \
  __(kSub, "sub", 0)              \
  __(kMul, "mul", 0)              \
  __(kDiv, "div", 0)              \
  __(kPow, "pow", 0)              \
  /** comparison **/              \
  __(kEq, "eq", 0)                \
  __(kNe, "ne", 0)                \
  __(kGt, "gt", 0)                \
  __(kGe, "ge", 0)                \
  __(kLt, "lt", 0)                \
  __(kLe, "le", 0)                \
  /** ternary **/                 \
  __(kTernary, "ternary", 1)      \
  __(kJumpTrue, "jump_true", 1)   \
  __(kJumpFalse, "jump_false", 1) \
  __(kAnd, "and", 1)              \
  __(kOr, "or", 1)                \
  __(kJump, "jump", 1)            \
  /** exception **/               \
  __(kTryStart, "try_start", 0)   \
  __(kTryEnd, "try_end", 0)       \
  /** literal **/                 \
  __(kLdId, "ld_id", 1)           \
  __(kLdStr, "ld_str", 1)         \
  __(kLdTrue, "ld_true", 0)       \
  __(kLdFalse, "ld_false", 0)     \
  __(kLdInt, "ld_int", 1)         \
  __(kLdReal, "ld_real", 1)       \
  __(kLdNull, "ld_null", 0)       \
  /** unary **/                   \
  __(kBoolean, "boolean", 0)      \
  __(kNegate, "negate", 0)        \
  __(kNot, "not", 0)              \
  /** list **/                    \
  __(kList, "list", 1)            \
  __(kPushList, "push_list", 0)   \
  /** dict **/                    \
  __(kDict, "dict", 1)            \
  __(kAddDict, "add_dict", 0)     \
  /** lookup **/                  \
  __(kAccess, "access", 1)        \
  __(kIndex, "index", 0)          \
  /** call **/                    \
  __(kInvoke, "invoke", 2)

// clang-format off
enum class Opcode : std::uint8_t {
#define __(A, ...) A,
  EVAL_INSTRUCTION_LIST(__)
#undef __  // __
  kSizeOfOpcodes
};

// Notes, the operator listing order matters!!
#define EVAL_TOKEN_LIST(__) \
  /** operators **/              \
  __(kTkAdd, "+")                \
  __(kTkSub, "-")                \
  __(kTkDiv, "/")                \
  __(kTkMul, "*")                \
  __(kTkPow, "**")               \
  __(kTkLe, "<=")                \
  __(kTkLt, "<")                 \
  __(kTkGe, ">=")                \
  __(kTkGt, "<")                 \
  __(kTkEq, "==")                \
  __(kTkNe, "!=")                \
  __(kTkNot, "!")                \
  __(kTkAnd, "&&")               \
  __(kTkOr, "||")                \
  /** literal/id **/             \
  __(kTkId, "id")                \
  __(kTkSId, "sid")              \
  __(kTkStr, "str")              \
  __(kTkInt, "int")              \
  __(kTkReal, "real")            \
  __(kTkTrue, "true")            \
  __(kTkFalse, "false")          \
  __(kTkNull, "null")            \
  /** punctuation **/            \
  __(kTkLPar, "(")               \
  __(kTkRPar, ")")               \
  __(kTkLSqr, "[")               \
  __(kTkRSqr, "]")               \
  __(kTkLBra, "{")               \
  __(kTkRBra, "}")               \
  __(kTkComma, ",")              \
  __(kTkDot, ".")                \
  __(kTkColon, ":")              \
  __(kTkQuestion, "?")           \
  __(kTkAssign, "=")             \
  /** keyword **/                \
  __(kTkTry, "try")              \
  __(kTkIf, "if")                \
  __(kTkElif, "elif")            \
  __(kTkElse, "else")            \
  __(kTkThen, "then")            \
  /** control **/                \
  __(kTkError, "<error>")        \
  __(kTkEof, "<eof>")

// clang-format off
//
// this really should be an enum class but needs to refactory large portion of
// the source code :(
enum Token : std::uint32_t {

#define __(A, ...) A,
  EVAL_TOKEN_LIST(__)
#undef __  // __

  kSizeOfToken
};
// clang-format on

inline const char* GetTokenName(Token);
inline bool IsBinary(Token tk) { return tk >= kTkAdd && tk <= kTkOr; }
inline bool IsLogic(Token tk) { return tk == kTkAnd || tk == kTkOr; }

// clang-format on
static_assert(sizeof(Opcode) == 1);

template <typename K>
K Get(std::optional<K>&& v) {
  DCHECK(v);
  return std::move(v).value();
}

template <typename K>
K& Get(std::optional<K>& v) {
  DCHECK(v);
  return v.value();
}

template <typename K>
const K& Get(const std::optional<K>& v) {
  DCHECK(v);
  return v.value();
}

inline const char* GetOpcodeName(Opcode);

static constexpr std::size_t kOpcodePayloadMaxSize = ~(0xff000000);

inline bool IsOpcodePayloadValid(std::uint32_t v) {
  return v <= kOpcodePayloadMaxSize;
}

inline std::uint32_t Encode(Opcode op, std::uint32_t val = 0);
inline Opcode Decode(std::uint32_t, std::uint32_t*);
inline std::size_t GetOpcodeArgument(Opcode op);

enum {
  kErrorLexer = 1,
  kErrorParser,
  kErrorEval,
  kErrorFunc,
  kErrorFuncArgType,
  kErrorFuncArgSize
};

Error Err(std::uint32_t, const char*, ...);
const ErrorCategory* ErrCat();

/* ---------------------------------------------------------------------
 * Runtime related information
 * --------------------------------------------------------------------*/

class Program;
class Trace;
class BytecodeWriter;
class Compiler;

using StaticEntry = std::pair<std::string, Value>;
using StaticMap = std::vector<StaticEntry>;

// Evaluation stack. Used to pass arguments and otherstuff. When invoke a
// C++ side function, the stack is used for passing arguments.
//
// The calling ABI is as following :
//
//   1) the arguments are passed in from left to right on the stack, ie the
//      right most one is one the top
//
//   2) user use |Stack| to fetch argument stored on the stack
//
//   [argN]
//   [argN-1]
//   [argN-2]
//   ...
//   [arg0]

class Stack {
 public:
  inline const Value& GetArgument(const std::size_t) const;

  std::size_t GetArgumentSize() const { return Get(call_); }
  bool IsInCall() const { return static_cast<bool>(call_); }

  const Value& call_return() const { return call_return_; }
  void set_call_return(Value&& v) {
    DCHECK(!v.IsStorageSize() && !v.IsDuration());
    call_return_ = std::move(v);
  }
  Value PassCallReturn() { return Pass(&call_return_); }
  void Yield() { yield_ = true; }

 private:
  const Value& Top() const { return stack_.back(); }
  Value& Top() { return stack_.back(); }
  void Push(Value&& v) { stack_.push_back(std::move(v)); }
  void Push(const Value& v) { stack_.push_back(v); }
  void PopAndPush(std::size_t size, Value&& v) {
    Pop(size);
    Push(std::move(v));
  }
  void Replace(Value&& v) { Top() = std::move(v); }
  void Clear() { stack_.clear(); }

  // Back offset from the TOS with offset specified
  const Value& Top(std::size_t offset) const {
    auto size = stack_.size();
    DCHECK(offset < size);
    auto idx = (size - 1) - offset;
    return stack_[idx];
  }
  Value& Top(std::size_t offset) {
    auto size = stack_.size();
    DCHECK(offset < size);
    auto idx = (size - 1) - offset;
    return stack_[idx];
  }

  void Pop() { stack_.pop_back(); }
  void Pop(std::size_t sz) {
    DCHECK(sz <= stack_.size());
    while (sz--) stack_.pop_back();
  }
  const Value& At(std::size_t idx) const { return stack_[idx]; }
  Value& At(std::size_t idx) { return stack_[idx]; }

  // Mark the stack is inside of the C++ registered callback
  void InCall(std::size_t size) {
    call_ = size;
    call_return_.SetNull();
  }
  void OutCall() {
    yield_ = false;
    call_ = std::nullopt;
  }

  // Used to get the final evaluation value. Notes, the top-most of the stack
  // will be assumed as value.
  Value PassFinalValue() {
    DCHECK(stack_.size() >= 1);
    auto v = std::move(Top());
    Clear();
    return v;
  }
  Value EnsureAndPassFinalValue() {
    DCHECK(stack_.size() == 1);
    return PassFinalValue();
  }

 private:
  Stack() = default;
  // For simplicity, we just use a stack of Value as evaluation stack. Better
  // alternative is using some boxing, ie NaN boxing and pack the machine
  // word onto a array structure. While performance is none-goal for this
  // small interpolation.
  using ValueStack = std::vector<Value>;

  ValueStack stack_{};
  std::optional<std::size_t> call_{};
  Value call_return_{};
  bool yield_{false};

  friend class Program;
  friend class Trace;

  DISALLOW_COPY_AND_ASSIGN(Stack);
};

// Related structure for lexing/parsing/evaluation
class Context {
 public:
  using VariableResolver =
      std::function<Result<Value>(Trace*, const std::string&)>;
  using Function = std::function<Error(Trace*, Stack*)>;

 public:
  virtual Function* GetFunction(const std::string&) const = 0;
  virtual VariableResolver* GetVariable(const std::string&) const = 0;

  virtual ~Context() {}

 public:
  static std::unique_ptr<Context> CreateDefault();
};

class DefaultContext : public Context {
 public:
  using Context::Function;
  using Context::VariableResolver;

  using VariableMap = std::map<std::string, VariableResolver>;
  using FunctionMap = std::map<std::string, Function>;

 public:
  inline void AddVariable(const std::string&, VariableResolver&&);
  inline void AddFunction(const std::string&, Function&&);

  inline bool HasVariable(const std::string&) const;
  inline bool HasFunction(const std::string&) const;

  Function* GetFunction(const std::string&) const override;
  VariableResolver* GetVariable(const std::string&) const override;

  inline bool RemoveVariable(const std::string&);
  inline bool RemoveFunction(const std::string&);

 private:
  mutable VariableMap variable_map_;
  mutable FunctionMap function_map_;
};

// Represent an index pointed back to the index table. Notes, if it is invalid
// basically just means the program been compiled are too complicated, ie has
// too much literals, which is unlikely to happen though
struct LiteralIndex {
  using Type = std::uint32_t;
  Type index;
  LiteralIndex(Type i) : index(i) {}
  LiteralIndex() : index(std::numeric_limits<Type>::max()) {}

  bool IsValid() const { return index != std::numeric_limits<Type>::max(); }
  static LiteralIndex Invalid() {
    return LiteralIndex{std::numeric_limits<Type>::max()};
  }
};

struct LiteralTable {
  using IntTable = std::vector<std::int64_t>;
  using RealTable = std::vector<double>;
  using StringTable = std::vector<std::string>;

  IntTable int_table;
  RealTable real_table;
  StringTable string_table;
};

class LiteralBuilder {
 public:
  explicit LiteralBuilder(LiteralTable* t) : t_(t) {}

  inline LiteralIndex AddInt(std::int64_t);
  inline LiteralIndex AddReal(double);
  inline LiteralIndex AddString(const std::string&);

 private:
  // The opcode uses 8 bits, so the maximum size of a valid literal ca at most
  // be (1 << 25) - 1
  template <typename T>
  constexpr bool AlreadyFull(T& c) noexcept {
    return std::size(c) == kOpcodePayloadMaxSize;
  }
  template <typename T>
  LiteralIndex CIdx(T& c) noexcept {
    return LiteralIndex{static_cast<std::uint32_t>(c.size() - 1)};
  }
  LiteralIndex Idx(std::size_t v) noexcept {
    return LiteralIndex{static_cast<std::uint32_t>(v)};
  }

 private:
  inline explicit LiteralBuilder(Program*);

  LiteralTable* t_;

  friend class Compiler;
  DISALLOW_COPY_AND_ASSIGN(LiteralBuilder);
};

// Iterator over the bytecode. Notes, this operation is only used for scanning
// the bytecode for debugging or diagnostic purpose. It is not used in the
// main loop for execution. The main reason is because the Call instruction is
// 2 words length.
class BytecodeIterator {
 public:
  using Result = std::tuple<Opcode, std::optional<std::uint32_t>,
                            std::optional<std::uint32_t>>;

  using CodeRef = std::reference_wrapper<const std::vector<std::uint32_t>>;

 public:
  BytecodeIterator(CodeRef c, std::size_t cursor)
      : c_(c), length_(0), cursor_(cursor), value_() {}

  BytecodeIterator(const BytecodeIterator&) = default;
  BytecodeIterator& operator=(const BytecodeIterator&) = default;

  Result operator*() const { return value_; }
  const Result* operator->() const { return &value_; }

  inline const BytecodeIterator& operator++() const;
  inline BytecodeIterator operator++(int) const;

  inline bool operator==(const BytecodeIterator&) const;
  inline bool operator!=(const BytecodeIterator&) const;

 private:
  CodeRef c_;
  mutable std::size_t length_;
  mutable std::size_t cursor_;
  mutable Result value_;
};

// A compiled program contain literal table and also bytecode. Notes program
// is immutable once been compiled. If user want to run it, needs to use the
// Trace to hold the execution status.
class Program {
 public:
  using IntTable = LiteralTable::IntTable;
  using RealTable = LiteralTable::RealTable;
  using StringTable = LiteralTable::StringTable;
  using Code = std::vector<std::uint32_t>;

  struct TryEntry {
    std::uint32_t start{0}, end{0};
    std::uint32_t handler_pc{0};
  };
  using TryEntryTable = std::vector<TryEntry>;

  Program(Program&&) = default;
  Program& operator=(Program&&) = default;

  const IntTable& int_table() const { return lit_table_.int_table; }
  const RealTable& real_table() const { return lit_table_.real_table; }
  const StringTable& string_table() const { return lit_table_.string_table; }
  const Code& code() const { return code_; }
  const TryEntryTable& try_handler() const { return try_handler_; }

  // Get a huaman readable debug dump of generated code
  std::string Print() const;

 public:
  inline std::int64_t GetInt(std::uint32_t) const;
  inline double GetReal(std::uint32_t) const;
  inline const std::string& GetString(std::uint32_t) const;

  BytecodeIterator begin() const { return BytecodeIterator(code(), 0); }
  BytecodeIterator end() const {
    return BytecodeIterator(code(), code().size());
  }

 private:
  IntTable& int_table() { return lit_table_.int_table; }
  RealTable& real_table() { return lit_table_.real_table; }
  StringTable& string_table() { return lit_table_.string_table; }
  Code& code() { return code_; }

 private:
  Program() = default;

  LiteralTable lit_table_;
  Code code_;

  // Unwinding table, ie exception handler table.
  TryEntryTable try_handler_;

  friend class LiteralBuilder;
  friend class BytecodeWriter;
  friend class Compiler;

  DISALLOW_COPY_AND_ASSIGN(Program);
};

// Used for storing execution purpose
class Trace {
  enum Status { kFinish, kFail, kYield };

 public:
  Trace() : error_(), stack_(), pc_(0), bind_info_(), yield_(false) {}

  // Run the program and return its internal status or result.
  //   The returned result can be failed, with Error provided
  //   or can result in a value as std::pair<bool, Value>
  //
  //     1. If std::pair's first is true, it means it is yielded, ie interrupted
  //        no valid value is provided and user should call |Resume| to resume
  //        the interpretation again when appropriate yielding function finishes
  //
  //     2. Otherwise, the value is concreted as TOS of the evaluation stack
  inline Result<std::pair<bool, Value>> Run(const Program&, const Context&);

  // Resume the suspended thread with provided value
  inline Result<std::pair<bool, Value>> Resume(Value&&);
  inline Result<std::pair<bool, Value>> Resume(const Value& v) {
    return Resume(Value{v});
  }

  bool yield() const { return yield_; }

 private:
  const Program& program() const {
    DCHECK(bind_info_.program);
    return *bind_info_.program;
  }

  const Context& context() const {
    DCHECK(bind_info_.context);
    return *bind_info_.context;
  }

  Status Run();
  Status RunInternal();
  Status TryUnwind();

  // Conversion --------------------------------------------------------
  //   Notes, some conversion may fail, the conversion only make sense
  //   in the context of evaluation perspectives.
  inline bool AsBoolean(const Value&) const;
  inline std::optional<double> AsReal(const Value&) const;
  inline std::optional<std::string> AsString(const Value&) const;

  // Binary operations
  template <typename OP>
  inline std::optional<Value> RunBin(const Value&, const Value&, const char*);
  inline std::optional<Value> RunAdd(const Value&, const Value&);
  inline std::optional<Value> RunSub(const Value&, const Value&);
  inline std::optional<Value> RunMul(const Value&, const Value&);
  inline std::optional<Value> RunDiv(const Value&, const Value&);
  inline std::optional<Value> RunPow(const Value&, const Value&);

  template <typename OP>
  inline std::optional<Value> RunComp(const Value&, const Value&, const char*);
  inline std::optional<Value> RunEq(const Value&, const Value&);
  inline std::optional<Value> RunNe(const Value&, const Value&);
  inline std::optional<Value> RunLt(const Value&, const Value&);
  inline std::optional<Value> RunLe(const Value&, const Value&);
  inline std::optional<Value> RunGt(const Value&, const Value&);
  inline std::optional<Value> RunGe(const Value&, const Value&);

  inline std::optional<Value> RunUnary(const Value&, Opcode);
  inline std::optional<Value> RunLoadId(std::uint32_t);
  inline std::optional<Value> RunAccess(const Value&, std::uint32_t);
  inline std::optional<Value> RunIndex(const Value&, const Value&);
  inline Status RunInvoke(std::uint32_t, std::uint32_t, Value*);

  inline void RunPushList();
  inline bool RunAddDict();

  // -----------------------------------------------
  // Error handler
  inline std::optional<Value> Ebin(const char*, const char*, ...);
  inline std::optional<Value> Ecomp(const char*, const char*, ...);
  inline std::optional<Value> Eunary(const char*, const char*, ...);
  inline std::optional<Value> Eresolver(Error&&);
  inline std::optional<Value> Erun(const char*, ...);

  // ------------------------------------------------
  // Final result generator
  Result<std::pair<bool, Value>> Ryield() {
    DCHECK(yield_);
    return Okay(std::make_pair(true, Value{}));
  }
  Result<std::pair<bool, Value>> Rokay(Value&& v) {
    return Okay(std::make_pair(false, std::move(v)));
  }
  Result<std::pair<bool, Value>> Rfail() {
    DCHECK(error_);
    return Result<std::pair<bool, Value>>{Pass(&error_)};
  }

 private:
  Error error_;
  Stack stack_;
  std::uint32_t pc_;

  struct {
    const Program* program{nullptr};
    const Context* context{nullptr};
    void Bind(const Program* p, const Context* c) {
      program = p;
      context = c;
    }

    bool IsBinded() const { return program && context; }
    void Unbind() {
      program = nullptr;
      context = nullptr;
    }
  } bind_info_;

  bool yield_;

  DISALLOW_COPY_AND_ASSIGN(Trace);
};

/* ---------------------------------------------------------------------
 * Parsing/Lexing/Compiling related
 * --------------------------------------------------------------------*/

// This lexer is a UTF-8 aware lexer, ie default to assume the input is UTF-8
// encoding and currently we are not capable of chaing the value of it for now.
class Lexer {
 public:
  static constexpr bool IsIdInit(std::uint32_t c) {
    return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
  }
  static constexpr bool IsIdRest(std::uint32_t c) {
    return IsIdInit(c) || (c >= '0' && c <= '9');
  }

 public:
  using Lexeme = Value;

  explicit Lexer(const std::string& source)
      : source_(source), cursor_(0), token_(), lexeme_() {}

  Token token() const { return token_; }
  const Lexeme& lexeme() const { return lexeme_; }
  std::size_t cursor() const { return cursor_; }
  const std::string& source() const { return source_; }
  inline std::string Highlight() const;

  bool IsError() const { return token_ == kTkError; }
  bool IsEOF() const { return token_ == kTkEof; }

 public:
  Token Next();
  inline bool Expect(Token tk);
  inline bool ExpectNext(Token);

 private:
  Token LexStr();
  Token LexNum();
  Token LexIdOrKeyword();

  inline Token Ret(Token);
  inline Token Ret2(std::uint32_t, Token);
  inline Token Pred(std::uint32_t, Token, Token);

  inline Token E(const char*, ...);
  inline Token Eutf8();

  // Compare the string from |Current| position until we match the specified
  // char sequences. Notes this function |DOES NOT| mutate state if the
  // comparison failed
  template <std::size_t N>
  bool Literal(const char (&)[N]);

 private:
  const char* GetCurrentSource() const { return source_.data() + cursor_; }
  std::size_t GetCurrentSize() const { return source_.size() - cursor_; }

  inline bool NextChar();
  inline void PrevChar();
  bool Eof() const { return GetCurrentSize() == 0; }

 private:
  const std::string& source_;
  std::size_t cursor_;
  Token token_;
  Lexeme lexeme_;

  // C0, ie the one we are looking at
  std::uint32_t c0_;
  std::uint32_t c0_length_;
};

class BytecodeWriter {
 public:
  explicit BytecodeWriter(Program* p) : p_(p) {}
  using Pos = std::size_t;

  Pos Pc() const { return p_->code_.size(); }

  inline Pos Resv();
  inline Pos Patch(Pos, Opcode);
  inline Pos Patch(Pos, Opcode, std::uint32_t);

  // Emit a normal operation with opcode with |an| external variable
  inline Pos Emit(Opcode);
  inline Pos Emit(Opcode, std::uint32_t);
  inline Pos Emit(Opcode, std::uint32_t, std::uint32_t);
  inline Pos Emit(Opcode, const LiteralIndex&);
  inline Pos Emit(Opcode, const LiteralIndex&, std::uint32_t);

 public:
  // Place the JumpTrue instruction at specified |Pos| and it jumps to the
  // current position. Notes, the jump position is been stored transparently
  // from user standpoint. Up to now, it is been stored as absolute position
  inline Pos Jump(Pos);
  inline Pos JumpTrue(Pos);
  inline Pos JumpFalse(Pos);
  inline Pos And(Pos);
  inline Pos Or(Pos);
  inline Pos Ternary(Pos);

 private:
  Program* p_;
  DISALLOW_COPY_AND_ASSIGN(BytecodeWriter);
};

// We do a code emittion on the fly so parser/compiler is essnetially the
// same shit.
class Compiler {
 public:
  Compiler()
      : error_(),
        p_(nullptr),
        static_map_(nullptr),
        writer_(nullptr),
        builder_(nullptr),
        l_(nullptr) {}

  // Compile the the binded string along with the existed static map to
  // program
  Result<Program> Compile(const std::string&, const StaticMap&, std::size_t*);

 private:
  static constexpr std::uint32_t kMaxOperatorPrecedence = 6;

  bool CompileId(const std::string&);
  bool CompileSId(const std::string&);

  // primary
  bool CompileListTemplate(std::function<bool()>&&);
  bool CompileList();
  bool CompileStaticList();
  bool CompileStaticList(const Value&);

  bool CompileDictionaryTemplate(std::function<bool()>&&,
                                 std::function<bool()>&&);
  bool CompileDictionary();
  bool CompileStaticDictionary();
  bool CompileStaticDictionary(const Value&);
  bool CompileValue(const Value&);

  bool CompileIntLiteral(std::int64_t);
  bool CompileRealLiteral(double);
  bool CompileStringLiteral(const std::string&);
  bool CompilePrimary();

  // prefix expression
  bool CompileIndex();
  bool CompileDot();
  bool CompileFuncCall(std::string&&);
  bool CompileFuncCallAndPostfix(std::string&&);
  bool CompilePrefixExpr(std::string&&);
  bool CompilePrefixOrSuffix();

  bool CompileUnary();
  bool CompileBinary(std::uint32_t);
  bool CompileBinary() { return CompileBinary(0); }
  static std::uint32_t GetBinaryOpPrecedence(Token);

  bool CompileTernary();

  bool CompileStatement();
  bool CompileBranch(const char*, BytecodeWriter::Pos*, BytecodeWriter::Pos*);
  bool CompileIf();

  bool CompileStaticExpression();
  bool CompileExpression();
  bool CompileTry();

  // Error handling
  inline bool E(const char*, ...);
  inline bool Eexpect(const char*, const char*);
  inline bool Eidx(const char*);
  inline bool Epair(const char*);

 private:
  Error error_;

  // Variable used when compiling
  Program* p_;
  const StaticMap* static_map_;
  BytecodeWriter* writer_;
  LiteralBuilder* builder_;
  Lexer* l_;
};

}  // namespace _

using eval::_::Context;
using eval::_::DefaultContext;
using eval::_::Program;
using eval::_::Stack;
using eval::_::StaticMap;
using eval::_::Trace;

// Notes this function will treat yield as error as well. This function only
// allows quick evaluation and the Yield semantic can only be used inside of
// registered C++ code.
inline Result<Value> Eval(const std::string&, const StaticMap&, const Context&);

// Compile the code into Program. User can then use Program to run code
// multiple times.
//
// The optional argument |finish_cursor| will point to code that cannot be
// recognized as part of the expression. It is useful for embedding this
// expression evaluation into other high level structural config parser or
// DSL etc.
inline Result<Program> Compile(const std::string&, const StaticMap&,
                               std::size_t* finish_cursor = nullptr);

}  // namespace eval
}  // namespace base

#include "eval-inl.h"

#endif  // _BASE_EVAL_H_
