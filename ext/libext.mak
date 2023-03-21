#cpp: $(CPPF)
#	$(CC) -msse3 $(MSSE) $(MARCH) -w -E -P $(CPPF)

CFLAGS+=-Iext 
CXXFLAGS+=$(DDEBUG) -w -fpermissive -Wall -fno-rtti
CXXFLAGS+=$(_SSE) -Iext/FastPFor/headers -std=gnu99 -DUSE_THREADS 

ifeq ($(ICCODEC),1)
LZ4=1
ZSTD=1
#FSE=1
#FSEHUF=1
#ZLIB=1
BITSHUFFLE=1
#LZTURBO=1
TURBORC=1
endif

ifeq ($(CODEC1),1)
CL=$(CXX)
BLOSC=1
FASTPFOR=1
MASKEDVBYTE=1
MESHOPT=1
#SIMDCOMP=1
SPDP=1
#STREAMVBYTE=1
#VTENC=1
SIMPLE8B=1
ZFP=1
#ZLIB=1
LIBROUNDFAST=1
BITGROOMING=1
endif

ifeq ($(CODEC2),1)
LIBFOR=1
LITTLEINTPACKER=1
POLYCOM=1
QMX=1
RICE=1
VARINTG8IU=1
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
CFLAGS+=-D_BLOSC -Iext/c-blosc2/include -Iext/c-blosc2/include/blosc2 -Iext/lz4/lib -DHAVE_ZSTD

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
ext/c-blosc2/blosc/bitshuffle-generic.o ext/c-blosc2/blosc/stune.o ext/c-blosc2/blosc/fastcopy.o ext/c-blosc2/blosc/delta.o ext/c-blosc2/blosc/blosc2-stdio.o ext/c-blosc2/blosc/timestamp.o ext/c-blosc2/blosc/trunc-prec.o

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
CFLAGS+=-D_BITSHUFFLE -Iext/bitshuffle/lz4 
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
CFLAGS+=-D_FASTPFOR
OB+=ext/FastPFor/src/simdbitpacking.o ext/FastPFor/src/simdunalignedbitpacking.o ext/fastpfor.o ext/FastPFor/src/bitpacking.o
endif

ifeq ($(LIBROUNDFAST),1)
CFLAGS+=-D_LIBROUNDFAST
endif

ifeq ($(BITGROOMING),1)
CFLAGS+=-D_BITGROOMING
endif

ifeq ($(LIBFOR),1)
CFLAGS+=-D_LIBFOR
OB+=ext/libfor/for.o
endif

ifeq ($(LITTLEINTPACKER),1)
CFLAGS+=-D_LITTLEINTPACKER -Iext/LittleIntPacker/include 
OB+=ext/LittleIntPacker/src/bitpacking32.o ext/LittleIntPacker/src/turbobitpacking32.o ext/LittleIntPacker/src/scpacking32.o
OB+=ext/LittleIntPacker/src/horizontalpacking32.o
ifeq ($(AVX2),1)
OB+=ext/LittleIntPacker/src/bmipacking32.o
endif
endif

ifeq ($(LZ4),1)
CFLAGS+=-D_LZ4 -Ilz4/lib 
OB+=ext/lz4/lib/lz4hc.o ext/lz4/lib/lz4.o ext/lz4/lib/lz4frame.o ext/lz4/lib/xxhash.o

endif

ifeq ($(LZTURBO),1)
CFLAGS+=-D_LZTURBO
LDFLAGS+=-lpthread
include lzturbo.mak
endif

ifeq ($(MASKEDVBYTE),1)
CFLAGS+=-D_MASKEVBYTE -Iext/MaskedVByte/include

ext/MaskedVByte/src/varintencode.o: ext/MaskedVByte/src/varintencode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c ext/MaskedVByte/src/varintencode.c -o ext/MaskedVByte/src/varintencode.o

