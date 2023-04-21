#!/bin/env python

""" Sampler

Sampler is just a wrapper to generate some instance for test case generation

"""

def _min_max_zero_normal(min, max, zero, normal, nan = None, ninf=None, pinf=None):
    out = {
        'min': min,
        'max': max,
        'zero': zero,
        'normal': normal
    }

    if nan is not None:
        out['nan'] = nan

    if ninf is not None:
        out['ninf'] = ninf

    if pinf is not None:
        out['pinf'] = pinf

    return out

def _init_f64(o):
    o['f64'] = _min_max_zero_normal(
        -1.79769313486231570e+308,
        1.79769313486231570e+308,
        2.2250738585072014E-308,
        13.0,
        'NaN',
        'Infinity',
        '-Infinity'
    )

def _init_i32(o):
    o['i32'] = _min_max_zero_normal(-2147483648, 2147483647, 0, 14)

def _init_u32(o):
    o['u32'] = _min_max_zero_normal(0, 4294967295, 0, 15)

def _init_bool(o):
    o['bool'] = _min_max_zero_normal('false', 'true', 'false', 'true')

def _init_undefined(o):
    o['undefined'] = _min_max_zero_normal('undefined', 'undefined', 'undefined', 'undefined')

def _init_null(o):
    o['null'] = _min_max_zero_normal('null', 'null', 'null', 'null')

def _init_undefined_or_null(o):
    o['undefined_or_null'] = _min_max_zero_normal('undefined', 'null', 'undefined', 'null')

def _init_string(o):
    o['string'] = _min_max_zero_normal('\'\'', '\''+'a'*1024+'\'', '\'\'', '\'aaaabbccc\'')

def _init_stri32(o):
    o['stri32'] = _min_max_zero_normal(
            '\'-2147483648\'',
            '\'2147483647\'',
            '\'1\'',
            '\'1000\'')

def _init_strf64(o):
    o['strf64'] = _min_max_zero_normal(
            '\'-2.2250738585072014E-308\'',
            '\'1.79769313486231570e+308\'',
            '\'2.2250738585072014E-308\'',
            '\'13.2\'')

def _init_object(o):
    o['object'] = _min_max_zero_normal(
        '{}',
        '{\'a\':1}',
        '{}',
        '{\'a\':100}')

initializer = {
    'i32': _init_i32,
    'f64': _init_f64,
    'bool': _init_bool,
    'undefined': _init_undefined,
    'null': _init_null,
    "undefined_or_null": _init_undefined_or_null,
    'string': _init_string,
    'object': _init_object,
    'stri32': _init_stri32,
    'strf64': _init_strf64
}

def make_sampler_list(type_list):
    out = dict()

    for x in type_list:
        if x in initializer:
            init = initializer[x]
            init(out)

    return out

def foreach_unary(type_list, sampler_list, taboo, foo):
    for tp in type_list:
        sampler = None
        if tp not in sampler_list:
            sampler = {'literal': tp}
        else:
            sampler = sampler_list[tp]

        for sample_key in sampler:
            if sample_key in taboo:
                continue
            sample_value = sampler[sample_key]
            foo(tp, sample_key, sample_value)

def foreach_binary(lhs_type_list, lhs_sampler_list, rhs_type_list, rhs_sampler_list, taboo, foo):
    for lhs_tp in lhs_type_list:
        lhs_sampler = None
        rhs_sampler = None

        if lhs_tp not in lhs_sampler_list:
            lhs_sampler = {'literal': lhs_tp}
        else:
            lhs_sampler = lhs_sampler_list[lhs_tp]


        for rhs_tp in rhs_type_list:
            if rhs_tp not in rhs_sampler_list:
                rhs_sampler = {'literal': rhs_tp}
            else:
                rhs_sampler = rhs_sampler_list[rhs_tp]

            for lhs_key in lhs_sampler:
                if lhs_key in taboo:
                    continue

                for rhs_key in rhs_sampler:
                    if rhs_key in taboo:
                        continue

                    lhs_value = lhs_sampler[lhs_key]
                    rhs_value = rhs_sampler[rhs_key]

                    foo(lhs_tp, lhs_key, lhs_value, rhs_tp, rhs_key, rhs_value)
