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
//   "Integer Compression" utility - delta, for, zigzag / Floating point compression
#pragma warning( disable : 4005)
#pragma warning( disable : 4090)
#pragma warning( disable : 4068)

#include <math.h> //nan
#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h> //_Float16

#include "include_/conf.h"
#include "include_/bitutil.h"

#include "include_/bitutil_.h"

#define BT(_i_) { o |= ip[_i_]; x |= ip[_i_] ^ u0; }

#ifdef __AVX2__

uint32_t bit256v32(uint32_t *in, unsigned n, uint32_t *px) {
  uint32_t o = 0,x,u0 = in[0], *ip = in;
  __m256i vb0 = _mm256_set1_epi32(*in), 
          vo0 = _mm256_setzero_si256(), vx0 = _mm256_setzero_si256(),
          vo1 = _mm256_setzero_si256(), vx1 = _mm256_setzero_si256();
  for(; ip != in+(n&~(16-1)); ip += 16) {                                PREFETCH(ip+512,0);
    __m256i v0 = _mm256_loadu_si256((__m256i *) ip);
    __m256i v1 = _mm256_loadu_si256((__m256i *)(ip+8));
    vo0 = _mm256_or_si256(vo0, v0);
    vo1 = _mm256_or_si256(vo1, v1);
    vx0 = _mm256_or_si256(vx0, _mm256_xor_si256(v0, vb0));
    vx1 = _mm256_or_si256(vx1, _mm256_xor_si256(v1, vb0));
  }
  vo0 = _mm256_or_si256(vo0, vo1); o = mm256_hor_epi32(vo0);
  vx0 = _mm256_or_si256(vx0, vx1); x = mm256_hor_epi32(vx0);
  for(; ip != in+n; ip++) BT(0);
  if(px) *px = x;
  return o;
}

// delta ---------------------------------------------------------------------------------------------------------------
uint32_t bitd256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) { 
  uint32_t o = 0, x, *ip = in, u0 = in[0] - start;
  __m256i vb0 = _mm256_set1_epi32(u0),
          vo0 = _mm256_setzero_si256(), vx0 = _mm256_setzero_si256(),
          vo1 = _mm256_setzero_si256(), vx1 = _mm256_setzero_si256();           __m256i vs = _mm256_set1_epi32(start);
  for(; ip != in+(n&~(16-1)); ip += 16) {                                PREFETCH(ip+512,0);
    __m256i vi0 = _mm256_loadu_si256((__m256i *) ip);
    __m256i vi1 = _mm256_loadu_si256((__m256i *)(ip+8));                        __m256i v0 = mm256_delta_epi32(vi0,vs); vs = vi0;
                                                                                __m256i v1 = mm256_delta_epi32(vi1,vs); vs = vi1;
    vo0 = _mm256_or_si256(vo0, v0);
    vo1 = _mm256_or_si256(vo1, v1);
    vx0 = _mm256_or_si256(vx0, _mm256_xor_si256(v0, vb0));
    vx1 = _mm256_or_si256(vx1, _mm256_xor_si256(v1, vb0));
  }                                                                             start = (unsigned)_mm256_extract_epi32(vs, 7);
  vo0 = _mm256_or_si256(vo0, vo1); o = mm256_hor_epi32(vo0);
  vx0 = _mm256_or_si256(vx0, vx1); x = mm256_hor_epi32(vx0);

  for(;ip != in+n; ip++) {
    uint32_t u = *ip - start; start = *ip;
    o |= u;
    x |= u ^ u0;
  }
  if(px) *px = x;
  return o;
}

void bitddec256v32(uint32_t *in, unsigned n, unsigned start) {
  unsigned *ip = in;
  __m256i vs = _mm256_set1_epi32(start);
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m256i v =  _mm256_loadu_si256((__m256i *)ip);
    vs = mm256_scan_epi32(v,vs);
    _mm256_storeu_si256((__m256i *)ip, vs);
  }
  start = (unsigned)_mm256_extract_epi32(vs, 7);
  while(ip != in+n) {
    *ip = (start += (*ip));
    ip++;
  }
}

//-- delta 1 --------------------------------------------------------------------------------------------------------------------------------------
uint32_t bitd1256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) {
  uint32_t o, x, *ip = in, u0 = in[0]-start-1;
   __m256i vb0 = _mm256_set1_epi32(u0),
           vo0 = _mm256_setzero_si256(), vx0 = _mm256_setzero_si256(),
           vo1 = _mm256_setzero_si256(), vx1 = _mm256_setzero_si256();          __m256i vs = _mm256_set1_epi32(start), cv = _mm256_set1_epi32(1);
  for(; ip != in+(n&~(16-1)); ip += 16) {                                       PREFETCH(ip+512,0);
    __m256i vi0 = _mm256_loadu_si256((__m256i *)ip);
    __m256i vi1 = _mm256_loadu_si256((__m256i *)(ip+8));                        __m256i v0 = _mm256_sub_epi32(mm256_delta_epi32(vi0,vs),cv); vs = vi0;
                                                                                __m256i v1 = _mm256_sub_epi32(mm256_delta_epi32(vi1,vs),cv); vs = vi1;
    vo0 = _mm256_or_si256(vo0, v0);
    vo1 = _mm256_or_si256(vo1, v1);
    vx0 = _mm256_or_si256(vx0, _mm256_xor_si256(v0, vb0));
    vx1 = _mm256_or_si256(vx1, _mm256_xor_si256(v1, vb0));
  }                                                                             start = (unsigned)_mm256_extract_epi32(vs, 7);
  vo0 = _mm256_or_si256(vo0, vo1); o = mm256_hor_epi32(vo0);
  vx0 = _mm256_or_si256(vx0, vx1); x = mm256_hor_epi32(vx0);
  for(;ip != in+n; ip++) {
    uint32_t u = ip[0] - start-1; start = *ip;
    o |= u;
    x |= u ^ u0;
}
  if(px) *px = x;
  return o;
}

void bitd1dec256v32(uint32_t *in, unsigned n, uint32_t start) {
  __m256i vs = _mm256_set1_epi32(start),zv = _mm256_setzero_si256(), cv = _mm256_set_epi32(8,7,6,5,4,3,2,1);
  unsigned *ip = in;
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m256i v =  _mm256_loadu_si256((__m256i *)ip);                             vs = mm256_scani_epi32(v, vs, cv);
    _mm256_storeu_si256((__m256i *)ip, vs);
  }
                                                                                start = (unsigned)_mm256_extract_epi32(vs, 7);
  while(ip != in+n) {
    *ip = (start += (*ip) + 1);
    ip++;
  }
}

//--  Xor ----------------------------------------------------------------------------------------------------------------------
uint32_t bitx256v32(unsigned *in, unsigned n, uint32_t *px, unsigned start) {
  uint32_t o = 0, *ip = in;
  __m256i vo0 = _mm256_setzero_si256(),
          vo1 = _mm256_setzero_si256(), 
		   vs = _mm256_set1_epi32(start);
		   
  for(ip = in; ip != in+(n&~(16-1)); ip += 16) {                                //PREFETCH(ip+512,0);
    __m256i vi0 = _mm256_loadu_si256((__m256i *) ip);
    __m256i vi1 = _mm256_loadu_si256((__m256i *)(ip+8));                        __m256i v0 = mm256_xore_epi32(vi0,vs); vs = vi0; 
                                                                                __m256i v1 = mm256_xore_epi32(vi1,vs); vs = vi1; 
    vo0 = _mm256_or_si256(vo0, v0);
    vo1 = _mm256_or_si256(vo1, v1);
  }                                                                             start = (unsigned)_mm256_extract_epi32(vs, 7);
  vo0 = _mm256_or_si256(vo0, vo1); o = mm256_hor_epi32(vo0);
  for(;ip != in+n; ip++) {
    o |= ip[0] ^ start; start = ip[0];
  }
  if(px) *px = o;
  return o; 
}

