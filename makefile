# powturbo (c) Copyright 2013-2019
# ----------- Downloading + Compiling ----------------------
# Download or clone TurboPFor:
# git clone git://github.com/powturbo/TurboPFor.git 
# make
#
# To compile icapp with LZ4 and BITSHUFFLE. (lz4+Bitshuffle muss be in directory ext)
# make icapp LZ4=1 or make LZ4=1 BITSHUFFLE=1
#
# To benchmark external libraries also:
# git clone --recursive git://github.com/powturbo/TurboPFor.git 
#        make: "make CODEC1=1 CODEC2=1" 
# on arm make: "make CODEC1=1" 

# Linux: "export CC=clang" "export CXX=clang". windows mingw: "set CC=gcc" "set CXX=g++" or uncomment the CC,CXX lines
CC ?= gcc
CXX ?= g++
#CC=clang-8
#CXX=clang++-8

#CC = gcc-8
#CXX = g++-8

#CC=powerpc64le-linux-gnu-gcc
#CXX=powerpc64le-linux-gnu-g++

DDEBUG=-DNDEBUG -s
#DDEBUG=-g

ifneq (,$(filter Windows%,$(OS)))
  OS := Windows
CFLAGS+=-D__int64_t=int64_t
CC=gcc
CXX=g++
else
  OS := $(shell uname -s)
  ARCH := $(shell uname -m)
ifneq (,$(findstring powerpc64le,$(CC)))
  ARCH = ppc64le
endif
ifneq (,$(findstring aarch64,$(CC)))
  ARCH = aarch64
endif
endif

#------ ARMv8 
ifeq ($(ARCH),aarch64)
ifneq (,$(findstring clang, $(CC)))
MSSE=-O3 -march=armv8-a -mcpu=cortex-a72 -falign-loops -fomit-frame-pointer 
else
MSSE=-O3 -march=armv8-a -mcpu=cortex-a72 -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer
endif

else
# ----- Power9
ifeq ($(ARCH),ppc64le)
MSSE=-D__SSE__ -D__SSE2__ -D__SSE3__ -D__SSSE3__
MARCH=-march=power9 -mtune=power9
CFLAGS+=-DNO_WARN_X86_INTRINSICS
CXXFLAGS+=-DNO_WARN_X86_INTRINSICS
#------ x86_64 : minimum SSE = Sandy Bridge,  AVX2 = haswell 
else
MSSE=-march=corei7-avx -mtune=corei7-avx
# -mno-avx -mno-aes (add for Pentium based Sandy bridge)
CFLAGS+=-mssse3
MAVX2=-march=haswell
endif
endif

ifeq ($(OS),$(filter $(OS),Linux Darwin GNU/kFreeBSD GNU OpenBSD FreeBSD DragonFly NetBSD MSYS_NT Haiku))
LDFLAGS+=-lpthread -lm
ifneq ($(OS),Darwin)
LDFLAGS+=-lrt
endif
endif

# Minimum CPU architecture 
#MARCH=-march=native
MARCH=$(MSSE)

ifeq ($(NSIMD),1)
DEFS+=-DNSIMD
else
CFLAGS+=-DUSE_SSE 
CXXFLAGS+=-DUSE_SSE
endif

ifeq ($(AVX2),1)
MARCH+=-mbmi2 -mavx2
CFLAGS+=-DUSE_AVX2
CXXFLAGS+=-DUSE_AVX2
else
AVX2=0
endif

ifeq ($(CODEC1),1)
LZ4=1
BITSHUFFLE=1
endif

ifeq ($(FLOAT16),1)
CFLAGS+=-DUSE_FLOAT16
endif

#----------------------------------------------
ifeq ($(STATIC),1)
LDFLAGS+=-static
endif

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
WORDSISE=64
else
ifeq ($(WORDSIZE),32)
CFLAGS=-fomit-frame-pointer
else
WORDSIZE=64
endif
endif

#---------------------- make args --------------------------
ifeq ($(CODEC1),1)
DEFS+=-DCODEC1=1
CFLAGS+=-Iext/lz4/lib -Iext/simdcomp/include -Iext/MaskedVByte/include -Iext/LittleIntPacker/include -Iext/streamvbyte/include -Iext/streamvbyte/src
endif

ifeq ($(CODEC2),1)
DEFS+=-DCODEC2=1
endif

ifeq ($(BLOSC),1)
DEFS+=-DBLOSC
endif

ifeq ($(QMX),1)
DEFS+=-DQMX
else
CXXFLAGS+=-std=gnu++0x
endif

ifeq ($(LZ4),1)
CFLAGS+=-DLZ4 -Ilz4/lib 
endif

ifeq ($(BITSHUFFLE),1)
CFLAGS+=-DBITSHUFFLE -Iext/bitshuffle/lz4
endif

