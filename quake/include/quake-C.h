#ifndef __QUAKE_C_H__
#define __QUAKE_C_H__

#ifdef QUAKE_JIT
#include "../../quickjs.h"
#include "../../quickjs-internal.h"
#else
#include "../../quickjs.h"
#include "../../quickjs-internal.h"
#endif

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct QQuake QQuake;
typedef struct QRecoveryFrame QRecoveryFrame;

typedef struct QRecoveryFrame {
  QRecoveryFrame* prev;

  JSValue* slot;
  size_t slot_size;

  JSValue* param;
  JSValue* local;
  JSValue* temp;
  JSValue* temp_cur;  // current temp

  JSValue func_object;
  JSValue this_object;
  JSValue target_object;

  const void* next_bc;
} QRecoveryFrame;

QRecoveryFrame* QuakeNewQRecoveryFrame(size_t, size_t, size_t);

typedef struct QuakeRT {
  QQuake* quake;

  // --------------------------------------------------------------------------
  // entry frame's param/local/temp, notes owned by the VM interpreter
  JSValue* param;
  JSValue* param_end;

  JSValue* local;
  JSValue* local_end;

  // notes since QJS has a stack machine
  JSValue* temp;
  JSValue* temp_cur;
  JSValue* temp_end;

  // var_ref
  JSVarRef** var_ref;
  JSVarRef** var_ref_end;

  // --------------------------------------------------------------------------
  // Context infromation
  JSContext* context;
  JSStackFrame* current_stack_frame;
  JSValue func_object;
  JSValue this_object;
  JSValue target_object;

  // --------------------------------------------------------------------------
  // Other snapshot/recovery frame etc..
  int next;                // next option, ie DeoptNext enum
  int32_t exit_id;         // deoptimization id
  const void* next_bc;     // the top most frame's next id, other inner frame is
                           // been put into the QRecoveryFrame object
  QRecoveryFrame* rframe;  // inner most
} QuakeRT;

/* Since QJS wraps all the internal API as static call, we want them to be
 * exposed as function pointer to allow maximum portability */
enum QJSMode { QJS_MODE_UNKNOWN = 0, QJS_MODE_STRICT = 1 };

enum JSGlobalCheckResult {
  QJS_GLOBAL_UNKNOWN = 0,
  QJS_MUST_GLOBAL = 1,
  QJS_GLOBAL_VAR = 2
};

typedef struct {
  /** 1. JSContext --------------------------------------------- */

  /** convert a JSAtom reference (U32) into a JSValue object **/
  JSValue (*context_get_atom)(JSContext*, JSAtom atom, int);

  /** obtain current JSContext's global object **/
  JSValue (*context_get_global)(JSContext*);

  /** obtain a conversion from any value to object, can throw and exception */
  JSValue (*context_to_object)(JSContext*, JSValue);

  /** 2. Bytecode --------------------------------------------- */

  /** get the constant located at constant pool from bytecode **/
  JSValue (*bytecode_get_constant)(JSContext*, void*, uint32_t);

  /** get the bytecode object's mode */
  int (*bytecode_get_mode)(JSContext*, void*);

  /** 3. VM helpers ------------------------------------------- */
  JSValue (*js_call)(JSContext*, JSValueConst, JSValueConst, JSValueConst, int,
                     JSValue*, int);

  JSValue (*js_call_ctor)(JSContext*, JSValueConst, JSValueConst, int, JSValue*,
                          int);

  JSValue (*js_call_apply)(JSContext*, JSValueConst, int, JSValueConst*, int);

  int (*js_object_set_name)(JSContext*, JSValueConst, JSAtom, int);
  int (*js_object_set_name_computed)(JSContext*, JSValueConst, JSValueConst,
                                     int);

  int (*js_global_check)(JSContext*, JSAtom);
  JSValue (*js_global_get)(JSContext*, JSAtom, int);
  int (*js_global_put)(JSContext*, JSAtom, JSValue, int);
  int (*js_global_define)(JSContext*, JSAtom, int);
  int (*js_global_define_function)(JSContext*, JSAtom, JSValueConst, int);
  int (*js_global_check_define)(JSContext*, JSAtom, int);

  /** helper, used for specialization **/
  enum JSGlobalCheckResult (*js_global_check_symbol)(JSContext*, JSAtom, int);

  /** array/element **/
  JSValue (*js_get_property_value)(JSContext*, JSValueConst, JSValue);
  int (*js_set_property_value)(JSContext*, JSValueConst, JSValue, JSValue, int);
  int (*js_define_property_value_value)(JSContext*, JSValueConst, JSValue,
                                        JSValue, int);

  /** other(misc) **/
  int (*js_check_brand)(JSContext*, JSValueConst, JSValueConst);
  int (*js_add_brand)(JSContext*, JSValueConst, JSValueConst);
  JSValue (*js_regexp)(JSContext*, JSValueConst, JSValue, JSValue);
  JSValue (*js_import)(JSContext*, JSValueConst);

  int (*js_set_property_generic)(JSContext*, JSValueConst, JSAtom, JSValue,
                                 JSValueConst, int);

  int (*js_set_prototype_internal)(JSContext*, JSValueConst, JSValueConst, int);
  void (*js_method_set_home_object)(JSContext*, JSValueConst, JSValueConst);
  int (*js_copy_data_properties)(JSContext*, JSValueConst, JSValueConst,
                                 JSValueConst, int);
  JSValue (*js_append_enumerate)(JSContext*, JSValue, JSValue, JSValue);

  /** private field **/
  int (*js_set_private_field)(JSContext*, JSValueConst, JSValueConst, JSValue);
  int (*js_define_private_field)(JSContext*, JSValueConst, JSValueConst,
                                 JSValue);
  JSValue (*js_get_private_field)(JSContext*, JSValueConst, JSValueConst);

  /** string **/
  JSValue (*js_str_con)(JSContext*, JSValue, JSValue);
  double (*js_string_to_float64)(JSContext*, JSString*);
  int (*js_string_compare)(JSContext*, const JSString*, const JSString*);

  /** comparison */
  JSValue (*js_cmp_lt)(JSContext*, JSValue, JSValue);
  JSValue (*js_cmp_le)(JSContext*, JSValue, JSValue);
  JSValue (*js_cmp_gt)(JSContext*, JSValue, JSValue);
  JSValue (*js_cmp_ge)(JSContext*, JSValue, JSValue);
  JSValue (*js_cmp_eq)(JSContext*, JSValue, JSValue);
  JSValue (*js_cmp_ne)(JSContext*, JSValue, JSValue);

  /** operators **/
  JSAtom (*js_typeof_to_atom)(JSContext*, JSValueConst);
  JSValue (*js_delete)(JSContext*, JSValue, JSValue);
  JSValue (*js_to_object)(JSContext*, JSValueConst);

} QJSApi;

