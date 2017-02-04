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
//    "Integer Compression" utility - delta, for, zigzag 
#include "conf.h"  
#include "bitutil.h"

//------------- max. bits for bit packing -------------------
unsigned bit8( uint8_t  *in, unsigned n) { uint8_t  b; BITSIZE_( in, n, b, 8);  return b; }
unsigned bit16(uint16_t *in, unsigned n) { uint16_t b; BITSIZE_( in, n, b, 16); return b; }
unsigned bit32(uint32_t *in, unsigned n) { uint32_t b; BITSIZE32(in, n, b);     return b; }
unsigned bit64(uint64_t *in, unsigned n) { uint64_t b; BITSIZE_( in, n, b, 64); return b; }

//---------------- Delta ----------------------------------------
#define BITD(_p_,_n_, _inc_, _start_, _act_) {\
  typeof(_p_[0]) _x, *_p;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p+=4) {\
	_x = _p[0]-_start_-_inc_; _start_ = _p[0]; _act_;\
	_x = _p[1]-_start_-_inc_; _start_ = _p[1]; _act_;\
	_x = _p[2]-_start_-_inc_; _start_ = _p[2]; _act_;\
	_x = _p[3]-_start_-_inc_; _start_ = _p[3]; _act_;\
  }\
  for(;_p != _p_+_n_;_p++) {\
	_x = _p[0]-_start_-_inc_; _start_ = _p[0]; _act_;\
  }\
}

/*#define BITD(_p_,_n_, _inc_, _start_, _act_) {\
  typeof(_p_[0]) _x, *_p; int i;\
  for(_p = _p_+_n_, i = n&(4-1); i > 0; i--) { --_p; _p[i] -= _p[i-1]; } 
  while(_p != _p_) {\
	_p[-1] -= _p[-2]+_inc_; _act_;\
	_p[-2] -= _p[-3]+_inc_; _act_;\
	_p[-3] -= _p[-4]+_inc_; _act_;\
	_p[-4] -= _p[-5]+_inc_; _act_;\
  }\
  for(;_p != _p_+_n_;_p++) {\
	_x = _p[0]-_start_-_inc_; _start_ = _p[0]; _act_;\
  }\
}*/

// max. bits for delta bit packing
unsigned bitd32(uint32_t *in, unsigned n, uint32_t start) {
    #ifdef __SSE2__
  unsigned *ip,b; __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start);
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, DELTA128x32(iv,sv)); 
	sv = iv; 
  }
  
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128x32(bv, b);
  while(ip != in+n) { 
    unsigned x = *ip-start; 
	start = *ip++; 
	b |= x; 
  }
    #else
  typeof(in[0]) b = 0;
  BITD(in,n, 0, start, b |= _x);
    #endif
  return bsr32(b); 
}

unsigned bitd8( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  b = 0; BITD(in, n, 0, start, b |= _x); return bsr8(b); }
unsigned bitd16(uint16_t *in, unsigned n, uint16_t start) { uint16_t b = 0; BITD(in, n, 0, start, b |= _x); return bsr16(b); }
unsigned bitd64(uint64_t *in, unsigned n, uint64_t start) { uint64_t b = 0; BITD(in, n, 0, start, b |= _x); return bsr64(b); }

unsigned bitdenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, unsigned inc) {
    #ifdef __SSE2__
  unsigned *ip,b,*op = out; 
  __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(inc), dv;
  for(ip = in; ip != in+(n&~(4-1)); ip += 4,op += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, dv = _mm_sub_epi32(DELTA128x32(iv,sv),cv)); 
	sv = iv; 
	_mm_storeu_si128((__m128i *)op, dv); 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128x32(bv, b);
  while(ip != in+n) { 
    unsigned x = *ip-start-inc; 
	start = *ip++;
	b    |= x; 
	*op++ = x;
  }
    #else
  typeof(in[0]) b = 0,*op = out;
  BITD(in, n, inc, start, b |= _x;*op++ = _x);
    #endif
  return bsr32(b);
}

unsigned bitdenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, unsigned inc) { uint8_t  b = 0,*op = out; BITD(in, n, inc, start, b |= _x; *op++ = _x); return bsr8(b);}
unsigned bitdenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, unsigned inc) { uint16_t b = 0,*op = out; BITD(in, n, inc, start, b |= _x; *op++ = _x); return bsr16(b);}
unsigned bitdenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, unsigned inc) { uint64_t b = 0,*op = out; BITD(in, n, inc, start, b |= _x; *op++ = _x); return bsr64(b);}

