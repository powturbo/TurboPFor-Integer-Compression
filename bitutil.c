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
//   bitutil.h - "Integer Compression" 
#include "conf.h" 
#include "bitutil.h"

#define BITDELTA(__p,__n, __inc, __start, __act) {\
  typeof(__p[0]) _x, *_p;\
  for(_p = __p; _p != __p+(__n&~(4-1)); ) {\
	_x = (*_p)-__start-__inc; __start = *_p++; __act;\
	_x = (*_p)-__start-__inc; __start = *_p++; __act;\
	_x = (*_p)-__start-__inc; __start = *_p++; __act;\
	_x = (*_p)-__start-__inc; __start = *_p++; __act;\
  }\
  while(_p < __p+__n) { \
    _x = *_p-__start-__inc; __start = *_p++; __act;\
  }\
}

#define BITUNDELTA(__p, __n, __start, __inc) { typeof(__p[0]) *_p;\
  for(_p = __p; _p != __p+(__n&~(4-1)); ) {\
    *_p = (__start += (*_p) + __inc); _p++;\
    *_p = (__start += (*_p) + __inc); _p++;\
    *_p = (__start += (*_p) + __inc); _p++;\
    *_p = (__start += (*_p) + __inc); _p++;\
  }\
  while(_p < __p+__n) { *_p = (__start += (*_p) + __inc); _p++; }\
}

#define BITMINMAX(__p,__n, __mi, __mx) {\
  typeof(__p[0]) _x, *_p;\
  for(_p = __p, __mi = __mx = 0; _p != __p+(__n&~(4-1)); ) {\
	if(*_p < __mi) __mi = *_p; if(*_p > __mx) __mx = *_p; _p++; \
	if(*_p < __mi) __mi = *_p; if(*_p > __mx) __mx = *_p; _p++; \
	if(*_p < __mi) __mi = *_p; if(*_p > __mx) __mx = *_p; _p++; \
	if(*_p < __mi) __mi = *_p; if(*_p > __mx) __mx = *_p; _p++; \
  }\
  while(_p < __p+__n) { \
	if(*_p < __mi) __mi = *_p; if(*_p > __mx) __mx = *_p; _p++; \
  }\
}

unsigned bitdelta32(unsigned *in, unsigned n, unsigned *out, unsigned start, unsigned inc) {
    #ifdef __SSE2__
  unsigned *ip,b,*op = out; 
  __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(inc), dv;
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, dv = _mm_sub_epi32(DELTA128_32(iv,sv),cv)); 
	sv = iv; 
	_mm_storeu_si128((__m128i *)op, dv); 
	op += 4; 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128_32(bv, b);
  while(ip < in+n) { unsigned x = *ip-start-inc; start = *ip++; b |= x; *op++ = x; }
    #else
  typeof(in[0]) b = 0,*op = out; BITDELTA(in, n, inc, start, b |= _x;*op++ = _x);
    #endif
  return bsr32(b);
}

unsigned bitdelta64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, unsigned inc) {
  typeof(in[0]) b = 0,*op = out; BITDELTA(in, n, inc, start, b |= _x; *op++ = _x);
  return bsr64(b);
}

unsigned bit32(unsigned *in, unsigned n) {
  typeof(in[0]) b; BITSIZE32(in, n, b);
  return b; 
}

unsigned bitf32(  unsigned *in, unsigned n, unsigned start) { return n?bsr32(in[n-1] - start    ):0; }
unsigned bitf132( unsigned *in, unsigned n, unsigned start) { return n?bsr32(in[n-1] - start - n):0; }

unsigned bitfm32(unsigned *in, unsigned n, unsigned *pmin) {
  unsigned mi,mx; 
  BITMINMAX(in, n, mi, mx);
  *pmin = mi;
  return bsr32(mx - mi);
}

unsigned bitf1m32(unsigned *in, unsigned n, unsigned *pmin) {
  unsigned mi,mx; 
  BITMINMAX(in, n, mi, mx);
  *pmin = mi;
  return bsr32(mx - mi);
}

