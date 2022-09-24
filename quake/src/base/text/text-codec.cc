#include <base/text/text-codec.h>
#include <base/text/utf-codec.h>
#include <base/text/gbk-codec.h>
#include <base/text/gb2312-codec.h>

namespace base {
namespace text {

std::unique_ptr<TextDecoder> TextDecoder::Create(const std::string& t) {
  // Defined by web-spec
  static const char* kUtf8Name[] = {
      "unicode-1-1-utf-1", "unicode11utf8", "unicode20utf8", "utf-8", "utf8",
      "x-unicode20utf8"};

  static const char* kGBKName[] = {"GBK", "gbk"};

  static const char* kGB2312Name[] = {"GB2312", "gb2312", "gb-2312", "GB-2312"};

  if (std::find_if(std::begin(kUtf8Name), std::end(kUtf8Name),
                   [&](const auto& v) { return t == v; }) !=
      std::end(kUtf8Name)) {
    return std::make_unique<Utf8Decoder>();
  }

  if (std::find_if(std::begin(kGBKName), std::end(kGBKName),
                   [&](const auto& v) { return t == v; }) !=
      std::end(kGBKName)) {
    return std::make_unique<GBKDecoder>();
  }

  if (std::find_if(std::begin(kGB2312Name), std::end(kGB2312Name),
                   [&](const auto& v) { return t == v; }) !=
      std::end(kGB2312Name)) {
    return std::make_unique<GB2312Decoder>();
  }

  return {};
}

// This is super slow, since it does the decoding-encoding twice. The derive the
// codec should optimize the DecodeToUtf8 since it is the most frequently used
// functions
bool TextDecoder::DecodeToUtf8(const void* input, std::size_t size,
                               std::vector<std::uint8_t>* out) {
  std::vector<std::uint32_t> cp;
  if (!Decode(input, size, &cp)) {
    return false;
  }
  if (cp.empty()) {
    return true;
  } else {
    if (!Utf8Encode(cp, out)) {
      return false;
    }
    return true;
  }
}

bool TextDecoder::FinishToUtf8(std::vector<std::uint8_t>* out) {
  std::vector<std::uint32_t> cp;
  if (!Finish(&cp)) return false;
  if (cp.empty()) {
    return true;
  } else {
    return Utf8Encode(cp, out);
  }
}

}  // namespace text
}  // namespace base