//-- zigzag ------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t bitz256v32(unsigned *in, unsigned n, uint32_t *px, unsigned start) {
  uint32_t o, x, *ip; uint32_t u0 = zigzagenc32((int)in[0] - (int)start);
  __m256i vb0 = _mm256_set1_epi32(u0), 
          vo0 = _mm256_setzero_si256(), vx0 = _mm256_setzero_si256(),
          vo1 = _mm256_setzero_si256(), vx1 = _mm256_setzero_si256(),
		   vs = _mm256_set1_epi32(start);
		   
  for(ip = in; ip != in+(n&~(16-1)); ip += 16) {                                //PREFETCH(ip+512,0);
    __m256i vi0 = _mm256_loadu_si256((__m256i *) ip);
    __m256i vi1 = _mm256_loadu_si256((__m256i *)(ip+8));                        __m256i v0 = mm256_delta_epi32(vi0,vs); vs = vi0; v0 = mm256_zzage_epi32(v0);
                                                                                __m256i v1 = mm256_delta_epi32(vi1,vs); vs = vi1; v1 = mm256_zzage_epi32(v1);
    vo0 = _mm256_or_si256(vo0, v0);
    vo1 = _mm256_or_si256(vo1, v1);
    vx0 = _mm256_or_si256(vx0, _mm256_xor_si256(v0, vb0));
    vx1 = _mm256_or_si256(vx1, _mm256_xor_si256(v1, vb0));
  }                                                                             start = (unsigned)_mm256_extract_epi32(vs, 7);
  vo0 = _mm256_or_si256(vo0, vo1); o = mm256_hor_epi32(vo0);
  vx0 = _mm256_or_si256(vx0, vx1); x = mm256_hor_epi32(vx0);

  for(;ip != in+n; ip++) {
    uint32_t u = zigzagenc32((int)ip[0] - (int)start); start = *ip; //((int)(*ip) - (int)start);    //i = (i << 1) ^ (i >> 31);
    o |= u;
    x |= u ^ u0;
  }
  if(px) *px = x;
  return o; 
}

/* slower than SSE
void bitzdec256v32(unsigned *in, unsigned n, unsigned start) {
  __m256i vs = _mm256_set1_epi32(start);
  unsigned *ip = in;
  for(; ip != in+(n&~(16-1)); ip += 16) {
    __m256i iv0 = _mm256_loadu_si256((__m256i *)ip),
            iv1 = _mm256_loadu_si256((__m256i *)(ip+8));
    iv0 = mm256_zzagd_epi32(iv0);
    iv1 = mm256_zzagd_epi32(iv1);
    vs = mm256_scan_epi32(iv0, vs);
	//__m256i _vs = vs;
    _mm256_storeu_si256((__m256i *)ip, vs);
    vs = mm256_scan_epi32(iv1, vs);
    _mm256_storeu_si256((__m256i *)(ip+8), vs);
  }
  start = (unsigned)_mm256_extract_epi32(_mm256_srli_si256(vs,12), 4);
  while(ip != in+n) {
    unsigned z = *ip;
    *ip++ = (start += (z >> 1 ^ -(z & 1)));
  }
}*/

#else // avx2
//------------ 'or' for bitsize + 'xor' for all duplicate ------------------
#define BIT(_in_, _n_, _usize_) {\
  u0 = _in_[0]; o = x = 0;\
  for(ip = _in_; ip != _in_+(_n_&~(4-1)); ip += 4) { BT(0); BT(1); BT(2); BT(3); }\
  for(;ip != _in_+_n_; ip++) BT(0);\
}

uint8_t  bit8( uint8_t  *in, unsigned n, uint8_t  *px) { uint8_t  o,x,u0,*ip; BIT(in, n,  8); if(px) *px = x; return o; }
uint64_t bit64(uint64_t *in, unsigned n, uint64_t *px) { uint64_t o,x,u0,*ip; BIT(in, n, 64); if(px) *px = x; return o; }

uint16_t bit16(uint16_t *in, unsigned n, uint16_t *px) {
  uint16_t o, x, u0 = in[0], *ip = in;
  
    #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vo0 = _mm_setzero_si128(), vx0 = _mm_setzero_si128(),
          vo1 = _mm_setzero_si128(), vx1 = _mm_setzero_si128(), vb0 = _mm_set1_epi16(u0);
									
  for(; ip != in+(n&~(16-1)); ip += 16) {                                PREFETCH(ip+512,0);
    __m128i v0 = _mm_loadu_si128((__m128i *) ip);
    __m128i v1 = _mm_loadu_si128((__m128i *)(ip+8));
    vo0 = _mm_or_si128( vo0, v0);
    vo1 = _mm_or_si128( vo1, v1);
    vx0 = _mm_or_si128(vx0, _mm_xor_si128(v0, vb0));
    vx1 = _mm_or_si128(vx1, _mm_xor_si128(v1, vb0));
  }
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi16(vo0);
  vx0 = _mm_or_si128(vx0, vx1); x = mm_hor_epi16(vx0);
    #else
  ip = in; o = x = 0;
    #endif
	
  for(; ip != in+n; ip++) BT(0);
  if(px) *px = x;
  return o;
}

uint32_t bit32(uint32_t *in, unsigned n, uint32_t *px) {
  uint32_t o,x,u0 = in[0], *ip = in;
  
    #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vo0 = _mm_setzero_si128(), vx0 = _mm_setzero_si128(),
          vo1 = _mm_setzero_si128(), vx1 = _mm_setzero_si128(), vb0 = _mm_set1_epi32(u0);
									
  for(; ip != in+(n&~(8-1)); ip += 8) {                                  PREFETCH(ip+512,0);
    __m128i v0 = _mm_loadu_si128((__m128i *) ip);
    __m128i v1 = _mm_loadu_si128((__m128i *)(ip+4));
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
    vx0 = _mm_or_si128(vx0, _mm_xor_si128(v0, vb0));
    vx1 = _mm_or_si128(vx1, _mm_xor_si128(v1, vb0));
  }
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi32(vo0);
  vx0 = _mm_or_si128(vx0, vx1); x = mm_hor_epi32(vx0);
    #else
  ip = in; o = x = 0;
    #endif
	
  for(; ip != in+n; ip++) BT(0);
  if(px) *px = x;
  return o;
}

