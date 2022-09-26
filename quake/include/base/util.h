#ifndef _BASE_UTIL_H_
#define _BASE_UTIL_H_
#include "check.h"
#include "common.h"
#include "base/_/utf8-internal.h"

// This file contains many useful small helper functions/objects for utility
// purpose. Many of them has been my previous experience. In general, anything
// that is hard to be categorized can be put into this file

namespace base {
namespace _ {

class Registry {
 public:
  template <typename Callback>
  Registry(Callback&& c) {
    c();
  }
};

struct FreeDeleter {
  template <typename T>
  void operator()(T* p) const {
    ::free(const_cast<std::remove_const_t<T>*>(p));
  }
};

struct NullDeleter {
  template <typename T>
  void operator()(T*) const {}
};

struct CtorDefault {
  template <typename T>
  void operator()(T* buffer) {
    NO_EXCEPT(CallCtor(buffer));
  }
};

// This one should be treated as opaque type only if you want std::function
// to be shared when it is not constructed from copbasele object. It works
// as if the std::function is shared/copbasele regardless where it is
// constructed
template <typename F>
struct SharedFunction {
  explicit SharedFunction(F&& f)
      : f_(std::make_shared<F>(std::forward<F>(f))) {}

  SharedFunction() = default;
  SharedFunction(SharedFunction&&) = default;
  SharedFunction(const SharedFunction&) = default;
  SharedFunction& operator=(SharedFunction&&) = default;
  SharedFunction& operator=(const SharedFunction&) = default;

  // mimic the operator () for std::function types.
  template <typename... T>
  auto operator()(T&&... args) const {
    return (*f_)(std::forward<T>(args)...);
  }

 private:
  std::shared_ptr<F> f_;
};

}  // namespace _

// Common exponential back-off stratey for growing memory/buffer or else
// resource.
template <typename T>
T ExpBackoffGrow(T current, T at_least, T init, T max) {
  if (!current) {
    current = init;
  } else {
    current *= 2;
  }
  if (current > max) {
    current = max;
  }
  if (current < at_least) {
    current = at_least;
  }
  return current;
}

// A buffer object that can be used to hold memory on stack |OR| on heap. Notes
// this object is kind of simple to use and in general it is considered to be
// immutable once it is been constructed.
template <std::size_t kStackSize = kKB>
class SSOBuffer {
 public:
  explicit SSOBuffer(std::size_t size) { InitBuffer(size); }
  char* buffer() {
    return heap_storage_ ? heap_storage_.get() : inline_storage_;
  }
  bool IsStackBuffer() const { return !static_cast<bool>(heap_storage_); }

 private:
  void InitBuffer(std::size_t);
  char inline_storage_[kStackSize];
  std::unique_ptr<char[]> heap_storage_;

  DISALLOW_COPY_AND_ASSIGN(SSOBuffer);
};

class DetectDestruct {
 public:
  // Here we declare a simple RAII to avoid stack overflow for the flag set on
  // the stack after the ContinueIfNotDeleted call.
  class ScopeFlag {
   public:
    inline explicit ScopeFlag(DetectDestruct*);
    inline ~ScopeFlag();
    bool flag() const { return flag_; }

   private:
    DetectDestruct* d_;
    bool flag_;
  };

  inline ~DetectDestruct();

 protected:
  DetectDestruct() = default;

 private:
  bool* flag_{nullptr};
  friend class ScopeFlag;
};

// Call F1 and F2 handler based on certain condition. This function will always
// call F1 handler, and check the DetectDestruct flag to see whether the object
// is destructed or not. If not, F2 handler is invoked. If f2 is invoked, it
// returns true otherwise false. It is useful when user want to handle callback
// in sync and callback can potentially delete |this/callee|
template <typename F1, typename F2>
bool ContinueIfNotDestruct(DetectDestruct*, F1&&, F2&&);

template <typename T, typename Ctor = _::CtorDefault>
class LazyInstance {
 public:
  ~LazyInstance() { MaybeDeinit(); }

  // Manually intialization of the instance
  template <typename... ARGS>
  void Init(ARGS&&... args);

  // Manually destruct the object. If the object is not intialized, this
  // function gonna crash with CHECK
  void Deinit();

  // Try to deinit the object if applicable, if does so then returns true
  // otherwise false
  bool MaybeDeinit();

 public:
  // Whether this object has been initialized or not
  bool init() const { return init_; }

