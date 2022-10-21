#ifndef _QUAKE_LIR_HTYPE_H_
#define _QUAKE_LIR_HTYPE_H_
#include "base.h"
#include "qjs.h"

namespace quake {
namespace lir {

// LIR's type. In general LIR is strictly typped but never been
// explicitly make so. The HOpcode opcode itself typically already shows
// you what type it is indeed
//
// Notes the HType is a strict lattice, the bottom is kNone, and
// the top is kBits
enum class HType : std::uint8_t {
  // --------------------------------------------------------------------------
  kBits,  // no type information, treated it as binary, match against
          // all type ie it is at the top of the lattice

  // --------------------------------------------------------------------------
  kInt,    // int32 value
  kI64,    // int64 value
  kFloat,  // floating point type
  kPtr,    // pointer value, notes, unknown to us wether it is GCed or not
  kGC,     // gc pointer value, notes it is GC pointer, so we can do ref
           // count

  // --------------------------------------------------------------------------
  kBox,     // real box value, currently it takes 16 bytes
  kBoxRef,  // box value's ref, ie a pointer to a box value
  kNone     // no type, or irrevelant
};

inline RegisterType HTypeToRegisterType(HType t) {
  return t == HType::kFloat ? RegisterType::kFloat : RegisterType::kGeneral;
}

inline HType JValT2HType(int x) {
  switch (x) {
    default:
      if (x < 0) {
        return HType::kGC;  // must be GCed object
      }
      return HType::kNone;

    case JVal::kInt:
    case JVal::kBool:
      return HType::kInt;

    case JVal::kFloat64:
      return HType::kFloat;

    case JVal::kNull:
    case JVal::kUndefined:
    case JVal::kUninitialized:
    case JVal::kCatchOffset:
    case JVal::kException:
      return HType::kNone;
  }
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_HTYPE_H_
