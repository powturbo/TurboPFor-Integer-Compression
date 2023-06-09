#cpp: $(CPPF)
#	$(CC) -msse3 $(MSSE) $(MARCH) -w -E -P $(CPPF)

CFLAGS+=-Iext 
CXXFLAGS+=$(DDEBUG) -w -fpermissive -Wall -fno-rtti
CXXFLAGS+=$(_SSE) -I$(LB)FastPFor/headers -std=gnu99 -DUSE_THREADS 

ifeq ($(ICCODEC),1)
LZ4=1
ZSTD=1
FSE=1
FSEHUF=1
#ZLIB=1
BITSHUFFLE=1
#LZTURBO=1
TURBORC=1
ANS=1
endif

ifeq ($(CODEC1),1)
CL=$(CXX)
BLOSC=1
FASTPFOR=1
MASKEDVBYTE=1
MESHOPT=1
#SIMDCOMP=1
SPDP=1
STREAMVBYTE=1
#VTENC=1
SIMPLE8B=1
ZFP=1
#ZLIB=1
LIBROUNDFAST=1
BITGROOMING=1
QCOMPRESS=1
endif

ifeq ($(CODEC2),1)
LIBFOR=1
LITTLEINTPACKER=1
POLYCOM=1
QMX=1
RICE=1
VARINTG8IU=1
endif

LB=lib/ext/
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

idxseg:   $(SRC)idxseg.o libic.a
	$(CC) $^ $(LDFLAGS) -o idxseg

ictest:   ictest.o $(LIB)
	$(CC) $^ $(LDFLAGS) -o ictest

ifeq ($(OS), Linux)
para: CFLAGS += -DTHREADMAX=32	
para: idxqry
endif

idxcr:   $(SRC)idxcr.o libic.a  
	$(CC) $^ $(LDFLAGS) -o idxcr $(LFLAGS)

idxqry:  $(SRC)idxqry.o libic.a
	$(CC) $^ $(LDFLAGS) $(LIBTHREAD) $(LIBRT) -o idxqry $(LFLAGS)

$(LB)polycom/optpfd.o: $(LB)polycom/optpfd.c
	$(CC) -O2 $(MARCH) $(CFLAGS) $< -c -o $@ 

varintg8iu.o: $(LB)varintg8iu.c $(LB)varintg8iu.h 
	$(CC) -O2 $(CFLAGS) $(MARCH) -c -std=c99 $(LB)varintg8iu.c

#-------------------------------------------------------------------
ifeq ($(BLOSC),1)
LDFLAGS+=-lpthread
CFLAGS+=-D_BLOSC -I$(LB)c-blosc2/include -I$(LB)c-blosc2/include/blosc2 -I$(LB)c-blosc2/blosc -Iext/lz4/lib -DHAVE_ZSTD -DHAVE_PLUGINS

$(LB)c-blosc2/blosc/shuffle-sse2.o: $(LB)c-blosc2/blosc/shuffle-sse2.c
	$(CC) -O3 $(CFLAGS) -msse2 -c $(LB)c-blosc2/blosc/shuffle-sse2.c -o $(LB)c-blosc2/blosc/shuffle-sse2.o

$(LB)c-blosc2/blosc/shuffle-generic.o: $(LB)c-blosc2/blosc/shuffle-generic.c
	$(CC) -O3 $(CFLAGS) -c $(LB)c-blosc2/blosc/shuffle-generic.c -o $(LB)c-blosc2/blosc/shuffle-generic.o

$(LB)c-blosc2/blosc/shuffle-avx2.o: lib/$(LB)c-blosc2/blosc/shuffle-avx2.c
	$(CC) -O3 $(CFLAGS) -mavx2 -c $(LB)c-blosc2/blosc/shuffle-avx2.c -o $(LB)c-blosc2/blosc/shuffle-avx2.o

$(LB)c-blosc2/blosc/shuffle-neon.o: $(LB)c-blosc2/blosc/shuffle-neon.c
	$(CC) -O3 $(CFLAGS) -flax-vector-conversions -c $(LB)c-blosc2/blosc/shuffle-neon.c -o $(LB)c-blosc2/blosc/shuffle-neon.o

$(LB)c-blosc2/blosc/bitshuffle-neon.o: $(LB)c-blosc2/blosc/bitshuffle-neon.c
	$(CC) -O3 $(CFLAGS) -flax-vector-conversions -c $(LB)c-blosc2/blosc/bitshuffle-neon.c -o $(LB)c-blosc2/blosc/bitshuffle-neon.o

