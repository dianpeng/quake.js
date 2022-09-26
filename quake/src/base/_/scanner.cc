#include "base/_/scanner.h"
#include "base/string-util.h"

namespace base {
namespace _ {

const char* GetTokenName(Token tk) {
  switch (tk) {
    // clang-format off
#define __(A, B) case A: return B;
    CONFIG_TOKEN_LIST(__)
#undef __  // __
    // clang-format on
    default:
      UNREACHABLE();
      return nullptr;
  }
}

Token Scanner::LexEval(bool is_mstring) {
  Token token;
  // lex eval is same as quoted string from here, all we need to do is to let it
  // LexStr and then modify the token
  if (is_mstring) {
    token = LexMStr();
  } else {
    DCHECK(c0_ == '"' || c0_ == '\'');
    token = LexStr();
  }

  if (token == kString) {
    return (token_ = kEval);
  } else {
    DCHECK(token == kError);
    auto msg = text_;
    text_ = "$eval failed when scanning code: " + msg;
    return token;
  }
}

/**
 ** Lexing string, the routine should support 3 different styles to ease the
 ** pain when writing super long strings. Notes the heredoc doesn't help since
 ** the whitespace rules sometimes mess up the original intention of string
 **
 **  1) "normal string"
 **  2) "normal string \
         concatenation"
 **  3) "a" \
 **     "b" \
 **     "c"
 **     The above concatenation will result in "abc"
 **/

Token Scanner::LexStr() {
  CHECK(c0_ == '\'' || c0_ == '"');
  const auto current_quote = c0_;
  text_.clear();

  // Scanning from the current position until we hit the a single quoted string
  // segment, ie "abc", this routine covers cases that user explicit want to
  // include mutliple lines
  auto scan_string_segment = [&](std::uint32_t quote) -> std::optional<Token> {
    while (cursor_ < length_) {
      if (!NextChar()) return Eutf8();
      if (c0_ == quote) {
        return {};
      } else if (c0_ == '\\') {
        if (cursor_ >= length_) {
          return Estr("string early EOFed");
        }
        if (!NextChar()) return Eutf8();

        switch (c0_) {
          case '"':
            text_.push_back('"');
            break;
          case '\\':
            text_.push_back('\\');
            break;
          case '/':
            text_.push_back('/');
            break;
          case 'b':
            text_.push_back('\b');
            break;
          case 'f':
            text_.push_back('\f');
            break;
          case 'n':
            text_.push_back('\n');
            break;
          case 'r':
            text_.push_back('\r');
            break;
          case 't':
            text_.push_back('\t');
            break;
          case 'v':
            text_.push_back('\v');
            break;
          case 'u':
            // TODO(dpeng): Support unicode escape ? Currently we don't allow
            //   utf16 been used and also we don't suggest using utf16 due to
            //   its incompatible with current web standard.
            return Ena("utf16/unicode escape not supported");

          case '\n':
            // String concatenation
            break;
          default:
            // Notes(dpeng), specialized extension for escaping single quote,
            //   if the string is quoted via single quote, then we optionally
            //   allow single quote to be escaped here.
            if (quote == '\'' && c0_ == '\'') {
              text_.push_back('\'');
              break;
            }

            return Eescape();
        }
      } else {
        switch (c0_) {
          case '\b':
          case '\r':
          case '\n':
          case '\v':
          case '\t':
          case '\f':
            return Estr("invisiable character should be escapped");
          default:
            break;
        }

        // TODO(dpeng): This can be optimized with previous decoding's cache,
        // but
        //   I don't know whether it worths the efforts though
        CHECK(AppendUtf8(c0_, &text_));
      }
    }

    return Estr("string early EOFed");
  };

  auto is_whitespace = [this]() {
    switch (c0_) {
      case ' ':
      case '\t':
      case '\b':
      case '\f':
      case '\v':
      case '\r':
      case '\n':
        return true;
      default:
        return false;
    }
  };

  // Probe until we hit \\n sequences otherwise just stop at the first none
  // recognized token and treat the rest as unreaded tokens. This is used to
  // help us to lookahead to find out whether we need to do concatenation
  // If an empty returned, then error happened and already set, otherwise true
  // indicate we find one and false indicate that we should just bailout
  auto probe_concate = [&]() -> std::optional<bool> {
    while (cursor_ < length_) {
      if (!NextChar()) {
        Eutf8();
        return {};
      }
      if (is_whitespace()) continue;
      if (c0_ == '\\' && EqFrom('\n')) return true;
      break;
    }
    PrevChar();
    return false;
  };

  // Helper to let us to probe the next '"' Notes, this function will just
  // skip the whitespaces and try to find the quote and anyother token will
  // just make it stop since the lexer should be back to normal lexing stage
  auto probe_next_quote = [&]() -> std::optional<bool> {
    while (cursor_ < length_) {
      if (!NextChar()) {
        Eutf8();
        return {};
      }
      if (is_whitespace()) continue;
      if (c0_ == '"' || c0_ == '\'') return true;
      break;
    }
    PrevChar();
    return false;
  };

  // (0) scan the first quoted string. Notes if returns a fulfilled option
  //     then means an error happened
  if (auto e = scan_string_segment(current_quote); e) {
    return *e;
  }

  // (1) try to scan as much segment as possible for concatenation
  while (cursor_ < length_) {
    // probing for the concatenation
    if (auto e = probe_concate(); !e) {
      return token_;
    } else {
      if (!*e) {
        break;
      }
    }

    // start to probing the next quote, otherwise bailout
    if (auto e = probe_next_quote(); !e) {
      return token_;
    } else {
      if (!*e) {
        break;
      }
    }

    auto quote = c0_;
    if (auto e = scan_string_segment(quote); e) {
      return *e;
    }
  }

  return Y(kString);
}

/**
 * Lexing MStr, ie multiple line string. The MStr section is been recognized
 * as following:
 *
 *   a token starts with at least ```
 *
 * To identify the ending of the string, we should learn how many consequent
 * backtick is needed to finish the sequences. User is capable of writing
 * as many as they want to demonstrate the uniqueness from the surrounding
 * literal
 */

Token Scanner::LexMStr() {
  auto done = false;
  text_.clear();
  std::size_t tag_size = 3;

  // Helper to peek the next token from the current sequences
  auto peek = [&](std::size_t offset)
      -> std::optional<std::pair<std::uint32_t, std::size_t>> {
    if (offset >= length_) {
      return {};
    }
    std::uint32_t ncp = 0;
    auto v = DecodeOneUtf8(source_ + offset, (length_ - offset), &ncp);
    if (v) {
      return std::make_pair(ncp, v.value());
    } else {
      return {};
    }
  };

  // Helper to testing from current position c0_, whether we see the needed
  // sequences to stop the MString. Notes assert : c0_ == ')'
  auto try_ending_seq = [&]() -> std::optional<bool> {
    CHECK(c0_ == ')');
    auto offset = cursor_;
    for (std::size_t i = 0; i < tag_size; ++i) {
      if (auto maybe = peek(offset); !maybe) {
        return {};
      } else {
        if (auto [code, len] = Unwrap(maybe); code == '`') {
          offset += len;
        } else {
          return false;
        }
      }
    }
    cursor_ = offset;
    return true;
  };

  // (0) Finding out the tagging after the initial ```*(
  {
    auto see_start = false;
    while (cursor_ < length_) {
      if (!NextChar()) return Eutf8();
      if (c0_ == '`') {
        tag_size++;
      } else if (c0_ == '(') {
        see_start = true;
        // We do require a ( to indicate the start of the content
        break;
      } else {
        return Etoken("invalid token from ```, expect a ( to start the string");
      }
    }
    if (!see_start) {
      return Etoken("invalid token from ```, expect a ( to start the string");
    }
  }

  // (1) Start to scan all the multi-line string until we hit the end
  while (cursor_ < length_) {
    if (!NextChar()) return Eutf8();
    if (c0_ == ')') {
      if (auto maybe_ending = try_ending_seq(); !maybe_ending) {
        return Eutf8();
      } else {
        if (*maybe_ending) {
          done = true;
          break;
        }
      }
    }

    CHECK(AppendUtf8(c0_, &text_));
  }

  if (!done) {
    return Estr("string early EOFed");
  } else {
    return Y(kString);
  }
}

Token Scanner::LexNumber() {
  auto got_dot = false;
  auto got_exp = false;

  std::string buf;
  buf.reserve(32);

  auto is_digit = [](std::uint32_t c) -> bool {
    return (c >= '0' && c <= '9');
  };

  // helper, read from the current places until we see a none digits, or EOFed.
  auto read_ints = [&](std::size_t at_least) -> std::optional<Token> {
    std::size_t cnt = 0;
    while (cursor_ < length_) {
      if (!NextChar()) return Eutf8();
      if (is_digit(c0_)) {
        ++cnt;
        buf.push_back(static_cast<char>(c0_));
      } else {
        PrevChar();
        break;
      }
    }
    if (cnt < at_least) {
      return Enum("expecting digits");
    }
    return {};
  };

  // add the first character belongs to number sequences
  buf.push_back(static_cast<char>(c0_));

  // (0) leading integer parts until we hit EOF-number or . or |e|E|
  if (auto x = read_ints(0); x) {
    return x.value();
  }

  // (1) try optional decimal part
  if (PeekChar()) {
    if (peek_c_ == '.') {
      got_dot = true;
      buf.push_back('.');
      ConsumePeekChar();
      if (auto x = read_ints(1); x) {
        return x.value();
      }
    }
  }

  // (2) try optional exponential part
  if (PeekChar()) {
    if (peek_c_ == 'e' || peek_c_ == 'E') {
      got_exp = true;
      buf.push_back(static_cast<char>(peek_c_));
      ConsumePeekChar();
      if (PeekChar()) {
        if (peek_c_ == '+' || peek_c_ == '-') {
          buf.push_back(static_cast<char>(peek_c_));
          ConsumePeekChar();
        }
      }
      if (auto x = read_ints(1); x) {
        return x.value();
      }
    }
  }

  auto to_real = [&]() -> Token {
    if (auto v = LexicalCast<double>(buf); v) {
      real_ = v.value();
      return Y(kReal);
    }
    return Enum("invalid real number: %s", buf.c_str());
  };

  auto to_int = [&]() -> Token {
    if (auto v = LexicalCast<std::int64_t>(buf); v) {
      integer_ = v.value();
      return Y(kInt);
    }
    return Enum("invalid int number: %s", buf.c_str());
  };

  // (3) convert to integer/double as we expected.
  if (!got_exp) {
    if (got_dot) {
      return to_real();
    } else {
      return to_int();
    }
  } else {
    // Always convert to real number regardlessly if user uses scientific method
    // to represent the number literal
    return to_real();
  }
}

Token Scanner::LexIdOrKeyword() {
  switch (c0_) {
    case 't':
      if (Match("rue")) return Y(kTrue);
      break;
    case 'f':
      if (Match("alse")) return Y(kFalse);
      break;
    case 'n':
      if (Match("ull")) return Y(kNull);
      break;
    case 'v':
      if (allow_var_) {
        if (Match("ar")) return Y(kVar);
      }
      break;
    default:
      break;
  }

  // Notes(dpeng):
  // Identifier are all ascii-characters, so just do push back here and we don't
  // bother using AppendUtf8 function for now

  // Try to lex the thing as identifier
  if (IsIdIChar(c0_)) {
    text_.clear();
    text_.push_back(static_cast<char>(c0_));

    while (cursor_ < length_) {
      if (!NextChar()) return Eutf8();
      if (IsIdRChar(c0_)) {
        text_.push_back(static_cast<char>(c0_));
      } else {
        PrevChar();
        break;
      }
    }

    return Y(kId);
  }
  return Etoken("unknown token, not id or keywords");
}

bool Scanner::SkipComment(CommentType t) {
  if (t == kCommentLineBreak) {
    while (cursor_ < length_) {
      if (!NextChar()) return false;
      if (c0_ == '\n') break;
    }
    return true;
  } else {
    auto done = false;
    while (cursor_ < length_) {
      if (!NextChar()) return false;
      if (c0_ == '*' && EqFrom('/')) {
        done = true;
        break;
      }
    }

    if (done) {
      return true;
    } else {
      return Ecmt("expect '*/' to close comment");
    }
  }
}

std::string Scanner::Highlight() const {
  // (0) get the cursor coordinates, kind of complicated here.
  std::size_t raw = 1;
  std::size_t col = 1;

  for (std::size_t i = 0; i < cursor_;) {
    std::uint32_t cp = 0;
    auto v = DecodeOneUtf8(source_ + i, (cursor_ - i), &cp);
    if (!v) {
      // TODO(dpeng): We could just crash here since the decoding should never
      //   failed until the cursor_ part, not sure what to do now.
      break;
    }
    if (cp == '\n') {
      raw = 1;
      col++;
    } else {
      raw++;
    }

    i += v.value();
  }

  // (1) get a sample string related around the error part
  std::size_t start, end;
  if (cursor_ >= 1) {
    start = cursor_ - 1;
  } else {
    start = 0;
  }

  end = cursor_ + 1;
  if (end >= length_) {
    end = length_;
  }
  auto sample = HighlightString(data_, start, end, 64);

  return Fmt("around(%zu:%zu): --------------\n%s\n", col, raw, sample.c_str());
}

Token Scanner::LexAttr() {
  DCHECK(c0_ == '@');
  text_.clear();
  while (cursor_ < length_) {
    if (!NextChar()) return Eutf8();
    if (IsIdRChar(c0_)) {
      text_.push_back(static_cast<char>(c0_));
    } else {
      PrevChar();
      break;
    }
  }
  if (text_.empty()) {
    return Eattr("empty attribute name");
  } else {
    return Y(kAttribute);
  }
}

Token Scanner::Next() {
  while (cursor_ < length_) {
    if (!NextChar()) return Eutf8();

    switch (c0_) {
      // just be robust, in case we call NextChar on exahusted sources, then
      // NextChar will just place 0 to c0_ as indicator of eof data
      case 0:
        goto done;

      // whitespaces
      case '\r':
      case '\n':
      case '\b':
      case '\v':
      case ' ':
      case '\t':
        break;

      // concatenation
      case '\\':
        if (EqFrom('\n')) {
          break;
        } else {
          return Etoken("unknown token, do you mean concatenation?");
        }

      // tokens
      case '$':
        // Eval syntax, ie $"", the quoted string contained includes a
        // expression that needs to be evaluated immediate at the current
        // context. It is the parser's responsibility to invoke the |eval|
        // operation though
        if (allow_eval_ && (cursor_ + 1 < length_)) {
          if (!NextChar()) return Eutf8();
          if (c0_ == '"' || c0_ == '\'') return LexEval(false);
          if (c0_ == '`' && EqFrom('`', '`')) return LexEval(true);
        }
        return Etoken("unknown token, leading by $");

      case '{':
        return Y(kObjectStart);
      case '}':
        return Y(kObjectEnd);
      case '[':
        return Y(kArrayStart);
      case ']':
        return Y(kArrayEnd);
      case '(':
        return Y(kLPar);
      case ')':
        return Y(kRPar);
      case '@':
        return LexAttr();
      case ',':
        return Y(kComma);
      case ':':
        return Y(kColon);
      case ';':
        return Y(kSemicolon);
      case '=':
        return Y(kAssign);
      case '+':
        if (EqFrom('=')) {
          return Y(kAppend);
        } else {
          return Etoken("unknown token, do you mean += ?");
        }
        break;

      case '.':
        return Y(kDot);
      case '%':
        return Y(kPercent);
      case '!':
        return Y(kEmotion);
      case '?':
        return Y(kQuestion);
      case '|':
        return Y(kVBar);

      // literals
      case '\'':
      case '"':
        return LexStr();

      case '`':
        if (EqFrom('`', '`')) {
          return LexMStr();
        } else {
          return Etoken("do you mean ```?");
        }
        break;

      // number
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
        return LexNumber();

      // comment
      case '#':
        if (!SkipComment(kCommentLineBreak)) {
          return kError;
        }
        break;

      case '/':
        if (EqFrom('/')) {
          if (!SkipComment(kCommentLineBreak)) {
            return kError;
          }
        } else if (EqFrom('*')) {
          if (!SkipComment(kCommentScope)) {
            return kError;
          }
        } else {
          return Etoken("do you mean // for comment?");
        }
        break;

      default:
        return LexIdOrKeyword();
    }
  }

done:
  return Y(kEof);
}

}  // namespace _
}  // namespace base