unsigned bitd132(uint32_t *in, unsigned n, uint32_t start) {
    #ifdef __SSE2__
  unsigned *ip,b; __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, _mm_sub_epi32(DELTA128x32(iv,sv),cv)); 
	sv = iv; 
  }
  
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128x32(bv, b);
  while(ip != in+n) { 
    unsigned x = *ip-start-1; 
	start = *ip++; 
	b |= x; 
  }
    #else
  uint32_t b = 0; 
  BITD(in, n, 1, start, b |= _x);
	#endif
  return bsr32(b); 
}

unsigned bitd18( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  b = 0; BITD(in, n, 1, start, b |= _x); return bsr8(b); }
unsigned bitd116(uint16_t *in, unsigned n, uint16_t start) { uint16_t b = 0; BITD(in, n, 1, start, b |= _x); return bsr16(b); }
unsigned bitd164(uint64_t *in, unsigned n, uint64_t start) { uint64_t b = 0; BITD(in, n, 1, start, b |= _x); return bsr64(b); }

// -- undelta: in-place prefix sum ---------------
#define BITDDEC(_p_, _n_, _start_, _inc_) {\
  typeof(_p_[0]) *_p;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) {\
    _p[0] = (_start_ += _p[0] + _inc_);\
    _p[1] = (_start_ += _p[1] + _inc_);\
    _p[2] = (_start_ += _p[2] + _inc_);\
    _p[3] = (_start_ += _p[3] + _inc_);\
  }\
  for(;_p != _p_+_n_; _p++)\
    _p[0] = (_start_ += _p[0] + _inc_);\
}

void bitddec8(  uint8_t  *p, unsigned n, uint8_t  start) { BITDDEC(p, n, start, 0); }
void bitddec16( uint16_t *p, unsigned n, uint16_t start) { BITDDEC(p, n, start, 0); }
void bitddec32( uint32_t *p, unsigned n, uint32_t start) { BITDDEC(p, n, start, 0); }
void bitddec64( uint64_t *p, unsigned n, uint64_t start) { BITDDEC(p, n, start, 0); }

void bitd1dec32(unsigned *p, unsigned n, unsigned start) { 
    #ifdef __SSE2__
  __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4,3,2,1);
  unsigned *ip;
  for(ip = p; ip != p+(n&~(4-1)); ip += 4) {
    __m128i v =  _mm_loadu_si128((__m128i *)ip); 
	SCANI128x32(v, sv, cv); 
	_mm_storeu_si128((__m128i *)ip, sv); 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  while(ip != p+n) { 
    *ip = (start += (*ip) + 1); 
	ip++; 
  }
    #else
  BITDDEC(p, n, start, 1); 
    #endif
}

void bitd1dec8( uint8_t  *p, unsigned n, uint8_t  start) { BITDDEC(p, n, start, 1); } 
void bitd1dec16(uint16_t *p, unsigned n, uint16_t start) { BITDDEC(p, n, start, 1); }
void bitd1dec64(uint64_t *p, unsigned n, uint64_t start) { BITDDEC(p, n, start, 1); }

void bitddecn8( uint8_t  *p, unsigned n, uint8_t  start, uint8_t  inc) { BITDDEC(p, n, start, inc); }
void bitddecn16(uint16_t *p, unsigned n, uint16_t start, uint16_t inc) { BITDDEC(p, n, start, inc); }
void bitddecn32(uint32_t *p, unsigned n, uint32_t start, uint32_t inc) { BITDDEC(p, n, start, inc); }
void bitddecn64(uint64_t *p, unsigned n, uint64_t start, uint64_t inc) { BITDDEC(p, n, start, inc); }


#define BITDI(_p_,_n_, _start_, _inc_) {\
  typeof(_p_[0]) _x, *_p; _inc_= (uint32_t)(-1);\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p+=4) {\
	_x = _p[0]-_start_; _start_ = _p[0]; if(_x<_inc_) _inc_=_x;\
	_x = _p[1]-_start_; _start_ = _p[1]; if(_x<_inc_) _inc_=_x;\
	_x = _p[2]-_start_; _start_ = _p[2]; if(_x<_inc_) _inc_=_x;\
	_x = _p[3]-_start_; _start_ = _p[3]; if(_x<_inc_) _inc_=_x;\
  }\
  for(;_p != _p_+_n_;_p++) {\
	_x = _p[0]-_start_; _start_ = _p[0]; if(_x<_inc_) _inc_=_x;\
  }\
}

