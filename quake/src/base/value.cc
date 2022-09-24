#include <base/_/scanner.h>
#include <base/value.h>

using namespace base::_;

namespace base {
namespace {

struct Formatter {
  static std::string F(const std::pair<std::string, Value>& v) {
    return Fmt("%s => %s", v.first.c_str(), v.second.Print().c_str());
  }
  static std::string F(const Value& v) { return v.Print(); }
};

}  // namespace

std::string Value::Print() const {
  switch (type()) {
    case kNull:
      return "null";
    case kInt:
      return std::to_string(ToInt());
    case kReal:
      return std::to_string(ToReal());
    case kBoolean:
      return std::to_string(ToBoolean());
    case kString:
      return ToString();
    case kList:
      return ToList()->Print();
    case kDictionary:
      return ToDictionary()->Print();
    case kDuration:
      return std::to_string(ToDuration());
    case kStorageSize:
      return std::to_string(ToStorageSize());
    default:
      UNREACHABLE();
      return std::string{};
  }
}

std::string List::Print() const {
  return Join<const_iterator, Formatter>(begin(), end());
}

std::string Dictionary::Print() const {
  return Join<const_iterator, Formatter>(begin(), end());
}

std::shared_ptr<List> List::Clone() const {
  auto l = std::make_shared<List>();
  Clone(l.get());
  return l;
}

void List::Slice(List* that, std::size_t start, std::size_t end) const {
  for (std::size_t i = start; i < end && i < storage_.size(); ++i) {
    that->Add(storage_[i]);
  }
}

std::shared_ptr<List> List::Slice(std::size_t start, std::size_t end) const {
  auto l = std::make_shared<List>();
  Slice(l.get(), start, end);
  return l;
}

void List::Clone(List* l) const { l->storage_ = storage_; }

std::vector<std::string> Dictionary::GetKeyList() const {
  std::vector<std::string> r;
  r.reserve(storage_.size());

  for (const auto& pair : storage_) {
    r.push_back(pair.first);
  }
  return r;
}

std::shared_ptr<Dictionary> Dictionary::Clone() const {
  auto d = std::make_shared<Dictionary>();
  Clone(d.get());
  return d;
}

void Dictionary::Clone(Dictionary* d) const { d->storage_ = storage_; }

/** ---------------------------------------------------------------
 **      ToJSON implementation
 ** -------------------------------------------------------------*/

namespace {

template <ValueToJSON kPrettyPrintLevel>
class Printer {
 public:
  explicit Printer(std::ostream& formatter)
      : indent_(0), formatter_(formatter), root_() {}

  void Print(const Value&);
  void PrintList(const List&);
  void PrintDictionary(const Dictionary&);

 private:
  std::string EscapeString(const std::string&);
  void PrettyPrintMultiLineString(const std::string&);

  std::ostream& Indent();
  void BumpIndent() {
    if (IsPrettyPrint()) ++indent_;
  }
  void DecIndent() {
    if (IsPrettyPrint()) --indent_;
  }
  bool IsPrettyPrint() const {
    return static_cast<int>(kPrettyPrintLevel) >=
           static_cast<int>(ValueToJSON::kPrettyPrint);
  }
  bool IsVerbosePrettyPrint() const {
    return kPrettyPrintLevel == ValueToJSON::kVerbosePrettyPrint;
  }

