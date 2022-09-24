#ifndef _BASE_TRAIT_H_
#define _BASE_TRAIT_H_
#include <cstdint>
#include <iterator>
#include <string>
#include <type_traits>

// Here we define some type_traits that is not standard comaptible but we need
// to use them somehow. This file needs to be included into the "common.h" and
// should not be directly used.
#define BASE_SINT_TYPE_LIST(__) \
  __(std::int8_t)              \
  __(std::int16_t)             \
  __(std::int32_t)             \
  __(std::int64_t)

#define BASE_UINT_TYPE_LIST(__) \
  __(std::uint8_t)             \
  __(std::uint16_t)            \
  __(std::uint32_t)            \
  __(std::uint64_t)

#define BASE_INT_TYPE_LIST(__) \
  BASE_SINT_TYPE_LIST(__)      \
  BASE_UINT_TYPE_LIST(__)

#define BASE_STR_TYPE_LIST(__) \
  __(const char*)             \
  __(std::string)

// Concept helpers. The concept allow us to declare a concept, ie a certain
// type |MUST| have certain function/method. The testing result can be obtained
// by concept name. The concept is comming into C++ standard in C++20/23.
//
// The usage is as following, notes the concept definition goes into the current
// namespace you call DEFINE_CONCEPT.
//
//   DEFINE_CONCEPT(MyConcept, void (_::*)(int, double), &_::Foo, HasFoo);
//
// The _ inside of the DEFINE_CONCEPT is the placeholder to represent the object
// you gonna test against with.
//
// To check whether a certain type has function Foo with type related above,
// just do HasFoo<T>::kValue gonna return you yes or no.

#define DEFINE_CONCEPT(NAME, SIG, MF)                                       \
  template <typename __T>                                                   \
  struct NAME {                                                             \
   private:                                                                 \
    template <typename _, SIG>                                              \
    struct SFINAE {};                                                       \
    template <typename _>                                                   \
    static char Check(SFINAE<_, MF>*);                                      \
    template <typename _>                                                   \
    static int Check(...);                                                  \
                                                                            \
   public:                                                                  \
    static constexpr bool kValue = (sizeof(Check<__T>(0)) == sizeof(char)); \
  }

namespace base {
// Our traits' true/false type, instead of using |value| as variable name, we
// use kValue to make our code consistent, well best efforts indeed :)
struct TrueType {
  static constexpr bool kValue = true;
};

struct FalseType {
  static constexpr bool kValue = false;
};

namespace _ {

template <typename T>
struct IsIntImpl {
  static constexpr bool kValue = false;
};

#define __(T)                            \
  template <>                            \
  struct IsIntImpl<T> {                  \
    static constexpr bool kValue = true; \
  };
BASE_INT_TYPE_LIST(__)
#undef __  // __

template <typename T>
struct IsUIntImpl {
  static constexpr bool kValue = false;
};

#define __(T)                            \
  template <>                            \
  struct IsUIntImpl<T> {                 \
    static constexpr bool kValue = true; \
  };
BASE_UINT_TYPE_LIST(__)
#undef __  // __

template <typename T>
struct IsSIntImpl {
  static constexpr bool kValue = false;
};

#define __(T)                            \
  template <>                            \
  struct IsSIntImpl<T> {                 \
    static constexpr bool kValue = true; \
  };
BASE_SINT_TYPE_LIST(__)
#undef __  // __

template <typename T>
struct IsBooleanImpl {
  static constexpr bool kValue = false;
};

template <>
struct IsBooleanImpl<bool> {
  static constexpr bool kValue = true;
};

}  // namespace _

template <typename T>
struct IsInteger {
  static constexpr bool kValue =
      _::IsIntImpl<typename std::remove_cv<T>::type>::kValue;
};

template <typename T>
struct IsSignedInteger {
  static constexpr bool kValue =
      _::IsSIntImpl<typename std::remove_cv<T>::type>::kValue;
};

template <typename T>
struct IsUnsignedInteger {
  static constexpr bool kValue =
      _::IsUIntImpl<typename std::remove_cv<T>::type>::kValue;
};

template <typename T>
struct IsBoolean {
  static constexpr bool kValue =
      _::IsBooleanImpl<typename std::remove_cv<T>::type>::kValue;
};

template <typename T>
struct IsString {
  static constexpr bool kValue =
      std::is_same<std::string, typename std::remove_cv<T>::type>::value;
};

// --------------------------------------------------------------------
//  The following traits extraction code is used for handling following
//  functions, ie we only care function that can be invoked without
//  receiever type, ie not member function. For member function, user
//  can use std::bind to convert them into std::function to work with
//  the following traits
//
//    1. functor, ie operator ()
//    2. free global function
//    3. std::function<...>, essentially it is an functor though
//
// --------------------------------------------------------------------

namespace _ {

template <typename SIG, bool Callable>
struct ExtractFunctionSigInternal : FalseType {};

// Selector to find out whether the call is a operator (), ie functor
template <typename F, typename SFINE = void>
struct IsFunctorObject : FalseType {};

// Specialization for operator (), ie functor
template <typename Callable>
struct IsFunctorObject<Callable, std::void_t<decltype(&Callable::operator())>>
    : TrueType {};

// Extractor for functor/functor-like/member-function signature and various
// component
template <typename Callable,
          typename Signature = decltype(&Callable::operator())>
struct ExtractFunctorSig {};

// Partial specialization for member function, none-const version
template <typename Callable, typename R, typename... ARGS>
struct ExtractFunctorSig<Callable, R (Callable::*)(ARGS...)> {
  using ReturnType = R;
  using Signature = R(ARGS...);

