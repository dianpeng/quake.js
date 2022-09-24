#ifndef _BASE_CONFIG_JSONISH_H_
#define _BASE_CONFIG_JSONISH_H_
#include <base/_/scanner.h>
#include <base/check.h>
#include <base/log.h>
#include <base/value.h>

#include "basic.h"

namespace base {
namespace config {

// A Jsonish like syntax DSL for configuration purpose. Notes the Jsonish is
// superset of Json and a valid Json can be parsed correctly by Jsonish parser.
// But Jsonish will accept richer grammar/syntax than plain Json.
//
// Jsonish grammar, briefly described as EBNF :
//
// Notes, for term1 (terminator2) separated list, we have shortcuts as following
//
//   1. production-rule@(',') := EMPTY | production-rule (',' production-rule)*
//     * Notes the separator can be yet another production rule
//
//   2. EMPTY represents an empty placeholder
//
// value := atomic | object | array | named-object
// atomic:= QSTRING | INT | REAL | MSTRING | NULL | TRUE | FALSE
//
// (0) document := instruction-list document-rest
//     document-rest := value | object | implicit-statement-list
//
// (1) value part production rule
//
//   compound-separator := ','
//
//   named-object := ID '(' named-object-entry-list ')'
//   named-object-entry-list := (object-key '=' value)@(',')
//
//   object := OBJECT_START object-entry-list OBJECT_END
//   object-key : QSTRING | ID
//   object-entry-list := (object-key (':' | '=') value)@(compound-separator)
//
//   array := ARRAY_START array-entry-list ARRAY_END
//   array-entry-list := value@(compound-separator)
//
//   implicit-statement := implicit-object | implicit-array
//   implicit-object := object-entry-list
//   implicit-statement-list := implicit-statement*
//
//   implicit-array  := object-key implicit-array-attribute-list object
//   implicit-array-attribute-list := EMPTY |
//      '[' named-object-entry-list ']'
//
// (2) instruction, notes, instruction can only appear at the very top of the
//     configuration file. It is basically used for internal configuration
//     parsing instruction guidance purpose. Notes, all instruction will be
//     terminated by having a semicolon. This is on purposed
//
//   instruction-list := instruction*
//   instruction := include
//
//   include := "@include" (object-key (':' | '=')) * QSTRING SEMICOLON

class Jsonish {
 public:
  // Include handler, ie helpers to perform actual inclusion operations. If
  // not registered, then the include instruction is been disabled.
  class IncludeInstruction {
   public:
    // LoadedContent for Jsonish. Notes, user needs to use this to return
    // Jsonish a pair, the first element points to a id/path which used to
    // identify the loaded content, and the second to contain the whole loaded
    // content for parsing. Jsonish uses the first id/path to tell user
    // the current parsing content for 1) diagnostic information reporting and
    // 2) circular inclusion handling.
    using LoadedContent = std::pair<std::string, std::string>;

    virtual Result<LoadedContent> Run(Jsonish*, const std::string&) = 0;

    virtual ~IncludeInstruction() {}
  };

  using EvalCallback = std::function<Result<Value>(const std::string&)>;
  using VarCallback = std::function<Error(std::string&&, Value&&)>;

  struct Option {
    std::size_t max_include_nested_size;
    IncludeInstruction* include_instruction;
    bool as_json;

    // Eval related, notes, support eval is kind of insecure. Make sure it is
    // off if you just want to parse the JSON
    //
    // If allow_eval is true, then whenever a eval token is encountered, it will
    // call function do_eval to let it do the eval. The do_eval should generate
    // a Value or Error based on the eval input.
    bool allow_eval;
    EvalCallback do_eval;
    VarCallback do_var;

    // Whether allow pragma statement or not
    bool pragma;

    // Prgama option control ---------------------------------------------------
    // user should set it manually or via pragma statement to set inside of the
    // configuration file
    bool pragma_dup_key_merge{false};

    Option()
        : max_include_nested_size(32),
          include_instruction(nullptr),
          as_json(false),
          allow_eval(false),
          do_eval(),
          do_var(),
          pragma(true) {}

    explicit Option(std::size_t max_nsize, IncludeInstruction* ii = nullptr,
                    bool as_j = false, bool ev = false, EvalCallback&& ec = {},
                    VarCallback&& vc = {}, bool prag = true)
        : max_include_nested_size(max_nsize),
          include_instruction(ii),
          as_json(as_j),
          allow_eval(ev),
          do_eval(ec),
          do_var(vc),
          pragma(prag) {}

    bool AllowEval() const { return allow_eval && do_eval && do_var; }
  };

 public:
  static inline Result<Value> Parse(std::string&&, Option&& _ = {});
  static inline Result<Value> ParseJSON(std::string&&);
  static inline Result<Value> ParseNamedMap(std::string&&);
  static inline Result<Value> ParseValue(std::string&&);