unsigned bitd32(unsigned *in, unsigned n, unsigned start) {
    #ifdef __SSE2__
  unsigned *ip,b; __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start);
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, DELTA128_32(iv,sv)); 
	sv = iv; 
  }
  
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128_32(bv, b);
  while(ip < in+n) { 
    unsigned x = *ip-start; 
	start = *ip++; 
	b |= x; 
  }
    #else
  typeof(in[0]) b = 0; BITDELTA(in,n, 0, start, b |= _x);
    #endif
  return bsr32(b); 
}

unsigned bitd132(unsigned *in, unsigned n, unsigned start) {
    #ifdef __SSE2__
  unsigned *ip,b; __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, _mm_sub_epi32(DELTA128_32(iv,sv),cv)); 
	sv = iv; 
  }
  
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128_32(bv, b);
  while(ip < in+n) { 
    unsigned x = *ip-start-1; 
	start = *ip++; 
	b |= x; 
  }
    #else
  typeof(in[0]) b = 0; BITDELTA(in, n, 1, start, b |= _x);
	#endif
  return bsr32(b); 
}

void bitund32( unsigned *p, unsigned n, unsigned x) { BITUNDELTA(p, n, x, 0); }
void bitund64( uint64_t *p, unsigned n, uint64_t x) { BITUNDELTA(p, n, x, 0); }

void bitund132(unsigned *p, unsigned n, unsigned x) { 
    #ifdef __SSE2__
  __m128i sv = _mm_set1_epi32(x), cv = _mm_set_epi32(4,3,2,1);
  unsigned *ip;
  for(ip = p; ip != p+(n&~(4-1)); ) {
    __m128i v =  _mm_loadu_si128((__m128i *)ip); 
	SCANI128_32(v, sv, cv); 
	_mm_storeu_si128((__m128i *)ip, sv); 
	ip += 4;
  }
  x = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  while(ip < p+n) { 
    *ip = (x += (*ip) + 1); 
	ip++; 
  }
    #else
  BITUNDELTA(p, n, x, 1); 
    #endif
}

void bitundx32(unsigned *p, unsigned n, unsigned x, unsigned inc) { BITUNDELTA(p, n, x, inc); }
void bitundx64(uint64_t *p, unsigned n, uint64_t x, unsigned inc) { BITUNDELTA(p, n, x, inc); }

//----------------------------- zigzag --------------------------------------------------------
#define BITZIGZAG(__p,__n, __start, __act) {\
  typeof(__p[0]) *_p;\
  for(_p = __p; _p != __p+(__n&~(4-1)); ) {\
	_x = ((int)(*_p)-(int)__start); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); __start = *_p++; __act;\
	_x = ((int)(*_p)-(int)__start); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); __start = *_p++; __act;\
	_x = ((int)(*_p)-(int)__start); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); __start = *_p++; __act;\
	_x = ((int)(*_p)-(int)__start); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); __start = *_p++; __act;\
  }\
  while(_p != __p+__n) { \
	_x = ((int)(*_p)-(int)__start); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); __start = *_p++; __act;\
  }\
}

#define BITUNZIGZAG(__p, __n, __start) { typeof(__p[0]) *_p, _z;\
  for(_p = __p; _p != __p+(__n&~(4-1)); ) {\
    _z = *_p; *_p = (__start += (_z >> 1 ^ -(_z & 1))); _p++;\
    _z = *_p; *_p = (__start += (_z >> 1 ^ -(_z & 1))); _p++;\
    _z = *_p; *_p = (__start += (_z >> 1 ^ -(_z & 1))); _p++;\
    _z = *_p; *_p = (__start += (_z >> 1 ^ -(_z & 1))); _p++;\
  }\
  while(_p != __p+__n) { _z = *_p; *_p = (__start += (_z >> 1 ^ -(_z & 1))); _p++; }\
}

