#!/bin/bash

## used for generating the version of the build. Notes we embed the several
## build information into the binary for ease of been visible externally
## ------------------------------------------------------------------------
## 1. build commits
## 2. version
##    Notes the version is been fed into the include/version.in file which
##    will be used as input to generate include/version.h when build

TARGET="include/version.in"
OUTPUT="include/version.h"

COMMITS_INFO=""
VER_MAJOR=0
VER_MINOR=0
VER_BUILD=0
VER_PATCH=0
VER_ISDEV=1
VER_STRING="0.0.0.0[!!dev!!]"

function show {
  echo "==========================================="
  echo "$@"
  echo "==========================================="
}

function fail {
  show "$@"
  exit 255
}

function warn {
  show "warn: $@"
}

function set_build_commits {
  COMMITS_INFO=`git log -1 --pretty="%h %cD %cn % s"`

  [[ $? -eq 0 ]] || fail "cannot get git log for build commits information"
}

## function to get the major/minor/build/patch level version number from the
## tagging name. Notes the tagging name must be in a specific form otherwise
## the parsing will not work

function get_major {
  echo "$@" | awk -F"." '{print $1}'
}

function get_minor {
  echo "$@" | awk -F"." '{print $2}'
}

function get_build {
  echo "$@" | awk -F"." '{print $3}'
}

function get_patch {
  echo "$@" | awk -F"." '{if ($4 == "") print "0"; else print $4;}'
}

function set_version {
  xx=`git describe --tags 2>/dev/null`
  if [[ $? -eq 0 ]]; then
    V0="tag-"
    V1="release-"
    V2="v-"

    xx=${xx/#$V0}
    xx=${xx/#$V1}
    xx=${xx/#$V2}
    
    xx=`echo $xx | awk -F"-" '{print $1}'`

    show "version tag: $xx"

    VER_MAJOR=$(get_major "$xx")
    VER_MINOR=$(get_minor "$xx")
    VER_BUILD=$(get_build "$xx")
    VER_PATCH=$(get_patch "$xx")
    VER_ISDEV=0
    VER_STRING="$VER_MAJOR.$VER_MINOR.$VER_BUILD.$VER_PATCH"
  else
    warn "version is not setup, default to dev"
  fi
}

function replace {
  cp $TARGET $OUTPUT
  sed -i "s/@@VER_MAJOR@@/$VER_MAJOR/g" $OUTPUT
  sed -i "s/@@VER_MINOR@@/$VER_MINOR/g" $OUTPUT
  sed -i "s/@@VER_BUILD@@/$VER_BUILD/g" $OUTPUT
  sed -i "s/@@VER_PATCH@@/$VER_PATCH/g" $OUTPUT
  sed -i "s/@@VER_ISDEV@@/$VER_ISDEV/g" $OUTPUT
  sed -i "s/@@VER_STRING@@/$VER_STRING/g" $OUTPUT
  echo "#define SPARROW_COMMITS_INFO R\"----($COMMITS_INFO)----\"" > include/sparrow-commits-info.inl
}

set_build_commits
set_version
replace

