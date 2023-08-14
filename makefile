# powturbo (c) Copyright 2013-2023
# 1 - Download or clone TurboPFor:
# git clone git://github.com/powturbo/TurboPFor.git 
# make
#
# 2 - include general purpose compression codecs
# git clone --recursive https://github.com/powturbo/TurboPFor-Integer-Compression.git
# make ICCODEC=1
# To benchmark external libraries:
# 1 - clone/download the external repository into the TurboPFor directoty
# 2 - activate the repository in the makefile lib/libext.mak
# 3 -       make: "make CODEC1=1 ICCODEC=1" 
#           or    "make CODEC1=1 CODEC2=1 ICCODEC=1"
# aarch64 cross compile 
# export CC=aarch64-linux-gnu-gcc

#ICCODEC=1
#AVX2=1
SSE=1
#-------------------------------------------------
CC ?= gcc
CXX ?= g++

#CC=powerpc64le-linux-gnu-gcc
CL = $(CC)
#DEBUG=-DDEBUG -g
DEBUG=-DNDEBUG
JAVA_HOME ?= /usr/lib/jvm/java-8-openjdk-amd64
PREFIX ?= /usr/local
DIRBIN ?= $(PREFIX)/bin
DIRINC ?= $(PREFIX)/include
DIRLIB ?= $(PREFIX)/lib
SRC ?= lib/

OPT=-fstrict-aliasing -fPIC

#------- OS/ARCH -------------------
ifneq (,$(filter Windows%,$(OS)))
  OS := Windows
  CC=gcc
  CXX=g++
#  CC=clang
  ARCH=x86_64
ifeq ($(ICCODEC),1) 
  LDFLAGS=-Wl,--stack,33554432
endif
else
  OS := $(shell uname -s)
  ARCH := $(shell uname -m)

ifneq (,$(findstring aarch64,$(CC)))
  ARCH = aarch64
else ifneq (,$(findstring arm64,$(ARCH)))
  ARCH = aarch64
else ifneq (,$(findstring iPhone,$(ARCH)))
  ARCH = aarch64
  CFLAGS=-DHAVE_MALLOC_MALLOC
else ifneq (,$(findstring powerpc64le,$(CC)))
  ARCH = ppc64le
endif
endif

ifeq ($(ARCH),ppc64le)
  _SSE=-D__SSSE3__
  CFLAGS=-mcpu=power9 -mtune=power9 $(_SSE)
else ifeq ($(ARCH),aarch64)
  CFLAGS=-march=armv8-a 
ifneq (,$(findstring clang, $(CC)))
  OPT+=-fomit-frame-pointer 
#-fmacro-backtrace-limit=0
endif
  _SSE=-march=armv8-a
else ifeq ($(ARCH),$(filter $(ARCH),x86_64))
# set minimum arch sandy bridge SSE4.1 + AVX
  _SSE=-march=corei7-avx -mtune=corei7-avx 
# _SSE+=-mno-avx -mno-aes
  _AVX2=-march=haswell
endif

ifeq (,$(findstring clang, $(CC)))
OPT+=-falign-loops 
endif

CFLAGS+=$(DEBUG) $(OPT) 
#CFLAGS+=-Wno-macro-redefined -Wno-incompatible-pointer-types -Wno-tautological-constant-out-of-range-compare -Wno-discarded-qualifiers
CFLAGS+=-w -Wall -pedantic
CXXFLAGS+=-w 
#-Wall -Wincompatible-pointer-types
ifeq ($(OS),$(filter $(OS),Linux GNU/kFreeBSD GNU OpenBSD FreeBSD DragonFly NetBSD MSYS_NT Haiku))
LDFLAGS+=-lrt -lm
endif

ifeq ($(STATIC),1)
CFLAGS+=-D_STATIC
LDFLAGS+=-static
endif

all: icapp 

$(SRC)bitutil_avx2.o: $(SRC)bitutil.c
	$(CC) -O3 -w $(_AVX2) $(OPT) -c $(SRC)bitutil.c -o $(SRC)bitutil_avx2.o

$(SRC)vp4c_avx2.o: $(SRC)vp4c.c
	$(CC) -O3 -w $(_AVX2) $(OPT) -c $(SRC)vp4c.c -o $(SRC)vp4c_avx2.o

