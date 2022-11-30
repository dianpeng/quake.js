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

    case BuiltinFunction::kGlobalCheck:
      return reinterpret_cast<const void*>(&RCallGlobalCheck);

    case BuiltinFunction::kGlobalGet:
      return reinterpret_cast<const void*>(&RCallGlobalGet);

    case BuiltinFunction::kGlobalPut:
      return reinterpret_cast<const void*>(&RCallGlobalPut);

    case BuiltinFunction::kGlobalDefine:
      return reinterpret_cast<const void*>(&RCallGlobalDefine);

    case BuiltinFunction::kGlobalCheckDefine:
      return reinterpret_cast<const void*>(&RCallGlobalCheckDefine);

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

    case BuiltinFunction::kCheckBrand:
      return reinterpret_cast<const void*>(&RCallCheckBrand);

    case BuiltinFunction::kAddBrand:
      return reinterpret_cast<const void*>(&RCallAddBrand);

    case BuiltinFunction::kRegexp:
      return reinterpret_cast<const void*>(&RCallRegexp);

    case BuiltinFunction::kImport:
      return reinterpret_cast<const void*>(&RCallImport);

    case BuiltinFunction::kGetSuper:
      return reinterpret_cast<const void*>(&RCallGetSuper);

    default:
      UNREACHABLE();
      return nullptr;
  }
}

void QuakeJit::JSCallApply(::QuakeRT* rt, ScriptCall* sc) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);

  *sc->return_address = quake->qjs()->api.js_call_apply(
      rt->context, *sc->this_object, 2, sc->parameter_start, sc->option);
}

void QuakeJit::JSCallArrayFrom(::QuakeRT* rt, ScriptCall* sc) {
  auto array = JS_NewArray(rt->context);

  if (JS_IsException(array)) {
    *sc->return_address = JS_EXCEPTION;
    return;
  }

  // populate the array's data
  for (auto i = 0; i < sc->parameter_size; i++) {
    auto r = JS_DefinePropertyValue(rt->context, array, __JS_AtomFromUInt32(i),
                                    sc->parameter_start[i],
                                    JS_PROP_C_W_E | JS_PROP_THROW);

    if (r < 0) {
      JS_FreeValue(rt->context, array);
      *sc->return_address = JS_EXCEPTION;
      return;
    }
  }

  *sc->return_address = array;
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

void QuakeJit::RCallGCFree(QuakeRT* rt, void* object, int tag) {
  __JS_FreeValue(rt->context, JS_MKPTR(tag, object));
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

JSValue QuakeJit::RCallGlobalCheck(::QuakeRT* rt, std::uint32_t atom) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto r = quake->qjs()->api.js_global_check(rt->context, atom);
  if (r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_MKVAL(JS_TAG_BOOL, r);
  }
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

JSValue QuakeJit::RCallGlobalDefine(::QuakeRT* rt, std::uint32_t atom,
                                    int flag) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (auto r = quake->qjs()->api.js_global_define(rt->context, atom, flag);
      r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallGlobalCheckDefine(::QuakeRT* rt, std::uint32_t atom,
                                         int flag) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (auto r =
          quake->qjs()->api.js_global_check_define(rt->context, atom, flag);
      r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

int QuakeJit::DeoptPrepare(void* a1, void* a2) {
  return static_cast<int>(Deopt::DeoptFromJit(a1, a2));
}

JSValue QuakeJit::RCallAddBrand(::QuakeRT* rt, JSValue* l, JSValue* r) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (auto ret = quake->qjs()->api.js_add_brand(rt->context, *l, *r); ret < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallCheckBrand(::QuakeRT* rt, JSValue* l, JSValue* r) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (auto ret = quake->qjs()->api.js_check_brand(rt->context, *l, *r);
      ret < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallRegexp(::QuakeRT* rt, JSValue* l, JSValue* r) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_regexp(rt->context, *l, *r);
}

JSValue QuakeJit::RCallGetSuper(::QuakeRT* rt, JSValue* t) {
  return JS_GetPrototype(rt->context, *t);
}

JSValue QuakeJit::RCallImport(::QuakeRT* rt, JSValue* t) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_import(rt->context, *t);
}

}  // namespace quake