 private:
  std::size_t indent_;
  std::ostream& formatter_;
  Value root_;
};

template <ValueToJSON kPrettyPrintLevel>
void Printer<kPrettyPrintLevel>::PrintList(const List& l) {
  if (l.IsEmpty()) {
    formatter_ << "[]";
  } else {
    if (IsPrettyPrint()) {
      formatter_ << "[\n";
    } else {
      formatter_ << '[';
    }

    {
      std::size_t idx = 0;
      BumpIndent();
      for (const auto& x : l) {
        Indent();
        Print(x);
        if (idx < l.GetSize() - 1) {
          if (IsPrettyPrint()) {
            formatter_ << ",\n";
          } else {
            formatter_ << ',';
          }
        } else {
          if (IsPrettyPrint()) {
            formatter_ << '\n';
          }
        }
        ++idx;
      }
      DecIndent();
    }
    Indent() << "]";
  }
}

template <ValueToJSON kPrettyPrintLevel>
void Printer<kPrettyPrintLevel>::PrintDictionary(const Dictionary& o) {
  if (IsPrettyPrint()) {
    formatter_ << "{\n";
  } else {
    formatter_ << '{';
  }

  {
    std::size_t idx = 0;
    BumpIndent();
    for (const auto& kv : o) {
      Indent() << '"' << EscapeString(kv.first);
      if (IsPrettyPrint()) {
        formatter_ << "\" : ";
      } else {
        formatter_ << "\":";
      }
      Print(kv.second);
      if (idx < o.GetSize() - 1) {
        if (IsPrettyPrint()) {
          formatter_ << ",\n";
        } else {
          formatter_ << ',';
        }
      } else {
        if (IsPrettyPrint()) {
          formatter_ << '\n';
        }
      }
      ++idx;
    }
    DecIndent();
  }
  Indent() << '}';
}

template <ValueToJSON kPrettyPrintLevel>
std::ostream& Printer<kPrettyPrintLevel>::Indent() {
  static const char* kIndent = "  ";
  if (IsPrettyPrint()) {
    for (std::size_t i = 0; i < indent_; ++i) {
      formatter_ << kIndent;
    }
  }
  return formatter_;
}
template <ValueToJSON kPrettyPrintLevel>
std::string Printer<kPrettyPrintLevel>::EscapeString(const std::string& str) {
#define XX(X)            \
  do {                   \
    ret.push_back('\\'); \
    ret.push_back((X));  \
  } while (false)

  std::string ret;
  ret.reserve(str.size());
  for (const auto& x : str) {
    switch (x) {
      case '\\':
        XX('\\');
        break;
      case '\v':
        XX('v');
        break;
      case '\r':
        XX('r');
        break;
      case '\n':
        XX('n');
        break;
      case '\b':
        XX('b');
        break;
      case '"':
        XX('"');
        break;
      case '\t':
        XX('t');
        break;
      default:
        ret.push_back(x);
        break;
    }
  }
  return ret;

#undef XX  // XX
}

template <ValueToJSON kPrettyPrintLevel>
void Printer<kPrettyPrintLevel>::PrettyPrintMultiLineString(
    const std::string& str) {
  std::size_t start = 0;
  std::size_t end = 0;

  {
    auto lb = str.find_first_of('\n');
    if (lb == std::string::npos) {
      formatter_ << '"' << EscapeString(str) << '"';
      return;
    }

    end = lb;
  }

  formatter_ << "[\n";
  BumpIndent();
  Indent();
  formatter_ << '"' << EscapeString(str.substr(start, (end - start))) << '"';

  for (start = end + 1; end != str.size() && start != str.size();
       start = end + 1) {
    formatter_ << ",\n";
    Indent();
    end = str.find_first_of('\n', start);
    if (end == std::string::npos) {
      end = str.size();
    }
    formatter_ << '"' << EscapeString(str.substr(start, (end - start))) << '"';
  }

  formatter_ << '\n';
  DecIndent();
  Indent() << ']';
}

template <ValueToJSON kPrettyPrintLevel>
void Printer<kPrettyPrintLevel>::Print(const Value& v) {
  switch (v.type()) {
    case Value::kInt:
      formatter_ << v.ToInt();
      break;
    case Value::kReal:
      formatter_ << v.ToReal();
      break;
    case Value::kNull:
      formatter_ << "null";
      break;
    case Value::kString:
      if (IsVerbosePrettyPrint()) {
        PrettyPrintMultiLineString(v.ToString());
      } else {
        formatter_ << '"' << EscapeString(v.ToString()) << '"';
      }
      break;
    case Value::kBoolean:
      formatter_ << (v.ToBoolean() ? "true" : "false");
      break;
    case Value::kList:
      PrintList(*v.ToListPtr());
      break;
    case Value::kDictionary:
      PrintDictionary(*v.ToDictionaryPtr());
      break;
    case Value::kDuration:
      formatter_ << '"' << std::to_string(v.ToDuration()) << '"';
      break;
    case Value::kStorageSize:
      formatter_ << '"' << std::to_string(v.ToStorageSize()) << '"';
      break;
    default:
      UNREACHABLE();
      return;
  }
}

}  // namespace

void Value::ToJSON(std::ostream& formatter, ValueToJSON opt) const {
  switch (opt) {
    case ValueToJSON::kCompact:
      Printer<ValueToJSON::kCompact>(formatter).Print(*this);
      break;

    case ValueToJSON::kPrettyPrint:
      Printer<ValueToJSON::kPrettyPrint>(formatter).Print(*this);
      break;

    default:
      Printer<ValueToJSON::kVerbosePrettyPrint>(formatter).Print(*this);
      break;
  }
}

void List::ToJSON(std::ostream& formatter, ValueToJSON opt) const {
  switch (opt) {
    case ValueToJSON::kCompact:
      Printer<ValueToJSON::kCompact>(formatter).PrintList(*this);
      break;

    case ValueToJSON::kPrettyPrint:
      Printer<ValueToJSON::kPrettyPrint>(formatter).PrintList(*this);
      break;

    default:
      Printer<ValueToJSON::kVerbosePrettyPrint>(formatter).PrintList(*this);
      break;
  }
}

void Dictionary::ToJSON(std::ostream& formatter, ValueToJSON opt) const {
  switch (opt) {
    case ValueToJSON::kCompact:
      Printer<ValueToJSON::kCompact>(formatter).PrintDictionary(*this);
      break;

    case ValueToJSON::kPrettyPrint:
      Printer<ValueToJSON::kPrettyPrint>(formatter).PrintDictionary(*this);
      break;

    default:
      Printer<ValueToJSON::kVerbosePrettyPrint>(formatter).PrintDictionary(
          *this);
      break;
  }
}

namespace {

enum ValueSearchError { kErrorSearchInvalidToken, kErrorSearchInvalidGrammar };

struct ValueSearchErrorCategory : public ErrorCategory {
  const char* GetName() const override { return "value-error"; }
  std::string GetInformation(std::uint32_t c) const override {
    switch (c) {
      case kErrorSearchInvalidToken:
        return "value-error[invalid token]";
      case kErrorSearchInvalidGrammar:
        return "value-error[invalid grammar]";
      default:
        return "value-error[none]";
    }
  }
};

const ErrorCategory* Cat() {
  static const ValueSearchErrorCategory kC;
  return &kC;
}

Error Err(ValueSearchError code, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);