ifeq ($(TURBOPFOR),1)
# use TurboPFor "p4d1enc" instead of the default bitpack functions in "idxcr.c" and "idxqry.c" (inverted index demo)
DEFS+=-D_TURBOPFOR
endif

CFLAGS+=$(DDEBUG) -w -Wall -std=gnu99 -DUSE_THREADS  -fstrict-aliasing -Iext $(DEFS)
CXXFLAGS+=$(DDEBUG) -w -fpermissive -Wall -fno-rtti -Iext/FastPFor/headers $(DEFS)

all: icbench idxcr idxqry idxseg icapp

cpp: $(CPPF)
	$(CC) -msse3 $(MSSE) $(MARCH) -w -E -P $(CPPF)

bitutil.o: bitutil.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -falign-loops $< -c -o $@
#----------

vp4c.o: vp4c.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE -DUSE_AVX2 -falign-loops -c vp4c.c -o vp4c.o

vp4c_sse.o: vp4c.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c vp4c.c -o vp4c_sse.o

vp4c_avx2.o: vp4c.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2) -c vp4c.c -o vp4c_avx2.o
#---
vp4d.o: vp4d.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE  -DUSE_AVX2 -falign-loops -c vp4d.c -o vp4d.o

vp4d_sse.o: vp4d.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c vp4d.c -o vp4d_sse.o

vp4d_avx2.o: vp4d.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2)  -c vp4d.c -o vp4d_avx2.o
#------------
bitpack.o: bitpack.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE -falign-loops -c bitpack.c -o bitpack.o

bitpack_sse.o: bitpack.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c bitpack.c -o bitpack_sse.o

bitpack_avx2.o: bitpack.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2) -c bitpack.c -o bitpack_avx2.o
#---
bitunpack.o: bitunpack.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE -falign-loops -c bitunpack.c -o bitunpack.o

bitunpack_sse.o: bitunpack.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c bitunpack.c -o bitunpack_sse.o

bitunpack_avx2.o: bitunpack.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2) -c bitunpack.c -o bitunpack_avx2.o
#------------
fp.o: fp.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -falign-loops  -c fp.c

vint.o: vint.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -falign-loops  -c vint.c

v8.o: v8.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -falign-loops  -c v8.c

vsimple.o: vsimple.c
	$(CC) -O2 $(CFLAGS) $(MARCH) -c vsimple.c
	
eliasfano.o: eliasfano.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -c eliasfano.c
#-----------
transpose.o: transpose.c
	$(CC) -O3 $(CFLAGS) -c -DUSE_SSE transpose.c -o transpose.o

transpose_sse.o: transpose.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c transpose.c -o transpose_sse.o

transpose_avx2.o: transpose.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2) -c transpose.c -o transpose_avx2.o
#---------
idxqryp.o: idxqry.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -c idxqry.c -o idxqryp.o

varintg8iu.o: ext/varintg8iu.c ext/varintg8iu.h 
	$(CC) -O2 $(CFLAGS) $(MARCH) -c -std=c99 ext/varintg8iu.c

#-------------------------------------------------------------------
ifeq ($(CODEC1), 1)
ifeq ($(ARCH),aarch64)
CFLAGS+=-D__ARM_NEON__
endif
OB+=ext/libfor/for.o

OB+=ext/LittleIntPacker/src/bitpacking32.o ext/LittleIntPacker/src/turbobitpacking32.o ext/LittleIntPacker/src/scpacking32.o

SV=ext/streamvbyte/src/
XLIB+=$(SV)streamvbyte_decode.o $(SV)streamvbyte_encode.o $(SV)streamvbytedelta_encode.o $(SV)streamvbytedelta_decode.o $(SV)streamvbyte_0124_encode.o  $(SV)streamvbyte_0124_decode.o $(SV)streamvbyte_zigzag.o

OB+=ext/rc.o

OB+=ext/lz4/lib/lz4hc.o ext/lz4/lib/lz4.o  
LZ4=1
BITSHUFFLE=1

ifeq ($(ARCH),aarch64)
#OB+=ext/LittleIntPacker/src/util.o
else
# Only x86
OB+=ext/LittleIntPacker/src/horizontalpacking32.o
XLIB+=ext/MaskedVByte/src/varintencode.o ext/MaskedVByte/src/varintdecode.o
OB+=ext/simdcomp/src/simdintegratedbitpacking.o ext/simdcomp/src/simdcomputil.o ext/simdcomp/src/simdbitpacking.o ext/simdcomp/src/simdpackedselect.o 
OB+=ext/simdcomp_/simdfor.o

ifeq ($(AVX2),1)
OB+=ext/simdcomp/src/avxbitpacking.o 
OB+=ext/LittleIntPacker/src/bmipacking32.o
endif

ifeq ($(QMX),1)
OB+=ext/JASSv2/source/compress_integer_qmx_improved.o ext/JASSv2/source/asserts.o
endif
OB+=ext/varintg8iu.o
endif
endif

