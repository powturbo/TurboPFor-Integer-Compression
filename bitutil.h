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
//     bitutil.h - "Integer Compression" 
#include <stdint.h>

#define _BITFORZERO(out, n, start, inc) do {\
 for(i = 0; i != (n&~3); ) {\
   out[i] = start+i*inc; i++;\
   out[i] = start+i*inc; i++;\
   out[i] = start+i*inc; i++;\
   out[i] = start+i*inc; i++;\
 }\
 while(i < n) out[i] = start+i*inc,++i;\
} while(0)

#define BITSIZE(__in, __n, __b, __usize) { typeof(__in[0]) *_ip;\
  for(__b=0,_ip = __in; _ip != __in+(__n&~(4-1)); )\
    __b |= *_ip++ | *_ip++ | *_ip++ | *_ip++;\
  while(_ip != __in+__n) __b |= *_ip++;\
  __b = TEMPLATE(bsr, __usize)(__b);\
}

static inline unsigned zigzagenc32(int      x) { return x << 1 ^ x >> 31; }
static inline unsigned zigzagdec32(unsigned x) { return x >> 1 ^ -(x & 1); }

  #ifdef __SSE2__
#include <emmintrin.h>

#define DELTA128_32(__v, __sv) _mm_sub_epi32(__v, _mm_or_si128(_mm_srli_si128(__sv, 12), _mm_slli_si128(__v, 4)))

#define SCAN128_32( __v, __sv) __v = _mm_add_epi32(__v, _mm_slli_si128(__v, 4)); __sv = _mm_add_epi32(_mm_shuffle_epi32(__sv, _MM_SHUFFLE(3, 3, 3, 3)), _mm_add_epi32(_mm_slli_si128(__v, 8), __v) )
#define SCANI128_32(__v, __sv, __vi) SCAN128_32(__v, __sv); __sv = _mm_add_epi32(__sv, __vi)

#define   ZIGZAG128_32(__v) _mm_xor_si128(_mm_slli_epi32(__v,1), _mm_srai_epi32(__v,31))
#define UNZIGZAG128_32(__v) _mm_xor_si128(_mm_srli_epi32(__v,1), _mm_srai_epi32(_mm_slli_epi32(__v,31),31) ) //_mm_sub_epi32(cz, _mm_and_si128(iv,c1))
// SIMD Horizontal OR
#define HOR128_32(__v,__b) __v = _mm_or_si128(__v, _mm_srli_si128(__v, 8)); __v = _mm_or_si128(__v, _mm_srli_si128(__v, 4)); __b = (unsigned)_mm_cvtsi128_si32(__v)

#define BITSIZE32(__in, __n, __b) { typeof(__in[0]) *_ip; __m128i v = _mm_setzero_si128();\
  for(_ip = __in; _ip != __in+(__n&~(4-1)); _ip+=4) v = _mm_or_si128(v, _mm_loadu_si128((__m128i*)_ip));\
  HOR128_32(v,__b);\
  while(_ip != __in+__n) __b |= *_ip++;\
  __b = bsr32(__b);\
}

#define BITZERO32(out, n, start) do {\
  __m128i sv = _mm_set1_epi32(start), *ov = (__m128i *)(out), *ove = (__m128i *)(out + n);\
  do { _mm_storeu_si128(ov++, sv); } while(ov < ove); \
} while(0)

#define BITFORZERO32(out, n, start, inc) do {\
  __m128i sv = _mm_set1_epi32(start), *ov=(__m128i *)(out), *ove = (__m128i *)(out + n), cv = _mm_set_epi32(3*inc,2*inc,1*inc,0); \
    sv = _mm_add_epi32(sv, cv);\
    cv = _mm_set1_epi32(4);\
  do { _mm_storeu_si128(ov++, sv); sv = _mm_add_epi32(sv, cv); } while(ov < ove);\
} while(0)

#define BITDIZERO32(out, n, start, inc) do { __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(3+inc,2+inc,1+inc,inc), *ov=(__m128i *)(out), *ove = (__m128i *)(out + n);\
  sv = _mm_add_epi32(sv, cv); cv = _mm_set1_epi32(4*inc); do { _mm_storeu_si128(ov++, sv), sv = _mm_add_epi32(sv, cv); } while(ov < ove);\
} while(0)

  #else																					
