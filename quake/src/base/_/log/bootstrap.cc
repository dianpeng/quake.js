#include "bootstrap.h"

#include <time.h>

#include <fstream>
#include <thread>

#define ESCAPE_LIST(__)  \
  __('t', '\t', "\\t")   \
  __('v', '\v', "\\v")   \
  __('r', '\r', "\\r")   \
  __('b', '\b', "\\b")   \
  __('n', '\n', "\\n")   \
  __('\'', '\'', "\\\'") \
  __('\\', '\\', "\\\\") \
  __('\"', '\"', "\\\"")

namespace base {
namespace log {
namespace _ {

// This function tries to show the highlighted part of scanning text as
// following :
//
//   <text before highlight> ... <highlighted> ... <text after highlight>
std::string Scanner::GetHighlight() const {
  static const std::size_t kHalfSize = 64;
  std::string s1, s2, payload;

  // First half
  {
    std::size_t start;
    if (cursor_ > kHalfSize) {
      start = cursor_ - kHalfSize;
    } else {
      start = 0;
    }

    std::size_t end = start + kHalfSize;
    if (end > cursor_) {
      end = cursor_;
    }

    s1 = input_.substr(start, (end - start));
  }

  // Payload, if nothing has been scanned, we still put some valid character
  // here instead of letting invisiable character shows up which is kind of
  // wired in many cases.
  {
    if (cursor_ == input_.size()) {
      payload = "<eof>";
    } else {
      payload = input_[cursor_];
    }
  }

  // Second half
  {
    std::size_t start = cursor_ + 1;
    std::size_t end = start + kHalfSize;
    if (start >= input_.size()) {
      s2 = "<eof>";
    } else {
      if (end > input_.size()) {
        end = input_.size();
      }
      s2 = input_.substr(start, (end - start));
    }
  }

  return Fmt("%s ... %s ... %s", s1.c_str(), payload.c_str(), s2.c_str());
}

int Scanner::TryLexKeywordOrId(char c) {
  switch (c) {
    case 't':
      if (MatchKeyword("rue")) {
        return Y(4, kTrue);
      }
      break;
    case 'f':
      if (MatchKeyword("alse")) {
        return Y(5, kFalse);
      }
      break;
    case 'n':
      if (MatchKeyword("ull")) {
        return Y(4, kNull);
      }
      break;
    default:
      break;
  }

  return TryKeyword(c);
}

int Scanner::TryKeyword(char c) {
  if (!IsIdInitChar(c)) {
    return E("unknown character: %c", c);
  }

  lexeme_.text.clear();
  lexeme_.text.push_back(c);

  for (cursor_++; cursor_ < input_.size(); cursor_++) {
    auto nc = input_[cursor_];
    if (IsIdRestChar(nc)) {
      lexeme_.text.push_back(nc);
    } else {
      break;
    }
  }
  return (token_ = kId);
}

int Scanner::LexString(char quote) {
  auto pusher = [this](char c) {
    cursor_++;
    lexeme_.text.push_back(c);
  };

  lexeme_.text.clear();

  for (cursor_++;; cursor_++) {
    if (cursor_ == input_.size()) {
      return E("early termination of string literal");
    }

    auto c = input_[cursor_];
    if (c == '\\') {
      if ((cursor_ + 1) == input_.size()) {
        return E("the quoted string is terminated early");
      }
      auto nc = input_[cursor_ + 1];
      switch (nc) {
#define __(A, B, C) \
  case A:           \
    pusher(B);      \
    break;
        ESCAPE_LIST(__)
#undef __  // __

        default:
          return E("invalid escape character: \\%c", nc);
      }
    } else if (c == quote) {
      ++cursor_;
      break;
    } else {
      lexeme_.text.push_back(c);
    }
  }

  return (token_ = kStr);
}

int Scanner::LexNumber(char c) {
  std::string buffer;
  buffer.push_back(c);
  auto has_dot = false;

  for (cursor_++; cursor_ < input_.size(); cursor_++) {
    auto c = input_[cursor_];
    switch (c) {
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
        buffer.push_back(c);
        break;
      case '.':
        if (has_dot) {
          goto done;
        } else {
          has_dot = true;
          buffer.push_back('.');
        }
        break;
      default:
        goto done;
    }
  }

done:
  if (has_dot) {
    if (auto v = LexicalCast<double>(buffer); v) {
      lexeme_.real_value = v.value();
      return (token_ = kReal);
    } else {
      return E("%s is not valid double", buffer.c_str());
    }
  } else {
    if (auto v = LexicalCast<std::int64_t>(buffer); v) {
      lexeme_.int_value = v.value();
      return (token_ = kInt);
    } else {
      return E("%s is not valid int64", buffer.c_str());
    }
  }
}

int Scanner::Next() {
  for (; cursor_ < input_.size();) {
    auto c = input_[cursor_];
    switch (c) {
      case '\t':
      case '\r':
      case '\n':
      case '\b':
      case ' ':
      case '\v':
        cursor_ += 1;
        continue;

      case '(':
        return Y(1, kLPar);
      case ')':
        return Y(1, kRPar);
      case '[':
        return Y(1, kLSqr);
      case ']':
        return Y(1, kRSqr);
      case ',':
        return Y(1, kComma);
      case ';':
        return Y(1, kSemicolon);
      case '=':
        return Y(1, kEq);

      // Notes, this lexer is different from a language lexer, a language
      // lexer will not treat - as part of the number since '- number' is
      // been handled by the parser as unary expression. For us, this is
      // not needed since we don't support expression so no ambigiouty.
      case '-':
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
        return LexNumber(c);

      case '"':
      case '\'':
        return LexString(c);

      default:
        return TryLexKeywordOrId(c);
    }
  }
  return Y(0, kEof);
}

bool Parser::E(const char* msg) {
  error_ = scanner_.GetHighlight();
  error_ += "): ";
  error_ += msg;
  return false;
}

bool Parser::ParseEntryList(Table* t) {
  scanner_.Next();

  for (;;) {
    auto token = scanner_.token();
    if (token == Scanner::kEof) {
      break;
    }
    std::string name;
    Attributes attribute;
    if (!ParseEntry(&name, &attribute)) {
      return false;
    }
    t->insert(std::make_pair(std::move(name), std::move(attribute)));
    if (!scanner_.Check(Scanner::kSemicolon)) {
      return E("expect a \";\"");
    }
  }
  return true;
}

bool Parser::ParseEntry(std::string* name, Attributes* attr) {
  if (scanner_.token() != Scanner::kId) {
    return false;
  }
  *name = scanner_.lexeme().text;
  if (auto tk = scanner_.Next(); tk == Scanner::kLPar) {
    scanner_.Next();
    while (scanner_.token() != Scanner::kRPar) {
      std::string name;
      Value value;
      if (scanner_.token() != Scanner::kId) {
        return E("expect a identifier here to represent key of attribute");
      }
      name = scanner_.lexeme().text;

      if (!scanner_.Expect(Scanner::kEq)) {
        return E("expect a \"=\"");
      }
      scanner_.Next();

      if (!ParseValue(&value)) {
        return false;
      }
      attr->insert(std::make_pair(std::move(name), std::move(value)));

      if (scanner_.token() == Scanner::kComma) {
        scanner_.Next();
      } else if (scanner_.token() != Scanner::kRPar) {
        return E("expect a \",\" or \")\"");
      }
    }
    scanner_.Next();
  }
  return true;
}

bool Parser::ParseSubValue(Value* v) {
  scanner_.Next();
  if (!ParseValue(v)) {
    return false;
  }
  if (!scanner_.Check(Scanner::kRPar)) {
    return E("expect a \")\" to close a sub-value");
  }
  return true;
}

bool Parser::ParseList(Value* v) {
  auto l = std::make_shared<List>();
  scanner_.Next();
  while (scanner_.token() != Scanner::kRSqr) {
    Value v;
    if (!ParseValue(&v)) {
      return false;
    }
    l->push_back(std::move(v));
    if (scanner_.token() == Scanner::kComma) {
      scanner_.Next();
    } else if (scanner_.token() != Scanner::kRSqr) {
      return E("expect a \",\" or \"]\"");
    }
  }

  *v = std::move(l);
  scanner_.Next();
  return true;
}

bool Parser::ParseValue(Value* v) {
  switch (scanner_.token()) {
    case Scanner::kTrue:
      *v = true;
      break;
    case Scanner::kFalse:
      *v = false;
      break;
    case Scanner::kInt:
      *v = scanner_.lexeme().int_value;
      break;
    case Scanner::kReal:
      *v = scanner_.lexeme().real_value;
      break;
    case Scanner::kStr:
      *v = scanner_.lexeme().text;
      break;
    case Scanner::kLPar:
      return ParseSubValue(v);
    case Scanner::kLSqr:
      return ParseList(v);
    case Scanner::kNull:
      *v = Parser::Null{};
      break;
    default:
      return E("expect a valid value here");
  }

  scanner_.Next();
  return true;
}

// This function tries to dump the last word out and crashed
void Urgent(bool die, const char* file, int line, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = Fmt("@(%s, %d) URGENT]: %s", file, line, FmtV(fmt, vl).c_str());
  va_end(vl);

  auto st = GetStackTrace();

#ifndef BASE_LOG_URGENT_NO_FILE
  static const char* kSilent = "BASE_LOG_URGENT_SILENT";
  static const char* kFilename = "BASE_LOG_URGENT_FILENAME";

  // Create a specialized file and dump the last word out. Notes, here just use
  // std::fstream to create the file since all we need to do is to dump the
  // stuff out. We also gonna append the timestamp at end of the file out.
  if (!GetEnv<std::string>(kSilent)) {
    std::string filename;

    if (auto v = GetEnv<std::string>(kFilename); v) {
      filename = v.value();
    } else {
      filename = Fmt("urgent.%d.%u", GetPid(),
                     static_cast<std::uint32_t>(NowInMilliseconds()));
    }
    if (std::fstream f{filename.c_str(), std::ios::out | std::ios::trunc}; f) {
      f << msg << '\n' << st << std::endl;
    }
  }
#endif  // BASE_LOG_URGENT_NOT_FILE

  // Dump to std::cerr
  std::cerr << msg << '\n' << st << std::endl;

  // If need to die, then just die here.
  if (die) {
    DieNow();
  }
}

void GetNowAsLogTime(LogTime* lt) {
#if defined(OS_POSIX) || defined(OS_MAC)
  std::uint64_t now = static_cast<std::uint64_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count());