unsigned bitz32(unsigned *in, unsigned n, unsigned start) { 
    #ifdef __SSE2__
  unsigned *ip,b; 
  __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), dv;
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	dv = DELTA128_32(iv,sv); 
	sv = iv; 
    dv = ZIGZAG128_32(dv); 
    bv = _mm_or_si128(bv, dv);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128_32(bv, b);
  while(ip != in+n) { 
    int x = ((int)(*ip)-(int)start); x = (x << 1) ^ (x >> 31); start = *ip++; b |= x; 
  }
    #else
  typeof(in[0]) b = 0,*op = out; int _x; BITZIGZAG(in, n, start, b |= (unsigned)_x);
    #endif
  return bsr32(b);
}

unsigned bitzigzag32(unsigned *in, unsigned n, unsigned *out, unsigned start) {
    #ifdef __SSE2__
  unsigned *ip,b,*op = out; 
  __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), dv;
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	dv = DELTA128_32(iv,sv); 
	sv = iv; 
    dv = ZIGZAG128_32(dv); 
    bv = _mm_or_si128(bv, dv);
	_mm_storeu_si128((__m128i *)op, dv); 
	op += 4; 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128_32(bv, b);
  while(ip != in+n) { 
    int x = ((int)(*ip)-(int)start); x = (x << 1) ^ (x >> 31); start = *ip++; b |= x; *op++ = x; 
  }
    #else
  typeof(in[0]) b = 0,*op = out; int _x; BITZIGZAG(in, n, start, b |= (unsigned)_x; *op++ = _x);
    #endif
  return bsr32(b);
}

void bitunzigzag32(unsigned *p, unsigned n, unsigned start) { 
    #ifdef __SSE2__
  __m128i sv = _mm_set1_epi32(start); //, c1 = _mm_set1_epi32(1), cz = _mm_setzero_si128();
  unsigned *ip;
  for(ip = p; ip != p+(n&~(4-1)); ) {
    __m128i iv =  _mm_loadu_si128((__m128i *)ip); 
    iv = UNZIGZAG128_32(iv); 
	SCAN128_32(iv, sv);
	_mm_storeu_si128((__m128i *)ip, sv); 
	ip += 4;
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  while(ip != p+n) { 
    unsigned z = *ip; *ip = (start += (z >> 1 ^ -(z & 1))); ip++;
  }
    #else
  BITUNZIGZAG(p, n, start);
    #endif
}

unsigned bitzigzag64(unsigned *in, unsigned n, unsigned *out, unsigned start) {
  typeof(in[0]) b = 0,*op = out; long long _x; BITZIGZAG(in, n, start, b |= (unsigned long long)_x; *op++ = _x);
  return bsr32(b);
}

void bitunzigzag64(unsigned *p, unsigned n, unsigned start) { 
  BITUNZIGZAG(p, n, start);
}

//------------------- De-/Compose Floating Point -----------------------------------------
void bitdouble(double *in, unsigned n, unsigned *sgn, unsigned *expo, uint64_t *mant) {
  double *ip;
  uint64_t u;
  for(ip = in; ip < in+n; ip++) { 
    u = *(uint64_t *)ip; BITFLOAT(u, *sgn++, *expo++, *mant++, DMANT_BITS, 1ull);
  }
}

void bitundouble(unsigned *sgn, unsigned *expo, uint64_t *mant, unsigned n, double *out) {
  double *op; 
  uint64_t u;
  for(op = out; op < out+n; op++) {
    BITUNFLOAT((uint64_t)(*sgn++), (uint64_t)(*expo++), *mant++, u, DMANT_BITS); *op = *(double *)&u;
  }
}

void bitfloat(float *in, unsigned n, unsigned *sgn, unsigned *expo, unsigned *mant) {
  float *ip;
  unsigned u;
  for(ip = in; ip < in+n; ip++) { 
    u = *(unsigned *)ip; BITFLOAT(u, *sgn++, *expo++, *mant++, FMANT_BITS, 1u);
  }
}

void bitunfloat(unsigned *sgn, unsigned *expo, unsigned *mant, unsigned n, float *out) {
  float *op; 
  unsigned u;
  for(op = out; op < out+n; op++) {
    BITUNFLOAT((*sgn++), (*expo++), *mant++, u, FMANT_BITS); *op = *(float *)&u;
  }
}

