#include <base/crash.h>
#include <base/log.h>
#include <base/check.h>
#include <base/os.h>

#ifdef OS_POSIX
#include <signal.h>
#endif  // OS_POSIX

namespace base {
namespace {

[[noreturn]] void OnTerminate() noexcept {
  std::set_terminate(nullptr);

  if (auto excep_ptr = std::current_exception(); excep_ptr) {
    try {
      std::rethrow_exception(excep_ptr);
    } catch (const std::exception& e) {
      Die("unhandle exception: %s", e.what());
    } catch (...) {
      Die("unknown exception");
    }
  }

  Die("unknown terminate");
}

#ifdef OS_LINUX

void OnBadSyscall(int sig_number, siginfo_t* info, void* ucontext) {
  if (sig_number != SIGSYS) return;
  LOG(FATAL) << '\n'
             << "================================\n"
             << "===== FATAL SIGNAL HAPPENED ====\n"
             << "+++++ Invalid System Call   ++++\n"
             << "================================\n"
             << "syscall number: " << info->si_syscall;
}

void InstallOnBadSyscall() {
  struct sigaction sigact = {0};
  sigact.sa_sigaction = OnBadSyscall;
  sigact.sa_flags = SA_RESTART | SA_SIGINFO;

  // Just register SIGSEGV first, and we may need to register other signal as
  // well here.
  CHECK(!::sigaction(SIGSYS, &sigact, nullptr));
}

#endif  // OS_LINUX

#ifdef OS_POSIX
#ifndef ASAN_ON

void OnDeadSignal(int sig_number, siginfo_t* info, void* ucontext) {
  UNUSE(info);
  UNUSE(ucontext);

  auto maybe_signal_name = os::GetSignalName(sig_number);
  auto signal_name =
      (maybe_signal_name ? maybe_signal_name.PassResult() : std::string{"N/A"});

  LOG(FATAL) << '\n'
             << "================================\n"
             << "===== FATAL SIGNAL HAPPENED ====\n"
             << "===== Signal(" << signal_name << "@" << sig_number
             << ") =====\n"
             << "================================";
}

void InstallDeadSignalHandler() {
  struct sigaction sigact = {0};
  sigact.sa_sigaction = OnDeadSignal;
  sigact.sa_flags = SA_RESTART | SA_SIGINFO;

  // Just register SIGSEGV first, and we may need to register other signal as
  // well here.
  CHECK(!::sigaction(SIGSEGV, &sigact, nullptr));

#ifdef OS_LINUX
  InstallOnBadSyscall();
#endif  // OS_LINUX
}

#else

void InstallDeadSignalHandler() {
#ifdef OS_LINUX
  InstallOnBadSyscall();
#endif  // OS_LINUX
}

#endif  // ASAN_ON
#endif  // OS_POSIX

}  // namespace

void InstallCrashHandler() {
  std::set_terminate(OnTerminate);
#ifdef OS_POSIX
  InstallDeadSignalHandler();
#endif  // OS_POSIX
}

}  // namespace base
