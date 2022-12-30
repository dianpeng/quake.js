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
#define __(A, B, C, ...)   \
  case BuiltinFunction::A: \
    return reinterpret_cast<const void*>(&QuakeJit::C);
    QUAKE_BUILTIN_FUNCTION_LIST(__)
#undef __  // __

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

// This pow is JavaScript's version of POW, which is not compatible with the
// IEEE 754. It is a inline replication of what QuickJS does
double QuakeJit::RCallFPow(QuakeRT*, double x, double y) {
  if (std::isfinite(x) && std::fabs(y) == 1) {
    return base::kDoubleNaN;
  } else {
    return std::pow(x, y);
  }
}

void QuakeJit::RCallGCFree(QuakeRT* rt, void* object, int tag) {
  __JS_FreeValue(rt->context, JS_MKPTR(tag, object));
}

JSValue QuakeJit::RCallObjectNew(QuakeRT* rt) {
  return JS_NewObject(rt->context);
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

JSValue QuakeJit::RCallObjectFieldSetName(::QuakeRT* rt, void* object,
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

JSValue QuakeJit::RCallObjectFieldSetNameComputed(::QuakeRT* rt, void* object,
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

JSValue QuakeJit::RCallGlobalDefineFunction(::QuakeRT* rt, std::uint32_t atom,
                                            void* func, int flag) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (auto r = quake->qjs()->api.js_global_define_function(
          rt->context, atom, JS_MKPTR(JS_TAG_OBJECT, func), flag);
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

JSValue QuakeJit::RCallLoadArray(::QuakeRT* rt, JSValue* array,
                                 JSValue* index) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_get_property_value(rt->context, *array, *index);
}

JSValue QuakeJit::RCallLoadArrayInt(::QuakeRT* rt, void* array, int index) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto ret = quake->qjs()->api.js_get_property_value(
      rt->context, JS_MKPTR(JS_TAG_OBJECT, array), JS_MKVAL(JS_TAG_INT, index));
  return ret;
}

JSValue QuakeJit::RCallSetPropertyValue(::QuakeRT* rt, JSValue* target,
                                        JSValue* index, JSValue* value,
                                        int flag) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (auto r = quake->qjs()->api.js_set_property_value(rt->context, *target,
                                                       *index, *value, flag);
      r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallDefinePropertyValueValue(::QuakeRT* rt, JSValue* target,
                                                JSValue* index, JSValue* value,
                                                int flag) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (auto r = quake->qjs()->api.js_define_property_value_value(
          rt->context, *target, *index, *value, flag);
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
  return quake->qjs()->api.js_regexp(rt->context, JS_UNDEFINED, *l, *r);
}

JSValue QuakeJit::RCallGetSuper(::QuakeRT* rt, JSValue* t) {
  return JS_GetPrototype(rt->context, *t);
}

JSValue QuakeJit::RCallImport(::QuakeRT* rt, JSValue* t) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_import(rt->context, *t);
}

JSAtom QuakeJit::RCallValueToAtom(::QuakeRT* rt, JSValue* v) {
  return JS_ValueToAtom(rt->context, *v);
}

JSValue QuakeJit::RCallAtomToValue(::QuakeRT* rt, JSAtom atom) {
  return JS_AtomToValue(rt->context, atom);
}

JSValue QuakeJit::RCallGetSuperValue(::QuakeRT* rt, JSValue* a0, JSAtom a1,
                                     JSValue* a2) {
  auto v = JS_GetPropertyInternal(rt->context, *a0, a1, *a2, 0);
  JS_FreeAtom(rt->context, a1);
  return v;
}

JSValue QuakeJit::RCallPutSuperValue(::QuakeRT* rt, JSValue* a0, JSAtom a1,
                                     JSValue* a2, JSValue* a3) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto v = quake->qjs()->api.js_set_property_generic(rt->context, *a0, a1, *a2,
                                                     *a3, JS_PROP_THROW_STRICT);
  JS_FreeAtom(rt->context, a1);
  return v < 0 ? JS_EXCEPTION : JS_UNDEFINED;
}

JSValue QuakeJit::RCallSetProto(::QuakeRT* rt, JSValue* a0, JSValue* a1) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (quake->qjs()->api.js_set_prototype_internal(rt->context, *a0, *a1, TRUE) <
      0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

void QuakeJit::RCallSetHomeObject(::QuakeRT* rt, JSValue* a0, JSValue* a1) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  quake->qjs()->api.js_method_set_home_object(rt->context, *a0, *a1);
}

JSValue QuakeJit::RCallCopyDataProperties(::QuakeRT* rt, JSValue* a0,
                                          JSValue* a1, JSValue* a2) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (quake->qjs()->api.js_copy_data_properties(rt->context, *a0, *a1, *a2,
                                                0)) {
    return JS_EXCEPTION;
  }
  return JS_UNDEFINED;
}

