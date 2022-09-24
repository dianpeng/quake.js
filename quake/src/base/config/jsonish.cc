#include <base/config/jsonish.h>
#include <base/string-util.h>

using namespace base::_;

namespace base {
namespace config {

Result<Value> Jsonish::DoParseValue(std::string&& content) {
  option_.include_instruction = nullptr;
  p_stack_.clear();

  Enter("<tos>", std::move(content));
  L()->Next();
  return ParseValue();
}

Result<Value> Jsonish::DoParseDocument(std::string&& content, Option&& option) {
  option_ = std::move(option);
  p_stack_.clear();

  Enter("<tos>", std::move(content));
  return ParseTOS();
}

Result<Value> Jsonish::DoParseNamedMap(std::string&& content) {
  option_.include_instruction = nullptr;
  p_stack_.clear();

  Enter("<tos>", std::move(content));
  L()->Next();
  if (auto r = ParseObjectKey(); !r) {
    return r.PassError();
  } else {
    return ParseNamedObject(r.PassResult());
  }
}

Error Jsonish::ParseNamedObjectEntryList(Dictionary* d, Token tk) {
  return ParseCommaSeparatedKVList(d, tk);
}

Error Jsonish::ParseNamedObject(Dictionary* d, std::string&& name) {
  d->Add("@name", Value{std::move(name)});

  if (L()->token() != kLPar && L()->token() != kObjectStart) {
    return Egrammar("expect a '(' or '{'");
  }

  const auto etk = (L()->token() == kLPar ? kRPar : kObjectEnd);
  if (auto e = ParseNamedObjectEntryList(d, etk); e) {
    return e;
  }
  return {};
}

Result<Value> Jsonish::ParseNamedObject(std::string&& name) {
  auto dict = std::make_shared<Dictionary>();
  if (auto e = ParseNamedObject(dict.get(), std::move(name)); e) {
    return e;
  }
  return Value{std::move(dict)};
}

Error Jsonish::ParseCommaSeparatedKVList(Dictionary* d, Token terminator) {
  if (auto tk = L()->Next(); tk == terminator) {
    L()->Next();
    return {};
  }
  auto handler = [&]() -> Error {
    std::string key;
    if (auto r = ParseObjectKey(); r) {
      key = r.PassResult();
    } else {
      return r.PassError();
    }
    if (auto e = Expect(kAssign); e) {
      return e;
    }
    if (auto r = ParseValue(); r) {
      d->Add(key, r.PassResult());
    } else {
      return r.PassError();
    }

    return {};
  };

  auto delimiter = [](Token tk) -> bool {
    return (tk == kComma || tk == kSemicolon);
  };
  auto term = [&](Token tk) -> bool { return (tk == terminator); };

  if (auto e =
          ParseAt(std::move(handler), std::move(delimiter), std::move(term));
      e) {
    return e;
  }
  return {};
}

Error Jsonish::ParseShortcutMap(Dictionary* d) {
  if (auto e = ParseCommaSeparatedKVList(d, kRPar); e) {
    return e;
  }
  return {};
}

Result<Value> Jsonish::ParseShortcutMap() {
  DCHECK(L()->token() == kLPar);
  auto d = std::make_shared<Dictionary>();
  if (auto e = ParseShortcutMap(d.get()); e) {
    return e;
  }
  return Value{std::move(d)};
}

Error Jsonish::ParseObjectEntryList(Dictionary* d) {
  if (auto tk = L()->Next(); tk == kObjectEnd) {
    L()->Next();
    return {};
  }

  auto handler = [&]() -> Error {
    std::string key;
    if (auto r = ParseObjectKey(); r) {
      key = r.PassResult();
    } else {
      return r.PassError();
    }
    if (auto e = Expect(kAssign, kColon); e) {
      return e;
    }
    if (auto r = ParseValue(); r) {
      d->Add(key, r.PassResult());
    } else {
      return r.PassError();
    }

    return {};
  };

  auto delimiter = [](Token tk) -> bool {
    return (tk == kComma || tk == kSemicolon);
  };

  auto terminator = [](Token tk) -> bool { return (tk == kObjectEnd); };

  if (auto e = ParseAt(std::move(handler), std::move(delimiter),
                       std::move(terminator));
      e) {
    return e;
  }
  return {};
}

Error Jsonish::ParseObject(Dictionary* d) {
  if (auto e = ParseObjectEntryList(d); e) {
    return e;
  } else {
    return {};
  }
}

Result<Value> Jsonish::ParseObject() {
  DCHECK(L()->token() == kObjectStart);
  auto d = std::make_shared<Dictionary>();
  if (auto e = ParseObject(d.get())) {
    return e;
  }
  return Value{std::move(d)};
}

Error Jsonish::ParseArray(List* l) {
  if (auto tk = L()->Next(); tk == kArrayEnd) {
    L()->Next();
    return {};
  }

  auto handler = [&]() -> Error {
    if (auto r = ParseValue(); r) {
      l->Add(r.PassResult());
    } else {
      return r.PassError();
    }
    return {};
  };
  auto delimiter = [](Token tk) -> bool {
    return (tk == kComma || tk == kSemicolon);
  };
  auto terminator = [](Token tk) -> bool { return (tk == kArrayEnd); };

  if (auto e = ParseAt(std::move(handler), std::move(delimiter),
                       std::move(terminator));
      e) {
    return e;
  }
  return {};
}

Result<Value> Jsonish::ParseArray() {
  DCHECK(L()->token() == kArrayStart);
  auto l = std::make_shared<List>();
  if (auto e = ParseArray(l.get()); e) {
    return e;
  }
  return Value{std::move(l)};
}

Error Jsonish::ParseImplicitNamedMap(std::string&& key) {
  if (auto v = ParseNamedObject(std::move(key)); !v) {
    return v.PassError();
  } else {
    DCHECK(tos_object_.IsDictionary());
    auto object = v.PassResult();
    auto root = tos_object_.ToDictionaryPtr();
    auto key = object.ToDictionaryPtr()->Get("@name")->ToString();
    root->Add(std::move(key), std::move(object));
    return {};
  }
}

bool Jsonish::TryMerge(Value* lhs, Value* rhs) const {
  if (lhs->IsDictionary() && rhs->IsDictionary()) {
    lhs->ToDictionaryPtr()->Extend(std::move(*rhs->ToDictionary()));
    return true;
  }
  if (lhs->IsList() && rhs->IsList()) {
    lhs->ToListPtr()->Extend(std::move(*rhs->ToListPtr()));
    return true;
  }
  return false;
}

Error Jsonish::DoParseImplicitObject(std::string&& key, bool try_merge) {
  DCHECK(tos_object_.IsDictionary());
  auto root = tos_object_.ToDictionaryPtr();
  L()->Next();
  if (auto r = ParseValue(); r) {
    auto value = r.PassResult();
    if (auto existed = root->Get(key); existed) {
      if (try_merge && TryMerge(existed, &value)) return {};
    }
    root->Add(key, std::move(value));
  } else {
    return r.PassError();
  }
  return {};
}

Error Jsonish::ParseImplicitStatement() {
  DCHECK(L()->token() == kString || L()->token() == kId);
  std::string key = L()->text();
  auto tk = L()->Next();
  switch (tk) {
    case kObjectStart:
    case kLPar:
      return ParseImplicitNamedMap(std::move(key));

    case kAppend:
      return DoParseImplicitObject(std::move(key), true);

    case kAssign:
    case kColon:
      return DoParseImplicitObject(std::move(key),
                                   option_.pragma_dup_key_merge);
    default:
      break;
  }
  return {};
}

Error Jsonish::ParseVar() {
  DCHECK(option_.AllowEval());
  auto tk = L()->Next();
  if (tk != kObjectStart) {
    return Egrammar("expect a { for var scope");
  }
  L()->Next();

  while (true) {
    switch (L()->token()) {
      case kString:
      case kId:
        break;
      default:
        return Egrammar("expect a id/string to represent var name");
    }
    std::string var_name = L()->text();
    switch (L()->Next()) {
      case kColon:
      case kAssign:
        break;
      default:
        return Egrammar("expect a \":\" or \"=\"");
    }
    L()->Next();
    if (auto maybe_value = ParseValue(); !maybe_value) {
      return maybe_value.PassError();
    } else {
      option_.do_var(std::move(var_name), maybe_value.PassResult());
    }

    if (L()->token() == kSemicolon) {
      L()->Next();
    }
    if (L()->token() == kObjectEnd) {
      L()->Next();
      break;
    }
  }

  return {};
}

Error Jsonish::ParseImplicitStatementList() {
  auto done = false;
  while (!done) {
    switch (L()->token()) {
      case kString:
      case kId:
        if (auto e = ParseImplicitStatement(); e) {
          return e;
        }
        break;
      case kAttribute:
        if (auto e = ParseInstructionStatement(); e) {
          return e;
        }
        break;
      case kVar:
        if (auto e = ParseVar(); e) {
          return e;
        }
        break;

      case kError:
        return Egrammar("invalid token");

      default:
        done = true;
        break;
    }

    switch (L()->token()) {
      case kSemicolon:
      case kComma:
        L()->Next();
        break;
      default:
        break;
    }
  }

  return {};
}

Result<Value> Jsonish::ParseValue() {
  Value v;
  switch (L()->token()) {
    case kId:
    case kString: {
      auto str = L()->text();
      auto tk = L()->Next();
      if (tk == kLPar || tk == kObjectStart) {
        return ParseNamedObject(std::move(str));
      } else {
        v.SetString(std::move(str));
      }
      break;
    }
    case kEval: {
      if (option_.AllowEval()) {
        auto str = L()->text();
        L()->Next();
        return option_.do_eval(str);
      } else {
        return Err(kErrorEvalDisallowed);
      }
    }
    case kLPar:
      return ParseShortcutMap();

    case kInt:
      v.SetInt(L()->integer());
      L()->Next();
      break;

    case kReal:
      v.SetReal(L()->real());
      L()->Next();
      break;

    case kTrue:
      v.SetBoolean(true);
      L()->Next();
      break;

    case kFalse:
      v.SetBoolean(false);
      L()->Next();
      break;

    case kNull:
      v.SetNull();
      L()->Next();
      break;

    case kObjectStart:
      return ParseObject();

    case kArrayStart:
      return ParseArray();

    // the @include can reach here
    case kAttribute:
      if (L()->text() == "include") {
        return IncludeValue();
      }
      // fallthrough
    default:
      return Egrammar(
          "expect a valid value, ie string/bool/number/array/object");
  }

  return v;
}

Result<Value> Jsonish::ParseTOS() {
  auto tk = L()->Next();
  switch (tk) {
    case kObjectStart: {
      auto root = tos_object_.SetDictionary();
      if (auto e = ParseObject(root.get()); e) {
        return e;
      }
      break;
    }

    case kArrayStart: {
      auto root = tos_object_.SetList();
      if (auto e = ParseArray(root.get()); e) {
        return e;
      }
      break;
    }

    default: {
      if (!option_.as_json) {
        tos_object_.SetDictionary();
        if (auto e = ParseImplicitStatementList(); e) {
          return e;
        }
      } else {
        return Egrammar("expect a '[' or \"{\" to start a Json document");
      }
      break;
    }
  }
  return Pass(&tos_object_);
}

Result<Value> Jsonish::ParseInclude(std::string&& cid, std::string&& content,
                                    bool as_statement) {
  DEFER([this]() { Leave(); });
  Enter(std::move(cid), std::move(content));

  L()->Next();

  // Notes this is for inlcude usage. If user pallow us to use as_statement
  // then we just treat the whole file as extension of the global scope, or
  // treat the content of the include a value
  if (as_statement) {
    auto tk = L()->token();

    switch (tk) {
      case kObjectStart:
        return ParseObject();
      case kArrayStart:
        return ParseArray();
      default:
        if (!option_.as_json) {
          DCHECK(tos_object_.IsDictionary());
          if (auto e = ParseImplicitStatementList(); e) {
            return e;
          }
        } else {
          return Egrammar("expect a '[' or \"{\" to start a Json document");
        }
        break;
    }

    return Value{};
  } else {
    // Okay treat the whole file content as normal JSON file which produce
    // a json value. Notes, it can be a primitive value either.
    return ParseValue();
  }
}

Error Jsonish::ParseInstructionStatement() {
  Error e;
  if (L()->text() == "include") {
    e = IncludeStatement();
  } else if (L()->text() == "pragma") {
    e = PragmaStatement();
  } else {
    return Eunknownins(L()->text().c_str());
  }

  if (!e) {
    // Skip the optional semicolon after the statement
    if (L()->token() == kSemicolon) {
      L()->Next();
    }
  }
  return e;
}

Error Jsonish::CheckIncludeConstraints(const std::string& name) const {
  if (p_stack_.size() == option_.max_include_nested_size) {
    return Err(kErrorInvalidInstruction, "nested included file is too much");
  }

  for (const auto& e : p_stack_) {
    if (e->content_id == name) {
      return Err(kErrorInvalidInstruction, "circular include: %s",
                 name.c_str());
    }
  }
  return {};
}

Result<Value> Jsonish::IncludeValue() { return DoInclude(false); }

Error Jsonish::IncludeStatement() {
  // Notes, for statement, the returned value of inclusion will be dropped, we
  // expect user to use implicit statement, ie directly insertion of the object
  // into the global scope. If user want to include a value, then just use the
  // include value way, ie write the include into the places a value is expected
  if (auto r = DoInclude(true); !r) {
    return r.PassError();
  }
  return {};
}

// @pragma "options";
Error Jsonish::PragmaStatement() {
  if (!option_.pragma) {
    return Einstruction("pragma", "pragma is not allowed");
  }
  if (auto tk = L()->Next(); tk != kString) {
    return Einstruction("pragma", "expect a quoted string");
  }

  // Parsing the pragma option in a much more concrete way ?
  auto options = L()->text();
  auto option_list = Tokenize(options, ";", true);
  for (const auto& entry : option_list) {
    if (entry == "dup_key_merge") {
      option_.pragma_dup_key_merge = true;
    }
  }

  L()->Next();
  return {};
}

Result<Value> Jsonish::DoInclude(bool as_statement) {
  DCHECK(L()->token() == kAttribute && L()->text() == "include");
  if (!option_.include_instruction) {
    return Einstruction("include", "include instruction is not allowed");
  }

  if (auto tk = L()->Next(); tk != kString) {
    return Einstruction("include", "expect a quoted string");
  }

  std::string path = L()->text();
  L()->Next();

  if (auto e = CheckIncludeConstraints(path); e) {
    return e;
  }
  if (auto result = option_.include_instruction->Run(this, path); result) {
    return ParseInclude(std::move(result->first), std::move(result->second),
                        as_statement);
  } else {
    return result.PassError();
  }
}

std::string Jsonish::ErrorPosition() const {
  std::stringstream fmt;
  std::size_t idx = 0;
  for (auto itr = p_stack_.rbegin(); itr != p_stack_.rend(); ++itr) {
    fmt << idx << ". " << (*itr)->content_id << " "
        << (*itr)->scanner.Highlight() << "\n";
    ++idx;
  }
  return fmt.str();
}

}  // namespace config
}  // namespace base
