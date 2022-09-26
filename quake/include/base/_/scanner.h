#ifndef _BASE___SCANNER_H_
#define _BASE___SCANNER_H_

#include "base/check.h"
#include "base/result.h"
#include "base/value.h"
#include "base/util.h"

namespace base {
namespace _ {

#define CONFIG_TOKEN_LIST(__)                 \
  __(kInt, "int")                             \
  __(kReal, "real")                           \
  __(kString, "string")                       \
  __(kTrue, "true")                           \
  __(kFalse, "false")                         \
  __(kNull, "null")                           \
  __(kId, "id")                               \
  __(kAttribute, "attribute")                 \
  __(kAssign, "assign")                       \
  __(kAppend, "append")                       \
  __(kColon, "colon")                         \
  __(kComma, "comma")                         \
  __(kSemicolon, "semicolon")                 \
  __(kObjectStart, "{")                       \
  __(kObjectEnd, "}")                         \
  __(kArrayStart, "[")                        \
  __(kArrayEnd, "]")                          \
  __(kLPar, "(")                              \
  __(kRPar, ")")                              \
  __(kDot, ".")                               \
  __(kEval, "<eval>")                         \
  __(kPercent, "%")                           \
  __(kQuestion, "?")                          \
  __(kVar, "<var>")                           \
  __(kEmotion, "!")                           \
  /** only used by the search path for now */ \
  __(kVBar, "|")                              \
  __(kEof, "<eof>")                           \
  __(kError, "<error>")

enum Token {
// clang-format off
#define __(A, B) A,
  CONFIG_TOKEN_LIST(__)
#undef __  // __
  kSizeOfTokens
  // clang-format on
};

const char* GetTokenName(Token);

// It is a general purpose scanner, that is been used by manly parts of the code
// in Base instead of logging, which must be dependency free. It covers most of
// the token we need to form different types of syntax, ie Json, search path,
// etc ...
class Scanner {
 public:
  // Control some aspects of Scanner. Mostly been used by config::Jsonish parser
  // for generating different types of syntax tree under different requirements
  struct Option {
    // Whehter allow eval syntax
    bool allow_eval;

    // Whether allow |var| syntax, ie var been treated as keywords instead of a
    // normal syntax error
    bool allow_var;

    Option(bool ae = false, bool av = false) : allow_eval(ae), allow_var(av) {}
  };

  inline explicit Scanner(const std::string&, Option&& opt = {});

  Token Next();

  // Returns a error string when the previous Next causes a kError been returned
  inline std::optional<std::string> GetLastError() const;

  // Get highlighted string
  std::string Highlight() const;

 public:
  Token token() const { return token_; }
  const std::string& text() const { return text_; }
  std::int64_t integer() const { return integer_; }
  double real() const { return real_; }
  const char* token_name() const { return GetTokenName(token()); }

 private:
  // Lexing related functions
  //
  // Decoding
  inline bool NextChar();
  inline void PrevChar();

  inline bool PeekChar();
  inline void ConsumePeekChar();

  inline bool EqFrom(std::uint32_t);
  inline bool EqFrom(std::uint32_t, std::uint32_t);

  inline bool IsIdIChar(std::uint32_t) const;
  inline bool IsIdRChar(std::uint32_t) const;

  // Misc
  Token Y(Token t) { return (token_ = t); }

  template <std::size_t N>
  inline bool Match(const char (&)[N]);

  // Errors
  inline Token Eutf8();
  inline Token Etoken(const char*);
  inline Token Estr(const char*);
  inline Token Eescape();
  inline Token Ena(const char*);
  inline Token Ecmt(const char*);
  inline Token Enum(const char*, ...);
  inline Token Eattr(const char*);

 private:
  Token LexAttr();
  Token LexStr();
  Token LexMStr();
  Token LexEval(bool is_mstring);
  Token LexNumber();
  Token LexIdOrKeyword();

  // Skip the comment, we support following types of comment :
  //   1) # starts line break comment
  //   2) // starts line break comment
  //   3) scopped comment with /* and */
  enum CommentType { kCommentLineBreak, kCommentScope };
  bool SkipComment(CommentType);

 private:
  const std::string& data_;
  const char* source_;
  std::size_t length_;
  std::size_t cursor_;

  // Current codepoint
  std::uint32_t c0_;
  std::uint32_t c0_size_;

  // Peek codepoint
  std::uint32_t peek_c_;
  std::uint32_t peek_size_;

  // Lexeme/Attributes
  Token token_;
  std::string text_;
  std::int64_t integer_;
  double real_;

  // Option
  bool allow_eval_;
  bool allow_var_;

  DISALLOW_COPY_AND_ASSIGN(Scanner);
};

}  // namespace _
}  // namespace base

#include "scanner-inl.h"

#endif  // _BASE___SCANNER_H_
