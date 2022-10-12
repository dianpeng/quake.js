#include "template.h"

using namespace base;

namespace quake {
namespace {

class Parser {
 public:
  Parser(const std::string_view& content) : content_(content), cursor_(0) {}

  // Parsing the input textual data into Template object
  Result<Template> Parse();

 private:
  struct Tag {
    std::size_t count = 0;  // count of the %, guess not very useful at all
    std::string name;       // the name will be normalized, ie stripping out its
                            // trailing spaces
    std::string delimiter;  // raw tag name needs to be used as delimiter

    bool IsEmpty() const { return count == 0; }
  };

 private:
  // Try to get next tag from the input stream. If eof meets or anything unknown
  // meets, returns {}
  Result<Tag> NextTag();
  Error Err(const char*, ...);
  void GetPosition(int*, int*);
  void SkipComment();

 private:
  std::string_view content_;
  std::size_t cursor_;
};

Error Parser::Err(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto msg = FmtV(fmt, vl);
  va_end(vl);

  int line, col;
  GetPosition(&line, &col);

  return Error::Mk("around(%d:%d): %s", line, col, msg.c_str());
}

void Parser::GetPosition(int* l, int* c) {
  *l = *c = 1;
  for (std::size_t i = 0; i < cursor_; i++) {
    auto cc = content_[i];
    if (cc == '\n') {
      ++*l;
      *c = 1;
    } else {
      ++*c;
    }
  }
}

Result<Template> Parser::Parse() {
  Template t;

  while (true) {
    if (auto r = NextTag(); !r) {
      return r.PassError();
    } else {
      if (auto v = r.PassResult(); v.IsEmpty()) {
        break;
      } else {
        // okay, find out a section, just hit next
        if (auto idx = content_.find(v.delimiter, cursor_);
            idx == std::string_view::npos) {
          return Err("section is not closed properly: %s", v.delimiter.c_str());
        } else {
          auto len = idx - cursor_;

          // looking back to check whether we have at least a linebreak or not
          // if so, just ignore it as well, otherwise
          if (content_[idx - 1] == '\n' && len > 0) len--;

          auto content = content_.substr(cursor_, len);
          t.content.push_back(
              Template::Section{v.name, std::to_string(content)});

          cursor_ = idx + v.delimiter.size();
        }
      }
    }
  }

  return t;
}

void Parser::SkipComment() {
  for (cursor_++; cursor_ < content_.size(); cursor_++) {
    if (content_[cursor_] == '\n') {
      ++cursor_;
      break;
    }
  }
  return;
}

Result<Parser::Tag> Parser::NextTag() {
  Tag tag;

  for (; cursor_ < content_.size();) {
    auto c = content_[cursor_];
    switch (c) {
      // whitespaces, just ignore them
      case ' ':
      case '\t':
      case '\r':
      case '\n':
      case '\b':
      case '\v':
        cursor_++;
        continue;

      case '!':
        SkipComment();
        continue;

      case '%':
        tag.count = 1;
        tag.delimiter.push_back(c);

        // try to parse it as tag. The tag is any numbers of percent
        for (cursor_++; cursor_ < content_.size(); cursor_++) {
          auto nc = content_[cursor_];
          tag.delimiter.push_back(nc);

          if (nc == '%') {
            tag.count++;
            continue;
          } else {
            tag.name.push_back(nc);

            for (cursor_++; cursor_ < content_.size(); cursor_++) {
              if (auto nc = content_[cursor_]; nc == '\n') {
                ++cursor_;  // do not include the linebreak, just skip it
                break;
              } else {
                tag.delimiter.push_back(nc);
                tag.name.push_back(nc);
              }
            }
            break;
          }
        }

        tag.name = std::to_string(TrimWS(tag.name));

        if (tag.name.empty()) {
          return Err("empty tag name");
        } else {
          return tag;
        }

      default:
        return Err("unknown characters");
    }
  }

  return tag;
}

}  // namespace

Result<Template> ParseTemplate(const std::string_view& data) {
  Parser p{data};
  return p.Parse();
}

Result<Template> ParseTemplateFile(const std::string_view& path) {
  if (auto data = LoadWholeFile(path); data) {
    return ParseTemplate(*data);
  } else {
    return Error::Mk("cannot load file");
  }
}

std::optional<std::string_view> Template::GetSection(
    const std::string_view& name) const {
  if (auto itr =
          std::find_if(content.begin(), content.end(),
                       [&](const Section& s) -> bool { return s.tag == name; });
      itr != content.end()) {
    return itr->text;
  } else {
    return {};
  }
}

}  // namespace quake
