#!/bin/env python

import json
import math
import os
import verify

CASE_TEMPLATE_BINARY="""

function {test_name}() {{
  dump("testing: {test_name}");
  let a1 = {sample1};
  let test_value = {sample0};

  let expect = {sample0};
  expect {binary_op} a1;

  for (let i = 0; i < 50; i++) {{
    test_value {binary_op} a1;
  }}

  if (isNaN(test_value)) {{
    yes(isNaN(expect));
  }} else {{
    yes(test_value === expect);
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
    'stri32',             # string int32
    'strf64',             # string f64
    'string'              # string
]

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

        assert len(self._type) == 2

    def generate(self):
        self._generate_binary()

    def _generate_binary(self):
        global TYPE;
        global SAMPLER;
        global CASE_TEMPLATE_BINARY;

        def _on(lhs_type, lhs_key, lhs_val, rhs_type, rhs_key, rhs_val):
            test_name = 'test_{}_{}_{}_{}_{}'.format(self._name, 
                    lhs_type, 
                    rhs_type, 
                    lhs_key, 
                    rhs_key)

            code = CASE_TEMPLATE_BINARY.format(
                    test_name = test_name,
                    sample0=lhs_val,
                    sample1=rhs_val,
                    binary_op=self._token)

            self.output.append(code)
            self.test_list.append(test_name)

        verify.foreach_binary(
            self._type[0],
            SAMPLER,
            self._type[1],
            SAMPLER,
            self._fbd,
            _on)

PATH="./output/aggop_gen"
DSCP="./aggop_gen.json"

verify.generate(PATH, DSCP, Gen)
