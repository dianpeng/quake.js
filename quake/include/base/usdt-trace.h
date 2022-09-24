#ifndef _BASE_USDT_TRACE_H_
#define _BASE_USDT_TRACE_H_
#include <cstddef>
#include "macro.h"

// The following code is heavily modified from Folly's static tracing with
// minor adaption for us to be used.
/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ----------------------------------------------------------------------------
// Section for Linux system which has capability of performing usdt tracing.
// On other platform, ie BSD DTrace, Windows's event system has similar concept,
// and we will implement them accordingly when we want to support multiple
// platform
// ----------------------------------------------------------------------------
#ifdef OS_LINUX
#ifndef BINGODB_USDT_TRACE_DISABLE
#define BINGODB_HAS_USDT_TRACE

// clang-format off

// Default constraint for the probe arguments as operands.
#ifndef BINGODB_SDT_ARG_CONSTRAINT
#define BINGODB_SDT_ARG_CONSTRAINT      "nor"
#endif

// Instruction to emit for the probe.
#define BINGODB_SDT_NOP                 nop

// Note section properties.
#define BINGODB_SDT_NOTE_NAME           "stapsdt"
#define BINGODB_SDT_NOTE_TYPE           3

// Semaphore variables are put in this section
#define BINGODB_SDT_SEMAPHORE_SECTION   ".probes"

// Size of address depending on platform.
#ifdef __LP64__
#define BINGODB_SDT_ASM_ADDR            .8byte
#else
#define BINGODB_SDT_ASM_ADDR            .4byte
#endif

// Assembler helper Macros.
#define BINGODB_SDT_S(x)                #x
#define BINGODB_SDT_ASM_1(x)            BINGODB_SDT_S(x) "\n"
#define BINGODB_SDT_ASM_2(a, b)         BINGODB_SDT_S(a) "," BINGODB_SDT_S(b) "\n"
#define BINGODB_SDT_ASM_3(a, b, c)      BINGODB_SDT_S(a) "," BINGODB_SDT_S(b) ","  \
                                        BINGODB_SDT_S(c) "\n"
#define BINGODB_SDT_ASM_STRING(x)       BINGODB_SDT_ASM_1(.asciz BINGODB_SDT_S(x))

// Helper to determine the size of an argument.
#define BINGODB_SDT_IS_ARRAY_POINTER(x)  ((__builtin_classify_type(x) == 14) ||  \
                                          (__builtin_classify_type(x) == 5))

#define BINGODB_SDT_ARGSIZE(x)  (BINGODB_SDT_IS_ARRAY_POINTER(x)               \
                               ? sizeof(void*)                                 \
                               : sizeof(x))

