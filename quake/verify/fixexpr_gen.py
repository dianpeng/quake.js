#!/bin/env python

import json
import math
import os
import verify

## To avoid depending on other template, like jinja, we just use normal string
## python template

CASE_TEMPLATE_PREFIX ="""

function {test_name}() {{
  dump("testing: {test_name}");

  let original = {sample0};
  let a1 = {sample0};

  let test_after = undefined;
  let test_value = undefined;

  let a0 = {sample0};
  let expect_after = {unary_op}a0;
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {{
    test_after = {unary_op}a1;
    test_value = a1;

    a1 = original;
  }}

  if (isNaN(test_value)) {{
    yes(isNaN(expect_value));
  }} else {{
    yes(test_value === expect_value);
  }}

  if (isNaN(test_after)) {{
    yes(isNaN(expect_after));
  }} else {{
    yes(test_after === expect_after);
  }}
}}
"""

CASE_TEMPLATE_POSTFIX="""

function {test_name}() {{
  dump("testing: {test_name}");

  let original = {sample0};
  let a1 = {sample0};

  let test_after = undefined;
  let test_value = undefined;

  let a0 = {sample0};
  let expect_after = a0{unary_op};
  let expect_value = a0;

  for (let i = 0; i < 50; i++) {{
    test_after = a1{unary_op};
    test_value = a1;

    a1 = original;
  }}

  if (isNaN(test_value)) {{
    yes(isNaN(expect_value));
  }} else {{
    yes(test_value === expect_value);
  }}

  if (isNaN(test_after)) {{
    yes(isNaN(expect_after));
  }} else {{
    yes(test_after === expect_after);
  }}
}}
"""

TYPE=[
    'f64',                # a float64
    'i32',                # int32
    'u32',                # uint32
    'bool',               # boolean
    'undefined',          # undefined 
    'null',               # null
    'undefined_or_null',  # null or undefined
    'object',             # object
    'stri32',             # string i32
    'strf64',             # string f64
    'string'              # string
]

## foreach type, we assign some basic sample method, ie sample its certain
## types
SAMPLER=verify.make_sampler_list(TYPE)

class Gen:
    def __init__(self, des):
        self.d = des         # notes this already in json format
        self.output = []     # the generated code template
        self.test_list = []  # list of function name for testing

        self._token = self.d["token"]
        self._comment = self.d["comment"]
        self._name = self.d["name"]
        self._type = self.d["type"]
        self._fbd = self.d.get("forbid", [])
        self._position = self.d.get("position", "after")

        ## we only support unary or binary for now, notes for tracing jit,
        ## without stitch trace, no way to JIT branch, ie ternary is not
        ## working for us
        assert len(self._type) == 1 or len(self._type) == 2

    def generate(self):
        if len(self._type) == 1:
            self._generate_unary()
        else:
            self._generate_binary()

    def _generate_unary(self):
        global TYPE;
        global SAMPLER;
        global CASE_TEMPLATE_UNARY;

        def _on(type, key, value):
            test_name = 'test_{}_{}_{}'.format(self._name, type, key)
            code = None

            if self._position == 'after':
                code = CASE_TEMPLATE_POSTFIX.format(
                    test_name=test_name,
                    sample0=value,
                    unary_op=self._token)

            else:
                code = CASE_TEMPLATE_PREFIX.format(
                    test_name=test_name,
                    sample0=value,
                    unary_op=self._token)

            self.output.append(code)
            self.test_list.append(test_name)

        verify.foreach_unary(self._type[0], SAMPLER, self._fbd, _on)

PATH="./output/fixexpr_gen"
DSCP="./fixexpr_gen.json"

verify.generate(PATH, DSCP, Gen)
