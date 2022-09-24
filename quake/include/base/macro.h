#ifndef _BASE_MACRO_H_
#define _BASE_MACRO_H_

#define DISALLOW_COPY_AND_ASSIGN(X)  \
  void operator=(const X&) = delete; \
  X(const X&) = delete;

#define DISALLOW_COPY_MOVE_AND_ASSIGN(X) \
  void operator=(const X&) = delete;     \
  X(const X&) = delete;                  \
  void operator=(X&&) = delete;          \
  X(X&&) = delete;

// Used to force an object to be placed on stack instead of on heap
#define DISALLOW_HEAP_ALLOCATION(X)                \
  void* operator new(std::size_t) = delete;        \
  void* operator new(std::size_t, void*) = delete; \
  void* operator new[](std::size_t) = delete;      \
  void* operator new[](std::size_t, void*) = delete

// Used to detect OS type. Currently we only support OS_LINUX/POSIX. Just in
// case we need to migrate to other OS/platform.
#if defined(__linux__)
#define OS_LINUX
#define OS_POSIX
#define OS_SYSV
#elif defined(__APPLE__)
#define OS_MAC
#define OS_POSIX
#define OS_SYSV
#else
#error "OS not supported yet"
#endif

// STRINGIFY, ie concatenation of multiple chunk of C lexical tokens as string
#define STR(...) #__VA_ARGS__

// The c++17's new raw-string based macro, we will finally migrate the STR to
// be RSTR based way, ie get rid of the old #__VA_ARGS__ way to concate string
#define RSTR(...) R""""""(__VA_ARGS__)""""""
#define CONCATE(A, B) A##B

// Compiler
#if defined(__GNUC__)
#define COMPILER_GCC
#elif defined(__clang__)
#define COMPILER_CLANG
#else
#define COMPILER_UNKNOWN
#endif

// Generalize warning push/pop.
#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)

#define BASE_PUSH_WARNING _Pragma("GCC diagnostic push")
#define BASE_POP_WARNING _Pragma("GCC diagnostic pop")

#define BASE_GNU_DISABLE_WARNING_INTERNAL2(WarningName) #WarningName
#define BASE_GNU_DISABLE_WARNING(WarningName) \
  _Pragma(                                    \
      BASE_GNU_DISABLE_WARNING_INTERNAL2(GCC diagnostic ignored WarningName))

#ifdef COMPILER_CLANG
#define BASE_CLANG_DISABLE_WARNING(WarningName) \
  BASE_GNU_DISABLE_WARNING(WarningName)
#define BASE_GCC_DISABLE_WARNING(WarningName)
#else
#define BASE_CLANG_DISABLE_WARNING(WarningName)
#define BASE_GCC_DISABLE_WARNING(WarningName) \
  BASE_GNU_DISABLE_WARNING(WarningName)
#endif  // COMPILER_CLANG

#else
#define BASE_PUSH_WARNING
#define BASE_POP_WARNING
#define BASE_GNU_DISABLE_WARNING(WarningName)
#define BASE_GCC_DISABLE_WARNING(WarningName)
#define BASE_CLANG_DISABLE_WARNING(WarningName)
#endif

#ifdef BASE_HAVE_SHADOW_LOCAL_WARNINGS
#define BASE_GCC_DISABLE_NEW_SHADOW_WARNINGS            \
  BASE_GNU_DISABLE_WARNING("-Wshadow-compatible-local") \
  BASE_GNU_DISABLE_WARNING("-Wshadow-local")            \
  BASE_GNU_DISABLE_WARNING("-Wshadow")
#else
#define BASE_GCC_DISABLE_NEW_SHADOW_WARNINGS /* empty */
#endif

template <typename T>
void UNUSE(T&&) noexcept {}

// Other common macros or helpers

// Usage, guard a piece of code by the mutex specified
//
//   LOCK_GUARD(mtx_) {
//     // your code ...
//   }

#define _GUARD_NAME(PREFIX) CONCATE(PREFIX, __LINE__)
#define LOCK_GUARD(M) \
  if (std::lock_guard<decltype(M)> _GUARD_NAME(__guard){(M)}; true)

// Used to mark the [[noreturn]] function to be terminated. Use this
// instead of UNREACHABLE() since this macro is standard compatible
#define NEVER_RETURN() std::abort()

// No inline attributes, used for USDT tracing. An inlined frame cannot be
// traced for now
#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
#define BASE_NOINLINE __attribute__((noinline))
#else
#define BASE_NOINLINE
#endif  // COMPILER_GCC || COMPILER_CLANG

// ----------------------------------------------------------------------------
// Arch detection, used for various backend since we are a JIT :)
#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
#if __x86_64__
#define QUAKE_TARGET_X64
#else
#error "unsupported ARCH"
#endif  // __x86_64__
#endif  // COMPILER_GCC || COMPILER_CLANG

#endif  // _BASE_MACRO_H_
