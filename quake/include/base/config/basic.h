#ifndef _BASE_CONFIG_BASIC_H_
#define _BASE_CONFIG_BASIC_H_

#include <base/common.h>
#include <base/error.h>

namespace base {
namespace config {

#define CONFIG_ERROR_LIST(__)                                 \
  __(kErrorReceiver, "receiver error")                        \
  __(kErrorSetType, "attribute set type error")               \
  __(kErrorVerify, "value verification error")                \
  __(kErrorGetType, "attribute get type error")               \
  /** parsing/lexing error **/                                \
  __(kErrorUnknownToken, "unknown token/character sequences") \
  __(kErrorInvalidAttribute, "invalid attribute")             \
  __(kErrorInvalidEscape, "invalid string escape")            \
  __(kErrorInvalidNumber, "invalid number")                   \
  __(kErrorInvalidUtf8, "invalid utf8 encoding")              \
  __(kErrorInvalidString, "invalid string")                   \
  __(kErrorInvalidComment, "invalid comment")                 \
  __(kErrorInvalidGrammar, "invalid grammar")                 \
  __(kErrorInvalidInstruction, "invalid instruction")         \
  __(kErrorNotAvailable, "feature not available")             \
  __(kErrorTooMuchNesting, "too much nesting")                \
  __(kErrorExpectToken, "expect token")                       \
  __(kErrorUnexpectedToken, "unexpected token")               \
  __(kErrorEvalDisallowed, "eval is disallowed")              \
  /** unmarshel **/                                           \
  __(kErrorUnmarshelFieldNotFound, "field not found")

enum ConfigError {
// clang-format off
#define __(A, B) A,
  CONFIG_ERROR_LIST(__)
#undef __  // __
  kSizeOfError
  // clang-format on
};

Error Err(ConfigError);
Error Err(ConfigError, const char*, ...);
Error ErrV(ConfigError, const char*, va_list);

}  // namespace config
}  // namespace base

#endif  // _BASE_CONFIG_BASIC_H_
