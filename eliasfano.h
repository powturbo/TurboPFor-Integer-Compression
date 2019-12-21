/**
    Copyright (C) powturbo 2013-2019
    GPL v2 License

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    - homepage : https://sites.google.com/site/powturbo/
    - github   : https://github.com/powturbo
    - twitter  : https://twitter.com/powturbo
    - email    : powturbo [_AT_] gmail [_DOT_] com
**/
//    eliasfano.h - "Integer Compression" Elias Fano c/c++ header
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_MSC_VER) && _MSC_VER < 1600
#include "vs/stdint.h"
#else
#include <stdint.h>
#endif

// compress/decompress integer array with n values to the buffer out. Return value = end of output/input buffer
unsigned char *efanoenc32(     unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efanoenc64(     uint64_t      *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t start);

unsigned char *efanodec32(     unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);
unsigned char *efanodec64(     unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start);

unsigned char *efano1enc32(    unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efano1enc64(    uint64_t      *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t start);

unsigned char *efano1dec32(    unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);
unsigned char *efano1dec64(    unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start);

unsigned char *efanoenc128v32( unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efanodec128v32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

unsigned char *efano1enc128v32(unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efano1dec128v32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

unsigned char *efanoenc256v32( unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efanodec256v32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

unsigned char *efano1enc256v32(unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efano1dec256v32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

#ifdef __cplusplus
}
#endif
