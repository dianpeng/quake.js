#ifndef _BASE_ERROR_H_
#define _BASE_ERROR_H_
#include "bits.h"
#include "check.h"
#include "common.h"

namespace base {

// Similar as std::error_code's ErrorCategory plugin, each module can use this
// mechanism to extend the error code category using static global objects.
class ErrorCategory {
 public:
  // Return a human readable name for this error category
  virtual const char* GetName() const = 0;

  // Given a specific error category code, returns a readable message for
  // dianostic purpose
  virtual std::string GetInformation(std::uint32_t) const = 0;

  virtual ~ErrorCategory() {}
};

// The error object represent a general error and it stores the data as a
// sequences of characters to avoid bloat of the |Error| itself.
//
// The |Error| stores the following three levels of information :
//
//   1. Category
//   2. Reason
//   3. Descriptive message
//
// Notes each module should define its error factory method to avoid the
// tedious pointer of ErrorCategory pass in. This also makes the code much
// easier to maintain.
//
// The only difference between std::error_code and Error is that we support
// a customized message appeneded in. This is really helpful in context like
// parsing failure, where you typically want to report context information,
// ie where the grammar failed.
class Error {
 public:
  // Some built-in categories
  static const ErrorCategory* GeneralCategory();
  static const ErrorCategory* ErrnoCategory();

  // All the builtin reasons that is defined
  enum { kReasonUnspecified = 1, kReasonInternal };

 public:
  static constexpr std::uint32_t kNoReason = 0;

  static inline Error Mk(const ErrorCategory*, std::uint32_t);
  static inline Error Mk(const ErrorCategory*, std::uint32_t,
                         const std::string&);
  static inline Error Mk(const ErrorCategory*, std::uint32_t, std::string&&);

  static inline Error Mk(const ErrorCategory*, std::uint32_t, const char*, ...);
  static inline Error MkV(const ErrorCategory*, std::uint32_t, const char*,
                          va_list);

  static inline Error Mk(const std::string&);
  static inline Error Mk(std::string&&);
  static inline Error Mk(const char*, ...);
  static inline Error MkV(const char*, va_list);

  static Error MkGeneral(std::uint32_t reason) {
    return Mk(GeneralCategory(), reason);
  }
  static Error MkGeneral(std::uint32_t reason, const std::string& msg) {
    return Mk(GeneralCategory(), reason, msg);
  }
  static Error MkGeneral(std::uint32_t reason, std::string&& msg) {
    return Mk(GeneralCategory(), reason, std::move(msg));
  }

  // This is useful for converting an errono into a system error.
  //
  // Notes(dpeng): Why not errno_t ?
  //   Sadly, errno_t is never a thing and the only documented standard is C11,
  //   which I find out in practise most vendor doesn't define it. So I just
  //   use the int as a normal way to accpet the errno. Another option is to
  //   make it as template function and internally cast the type to std::uint32
  //   Anyway, this is where we end up with. Notes, the errno |can| hold
  //   negative number
  static Error MkSys(int);

 public:
  inline Error();
  inline Error(const ErrorCategory*, std::uint32_t);
  inline Error(const ErrorCategory*, std::uint32_t, const std::string&);
  inline Error(const ErrorCategory*, std::uint32_t, std::string&&);

  inline Error(const Error&);
  inline Error(Error&&);
  inline Error& operator=(const Error&);
  inline Error& operator=(Error&&);
  inline ~Error();

 public:
  // Get the message component, if applicable. The Error can have no message
  // component if the creation doesn't bundle a message.
  inline std::string GetMessage() const;

  // Steal the message, ie move. This operation turns Error into empty again
  inline std::string PassMessage();

  // Get the display information. It always return something for human
  // consumption.
  inline std::string GetDisplay() const;

  // Return a safe message that can be printted. A safe message is considered to
  // be valided to return to unknown user which doesn't leak information. We can
  // do encoding or just return some vague words
  inline std::string GetSafeMessage() const;

