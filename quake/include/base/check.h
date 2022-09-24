#ifndef _BASE_CHECK_H_
#define _BASE_CHECK_H_
#include "log.h"

namespace base {
namespace log {
namespace _ {

struct CheckWarningVoidify {
  template <typename T>
  void operator&(T&) {}
};

}  // namespace _
}  // namespace log
}  // namespace base

#if defined(CHECK)
#undef CHECK
#endif  // CHECK

#if defined(DCHECK)
#undef DCHECK
#endif  // DCHECK

#define CHECK(COND)                                                       \
  static_cast<void>(0), !!(COND) ? (void)0                                \
                                 : ::base::log::_::CheckWarningVoidify() & \
                                       TLOG("CHECK: " #COND, FATAL)

#define DCHECK(COND)                                                      \
  static_cast<void>(0), !!(COND) ? (void)0                                \
                                 : ::base::log::_::CheckWarningVoidify() & \
                                       DTLOG("DCHECK: " #COND, FATAL)

#define CHECK_NOT(V) CHECK(!(V))
#define DCHECK_NOT(V) DCHECK(!(V))

#define CHECK_EQ(LHS, RHS) CHECK((LHS) == (RHS))
#define CHECK_NE(LHS, RHS) CHECK((LHS) != (RHS))
#define CHECK_GT(LHS, RHS) CHECK((LHS) > (RHS))
#define CHECK_GE(LHS, RHS) CHECK((LHS) >= (RHS))
#define CHECK_LT(LHS, RHS) CHECK((LHS) < (RHS))
#define CHECK_LE(LHS, RHS) CHECK((LHS) <= (RHS))

#define DCHECK_EQ(LHS, RHS) DCHECK((LHS) == (RHS))
#define DCHECK_NE(LHS, RHS) DCHECK((LHS) != (RHS))
#define DCHECK_GT(LHS, RHS) DCHECK((LHS) > (RHS))
#define DCHECK_GE(LHS, RHS) DCHECK((LHS) >= (RHS))
#define DCHECK_LT(LHS, RHS) DCHECK((LHS) < (RHS))
#define DCHECK_LE(LHS, RHS) DCHECK((LHS) <= (RHS))

#define UNREACHABLE()                                       \
  ::base::log::LogMessage(__FILE__, __LINE__, "UNREACHABLE", \
                         ::base::log::kFatal)                \
      .stream()

#define OOPS()                                                           \
  ::base::log::LogMessage(__FILE__, __LINE__, "Oops", ::base::log::kFatal) \
      .stream()

// Helper macro for deferencing pointer that maybe null. Notes, if the pointer
// is null, it gonna crash with a hint if supplied. Notes, the whole statement
// is actually an expression, so it can be used in whatever places of expression

#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)

// Use statement expression feature provided by GCC/Clang. This is a very useful
// way to orgnize multiple statement but as an normal expression
#define NOT_NULLX(PTR, HINT)                                \
  ({                                                        \
    CHECK((PTR)) << "not-null(" << #PTR << "): " << (HINT); \
    (PTR);                                                  \
  })

#define NOT_NULL(PTR)                           \
  ({                                            \
    CHECK((PTR)) << "not-null(" << #PTR << ")"; \
    (PTR);                                      \
  })

#else

namespace base {
namespace log {
namespace _ {

inline void CondCheck(bool cond, const char* file, int line, const char* expr,
                      const char* hint) {
  if (!cond) {
    LogMessage msg{file, line, "CHECK", kFatal};
    if (hint) {
      msg.stream() << "not-null(" << expr << "): " << hint;
    } else {
      msg.stream() << "not-null(" << expr << ")";
    }
  }
}

template <typename T>
const T* PtrNotNull(const T* ptr, const char* file, int line, const char* expr,
                    const char* hint) {
  CondCheck(ptr != nullptr, file, line, expr, hint);
  return ptr;
}

template <typename T>
T* PtrNotNull(T* ptr, const char* file, int line, const char* expr,
              const char* hint) {
  CondCheck(ptr != nullptr, file, line, expr, hint);
  return ptr;
}

}  // namespace _
}  // namespace log
}  // namespace base

#define NOT_NULLX(PTR, HINT) \
  ::base::log::_::PtrNotNull((PTR), __FILE__, __LINE__, #PTR, (HINT))

#define NOT_NULL(PTR) \
  ::base::log::_::PtrNotNull((PTR), __FILE__, __LINE__, #PTR, nullptr)

#endif  // COMPILER_GCC || COMPILER_CLANG

#endif  // _BASE_CHECK_H_