#define BITDIE(_p_,_n_, _start_, _inc_, _act_) {\
  typeof(_p_[0]) _x, *_p;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p+=4) {\
	_x = _p[0]-_start_-_inc_; _start_ = _p[0]; _act_; \
	_x = _p[1]-_start_-_inc_; _start_ = _p[1]; _act_;\
	_x = _p[2]-_start_-_inc_; _start_ = _p[2]; _act_;\
	_x = _p[3]-_start_-_inc_; _start_ = _p[3]; _act_;\
  }\
  for(;_p != _p_+_n_;_p++) {\
	_x = _p[0]-_start_-_inc_; _start_ = _p[0]; _act_;\
  }\
}

uint8_t  bitdi8( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  i; BITDI(in, n, start, i); return i; }
uint16_t bitdi16(uint16_t *in, unsigned n, uint16_t start) { uint16_t i; BITDI(in, n, start, i); return i; }
uint32_t bitdi32(uint32_t *in, unsigned n, uint32_t start) { uint32_t i; BITDI(in, n, start, i); return i; }
uint64_t bitdi64(uint64_t *in, unsigned n, uint64_t start) { uint64_t i; BITDI(in, n, start, i); return i; }

unsigned bitdienc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, unsigned inc) { uint8_t  b = 0,*op = out; BITDIE(in, n, start, inc, b |= _x; *op++ = _x); return bsr8(b); }
unsigned bitdienc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, unsigned inc) { uint16_t b = 0,*op = out; BITDIE(in, n, start, inc, b |= _x; *op++ = _x); return bsr16(b);}
unsigned bitdienc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, unsigned inc) { uint32_t b = 0,*op = out; BITDIE(in, n, start, inc, b |= _x; *op++ = _x); return bsr32(b);}
unsigned bitdienc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, unsigned inc) { uint64_t b = 0,*op = out; BITDIE(in, n, start, inc, b |= _x; *op++ = _x); return bsr64(b);}

void bitdidec8(  uint8_t  *p, unsigned n, uint8_t  start, uint8_t  inc) { BITDDEC(p, n, start, inc); }
void bitdidec16( uint16_t *p, unsigned n, uint16_t start, uint16_t inc) { BITDDEC(p, n, start, inc); }
void bitdidec32( uint32_t *p, unsigned n, uint32_t start, uint32_t inc) { BITDDEC(p, n, start, inc); }
void bitdidec64( uint64_t *p, unsigned n, uint64_t start, uint64_t inc) { BITDDEC(p, n, start, inc); }

//------------------- For ------------------------------
unsigned bitf32(  uint32_t *in, unsigned n, uint32_t start) { return n?bsr32(in[n-1] - start    ):0; }
unsigned bitf132( uint32_t *in, unsigned n, uint32_t start) { return n?bsr32(in[n-1] - start - n):0; }

//----------------------------- zigzag --------------------------------------------------------
#define BITZENC(_p_,_n_, _start_, _act_) {\
  typeof(_p_[0]) *_p;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); ) {\
	_x = ((int)(*_p)-(int)_start_); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); _start_ = *_p++; _act_;\
	_x = ((int)(*_p)-(int)_start_); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); _start_ = *_p++; _act_;\
	_x = ((int)(*_p)-(int)_start_); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); _start_ = *_p++; _act_;\
	_x = ((int)(*_p)-(int)_start_); _x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); _start_ = *_p++; _act_;\
  }\
  while(_p != _p_+_n_) { \
	_x = ((int)(*_p)-(int)_start_);	_x = (_x << 1) ^ (_x >> (sizeof(_x)*8-1)); _start_ = *_p++; _act_;\
  }\
}

#define BITZDEC(_p_, _n_, _start_) {\
  typeof(_p_[0]) *_p, _z;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); ) {\
    _z = *_p; *_p = (_start_ += (_z >> 1 ^ -(_z & 1))); _p++;\
    _z = *_p; *_p = (_start_ += (_z >> 1 ^ -(_z & 1))); _p++;\
    _z = *_p; *_p = (_start_ += (_z >> 1 ^ -(_z & 1))); _p++;\
    _z = *_p; *_p = (_start_ += (_z >> 1 ^ -(_z & 1))); _p++;\
  }\
  while(_p != _p_+_n_) {\
    _z = *_p; *_p = (_start_ += (_z >> 1 ^ -(_z & 1)));	_p++;\
  }\
}

