#ifndef _QUAKE_LIR_ALLOWED_BYTECODE_LIST_H_
#define _QUAKE_LIR_ALLOWED_BYTECODE_LIST_H_

/**
 * ----------------------------------------------------------------------------
 * TODO(dpeng):
 *
 * 1) Add support for async/generator. This include correctly implement
 *   1.1 await
 *   1.2 for_await_of_start
 *   1.3 initial_yield
 *   1.4 yield
 *   1.5 yield_star
 *   1.6 async_yield_star
 *   1.7 iterator_call
 *
 * ----------------------------------------------------------------------------
 * Some bytecode that is not handled purposely ...
 *
 * 1) define_method
 * 2) define_method_computed
 * 3) define_class
 * 4) define_class_computed
 *
 * Well, I don't think we should expect someone write a complicated loop logic
 * and then inside of the loop to define the function/method, which is really
 * wired :(
 *
 * ----------------------------------------------------------------------------
 */

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
  __(OP_object)                     \
  /** stack manipulation **/        \
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
  /** var ref **/                   \
  __(OP_get_var_ref)                \
  __(OP_put_var_ref)                \
  __(OP_set_var_ref)                \
  __(OP_get_var_ref_check)          \
  __(OP_put_var_ref_check)          \
  __(OP_put_var_ref_check_init)     \
  /** local variables **/           \
  __(OP_get_loc)                    \
  __(OP_set_loc)                    \
  __(OP_put_loc)                    \
  __(OP_set_loc_uninitialized)      \
  __(OP_get_loc_check)              \
  __(OP_put_loc_check)              \
  __(OP_put_loc_check_init)         \
  /** ref value **/                 \
  __(OP_get_ref_value)              \
  __(OP_put_ref_value)              \
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
  __(OP_define_func)                \
  __(OP_check_define_var)           \
  /** array **/                     \
  __(OP_get_array_el)               \
  __(OP_get_array_el2)              \
  __(OP_put_array_el)               \
  __(OP_define_array_el)            \
  /** super value **/               \
  __(OP_get_super_value)            \
  __(OP_put_super_value)            \
  /** misc **/                      \
  __(OP_check_ctor_return)          \
  __(OP_check_ctor)                 \
  __(OP_add_brand)                  \
  __(OP_check_brand)                \
  __(OP_regexp)                     \
  __(OP_get_super)                  \
  __(OP_import)                     \
  __(OP_set_proto)                  \
  __(OP_set_home_object)            \
  /** private field **/             \
  __(OP_put_private_field)          \
  __(OP_define_private_field)       \
  /** misc properties **/           \
  __(OP_copy_data_properties)       \
  __(OP_append)                     \
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
  __(OP_strict_eq)                  \
  __(OP_strict_neq)                 \
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
  __(OP_typeof)                     \
  __(OP_instanceof)                 \
  __(OP_to_object)                  \
  __(OP_to_propkey)                 \
  __(OP_to_propkey2)                \
  __(OP_delete)                     \
  __(OP_delete_var)                 \
  /** iterator **/                  \
  __(OP_iterator_check_object)      \
  __(OP_iterator_get_value_done)    \
  __(OP_iterator_close)             \
  __(OP_iterator_close_return)      \
  __(OP_iterator_next)              \
  /** for of **/                    \
  __(OP_for_of_start)               \
  __(OP_for_of_next)                \
  /** for in **/                    \
  __(OP_for_in_start)               \
  __(OP_for_in_next)                \
  /** control flow **/              \
  __(OP_if_true)                    \
  __(OP_if_false)                   \
  __(OP_goto)                       \
  /** bailout bytecode **/          \
  __(OP_throw)                      \
  __(OP_throw_error)

#endif  // ALLOWED_BYTECODE_LIST
