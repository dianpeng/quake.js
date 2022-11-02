CXX:=g++
MAKE:=make
RELEASE_OPTIMIZATION:=-O3 -g3
DEBUG_OPTIMIZATION:=-O0 -g3

## Some gcc/g++ diagnostic warning is notorius for false positive, so just
## disable them currently or switch to clang in the future
WARNINGS:=-Wall -Werror

ifeq (g++,$(findstring g++,$(CXX)))
  WARNINGS += -Wno-maybe-uninitialized
endif

ifeq (clang,$(findstring clang,$(CXX)))
  WARNINGS += -Wno-unused-command-line-argument
endif

SANITIZER:=-fsanitize=address
COVERAGE:=-ftest-coverage -fprofile-arcs
CXXFLAGS:=-static-libstdc++ -std=c++17 -frtti -rdynamic $(WARNINGS)

## ============================================================================
## setup the basic LIBC/C++ runtime's dependency
## ============================================================================
## stdc++fs is used to avoid linking issue, generating coredump, for g++ - 8.xx
## currently we are using g++-8
# LDFLAGS:=-lstdc++fs
## ============================================================================

TOPDIR=$(shell pwd)
QJSDIR=$(TOPDIR)/../
BUILDDIR=out
NAME=quake

## ============================================================================
## Source/Header detection
## ============================================================================
SOURCE:=$(shell find src/ -type f -name "*.cc")

_OBJECT:=${SOURCE:.cc=.o}
OBJECT:=$(foreach entry,$(_OBJECT),$(BUILDDIR)/$(entry))

_DEPS=${SOURCE:.cc=.deps}
DEPS=$(foreach entry,$(_DEPS),$(BUILDDIR)/deps/$(entry))

## include the dependency files if and only if we are not cleaning
ifneq (clean,$(MAKECMDGOALS))
-include $(DEPS)
endif

UTSOURCE=$(shell find unittest/ -type f -name "*.cc")
_UTBINARY=${UTSOURCE:.cc=.t}
UTBINARY=$(foreach entry,$(_UTBINARY),$(BUILDDIR)/$(entry))
_UTDEPS=${UTSOURCE:.cc=.deps}
UTDEPS=$(foreach entry,$(_UTDEPS),$(BUILDDIR)/deps/$(entry))

ifeq (utest,$(MAKECMDGOALS))
	-include $(UTDEPS)
endif

ifeq ($(NAME), $(MAKECMDGOALS))

ifeq (, $(MODE))
	OPT=RELEASE
endif

ifeq (TESTING, $(MODE))
	OPT=DEBUG
	CXXFLAGS += -DENABLE_TEST
endif

ifeq (RELEASE_TEST, $(MODE))
	opt=RELEASE
	CXXFLAGS += -DENABLE_TEST
endif

endif

## Allow building with sanitizer, ie "make target SAN=ON"
ifeq (ON,$(SAN))
	CXXFLAGS += $(SANITIZER) -DASAN_ON
endif

## Allow building with coverage support, ie "make target COV=ON"
ifeq (ON,$(COV))
	CXXFLAGS += $(COVERAGE) -DCOV_ON
endif

## ***************************************************
## Memory Allocator Hook
##   Notes, when the SANITIZER is on, the memory hook
##   is off by default to support ASAN
##w ***************************************************
ALLOCATOR_INCLUDE=
ALLOCATOR_LIBINC=
ALLOCATOR_LIB=
ALLOC=jemalloc

ifeq (,$(SAN))
ifeq (tcmalloc, $(ALLOC))
	ALLOCATOR_INCLUDE=-I$(THIRDPARTY)/gperftools/src/
	ALLOCATOR_LIBINC=-L$(THIRDPARTY)/gperftools/.libs/
	ALLOCATOR_LIB=-ltcmalloc
	CXXFLAGS += -DSPARROW_HAS_TCMALLOC
else
ifeq (jemalloc, $(ALLOC))
	ALLOCATOR_INCLUDE=-I$(THIRDPARTY)/jemalloc/include/
	ALLOCATOR_LIBINC=-L$(THIRDPARTY)/jemalloc/lib/
	ALLOCATOR_LIB=-ljemalloc
	CXXFLAGS += -DSPARROW_HAS_JEMALLOC
else
endif
endif
endif

## ***************************************************
## Include v8's special macro
## ***************************************************
CXXFLAGS+=$(V8_EMBED_MACRO)

## ***************************************************
## Include/3rd parties
## ***************************************************
THIRDPARTY=$(TOPDIR)/third-party

_INCLUDE=-I$(TOPDIR)/include               \
	-I$(THIRDPARTY)/boost/                   \
	-I$(THIRDPARTY)/openssl/include          \
	-I$(THIRDPARTY)/udns/                    \
	-I$(THIRDPARTY)/llhttp/include           \
	-I$(THIRDPARTY)/gperftools/src/          \
  -I$(THIRDPARTY)/prometheus/core/include  \
	-I$(THIRDPARTY)/linenoise/               \
	-I$(THIRDPARTY)/kafel/include            \
	-I$(THIRDPARTY)/libiconv/include         \
	-I$(THIRDPARTY)/icu/source/common/       \
	-I$(THIRDPARTY)/asmjit/src/              \
	-I$(THIRDPARTY)/zydis/include/           \
	-I$(THIRDPARTY)/zydis/dependencies/zycore/include \
	$(ALLOCATOR_INCLUDE)                     \
	-I$(QJSDIR)                              \
	-I$(THIRDPARTY)/span/ 

