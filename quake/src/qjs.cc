#include "qjs.h"

namespace quake {

#ifdef JS_NAN_BOXING

const std::int64_t JVal::kBitTrue = ToBit(JS_TRUE);
const std::int64_t JVal::kBitFalse = ToBit(JS_FALSE);
const std::int64_t JVal::kBitNull = ToBit(JS_NULL);
const std::int64_t JVal::kBitUndefined = ToBit(JS_UNDEFINED);

#endif  // JS_NAN_BOXING

}  // namespace quake
