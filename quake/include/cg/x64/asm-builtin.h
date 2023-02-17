#ifndef _QUAKE_CG_X64_ASM_BUILTIN_H_
#define _QUAKE_CG_X64_ASM_BUILTIN_H_

#define QUAKE_CG_X64_ASM_BUILTIN_STUB_LIST(__) \
  __(prologue)                                 \
  __(epilogue)                                 \
  __(trace_prologue)                           \
  __(trace_epilogue)                           \
  __(deopt_jump_entry)

#define QUAKE_CG_X64_ASM_BUILTIN_RUNTIME_LIST(__) \
  __(deopt)                                       \
  __(cast_f64_to_b32)                             \
  __(object_field_get_atom_fast)                  \
  __(object_field_set_atom_fast)                  \
  __(global_get_global_atom_fast)                 \
  __(any_to_bool)

#define QUAKE_CG_X64_ASM_BUILTIN_LIST(__) \
  QUAKE_CG_X64_ASM_BUILTIN_STUB_LIST(__)  \
  QUAKE_CG_X64_ASM_BUILTIN_RUNTIME_LIST(__)

#endif  // _QUAKE_CG_X64_ASM_BUILTIN_H_