CXXFLAGS+=$(_INCLUDE)

_LIBRARYINCLUDE=                       \
	-L$(THIRDPARTY)/boost/stage/lib      \
	-L$(THIRDPARTY)/openssl/             \
	-L$(THIRDPARTY)/udns/                \
	-L$(THIRDPARTY)/llhttp/              \
	-L$(THIRDPARTY)/libunwind/lib/       \
	-L$(THIRDPARTY)/gperftools/.libs/    \
  -L$(THIRDPARTY)/prometheus/build/lib \
	-L$(THIRDPARTY)/linenoise/           \
	-L$(THIRDPARTY)/kafel/               \
	-L$(THIRDPARTY)/libiconv/lib/.libs/  \
  -L$(THIRDPARTY)/icu/source/lib/      \
	-L$(THIRDPARTY)/zydis/build/         \
	-L$(THIRDPARTY)/asmjit/build/        \
	$(ALLOCATOR_LIBINC)                  \
	-L$(QJSDIR)/

CXXFLAGS+=$(_LIBRARYINCLUDE)

_LIBRARYLINK=-ldl                    \
						 -pthread                \
						 -lm                     \
						 -lssl                   \
						 -lcrypto                \
						 -ludns                  \
						 -lllhttp                \
						 -lz                     \
						 -lboost_program_options \
             -lboost_context         \
						 -lprofiler              \
             -lprometheus-cpp-core   \
						 -llinenoise             \
						 -lkafel                 \
						 -liconv                 \
						 -licuuc                 \
						 -licudata               \
						 -lasmjit                \
						 -lZydis                 \
						 $(ALLOCATOR_LIB)        \
						 -lunwind                \
						 -lquickjs               \
					 	 -lrt

LDFLAGS+=$(_LIBRARYLINK)

## ============================================================================
## Arguments handling
##
##   We try to get whether the build type, listed below
##     
##     1. DEBUG
##     2. RELEASE
##    
## Based on these 2 types, specific addition to CXXFLAGS
## and LDFLAGS will be modified
##
## ============================================================================
ifeq (RELEASE,$(OPT))
	CXXFLAGS+=$(RELEASE_OPTIMIZATION)
else
	CXXFLAGS+=$(DEBUG_OPTIMIZATION)
endif

g.DEFAULT_GOAL=all

## ============================================================================
## Runtime building
## ============================================================================
all : object

object : $(OBJECT)

## ============================================================================
## Binary
## ============================================================================
$(NAME): quake/main.cc object
	$(CXX) $(CXXFLAGS) $< -L$(TOPDIR)/$(BUILDDIR) $(OBJECT) $(LDFLAGS) \
  -o $(BUILDDIR)/$@

## ============================================================================
## unittest
## ============================================================================
# utest: CXXFLAGS=$(_UT_CXXFLAGS)
# utest: LDFLAGS=$(_UT_LDFLAGS)

utest: $(UTBINARY) object

unittest/%.t : out/unittest/%.t

## ============================================================================
## Assets building
## ============================================================================
$(BUILDDIR)/deps/src/%.deps : src/%.cc
	build/mkdir.sh $@
	$(CXX) $(CXXFLAGS) -MM -MT                      \
		'$(patsubst src/%.cc,$(BUILDDIR)/src/%.o,$<)' \
		$< -MF $@

$(BUILDDIR)/deps/unittest/%.deps : unittest/%.cc
	build/mkdir.sh $@
	$(CXX) $(CXXFLAGS) -MM -MT                                \
		'$(patsubst unittest/%.cc,$(BUILDDIR)/unittest/%.t,$<)' \
		$< -MF $@

$(BUILDDIR)/src/%.o : src/%.cc
	build/mkdir.sh $@
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# ------------------- unittest ------------------------
_UT_CXXFLAGS:=-DENABLE_TEST                   \
	            -DBASE_DEBUG                    \
						  -I$(TOPDIR)/                    \
							-I$(THIRDPARTY)/catch2/include  \
						  -L$(TOPDIR)/$(BUILDDIR) $(CXXFLAGS)

## order matters, needs to link quake first
_UT_LDFLAGS:=$(LDFLAGS)

$(BUILDDIR)/unittest/%.t: CXXFLAGS=$(_UT_CXXFLAGS)
$(BUILDDIR)/unittest/%.t: LDFLAGS=$(_UT_LDFLAGS)
$(BUILDDIR)/unittest/%.t : unittest/%.cc object
	build/mkdir.sh $@
	$(CXX) $(CXXFLAGS) $(OBJECT) -include unittest/x.h $< $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(EXTERNAL_ASSETS)

.PHONY: clean
