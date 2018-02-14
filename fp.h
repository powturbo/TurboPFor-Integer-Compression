/**
    Copyright (C) powturbo 2013-2018
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
//     "Floating Point/Integer Compressionw/ TurboPFor"
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_MSC_VER) && _MSC_VER < 1600
#include "vs/stdint.h"
#else 
#include <stdint.h>
#endif
//---- Last value Predictor 
size_t fppenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fppdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

size_t fpgenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpgdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

size_t fppdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
size_t fpddenc64(  uint64_t *in,      size_t n, unsigned char *out, uint64_t start);

//---- Delta of delta
size_t bitgenc32(  uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bitgdec32(  unsigned char *in, size_t n, uint32_t      *out, uint32_t start);

size_t bitgenc64( uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bitgdec64( unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//size_t bitg0enc32(  uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
//size_t bitg0dec32(  unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
//size_t bitg0enc64(  uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
//size_t bitg0dec64(  unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//---- FCM: Finite Context Method Predictor 
size_t fpfcmenc64( uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpfcmdec64( unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

// DFCM: Differential Finite Context Method Predictor 
size_t fpdfcmenc64(uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpdfcmdec64(unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
#ifdef __cplusplus
}
#endif
