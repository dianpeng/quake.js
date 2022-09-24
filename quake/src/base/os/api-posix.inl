#include <base/common.h>

#if defined(OS_POSIX)
#include <base/os/api.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif  // _GNU_SOURCE

#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>

#if defined(OS_LINUX)
#include <time.h>
#endif  // OS_LINUX

#define POSIX_SIGNAL_LIST(__) \
  __(SIGHUP, HUP)             \
  __(SIGABRT, ABRT)           \
  __(SIGBUS, BUS)             \
  __(SIGSEGV, SEGV)           \
  __(SIGSYS, SYS)             \
  __(SIGTERM, TERM)           \
  __(SIGPIPE, PIPE)           \
  __(SIGALRM, ALRM)           \
  __(SIGINT, INT)             \
  __(SIGCONT, CONT)           \
  __(SIGQUIT, QUIT)           \
  __(SIGILL, ILL)             \
  __(SIGTRAP, TRAP)           \
  __(SIGSTOP, STOP)           \
  __(SIGTSTP, TSTP)           \
  __(SIGURG, URG)             \
  __(SIGCHLD, CHLD)           \
  __(SIGTTIN, TTIN)           \
  __(SIGTTOU, TTOU)           \
  __(SIGPROF, PROF)           \
  __(SIGWINCH, WINCH)         \
  __(SIGUSR1, USR1)           \
  __(SIGUSR2, USR2)

static_assert(sizeof(uid_t) <= sizeof(std::int64_t));
static_assert(sizeof(gid_t) <= sizeof(std::int64_t));

namespace base {
namespace os {

Error Chdir(const char* path) {
  errno = 0;
  if (::chdir(path)) {
    return ErrSys("chdir", errno);
  }
  return {};
}

Result<std::string> PWD() {
  std::string p;
  p.resize(PATH_MAX);
  if (::getcwd(p.data(), p.size())) {
    auto size = std::strlen(p.data());
    CHECK(size <= p.size());
    p.resize(size);
    if (!p.empty() && p[0] != '/') {
      return ErrSys("getcwd, chroot without setting pwd", errno);
    } else {
      return Okay(std::move(p));
    }
  } else {
    return ErrSys("getcwd", errno);
  }
}

std::string PWD(const char* def) {
  if (auto r = PWD(); !r) {
    return def;
  } else {
    return r.PassResult();
  }
}

Result<const char*> GetSignalName(int sig) {
  switch (sig) {
#define __(A, B) \
  case A:        \
    return Okay(static_cast<const char*>(#B));
    POSIX_SIGNAL_LIST(__)
#undef __

    default:
      return Err(kErrorInvalidInput, "signal %d unknown", sig);
  }
}

Result<int> ParseSignal(const std::string& str) {
#define __(A, B)                \
  if (str == #B || str == #A) { \
    return Okay(A);             \
  }

  POSIX_SIGNAL_LIST(__)

#undef __

  if (auto v = LexicalCast<int>(str); v) {
    return Okay(v.value());
  }
  return Err(kErrorInvalidInput, "signal %s unknown", str.c_str());
}

std::size_t GetPageSize() {
  static std::size_t kPageSize = 0;
  if (!kPageSize) {
    kPageSize = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));
  }
  return kPageSize;
}

Result<std::uint64_t> ProcessGetCpuTime() {
#if defined(OS_LINUX)
  struct timespec r {};
  errno = 0;
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &r)) {
    return ErrSys("clock_gettime", errno);
  }
  return Okay(
      static_cast<std::uint64_t>(r.tv_sec * 1000000 + r.tv_nsec / 1000));
#else
  LOG(ERROR) << "OS doesn't support";
  return ErrNA("ProcessGetCpuTime");
#endif  // OS_LINUX
}

std::int32_t GetPid() { return static_cast<std::int32_t>(::getpid()); }

Error Kill(std::int32_t pid, int sig) {
  if (POSIX_SYSCALL(kill(static_cast<pid_t>(pid), sig))) {
    return ErrSys("kill", errno);
  }
  return {};
}

bool GetDefaultDNSServer(std::vector<std::string>* output) {
  static const char* kResolveConf = "/etc/resolve.conf";

  if (auto v = LoadWholeFile(kResolveConf); v) {
    ParseNameServerFromResolveConf(output, v.value());
    return true;
  }
  return false;
}

Error Link(const std::string& target, const std::string& link,
           LinkType link_type) {
  if (link_type == kSymbolic) {
    if (auto e = POSIX_SYSCALL(symlink(target.c_str(), link.c_str())); e) {
      return ErrSys("symlink", errno);
    }
    return Error{};
  } else {
    return ErrNA("Link");
  }
}

Error Unlink(const std::string& target) {
  if (auto e = POSIX_SYSCALL(unlink(target.c_str())); e) {
    return ErrSys("unlink", errno);
  }
  return Error{};
}

Result<std::string> FollowLink(const std::string& link) {
  std::string b;
  b.resize(PATH_MAX + 1);

  if (auto v = readlink(link.c_str(), b.data(), b.size()); v >= 0) {
    b.resize(static_cast<std::size_t>(v));
    return b;
  }
  return ErrSys("readlink", errno);
}

}  // namespace os
}  // namespace base

#endif  // OS_POSIX