  return Error(Cat(), code, std::move(msg));
}

// --------------------------------------------------------------------------
// Search Implementations
enum Vtype : std::uint8_t {
  kVtNull,
  kVtInt,
  kVtUInt,
  kVtReal,
  kVtNum,
  kVtStr,
  kVtBool,
  kVtArr,
  kVtObj,
  // None Json extensions
  kVtDuration,
  kVtStorage
};

const char* GetVtypeName(Vtype t) {
  switch (t) {
    case kVtNull:
      return "null";
    case kVtInt:
      return "int";
    case kVtUInt:
      return "uint";
    case kVtReal:
      return "real";
    case kVtNum:
      return "num";
    case kVtStr:
      return "str";
    case kVtBool:
      return "bool";
    case kVtArr:
      return "arr";
    case kVtObj:
      return "obj";
    case kVtDuration:
      return "duration";
    case kVtStorage:
      return "storage";
    default:
      UNREACHABLE();
      return nullptr;
  }
}

// Points back to the path string to indicate the range of string piece
struct NameRange {
  std::uint32_t start, end;
};

struct SliceRange {
  std::uint32_t start, end;
};

// Opcode, guis how to perform the search
enum Opcode : std::uint8_t {
  kOpAccess,  // access the next object, using paired information.
  kOpIndex,   // index the next object as an array
  kOpSlice,   // slice the array, using paired information

