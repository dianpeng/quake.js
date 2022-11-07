#!/bin/bash

PWD=`pwd`
PLATFORM=""

function getPlatform() {
  temp=`uname`
  if [ "$temp" = "Linux" ];
  then
    PLATFORM="linux64"
  elif [ "$temp" = "Darwin" ];
  then
    PLATFORM="mac"
  else
    echo "Unsupported platform"
    exit -1
  fi
}

function getVersion() {
  echo $1 | awk '{                                  \
    l = length($1);                                 \
    for (i = l; i > 0; --i) {                       \
      ddd = substr($1, i, 1);                       \
      if (ddd == "-" || ddd == "_" || ddd == ".") { \
        break;                                      \
      }                                             \
    }                                               \
    print substr($1, i+1, (l -i));                  \
  }'
}

function addLink() {
  name=$1
  if [ -L "$name" ]; then
    unlink $1
  fi
  path="$(find $PWD -maxdepth 1 -type d -name $1-* -o -name $1_*)"

  echo "==========================================="
  echo "create link for package: ${name}"
  ver="$(getVersion $path)"
  echo "package ${name}'s version is ${ver}"
  echo "==========================================="

  ln -s $path $1
}

function emphasize() {
  echo "============================="
  echo "$1"
  echo "============================="
}

## normal CMake style build
function buildWithCMake() {
  extra=""
  if [[ ! -z "$CC"  ]]; then
    extra="${extra} -DCMAKE_CC_COMPILER=${CC}"
  fi

  if [[ ! -z "$CXX" ]]; then
    extra="${extra} -DCMAKE_CXX_COMPILER=${CXX}"
  fi

  emphasize "build $1"
  pushd $2
  mkdir cmake
  pushd cmake
  cmake .. ${extra} $3
  make -j8
  popd
  popd
}

function buildBoost() {
  emphasize "build boost"
  pushd boost/
  ./bootstrap.sh
  ./b2 link=static variant=release cxxflags="-std=c++17"
  popd
}

function buildJemalloc() {
  pushd jemalloc
  ./build.sh
  popd
}

function buildGperftools() {
  pushd gperftools 
  ./build.sh
  popd
}

function buildZydis() {
  pushd zydis
  ./build.sh
  pop zydis
}

function buildASMJIT() {
  pushd asmjit
  ./build.sh
  popd asmjit
}

## The libunwind |MUST| be built before the tcmalloc since the tcmalloc rely
## on libunwind build
function buildLibunwind() {
  pushd libunwind
  ./build.sh
  popd
}

function link() {
  pushd third-party
  addLink spag
  addLink boost
  addLink catch2
  addLink libunwind
  addLink jemalloc
  addLink gperftools
  addLink zydis
  addLink asmjit
  popd
}

function build() {
  pushd third-party
  buildBoost
  ## order matters here, libunwind bust be built before gperftools ------------
  buildLibunwind
  buildJemalloc
  buildGperftools
  buildZydis
  buildASMJIT
  popd
}

function checkDep() {
  if [ -d "./third-party" ]; then
    md5=`md5sum ./third-party.zip`
    if [ -f "./third-party/MD5" ]; then
      tar=`cat ./third-party/MD5`
      if [ "$md5" == "$tar" ]; then
        echo "already build, skipped"
        exit 0
      fi
    fi
  fi
}

function _main() {
  link
}

_main