  using WrapperSignature = Signature;
  using WrapperType = std::function<WrapperSignature>;
};

// Partial specialization for member function, const version
template <typename Callable, typename R, typename... ARGS>
struct ExtractFunctorSig<Callable, R (Callable::*)(ARGS...) const> {
  using ReturnType = R;
  using Signature = R(ARGS...) const;

  using WrapperSignature = R(ARGS...);
  using WrapperType = std::function<WrapperSignature>;
};

// Concrete implementation of extract function signature trait
//
// Global/Free functions
template <typename R, typename... ARGS>
struct ExtractFunctionSigInternal<R (&)(ARGS...), false> : TrueType {
  using ReturnType = R;
  using Signature = R(ARGS...);

  using WrapperSignature = Signature;
  using WrapperType = std::function<WrapperSignature>;
};

template <typename R, typename... ARGS>
struct ExtractFunctionSigInternal<R (*)(ARGS...), false> : TrueType {
  using ReturnType = R;
  using Signature = R(ARGS...);

  using WrapperSignature = Signature;
  using WrapperType = std::function<WrapperSignature>;
};

// Functor/MemberFunction traits, utilize ExtractFunctorSig for traits
// reasoning.
template <typename Callable>
struct ExtractFunctionSigInternal<Callable, true> : TrueType {
  using ReturnType = typename ExtractFunctorSig<Callable>::ReturnType;
  using Signature = typename ExtractFunctorSig<Callable>::Signature;

