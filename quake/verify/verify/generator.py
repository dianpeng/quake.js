#!/bin/env python

import os
import json

def _generate_one(output_path, jd, generator):
    code = []

    ## for each case
    g = generator(jd)
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
    f = open("{}/test_{}.js".format(output_path, jd["name"]), "w+")
    f.write(code)
    f.close()

def generate(output_path, json_description, generator):
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    f = open(json_description)
    j = json.load(f)
    code = []
    for x in j:
        if isinstance(x, str) or isinstance(x, unicode):
            continue
        _generate_one(output_path, x, generator)
