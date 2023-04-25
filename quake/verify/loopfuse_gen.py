#!/bin/env python

import json
import os
import verify

""" JSON description for nested loop fusion/permutation

{
  "iterator" : ["for(30)", "in(20)", "of(20)", "for(30)", "in(20)", "of(15)"],
  "before": [
    "list",
    "of",
    "code",
    "show up",
    "before",
    "loop"
  ],
  "after": [
    "list",
    "of",
    "code",
    "show up",
    "after",
    "loop"
  ]
}

"""

def parse_loop_sampler_key(k):
    par_start = k.index('(')
    par_end = k.index(')')

    key = k[:par_start]
    par = k[par_start+1:par_end]

    return (key, int(par))

NAME_IDX=0

def index_name():
    global NAME_IDX
    name = "iterator_{}".format(NAME_IDX)
    NAME_IDX = NAME_IDX + 1
    return name

class ForGenerator:
    def __init__(self, type, mode, tripcount):
        self._type = type
        self._mode = mode
        self._tripcount = tripcount

    def generate(self):
        if self._type == "for":
            return self._generate_for()
        elif self._type == "in":
            return self._generate_for_in()
        else:
            return self._generate_for_of()

    def _generate_for(self):
        name = index_name()
        return "for (let {iter} = 0; {iter} < {count}; {iter}++)".format(
            iter=name,
            count=self._tripcount
        )

    def _generate_foreach(self, keyword):
        name = index_name()

        if self._mode == "basic":
            return "for (let {name} {keyword} new Array({count}).fill(1))".format(
                name=name, 
                count=self._tripcount,
                keyword=keyword
            )

        elif self._mode == "literal":
            l = [str(x) for x in range(self._tripcount)]
            l = ','.join(l)
            l = '[' + l + ']'
            return "for (let {name} {keyword} {array})".format(
                name=name,
                array=l,
                keyword=keyword
            )

        elif self._mode == "object":
            l = ["{}" for x in range(self._tripcount)]
            l = ','.join(l)
            l = '[' + l + ']'
            return "for (let {name} {keyword} {array})".format(
                name=name,
                array=l,
                keyword=keyword
            )
        elif self._mode == "str":
            l = ["'hello world'" for x in range(self._tripcount)]
            l = ','.join(l)
            l = '[' + l + ']'
            return "for (let {name} {keyword} {array})".format(
                name=name,
                array=l,
                keyword=keyword
            )
        else:
            raise Exception("unknown mode {}".format(self._mode))

    def _generate_for_in(self):
        return self._generate_foreach("in")

    def _generate_for_of(self):
        return self._generate_foreach("of")

def new_for_generator(key):
    out = []
    parameter = parse_loop_sampler_key(key)

    if parameter[0] == 'for':
        out.append(ForGenerator('for', 'N/A', parameter[1]))
        return out

    if parameter[0] == 'in':
        out.append(ForGenerator('in', 'basic', parameter[1]))
        out.append(ForGenerator('in', 'literal', parameter[1]))
        out.append(ForGenerator('in', 'object', parameter[1]))
        out.append(ForGenerator('in', 'str', parameter[1]))
        return out

    if parameter[0] == 'of':
        out.append(ForGenerator('of', 'basic', parameter[1]))
        out.append(ForGenerator('of', 'literal', parameter[1]))
        out.append(ForGenerator('of', 'object', parameter[1]))
        out.append(ForGenerator('of', 'str', parameter[1]))
        return out

    raise Exception('{} unknown for generator'.format(key))

def indent(n):
    return ''.join(['  ' for x in range(n)])

def cat_string(string_or_list, ind):
    if isinstance(string_or_list, str) or isinstance(string_or_list, unicode):
        return "{}{}".format(ident(ind), string_or_list)
    else:
        oo = ["{}{}".format(indent(ind), x) for x in string_or_list]
        return "\n" + "".join(oo)

class Gen:
    def __init__(self, des):
        self.d = des         # notes this already in json format
        self.output = []     # the generated code template
        self.test_list = []  # list of function name for testing

        self._name = self.d["name"]
        self._iterator = self.d["iterator"]
        self._before  = self.d.get("before", "")
        self._inner = self.d.get("inner", "")
        self._after = self.d.get("after", "")

    def generate(self):
        rrr = self._generate_loop_body_list()
        loop_body_list = rrr[0]
        id_level = rrr[1]

        before = cat_string(self._before, 2)
        after = cat_string(self._after, 2)

        idx = 0
        for body in loop_body_list:
            name = "{}_{}".format(self._name, idx)
            code = """
  function {name} () {{
    {before}
{loop_body}
    {after}
  }}
""".format(name = name, before = before, loop_body = body, after=after)
            idx = idx + 1
            self.output.append(code)
            self.test_list.append(name)


    def _expand_next(self, l):
        if len(l) == 0:
            return None
        else:
            out = []
            top = l[0]
            gen_list = new_for_generator(top)
            for x in gen_list:
                nested = self._expand_next(l[1:])
                if nested is None:
                    out.append([x])
                else:
                    for y in nested:
                        sub = [x]
                        sub.extend(y)
                        out.append(sub)
            return out

    def _expand_iterator(self):
        return self._expand_next(self._iterator[0:])

    def _generate_loop_body_list(self):
        path = self._expand_iterator()
        list = []
        inner = cat_string(self._inner, len(path) + 2)

        for p in path:
            nested_level = 2
            out = []
            for pp in p:
                out.append("{}{} {{\n".format(
                    indent(nested_level),
                    pp.generate()
                ))
                nested_level = nested_level + 1

            nested_level = len(p) + 1
            out.append("{}\n".format(inner))

            for _ in p:
                out.append("{} }}\n".format(indent(nested_level)))
                nested_level = nested_level - 1

            list.append(''.join(out))

        return (list, len(path))

PATH="./output/loopfuse_gen"
DSCP="./loopfuse_gen.json"
verify.generate(PATH, DSCP, Gen)
