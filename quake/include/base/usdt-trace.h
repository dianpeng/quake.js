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
#ifndef QUAKE_USDT_TRACE_DISABLE
#define QUAKE_HAS_USDT_TRACE

// clang-format off

// Default constraint for the probe arguments as operands.
#ifndef QUAKE_SDT_ARG_CONSTRAINT
#define QUAKE_SDT_ARG_CONSTRAINT      "nor"
#endif

// Instruction to emit for the probe.
#define QUAKE_SDT_NOP                 nop

// Note section properties.
#define QUAKE_SDT_NOTE_NAME           "stapsdt"
#define QUAKE_SDT_NOTE_TYPE           3

// Semaphore variables are put in this section
#define QUAKE_SDT_SEMAPHORE_SECTION   ".probes"

// Size of address depending on platform.
#ifdef __LP64__
#define QUAKE_SDT_ASM_ADDR            .8byte
#else
#define QUAKE_SDT_ASM_ADDR            .4byte
#endif

// Assembler helper Macros.
#define QUAKE_SDT_S(x)                #x
#define QUAKE_SDT_ASM_1(x)            QUAKE_SDT_S(x) "\n"
#define QUAKE_SDT_ASM_2(a, b)         QUAKE_SDT_S(a) "," QUAKE_SDT_S(b) "\n"
#define QUAKE_SDT_ASM_3(a, b, c)      QUAKE_SDT_S(a) "," QUAKE_SDT_S(b) ","  \
                                        QUAKE_SDT_S(c) "\n"
#define QUAKE_SDT_ASM_STRING(x)       QUAKE_SDT_ASM_1(.asciz QUAKE_SDT_S(x))

// Helper to determine the size of an argument.
#define QUAKE_SDT_IS_ARRAY_POINTER(x)  ((__builtin_classify_type(x) == 14) ||  \
                                          (__builtin_classify_type(x) == 5))

#define QUAKE_SDT_ARGSIZE(x)  (QUAKE_SDT_IS_ARRAY_POINTER(x)               \
                               ? sizeof(void*)                                 \
                               : sizeof(x))

