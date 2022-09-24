#ifndef _BASE_TEXT___ICONV_UTIL_H_
#define _BASE_TEXT___ICONV_UTIL_H_
#include <cstddef>

namespace base {
namespace text {
namespace _ {

// Libiconv requires that the input of data to be mutable but in most our cases,
// this is not true. This means we need to setup a buffer to copy the original
// data from user and create a temporary buffer to be used by libiconv. To
// optimize for performance we use a thread local storage to hold a buffer to
// be used by our runtime to do this sort of conversion.
char* IconvConstCharToCharArray(const void*, std::size_t);

}  // namespace _
}  // namespace text
}  // namespace base

#endif  // _BASE_TEXT___ICONV_UTIL_H_
