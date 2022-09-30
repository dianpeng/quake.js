#ifndef _QUAKE_CG_X64_DEOPT_DATA_H_
#define _QUAKE_CG_X64_DEOPT_DATA_H_

#include "base.h"
#include "deopt.h"
#include "cg/x64/register.h"

namespace quake {
namespace cg {
namespace x64 {

class DeoptDataX64 : public DeoptData {
 public:
  static const std::size_t kSavedRegisterSize = 8 * (16 + 16);
  static const std::size_t kAlignment = 8;

 public:
  using RawValue = DeoptData::RawValue;
  DeoptDataX64(const void* rbp, const void* rsp) : rbp_(rbp), rsp_(rsp) {}
  ~DeoptDataX64() override {}

  RawValue GetSpill(std::int32_t spill_slot) const override;

  // Notes this contains stack adjustment for SSE
  std::size_t GetSpillSize() const override;

  RawValue GetRegister(lir::RegisterKind) const override;

  // Context is been stored inside of R15
  QuakeRT* GetQuakeRT() const override {
    QuakeRT* quake_rt = nullptr;
    auto v = GetRegister(RegisterKind::kR15);
    std::memcpy(&quake_rt, &v, sizeof(v));
    return quake_rt;
  }

  // DeoptId is been stored inside of the rax
  Deopt::DeoptId GetDeoptId() const override {
    return static_cast<Deopt::DeoptId>(GetRegister(RegisterKind::kRax));
  }

 private:
  const std::uint8_t* byte_rbp() const {
    return static_cast<const std::uint8_t*>(rbp_);
  }
  const std::uint8_t* byte_rsp() const {
    return static_cast<const std::uint8_t*>(rsp_);
  }
  const std::uint8_t* reg_start() const {
    return byte_rsp() + kSavedRegisterSize;
  }

 private:
  const void* rbp_;
  const void* rsp_;
};

}  // namespace x64
}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_X64_DEOPT_DATA_H_
