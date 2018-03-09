# powturbo (c) Copyright 2013-2018
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
# make: "make CODEC1=1 CODEC2=1" 

# Linux: "export CC=clang" "export CXX=clang". windows mingw: "set CC=gcc" "set CXX=g++" or uncomment the CC,CXX lines
CC ?= gcc
CXX ?= g++
#CC=clang
#CXX=clang++

DDEBUG=-DNDEBUG 
#DDEBUG=-g

#Minimum SSE = Sandy Bridge,  AVX2 = haswell 
MSSE=-march=corei7-avx -mtune=corei7-avx
# -mno-avx -mno-aes (add for Pentium based Sandy bridge)
MAVX2=-march=haswell

# Minimum CPU architecture 
MARCH=-march=native
#MARCH=$(MSSE)
#MARCH=-march=broadwell 

ifeq ($(NSIMD),1)
DEFS+=-DNSIMD
else
CFLAGS+=-DUSE_SSE -mssse3
CXXFLAGS+=-DUSE_SSE
endif

ifeq ($(AVX2),1)
MARCH+=-mbmi2 -mavx2
CFLAGS+=-DUSE_AVX2
CXXFLAGS+=-DUSE_AVX2
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
LDFLAGS+=-lpthread -lrt -lm
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
ifeq ($(CODEC1),1)
DEFS+=-DCODEC1=1
CFLAGS+=-Iext/lz4/lib -Iext/simdcomp/include -Iext/MaskedVByte/include -Iext/LittleIntPacker/include -Iext/streamvbyte/include
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

CFLAGS+=$(DDEBUG) -w -Wall -std=gnu99 -DUSE_THREADS  -fstrict-aliasing -Iext $(DEFS)
CXXFLAGS+=$(DDEBUG) -w -fpermissive -Wall -fno-rtti -Iext/FastPFor/headers $(DEFS)

all: icbench idxcr idxqry idxseg icapp

cpp: $(CPPF)
	$(CC) -msse3 $(MSSE) $(MARCH) -w -E -P $(CPPF)

bitutil.o: bitutil.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -falign-loops=32 $< -c -o $@
#----------

vp4c.o: vp4c.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE -DUSE_AVX2 -falign-loops=32 -c vp4c.c -o vp4c.o

vp4c_sse.o: vp4c.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c vp4c.c -o vp4c_sse.o

vp4c_avx2.o: vp4c.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2) -c vp4c.c -o vp4c_avx2.o
#---
vp4d.o: vp4d.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE  -DUSE_AVX2 -falign-loops=32 -c vp4d.c -o vp4d.o

vp4d_sse.o: vp4d.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c vp4d.c -o vp4d_sse.o

vp4d_avx2.o: vp4d.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2)  -c vp4d.c -o vp4d_avx2.o
#------------
bitpack.o: bitpack.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE -falign-loops=32 -c bitpack.c -o bitpack.o

bitpack_sse.o: bitpack.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c bitpack.c -o bitpack_sse.o

bitpack_avx2.o: bitpack.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2) -c bitpack.c -o bitpack_avx2.o
#---
bitunpack.o: bitunpack.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -DUSE_SSE -falign-loops=32 -c bitunpack.c -o bitunpack.o

bitunpack_sse.o: bitunpack.c
	$(CC) -O3 $(CFLAGS) -DSSE2_ON $(MSSE) -c bitunpack.c -o bitunpack_sse.o

bitunpack_avx2.o: bitunpack.c
	$(CC) -O3 $(CFLAGS) -DAVX2_ON $(MAVX2) -c bitunpack.c -o bitunpack_avx2.o
#------------
fp.o: fp.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -falign-loops=32  -c fp.c

vint.o: vint.c
	$(CC) -O3 $(CFLAGS) $(MARCH) -falign-loops=32  -c vint.c

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
ifeq ($(QMX),1)
OB+=ext/JASSv2/source/compress_integer_qmx_improved.o ext/JASSv2/source/asserts.o
endif
OB+=ext/varintg8iu.o
OB+=ext/rc.o
OB+=ext/lz4/lib/lz4hc.o ext/lz4/lib/lz4.o  
LZ4=1
BITSHUFFLE=1
endif

#----------------------------------------
ifeq ($(CODEC2), 1)
ext/polycom/optpfd.o: ext/polycom/optpfd.c
	$(CC) -O2 $(MARCH) $(CFLAGS) $< -c -o $@ 

OB+=ext/polycom/optpfd.o
OB+=ext/polycom/polyvbyte.o

OB+=ext/FastPFor/src/bitpacking.o ext/FastPFor/src/simdbitpacking.o ext/FastPFor/src/simdunalignedbitpacking.o
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
ifeq ($(AVX2),1)
#CFLAGS+=-DUSEAVX2
endif
OB+=ext/bitshuffle/src/bitshuffle.o ext/bitshuffle/src/iochain.o ext/bitshuffle/src/bitshuffle_core.o
endif

endif

OB+=eliasfano.o vsimple.o $(TRANSP) transpose.o transpose_sse.o
ifeq ($(AVX2),1)
OB+=transpose_avx2.o 
endif

#------------------------
ICLIB=bitpack.o bitpack_sse.o bitunpack.o bitunpack_sse.o vp4c.o vp4c_sse.o vp4d.o vp4d_sse.o bitutil.o fp.o vint.o vsimple.o transpose.o transpose_sse.o ext/trlec.o ext/trled.o

ifeq ($(LZTURBO),1)
DEFS+=-DLZTURBO
OB+=../dev/lz/lz8c.o ../dev/lz/lzbc01.o ../lz/lz8c.o 
#../lz/lzbc.o ../lz/lz8d.o ../lz/lzbd.o
ICLIB+=../lz/lz8c0.o ../lz/lzbc0.o ../lz/lz8d.o 
#../lz/lzbd.o
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
	$(CC) -O2 $(MARCH) $(CFLAGS) $< -c -o $@  

.cc.o:
	$(CXX) -O3 $(MARCH) $(CXXFLAGS)  $< -c -o $@ 

.cpp.o:
	$(CXX) -O3 $(MARCH) $(CXXFLAGS) $< -c -o $@ 

idxseg:   idxseg.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) -o idxseg

ictest:   ictest.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) -o ictest

icapp:   icapp.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) -o icapp

ifeq ($(UNAME), Linux)
para: CFLAGS += -DTHREADMAX=32	
para: idxqry
endif

idxcr:   idxcr.o $(ICLIB)  
	$(CC) $^ $(LDFLAGS) -o idxcr $(LFLAGS)

idxqry:  idxqry.o $(ICLIB)
	$(CC) $^ $(LDFLAGS) $(LIBTHREAD) $(LIBRT) -o idxqry $(LFLAGS)

clean:
	@find . -type f -name "*\.o" -delete -or -name "*\~" -delete -or -name "core" -delete -or -name "icbench" -delete -or -name "idxqry" -delete
	@find . -type f -name "icbench" -delete -or -name "idxqry" -delete -or -name "idxseg" -delete -or -name "idxcr" -delete

cleanw:
	del /S ..\*.o
	del /S ..\*~

