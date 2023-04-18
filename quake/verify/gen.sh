#!/bin/bash


## foreach types of generator, performs the generation of test cases

./op_gen.py
./cmp_gen.py

## move the folder from here to unittest cases. We run this script on demand

rm -rf ../unittest/jit/case/gen
mv ./output ../unittest/jit/case/gen
