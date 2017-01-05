# powturbo  (c) Copyright 2013-2016
# ----------- Downloading + Compiling ----------------------
# git clone --recursive git://github.com/powturbo/TurboPFor.git 
# make
#
# Minimum make: "make NCODEC1=1 NCODEC2=1 NTRANFORM=1" to compile only TurboPFor

# Linux: "export CC=clang" "export CXX=clang". windows mingw: "set CC=gcc" "set CXX=g++" or uncomment the CC,CXX lines
CC ?= gcc
CXX ?= g++
#CC=clang
#CXX=clang++

DDEBUG=-DNDEBUG -s
#DDEBUG=-g

MARCH=-march=native
#MARCH=-march=broadwell

ifeq ($(NSIMD),1)
DEFS+=-DNSIMD
NCODEC1=1
NCODEC2=1
else
NSIMD=0
endif

ifeq ($(AVX2),1)
MARCH+=-mavx2 -mbmi2
else
AVX2=0
endif

#----------------------------------------------
ifeq ($(OS),Windows_NT)
  UNAME := Windows
CC=gcc
CXX=g++
CFLAGS+=-D__int64_t=int64_t
else
  UNAME := $(shell uname -s)
ifeq ($(UNAME),$(filter $(UNAME),Linux Darwin FreeBSD GNU/kFreeBSD))
LDFLAGS+=-lpthread -lrt
endif
endif

ifeq ($(STATIC),1)
LDFLAGS+=-static
endif

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
ARCH=64
else
ifeq ($(ARCH),32)
CFLAGS=-fomit-frame-pointer
else
ARCH=64
endif
endif

#---------------------- make args --------------------------
ifeq ($(NCODEC1),1)
DEFS+=-DNCODEC1
else
NCODEC1=0
endif

ifeq ($(NCODEC2),1)
DEFS+=-DNCODEC2
else
NCODEC2=0
endif

ifeq ($(NTRANFORM),1)
DEFS+=-DNTRANSFORM
else
NTRANSFORM=0
endif

ifeq ($(LZTURBO),1)
DEFS+=-DLZTURBO
endif

CFLAGS+=$(DDEBUG) -w -Wall -std=gnu99 -DNDEBUG -DUSE_THREADS  -fstrict-aliasing -Iext -Iext/lz4/lib -Iext/simdcomp/include -Iext/MaskedVByte/include -Iext/LittleIntPacker/include -Iext/streamvbyte/include $(DEFS)
CXXFLAGS+=$(DDEBUG) $(MARCH) -std=gnu++0x -w -fpermissive -Wall -fno-rtti $(DEFS) -Iext/FastPFor/headers

all: icbench idxcr idxqry idxseg

cpp: vp4c.c
	$(CC) $(MARCH) -E vp4c.c

bitpack.o: bitpack.c bitpack.h bitpack64_.h
	$(CC) -O2 $(CFLAGS) $(MARCH) -c bitpack.c

varintg8iu.o: ext/varintg8iu.c ext/varintg8iu.h 
	$(CC) -O2 $(CFLAGS) $(MARCH) -c -std=c99 ext/varintg8iu.c

idxqryp.o: idxqry.c
	$(CC) -O3 $(CFLAGS) -c idxqry.c -o idxqryp.o

vp4c.o: vp4c.c
	$(CC) -O3 $(CFLAGS) -DP4NSIMD -c vp4c.c -o vp4c.o

vsimple.o: vsimple.c
	$(CC) -O2 $(CFLAGS) $(MARCH) -c vsimple.c

#-------------------------------------------------------------------
ifeq ($(NCODEC1), 0)
OB+=ext/streamvbyte/src/streamvbyte.o ext/streamvbyte/src/streamvbytedelta.o 
OB+=ext/MaskedVByte/src/varintencode.o ext/MaskedVByte/src/varintdecode.o
OB+=ext/simdcomp/src/simdintegratedbitpacking.o ext/simdcomp/src/simdcomputil.o ext/simdcomp/src/simdbitpacking.o ext/simdcomp/src/simdpackedselect.o 
OB+=ext/simdcomp_/simdfor.o 

ifeq ($(AVX2),1)
OB+=ext/simdcomp/src/avxbitpacking.o 
endif

