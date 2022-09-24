#ifndef _BASE_COMMON_H_
#define _BASE_COMMON_H_

#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <cerrno>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <filesystem>
#include <forward_list>
#include <functional>
#include <future>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <span.hpp>

// Order matters, and should be placed at very first of the file
#include "debug.h"
#include "env.h"
#include "macro.h"
#include "no-destructor.h"
#include "numeric.h"
#include "platform.h"
#include "trait.h"

#if defined(OS_LINUX)
#include "common-linux.h"
#else
#error "OS not supported"
#endif  // OS_LINUX

// This file defines common routine to be used throughout the project. The stuff
// defined here will not have any dependency except third-party or std library.
// We try to put as much practical function into here as possible to avoid
// fragmented source code and project orgnization.
namespace base {

static constexpr std::size_t kKB = 1024;
static constexpr std::size_t kMB = 1024 * kKB;
static constexpr std::size_t kGB = 1024 * kMB;
static constexpr std::size_t kTB = 1024 * kGB;

static constexpr std::size_t kMaxSizeT =
    std::numeric_limits<std::size_t>::max();

using byte_t = std::uint8_t;

using StringList = std::vector<std::string>;
using BoolList = std::vector<bool>;
using FloatList = std::vector<float>;
using DoubleList = std::vector<double>;

using Int8List = std::vector<std::int8_t>;
using Int16List = std::vector<std::int16_t>;
using Int32List = std::vector<std::int32_t>;
using Int64List = std::vector<std::int64_t>;

using UInt8List = std::vector<std::uint8_t>;
using UInt16List = std::vector<std::uint16_t>;
using UInt32List = std::vector<std::uint32_t>;
using UInt64List = std::vector<std::uint64_t>;

/** ---------------------------------------------------------------------
 ** std::chrono wrapper
 **   Notes, std::chrono is a horrible library, hard to use and error prone.
 **   We don't want to build our yet another timer library but have to stick
 **   to it for now to simplify our dependency. The wrapper do some typedef
 **   to save us typing and also the most important thing is to avoid error
 **   when adding large duration to timepoint which causes wrapping of numbers.
 **   It typically happens for the TTL, we store the ts as last access and may
 **   store a super large TTL to represent unlimited TTL, but the addition of
 **   large TTL into time_point result in time_point wrapping :(
 **
 **   Seconds::max() + Now() -> result in time_point to be wrapped, sigh ...
 **
 **   We dedicate a function called AddTTL here to do clamping internally, so
 **   for addition of duration that does overflow, we return time_point::max
 **/

using Nanoseconds = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;
using Minutes = std::chrono::minutes;
using Hours = std::chrono::hours;

// Notes(dpeng): C++20 adds the std::chrono::days/weeks/months/years but
//   BASE assume C++17 as minimum C++ standard
using Days = std::chrono::duration<std::int64_t, std::ratio<86400>>;
using Weeks = std::chrono::duration<std::int64_t, std::ratio<604800>>;
using Months = std::chrono::duration<std::int64_t, std::ratio<2620746>>;
using Years = std::chrono::duration<std::int64_t, std::ratio<31556952>>;

// Timestamp represents a system clock timestamp, ie it may drift, though mostly
// will not, and it can be used to convert to epoch time, ie representation of
// wall clock timestamp.
using TsClock = std::chrono::system_clock;
using Ts = std::chrono::time_point<TsClock>;

using MonoClock = std::chrono::steady_clock;
using MonoTs = std::chrono::time_point<MonoClock>;

// Retrieve the system clock's timestamp, ie UTC.
inline Ts Now();
inline std::uint64_t TsToU64(base::Ts ts) {
  return static_cast<std::uint64_t>(
      std::chrono::duration_cast<Milliseconds>(ts.time_since_epoch()).count());
}
inline Ts FromUnixTs(Milliseconds);

inline std::uint64_t NowInMicroseconds();
inline std::uint64_t NowInMilliseconds();
inline std::uint64_t NowInSeconds();

inline MonoTs MonoNow();

// Add a ttl to the specified timestamp, if the ts overflow, then just returns
// the Ts::max to avoid wrapping issue
template <typename Duration>
inline Ts AddTTL(Ts, Duration);

// Time drifting avoidance time diff, if detect time drift, ie the now < before
// it will output {0} instead of been rounding up. It is also safely specialized
// to be used with different
template <typename O>
O TimeDiff(Ts now, Ts before) {
  return (now > before ? std::chrono::duration_cast<O>(now - before) : O{0});
}

// Used when you want to have a dummy struct/type/object/class
struct Dummy {};

// Some policy tag class used for most of the codebase
struct ThreadSafePolicy {};
struct ThreadUnsafePolicy {};

template <typename T, std::size_t N>
constexpr std::size_t ArraySize(const T (&)[N]) noexcept {
  return N;
}

template <typename T, typename... ARGS>
T* CallCtor(void* buffer, ARGS&&... args) {
  return new (buffer) T(std::forward<ARGS>(args)...);
}

// Manually invoke the destructor of the ptr belongs to type |T|
template <typename T>
void CallDtor(T* ptr) {
  if (ptr) ptr->~T();
}

// Pointer advancing, used to move pointer forward.
// Notes the PtrForward cannot accept pointer as |T| even you want to output
// a pointer of pointer. If that's the case do the cast by yourself. The reason
// is to avoid mistake that user want to unintended mistake.
template <typename T, typename U>
inline T* PtrForward(U* ptr, std::size_t offset) noexcept {
  static_assert(!std::is_pointer<T>::value);
  return reinterpret_cast<T*>(reinterpret_cast<char*>(ptr) + offset);
}

template <typename T, typename U>
inline const T* PtrForward(const U* ptr, std::size_t offset) noexcept {
  static_assert(!std::is_pointer<T>::value);
  return reinterpret_cast<const T*>(reinterpret_cast<const char*>(ptr) +
                                    offset);
}

template <typename T, typename U>
inline std::size_t PtrDifference(T* a, U* b) {
  return static_cast<const char*>(a) - static_cast<const char*>(b);
}

template <typename T>
inline T Align(T value, T mask) noexcept {
  static_assert(std::is_integral<T>::value);
  return (value + (mask - 1)) & (~(mask - 1));
}

// Format function. Like printf but returns a std::string. Notes, this is C
// style formatting API. User should prefer modern format counter part by
// using Format
std::string Fmt(const char*, ...);
std::string FmtV(const char*, va_list);

// Misc string utilities
// errno turns into std::string. This function is thread safe
std::string ErrString(int);

// turn current errno to std::string
std::string ErrString();

// Escape the std::string into JSON friendly string for storage purpose. Ie
// escape any sequences that is either invisible or are not supposed to show
// up inside of JSON string. This function assume the input is a valid ASCII
// or ISO-Latin-1 string. UTF-X encoding is undefined behavior
std::string JSONEscape(const std::string_view&);
void JSONEscape(const std::string_view&, std::string*);

// Function to abort the process immediately. User is allowed to feed the last
// words and it will be dumpped out accordinly.
[[noreturn]] void DieNow() noexcept;
[[noreturn]] void Die() noexcept;
[[noreturn]] void Die(const char*, ...);
[[noreturn]] void DieF(const char*, va_list);

// Generate a stacktrace of the current thread's frame. Notes, this function
// may be expensive. Better just use it when you gonna die
std::string GetStackTrace();
void AppendStackTrace(std::string*);

namespace _ {

template <typename R, typename T>
R ExceptionRun(T&& h, const char* expr, const char* file, int line) {
  try {
    return h();
  } catch (std::exception& e) {
    Die("@(%s,%d): %s thrown exception %s", file, line, expr, e.what());
  } catch (...) {
    Die("@(%s,%d): %s thrown unknown exception", file, line, expr);
  }
}

}  // namespace _

// Used to guard against exception
#define NO_EXCEPT(...)                                                        \
  ::base::_::ExceptionRun<decltype(__VA_ARGS__)>(                             \
      [&]() -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }, #__VA_ARGS__, \
      __FILE__, __LINE__)

