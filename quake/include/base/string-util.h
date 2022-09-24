#ifndef _BASE_STRING_UTIL_H_
#define _BASE_STRING_UTIL_H_
#include "check.h"
#include "common.h"

namespace base {

// Notes(dpeng): Consider get rid of the std::tolower/std::toupper in
//   performance critical code, particularly we know the encoding is
//   latino-1 or ASCII. The above function is relatively slow since they
//   touch the local settings.

inline std::string ToLower(const std::string_view&);
inline std::string ToLower(const char*, std::size_t);
inline std::string ToUpper(const std::string_view&);
inline std::string ToUpper(const char*, std::size_t);
inline void ToLower(std::string*);
inline void ToUpper(std::string*);

inline bool IsLower(const std::string_view&);
inline bool IsUpper(const std::string_view&);

// ------------------------------------------------------
// Caseless character/string operations
struct CaselessLessComparatorChar
    : public std::binary_function<char, char, bool> {
  bool operator()(const char& c1, const char& c2) const {
    return std::tolower(c1) < std::tolower(c2);
  }
};

struct CaselessLessComparatorString
    : public std::binary_function<std::string, std::string, bool> {
  bool operator()(const std::string_view& lhs,
                  const std::string_view& rhs) const {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end(),
                                        CaselessLessComparatorChar());
  }
};

template <typename T>
using CaselessSTLMap =
    std::map<std::string, T, CaselessLessComparatorString, std::less<>>;

template <typename T>
using CaselessSTLSet = std::set<T, CaselessLessComparatorString, std::less<>>;

inline bool CaselessEqual(const std::string_view&, const std::string_view&);
inline bool CaselessEqual(const char*, const char*);
inline bool CaselessEqual(const char*, std::size_t, const char*, std::size_t);

inline typename std::string_view::const_iterator CaselessFind(
    const std::string_view&, const std::string_view&);

inline const char* CaselessFind(const char*, std::size_t, const char*,
                                std::size_t);

inline const char* CaselessFind(const char*, const char*);

inline bool CaselessContain(const std::string_view&, const std::string_view&);

inline bool CaselessContain(const char*, std::size_t, const char*, std::size_t);

inline bool CaselessContain(const char*, const char*);

// ------------------------------------------------------
// strict characater/string operations

template <std::size_t N>
inline bool LiteralEqual(const char*, const char (&)[N]);

inline typename std::string_view::const_iterator Find(const std::string_view&,
                                                      const std::string_view&);

inline const char* Find(const char*, std::size_t, const char*, std::size_t);
inline const char* Find(const char*, const char*);

inline bool Contain(const std::string_view&, const std::string_view&);
inline bool Contain(const char*, std::size_t, const char*, std::size_t);
inline bool Contain(const char*, const char*);

inline bool IsASCIIWhitespace(char);

// FIXME(dpeng): We shuold really deprecate the SkipXXX apis but prefer Trim
//   APIs, the SkipXXX uses a strchr to locate a char which doesn't distinguish
//   null terminator and in general doesn't even work correctly in Web standard
//   context.
//
// skip the leading whitespace until we find the |end| character, specified
// by user. returns' the first none whitespace character position
const char* SkipLeadingWhitespace(const char*, const char*);
const char* SkipLeadingChar(const char*, const char*, const char*);

// Similar as above, but skipping from the backwards, ie trailing
const char* SkipTrailingWhitespace(const char*, const char*);
const char* SkipTrailingChar(const char*, const char*, const char*);

// Trim APIs, works mostly as SkipXXX apis except it doesn't work with C string
//
// For std::string_view to trim spaces at front and back
template <typename H>
inline std::string_view TrimLeading(const std::string_view&, H&&);
template <typename H>
inline std::string_view TrimTrailing(const std::string_view&, H&&);
template <typename H>
inline std::string_view Trim(const std::string_view&, H&&);
inline std::string_view TrimWS(const std::string_view&);

inline bool StartsWith(const std::string_view&, const std::string_view&);
inline bool EndsWith(const std::string_view&, const std::string_view&);

// Substring inplace for std::string, ie shrink or expand the std::string
void InplaceSubString(std::string*, std::size_t,
                      std::size_t len = std::string::npos);

// Dump binary data in human readable hexify output
std::string PrintBinary(const void*, std::size_t);

// Dump size in human readable way
std::string PrintSize(std::size_t);

// Append the codepoint into std::string with utf8 encoding scheme
inline bool AppendUtf8(std::uint32_t, std::string*);

// -------------------------------------------------------------
// Parsing related stuff, common string related parser helpers
//
// Parse the specified input with given dilimiter, for each segment the
// callback is invoked.
//
// Example, parse a comma separated list into std::vector<std::string>
//
//   Tokenize(your_data, size, ",",
//            [](const char* start, std::size_t size) -> bool {
//              vec.push_back(std::string{start, size});
//              return true;
//            });
//
//  The |vec| will have all comma separated part stored
//
std::size_t Tokenize(const char*, std::size_t, const char*,
                     std::function<bool(const char*, std::size_t)>&& callback,
                     bool trim = false);

// No callback version
inline std::vector<std::string_view> Tokenize(const std::string_view&,
                                              const char*, bool trim = false);

// Parse identifier. Identifier is defined as following :
//   [a-zA-Z0-9_]
std::pair<const char*, std::size_t> ParseIdentifier(
    const char*, std::size_t, bool skip_leading_ws = true);

// Highlight a string with specific pattern/format. Useful when want to dump
// diagnostic information for debug purpose
std::string HighlightString(const std::string_view&, std::size_t start,
                            std::size_t end, std::size_t length = 32,
                            const char* ld = ">>>>", const char* rd = "<<<<");
}  // namespace base

#include "string-util-inl.h"

#endif  // _BASE_STRING_UTIL_H_
