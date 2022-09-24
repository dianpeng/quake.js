#ifndef _BASE_SRC___LOG_BOOTSTRAP_H_
#define _BASE_SRC___LOG_BOOTSTRAP_H_
#include <base/common.h>

// For base::log::Severity
#include <base/log.h>

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace base {
namespace log {
namespace _ {

// This file contains a very simple log DSL parser. It is totally standalone
// since the log interface is the base for everything in |base| and should be
// initialized at very first. We want to have something called 0 config but
// still allow user to do flexible config setup. The kBootstrap contains a DSL
// config which will use the following parser.
class Scanner {
 public:
  enum {
    kId = 0,
    kLPar,
    kRPar,
    kLSqr,
    kRSqr,
    kComma,
    kSemicolon,
    kEq,
    kStr,
    kTrue,
    kFalse,
    kNull,
    kReal,
    kInt,
    kError,
    kEof
  };

  struct Lexeme {
    std::string text;
    std::int64_t int_value;
    double real_value;
  };

 public:
  inline explicit Scanner(const std::string&);

  int Next();
  bool Expect(int t) { return (Next() == t); }
  inline bool Check(int);
  int token() const { return token_; }
  const Lexeme& lexeme() const { return lexeme_; }

  // Get a highlight string of scanning part, mainly used for diagnostic purpose
  std::string GetHighlight() const;

 private:
  int Y(std::size_t, int);
  int E(const char*, ...);

  int LexNumber(char);
  int LexString(char);
  int TryLexKeywordOrId(char);

  template <std::size_t kSize>
  bool MatchKeyword(const char (&t)[kSize]) const;

  int TryKeyword(char);

  inline bool IsIdInitChar(char) const;
  inline bool IsIdRestChar(char) const;

 private:
  int token_;
  Lexeme lexeme_;
  std::size_t cursor_;
  std::string input_;
};

// The parser will try to parse the following grammer. The paser will generate
// a AST like structure for later manipulation. The none official grammaer is
// been represented as BNF as following, EMPTY is represented as none :
//
// all := entry-list
// entry-list := EMPTY | entry (';' entry)*
// entry := ID '(' attribute-list ')'
// attribute-list := EMPTY | attribute (',' attribute)*
// attribute := ID '=' value
// value := STRING | INTEGER | REAL | TRUE | FALSE | list
// lisrt := '[' list-entry-list ']'
// list-entry-list := EMPTY | value (',' value)*
class Parser {
 public:
  struct Null {};
  struct List;

  // Order matters, matches the std::variant index
  enum { kNull = 0, kInt, kReal, kString, kBoolean, kList };

  // Notes(dpeng):
  //   Before C++20 or GCC10, there's an issue with std::variant conversion,
  //   basically if you write following:
  //
  //     std::variant<std::string, bool> a{"bc"};
  //
  //   will make a holds bool instead of string, unless specified explicitly
  //   use std::string as constructor. This has been fixed in C++20.
  using Value = std::variant<Null, std::int64_t, double, std::string, bool,
                             std::shared_ptr<List>>;

  struct List : public std::vector<Value> {};
  using Attributes = std::map<std::string, Value>;
  using Table = std::map<std::string, Attributes>;

 public:
  // Due to the conversion rules of C++, directly puting integer number literal
  // into Value is kind of complicated. The following functions does factory
  // job for user and guarantee some sort of conversion rules
  static Value Mk(bool v) { return Value{v}; }
  static Value Mk(const std::string& v) { return Value{std::string{v}}; }
  static Value Mk(const char* v) { return Value{std::string{v}}; }
  template <typename T>
  static Value Mk(const T&);

  // Helper function to safely extract specific type from Value. Failure with
  // a empty std::optional return
  template <typename T>
  static std::optional<T> Get(const Value&);

  template <typename T>
  static std::optional<T> Get(const Attributes&, const char*);

 public:
  explicit Parser(const std::string& input) : scanner_(input), error_() {}

  // Parse the specified information into Table. If an error happened, then
  // just return a none-empty string to represent the error information.
  std::string Parse(Table*);
  std::string ParseItem(std::string*, Attributes*);

 private:
  bool ParseEntryList(Table*);
  bool ParseEntry(std::string*, Attributes*);
  bool ParseValue(Value*);
  bool ParseSubValue(Value*);
  bool ParseList(Value*);
  bool E(const char*);

 private:
  Scanner scanner_;
  std::string error_;
};

// An urgent log which prints the
void Urgent(bool, const char*, int, const char*, ...);

inline const char* GetSeverityTag(Severity);
inline const char* GetSeverityName(Severity);

struct LogTime {
  std::uint16_t year;
  std::uint8_t month;
  std::uint8_t day;
  std::uint8_t hour;
  std::uint8_t minute;
  std::uint8_t second;
  std::uint64_t msecond;
};
void GetNowAsLogTime(LogTime*);

std::uint32_t GetPid();
std::uint32_t GetTid();

// This macro is only visible inside of the log module
#define UFATAL(FMT, ...) Urgent(true, __FILE__, __LINE__, (FMT), __VA_ARGS__)
#define UWARNING(FMT, ...) Urgent(false, __FILE__, __LINE__, (FMT), __VA_ARGS__)

}  // namespace _
}  // namespace log
}  // namespace base

#include "bootstrap-inl.h"

#endif  // _BASE_SRC___LOG_BOOTSTRAP_H_
