#ifdef OS_LINUX
#include <malloc.h>
#include <features.h>

// Notes, glibc >= 2.33 deprecates mallinfo() (it uses 32-bit counters) which
// turns into a hard error under -Werror. mallinfo2() is the drop-in
// replacement with 64-bit fields; fall back to mallinfo() on older glibc.
#if defined(__GLIBC__) && defined(__GLIBC_MINOR__) && \
    (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 33))
#define QUAKE_MALLINFO() mallinfo2()
#else
#define QUAKE_MALLINFO() mallinfo()
#endif
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
  auto minfo = QUAKE_MALLINFO();
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
  auto minfo = QUAKE_MALLINFO();
  return Fmt(
      "Arena: %lu\n"
      "OrdBlks: %lu\n"
      "SmBlks: %lu\n"
      "HBlks: %lu\n"
      "HBlkHd: %lu\n"
      "UsmBlks: %lu\n"
      "FsmBlks: %lu\n"
      "UOrdBlks: %lu\n"
      "FOrdBlks: %lu\n"
      "KeepCost: %lu\n",
      static_cast<unsigned long>(minfo.arena),
      static_cast<unsigned long>(minfo.ordblks),
      static_cast<unsigned long>(minfo.smblks),
      static_cast<unsigned long>(minfo.hblks),
      static_cast<unsigned long>(minfo.hblkhd),
      static_cast<unsigned long>(minfo.usmblks),
      static_cast<unsigned long>(minfo.fsmblks),
      static_cast<unsigned long>(minfo.uordblks),
      static_cast<unsigned long>(minfo.fordblks),
      static_cast<unsigned long>(minfo.keepcost));
#else
  return std::string{};
#endif  // OS_LINUX
}

}  // namespace base