  using WrapperSignature =
      typename ExtractFunctorSig<Callable>::WrapperSignature;
  using WrapperType = typename ExtractFunctorSig<Callable>::WrapperType;
};

// -----------------------------------------------------------
// Unary function
// -----------------------------------------------------------
template <typename SIG, bool IsFunctor, typename DefaultType>
struct IsUnaryFunctionImpl : FalseType {
  using ArgType = DefaultType;
};

template <typename F, typename R, typename _>
struct IsUnaryFunctionImpl<R (*)(F), false, _> : TrueType {
  using ArgType = F;
};

template <typename F, typename R, typename _>
struct IsUnaryFunctionImpl<R (&)(F), false, _> : TrueType {
  using ArgType = F;
};

template <typename F, typename R, typename O, typename _>
struct IsUnaryFunctionImpl<R (O::*)(F), false, _> : TrueType {
  using ArgType = F;
};

template <typename O, typename Default, typename _ = decltype(&O::operator())>
struct UnaryFunctionFunctorImpl : FalseType {
  using ArgType = Default;
};

template <typename O, typename R, typename F, typename _>
struct UnaryFunctionFunctorImpl<O, _, R (O::*)(F)> : TrueType {
  using ArgType = F;
};

template <typename O, typename R, typename F, typename _>
struct UnaryFunctionFunctorImpl<O, _, R (O::*)(F) const> : TrueType {
  using ArgType = F;
};

// A unary function is rarely carry a void argument which result in a 0ary
// function actually
template <typename SIG, typename Default>
struct IsUnaryFunctionImpl<SIG, true, Default> : TrueType {
  using ArgType = typename UnaryFunctionFunctorImpl<SIG, Default>::ArgType;
};

// -----------------------------------------------------------
// Binary function
// -----------------------------------------------------------
template <typename SIG, bool IsFunctor, typename Default>
struct IsBinaryFunctionImpl : FalseType {
  using Arg1Type = Default;
  using Arg2Type = Default;
};

template <typename A1, typename A2, typename R, typename _>
struct IsBinaryFunctionImpl<R (*)(A1, A2), false, _> : TrueType {
  using Arg1Type = A1;
  using Arg2Type = A2;
};

template <typename A1, typename A2, typename R, typename _>
struct IsBinaryFunctionImpl<R (&)(A1, A2), false, _> : TrueType {
  using Arg1Type = A1;
  using Arg2Type = A2;
};

template <typename A1, typename A2, typename R, typename O, typename _>
struct IsBinaryFunctionImpl<R (O::*)(A1, A2), false, _> : TrueType {
  using Arg1Type = A1;
  using Arg2Type = A2;
};

template <typename O, typename Default, typename _ = decltype(&O::operator())>
struct BinaryFunctionFunctorImpl : FalseType {
  using Arg1Type = Default;
  using Arg2Type = Default;
};

template <typename A1, typename A2, typename O, typename R, typename _>
struct BinaryFunctionFunctorImpl<O, _, R (O::*)(A1, A2)> : TrueType {
  using Arg1Type = A1;
  using Arg2Type = A2;
};

template <typename A1, typename A2, typename O, typename R, typename _>
struct BinaryFunctionFunctorImpl<O, _, R (O::*)(A1, A2) const> : TrueType {
  using Arg1Type = A1;
  using Arg2Type = A2;
};

// A unary function is rarely carry a void argument which result in a 0ary
// function actually
template <typename SIG, typename Default>
struct IsBinaryFunctionImpl<SIG, true, Default> : TrueType {
  using Arg1Type = typename BinaryFunctionFunctorImpl<SIG, Default>::Arg1Type;
  using Arg2Type = typename BinaryFunctionFunctorImpl<SIG, Default>::Arg2Type;
};

}  // namespace _

template <typename _, template <typename...> class T>
struct IsSpecialization : FalseType {};

template <template <typename...> class T, typename... Args>
struct IsSpecialization<T<Args...>, T> : TrueType {};

// Used for BASE related iterator adaption
struct IteratorTag {};

template <typename Itr, typename _ = void>
struct IsIterator {
  static constexpr bool kValue = std::is_base_of<IteratorTag, Itr>::value;
};

template <typename Itr>
struct IsIterator<
    Itr, std::enable_if_t<!std::is_same<
             typename std::iterator_traits<Itr>::value_type, void>::value>>
    : TrueType {};

template <typename SIG, typename DefaultType = void>
using IsUnaryFunction =
    _::IsUnaryFunctionImpl<SIG, _::IsFunctorObject<SIG>::kValue, DefaultType>;

template <typename SIG, typename DefaultType = void>
using IsBinaryFunction =
    _::IsBinaryFunctionImpl<SIG, _::IsFunctorObject<SIG>::kValue, DefaultType>;

// Specialization for type which is |const char*|
template <>
struct IsString<const char*> {
  static constexpr bool kValue = true;
};

template <typename SIG>
struct ExtractFunctionSig {
  static_assert(!std::is_member_function_pointer<SIG>::value,
                "cannot work with member function");

  using ImplType =
      _::ExtractFunctionSigInternal<SIG, _::IsFunctorObject<SIG>::kValue>;

  using ReturnType = typename ImplType::ReturnType;
  using Signature = typename ImplType::Signature;
  using WrapperSignature = typename ImplType::WrapperSignature;
  using WrapperType = typename ImplType::WrapperType;

  static constexpr bool kValue = ImplType::kValue;
};

template <typename SIG>
using ReturnType = typename ExtractFunctionSig<SIG>::ReturnType;

template <typename SIG>
struct IsCallable {
  static constexpr bool kValue =
      !std::is_member_function_pointer<SIG>::value &&  // not member function
      ExtractFunctionSig<SIG>::kValue;                 // recognizable function
};

template <typename SIG>
struct IsVoidCallable {
  static constexpr bool kIsCallable = IsCallable<SIG>::kValue;
  static constexpr bool kValue =
      kIsCallable && std::is_void<ReturnType<SIG>>::value;
};

}  // namespace base
#endif  // _BASE_TRAIT_H_
