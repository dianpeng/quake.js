#include <base/common.h>

// Notes(dpeng): We should build our binary with -rdynamic flags to allow the
//   stacktrace a little bit more accurate. If we use the default backtrace,
//   issue is it is too slow, ie forking process to call addrtoline one by one
//   to generate location information is horribly slow.
#include <boost/stacktrace/frame.hpp>
#include <boost/stacktrace/stacktrace.hpp>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif               // _GNU_SOURCE
#include <string.h>  // for strerror_r which is thread safe

#include <fstream>
#include <iterator>

namespace base {

std::optional<std::string> LoadWholeFile(const std::string_view& path,
                                         std::size_t max) {
  std::string path_string{path.data(), path.size()};
  std::ifstream f{path_string.c_str()};
  if (!f) {
    return {};
  }
  f >> std::noskipws;

  // Notes, fstream does throw exception, we will need to guard it. For example,
  // if trying to read data from a fstream that is been mutated by external
  // process which may break the internal buffer state thus leading to an
  // exception. We just guard everything and then return an {} to indicate the
  // read failed
  try {
    f.seekg(0, std::ios_base::end);
    const auto file_size = static_cast<std::size_t>(f.tellg());
    const auto buf_size = std::min(max, file_size);
    f.seekg(0, std::ios_base::beg);

    std::string r;
    r.resize(buf_size);
    auto buffer_size = f.readsome(r.data(), r.size());
    r.resize(buffer_size);
    return r;
  } catch (...) {
    return {};
  }
}

std::string Fmt(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto b = FmtV(fmt, vl);
  va_end(vl);
  return b;
}

std::string FmtV(const char* fmt, va_list vl) {
  static const int kBufferSize = 1024;

  std::string b;
  va_list backup;
  va_copy(backup, vl);

  b.resize(kBufferSize);

  // Notes(dpeng):
  //   We are using c++17 which means std::string::data returns a
  //   none const pointer if std::string itself is not const. If we switch back
  //   to < c++17, then the following code won't compile since vsnprintf
  //   requires a mutable pointer.
  auto r = ::vsnprintf(b.data(), kBufferSize, fmt, vl);

  if (r >= kBufferSize) {
    b.resize(r + 1);
    r = ::vsnprintf(b.data(), r + 1, fmt, backup);
  }
  b.resize(r);

  va_end(backup);
  return b;
}

void DieNow() noexcept { std::abort(); }

void Die() noexcept {
  std::cerr << "[bt]: \n" << GetStackTrace() << std::endl;
  DieNow();
}

void DieF(const char* fmt, va_list vl) {
  auto msg = FmtV(fmt, vl);
  std::cerr << "[die]: " << msg << std::endl;
  Die();
}

void Die(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  DieF(fmt, vl);
  // never reach here
  va_end(vl);
}

std::string ErrString(int e) {
  char buf[1024];
  return std::string{strerror_r(e, buf, 1024)};
}

std::string ErrString() { return ErrString(errno); }

std::string GetStackTrace() {
  std::string b;
  AppendStackTrace(&b);
  return b;
}

void AppendStackTrace(std::string* b) {
  int i = 0;
  for (const auto& entry : boost::stacktrace::stacktrace()) {
    b->append(Fmt("#%d %s(%p) %d:%s\n", i, entry.name().c_str(),
                  entry.address(), entry.source_line(),
                  entry.source_file().c_str()));
    ++i;
  }
}

void JSONEscape(const std::string_view& s, std::string* o) {
  o->reserve(o->size() + (s.size() + 8));

  // Notes(dpeng): Unicode code sequences are not handled by design. The routine
  //   is only good for ASCII/Latino-1 character sequences as for now. See JSON
  //   official spec for more information
  for (const auto& c : s) {
    switch (c) {
      case '\t':
        o->push_back('\\');
        o->push_back('t');
        break;
      case '\n':
        o->push_back('\\');
        o->push_back('n');
        break;
      case '\f':
        o->push_back('\\');
        o->push_back('f');
        break;
      case '\b':
        o->push_back('\\');
        o->push_back('n');
        break;
      case '\r':
        o->push_back('\\');
        o->push_back('r');
        break;
      case '\\':
        o->push_back('\\');
        o->push_back('\\');
        break;
      case '/':
        o->push_back('\\');
        o->push_back('/');
        break;
      case '"':
        o->push_back('\\');
        o->push_back('"');
        break;
      default:
        o->push_back(c);
        break;
    }
  }
}

std::string JSONEscape(const std::string_view& s) {
  std::string o;
  JSONEscape(s, &o);
  return o;
}

}  // namespace base
