#include <base/text/_/gbx-codec.h>
#include <base/text/_/iconv-util.h>
#include <base/text/utf-codec.h>
#include <cerrno>

namespace base {
namespace text {
namespace _ {

std::optional<std::size_t> GBXDecoder::Convert(
    const void* input, std::size_t input_size,
    std::vector<std::uint8_t>* output) {
  static constexpr std::size_t kInvalid = static_cast<std::size_t>(-1);
  static constexpr std::size_t kChunkSize = kKB * 16;

  if (input_size == 0) {
    output->clear();
    return 0;
  }

  std::size_t consumed_size = input_size;

  // Checking if we have left data inside of the ool_buffer_
  if (ool_buffer_) {
    auto ool_char = Pass(&ool_buffer_);
    char tbuf[2] = {static_cast<char>(ool_char),
                    (static_cast<const char*>(input))[0]};

    char obuf[4];  // at most 4 bytes will be generated for a single utf8

    std::size_t osize = 4;
    std::size_t isize = 2;
    char* iptr = tbuf;
    char* optr = obuf;

    // iconv does touch the errno field
    errno = 0;
    if (auto ret = ::iconv(iconv_, &iptr, &isize, &optr, &osize);
        ret == kInvalid) {
      // the possible value are as following per man page
      if (errno == EILSEQ) {
        return {};
      } else if (errno == EINVAL) {
        // this situation really should not happen since it indicates the input
        // seqeuence is not complete, which should not be the case, anyway, we
        // just hint an error here and
        LOG(BUG) << name_ << " conversion meet unexpected error: "
                 << "the input sequences are : ["
                 << static_cast<std::uint32_t>(tbuf[0]) << ", "
                 << static_cast<std::uint32_t>(tbuf[1])
                 << "] and this sequence cannot be converted to UTF8";
        return {};
      } else {
        DCHECK(errno == E2BIG);

        LOG(BUG) << name_ << " conversion meet unexpected error: "
                 << "the input sequences are : ["
                 << static_cast<std::uint32_t>(tbuf[0]) << ", "
                 << static_cast<std::uint32_t>(tbuf[1])
                 << "] and it complains 4 bytes buffer are too small";
        return {};
      }
    } else {
      output->insert(output->end(), obuf, obuf + (4 - osize));
      input = PtrForward<void>(input, 1);
      input_size -= 1;
    }
  }

  if (input_size == 0) {
    return consumed_size;
  }

  auto input_char = IconvConstCharToCharArray(input, input_size);
  auto input_end = PtrForward<void>(input_char, input_size);

  // Encode the rest GBX into the output buffer. since we don't know the exact
  // size of the GBX output, we do an guess, which at most will be 4 times of
  // the input, this may cause too much memory though
  do {
    auto size = std::min(kChunkSize, input_size * 4);
    auto old_size = output->size();
    output->resize(old_size + size);

    auto optr = reinterpret_cast<char*>(output->data()) + old_size;
    auto osize = size;

    errno = 0;
    if (auto ret = ::iconv(iconv_, &input_char, &input_size, &optr, &osize);
        ret == kInvalid) {
      output->resize(old_size + (size - osize));

      if (errno == EILSEQ) {
        return {};
      } else if (errno == EINVAL) {
        // sequence are splitted, we should expect that the input has been
        // consumed done
        auto left = PtrDifference(input_end, input_char);
        CHECK(left == 1);
        ool_buffer_.emplace(*reinterpret_cast<const std::uint8_t*>(input_char));
        return consumed_size;
      } else {
        DCHECK(errno == E2BIG);
        continue;
      }
    } else {
      output->resize(old_size + (size - osize));
      return consumed_size;
    }

  } while (true);

  UNREACHABLE();
  return {};
}

// Used to flush the libiconv's internal status, so the decoder can be used
// after one parsing done
void GBXDecoder::ResetIConv() {
  char tmp[4];
  auto obuf = tmp;
  std::size_t osz = 4;

  ::iconv(iconv_, nullptr, nullptr, &obuf, &osz);
  UNUSE(obuf);
  UNUSE(osz);
}

bool GBXDecoder::FinishConversion() {
  ResetIConv();
  if (ool_buffer_) {
    ool_buffer_.reset();
    return false;
  } else {
    return true;
  }
}

bool GBXDecoder::DecodeToUtf8(const void* input, std::size_t input_size,
                              std::vector<std::uint8_t>* output) {
  if (auto r = Convert(input, input_size, output); !r) {
    ResetIConv();
    return false;
  } else {
    return true;
  }
}

bool GBXDecoder::FinishToUtf8(std::vector<std::uint8_t>*) {
  return FinishConversion();
}

bool GBXDecoder::Decode(const void* input, std::size_t input_size,
                        std::vector<std::uint32_t>* output) {
  std::vector<std::uint8_t> utf8_output;

  // need a utf8decoder helper to convert utf8 codec into unicode
  Utf8Decoder utf8_decoder{};

  auto r = Convert(input, input_size, &utf8_output);

  if (!r) {
    ResetIConv();
    return false;
  }

  // convert all the utf8 to unicode code point
  return utf8_decoder.Decode(utf8_output.data(), utf8_output.size(), output);
}

bool GBXDecoder::Finish(std::vector<std::uint32_t>*) {
  return FinishConversion();
}

}  // namespace _
}  // namespace text
}  // namespace base
