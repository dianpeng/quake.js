#ifndef _BASE_OS_THREAD_H_
#define _BASE_OS_THREAD_H_
#include <base/common.h>
#include <base/result.h>

#include "basic.h"

namespace base {
namespace os {

// A thread wrapper around std::thread which provides stable sycnhornization
// point. Notes, in older g++, ie older than 8.0, the exception throwned by
// std::thread, if uncaptured, will not show its stacktrace in gdb. If that's
// the case, one can switch the thread from std::thread to boost::thread which
// does a proper wrapper for pthread underlying
class Thread {
 public:
  Thread() : context_() {}
  ~Thread();

  // Linear type, thread can only be moved
  Thread(Thread&&) = default;
  Thread& operator=(Thread&&) = default;

 public:
  using ThreadEntry = std::function<void(void*)>;

  // Starts thread, ie letting it run. The return of this function doesn't
  // mean thread has already started. See following sycnnorization functions
  Error Start(ThreadEntry&&, void*);

  // Block for the initializaiton of function happened and returned. Notes
  // this function only returns when the entry starts to run
  Result<bool> BlockForInit(Milliseconds);
  inline void BlockForInit();

  // Similar as Init, but only returned when the thread indeed quit, ie the
  // entry function returned. Notes, if the entry function calls things like
  // pthread_exit, this function will never return. Pls be aware that do not
  // use OS level thread exit routine. Currently we do not bother do per thread
  // atexit.
  Result<bool> BlockForExit(Milliseconds);
  inline void BlockForExit();

  Error Detach();

 public:
  // Set the |Current| thread's name, useful for debugging purpose
  static void SetName(const char*);

  // Get the |Current| thread's CPU time. Notes, if platform unsupported, return
  // 0
  static std::uint64_t GetCpuTime();

  // Get the |Currnet| thread's thread id, ie OS level id.
  static std::uint64_t GetThreadId();

 private:
  static void ThreadMain(void*);

  struct Data {
    void* user_data;
    ThreadEntry callback;
    std::promise<void> init;
    std::promise<void> exit;
    Data(ThreadEntry&& c, void* ud)
        : user_data(ud), callback(std::move(c)), init(), exit() {}
  };

  struct Context {
    std::thread thread;
    std::future<void> init_future;
    std::future<void> exit_future;
    Context(void (*e)(void*), void* ptr)
        : thread(e, ptr), init_future(), exit_future() {}
  };

  std::unique_ptr<Context> context_;
  DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  // namespace os
}  // namespace base

#include "thread-inl.h"

#endif  // _BASE_OS_THREAD_H_