JSValue QuakeJit::RCallAppend(::QuakeRT* rt, JSValue* a0, JSValue* a1,
                              JSValue* a2) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_append_enumerate(rt->context, *a0, *a1, *a2);
}

JSValue QuakeJit::RCallPrivateFieldPut(::QuakeRT* rt, JSValue* a0, JSValue* a1,
                                       JSValue* a2) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (quake->qjs()->api.js_set_private_field(rt->context, *a0, *a1, *a2) < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

JSValue QuakeJit::RCallPrivateFieldDefine(::QuakeRT* rt, JSValue* a0,
                                          JSValue* a1, JSValue* a2) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  if (quake->qjs()->api.js_define_private_field(rt->context, *a0, *a1, *a2) <
      0) {
    return JS_EXCEPTION;
  } else {
    return JS_UNDEFINED;
  }
}

void* QuakeJit::RCallInt32ToStr(::QuakeRT* rt, std::int32_t value) {
  char buf[32];
  ::snprintf(buf, sizeof(buf), "%d", value);
  auto jsv = JS_NewString(rt->context, buf);
  return JS_VALUE_GET_PTR(jsv);
}

void* QuakeJit::RCallFloat64ToStr(::QuakeRT* rt, double value) {
  auto jsv = JS_ToString(rt->context, __JS_NewFloat64(rt->context, value));
  return JS_VALUE_GET_PTR(jsv);
}

void* QuakeJit::RCallObjectToStr(::QuakeRT* rt, void* value) {
  auto jsv = JS_ToString(rt->context, JS_MKPTR(JS_TAG_OBJECT, value));
  return JS_VALUE_GET_PTR(jsv);
}

void* QuakeJit::RCallStrCon(::QuakeRT* rt, void* lhs, void* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto jsv = quake->qjs()->api.js_str_con(
      rt->context, JS_MKPTR(JS_TAG_STRING, lhs), JS_MKPTR(JS_TAG_STRING, rhs));

  return JS_VALUE_GET_PTR(jsv);
}

JSValue QuakeJit::RCallStrFieldGet(::QuakeRT* rt, void* target, JSAtom atom) {
  return JS_GetProperty(rt->context, JS_MKPTR(JS_TAG_STRING, target), atom);
}

double QuakeJit::RCallStringToFloat64(::QuakeRT* rt, void* string) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_string_to_float64(
      rt->context, reinterpret_cast<JSString*>(string));
}

int QuakeJit::RCallStrLt(::QuakeRT* rt, void* lhs, void* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto ret = quake->qjs()->api.js_string_compare(
      rt->context, reinterpret_cast<JSString*>(lhs),
      reinterpret_cast<JSString*>(rhs));
  return ret < 0;
}

int QuakeJit::RCallStrLe(::QuakeRT* rt, void* lhs, void* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto ret = quake->qjs()->api.js_string_compare(
      rt->context, reinterpret_cast<JSString*>(lhs),
      reinterpret_cast<JSString*>(rhs));
  return ret <= 0;
}