#----------------------------------------
ifeq ($(CODEC2), 1)
OB+=eliasfano.o vsimple.o $(TRANSP) transpose.o transpose_sse.o

OB+=ext/bitshuffle/src/bitshuffle.o ext/bitshuffle/src/iochain.o ext/bitshuffle/src/bitshuffle_core.o

ifneq ($(ARCH),aarch64)
XLIB+=ext/FastPFor/src/bitpacking.o ext/FastPFor/src/simdbitpacking.o ext/FastPFor/src/simdunalignedbitpacking.o ext/fastpfor.o
endif

ext/polycom/optpfd.o: ext/polycom/optpfd.c
	$(CC) -O2 $(MARCH) $(CFLAGS) $< -c -o $@ 

OB+=ext/polycom/optpfd.o
OB+=ext/polycom/polyvbyte.o
OB+=ext/simple8b.o 

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

ifeq ($(ARCH),aarch64)
else

ifeq ($(BLOSC),1)
LDFLAGS+=-lpthread 
#ext/c-blosc/blosc/libblosc.a
CFLAGS+=-Iext/ -DSHUFFLE_SSE2_ENABLED -DBLOSC -DPREFER_EXTERNAL_LZ4=ON -DHAVE_LZ4 -DHAVE_LZ4HC
#-DHAVE_ZLIB 
ifeq ($(AVX2),1)
CFLAGS+=-DSHUFFLE_AVX2_ENABLED
OB+=ext/c-blosc/blosc/shuffle-avx2.o ext/c-blosc/blosc/bitshuffle-avx2.o
endif
OB+=ext/c-blosc/blosc/blosc.o ext/c-blosc/blosc/blosclz.o ext/c-blosc/blosc/shuffle.o ext/c-blosc/blosc/shuffle-generic.o ext/c-blosc/blosc/shuffle-sse2.o \
ext/c-blosc/blosc/bitshuffle-generic.o ext/c-blosc/blosc/bitshuffle-sse2.o

#OB+=ext/c-blosc2/blosc/delta.o ext/c-blosc2/blosc/schunk.o 
else
endif
endif

ifeq ($(AVX2),1)
OB+=transpose_avx2.o 
endif
endif

OB+=$(XLIB)
#------------------------
ICLIB=bitpack.o bitpack_sse.o bitunpack.o bitunpack_sse.o vp4c.o vp4c_sse.o vp4d.o vp4d_sse.o bitutil.o fp.o v8.o vint.o vsimple.o transpose.o transpose_sse.o trlec.o trled.o eliasfano.o

ifeq ($(LZTURBO),1)
include lzturbo.mak
endif

ifeq ($(LZ4),1)
ICLIB+=ext/lz4/lib/lz4hc.o ext/lz4/lib/lz4.o
endif

ifeq ($(BITSHUFFLE),1)
ICLIB+=ext/bitshuffle/src/bitshuffle.o ext/bitshuffle/src/iochain.o ext/bitshuffle/src/bitshuffle_core.o
ifeq ($(LZ4),1)
else
ICLIB+=ext/bitshuffle/lz4/lz4.o
endif
endif

ifeq ($(AVX2),1)
ICLIB+=bitpack_avx2.o bitunpack_avx2.o vp4c_avx2.o vp4d_avx2.o transpose_avx2.o
endif

ifeq ($(SIMDH),1)
ICLIB+=bitunpack128h.o
endif
#---------------------
OB+=$(ICLIB)

icbench: $(OB) icbench.o plugins.o 
	$(CXX) $^ $(LDFLAGS) -o icbench

.c.o:
	$(CC) -O3 $(MARCH) $(CFLAGS) $< -c -o $@  

.cc.o:
	$(CXX) -O2 $(MARCH) $(CXXFLAGS)  $< -c -o $@ 

.cpp.o:
	$(CXX) -O2 $(MARCH) $(CXXFLAGS) $< -c -o $@ 

idxseg:   idxseg.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) -o idxseg

ictest:   ictest.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) -o ictest

icapp:   icapp.o $(ICLIB) $(XLIB)
	$(CXX) $^ $(LDFLAGS) -o icapp

ifeq ($(OS), Linux)
para: CFLAGS += -DTHREADMAX=32	
para: idxqry
endif

idxcr:   idxcr.o $(ICLIB)  
	$(CC) $^ $(LDFLAGS) -o idxcr $(LFLAGS)

idxqry:  idxqry.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) $(LIBTHREAD) $(LIBRT) -o idxqry $(LFLAGS)

clean:
	@find . -type f -name "*\.o" -delete -or -name "*\~" -delete -or -name "core" -delete -or -name "icbench" -delete -or -name "idxqry" -delete
	@find . -type f -name "icbench" -delete -or -name "idxqry" -delete -or -name "idxseg" -delete -or -name "idxcr" -delete -or -name "icapp" -delete

cleanw:
	del /S ..\*.o
	del /S ..\*~