  // Get the error description as valid JSON. It will do proper JSON escape if
  // needed. The output format is as following :
  // {
  //   "category" : ...,
  //   "reason"   : ...,
  //   "message"  : "..."
  // }
  std::string GetJSON() const;

  // Component to get category or reason
  const ErrorCategory* category() const { return LoadCategory(); }
  std::uint32_t reason() const { return LoadReason(); }
  std::string information() const {
    return category()->GetInformation(reason());
  }

  // Check whether the Error object has an attached message or not.
  bool Has() const { return !storage_.IsEmpty(); }
  bool HasMessage() const { return IsOOL(); }
  bool IsSuccess() const { return !Has(); }
  operator bool() const { return Has(); }

  // Clear the Error if needed. After the |Clear|, the error becomes empty, ie
  // no error is specified.
  void Clear() { storage_.Clear(); }

  // Typical swap operation. Used to implement operator =
  void Swap(Error* e) { storage_.Swap(&e->storage_); }

 private:
  // OOLPayload storage, used to store OOL information when user provide message
  // field, ie a std::string. Notes the OOLPayload is still constructed in place
  struct OOLPayload {
    const ErrorCategory* category;
    std::string message;

    void Swap(OOLPayload* that) {
      std::swap(category, that->category);
      std::swap(message, that->message);
    }

    OOLPayload() : category(nullptr), message() {}

    OOLPayload(const ErrorCategory* ec, const std::string& m)
        : category(ec), message(m) {}

    OOLPayload(const ErrorCategory* ec, std::string&& m)
        : category(ec), message(std::move(m)) {}

    OOLPayload(const OOLPayload&) = default;

    OOLPayload(OOLPayload&&) = default;
  };

  // The storage of error can be inlined, which means all the data requires no
  // extra heap allocation if user only specify category and error code. If
  // additional message is provided, then Storage will uses OOL(out of line)
  // storage which makes the ptr' ool field works. The indication of whether it
  // is OOL or not, is stored currently along with boolean flag. Notes, we don't
  // prefer a tagged pointer method here since we have 4 extra bytes as waste
  // for now unless value field can uses more than 4 bytes which is not likely
  struct Storage {
    union {
      const ErrorCategory* category;
      void* raw;
      byte_t ool[sizeof(OOLPayload)];
    } info;

    std::uint32_t reason;
    bool is_ool;

    OOLPayload* OOL() { return reinterpret_cast<OOLPayload*>(&info.ool); }
    const OOLPayload* OOL() const {
      return reinterpret_cast<const OOLPayload*>(&info.ool);
    }

    Storage() : info(), reason(kNoReason), is_ool(false) { info.raw = nullptr; }

    Storage(const ErrorCategory* category, std::uint32_t r, std::string&& m)
        : info(), reason(r), is_ool(true) {
      CallCtor<OOLPayload>(&info.ool, category, std::move(m));
    }

    Storage(const ErrorCategory* category, std::uint32_t r,
            const std::string& m)
        : info(), reason(r), is_ool(true) {
      CallCtor<OOLPayload>(&info.ool, category, m);
    }

    Storage(const ErrorCategory* category, std::uint32_t r)
        : info(), reason(r), is_ool(false) {
      info.category = category;
    }

    inline Storage(Storage&&);
    inline Storage(const Storage&);

    inline bool IsEmpty() const { return !is_ool && info.raw == nullptr; }

    inline void Clear();
    inline bool TryClearOOL();
    inline void Swap(Storage*);
    inline void MarkEmpty();

    ~Storage() { TryClearOOL(); }
  };

  // Load
  inline std::uint32_t LoadReason() const;
  inline const ErrorCategory* LoadCategory() const;

  // Misc
  bool IsOOL() const { return storage_.is_ool; }

  const OOLPayload* OOL() const { return storage_.OOL(); }
  OOLPayload* OOL() { return storage_.OOL(); }

  void MarkEmpty() { storage_.MarkEmpty(); }

 private:
  Storage storage_;
};

}  // namespace base

#include "error-inl.h"

#endif  // _BASE_ERROR_H_