//----------------------------------------------------------- Delta ----------------------------------------------------------------
#define DE(_ip_,_i_) u = (_ip_[_i_]-start)-_md; start = _ip_[_i_];
#define BITDE(_t_, _in_, _n_, _md_, _act_) { _t_ _md = _md_, *_ip; o = x = 0;\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip += 4) { DE(_ip,0);_act_; DE(_ip,1);_act_; DE(_ip,2);_act_; DE(_ip,3);_act_; }\
  for(;_ip != _in_+_n_;_ip++) { DE(_ip,0); _act_; }\
}
//---- (min. Delta = 0)
//-- delta encoding
uint8_t   bitd8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { uint8_t  u, u0 = in[0]-start, o, x; BITDE(uint8_t,  in, n, 0, o |= u; x |= u^u0); if(px) *px = x; return o; }
uint64_t  bitd64(uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { uint64_t u, u0 = in[0]-start, o, x; BITDE(uint64_t, in, n, 0, o |= u; x |= u^u0); if(px) *px = x; return o; }

uint16_t bitd16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) {
  uint16_t o, x, *ip = in, u0 = in[0] - start;
    #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vb0 = _mm_set1_epi16(u0),
          vo0 = _mm_setzero_si128(), vx0 = _mm_setzero_si128(),
          vo1 = _mm_setzero_si128(), vx1 = _mm_setzero_si128();                 __m128i vs = _mm_set1_epi16(start);
  for(; ip != in+(n&~(16-1)); ip += 16) {                                PREFETCH(ip+512,0);
    __m128i vi0 = _mm_loadu_si128((__m128i *) ip);
    __m128i vi1 = _mm_loadu_si128((__m128i *)(ip+8));                           __m128i v0 = mm_delta_epi16(vi0,vs); vs = vi0;
                                                                                __m128i v1 = mm_delta_epi16(vi1,vs); vs = vi1;
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
    vx0 = _mm_or_si128(vx0, _mm_xor_si128(v0, vb0));
    vx1 = _mm_or_si128(vx1, _mm_xor_si128(v1, vb0));
  }                                                                             start = mm_cvtsi128_si16(_mm_srli_si128(vs,14));
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi16(vo0);
  vx0 = _mm_or_si128(vx0, vx1); x = mm_hor_epi16(vx0);
    #else
  ip = in; o = x = 0;
    #endif
	
  for(;ip != in+n; ip++) {
    uint16_t u = *ip - start; start = *ip;
    o |= u;
    x |= u ^ u0;
  }
  if(px) *px = x;
  return o;
}

uint32_t bitd32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) {
  uint32_t o = 0, x=0, *ip = in, u0 = in[0] - start;
  
    #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vb0 = _mm_set1_epi32(u0),
          vo0 = _mm_setzero_si128(), vx0 = _mm_setzero_si128(),
          vo1 = _mm_setzero_si128(), vx1 = _mm_setzero_si128();                 __m128i vs = _mm_set1_epi32(start);
  for(; ip != in+(n&~(8-1)); ip += 8) {                                  PREFETCH(ip+512,0);
    __m128i vi0 = _mm_loadu_si128((__m128i *)ip);
    __m128i vi1 = _mm_loadu_si128((__m128i *)(ip+4));                           __m128i v0 = mm_delta_epi32(vi0,vs); vs = vi0;
                                                                                __m128i v1 = mm_delta_epi32(vi1,vs); vs = vi1;
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
    vx0 = _mm_or_si128(vx0, _mm_xor_si128(v0, vb0));
    vx1 = _mm_or_si128(vx1, _mm_xor_si128(v1, vb0));
  }                                                                             start = _mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi32(vo0);
  vx0 = _mm_or_si128(vx0, vx1); x = mm_hor_epi32(vx0);
    #else
  ip = in; o = x = 0;
    #endif
	
  for(;ip != in+n; ip++) {
    uint32_t u = *ip - start; start = *ip;
    o |= u;
    x |= u ^ u0;
  }
  if(px) *px = x;
  return o;
}

//----- Undelta: In-place prefix sum (min. Delta = 0) -------------------
#define DD(i) _ip[i] = (start += _ip[i] + _md);
#define BITDD(_t_, _in_, _n_, _md_) { _t_ *_ip; const unsigned _md = _md_;\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip += 4) { DD(0); DD(1); DD(2); DD(3); }\
  for(;_ip != _in_+_n_; _ip++) DD(0);\
}

void bitddec8( uint8_t  *in, unsigned n, uint8_t  start) { BITDD(uint8_t,  in, n, 0); }
void bitddec16(uint16_t *in, unsigned n, uint16_t start) { BITDD(uint16_t, in, n, 0); }
void bitddec64(uint64_t *in, unsigned n, uint64_t start) { BITDD(uint64_t, in, n, 0); }

void bitddec32(uint32_t *in, unsigned n, unsigned start) {
    #if defined(__SSSE3__) || defined(__ARM_NEON)
  __m128i vs = _mm_set1_epi32(start);
  unsigned *ip = in;
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m128i v0 =  _mm_loadu_si128((__m128i *)ip);
    __m128i v1 =  _mm_loadu_si128((__m128i *)(ip+4));
    vs = mm_scan_epi32(v0, vs);
    _mm_storeu_si128((__m128i *)ip, vs);
    vs = mm_scan_epi32(v1, vs);
    _mm_storeu_si128((__m128i *)(ip+4), vs);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  while(ip != in+n) {
    *ip = (start += (*ip));
    ip++;
  }
    #else
  BITDD(uint32_t, in, n, 0);
    #endif
}

//----------- Zigzag Delta ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ZDE(i, _usize_) d = (_ip[i]-start)-_md; u = T2(zigzagenc, _usize_)(d - startd); startd = d; start = _ip[i]
#define BITZDE(_t_, _in_, _n_, _md_, _usize_, _act_) { _t_ *_ip, _md = _md_;\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip += 4) { ZDE(0, _usize_);_act_; ZDE(1, _usize_);_act_; ZDE(2, _usize_);_act_; ZDE(3, _usize_);_act_; }\
  for(;_ip != _in_+_n_;_ip++) { ZDE(0, _usize_); _act_; }\
}

uint8_t  bitzz8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { uint8_t  o=0, x=0,d,startd=0,u; BITZDE(uint8_t,  in, n, 1,  8, o |= u; x |= u ^ in[0]); if(px) *px = x; return o; }
uint16_t bitzz16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) { uint16_t o=0, x=0,d,startd=0,u; BITZDE(uint16_t, in, n, 1, 16, o |= u; x |= u ^ in[0]); if(px) *px = x; return o; }
uint32_t bitzz32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) { uint64_t o=0, x=0,d,startd=0,u; BITZDE(uint32_t, in, n, 1, 32, o |= u; x |= u ^ in[0]); if(px) *px = x; return o; }
uint64_t bitzz64(uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { uint64_t o=0, x=0,d,startd=0,u; BITZDE(uint64_t, in, n, 1, 64, o |= u; x |= u ^ in[0]); if(px) *px = x; return o; }

uint8_t  bitzzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta) { uint8_t  o=0,*op = out,u,d,startd=0; BITZDE(uint8_t,  in, n, mindelta,  8,o |= u;*op++ = u); return o;}
uint16_t bitzzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta) { uint16_t o=0,*op = out,u,d,startd=0; BITZDE(uint16_t, in, n, mindelta, 16,o |= u;*op++ = u); return o;}
uint32_t bitzzenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta) { uint32_t o=0,*op = out,u,d,startd=0; BITZDE(uint32_t, in, n, mindelta, 32,o |= u;*op++ = u); return o;}
uint64_t bitzzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta) { uint64_t o=0,*op = out,u,d,startd=0; BITZDE(uint64_t, in, n, mindelta, 64,o |= u;*op++ = u); return o;}

