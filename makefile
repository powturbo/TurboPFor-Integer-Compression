# powturbo (c) Copyright 2013-2019
# Download or clone TurboPFor:
# git clone git://github.com/powturbo/TurboPFor.git 
# make
#
# To benchmark external libraries:
# git clone --recursive git://github.com/powturbo/TurboPFor.git 
#        make: "make CODEC1=1 CODEC2=1 LZ=1" 
# on arm make: "make CODEC1=1 LZ=1"

CC ?= gcc
CXX ?= g++

#CC=powerpc64le-linux-gnu-gcc
CL = $(CC)
#DEBUG=-DDEBUG -g

#------- OS/ARCH -------------------
ifneq (,$(filter Windows%,$(OS)))
  OS := Windows
  CC=gcc
  CXX=g++
  ARCH=x86_64
else
  OS := $(shell uname -s)
  ARCH := $(shell uname -m)

ifneq (,$(findstring aarch64,$(CC)))
  ARCH = aarch64
else ifneq (,$(findstring powerpc64le,$(CC)))
  ARCH = ppc64le
endif
endif

ifeq ($(ARCH),ppc64le)
  SSE=-D__SSSE3__
  CFLAGS=-mcpu=power9 -mtune=power9 $(SSE)
#  LDFLAGS+=-lm
else ifeq ($(ARCH),aarch64)
  CFLAGS+=-march=armv8-a 
ifneq (,$(findstring clang, $(CC)))
  CFLAGS+=-march=armv8-a -falign-loops -fomit-frame-pointer
else
  CFLAGS+=-march=armv8-a 
endif
  SSE=-march=armv8-a
else ifeq ($(ARCH),$(filter $(ARCH),ppc64le))
  CFLAGS=-march=native
  SSE=-mssse3
else ifeq ($(ARCH),$(filter $(ARCH),x86_64))
# set minimum arch sandy bridge w/o avx
  SSE=-march=corei7-avx -mtune=corei7-avx -mno-avx -mno-aes
  CFLAGS=$(SSE)
  AVX2=-march=haswell 
endif

ifeq (,$(findstring clang, $(CC)))
DEFS+=-falign-loops
endif

ifeq ($(OS),$(filter $(OS),Linux GNU/kFreeBSD GNU OpenBSD FreeBSD DragonFly NetBSD MSYS_NT Haiku))
LDFLAGS+=-lrt -lm
endif

CFLAGS+=$(DEBUG) -DPLAIN -w -Wall -falign-loops -fstrict-aliasing

# compiler supports float16
ifeq ($(FLOAT16),1)
CFLAGS+=-DUSE_FLOAT16
endif
ifeq ($(STATIC),1)
LDFLAGS+=-static
endif

all: icapp 

vp4c_sse.o: vp4c.c
	$(CC) -O3 -w $(SSE) -c vp4c.c -o vp4c_sse.o

vp4c_avx2.o: vp4c.c
	$(CC) -O3 -w $(AVX2) -c vp4c.c -o vp4c_avx2.o

vp4d_sse.o: vp4d.c
	$(CC) -O3 -w $(SSE) -c vp4d.c -o vp4d_sse.o

vp4d_avx2.o: vp4d.c
	$(CC) -O3 -w $(AVX2) -c vp4d.c -o vp4d_avx2.o

bitpack_sse.o: bitpack.c
	$(CC) -O3 -w $(SSE) -c bitpack.c -o bitpack_sse.o

bitpack_avx2.o: bitpack.c
	$(CC) -O3 -w $(AVX2) -c bitpack.c -o bitpack_avx2.o

bitunpack_sse.o: bitunpack.c
	$(CC) -O3 -w $(SSE) -c bitunpack.c -o bitunpack_sse.o

bitunpack_avx2.o: bitunpack.c
	$(CC) -O3 -w $(AVX2) -c bitunpack.c -o bitunpack_avx2.o

transpose_sse.o: transpose.c
	$(CC) -O3 -w $(SSE) -c transpose.c -o transpose_sse.o

transpose_avx2.o: transpose.c
	$(CC) -O3 -w $(AVX2) -c transpose.c -o transpose_avx2.o

-include ext/libext.mak

LIB=bitpack.o bitpack_sse.o bitunpack.o bitunpack_sse.o \
    vp4c.o vp4c_sse.o vp4d.o vp4d_sse.o \
	bitutil.o fp.o v8.o vint.o transpose.o transpose_sse.o trlec.o trled.o vsimple.o eliasfano.o
ifeq ($(ARCH),x86_64)
LIB+=bitpack_avx2.o bitunpack_avx2.o vp4c_avx2.o vp4d_avx2.o transpose_avx2.o
endif

icapp:   icapp.o $(LIB) $(OB)
	$(CL) $^ $(LDFLAGS) -o icapp

myapp:   myapp.o $(LIB)
	$(CC) $^ $(LDFLAGS) -o myapp

.c.o:
	$(CC) -O3 $(CFLAGS) $< -c -o $@  

.cc.o:
	$(CXX) -O2 $(CXXFLAGS) $< -c -o $@ 

.cpp.o:
	$(CXX) -O2 $(CXXFLAGS) $< -c -o $@ 

ifeq ($(OS),Windows_NT)
clean:
	del /S *.o
	del /S *~
else
clean:
	@find . -type f -name "*\.o" -delete -or -name "*\~" -delete -or -name "core" -delete -or -name "icbench" -delete -or -name "idxqry" -delete
	@find . -type f -name "icbench" -delete -or -name "idxqry" -delete -or -name "idxseg" -delete -or -name "idxcr" -delete -or -name "icapp" -delete
endif

