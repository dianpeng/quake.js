#ifndef _BASE_TEXT_GB2312_CODEC_H_
#define _BASE_TEXT_GB2312_CODEC_H_
#include "_/gbx-codec.h"

namespace base {
namespace text {

class GB2312Decoder : public _::GBXDecoder {
 public:
  GB2312Decoder() : _::GBXDecoder("gb2312") {}

 public:
  // Convinient method for external usage
  static inline bool ToUtf8(const void*, std::size_t,
                            std::vector<std::uint8_t>*);

  static inline bool ToUnicode(const void*, std::size_t,
                               std::vector<std::uint32_t>*);
};

inline bool GB2312Decoder::ToUtf8(const void* input, std::size_t input_size,
                                  std::vector<std::uint8_t>* output) {
  GB2312Decoder d;
  return d.DecodeToUtf8(input, input_size, output) && d.FinishToUtf8(output);
}

inline bool GB2312Decoder::ToUnicode(const void* input, std::size_t input_size,
                                     std::vector<std::uint32_t>* output) {
  GB2312Decoder d;
  return d.Decode(input, input_size, output) && d.Finish(output);
}

}  // namespace text
}  // namespace base

#endif  // _BASE_TEXT_GBK_CODEC_H_
