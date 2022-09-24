#ifndef _QUAKE_IR_TYPE_H_
#define _QUAKE_IR_TYPE_H_
#include "base.h"
#include "qjs.h"

namespace quake {
namespace ir {

class Node;

// Type of IR node, implicit represented by the IR node and cached by the
// SSAVar
class Type {
 public:
  enum Tag {
    kNone = -1,     // no type can be related to
    kInfer = -2,    // will have type, but require type inferrence
    kUnknown = -3,  // unknown type from QuickJS, which is not supported in JIT

    // ------------------------------------------------------------------------
    // Internal type. Just assume type to be a lattice, these type shows up in
    // the middle (none-leaf) inside of lattice's haas graph
    kBox = 0,

    // ------------------------------------------------------------------------
    // Primitive types
    kInt = 10,
    kFloat64,
    kBool,
    kNull,
    kUndefined,
    kI64,  // int64, used mostly for internal usage of machine qword

    // ------------------------------------------------------------------------
    // Heap object, ie require RC management
    kString = 30,
    kSymbol,
    kObject,
    kModule,
    kJSFunction,

    kSizeOfTag
  };

  enum {
    kHeapObject = kString,
  };

  static bool IsHeapTag(int t) { return t >= kHeapObject; }
  static bool IsPrimitiveTag(int t) { return t >= 0 && t < kHeapObject; }

  static Type kNoneType;
  static Type kInferType;
  static Type kUnknownType;

 public:
  explicit Type(Tag t) : tag_(t) {}
  Type() = default;

  // Convert a QJS tag into Type object
  static Type FromQJS(int);

 public:
  Tag tag() const { return tag_; }

  bool IsNone() const { return tag_ == kNone; }
  bool IsInfer() const { return tag_ == kInfer; }
  bool IsUnknown() const { return tag_ == kUnknown; }
  bool IsInvalid() const {
    return tag_ == kNone || tag_ == kInfer || tag_ == kUnknown;
  }
  bool IsValid() const { return !IsInvalid(); }

 public:
  bool IsInt() const { return tag_ == kInt; }
  bool IsReal() const { return tag_ == kFloat64; }
  bool IsI64() const { return tag_ == kI64; }
  bool IsNumber() const { return IsInt() || IsI64() || IsReal(); }

  bool IsNull() const { return tag_ == kNull; }
  bool IsUndefined() const { return tag_ == kUndefined; }
  bool IsString() const { return tag_ == kString; }
  bool IsSymbol() const { return tag_ == kSymbol; }
  bool IsStringLike() const { return IsString() || IsSymbol(); }
  bool IsObject() const { return tag_ == kObject; }
  bool IsModule() const { return tag_ == kModule; }
  bool IsJSFunction() const { return tag_ == kJSFunction; }

  bool IsHeap() const { return IsHeapTag(tag_); }
  bool IsPrimitive() const { return IsPrimitiveTag(tag_); }

  // Boxed type, we don't know
  bool IsBox() const { return tag_ == kBox; }

 private:
  Tag tag_;
};

// Map qjs::JVal's type to Type
inline Type::Tag JValT2T(int jval_tag) {
  switch (jval_tag) {
    case JVal::kBigDecimal:
    case JVal::kBigInt:
    case JVal::kBigFloat:
      return Type::kUnknown;
    case JVal::kSymbol:
      return Type::kSymbol;
    case JVal::kString:
      return Type::kString;
    case JVal::kModule:
      return Type::kModule;
    case JVal::kFunctionBytecode:
      return Type::kJSFunction;
    case JVal::kObject:
      return Type::kObject;

    case JVal::kInt:
      return Type::kInt;
    case JVal::kBool:
      return Type::kBool;
    case JVal::kNull:
      return Type::kNull;
    case JVal::kUndefined:
      return Type::kUndefined;
    case JVal::kFloat64:
      return Type::kFloat64;

    case JVal::kUninitialized:
    case JVal::kCatchOffset:
    case JVal::kException:
      return Type::kUnknown;

    default:
      return Type::kNone;
  }
}

inline Type JValT2Type(int jval_tag) { return Type{JValT2T(jval_tag)}; }

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_TYPE_H_