// max. bits for zigzag encoding
unsigned bitz32(unsigned *in, unsigned n, unsigned start) { 
    #ifdef __SSE2__
  unsigned *ip,b; 
  __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), dv;
  for(ip = in; ip != in+(n&~(4-1)); ip += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	dv = DELTA128x32(iv,sv); 
	sv = iv; 
    dv = ZIGZAG128x32(dv); 
    bv = _mm_or_si128(bv, dv);
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128x32(bv, b);
  while(ip != in+n) { 
    int x = ((int)(*ip)-(int)start); 
	x = (x << 1) ^ (x >> 31); 
	start = *ip++; 
	b |= x; 
  }
    #else
  typeof(in[0]) b = 0; 
  int _x; 
  BITZENC(in, n, start, b |= (unsigned)_x);
    #endif
  return bsr32(b);
}

unsigned bitz8( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  b = 0, _x; BITZENC(in, n, start, b |= (uint8_t )_x); return bsr8( b); }
unsigned bitz16(uint16_t *in, unsigned n, uint16_t start) { uint16_t b = 0, _x; BITZENC(in, n, start, b |= (uint16_t)_x); return bsr16(b); }
unsigned bitz64(uint64_t *in, unsigned n, uint64_t start) { uint64_t b = 0, _x; BITZENC(in, n, start, b |= (uint64_t)_x); return bsr64(b); }

unsigned bitzenc32(unsigned *in, unsigned n, unsigned *out, unsigned start, uint32_t inc) {
    #ifdef __SSE2__
  unsigned *ip,b,*op = out; 
  __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), dv;
  for(ip = in; ip != in+(n&~(4-1)); ip += 4,op += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	dv = DELTA128x32(iv,sv); 
	sv = iv; 
    dv = ZIGZAG128x32(dv); 
    bv = _mm_or_si128(bv, dv);
	_mm_storeu_si128((__m128i *)op, dv); 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128x32(bv, b);
  while(ip != in+n) { 
    int x = ((int)(*ip)-(int)start); 
	x = (x << 1) ^ (x >> 31); 
	start = *ip++; 
	b |= x; 
	*op++ = x; 
  }
    #else
  typeof(in[0]) b = 0, *op = out; 
  int _x; 
  BITZENC(in, n, start, b |= (unsigned)_x; *op++ = _x);
    #endif
  return bsr32(b);
}

void bitzdec32(unsigned *p, unsigned n, unsigned start) { 
    #ifdef __AVX2__
  __m256i sv = _mm256_set1_epi32(start), zv = _mm256_setzero_si256(); //, c1 = _mm_set1_epi32(1), cz = _mm_setzero_si128();
  unsigned *ip;
  for(ip = p; ip != p+(n&~(8-1)); ip += 8) {
    __m256i iv =  _mm256_loadu_si256((__m256i *)ip); 
    iv = UNZIGZAG256x32(iv); 
	SCAN256x32(iv, sv);
	_mm256_storeu_si256((__m256i *)ip, sv); 
  }
  start = (unsigned)_mm256_extract_epi32(_mm256_srli_si256(sv,12), 0);
  while(ip != p+n) { 
    unsigned z = *ip; 
	*ip++ = (start += (z >> 1 ^ -(z & 1))); 
  }
    #elif defined(__SSE2__)
  __m128i sv = _mm_set1_epi32(start); //, c1 = _mm_set1_epi32(1), cz = _mm_setzero_si128();
  unsigned *ip;
  for(ip = p; ip != p+(n&~(4-1)); ip += 4) {
    __m128i iv =  _mm_loadu_si128((__m128i *)ip); 
    iv = UNZIGZAG128x32(iv); 
	SCAN128x32(iv, sv);
	_mm_storeu_si128((__m128i *)ip, sv); 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  while(ip != p+n) { 
    unsigned z = *ip; 
	*ip++ = (start += (z >> 1 ^ -(z & 1))); 
  }
    #else
  BITZDEC(p, n, start);
    #endif
}

unsigned bitzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  inc) { uint8_t  b = 0,*op = out; int8_t  _x; BITZENC(in, n, start, b |= (uint8_t)_x; *op++ = _x);  return bsr8(b);  }
unsigned bitzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t inc) { uint16_t b = 0,*op = out; int16_t _x; BITZENC(in, n, start, b |= (uint16_t)_x; *op++ = _x); return bsr16(b); }
unsigned bitzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t inc) { uint64_t b = 0,*op = out; int64_t _x; BITZENC(in, n, start, b |= (uint64_t)_x; *op++ = _x); return bsr32(b); }