int QuakeJit::RCallStrGt(::QuakeRT* rt, void* lhs, void* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto ret = quake->qjs()->api.js_string_compare(
      rt->context, reinterpret_cast<JSString*>(lhs),
      reinterpret_cast<JSString*>(rhs));
  return ret > 0;
}

int QuakeJit::RCallStrGe(::QuakeRT* rt, void* lhs, void* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto ret = quake->qjs()->api.js_string_compare(
      rt->context, reinterpret_cast<JSString*>(lhs),
      reinterpret_cast<JSString*>(rhs));
  return ret >= 0;
}

int QuakeJit::RCallStrEq(::QuakeRT* rt, void* lhs, void* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto ret = quake->qjs()->api.js_string_compare(
      rt->context, reinterpret_cast<JSString*>(lhs),
      reinterpret_cast<JSString*>(rhs));
  return ret == 0;
}

int QuakeJit::RCallStrNe(::QuakeRT* rt, void* lhs, void* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto ret = quake->qjs()->api.js_string_compare(
      rt->context, reinterpret_cast<JSString*>(lhs),
      reinterpret_cast<JSString*>(rhs));
  return ret != 0;
}

JSValue QuakeJit::RCallCmpNe(::QuakeRT* rt, JSValue* lhs, JSValue* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_cmp_ne(rt->context, *lhs, *rhs);
}

JSValue QuakeJit::RCallCmpEq(::QuakeRT* rt, JSValue* lhs, JSValue* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_cmp_eq(rt->context, *lhs, *rhs);
}

JSValue QuakeJit::RCallCmpLt(::QuakeRT* rt, JSValue* lhs, JSValue* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_cmp_lt(rt->context, *lhs, *rhs);
}

JSValue QuakeJit::RCallCmpLe(::QuakeRT* rt, JSValue* lhs, JSValue* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_cmp_le(rt->context, *lhs, *rhs);
}

JSValue QuakeJit::RCallCmpGt(::QuakeRT* rt, JSValue* lhs, JSValue* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_cmp_gt(rt->context, *lhs, *rhs);
}

JSValue QuakeJit::RCallCmpGe(::QuakeRT* rt, JSValue* lhs, JSValue* rhs) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_cmp_ge(rt->context, *lhs, *rhs);
}

void* QuakeJit::RCallTypeOf(::QuakeRT* rt, JSValue* v) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  auto atom = quake->qjs()->api.js_typeof_to_atom(rt->context, *v);
  auto string = JS_AtomToString(rt->context, atom);
  return JS_VALUE_GET_PTR(string);  // reference counting is transferred
}

JSValue QuakeJit::RCallInstanceOf(::QuakeRT* rt, JSValue* a, JSValue* b) {
  auto r = JS_IsInstanceOf(rt->context, *a, *b);
  if (r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_NewBool(rt->context, r);
  }
}

JSValue QuakeJit::RCallDelete(::QuakeRT* rt, JSValue* a, JSValue* b) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_delete(rt->context, *a, *b);
}

JSValue QuakeJit::RCallDeleteVar(::QuakeRT* rt, JSAtom atom) {
  auto ctx = rt->context;
  auto glb = JS_GetGlobalObject(ctx);
  DEFER([&]() { JS_FreeValue(ctx, glb); });

  if (auto r = JS_DeleteProperty(ctx, glb, atom, 0); r < 0) {
    return JS_EXCEPTION;
  } else {
    return JS_NewBool(ctx, r);
  }
}

JSValue QuakeJit::RCallToObject(::QuakeRT* rt, JSValue* value) {
  auto quake = reinterpret_cast<Quake*>(rt->quake);
  return quake->qjs()->api.js_to_object(rt->context, *value);
}

JSValue QuakeJit::RCallToPropKey(::QuakeRT* rt, JSValue* value) {
  return JS_ToPropertyKey(rt->context, *value);
}

}  // namespace quake
