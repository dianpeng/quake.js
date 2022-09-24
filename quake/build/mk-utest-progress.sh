#!/bin/bash

### progress indicator for making the freaking unittests, which typically uses
### many resources and times

PWD=`pwd`
TEST_DIR="$PWD/unittest"
OUT_DIR="$PWD/out/unittest"

SLP=2
BARCNT=50
CURRENT=""
TOTAL=$(find $TEST_DIR -type f -name "*.cc" | wc -l)

function current() {
  CURRENT=$(find $OUT_DIR -type f -name "*.t" | wc -l)
}

function print_progress() {
  tt=$((CURRENT * BARCNT))
  cnt=$((tt / TOTAL))
  for i in $(seq 1 1 $cnt)
  do
    echo -n "#"
  done
  echo -ne " ($CURRENT/$TOTAL)\r"
}

function progress() {
  while :
  do
    current
    print_progress
    sleep $SLP
  done
}

progress