// Format of each probe arguments as operand.
// Size of the arugment tagged with BINGODB_SDT_Sn, with "n" constraint.
// Value of the argument tagged with BINGODB_SDT_An, with configured constraint.
#define BINGODB_SDT_ARG(n, x)                                                  \
  [BINGODB_SDT_S##n] "n"                ((size_t)BINGODB_SDT_ARGSIZE(x)),      \
  [BINGODB_SDT_A##n] BINGODB_SDT_ARG_CONSTRAINT (x)

// Templates to append arguments as operands.
#define BINGODB_SDT_OPERANDS_0()        [__sdt_dummy] "g" (0)
#define BINGODB_SDT_OPERANDS_1(_1)      BINGODB_SDT_ARG(1, _1)
#define BINGODB_SDT_OPERANDS_2(_1, _2)                                         \
  BINGODB_SDT_OPERANDS_1(_1), BINGODB_SDT_ARG(2, _2)
#define BINGODB_SDT_OPERANDS_3(_1, _2, _3)                                     \
  BINGODB_SDT_OPERANDS_2(_1, _2), BINGODB_SDT_ARG(3, _3)
#define BINGODB_SDT_OPERANDS_4(_1, _2, _3, _4)                                 \
  BINGODB_SDT_OPERANDS_3(_1, _2, _3), BINGODB_SDT_ARG(4, _4)
#define BINGODB_SDT_OPERANDS_5(_1, _2, _3, _4, _5)                             \
  BINGODB_SDT_OPERANDS_4(_1, _2, _3, _4), BINGODB_SDT_ARG(5, _5)
#define BINGODB_SDT_OPERANDS_6(_1, _2, _3, _4, _5, _6)                         \
  BINGODB_SDT_OPERANDS_5(_1, _2, _3, _4, _5), BINGODB_SDT_ARG(6, _6)
#define BINGODB_SDT_OPERANDS_7(_1, _2, _3, _4, _5, _6, _7)                     \
  BINGODB_SDT_OPERANDS_6(_1, _2, _3, _4, _5, _6), BINGODB_SDT_ARG(7, _7)
#define BINGODB_SDT_OPERANDS_8(_1, _2, _3, _4, _5, _6, _7, _8)                 \
  BINGODB_SDT_OPERANDS_7(_1, _2, _3, _4, _5, _6, _7), BINGODB_SDT_ARG(8, _8)
#define BINGODB_SDT_OPERANDS_9(_1, _2, _3, _4, _5, _6, _7, _8, _9)             \
  BINGODB_SDT_OPERANDS_8(_1, _2, _3, _4, _5, _6, _7, _8), BINGODB_SDT_ARG(9, _9)

// Templates to reference the arguments from operands in note section.
#define BINGODB_SDT_ARGFMT(no)        %n[BINGODB_SDT_S##no]@%[BINGODB_SDT_A##no]
#define BINGODB_SDT_ARG_TEMPLATE_0    /*No arguments*/
#define BINGODB_SDT_ARG_TEMPLATE_1    BINGODB_SDT_ARGFMT(1)
#define BINGODB_SDT_ARG_TEMPLATE_2    BINGODB_SDT_ARG_TEMPLATE_1 BINGODB_SDT_ARGFMT(2)
#define BINGODB_SDT_ARG_TEMPLATE_3    BINGODB_SDT_ARG_TEMPLATE_2 BINGODB_SDT_ARGFMT(3)
#define BINGODB_SDT_ARG_TEMPLATE_4    BINGODB_SDT_ARG_TEMPLATE_3 BINGODB_SDT_ARGFMT(4)
#define BINGODB_SDT_ARG_TEMPLATE_5    BINGODB_SDT_ARG_TEMPLATE_4 BINGODB_SDT_ARGFMT(5)
#define BINGODB_SDT_ARG_TEMPLATE_6    BINGODB_SDT_ARG_TEMPLATE_5 BINGODB_SDT_ARGFMT(6)
#define BINGODB_SDT_ARG_TEMPLATE_7    BINGODB_SDT_ARG_TEMPLATE_6 BINGODB_SDT_ARGFMT(7)
#define BINGODB_SDT_ARG_TEMPLATE_8    BINGODB_SDT_ARG_TEMPLATE_7 BINGODB_SDT_ARGFMT(8)
#define BINGODB_SDT_ARG_TEMPLATE_9    BINGODB_SDT_ARG_TEMPLATE_8 BINGODB_SDT_ARGFMT(9)

// Semaphore define, declare and probe note format

#define BINGODB_SDT_SEMAPHORE(provider, name)                                  \
  bingodb_sdt_semaphore_##provider##_##name

#define BINGODB_SDT_DEFINE_SEMAPHORE(provider, name)                           \
  extern "C" {                                                                 \
    volatile unsigned short BINGODB_SDT_SEMAPHORE(provider, name)              \
    __attribute__((section(BINGODB_SDT_SEMAPHORE_SECTION), used)) = 0;         \
  }

#define BINGODB_SDT_DECLARE_SEMAPHORE(provider, name)                          \
  extern "C" volatile unsigned short BINGODB_SDT_SEMAPHORE(provider, name)

#define BINGODB_SDT_SEMAPHORE_NOTE_0(provider, name)                           \
  BINGODB_SDT_ASM_1(     BINGODB_SDT_ASM_ADDR 0) /*No Semaphore*/              \

#define BINGODB_SDT_SEMAPHORE_NOTE_1(provider, name)                           \
  BINGODB_SDT_ASM_1(BINGODB_SDT_ASM_ADDR BINGODB_SDT_SEMAPHORE(provider, name))

// Structure of note section for the probe.
#define BINGODB_SDT_NOTE_CONTENT(provider, name, has_semaphore, arg_template)  \
  BINGODB_SDT_ASM_1(990: BINGODB_SDT_NOP)                                      \
  BINGODB_SDT_ASM_3(     .pushsection .note.stapsdt,"","note")                 \
  BINGODB_SDT_ASM_1(     .balign 4)                                            \
  BINGODB_SDT_ASM_3(     .4byte 992f-991f, 994f-993f, BINGODB_SDT_NOTE_TYPE)   \
  BINGODB_SDT_ASM_1(991: .asciz BINGODB_SDT_NOTE_NAME)                         \
  BINGODB_SDT_ASM_1(992: .balign 4)                                            \
  BINGODB_SDT_ASM_1(993: BINGODB_SDT_ASM_ADDR 990b)                            \
  BINGODB_SDT_ASM_1(     BINGODB_SDT_ASM_ADDR 0) /*Reserved for Base Address*/ \
  BINGODB_SDT_SEMAPHORE_NOTE_##has_semaphore(provider, name)                   \
  BINGODB_SDT_ASM_STRING(provider)                                             \
  BINGODB_SDT_ASM_STRING(name)                                                 \
  BINGODB_SDT_ASM_STRING(arg_template)                                         \
  BINGODB_SDT_ASM_1(994: .balign 4)                                            \
  BINGODB_SDT_ASM_1(     .popsection)

// Main probe Macro.
#define BINGODB_SDT_PROBE(provider, name, has_semaphore, n, arglist)         \
    __asm__ __volatile__ (                                                   \
      BINGODB_SDT_NOTE_CONTENT(                                              \
        provider, name, has_semaphore, BINGODB_SDT_ARG_TEMPLATE_##n)         \
      :: BINGODB_SDT_OPERANDS_##n arglist                                    \
    )                                                                        \

// Helper Macros to handle variadic arguments.
#define BINGODB_SDT_NARG_(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define BINGODB_SDT_NARG(...)                                                \
  BINGODB_SDT_NARG_(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define BINGODB_SDT_PROBE_N(provider, name, has_semaphore, N, ...)           \
  BINGODB_SDT_PROBE(provider, name, has_semaphore, N, (__VA_ARGS__))

#define BINGODB_SDT(provider, name, ...)                                     \
  BINGODB_SDT_PROBE_N(provider, name, 0, BINGODB_SDT_NARG(0, ##__VA_ARGS__), \
		    ##__VA_ARGS__)
// Use BINGODB_SDT_DEFINE_SEMAPHORE(provider, name) to define the semaphore
// as global variable before using the BINGODB_SDT_WITH_SEMAPHORE macro
#define BINGODB_SDT_WITH_SEMAPHORE(provider, name, ...)                      \
  BINGODB_SDT_PROBE_N(provider, name, 1, BINGODB_SDT_NARG(0, ##__VA_ARGS__), \
		    ##__VA_ARGS__)
#define BINGODB_SDT_IS_ENABLED(provider, name) \
  (BINGODB_SDT_SEMAPHORE(provider, name) > 0)

// clang-format on

#endif  // BINGODB_USDT_TRACE_DISABLE
#endif  // OS_LINUX

// ----------------------------------------------------------------------------
// Bingodb's USDT trace macros
// ----------------------------------------------------------------------------

#ifndef BINGODB_HAS_USDT_TRACE

#define BINGODB_SDT(provider, name, ...) \
  do {                                   \
  } while (false)

#define BINGODB_SDT_WITH_SEMAPHORE(provider, name, ...) \
  do {                                                  \
  } while (false)

#define BINGODB_SDT_IS_ENABLED(provider, name) (false)
#define BINGODB_SDT_DEFINE_SEMAPHORE(provider, name)
#define BINGODB_SDT_DECLARE_SEMAPHORE(provider, name)

#endif  // BINGODB_HAS_USDT_TRACE

// Really just a macro for code documentation, ie indicating the code inside of
// it is really meant for USDT definition
#ifdef BINGODB_HAS_USDT_TRACE
#define USDT(...) \
  do {            \
    __VA_ARGS__   \
  } while (false)
#else
#define USDT(...) (void*)(0)
#endif  // SARROW_HAS_USDT_TRACE

#endif  // _BASE_USDT_TRACE_H_
