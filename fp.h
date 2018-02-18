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
// delta
size_t fppenc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fppdec8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fppenc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fppdec16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fppenc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fppdec32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fppenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fppdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
// delta of delta
size_t fpddenc8(       uint8_t *in,   size_t n, unsigned char *out, uint8_t  start);
size_t fpdddec8( unsigned char *in,   size_t n, uint8_t       *out, uint8_t  start);
size_t fpddenc16(     uint16_t *in,   size_t n, unsigned char *out, uint16_t start);
size_t fpdddec16(unsigned char *in,   size_t n, uint16_t      *out, uint16_t start);
size_t fpddenc32(     uint32_t *in,   size_t n, unsigned char *out, uint32_t start);
size_t fpdddec32(unsigned char *in,   size_t n, uint32_t      *out, uint32_t start);
size_t fpddenc64(     uint64_t *in,   size_t n, unsigned char *out, uint64_t start);
size_t fpdddec64(unsigned char *in,   size_t n, uint64_t      *out, uint64_t start);
// gorilla style
size_t fpgenc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpgdec8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpgenc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpgdec16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpgenc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpgdec32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpgenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpgdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//---- Delta of delta w/ bit/io
size_t bitgenc8(   uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t bitgdec8(   unsigned char *in, size_t n, uint8_t      *out,  uint8_t  start);
size_t bitgenc16(  uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t bitgdec16(  unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t bitgenc32(  uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bitgdec32(  unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t bitgenc64(  uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bitgdec64(  unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//---- FCM: Finite Context Method Predictor 
size_t fpfcmenc8(  uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpfcmdec8(  unsigned char *in, size_t n, uint8_t      *out,  uint8_t  start);
size_t fpfcmenc16( uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpfcmdec16( unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpfcmenc32( uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpfcmdec32( unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpfcmenc64( uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpfcmdec64( unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

// DFCM: Differential Finite Context Method Predictor 
size_t fpdfcmenc8( uint8_t       *in, size_t n, unsigned char *out, uint8_t start);
size_t fpdfcmdec8( unsigned char *in, size_t n, uint8_t       *out, uint8_t start);
size_t fpdfcmenc16(uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpdfcmdec16(unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpdfcmenc32(uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpdfcmdec32(unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpdfcmenc64(uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpdfcmdec64(unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
#ifdef __cplusplus
}
#endif
