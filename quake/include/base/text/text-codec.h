#ifndef _BASE_TEXT_TEXT_CODEC_H_
#define _BASE_TEXT_TEXT_CODEC_H_
#include <base/common.h>

namespace base {
namespace text {

// TextDecoder is a streaming based decoder interfaces allow user to convert
// text encoded by various standard, ie utf8/16/32 to other format.
//
// Notes, this object is a |stateful| interface and have a streaming semantic.
// Each call of Decode with buffer incrementally bumpping may result in some
// data been deocded and stored inside of the vector, if the previously provided
// data is not enough for decoding, the data will be temporary stored along with
// decode itself and will be prepend when the next |Decode| is called. If user
// want to signal the end of data, user needs to call Finish function to reset
// the state. After |Finish| call the decoder can be reused again.
class TextDecoder {
 public:
  virtual bool Decode(const void*, std::size_t,
                      std::vector<std::uint32_t>*) = 0;

  // Indication of the end of the parsing/decoding. The call will reset the
  // internal buffer and the object can be resued again after the call.
  virtual bool Finish(std::vector<std::uint32_t>*) = 0;

  // Implementation that allows us to transfer a encoding to utf8. Notes, the
  // default implementation may not be very performant, since it does Decode to
  // 32 bits code point and then transfer them into utf8. The derived the codec
  // can do it much more performant
  virtual bool DecodeToUtf8(const void*, std::size_t,
                            std::vector<std::uint8_t>*);
  virtual bool FinishToUtf8(std::vector<std::uint8_t>*);

  // Get the name of the decoder
  virtual const char* GetName() const = 0;

  // Option for the text decoder. Some of them may not be useful for certain
  // types of text decoder
  struct Option {
    // BOM related options
    //
    // ignore_bom, if is true, and if BOM presents, the output unicode will have
    // the BOM marker as 0xfeff, otherwise BOM will be swallowed. Default to be
    // swallowed by the decoder
    bool ignore_bom;

    // Tell decoder that the bom header will never present
    bool assume_no_bom;

    // Whether perform error recovery, currently not in use
    bool error_recovery;

    Option(bool ib = false, bool nb = false, bool er = false)
        : ignore_bom(ib), assume_no_bom(nb), error_recovery(er) {}

    static Option IgnoreBOM() { return Option(true); }
    static Option AssumeNoBOM() { return Option(false, true); }
  };

  // Call this function before the decode/finish call etc. After the settings,
  // it |MAY| impact the next Decode/Finish operation
  virtual void SetOption(Option&&) = 0;

  virtual bool ignore_bom() const = 0;
  virtual bool assume_no_bom() const = 0;
  virtual bool error_recovery() const = 0;

  virtual ~TextDecoder() {}

 public:
  static std::unique_ptr<TextDecoder> Create(const std::string&);
};

}  // namespace text
}  // namespace base

#endif  // _BASE_TEXT_TEXT_CODEC_H_
