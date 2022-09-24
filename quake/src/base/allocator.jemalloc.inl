#if defined(SPARROW_HAS_JEMALLOC)
#include <jemalloc/jemalloc.h>

namespace base {

#define MALLCTL_READ(cfg, obj, sz) ::mallctl(cfg, obj, sz, NULL, 0)
#define MALLCTL_WRITE(cfg, obj, sz) ::mallctl(cfg, NULL, NULL, obj, sz)

// JEMalloc doesn't support on-the-fly heap profiling but needs to use the
// environment variable to tweak. So we just disable the profiler here.
bool HasMemoryProfiler() { return true; }
bool StartMemoryProfiler(const char* filename) {
  if (IsMemoryProfilerRunning()) {
    StopMemoryProfiler();
  }

  std::string fn{};
  if (!filename) {
    auto pid = os::GetPid();
    fn = Fmt("/tmp/profiler.heap.%d", static_cast<int>(pid));
  } else {
    fn = filename;
  }

  const char* raw_fn = fn.c_str();
  std::size_t sz = sizeof(raw_fn);
  if (auto e = MALLCTL_WRITE("prof.dump", &raw_fn, sz); e) {
    return false;
  }

  bool prof_active = true;
  sz = sizeof(prof_active);
  if (auto e = MALLCTL_WRITE("prof.active", &prof_active, sz); e) {
    return false;
  }

  return true;
}

bool StopMemoryProfiler() {
  if (!IsMemoryProfilerRunning()) {
    return false;
  }

  bool prof_active = false;
  std::size_t sz = sizeof(prof_active);
  if (auto e = MALLCTL_WRITE("prof.active", &prof_active, sz); e) {
    return false;
  }

  return true;
}

bool IsMemoryProfilerRunning() {
  bool prof_active = false;
  std::size_t sz = sizeof(prof_active);
  if (auto e = MALLCTL_READ("prof.active", &prof_active, &sz); e) {
    return false;
  }

  return prof_active;
}

// Release memory to OS option
bool HasReleaseMemoryToOS() { return false; }
void ReleaseMemoryToOS(std::size_t) {}
void ReleaseMemoryToOS() {}

bool GetAllocatorMemoryUsage(AllocatorMemoryUsage* usg) {
  std::size_t native_held_memory;
  std::size_t allocator_phy_memory;
  std::size_t sz = sizeof(native_held_memory);
  // stats.allocated: total number of bytes allocated by the application
  if (auto e = MALLCTL_READ("stats.allocated", &native_held_memory, &sz); e) {
    return false;
  }

  //resident physical data pages, including metadata, dirty pages, active pages
  if (auto e = MALLCTL_READ("stats.resident", &allocator_phy_memory, &sz); e) {
    return false;
  }

  usg->total_memory = allocator_phy_memory;
  usg->alloc_memory = native_held_memory;

  if(usg->alloc_memory > usg->total_memory) {
    return false;
  }

  return true;
}

// Allocator memory info
std::string GetAllocatorMemoryInfo() {
  auto cb = [](void* ctx, const char* data) {
    auto ptr = static_cast<std::string*>(ctx);
    ptr->append(data);
  };

  std::string o;
  // a: per arena statistics
  // x: mutex statistics
  ::malloc_stats_print(cb, &o, "bdeglm");
  return o;
}

}  // namespace base

#endif  // SPARROW_HAS_JEMALLOC