OB+=ext/LittleIntPacker/src/bitpacking32.o ext/LittleIntPacker/src/turbobitpacking32.o ext/LittleIntPacker/src/scpacking32.o ext/LittleIntPacker/src/horizontalpacking32.o
ifeq ($(AVX2),1)
OB+=ext/LittleIntPacker/src/bmipacking32.o
endif

OB+=ext/libfor/for.o
#modified QMX for unaligned SIMD load/store
OB+=ext/bench_/bench/compress_qmx.o ext/bench_/bench/compress_qmx_v2.o ext/bench_/bench/compress_qmx_v3.o ext/bench_/bench/compress_qmx_v4.o
#OB+=ext/qmx.o
#OB+=ext/qmx/compress_qmx.o
OB+=ext/varintg8iu.o
OB+=ext/rc.o
endif

#----------------------------------------
ifeq ($(NCODEC2), 0)
ext/polycom/optpfd.o: ext/polycom/optpfd.c
	$(CC) -O2 $(MARCH) $(CFLAGS) $< -c -o $@ 

OB+=ext/polycom/optpfd.o
OB+=ext/polycom/polyvbyte.o

OB+=ext/FastPFor/src/bitpacking.o ext/FastPFor/src/simdbitpacking.o ext/FastPFor/src/simdunalignedbitpacking.o

ifeq ($(HAVE_ZLIB), 1)
CDEFS+=-DZLIB
ifeq ($(STATIC),1)
OB+=/usr/lib/x86_64-linux-gnu/libz.a
else
OB+=-lz
endif
else
#ZD=zlib/
#OB+=$(ZD)adler32.o $(ZD)crc32.o $(ZD)compress.o $(ZD)deflate.o $(ZD)infback.o $(ZD)inffast.o $(ZD)inflate.o $(ZD)inftrees.o $(ZD)trees.o $(ZD)uncompr.o $(ZD)zutil.o
endif

OB+=ext/lz4/lib/lz4hc.o ext/lz4/lib/lz4.o  
OB+=ext/bitshuffle/src/bitshuffle.o ext/bitshuffle/src/iochain.o ext/bitshuffle/src/bitshuffle_core.o 

ifeq ($(BLOSC),1)
LDFLAGS+=-lpthread 
CFLAGS+=-Iext/ -DSHUFFLE_SSE2_ENABLED
OB+=ext/c-blosc2/blosc/blosc.o ext/c-blosc2/blosc/blosclz.o ext/c-blosc2/blosc/schunk.o ext/c-blosc2/blosc/delta.o ext/c-blosc2/blosc/shuffle.o ext/c-blosc2/blosc/shuffle-generic.o ext/c-blosc2/blosc/shuffle-sse2.o \
ext/c-blosc2/blosc/bitshuffle-generic.o ext/c-blosc2/blosc/bitshuffle-sse2.o
endif

endif

OB+=eliasfano.o vsimple.o $(TRANSP) ext/simple8b.o transpose.o 
#------------------------
ICLIB=bitpack.o bitunpack.o vint.o vp4d.o vp4c.o bitutil.o 

ifeq ($(NSIMD),0)
ICLIB+=bitpack128v.o bitunpack128v.o

ifeq ($(AVX2),1)
ICLIB+=bitpack256v.o bitunpack256v.o vp4c256v.o vp4d256v.o
endif
endif
#---------------------
OB+=$(ICLIB)

icbench: $(OB) icbench.o plugins.o 
	$(CXX) $^ $(LDFLAGS) -o icbench

.c.o:
	$(CC) -O3 $(MARCH) $(CFLAGS) $< -c -o $@  

.cc.o:
	$(CXX) -O3 $(MARCH) $(CXXFLAGS)  $< -c -o $@ 

.cpp.o:
	$(CXX) -O3 $(MARCH) $(CXXFLAGS) $< -c -o $@ 

idxseg:   idxseg.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) -o idxseg

ifeq ($(UNAME), Linux)
para: CFLAGS += -DTHREADMAX=32	
para: idxqry
endif

idxcr:   idxcr.o $(ICLIB)  
	$(CC) $^ $(LDFLAGS) -o idxcr $(LFLAGS)

idxqry:  idxqry.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) $(LIBTHREAD) $(LIBRT) -o idxqry $(LFLAGS)

clean:
	@find . -type f -name "*\.o" -delete -or -name "*\~" -delete -or -name "core" -delete

cleanw:
	del /S ..\*.o
	del /S ..\*~

