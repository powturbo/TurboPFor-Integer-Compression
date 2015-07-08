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
//    bitunpack.h - "Integer Compression" Binary Packing 

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#include "conf.h"

// ---------------- Unpack a bit packed integer array --------------------------------------------------------------------------------------
// unpack a bitpacked integer array. Return value = end of packed buffer in 
unsigned char *bitunpack32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned b);
unsigned char *bitunpack16(unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned b);
unsigned char *bitunpack64(unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, unsigned b);

// ---------------- Direct Access to a single packed integer array entry --------------------------------------------------------------------
  #ifdef __AVX2__
#include <x86intrin.h>
  #else
#define _bzhi_u64(__u, __b) ((__u) & ((1ull<<__b)-1))
#define _bzhi_u32(__u, __b) ((__u) & ((1u  <<__b)-1))
  #endif

// Get a single 32 bits value with index "idx" (or bit index b*idx) from packed integer array
static ALWAYS_INLINE unsigned  bitgetx32(unsigned char *__restrict in, unsigned b, unsigned  idx) { unsigned bidx = b*idx; return _bzhi_u64( (*(unsigned long long *)((unsigned *)in+(bidx>>5))) >> (bidx&0x1f), b ); }
static ALWAYS_INLINE unsigned _bitgetx32(unsigned char *__restrict in, unsigned b, unsigned bidx) {                        return _bzhi_u64( (*(unsigned long long *)((unsigned *)in+(bidx>>5))) >> (bidx&0x1f), b ); }
	
// like  bitgetx32 but for 16 bits integer array
static ALWAYS_INLINE unsigned  bitgetx16(unsigned char *__restrict in, unsigned b, unsigned  idx) { unsigned bidx = b*idx; return _bzhi_u32( (*(unsigned           *)((unsigned *)in+(bidx>>4))) >> (bidx& 0xf), b ); }
static ALWAYS_INLINE unsigned _bitgetx16(unsigned char *__restrict in, unsigned b, unsigned bidx) {                        return _bzhi_u32( (*(unsigned           *)((unsigned *)in+(bidx>>4))) >> (bidx& 0xf), b ); }

// Set a single value with index "idx" 
static ALWAYS_INLINE void      bitsetx32(unsigned char *__restrict in, unsigned b, unsigned  idx, unsigned v) { unsigned bidx = b*idx;  unsigned long long *p = (unsigned long long *)((unsigned *)in+(bidx>>5)); *p = ( *p & ~(((1ull<<b)-1) << (bidx&0x1f)) ) | (unsigned long long)v<<(bidx&0x1f);}
static ALWAYS_INLINE void      bitsetx16(unsigned char *__restrict in, unsigned b, unsigned  idx, unsigned v) { unsigned bidx = b*idx;  unsigned           *p = (unsigned           *)             in+(bidx>>4) ; *p = ( *p & ~(((1u  <<b)-1) << (bidx& 0xf)) ) |                     v<<(bidx& 0xf);}

// ---------------- DFOR : integrated bitpacking, for delta packed SORTED array (Ex. DocId in inverted index) -------------------------------
// start < out[0] < out[1] < ... < out[n-2] < out[n-1] < (1<<N)-1,    N=32,16
// out[0] = start + in[0] + 1;  out[1] = out[0] + in[1] + 1; ... ;  out[i] = out[i-1] + in[i] +  1
unsigned char *bitd1unpack32(  unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1unpack16(  unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);

// start <= out[0] <= out[1] <= ... <= out[n-2] <= out[n-1] <= (1<<N)-1  N=32,16
// out[0] = start + in[0];  out[1] = out[0] + in[1]; ... ;  out[i] = out[i-1] + in[i]
unsigned char *bitdunpack32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitdunpack16( unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);

unsigned char *bitzunpack32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);

// ---------------- For : Direct Access for packed SORTED array (Ex. DocId in inverted index) --------------------------------------------
// out[i] = start + in[i] + i + 1
unsigned char *bitf1unpack32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitf1unpack16( unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);

// out[i] = start + in[i] + i 
unsigned char *bitfunpack32(  unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitfunpack16(  unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b);

// ---------------- SIMD : unpack a bit packed integer array -------------------------------------------------------------------------------
// SIMD unpack a bitpacked integer array. Return value = end of packed buffer in
unsigned char *bitunpackv32(  unsigned char *__restrict in, unsigned n, unsigned       *__restrict out				  , unsigned b);
unsigned char *bitdunpackv32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitd1unpackv32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);
unsigned char *bitzunpackv32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b);

#ifdef __cplusplus
}
#endif