#define ZDD(i) u = _ip[i]; d = u - start; _ip[i] = zigzagdec64(u)+(int64_t)startd+_md; startd = d; start = u
#define BITZDD(_t_, _in_, _n_, _md_) { _t_ *_ip, startd=0,d,u; const unsigned _md = _md_;\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip += 4) { ZDD(0); ZDD(1); ZDD(2); ZDD(3); }\
  for(;_ip != _in_+_n_; _ip++) ZDD(0);\
}
void bitzzdec8( uint8_t  *in, unsigned n, uint8_t  start) { BITZDD(uint8_t,  in, n, 1); }
void bitzzdec16(uint16_t *in, unsigned n, uint16_t start) { BITZDD(uint16_t, in, n, 1); }
void bitzzdec64(uint64_t *in, unsigned n, uint64_t start) { BITZDD(uint64_t, in, n, 1); }
void bitzzdec32(uint32_t *in, unsigned n, uint32_t start) { BITZDD(uint32_t, in, n, 1); }

//-----Undelta: In-place prefix sum (min. Delta = 1) -------------------
uint8_t  bitd18( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { uint8_t  o=0,x=0,u; BITDE(uint8_t,  in, n, 1, o |= u; x |= u ^ in[0]); if(px) *px = x; return o; }
uint16_t bitd116(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) { uint16_t o=0,x=0,u; BITDE(uint16_t, in, n, 1, o |= u; x |= u ^ in[0]); if(px) *px = x; return o; }
uint64_t bitd164(uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { uint64_t o=0,x=0,u; BITDE(uint64_t, in, n, 1, o |= u; x |= u ^ in[0]); if(px) *px = x; return o; }

uint32_t bitd132(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) {
  uint32_t o = 0, x=0, *ip = in, u0 = in[0]-start-1;
  
   #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vb0 = _mm_set1_epi32(u0),
          vo0 = _mm_setzero_si128(), vx0 = _mm_setzero_si128(),
          vo1 = _mm_setzero_si128(), vx1 = _mm_setzero_si128();                 __m128i vs = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  for(; ip != in+(n&~(8-1)); ip += 8) {                                  PREFETCH(ip+512,0);
    __m128i vi0 = _mm_loadu_si128((__m128i *)ip);
    __m128i vi1 = _mm_loadu_si128((__m128i *)(ip+4));                           __m128i v0 = _mm_sub_epi32(mm_delta_epi32(vi0,vs),cv); vs = vi0;
                                                                                __m128i v1 = _mm_sub_epi32(mm_delta_epi32(vi1,vs),cv); vs = vi1;
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
    vx0 = _mm_or_si128(vx0, _mm_xor_si128(v0, vb0));
    vx1 = _mm_or_si128(vx1, _mm_xor_si128(v1, vb0));
  }                                                                             start = _mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi32(vo0);
  vx0 = _mm_or_si128(vx0, vx1); x = mm_hor_epi32(vx0);
    #else
  ip = in; o = x = 0;
    #endif
	
  for(;ip != in+n; ip++) {
    uint32_t u = ip[0] - start-1; start = *ip;
    o |= u;
    x |= u ^ u0;
  }
  if(px) *px = x;
  return o;
}

uint16_t bits128v16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) {
    #if defined(__SSE2__) || defined(__ARM_NEON)
  uint16_t *ip = in,b; __m128i bv = _mm_setzero_si128(), vs = _mm_set1_epi16(start), cv = _mm_set1_epi16(8);
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m128i iv = _mm_loadu_si128((__m128i *)ip);
    bv = _mm_or_si128(bv,_mm_sub_epi16(SUBI16x8(iv,vs),cv));
    vs = iv;
  }
  start = (unsigned short)_mm_cvtsi128_si32(_mm_srli_si128(vs,14));
  b = mm_hor_epi16(bv);
  if(px) *px = 0;
  return b;
    #endif
}

