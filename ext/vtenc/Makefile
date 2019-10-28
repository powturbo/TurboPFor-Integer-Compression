# Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
# Licensed under the MIT License.
# See LICENSE file in the project root for full license information.

TESTSDIR = tests
UNITTESTSDIR = tests/unit

CC = gcc
CFLAGS = -Wall -O3
DEBUGFLAGS = -Wall -O0 -g3
LDFLAGS = -shared
AR = ar

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: default
default: lib

.PHONY: all
all: lib test

.PHONY: debug
debug: CFLAGS = $(DEBUGFLAGS)
debug: all

%.o: %.c
	${CC} -c $(CFLAGS) $<

libvtenc.a: $(OBJ)
	$(AR) rcs $@ $^

libvtenc.so: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: lib
lib: libvtenc.a libvtenc.so

.PHONY: test
test: lib
	$(MAKE) -C $(TESTSDIR) all CFLAGS="$(CFLAGS)"
	$(MAKE) -C $(UNITTESTSDIR) all CFLAGS="$(CFLAGS)"

.PHONY: clean
clean:
	rm -f *.o libvtenc.a libvtenc.so
	$(MAKE) -C $(TESTSDIR) clean
	$(MAKE) -C $(UNITTESTSDIR) clean