OB+=$(LB)c-blosc2/blosc/blosc2.o $(LB)c-blosc2/blosc/blosclz.o $(LB)c-blosc2/blosc/shuffle.o $(LB)c-blosc2/blosc/shuffle-generic.o \
$(LB)c-blosc2/blosc/bitshuffle-generic.o $(LB)c-blosc2/blosc/stune.o $(LB)c-blosc2/blosc/fastcopy.o $(LB)c-blosc2/blosc/delta.o $(LB)c-blosc2/blosc/blosc2-stdio.o \
   $(LB)c-blosc2/blosc/timestamp.o $(LB)c-blosc2/blosc/trunc-prec.o $(LB)c-blosc2/plugins/filters/bytedelta/bytedelta.o $(LB)c-blosc2/plugins/filters/filters-registry.o \
   $(LB)c-blosc2/plugins/filters/ndcell/ndcell.o $(LB)c-blosc2/plugins/plugin_utils.o $(LB)c-blosc2/plugins/filters/ndmean/ndmean.o $(LB)c-blosc2/plugins/codecs/codecs-registry.o \
   $(LB)c-blosc2/plugins/codecs/zfp/blosc2-zfp.o $(LB)c-blosc2/plugins/codecs/ndlz/ndlz.o $(LB)c-blosc2/plugins/codecs/ndlz/ndlz8x8.o $(LB)c-blosc2/plugins/codecs/ndlz/ndlz4x4.o
   
ifeq ($(AVX2),1)
CFLAGS+=-DSHUFFLE_AVX2_ENABLED
OB+=$(LB)c-blosc2/blosc/shuffle-avx2.o $(LB)c-blosc2/blosc/bitshuffle-avx2.o
endif
ifeq ($(ARCH),aarch64)
CFLAGS+=-DSHUFFLE_NEON_ENABLED 
OB+=$(LB)c-blosc2/blosc/shuffle-neon.o $(LB)c-blosc2/blosc/bitshuffle-neon.o
else
CFLAGS+=-DSHUFFLE_SSE2_ENABLED 
OB+=$(LB)c-blosc2/blosc/bitshuffle-sse2.o $(LB)c-blosc2/blosc/shuffle-sse2.o
endif

else

ifeq ($(BITSHUFFLE),1)
CFLAGS+=-D_BITSHUFFLE -I$(LB)bitshuffle/lz4 
#-DLZ4_ON
ifeq ($(ARCH),aarch64)
CFLAGS+=-DUSEARMNEON
else
ifeq ($(AVX2),1)
CFLAGS+=-DUSEAVX2
endif
endif

OB+=$(LB)bitshuffle/src/bitshuffle.o $(LB)bitshuffle/src/iochain.o $(LB)bitshuffle/src/bitshuffle_core.o
#OB+=$(LB)bitshuffle/lz4/lz4.o
endif

endif

ifeq ($(BITGROOMING),1)
CFLAGS+=-D_BITGROOMING
endif

ifeq ($(EFANO),1)
OB+=eliasfano.o 
endif

ifeq ($(FASTPFOR),1)
CFLAGS+=-D_FASTPFOR
OB+=$(LB)FastPFor/src/simdbitpacking.o $(LB)FastPFor/src/simdunalignedbitpacking.o $(LB)fastpfor.o $(LB)FastPFor/src/bitpacking.o
endif

ifeq ($(LIBROUNDFAST),1)
CFLAGS+=-D_LIBROUNDFAST
endif

ifeq ($(LIBFOR),1)
CFLAGS+=-D_LIBFOR
OB+=$(LB)libfor/for.o
endif

ifeq ($(LITTLEINTPACKER),1)
CFLAGS+=-D_LITTLEINTPACKER -I$(LB)LittleIntPacker/include 
OB+=$(LB)LittleIntPacker/src/bitpacking32.o $(LB)LittleIntPacker/src/turbobitpacking32.o $(LB)LittleIntPacker/src/scpacking32.o
OB+=$(LB)LittleIntPacker/src/horizontalpacking32.o
ifeq ($(AVX2),1)
OB+=$(LB)LittleIntPacker/src/bmipacking32.o
endif
endif

ifeq ($(LZ4),1)
CFLAGS+=-D_LZ4 -I$(LB)lz4/lib 
OB+=$(LB)lz4/lib/lz4hc.o $(LB)lz4/lib/lz4.o $(LB)lz4/lib/lz4frame.o $(LB)lz4/lib/xxhash.o

endif

ifeq ($(LZTURBO),1)
CFLAGS+=-D_LZTURBO
LDFLAGS+=-lpthread
include lib/lzturbo.mak
endif