unsigned bits128v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) {
    #if defined(__SSE2__) || defined(__ARM_NEON)
  unsigned *ip = in,b; __m128i bv = _mm_setzero_si128(), vs = _mm_set1_epi32(start), cv = _mm_set1_epi32(4);
  for(; ip != in+(n&~(4-1)); ip += 4) {
    __m128i iv = _mm_loadu_si128((__m128i *)ip);
    bv = _mm_or_si128(bv,_mm_sub_epi32(SUBI32x4(iv,vs),cv));
    vs = iv;
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  b = mm_hor_epi32(bv);
  if(px) *px = 0;
  return b;
    #endif
}

void bitd1dec8( uint8_t  *in, unsigned n, uint8_t  start) { BITDD(uint8_t,  in, n, 1); }
void bitd1dec16(uint16_t *in, unsigned n, uint16_t start) { BITDD(uint16_t, in, n, 1); }
void bitd1dec64(uint64_t *in, unsigned n, uint64_t start) { BITDD(uint64_t, in, n, 1); }

void bitd1dec32(uint32_t *in, unsigned n, uint32_t start) {
    #if defined(__SSSE3__) || defined(__ARM_NEON)
  __m128i vs = _mm_set1_epi32(start), cv = _mm_set_epi32(4,3,2,1);
  unsigned *ip = in;
  for(; ip != in+(n&~(4-1)); ip += 4) {
    __m128i v =  _mm_loadu_si128((__m128i *)ip);
    vs = mm_scani_epi32(v, vs, cv);
    _mm_storeu_si128((__m128i *)ip, vs);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  while(ip != in+n) {
    *ip = (start += (*ip) + 1);
    ip++;
  }
    #else
  BITDD(uint32_t, in, n, 1);
    #endif
}

//---------Delta encoding/decoding (min. Delta = mindelta) -------------------
//determine min. delta for encoding w/ bitdiencNN function
#define DI(_ip_,_i_) u = _ip_[_i_] - start; start = _ip_[_i_]; if(u < mindelta) mindelta = u
#define BITDIE(_in_, _n_) {\
  for(_ip = _in_,mindelta = _ip[0]; _ip != _in_+(_n_&~(4-1)); _ip+=4) { DI(_ip,0); DI(_ip,1); DI(_ip,2); DI(_ip,3); }\
  for(;_ip != _in_+_n_;_ip++) DI(_ip,0);\
}

uint8_t  bitdi8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { uint8_t  mindelta,u,*_ip; BITDIE(in, n); if(px) *px = 0; return mindelta; }
uint16_t bitdi16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) { uint16_t mindelta,u,*_ip; BITDIE(in, n); if(px) *px = 0; return mindelta; }
uint32_t bitdi32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) { uint32_t mindelta,u,*_ip; BITDIE(in, n); if(px) *px = 0; return mindelta; }
uint64_t bitdi64(uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { uint64_t mindelta,u,*_ip; BITDIE(in, n); if(px) *px = 0; return mindelta; }

uint8_t  bitdienc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta) { uint8_t  o=0,x=0,*op = out,u; BITDE(uint8_t,  in, n, mindelta, o |= u; x |= u ^ in[0]; *op++ = u); return o; }
uint16_t bitdienc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta) { uint16_t o=0,x=0,*op = out,u; BITDE(uint16_t, in, n, mindelta, o |= u; x |= u ^ in[0]; *op++ = u); return o; }
uint64_t bitdienc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta) { uint64_t o=0,x=0,*op = out,u; BITDE(uint64_t, in, n, mindelta, o |= u; x |= u ^ in[0]; *op++ = u); return o; }
uint32_t bitdienc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta) {
    #if defined(__SSE2__) || defined(__ARM_NEON)
  unsigned *ip = in,b,*op = out;
  __m128i bv = _mm_setzero_si128(), vs = _mm_set1_epi32(start), cv = _mm_set1_epi32(mindelta), dv;
  for(; ip != in+(n&~(4-1)); ip += 4,op += 4) {
    __m128i iv = _mm_loadu_si128((__m128i *)ip);
    bv = _mm_or_si128(bv, dv = _mm_sub_epi32(mm_delta_epi32(iv,vs),cv));
    vs = iv;
    _mm_storeu_si128((__m128i *)op, dv);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  b = mm_hor_epi32(bv);
  while(ip != in+n) {
    unsigned x = *ip-start-mindelta;
    start = *ip++;
    b    |= x;
    *op++ = x;
  }
    #else
  uint32_t b = 0,*op = out, x, *_ip;
  BITDE(uint32_t, in, n, mindelta, b |= x; *op++ = x);
    #endif
  return b;
}

void bitdidec8(  uint8_t  *in, unsigned n, uint8_t  start, uint8_t  mindelta) { BITDD(uint8_t,  in, n, mindelta); }
void bitdidec16( uint16_t *in, unsigned n, uint16_t start, uint16_t mindelta) { BITDD(uint16_t, in, n, mindelta); }
void bitdidec32( uint32_t *in, unsigned n, uint32_t start, uint32_t mindelta) { BITDD(uint32_t, in, n, mindelta); }
void bitdidec64( uint64_t *in, unsigned n, uint64_t start, uint64_t mindelta) { BITDD(uint64_t, in, n, mindelta); }

//------------------- For ---------------------------------------------------------------------------------------------------
uint8_t  bitf8(  uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { if(px) *px = 0; return n?in[n-1] - start    :0; }
uint8_t  bitf18( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { if(px) *px = 0; return n?in[n-1] - start - n:0; }
uint16_t bitf16( uint16_t *in, unsigned n, uint16_t *px, uint16_t start) { if(px) *px = 0; return n?in[n-1] - start    :0; }
uint16_t bitf116(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) { if(px) *px = 0; return n?in[n-1] - start - n:0; }
uint32_t bitf32( uint32_t *in, unsigned n, uint32_t *px, uint32_t start) { if(px) *px = 0; return n?in[n-1] - start    :0; }
uint32_t bitf132(uint32_t *in, unsigned n, uint32_t *px, uint32_t start) { if(px) *px = 0; return n?in[n-1] - start - n:0; }
uint64_t bitf64( uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { if(px) *px = 0; return n?in[n-1] - start    :0; }
uint64_t bitf164(uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { if(px) *px = 0; return n?in[n-1] - start - n:0; }

//------------------- Zigzag -------------------------------------------------------------------------------------------------------------------------------------
#define ZE(i,_it_,_usize_) u = T2(zigzagenc, _usize_)((_it_)_ip[i]-(_it_)start); start = _ip[i]
#define BITZENC(_ut_, _it_, _usize_, _in_,_n_, _act_) { _ut_ *_ip; x = -1;\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip += 4) { ZE(0,_it_,_usize_);_act_; ZE(1,_it_,_usize_);_act_; ZE(2,_it_,_usize_);_act_; ZE(3,_it_,_usize_);_act_; }\
  for(;_ip != _in_+_n_; _ip++) { ZE(0,_it_,_usize_); _act_; }\
}

// 'or' bits for zigzag encoding
uint8_t  bitz8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { uint8_t  o=0, u,x; BITZENC(uint8_t,  int8_t, 8, in, n, o |= x); if(px) *px = 0; return o; }
uint64_t bitz64(uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { uint64_t o=0, u,x; BITZENC(uint64_t, int64_t,64,in, n, o |= x); if(px) *px = 0; return o; }

uint16_t bitz16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) {
  uint16_t o, x, *ip = in; 
  uint32_t u0 = zigzagenc16((int)in[0] - (int)start);

    #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vb0 = _mm_set1_epi16(u0), vo0 = _mm_setzero_si128(), vx0 = _mm_setzero_si128(),
                                    vo1 = _mm_setzero_si128(), vx1 = _mm_setzero_si128(); __m128i vs = _mm_set1_epi16(start);
  for(; ip != in+(n&~(16-1)); ip += 16) {            PREFETCH(ip+512,0);
    __m128i vi0 = _mm_loadu_si128((__m128i *) ip);
    __m128i vi1 = _mm_loadu_si128((__m128i *)(ip+8));                                      __m128i v0 = mm_delta_epi16(vi0,vs); vs = vi0; v0 = mm_zzage_epi16(v0);
                                                                                           __m128i v1 = mm_delta_epi16(vi1,vs); vs = vi1; v1 = mm_zzage_epi16(v1);
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
    vx0 = _mm_or_si128(vx0, _mm_xor_si128(v0, vb0));
    vx1 = _mm_or_si128(vx1, _mm_xor_si128(v1, vb0));
  }                                                                                         start = mm_cvtsi128_si16(_mm_srli_si128(vs,14));
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi16(vo0);
  vx0 = _mm_or_si128(vx0, vx1); x = mm_hor_epi16(vx0);
    #else
  ip = in; //uint16_t u; o=x=0; BITDE(uint16_t, in, n, 0, o |= u; x |= u^u0); //BITZENC(uint16_t, int16_t, 16, in, n, o |= u,x &= u^u0);
    #endif
  for(;ip != in+n; ip++) {
    uint16_t u = zigzagenc16((int)ip[0] - (int)start); //int i = ((int)(*ip) - (int)start); i = (i << 1) ^ (i >> 15);
    start = *ip;
    o |= u;
    x |= u ^ u0;
  }
  if(px) *px = x;
  return o;
}

uint32_t bitz32(unsigned *in, unsigned n, uint32_t *px, unsigned start) {
  uint32_t o, x, *ip=in,
           u0 = zigzagenc32((int)in[0] - (int)start);
    #if defined(__SSE2__) || defined(__ARM_NEON)
   __m128i vb0 = _mm_set1_epi32(u0),
           vo0 = _mm_setzero_si128(), vx0 = _mm_setzero_si128(),
           vo1 = _mm_setzero_si128(), vx1 = _mm_setzero_si128();                __m128i vs = _mm_set1_epi32(start);
  for(; ip != in+(n&~(8-1)); ip += 8) {                                  //PREFETCH(ip+512,0);
    __m128i vi0 = _mm_loadu_si128((__m128i *) ip);
    __m128i vi1 = _mm_loadu_si128((__m128i *)(ip+4));                           __m128i v0 = mm_delta_epi32(vi0,vs); vs = vi0; v0 = mm_zzage_epi32(v0);
                                                                                __m128i v1 = mm_delta_epi32(vi1,vs); vs = vi1; v1 = mm_zzage_epi32(v1);
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
    vx0 = _mm_or_si128(vx0, _mm_xor_si128(v0, vb0));
    vx1 = _mm_or_si128(vx1, _mm_xor_si128(v1, vb0));
  }                                                                             start = _mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi32(vo0);
  vx0 = _mm_or_si128(vx0, vx1); x = mm_hor_epi32(vx0);
    #else
  ip = in; o = x = 0; //uint32_t u; BITDE(uint32_t, in, n, 0, o |= u; x |= u^u0);
    #endif
  for(;ip != in+n; ip++) {
    uint32_t u = zigzagenc32((int)ip[0] - (int)start); start = *ip; //((int)(*ip) - (int)start);    //i = (i << 1) ^ (i >> 31);
    o |= u;
    x |= u ^ u0;
  }
  if(px) *px = x;
  return o;
}

uint8_t  bitzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta) { uint8_t  o=0,x,u,*op = out; BITZENC(uint8_t,  int8_t,  8,in, n, o |= u; *op++ = u); return o; }
uint16_t bitzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta) { uint16_t o=0,x,u,*op = out; BITZENC(uint16_t, int16_t,16,in, n, o |= u; *op++ = u); return o; }
uint64_t bitzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta) { uint64_t o=0,x,u,*op = out; BITZENC(uint64_t, int64_t,64,in, n, o |= u; *op++ = u); return o; }

uint32_t bitzenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta) {
    #if defined(__SSE2__) || defined(__ARM_NEON)
  unsigned *ip = in,b,*op = out;
  __m128i bv = _mm_setzero_si128(), vs = _mm_set1_epi32(start);
  for(; ip != in+(n&~(8-1)); ip += 8,op += 8) {
    __m128i iv0 = _mm_loadu_si128((__m128i *)ip),     dv0;
	__m128i iv1 = _mm_loadu_si128((__m128i *)(ip+4)), dv1;
    dv0 = mm_delta_epi32(iv0,vs); vs = iv0;
    dv0 = mm_zzage_epi32(dv0);
    bv = _mm_or_si128(bv, dv0);
    dv1 = mm_delta_epi32(iv1,vs); vs = iv1;
    dv1 = mm_zzage_epi32(dv1);
    bv = _mm_or_si128(bv, dv1);
    _mm_storeu_si128((__m128i *)op, dv0);
    _mm_storeu_si128((__m128i *)(op+4), dv1);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  b = mm_hor_epi32(bv);
  while(ip != in+n) {
    int x = ((int)(*ip)-(int)start);
    x = (x << 1) ^ (x >> 31);
    start = *ip++;
    b |= x;
    *op++ = x;
  }
    #else
  uint32_t b = 0, *op = out,x, u;
  BITZENC(uint32_t, int32_t, 32,in, n, b |= x; *op++ = x);
    #endif
  return bsr32(b);
}

#define ZD(_t_, _usize_, i) { _t_ _z = _ip[i]; _ip[i] = (start += T2(zigzagdec, _usize_)(_z)); }
#define BITZDEC(_t_, _usize_, _in_, _n_) { _t_ *_ip;\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip += 4) { ZD(_t_, _usize_, 0); ZD(_t_, _usize_, 1); ZD(_t_, _usize_, 2); ZD(_t_, _usize_, 3); }\
  for(;_ip != _in_+_n_;_ip++) ZD(_t_, _usize_, 0);\
}

