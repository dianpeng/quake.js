#include <base/text/utf-codec.h>

namespace base {
namespace text {

bool Utf8Decoder::Decode(const void* data, std::size_t length,
                         std::vector<std::uint32_t>* output) {
  // Already failed in the previous call and haven't reset
  if (in_error_) {
    return false;
  }

  // Given an empty sequences, just return true
  if (length == 0) {
    return true;
  }

  // Skip BOM if needed
  std::uint32_t maybe_bom = 0;
  switch (DecodeBOM(&data, &length, &maybe_bom)) {
    case kAccept:
    case kReject:
      break;
    default:
      return true;
  }

  if (maybe_bom == 0xfeff) {
    DCHECK(handle_bom_);
    DCHECK(ignore_bom_);
    output->push_back(0xfeff);
  }

  // Helper routine to decode a span of data
  auto decode_span = [&](const void* data, std::size_t length) {
    const auto byte_ptr = static_cast<const std::uint8_t*>(data);
    for (std::size_t i = 0; i < length; ++i) {
      std::uint32_t oc;
      switch (ProcessOneByte(byte_ptr[i], &oc)) {
        case kAccept:
          output->push_back(oc);
          break;

        case kReject:
          in_error_ = true;
          return false;

        case kRetry:
          if (error_recovery_) {
            --i;
            break;
          } else {
            in_error_ = true;
            return false;
          }

        default:
          break;
      }
    }

    return true;
  };

  // Similar as main decoding loop, consume remaining bytes in the bom buffer
  {
    auto bom_buf = bom_buffer();
    if (bom_buf->size) {
      if (!decode_span(reinterpret_cast<const void*>(bom_buf->buffer),
                       static_cast<std::size_t>(bom_buf->size)))
        return false;

      // bom buffer has been drained
      bom_buf->size = 0;
    }
  }

  // Main decoding loop
  return decode_span(data, length);
}

bool IsASCIIEncoding(const std::uint8_t* b, std::size_t l) {
  // This loop is super fast due to the loop body is small which can
  // easily fit into CPU's loop buffer (<= 16 bytes on most arch).
  // The interesting thing is when doing decoding, it is mostly worth the
  // efforts to detect whether the input sequence is a ASCII to avoid decoding
  // utf8/16/32. And in general, the result algorithm is way more faster in
  // practical usage. Most input on the web are chunks of ASCII when you
  // consider things like API call, etc ...
  auto is_ascii = false;
  {
    std::uint8_t char_mask = 0;
    for (std::size_t i = 0; i < l; ++i) {
      char_mask |= b[i];
    }
    is_ascii = !((char_mask & 0x80));
  }

  return is_ascii;
}

bool IsLatin1Encoding(const std::uint8_t* b, std::size_t l) {
  auto end = b + l;

  while (b != end) {
    std::uint32_t cp;
    if (auto maybe_bump = Utf8Decoder::DecodeOne(b, end - b, &cp);
        !maybe_bump) {
      return false;
    } else {
      if (cp > 255) {
        return false;
      }

      b += *maybe_bump;
    }
  }

  return true;
}

std::string Latin1ToUtf8(const std::uint8_t* b, std::size_t l) {
  std::string r;

  if (!IsASCIIEncoding(b, l)) {
    r.reserve(l * 1.5);
    for (std::size_t i = 0; i < l; ++i) {
      std::uint8_t e1, e2;
      if (Latin1ToUtf8(b[i], &e1, &e2) == 2) {
        r.push_back(static_cast<char>(e1));
        r.push_back(static_cast<char>(e2));
      } else {
        r.push_back(static_cast<char>(e1));
      }
    }
  } else {
    r.assign(reinterpret_cast<const char*>(b), l);
  }

  return r;
}

}  // namespace text
}  // namespace base
