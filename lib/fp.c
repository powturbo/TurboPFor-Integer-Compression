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
//   "Floating Point + Integer Compression (All integer compression functions can be used for float/double and vice versa)"
  #ifndef USIZE
#include <string.h>
#include "include_/conf.h"
#include "include_/bitutil.h"
#include "include_/fp.h"
#include "include_/vp4.h"

#include "include_/bitutil_.h"

#pragma warning( disable : 4005)
#pragma warning( disable : 4090)
#pragma warning( disable : 4068)

#define CCODEL     3
#define lz2l(_x_)  cquant[_x_] // quantize leading zeros
#define l2c( _x_)  ccode[_x_]  // encode
#define c2l( _x_)  dcode[_x_]  // decode
 
static unsigned char ccode[] = { // optimized for 32/64 bits
  0,  0,  0,  0,  0,  0,  1,  1,   1,  1,  2,  2,  2,  2,  3,  3,
  3,  3,  4,  4,  5,  5,  6,  6,   7,  7,  7,  7,  7,  7,  7,  7,
  7,  7,  7,  7,  7,  7,  7,  7,   7,  7,  7,  7,  7,  7,  7,  7,
  7,  7,  7,  7,  7,  7,  7,  7,   7,  7,  7,  7,  7,  7,  7,  7
};
static unsigned char cquant[] = {
  0,  0,  0,  0,  0,  0,  6,  6,   6,  6, 10, 10, 10, 10, 14, 14,
 14, 14, 18, 18, 20, 20, 22, 22,  24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24,  24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24,  24, 24, 24, 24, 24, 24, 24, 24
};
static unsigned char dcode[] = {0, 6,10, 14, 18, 20, 22, 24};

//---------------------- template generation --------------------------------------------
#define VSIZE 128

#define P4ENC  p4enc
#define P4DEC  p4dec
#define P4ENCV p4enc
#define P4DECV p4dec

#define NL 18
#define N4 17 // must be > 16

#define N_0 3
#define N_1 4

#define N2  3
#define N3  5
#define USIZE 8
#include "fp.c"
#undef P4ENCV
#undef P4DECV
#undef N_0
#undef N_1
#undef N2
#undef N3

#define P4ENCV p4enc128v
#define P4DECV p4dec128v

#define N_0 3
#define N_1 5

#define N2   6
#define N3  12
#define USIZE 16
#include "fp.c"
#undef N_0
#undef N_1
#undef N2 
#undef N3

#define N_0 4
#define N_1 6

#define N2  6 // for seconds time series
#define N3 10
#define USIZE 32
#include "fp.c"
#undef N4
#undef N1
#undef N3
#undef N_1

#define N_1 7
#define N2  6    // for seconds/milliseconds,... time series
#define N3 12
#define N4 20    // must be > 16
#define USIZE 64
#include "fp.c"

  #else //-------------------------------------- Template functions ------------------------------------------------------------

#define XORENC( _u_, _pu_, _usize_) ((_u_)^(_pu_))  // xor predictor
#define XORDEC( _u_, _pu_, _usize_) ((_u_)^(_pu_))
#define ZZAGENC(_u_, _pu_, _usize_)  T2(zigzagenc,_usize_)((_u_)-(_pu_)) //zigzag predictor
#define ZZAGDEC(_u_, _pu_, _usize_) (T2(zigzagdec,_usize_)(_u_)+(_pu_))

#define uint_t T3(uint, USIZE, _t)
#define int_t  T3(int,  USIZE, _t)

//-------- TurboPFor Zigzag of zigzag for unsorted/sorted integer/floating point array ---------------------------------------
size_t T2(p4nzzenc128v,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        _p[VSIZE+32], *ip, *p, pd = 0;
  unsigned char *op = out, *out_ = out+n*USIZE/8;

  #define FE(_i_,_usize_) { T3(uint, USIZE, _t) u = ip[_i_]; start = u-start; p[_i_] = ZZAGENC(start,pd,_usize_); pd = start; start = u; }
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
    op = T2(P4ENCV,USIZE)(_p, VSIZE, op);                                       PREFETCH(ip+512,0); if(op >= out_) goto e;
  }
  if((n = (in+n)-ip) != 0) {
    for(p = _p; p != &_p[n]; p++,ip++) FE(0,USIZE);
    op = T2(P4ENC,USIZE)(_p, n, op);                                            if(op >= out_) goto e;
  }
  if(op >= out_) {
    e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op+=n*(USIZE/8);
  }
  return op - out;
  #undef FE
}