  kOpTyAssert,  // assert the type of current object

  kOpArrTyFilter,  // filter the current array's internal element
  kOpArrTyAssert,  // assert the current array's internal element
  kOpArrTySelect,  // select the current arrays' internal element, ie filter
                   // out those unwanted

  kOpReset,  // go back to root and reset the current searching value. Used by
             // or operator's since the next rules is comming. Internally it
             // will bump the rules counter.

  kSizeOfOp
};

const char* GetOpcodeName(Opcode c) {
  switch (c) {
    case kOpAccess:
      return "access";
    case kOpIndex:
      return "index";
    case kOpSlice:
      return "slice";
    case kOpTyAssert:
      return "type-assert";
    case kOpArrTyFilter:
      return "arr-type-filter";
    case kOpArrTyAssert:
      return "arr-type-assert";
    case kOpArrTySelect:
      return "arr-type-select";
    case kOpReset:
      return "reset";
    default:
      UNREACHABLE();
      return nullptr;
  }
}

struct Op {
  Opcode opcode;

  union {
    NameRange range;
    SliceRange slice;
    Vtype type;
    std::uint32_t index;
  } u;

  Op(Opcode opc) : opcode(opc) {}
};

using BytecodeList = small_vector<Op, 128>;
using CharBuffer = small_vector<char, 1024>;

// Parse/Compile or whatever, it spits out the bytecode sequences for later
// evaluation purpose.
class Parser {
 public:
  Parser(BytecodeList* bc, CharBuffer* cb, const std::string& sp)
      : bc_(bc), cb_(cb), scanner_(sp) {}

 public:
  Error Parse();

 private:
  Error ParseArrayType();
  Error ParseIndexOrSlice();
  Error ParseAccess();
  Error ParseSlice(std::uint32_t);
  Error ParseOneRule();
  Error ParseTypeAssert();

  Error ParseType(Vtype*);
  Error ParseId(NameRange*);
  Error ParseIdx(std::uint32_t*);

  Error ExpTk(Token);

 private:
  void OpReset() { bc_->push_back(Op(kOpReset)); }
  void OpAccess(const NameRange&);
  void OpIndex(std::uint32_t);
  void OpSlice(std::uint32_t, std::uint32_t);
  void OpTyAssert(Vtype);

 private:
  Error Eerr(const char*) const;
  Error Edangling() const;
  Error Etoken(Token) const;
  Error Etoken(const char*, ...) const;

 private:
  BytecodeList* bc_;
  CharBuffer* cb_;
  Scanner scanner_;
};

class Runner {
 public:
  Runner(const Value& r, const BytecodeList& bc, const CharBuffer& cb)
      : root_(r),
        bc_(bc),
        cb_(cb),
        current_(),
        pc_(0),
        rule_count_(0),
        dup_(false) {}

  bool dup() const { return dup_; }
  std::size_t rule_count() const { return rule_count_; }

  std::optional<Value> Run();

 private:
  std::optional<Value> RunOne();

  std::string_view Name(const NameRange& r) const {
    DCHECK(r.start <= cb_.size());
    DCHECK(r.end <= cb_.size());
    if (r.start == r.end) {
      return std::string_view{};
    } else {
      return std::string_view{cb_.data() + r.start, r.end - r.start};
    }
  }

  bool RunAccess(const Op&);
  bool RunIndex(const Op&);
  bool RunSlice(const Op&);
  bool RunTyAssert(const Op&);
  bool RunArrayType(const Op&);

  bool MatchType(Vtype, const Value&) const;

 private:
  const Value& root_;
  const BytecodeList& bc_;
  const CharBuffer& cb_;
  Value current_;
  std::size_t pc_;
  std::size_t rule_count_;
  bool dup_;
};

class SearchImpl {
 public:
  Error Parse(const std::string&);

  std::tuple<std::optional<Value>, std::size_t, bool> Search(
      const Value&) const;

