# powturbo  (c) Copyright 2007-2015
CFLAGS=-ffast-math -DNDEBUG -fstrict-aliasing -m64 -march=native

BIT=./
all: icbench idxcr idxqry

bitunpack.o: $(BIT)bitunpack.c $(BIT)bitunpack_.h $(BIT)bitunpack.h $(BIT)bitunpack64_.h
	gcc -O3 $(CFLAGS) -c $(BIT)bitunpack.c

bitpack.o: $(BIT)bitpack.c $(BIT)bitpack_.h $(BIT)bitpack.h $(BIT)bitpack64_.h
	gcc -O2 $(CFLAGS) -c $(BIT)bitpack.c

vp4dc.o: $(BIT)vp4dc.c
	gcc -O3 $(CFLAGS) -funroll-loops -c $(BIT)vp4dc.c

vp4dd.o: $(BIT)vp4dd.c
	gcc -O3 $(CFLAGS) -funroll-loops -c $(BIT)vp4dd.c

SIMDCOMPD=ext/simdcomp/
SIMDCOMP=$(SIMDCOMPD)bitpacka.o $(SIMDCOMPD)src/simdintegratedbitpacking.o $(SIMDCOMPD)src/simdcomputil.o $(SIMDCOMPD)src/simdbitpacking.o

varintg8iu.o: $(BIT)ext/varintg8iu.c $(BIT)ext/varintg8iu.h
	gcc -O2 $(CFLAGS) -c -funroll-loops -std=c99 $(BIT)ext/varintg8iu.c

icbench:    icbench.o bitpack.o bitunpack.o vsimple.o vp4dd.o vp4dc.o varintg8iu.o ext/simple8b.o $(SIMDCOMP) 
	gcc -O3 icbench.o bitpack.o bitunpack.o vsimple.o vp4dd.o vp4dc.o varintg8iu.o ext/simple8b.o $(SIMDCOMP) -lm -o icbench $(LFLAGS)

idxcr:   idxcr.o bitpack.o $(SIMDCOMP) vp4dc.o vsimple.o
	gcc -O3 idxcr.o bitpack.o $(SIMDCOMP) vp4dc.o vsimple.o -o idxcr $(LFLAGS)

idxqry:   idxqry.o bitunpack.o $(SIMDCOMP) vp4dd.o
	gcc -O3 idxqry.o bitunpack.o $(SIMDCOMP) vp4dd.o -o idxqry $(LFLAGS)

.c.o:
	gcc -O3 $(CFLAGS) $< -c -o $@

clean:
	rm  *.o
	rm ext/*.o
	rm ext/simdcomp/*.o
	rm ext/simdcomp/src/*.o

cleanw:
	del .\*.o
	del ext\*.o
	del ext\simdcomp\*.o
	del ext\simdcomp\src\*.o
