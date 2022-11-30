#ifndef _QUAKE_LIR_ALLOWED_BYTECODE_LIST_H_
#define _QUAKE_LIR_ALLOWED_BYTECODE_LIST_H_

// A list of allowed bytecode, ie can be jitted. If function contains bytecode
// it is not showed up inside of our bytecode list, the JIT will bailout back
// to interpreter and always mark the function not able to be JITTed
#define QUAKE_JIT_BYTECODE_LIST(__) \
  /** value operations **/          \
  __(OP_push_i32)                   \
  __(OP_push_const)                 \
  __(OP_push_atom_value)            \
  __(OP_undefined)                  \
  __(OP_null)                       \
  __(OP_push_false)                 \
  __(OP_push_true)                  \
  __(OP_push_this)                  \
  __(OP_drop)                       \
  __(OP_nip)                        \
  __(OP_nip1)                       \
  __(OP_dup)                        \
  __(OP_dup1)                       \
  __(OP_dup2)                       \
  __(OP_dup3)                       \
  __(OP_insert2)                    \
  __(OP_insert3)                    \
  __(OP_insert4)                    \
  __(OP_rot3l)                      \
  __(OP_rot4l)                      \
  __(OP_rot5l)                      \
  __(OP_rot3r)                      \
  __(OP_perm3)                      \
  __(OP_perm4)                      \
  __(OP_perm5)                      \
  __(OP_swap)                       \
  __(OP_swap2)                      \
  /** local variables **/           \
  __(OP_get_loc)                    \
  __(OP_set_loc)                    \
  __(OP_put_loc)                    \
  __(OP_set_loc_uninitialized)      \
  __(OP_get_loc_check)              \
  __(OP_put_loc_check)              \
  __(OP_put_loc_check_init)         \
  /** object property **/           \
  __(OP_get_field)                  \
  __(OP_get_field2)                 \
  __(OP_put_field)                  \
  __(OP_define_field)               \
  __(OP_set_name)                   \
  __(OP_set_name_computed)          \
  /** call **/                      \
  __(OP_call)                       \
  __(OP_tail_call)                  \
  __(OP_call_constructor)           \
  __(OP_call_method)                \
  __(OP_tail_call_method)           \
  __(OP_array_from)                 \
  __(OP_apply)                      \
  /** global(var) **/               \
  __(OP_check_var)                  \
  __(OP_get_var_undef)              \
  __(OP_get_var)                    \
  __(OP_put_var)                    \
  __(OP_put_var_strict)             \
  __(OP_put_var_init)               \
  __(OP_define_var)                 \
  __(OP_check_define_var)           \
  /** misc **/                      \
  __(OP_check_ctor_return)          \
  __(OP_check_ctor)                 \
  __(OP_add_brand)                  \
  __(OP_check_brand)                \
  __(OP_regexp)                     \
  __(OP_get_super)                  \
  __(OP_import)                     \
  /** parameters **/                \
  __(OP_get_arg)                    \
  __(OP_set_arg)                    \
  __(OP_put_arg)                    \
  __(OP_add)                        \
  __(OP_sub)                        \
  __(OP_mul)                        \
  __(OP_div)                        \
  __(OP_mod)                        \
  __(OP_pow)                        \
  __(OP_is_undefined_or_null)       \
  __(OP_shl)                        \
  __(OP_shr)                        \
  __(OP_sar)                        \
  __(OP_and)                        \
  __(OP_or)                         \
  __(OP_xor)                        \
  __(OP_lt)                         \
  __(OP_lte)                        \
  __(OP_gte)                        \
  __(OP_gt)                         \
  __(OP_eq)                         \
  __(OP_neq)                        \
  __(OP_plus)                       \
  __(OP_neg)                        \
  __(OP_inc)                        \
  __(OP_dec)                        \
  __(OP_post_dec)                   \
  __(OP_post_inc)                   \
  __(OP_dec_loc)                    \
  __(OP_inc_loc)                    \
  __(OP_not)                        \
  __(OP_lnot)                       \
  __(OP_add_loc)                    \
  /** control flow **/              \
  __(OP_if_true)                    \
  __(OP_if_false)                   \
  __(OP_goto)

#endif  // ALLOWED_BYTECODE_LIST