$(SRC)vp4d_avx2.o: $(SRC)vp4d.c
	$(CC) -O3 -w $(_AVX2) $(OPT) -c $(SRC)vp4d.c -o $(SRC)vp4d_avx2.o

$(SRC)bitpack_avx2.o: $(SRC)bitpack.c
	$(CC) -O3 -w $(_AVX2) $(OPT) -c $(SRC)bitpack.c -o $(SRC)bitpack_avx2.o

$(SRC)bitunpack_avx2.o: $(SRC)bitunpack.c
	$(CC) -O3 -w $(_AVX2) $(OPT) -c $(SRC)bitunpack.c -o $(SRC)bitunpack_avx2.o

$(SRC)transpose_avx2.o: $(SRC)transpose.c
	$(CC) -O3 -w $(_AVX2) $(OPT) -c $(SRC)transpose.c -o $(SRC)transpose_avx2.o

-include lib/libext.mak

LIB=$(SRC)bic.o $(SRC)bitunpack.o $(SRC)bitpack.o $(SRC)bitutil.o $(SRC)eliasfano.o $(SRC)fp.o $(SRC)transpose.o $(SRC)transpose_.o $(SRC)trlec.o $(SRC)trled.o $(SRC)vp4c.o $(SRC)vp4d.o $(SRC)v8.o $(SRC)v8pack.o $(SRC)vint.o $(SRC)vsimple.o $(SRC)vbit.o 
ifeq ($(ARCH),x86_64)
LIB+=$(SRC)vp4c_avx2.o $(SRC)vp4d_avx2.o $(SRC)transpose_avx2.o $(SRC)bitpack_avx2.o $(SRC)bitunpack_avx2.o $(SRC)bitutil_avx2.o
else
CFLAGS+=-D_NAVX2
endif

ifeq ($(AVX2),1)
CFLAGS+=$(_AVX2)
else
ifeq ($(SSE),1) 
CFLAGS+=$(_SSE)
endif
endif

ifeq ($(ICCODEC),1) 
CFLAGS+=-D_ICCODEC
LIB+=$(SRC)iccodec.o
endif

libic.a: $(LIB)
	ar cr $@ $+

libic.so : $(LIB)
	$(CC) -shared $+ -o $@

JAVA_SUBDIR = java

jic.h: $(JAVA_SUBDIR)/jic.java
	# cp jic.h ..
	cd $(JAVA_SUBDIR) && javah -jni jic && cp jic.h ..

# $(JAVA_SUBDIR)/jic.class : $(JAVA_SUBDIR)/jic.java
# 	cd $(JAVA_SUBDIR) && javac jic.java

$(JAVA_SUBDIR)/libic.so : libic.a jic.h jic.c
	$(CC) -shared $(CFLAGS) -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux jic.c libic.a -o $(JAVA_SUBDIR)/libic.so

$(JAVA_SUBDIR)/jicbench : $(JAVA_SUBDIR)/jicbench.java $(JAVA_SUBDIR)/libic.so
	cd $(JAVA_SUBDIR) && javac jicbench.java && java -Djava.library.path=. jicbench

icapp: $(SRC)icapp.o libic.a $(OB)
	$(CL) $^ $(LDFLAGS) -o icapp

myapp: myapp.o libic.a
	$(CC) $^ $(LDFLAGS) -o myapp

mycpp: mycpp.o libic.a
	$(CXX) $^ $(LDFLAGS) -o mycpp

%.o: %.c
	$(CC) -O3 $(CFLAGS) $< -c -o $@  

.cc.o:
	$(CXX) -O2 $(CXXFLAGS) $< -c -o $@ 

.cpp.o:
	$(CXX) -O2 $(CXXFLAGS) $< -c -o $@ 

ifeq ($(OS),Windows)
clean:
	rm lib/*.o
	rm libic.a
	rm icapp.exe
else
clean:
	@find . -type f -name "*\.o" -delete -or -name "*\~" -delete -or -name "core" -delete -or -name "icbench" -delete -or -name "libic.a" -delete
	@find . -type f -name "icbench" -delete -or -name "idxqry" -delete -or -name "idxseg" -delete -or -name "idxcr" -delete -or -name "icapp" -delete
endif
