#ifndef _BASE_STRING_TEMPLATE_H_
#define _BASE_STRING_TEMPLATE_H_
#include "common.h"
#include "result.h"

namespace base {

// A string interpolation engine.
//
// The string template object has been extended to have basic scripting
// functions and behaves like a simple stateless expression engine. The purpose
// of it is to flexibly customize the runtime configuration of Quake Runtime
// instance. It was a simple template engine until I extend it to have full
// expression capabilities.
class StringTemplate {
 public:
  struct Undefined {};

  // Literal name for Val.index() return value. Use the literal name
  // to avoid using constant value.
  enum {
    kValUndefined = 0,
    kValStringRef,
    kValString,
    kValInt,
    kValReal,
    kValBool
  };

  using Val = std::variant<Undefined, const std::string*, std::string,
                           std::int32_t, double, bool>;
  using Function = std::function<Error(const std::vector<Val>&, std::size_t,
                                       std::size_t, Val*)>;
  using FunctionResolver = std::function<Error(const std::string&, Function*)>;
  using VariableResolver = std::function<Error(const std::string&, Val*)>;

  // The evaluation context of the template
  struct Context {
    std::map<std::string, Function> function_table;
    FunctionResolver function_resolver;
    std::map<std::string, VariableResolver> variable_table;
    VariableResolver variable_resolver;
  };

 public:
  // This function convert the value into std::string, ie convert int/real
  // to std::string.
  static std::string ToString(const Val&);

  static bool IsUndefined(const Val& v) { return v.index() == kValUndefined; }

 public:
  StringTemplate();

  // Compile a template string into its internal representation. Later on
  // each execution will result in a std::string which reflects the semantic
  // specified in the std::string compiled.
  Error Compile(const std::string&, Context&&, bool gen_nop = false);

  // Execute the template operation. The output the string will be put
  // into the std::string as output.
  Result<std::string> Run();

  // Dump the internal gut of everything. Used for debugging
  std::string Dump() const;

 public:
  enum Token {
    kText,
    kId,
    kString,
    kInt,
    kReal,
    kTrue,
    kFalse,
    kLPar,
    kRPar,
    kComma,
    kSub,
    kError,
    kRetry,
    kEof
  };
  static const char* GetTokenName(Token);

  struct Lexeme {
    std::int32_t ival;
    double rval;
    std::string str;
    Token token;
    const char* token_name() const {
      return StringTemplate::GetTokenName(token);
    }
  };

  class Lexer {
   public:
    Lexer(const char* src)
        : lexeme_(), pos_(0), source_(src), state_(kStateText) {}

    Token Next();
    const Lexeme& lexeme() const { return lexeme_; }

   private:
    Token LexStr(char);
    Token LexIntOrReal();
    Token LexKeywordOrId();
    Token LexVar();
    Token LexError(const char*);
    Token NextExpression();
    Token NextText();
    Token Y(Token tk, std::size_t len) {
      pos_ += len;
      return (lexeme_.token = tk);
    }

   private:
    enum State { kStateExpression, kStateText };

    Lexeme lexeme_;
    std::size_t pos_;
    const char* source_;

    // Due to the fact that the lexer needs to identify its lexing state
    // based on whether we are in the ${...} or not. The lexer itself has
    // state. It switch its state on and off automatically whithout notifying
    // the parser.
    State state_;
  };

 private:
  Error Compile(Lexer&);
  Error CompileText(Lexer&);
  Error CompileId(Lexer&);
  Error CompileVar(std::string&&);
  Error CompileExpr(Lexer&);
  Error CompileUnary(Lexer&);
  Error CompilePrimary(Lexer&);
  Error CompileBool(Lexer&);
  Error CompileInt(Lexer&);
  Error CompileReal(Lexer&);
  Error CheckIdx(std::size_t);

 private:
  // The program is as simple as a Con list. Which we implicitly define a Con
  // operator works on a list of Opcode sequences. For example :
  //
  // a${foo($value,1)}b
  //
  // will be compiled into
  //
  //   [(kOpLdStr), (LD_VAR, kOpLdInt, kOpLdFn, kOpInvoke), (kOpLdStr)]
  //
  // Then we just need to concatenate all three seqeuences of Opcodes evaluation
  // result together to format the output of the template. To avoid array of
  // array, we just do nothing but linearly evaluate the opcode, then
  // concatenate all the stuff on the stack then we result in same result.

  using Program = std::vector<std::uint32_t>;
  using EvalStack = std::vector<Val>;

  // The template will be compiled into a simple bytecode representation which
  // resembles a stack virtual machine. It is slightly more performant than a
  // simple linked list (pointer chasing) style AST for interpretation.
  // Each opcode will at most takes one argument
  enum Opcode : std::uint32_t {
    // Constant loading
    kOpLdInt,    // Load a integer to stack
    kOpLdReal,   // Load a real to stack
    kOpLdStr,    // Load a string to stack
    kOpLdTrue,   // Load true
    kOpLdFalse,  // Load false
    kOpLdSVar,   // Load a variable via var_table (static lookup)
    kOpLdDVar,   // Load a variable via variable_resolver
    kOpLdFn,     // Load a function index into stack, alias of kOpLdInt

    kOpNeg,  // Used to generate a negative number. It is needed sololy
             // because we may want to support expressoin in future. The
             // lexer cannot generate a negative value(number) directly if
             // we want to be forward compatible

    kOpInvoke,  // Call a function with the specified argument. The argument and
                // the name of the function is placed on the stack. In following
    // The function name will be assembled into index instead of using
    // the string to avoid map lookup.
    // format: [ fn_name ]
    //         [ argN    ]
    //         [ argN-1  ]
    //            ...
    //         [ arg0    ]
    //
    // After invocation, the returned value will be placed on the stack
    // or error happened, the whole stack will be unwinded.
    kOpNop,  // Used to separate segment of code, maybe used for debugging
    kSizeOfOpcode
  };

  struct Func {
    std::string name;
    Function entry;
    Func() = default;
    Func(std::string&& n, Function&& e)
        : name(std::move(n)), entry(std::move(e)) {}
  };

  struct Vars {
    std::string name;
    VariableResolver entry;
    Vars(std::string&& n, VariableResolver&& e)
        : name(std::move(n)), entry(std::move(e)) {}
    Vars() = default;
  };

  // Get the name of opcode
  static const char* GetOpcodeName(Opcode);

  // Encode routine
  static inline std::uint32_t EncodeBytecode(Opcode, std::size_t arg = 0);
  static inline bool DecodeBytecode(std::uint32_t, Opcode*, std::uint32_t*);
  void Disassemble(std::ostream& out) const;

 private:
  Context context_;
  Program program_;
  EvalStack stack_;
  std::vector<std::int32_t> int_table_;
  std::vector<double> real_table_;
  std::vector<std::string> string_table_;
  std::vector<Func> func_table_;
  std::vector<Vars> var_table_;

  DISALLOW_COPY_AND_ASSIGN(StringTemplate);
};

inline std::uint32_t StringTemplate::EncodeBytecode(Opcode opcode,
                                                    std::size_t arg) {
  return opcode | (static_cast<std::uint32_t>(arg) << 8);
}

inline bool StringTemplate::DecodeBytecode(std::uint32_t c, Opcode* op,
                                           std::uint32_t* arg) {
  *op = static_cast<Opcode>(c & 0xff);
  *arg = (c >> 8);
  return *op < kSizeOfOpcode;
}

// Simple string based template rendering
Result<std::string> TemplateRender(
    const std::string&, std::function<std::string(const std::string&)>&&);

}  // namespace base
#endif  // _BASE_STRING_TEMPLATE_H_
