#include "cg/x64/deopt-data.h"

using namespace quake::lir;

namespace quake {
namespace cg {
namespace x64 {

DeoptDataX64::RawValue DeoptDataX64::GetSpill(std::int32_t spill_slot) const {
  const auto spill_addr =
      reinterpret_cast<const RawValue*>(byte_rbp() + spill_slot * kAlignment);

  return *spill_addr;
}

std::size_t DeoptDataX64::GetSpillSize() const {
  const std::size_t total_diff_in_bytes = byte_rsp() - byte_rbp();
  return total_diff_in_bytes / kAlignment;
}

DeoptDataX64::RawValue DeoptDataX64::GetRegister(RegisterKind rk) const {
  const auto index = static_cast<std::uint32_t>(rk);
  return *reinterpret_cast<const RawValue*>(reg_start() + index * 8);
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
