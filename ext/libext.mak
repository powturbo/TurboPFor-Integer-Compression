#cpp: $(CPPF)
#	$(CC) -msse3 $(MSSE) $(MARCH) -w -E -P $(CPPF)

CFLAGS+=-Iext 
CXXFLAGS+=$(DDEBUG) -w -fpermissive -Wall -fno-rtti
CXXFLAGS+=$(SSE) -Iext/FastPFor/headers -std=gnu99 -DUSE_THREADS 

ifeq ($(LZ),1)
LZ4=1
#ZLIB=1
#ZSTD=1
BITSHUFFLE=1
#BLOSC=1
endif

ifeq ($(CODEC1),1)
CL=$(CXX)
FASTPFOR=1
MASKEDVBYTE=1
SIMDCOMP=1
SPDP=1
STREAMVBYTE=1
#VTENC=1
SIMPLE8B=1
XDEFS+=-DCODEC1
endif

ifeq ($(CODEC2),1)
LIBFOR=1
LITTLEINTPACKER=1
POLYCOM=1
#QMX=1
RICE=1
VARINTG8IU=1
XDEFS+=-DCODEC2
endif

#------------------------------------------------
# use TurboPFor "p4d1enc" instead of the default bitpack functions in "idxcr.c" and "idxqry.c" (inverted index demo)
ifeq ($(TURBOPFOR),1)
CFLAGS+=-D_TURBOPFOR
endif
#ifeq ($(SIMDH),1)
#LIB+=bitunpack128h.o
#endif
#----------------------------------------
icbench: $(OB) icbench.o plugins.o 
	$(CXX) $(XDEFS) $^ $(LDFLAGS) -o icbench

idx: idxcr idxqry idxseg

icb: icbench 

idxseg:   idxseg.o $(LIB)
	$(CC) $^ $(LDFLAGS) -o idxseg

ictest:   ictest.o $(LIB)
	$(CC) $^ $(LDFLAGS) -o ictest

ifeq ($(OS), Linux)
para: CFLAGS += -DTHREADMAX=32	
para: idxqry
endif

idxcr:   idxcr.o $(LIB)  
	$(CC) $^ $(LDFLAGS) -o idxcr $(LFLAGS)

idxqry:  idxqry.o $(LIB)
	$(CC) $^ $(LDFLAGS) $(LIBTHREAD) $(LIBRT) -o idxqry $(LFLAGS)

ext/polycom/optpfd.o: ext/polycom/optpfd.c
	$(CC) -O2 $(MARCH) $(CFLAGS) $< -c -o $@ 

varintg8iu.o: ext/varintg8iu.c ext/varintg8iu.h 
	$(CC) -O2 $(CFLAGS) $(MARCH) -c -std=c99 ext/varintg8iu.c

#-------------------------------------------------------------------
ifeq ($(BLOSC),1)
LDFLAGS+=-lpthread
CFLAGS+=-DBLOSC 

ext/c-blosc2/blosc/shuffle-sse2.o: ext/c-blosc2/blosc/shuffle-sse2.c
	$(CC) -O3 $(CFLAGS) -msse2 -c ext/c-blosc2/blosc/shuffle-sse2.c -o ext/c-blosc2/blosc/shuffle-sse2.o

ext/c-blosc2/blosc/shuffle-generic.o: ext/c-blosc2/blosc/shuffle-generic.c
	$(CC) -O3 $(CFLAGS) -c ext/c-blosc2/blosc/shuffle-generic.c -o ext/c-blosc2/blosc/shuffle-generic.o

ext/c-blosc2/blosc/shuffle-avx2.o: ext/c-blosc2/blosc/shuffle-avx2.c
	$(CC) -O3 $(CFLAGS) -mavx2 -c ext/c-blosc2/blosc/shuffle-avx2.c -o ext/c-blosc2/blosc/shuffle-avx2.o

ext/c-blosc2/blosc/shuffle-neon.o: ext/c-blosc2/blosc/shuffle-neon.c
	$(CC) -O3 $(CFLAGS) -flax-vector-conversions -c ext/c-blosc2/blosc/shuffle-neon.c -o ext/c-blosc2/blosc/shuffle-neon.o

ext/c-blosc2/blosc/bitshuffle-neon.o: ext/c-blosc2/blosc/bitshuffle-neon.c
	$(CC) -O3 $(CFLAGS) -flax-vector-conversions -c ext/c-blosc2/blosc/bitshuffle-neon.c -o ext/c-blosc2/blosc/bitshuffle-neon.o