  std::string PrintBytecode() const;

 private:
  std::string_view Name(const NameRange& r) const {
    DCHECK(r.start <= char_buffer_.size());
    DCHECK(r.end <= char_buffer_.size());
    if (r.start == r.end) {
      return std::string_view{};
    } else {
      return std::string_view{char_buffer_.data() + r.start, r.end - r.start};
    }
  }

 private:
  BytecodeList bc_;
  CharBuffer char_buffer_;
};

Error SearchImpl::Parse(const std::string& value) {
  Parser p{&bc_, &char_buffer_, value};
  return p.Parse();
}

std::tuple<std::optional<Value>, std::size_t, bool> SearchImpl::Search(
    const Value& v) const {
  Runner r{v, bc_, char_buffer_};
  auto result = r.Run();
  return std::make_tuple(std::move(result), r.rule_count(), r.dup());
}

std::string SearchImpl::PrintBytecode() const {
  std::stringstream fmt;
  for (const auto& code : bc_) {
    fmt << GetOpcodeName(code.opcode);
    switch (code.opcode) {
      case kOpAccess:
        fmt << ' ' << Name(code.u.range);
        break;

      case kOpIndex:
        fmt << ' ' << '$' << code.u.index;
        break;

      case kOpSlice:
        fmt << ' ' << '$' << code.u.slice.start << ' ' << '$'
            << code.u.slice.end;
        break;

      case kOpTyAssert:
      case kOpArrTyAssert:
      case kOpArrTyFilter:
      case kOpArrTySelect:
        fmt << ' ' << GetVtypeName(code.u.type);
        break;

      case kOpReset:
        break;

      default:
        UNREACHABLE();
        break;
    }
    fmt << '\n';
  }

  return fmt.str();
}

Error Parser::Etoken(const char* fmt, ...) const {
  if (scanner_.token() == kError) {
    return Err(kErrorSearchInvalidToken, scanner_.text().c_str());
  } else {
    va_list vl;
    va_start(vl, fmt);
    auto msg = FmtV(fmt, vl);
    va_end(vl);
    return Err(kErrorSearchInvalidToken, "%s]: %s",
               scanner_.Highlight().c_str(), msg.c_str());
  }
}

Error Parser::Etoken(Token tk) const {
  return Etoken(Fmt("expect %s", GetTokenName(tk)).c_str());
}

Error Parser::Edangling() const {
  return Etoken("dangling text after the last search component");
}

Error Parser::Eerr(const char* msg) const {
  if (scanner_.token() == kError) {
    return Err(kErrorSearchInvalidToken, scanner_.text().c_str());
  } else {
    return Err(kErrorSearchInvalidGrammar, "%s]: %s",
               scanner_.Highlight().c_str(), msg);
  }
}

Error Parser::ExpTk(Token tk) {
  if (scanner_.token() == tk) {
    scanner_.Next();
    return {};
  }
  return Etoken(tk);
}

void Parser::OpAccess(const NameRange& n) {
  Op o{kOpAccess};
  o.u.range = n;
  bc_->push_back(o);
}

void Parser::OpIndex(std::uint32_t index) {
  Op o{kOpIndex};
  o.u.index = index;
  bc_->push_back(o);
}

void Parser::OpSlice(std::uint32_t start, std::uint32_t end) {
  Op o{kOpSlice};
  o.u.slice.start = start;
  o.u.slice.end = end;
  bc_->push_back(o);
}

void Parser::OpTyAssert(Vtype t) {
  Op o{kOpTyAssert};
  o.u.type = t;
  bc_->push_back(o);
}

Error Parser::ParseType(Vtype* t) {
  if (scanner_.token() != kId || scanner_.text().size() != 1) {
    return Etoken("expect type specifier here");
  }
  switch (scanner_.text()[0]) {
    case 'N':
      *t = kVtNull;
      break;
    case 'i':
      *t = kVtInt;
      break;
    case 'u':
      *t = kVtUInt;
      break;
    case 'd':
    case 'r':
    case 'f':
      *t = kVtReal;
      break;
    case 'n':
      *t = kVtNum;
      break;
    case 'b':
      *t = kVtBool;
      break;
    case 's':
      *t = kVtStr;
      break;
    case 'a':
      *t = kVtArr;
      break;
    case 'o':
      *t = kVtObj;
      break;
    case 'D':
      *t = kVtDuration;
      break;
    case 'Q':
      *t = kVtStorage;
      break;
    default:
      return Etoken("unexpected type specifier: %s", scanner_.text().c_str());
  }

  scanner_.Next();
  return {};
}

Error Parser::ParseId(NameRange* r) {
  static const std::size_t kMaxCharBufferSize =
      static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max());