size_t T2(p4nzzdec128v,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t        _p[VSIZE+32],*p, *op, pd=0;
  unsigned char *ip = in;
  if(*ip == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return n*(USIZE/8); }

  #define FD(_i_,_usize_) { T3(uint, USIZE, _t) u = ZZAGDEC(p[_i_],start+pd,_usize_); op[_i_] = u; pd = u - start; start = u; }
  for(op = out; op != out+(n&~(VSIZE-1)); ) {                           PREFETCH(ip+512,0);
    for(ip = T2(P4DECV,USIZE)(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if((n = (out+n) - op) != 0)
    for(ip = T2(P4DEC,USIZE)(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  return ip - in;
  #undef FD
}

/*---------------- TurboFloat XOR: last value Predictor with TurboPFor ---------------------------------------------------------
 Compress significantly (115% - 160%) better and faster than facebook's Gorilla algorithm for values
 BEST results are obtained with LOSSY COMPRESSION (using fprazor32/fprazor64 in bitutil.c)
 Algorithm:
 1: XOR value with previous value. We have now leading (for common sign/exponent bits) + mantissa trailing zero bits
 2: Eliminate the common block leading zeros of sign/exponent by shifting all values in the block to left
 3: reverse values to bring the mantissa trailing zero bits to left for better compression with TurboPFor
*/
size_t T2(fpxenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t         _p[VSIZE+32], *ip, *p;
  unsigned char *op = out, *out_ = out+n*USIZE/8;

    #if defined(__AVX2__) && USIZE >= 32
  #define _mm256_set1_epi64(a) _mm256_set1_epi64x(a)
  __m256i sv = T2(_mm256_set1_epi, USIZE)(start);
    #elif (defined(__SSSE3__) || defined(__ARM_NEON)) && (USIZE == 16 || USIZE == 32)
  #define _mm_set1_epi64(a) _mm_set1_epi64x(a)
  __m128i sv = T2(_mm_set1_epi, USIZE)(start);
    #endif

  #define FE(_i_,_usize_) { T3(uint, _usize_, _t) u = ip[_i_]; p[_i_] = XORENC(u, start,_usize_); b |= p[_i_]; start = u; }
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) { uint_t b = 0;
      #if defined(__AVX2__) && USIZE >= 32
    __m256i bv = _mm256_setzero_si256();
    for(p = _p; p != &_p[VSIZE]; p+=64/(USIZE/8),ip+=64/(USIZE/8)) {
      __m256i v0 = _mm256_loadu_si256((__m256i *) ip);
      __m256i v1 = _mm256_loadu_si256((__m256i *)(ip+32/(USIZE/8)));
              sv = T2(mm256_xore_epi, USIZE)(v0,sv); bv = _mm256_or_si256(bv, sv); _mm256_storeu_si256((__m256i *) p,               sv); sv = v0;
              sv = T2(mm256_xore_epi, USIZE)(v1,sv); bv = _mm256_or_si256(bv, sv); _mm256_storeu_si256((__m256i *)(p+32/(USIZE/8)), sv); sv = v1;
    }
    start = (uint_t)T2(_mm256_extract_epi,USIZE)(sv, 256/USIZE-1);
    b     = T2(mm256_hor_epi, USIZE)(bv);
      #elif (defined(__SSSE3__) || defined(__ARM_NEON)) && (USIZE == 16 || USIZE == 32)
    __m128i bv = _mm_setzero_si128();
    for(p = _p; p != &_p[VSIZE]; p+=32/(USIZE/8),ip+=32/(USIZE/8)) {
      __m128i v0 = _mm_loadu_si128((__m128i *) ip);
      __m128i v1 = _mm_loadu_si128((__m128i *)(ip+16/(USIZE/8)));
              sv = T2(mm_xore_epi, USIZE)(v0,sv);    bv = _mm_or_si128(bv, sv);        _mm_storeu_si128((__m128i *) p,               sv); sv = v0;
              sv = T2(mm_xore_epi, USIZE)(v1,sv);    bv = _mm_or_si128(bv, sv);        _mm_storeu_si128((__m128i *)(p+16/(USIZE/8)), sv); sv = v1;
    }
    start = (uint_t)T2(mm_cvtsi128_si,USIZE)(_mm_srli_si128(sv,16-USIZE/8));
    b     = T2(mm_hor_epi, USIZE)(bv);
      #else
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
      #endif
    *op++ = b = T2(clz,USIZE)(b);
    #define TR(_i_,_usize_) p[_i_] = T2(rbit,_usize_)(p[_i_]<<b)
      #if defined(__AVX2__) && USIZE >= 32
    for(p = _p; p != &_p[VSIZE]; p+=64/(USIZE/8)) {
      __m256i v0 = _mm256_loadu_si256((__m256i *)p);
      __m256i v1 = _mm256_loadu_si256((__m256i *)(p+32/(USIZE/8)));
              v0 = T2(_mm256_slli_epi, USIZE)(v0,b);
              v1 = T2(_mm256_slli_epi, USIZE)(v1,b);
              v0 = T2( mm256_rbit_epi, USIZE)(v0);
              v1 = T2( mm256_rbit_epi, USIZE)(v1);
                   _mm256_storeu_si256((__m256i *) p, v0);
                   _mm256_storeu_si256((__m256i *)(p+32/(USIZE/8)), v1);
    }
      #elif (defined(__SSSE3__) || defined(__ARM_NEON)) && (USIZE == 16 || USIZE == 32)
    for(p = _p; p != &_p[VSIZE]; p+=32/(USIZE/8)) {
      __m128i v0 = _mm_loadu_si128((__m128i *) p);
      __m128i v1 = _mm_loadu_si128((__m128i *)(p+16/(USIZE/8)));
              v0 = T2(_mm_slli_epi, USIZE)(v0,b);
              v0 = T2( mm_rbit_epi, USIZE)(v0);
              v1 = T2(_mm_slli_epi, USIZE)(v1,b);
              v1 = T2( mm_rbit_epi, USIZE)(v1);
      _mm_storeu_si128((__m128i *) p,               v0);
      _mm_storeu_si128((__m128i *)(p+16/(USIZE/8)), v1);
    }
      #else
    for(p = _p; p != &_p[VSIZE]; p+=4) { TR(0,USIZE); TR(1,USIZE); TR(2,USIZE); TR(3,USIZE); }
      #endif
    op = T2(P4ENCV,USIZE)(_p, VSIZE, op);                                                    PREFETCH(ip+512,0); if(op >= out_) goto e;
  }
  if((n = (in+n)-ip) != 0) { uint_t b = 0;
    for(p = _p; p != &_p[n]; p++,ip++) FE(0,USIZE);
    b = T2(clz,USIZE)(b);
    *op++ = b;
    for(p = _p; p != &_p[n]; p++) TR(0,USIZE);
    op = T2(P4ENC,USIZE)(_p, n, op);                                                         if(op >= out_) goto e;
  }
  if(op >= out_) {
    e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op+=n*(USIZE/8);
  }
  return op - out;
  #undef FE
}

size_t T2(fpxdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t        *op, _p[VSIZE+32],*p;
  unsigned char *ip = in;
  if(*ip == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return n*(USIZE/8); }

    #if defined(__AVX2__) && USIZE >= 32
  #define _mm256_set1_epi64(a) _mm256_set1_epi64x(a)
  __m256i sv = T2(_mm256_set1_epi, USIZE)(start);
    #elif (defined(__SSSE3__) || defined(__ARM_NEON)) && (USIZE == 16 || USIZE == 32)
  #define _mm_set1_epi64(a) _mm_set1_epi64x(a)
  __m128i sv = T2(_mm_set1_epi, USIZE)(start);
    #endif
  #define FD(_i_,_usize_) { T3(uint, USIZE, _t) u = p[_i_]; u = T2(rbit,_usize_)(u)>>b; u = XORDEC(u, start,_usize_); op[_i_] = start = u; }
  for(op = out; op != out+(n&~(VSIZE-1)); ) {                           PREFETCH(ip+384,0);
    unsigned b = *ip++; ip = T2(P4DECV,USIZE)(ip, VSIZE, _p);

      #if defined(__AVX2__) && USIZE >= 32
    for(p = _p; p != &_p[VSIZE]; p+=64/(USIZE/8),op+=64/(USIZE/8)) {
      __m256i v0 = _mm256_loadu_si256((__m256i *)p);
      __m256i v1 = _mm256_loadu_si256((__m256i *)(p+32/(USIZE/8)));
              v0 = T2( mm256_rbit_epi, USIZE)(v0);
              v1 = T2( mm256_rbit_epi, USIZE)(v1);
              v0 = T2(_mm256_srli_epi, USIZE)(v0,b);
              v1 = T2(_mm256_srli_epi, USIZE)(v1,b);
              v0 = T2( mm256_xord_epi, USIZE)(v0,sv);
              sv = T2( mm256_xord_epi, USIZE)(v1,v0);
                   _mm256_storeu_si256((__m256i *)op, v0);
                   _mm256_storeu_si256((__m256i *)(op+32/(USIZE/8)), sv);
    }
    start = (uint_t)T2(_mm256_extract_epi,USIZE)(sv, 256/USIZE-1);
      #elif (defined(__SSSE3__) || defined(__ARM_NEON)) && (USIZE == 16 || USIZE == 32)
    for(p = _p; p != &_p[VSIZE]; p+=32/(USIZE/8),op+=32/(USIZE/8)) {
      __m128i v0 = _mm_loadu_si128((__m128i *)p);
      __m128i v1 = _mm_loadu_si128((__m128i *)(p+16/(USIZE/8)));
              v0 = T2( mm_rbit_epi, USIZE)(v0);
              v0 = T2(_mm_srli_epi, USIZE)(v0,b);
              v0 = T2( mm_xord_epi, USIZE)(v0,sv);
              v1 = T2( mm_rbit_epi, USIZE)(v1);
              v1 = T2(_mm_srli_epi, USIZE)(v1,b);
              sv = T2( mm_xord_epi, USIZE)(v1,v0);
      _mm_storeu_si128((__m128i *) op,               v0);
      _mm_storeu_si128((__m128i *)(op+16/(USIZE/8)), sv);
    }
    start = (uint_t)T2(mm_cvtsi128_si,USIZE)(_mm_srli_si128(sv,16-USIZE/8));
      #else
    for(p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
      #endif
  }
  if((n = (out+n) - op) != 0) {
    uint_t b = *ip++;
    for(ip = T2(P4DEC,USIZE)(ip, n, _p), p = _p; p < &_p[n]; p++,op++) FD(0,USIZE);
  }
  return ip - in;
  #undef FD
}

//-------- TurboFloat FCM: Finite Context Method Predictor ---------------------------------------------------------------
#define HBITS 13 //15
#define HASH64(_h_,_u_) (((_h_)<<5 ^ (_u_)>>50) & ((1u<<HBITS)-1))
#define HASH32(_h_,_u_) (((_h_)<<4 ^ (_u_)>>23) & ((1u<<HBITS)-1))
#define HASH16(_h_,_u_) (((_h_)<<3 ^ (_u_)>>12) & ((1u<<HBITS)-1))
#define HASH8( _h_,_u_) (((_h_)<<2 ^ (_u_)>> 5) & ((1u<<HBITS)-1))

size_t T2(fpfcmenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        htab[1<<HBITS] = {0}, _p[VSIZE+64], *ip, *p;
  unsigned      h = 0, m;
  unsigned char *op = out, *out_ = out+n*(USIZE/8);

    #if defined(__AVX2__) && USIZE >= 32
  #define _mm256_set1_epi64(a) _mm256_set1_epi64x(a)
  __m256i sv = T2(_mm256_set1_epi, USIZE)(start);
    #elif (defined(__SSSE3__) || defined(__ARM_NEON)) && (USIZE == 16 || USIZE == 32)
  #define _mm_set1_epi64(a) _mm_set1_epi64x(a)
  __m128i sv = T2(_mm_set1_epi, USIZE)(start);
    #endif

  for(ip = in; ip != in + (n&~(VSIZE-1)); ) { 
    uint_t b = 0;
    #define FE(_i_,_usize_) { T3(uint, _usize_, _t) u = ip[_i_]; p[_i_] = XORENC(u, htab[h],_usize_); b |= p[_i_]; htab[h] = u; h = T2(HASH,_usize_)(h,u); }
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
    *op++ = b = T2(clz,USIZE)(b);
      #if defined(__AVX2__) && USIZE >= 32
    for(p = _p; p != &_p[VSIZE]; p+=64/(USIZE/8)) {
      __m256i v0 = _mm256_loadu_si256((__m256i *)p);
      __m256i v1 = _mm256_loadu_si256((__m256i *)(p+32/(USIZE/8)));
              v0 = T2(_mm256_slli_epi, USIZE)(v0,b);
              v1 = T2(_mm256_slli_epi, USIZE)(v1,b);
              v0 = T2( mm256_rbit_epi, USIZE)(v0);
              v1 = T2( mm256_rbit_epi, USIZE)(v1);
                   _mm256_storeu_si256((__m256i *) p, v0);
                   _mm256_storeu_si256((__m256i *)(p+32/(USIZE/8)), v1);
    }
      #elif (defined(__SSSE3__) || defined(__ARM_NEON)) && (USIZE == 16 || USIZE == 32)
    for(p = _p; p != &_p[VSIZE]; p+=32/(USIZE/8)) {
      __m128i v0 = _mm_loadu_si128((__m128i *) p);
      __m128i v1 = _mm_loadu_si128((__m128i *)(p+16/(USIZE/8)));
              v0 = T2(_mm_slli_epi, USIZE)(v0,b);
              v0 = T2( mm_rbit_epi, USIZE)(v0);
              v1 = T2(_mm_slli_epi, USIZE)(v1,b);
              v1 = T2( mm_rbit_epi, USIZE)(v1);
      _mm_storeu_si128((__m128i *) p,               v0);
      _mm_storeu_si128((__m128i *)(p+16/(USIZE/8)), v1);
    }
      #else
    #define TR(_i_,_usize_) p[_i_] = T2(rbit,_usize_)(p[_i_]<<b)
    for(p = _p; p != &_p[VSIZE]; p+=4) { TR(0,USIZE); TR(1,USIZE); TR(2,USIZE); TR(3,USIZE); }
      #endif
    op = T2(P4ENCV,USIZE)(_p, VSIZE, op);                                       PREFETCH(ip+512,0); if(op >= out_) goto e;
  }
  if((m = (in+n)-ip) != 0) { 
    uint_t b = 0;
    for(p = _p; p != &_p[m]; p++,ip++) FE(0,USIZE);
    b = b?T2(clz,USIZE)(b):USIZE;
    *op++ = b;
    for(p = _p; p != &_p[m]; p++) TR(0,USIZE);
    op = T2(P4ENC,USIZE)(_p, m, op);                                            if(op >= out_) goto e;
  }
  if(op >= out_) { e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op += n*(USIZE/8); }
  return op - out;
  #undef FE
}

size_t T2(fpfcmdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t        *op, _p[VSIZE+32], htab[1<<HBITS] = {0}, *p;
  unsigned      h = 0;
  unsigned char *ip = in;
  if(*in == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return 1+n*(USIZE/8); }

  #define FD(_i_,_usize_) { T3(uint, _usize_, _t) u = p[_i_]; u = T2(rbit,_usize_)(u)>>b;\
    u = XORDEC(u, htab[h], _usize_); op[_i_] = u; htab[h] = u; h = T2(HASH,_usize_)(h,u);\
  }
  for(op = (uint_t*)out; op != out+(n&~(VSIZE-1)); ) {                          PREFETCH(ip+512,0);
     unsigned b = *ip++; ip = T2(P4DECV,USIZE)(ip, VSIZE, _p);
    for(p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if((n = ((uint_t *)out+n) - op) != 0) {
    unsigned b = *ip++; ip = T2(P4DEC,USIZE)(ip, n, _p);
    for(p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  }
  return ip - in;
  #undef FD
}

//-------- TurboFloat DFCM: Differential Finite Context Method Predictor ----------------------------------------------------------
size_t T2(fpdfcmenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip, _p[VSIZE+64], htab[1<<HBITS] = {0}, *p;
  unsigned      h = 0, m;
  unsigned char *op = out, *out_ = out+n*(USIZE/8);

  #define FE(_i_,_usize_) { T3(uint, _usize_, _t) u = ip[_i_]; p[_i_] = XORENC(u, (htab[h]+start),_usize_); b |= p[_i_]; \
    htab[h] = start = u - start; h = T2(HASH,_usize_)(h,start); start = u;\
  }
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) { 
    uint_t b = 0;
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
    #define TR(_i_,_usize_) p[_i_] = T2(rbit,_usize_)(p[_i_]<<b)
    b = b?T2(clz,USIZE)(b):USIZE;
    for(p = _p; p != &_p[VSIZE]; p+=4) { TR(0,USIZE); TR(1,USIZE); TR(2,USIZE); TR(3,USIZE); }
    *op++ = b; 
	op = T2(P4ENCV,USIZE)(_p, VSIZE, op);                            PREFETCH(ip+512,0); if(op >= out_) goto e;
  }
  if((m = (in+n)-ip) != 0) { 
    uint_t b = 0;
    for(p = _p; p != &_p[m]; p++,ip++) FE(0,USIZE);
    b = b?T2(clz,USIZE)(b):USIZE;
    for(p = _p; p != &_p[m]; p++) TR(0,USIZE);
    *op++ = b; op = T2(P4ENC,USIZE)(_p, m, op);                                 if(op >= out_) goto e;
  }
  if(op >= out_) { e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op += n*(USIZE/8); }
  return op - out;
  #undef FE
}

size_t T2(fpdfcmdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t        _p[VSIZE+64], htab[1<<HBITS] = {0}, *op, *p;
  unsigned      h = 0;
  unsigned char *ip = in;
  if(*in == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return 1+n*(USIZE/8); }

  #define FD(_i_,_usize_) { T3(uint, _usize_, _t) u = T2(rbit,_usize_)(p[_i_])>>b; u = XORDEC(u, (htab[h]+start),_usize_); \
    op[_i_] = u; htab[h] = start = u-start; h = T2(HASH,_usize_)(h,start); start = u;\
  }
  for(op = (uint_t*)out; op != out+(n&~(VSIZE-1)); ) {                                          PREFETCH(ip+512,0);
    uint_t b = *ip++;
    ip = T2(P4DECV,USIZE)(ip, VSIZE, _p);
    for(p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if((n = ((uint_t *)out+n) - op) != 0) {
    uint_t b = *ip++;
    ip = T2(P4DEC,USIZE)(ip, n, _p);
    for(p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  }
  return ip - in;
  #undef FD
}

//-------- TurboFloat Double delta DFCM: Differential Finite Context Method Predictor ----------------------------------------------------------
size_t T2(fp2dfcmenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip, _p[VSIZE+32], htab[1<<HBITS] = {0}, *p,start0 = start; start = 0;
  unsigned      h = 0,m;
  unsigned char *op = out, *out_ = out+n*USIZE/8;

  #define FE(_i_,_usize_) { T3(uint, _usize_, _t) u = ip[_i_]; p[_i_] = XORENC(u, (htab[h]+start),_usize_); b |= p[_i_]; \
    htab[h] = start = u - start; h = T2(HASH,_usize_)(h,start); start = start0; start0 = u;\
  }
  #define TR(_i_,_usize_) p[_i_] = T2(rbit,_usize_)(p[_i_]<<b)

  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {
    uint_t b = 0;
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
    b = b?T2(clz,USIZE)(b):USIZE;

    for(p = _p; p != &_p[VSIZE]; p+=4) { TR(0,USIZE); TR(1,USIZE); TR(2,USIZE); TR(3,USIZE); }
    *op++ = b; op = T2(P4ENCV,USIZE)(_p, VSIZE, op);                            PREFETCH(ip+512,0); if(op >= out_) goto e;
  }
  if((m = (in+n)-ip) != 0) {
    uint_t b = 0;
    for(p = _p; p != &_p[m]; p++,ip++) FE(0,USIZE);
    b = T2(clz,USIZE)(b);

    for(p = _p; p != &_p[m]; p++) TR(0,USIZE);
    *op++ = b; op = T2(P4ENC,USIZE)(_p, m, op);                                 if(op >= out_) goto e;
  }
  if(op >= out_) { e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op+=n*(USIZE/8); }
  return op - out;
  #undef FE
}

size_t T2(fp2dfcmdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t        _p[VSIZE+32], htab[1<<HBITS] = {0}, *op, *p, start0 = start; start = 0;
  unsigned      h = 0;
  unsigned char *ip = in;
  if(*ip == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return n*(USIZE/8); }
  #define FD(_i_,_usize_) { T3(uint, _usize_, _t) u = T2(rbit,_usize_)(p[_i_])>>b; u = XORDEC(u, (htab[h]+start),_usize_);\
    op[_i_] = u; htab[h] = start = u-start; h = T2(HASH,_usize_)(h,start); start = start0; start0 = u;\
  }

  for(op = (uint_t*)out; op != out+(n&~(VSIZE-1)); ) {                      PREFETCH(ip+512,0);
    uint_t b = *ip++;
    ip = T2(P4DECV,USIZE)(ip, VSIZE, _p);
    for(p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if((n = ((uint_t *)out+n) - op) != 0) {
    uint_t b = *ip++;
    ip = T2(P4DEC,USIZE)(ip, n, _p);
    for(p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  }
  return ip - in;
  #undef FD
}

#define BSIZE(_usize_) (_usize_==64?6:(_usize_==32?5:(_usize_==16?4:3)))
//-------- TurboGorilla : Improved Gorilla style (see Facebook paper) Floating point compression with bitio ------------------------------------
#define bitput2(_bw_,_br_, _n1_, _n2_, _x_) {\
           if(!_x_)                  bitput(_bw_,_br_,      1,       1);/*1*/\
      else if( _x_ < (1<< (_n1_-1))) bitput(_bw_,_br_, _n1_+2,_x_<<2|2);/*10*/\
      else                           bitput(_bw_,_br_, _n2_+2,_x_<<2  );/*00*/\
}

#define bitget2(_bw_,_br_, _n1_, _n2_, _x_) { _x_ = bitbw(_bw_,_br_);\
       if(_x_ & 1) bitrmv(_bw_,_br_,   0+1), _x_ = 0;\
  else if(_x_ & 2) bitrmv(_bw_,_br_,_n1_+2), _x_ = BZHI32(_x_>>2, _n1_);\
  else             bitrmv(_bw_,_br_,_n2_+2), _x_ = BZHI32(_x_>>2, _n2_);\
}

size_t T2(fpgenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip = in;
  unsigned       ol = 0, ot = 0;
  unsigned char *op = out, *out_ = out+n*USIZE/8;
  bitdef(bw,br);
  if(start) { ol = T2(clz,USIZE)(start); ot = T2(ctz,USIZE)(start); }
  *op++ = 0;
  #define FE(_i_,_usize_) { T3(uint, _usize_, _t) z = XORENC(ip[_i_], start,_usize_); start = ip[_i_];\
    if(unlikely(!z))                         bitput( bw,br, 2, 2); /*01*/\
    else {\
	  unsigned t = T2(ctz,_usize_)(z), l = /*lz2l[*/T2(clz,_usize_)(z);\
      unsigned s = _usize_ - l - t, os = _usize_ - ol - ot;\
      if(l >= ol && t >= ot && os < 6+5+s) { bitput( bw,br, 1, 1); /*1*/                                                             T2(bitput,_usize_)(bw,br, os, z>>ot,op); }\
      else {                                                       /*00*/ /*bitput( bw,br, 2, 0); bitput(bw,br, 3, lz2c(l));*/\
	                                         bitput( bw,br, 2+BSIZE(_usize_), l<<2); bitput2(bw,br, N_0, N_1, t); bitenorm(bw,br,op);T2(bitput,_usize_)(bw,br,  s, z>>t,op); ol = l; ot = t; }\
    } bitenorm(bw,br,op); if(op >= out_) goto e;\
  }
  for(; ip != in + (n&~(4-1)); ip+=4) { PREFETCH(ip+512,0); FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
  for(; ip != in +  n        ; ip++) FE(0,USIZE);
  bitflush(bw,br,op);
  if(op >= out_) {
    e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op+=n*(USIZE/8);
  }
  return op - out;
  #undef FE
}

size_t T2(fpgdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) { if(!n) return 0;
  uint_t        *op;
  int       ol = 0,ot = 0,x;
  unsigned char *ip = in;
  bitdef(bw,br);
  if(*ip++ == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return n*(USIZE/8); }
  if(start) { ol = T2(clz,USIZE)(start); ot = T2(ctz,USIZE)(start); }

  #define FD(_i_,_usize_) { T3(uint, _usize_, _t) z=0; unsigned _x,_y; BITGET32(bw,br,1,_x); \
    if(likely(!_x)) { BITGET32(bw,br,1,_y); _x = _y << 1 | _x; }\
	if(_x < 2) { \
      if(!_x) { /*BITGET32(bw,br, 3, ol); ol = c2l(ol);*/BITGET32(bw,br,BSIZE(_usize_),ol); bitget2(bw,br, N_0, N_1, ot); bitdnorm(bw,br,ip); }\
      T2(bitget,_usize_)(bw,br,_usize_ - ol - ot,z,ip);\
      z<<=ot;\
    }  op[_i_] = start = XORDEC(z, start,_usize_); bitdnorm(bw,br,ip);\
  }
  for(bitdnorm(bw,br,ip),op = out; op != out+(n&~(4-1)); op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); PREFETCH(ip+512,0); }
  for(        ; op != out+n; op++) FD(0,USIZE);
  bitalign(bw,br,ip);
  return ip - in;
  #undef FD
}

//--- Chimp: Efficient Lossless Floating Point Compression for Time Series Databases - https://www.vldb.org/pvldb/vol15/p3058-liakos.pdf
size_t T2(fphenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip = in;
  int            ol = USIZE+1,s;
  unsigned char *op = out, *out_ = out+n*USIZE/8;
  *op++ = 0;
  bitdef(bw,br);
  if(start) ol = T2(clz,USIZE)(start); 
  #define FE(_i_,_usize_) { T3(uint, _usize_, _t) z = XORENC(ip[_i_], start,_usize_); start = ip[_i_];\
    if(unlikely(!z)) { ol = _usize_+1;                              bitput(bw,br, 2, 0); }\
    else { int t = T2(ctz,_usize_)(z), l = lz2l(T2(clz,_usize_)(z));\
	  if(t > BSIZE(_usize_)) { s = _usize_ - l - t; ol = _usize_+1; bitput(bw,br, 2, 1); bitput(bw,br, CCODEL, l2c(l)); bitput(bw,br, BSIZE(_usize_), s); z>>=t; \
		bitenorm(bw,br,op); T2(bitput,_usize_)(bw,br, s, z, op); \
	  } else if (l == ol) { s = _usize_ - l;	                    bitput(bw,br, 2, 2);\
	    bitenorm(bw,br,op); T2(bitput,_usize_)(bw,br, s, z, op);\
	  } else              { s = _usize_ - l;     ol = l;            bitput(bw,br, 2, 3); bitput(bw,br, CCODEL, l2c(l));\
	    bitenorm(bw,br,op); T2(bitput,_usize_)(bw,br, s, z, op); }\
    } bitenorm(bw,br,op); if(op >= out_) goto e;\
  }
  for(; ip != in + (n&~(4-1)); ip+=4) { PREFETCH(ip+512,0); FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
  for(; ip != in +  n        ; ip++) FE(0,USIZE);
  bitflush(bw,br,op);
  if(op >= out_) {
    e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op+=n*(USIZE/8);
  }
  return op - out;
  #undef FE
}

size_t T2(fphdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) { if(!n) return 0;
  uint_t        *op = out;
  int       ol = 0,s;
  unsigned char *ip = in; 
  bitdef(bw,br); 
  if(*ip++ == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return n*(USIZE/8); }
  if(start) ol = T2(clz,USIZE)(start);

  #define FD(_i_,_usize_) { T3(uint, _usize_, _t) z=0; unsigned _x; bitdnorm(bw,br,ip); BITGET32(bw,br,2,_x);\
    switch(_x) {\
      case 3: BITGET32(bw,br, CCODEL, ol); ol = c2l(ol); bitdnorm(bw,br,ip); T2(bitget,_usize_)(bw,br,(_usize_-ol), z, ip); s = _usize_ == ol?_usize_:_usize_ - ol;\
        break;\
      case 2: bitdnorm(bw,br,ip); T2(bitget,_usize_)(bw,br,(_usize_-ol), z, ip);                                            s = _usize_ == ol?_usize_:_usize_ - ol;\
        break;\
      case 1: BITGET32(bw,br, CCODEL, ol); ol = c2l(ol); BITGET32(bw,br,BSIZE(_usize_), s);                                 s = s?s:_usize_;\
        int ot = _usize_ - s - ol;\
        bitdnorm(bw,br,ip); T2(bitget,_usize_)(bw,br, (_usize_ - ol - ot), z, ip); z <<= ot;\
        break;\
	  case 0: ;\
    } op[_i_] = start = XORDEC(z, start,_usize_);\
  }
  bitdnorm(bw,br,ip);
  for(; op != out+(n&~(4-1)); op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); PREFETCH(ip+512,0); }
  for(; op != out+n; op++) FD(0,USIZE);
  bitalign(bw,br,ip);
  return ip - in;
  #undef FD
}

//-- TurboFloatLz (lz77 like parsing) --------------------------------------
  #if USIZE == 64
#define VA_BITS          7 // window size for 64 bits (integer/double)
#define FX_BITS         14
  #elif USIZE == 32
#define VA_BITS          4 // 32 bits (intger/float)
#define FX_BITS         12
  #else
#define VA_BITS          4 // 16 bits (intger/float)
#define FX_BITS         10
  #endif

#define CTZMIN(_usize_) (VA_BITS+BSIZE(_usize_))
#define HASH(_x_)       (_x_ & ((1<<FX_BITS)-1))  

#define HASH16(_h_,_u_) HASH(_u_)
#define HASH32(_h_,_u_) HASH(_u_) //(((_h_) ^ (_u_)>>23) & ((1<<FX_BITS)-1))
#define HASH64(_h_,_u_) HASH(_u_) //(((_h_) ^ (_u_)>>50) & ((1<<FX_BITS)-1))

size_t T2(fpc0enc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) { // simple hash table
  uint_t        *ip = in;
  unsigned char *op = out, *out_ = out+n*USIZE/8;
  unsigned      htab[1<<FX_BITS] = {0}, ol = USIZE+1; 
  bitdef(bw,br);    							//if(start) ol = T2(clz,USIZE)(start);
  
  for(*op++ = 0,*(uint_t *)op = *ip++, op += sizeof(uint_t); ip != in + n; ip++) {
	uint_t   u = *ip, z;
	unsigned hu = T2(HASH,USIZE)(u,u), h = htab[hu], ofs = (ip-in) - h - 1, c = 0, ctz; 
	                           htab[hu] = ip - in;
    if(ofs < (1<<VA_BITS)) {
	  ctz = T2(ctz,USIZE)(z = u ^ in[h]);
      ofs = (c = ctz > CTZMIN(USIZE))?ofs:0;
    }
	z = c?z:u ^ ip[-1];
    if(z) {
      unsigned l = lz2l(T2(clz,USIZE)(z)), t;
      if(c) {
        t = USIZE - l - ctz;
		bitput(bw,br, (VA_BITS+CCODEL+BSIZE(USIZE)+2), ofs << (CCODEL+BSIZE(USIZE)+2) | (unsigned)l2c(l) << (BSIZE(USIZE)+2) | t<<2);
		bitenorm(bw,br,op); T2(bitput,USIZE)(bw,br, t, z >> ctz, op);
        ol = USIZE+1;
      } else {
        if(l == ol) {        bitput(bw,br,        2,           1);               }
		else {               bitput(bw,br, CCODEL+2, l2c(l)<<2|2); ol = l;       }
        bitenorm(bw,br,op); T2(bitput,USIZE)(bw,br, USIZE - l, z, op);         
      }
    } else {                 bitput(bw,br,VA_BITS+2,    ofs<<2|3  ); ol = USIZE+1; }
	bitenorm(bw,br,op); if(op >= out_) goto e;									       
  }
  bitflush(bw,br,op);
  if(op >= out_) {
    e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op+=n*(USIZE/8);
  }
  return op - out;
}

size_t T2(fpcenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) { // double entry hashtable
  uint_t        *ip = in;
  unsigned char *op = out, *out_ = out+n*(USIZE/8);
  unsigned      htab[1<<(FX_BITS+1)] = {0}, ol = USIZE+1; 
  bitdef(bw,br);                                                  //if(start) ol = T2(clz,USIZE)(start);
  
  for(*op++ = 0,*(uint_t *)op = *ip++, op += sizeof(uint_t); ip != in + n; ip++) {
	uint_t   u = *ip, z;
	unsigned hu = T2(HASH,USIZE)(u,u)<<1, h = htab[hu], ofs = (ip-in) - h - 1, c = 0, ctz = 0;
	htab[hu+1] = h, htab[hu] = ip - in;
    if(ofs < (1<<VA_BITS)) { 
	  ctz = T2(ctz,USIZE)(z = u ^ in[h]);	  
	  unsigned h1 = htab[hu+1], ofs1 = (ip-in) - h1 - 1;	  
      if(ofs1 < (1<<VA_BITS)) {
	    uint_t   z1;
	    unsigned ctz1 = T2(ctz,USIZE)(z1 = u ^ in[h1]), c1 = 0; 
	    if(ctz1 > ctz) { ofs = ofs1; c = c1;  z = z1;  ctz = ctz1; }
      }	
      ofs = (c = ctz > CTZMIN(USIZE))?ofs:0;
    }
	z = c?z:u ^ ip[-1];
    if(z) {
      unsigned l = lz2l(T2(clz,USIZE)(z)), t;	  
      if(c) {
        t = USIZE - l - ctz;
		bitput(bw,br, (VA_BITS+CCODEL+BSIZE(USIZE)+2), ofs << (CCODEL+BSIZE(USIZE)+2) | (unsigned)l2c(l) << (BSIZE(USIZE)+2) | t<<2);
		bitenorm(bw,br,op); T2(bitput,USIZE)(bw,br, t, z >> ctz, op);  							
        ol = USIZE+1;
      } else {
        if(l == ol) {        bitput(bw,br,        2,           1);               }
		else {               bitput(bw,br, CCODEL+2, l2c(l)<<2|2); ol = l;       }
        bitenorm(bw,br,op); T2(bitput,USIZE)(bw,br, USIZE - l, z, op);         
      }
    } else {                 bitput(bw,br,VA_BITS+2,    ofs<<2|3 ); ol = USIZE+1; }
	bitenorm(bw,br,op); if(op >= out_) goto e;										       
  }
  bitflush(bw,br,op);
  if(op >= out_) {
    e:op = out; *op++ = 0xff; memcpy(op, in, n*(USIZE/8)); op+=n*(USIZE/8);
  }
  return op - out;
}

size_t T2(fpcdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) { if(!n) return 0;
  uint_t        *op = out, v = 0, z;
  unsigned char *ip = in; 
  unsigned      ol = USIZE+1, t;
  bitdef(bw,br); 									//if(start) ol = T2(clz,USIZE)(start);  
  if(*ip++ == 0xff) { memcpy(out, in+1, n*(USIZE/8)); return n*(USIZE/8); }

  for(v = *op++ = *(uint_t *)ip, ip += sizeof(uint_t); op != out+n; op++) {
    bitdnorm(bw,br,ip);	BITGET32(bw,br, 2, t); 
	switch(t) {
	  case 0: 	
	    BITGET32(bw,br, VA_BITS+CCODEL+BSIZE(USIZE), t);
        ol = c2l(bextr32(t, BSIZE(USIZE), CCODEL));        
		v = *(op - bextr32(t, BSIZE(USIZE)+CCODEL, VA_BITS)-1);
        t &= ((1<<BSIZE(USIZE))-1);        							
        t  = t?t:USIZE;
        bitdnorm(bw,br,ip); T2(bitget,USIZE)(bw,br, t, z, ip);
        v ^= z << (USIZE - t - ol);
        break;
      case 1: bitdnorm(bw,br,ip); T2(bitget,USIZE)(bw,br, USIZE - ol, z, ip); v ^= z; break; 
      case 2: BITGET32(bw,br, CCODEL, ol); ol = c2l(ol); bitdnorm(bw,br,ip); T2(bitget,USIZE)(bw,br, USIZE - ol, z, ip); v ^= z; break;
	  case 3: BITGET32(bw,br,VA_BITS,t); v = *(op-t-1); break;
    }
	*op = v;	
  }
  bitalign(bw,br,ip);
  return ip - in;
}

//------ Zigzag of zigzag for timestamps with bitio ------------------------------------------------------------------------------------------
// Improved Gorilla style compression with sliding zigzag of delta + RLE + overflow handling for timestamps in time series.
// More than 300 times better compression and several times faster
#define OVERFLOW if(op >= out_) { *out++ = 1<<4; /*bitini(bw,br); bitput(bw,br,4+3,1<<4); bitflush(bw,br,out);*/ memcpy(out,in,n*sizeof(in[0])); return 1+n*sizeof(in[0]); }

size_t T2(bvzzenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip = in, pd = 0, *pp = in,dd;
  unsigned char *op = out, *out_ = out+n*sizeof(in[0]);

  bitdef(bw,br);
  #define FE(_pp_, _ip_, _d_, _op_,_usize_) do {\
    uint64_t _r = _ip_ - _pp_;\
    if(_r > NL) { _r -= NL; unsigned _b = (bsr64(_r)+7)>>3; bitput(bw,br,4+3+3,(_b-1)<<(4+3)); bitput64(bw,br,_b<<3, _r, _op_); bitenorm(bw,br,_op_); }\
    else while(_r--) { bitput(bw,br,1,1); bitenorm(bw,br,_op_); }\
    _d_ = T2(zigzagenc,_usize_)(_d_);\
         if(!_d_)                bitput(bw,br,    1,       1);\
    else if(_d_ <  (1<< (N2-1))) bitput(bw,br, N2+2,_d_<<2|2);\
    else if(_d_ <  (1<< (N3-1))) bitput(bw,br, N3+3,_d_<<3|4);\
    else if(_d_ <  (1<< (N4-1))) bitput(bw,br, N4+4,_d_<<4|8);\
    else { unsigned _b = (T2(bsr,_usize_)(_d_)+7)>>3; bitput(bw,br,4+3,(_b-1)<<4); T2(bitput,_usize_)(bw,br, _b<<3, _d_,_op_); }\
    bitenorm(bw,br,_op_);\
  } while(0)

  if(n > 4)
    for(; ip < in+(n-1-4);) {
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;     PREFETCH(ip+256,0);
      continue;
      a:;
      FE(pp,ip, dd, op,USIZE);
      pp = ++ip;        OVERFLOW;
    }

  for(;ip < in+n;) {
    start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto b; ip++;
    continue;
    b:;
    FE(pp,ip, dd, op,USIZE);
    pp = ++ip; OVERFLOW;
  }
  if(ip > pp) {
    start = ip[0] - start; dd = start-pd;
    FE(pp, ip, dd, op, USIZE); OVERFLOW;
  }
  bitflush(bw,br,op);
  return op - out;
  #undef FE
}

size_t T2(bvzzdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) { if(!n) return 0;
  uint_t *op = out, pd = 0;
  unsigned char *ip = in;

  bitdef(bw,br);
  for(bitdnorm(bw,br,ip); op < out+n; ) {                                                           PREFETCH(ip+384,0);
     #if USIZE == 64
    uint_t dd = bitbw(bw,br);
     #else
    uint32_t dd = bitbw(bw,br);
     #endif
         if(dd & 1) bitrmv(bw,br, 0+1), dd = 0;
    else if(dd & 2) bitrmv(bw,br,N2+2), dd = BZHI32(dd>>2, N2);
    else if(dd & 4) bitrmv(bw,br,N3+3), dd = BZHI32(dd>>3, N3);
    else if(dd & 8) bitrmv(bw,br,N4+4), dd = BZHI32(dd>>4, N4);
    else {
      unsigned b; uint_t *_op; uint64_t r;
      BITGET32(bw,br, 4+3, b);
      if((b>>=4) <= 1) {
        if(b==1) {                                                           // No compression, because of overflow
          memcpy(out,in+1, n*sizeof(out[0]));
          return 1+n*sizeof(out[0]);
        }
        BITGET32(bw,br,3,b); bitget32(bw,br,(b+1)<<3,r,ip); bitdnorm(bw,br,ip);//RLE     //r+=NL; while(r--) *op++=(start+=pd);
          #if (defined(__SSE2__) /*|| defined(__ARM_NEON)*/) && USIZE == 32
        __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4*pd,3*pd,2*pd,1*pd);
        for(r += NL, _op = op; op != _op+(r&~7);) {
          sv = _mm_add_epi32(sv,cv); _mm_storeu_si128((__m128i *)op, sv); sv = mm_shuffle_nnnn_epi32(sv, 3); op += 4; //_mm_shuffle_epi32(sv, _MM_SHUFFLE(3, 3, 3, 3))->mm_shuffle_nnnn_epi32(sv, 3)
          sv = _mm_add_epi32(sv,cv); _mm_storeu_si128((__m128i *)op, sv); sv = mm_shuffle_nnnn_epi32(sv, 3); op += 4;
        }
        start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
          #else
        for(r+=NL, _op = op; op != _op+(r&~7); op += 8)
          op[0]=(start+=pd),
          op[1]=(start+=pd),
          op[2]=(start+=pd),
          op[3]=(start+=pd),
          op[4]=(start+=pd),
          op[5]=(start+=pd),
          op[6]=(start+=pd),
          op[7]=(start+=pd);
          #endif
        for(; op != _op+r; op++)
          *op = (start+=pd);
        continue;
      }
      T2(bitget,USIZE)(bw,br,(b+1)<<3,dd,ip);
    }
    pd += T2(zigzagdec,USIZE)(dd);
    *op++ = (start += pd);
    bitdnorm(bw,br,ip);
  }
  bitalign(bw,br,ip);
  return ip - in;
}

