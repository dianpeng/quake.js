#!/bin/bash

## incrementally build the unittest, used for troubleshooting unittest and its
## building

UNITTEST_PATH=unittest/
UNITTEST_OUT=out/unittest/

TARGET=$(find $UNITTEST_PATH -type f -name '*.cc' | awk -f <(cat - <<-'EOF'
{
  len = length($0);
  print "out/"substr($0, 0, len - 3)".t"
}
EOF
) | sort | uniq)

COMPILE=$(find $UNITTEST_OUT -type f -name *.t | sort | uniq)

## calculate the diff
echo $TARGET | tr ' ' '\n' > /tmp/_1.txt
echo $COMPILE | tr ' ' '\n' > /tmp/_2.txt
DIFF_RAW=$(diff --unchanged-line-format="" --suppress-common-lines /tmp/_1.txt /tmp/_2.txt | tr '\n' ' ')

IFS=' ' read -r -a array <<< "$DIFF_RAW"
for arr in "${array[@]}"
do
  if [ -z "$arr" ]
  then
    echo "pass"
  else
    echo "compile $arr"
  fi
done


make SAN=ON $DIFF_RAW -j3