  // The way we format the name is to store the char sequences into the cb_,
  // notes, without the trailing \0 for now, and use a NameRange to point into
  // the char_buffer and later on just use std::string_view for manipulation
  if (scanner_.token() == kId || scanner_.token() == kString) {
    if (scanner_.text().size() + cb_->size() > kMaxCharBufferSize) {
      return Eerr("name is too long");
    }

    auto start = cb_->size();
    cb_->insert(cb_->end(), scanner_.text().begin(), scanner_.text().end());
    auto end = cb_->size();

    r->start = static_cast<std::uint32_t>(start);
    r->end = static_cast<std::uint32_t>(end);

    scanner_.Next();
    return {};
  } else {
    return Etoken("expect a identifier or quoted string");
  }
}

Error Parser::ParseIdx(std::uint32_t* idx) {
  static const std::int64_t kMaxIndexValue =
      static_cast<std::int64_t>(std::numeric_limits<std::uint32_t>::max());

  if (scanner_.token() != kInt) {
    return Etoken("expect a integer to represent index");
  }
  if (scanner_.integer() > kMaxIndexValue) {
    return Eerr("index too large");
  }
  *idx = static_cast<std::uint32_t>(scanner_.integer());
  scanner_.Next();
  return {};
}

Error Parser::ParseTypeAssert() {
  DCHECK(scanner_.token() == kPercent);
  scanner_.Next();
  Vtype t = kVtNull;
  if (auto e = ParseType(&t); e) {
    return e;
  }
  OpTyAssert(t);
  return {};
}

Error Parser::ParseAccess() {
  DCHECK(scanner_.token() == kDot);
  scanner_.Next();

  NameRange r;
  if (auto e = ParseId(&r); e) {
    return e;
  }
  OpAccess(r);
  if (scanner_.token() == kPercent) {
    if (auto e = ParseTypeAssert(); e) {
      return e;
    }
  }
  return {};
}

Error Parser::ParseArrayType() {
  DCHECK(scanner_.token() == kPercent);

  Opcode op = kOpArrTyAssert;
  if (auto ntk = scanner_.Next(); ntk == kLPar) {
    auto op_tk = scanner_.Next();
    if (op_tk == kQuestion)
      op = kOpArrTyFilter;
    else if (op_tk == kEmotion)
      op = kOpArrTySelect;
    else
      return Etoken("unknown array type assertion type");

    scanner_.Next();
    if (auto e = ExpTk(kRPar); e) {
      return e;
    }
  }

  Vtype t;
  if (auto e = ParseType(&t); e) {
    return e;
  }

  Op o{op};
  o.u.type = t;
  bc_->push_back(o);
  return {};
}

Error Parser::ParseSlice(std::uint32_t start) {
  DCHECK(scanner_.token() == kColon);

  // The end tag can be optionally ignored/ommitted to represent read util the
  // end. Currently we set the largest std::uint32_t to indicate so if user
  // ignore it
  std::uint32_t end = 0;
  if (auto tk = scanner_.Next(); tk == kInt) {
    if (auto e = ParseIdx(&end); e) {
      return e;
    }
  } else {
    end = std::numeric_limits<std::uint32_t>::max();
  }

  if (start > end) {
    start = end;
  }
  OpSlice(start, end);

  if (scanner_.token() == kPercent) {
    if (auto e = ParseArrayType(); e) {
      return e;
    }
  }
  return {};
}

