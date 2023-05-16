/**
    Copyright (C) powturbo 2013-2023
    SPDX-License-Identifier: GPL v2 License

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
//-- "Interpolative Coding" --------------------------------------------------------------
#if defined(_MSC_VER) && (_MSC_VER < 1600)
  #if !defined(_STDINT) && !defined(_MSC_STDINT_H_)
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
  #endif
#else
#include <stdint.h>
#endif
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//-- maximum output size for encoding
size_t bicbound16(size_t n);
size_t bicbound32(size_t n);

//-- Simple binary
unsigned bicbenc16(uint16_t      *in, unsigned n, unsigned char *out);
unsigned bicbdec16(unsigned char *in, unsigned n, uint16_t      *out);
unsigned bicbenc32(uint32_t      *in, unsigned n, unsigned char *out);
unsigned bicbdec32(unsigned char *in, unsigned n, uint32_t      *out);

//-- Leftmost minimal 
unsigned bicenc16( uint16_t      *in, unsigned n, unsigned char *out);
unsigned bicdec16( unsigned char *in, unsigned n, uint16_t      *out);
unsigned bicenc32( uint32_t      *in, unsigned n, unsigned char *out);
unsigned bicdec32( unsigned char *in, unsigned n, uint32_t      *out);

//-- Center Minimal
unsigned bicmenc16(uint16_t      *in, unsigned n, unsigned char *out);
unsigned bicmdec16(unsigned char *in, unsigned n, uint16_t      *out);
unsigned bicmenc32(uint32_t      *in, unsigned n, unsigned char *out);
unsigned bicmdec32(unsigned char *in, unsigned n, uint32_t      *out);

#ifdef __cplusplus
}
#endif

//-- bitpack -------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

size_t bitnbound8(     size_t n);
size_t bitnbound16(    size_t n);
size_t bitnbound32(    size_t n);
size_t bitnbound64(    size_t n);

size_t bitnbound128v8( size_t n);
size_t bitnbound128v16(size_t n);
size_t bitnbound128v32(size_t n);
size_t bitnbound128v64(size_t n);

size_t bitnbound256v8( size_t n);
size_t bitnbound256v16(size_t n);
size_t bitnbound256v32(size_t n);
size_t bitnbound256v64(size_t n);

//******************** Bit Packing High Level API - n unlimited ****************************
size_t bitnpack8(         uint8_t  *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnpack16(        uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnpack32(        uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnpack64(        uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnpack128v16(    uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnpack128v32(    uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnpack128v64(    uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnpack256v32(    uint32_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t bitndpack8(        uint8_t  *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitndpack16(       uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitndpack32(       uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitndpack64(       uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitndpack128v16(   uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitndpack128v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitndpack256v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t bitnd1pack8(       uint8_t  *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnd1pack16(      uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnd1pack32(      uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnd1pack64(      uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnd1pack128v16(  uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnd1pack128v32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnd1pack256v32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t bitnzpack8(        uint8_t  *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnzpack16(       uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnzpack32(       uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnzpack64(       uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnzpack128v16(   uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnzpack128v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnzpack256v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t bitnxpack8(        uint8_t  *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnxpack16(       uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnxpack32(       uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnxpack64(       uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnxpack128v16(   uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnxpack128v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnxpack256v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t bitnfpack8(        uint8_t  *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnfpack16(       uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnfpack32(       uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnfpack64(       uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnfpack128v16(   uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnfpack128v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitnfpack256v32(   uint32_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t bitnunpack8(       unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t bitnunpack16(      unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnunpack32(      unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnunpack64(      unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t bitnunpack128v16(  unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnunpack128v32(  unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnunpack128v64(  unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t bitnunpack256v32(  unsigned char *__restrict in, size_t n, uint32_t *__restrict out);

size_t bitndunpack8(      unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t bitndunpack16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitndunpack32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitndunpack64(     unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t bitndunpack128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitndunpack128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitndunpack256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);

size_t bitnd1unpack8(     unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t bitnd1unpack16(    unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnd1unpack32(    unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnd1unpack64(    unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t bitnd1unpack128v16(unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnd1unpack128v32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnd1unpack256v32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out);

size_t bitnzunpack8(      unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t bitnzunpack16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnzunpack32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnzunpack64(     unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t bitnzunpack128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnzunpack128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnzunpack256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);

size_t bitnxunpack8(      unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t bitnxunpack16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnxunpack32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnxunpack64(     unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t bitnxunpack128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnxunpack128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnxunpack256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);

size_t bitnfunpack8(      unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t bitnfunpack16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnfunpack32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnfunpack64(     unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t bitnfunpack128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitnfunpack128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t bitnfunpack256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);

size_t bitns1pack128v16(  uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitns1pack128v32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t bitns1unpack128v16(unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t bitns1unpack128v32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out);


//******** Bit Packing Low level API ****************************************************************
// bipackNN: Pack array with n unsigned (NN bits in[n]) values to the buffer out using nbits per value. Return value = end of compressed buffer out
unsigned char *bitpack8(    uint8_t  *__restrict in, unsigned n, const unsigned char *__restrict out , unsigned b);
unsigned char *bitpack16(   uint16_t *__restrict in, unsigned n, const unsigned char *__restrict out , unsigned b);
unsigned char *bitpack32(   uint32_t *__restrict in, unsigned n, const unsigned char *__restrict out , unsigned b);
unsigned char *bitpack64(   uint64_t *__restrict in, unsigned n, const unsigned char *__restrict out , unsigned b);

// delta bit packing
unsigned char *bitdpack8(   uint8_t  *__restrict in, unsigned n, const unsigned char *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitdpack16(  uint16_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint16_t start, unsigned b);
unsigned char *bitdpack32(  uint32_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint32_t start, unsigned b);
unsigned char *bitdpack64(  uint64_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint64_t start, unsigned b);

unsigned char *bitd1pack8(  uint8_t  *__restrict in, unsigned n, const unsigned char *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitd1pack16( uint16_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint16_t start, unsigned b);
unsigned char *bitd1pack32( uint32_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint32_t start, unsigned b);
unsigned char *bitd1pack64( uint64_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint64_t start, unsigned b);

// FOR bit packing : sorted integer array
unsigned char *bitfpack8(   uint8_t  *__restrict in, unsigned n, const unsigned char *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitfpack16(  uint16_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint16_t start, unsigned b);
unsigned char *bitfpack32(  uint32_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint32_t start, unsigned b);
unsigned char *bitfpack64(  uint64_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint64_t start, unsigned b);

unsigned char *bitf1pack8(  uint8_t  *__restrict in, unsigned n, const unsigned char *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitf1pack16( uint16_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint16_t start, unsigned b);
unsigned char *bitf1pack32( uint32_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint32_t start, unsigned b);
unsigned char *bitf1pack64( uint64_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint64_t start, unsigned b);

// zigzag : unsorted integer array
unsigned char *bitzpack8(   uint8_t  *__restrict in, unsigned n, const unsigned char *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitzpack16(  uint16_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint16_t start, unsigned b);
unsigned char *bitzpack32(  uint32_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint32_t start, unsigned b);
unsigned char *bitzpack64(  uint64_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint64_t start, unsigned b);

// xor : unsorted integer array
unsigned char *bitxpack8(   uint8_t  *__restrict in, unsigned n, const unsigned char *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitxpack16(  uint16_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint16_t start, unsigned b);
unsigned char *bitxpack32(  uint32_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint32_t start, unsigned b);
unsigned char *bitxpack64(  uint64_t *__restrict in, unsigned n, const unsigned char *__restrict out, uint64_t start, unsigned b);

//-------------------------------------- SIMD ------------------------------------------------------------------------------------------
// Pack array with 128 unsigned (32 bits in[n]) values to the buffer out using nbits per value. Return value = end of compressed buffer out
unsigned char *bitpack128v16(  unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out                      , unsigned b);
unsigned char *bitdpack128v16( unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start, unsigned b);
unsigned char *bitd1pack128v16(unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start, unsigned b);
unsigned char *bitfpack128v16( unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start, unsigned b);
unsigned char *bitf1pack128v16(unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start, unsigned b);
unsigned char *bitzpack128v16( unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start, unsigned b);
unsigned char *bitxpack128v16( unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start, unsigned b);

unsigned char *bitpack128v32(  unsigned          *__restrict in, unsigned n, unsigned char *__restrict out,                       unsigned b);
unsigned char *bitdpack128v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitd1pack128v32(unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitfpack128v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitf1pack128v32(unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitzpack128v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitxpack128v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);

//unsigned char *bitpack256w32(  unsigned          *__restrict in, unsigned n, unsigned char *__restrict out                  , unsigned b);
unsigned char *bitpack128v64(  uint64_t          *__restrict in, unsigned n, unsigned char *__restrict out,                       unsigned b);

unsigned char *bitpack256v32(  unsigned          *__restrict in, unsigned n, unsigned char *__restrict out,                       unsigned b);
unsigned char *bitdpack256v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitd1pack256v32(unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitfpack256v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitf1pack256v32(unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitzpack256v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);
unsigned char *bitxpack256v32( unsigned          *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start, unsigned b);

unsigned char *bits1pack128v16(unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start, unsigned b);
unsigned char *bits1pack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b);
unsigned char *bits1unpack128v16( const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b);
unsigned char *bits1unpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
//********************************** Bit Packing : Unpack ****************************************************************

// ---------------- Unpack a b-bits packed integer array -------------------------------------------------------------------------------
// unpack a bitpacked integer array. Return value = end of packed buffer in
unsigned char *bitunpack8(  const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, unsigned b);
unsigned char *bitunpack16( const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, unsigned b);
unsigned char *bitunpack32( const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, unsigned b);
unsigned char *bitunpack64( const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, unsigned b);

// ---------------- Direct Access to a single packed integer array entry --------------------------------------------------------------
  #ifdef TURBOPFOR_DAC
#include "conf.h"

static ALWAYS_INLINE unsigned  bitgetx32(const unsigned char *__restrict in, unsigned  idx, unsigned b) { unsigned bidx = b*idx; return bzhi64( ctou64((uint32_t *)in+(bidx>>5)) >> (bidx&0x1f), b ); }
//static ALWAYS_INLINE unsigned  bitgetx32(const unsigned char *__restrict in, unsigned  idx, unsigned b) { unsigned bidx = b*idx;
 //return (ctou64((uint32_t *)in+(bidx>>5)) << 32+(bidx&0x1f)) >> (64-b);
// return bzhi64( ctou64((uint32_t *)in+(bidx>>5)) >> (bidx&0x1f), b ); }
static ALWAYS_INLINE unsigned _bitgetx32(const unsigned char *__restrict in, uint64_t bidx, unsigned b) {                        return bzhi64( ctou64((uint32_t *)in+(bidx>>5)) >> (bidx&0x1f), b ); }

// like  bitgetx32 but for 16 bits integer array
static ALWAYS_INLINE unsigned  bitgetx8( const unsigned char *__restrict in, unsigned  idx, unsigned b) { unsigned bidx = b*idx; return bzhi32( ctou16((uint16_t *)in+(bidx>>4)) >> (bidx& 0xf), b ); }
static ALWAYS_INLINE unsigned _bitgetx8( const unsigned char *__restrict in, unsigned bidx, unsigned b) {                        return bzhi32( ctou16((uint16_t *)in+(bidx>>4)) >> (bidx& 0xf), b ); }
static ALWAYS_INLINE unsigned  bitgetx16(const unsigned char *__restrict in, unsigned  idx, unsigned b) { unsigned bidx = b*idx; return bzhi32( ctou32((uint32_t *)in+(bidx>>4)) >> (bidx& 0xf), b ); }
static ALWAYS_INLINE unsigned _bitgetx16(const unsigned char *__restrict in, unsigned bidx, unsigned b) {                        return bzhi32( ctou32((uint32_t *)in+(bidx>>4)) >> (bidx& 0xf), b ); }

// Set a single value with index "idx"
static ALWAYS_INLINE void      bitsetx16(const unsigned char *__restrict in, unsigned  idx, unsigned v, unsigned b) { unsigned bidx = b*idx;  unsigned           *p = (unsigned           *)             in+(bidx>>4) ; *p = ( *p & ~(((1u  <<b)-1) << (bidx& 0xf)) ) |                     v<<(bidx& 0xf);}
static ALWAYS_INLINE void      bitsetx32(const unsigned char *__restrict in, unsigned  idx, unsigned v, unsigned b) { unsigned bidx = b*idx;  unsigned long long *p = (unsigned long long *)((unsigned *)in+(bidx>>5)); *p = ( *p & ~(((1ull<<b)-1) << (bidx&0x1f)) ) | (unsigned long long)v<<(bidx&0x1f);}
   #endif
// ---------------- DFOR : integrated bitpacking, for delta packed SORTED array (Ex. DocId in inverted index) -------------------------------
// start <= out[0] <= out[1] <= ... <= out[n-2] <= out[n-1] <= (1<<N)-1  N=8,16,32 or 64
// out[0] = start + in[0];  out[1] = out[0] + in[1]; ... ;  out[i] = out[i-1] + in[i]
unsigned char *bitdunpack8(  const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitdunpack16( const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b);
unsigned char *bitdunpack32( const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b);
unsigned char *bitdunpack64( const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b);

// start < out[0] < out[1] < ... < out[n-2] < out[n-1] < (1<<N)-1,    N=8,16,32 or 64
// out[0] = start + in[0] + 1;  out[1] = out[0] + in[1] + 1; ... ;  out[i] = out[i-1] + in[i] +  1
unsigned char *bitd1unpack8( const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitd1unpack16(const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b);
unsigned char *bitd1unpack32(const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b);
unsigned char *bitd1unpack64(const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b);

// ---------------- ZigZag : integrated bitpacking, for zigzag packed unsorted
unsigned char *bitzunpack8(  const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitzunpack16( const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b);
unsigned char *bitzunpack32( const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b);
unsigned char *bitzunpack64( const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b);

// ---------------- Xor : integrated bitpacking, for xor packed unsorted
unsigned char *bitxunpack8(  const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitxunpack16( const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b);
unsigned char *bitxunpack32( const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b);
unsigned char *bitxunpack64( const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b);

// ---------------- For : Direct Access for packed SORTED array  --------------------------------------------
// out[i] = start + in[i] + i
unsigned char *bitfunpack8(  const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitfunpack16( const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b);
unsigned char *bitfunpack32( const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b);
unsigned char *bitfunpack64( const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b);

// out[i] = start + in[i] + i + 1
unsigned char *bitf1unpack8( const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b);
unsigned char *bitf1unpack16(const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b);
unsigned char *bitf1unpack32(const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b);
unsigned char *bitf1unpack64(const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b);

// ---------------- SIMD : unpack a SIMD bit packed integer array -------------------------------------------------------------------------------
// SIMD unpack a 128/256 bitpacked integer array. Return value = end of packed buffer in
unsigned char *bitunpack128v16(  const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out,                       unsigned b);
unsigned char *bitzunpack128v16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b);
unsigned char *bitxunpack128v16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b);
unsigned char *bitdunpack128v16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b);
unsigned char *bitd1unpack128v16(const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b);
unsigned char *bitfunpack128v16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b);
unsigned char *bitf1unpack128v16(const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b);

unsigned char *bitunpack128v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out,                 unsigned b);
unsigned char *bitzunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitxunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1unpack128v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitfunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitf1unpack128v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);

unsigned char *bitunpack256w32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out,                 unsigned b);
unsigned char *bitunpack128v64(  const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out,                 unsigned b);

unsigned char *bitunpack256v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out,                 unsigned b);
unsigned char *bitzunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitxunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1unpack256v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitfunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitf1unpack256v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);

unsigned char *bitunpack128h32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out,                 unsigned b);
unsigned char *bitzunpack128h32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack128h32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1unpack128h32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);

// internal TurboPFor functions: masked unpack
unsigned char *_bitunpack128v16(  const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out,                       unsigned b, unsigned short *__restrict pex, unsigned char *bb);
unsigned char *_bitdunpack128v16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b, unsigned short *__restrict pex, unsigned char *bb);
unsigned char *_bitd1unpack128v16(const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b, unsigned short *__restrict pex, unsigned char *bb);
unsigned char *_bitzunpack128v16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start, unsigned b, unsigned short *__restrict pex, unsigned char *bb);

unsigned char *_bitunpack128v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitdunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitd1unpack128v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitzunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);

unsigned char *_bitunpack128h32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitdunpack128h32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitd1unpack128h32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);

unsigned char *_bitunpack256w32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitunpack128v64(  const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, unsigned b, uint32_t *__restrict pex, unsigned char *bb);

unsigned char *_bitunpack256v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitd1unpack256v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitzunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
#ifdef __cplusplus
}
#endif

//-- "Integer Compression: max.bits, delta, zigzag, xor" -----------------------------------------------------------------------
//---------- max. bit length + transform for sorted/unsorted arrays, delta,delta 1, delta > 1, zigzag, zigzag of delta, xor, FOR,----------------
#ifdef __cplusplus
extern "C" {
#endif
//------ ORed array, used to determine the maximum bit length of the elements in an unsorted integer array ---------------------
uint8_t  bit8(     uint8_t  *in, unsigned n, uint8_t  *px);
uint16_t bit16(    uint16_t *in, unsigned n, uint16_t *px);
uint32_t bit32(    uint32_t *in, unsigned n, uint32_t *px);
uint64_t bit64(    uint64_t *in, unsigned n, uint64_t *px);
uint32_t bit256v32(uint32_t *in, unsigned n, uint32_t *px);

//-------------- delta = 0: Sorted integer array w/ mindelta = 0 ----------------------------------------------
//-- ORed array, maximum bit length of the non decreasing integer array. out[i] = in[i] - in[i-1]
uint8_t  bitd8(     uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitd16(    uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitd32(    uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitd64(    uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitd256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//-- in-place reverse delta 0
void bitddec8(  uint8_t  *p,  unsigned n, uint8_t  start); // non decreasing (out[i] = in[i] - in[i-1])
void bitddec16( uint16_t *p,  unsigned n, uint16_t start);
void bitddec32( uint32_t *p,  unsigned n, uint32_t start);
void bitddec64( uint64_t *p,  unsigned n, uint64_t start);

//-- vectorized fast delta4 one: out[0] = in[4]-in[0], out[1]=in[5]-in[1], out[2]=in[6]-in[2], out[3]=in[7]-in[3],...
uint16_t bits128v16(   uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bits128v32(   uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//------------- delta = 1: Sorted integer array w/ mindelta = 1 ---------------------------------------------
//-- get delta maximum bit length of the non strictly decreasing integer array. out[i] = in[i] - in[i-1] - 1
uint8_t  bitd18( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitd116(uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitd132(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitd164(uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitd1256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//-- in-place reverse delta one
void bitd1dec8(     uint8_t  *p,  unsigned n, uint8_t  start); // non strictly decreasing (out[i] = in[i] - in[i-1] - 1)
void bitd1dec16(    uint16_t *p,  unsigned n, uint16_t start);
void bitd1dec32(    uint32_t *p,  unsigned n, uint32_t start);
void bitd1dec64(    uint64_t *p,  unsigned n, uint64_t start);

//------------- delta > 1: Sorted integer array w/ mindelta > 1 ---------------------------------------------
//-- ORed array, for max. bit length get min. delta ()
uint8_t  bitdi8(    uint8_t  *in, unsigned n, uint8_t  *px,  uint8_t  start);
uint16_t bitdi16(   uint16_t *in, unsigned n, uint16_t *px,  uint16_t start);
uint32_t bitdi32(   uint32_t *in, unsigned n, uint32_t *px,  uint32_t start);
uint64_t bitdi64(   uint64_t *in, unsigned n, uint64_t *px,  uint64_t start);
//-- transform sorted integer array to delta array: out[i] = in[i] - in[i-1] - mindelta
uint8_t  bitdienc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta);
uint16_t bitdienc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta);
uint32_t bitdienc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta);
uint64_t bitdienc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta);
//-- in-place reverse delta
void     bitdidec8( uint8_t  *in, unsigned n,                uint8_t  start, uint8_t  mindelta);
void     bitdidec16(uint16_t *in, unsigned n,                uint16_t start, uint16_t mindelta);
void     bitdidec32(uint32_t *in, unsigned n,                uint32_t start, uint32_t mindelta);
void     bitdidec64(uint64_t *in, unsigned n,                uint64_t start, uint64_t mindelta);

//------------- FOR : array bit length: ---------------------------------------------------------------------
//------ ORed array, for max. bit length of the non decreasing integer array.  out[i] = in[i] - start
uint8_t  bitf8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitf16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitf32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitf64(uint64_t *in, unsigned n, uint64_t *px, uint64_t start);

//------ ORed array, for max. bit length of the non strictly decreasing integer array out[i] = in[i] - 1 - start
uint8_t  bitf18( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitf116(uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitf132(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitf164(uint64_t *in, unsigned n, uint64_t *px, uint64_t start);

//------ ORed array, for max. bit length for usorted array
uint8_t  bitfm8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  *pmin);  // unsorted
uint16_t bitfm16(uint16_t *in, unsigned n, uint16_t *px, uint16_t *pmin);
uint32_t bitfm32(uint32_t *in, unsigned n, uint32_t *px, uint32_t *pmin);
uint64_t bitfm64(uint64_t *in, unsigned n, uint64_t *px, uint64_t *pmin);

//------------- Zigzag encoding for unsorted integer lists: out[i] = in[i] - in[i-1] ------------------------
//-- ORed array, to get maximum zigzag bit length integer array
uint8_t  bitz8(    uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitz16(   uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitz32(   uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitz64(   uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitz256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
//-- Zigzag transform
uint8_t  bitzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta);
uint16_t bitzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta);
uint32_t bitzenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta);
uint64_t bitzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta);
//-- in-place zigzag reverse transform
void bitzdec8(     uint8_t  *in, unsigned n,                uint8_t  start);
void bitzdec16(    uint16_t *in, unsigned n,                uint16_t start);
void bitzdec32(    uint32_t *in, unsigned n,                uint32_t start);
void bitzdec64(    uint64_t *in, unsigned n,                uint64_t start);

//------------- Zigzag of zigzag/delta : unsorted/sorted integer array ----------------------------------------------------
//-- get delta maximum bit length of the non strictly decreasing integer array. out[i] = in[i] - in[i-1] - 1
uint8_t  bitzz8(    uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitzz16(   uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitzz32(   uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitzz64(   uint64_t *in, unsigned n, uint64_t *px, uint64_t start);

uint8_t  bitzzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta);
uint16_t bitzzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta);
uint32_t bitzzenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta);
uint64_t bitzzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta);

//-- in-place reverse zigzag of delta (encoded w/ bitdiencNN and parameter mindelta = 1)
void bitzzdec8(     uint8_t  *in,  unsigned n, uint8_t  start); // non strictly decreasing (out[i] = in[i] - in[i-1] - 1)
void bitzzdec16(    uint16_t *in,  unsigned n, uint16_t start);
void bitzzdec32(    uint32_t *in,  unsigned n, uint32_t start);
void bitzzdec64(    uint64_t *in,  unsigned n, uint64_t start);

//------------- XOR encoding for unsorted integer lists: out[i] = in[i] - in[i-1] -------------
//-- ORed array, to get maximum zigzag bit length integer array
uint8_t  bitx8(     uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitx16(    uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitx32(    uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitx64(    uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitx256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//-- XOR transform
uint8_t  bitxenc8(  uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start);
uint16_t bitxenc16( uint16_t *in, unsigned n, uint16_t *out, uint16_t start);
uint32_t bitxenc32( uint32_t *in, unsigned n, uint32_t *out, uint32_t start);
uint64_t bitxenc64( uint64_t *in, unsigned n, uint64_t *out, uint64_t start);

//-- XOR in-place reverse transform
void bitxdec8(      uint8_t  *p,  unsigned n, uint8_t  start);
void bitxdec16(     uint16_t *p,  unsigned n, uint16_t start);
void bitxdec32(     uint32_t *p,  unsigned n, uint32_t start);
void bitxdec64(     uint64_t *p,  unsigned n, uint64_t start);
//--------------------------------------------------------------------
void bitxenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void bitxdec(unsigned char *in, unsigned n,                     unsigned esize);
void bitzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void bitzdec(unsigned char *in, unsigned n,                     unsigned esize);

//------- Floating point ----------------------------------------
#define FLOAT2INT(fval, scalse, bias) round(fval * scale + bias)
#define INT2FLOAT(ival, scalse, bias) ((ival - bias) / scale)

//------- Lossy floating point transform: pad the trailing mantissa bits with zeros according to the error e (ex. e=0.00001)
  #ifdef USE_FLOAT16
_Float16 _fprazor16(_Float16 d, float e, int lg2e);  
void fprazor16(_Float16 *in, unsigned n, _Float16  *out, float  e);
  #endif

float  _fprazor32(float  d, float  e, int lg2e);
double _fprazor64(double d, double e, int lg2e);
void   fprazor32( float  *in, unsigned n, float  *out, float  e);
void   fprazor64(double  *in, unsigned n, double *out, double e);

#ifdef __cplusplus
}
#endif

//--  eliasfano.h - "Integer Compression" Elias Fano c/c++ header ---------------------------------------------------------

#ifdef __cplusplus
extern "C" {
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

//---------- "Floating Point + Integer Compression" -----------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// ---------- TurboPFor Zigzag of delta (=delta of delta + zigzag encoding) (TurboPFor)
size_t p4nzzenc128v8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t p4nzzdec128v8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t p4nzzenc128v16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t p4nzzdec128v16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t p4nzzenc128v32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t p4nzzdec128v32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t p4nzzenc128v64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t p4nzzdec128v64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- Zigzag (bit/io) -------------------------------------------------------
size_t bvzenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t bvzdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t bvzenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t bvzdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t bvzenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bvzdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t bvzenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bvzdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
//----------- Zigzag of delta (bit/io) ---------------------------------------------
size_t bvzzenc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t bvzzdec8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t bvzzenc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t bvzzdec16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t bvzzenc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bvzzdec32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t bvzzenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bvzzdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboGorilla : Improved gorilla style + RLE (bit/io) ------------------
size_t fpgenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpgdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpgenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpgdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpgenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpgdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpgenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpgdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- Chimp algo : (bit/io) ------------------------------------------------
size_t fphenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fphdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fphenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fphdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fphenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fphdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fphenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fphdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloatLz (bitio/io) ------------------
size_t fpc0enc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start); // fpcenc slower but stronger than fpc0enc
size_t fpc0enc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpc0enc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpc0enc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpcdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
size_t fpcenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpcdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpcenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpcdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpcenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpcdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpcenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpcdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat XOR : Last value predictor (TurboPFor) ---------------------
size_t fpxenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpxdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpxenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpxdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpxenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpxdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpxenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpxdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat FCM: Finite Context Method Predictor (TurboPFor) -----------
size_t fpfcmenc8(   uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpfcmdec8(   unsigned char *in, size_t n, uint8_t      *out,  uint8_t  start);
size_t fpfcmenc16(  uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpfcmdec16(  unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpfcmenc32(  uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpfcmdec32(  unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpfcmenc64(  uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpfcmdec64(  unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat DFCM: Differential Finite Context Method Predictor (TurboPFor)
size_t fpdfcmenc8(  uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpdfcmdec8(  unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpdfcmenc16( uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpdfcmdec16( unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpdfcmenc32( uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpdfcmdec32( unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpdfcmenc64( uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpdfcmdec64( unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat 2D DFCM: Differential Finite Context Method Predictor -----
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

//--  transpose.h - Byte/Nibble transpose for further compressing with lz77 or other compressors -------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
// Syntax
// in    : Input buffer
// n     : Total number of bytes in input buffer
// out   : output buffer
// esize : element size in bytes (ex. 2, 4, 8,... )

//---------- High level functions with dynamic cpu detection and JIT scalar/sse/avx2 switching
void tpenc(        unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // tranpose
void tpdec(        unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // reverse transpose

void tpzenc(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // zigzag integrated
void tpzdec(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

void tpxenc(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // xor integrated
void tpxdec(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

//2D transpose
void tp2denc(      unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out, unsigned esize); 
void tp2ddec(      unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out, unsigned esize);

void tp2denc2(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out); 
void tp2ddec2(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out);

void tp2denc4(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out); 
void tp2ddec4(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out);

void tp2denc8(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out); 
void tp2ddec8(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out);

//3D transpose
void tp3denc(      unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize); 
void tp3ddec(      unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize);

void tp3denc2(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp3ddec2(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp3denc4(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp3ddec4(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp3denc8(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp3ddec8(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

//4D transpose
void tp4denc(      unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize); 
void tp4ddec(      unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize);

void tp4denc2(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp4ddec2(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp4denc4(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp4ddec4(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp4denc8(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp4ddec8(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

// Nibble transpose SIMD (SSE2,AVX2, ARM Neon)
void tp4enc(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void tp4dec(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

void tp4zenc(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // zigzag delta integrated
void tp4zdec(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

void tp4xenc(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // xor integrated
void tp4xdec(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

//---------- Low level functions --------------------------------------------------------------------------------------------
void tpenc2(       unsigned char *in, unsigned n, unsigned char *out);  // scalar
void tpdec2(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc3(       unsigned char *in, unsigned n, unsigned char *out);
void tpdec3(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc4(       unsigned char *in, unsigned n, unsigned char *out);
void tpdec4(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc8(       unsigned char *in, unsigned n, unsigned char *out);
void tpdec8(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc16(      unsigned char *in, unsigned n, unsigned char *out);
void tpdec16(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc2(      unsigned char *in, unsigned n, unsigned char *out);  // scalar zigzag
void tpzdec2(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc3(      unsigned char *in, unsigned n, unsigned char *out);
void tpzdec3(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc4(      unsigned char *in, unsigned n, unsigned char *out);
void tpzdec4(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc8(      unsigned char *in, unsigned n, unsigned char *out);
void tpzdec8(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc16(     unsigned char *in, unsigned n, unsigned char *out);
void tpzdec16(     unsigned char *in, unsigned n, unsigned char *out);

void tpxenc2(      unsigned char *in, unsigned n, unsigned char *out);  // scalar xor
void tpxdec2(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc3(      unsigned char *in, unsigned n, unsigned char *out);
void tpxdec3(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc4(      unsigned char *in, unsigned n, unsigned char *out);
void tpxdec4(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc8(      unsigned char *in, unsigned n, unsigned char *out);
void tpxdec8(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc16(     unsigned char *in, unsigned n, unsigned char *out);
void tpxdec16(     unsigned char *in, unsigned n, unsigned char *out);

//-- byte transpose - sse ---------
void tpenc128v2(   unsigned char *in, unsigned n, unsigned char *out); // 16 bits
void tpdec128v2(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc128v2(  unsigned char *in, unsigned n, unsigned char *out);
void tpzdec128v2(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc128v2(  unsigned char *in, unsigned n, unsigned char *out);
void tpxdec128v2(  unsigned char *in, unsigned n, unsigned char *out);
//--
void tpenc128v4(   unsigned char *in, unsigned n, unsigned char *out); // 32 bits
void tpdec128v4(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc128v4(  unsigned char *in, unsigned n, unsigned char *out);
void tpzdec128v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc128v4(  unsigned char *in, unsigned n, unsigned char *out);
void tpxdec128v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpenc128v8(   unsigned char *in, unsigned n, unsigned char *out); // 64 bits
void tpdec128v8(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc128v8(  unsigned char *in, unsigned n, unsigned char *out);
void tpzdec128v8(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc128v8(  unsigned char *in, unsigned n, unsigned char *out);
void tpxdec128v8(  unsigned char *in, unsigned n, unsigned char *out);

//-- nibble transpose
void tp4enc128v2(  unsigned char *in, unsigned n, unsigned char *out); // 16 bits
void tp4dec128v2(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc128v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4zdec128v2( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc128v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4xdec128v2( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc128v4(  unsigned char *in, unsigned n, unsigned char *out); // 32 bits
void tp4dec128v4(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc128v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4zdec128v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc128v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4xdec128v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc128v8(  unsigned char *in, unsigned n, unsigned char *out); // 64 bits
void tp4dec128v8(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc128v8( unsigned char *in, unsigned n, unsigned char *out);
void tp4zdec128v8( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc128v8( unsigned char *in, unsigned n, unsigned char *out);
void tp4xdec128v8( unsigned char *in, unsigned n, unsigned char *out);

//-- avx2 
//-- byte transpose
void tpenc256v2(   unsigned char *in, unsigned n, unsigned char *out); //-- 16 bits
void tpdec256v2(   unsigned char *in, unsigned n, unsigned char *out);

void tpenc256v4(   unsigned char *in, unsigned n, unsigned char *out); //-- 32 bits
void tpdec256v4(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc256v4(  unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tpzdec256v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc256v4(  unsigned char *in, unsigned n, unsigned char *out); // xor
void tpxdec256v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpenc256v8(   unsigned char *in, unsigned n, unsigned char *out); //-- 64 bits
void tpdec256v8(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc256v8(  unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tpzdec256v8(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc256v8(  unsigned char *in, unsigned n, unsigned char *out); // xor
void tpxdec256v8(  unsigned char *in, unsigned n, unsigned char *out);

//-- Nibble transpose
void tp4enc256v2(  unsigned char *in, unsigned n, unsigned char *out); //-- 16 bits
void tp4dec256v2(  unsigned char *in, unsigned n, unsigned char *out);

void tp4enc256v4(  unsigned char *in, unsigned n, unsigned char *out); //-- 32 bits
void tp4dec256v4(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc256v4( unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tp4zdec256v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc256v4( unsigned char *in, unsigned n, unsigned char *out); // xor
void tp4xdec256v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc256v8(  unsigned char *in, unsigned n, unsigned char *out); // 64 bits
void tp4dec256v8(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc256v8( unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tp4zdec256v8( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc256v8( unsigned char *in, unsigned n, unsigned char *out); // xor
void tp4xdec256v8( unsigned char *in, unsigned n, unsigned char *out);

//------- CPU instruction set
// cpuiset  = 0: return current simd set,
// cpuiset != 0: set simd set 0:scalar, 20:sse2, 52:avx2
unsigned cpuini(unsigned cpuiset);

// convert simd set to string "sse3", "sse3", "sse4.1" or "avx2"
// Ex.: printf("current cpu set=%s\n", cpustr(cpuini(0)) );
char *cpustr(unsigned cpuisa);

unsigned cpuisa(void);
#ifdef __cplusplus
}
#endif

// ----- TurboRLE - "Most efficient and fastest Run Length Encoding" -----------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
// RLE with specified escape char
unsigned _srlec8( const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint8_t e);
unsigned _srled8( const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint8_t e);

unsigned _srlec16(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint16_t e);
unsigned _srled16(const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint16_t e);

unsigned _srlec32(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint32_t e);
unsigned _srled32(const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint32_t e);

unsigned _srlec64(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint64_t e);
unsigned _srled64(const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint64_t e);

// functions w/ overflow handling
unsigned  srlec8( const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint8_t e);
unsigned  srled8( const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint8_t e);

unsigned  srlec16(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint16_t e);
unsigned  srled16(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint16_t e);

unsigned  srlec32(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint32_t e);
unsigned  srled32(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint32_t e);

unsigned  srlec64(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint64_t e);
unsigned  srled64(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint64_t e);

// RLE w. automatic escape char determination
unsigned  srlec(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out);
unsigned _srled(  const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen);
unsigned  srled(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen);

// Turbo RLE
unsigned  trlec(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out);
unsigned _trled(  const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen);
unsigned  trled(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen);
#ifdef __cplusplus
}
#endif

//-- "Integer Compression" variable length encoding with bitio ------------------

#ifdef __cplusplus
extern "C" {
#endif
//-- Gamma coding (length limited) : Full 32 bit range
size_t bitgenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  
size_t bitgdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

//-- Rice Coding (adaptive length limited) : Full 32 bit range ---
size_t bitrenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);   
size_t bitrdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlcenc32(   unsigned char *_in, size_t _inlen,  unsigned char  *out);   // TurboVLC 32 bits
size_t vlcdec32(   unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlcenc16(   unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 16 bits
size_t vlcdec16(   unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlczenc16(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 16 bits zigzag
size_t vlczdec16(  unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlczenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 32 bits zigzag
size_t vlczdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

#ifdef __cplusplus
}
#endif

//-- "Integer Compression" variable byte (scalar TurboVByte+ SIMD TurboByte) ---------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------- TurboVByte 'vb':Variable byte + SIMD TurboByte 'v8': array functions -----
size_t vbbound8( size_t n);
size_t vbbound16(size_t n);
size_t vbbound32(size_t n);
size_t vbbound64(size_t n);

// Encoding/Decoding: Return value = end of compressed/decompressed output/input buffer out/in

//----------------------- Encoding/Decoding unsorted array with n integer values --------------------------
unsigned char *vbenc16(  unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out); //TurboVByte
unsigned char *vbdec16(  unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out);

unsigned char *vbenc32(  unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbdec32(  unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out);

unsigned char *vbenc64(  uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbdec64(  unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out);

//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbgetx16( unsigned char *__restrict in, unsigned idx);
unsigned       vbgetx32( unsigned char *__restrict in, unsigned idx);
uint64_t       vbgetx64( unsigned char *__restrict in, unsigned idx);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
unsigned vbgeteq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short key);
unsigned vbgeteq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       key);
unsigned vbgeteq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       key);

//---------------------- Delta encoding/decoding sorted array ---------------------------------------------
//-- Increasing integer array. out[i] = out[i-1] + in[i]
unsigned char *vbdenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbddec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbdenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbddec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbdenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbddec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbdgetx16(  unsigned char *__restrict in, unsigned idx, unsigned short start);
unsigned       vbdgetx32(  unsigned char *__restrict in, unsigned idx, unsigned start);
uint64_t       vbdgetx64(  unsigned char *__restrict in, unsigned idx, uint64_t start);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
unsigned vbdgetgeq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short *key, unsigned short start);
unsigned vbdgetgeq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       *key, unsigned       start);
unsigned vbdgetgeq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       *key, uint64_t       start);

//-- Strictly increasing (never remaining constant or decreasing) integer array. out[i] = out[i-1] + in[i] + 1
unsigned char *vbd1enc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbd1enc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbd1enc64(uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);

unsigned char *vbd1dec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbd1dec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);
unsigned char *vbd1dec64(unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);


//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbd1getx16(  unsigned char *__restrict in, unsigned idx, unsigned short start);
unsigned       vbd1getx32(  unsigned char *__restrict in, unsigned idx, unsigned       start);
uint64_t       vbd1getx64(  unsigned char *__restrict in, unsigned idx, uint64_t       start);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
unsigned vbd1getgeq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short *key, unsigned short start);
unsigned vbd1getgeq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       *key, unsigned       start);
unsigned vbd1getgeq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       *key, uint64_t       start);

//---------------------- Zigzag encoding/decoding for unsorted integer lists.
unsigned char *vbzenc8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char start);
unsigned char *vbzdec8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char  start);

unsigned char *vbzenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbzdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbzenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbzdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbzenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbzdec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//---------------------- XOR encoding/decoding for unsorted integer lists.
unsigned char *vbxenc8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char start);
unsigned char *vbxdec8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char  start);

unsigned char *vbxenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbxdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbxenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbxdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbxenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbxdec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//---------------------- Delta of delta encoding/decoding for unsorted integer lists.
unsigned char *vbddenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbdddec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbddenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbdddec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbddenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbdddec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbzgetx16(  unsigned char *__restrict in, unsigned idx, unsigned short start);
unsigned       vbzgetx32(  unsigned char *__restrict in, unsigned idx, unsigned       start);
uint64_t       vbzgetx64(  unsigned char *__restrict in, unsigned idx, uint64_t       start);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
/*unsigned vbzgeteq15( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short key, unsigned start);
unsigned vbzgeteq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short key, unsigned start);
unsigned vbzgeteq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       key, unsigned start);
unsigned vbzgeteq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       key, unsigned start);*/