void bitzdec8( uint8_t  *p, unsigned n, uint8_t  start) { BITZDEC(p, n, start); }
void bitzdec16(uint16_t *p, unsigned n, uint16_t start) { BITZDEC(p, n, start); }
void bitzdec64(uint64_t *p, unsigned n, uint64_t start) { BITZDEC(p, n, start); }

//-------------- For --------------------------
#define BITFM(_p_,_n_, _mi_, _mx_) {\
  typeof(_p_[0]) *_p; \
  for(_p = _p_, _mi_ = _mx_ = *_p; _p != _p_+(_n_&~(4-1)); ) {\
	if(*_p < _mi_) _mi_ = *_p; if(*_p > _mx_) _mx_ = *_p; _p++; \
	if(*_p < _mi_) _mi_ = *_p; if(*_p > _mx_) _mx_ = *_p; _p++; \
	if(*_p < _mi_) _mi_ = *_p; if(*_p > _mx_) _mx_ = *_p; _p++; \
	if(*_p < _mi_) _mi_ = *_p; if(*_p > _mx_) _mx_ = *_p; _p++; \
  }\
  while(_p != _p_+_n_) { \
	if(*_p < _mi_) _mi_ = *_p; if(*_p > _mx_) _mx_ = *_p; _p++; \
  }\
}

unsigned bitfm8( uint8_t  *in, unsigned n, uint8_t  *pmin) { uint8_t  mi,mx; BITFM(in, n, mi, mx); *pmin = mi; return bsr8( mx - mi); }
unsigned bitfm16(uint16_t *in, unsigned n, uint16_t *pmin) { uint16_t mi,mx; BITFM(in, n, mi, mx); *pmin = mi; return bsr16(mx - mi); }
unsigned bitfm32(uint32_t *in, unsigned n, uint32_t *pmin) { uint32_t mi,mx; BITFM(in, n, mi, mx); *pmin = mi; return bsr32(mx - mi); }
unsigned bitfm64(uint64_t *in, unsigned n, uint64_t *pmin) { uint64_t mi,mx; BITFM(in, n, mi, mx); *pmin = mi; return bsr64(mx - mi); }

//------------------- De-/Compose Floating Point -----------------------------------------
void bitdouble(double *in, unsigned n, int *expo, uint64_t *mant) {
  double *ip;
  for(ip = in; ip < in+n; ip++) { 
    uint64_t u = *(uint64_t *)ip; 
    *expo++ = FLTEXPO(u, DMANT_BITS, 1ull); 
    *mant++ = FLTMANT(u, DMANT_BITS, 1ull);
  }
}

void bitddecouble(int *expo, uint64_t *mant, unsigned n, double *out) {
  double *op; 
  uint64_t u;
  for(op = out; op < out+n; ) {
    BITUNFLOAT( (int64_t)(*expo++), *mant++, u, DMANT_BITS); *op++ = *(double *)&u;
  }
}

void bitzdouble(double *in, unsigned n, int *expo, uint64_t *mant) {
  double *ip;
  for(ip = in; ip < in+n; ip++) { 
    uint64_t u = *(uint64_t *)ip; 
    *expo++ = zigzagenc32((int)FLTEXPO(u, DZMANT_BITS, 1ull)-1023);
    *mant++ = FLTMANT(u, DZMANT_BITS, 1ull);
  }
}

void bitzundouble(int *expo, uint64_t *mant, unsigned n, double *out) {
  double *op; 
  uint64_t u;
  for(op = out; op < out+n; ) {
    BITUNFLOAT( (int64_t)zigzagdec32(*expo++)+1023, *mant++, u, DZMANT_BITS); *op++ = *(double *)&u;
  }
}

void bitfloat(float *in, unsigned n, int *expo, unsigned *mant) {
  float *ip;
  for(ip = in; ip < in+n; ip++) { 
    unsigned u = *(unsigned *)ip; 
    *expo++ = FLTEXPO(u, FMANT_BITS, 1u);
    *mant++ = FLTMANT(u, FMANT_BITS, 1u);
  }
}

void bitunfloat(int *expo, unsigned *mant, unsigned n, float *out) {
  float *op; 
  unsigned u;
  for(op = out; op < out+n; op++) {
    BITUNFLOAT( (*expo++), *mant++, u, FMANT_BITS); *op = *(float *)&u;
  }
}