Error Parser::ParseIndexOrSlice() {
  DCHECK(scanner_.token() == kArrayStart);

  if (auto tk = scanner_.Next(); tk != kPercent) {
    std::uint32_t idx = 0;
    if (auto e = ParseIdx(&idx); e) {
      return e;
    }
    if (scanner_.token() == kColon) {
      if (auto e = ParseSlice(idx); e) {
        return e;
      }
    } else {
      OpIndex(idx);
    }
  } else {
    if (auto e = ParseArrayType(); e) {
      return e;
    }
  }

  // checking the matched ']'
  if (auto e = ExpTk(kArrayEnd); e) {
    return e;
  }

  // checking the type specifier, if any
  if (scanner_.token() == kPercent) {
    if (auto e = ParseTypeAssert(); e) {
      return e;
    }
  }

  return {};
}

Error Parser::ParseOneRule() {
  for (auto tk = scanner_.token(); tk != kEof && tk != kVBar;
       tk = scanner_.token()) {
    if (tk == kDot) {
      if (auto e = ParseAccess(); e) {
        return e;
      }
    } else if (tk == kArrayStart) {
      if (auto e = ParseIndexOrSlice(); e) {
        return e;
      }
    } else {
      return Etoken("expect a valid pattern");
    }
  }
  return {};
}

Error Parser::Parse() {
  auto tk = scanner_.Next();

  switch (tk) {
    case kDot:
    case kArrayStart:
      if (auto e = ParseOneRule(); e) {
        return e;
      }
      break;
    case kPercent:
      if (auto e = ParseTypeAssert(); e) {
        return e;
      }
      break;
    default:
      break;
  }

  while (scanner_.token() == kVBar) {
    scanner_.Next();
    OpReset();
    if (auto e = ParseOneRule(); e) {
      return e;
    }
  }

  if (scanner_.token() != kEof) {
    return Edangling();
  }

  return {};
}

bool Runner::RunAccess(const Op& c) {
  DCHECK(c.opcode == kOpAccess);
  if (!current_.IsDictionary()) {
    return {};
  }
  auto obj = current_.ToDictionaryPtr();
  if (auto v = obj->Get(Name(c.u.range)); v) {
    current_ = *v;
    return true;
  }
  return false;
}

bool Runner::RunIndex(const Op& c) {
  DCHECK(c.opcode == kOpIndex);
  if (!current_.IsList()) {
    return {};
  }
  auto l = current_.ToListPtr();
  if (c.u.index >= l->GetSize()) {
    return {};
  }
  current_ = l->Get(c.u.index);
  return true;
}

bool Runner::RunSlice(const Op& c) {
  DCHECK(c.opcode == kOpSlice);
  if (!current_.IsList()) {
    return {};
  }
  current_ = Value{current_.ToListPtr()->Slice(c.u.slice.start, c.u.slice.end)};
  return true;
}

bool Runner::MatchType(Vtype t, const Value& v) const {
  switch (t) {
    case kVtInt:
      return v.IsInt();
    case kVtUInt:
      return v.IsInt() && v.ToInt() >= 0;
    case kVtReal:
      return v.IsReal();
    case kVtNum:
      return v.IsNumber();
    case kVtBool:
      return v.IsBoolean();
    case kVtStr:
      return v.IsString();
    case kVtArr:
      return v.IsList();
    case kVtObj:
      return v.IsDictionary();
    case kVtDuration:
      return v.IsDuration();
    case kVtStorage:
      return v.IsStorageSize();
    case kVtNull:
      return v.IsNull();
    default:
      UNREACHABLE();
      return false;
  }
}

