#ifndef _BASE_TEXT_GBK_CODEC_H_
#define _BASE_TEXT_GBK_CODEC_H_
#include "_/gbx-codec.h"

namespace base {
namespace text {

class GBKDecoder : public _::GBXDecoder {
 public:
  GBKDecoder() : _::GBXDecoder("GBK") {}

 public:
  // Convinient method for external usage
  static inline bool ToUtf8(const void*, std::size_t,
                            std::vector<std::uint8_t>*);

  static inline bool ToUnicode(const void*, std::size_t,
                               std::vector<std::uint32_t>*);
};

inline bool GBKDecoder::ToUtf8(const void* input, std::size_t input_size,
                               std::vector<std::uint8_t>* output) {
  GBKDecoder d;
  return d.DecodeToUtf8(input, input_size, output) && d.FinishToUtf8(output);
}

inline bool GBKDecoder::ToUnicode(const void* input, std::size_t input_size,
                                  std::vector<std::uint32_t>* output) {
  GBKDecoder d;
  return d.Decode(input, input_size, output) && d.Finish(output);
}

}  // namespace text
}  // namespace base

#endif  // _BASE_TEXT_GBK_CODEC_H_
