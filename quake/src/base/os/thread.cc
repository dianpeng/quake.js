#include <base/os/thread.h>

#if defined(OS_LINUX)
#include <syscall.h>
#else
#error "not supported"
#endif  // OS_LINUX

// for CPU profiler
#include <gperftools/profiler.h>

namespace base {
namespace os {

void Thread::SetName(const char* name) {
  pthread_setname_np(pthread_self(), name);
}

std::uint64_t Thread::GetCpuTime() {
  struct timespec r {};
  errno = 0;
  if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &r)) {
    return 0;
  }
  return static_cast<std::uint64_t>(r.tv_sec * 1000000 + r.tv_nsec / 1000);
}

std::uint64_t Thread::GetThreadId() {
  static thread_local std::uint64_t kTid = 0;
#if defined(SYS_gettid)
  if (!kTid) {
    auto tid = syscall(SYS_gettid);
    kTid = static_cast<std::uint64_t>(tid);
  }
  return kTid;
#else
  return 0;
#endif  // SYS_gettid
}

Thread::~Thread() { DCHECK(!context_) << "Thread is not joined or detached"; }

Error Thread::Start(ThreadEntry&& callback, void* udata) {
  auto data = std::make_unique<Data>(std::move(callback), udata);

  // Notes, std::thread |CAN| throw exception during initialization, ie kernel
  // panic on memory etc ... We sandbox the exception here and return user a
  // properly crafted error
  try {
    context_.reset(new Context{Thread::ThreadMain, data.get()});
  } catch (const std::system_error& e) {
    context_.reset();
    return Err(kErrorThread, "cannot create thread: %s", e.what());
  } catch (...) {
    context_.reset();
    return Err(kErrorThread, "cannot create thread");
  }

  context_->init_future = data->init.get_future();
  context_->exit_future = data->exit.get_future();
  data.release();
  return {};
}

Result<bool> Thread::BlockForInit(Milliseconds time) {
  if (!context_) {
    return Err(kErrorThread, "Thread::BlockForInit on thread not started");
  }

  auto result = context_->init_future.wait_for(time);
  switch (result) {
    case std::future_status::timeout:
    case std::future_status::deferred:
      return Okay(false);
    default:
      return Okay(true);
  }
}

Result<bool> Thread::BlockForExit(Milliseconds time) {
  if (!context_) {
    return Err(kErrorThread, "Thread::BlockForInit on thread not started");
  }

  auto r = context_->exit_future.wait_for(time);
  auto done = false;

  switch (r) {
    case std::future_status::timeout:
    case std::future_status::deferred:
      done = false;
      break;
    default:
      done = true;
      break;
  }

  if (done) {
    context_->thread.join();
    context_.reset();
  }

  return Okay(done);
}

Error Thread::Detach() {
  if (!context_) {
    return Err(kErrorThread, "thread hasn't been started");
  }

  try {
    context_->thread.detach();
  } catch (const std::system_error& e) {
    return Err(kErrorThread, "thread detach failed: %s", e.what());
  } catch (...) {
    return Err(kErrorThread, "thread detach failed");
  }

  context_.reset();
  return {};
}

void Thread::ThreadMain(void* ptr) {
  auto data = static_cast<Data*>(ptr);
  DEFER([data]() { delete data; });

  // Telling gperftools that we want to profile the current thread. Notes, the
  // framework will always use os::Thread instead of std::thread, so we are
  // totally fine for letting gperftools to perform CPU profiling
  ::ProfilerRegisterThread();

  data->init.set_value();
  data->callback(data->user_data);
  data->exit.set_value();
}

}  // namespace os
}  // namespace base