bool Runner::RunArrayType(const Op& c) {
  switch (c.opcode) {
    case kOpArrTyFilter:
      if (!current_.IsList()) {
        return false;
      } else {
        auto list = std::make_shared<List>();
        dup_ = true;
        for (const auto& entry : *current_.ToListPtr()) {
          if (MatchType(c.u.type, entry)) {
            list->Add(entry);
          }
        }
        current_.SetList(std::move(list));
        return true;
      }

    case kOpArrTySelect:
      if (!current_.IsList()) {
        return false;
      } else {
        auto list = std::make_shared<List>();
        dup_ = true;
        for (const auto& entry : *current_.ToListPtr()) {
          if (!MatchType(c.u.type, entry)) {
            list->Add(entry);
          }
        }
        current_.SetList(std::move(list));
        return true;
      }

    case kOpArrTyAssert:
      if (!current_.IsList()) {
        return false;
      } else {
        for (const auto& entry : *current_.ToListPtr()) {
          if (!MatchType(c.u.type, entry)) return false;
        }
        return true;
      }

    default:
      UNREACHABLE();
      return false;
  }
}

bool Runner::RunTyAssert(const Op& c) {
  DCHECK(c.opcode == kOpTyAssert);
  return MatchType(c.u.type, current_);
}

std::optional<Value> Runner::RunOne() {
  for (; pc_ < bc_.size(); pc_++) {
    const auto& code = bc_[pc_];

    switch (code.opcode) {
      case kOpAccess:
        if (!RunAccess(code)) {
          ++pc_;
          return {};
        }
        break;

      case kOpIndex:
        if (!RunIndex(code)) {
          ++pc_;
          return {};
        }
        break;

      case kOpSlice:
        if (!RunSlice(code)) {
          ++pc_;
          return {};
        }
        break;

      case kOpTyAssert:
        if (!RunTyAssert(code)) {
          ++pc_;
          return {};
        }
        break;

      case kOpArrTyFilter:
      case kOpArrTySelect:
      case kOpArrTyAssert:
        if (!RunArrayType(code)) {
          ++pc_;
          return {};
        }
        break;

      default:
        goto done;
    }
  }

done:
  return std::optional<Value>{current_};
}

std::optional<Value> Runner::Run() {
  current_ = root_;
  rule_count_ = 1;

  while (pc_ < bc_.size()) {
    if (auto r = RunOne(); !r) {
      // Just check whether we have pending counters or not
      if (pc_ < bc_.size() && bc_[pc_].opcode == kOpReset) {
        ++pc_;
        rule_count_++;
        current_ = root_;
        continue;
      } else {
        return {};
      }
    } else {
      return r;
    }
  }

  return std::optional<Value>{current_};
}

}  // namespace

class SearchPatternImpl : public Value::SearchPattern {
 public:
  SearchImpl impl;

  std::string DebugPrint() const override { return impl.PrintBytecode(); }

  std::optional<Value> Search(const Value&, std::size_t*) const override;
};

std::optional<Value> SearchPatternImpl::Search(const Value& v,
                                               std::size_t* idx) const {
  auto r = impl.Search(v);
  if (!std::get<0>(r)) {
    return std::optional<Value>{};
  }
  if (idx) {
    *idx = std::get<1>(r);
  }
  return std::move(std::get<0>(r));
}

Result<std::unique_ptr<Value::SearchPattern>> Value::CompileSearchPattern(
    const std::string& pattern) {
  auto l = std::make_unique<SearchPatternImpl>();
  if (auto e = l->impl.Parse(pattern); e) {
    return e;
  }
  return std::unique_ptr<Value::SearchPattern>{std::move(l)};
}

std::optional<Value> Value::Search(const SearchPattern& p,
                                   std::size_t* idx) const {
  return p.Search(*this, idx);
}

Result<std::optional<Value>> Value::Search(const std::string& pattern,
                                           std::size_t* index) const {
  SearchImpl srch;
  if (auto e = srch.Parse(pattern); e) {
    return e;
  }
  auto v = srch.Search(*this);
  if (!std::get<0>(v)) {
    return std::optional<Value>{};
  }
  if (index) {
    *index = std::get<1>(v);
  }
  return std::move(std::get<0>(v));
}

}  // namespace base