ext/MaskedVByte/src/varintdecode.o: ext/MaskedVByte/src/varintdecode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c ext/MaskedVByte/src/varintdecode.c -o ext/MaskedVByte/src/varintdecode.o

OB+=ext/MaskedVByte/src/varintencode.o ext/MaskedVByte/src/varintdecode.o
endif

ifeq ($(MESHOPT), 1)
CFLAGS+=-D_MESHOPT -Iext/meshoptimizer/src
M=ext/meshoptimizer/src
OB+=$(M)/allocator.o $(M)/clusterizer.o $(M)/indexcodec.o $(M)/indexgenerator.o $(M)/meshoptimizer.h $(M)/overdrawanalyzer.o $(M)/overdrawoptimizer.o $(M)/simplifier.o $(M)/spatialorder.o \
 $(M)/stripifier.o $(M)/vcacheanalyzer.o $(M)/vcacheoptimizer.o $(M)/vertexcodec.o $(M)/vertexfilter.o $(M)/vfetchanalyzer.o $(M)/vfetchoptimizer.o
endif

ifeq ($(QMX),1)
CFLAGS+=-D_QMX
OB+=ext/JASSv2/source/compress_integer_qmx_improved.o ext/JASSv2/source/asserts.o
endif

ifeq ($(RICE),1)
CFLAGS+=-D_RICE
OB+=ext/rc.o
endif

ifeq ($(SIMDCOMP),1)
CFLAGS+=-D_SIMDCOMP -Iext/simdcomp/include 
OB+=ext/simdcomp/src/simdintegratedbitpacking.o ext/simdcomp/src/simdcomputil.o ext/simdcomp/src/simdbitpacking.o ext/simdcomp/src/simdpackedselect.o 
OB+=ext/simdcomp_/simdfor.o
ifeq ($(AVX2),1)
OB+=ext/simdcomp/src/avxbitpacking.o 
endif
endif

ifeq ($(SPDP),1)
XDEFS+=-D_SPDP
endif

ifeq ($(STREAMVBYTE),1)
CFLAGS+=-D_STREAMVBYTE -Iext/streamvbyte/include 
ifeq ($(ARCH),aarch64)
CFLAGS+=-D__ARM_NEON__
endif
SV=ext/streamvbyte/src/

$(SV)streamvbyte_encode.o: $(SV)streamvbyte_encode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c $(SV)streamvbyte_encode.c -o $(SV)streamvbyte_encode.o

$(SV)streamvbyte_decode.o: $(SV)streamvbyte_decode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c $(SV)streamvbyte_decode.c -o $(SV)streamvbyte_decode.o

$(SV)streamvbytedelta_encode.o: $(SV)streamvbytedelta_encode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c $(SV)streamvbytedelta_encode.c -o $(SV)streamvbytedelta_encode.o

$(SV)streamvbytedelta_decode.o: $(SV)streamvbytedelta_decode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c $(SV)streamvbytedelta_decode.c -o $(SV)streamvbytedelta_decode.o
	
$(SV)streamvbyte_zigzag.o: $(SV)streamvbyte_zigzag.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c $(SV)streamvbyte_zigzag.c -o $(SV)streamvbyte_zigzag.o

OB+=$(SV)streamvbyte_decode.o $(SV)streamvbyte_encode.o $(SV)streamvbytedelta_encode.o $(SV)streamvbytedelta_decode.o $(SV)streamvbyte_zigzag.o
endif

ifeq ($(TURBORC), 1)
CFLAGS+=-D_TURBORC
T=ext/Turbo-Range-Coder/
OB+=$(T)rc_s.o $(T)rc_ss.o $(T)rcutil.o $(T)libsais/src/libsais.o $(T)rccm_s.o $(T)rccm_ss.o $(T)bec_b.o $(T)rcbwt_s.o $(T)rcbwt_ss.o
#$(T)rcqlfc_s.o $(T)rcqlfc_ss.o $(T)rcbwt.o 
endif