OB+=ext/c-blosc2/blosc/blosc2.o ext/c-blosc2/blosc/blosclz.o ext/c-blosc2/blosc/shuffle.o ext/c-blosc2/blosc/shuffle-generic.o \
ext/c-blosc2/blosc/bitshuffle-generic.o ext/c-blosc2/blosc/btune.o ext/c-blosc2/blosc/fastcopy.o ext/c-blosc2/blosc/delta.o ext/c-blosc2/blosc/timestamp.o ext/c-blosc2/blosc/trunc-prec.o

ifeq ($(AVX2),1)
CFLAGS+=-DSHUFFLE_AVX2_ENABLED
OB+=ext/c-blosc2/blosc/shuffle-avx2.o ext/c-blosc2/blosc/bitshuffle-avx2.o
endif
ifeq ($(ARCH),aarch64)
CFLAGS+=-DSHUFFLE_NEON_ENABLED 
OB+=ext/c-blosc2/blosc/shuffle-neon.o ext/c-blosc2/blosc/bitshuffle-neon.o
else
CFLAGS+=-DSHUFFLE_SSE2_ENABLED 
OB+=ext/c-blosc2/blosc/bitshuffle-sse2.o ext/c-blosc2/blosc/shuffle-sse2.o
endif

else

ifeq ($(BITSHUFFLE),1)
CFLAGS+=-DBITSHUFFLE -Iext/bitshuffle/lz4 
#-DLZ4_ON
ifeq ($(ARCH),aarch64)
CFLAGS+=-DUSEARMNEON
else
ifeq ($(AVX2),1)
CFLAGS+=-DUSEAVX2
endif
endif

OB+=ext/bitshuffle/src/bitshuffle.o ext/bitshuffle/src/iochain.o ext/bitshuffle/src/bitshuffle_core.o
#OB+=ext/bitshuffle/lz4/lz4.o
endif

endif

ifeq ($(FASTPFOR),1)
XDEFS+=-DFASTPFOR
OB+=ext/FastPFor/src/bitpacking.o ext/FastPFor/src/simdbitpacking.o ext/FastPFor/src/simdunalignedbitpacking.o ext/fastpfor.o
endif

ifeq ($(LIBFOR),1)
XDEFS+=-DLIBFOR
OB+=ext/libfor/for.o
endif

ifeq ($(LITTLEINTPACKER),1)
XDEFS+=-Iext/LittleIntPacker/include -DLITTLEINTPACKER
OB+=ext/LittleIntPacker/src/bitpacking32.o ext/LittleIntPacker/src/turbobitpacking32.o ext/LittleIntPacker/src/scpacking32.o
OB+=ext/LittleIntPacker/src/horizontalpacking32.o
ifeq ($(AVX2),1)
OB+=ext/LittleIntPacker/src/bmipacking32.o
endif
endif

ifeq ($(LZ4),1)
XDEFS+=-DLZ4 -Ilz4/lib 
OB+=ext/lz4/lib/lz4hc.o ext/lz4/lib/lz4.o ext/lz4/lib/lz4frame.o ext/lz4/lib/xxhash.o
endif

ifeq ($(LZTURBO),1)
CFLAGS+=-DLZTURBO 
TURBORC=1
include lzturbo.mak
endif

ifeq ($(MASKEDVBYTE),1)
XDEFS+=-DMASKEVBYTE
CFLAGS+=-Iext/MaskedVByte/include 
OB+=ext/MaskedVByte/src/varintencode.o ext/MaskedVByte/src/varintdecode.o
endif

ifeq ($(QMX),1)
XDEFS+=-DQMX
OB+=ext/JASSv2/source/compress_integer_qmx_improved.o ext/JASSv2/source/asserts.o
endif

ifeq ($(RICE),1)
XDEFS+=-DRICE
OB+=ext/rc.o
endif

ifeq ($(SIMDCOMP),1)
XDEFS+=-Iext/simdcomp/include -DSIMDCOMP
OB+=ext/simdcomp/src/simdintegratedbitpacking.o ext/simdcomp/src/simdcomputil.o ext/simdcomp/src/simdbitpacking.o ext/simdcomp/src/simdpackedselect.o 
OB+=ext/simdcomp_/simdfor.o
ifeq ($(AVX2),1)
OB+=ext/simdcomp/src/avxbitpacking.o 
endif
endif

