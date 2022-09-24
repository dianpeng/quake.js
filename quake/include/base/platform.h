#ifndef _BASE_PLATFORM_H_
#define _BASE_PLATFORM_H_

#if defined(__linux__)
#define OS_LINUX
#define OS_POSIX
#elif defined(__APPLE__)
#define OS_MAC
#define OS_POSIX
#else
#error "OS not supported yet"
#endif

#if defined(__x86_64__)

#define ARCH_64

static constexpr unsigned int kPlatformAlignmentBit = 8;
static constexpr unsigned int kPlatformAlignmentSize = 8;

#else

#error "Arch not supported yet"

#endif  // __x84_64__

#endif  // _BASE_PLATFORM_H_
