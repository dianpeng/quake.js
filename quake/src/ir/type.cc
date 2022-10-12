#include "ir/type.h"
#include "ir/node.h"

#include "quick-js-inc.h"

namespace quake {
namespace ir {

Type Type::kNoneType{kNone};
Type Type::kInferType{kInfer};
Type Type::kUnknownType{kUnknown};

Type Type::FromQJS(int tag) {
  switch (tag) {
    case JS_TAG_SYMBOL:
      return Type{kSymbol};
    case JS_TAG_STRING:
      return Type{kString};
    case JS_TAG_MODULE:
      return Type{kModule};
    case JS_TAG_OBJECT:
      return Type{kObject};
    case JS_TAG_INT:
      return Type{kInt};
    case JS_TAG_BOOL:
      return Type{kBool};
    case JS_TAG_NULL:
      return Type{kNull};
    case JS_TAG_UNDEFINED:
      return Type{kUndefined};
    case JS_TAG_UNINITIALIZED:
      return Type{kUninitialized};
    case JS_TAG_FLOAT64:
      return Type{kFloat64};

    default:
      break;
  }

  return Type{kUnknown};
}

}  // namespace ir
}  // namespace quake