// std::get wrapper. The core gain here is that we |Die| when we see any
// throwned exception from std::get
template <typename I, typename... ARGS>
I& Get(std::variant<ARGS...>& v) {
  return NO_EXCEPT(std::get<I>(v));
}

template <typename I, typename... ARGS>
const I& Get(const std::variant<ARGS...>& v) {
  return NO_EXCEPT(std::get<I>(v));
}

template <typename I, typename... ARGS>
I Get(std::variant<ARGS...>&& v) {
  return NO_EXCEPT(std::get<I>(std::move(v)));
}

template <typename K, typename U>
K Get(std::optional<K>&& v, U&& def) {
  return v ? K{std::move(v.value())} : K{std::move(def)};
}

template <typename K, typename U>
K Get(const std::optional<K>& v, const U& def) {
  return v ? v.value() : static_cast<const K&>(def);
}

template <typename K, typename U>
K Get(std::optional<K>& v, const U& def) {
  return v ? v.value() : static_cast<const K&>(def);
}

template <typename K>
K* Get(std::optional<K>* v) {
  return v->has_value() ? &(v->value()) : nullptr;
}

template <typename K>
const K* Get(const std::optional<K>* v) {
  return v->has_value() ? &(v->value()) : nullptr;
}

// This interface is specifically for std::optional, but instead of throwing an
// exception but act like rust, crash/panic
template <
    typename T, typename _T0 = std::remove_cv_t<std::remove_reference_t<T>>,
    typename _IT = typename _T0::value_type, typename _OT = std::optional<_IT>,
    std::enable_if_t<std::is_same<_T0, _OT>::value, int> _ = 0>
