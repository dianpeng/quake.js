#include <gperftools/malloc_extension.h>
#include <gperftools/heap-profiler.h>

namespace base {

bool HasMemoryProfiler() { return true; }

bool StartMemoryProfiler(const char* filename) {
  if (IsMemoryProfilerRunning()) {
    return false;
  }

  // Only start the profiler when it is running
  std::string fn{};
  if (!filename) {
    auto pid = os::GetPid();
    fn = Fmt("/tmp/profiler.heap.%d", static_cast<int>(pid));
  } else {
    fn = filename;
  }
  ::HeapProfilerStart(fn.c_str());
  return true;
}

bool IsMemoryProfilerRunning() { return ::IsHeapProfilerRunning(); }

bool StopMemoryProfiler() {
  if (IsMemoryProfilerRunning()) {
    ::HeapProfilerStop();
    return true;
  }
  return false;
}

bool GetAllocatorMemoryUsage(AllocatorMemoryUsage* usg) {
  std::size_t native_held_memory;
  std::size_t allocator_total_memory;

  if (!MallocExtension::instance()->GetNumericProperty(
          "generic.current_allocated_bytes", &native_held_memory)) {
    return false;
  }

  if (!MallocExtension::instance()->GetNumericProperty(
          "generic.heap_size", &allocator_total_memory)) {
    return false;
  }
  std::size_t unmapped_bytes{};

  if (!MallocExtension::instance()->GetNumericProperty(
          "tcmalloc.pageheap_unmapped_bytes", &unmapped_bytes)) {
    return false;
  }
  if (unmapped_bytes > allocator_total_memory) {
    return false;
  }

  usg->total_memory = allocator_total_memory - unmapped_bytes;
  usg->alloc_memory = native_held_memory;
  if (usg->alloc_memory > usg->total_memory) {
    return false;
  }
  return true;
}

bool HasReleaseMemoryToOS() { return true; }

void ReleaseMemoryToOS(std::size_t hint) {
  MallocExtension::instance()->ReleaseToSystem(hint);
}

void ReleaseMemoryToOS() { MallocExtension::instance()->ReleaseFreeMemory(); }

std::string GetAllocatorMemoryInfo() {
  char buffer[10240];
  MallocExtension::instance()->GetStats(buffer, 10240);
  return std::string{buffer};
}

}  // namespace base