  static inline Result<Value> ParseNamedMap(const std::string_view& x) {
    return ParseNamedMap(std::to_string(x));
  }
  static inline Result<Value> ParseValue(const std::string_view& x) {
    return ParseValue(std::to_string(x));
  }
  static inline Result<Value> ParseJSON(const std::string_view& x) {
    return ParseJSON(std::to_string(x));
  }

  static inline Result<Value> ParseJSON(const char* s) {
    return ParseJSON(std::string(s));
  }
  static inline Result<Value> ParseNamedMap(const char* s) {
    return ParseNamedMap(std::string(s));
  }
  static inline Result<Value> ParseValue(const char* s) {
    return ParseValue(std::string(s));
  }

 public:
  const std::string& current_content() const {
    return p_stack_.back()->content;
  }
  const std::string& current_content_id() const {
    return p_stack_.back()->content_id;
  }

 private:
  // Parse the full document
  Result<Value> DoParseDocument(std::string&&, Option&&);

  // Parse the string as compact map grammar. Notes the leading ID of the
  // compact map will be stored into the map as key value pair of:
  //   "@name" : "the string goes here"
  Result<Value> DoParseNamedMap(std::string&&);

  // Helper for ParseValue function
  Result<Value> DoParseValue(std::string&&);

 private:
  // recursive descent parser ---------------------------------

  // utility function, used to abstract out the @(delimiter) grammar
  template <typename H, typename Delimiter, typename Terminator>
  Error ParseAt(H&&, Delimiter&&, Terminator&&);

  // Parse the object key, ie string or id based, returns a std::string
  inline Result<std::string> ParseObjectKey();
  inline Error Expect(base::_::Token);
  inline Error Expect(base::_::Token, base::_::Token);

  Error ParseCommaSeparatedKVList(Dictionary*, base::_::Token);

  // named-map, the parsing of compact map requires a look ahead to check
  // whether we have a following '(' or not. So the id has already been parsed
  Result<Value> ParseNamedObject(std::string&&);
  Error ParseNamedObject(Dictionary*, std::string&&);
  Error ParseNamedObjectEntryList(Dictionary*, base::_::Token);

  // map written as (id=1, plus=2) style
  Error ParseShortcutMap(Dictionary*);
  Result<Value> ParseShortcutMap();

  Error ParseArray(List*);
  Result<Value> ParseArray();

  Result<Value> ParseObject();
  Error ParseObject(Dictionary*);
  Error ParseObjectEntryList(Dictionary*);

  Error ParseImplicitStatement();
  Error ParseImplicitStatementList();
  Error ParseImplicitNamedMap(std::string&&);

  // The implicit-array and implicit-object needs a look ahead to know the rule
  // to reduce
  Error DoParseImplicitObject(std::string&&, bool);
  bool TryMerge(base::Value*, base::Value*) const;

  // Var scope, if eval is on
  Error ParseVar();

  // instructions
  Error ParseInstructionStatement();
  Result<Value> IncludeValue();
  Error IncludeStatement();
  Result<Value> DoInclude(bool);
  Result<Value> ParseInclude(std::string&&, std::string&&, bool);
  Error PragmaStatement();

  // TOS object
  Result<Value> ParseValue();
  Result<Value> ParseTOS();

 private:
  // report a comprehensitve error position information for diagnostic purpose
  std::string ErrorPosition() const;

  // Error handling
  inline Error Etoken(base::_::Token);
  inline Error Etoken(base::_::Token, base::_::Token);
  inline Error Eimplarr(const char*, ...);
  inline Error Egrammar(const char*, ...);
  inline Error Einstruction(const char*, const char*, ...);
  inline Error Eunknownins(const char*);

  // Check the include constraints, ie whether we have circular inclusion or
  // the maximum include nested size has reached
  Error CheckIncludeConstraints(const std::string&) const;

  // pointer to the current/tos context's scanner. Just a shortcuts, don't blame
  // me for the naming thing ...
  base::_::Scanner* L() { return &(p_stack_.back()->scanner); }

  inline void Enter(std::string&&, std::string&&);
  inline void Leave();

 private:
  // Context of the current parsing. This is really important since
  struct CurrentContext {
    std::string content_id;
    std::string content;
    base::_::Scanner scanner;
    CurrentContext(std::string&& cid, std::string&& c,
                   base::_::Scanner::Option&& opt = {})
        : content_id(std::move(cid)),
          content(std::move(c)),
          scanner(content, std::move(opt)) {}
  };

  Option option_{};
  std::vector<std::unique_ptr<CurrentContext>> p_stack_{};
  Value tos_object_{};
};

}  // namespace config
}  // namespace base

#include "jsonish-inl.h"

#endif  // _BASE_CONFIG_JSONISH_H_