//-------------------------- TurboByte (SIMD Group varint) --------------------------------------------------------------
size_t v8bound16(size_t n);
size_t v8bound32(size_t n);

size_t v8len32(const uint32_t *in, size_t n);
size_t v8len16(const uint16_t *in, size_t n);

unsigned char *v8enc16(  unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out); //TurboByte
unsigned char *v8dec16(  unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out);

unsigned char *v8enc32(  unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *v8dec32(  unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out);

//------ delta ---------
unsigned char *v8denc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8ddec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *v8denc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *v8ddec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//------ delta 1 -------
unsigned char *v8d1enc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8d1dec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *v8d1enc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *v8d1dec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//------- zigzag -------
unsigned char *v8zenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8zdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *v8zenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *v8zdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//------- xor ----------
unsigned char *v8xenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8xenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);

unsigned char *v8xdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *v8xdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//-------------------------- TurboByte Hybrid (SIMD Group varint) + Bitpacking -------------------------------------------
size_t v8nenc16(  uint16_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec16(  unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nenc32(  uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec32(  unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8ndenc16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta
size_t v8nddec16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8ndenc32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nddec32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nd1enc16(uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta 1
size_t v8nd1dec16(unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nd1enc32(uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nd1dec32(unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nzenc16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // zigzag delta
size_t v8nzdec16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nzenc32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nzdec32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nxenc16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // xor
size_t v8nxdec16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nxenc32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nxdec32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

//------------- sse / neon ----------------------------------------------------------------------------
size_t v8nenc128v16(  uint16_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec128v16(  unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nenc128v32(  uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec128v32(  unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8ndenc128v16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta
size_t v8nddec128v16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8ndenc128v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nddec128v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nd1enc128v16(uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta 1
size_t v8nd1dec128v16(unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nd1enc128v32(uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nd1dec128v32(unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nzenc128v16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // zigzag delta
size_t v8nzdec128v16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nzenc128v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nzdec128v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nxenc128v16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // xor
size_t v8nxdec128v16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nxenc128v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nxdec128v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

//------------- avx2 ------------------------------------------------------------------------------------
size_t v8nenc256v32(  uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec256v32(  unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8ndenc256v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nddec256v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nd1enc256v32(uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nd1dec256v32(unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nzenc256v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nzdec256v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nxenc256v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nxdec256v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

#ifdef __cplusplus
}
#endif

//-- "TurboPFor: Integer Compression" PFor/PForDelta  + Direct access ----------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

size_t p4nbound8( size_t n);
size_t p4nbound16(size_t n);
size_t p4nbound32(size_t n);
size_t p4nbound64(size_t n);

size_t p4nbound128v8( size_t n);
size_t p4nbound128v16(size_t n);
size_t p4nbound128v32(size_t n);
size_t p4nbound128v64(size_t n);

size_t p4nbound256v8( size_t n);
size_t p4nbound256v16(size_t n);
size_t p4nbound256v32(size_t n);
size_t p4nbound256v64(size_t n);

//************************************************ High level API - n unlimited *******
// Compress integer array with n values to the buffer out.
// Return value = number of bytes written to compressed buffer out
// n:number of integers in the input in
size_t p4nenc8(       uint8_t  *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nenc16(      uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nenc32(      uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nenc64(      uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nenc128v16(  uint16_t *__restrict in, size_t n, unsigned char *__restrict out); // SIMD (Vertical bitpacking)
size_t p4nenc128v32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nenc128v64(  uint64_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nenc256w32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nenc256v32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out);


size_t p4ndenc8(      uint8_t  *__restrict in, size_t n, unsigned char *__restrict out); // delta 0
size_t p4ndenc16(     uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4ndenc32(     uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4ndenc128v16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4ndenc128v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4ndenc256v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4ndenc64(     uint64_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t p4nd1enc8(     uint8_t  *__restrict in, size_t n, unsigned char *__restrict out); // delta 1
size_t p4nd1enc16(    uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nd1enc32(    uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nd1enc128v16(uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nd1enc128v32(uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nd1enc256v32(uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nd1enc64(    uint64_t *__restrict in, size_t n, unsigned char *__restrict out);

size_t p4nzenc8(      uint8_t  *__restrict in, size_t n, unsigned char *__restrict out); // zigzag
size_t p4nzenc16(     uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nzenc32(     uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nzenc128v16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nzenc128v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nzenc256v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out);
size_t p4nzenc64(     uint64_t *__restrict in, size_t n, unsigned char *__restrict out);

// Decompress the compressed n values in input buffer in to the integer array out.
// Return value = number of bytes read from the compressed buffer in
size_t p4ndec8(       unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t p4ndec16(      unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4ndec32(      unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4ndec64(      unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t p4ndec128v16(  unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4ndec128v32(  unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4ndec128v64(  unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
size_t p4ndec256v32(  unsigned char *__restrict in, size_t n, uint32_t *__restrict out);

// Delta minimum = 0
size_t p4nddec8(      unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t p4nddec16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4nddec32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nddec128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4nddec128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nddec256w32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nddec256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nddec64(     unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
// Delta minimum = 1
size_t p4nd1dec8(     unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t p4nd1dec16(    unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4nd1dec32(    unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nd1dec128v16(unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4nd1dec128v32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nd1dec256v32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nd1dec64(    unsigned char *__restrict in, size_t n, uint64_t *__restrict out);
//Zigzag
size_t p4nzdec8(      unsigned char *__restrict in, size_t n, uint8_t  *__restrict out);
size_t p4nzdec16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4nzdec32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nzdec128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out);
size_t p4nzdec128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nzdec256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out);
size_t p4nzdec64(     unsigned char *__restrict in, size_t n, uint64_t *__restrict out);

//************** Low level API - n limited to 128/256 ***************************************
#define P4D_MAX 256

// -------------- TurboPFor: Encode ------------
//#include <assert.h>
// Low level API: Single block n limited
//compress integer array with n values to the buffer out. Return value = end of compressed buffer out
unsigned char *p4enc8(       uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4enc16(      uint16_t *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4enc32(      uint32_t *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4enc128v16(  uint16_t *__restrict in, unsigned n, unsigned char *__restrict out); // SSE (Vertical bitpacking)
unsigned char *p4enc128v32(  uint32_t *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4enc128v64(  uint64_t *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4enc256v32(  uint32_t *__restrict in, unsigned n, unsigned char *__restrict out); // AVX2
unsigned char *p4enc64(      uint64_t *__restrict in, unsigned n, unsigned char *__restrict out);

unsigned char *p4enc256w32(  uint32_t *__restrict in, unsigned n, unsigned char *__restrict out);

unsigned char *p4encx8(      uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out);// Direct access
unsigned char *p4encx16(     uint16_t *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4encx32(     uint32_t *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4encx64(     uint64_t *__restrict in, unsigned n, unsigned char *__restrict out);

unsigned char *p4denc8(      uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out, uint8_t  start);
unsigned char *p4denc16(     uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start);
unsigned char *p4denc32(     uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4denc128v16( uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start);
unsigned char *p4denc128v32( uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4denc256v32( uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4denc64(     uint64_t *__restrict in, unsigned n, unsigned char *__restrict out, uint64_t start);

unsigned char *p4denc256w32( uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);

unsigned char *p4dencx8(     uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out, uint8_t  start); // Direct access
unsigned char *p4dencx16(    uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start);
unsigned char *p4dencx32(    unsigned *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);

unsigned char *p4d1enc8(     uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out, uint8_t  start);
unsigned char *p4d1enc16(    uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start);
unsigned char *p4d1enc32(    uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4d1enc128v16(uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start); // SIMD (Vertical bitpacking)
unsigned char *p4d1enc128v32(uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4d1enc256v32(uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4d1enc64(    uint64_t *__restrict in, unsigned n, unsigned char *__restrict out, uint64_t start);

unsigned char *p4d1encx8(    uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out, uint8_t  start); // Direct access
unsigned char *p4d1encx16(   uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start);
unsigned char *p4d1encx32(   uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);

unsigned char *p4zenc8(      uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out, uint8_t  start);
unsigned char *p4zenc16(     uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start);
unsigned char *p4zenc32(     uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4zenc128v16( uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, uint16_t start);
unsigned char *p4zenc128v32( uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4zenc256v32( uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, uint32_t start);
unsigned char *p4zenc64(     uint64_t *__restrict in, unsigned n, unsigned char *__restrict out, uint64_t start);

unsigned char *p4senc16(uint16_t      *in, unsigned n, unsigned char *out, uint16_t start);
unsigned char *p4senc32(uint32_t      *in, unsigned n, unsigned char *out, uint32_t start);
unsigned char *p4senc64(uint64_t      *in, unsigned n, unsigned char *out, uint64_t start);

unsigned char *p4sdec16(unsigned char *in, unsigned n, uint16_t *out,      uint16_t start);
unsigned char *p4sdec32(unsigned char *in, unsigned n, uint32_t *out,      uint32_t start);
unsigned char *p4sdec64(unsigned char *in, unsigned n, uint64_t *out,      uint64_t start);

size_t p4nsenc16(uint16_t *in, size_t n, unsigned char *out);
size_t p4nsenc32(uint32_t *in, size_t n, unsigned char *out);
size_t p4nsenc64(uint64_t *in, size_t n, unsigned char *out);

size_t p4nsdec16(unsigned char *in, size_t n, uint16_t *out);
size_t p4nsdec32(unsigned char *in, size_t n, uint32_t *out);
size_t p4nsdec64(unsigned char *in, size_t n, uint64_t *out);

// same as p4enc, but with b and bx as parameters. Call after _p4bitsXX
unsigned char *_p4enc8(      uint8_t  *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4enc16(     uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4enc32(     uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4enc128v16( uint16_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx); // SIMD (Vertical bitpacking)
unsigned char *_p4enc128v32( uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx); // SIMD (Vertical bitpacking)
unsigned char *_p4enc128v64( uint64_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx); // SIMD (Vertical bitpacking)
unsigned char *_p4enc256v32( uint32_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4enc64(     uint64_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
// calculate the best bit sizes b and bx, return b.
unsigned _p4bits8(           uint8_t  *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bits16(          uint16_t *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bits32(          uint32_t *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bits64(          uint64_t *__restrict in, unsigned n, unsigned *pbx);

unsigned _p4bitsx8(          uint8_t  *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bitsx16(         uint16_t *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bitsx32(         uint32_t *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bitsx64(         uint64_t *__restrict in, unsigned n, unsigned *pbx);

#define P4HVE(_out_, _b_, _bx_,_usize_) do { if(!_bx_) *_out_++ = _b_;else if(_bx_ <= _usize_) *_out_++ = 0x80|_b_, *_out_++ = _bx_; else *_out_++= (_bx_ == _usize_+1?0x40:0xc0)|_b_; } while(0)

#define P4HVE8( _out_, _b_, _bx_) P4HVE(_out_, _b_, _bx_, 8)
#define P4HVE16(_out_, _b_, _bx_) P4HVE(_out_, _b_, _bx_,16)
#define P4HVE32(_out_, _b_, _bx_) P4HVE(_out_, _b_, _bx_,32)
#define P4HVE64(_out_, _b_, _bx_) do { unsigned _c = _b_==64?64-1:_b_; P4HVE(_out_, _c, _bx_,64); } while(0)

//---------------------------- TurboPFor: Decode --------------------------------------------------------
// decompress a previously (with p4enc32) bit packed array. Return value = end of packed buffer in
//-- scalar. (see p4getx32 for direct access)
// b and bx specified (not stored within the compressed stream header)
unsigned char *_p4dec8(       unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4dec16(      unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4dec32(      unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4dec128v16(  unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, unsigned b, unsigned bx); // SIMD (Vertical BitPacking)
unsigned char *_p4dec128v32(  unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4dec128v64(  unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4dec64(      unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, unsigned b, unsigned bx);

unsigned char *p4dec8(        unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out);
unsigned char *p4dec16(       unsigned char *__restrict in, unsigned n, uint16_t *__restrict out);
unsigned char *p4dec32(       unsigned char *__restrict in, unsigned n, uint32_t *__restrict out);
unsigned char *p4dec128v16(   unsigned char *__restrict in, unsigned n, uint16_t *__restrict out);  // SIMD (Vertical BitPacking)
unsigned char *p4dec128v32(   unsigned char *__restrict in, unsigned n, uint32_t *__restrict out);
unsigned char *p4dec128v64(   unsigned char *__restrict in, unsigned n, uint64_t *__restrict out);
unsigned char *p4dec256v32(   unsigned char *__restrict in, unsigned n, uint32_t *__restrict out);
unsigned char *p4dec64(       unsigned char *__restrict in, unsigned n, uint64_t *__restrict out);
//------ Delta decoding --------------------------- Return value = end of packed input buffer in ---------------------------
//-- Increasing integer lists. out[i] = out[i-1] + in[i]
// b and bx specified
unsigned char *_p4ddec8(      unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b, unsigned bx);
unsigned char *_p4ddec16(     unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b, unsigned bx);
unsigned char *_p4ddec32(     unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4ddec128v16( unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b, unsigned bx);
unsigned char *_p4ddec128v32( unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4ddec256v32( unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4ddec64(     unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b, unsigned bx);

unsigned char *p4ddec8(       unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start);
unsigned char *p4ddec16(      unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start);
unsigned char *p4ddec32(      unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4ddec128v16(  unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start); // SIMD (Vertical BitPacking)
unsigned char *p4ddec128v32(  unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4ddec256v32(  unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4ddec64(      unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start);

//-- Strictly increasing (never remaining constant or decreasing) integer lists. out[i] = out[i-1] + in[i] +  1
// b and bx specified (see idxcr.c/idxqry.c for an example)
unsigned char *_p4d1dec8(     unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b, unsigned bx);
unsigned char *_p4d1dec16(    unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b, unsigned bx);
unsigned char *_p4d1dec32(    unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4d1dec128v16(unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b, unsigned bx); // SIMD (Vertical BitPacking)
unsigned char *_p4d1dec128v32(unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4d1dec256v32(unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4d1dec64(    unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b, unsigned bx);

unsigned char *p4d1dec8(      unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start);
unsigned char *p4d1dec16(     unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start);
unsigned char *p4d1dec32(     unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4d1dec128v16( unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start); // SIMD (Vertical BitPacking)
unsigned char *p4d1dec128v32( unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4d1dec256v32( unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4d1dec64(     unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start);

// ZigZag encoding
unsigned char *_p4zdec8(      unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start, unsigned b, unsigned bx);
unsigned char *_p4zdec16(     unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b, unsigned bx);
unsigned char *_p4zdec32(     unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4zdec128v16( unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start, unsigned b, unsigned bx);
unsigned char *_p4zdec128v32( unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4zdec256v32( unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start, unsigned b, unsigned bx);
unsigned char *_p4zdec64(     unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start, unsigned b, unsigned bx);

unsigned char *p4zdec8(       unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t  start);
unsigned char *p4zdec16(      unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start);
unsigned char *p4zdec32(      unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4zdec128v16(  unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start); // SIMD (Vertical BitPacking)
unsigned char *p4zdec128v32(  unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4zdec256v32(  unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
unsigned char *p4zdec64(      unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start);

//---------------- Direct Access functions to compressed TurboPFor array p4encx16/p4encx32 -------------------------------------------------------
  #ifdef TURBOPFOR_DAC
#include "conf.h"
#define BITPACK_DAC
#include "../include_/bitpack.h"
#define P4D_PAD8(_x_)       ( (((_x_)+8-1)/8) )
#define P4D_B(_x_)          ((_x_) & 0x7f)
#define P4D_XB(_x_)         (((_x_) & 0x80)?((_x_) >> 8):0)
#define P4D_ININC(_in_, _x_) _in_ += 1+((_x_) >> 7)

static inline unsigned p4bits(unsigned char *__restrict in, int *bx) { unsigned i = ctou16(in); *bx = P4D_XB(i); return P4D_B(i); }

struct p4 {
  unsigned long long *xmap;
  unsigned char *ex;
  unsigned isx,bx,cum[P4D_MAX/64+1];
  int oval,idx;
};

static unsigned long long p4xmap[P4D_MAX/64+1] = { 0 };

// prepare direct access usage
static inline void p4ini(struct p4 *p4, unsigned char **pin, unsigned n, unsigned *b) { unsigned char *in = *pin;
  unsigned p4i  = ctou16(in);
  p4->isx       = p4i&0x80;
  *b            = P4D_B(p4i);
  p4->bx        = P4D_XB(p4i);              //printf("p4i=%x,b=%d,bx=%d ", p4->i, *b, p4->bx);                        //assert(n <= P4D_MAX);
  *pin = p4->ex = ++in;
  if(p4->isx) {
    unsigned num=0,j;
    unsigned char *p;
    ++in;
    p4->xmap = (unsigned long long *)in;
    for(j=0; j < n/64; j++) { p4->cum[j] = num; num += popcnt64(ctou64(in+j*8)); }
    if(n & 0x3f) num += popcnt64(ctou64(in+j*8) & ((1ull<<(n&0x3f))-1) );
    p4->ex = p = in + (n+7)/8;
    *pin   = p = p4->ex+(((uint64_t)num*p4->bx+7)/8);
  } else p4->xmap = p4xmap;
  p4->oval = p4->idx  = -1;
}

//---------- Get a single value with index "idx" from a "p4encx32" packed array
static ALWAYS_INLINE uint8_t  p4getx8( struct p4 *p4, unsigned char *in, unsigned idx, unsigned b) { unsigned bi, cl, u = bitgetx8( in, idx, b);
  if(p4->xmap[bi=idx>>6] & (1ull<<(cl=idx&63))) u += bitgetx8(p4->ex, p4->cum[bi] + popcnt64(p4->xmap[bi] & ~(~0ull<<cl)), p4->bx) << b;
  return u;
}

static ALWAYS_INLINE uint16_t p4getx16(struct p4 *p4, unsigned char *in, unsigned idx, unsigned b) { unsigned bi, cl, u = bitgetx16(in, idx, b);
  if(p4->xmap[bi=idx>>6] & (1ull<<(cl=idx&63))) u += bitgetx16(p4->ex, p4->cum[bi] + popcnt64(p4->xmap[bi] & ~(~0ull<<cl)), p4->bx) << b;
  return u;
}
static ALWAYS_INLINE uint32_t p4getx32(struct p4 *p4, unsigned char *in, unsigned idx, unsigned b) { unsigned bi, cl, u = bitgetx32(in, idx, b);
  if(p4->xmap[bi=idx>>6] & (1ull<<(cl=idx&63))) u += bitgetx32(p4->ex, p4->cum[bi] + popcnt64(p4->xmap[bi] & ~(~0ull<<cl)), p4->bx) << b;
  return u;
}

// Get the next single value greater of equal to val
static ALWAYS_INLINE uint16_t p4geqx8( struct p4 *p4, unsigned char *in, unsigned b, uint8_t  val) { do p4->oval += p4getx8( p4, in, ++p4->idx, b)+1; while(p4->oval < val); return p4->oval; }
static ALWAYS_INLINE uint16_t p4geqx16(struct p4 *p4, unsigned char *in, unsigned b, uint16_t val) { do p4->oval += p4getx16(p4, in, ++p4->idx, b)+1; while(p4->oval < val); return p4->oval; }
static ALWAYS_INLINE uint32_t p4geqx32(struct p4 *p4, unsigned char *in, unsigned b, uint32_t val) { do p4->oval += p4getx32(p4, in, ++p4->idx, b)+1; while(p4->oval < val); return p4->oval; }

/* DO NOT USE : like p4dec32 but using direct access. This is only a demo showing direct access usage. Use p4dec32 instead for decompressing entire blocks */
unsigned char *p4decx32(   unsigned char *in, unsigned n, uint32_t *out);  // unsorted
unsigned char *p4fdecx32(  unsigned char *in, unsigned n, uint32_t *out, uint32_t start); // FOR increasing
unsigned char *p4f1decx32( unsigned char *in, unsigned n, uint32_t *out, uint32_t start); // FOR strictly increasing
  #endif

#ifdef __cplusplus
}
#endif

// -- "Integer Compression" variable simple "SimpleV" -----------------------------------------------------------
//  this belongs to the integer compression known as "simple family", like simple-9,simple-16
//  or simple-8b. SimpleV is compressing integers in groups into variable word size 32, 40 and 64 bits + RLE (run length encoding)
//  SimpleV is faster than simple-16 and compress better than simple-16 or simple-8b.

#ifdef __cplusplus
extern "C" {
#endif

size_t vsbound8( size_t n);
size_t vsbound16(size_t n);
size_t vsbound32(size_t n);
size_t vsbound64(size_t n);

// vsencNN: compress array with n unsigned (NN bits in[n]) values to the buffer out. Return value = end of compressed output buffer out
unsigned char *vsenc8( unsigned char  *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsenc16(unsigned short *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsenc32(unsigned       *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsenc64(uint64_t       *__restrict in, size_t n, unsigned char  *__restrict out);

// vsdecNN: decompress buffer into an array of n unsigned values. Return value = end of compressed input buffer in
unsigned char *vsdec8( unsigned char  *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsdec16(unsigned char  *__restrict in, size_t n, unsigned short *__restrict out);
unsigned char *vsdec32(unsigned char  *__restrict in, size_t n, unsigned       *__restrict out);
unsigned char *vsdec64(unsigned char  *__restrict in, size_t n, uint64_t       *__restrict out);

#ifdef __cplusplus
}
#endif

