/**
    Copyright (C) powturbo 2013-2017
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
//  "Integer Compression" Bit Packing 

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "conf.h"

// ---------------- Unpack a b-bits packed integer array --------------------------------------------------------------------------------------
// unpack a bitpacked integer array. Return value = end of packed buffer in 
unsigned char *bitunpack16(const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned b);
unsigned char *bitunpack32(const unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned b);
unsigned char *bitunpack64(const unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, unsigned b);

// ---------------- Direct Access to a single packed integer array entry --------------------------------------------------------------------
// Get a single 32 bits value with index "idx" (or bit index b*idx) from packed integer array
static ALWAYS_INLINE unsigned  bitgetx32(const unsigned char *__restrict in, unsigned  idx, unsigned b) { unsigned bidx = b*idx; return bzhi64( ctou64((unsigned *)in+(bidx>>5)) >> (bidx&0x1f), b ); }
static ALWAYS_INLINE unsigned _bitgetx32(const unsigned char *__restrict in, unsigned bidx, unsigned b) {                        return bzhi64( ctou64((unsigned *)in+(bidx>>5)) >> (bidx&0x1f), b ); }

// like  bitgetx32 but for 16 bits integer array
static ALWAYS_INLINE unsigned  bitgetx16(const unsigned char *__restrict in, unsigned  idx, unsigned b) { unsigned bidx = b*idx; return bzhi32( ctou32((unsigned *)in+(bidx>>4)) >> (bidx& 0xf), b ); }
static ALWAYS_INLINE unsigned _bitgetx16(const unsigned char *__restrict in, unsigned bidx, unsigned b) {                        return bzhi32( ctou32((unsigned *)in+(bidx>>4)) >> (bidx& 0xf), b ); }

// Set a single value with index "idx" 
static ALWAYS_INLINE void      bitsetx16(const unsigned char *__restrict in, unsigned  idx, unsigned v, unsigned b) { unsigned bidx = b*idx;  unsigned           *p = (unsigned           *)             in+(bidx>>4) ; *p = ( *p & ~(((1u  <<b)-1) << (bidx& 0xf)) ) |                     v<<(bidx& 0xf);}
static ALWAYS_INLINE void      bitsetx32(const unsigned char *__restrict in, unsigned  idx, unsigned v, unsigned b) { unsigned bidx = b*idx;  unsigned long long *p = (unsigned long long *)((unsigned *)in+(bidx>>5)); *p = ( *p & ~(((1ull<<b)-1) << (bidx&0x1f)) ) | (unsigned long long)v<<(bidx&0x1f);}

// ---------------- DFOR : integrated bitpacking, for delta packed SORTED array (Ex. DocId in inverted index) -------------------------------
// start < out[0] < out[1] < ... < out[n-2] < out[n-1] < (1<<N)-1,    N=32,16
// out[0] = start + in[0] + 1;  out[1] = out[0] + in[1] + 1; ... ;  out[i] = out[i-1] + in[i] +  1
unsigned char *bitd1unpack64( const unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start, unsigned b);
unsigned char *bitd1unpack32( const unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1unpack16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);

// start <= out[0] <= out[1] <= ... <= out[n-2] <= out[n-1] <= (1<<N)-1  N=32,16
// out[0] = start + in[0];  out[1] = out[0] + in[1]; ... ;  out[i] = out[i-1] + in[i]
unsigned char *bitdunpack32(  const unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack16(  const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack64( const unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start, unsigned b);

unsigned char *bitzunpack32(  const unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);

// ---------------- For : Direct Access for packed SORTED array (Ex. DocId in inverted index) --------------------------------------------
// out[i] = start + in[i] + i + 1
unsigned char *bitf1unpack32( const unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitf1unpack16( const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);

// out[i] = start + in[i] + i 
unsigned char *bitfunpack32(  const unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitfunpack16(  const unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);

// ---------------- SIMD : unpack a bit packed integer array -------------------------------------------------------------------------------
// SIMD unpack a 128/256 bitpacked integer array. Return value = end of packed buffer in
unsigned char *bitunpack128v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b);
unsigned char *bitzunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b); 
unsigned char *bitd1unpack128v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);

unsigned char *bitunpack256v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b);
unsigned char *bitzunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b); 
unsigned char *bitd1unpack256v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);

unsigned char *bitunpack128h32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b);
unsigned char *bitzunpack128h32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack128h32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b); 
unsigned char *bitd1unpack128h32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b);

// internal TurboPFor functions: masked unpack
unsigned char *_bitunpack128v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitdunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitd1unpack128v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);

unsigned char *_bitunpack128h32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitdunpack128h32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitd1unpack128h32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);

unsigned char *_bitunpack256v32(  const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);
unsigned char *_bitd1unpack256v32(const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb);

#ifdef __cplusplus
}
#endif
