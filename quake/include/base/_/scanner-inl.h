#ifndef _BASE___SCANNER_INL_H_
#define _BASE___SCANNER_INL_H_
#include <base/common.h>
#include <base/_/utf8-internal.h>

namespace base {
namespace _ {

inline std::optional<std::size_t> DecodeOneUtf8(const void* data,
                                                std::size_t size,
                                                std::uint32_t* oc) {
  auto state = utf8::State::kAccept;
  std::uint32_t o = 0;
  std::size_t off = 0;
  auto bbuffer = static_cast<const std::uint8_t*>(data);

  while (off < size) {
    utf8::Decode(bbuffer[off], &state, &o);
    ++off;

    switch (state) {
      case utf8::State::kAccept:
        *oc = o;
        return off;
      case utf8::State::kReject:
        return {};
      default:
        break;
    }
  }

  // failed with decoding a single byte
  return {};
}

inline bool Scanner::NextChar() {
  if (length_ == cursor_) {
    c0_ = 0;
    c0_size_ = 0;
    return true;
  }

  if (auto len = DecodeOneUtf8(source_ + cursor_, (length_ - cursor_), &c0_);
      len) {
    c0_size_ = len.value();
    cursor_ += len.value();
    return true;
  }
  return false;
}

inline bool Scanner::PeekChar() {
  if (length_ == cursor_) {
    peek_c_ = 0;
    peek_size_ = 0;
    return true;
  }

  if (auto len =
          DecodeOneUtf8(source_ + cursor_, (length_ - cursor_), &peek_c_);
      len) {
    peek_size_ = len.value();
    return true;
  }
  return false;
}

inline void Scanner::PrevChar() {
  DCHECK(c0_size_);
  cursor_ -= c0_size_;
  c0_size_ = 0;
}

inline void Scanner::ConsumePeekChar() {
  DCHECK(peek_size_);
  cursor_ += peek_size_;
  peek_size_ = 0;
}

inline bool Scanner::EqFrom(std::uint32_t a) {
  if (cursor_ >= length_) {
    return false;
  }

  std::uint32_t ncp = 0;
  if (auto v = DecodeOneUtf8(source_ + cursor_, (length_ - cursor_), &ncp); v) {
    if (ncp == a) {
      cursor_ += v.value();
      c0_ = ncp;
      c0_size_ = v.value();
      return true;
    }
  }

  return false;
}

inline bool Scanner::EqFrom(std::uint32_t a1, std::uint32_t a2) {
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

  auto lookahead1 = peek(cursor_);
  if (!lookahead1) return false;
  if (lookahead1->first != a1) return false;

  auto lookahead2 = peek(cursor_ + lookahead1->second);
  if (!lookahead2) return false;
  if (lookahead2->first != a2) return false;

  cursor_ += (lookahead1->second + lookahead2->second);
  c0_ = a2;
  c0_size_ = lookahead2->second;
  return true;
}

inline bool Scanner::IsIdIChar(std::uint32_t c) const {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

inline bool Scanner::IsIdRChar(std::uint32_t c) const {
  return IsIdIChar(c) || (c >= '0' && c <= '9');
}

template <std::size_t N>
inline bool Scanner::Match(const char (&rhs)[N]) {
  const auto len = (N - 1);

  // NOTES(dpeng), we don't do a full utf-8 decoding here since we already know
  //   the target, if matched, must be ASCII codec.
  for (std::size_t i = 0; i < len; ++i) {
    if ((cursor_ + i) >= length_) {
      return false;
    }
    if (rhs[i] != source_[cursor_ + i]) {
      return false;
    }
  }

  // We do a peek character here to check whether the next codepoint is a
  // trailing character or not
  if (cursor_ + len < length_) {
    std::uint32_t cp = 0;
    if (auto r = DecodeOneUtf8(source_ + cursor_ + len, length_ - cursor_ - len,
                               &cp);
        r) {
      if (IsIdRChar(cp)) {
        return false;
      }
    }
  }

  cursor_ += len;
  c0_ = rhs[len];
  c0_size_ = 1;
  return true;
}

inline std::optional<std::string> Scanner::GetLastError() const {
  if (token_ == kError) {
    return text_;
  } else {
    return {};
  }
}

inline Token Scanner::Eutf8() {
  text_ = Fmt("%s]: invalid utf8 encoding", Highlight().c_str());
  return Y(kError);
}

inline Token Scanner::Etoken(const char* msg) {
  text_ = Fmt("%s]: %s", Highlight().c_str(), msg);
  return Y(kError);
}

inline Token Scanner::Estr(const char* msg) {
  text_ = Fmt("%s]: %s", Highlight().c_str(), msg);
  return Y(kError);
}

inline Token Scanner::Eescape() {
  text_ = Fmt("%s]: invalid string escape", Highlight().c_str());
  return Y(kError);
}

inline Token Scanner::Ena(const char* msg) {
  text_ = Fmt("%s]: %s", Highlight().c_str(), msg);
  return Y(kError);
}

inline Token Scanner::Ecmt(const char* msg) {
  text_ = Fmt("%s]: %s", Highlight().c_str(), msg);
  return Y(kError);
}

inline Token Scanner::Eattr(const char* msg) {
  text_ = Fmt("%s]: %s", Highlight().c_str(), msg);
  return Y(kError);
}

inline Token Scanner::Enum(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);

  text_ = Fmt("%s]: %s", Highlight().c_str(), msg.c_str());
  return Y(kError);
}

inline Scanner::Scanner(const std::string& data, Option&& opt)
    : data_(data),
      source_(data.c_str()),
      length_(data.size()),
      cursor_(0),
      c0_(0),
      c0_size_(0),
      peek_c_(0),
      peek_size_(0),
      token_(),
      text_(),
      integer_(0),
      real_(0.0),
      allow_eval_(opt.allow_eval),
      allow_var_(opt.allow_var) {}

}  // namespace _
}  // namespace base

#endif  // _BASE___SCANNER_INL_H_