_IT Unwrap(T&& v) {
  if (!v) {
    Die("Unwrap failed");
  }
  return v.value();
}

// InRange. Just a self document function to do following things :
//
//   a >= start && a < end
//
// This is easier to read the code and just for documenting purpose of the code

template <typename T, typename U1, typename U2>
bool InRange(const T& v, const U1& lower, const U2& upper,
             bool inclusive = false) {
  return v >= static_cast<T>(lower) &&
         ((inclusive && v <= static_cast<T>(upper)) ||
          (!inclusive && v < static_cast<T>(upper)));
}

template <typename T>
std::optional<T> LexicalCast(const std::string_view&);

// One shot file reading api, read the whole file into the std::string and
// truncated at maximum size specified as max. If nothing can be loaded,
// returns an empty optional
std::optional<std::string> LoadWholeFile(const std::string_view&,
                                         std::size_t max = kMB * 32);

enum class Tribool : std::int8_t { kFalse = 0, kTrue = 1, kException = -1 };

// Here we define a relative standard compatible move operation. The standard
// says that an moved object is in an |unspecified| but |valid| state. Simply
// puts it the object is not useful but its state is valid. And standard gives
// no description what's so ever. But typically after we move a certain object
// away, we still want to use the moved object. The safe move concept comes in.
// The safe move is just a wrapper around Swap/swap. The swap operation is well
// defined and object been swapped is still usable.
//
// Notes, we support 2 types of swap, the std::swap, which takes an reference
// and our own swap, typically written in |Swap| and it takes a pointer in. This
// because coding standard force us to take pointer when we want to mutate it.
namespace _ {

DEFINE_CONCEPT(HasSTDSwap, void (_::*)(_&), &_::swap);
DEFINE_CONCEPT(HasUserSwap, void (_::*)(_*), &_::Swap);

template <typename T>
struct NoSwapConcept {
  static constexpr bool kValue =
      !HasUserSwap<T>::kValue && !HasSTDSwap<T>::kValue;
};

}  // namespace _

// Weak pointer helpers
template <typename T>
std::weak_ptr<T> MkWeak(T* ptr) {
  return std::static_pointer_cast<T>(NO_EXCEPT(ptr->weak_from_this()).lock());
}

template <typename T>
std::weak_ptr<const T> MkWeak(const T* ptr) {
  return std::static_pointer_cast<const T>(
      NO_EXCEPT(ptr->weak_from_this()).lock());
}

