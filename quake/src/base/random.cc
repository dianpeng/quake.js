#include <base/random.h>

#include <random>

namespace base {
namespace {

class RndEngine {
 public:
  RndEngine() : device_(), randomizer_(device_()) {}
  std::mt19937& randomizer() { return randomizer_; }

 private:
  std::random_device device_;
  std::mt19937 randomizer_;
};

static thread_local std::unique_ptr<RndEngine> kEngine;

RndEngine* Get() {
  if (!kEngine) {
    kEngine = std::make_unique<RndEngine>();
  }
  return kEngine.get();
}

template <typename T>
T Rnd(const T& min, const T& max) {
  if (min >= max) {
    return min;
  }

  auto eng = Get();
  std::uniform_int_distribution<T> dist{min, max};
  return dist(eng->randomizer());
}

}  // namespace

std::int64_t RandInt64(std::int64_t min, std::int64_t max) {
  return Rnd(min, max);
}

std::uint64_t RandUInt64(std::uint64_t min, std::uint64_t max) {
  return Rnd(min, max);
}

std::uint8_t RandByte() { return Rnd<std::uint8_t>(0, 255); }

void RandByte(std::uint8_t* input, std::size_t len) {
  auto eng = Get();
  auto& rnd = eng->randomizer();

  std::uniform_int_distribution<std::uint8_t> gen{0, 255};
  for (std::size_t i = 0; i < len; ++i) {
    input[i] = gen(rnd);
  }
}

void RandString(std::string* o, std::size_t length) {
  static const char kCharTable[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
      "0123456789"
      "!@#$%^&*()+-?"
      "_<>?{}[]a|\\:,/\"/\'";
  static const std::size_t kCharTableSize = ArraySize(kCharTable) - 1;
  o->reserve(length);

  auto eng = Get();
  auto& rnd = eng->randomizer();
  std::uniform_int_distribution<std::uint8_t> gen{0, kCharTableSize};
  for (std::size_t i = 0; i < length; ++i) {
    o->push_back(kCharTable[gen(rnd)]);
  }
}

}  // namespace base
