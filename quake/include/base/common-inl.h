#ifndef _BASE_COMMON_INL_H_
#define _BASE_COMMON_INL_H_

// Currently we still use boost/lexical_cast since it is better than the C++1X
// std::stoxxx which doesn't handle negative number properly
#include <boost/lexical_cast.hpp>

namespace base {

template <typename T>
std::optional<T> LexicalCast(const std::string_view& data) {
  T v;
  if (boost::conversion::try_lexical_convert(data, v)) {
    return std::optional<T>(std::move(v));
  }
  return std::optional<T>{};
}

template <>
inline std::optional<std::string> LexicalCast(const std::string_view& d) {
  return std::optional<std::string>{d};
}

template <>
inline std::optional<bool> LexicalCast(const std::string_view& d) {
  if (d == "true") {
    return true;
  } else if (d == "false") {
    return false;
  } else {
    return std::optional<bool>{};
  }
}

template <typename T, typename Res>
inline PerfTimer<T, Res>::PerfTimer(std::uint64_t* diff_output, bool start)
    : ts_start_(), output_(diff_output) {
  if (start) {
    ts_start_ = T::now();
  }
}

template <typename T, typename Res>
inline std::uint64_t PerfTimer<T, Res>::Stop() {
  auto now = T::now();
  return static_cast<std::uint64_t>(
      std::chrono::duration_cast<Res>(now - ts_start_).count());
}

template <typename T, typename Res>
inline PerfTimer<T, Res>::~PerfTimer() {
  if (output_) {
    auto out = output_;
    output_ = nullptr;
    *out = Stop();
  }
}

namespace _ {

template <typename A, typename B>
std::string StdFormatter::F(const std::pair<A, B>& p) {
  return Fmt("%s = %s", F(p.first).c_str(), F(p.second).c_str());
}

template <typename V>
std::string StdFormatter::F(const V& p) {
  return std::to_string(p);
}

}  // namespace _

template <typename I, typename F>
std::string Join(I beg, I end, const char* d) {
  std::stringstream f;

  // Very first one, we print the whole stuff as following BNF:
  //    first (delimiter REST)*
  if (beg != end) {
    f << F::F(*beg);
    ++beg;
  }

  // Rest
  for (; beg != end; ++beg) {
    f << d;
    f << F::F(*beg);
  }
  return f.str();
}

inline Ts Now() { return TsClock::now(); }
inline MonoTs MonoNow() { return MonoClock::now(); }
inline Ts FromUnixTs(Milliseconds ts) { return Ts{ts}; }

template <typename Duration>
inline Ts AddTTL(Ts now, Duration ttl) {
  const auto max = Ts::max();
  const auto duration = std::chrono::duration_cast<Duration>(max - now);
  if (duration <= ttl) {
    return max;
  } else {
    return now + ttl;
  }
}

inline std::uint64_t NowInMicroseconds() {
  auto n = std::chrono::system_clock::now().time_since_epoch();
  return static_cast<std::uint64_t>(
      std::chrono::duration_cast<Microseconds>(n).count());
}

inline std::uint64_t NowInMilliseconds() { return NowInMicroseconds() / 1000; }

inline std::uint64_t NowInSeconds() { return NowInMilliseconds() / 1000; }

inline std::uint64_t Duration::AsMillisecond() const {
  auto ns = (nanosecond ? nanosecond.value() : 0) / 1000000;
  auto us = (microsecond ? microsecond.value() : 0) / 1000;
  auto ms = (millisecond ? millisecond.value() : 0);
  auto s = (second ? second.value() : 0) * 1000;
  auto m = (minute ? minute.value() : 0) * 1000 * 60;
  auto h = (hour ? hour.value() : 0) * 1000 * 60 * 60;
  return ns + us + ms + s + m + h;
}

inline std::uint64_t Duration::AsSecond() const {
  return AsMillisecond() / 1000;
}

inline std::uint64_t Duration::AsMinute() const { return AsSecond() / 60; }

inline std::uint64_t Duration::AsHour() const { return AsMinute() / 60; }

inline std::uint64_t StorageSize::AsBSize() const {
  auto bs = (byte_size ? byte_size.value() : 0);
  auto ks = (kbyte_size ? kbyte_size.value() : 0) * 1024;
  auto ms = (mbyte_size ? mbyte_size.value() : 0) * 1024 * 1024;
  auto gs = (gbyte_size ? gbyte_size.value() : 0) * 1024 * 1024 * 1024;
  return bs + ks + ms + gs;
}

inline std::uint64_t StorageSize::AsKBSize() const { return AsBSize() / 1024; }

inline std::uint64_t StorageSize::AsMBSize() const { return AsKBSize() / 1024; }

inline std::uint64_t StorageSize::AsGBSize() const { return AsMBSize() / 1024; }

}  // namespace base

namespace std {

inline std::string to_string(const ::base::Duration& d) {
  return ::base::Fmt(
      "duration("
      "ns=%" PRIu64
      ","
      "us=%" PRIu64
      ","
      "ms=%" PRIu64
      ","
      "s=%" PRIu64
      ","
      "m=%" PRIu64
      ","
      "h=%" PRIu64 ")",
      ::base::Get(d.nanosecond, 0), ::base::Get(d.microsecond, 0),
      ::base::Get(d.millisecond, 0), ::base::Get(d.second, 0),
      ::base::Get(d.minute, 0), ::base::Get(d.hour, 0));
}

inline std::string to_string(const ::base::StorageSize& v) {
  return ::base::Fmt(
      "storage("
      "byte=%" PRIu64
      ","
      "kbyte=%" PRIu64
      ","
      "mbyte=%" PRIu64
      ","
      "gbyte=%" PRIu64 ")",
      ::base::Get(v.byte_size, 0), ::base::Get(v.kbyte_size, 0),
      ::base::Get(v.mbyte_size, 0), ::base::Get(v.gbyte_size, 0));
}

}  // namespace std

template <typename K>
std::ostream& operator<<(std::ostream& os, const std::vector<K>& v) {
  os << std::to_string(v);
  return os;
}

template <typename K>
std::ostream& operator<<(std::ostream& os, const std::list<K>& v) {
  os << std::to_string(v);
  return os;
}

template <typename K>
std::ostream& operator<<(std::ostream& os, const std::deque<K>& v) {
  os << std::to_string(v);
  return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& v) {
  os << std::to_string(v);
  return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& v) {
  os << std::to_string(v);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const ::base::Duration& v) {
  os << std::to_string(v);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const ::base::StorageSize& v) {
  os << std::to_string(v);
  return os;
}

#endif  // _BASE_COMMON_INL_H_
