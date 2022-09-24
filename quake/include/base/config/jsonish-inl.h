#ifndef _BASE_CONFIG_JSONISH_INL_H_
#define _BASE_CONFIG_JSONISH_INL_H_

namespace base {
namespace config {

inline Result<Value> Jsonish::Parse(std::string&& content, Option&& option) {
  Jsonish j;
  return j.DoParseDocument(std::move(content), std::move(option));
}

inline Result<Value> Jsonish::ParseJSON(std::string&& content) {
  Jsonish j;
  return j.DoParseDocument(std::move(content), Option{0, nullptr, true});
}

inline Result<Value> Jsonish::ParseNamedMap(std::string&& data) {
  Jsonish j;
  return j.DoParseNamedMap(std::move(data));
}

inline Result<Value> Jsonish::ParseValue(std::string&& content) {
  Jsonish j;
  return j.DoParseValue(std::move(content));
}

inline Error Jsonish::Expect(base::_::Token tk) {
  if (L()->token() == tk) {
    L()->Next();
    return {};
  }
  return Etoken(tk);
}

inline Error Jsonish::Expect(base::_::Token tk1, base::_::Token tk2) {
  if (L()->token() == tk1 || L()->token() == tk2) {
    L()->Next();
    return {};
  }
  return Etoken(tk1, tk2);
}

inline void Jsonish::Enter(std::string&& cid, std::string&& c) {
  auto allow_eval = option_.AllowEval();
  p_stack_.emplace_back(std::make_unique<CurrentContext>(
      std::move(cid), std::move(c),
      base::_::Scanner::Option{allow_eval, allow_eval}));
}

inline void Jsonish::Leave() {
  DCHECK(!p_stack_.empty());
  p_stack_.pop_back();
}

template <typename H, typename Delimiter, typename Terminator>
Error Jsonish::ParseAt(H&& handler, Delimiter&& d, Terminator&& t) {
  do {
    if (auto e = handler(); e) {
      return e;
    }

    if (d(L()->token())) {
      L()->Next();
    }

    // Notes, not |else if| to allow a dangling delimiter shows up before the
    // last end token for the compound structure. For example, this makes
    // [ a, b, c, ] works, noted there's a dangling comma after last element
    if (t(L()->token())) {
      L()->Next();
      break;
    }
  } while (true);

  return {};
}

inline Result<std::string> Jsonish::ParseObjectKey() {
  switch (L()->token()) {
    case base::_::kString:
    case base::_::kId: {
      auto r = std::string{L()->text()};
      L()->Next();
      return r;
    }
    default:
      return Egrammar("expect quoted string or identifier");
  }
}

inline Error Jsonish::Etoken(base::_::Token tk) {
  return Err(kErrorExpectToken, "%s]] expect token: %s",
             ErrorPosition().c_str(), GetTokenName(tk));
}

inline Error Jsonish::Etoken(base::_::Token tk1, base::_::Token tk2) {
  return Err(kErrorExpectToken, "%s]] expect token %s or %s",
             ErrorPosition().c_str(), GetTokenName(tk1), GetTokenName(tk2));
}

inline Error Jsonish::Eimplarr(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);
  return Err(kErrorInvalidGrammar, "%s]] invalid implicit array: %s",
             ErrorPosition().c_str(), msg.c_str());
}

inline Error Jsonish::Egrammar(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);
  return Err(kErrorInvalidGrammar, "%s]] %s", ErrorPosition().c_str(),
             msg.c_str());
}

inline Error Jsonish::Einstruction(const char* ins, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);
  return Err(kErrorInvalidInstruction, "%s]] instruction(%s)): %s",
             ErrorPosition().c_str(), ins, msg.c_str());
}

inline Error Jsonish::Eunknownins(const char* ins) {
  return Err(kErrorInvalidInstruction, "%s]] instruction %s unknown",
             ErrorPosition().c_str(), ins);
}

}  // namespace config
}  // namespace base

#endif  // _BASE_CONFIG_JSONISH_INL_H_
