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

inline bool AppendUtf8(std::uint32_t codepoint, std::string* o) {
  auto size = o->size();
  o->resize(size + 4);
  if (auto v = Utf8Encode(
          codepoint, reinterpret_cast<std::uint8_t*>(o->data() + size));
      v) {
    o->resize(size + v.value());
    return true;
  } else {
    return false;
  }
}

inline std::optional<std::size_t> Utf8Encode(std::uint32_t cp,
                                             std::uint8_t* b) {
  if (!_::IsCodePointValid(cp)) {
    return {};
  }
  auto start = b;

  if (cp < 0x80) {
    *(b++) = static_cast<std::uint8_t>(cp);
  } else if (cp < 0x800) {
    *(b++) = static_cast<std::uint8_t>((cp >> 6) | 0xc0);
    *(b++) = static_cast<std::uint8_t>((cp & 0x3f) | 0x80);
  } else if (cp < 0x10000) {
    *(b++) = static_cast<std::uint8_t>((cp >> 12) | 0xe0);
    *(b++) = static_cast<std::uint8_t>(((cp >> 6) & 0x3f) | 0x80);
    *(b++) = static_cast<std::uint8_t>((cp & 0x3f) | 0x80);
  } else {
    *(b++) = static_cast<uint8_t>((cp >> 18) | 0xf0);
    *(b++) = static_cast<uint8_t>(((cp >> 12) & 0x3f) | 0x80);
    *(b++) = static_cast<uint8_t>(((cp >> 6) & 0x3f) | 0x80);
    *(b++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
  }
  return static_cast<std::size_t>(b - start);
}

inline bool Utf8Encode(const std::vector<std::uint32_t>& in,
                       std::vector<std::uint8_t>* out) {
  auto idx = out->size();
  out->reserve(idx + in.size() * 2);
  for (const auto& entry : in) {
    std::uint8_t t[4];
    auto size = Utf8Encode(entry, t);
    if (size) {
      switch (*size) {
        case 1:
          out->push_back(t[0]);
          break;
        case 2:
          out->push_back(t[0]);
          out->push_back(t[1]);
          break;
        case 3:
          out->push_back(t[0]);
          out->push_back(t[1]);
          out->push_back(t[2]);
          break;
        case 4:
          out->push_back(t[0]);
          out->push_back(t[1]);
          out->push_back(t[2]);
          out->push_back(t[3]);
          break;
        default:
          UNREACHABLE();
          break;
      }
    } else {
      return false;
    }
  }
  return true;
}

inline std::optional<std::size_t> DecodeOneUtf8(const void* data,
                                                std::size_t size,
                                                std::uint32_t* oc) {
  auto state = utf8::State::kAccept;
  std::uint32_t o = 0;
  std::size_t off = 0;
  auto bbuffer = static_cast<const std::uint8_t*>(data);

  while (off < size) {
    utf8::Decode(bbuffer[off], &state, &o);
    ++off;

    switch (state) {
      case utf8::State::kAccept:
        *oc = o;
        return off;
      case utf8::State::kReject:
        return {};
      default:
        break;
    }
  }

  // failed with decoding a single byte
  return {};
}

}  // namespace base

// Used to invoke certain function, typically automatical register function
// glboally. It just uses global defined object's ctor will be invoked
// automatically in C++. Notes, the order is not guaranteed since it relies on
// link time.
#define BASE_AUTO_INVOKE(NAME, ...) \
  ::base::_::Registry NAME { __VA_ARGS__ }

#endif  // _BASE_UTIL_INL_H_
