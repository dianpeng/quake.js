#ifndef _BASE_RESULT_H_
#define _BASE_RESULT_H_

#include "common.h"
#include "error.h"

// Result, which is similar to Rust's result or v8's Maybe, is a POD like object
// to flexiblly represent an operation's result that may fail. The difference is
// Result |can| carry out a |Error| component which is used to represent the
// reason why this result failed. In many other cases, Result behahves almost
// the same as std::optional.
//
// Notes, we still use std::optional in many places, espcailly some primitive
// functions. When the reason for error is not relevent, using std::optional
// still make sense

namespace base {

// Result has states. It can be in following 3 states and in general Result is
// immutable.
//
//   States of |Result| :
//
//     1. Empty, which means no object or error has been related to it
//     2. Satisfied, which means the object contains a valid result/output
//     3. Failed, which means there's an error W.R.T the operation
//
template <typename O>
class Result {
 public:
  using ValueType = O;

  // Construct an empty |Result| object.
  Result() : v_() {}

  // Notes(dpeng): We do allow implicit conversion from O to Result object to
  //   save us from several typing in code.
  Result(const O& o) : v_(o) {}
  Result(O&& o) : v_(std::move(o)) {}

  Result(Error&& e) : v_(std::move(e)) {}
  Result(const Error& e) : v_(e) {}

  Result(const ErrorCategory* ec, std::uint32_t reason)
      : v_(Error{ec, reason}) {}

  Result(const ErrorCategory* ec, std::uint32_t reason, const std::string& m)
      : v_(Error{ec, reason, m}) {}

  Result(const ErrorCategory* ec, std::uint32_t reason, std::string&& m)
      : v_(Error{ec, reason, std::move(m)}) {}

  template <typename U,
            std::enable_if_t<std::is_convertible<U, O>::value, int> _ = 0>
  Result(Result<U>&& o)
      : Result(o ? Result<O>{o.PassResult()} : Result<O>{o.PassError()}) {}

 public:
  Result(const Result&) = default;
  Result(Result&&) = default;
  Result& operator=(const Result&) = default;
  Result& operator=(Result&&) = default;

 public:
  enum { kEmpty = 0, kSatisfied, kFailed };

  int state() const {
    // Sadly, the std::variant can fail when any type throw exception during the
    // move or copy operation. This will make index() call return -1 or make
    // function |valueness_by_exception| returns true. It is not consistent any
    // more though we never use exception in our code base. For sanity, we do
    // an assertion here
    auto i = v_.index();
    DCHECK(i >= 0);
    return i;
  }

  bool IsEmpty() const { return state() == kEmpty; }
  bool IsSatisfied() const { return state() == kSatisfied; }
  bool IsFailed() const { return state() == kFailed; }
  operator bool() const { return IsSatisfied(); }

 public:
  // Accessor for underlying O(object). Used when user have already test it
  // against whether it is a Satisfied object or not.
  O* operator->() { return Optr(); }
  const O* operator->() const { return Optr(); }

  O& operator*() { return *Optr(); }
  const O& operator*() const { return *Optr(); }

 public:
  // Steal the Result/Error from the Result object. After the stleaing, the
  // object becomes kEmpty again.
  inline O PassResult();
  inline Error PassError();

  // Try to get the Result/Error object. DCHECK failed when Result object is
  // not in the corresponding state.
  const O& GetResult() const { return *Optr(); }
  O& GetResult() { return *Optr(); }
  const Error& GetError() const { return *Eptr(); }
  Error& GetError() { return *Eptr(); }

  // Try to get the Result/Error object, if state unstaitisfied, then returns
  // nullptr instead
  const O* MaybeResult() const {
    return IsSatisfied() ? &(NO_EXCEPT(std::get<O>(v_))) : nullptr;
  }
  O* MaybeResult() {
    return IsSatisfied() ? &(NO_EXCEPT(std::get<O>(v_))) : nullptr;
  }
  Error* MaybeError() {
    return IsFailed() ? &(NO_EXCEPT(std::get<Error>(v_))) : nullptr;
  }
  const Error* MaybeError() const {
    return IsFailed() ? &(NO_EXCEPT(std::get<Error>(v_))) : nullptr;
  }

 private:
  inline O* Optr();
  inline const O* Optr() const;
  inline Error* Eptr();
  inline const Error* Eptr() const;

 private:
  struct Null {};
  using Storage = std::variant<Null, O, Error>;
  Storage v_;
};

// Specialization for Result<Void>
template <>
class Result<void> {
 public:
  using ValueType = void;

  Result() : v_() {}
  Result(Error&& e) : v_(std::move(e)) {}
  Result(const Error& e) : v_(e) {}

  Result(const Result&) = default;
  Result(Result&&) = default;
  Result& operator=(const Result&) = default;
  Result& operator=(Result&&) = default;

  bool IsEmpty() const { return false; }

  bool IsSatisfied() const { return static_cast<bool>(!v_); }

  bool IsFailed() const { return static_cast<bool>(v_); }
  operator bool() const { return IsSatisfied(); }

 public:
  Error PassError() { return Pass(&v_); }
  const Error& GetError() const { return v_; }
  Error& GetError() { return v_; }
  Error* MaybeError() { return v_ ? &v_ : nullptr; }
  const Error* MaybeError() const { return v_ ? &v_ : nullptr; }

 private:
  Error v_;
};

template <typename T>
T Pass(Result<T>& r) {
  return r.PassResult();
}

template <typename T>
T Pass(Result<T>* r) {
  return r->PassResult();
}

// The C++ template deduction rule will have following effect :
//
//   When Okay is called with a LVALUE, then captures T&
//   When Okay is called with a RVALUE, then cpatures T&&
//
// std::remove_reference will take care of everything here and deduct to
// valid result type
template <typename T, typename _R = typename std::remove_reference<T>::type>
Result<_R> Okay(T&& v) {
  return Result<_R>{std::forward<T>(v)};
}

namespace _ {

template <typename T>
struct IsResultImpl : FalseType {};

template <typename T>
struct IsResultImpl<Result<T>> : TrueType {};

}  // namespace _

template <typename O>
struct IsResult : _::IsResultImpl<std::decay_t<O>> {};

}  // namespace base

#include "result-inl.h"

#endif  // _BASE_RESULT_H_
