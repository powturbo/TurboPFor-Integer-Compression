# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h

CFLAGS = -fPIC -std=c99 -O3 -Wall -Wextra -Wno-unused-parameter -pedantic
LDFLAGS = -shared
LIBNAME=libsimdcomp.so.0.0.3
all:  unit $(LIBNAME)
test: 
	./unit
install: $(OBJECTS)
	cp $(LIBNAME) /usr/local/lib
	ln -s /usr/local/lib/$(LIBNAME) /usr/local/lib/libsimdcomp.so
	ldconfig
	cp $(HEADERS) /usr/local/include



HEADERS=./include/simdbitpacking.h ./include/simdcomputil.h ./include/simdintegratedbitpacking.h ./include/simdcomp.h 

uninstall:
	for h in $(HEADERS) ; do rm  /usr/local/$$h; done
	rm  /usr/local/lib/$(LIBNAME)
	rm /usr/local/lib/libsimdcomp.so
	ldconfig


OBJECTS= simdbitpacking.o simdintegratedbitpacking.o simdcomputil.o

$(LIBNAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(LIBNAME) $(OBJECTS)  $(LDFLAGS) 



simdcomputil.o: ./src/simdcomputil.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/simdcomputil.c -Iinclude  

simdbitpacking.o: ./src/simdbitpacking.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/simdbitpacking.c -Iinclude  

simdintegratedbitpacking.o: ./src/simdintegratedbitpacking.c  $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/simdintegratedbitpacking.c -Iinclude  

example: ./example.c    $(HEADERS) $(OBJECTS)
	$(CC) $(CFLAGS) -o example ./example.c -Iinclude  $(OBJECTS)

unit: ./src/unit.c    $(HEADERS) $(OBJECTS)
	$(CC) $(CFLAGS) -o unit ./src/unit.c -Iinclude  $(OBJECTS)
dynunit: ./src/unit.c    $(HEADERS) $(LIBNAME)
	$(CC) $(CFLAGS) -o dynunit ./src/unit.c -Iinclude  -lsimdcomp 

clean: 
	rm -f unit *.o $(LIBNAME)
