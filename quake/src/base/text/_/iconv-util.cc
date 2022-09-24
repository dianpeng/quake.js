#include <base/text/_/iconv-util.h>
#include <base/common.h>

namespace base {
namespace text {
namespace _ {

// The libiconv requires that the input buffer to be mutable char array which
// requires a separate marshal. We setup a global (TLS) array to be used in
// this situation.
namespace {

thread_local std::vector<char> kGBXBuffer;

}  // namespace

char* IconvConstCharToCharArray(const void* i, std::size_t size) {
  kGBXBuffer.resize(size);
  std::memcpy(kGBXBuffer.data(), static_cast<const char*>(i), size);
  return kGBXBuffer.data();
}

}  // namespace _
}  // namespace text
}  // namespace base
