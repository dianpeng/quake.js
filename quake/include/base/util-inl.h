#ifndef _BASE_UTIL_INL_H_
#define _BASE_UTIL_INL_H_

namespace base {

// unique_ptr for C pointer, ie invoke free when it goes out of the scope
template <typename T>
using unique_cptr = std::unique_ptr<T, _::FreeDeleter>;

template <typename F>
_::SharedFunction<std::decay_t<F>> MkSharedFunction(F&& f) {
  return _::SharedFunction{std::forward<F>(f)};
}

template <std::size_t kStackSize>
void SSOBuffer<kStackSize>::InitBuffer(std::size_t size) {
  if (size > kStackSize) {
    heap_storage_.reset(new char[size]);
  }
}

inline DetectDestruct::ScopeFlag::ScopeFlag(DetectDestruct* d)
    : d_(d), flag_(false) {
  d_->flag_ = &flag_;
}

inline DetectDestruct::ScopeFlag::~ScopeFlag() {
  if (!flag_) {
    d_->flag_ = nullptr;
  }
}

inline DetectDestruct::~DetectDestruct() {
  if (flag_) {
    *flag_ = true;
  }
}

template <typename F1, typename F2>
bool ContinueIfNotDestruct(DetectDestruct* n, F1&& f1, F2&& f2) {
  DetectDestruct::ScopeFlag f{n};
  f1();
  if (!f.flag()) {
    f2();
    return true;
  }
  return false;
}

template <typename T, typename Ctor>
T* LazyInstance<T, Ctor>::LazyAccess() {
  if (!init_) {
    Ctor{}(raw_ptr());
    init_ = true;
  }
  return raw_ptr();
}

template <typename T, typename Ctor>
void LazyInstance<T, Ctor>::Deinit() {
  DCHECK(init_);
  CallDtor(raw_ptr());
  init_ = false;
}

template <typename T, typename Ctor>
bool LazyInstance<T, Ctor>::MaybeDeinit() {
  if (init_) {
    Deinit();
    return true;
  }
  return false;
}

template <typename T, typename Ctor>
template <typename... ARGS>
void LazyInstance<T, Ctor>::Init(ARGS&&... args) {
  DCHECK(init_);
  CallCtor(raw_ptr(), std::forward<ARGS>(args)...);
  init_ = true;
}

inline void CountdownLatch::Countdown() {
  bool notify = false;
  LOCK_GUARD(lock_) {
    CHECK(countdown_ > 0);
    --countdown_;
    if (countdown_ == 0) notify = true;
  }
  if (notify) cv_.notify_all();
}

inline void CountdownLatch::Join() {
  std::unique_lock<std::mutex> l{lock_};
  cv_.wait(l, [&]() { return countdown_ == 0; });
}

inline bool CountdownLatch::Join(Milliseconds timeout) {
  std::unique_lock<std::mutex> l{lock_};
  return cv_.wait_for(l, timeout, [&]() { return countdown_ == 0; });
}

}  // namespace base

// Used to invoke certain function, typically automatical register function
// glboally. It just uses global defined object's ctor will be invoked
// automatically in C++. Notes, the order is not guaranteed since it relies on
// link time.
#define BASE_AUTO_INVOKE(NAME, ...) \
  ::base::_::Registry NAME { __VA_ARGS__ }

#endif  // _BASE_UTIL_INL_H_