// Format of each probe arguments as operand.
// Size of the arugment tagged with QUAKE_SDT_Sn, with "n" constraint.
// Value of the argument tagged with QUAKE_SDT_An, with configured constraint.
#define QUAKE_SDT_ARG(n, x)                                                  \
  [QUAKE_SDT_S##n] "n"                ((size_t)QUAKE_SDT_ARGSIZE(x)),      \
  [QUAKE_SDT_A##n] QUAKE_SDT_ARG_CONSTRAINT (x)

// Templates to append arguments as operands.
#define QUAKE_SDT_OPERANDS_0()        [__sdt_dummy] "g" (0)
#define QUAKE_SDT_OPERANDS_1(_1)      QUAKE_SDT_ARG(1, _1)
#define QUAKE_SDT_OPERANDS_2(_1, _2)                                         \
  QUAKE_SDT_OPERANDS_1(_1), QUAKE_SDT_ARG(2, _2)
#define QUAKE_SDT_OPERANDS_3(_1, _2, _3)                                     \
  QUAKE_SDT_OPERANDS_2(_1, _2), QUAKE_SDT_ARG(3, _3)
#define QUAKE_SDT_OPERANDS_4(_1, _2, _3, _4)                                 \
  QUAKE_SDT_OPERANDS_3(_1, _2, _3), QUAKE_SDT_ARG(4, _4)
#define QUAKE_SDT_OPERANDS_5(_1, _2, _3, _4, _5)                             \
  QUAKE_SDT_OPERANDS_4(_1, _2, _3, _4), QUAKE_SDT_ARG(5, _5)
#define QUAKE_SDT_OPERANDS_6(_1, _2, _3, _4, _5, _6)                         \
  QUAKE_SDT_OPERANDS_5(_1, _2, _3, _4, _5), QUAKE_SDT_ARG(6, _6)
#define QUAKE_SDT_OPERANDS_7(_1, _2, _3, _4, _5, _6, _7)                     \
  QUAKE_SDT_OPERANDS_6(_1, _2, _3, _4, _5, _6), QUAKE_SDT_ARG(7, _7)
#define QUAKE_SDT_OPERANDS_8(_1, _2, _3, _4, _5, _6, _7, _8)                 \
  QUAKE_SDT_OPERANDS_7(_1, _2, _3, _4, _5, _6, _7), QUAKE_SDT_ARG(8, _8)
#define QUAKE_SDT_OPERANDS_9(_1, _2, _3, _4, _5, _6, _7, _8, _9)             \
  QUAKE_SDT_OPERANDS_8(_1, _2, _3, _4, _5, _6, _7, _8), QUAKE_SDT_ARG(9, _9)

// Templates to reference the arguments from operands in note section.
#define QUAKE_SDT_ARGFMT(no)        %n[QUAKE_SDT_S##no]@%[QUAKE_SDT_A##no]
#define QUAKE_SDT_ARG_TEMPLATE_0    /*No arguments*/
#define QUAKE_SDT_ARG_TEMPLATE_1    QUAKE_SDT_ARGFMT(1)
#define QUAKE_SDT_ARG_TEMPLATE_2    QUAKE_SDT_ARG_TEMPLATE_1 QUAKE_SDT_ARGFMT(2)
#define QUAKE_SDT_ARG_TEMPLATE_3    QUAKE_SDT_ARG_TEMPLATE_2 QUAKE_SDT_ARGFMT(3)
#define QUAKE_SDT_ARG_TEMPLATE_4    QUAKE_SDT_ARG_TEMPLATE_3 QUAKE_SDT_ARGFMT(4)
#define QUAKE_SDT_ARG_TEMPLATE_5    QUAKE_SDT_ARG_TEMPLATE_4 QUAKE_SDT_ARGFMT(5)
#define QUAKE_SDT_ARG_TEMPLATE_6    QUAKE_SDT_ARG_TEMPLATE_5 QUAKE_SDT_ARGFMT(6)
#define QUAKE_SDT_ARG_TEMPLATE_7    QUAKE_SDT_ARG_TEMPLATE_6 QUAKE_SDT_ARGFMT(7)
#define QUAKE_SDT_ARG_TEMPLATE_8    QUAKE_SDT_ARG_TEMPLATE_7 QUAKE_SDT_ARGFMT(8)
#define QUAKE_SDT_ARG_TEMPLATE_9    QUAKE_SDT_ARG_TEMPLATE_8 QUAKE_SDT_ARGFMT(9)

// Semaphore define, declare and probe note format

#define QUAKE_SDT_SEMAPHORE(provider, name)                                  \
  quake_sdt_semaphore_##provider##_##name

#define QUAKE_SDT_DEFINE_SEMAPHORE(provider, name)                           \
  extern "C" {                                                                 \
    volatile unsigned short QUAKE_SDT_SEMAPHORE(provider, name)              \
    __attribute__((section(QUAKE_SDT_SEMAPHORE_SECTION), used)) = 0;         \
  }

#define QUAKE_SDT_DECLARE_SEMAPHORE(provider, name)                          \
  extern "C" volatile unsigned short QUAKE_SDT_SEMAPHORE(provider, name)

#define QUAKE_SDT_SEMAPHORE_NOTE_0(provider, name)                           \
  QUAKE_SDT_ASM_1(     QUAKE_SDT_ASM_ADDR 0) /*No Semaphore*/              \

#define QUAKE_SDT_SEMAPHORE_NOTE_1(provider, name)                           \
  QUAKE_SDT_ASM_1(QUAKE_SDT_ASM_ADDR QUAKE_SDT_SEMAPHORE(provider, name))

// Structure of note section for the probe.
#define QUAKE_SDT_NOTE_CONTENT(provider, name, has_semaphore, arg_template)  \
  QUAKE_SDT_ASM_1(990: QUAKE_SDT_NOP)                                      \
  QUAKE_SDT_ASM_3(     .pushsection .note.stapsdt,"","note")                 \
  QUAKE_SDT_ASM_1(     .balign 4)                                            \
  QUAKE_SDT_ASM_3(     .4byte 992f-991f, 994f-993f, QUAKE_SDT_NOTE_TYPE)   \
  QUAKE_SDT_ASM_1(991: .asciz QUAKE_SDT_NOTE_NAME)                         \
  QUAKE_SDT_ASM_1(992: .balign 4)                                            \
  QUAKE_SDT_ASM_1(993: QUAKE_SDT_ASM_ADDR 990b)                            \
  QUAKE_SDT_ASM_1(     QUAKE_SDT_ASM_ADDR 0) /*Reserved for Base Address*/ \
  QUAKE_SDT_SEMAPHORE_NOTE_##has_semaphore(provider, name)                   \
  QUAKE_SDT_ASM_STRING(provider)                                             \
  QUAKE_SDT_ASM_STRING(name)                                                 \
  QUAKE_SDT_ASM_STRING(arg_template)                                         \
  QUAKE_SDT_ASM_1(994: .balign 4)                                            \
  QUAKE_SDT_ASM_1(     .popsection)

// Main probe Macro.
#define QUAKE_SDT_PROBE(provider, name, has_semaphore, n, arglist)         \
    __asm__ __volatile__ (                                                   \
      QUAKE_SDT_NOTE_CONTENT(                                              \
        provider, name, has_semaphore, QUAKE_SDT_ARG_TEMPLATE_##n)         \
      :: QUAKE_SDT_OPERANDS_##n arglist                                    \
    )                                                                        \

// Helper Macros to handle variadic arguments.
#define QUAKE_SDT_NARG_(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define QUAKE_SDT_NARG(...)                                                \
  QUAKE_SDT_NARG_(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define QUAKE_SDT_PROBE_N(provider, name, has_semaphore, N, ...)           \
  QUAKE_SDT_PROBE(provider, name, has_semaphore, N, (__VA_ARGS__))

#define QUAKE_SDT(provider, name, ...)                                     \
  QUAKE_SDT_PROBE_N(provider, name, 0, QUAKE_SDT_NARG(0, ##__VA_ARGS__), \
		    ##__VA_ARGS__)
// Use QUAKE_SDT_DEFINE_SEMAPHORE(provider, name) to define the semaphore
// as global variable before using the QUAKE_SDT_WITH_SEMAPHORE macro
#define QUAKE_SDT_WITH_SEMAPHORE(provider, name, ...)                      \
  QUAKE_SDT_PROBE_N(provider, name, 1, QUAKE_SDT_NARG(0, ##__VA_ARGS__), \
		    ##__VA_ARGS__)
#define QUAKE_SDT_IS_ENABLED(provider, name) \
  (QUAKE_SDT_SEMAPHORE(provider, name) > 0)

// clang-format on

#endif  // QUAKE_USDT_TRACE_DISABLE
#endif  // OS_LINUX

// ----------------------------------------------------------------------------
// QUAKE's USDT trace macros
// ----------------------------------------------------------------------------

#ifndef QUAKE_HAS_USDT_TRACE

#define QUAKE_SDT(provider, name, ...) \
  do {                                 \
  } while (false)

#define QUAKE_SDT_WITH_SEMAPHORE(provider, name, ...) \
  do {                                                \
  } while (false)

#define QUAKE_SDT_IS_ENABLED(provider, name) (false)
#define QUAKE_SDT_DEFINE_SEMAPHORE(provider, name)
#define QUAKE_SDT_DECLARE_SEMAPHORE(provider, name)

#endif  // QUAKE_HAS_USDT_TRACE

// Really just a macro for code documentation, ie indicating the code inside of
// it is really meant for USDT definition
#ifdef QUAKE_HAS_USDT_TRACE
#define USDT(...) \
  do {            \
    __VA_ARGS__   \
  } while (false)
#else
#define USDT(...) (void*)(0)
#endif  // SARROW_HAS_USDT_TRACE

#endif  // _BASE_USDT_TRACE_H_
