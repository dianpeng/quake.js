#ifndef __QUAKE_C_H__
#define __QUAKE_C_H__

#include "../../quickjs.h"
#include "../../quickjs-internal.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct QQuake QQuake;
typedef struct JSStackFrame QuakeRT;

/* Since QJS wraps all the internal API as static call, we want them to be
 * exposed as function pointer to allow maximum portability */
enum QJSMode { QJS_MODE_UNKNOWN = 0, QJS_MODE_STRICT = 1 };

enum JSGlobalCheckResult {
  QJS_GLOBAL_UNKNOWN = 0,
  QJS_MUST_GLOBAL = 1,
  QJS_GLOBAL_VAR = 2
};

enum {
  QUAKE_COMPILE_DONE = 0,
  QUAKE_COMPILE_STOP,
  QUAKE_COMPILE_NYI,
  QUAKE_COMPILE_BYTECODE_OVERFLOW,
  QUAKE_COMPILE_BYTECODE_ERROR,
  QUAKE_COMPILE_REFRENCE_ERROR,
  QUAKE_COMPILE_CODEGEN_ERROR
};

enum {
  QUAKE_JIT_START_PENDING, /* the jit trigger is still pending */
  QUAKE_JIT_START_FIRE,    /* the jit trigger should fire */
  QUAKE_JIT_START_DISABLE  /* the jit trigger is disabled for such PC */
};

/**
 * Some of the JIT penalty value. Used to indicate certain bytecode *cannot*
 * be jitted and should be only interpreted inside of the VM
 */

#define QUAKE_JIT_THRESHOLD_DISABLE (65535)

/**
 * All the quake JIT's threshold *cannot* be larger than this value and this
 * is needed internally
 */

#define QUAKE_JIT_THRESHOLD_MAX (50000)
#define QUAKE_JIT_THRESHOLD_BACKEDGE_DEFAULT (10)
#define QUAKE_JIT_THRESHOLD_CALL_DEFAULT (11)

typedef struct {
  JSStackFrame* stackframe; /* pointer to JSStackFrame(who calls into trace) */
  JSValue* arg;             /* base address of argument */
  uint32_t arg_size;        /* argument size */
  uint32_t caller_arg_size; /* the actual argument size passed by caller */
  uint32_t caller_arg_copy; /* whether the argument has been copied */
  JSValue* loc;             /* base address of locals */
  uint32_t loc_size;        /* local size */
  JSValue* temp;            /* base address of temporary */
  uint32_t temp_size;       /* temporary size */
  JSVarRef** var_ref;       /* base address of var_ref */
  uint32_t var_ref_size;    /* var_ref size */

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

typedef struct QuakeJitThreshold {
  uint16_t backedge_threshold;
  uint16_t call_threshold;
} QuakeJitThreshold;

inline int QuakeIsJitDisable(uint16_t threshold) {
  return threshold == QUAKE_JIT_THRESHOLD_DISABLE;
}

/* Declared else where and in order to make our C++ flag able to utilize it */
extern QuakeJitThreshold kQuakeJitThreshold;

typedef int (*QJitEntry)(QuakeRT*);

/** Free all the resources related to the QQuake, notes, should make sure
 ** no frame is been called when calling this function **/
void QuakeFree(QQuake*);
void QuakeFreeFrame(JSStackFrame*);
void QuakeEnterCall(JSFunctionBytecode*, QQuake*);
int QuakeLeaveCall(JSFunctionBytecode*, QQuake*);

/** get last error's descriptive description, if needed. This can be invoked
 *  after an error is emitted by Trace or Compile **/
const char* QuakeGetLastError(QQuake*);

int QuakeRun(QQuake*, QuakeRT*); /* execute the jit code */

/** property access, ie getting various aspects of the QQuake object **/
void QuakeGetBytecodeRange(QQuake*, const void**, const void**);
int QuakeIsRunning(QQuake*);
int QuakeIsJitComplete(QQuake*);

/**
 * Create a QFunction attached to a JSFunctionBytecode object
 * Notes, this function is only been invoked in quickjs.c:js_create_function
 */
void QuakeNewQFunction(void*, JSContext*);
void QuakeFreeQFunction(void*);

QQuake* QuakeOnLoopBack(JSFunctionBytecode*, QFunction* qfunc,
                        const void* loop_back, const void* target_bc,
                        int in_jit);

QQuake* QuakeOnCall(JSFunctionBytecode*, QFunction* qfunc, const void* entry);

inline int QuakeIsInlineTrace(JSContext* ctx, QQuake* qptr) {
  return ctx->jit_parent == qptr;
}

/** invoked by quickjs's OP_goto to try to trigger jit **/
inline int QuakeJitOpcodeHash(const void* pc) {
  int64_t v;
  memcpy(&v, &pc, sizeof(v));

  return (int)(v) & (32 - 1);
}

inline int QuakeJitStartOnLoopback(QFunction* b, const void* pc) {
  int idx = QuakeJitOpcodeHash(pc);
  int value = b->jit_hc[idx];

  if (QuakeIsJitDisable(value)) {
    return QUAKE_JIT_START_DISABLE;
  } else if (value >= kQuakeJitThreshold.backedge_threshold) {
    return QUAKE_JIT_START_FIRE;
  } else {
    b->jit_hc[idx]++;
    return (b->jit_hc[idx] >= kQuakeJitThreshold.backedge_threshold)
               ? QUAKE_JIT_START_FIRE
               : QUAKE_JIT_START_PENDING;
  }
}

inline int QuakeJitStartOnCall(QFunction* b, const void* pc) {
  int value = b->entry_hc;

  if (QuakeIsJitDisable(value)) {
    return QUAKE_JIT_START_DISABLE;
  } else if (value >= kQuakeJitThreshold.call_threshold) {
    return QUAKE_JIT_START_FIRE;
  } else {
    b->entry_hc++;
    return (b->entry_hc == kQuakeJitThreshold.call_threshold)
               ? QUAKE_JIT_START_FIRE
               : QUAKE_JIT_START_PENDING;
  }
}

inline QQuake* QuakeTriggerOnBackedge(JSFunctionBytecode* bfunc,
                                      QFunction* qfunc, const void* loop_back,
                                      const void* target_bc, int in_jit) {
  if (QuakeJitStartOnLoopback(qfunc, loop_back) == QUAKE_JIT_START_FIRE) {
    return QuakeOnLoopBack(bfunc, qfunc, loop_back, target_bc, in_jit);
  } else {
    return NULL;
  }
}

inline QQuake* QuakeTriggerOnCall(JSFunctionBytecode* bfunc, QFunction* qfunc,
                                  const void* call) {
  if (qfunc->js_context->jit_parent) {
    QuakeEnterCall(bfunc, (QQuake*)(qfunc->js_context->jit_parent));
    return (QQuake*)(qfunc->js_context->jit_parent);
  }

  if (QuakeJitStartOnCall(qfunc, call) == QUAKE_JIT_START_FIRE) {
    return QuakeOnCall(bfunc, qfunc, call);
  } else {
    return NULL;
  }
}

/* step one trace machine */
int QuakeStepTraceMachine(QFunction* qfunc, QQuake*, QInterpInfo*);

inline uint64_t QuakeRTMakeDeoptId(uint32_t did1, uint32_t did2) {
  union {
    struct {
      uint32_t d1;
      uint32_t d2;
    } did;
    uint64_t value;
  } u;
  u.did.d1 = did1;
  u.did.d2 = did2;
  return u.value;
}

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __QUAKE_C_H__
