#include <base/string-util.h>

namespace base {

const char* SkipLeadingChar(const char* start, const char* end,
                            const char* table) {
  DCHECK(start <= end);
  for (; start < end; ++start) {
    if (!std::strchr(table, *start)) {
      break;
    }
  }
  return start;
}

const char* SkipLeadingWhitespace(const char* start, const char* end) {
  return SkipLeadingChar(start, end, _::kASCIIWhitespace);
}

const char* SkipTrailingChar(const char* start, const char* end,
                             const char* table) {
  DCHECK(end >= start);

  for (--end; end >= start; --end) {
    if (!std::strchr(table, *end)) {
      break;
    }
  }
  return end;
}

const char* SkipTrailingWhitespace(const char* start, const char* end) {
  return SkipTrailingChar(start, end, _::kASCIIWhitespace);
}

void InplaceSubString(std::string* str, std::size_t start, std::size_t len) {
  DCHECK(start <= str->size());

  // Clamp the size if needed
  if (len == std::string::npos) {
    len = str->size() - start;
    if (len == 0) {
      // The possible resized length is 0, so just clear the string
      str->clear();
      return;
    }
  } else {
    if (auto left = str->size() - start; len > left) {
      len = left;
    }
  }

  // Adjust the string inplace now
  if (start == 0) {
    str->resize(len);
    return;
  } else {
    // We resize the string at first then do the erase to save the time for
    // moving unused part.

    // This allow us to save first character until our substring's end
    str->resize(start + len);

    // Do the erase
    auto itr = str->begin();
    std::advance(itr, start);
    str->erase(str->begin(), itr);

    DCHECK(str->size() == len);
  }
}

std::size_t Tokenize(const char* source, std::size_t length,
                     const char* delimiters,
                     std::function<bool(const char*, std::size_t length)>&& cb,
                     bool trim) {
  using CRange = std::pair<const char*, const char*>;
  std::size_t size = 0;
  auto pos = source;
  auto pend = source + length;

  // Function to find the next section which contains the data delimited by
  // delimiters or EOM, notes the section may also contain additional spaces
  // which needs to be trimed by the trimmer function. This function does mutate
  // captured value as its internal state
  auto next_section = [&]() -> CRange {
    auto start = pos;
    while ((pos != pend) && !std::strchr(delimiters, *pos)) ++pos;

    auto end = pos;

    // bump pos, ie cursor, one more byte to bypass the delimiters
    if (pos != pend) ++pos;
    return std::make_pair(start, end);
  };

  // Trim the whitespace at leading or trailing places specified by the range
  auto trimmer = [](const CRange& range) {
    auto new_start = SkipLeadingWhitespace(range.first, range.second);
    if (new_start == range.second) {
      return std::make_pair(new_start, range.second);
    }

    // Notes(dpeng), the trailing whitespace function returns one off the
    //   start to indicate an empty sequences, ie all trailing spaces.
    auto new_end = SkipTrailingWhitespace(new_start, range.second);
    if (new_end < new_start) {
      return std::make_pair(new_start, new_start);
    } else {
      return std::make_pair(new_start, new_end + 1);
    }
  };

  while (pos != pend) {
    auto range = next_section();
    if (trim && range.first != range.second) {
      range = trimmer(range);
    }

    ++size;
    if (!cb(range.first, range.second - range.first)) return size;
  }

  return size;
}

std::pair<const char*, std::size_t> ParseIdentifier(const char* input,
                                                    std::size_t length,
                                                    bool skip) {
  if (length == 0) return std::make_pair(nullptr, 0);

  // 0. Skip the leading whitespace if needed
  if (skip) {
    while (std::isspace(*input) && length) {
      ++input;
      --length;
    }
    if (length == 0) return std::make_pair(nullptr, 0);
  }

  // 1. Check the leading character
  if (!std::isalpha(input[0]) && input[0] != '_') {
    return std::make_pair(nullptr, 0);
  }

  // 2. Check the rest characters
  {
    std::size_t ret = 1;
    for (; ret < length; ++ret) {
      auto c = input[ret];
      if (!std::isalnum(c) && c != '_') {
        break;
      }
    }

    return std::make_pair(input, ret);
  }
}

std::string PrintBinary(const void* data, std::size_t l) {
  std::stringstream formatter{};
  auto byte = reinterpret_cast<const std::uint8_t*>(data);
  for (std::size_t i = 0; i < l; ++i) {
    formatter << "0x" << std::hex << static_cast<std::uint32_t>(byte[i])
              << std::dec << ' ';
  }
  return formatter.str();
}

std::string PrintSize(std::size_t byte) {
  static const char* kUnit[] = {"B", "KB", "MB", "G", "T", "P"};
  static const int kMax = ArraySize(kUnit);

  std::string stack[kMax];
  std::size_t size = 0;

  for (auto i = 0; i < kMax; ++i) {
    if (byte < 1024) {
      stack[i] = Fmt("%d%s ", static_cast<int>(byte), kUnit[i]);
      size += stack[i].size();
      break;
    } else {
      auto v = byte % 1024;
      stack[i] = Fmt("%d%s ", static_cast<int>(v), kUnit[i]);
      size += stack[i].size();
      byte = byte / 1024;
    }
  }

  std::string buffer{};
  buffer.reserve(size);
  for (auto beg = std::rbegin(stack), end = std::rend(stack); beg != end;
       ++beg) {
    buffer.append(*beg);
  }
  return buffer;
}

std::string HighlightString(const std::string_view& input, std::size_t start,
                            std::size_t end, std::size_t length, const char* ld,
                            const char* rd) {
  const auto data = input.data();
  const auto size = input.size();
  auto beg = start > length ? (start - length) : 0;
  auto fin = (end + length) > size ? size : (end + length);

  return Fmt("%.*s%s %.*s %s%.*s", static_cast<int>(start - beg), data + beg,
             ld, static_cast<int>(end - start), data + start, rd,
             static_cast<int>(fin - end), data + end);
}

}  // namespace base
