#ifndef _BASE_TEXT_UTF_CODEC_INL_H_
#define _BASE_TEXT_UTF_CODEC_INL_H_

namespace base {
namespace text {

inline std::optional<std::size_t> Utf8Decoder::DecodeOne(const void* data,
                                                         std::size_t size,
                                                         std::uint32_t* oc) {
  auto state = utf8::State::kAccept;
  std::uint32_t o = 0;
  std::size_t off = 0;
  auto bbuffer = static_cast<const std::uint8_t*>(data);

  while (off < size) {
    utf8::Decode(bbuffer[off], &state, &o);
    ++off;

    switch (state) {
      case utf8::State::kAccept:
        *oc = o;
        return off;
      case utf8::State::kReject:
        return {};
      default:
        break;
    }
  }

  // failed with decoding a single byte
  return {};
}

inline Utf8Decoder::Result Utf8Decoder::ProcessOneByte(std::uint8_t byte,
                                                       std::uint32_t* oc) {
  auto old_state = state_;
  utf8::Decode(byte, &state_, out_buffer());

  switch (state_) {
    case utf8::State::kAccept:
      *oc = *out_buffer();
      *out_buffer() = 0;
      return kAccept;

    case utf8::State::kReject:
      *out_buffer() = 0;
      state_ = utf8::State::kAccept;

      // Notes(dpeng): We still put the kRetry here for future updation.
      //
      //   The key idea is to find out the old retry byte and return error
      //   and only retry byte that is in the middle of the sequences.
      return (old_state == utf8::State::kAccept) ? kRetry : kReject;

    default:
      return kIncomplete;
  }
}

inline bool Utf8Decoder::Finish(std::vector<std::uint32_t>* output) {
  if (!handle_bom_) {
    auto bbuf = bom_buffer();
    std::uint32_t cp;
    DEFER([bbuf]() { bbuf->size = 0; });

    // Try to flush the bytes left inside of the bom buffer out for decoding
    for (std::size_t i = 0; i < bbuf->size; ++i) {
      switch (ProcessOneByte(bbuf->buffer[i], &cp)) {
        case kAccept:
          output->push_back(cp);
          break;

        default:
          // Notes(dpeng): can be kReject or kIncomplete, regardlessly these
          // are errors during the finish phase, we are handling the last few
          // bytes here.
          in_error_ = true;
          goto done;
      }
    }
  }

done:
  auto ret = (state_ == utf8::State::kAccept) && !in_error_;

  handle_bom_ = false;
  in_error_ = false;
  state_ = utf8::State::kAccept;
  *out_buffer() = 0;

  return ret;
}

// Notes for DecodeBOM, the DecodeBOM can return following codes :
//
//   kReject, nothing needs to be done, the step of BOM has been skipped
//
//   kAccept, 3 bytes have been consumed from the input, the buffer data
//            has been modified to reflect so
//
//   kIncomplete, wait for more bytes
//
inline Utf8Decoder::Result Utf8Decoder::DecodeBOM(const void** data,
                                                  std::size_t* length,
                                                  std::uint32_t* maybe_bom) {
  if (handle_bom_) {
    return kReject;
  }

  if (assume_no_bom_) {
    handle_bom_ = true;
    return kReject;
  }

  auto pdata = *reinterpret_cast<const std::uint8_t**>(data);
  auto size = *length;
  auto bbuf = bom_buffer();

  if (bbuf->size + size < 3) {
    DCHECK(static_cast<std::size_t>(3 - bbuf->size) > size);
    std::memcpy(bbuf->buffer + bbuf->size, pdata, size);

    bbuf->size += size;
    *data = nullptr;
    *length = 0;
    return kIncomplete;
  }

  auto skip_bom = false;
  std::size_t offset = 0;
  DCHECK(bbuf->size <= 2);

  switch (bbuf->size) {
    case 0:
      offset = 3;
      skip_bom = utf8::StartsWithBOM(pdata[0], pdata[1], pdata[2]);
      break;
    case 1:
      offset = 2;
      skip_bom = utf8::StartsWithBOM(bbuf->buffer[0], pdata[0], pdata[1]);
      break;
    case 2:
      offset = 1;
      skip_bom =
          utf8::StartsWithBOM(bbuf->buffer[0], bbuf->buffer[1], pdata[0]);
      break;
    default:
      UNREACHABLE();
      break;
  }

  if (skip_bom) {
    *data = PtrForward<const void>(*data, offset);
    *length = size - offset;

    // all the data in bom header should be consumed
    bbuf->size = 0;

    if (ignore_bom_) {
      // unicode's BOM value
      *maybe_bom = 0xfeff;
    }
  }

  // Never check the BOM again from now on
  handle_bom_ = true;
  return kAccept;
}

inline std::optional<std::size_t> Utf8Encode(std::uint32_t cp,
                                             std::uint8_t* b) {
  if (!utf::_::IsCodePointValid(cp)) {
    return {};
  }
  auto start = b;

  if (cp < 0x80) {
    *(b++) = static_cast<std::uint8_t>(cp);
  } else if (cp < 0x800) {
    *(b++) = static_cast<std::uint8_t>((cp >> 6) | 0xc0);
    *(b++) = static_cast<std::uint8_t>((cp & 0x3f) | 0x80);
  } else if (cp < 0x10000) {
    *(b++) = static_cast<std::uint8_t>((cp >> 12) | 0xe0);
    *(b++) = static_cast<std::uint8_t>(((cp >> 6) & 0x3f) | 0x80);
    *(b++) = static_cast<std::uint8_t>((cp & 0x3f) | 0x80);
  } else {
    *(b++) = static_cast<uint8_t>((cp >> 18) | 0xf0);
    *(b++) = static_cast<uint8_t>(((cp >> 12) & 0x3f) | 0x80);
    *(b++) = static_cast<uint8_t>(((cp >> 6) & 0x3f) | 0x80);
    *(b++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
  }
  return static_cast<std::size_t>(b - start);
}

inline bool Utf8Encode(const std::vector<std::uint32_t>& in,
                       std::vector<std::uint8_t>* out) {
  auto idx = out->size();
  out->reserve(idx + in.size() * 2);
  for (const auto& entry : in) {
    std::uint8_t t[4];
    auto size = Utf8Encode(entry, t);
    if (size) {
      switch (*size) {
        case 1:
          out->push_back(t[0]);
          break;
        case 2:
          out->push_back(t[0]);
          out->push_back(t[1]);
          break;
        case 3:
          out->push_back(t[0]);
          out->push_back(t[1]);
          out->push_back(t[2]);
          break;
        case 4:
          out->push_back(t[0]);
          out->push_back(t[1]);
          out->push_back(t[2]);
          out->push_back(t[3]);
          break;
        default:
          UNREACHABLE();
          break;
      }
    } else {
      return false;
    }
  }
  return true;
}

inline const void* Utf8Decoder::TrySkipBOM(const void* data, std::size_t len) {
  auto cdata = static_cast<const char*>(data);
  if (len >= 3) {
    if (utf8::StartsWithBOM(cdata, cdata + len)) {
      return reinterpret_cast<const void*>(cdata + 3);
    }
  }
  return data;
}

inline std::size_t Latin1ToUtf8(std::uint8_t latin, std::uint8_t* o1,
                                std::uint8_t* o2) {
  if (latin < 0x80) {
    *o1 = latin;
    return 1;
  } else {
    *o1 = (0xc0 | (latin >> 6));
    *o2 = (0x80 | (latin & 0x3f));
    return 2;
  }
}

inline void Latin1NoneASCIIToUtf8(std::uint8_t latin, std::uint8_t* o1,
                                  std::uint8_t* o2) {
  DCHECK(latin >= 0x80);
  *o1 = (0xc0 | (latin >> 6));
  *o2 = (0x80 | (latin & 0x3f));
}

inline std::string Latin1ToUtf8(const char* b, std::size_t l) {
  return Latin1ToUtf8(reinterpret_cast<const std::uint8_t*>(b), l);
}

inline std::string Latin1ToUtf8(const std::string& b) {
  return Latin1ToUtf8(b.c_str(), b.size());
}

// ----------------------------------------------------------------
//  Utf16 -> Utf8
// ----------------------------------------------------------------
template <typename Predicate, typename Writer>
std::size_t Utf16ToUtf8(const std::uint16_t* buffer, std::size_t length,
                        Predicate&& pred, Writer&& wr) {
  const auto end = buffer + length;
  auto start = buffer;
  std::size_t ccount = 0;
  std::size_t utf8 = 0;

  while (pred(ccount, utf8) && (buffer != end)) {
    std::size_t bump = 1;
    std::uint32_t cp = utf::_::Mask16(buffer[0]);

    if (utf::_::IsLeadSurrogate(cp)) {
      if ((buffer + 1) != end) {
        const std::uint32_t trail_surrogate = utf::_::Mask16(buffer[1]);
        if (utf::_::IsTrailSurrogate(trail_surrogate)) {
          cp = (cp << 10) + trail_surrogate + utf::_::kSurrogateOffset;
          ++bump;
        } else {
          goto unfinished;
        }
      } else {
        goto unfinished;
      }
    } else if (utf::_::IsTrailSurrogate(cp)) {
      goto unfinished;
    }

    if (!utf::_::IsCodePointValid(cp)) {
      goto unfinished;
    }

    buffer += bump;
    ++ccount;

    {
      if (cp < 0x80) {
        char b[1] = {static_cast<char>(cp)};
        wr(b, 1);
        utf8 += 1;
      } else if (cp < 0x800) {
        char b[2] = {static_cast<char>((cp >> 6) | 0xc0),
                     static_cast<char>((cp & 0x3f) | 0x80)};
        wr(b, 2);
        utf8 += 2;
      } else if (cp < 0x10000) {
        char b[3] = {static_cast<char>((cp >> 12) | 0xe0),
                     static_cast<char>(((cp >> 6) & 0x3f) | 0x80),
                     static_cast<char>((cp & 0x3f) | 0x80)};
        wr(b, 3);
        utf8 += 3;
      } else {
        char b[4] = {static_cast<char>((cp >> 18) | 0xf0),
                     static_cast<char>(((cp >> 12) & 0x3f) | 0x80),
                     static_cast<char>(((cp >> 6) & 0x3f) | 0x80),
                     static_cast<char>((cp & 0x3f) | 0x80)};
        wr(b, 4);
        utf8 += 4;
      }
    }
  }

unfinished:
  return (buffer - start);
}

template <typename O>
std::size_t Utf16ToUtf8Buffer(const std::uint16_t* buffer, std::size_t length,
                              O* output, std::size_t* utf8_length) {
  const auto end = buffer + length;
  auto start = buffer;
  std::size_t ccount = 0;
  std::size_t utf8 = 0;

  while ((buffer != end)) {
    std::size_t bump = 1;
    std::uint32_t cp = utf::_::Mask16(buffer[0]);

    if (utf::_::IsLeadSurrogate(cp)) {
      if ((buffer + 1) != end) {
        std::uint32_t trail_surrogate = utf::_::Mask16(buffer[1]);
        if (utf::_::IsTrailSurrogate(trail_surrogate)) {
          cp = (cp << 10) + trail_surrogate + utf::_::kSurrogateOffset;
          ++bump;
        } else {
          goto unfinished;
        }
      } else {
        goto unfinished;
      }
    } else if (utf::_::IsTrailSurrogate(cp)) {
      goto unfinished;
    }

    if (!utf::_::IsCodePointValid(cp)) {
      goto unfinished;
    }

    buffer += bump;
    ++ccount;

    {
      if (cp < 0x80) {
        output->push_back(static_cast<char>(cp));
        utf8 += 1;
      } else if (cp < 0x800) {
        output->push_back(static_cast<char>((cp >> 6) | 0xc0));
        output->push_back(static_cast<char>((cp & 0x3f) | 0x80));
        utf8 += 2;
      } else if (cp < 0x10000) {
        output->push_back(static_cast<char>((cp >> 12) | 0xe0));
        output->push_back(static_cast<char>(((cp >> 6) & 0x3f) | 0x80));
        output->push_back(static_cast<char>((cp & 0x3f) | 0x80));
        utf8 += 3;
      } else {
        output->push_back(static_cast<char>((cp >> 18) | 0xf0));
        output->push_back(static_cast<char>(((cp >> 12) & 0x3f) | 0x80));
        output->push_back(static_cast<char>(((cp >> 6) & 0x3f) | 0x80));
        output->push_back(static_cast<char>((cp & 0x3f) | 0x80));
        utf8 += 4;
      }
    }
  }

unfinished:
  if (utf8_length) {
    *utf8_length = utf8;
  }
  return (buffer - start);
}

inline std::size_t Utf16Encode(std::uint32_t cp, std::uint16_t* buffer) {
  if (cp > 0xffff) {
    *buffer++ = static_cast<std::uint16_t>((cp >> 10) + utf::_::kLeadOffset);
    *buffer =
        static_cast<std::uint16_t>((cp & 0x3ff) + utf::_::kTrailSurrogateMin);
    return 2;
  } else {
    *buffer = static_cast<std::uint16_t>(cp);
    return 1;
  }
}

}  // namespace text
}  // namespace base

#endif  // _BASE_TEXT_UTF_CODEC_INL_H_
