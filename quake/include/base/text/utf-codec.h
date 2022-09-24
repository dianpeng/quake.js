#ifndef _BASE_TEXT_UTF_CODEC_H_
#define _BASE_TEXT_UTF_CODEC_H_
#include <base/check.h>
#include <base/common.h>

#include "text-codec.h"
#include "utf-internal.h"
#include "utf8-internal.h"

namespace base {
namespace text {

// Utf8Decoder. Used to decode binary, pressumpbly encoded via utf-8, into
// clear code point. Notes, the Web's TextDecoder spec define a stupid flag
// called ignoreBom, but gives no algorithm or semantic to describe the
// behavior about it. Currently the decoder treat BOM correctly, as garbage.
// It can bypass it correctly initially. So this gives maximum tolerance and
// the code rely on it should never be needed to be aware of BOM.
class Utf8Decoder : public TextDecoder {
 private:
  // See below for usage
  struct BOMBuffer {
    std::uint8_t size;
    std::uint8_t buffer[3];
    BOMBuffer() : size(0) {}
  };

 public:
  explicit Utf8Decoder()
      : bom_buffer_(),
        out_buffer_(0),
        state_(utf8::State::kAccept),
        handle_bom_(false),
        ignore_bom_(false),
        assume_no_bom_(false),
        error_recovery_(false),
        in_error_(false) {}

  bool Decode(const void*, std::size_t, std::vector<std::uint32_t>*) override;
  inline bool Finish(std::vector<std::uint32_t>*) override;
  const char* GetName() const override { return "utf-8"; }

  void SetOption(TextDecoder::Option&& opt) override {
    ignore_bom_ = opt.ignore_bom;
    assume_no_bom_ = opt.assume_no_bom;
    error_recovery_ = opt.error_recovery;
  }

  bool ignore_bom() const override { return ignore_bom_; }
  bool assume_no_bom() const override { return assume_no_bom_; }
  bool error_recovery() const override { return error_recovery_; }

 public:
  static constexpr std::size_t kBOMHeaderSize = 3;

  // Helper function to skip the BOM if applicable for the input binary
  static inline const void* TrySkipBOM(const void*, std::size_t);

  // Decode one code point from the UTF-8 sequences. If returns empty, means
  // error happened. This doesn't handle BOM.
  static inline std::optional<std::size_t> DecodeOne(const void*, std::size_t,
                                                     std::uint32_t*);

 private:
  // ------------------------------------------------------------------
  // Result status for processing one single byte for utf8 decoding
  enum Result { kAccept, kReject, kRetry, kIncomplete };

  Result DecodeBOM(const void**, std::size_t*, std::uint32_t*);

  // Try to process one byte specified. Notes the internal field state_ and
  // out_buffer_ will be modified accordingly. The storage_ and external buffer
  // is left untouched and caller is supposed to handle it properly
  Result ProcessOneByte(std::uint8_t, std::uint32_t*);

  std::uint32_t* out_buffer() { return &out_buffer_; }

  BOMBuffer* bom_buffer() { return &bom_buffer_; }

 private:
  // BOM buffer, used to store the BOM data if applicable, notes, it may contain
  // remaining data for detecting BOM, ie the BOM may fail to consume the data
  // inside of the buffer, ie they are just normal bytes.
  BOMBuffer bom_buffer_;

  // out buffer for holding states of utf8 decoder
  std::uint32_t out_buffer_;

  // parser status. ie DFA related
  utf8::State state_;

  // options ---------------------------
  bool handle_bom_;
  bool ignore_bom_;
  bool assume_no_bom_;
  bool error_recovery_;
  bool in_error_;

  DISALLOW_COPY_AND_ASSIGN(Utf8Decoder);
};

// -----------------------------------------------------------------
// Utf8 related free functions
// Used by various part inside of the code for basic encoding manipulation

// Encode a codepoint into the buffer provided, at least 4 bytes. On success,
// ie the codepoint is valid, it returns the encoding size, otherwise empty
inline std::optional<std::size_t> Utf8Encode(std::uint32_t, std::uint8_t*);
inline bool Utf8Encode(const std::vector<std::uint32_t>& input,
                       std::vector<std::uint8_t>*);

// -----------------------------------------------------------------
// Latin1 related stuff
//
//   Latin1, ie ISO8895-1 encoding format is a 256 bits encoding
//   method which can cover up to a byte code point. It is superset
//   of ASCII scheme.
//
// Mostly we use UTF-8 encoding style due to Web use it as default

// Check a given |UTF8| encoded input contains only ASCII encoding
bool IsASCIIEncoding(const std::uint8_t*, std::size_t);

// Check a given |UTF8| encoded input contains only Latin1 encoding
bool IsLatin1Encoding(const std::uint8_t*, std::size_t);

inline void Latin1NoneASCIIToUtf8(std::uint8_t, std::uint8_t*, std::uint8_t*);
inline std::size_t Latin1ToUtf8(std::uint8_t, std::uint8_t*, std::uint8_t*);

std::string Latin1ToUtf8(const std::uint8_t*, std::size_t);
inline std::string Latin1ToUtf8(const char*, std::size_t);
inline std::string Latin1ToUtf8(const std::string&);

// -----------------------------------------------------------------
// Utf16 related

template <typename O>
std::size_t Utf16ToUtf8Buffer(const std::uint16_t*, std::size_t, O*,
                              std::size_t*);
template <typename Predicate, typename Writer>
std::size_t Utf16ToUtf8(const std::uint16_t*, std::size_t, Predicate&&,
                        Writer&&);

// Encode a codepoint as utf16 into a uint16_t buffer, the buffer will at least
// have 2 code points
inline std::size_t Utf16Encode(std::uint32_t, std::uint16_t*);

}  // namespace text
}  // namespace base

#include "utf-codec-inl.h"

#endif  // _BASE_TEXT_UTF_CODEC_H_