typedef struct {
  QJSApi api; /* all exposed internal API to be used by Quake JIT */
} QJS;

typedef struct {
  JSStackFrame* stackframe; /* pointer to JSStackFrame(who calls into trace) */

  JSValue* arg;      /* base address of argument */
  uint32_t arg_size; /* argument size */

  JSValue* loc;      /* base address of locals */
  uint32_t loc_size; /* local size */

  JSValue* temp;      /* base address of temporary */
  uint32_t temp_size; /* temporary size */

  JSVarRef** var_ref;    /* base address of var_ref */
  uint32_t var_ref_size; /* var_ref size */

  JSValue this_object;
  JSValue func_object;
  JSValue target_object;

} QFrameInfo;

typedef struct {
  void* bc_object; /* opaque bytecode object */

  const void* bc_start; /* bytecode start */
  uint32_t bc_size;     /* total bytecode size */

  const void* pc; /* current PC, potentially contain opcode's argument */
  uint8_t op;     /* opcode */

} QBytecodeInfo;

typedef struct {
  JSRuntime* qjs_runtime;
  JSContext* qjs_context;

  QFrameInfo frame;
  QBytecodeInfo bc;
} QInterpInfo;

/** JIT intrerfaces ------------------------------------------------------- **/
enum {
  kQDone = 0,
  kQStop = 1,
  kQNYI,
  kQBytecodeOverflow,
  kQBytecodeError,
  kQReferenceError,
  kQCompileError,
};

enum { kQuakeJitThreshold = 10 };

enum { kQPenaltyNone, kQPenaltyOn };

typedef int (*QJitEntry)(QuakeRT*);

QQuake* QuakeNew(QJS*, JSContext*);

/** Free all the resources related to the QQuake, notes, should make sure
 ** no frame is been called when calling this function **/
void QuakeFree(QQuake*);

/** get last error's descriptive description, if needed. This can be invoked
 *  after an error is emitted by Trace or Compile **/
const char* QuakeGetLastError(QQuake*);

/** Quake toggle, ie certain aspects of JIT can be tunned **/
int QuakeSetup(QQuake*, QInterpInfo*);
int QuakeTrace(QQuake*, QInterpInfo*);
int QuakeStop(QQuake*, QInterpInfo*);
int QuakeCompile(QQuake*);
int QuakeRun(QQuake*, QuakeRT*); /* execute the jit code */

/** property access, ie getting various aspects of the QQuake object **/
void QuakeGetBytecodeRange(QQuake*, const void**, const void**);
int QuakeIsRunning(QQuake*);
int QuakeIsJitComplete(QQuake*);

void QuakeNewQFunction(QFunction*, QJS*, JSContext*);
void QuakeFreeQFunction(QFunction*);

QQuake* QuakeOnLoopBack(QFunction* qfunc, const void* loop_back,
                        const void* target_bc);

/** invoked by quickjs's OP_goto to try to trigger jit **/
inline int QuakeJitOpcodeHash(const void* pc) {
  int64_t v;
  memcpy(&v, &pc, sizeof(v));

  return (int)(v) & (32 - 1);
}

inline int QuakeJitStart(QFunction* b, const void* pc) {
  int idx = QuakeJitOpcodeHash(pc);
  b->jit_hc[idx]++;
  return b->jit_hc[idx] > kQuakeJitThreshold;
}

inline QQuake* QuakeTrigger(QFunction* qfunc, const void* loop_back,
                            const void* target_bc) {
  if (QuakeJitStart(qfunc, loop_back)) {
    return QuakeOnLoopBack(qfunc, loop_back, target_bc);
  } else {
    return NULL;
  }
}

/* step one trace machine */
int QuakeStepTraceMachine(QFunction* qfunc, QQuake*, QInterpInfo*);

/* get target QQuake object from the loop_back bc address */
QQuake* QuakeGetQuake(QFunction*, const void*);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __QUAKE_C_H__
