#include "quake-rt.h"
#include "quake.h"
#include "deopt.h"

#include <cmath>

namespace quake {

const char* QuakeJit::GetUnreachableCodeName(int x) {
  switch (x) {
    case QuakeJit::kUnreachable:
      return "unreachable";
    case QuakeJit::kUnreachableEndOfBody:
      return "end(body)";
    case QuakeJit::kUnreachableEndOfPrologue:
      return "end(prologue)";
    case QuakeJit::kUnreachableEndOfEpilogue:
      return "end(epilogue)";
    case QuakeJit::kUnreachableEndOfDeoptimization:
      return "end(deoptimization)";
    case QuakeJit::kUnreachableEndOfCastF64ToB32:
      return "end(f64_to_b32)";
    case QuakeJit::kUnreachableEndOfFMod:
      return "end(fmod)";
    default:
      return "unknown";
  }
}

void QuakeJit::Unreachable(int x) {
  const char* msg = GetUnreachableCodeName(x);

  // there may not be any frame since the function is been invoked from JIT
  // code, we may need to generate some DWARF/EFI to allow stack unwind to
  // work.
  LOG(FATAL) << "(JIT-unreachable): " << msg;
}

const void* QuakeJit::MapRCall(BuiltinFunction bf) {
  switch (bf) {
    case BuiltinFunction::kFMod:
      return reinterpret_cast<const void*>(&RCallFMod);

    case BuiltinFunction::kFPow:
      return reinterpret_cast<const void*>(&RCallFPow);

    case BuiltinFunction::kGCFree:
      return reinterpret_cast<const void*>(&RCallGCFree);

    case BuiltinFunction::kGlobalGet:
      return reinterpret_cast<const void*>(&RCallGlobalGet);

    case BuiltinFunction::kGlobalPut:
      return reinterpret_cast<const void*>(&RCallGlobalPut);

    case BuiltinFunction::kObjectFieldGetAtom:
      return reinterpret_cast<const void*>(&RCallObjectFieldGetAtom);

    case BuiltinFunction::kObjectFieldPutAtom:
      return reinterpret_cast<const void*>(&RCallObjectFieldPutAtom);

    case BuiltinFunction::kObjectFieldDefineAtom:
      return reinterpret_cast<const void*>(&RCallObjectFieldDefineAtom);

    case BuiltinFunction::kObjectFieldNameSet:
      return reinterpret_cast<const void*>(&RCallObjectSetName);

    case BuiltinFunction::kObjectFieldNameSetComputed:
      return reinterpret_cast<const void*>(&RCallObjectSetNameComputed);

    default:
      UNREACHABLE();
      return nullptr;
  }
}

void QuakeJit::JSCall(::QuakeRT* rt, ScriptCall* sc) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);

  auto this_object =
      sc->this_object != nullptr ? *sc->this_object : JS_UNDEFINED;

  auto target_object =
      sc->target_object != nullptr ? *sc->target_object : JS_UNDEFINED;

  *sc->return_address = quake->qjs()->api.js_call(
      rt->context, *sc->function_object, this_object, target_object,
      sc->parameter_size, sc->parameter_start, 0);
}

void QuakeJit::JSCallCtor(::QuakeRT* rt, ScriptCall* sc) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto target_object = *sc->target_object;

  *sc->return_address = quake->qjs()->api.js_call_ctor(
      rt->context, *sc->function_object, target_object, sc->parameter_size,
      sc->parameter_start, 0);
}

void QuakeJit::PrintI32(std::int32_t value) {
  LOG(TRACE) << "(JIT): " << value;
}

void QuakeJit::PrintI64(std::int64_t value) {
  LOG(TRACE) << "(JIT): " << value;
}

void QuakeJit::PrintF64(double value) { LOG(TRACE) << "(JIT): " << value; }

double QuakeJit::RCallFMod(QuakeRT*, double x, double y) {
  return std::fmod(x, y);
}

double QuakeJit::RCallFPow(QuakeRT*, double x, double y) {
  return std::pow(x, y);
}

void QuakeJit::RCallGCFree(QuakeRT* rt, JSValue* jsv) {
  __JS_FreeValue(rt->context, *jsv);
}

JSValue QuakeJit::RCallObjectFieldGetAtom(QuakeRT* rt, void* object,
                                          std::uint32_t atom) {
  return JS_GetProperty(rt->context, JS_MKPTR(JS_TAG_OBJECT, object), atom);
}

JSValue QuakeJit::RCallObjectFieldPutAtom(QuakeRT* rt, void* object,
                                          std::uint32_t atom, JSValue* value) {
  auto r = JS_SetPropertyInternal(rt->context, JS_MKPTR(JS_TAG_OBJECT, object),
                                  atom, *value, JS_PROP_THROW_STRICT);
  if (r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallObjectFieldDefineAtom(::QuakeRT* rt, void* object,
                                             std::uint32_t atom,
                                             JSValue* value) {
  auto r = JS_DefinePropertyValue(rt->context, JS_MKPTR(JS_TAG_OBJECT, object),
                                  atom, *value, JS_PROP_C_W_E | JS_PROP_THROW);
  if (r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallObjectSetName(::QuakeRT* rt, void* object,
                                     std::uint32_t atom) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);

  auto r = quake->qjs()->api.js_object_set_name(
      rt->context, JS_MKPTR(JS_TAG_OBJECT, object), atom, JS_PROP_CONFIGURABLE);
  if (r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallObjectSetNameComputed(::QuakeRT* rt, void* object,
                                             JSValue* value) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);

  auto r = quake->qjs()->api.js_object_set_name_computed(
      rt->context, JS_MKPTR(JS_TAG_OBJECT, object), *value,
      JS_PROP_CONFIGURABLE);

  if (r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallGlobalGet(::QuakeRT* rt, std::uint32_t atom, int flag) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_global_get(rt->context, atom, flag);
}

JSValue QuakeJit::RCallGlobalPut(::QuakeRT* rt, std::uint32_t atom, JSValue* v,
                                 int flag) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);

  if (auto r = quake->qjs()->api.js_global_put(rt->context, atom, *v, flag);
      r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

int QuakeJit::DeoptPrepare(void* a1, void* a2) {
  return static_cast<int>(Deopt::DeoptFromJit(a1, a2));
}

}  // namespace quake
