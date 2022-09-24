#ifndef _BASE_RESULT_INL_H_
#define _BASE_RESULT_INL_H_

namespace base {

template <typename O>
inline Error* Result<O>::Eptr() {
  DCHECK(IsFailed());
  return &(NO_EXCEPT(std::get<Error>(v_)));
}

template <typename O>
inline const Error* Result<O>::Eptr() const {
  DCHECK(IsFailed());
  return &(NO_EXCEPT(std::get<Error>(v_)));
}

template <typename O>
inline O* Result<O>::Optr() {
  DCHECK(IsSatisfied());
  return &(NO_EXCEPT(std::get<O>(v_)));
}

template <typename O>
inline const O* Result<O>::Optr() const {
  DCHECK(IsSatisfied());
  return &(NO_EXCEPT(std::get<O>(v_)));
}

template <typename O>
inline Error Result<O>::PassError() {
  auto tmp(std::move(*Eptr()));
  v_ = Null{};
  return tmp;
}

template <typename O>
inline O Result<O>::PassResult() {
  auto tmp(std::move(*Optr()));
  v_ = Null{};
  return tmp;
}

}  // namespace base

#endif  // _BASE_RESULT_INL_H_
