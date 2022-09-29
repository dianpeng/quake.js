#ifndef __QUAKE_API_H__
#define __QUAKE_API_H__
#include "quick-js-inc.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/* Since QJS wraps all the internal API as static call, we want them to be
 * exposed as function pointer to allow maximum portability */

enum QJSMode { QJS_MODE_STRICT = 0 };

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

} QJSApi;

typedef struct {
  QJSApi api; /* all exposed internal API to be used by Quake JIT */
} QJS;

typedef struct {
  void* stackframe; /* pointer to JSStackFrame(who calls into trace) */

  JSValue* arg;      /* base address of argument */
  uint32_t arg_size; /* argument size */

  JSValue* loc;      /* base address of locals */
  uint32_t loc_size; /* local size */

  JSValue* temp;      /* base address of temporary */
  uint32_t temp_size; /* temporary size */

  JSValue* slot;
  uint32_t slot_size;

  JSValue this_object;
  JSValue func_object;
  JSValue target_object;

} QFrameInfo;

typedef struct {
  void* bc_object; /* opaque bytecode object */

  void* bc_start;   /* bytecode start */
  uint32_t bc_size; /* total bytecode size */

  void* pc;   /* current PC, potentially contain opcode's argument */
  uint8_t op; /* opcode */

} QBytecodeInfo;

typedef struct {
  JSRuntime* qjs_runtime;
  JSContext* qjs_context;

  QFrameInfo frame;
  QBytecodeInfo bc;
} QInterpInfo;

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __QUAKE_API_H__