void bitzdec8( uint8_t  *in, unsigned n, uint8_t  start) { BITZDEC(uint8_t,  8, in, n); }
void bitzdec64(uint64_t *in, unsigned n, uint64_t start) { BITZDEC(uint64_t, 64,in, n); }

void bitzdec16(uint16_t *in, unsigned n, uint16_t start) {
    #if defined(__SSSE3__) || defined(__ARM_NEON)
  __m128i vs = _mm_set1_epi16(start); //, c1 = _mm_set1_epi32(1), cz = _mm_setzero_si128();
  uint16_t *ip = in;
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m128i iv =  _mm_loadu_si128((__m128i *)ip);                   
	iv = mm_zzagd_epi16(iv);
    vs = mm_scan_epi16(iv, vs);
    _mm_storeu_si128((__m128i *)ip, vs);
  }
  start = (uint16_t)_mm_cvtsi128_si32(_mm_srli_si128(vs,14));
  while(ip != in+n) {
    uint16_t z = *ip;
    *ip++ = (start += (z >> 1 ^ -(z & 1)));
  }
    #else
  BITZDEC(uint16_t, 16, in, n);
    #endif
}

void bitzdec32(unsigned *in, unsigned n, unsigned start) {
    #if defined(__SSSE3__) || defined(__ARM_NEON)
  __m128i vs = _mm_set1_epi32(start);
  unsigned *ip = in;
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m128i iv0 = _mm_loadu_si128((__m128i *)ip),
            iv1 = _mm_loadu_si128((__m128i *)(ip+4));
    iv0 = mm_zzagd_epi32(iv0);
    iv1 = mm_zzagd_epi32(iv1);
    vs = mm_scan_epi32(iv0, vs);
	__m128i _vs = vs;
    vs = mm_scan_epi32(iv1, vs);
    _mm_storeu_si128((__m128i *)ip, _vs);
    _mm_storeu_si128((__m128i *)(ip+4), vs);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  while(ip != in+n) {
    unsigned z = *ip;
    *ip++ = (start += zigzagdec32(z));
  }
    #else
  BITZDEC(uint32_t, 32, in, n);
    #endif
}

//----------------------- XOR ------------------------------------------------------------------------------------------------------
#define XE(i) x = _ip[i] ^ start; start = _ip[i]
#define BITXENC(_t_, _in_, _n_, _act_) { _t_ *_ip;\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip += 4) { XE(0);_act_; XE(1);_act_; XE(2);_act_; XE(3);_act_; }\
  for(        ; _ip != _in_+ _n_;         _ip++   ) { XE(0);_act_; }\
}

uint8_t  bitx8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start) { uint8_t  o=0, u=0,x; BITXENC(uint8_t,  in, n, o |= x); if(px) *px = 0; return o; }
uint64_t bitx64(uint64_t *in, unsigned n, uint64_t *px, uint64_t start) { uint64_t o=0, u=0,x; BITXENC(uint64_t, in, n, o |= x); if(px) *px = 0; return o; }

uint16_t bitx16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start) {
  uint16_t o = 0, *ip = in;

    #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vo0 = _mm_setzero_si128(),
          vo1 = _mm_setzero_si128(),
           vs = _mm_set1_epi16(start);
  for(; ip != in+(n&~(16-1)); ip += 16) {            //PREFETCH(ip+512,0);
    __m128i vi0 = _mm_loadu_si128((__m128i *) ip);
    __m128i vi1 = _mm_loadu_si128((__m128i *)(ip+8));                                      __m128i v0 = mm_xore_epi16(vi0,vs); vs = vi0; 
                                                                                           __m128i v1 = mm_xore_epi16(vi1,vs); vs = vi1; 
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
  }                                                                                         start = mm_cvtsi128_si16(_mm_srli_si128(vs,14));
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi16(vo0);
    #endif
  for(;ip != in+n; ip++) {
    o |= ip[0] ^ start; start = ip[0];
  }
  if(px) *px = o;
  return o;
}
 
uint32_t bitx32(unsigned *in, unsigned n, uint32_t *px, uint32_t start) {
  uint32_t o = 0, *ip = in;
  
    #if defined(__SSE2__) || defined(__ARM_NEON)
  __m128i vo0 = _mm_setzero_si128(),
          vo1 = _mm_setzero_si128(),                                          
		   vs = _mm_set1_epi32(start);
  for(; ip != in+(n&~(8-1)); ip += 8) {                                  //PREFETCH(ip+512,0);
    __m128i vi0 = _mm_loadu_si128((__m128i *) ip);
    __m128i vi1 = _mm_loadu_si128((__m128i *)(ip+4));                           __m128i v0 = mm_xore_epi32(vi0,vs); vs = vi0; 
                                                                                __m128i v1 = mm_xore_epi32(vi1,vs); vs = vi1;
    vo0 = _mm_or_si128(vo0, v0);
    vo1 = _mm_or_si128(vo1, v1);
  }                                                                             start = _mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  vo0 = _mm_or_si128(vo0, vo1); o = mm_hor_epi32(vo0);
    #endif
  for(;ip != in+n; ip++) {
    o |= ip[0] ^ start; start = ip[0];
  }
  if(px) *px = o;
  return o;
}

