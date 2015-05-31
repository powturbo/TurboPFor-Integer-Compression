# powturbo  (c) Copyright 2013-2015
# Linux: "export CC=clang" windows mingw: "set CC=gcc" or uncomment one of following lines
# CC=clang
# CC=gcc
CFLAGS=-DNDEBUG -fstrict-aliasing -m64 -march=native

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIBTHREAD=-lpthread
LIBRT=-lrt
endif

BIT=./
all: icbench idxcr idxqry idxseg

bitpack.o: $(BIT)bitpack.c $(BIT)bitpack.h $(BIT)bitpack64_.h
	$(CC) -O2 $(CFLAGS) -c $(BIT)bitpack.c

bitpackv.o: $(BIT)bitpackv.c $(BIT)bitpack.h $(BIT)bitpackv32_.h
	$(CC) -O2 $(CFLAGS) -c $(BIT)bitpackv.c
	
vp4dc.o: $(BIT)vp4dc.c
	$(CC) -O3 $(CFLAGS) -funroll-loops -c $(BIT)vp4dc.c

vp4dd.o: $(BIT)vp4dd.c
	$(CC) -O3 $(CFLAGS) -funroll-loops -c $(BIT)vp4dd.c

varintg8iu.o: $(BIT)ext/varintg8iu.c $(BIT)ext/varintg8iu.h 
	$(CC) -O2 $(CFLAGS) -c -funroll-loops -std=c99 $(BIT)ext/varintg8iu.c

idxqryp.o: $(BIT)idxqry.c
	$(CC) -O3 $(CFLAGS) -c $(BIT)idxqry.c -o idxqryp.o

SIMDCOMPD=ext/simdcomp/
SIMDCOMP=$(SIMDCOMPD)bitpacka.o $(SIMDCOMPD)src/simdintegratedbitpacking.o $(SIMDCOMPD)src/simdcomputil.o $(SIMDCOMPD)src/simdbitpacking.o

#LZT=../lz/lz8c0.o ../lz/lz8d.o
#LZTB=../lz/lzbc0.o ../lz/lzbd.o
LZ4=ext/lz4.o 
MVB=ext/MaskedVByte/src/varintencode.o ext/MaskedVByte/src/varintdecode.o

OBJS=icbench.o bitutil.o vint.o bitpack.o bitunpack.o eliasfano.o vsimple.o vp4dd.o vp4dc.o varintg8iu.o bitpackv.o bitunpackv.o transpose.o ext/simple8b.o $(SIMDCOMP) $(MVB) $(LZT) $(LZTB) $(LZ4)

icbench: $(OBJS)
	$(CC) $(OBJS) -lm -o icbench $(LFLAGS)

idxseg:   idxseg.o
	$(CC) idxseg.o -o idxseg

ifeq ($(UNAME), Linux)
para: CFLAGS += -DTHREADMAX=32	
para: idxqry
endif

idxcr:   idxcr.o bitpack.o vp4dc.o bitutil.o
	$(CC) idxcr.o bitpack.o bitpackv.o vp4dc.o bitutil.o -o idxcr $(LFLAGS)

idxqry:   idxqry.o bitunpack.o vp4dd.o bitunpackv.o bitutil.o
	$(CC) idxqry.o bitunpack.o bitunpackv.o vp4dd.o bitutil.o $(LIBTHREAD) $(LIBRT) -o idxqry $(LFLAGS)

.c.o:
	$(CC) -O3 $(CFLAGS) $< -c -o $@

.cc.o:
	$(CXX) -O3 -DNDEBUG -std=c++11 $< -c -o $@

.cpp.o:
	$(CXX) -O3 -DNDEBUG -std=c++11 $< -c -o $@
	
clean:
	@find . -type f -name "*\.o" -delete -or -name "*\~" -delete -or -name "core" -delete

cleanw:
	del /S ..\*.o
	del /S ..\*~
