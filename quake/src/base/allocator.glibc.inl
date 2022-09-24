#ifdef OS_LINUX
#include <malloc.h>
#endif  // OS_LINUX

namespace base {

bool HasMemoryProfiler() { return false; }

bool StartMemoryProfiler(const char* filename) {
  UNUSE(filename);
  return false;
}

bool StopMemoryProfiler() { return false; }

bool IsMemoryProfilerRunning() { return false; }

bool GetAllocatorMemoryUsage(AllocatorMemoryUsage* usage) {
#ifdef OS_LINUX
  auto minfo = mallinfo();
  auto app_used = minfo.uordblks;
  auto free_used = minfo.fordblks;
  usage->total_memory = app_used + free_used;
  usage->alloc_memory = app_used;
  return true;
#else
  return false;
#endif  // OS_LINUX
}

bool HasReleaseMemoryToOS() { return false; }

void ReleaseMemoryToOS(std::size_t suggestion) { UNUSE(suggestion); }

void ReleaseMemoryToOS() {}

std::string GetAllocatorMemoryInfo() {
#ifdef OS_LINUX
  auto minfo = mallinfo();
  return Fmt(
      "Arena: %d\n"
      "OrdBlks: %d\n"
      "SmBlks: %d\n"
      "HBlks: %d\n"
      "HBlkHd: %d\n"
      "UsmBlks: %d\n"
      "FsmBlks: %d\n"
      "UOrdBlks: %d\n"
      "FOrdBlks: %d\n"
      "KeepCost: %d\n",
      minfo.arena, minfo.ordblks, minfo.smblks, minfo.hblks, minfo.hblkhd,
      minfo.usmblks, minfo.fsmblks, minfo.uordblks, minfo.fordblks,
      minfo.keepcost);
#else
  return std::string{};
#endif  // OS_LINUX
}

}  // namespace base