uint8_t  bitxenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start) { uint8_t  b = 0,*op = out,x; BITXENC(uint8_t,  in, n, b |= x; *op++ = x); return b; }
uint16_t bitxenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start) { uint16_t b = 0,*op = out,x; BITXENC(uint16_t, in, n, b |= x; *op++ = x); return b; }
uint32_t bitxenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start) { uint32_t b = 0,*op = out,x; BITXENC(uint32_t, in, n, b |= x; *op++ = x); return b; }
uint64_t bitxenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start) { uint64_t b = 0,*op = out,x; BITXENC(uint64_t, in, n, b |= x; *op++ = x); return b; }

#define XD(i) _ip[i] = (start ^= _ip[i])
#define BITXDEC(_t_, _in_, _n_) { _t_ *_ip, _x;\
  for(_ip = _in_;_ip != _in_+(_n_&~(4-1)); _ip += 4) { XD(0); XD(1); XD(2); XD(3); }\
  for(        ;_ip != _in_+ _n_        ; _ip++   )   XD(0);\
}

void bitxdec8( uint8_t  *in, unsigned n, uint8_t  start) { BITXDEC(uint8_t,  in, n); }
void bitxdec64(uint64_t *in, unsigned n, uint64_t start) { BITXDEC(uint64_t, in, n); }

void bitxdec16(uint16_t *in, unsigned n, uint16_t start) {
    #if defined(__SSSE3__) || defined(__ARM_NEON)
  __m128i vs = _mm_set1_epi16(start);
  uint16_t *ip = in;
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m128i iv =  _mm_loadu_si128((__m128i *)ip);                   
	vs = mm_xord_epi16(iv, vs);
    _mm_storeu_si128((__m128i *)ip, vs);
  }
  start = (uint16_t)_mm_cvtsi128_si32(_mm_srli_si128(vs,14));
  while(ip != in+n) {
    uint16_t z = *ip;
    *ip++ = (start ^= z);
  }
    #else
  BITXDEC(uint16_t, in, n);
    #endif
}

void bitxdec32(unsigned *in, unsigned n, unsigned start) {
    #if defined(__SSSE3__) || defined(__ARM_NEON)
  __m128i vs = _mm_set1_epi32(start);
  unsigned *ip = in;
  for(; ip != in+(n&~(8-1)); ip += 8) {
    __m128i iv0 = _mm_loadu_si128((__m128i *)ip),
            iv1 = _mm_loadu_si128((__m128i *)(ip+4));
    vs = mm_xord_epi32(iv0, vs);
	__m128i _vs = vs;
    vs = mm_xord_epi32(iv1, vs);
    _mm_storeu_si128((__m128i *)ip,    _vs);
    _mm_storeu_si128((__m128i *)(ip+4), vs);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(vs,12));
  while(ip != in+n) {
    unsigned z = *ip;
    *ip++ = (start ^= z);
  }
    #else
  BITXDEC(uint32_t, 32, in, n);
    #endif
}

//-------------- For : calc max. bits, min,max value ------------------------
#define FM(i) mi = _ip[i] < mi?_ip[i]:mi; mx = _ip[i] > mx?_ip[i]:mx
#define BITFM(_t_, _in_,_n_) { _t_ *_ip; \
  for(_ip = _in_, mi = mx = *_ip; _ip != _in_+(_n_&~(4-1)); _ip += 4) { FM(0); FM(1); FM(2); FM(3); }\
  for(;_ip != _in_+_n_; _ip++) FM(0);\
}

uint8_t  bitfm8( uint8_t  *in, unsigned n, uint8_t   *px, uint8_t  *pmin) { uint8_t  mi,mx; BITFM(uint8_t,  in, n); *pmin = mi; if(px) *px = 0; return mx - mi; }
uint16_t bitfm16(uint16_t *in, unsigned n, uint16_t  *px, uint16_t *pmin) { uint16_t mi,mx; BITFM(uint16_t, in, n); *pmin = mi; if(px) *px = 0; return mx - mi; }
uint32_t bitfm32(uint32_t *in, unsigned n, uint32_t  *px, uint32_t *pmin) { uint32_t mi,mx; BITFM(uint32_t, in, n); *pmin = mi; if(px) *px = 0; return mx - mi; }
uint64_t bitfm64(uint64_t *in, unsigned n, uint64_t  *px, uint64_t *pmin) { uint64_t mi,mx; BITFM(uint64_t, in, n); *pmin = mi; if(px) *px = 0; return mx - mi; }

//---------------------- any esize ----------------------------------
void bitxenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case 2 : bitxenc16(in, n/2, out, 0); break;
    case 4 : bitxenc32(in, n/4, out, 0); break;
    case 8 : bitxenc64(in, n/8, out, 0); break;
    default: bitxenc8( in, n/1, out, 0); break;
  }
}

void bitxdec(unsigned char *in, unsigned n, unsigned esize) {
  switch(esize) {
    case 2 : bitxdec16(in, n/2, 0);break;
    case 4 : bitxdec32(in, n/4, 0);break;
    case 8 : bitxdec64(in, n/8, 0);break;
    default: bitxdec8( in, n/1, 0);break;
  }
}

void bitzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case 2 : bitzenc16(in, n/2, out, 0, 0); break;
    case 4 : bitzenc32(in, n/4, out, 0, 0); break;
    case 8 : bitzenc64(in, n/8, out, 0, 0); break;
    default: bitzenc8( in, n/1, out, 0, 0); break;
  }
}

void bitzdec(unsigned char *in, unsigned n, unsigned esize) {
  switch(esize) {
    case 2 : bitzdec16(in, n/2, 0);break;
    case 4 : bitzdec32(in, n/4, 0);break;
    case 8 : bitzdec64(in, n/8, 0);break;
    default: bitzdec8( in, n/1, 0);break;
  }
}

//----------- Quantization -----------------------------------
#define ROUND16(x) roundf(x)
#define ROUND32(x) roundf(x)
#define ROUND64(x) round(x)

#define _FPQUANTE(t_s, _x_, _fmin_, _delta_) T2(ROUND,t_s)(((_x_) - _fmin_)*_delta_)

#define FPQUANTE(t_t, in, n, out, b, t_s, pfmin, pfmax) { t_t fmax = in[0], fmin = in[0], *ip;\
  for(ip = in; ip < in+n; ip++) { if(*ip > fmax) fmax = *ip; else if(*ip < fmin) fmin = *ip; } *pfmin = fmin; *pfmax = fmax;/*min,max*/\
  fmax = (fmax == fmin)?(t_t)0.0:BZMASK32(b)/(fmax - fmin);\
  for(ip = in; ip < in+n; ip++) *out++ = _FPQUANTE(t_s, ip[0],fmin,fmax);\
}

