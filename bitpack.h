/**
    Copyright (C) powturbo 2013-2015
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
//     bitpack.h - "Integer Compression" Binary Packing header file

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
// bipackNN: Pack array with n unsigned (NN bits in[n]) values to the buffer out using nbits per value. Return value = end of compressed buffer out
unsigned char *bitpack32(  unsigned           *__restrict in, unsigned n, unsigned char *__restrict out				   , unsigned b);
unsigned char *bitpack16(  unsigned short     *__restrict in, unsigned n, unsigned char *__restrict out				   , unsigned b);
unsigned char *bitpack64(  uint64_t           *__restrict in, unsigned n, unsigned char *__restrict out				   , unsigned b);

// delta bit packing
unsigned char *bitdpack32( unsigned           *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1pack32(unsigned           *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);

// for bit packing
unsigned char *bitfpack32( unsigned           *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);
unsigned char *bitf1pack32(unsigned           *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);

// zigzag
unsigned char *bitzpack32( unsigned           *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);

//-------------------------------------- SIMD ------------------------------------------------------------------------------------------
// Pack array with n unsigned (32 bits in[n]) values to the buffer out using nbits per value. Return value = end of compressed buffer out
unsigned char *bitpackv32(  unsigned          *__restrict in, unsigned n, unsigned char *__restrict out				   , unsigned b);
unsigned char *bitdpackv32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1packv32(unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);
unsigned char *bitzpackv32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);

// like bitpack32 but for 16 bits arrays
unsigned char *bitpackv16(  unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out				   , unsigned b);

#ifdef __cplusplus
}
#endif