ifeq ($(MASKEDVBYTE),1)
CFLAGS+=-D_MASKEVBYTE -I$(LB)MaskedVByte/include

$(LB)MaskedVByte/src/varintencode.o: $(LB)MaskedVByte/src/varintencode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c $(LB)MaskedVByte/src/varintencode.c -o $(LB)MaskedVByte/src/varintencode.o

$(LB)MaskedVByte/src/varintdecode.o: $(LB)MaskedVByte/src/varintdecode.c
	$(CC) -O3 -w $(_SSE) $(CFLAGS) -c $(LB)MaskedVByte/src/varintdecode.c -o $(LB)MaskedVByte/src/varintdecode.o

OB+=$(LB)MaskedVByte/src/varintencode.o $(LB)MaskedVByte/src/varintdecode.o
endif

ifeq ($(MESHOPT), 1)
CFLAGS+=-D_MESHOPT -I$(LB)meshoptimizer/src
M=$(LB)meshoptimizer/src
OB+=$(M)/allocator.o $(M)/clusterizer.o $(M)/indexcodec.o $(M)/indexgenerator.o $(M)/meshoptimizer.h $(M)/overdrawanalyzer.o $(M)/overdrawoptimizer.o $(M)/simplifier.o $(M)/spatialorder.o \
 $(M)/stripifier.o $(M)/vcacheanalyzer.o $(M)/vcacheoptimizer.o $(M)/vertexcodec.o $(M)/vertexfilter.o $(M)/vfetchanalyzer.o $(M)/vfetchoptimizer.o
endif

ifeq ($(POLYCOM),1)
OB+=$(LB)polycom/optpfd.o
OB+=$(LB)polycom/polyvbyte.o
endif

ifeq ($(QCOMPRESS),1)
CFLAGS+=-D_QCOMPRESS
endif


ifeq ($(QMX),1)
CFLAGS+=-D_QMX
OB+=$(LB)JASSv2/source/compress_integer_qmx_improved.o $(LB)JASSv2/source/asserts.o
endif

ifeq ($(RICE),1)
CFLAGS+=-D_RICE
OB+=$(LB)rc.o
endif

ifeq ($(SIMDCOMP),1)
CFLAGS+=-D_SIMDCOMP -I$(LB)simdcomp/include 
OB+=$(LB)simdcomp/src/simdintegratedbitpacking.o $(LB)simdcomp/src/simdcomputil.o $(LB)simdcomp/src/simdbitpacking.o $(LB)simdcomp/src/simdpackedselect.o 
OB+=$(LB)simdcomp_/simdfor.o
ifeq ($(AVX2),1)
OB+=$(LB)simdcomp/src/avxbitpacking.o 
endif
endif

ifeq ($(SIMPLE8B),1)
OB+=$(LB)simple8b.o 
endif

ifeq ($(SPDP),1)
XDEFS+=-D_SPDP
endif

ifeq ($(STREAMVBYTE),1)
CFLAGS+=-D_STREAMVBYTE -I$(LB)streamvbyte/include 
ifeq ($(ARCH),aarch64)
CFLAGS+=-D__ARM_NEON__
endif
SV=$(LB)streamvbyte/src/

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
CFLAGS+=-D_TURBORC -I$(T)libsais/include -D_NCPUISA -D_NQUANT
T=$(LB)Turbo-Range-Coder/
ifeq ($(ANS), 1)
CFLAGS+=-D_ANS 
$(T)anscdf0.o: $(T)anscdf.c $(T)anscdf_.h
	$(CC) -c -O3 $(CFLAGS) -mno-sse2 -falign-loops=32 $(T)anscdf.c -o $(T)anscdf0.o  

$(T)anscdfs.o: $(T)anscdf.c $(T)anscdf_.h
	$(CC) -c -O3 $(CFLAGS) -D_NDIVTDEF32 -march=corei7-avx -mtune=corei7-avx -mno-aes -falign-loops=32 $(T)anscdf.c -o $(T)anscdfs.o  

$(T)anscdfx.o: $(T)anscdf.c $(T)anscdf_.h
	$(CC) -c -O3 $(CFLAGS) -march=haswell -falign-loops=32 $(T)anscdf.c -o $(T)anscdfx.o
	
OB+=$(T)anscdfx.o $(T)anscdfs.o $(T)anscdf0.o
endif

OB+=$(T)rc_s.o $(T)rc_ss.o $(T)rcutil.o $(T)libsais/src/libsais.o $(T)rccm_s.o $(T)rccm_ss.o $(T)bec_b.o $(T)rcqlfc_s.o $(T)rcqlfc_ss.o $(T)rcbwt.o 
endif

