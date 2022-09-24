#ifndef _BASE_ERROR_INL_H_
#define _BASE_ERROR_INL_H_

namespace base {

inline bool Error::Storage::TryClearOOL() {
  if (is_ool) {
    CallDtor(reinterpret_cast<OOLPayload*>(&info.ool));
    return true;
  }
  return false;
}

inline void Error::Storage::Swap(Storage* s) {
  std::swap(is_ool, s->is_ool);
  std::swap(reason, s->reason);

  // The swap is little bit complicated since the OOL is been placed
  // in place, we can not do a bitwise swap here.
  if (is_ool) {
    OOL()->Swap(s->OOL());
  } else {
    std::swap(info.category, s->info.category);
  }
}

inline void Error::Storage::MarkEmpty() {
  is_ool = false;
  info.category = nullptr;
  reason = kNoReason;
}

inline Error::Storage::Storage(Storage&& that)
    : info(), reason(that.reason), is_ool(that.is_ool) {
  if (that.is_ool) {
    CallCtor<OOLPayload>(OOL(), std::move(*that.OOL()));
  } else {
    info.category = that.info.category;
  }

  that.MarkEmpty();
}

inline Error::Storage::Storage(const Storage& that)
    : info(), reason(that.reason), is_ool(that.is_ool) {
  if (that.is_ool) {
    CallCtor<OOLPayload>(OOL(), *that.OOL());
  } else {
    info.category = that.info.category;
  }
}

inline void Error::Storage::Clear() {
  TryClearOOL();
  MarkEmpty();
}

inline std::uint32_t Error::LoadReason() const { return storage_.reason; }

inline const ErrorCategory* Error::LoadCategory() const {
  return IsOOL() ? OOL()->category : storage_.info.category;
}

Error::Error() : storage_() {}

Error::Error(const ErrorCategory* category, std::uint32_t reason)
    : storage_(category, reason) {}

Error::Error(const ErrorCategory* category, std::uint32_t reason,
             const std::string& msg)
    : storage_(category, reason, msg) {}

Error::Error(const ErrorCategory* category, std::uint32_t reason,
             std::string&& msg)
    : storage_(category, reason, std::move(msg)) {}

Error::Error(const Error& error) : storage_(error.storage_) {}

Error::Error(Error&& error) : storage_(std::move(error.storage_)) {}

Error& Error::operator=(const Error& that) {
  if (this == &that) {
    return *this;
  }
  Error temp{that};
  Swap(&temp);
  return *this;
}

Error& Error::operator=(Error&& that) {
  // Notes, if this = std::move(this), we just do nothing
  if (this == &that) {
    return *this;
  }
  Clear();
  Swap(&that);
  return *this;
}

inline std::string Error::GetMessage() const {
  return IsOOL() ? OOL()->message : std::string{};
}

inline std::string Error::PassMessage() {
  std::string r;
  if (IsOOL()) {
    r = std::move(OOL()->message);
    Clear();
  }
  return r;
}

// TODO(dpeng):
//   this is barely useful, we need to refine it little bit once we have
//   stable way to define customized category and reason, also give them
//   dianogstic name/information.
inline std::string Error::GetDisplay() const {
  if (!Has()) return {};

  if (IsOOL()) {
    return Fmt("info: %s; message: %s",
               OOL()->category->GetInformation(reason()).c_str(),
               OOL()->message.c_str());
  } else {
    return Fmt("info: %s",
               storage_.info.category->GetInformation(reason()).c_str());
  }
}

inline std::string Error::GetSafeMessage() const {
  auto msg = GetMessage();
  if (msg.empty()) return information();
  return Fmt("%s] %s", information().c_str(), msg.c_str());
}

// Factory functions
inline Error Error::Mk(const ErrorCategory* c, std::uint32_t r) {
  return Error{c, r};
}

inline Error Error::Mk(const ErrorCategory* c, std::uint32_t r,
                       const std::string& m) {
  return Error{c, r, m};
}

inline Error Error::Mk(const ErrorCategory* c, std::uint32_t r,
                       std::string&& m) {
  return Error{c, r, std::move(m)};
}

inline Error Error::Mk(const ErrorCategory* c, std::uint32_t r, const char* fmt,
                       ...) {
  va_list vl;
  va_start(vl, fmt);
  auto e = MkV(c, r, fmt, vl);
  va_end(vl);
  return e;
}

inline Error Error::MkV(const ErrorCategory* c, std::uint32_t r,
                        const char* fmt, va_list v) {
  auto msg = FmtV(fmt, v);
  return Error{c, r, std::move(msg)};
}

inline Error Error::Mk(const std::string& msg) {
  return Error{GeneralCategory(), kReasonUnspecified, msg};
}

inline Error Error::Mk(std::string&& msg) {
  return Error{GeneralCategory(), kReasonUnspecified, std::move(msg)};
}

inline Error Error::Mk(const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  auto e = MkV(fmt, vl);
  va_end(vl);
  return e;
}

inline Error Error::MkV(const char* fmt, va_list vl) {
  return Mk(FmtV(fmt, vl));
}

inline Error Error::MkSys(int err) {
  auto i = static_cast<int>(err);
  auto e = BitCast<std::uint32_t>(i);
  return Mk(ErrnoCategory(), e);
}

inline Error::~Error() { Clear(); }

}  // namespace base

#endif  // _BASE_ERROR_INL_H_
