#!/bin/env python

import verify

CASE_TEMPLATE_INDEX_GET="""

function {test_name}() {{
  dump("testing: {test_name}");

  let obj = {object};
  let idx = {index};
  let expect = obj[idx];

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {{
    test_value = obj[idx];
  }}

  if (isNaN(test_value)) {{
    yes(isNaN(expect));
  }} else {{
    yes(test_value === expect);
  }}
}}

"""

CASE_TEMPLATE_INDEX_SET="""

function {test_name}() {{
  dump("testing: {test_name}");

  let obj = {object};
  let idx = {index};
  let val = {value};

  let test_value = undefined;
  obj[idx] = val;
  let expect = obj[idx];

  for (let i = 0; i < 50; i++) {{
    obj[idx] = val;
    test_value = obj[idx];
  }}

  if (isNaN(test_value)) {{
    yes(isNaN(expect));
  }} else {{
    yes(test_value === expect);
  }}
}}

"""

## A fairly complicated nested object
## match the index accessing

CASE_OBJECT_TEMPLATE = """

{
  "a": 1,
  "b": [
    {
      "c": {
        "d": true,
        "f": undefined,
        "g": {
          "[object Object]": {
            "a": {},
            "0": [],
            "1": "hello world"
          }
        }
      }
    }
  ]
}
"""

CASE_TEMPLATE_FIELD_GET ="""

function {test_name}() {{
  dump("testing: {test_name}");

  let obji = {{'xxx':111}};
  let obj = {object};
  let expect = obj.b[0].c.g[obji]{field};

  let test_value = undefined;
  for (let i = 0; i < 50; i++) {{
    test_value = obj.b[0].c.g[obji]{field}
  }}

  if (isNaN(test_value)) {{
    yes(isNaN(expect));
  }} else {{
    yes(test_value === expect);
  }}
}}

"""

CASE_TEMPLATE_FIELD_SET="""

function {test_name}() {{
  dump("testing: {test_name}");

  let obji = {{'xxx':111}};
  let obj = {object};
  let val = {value};

  let test_value = undefined;
  obj.b[0].c.g[obji]{field} = val;
  let expect = obj.b[0].c.g[obji]{field};

  for (let i = 0; i < 50; i++) {{
      obj.b[0].c.g[obji]{field} = val;
      test_value = obj.b[0].c.g[obji]{field};
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

## foreach type, we assign some basic sample method, ie sample its certain
## types
SAMPLER=verify.make_sampler_list(TYPE)

class Gen:
    def __init__(self, des):
        self.d = des         # notes this already in json format
        self.output = []     # the generated code template
        self.test_list = []  # list of function name for testing

        self._comment = self.d.get("comment", "")
        self._name    = self.d["name"]
        self._type    = self.d["type"]
        self._fbd     = self.d.get("forbid", [])
        self._mode    = self.d.get("mode", "index")

        ## we only support unary or binary for now, notes for tracing jit,
        ## without stitch trace, no way to JIT branch, ie ternary is not
        ## working for us
        assert len(self._type) == 1

    def generate(self):
        if self._mode == "index":
            self._generate_index()
        elif self._mode == "field":
            self._generate_field()
        else:
            raise Error("invalid generator type")

    def _generate_field(self):
        global TYPE;
        global SAMPLER;
        global CASE_TEMPLATE_FIELD_GET;
        global CASE_TEMPLATE_FIELD_SET;
        global CASE_OBJECT_TEMPLATE;

        obj = CASE_OBJECT_TEMPLATE

        def _on(type, key, value):
            field=None

            if key == "literal":
                field = ".{}".format(value)
            else:
                field = "[{}]".format(value)

            test_name = 'test_get_{}_{}_{}'.format(self._name, type, key)
            code = CASE_TEMPLATE_FIELD_GET.format(
                test_name=test_name,
                object=obj,
                field=field)
            self.output.append(code)
            self.test_list.append(test_name)

            test_name = 'test_set_{}_{}_{}'.format(self._name, type, key)
            code = CASE_TEMPLATE_FIELD_SET.format(
                test_name=test_name,
                object=obj,
                field=field,
                value='{}')
            self.output.append(code)
            self.test_list.append(test_name)

        verify.foreach_unary(self._type[0], SAMPLER, self._fbd, _on)

    def _generate_index(self):
        global TYPE;
        global SAMPLER;
        global CASE_TEMPLATE_INDEX_GET;
        global CASE_TEMPLATE_INDEX_SET;

        obj_sample = ["[]", "[1]", "[1, 2]"]

        def _on(type, key, value):
            idx = 0
            for v in obj_sample:
                test_name = 'test_get_{}_{}_{}_{}'.format(self._name, type, key, idx)
                code = CASE_TEMPLATE_INDEX_GET.format(
                    test_name=test_name,
                    object=v,
                    index=value)
                self.output.append(code)
                self.test_list.append(test_name)

                test_name = 'test_set_{}_{}_{}_{}'.format(self._name, type, key, idx)
                code = CASE_TEMPLATE_INDEX_SET.format(
                    test_name=test_name,
                    object=v,
                    index=value,
                    value='{}')
                self.output.append(code)
                self.test_list.append(test_name)

                idx = idx + 1

        verify.foreach_unary(self._type[0], SAMPLER, self._fbd, _on)

PATH="./output/subidx_gen"
DSCP="./subidx_gen.json"

verify.generate(PATH, DSCP, Gen)
