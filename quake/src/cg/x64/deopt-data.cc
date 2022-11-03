#include "cg/x64/deopt-data.h"
#include "cg/x64/register.h"
#include "quake.h"
#include "quake-rt.h"
#include "qjs.h"

using namespace quake::lir;

namespace quake {
namespace cg {
namespace x64 {

DeoptDataX64::DeoptDataX64(const void* rbp, const void* rsp)
    : rbp_(rbp), rsp_(rsp), quake_(GetQuakeRT()->quake) {}

DeoptDataX64::RawValue DeoptDataX64::GetSpill(std::int32_t spill_slot) const {
  const auto offset = quake_->cell_size();  // offset from the cell region

  const auto spill_addr = reinterpret_cast<const RawValue*>(
      byte_rbp() - offset - spill_slot * kAlignment - kAlignment);

  return *spill_addr;
}

std::size_t DeoptDataX64::GetSpillCount() const {
  return quake_->spill_size() / kAlignment;
}

DeoptDataX64::RawValue DeoptDataX64::GetRegister(RegisterKind rk) const {
  const auto index = static_cast<std::uint32_t>(rk);
  return *reinterpret_cast<const RawValue*>(reg_start() - index * 8 - 8);
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
