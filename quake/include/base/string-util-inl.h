#ifndef _BASE_STRING_UTIL_INL_H_
#define _BASE_STRING_UTIL_INL_H_

namespace base {
namespace _ {

static constexpr char kASCIIWhitespace[] = {0x20, 0x0c, 0x0a, 0x0d,
                                            0x09, 0x0b, 0x0};

}  // namespace _

inline std::string ToLower(const std::string_view& str) {
  return ToLower(str.data(), str.size());
}

inline std::string ToLower(const char* str, std::size_t size) {
  std::string data;
  data.resize(size);
  std::transform(str, str + size, data.begin(), ::tolower);
  return data;
}

inline std::string ToUpper(const std::string_view& str) {
  return ToUpper(str.data(), str.size());
}

inline std::string ToUpper(const char* str, std::size_t size) {
  std::string data;
  data.resize(size);
  std::transform(str, str + size, data.begin(), ::toupper);
  return data;
}

inline void ToLower(std::string* str) {
  std::transform(str->begin(), str->end(), str->begin(), ::tolower);
}

inline void ToUpper(std::string* str) {
  std::transform(str->begin(), str->end(), str->begin(), ::toupper);
}

inline bool IsLower(const std::string_view& str) {
  auto cb = [](const char& x) -> bool { return std::isupper(x); };
  return std::find_if(str.begin(), str.end(), std::move(cb)) == str.end();
}

inline bool IsUpper(const std::string_view& str) {
  auto cb = [](const char& x) -> bool { return std::islower(x); };
  return std::find_if(str.begin(), str.end(), std::move(cb)) == str.end();
}

inline bool CaselessEqual(const std::string_view& lhs,
                          const std::string_view& rhs) {
  if (lhs.size() == rhs.size()) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      [](const char& c1, const char& c2) -> bool {
                        return std::tolower(c1) == std::tolower(c2);
                      });
  } else {
    return false;
  }
}

inline bool CaselessEqual(const char* lhs, std::size_t lhs_sz, const char* rhs,
                          std::size_t rhs_sz) {
  if (lhs_sz != rhs_sz) {
    return false;
  } else {
    return std::equal(lhs, lhs + lhs_sz, rhs,
                      [](const char& c1, const char& c2) -> bool {
                        return std::tolower(c1) == std::tolower(c2);
                      });
  }
}

inline bool CaselessEqual(const char* lhs, const char* rhs) {
  return CaselessEqual(lhs, std::strlen(lhs), rhs, std::strlen(rhs));
}

inline typename std::string_view::const_iterator CaselessFind(
    const std::string_view& source, const std::string_view& target) {
  return std::search(source.begin(), source.end(), target.begin(), target.end(),
                     [](const char& lhs, const char& rhs) -> bool {
                       return std::tolower(lhs) == std::tolower(rhs);
                     });
}

inline const char* CaselessFind(const char* lhs, std::size_t lhs_sz,
                                const char* rhs, std::size_t rhs_sz) {
  return std::search(lhs, lhs + lhs_sz, rhs, rhs + rhs_sz,
                     [](const char& lhs, const char& rhs) -> bool {
                       return std::tolower(lhs) == std::tolower(rhs);
                     });
}

inline const char* CaselessFind(const char* lhs, const char* rhs) {
  return CaselessFind(lhs, std::strlen(lhs), rhs, std::strlen(rhs));
}

inline bool CaselessContain(const std::string_view& source,
                            const std::string_view& target) {
  return CaselessFind(source, target) != source.end();
}

inline bool CaselessContain(const char* lhs, std::size_t lhs_sz,
                            const char* rhs, std::size_t rhs_sz) {
  return CaselessFind(lhs, lhs_sz, rhs, rhs_sz) != (lhs + lhs_sz);
}

inline bool CaselessContain(const char* lhs, const char* rhs) {
  return CaselessContain(lhs, std::strlen(lhs), rhs, std::strlen(rhs));
}

inline const char* Find(const char* lhs, const char* rhs) {
  return Find(lhs, std::strlen(lhs), rhs, std::strlen(rhs));
}

inline typename std::string_view::const_iterator Find(
    const std::string_view& source, const std::string_view& target) {
  return std::search(
      source.begin(), source.end(), target.begin(), target.end(),
      [](const char& lhs, const char& rhs) -> bool { return lhs == rhs; });
}

inline const char* Find(const char* lhs, std::size_t lhs_sz, const char* rhs,
                        std::size_t rhs_sz) {
  return std::search(
      lhs, lhs + lhs_sz, rhs, rhs + rhs_sz,
      [](const char& lhs, const char& rhs) -> bool { return lhs == rhs; });
}

inline bool Contain(const std::string_view& source,
                    const std::string_view& target) {
  return Find(source, target) != source.end();
}

inline bool Contain(const char* lhs, std::size_t lhs_sz, const char* rhs,
                    std::size_t rhs_sz) {
  return Find(lhs, lhs_sz, rhs, rhs_sz) != (lhs + lhs_sz);
}

inline bool Contain(const char* lhs, const char* rhs) {
  return Contain(lhs, std::strlen(lhs), rhs, std::strlen(rhs));
}

template <typename H>
inline std::string_view TrimLeading(const std::string_view& str, H&& pred) {
  auto itr = std::find_if_not(str.begin(), str.end(), std::move(pred));
  return str.substr(std::distance(str.begin(), itr));
}

template <typename H>
inline std::string_view TrimTrailing(const std::string_view& str, H&& pred) {
  auto itr = std::find_if_not(str.rbegin(), str.rend(), std::move(pred));
  return str.substr(0, std::distance(itr, str.rend()));
}

template <typename H>
inline std::string_view Trim(const std::string_view& in, H&& pred) {
  return TrimTrailing(TrimLeading(in, [&](char c) { return pred(c); }),
                      [&](char c) { return pred(c); });
}

inline std::string_view TrimWS(const std::string_view& in) {
  return Trim(in, [](char c) { return std::isspace(c); });
}

inline bool StartsWith(const std::string_view& needle,
                       const std::string_view& pattern) {
  return needle.rfind(pattern, 0) == 0;
}

inline bool EndsWith(const std::string_view& needle,
                     const std::string_view& pattern) {
  auto pos = needle.find(pattern, 0);
  if (pos != std::string_view::npos) {
    return (pos + pattern.size()) == needle.size();
  }
  return false;
}

template <std::size_t N>
inline bool LiteralEqual(const char* lhs, const char (&rhs)[N]) {
  for (std::size_t i = 0; i < (N - 1); ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}

inline bool IsASCIIWhitespace(char c) {
  auto bitr = std::begin(_::kASCIIWhitespace);
  auto bend = std::end(_::kASCIIWhitespace);
  return std::find(bitr, bend, c) != bend;
}

inline std::vector<std::string_view> Tokenize(const std::string_view& input,
                                              const char* d, bool trim) {
  std::vector<std::string_view> vec{};
  Tokenize(
      input.data(), input.size(), d,
      [&](const char* v, std::size_t size) {
        vec.push_back(std::string_view{v, size});
        return true;
      },
      trim);

  return vec;
}

}  // namespace base

#endif  // _BASE_STRING_UTIL_INL_H_
