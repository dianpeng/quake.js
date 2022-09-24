#ifndef _BASE_TEXT___GBX_CODEC_H_
#define _BASE_TEXT___GBX_CODEC_H_
#include <base/check.h>
#include <base/common.h>

#include <base/text/text-codec.h>

#ifdef OS_LINUX
#include <iconv.h>
#else
#error "unsupported platform"
#endif  // OS_LINUX

namespace base {
namespace text {
namespace _ {

// The GBX codec is used in some Chinese website, notebly the tencent's main
// site qq.com is been transferred via GBX encoding. In web standard,
// specifically the service worker standard, it requires the page/data to be
// transferred in UTF-8 codec. To support some Chinese web sites, we will have
// to do codec transferring here.
//
// The main conversion happened here is to transfer gbk into utf8 and then it
// can work with rest of the APIs
class GBXDecoder : public TextDecoder {
  static iconv_t InvalidHandler() {
    static const std::intptr_t kInvPtr = -1;
    static_assert(sizeof(std::intptr_t) == sizeof(iconv_t));
    iconv_t v;
    std::memcpy(&v, &kInvPtr, sizeof(v));
    return v;
  }

 public:
  explicit GBXDecoder(const char* codec_name)
      : ool_buffer_(), iconv_(), name_(codec_name) {
    iconv_ = ::iconv_open("UTF-8", codec_name);
    CHECK(iconv_ != InvalidHandler())
        << "The linux library libiconv does not support " << codec_name
        << " and UTF conversion";
  }

  ~GBXDecoder() {
    if (iconv_ != InvalidHandler()) {
      ::iconv_close(iconv_);
    }
  }

  // Convert the GBX into unicode codec. This is done by using GBX to UTF-8 and
  // then convert UTF-8 back to unicode
  bool Decode(const void*, std::size_t, std::vector<std::uint32_t>*) override;

  // Finishing the GBX conversion
  bool Finish(std::vector<std::uint32_t>*) override;

  // Convert the GBX into the UTF-8 codec incrementally
  bool DecodeToUtf8(const void*, std::size_t,
                    std::vector<std::uint8_t>*) override;

  bool FinishToUtf8(std::vector<std::uint8_t>*);

 public:
  const char* GetName() const override { return name_.c_str(); }

  void SetOption(TextDecoder::Option&& opt) override { UNUSE(opt); }
  bool ignore_bom() const override { return true; }
  bool assume_no_bom() const override { return true; }
  bool error_recovery() const override { return false; }

 private:
  // Helper decode input into utf8 codec, if we have invalid codec point then
  // returns none, otherwise returns size of consumed bytes in input
  std::optional<std::size_t> Convert(const void*, std::size_t,
                                     std::vector<std::uint8_t>*);
  bool FinishConversion();
  void ResetIConv();

 private:
  // The GBX can occupy at most 2 bytes, so we just need to hold the last byte
  std::optional<std::uint8_t> ool_buffer_;

  // handler to perform GBX conversion, we just use GBX to UTF8
  iconv_t iconv_;

  // name of the codec
  std::string name_;
};

}  // namespace _
}  // namespace text
}  // namespace base

#endif  // _BASE_TEXT___GBX_CODEC_H_