//-------- Zigzag with bit/io + RLE --------------------------------------------------------------------------
size_t T2(bvzenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip = in, *pp = in,dd;
  unsigned char *op = out, *out_ = out+n*sizeof(in[0]);

  bitdef(bw,br);
  #define FE(_pp_, _ip_, _d_, _op_,_usize_) do {\
    uint64_t _r = _ip_ - _pp_;\
    if(_r > NL) { _r -= NL; unsigned _b = (bsr64(_r)+7)>>3; bitput(bw,br,4+3+3,(_b-1)<<(4+3)); bitput64(bw,br,_b<<3, _r, _op_); bitenorm(bw,br,_op_); }\
    else while(_r--) { bitput(bw,br,1,1); bitenorm(bw,br,_op_); }\
    _d_ = T2(zigzagenc,_usize_)(_d_);\
         if(!_d_)                bitput(bw,br,    1,       1);\
    else if(_d_ <  (1<< (N2-1))) bitput(bw,br, N2+2,_d_<<2|2);\
    else if(_d_ <  (1<< (N3-1))) bitput(bw,br, N3+3,_d_<<3|4);\
    else if(_d_ <  (1<< (N4-1))) bitput(bw,br, N4+4,_d_<<4|8);\
    else { unsigned _b = (T2(bsr,_usize_)(_d_)+7)>>3; bitput(bw,br,4+3,(_b-1)<<4); T2(bitput,_usize_)(bw,br, _b<<3, _d_,_op_); }\
    bitenorm(bw,br,_op_);\
  } while(0)

  if(n > 4)
    for(; ip < in+(n-1-4);) {
      dd = ip[0] - start; start = ip[0]; if(dd) goto a; ip++;
      dd = ip[0] - start; start = ip[0]; if(dd) goto a; ip++;
      dd = ip[0] - start; start = ip[0]; if(dd) goto a; ip++;
      dd = ip[0] - start; start = ip[0]; if(dd) goto a; ip++;   PREFETCH(ip+256,0);
      continue;
      a:;
      FE(pp,ip, dd, op,USIZE);
      pp = ++ip;        OVERFLOW;
    }

  for(;ip < in+n;) {
      dd = ip[0] - start; start = ip[0]; if(dd) goto b; ip++;
    continue;
    b:;
    FE(pp,ip, dd, op,USIZE);
    pp = ++ip; OVERFLOW;
  }
  if(ip > pp) {
    dd = ip[0] - start; start = ip[0];
    FE(pp, ip, dd, op, USIZE); OVERFLOW;
  }
  bitflush(bw,br,op);
  return op - out;
  #undef FE
}

