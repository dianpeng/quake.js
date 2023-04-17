#!/bin/env python

""" A test case generator based on simple type set theory

The generator basically just tries to perform set theory to generate simple
templated test cases for verify our implementation. The current cases generation
will be mostly focused on type enumeration and also its corresponding sample
value for verification.

The basic idea is as following, user needs to specify the operator's operand
types T.

For each opertor, it has Nth operands participate. Therefore we have combination

S(T)^N type combination. Each combination will need to sample some value from
each type value domain. The default strategy will try to sample its min, max
and normal value for verification purpose.

The verification result comes from the none jit code, which will be set as our
verification target

The user needs to specify a type setup foreach operand and the generator will
take care of the rest and generate the code, based on a builtin-template, to
perform the verification

{
  "name": "shl",                     // name of the testing case, used for
  "token": ">>",                     // token of the target operator
  "comment": "verify shl operator",  // comment of the test
  "type": [                          // an array of array, each element in the
    ["f64", "i32"],                  // array represents one of its operand's
    ["f64", "i32"]                   // type vector.
  ]
}

The generated verification code will contain following structure

// Case1: f64 shl i32
{
  // Case1.1: Max Max
  {
    let a0 = max(f64);
    let a1 = max(i32);
    let test_value = undefined;
    let expect = a0 >> a1;

    for (let i = 0; i < 50; i++) {
      test_value = a0 >> a1;
    }
    yes(test_value === expect);
  }

  // Case1.2: Min Min
  {
    let a0 = min(f64);
    let a1 = min(i32);
    let test_value = undefined;
    let expect = a0 >> a1;

    for (let i = 0; i < 50; i++) {
      test_value = a0 >> a1;
    }
    yes(test_value === expect);
  }

  // Case1.3: NoneZero Zero
  {
    let a0 = zero(f64);
    let a1 = zero(i32);
    let test_value = undefined;
    let expect = a0 >> a1;

    for (let i = 0; i < 50; i++) {
      test_value = a0 >> a1;
    }
    yes(test_value === expect);
  }

  // Case1.4: Random
  {
    let a0 = random(f64);
    let a1 = random(i32);
    let test_value = undefined;
    let expect = a0 >> a1;

    for (let i = 0; i < 50; i++) {
      test_value = a0 >> a1;
    }
    yes(test_value === expect);
  }
  ...
}

"""

import json
import math
import os

## To avoid depending on other template, like jinja, we just use normal string
## python template

CASE_TEMPLATE_UNARY="""

function {test_name}() {{
  dump("testing: {test_name}");
  let a0 = {sample0};
  let test_value = undefined;
  let expect = {unary_op} a0;
  for (let i = 0; i < 50; i++) {{
    test_value = {unary_op} a0;
  }}
  if (isNaN(test_value)) {{
    yes(isNaN(expect));
  }} else {{
    yes(test_value === expect);
  }}
}}
"""