#define BITSIZE32(__in, __n, __b)        BITSIZE(__in, __n, __b, 32)
#define BITFORZERO32(out, n, start, inc) _BITFORZERO(out, n, start, inc)
#define BITZERO32(out, n, start)         _BITFORZERO(out, n, start, 0)
  #endif


#define DELTR( __in, __n, __mode,      __out) { unsigned _v; for(      __out[0]=__in[0],_v = 1;     _v < __n; _v++) __out[_v] = (__in[_v] - __out[0]) -   _v*__mode; }
#define DELTRB(__in, __n, __mode, __b, __out) { unsigned _v; for(__b=0,__out[0]=__in[0],_v = 1;     _v < __n; _v++) __out[_v] = (__in[_v] - __out[0]) -   _v*__mode, __b |= __out[_v]; __b = bsr32(__b); }

#ifdef __cplusplus
extern "C" {
#endif

// get maximum bit length of the elements in the integer array
unsigned bit32(     unsigned *in, unsigned n);  

// transform sorted integer array to delta array. inc = increment
unsigned bitdelta32(unsigned *in, unsigned n, unsigned *out, unsigned start, unsigned inc);
unsigned bitdelta64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, unsigned inc);

// get delta maximum bit length of the non decreasing integer array
unsigned bitd32(    unsigned *in, unsigned n, unsigned start);  

// get delta maximum bit length of the non strictly decreasing integer array
unsigned bitd132(   unsigned *in, unsigned n, unsigned start);

void bitund32(      unsigned *p, unsigned n, unsigned x);
void bitund64(      uint64_t *p, unsigned n, uint64_t x);

void bitundx32(     unsigned *p, unsigned n, unsigned x, unsigned inc);
void bitundx64(     uint64_t *p, unsigned n, uint64_t x, unsigned inc);

void bitund132(     unsigned *p, unsigned n, unsigned x);

// for
unsigned bitf32(    unsigned *in, unsigned n, unsigned start);  // sorted
unsigned bitf132(   unsigned *in, unsigned n, unsigned start);
unsigned bitfm32(   unsigned *in, unsigned n, unsigned *pmin);  // unsorted
unsigned bitf1m32(  unsigned *in, unsigned n, unsigned *pmin);

// zigzag encoding for unsorted integer lists
unsigned bitz32(     unsigned *in, unsigned n, unsigned start);
unsigned bitzigzag32(unsigned *in, unsigned n, unsigned *out, unsigned start);
unsigned bitzigzag64(unsigned *in, unsigned n, unsigned *out, unsigned start);
void bitunzigzag32(  unsigned *p,  unsigned n, unsigned start);
void bitunzigzag64(  unsigned *p,  unsigned n, unsigned start);

//---- Floating point to Integer de-/composition ---------------------------------

#define FMANT_BITS    23
#define DMANT_BITS    52

#define BITFLOAT(__u, __sgn, __expo, __mant,      __mantbits, __one) __sgn = __u >> (sizeof(__u)*8-1); __expo = ((__u >> (__mantbits)) & ( (__one<<(sizeof(__u)*8 - 1 - __mantbits)) -1)); __mant = __u & ((__one<<__mantbits)-1);
#define BITUNFLOAT(   __sgn, __expo, __mant, __u, __mantbits)        __u = (__sgn) << (sizeof(__u)*8-1) | (__expo) << __mantbits | (__mant) 

// De-/Compose floating point array to/from integer arrays (sign,exponent,mantissa) for using with "Integer Compression" functions ------------
void bitdouble(  double *in, unsigned n, unsigned *sgn, unsigned *expo, uint64_t *mant);
void bitundouble(                        unsigned *sgn, unsigned *expo, uint64_t *mant, unsigned n, double *out);
void bitfloat(   float *in,  unsigned n, unsigned *sgn, unsigned *expo, unsigned *mant);
void bitunfloat(                         unsigned *sgn, unsigned *expo, unsigned *mant, unsigned n, float *out);

#ifdef __cplusplus
}
#endif
