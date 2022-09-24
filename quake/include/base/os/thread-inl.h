#ifndef _BASE_OS_THREAD_INL_H_
#define _BASE_OS_THREAD_INL_H_

namespace base {
namespace os {

inline void Thread::BlockForInit() {
  static const std::uint32_t kMax = std::numeric_limits<std::uint32_t>::max();
  auto r = BlockForInit(Milliseconds{kMax});
  CHECK(r) << r.GetError().GetDisplay();
  CHECK(*r);
}

inline void Thread::BlockForExit() {
  static const std::uint32_t kMax = std::numeric_limits<std::uint32_t>::max();
  auto r = BlockForExit(Milliseconds{kMax});
  CHECK(r) << r.GetError().GetDisplay();
  CHECK(*r);
}

}  // namespace os
}  // namespace base

#endif  // _BASE_OS_THREAD_INL_H_
