#include <base/allocator.h>
#include <fstream>

#include <base/os.h>
#include <base/string-util.h>

#ifdef OS_LINUX
#include <sys/resource.h>
#include <unistd.h>
#else
#error "unsupported OS"
#endif  // OS_POSIX

namespace base {

#if defined(OS_LINUX)

static Error HandleLine(const char* buf, std::size_t length, std::size_t* out) {
  bool has = false;
  std::size_t count = 0;

  Tokenize(
      buf, length, " ",
      [&](const char* data, std::size_t length) -> bool {
        ++count;
        if (count == 2) {
          if (auto v = LexicalCast<std::size_t>(std::string{data, length}); v) {
            has = true;
            *out = v.value();
          } else {
            has = false;
          }
          return false;
        } else {
          return true;
        }
      },
      true);

  if (!has) {
    return Error::Mk("GetProcessCurrentRSS",
                     "invalid/inapproriate format of statm: %s",
                     std::string(buf, length).c_str());
  }
  return Error{};
}

Result<std::size_t> GetProcessCurrentRSS() {
  static const std::size_t kMaxRetryTimes = 8;
  static const std::size_t kBufferSize = 4096;
  // We use a large enough buffer to allow a |single| read into the file to get
  // the data out. If we don't succeed, then we retry the operation for few
  // times and then bailout with unknown current RSS size.
  char buf[kBufferSize];
  std::optional<std::size_t> rss_page;
  Error last_error;

  for (std::size_t i = 0; i < kMaxRetryTimes; ++i) {
    os::RawFile file;

    if (auto e = file.Open("/proc/self/statm", os::RawFile::kRead); e) {
      return e;
    }

    std::size_t read_size = 0;
    std::size_t rss = 0;

    if (auto r = file.Read(buf, kBufferSize); !r) {
      last_error = r.PassError();
      continue;
    } else {
      read_size = r.PassResult();
    }
    if (auto e = HandleLine(buf, read_size, &rss); e) {
      last_error = std::move(e);
      continue;
    } else {
      rss_page = rss;
      break;
    }
  }
  if (rss_page) {
    return rss_page.value() * os::GetPageSize();
  } else {
    return last_error;
  }
}

#elif
#error "unsupported OS"
#endif  // OS_LINUX

}  // namespace base

#if defined(SPARROW_HAS_TCMALLOC)
#include "allocator.tcmalloc.inl"
#elif defined(SPARROW_HAS_JEMALLOC)
#include "allocator.jemalloc.inl"
#else
#include "allocator.glibc.inl"
#endif
