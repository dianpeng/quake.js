#ifndef _BASE_CRASH_H_
#define _BASE_CRASH_H_
#include "common.h"

namespace base {

// Invoke this function to register crash helper globally. Mainly used to detect
// things like unexpected signal or runtime error, ie terminate in c++
void InstallCrashHandler();

}  // namespace base

#endif  // _BASE_CRASH_H_