  struct ::tm tm;

  // Notes(dpeng): This places uses an implicit assumption of time() function
  //   always returns a second level resolution since epoch. This is a posix
  //   standard.
  time_t t = static_cast<time_t>(now / 1000);
  ::localtime_r(&t, &tm);

  lt->year = static_cast<std::uint32_t>(1900 + tm.tm_year);
  lt->month = static_cast<std::uint8_t>(1 + tm.tm_mon);
  lt->day = static_cast<std::uint8_t>(tm.tm_mday);
  lt->hour = static_cast<std::uint8_t>(tm.tm_hour);
  lt->minute = static_cast<std::uint8_t>(tm.tm_min);
  lt->second = static_cast<std::uint8_t>(tm.tm_sec);
  lt->msecond = static_cast<std::uint64_t>(now - 1000 * (now / 1000));
#else
#error "OS not supported"
#endif  // OS_POSIX || OS_MAC
}

}  // namespace _
}  // namespace log
}  // namespace base

#if defined(OS_LINUX)

#include <sys/syscall.h>
#include <unistd.h>

#else
#endif  // OS_LINUX

namespace base {
namespace log {
namespace _ {

std::uint32_t GetPid() {
#if defined(OS_LINUX)
  return static_cast<std::uint32_t>(::getpid());
#else
  return 0;
#endif  // OS_LINUX
}

std::uint32_t GetTid() {
#if defined(OS_LINUX) && defined(SYS_gettid)
  auto tid = syscall(SYS_gettid);
  return static_cast<std::uint32_t>(tid);
#else
  std::stringstream f;
  f << std::this_thread::get_id();
  auto v = LexicalCast<std::int64_t>(f.str());
  return v ? static_cast<std::uint32_t>(v.value()) : 0;
#endif  // OS_LINUX && SYS_gettid
}

}  // namespace _
}  // namespace log
}  // namespace base