ifeq ($(VARINTG8IU),1)
CFLAGS+=-D_VARINTG8IU
OB+=$(LB)varintg8iu.o
endif

ifeq ($(VBZ),1)
CFLAGS+=-D_VBZ 
CXXFLAGS+=-I$(LB)vbz_compression/third_party -I$(LB)vbz_compression/vbz -I$(LB)vbz_compression/third_party/stream_vbyte/include
OB+=$(LB)vbz_compression/vbz/vbz.o $(LB)vbz_compression/vbz/v0/vbz_streamvbyte.o $(LB)vbz_compression/vbz/v1/vbz_streamvbyte.o
endif

ifeq ($(VTENC),1)
CFLAGS+=-D_VTENC
OB+=$(LB)VTEnc/bits.o $(LB)VTEnc/bitstream.o $(LB)VTEnc/decode.o $(LB)VTEnc/encode.o $(LB)VTEnc/mem.o
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
ZD=$(LB)zlib/
OB+=$(ZD)adler32.o $(ZD)crc32.o $(ZD)compress.o $(ZD)deflate.o $(ZD)infback.o $(ZD)inffast.o $(ZD)inflate.o $(ZD)inftrees.o $(ZD)trees.o $(ZD)uncompr.o $(ZD)zutil.o
endif
endif

ifeq ($(ZSTD), 1)
CFLAGS+=-D_ZSTD -I$(LB)zstd/lib -I$(LB)zstd/lib/common
OB+=$(LB)zstd/lib/common/pool.o $(LB)zstd/lib/common/xxhash.o $(LB)zstd/lib/common/error_private.o \
    $(LB)zstd/lib/compress/hist.o $(LB)zstd/lib/compress/zstd_compress.o $(LB)zstd/lib/compress/zstd_compress_literals.o $(LB)zstd/lib/compress/zstd_compress_sequences.o $(LB)zstd/lib/compress/zstd_double_fast.o $(LB)zstd/lib/compress/zstd_fast.o $(LB)zstd/lib/compress/zstd_lazy.o $(LB)zstd/lib/compress/zstd_ldm.o $(LB)zstd/lib/compress/zstdmt_compress.o $(LB)zstd/lib/compress/zstd_opt.o \
    $(LB)zstd/lib/decompress/zstd_decompress.o $(LB)zstd/lib/decompress/zstd_decompress_block.o $(LB)zstd/lib/decompress/zstd_ddict.o $(LB)zstd/lib/compress/fse_compress.o $(LB)zstd/lib/common/fse_decompress.o \
	$(LB)zstd/lib/common/zstd_common.o $(LB)zstd/lib/common/entropy_common.o $(LB)zstd/lib/compress/zstd_compress_superblock.o\
    $(LB)zstd/lib/decompress/huf_decompress_amd64.o $(LB)zstd/lib/dictBuilder/zdict.o $(LB)zstd/lib/dictBuilder/fastcover.o $(LB)zstd/lib/dictBuilder/cover.o $(LB)zstd/lib/dictBuilder/divsufsort.o 
#$(LB)zstd/lib/compress/huf_compress.o $(LB)zstd/lib/decompress/huf_decompress.o 	
endif

ifeq ($(FSE), 1)
CFLAGS+=-D_FSE
OB+=$(LB)fse/fse_compress_.o $(LB)fse/fse_decompress_.o 
endif

ifeq ($(FSEHUF), 1)
CFLAGS+=-D_FSEHUF
OB+=$(LB)fse/huf_compress_.o $(LB)fse/huf_decompress_.o 
endif

ifeq ($(ZFP), 1)
CFLAGS+=-D_ZFP -I$(LB)zfp/include
F=$(LB)zfp/src/
OB+=$(F)bitstream.o $(F)decode1d.o $(F)decode1f.o $(F)decode1i.o $(F)decode1l.o $(F)decode2d.o $(F)decode2f.o $(F)decode2i.o $(F)decode2l.o $(F)decode3d.o $(F)decode3f.o $(F)decode3i.o $(F)decode3l.o \
 $(F)decode4d.o $(F)decode4f.o $(F)decode4i.o $(F)decode4l.o $(F)encode1d.o $(F)encode1f.o $(F)encode1i.o $(F)encode1l.o $(F)encode2d.o $(F)encode2f.o $(F)encode2i.o $(F)encode2l.o $(F)encode3d.o \
 $(F)encode3f.o $(F)encode3i.o $(F)encode3l.o $(F)encode4d.o $(F)encode4f.o $(F)encode4i.o $(F)encode4l.o $(F)zfp.o
endif
