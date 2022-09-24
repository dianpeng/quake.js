#ifndef _BASE_ALLOCATOR_H_
#define _BASE_ALLOCATOR_H_
#include "common.h"
#include "result.h"

namespace base {

// Memory profiler related APIs =======================

// Check whether we have runtime memory profiler or not
bool HasMemoryProfiler();

// Start the global memory profiler operations
bool StartMemoryProfiler(const char*);

// Stop current running memory profiler.
bool StopMemoryProfiler();

// Check whether the current profiler is running or not
bool IsMemoryProfilerRunning();

// Memory detail related APIs =========================

// This function returns the current RSS memory usage for the current process.
// ie the memory (physical memory) used by the current process
base::Result<std::size_t> GetProcessCurrentRSS();

// This function returns the underlying situation of allocator, this api may
// not be stable since mostly it is used to detect (1) how many memory has been
// allocated in total from allocator (2) how many memory has been used by the
// application, ie malloced but not freed.

struct AllocatorMemoryUsage {
  std::size_t total_memory;
  std::size_t alloc_memory;

  AllocatorMemoryUsage() : total_memory(0), alloc_memory(0) {}
};

bool GetAllocatorMemoryUsage(AllocatorMemoryUsage*);

bool HasReleaseMemoryToOS();

// Function used to release those unused memory back to the OS. This function is
// best efforts style. If too fragmented, we may not be able to release the mem
// back to OS. The first version allow you to specify a value to be released,
// the second one will try to release as much as possible. It is best efforts.
void ReleaseMemoryToOS(std::size_t);
void ReleaseMemoryToOS();

// Function used to dump the memory status into a std::string. It is used to get
// detail allocator level information and the format is left to be
// implementation dependent not for us.
std::string GetAllocatorMemoryInfo();

}  // namespace base

#endif  // _BASE_ALLOCATOR_H_