ifeq ($(SPDP),1)
XDEFS+=-DSPDP
endif

ifeq ($(STREAMVBYTE),1)
CFLAGS+=-Iext/streamvbyte/include -DSTREAMVBYTE
ifeq ($(ARCH),aarch64)
CFLAGS+=-D__ARM_NEON__
endif
SV=ext/streamvbyte/src/
OB+=$(SV)streamvbyte_decode.o $(SV)streamvbyte_encode.o $(SV)streamvbytedelta_encode.o $(SV)streamvbytedelta_decode.o $(SV)streamvbyte_0124_encode.o  $(SV)streamvbyte_0124_decode.o $(SV)streamvbyte_zigzag.o
endif

ifeq ($(TURBORC), 1)
XDEFS+=-DTURBORC
OB+=ext/TurboRC/turborcs.o ext/TurboRC/turborcss.o ext/TurboRC/turborcn.o
endif

ifeq ($(VARINTG8IU),1)
XDEFS+=-DVARINTG8IU
OB+=ext/varintg8iu.o
endif

ifeq ($(VBZ),1)
XDEFS+=-DVBZ 
CXXFLAGS+=-Iext/vbz_compression/third_party -Iext/vbz_compression/vbz -Iext/vbz_compression/third_party/stream_vbyte/include
OB+=ext/vbz_compression/vbz/vbz.o ext/vbz_compression/vbz/v0/vbz_streamvbyte.o ext/vbz_compression/vbz/v1/vbz_streamvbyte.o
endif

ifeq ($(VTENC),1)
XDEFS+=-DVTENC
OB+=ext/VTEnc/bits.o ext/VTEnc/bitstream.o ext/VTEnc/decode.o ext/VTEnc/encode.o ext/VTEnc/mem.o
endif

ifeq ($(ZLIB), 1)
XDEFS+=-DZLIB
ifeq ($(HAVE_ZLIB), 1)
ifeq ($(STATIC),1)
OB+=/usr/lib/x86_64-linux-gnu/libz.a
else
OB+=-lz
endif
else
ZD=ext/zlib/
OB+=$(ZD)adler32.o $(ZD)crc32.o $(ZD)compress.o $(ZD)deflate.o $(ZD)infback.o $(ZD)inffast.o $(ZD)inflate.o $(ZD)inftrees.o $(ZD)trees.o $(ZD)uncompr.o $(ZD)zutil.o
endif
endif

ifeq ($(ZSTD), 1)
CFLAGS+=-Iext/zstd/lib -Iext/zstd/lib/common
XDEFS+=-DZSTD
OB+=ext/zstd/lib/common/pool.o ext/zstd/lib/common/xxhash.o ext/zstd/lib/common/error_private.o \
    ext/zstd/lib/compress/hist.o ext/zstd/lib/compress/zstd_compress.o ext/zstd/lib/compress/zstd_compress_literals.o ext/zstd/lib/compress/zstd_compress_sequences.o ext/zstd/lib/compress/zstd_double_fast.o ext/zstd/lib/compress/zstd_fast.o ext/zstd/lib/compress/zstd_lazy.o ext/zstd/lib/compress/zstd_ldm.o ext/zstd/lib/compress/zstdmt_compress.o ext/zstd/lib/compress/zstd_opt.o \
    ext/zstd/lib/decompress/zstd_decompress.o ext/zstd/lib/decompress/zstd_decompress_block.o ext/zstd/lib/decompress/zstd_ddict.o ext/zstd/lib/compress/fse_compress.o ext/zstd/lib/common/fse_decompress.o ext/zstd/lib/compress/huf_compress.o ext/zstd/lib/decompress/huf_decompress.o ext/zstd/lib/common/zstd_common.o ext/zstd/lib/common/entropy_common.o ext/zstd/lib/compress/zstd_compress_superblock.o\
    ext/zstd/lib/decompress/huf_decompress_amd64.o
endif

ifeq ($(POLYCOM),1)
OB+=ext/polycom/optpfd.o
OB+=ext/polycom/polyvbyte.o
endif

ifeq ($(SIMPLE8B),1)
OB+=ext/simple8b.o 
endif

ifeq ($(EFANO),1)
OB+=eliasfano.o 
endif

CFLAGS+=$(XDEFS)
