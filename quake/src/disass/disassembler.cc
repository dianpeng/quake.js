#include "disass/disassembler.h"

#ifdef QUAKE_TARGET_X64
#include <Zydis/Zydis.h>

namespace {
using namespace quake::disass;

class X64Dis : public Disassembler {
 public:
  bool HasNext() const override;
  void Next() override;
  std::optional<std::string> Print() const override;
  std::size_t Cursor() const override { return cursor_; }

  X64Dis(const void* buffer, std::size_t size)
      : buffer_(buffer), size_(size), cursor_(0) {
    ZydisDecoderInit(&decoder_, ZYDIS_MACHINE_MODE_LONG_64,
                     ZYDIS_ADDRESS_WIDTH_64);
    ZydisFormatterInit(&fmt_, ZYDIS_FORMATTER_STYLE_INTEL);
    ZydisFormatterSetProperty(&fmt_, ZYDIS_FORMATTER_PROP_FORCE_SIZE,
                              ZYAN_TRUE);
  }

 private:
  const void* buffer_;
  std::size_t size_;
  std::size_t cursor_;

  mutable ZydisFormatter fmt_;
  mutable ZydisDecoder decoder_;
  mutable bool decode_error_ = false;
  mutable std::size_t ins_size_ = 0;
};

bool X64Dis::HasNext() const {
  if (decode_error_) return false;
  return cursor_ < size_;
}

void X64Dis::Next() {
  if (decode_error_) {
    cursor_ = size_;
  } else {
    cursor_ += ins_size_;
    ins_size_ = 0;
  }
}

std::optional<std::string> X64Dis::Print() const {
  CHECK(cursor_ < size_);

  ZydisDecodedInstruction ins;
  auto addr = static_cast<const char*>(buffer_) + cursor_;
  if (ZYAN_SUCCESS(
          ZydisDecoderDecodeBuffer(&decoder_, addr, size_ - cursor_, &ins))) {
    std::string data;

    data.resize(2048);
    ZydisFormatterFormatInstruction(&fmt_, &ins, data.data(), data.size(),
                                    reinterpret_cast<ZyanU64>(addr));

    // ZydisFormatterFormat function does not return size, but just set
    // null terminator.
    auto len = std::strlen(data.data());
    data.resize(len);

    ins_size_ = ins.length;
    return base::Fmt("%p: %s", addr, data.c_str());
  } else {
    decode_error_ = true;
    ins_size_ = 0;
    return {};
  }
}

}  // namespace

#endif  // QUAKE_TARGET_X64

namespace quake {
namespace disass {

std::unique_ptr<Disassembler> CreateDisassembler(const void* buffer,
                                                 std::size_t buffer_size) {
#ifdef QUAKE_TARGET_X64
  return std::make_unique<X64Dis>(buffer, buffer_size);
#else
  UNREACHABLE();
  return {};
#endif  // QUAKE_TARGET_X64
}

}  // namespace disass
}  // namespace quake

