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
//     "Floating Point + Integer Compression"
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_MSC_VER) && _MSC_VER < 1600
#include "vs/stdint.h"
#else 
#include <stdint.h>
#endif
//--------- Last value Predictor 
// delta (=zigzag). Same as p4zenc
size_t fppenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fppdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fppenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fppdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fppenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fppdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fppenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fppdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

// Zigzag of delta (=delta of delta + zigzag encoding)
size_t fpzzenc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpzzdec8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpzzenc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpzzdec16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpzzenc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpzzdec32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpzzenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpzzdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

// Improved gorilla style + RLE w/ bit/io
size_t fpgenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpgdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpgenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpgdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpgenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpgdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpgenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpgdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
//---- Zigzag w/ bit/io
size_t bvzenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t bvzdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t bvzenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t bvzdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t bvzenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bvzdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t bvzenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bvzdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
//---- Zigzag of delta w/ bit/io
size_t bvzzenc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t bvzzdec8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t bvzzenc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t bvzzdec16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t bvzzenc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bvzzdec32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t bvzzenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bvzzdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//------------- FCM: Finite Context Method Predictor 
size_t fpfcmenc8(   uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpfcmdec8(   unsigned char *in, size_t n, uint8_t      *out,  uint8_t  start);
size_t fpfcmenc16(  uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpfcmdec16(  unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpfcmenc32(  uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpfcmdec32(  unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpfcmenc64(  uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpfcmdec64(  unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//------------- DFCM: Differential Finite Context Method Predictor 
size_t fpdfcmenc8(  uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpdfcmdec8(  unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpdfcmenc16( uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpdfcmdec16( unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpdfcmenc32( uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpdfcmdec32( unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpdfcmenc64( uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpdfcmdec64( unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

// 2D DFCM
size_t fp2dfcmenc8( uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fp2dfcmdec8( unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fp2dfcmenc16(uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fp2dfcmdec16(unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fp2dfcmenc32(uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fp2dfcmdec32(unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fp2dfcmenc64(uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fp2dfcmdec64(unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

#ifdef __cplusplus
}
#endif
