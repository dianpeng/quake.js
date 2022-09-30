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
  __(OP_get_loc)                    \
  __(OP_set_loc)                    \
  __(OP_put_loc)                    \
  __(OP_get_arg)                    \
  __(OP_set_arg)                    \
  __(OP_put_arg)                    \
  __(OP_add)                        \
  __(OP_sub)                        \
  __(OP_mul)                        \
  __(OP_div)                        \
  __(OP_add_loc)                    \
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
  __(OP_not)                        \
  /** control flow **/

/*
  __(OP_if_true)                        \
  __(OP_if_false)                       \
  __(OP_goto)
*/

#endif  // _QUAKE_ALLOWED_BYTECODE_LIST_H_