ifeq ($(VARINTG8IU),1)
CFLAGS+=-D_VARINTG8IU
OB+=ext/varintg8iu.o
endif

ifeq ($(VBZ),1)
CFLAGS+=-D_VBZ 
CXXFLAGS+=-Iext/vbz_compression/third_party -Iext/vbz_compression/vbz -Iext/vbz_compression/third_party/stream_vbyte/include
OB+=ext/vbz_compression/vbz/vbz.o ext/vbz_compression/vbz/v0/vbz_streamvbyte.o ext/vbz_compression/vbz/v1/vbz_streamvbyte.o
endif

ifeq ($(VTENC),1)
CFLAGS+=-D_VTENC
OB+=ext/VTEnc/bits.o ext/VTEnc/bitstream.o ext/VTEnc/decode.o ext/VTEnc/encode.o ext/VTEnc/mem.o
endif

ifeq ($(ZLIB), 1)
CFLAGS+=-D_ZLIB
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
CFLAGS+=-D_ZSTD -Iext/zstd/lib -Iext/zstd/lib/common
OB+=ext/zstd/lib/common/pool.o ext/zstd/lib/common/xxhash.o ext/zstd/lib/common/error_private.o \
    ext/zstd/lib/compress/hist.o ext/zstd/lib/compress/zstd_compress.o ext/zstd/lib/compress/zstd_compress_literals.o ext/zstd/lib/compress/zstd_compress_sequences.o ext/zstd/lib/compress/zstd_double_fast.o ext/zstd/lib/compress/zstd_fast.o ext/zstd/lib/compress/zstd_lazy.o ext/zstd/lib/compress/zstd_ldm.o ext/zstd/lib/compress/zstdmt_compress.o ext/zstd/lib/compress/zstd_opt.o \
    ext/zstd/lib/decompress/zstd_decompress.o ext/zstd/lib/decompress/zstd_decompress_block.o ext/zstd/lib/decompress/zstd_ddict.o ext/zstd/lib/compress/fse_compress.o ext/zstd/lib/common/fse_decompress.o ext/zstd/lib/compress/huf_compress.o ext/zstd/lib/decompress/huf_decompress.o ext/zstd/lib/common/zstd_common.o ext/zstd/lib/common/entropy_common.o ext/zstd/lib/compress/zstd_compress_superblock.o\
    ext/zstd/lib/decompress/huf_decompress_amd64.o ext/zstd/lib/dictBuilder/zdict.o ext/zstd/lib/dictBuilder/fastcover.o ext/zstd/lib/dictBuilder/cover.o ext/zstd/lib/dictBuilder/divsufsort.o
endif

ifeq ($(FSE), 1)
CFLAGS+=-D_FSE
endif

ifeq ($(FSEHUF), 1)
CFLAGS+=-D_FSEHUF
endif

ifeq ($(ZFP), 1)
CFLAGS+=-D_ZFP -Iext/zfp/include
F=ext/zfp/src/
OB+=$(F)bitstream.o $(F)decode1d.o $(F)decode1f.o $(F)decode1i.o $(F)decode1l.o $(F)decode2d.o $(F)decode2f.o $(F)decode2i.o $(F)decode2l.o $(F)decode3d.o $(F)decode3f.o $(F)decode3i.o $(F)decode3l.o \
 $(F)decode4d.o $(F)decode4f.o $(F)decode4i.o $(F)decode4l.o $(F)encode1d.o $(F)encode1f.o $(F)encode1i.o $(F)encode1l.o $(F)encode2d.o $(F)encode2f.o $(F)encode2i.o $(F)encode2l.o $(F)encode3d.o \
 $(F)encode3f.o $(F)encode3i.o $(F)encode3l.o $(F)encode4d.o $(F)encode4f.o $(F)encode4i.o $(F)encode4l.o $(F)zfp.o
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