size_t T2(bvzdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) { if(!n) return 0;
  uint_t *op = out;
  unsigned char *ip = in;

  bitdef(bw,br);
  for(bitdnorm(bw,br,ip); op < out+n; ) {                                                           PREFETCH(ip+384,0);
     #if USIZE == 64
    uint_t dd = bitbw(bw,br);
     #else
    uint32_t dd = bitbw(bw,br);
     #endif
         if(dd & 1) bitrmv(bw,br, 0+1), dd = 0;
    else if(dd & 2) bitrmv(bw,br,N2+2), dd = BZHI32(dd>>2, N2);
    else if(dd & 4) bitrmv(bw,br,N3+3), dd = BZHI32(dd>>3, N3);
    else if(dd & 8) bitrmv(bw,br,N4+4), dd = BZHI32(dd>>4, N4);
    else {
      unsigned b; uint_t *_op; uint64_t r;
      BITGET32(bw,br, 4+3, b);
      if((b>>=4) <= 1) {
        if(b==1) {                                                           // No compression, because of overflow
          memcpy(out,in+1, n*sizeof(out[0]));
          return 1+n*sizeof(out[0]);
        }
        BITGET32(bw,br,3,b); bitget32(bw,br,(b+1)<<3,r,ip); bitdnorm(bw,br,ip);//RLE     //r+=NL; while(r--) *op++=(start+=pd);
          #if (defined(__SSE2__) || defined(__ARM_NEON)) && USIZE == 32
        __m128i sv = _mm_set1_epi32(start);
        for(r += NL, _op = op; op != _op+(r&~7);) {
          _mm_storeu_si128((__m128i *)op, sv); op += 4;
          _mm_storeu_si128((__m128i *)op, sv); op += 4;
        }
          #else
        for(r+=NL, _op = op; op != _op+(r&~7); op += 8)
          op[0]=op[1]=op[2]=op[3]=op[4]=op[5]=op[6]=op[7]=start;
          #endif
        for(; op != _op+r; op++)
          *op = start;
        continue;
      }
      T2(bitget,USIZE)(bw,br,(b+1)<<3,dd,ip);
    }
    dd = T2(zigzagdec,USIZE)(dd);
    *op++ = (start += dd);
    bitdnorm(bw,br,ip);
  }
  bitalign(bw,br,ip);
  return ip - in;
}

#undef USIZE
  #endif