CASE_TEMPLATE_BINARY="""

function {test_name}() {{
  dump("testing: {test_name}");
  let a0 = {sample0};
  let a1 = {sample1};
  let test_value = undefined;
  let expect = a0 {binary_op} a1;
  for (let i = 0; i < 50; i++) {{
    test_value = a0 {binary_op} a1;
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
    'string'              # string
]

## foreach type, we assign some basic sample method, ie sample its certain
## types
SAMPLER=dict()

def _min_max_zero_normal(min, max, zero, normal, nan=None, pinf=None, ninf=None):
    out = {'min': str(min), 'max': str(max), 'zero': str(zero), 'normal': str(normal)}

    if nan is not None:
        out['nan'] = 'NaN'

    if pinf is not None:
        out['pinf'] = 'Infinity'

    if ninf is not None:
        out['ninf'] = '-Infinity'

    return out

def _init():
    global SAMPLER;
    ## register all the needed sampler into the global sampler
    SAMPLER['f64'] = _min_max_zero_normal(1.79769313486231570e+308, 2.2250738585072014E-308, 0.0, 13.0, float('nan'), float('inf'), float('-inf'))
    SAMPLER['i32'] = _min_max_zero_normal(-2147483648, 2147483647, 0, 14)
    SAMPLER['u32'] = _min_max_zero_normal(0, 4294967295, 0, 15)
    SAMPLER['bool']= _min_max_zero_normal('false', 'true', 'false', 'true')
    SAMPLER['undefined'] = _min_max_zero_normal('undefined', 'undefined', 'undefined', 'undefined')
    SAMPLER['null'] = _min_max_zero_normal('null', 'null', 'null', 'null')
    SAMPLER['undefined_or_null'] = _min_max_zero_normal('undefined', 'null', 'undefined', 'null')
    SAMPLER['string'] = _min_max_zero_normal('\'\'', '\''+'a'*1024+'\'', '\'\'', '\'aaaabbccc\'')

## intialize global state
_init()

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

        for x in self._type[0]:
            if x not in TYPE:
                raise Error("type: %s is not known to us"%x)

            sampler = SAMPLER[x]
            assert sampler

            for sample_key in sampler:
                if sample_key in self._fbd:
                    continue

                test_name = 'test_{}_{}_{}'.format(self._name, x, sample_key)
                value = sampler[sample_key]

                ## now, let's generate the unary
                code = CASE_TEMPLATE_UNARY.format(
                    test_name=test_name,
                    sample0=value,
                    unary_op=self._token)

                self.output.append(code)
                self.test_list.append(test_name)


    def _generate_binary(self):
        global TYPE;
        global SAMPLER;
        global CASE_TEMPLATE_BINARY;

        for lhs in self._type[0]:
            for rhs in self._type[1]:
                if lhs not in TYPE:
                    raise Error("type(lhs): %s is not known to us"%lhs)

                if rhs not in TYPE:
                    raise Error("type(rhs): %s is not known to us"%rhs)

                lhs_sampler = SAMPLER[lhs]
                rhs_sampler = SAMPLER[rhs]

                for lhs_sample_key in lhs_sampler:
                    if lhs_sample_key in self._fbd:
                        continue

                    for rhs_sample_key in rhs_sampler:
                        if rhs_sample_key in self._fbd:
                            continue

                        test_name = 'test_{}_{}_{}_{}_{}'.format(self._name, 
                                lhs, 
                                rhs, 
                                lhs_sample_key, 
                                rhs_sample_key)

                        lhs_val = lhs_sampler[lhs_sample_key]
                        rhs_val = rhs_sampler[rhs_sample_key]

                        code = CASE_TEMPLATE_BINARY.format(
                                test_name = test_name,
                                sample0=lhs_val,
                                sample1=rhs_val,
                                binary_op=self._token)

                        self.output.append(code)
                        self.test_list.append(test_name)

PATH="./output/op_gen"
DSCP="./op_gen.json"

def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(path)

def _gen_one(x):
    global PATH

    code = []

    ## for each case
    g = Gen(x)
    g.generate()

    code.append("".join(g.output))
    for fn in g.test_list:
        code.append('{}();\n'.format(fn))

    cc = [
        "// Test case is generated, DO NOT MODIFY!!!\n",
        "// --------------------------------------------------------------\n",
        "{\n",
        "  ".join(code),
        "\n}\n",
        "// ----------------------------------------------------------------"
    ]
    code = ''.join(cc)
    f = open("{}/test_{}.js".format(PATH, x["name"]), "w+")
    f.write(code)
    f.close()

def _gen(json_path):
    f = open(json_path)
    j = json.load(f)

    code = []

    for x in j:
        ## treat it as comment
        if isinstance(x, str) or isinstance(x, unicode):
            continue
        _gen_one(x)

mkdir_p(PATH)
_gen(DSCP)