template <typename T>
std::weak_ptr<T> MkWeak(const std::shared_ptr<T>& p) {
  return std::weak_ptr<T>{p};
}

template <typename T>
std::decay_t<T> Dup(T&& v) {
  return T{std::forward<T>(v)};
}

// Defer macro. Used to defer a function to be executed later on. It comes with
// few variants to suit different needs.
namespace _ {

template <typename T>
struct DeferRunner {
 public:
  explicit DeferRunner(T&& h) : handler_(std::move(h)) {}
  ~DeferRunner() { handler_(); }

 private:
  T handler_;
};

template <typename T>
DeferRunner<T> MkDeferRunner(T&& h) {
  return DeferRunner{std::move(h)};
}

template <typename T>
class TransactionRunner {
 public:
  explicit TransactionRunner(T&& h) : handler_(std::move(h)), rollback_(true) {}
  void Commit() { rollback_ = false; }
  ~TransactionRunner() {
    if (rollback_) handler_();
  }

 private:
  T handler_;
  bool rollback_;
};

}  // namespace _

template <typename T>
T& Deref(T* v) {
  return *v;
}

template <typename T>
const T& Deref(const T* v) {
  return *v;
}

// For anytypes of reference, just do nothing but return it back
template <typename T>
T& Deref(T& v) {
  return v;
}

template <typename T>
const T& Deref(const T& v) {
  return v;
}

// Defer instance's name
#if defined(__COUNTER__)
#define __BASE_DEFER_INSTANCE_NAME(P) CONCATE(__defer, P)
#define BASE_DEFER_INSTANCE_NAME __BASE_DEFER_INSTANCE_NAME(__COUNTER__)
#else
#define __BASE_DEFER_INSTANCE_NAME(P) CONCATE(__defer, P)
#define BASE_DEFER_INSTANCE_NAME __BASE_DEFER_INSTANCE_NAME(__LINE__)
#endif  // __COUNTER__

// DEFER macro. Usage as following:
//
//   DEFER([]() { ... stuff be called after lexical scope exits });
#define DEFER(...) \
  auto BASE_DEFER_INSTANCE_NAME = ::base::_::MkDeferRunner(__VA_ARGS__)

// A simple scope based transaction object. Usage as following :
//
//   auto tr = MkTransaction([]() { // rollback });
//   tr.Commit(); // rollback will not be invoked if goes out of the scope
//
// If |Commit| is not invoked, the default behavior of transaction object is
// to execute the rollback function specified at destructor, ie goes out of the
// lexical scope
template <typename T>
_::TransactionRunner<T> MkTransaction(T&& h) {
  return _::TransactionRunner<T>{std::move(h)};
}

// Badge pattern for c++, used in public interface for achieving privacy,
//   see https://awesomekling.github.io/Serenity-C++-patterns-The-Badge/
//   for details. Used to shows up in hackernews

template <typename T>
class Badge {
 private:
  friend T;
  Badge() {}
};

// The |Pass| function can be used to safely move the object away and let the
// old object still be usable, from the perspective of standard.
template <typename T, std::enable_if_t<_::HasSTDSwap<T>::kValue, int> _ = 0>
T Pass(T* t) noexcept {
  T temp{};
  temp.swap(*t);
  return temp;
}

template <typename T, std::enable_if_t<_::HasUserSwap<T>::kValue, int> _ = 0>
T Pass(T* t) noexcept {
  T temp{};
  temp.Swap(t);
  return temp;
}

// This is the default version, which tries to use std::swap to swap the object
// if this fails, then an compiler error will be generated, which means user
// cannot use Pass function for type T, ie it doesn't have swap concept at all
template <typename T, std::enable_if_t<_::NoSwapConcept<T>::kValue, int> _ = 0>
T Pass(T* v) noexcept {
  T temp{};
  std::swap(temp, *v);
  return temp;
}