#define FPQUANTD(t_t, in, n, out, b, fmin, fmax) { t_t *op;\
  fmax = (fmax - fmin) / BZMASK32(b);\
  for(op = out; op < out+n; op++) *op = fmin + (*in++) * fmax; \
  t_t fmax = out[0], fmin = out[0]; for(op = out; op < out+n; op++) { if(*op > fmax) fmax = *op; else if(*op < fmin) fmin = *op; }   \
   printf("RANGE=[%g-%g]=%g ", (double)fmin, (double)fmax, (double)fmax - (double)fmin);\
}

  #if defined(FLT16_BUILTIN) 
void fpquant8e16( _Float16 *in, size_t n, uint8_t  *out, unsigned b, _Float16 *pfmin, _Float16 *pfmax) { FPQUANTE(_Float16, in, n, out, b, 16, pfmin, pfmax); }
void fpquant16e16(_Float16 *in, size_t n, uint16_t *out, unsigned b, _Float16 *pfmin, _Float16 *pfmax) { FPQUANTE(_Float16, in, n, out, b, 16, pfmin, pfmax); }
  #endif

void fpquant8e32(    float *in, size_t n, uint8_t  *out, unsigned b, float    *pfmin,    float *pfmax) { FPQUANTE(   float, in, n, out, b, 32, pfmin, pfmax); }
void fpquant16e32(   float *in, size_t n, uint16_t *out, unsigned b, float    *pfmin,    float *pfmax) { FPQUANTE(   float, in, n, out, b, 32, pfmin, pfmax); }
void fpquant32e32(   float *in, size_t n, uint32_t *out, unsigned b, float    *pfmin,    float *pfmax) { FPQUANTE(   float, in, n, out, b, 32, pfmin, pfmax); }

void fpquant8e64(   double *in, size_t n, uint8_t  *out, unsigned b, double   *pfmin,   double *pfmax) { FPQUANTE(  double, in, n, out, b, 64, pfmin, pfmax); }
void fpquant16e64(  double *in, size_t n, uint16_t *out, unsigned b, double   *pfmin,   double *pfmax) { FPQUANTE(  double, in, n, out, b, 64, pfmin, pfmax); }
void fpquant32e64(  double *in, size_t n, uint32_t *out, unsigned b, double   *pfmin,   double *pfmax) { FPQUANTE(  double, in, n, out, b, 64, pfmin, pfmax); }
void fpquant64e64(  double *in, size_t n, uint64_t *out, unsigned b, double   *pfmin,   double *pfmax) { FPQUANTE(  double, in, n, out, b, 64, pfmin, pfmax); }

  #if defined(FLT16_BUILTIN) 
void fpquant8d16( uint8_t  *in, size_t n, _Float16 *out, unsigned b, _Float16   fmin, _Float16   fmax) { FPQUANTD(_Float16, in, n, out, b,  fmin,  fmax); }
void fpquant16d16(uint16_t *in, size_t n, _Float16 *out, unsigned b, _Float16   fmin, _Float16   fmax) { FPQUANTD(_Float16, in, n, out, b,  fmin,  fmax); }
  #endif

void fpquant8d32( uint8_t  *in, size_t n, float    *out, unsigned b, float      fmin,    float   fmax) { FPQUANTD(   float, in, n, out, b,  fmin,  fmax); }
void fpquant16d32(uint16_t *in, size_t n, float    *out, unsigned b, float      fmin,    float   fmax) { FPQUANTD(   float, in, n, out, b,  fmin,  fmax); }
void fpquant32d32(uint32_t *in, size_t n, float    *out, unsigned b, float      fmin,    float   fmax) { FPQUANTD(   float, in, n, out, b,  fmin,  fmax); }

void fpquant8d64( uint8_t  *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax) { FPQUANTD(  double, in, n, out, b,  fmin,  fmax); }
void fpquant16d64(uint16_t *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax) { FPQUANTD(  double, in, n, out, b,  fmin,  fmax); }
void fpquant32d64(uint32_t *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax) { FPQUANTD(  double, in, n, out, b,  fmin,  fmax); }
void fpquant64d64(uint64_t *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax) { FPQUANTD(  double, in, n, out, b,  fmin,  fmax); }

//----------- Lossy floating point conversion: pad the trailing mantissa bits with zero bits according to the relative error e (ex. 0.00001)  ----------
  #if defined(FLT16_BUILTIN) 
// https://clang.llvm.org/docs/LanguageExtensions.html#half-precision-floating-point
_Float16 _fprazor16(_Float16 d, float e, int lg2e) {
  uint16_t du = ctou16(&d), sign, u;
  int      b  = (du>>10 & 0x1f) - 15; // exponent=[5 bits,bias=15], mantissa=10 bits SeeeeeMMMMMMMMMM
  _Float16 ed;
  if ((b = 12 - b - lg2e) <= 0) 
	return d;
  b     = b > 10?10:b;
  sign  = du & (1<<15); 
  du   &= 0x7fff;       
  for(d = ctof16(&du), ed = e * d;;) {
    u = du & (~((1u<<(--b))-1)); if(d - ctof16(&u) <= ed) break;
    u = du & (~((1u<<(--b))-1)); if(d - ctof16(&u) <= ed) break;
  }
  u |= sign;
  return ctof16(&u);
}

void fprazor16(_Float16 *in, unsigned n, _Float16 *out, float e) { 
  int lg2e = -log(e)/log(2.0); _Float16 *ip; 
  
  for (ip = in; ip < in+n; ip++,out++)
    *out = _fprazor16(*ip, e, lg2e); 
}
  #endif

float _fprazor32(float d, float e, int lg2e) {
  uint32_t du = ctou32(&d), sign, u;
  int      b  = (du>>23 & 0xff) - 0x7e;
  float    ed;
 
  if((b = 25 - b - lg2e) <= 0)
    return d;                                         AS(!isnan(d), "_fprazor32: isnan");
  b    = b > 23?23:b;
  sign = du & (1<<31);
  du  &= 0x7fffffffu;
  
  for(d = ctof32(&du), ed = e * d;;) {
    u = du & (~((1u<<(--b))-1)); if(d - ctof32(&u) <= ed) break;
    u = du & (~((1u<<(--b))-1)); if(d - ctof32(&u) <= ed) break;
    u = du & (~((1u<<(--b))-1)); if(d - ctof32(&u) <= ed) break;
  }
  u |= sign;
  return ctof32(&u);
}

void fprazor32(float *in, unsigned n, float *out, float e) { 
  int   lg2e = -log(e)/log(2.0); 
  float *ip; 
  for(ip = in; ip < in+n; ip++,out++) 
	*out = _fprazor32(*ip, e, lg2e); 
}

double _fprazor64(double d, double e, int lg2e) { //if(isnan(d)) return d;
  uint64_t du = ctou64(&d), sign, u;
  int      b  = (du>>52 & 0x7ff) - 0x3fe;
  double   ed;
  
  if((b = 54 - b - lg2e) <= 0)
    return d;
  b     = b > 52?52:b;
  sign  = du & (1ull<<63); 
  du   &= 0x7fffffffffffffffull;

  for(d = ctof64(&du), ed = e * d;;) {
    u = du & (~((1ull<<(--b))-1)); if(d - ctof64(&u) <= ed) break;
    u = du & (~((1ull<<(--b))-1)); if(d - ctof64(&u) <= ed) break;
  }
  u |= sign; 
  return ctof64(&u);
}

void fprazor64(double *in, unsigned n, double *out, double e) { 
  int    lg2e = -log(e)/log(2.0); 
  double *ip; 
  
  for(ip = in; ip < in+n; ip++,out++) 
	*out = _fprazor64(*ip, e, lg2e); 
}
#endif
