# powturbo  (c) Copyright 2007-2013
CFLAGS=-ffast-math -fstrict-aliasing -march=native -w -fpermissive

BIT=./
all: icbench

bitunpack.o: $(BIT)bitunpack.c $(BIT)bitunpack_.h $(BIT)bitunpack.h $(BIT)bitunpack64_.h
	cc -O2 $(CFLAGS) -c $(BIT)bitunpack.c

bitpack.o: $(BIT)bitpack.c $(BIT)bitpack_.h $(BIT)bitpack.h $(BIT)bitpack64_.h
	cc -O2 $(CFLAGS) -c $(BIT)bitpack.c

vp4dc.o: $(BIT)vp4dc.c
	cc -O3 $(CFLAGS) -funroll-loops -c $(BIT)vp4dc.c

SIMDCOMPD=aux/simdcomp/
SIMDCOMP=$(SIMDCOMPD)bitpacka.o $(SIMDCOMPD)src/simdintegratedbitpacking.o $(SIMDCOMPD)src/simdcomputil.o $(SIMDCOMPD)src/simdbitpacking.o

varintg8iu.o: $(BIT)aux/varintg8iu.c $(BIT)aux/varintg8iu.h
	cc -O2 $(CFLAGS) -c -funroll-loops -std=c99 $(BIT)aux/varintg8iu.c

icbench:   icbench.o bitpack.o bitunpack.o vsimple.o aux/simple8b.o varintg8iu.o vp4dd.o vp4dc.o  $(SIMDCOMP)
	cc -O3 icbench.o bitpack.o bitunpack.o vsimple.o aux/simple8b.o vp4dd.o vp4dc.o varintg8iu.o $(SIMDCOMP) -lm -o icbench $(LFLAGS)

.c.o:
	cc -O3 $(CFLAGS) $< -c -o $@