// NOTES, Pls do not use std::move(*v) or std::move(v.value()) to extract value
// from optional for move semantic. Especially in async code, this is extreamly
// unsafe, since the move operation doesn't create a temporary object to hold
// the memory, if the original optional value is a member field of an object
// and during the async operation, before the move is really settle down to a
// storage slot, the value can be nullified and result in heap use after free
// error. Always use Pass which does create a temporary storage that detach the
// relationship with original moving object. Yeah, it is more expensive but it
// guarantees correctness.
template <typename T>
T Pass(std::optional<T>* v) {
  if (!v) Die("optional is empty");
  DEFER([&]() { *v = std::nullopt; });
  return T{std::move(*(*v))};
}

template <typename T>
T Pass(std::optional<T>& v) {
  if (!v) Die("optional is empty");
  DEFER([&]() { v = std::nullopt; });
  return T{std::move(*v)};
}

namespace _ {

DEFINE_CONCEPT(HasToString, std::string (_::*)() const, &_::ToString);
DEFINE_CONCEPT(HasPrint, std::string (_::*)() const, &_::Print);

struct StdFormatter {
 public:
  template <typename A, typename B>
  inline static std::string F(const std::pair<A, B>&);

  template <typename V>
  inline static std::string F(const V&);

  template <typename V, std::enable_if_t<HasToString<V>::kValue, void>>
  inline static std::string F(const V&);

  template <typename V, std::enable_if_t<HasPrint<V>::kValue, void>>
  inline static std::string F(const V&);
};

}  // namespace _

template <typename I, typename F = _::StdFormatter>
std::string Join(I, I, const char* d = ";");

// Notes, this is a fully representation of duration, ie it stores very detail
// information instead of casting everything into 0s. It is mainly used as an
// intermediate representation until user finalize it into concrete
// representation
struct Duration {
  std::optional<std::uint64_t> nanosecond;
  std::optional<std::uint64_t> microsecond;
  std::optional<std::uint64_t> millisecond;
  std::optional<std::uint64_t> second;
  std::optional<std::uint64_t> minute;
  std::optional<std::uint64_t> hour;

  Duration(std::optional<std::uint64_t> ns = {},
           std::optional<std::uint64_t> us = {},
           std::optional<std::uint64_t> ms = {},
           std::optional<std::uint64_t> s = {},
           std::optional<std::uint64_t> m = {},
           std::optional<std::uint64_t> h = {})
      : nanosecond(ns),
        microsecond(us),
        millisecond(ms),
        second(s),
        minute(m),
        hour(h) {}

  Duration(const Duration&) = default;
  Duration(Duration&&) = default;
  Duration& operator=(const Duration&) = default;
  Duration& operator=(Duration&&) = default;

  static Duration Msec(std::uint64_t v) { return Duration{0, 0, v}; }
  static Duration Sec(std::uint64_t v) { return Duration{0, 0, 0, v}; }
  static Duration Min(std::uint64_t v) { return Duration{0, 0, 0, 0, v}; }
  static Duration Hour(std::uint64_t v) { return Duration{0, 0, 0, 0, 0, v}; }

  // Notes(dpeng): No convinient function to calculate nanosecond/microsecond
  //   personal experiense shows that these 2 factors are barely usable
  inline std::uint64_t AsMillisecond() const;
  inline std::uint64_t AsSecond() const;
  inline std::uint64_t AsMinute() const;
  inline std::uint64_t AsHour() const;
};

// Represent storage size, ie byte, kbyte, mbyte, gbyte, tbyte etc ...
struct StorageSize {
  std::optional<std::uint64_t> byte_size;
  std::optional<std::uint64_t> kbyte_size;
  std::optional<std::uint64_t> mbyte_size;
  std::optional<std::uint64_t> gbyte_size;

  StorageSize(std::optional<std::uint64_t> bs = {},
              std::optional<std::uint64_t> kbs = {},
              std::optional<std::uint64_t> mbs = {},
              std::optional<std::uint64_t> gbs = {})
      : byte_size(bs), kbyte_size(kbs), mbyte_size(mbs), gbyte_size(gbs) {}

  StorageSize(const StorageSize&) = default;
  StorageSize(StorageSize&&) = default;
  StorageSize& operator=(const StorageSize&) = default;
  StorageSize& operator=(StorageSize&&) = default;