  // Accessor, all the following accessor will implicitly construct the object.
  // The constructor will be invoked is by using concept Ctor to do. Ie, via
  //
  //   Ctor()(buffer);
  //
  // Inside of the Ctor, user is supposed to use CallCtor/placement new to
  // construct the buffer into whatever type user wants to have. The default
  // Ctor will just construct the buffer with default constructor
  T* ptr() { return LazyAccess(); }
  const T* ptr() const { return LazyAccess(); }

  T* operator->() { return LazyAccess(); }
  const T* operator->() const { return LazyAccess(); }

  T& operator*() { return *ptr(); }
  const T& operator*() const { return *ptr(); }

  operator bool() const { return init_; }

 private:
  inline T* LazyAccess();

  const T* LazyAccess() const { return const_cast<T*>(this)->LazyAccess(); }

  T* raw_ptr() { return reinterpret_cast<T*>(storage_); }
  const T* raw_ptr() const { return reinterpret_cast<const T*>(storage_); }

  alignas(T) std::uint8_t storage_[sizeof(T)];
  bool init_{false};

  DISALLOW_COPY_AND_ASSIGN(LazyInstance);
};

// CountdownLatch, a simple sychronization primitive used for countdown. It is
// just a simple wrapper around the condition variable. Notes this is not
// performant and shuold be used in rare situations
class CountdownLatch {
 public:
  explicit CountdownLatch(std::uint32_t cd) : lock_(), cv_(), countdown_(cd) {}
  inline void Countdown();
  // Join the countdown latch
  inline void Join();
  inline bool Join(Milliseconds);

 private:
  std::mutex lock_;
  std::condition_variable cv_;
  std::uint32_t countdown_;
};

template <typename T, std::size_t kSize>
inline constexpr T ArraySum(const T (&arr)[kSize]) {
  T v = 0;
  for (std::size_t i = 0; i < kSize; ++i) {
    v += arr[i];
  }
  return v;
}

namespace _ {

static constexpr std::uint16_t kLeadSurrogateMin = 0xd800u;
static constexpr std::uint16_t kLeadSurrogateMax = 0xdbffu;
static constexpr std::uint16_t kTrailSurrogateMin = 0xdc00u;
static constexpr std::uint16_t kTrailSurrogateMax = 0xdfffu;

static constexpr std::uint16_t kLeadOffset =
    kLeadSurrogateMin - (0x10000 >> 10);

static constexpr std::uint32_t kSurrogateOffset =
    0x10000u - (kLeadSurrogateMin << 10) - kTrailSurrogateMin;

static constexpr std::uint32_t kCodePointMax = 0x0010ffffu;

template <typename OType>
inline std::uint8_t Mask8(OType oc) {
  return static_cast<std::uint8_t>(0xff & oc);
}
template <typename U16Type>
inline std::uint16_t Mask16(U16Type oc) {
  return static_cast<std::uint16_t>(0xffff & oc);
}
template <typename OType>
inline bool IsTrail(OType oc) {
  return Mask8(oc) == 0x2;
}

template <typename T>
inline bool IsLeadSurrogate(T cp) {
  return (cp >= kLeadSurrogateMin && cp <= kLeadSurrogateMax);
}

template <typename T>
inline bool IsTrailSurrogate(T cp) {
  return (cp >= kTrailSurrogateMin && cp <= kTrailSurrogateMax);
}

template <typename T>
inline bool IsSurrogate(T cp) {
  return (cp >= kLeadSurrogateMin && cp <= kTrailSurrogateMax);
}

template <typename T>
inline bool IsCodePointValid(T cp) {
  static constexpr std::uint32_t kCodePointMax = 0x0010ffffu;
  return (cp <= kCodePointMax && !IsSurrogate(cp));
}

}  // namespace _

// Append the codepoint into std::string with utf8 encoding scheme
inline bool AppendUtf8(std::uint32_t, std::string*);
inline std::optional<std::size_t> Utf8Encode(std::uint32_t, std::uint8_t*);
inline bool Utf8Encode(const std::vector<std::uint32_t>&,
                       std::vector<std::uint8_t>*);
inline std::optional<std::size_t> DecodeOneUtf8(const void*, std::size_t,
                                                std::uint32_t*);

}  // namespace base

#include "util-inl.h"

#endif  // _BASE_UTIL_H_