  inline std::uint64_t AsBSize() const;
  inline std::uint64_t AsKBSize() const;
  inline std::uint64_t AsMBSize() const;
  inline std::uint64_t AsGBSize() const;
};

template <typename T = std::chrono::high_resolution_clock,
          typename Res = Microseconds>
class PerfTimer {
 public:
  using TimePointType = typename T::time_point;
  inline explicit PerfTimer(std::uint64_t* diff_output, bool start = true);

  void Start() { ts_start_ = T::now(); }
  inline std::uint64_t Stop();
  inline ~PerfTimer();

 private:
  TimePointType ts_start_;
  std::uint64_t* output_;
};

using PerfTimerMicroseconds =
    PerfTimer<std::chrono::high_resolution_clock, Microseconds>;

using PerfTimerMilliseconds =
    PerfTimer<std::chrono::high_resolution_clock, Milliseconds>;

using PerfTimerNanoseconds =
    PerfTimer<std::chrono::high_resolution_clock, Nanoseconds>;

using MonotonicTimerMicroseconds =
    PerfTimer<std::chrono::steady_clock, Microseconds>;

using MonotonicTimerMilliseconds =
    PerfTimer<std::chrono::steady_clock, Milliseconds>;

using MonotonicTimerNanoseconds =
    PerfTimer<std::chrono::steady_clock, Nanoseconds>;

}  // namespace base

// Bunch of convinient things for std related stuff
namespace std {

inline std::string to_string(const std::string_view& sv) {
  return std::string{sv.data(), sv.size()};
}

inline std::string to_string(const std::string& v) { return v; }

inline std::string to_string(char c) { return std::string(1, c); }

inline std::string to_string(bool v) {
  return v ? std::string{"true"} : std::string{"false"};
}

template <typename T>
std::string to_string(const std::vector<T>& v) {
  return ::base::Join(v.begin(), v.end(), ",");
}

template <typename T>
std::string to_string(const std::list<T>& v) {
  return ::base::Join(v.begin(), v.end(), ",");
}

template <typename T>
std::string to_string(const std::deque<T>& v) {
  return ::base::Join(v.begin(), v.end(), ",");
}

template <typename K, typename V>
std::string to_string(const std::map<K, V>& v) {
  return ::base::Join(v.begin(), v.end(), ",");
}

template <typename K, typename V>
std::string to_string(const std::unordered_map<K, V>& v) {
  return ::base::Join(v.begin(), v.end(), ",");
}

inline std::string to_string(const ::base::Duration&);
inline std::string to_string(const ::base::StorageSize&);
template <typename T>
inline std::string to_string(const T* ptr) {
  return ::base::Fmt("%p", static_cast<const void*>(ptr));
}

template <typename T>
inline std::string to_string(T* ptr) {
  return ::base::Fmt("%p", static_cast<void*>(ptr));
}

}  // namespace std

// Stream related helper function for allowing us to dump certain things much
// more conviniently.
template <typename K>
std::ostream& operator<<(std::ostream&, const std::vector<K>&);

template <typename K>
std::ostream& operator<<(std::ostream&, const std::list<K>&);

template <typename K>
std::ostream& operator<<(std::ostream&, const std::deque<K>&);

template <typename K, typename V>
std::ostream& operator<<(std::ostream&, const std::map<K, V>&);

template <typename K, typename V>
std::ostream& operator<<(std::ostream&, const std::unordered_map<K, V>&);

inline std::ostream& operator<<(std::ostream&, const ::base::Duration&);
inline std::ostream& operator<<(std::ostream&, const ::base::StorageSize&);

template <typename U,
          std::enable_if_t<::base::_::HasToString<U>::kValue, int> = 0>
inline std::ostream& operator<<(std::ostream& os, const U& v) {
  os << v.ToString();
  return os;
}

template <typename U, std::enable_if_t<::base::_::HasPrint<U>::kValue, int> = 0>
inline std::ostream& operator<<(std::ostream& os, const U& v) {
  os << v.Print();
  return os;
}

#include "common-inl.h"

#endif  // _BASE_COMMON_H_
