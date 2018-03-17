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
//    "Integer Compression" utility - delta, for, zigzag 
#include "conf.h"  
#include "bitutil.h"

//------------- max. bits for bit packing -------------
unsigned bit8( uint8_t  *in, unsigned n) { uint8_t  b; BITSIZE_( in, n, b, 8);  return b; }
unsigned bit16(uint16_t *in, unsigned n) { uint16_t b; BITSIZE_( in, n, b, 16); return b; }
unsigned bit32(uint32_t *in, unsigned n) { uint32_t b; BITSIZE32(in, n, b);     return b; }
unsigned bit64(uint64_t *in, unsigned n) { uint64_t b; BITSIZE_( in, n, b, 64); return b; }

//------------ Delta ----------------------------------
#define DE(i) x = (_p[i]-start)-_md; start = _p[i]
#define BITDE(_t_, _p_, _n_, _md_, _act_) { _t_ *_p, _md = _md_;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) { DE(0);_act_; DE(1);_act_; DE(2);_act_; DE(3);_act_; }\
  for(;_p != _p_+_n_;_p++) { DE(0); _act_; }\
}
//---- Delta (min. Delta = 0)
//-- max. bits for delta encoding
unsigned bitd8( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  b = 0,x; BITDE(uint8_t,  in, n, 0, b |= x); return bsr8( b); }
unsigned bitd64(uint64_t *in, unsigned n, uint64_t start) { uint64_t b = 0,x; BITDE(uint64_t, in, n, 0, b |= x); return bsr64(b); }

unsigned bitd16(uint16_t *in, unsigned n, uint16_t start) {
    #if defined(__SSE2__) && defined(USE_SSE)
  uint16_t *ip,b; __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi16(start);
  for(ip = in; ip != in+(n&~(8-1)); ip += 8) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, DELTA128x16(iv,sv)); 
	sv = iv; 
  }
  
  start = (unsigned short)_mm_cvtsi128_si32(_mm_srli_si128(sv,14));
  HOR128x16(bv, b);
  while(ip != in+n) { 
    unsigned x = *ip-start; 
	start = *ip++; 
	b |= x; 
  }
    #else
  uint16_t b = 0,x;
  BITDE(uint16_t,in,n, 0, b |= x);
    #endif
  return bsr32(b); 
}

unsigned bitd32(uint32_t *in, unsigned n, uint32_t start) {
    #if defined(__SSE2__) && defined(USE_SSE)
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
  uint32_t b = 0,x;
  BITDE(uint32_t,in,n, 0, b |= x);
    #endif
  return bsr32(b); 
}

//----- Undelta: In-place prefix sum (min. Delta = 0) -------------------
#define DD(i) _p[i] = (start += _p[i] + _md);
#define BITDD(_t_, _p_, _n_, _md_) { _t_ *_p; const _md = _md_;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) { DD(0); DD(1); DD(2); DD(3); }\
  for(;_p != _p_+_n_; _p++) DD(0);\
}

void bitddec8( uint8_t  *p, unsigned n, uint8_t  start) { BITDD(uint8_t,  p, n, 0); }
void bitddec16(uint16_t *p, unsigned n, uint16_t start) { BITDD(uint16_t, p, n, 0); }
void bitddec64(uint64_t *p, unsigned n, uint64_t start) { BITDD(uint64_t, p, n, 0); }
void bitddec32(uint32_t *p, unsigned n, unsigned start) { 
    #if defined(__AVX2__) && defined(USE_AVX2)
  __m256i sv = _mm256_set1_epi32(start),zv = _mm256_setzero_si256();
  unsigned *ip;
  for(ip = p; ip != p+(n&~(8-1)); ip += 8) {
    __m256i v =  _mm256_loadu_si256((__m256i *)ip); 
	SCAN256x32(v, sv); 
	_mm256_storeu_si256((__m256i *)ip, sv); 
  }
  start = (unsigned)_mm256_extract_epi32(sv, 7); 
  while(ip != p+n) { 
    *ip = (start += (*ip)); 
	ip++; 
  }	
    #elif defined(__SSE2__) && defined(USE_SSE)
  __m128i sv = _mm_set1_epi32(start);
  unsigned *ip;
  for(ip = p; ip != p+(n&~(4-1)); ip += 4) {
    __m128i v =  _mm_loadu_si128((__m128i *)ip); 
	SCAN128x32(v, sv); 
	_mm_storeu_si128((__m128i *)ip, sv); 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  while(ip != p+n) { 
    *ip = (start += (*ip)); 
	ip++; 
  }
    #else
  BITDD(uint32_t, p, n, 0); 
    #endif
}

//----------- Delta --------------------------
#define DDE(i, _usize_) d = (_p[i]-start)-_md; x = TEMPLATE2(zigzagenc, _usize_)(d - startd); startd = d; start = _p[i]
#define BITDDE(_t_, _p_, _n_, _md_, _usize_, _act_) { _t_ *_p, _md = _md_;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) { DDE(0, _usize_);_act_; DDE(1, _usize_);_act_; DDE(2, _usize_);_act_; DDE(3, _usize_);_act_; }\
  for(;_p != _p_+_n_;_p++) { DDE(0, _usize_); _act_; }\
}

unsigned bitdd8( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  b = 0,x,d,startd=0; BITDDE(uint8_t,  in, n, 1,  8, b |= x); return bsr8(b); }
unsigned bitdd16(uint16_t *in, unsigned n, uint16_t start) { uint16_t b = 0,x,d,startd=0; BITDDE(uint16_t, in, n, 1, 16, b |= x); return bsr16(b); }
unsigned bitdd64(uint64_t *in, unsigned n, uint64_t start) { uint64_t b = 0,x,d,startd=0; BITDDE(uint64_t, in, n, 1, 64, b |= x); return bsr64(b); }
unsigned bitdd32(uint32_t *in, unsigned n, uint32_t start) { uint64_t b = 0,x,d,startd=0; BITDDE(uint32_t, in, n, 1, 32, b |= x); return bsr32(b); }

unsigned bitddenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta) { uint8_t  b = 0,*op = out,x,d,startd=0; BITDDE(uint8_t,  in, n, mindelta,  8,b |= x;*op++ = x); return bsr8(b);}
unsigned bitddenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta) { uint16_t b = 0,*op = out,x,d,startd=0; BITDDE(uint16_t, in, n, mindelta, 16,b |= x;*op++ = x); return bsr16(b);}
unsigned bitddenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta) { uint64_t b = 0,*op = out,x,d,startd=0; BITDDE(uint64_t, in, n, mindelta, 64,b |= x;*op++ = x); return bsr64(b);}
unsigned bitddenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta) { uint32_t b = 0,*op = out,x,d,startd=0; BITDDE(uint32_t, in, n, mindelta, 32,b |= x;*op++ = x); return bsr32(b);}

#define DDD(i) x = _p[i]; d = x - start; _p[i] = zigzagdec64(x)+(int64_t)startd+_md; startd = d; start = x
#define BITDDD(_t_, _p_, _n_, _md_) { _t_ *_p, startd=0,d,x; const _md = _md_;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) { DDD(0); DDD(1); DDD(2); DDD(3); }\
  for(;_p != _p_+_n_; _p++) DDD(0);\
}
void bitdddec8( uint8_t  *p, unsigned n, uint8_t  start) { BITDDD(uint8_t,  p, n, 1); } 
void bitdddec16(uint16_t *p, unsigned n, uint16_t start) { BITDDD(uint16_t, p, n, 1); }
void bitdddec64(uint64_t *p, unsigned n, uint64_t start) { BITDDD(uint64_t, p, n, 1); }
void bitdddec32(uint32_t *p, unsigned n, uint32_t start) { BITDDD(uint32_t, p, n, 1); }

//-----Undelta: In-place prefix sum (min. Delta = 1) -------------------
unsigned bitd18( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  b = 0,x; BITDE(uint8_t,  in, n, 1, b |= x); return bsr8(b); }
unsigned bitd116(uint16_t *in, unsigned n, uint16_t start) { uint16_t b = 0,x; BITDE(uint16_t, in, n, 1, b |= x); return bsr16(b); }
unsigned bitd164(uint64_t *in, unsigned n, uint64_t start) { uint64_t b = 0,x; BITDE(uint64_t, in, n, 1, b |= x); return bsr64(b); }
unsigned bitd132(uint32_t *in, unsigned n, uint32_t start) {
    #if defined(__SSE2__) && defined(USE_SSE)
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
  uint32_t b = 0,x; 
  BITDE(uint32_t, in, n, 1, b |= x);
	#endif
  return bsr32(b); 
}

void bitd1dec8( uint8_t  *p, unsigned n, uint8_t  start) { BITDD(uint8_t,  p, n, 1); } 
void bitd1dec16(uint16_t *p, unsigned n, uint16_t start) { BITDD(uint16_t, p, n, 1); }
void bitd1dec64(uint64_t *p, unsigned n, uint64_t start) { BITDD(uint64_t, p, n, 1); }
void bitd1dec32(uint32_t *p, unsigned n, uint32_t start) {
    #if defined(__AVX2__) && defined(USE_AVX2)
  __m256i sv = _mm256_set1_epi32(start),zv = _mm256_setzero_si256(), cv = _mm256_set_epi32(8,7,6,5,4,3,2,1);
  unsigned *ip;
  for(ip = p; ip != p+(n&~(8-1)); ip += 8) {
    __m256i v =  _mm256_loadu_si256((__m256i *)ip); 
	SCANI256x32(v, sv, cv); 
	_mm256_storeu_si256((__m256i *)ip, sv); 
  }
  start = (unsigned)_mm256_extract_epi32(sv, 7); 
  while(ip != p+n) { 
    *ip = (start += (*ip) + 1); 
	ip++; 
  }	
    #elif defined(__SSE2__) && defined(USE_SSE)
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
  BITDD(uint32_t, p, n, 1); 
    #endif
}

//---------Delta encoding/decoding (min. Delta = mindelta) -------------------
//determine min. delta for encoding w/ bitdiencNN function
#define DI(i) x = _p[0] - start; start = _p[0]; if(x < mindelta) mindelta = x
#define BITDIE(_p_, _n_) {\
  for(_p = _p_,mindelta = _p[0]; _p != _p_+(_n_&~(4-1)); _p+=4) { DI(0); DI(1); DI(2); DI(3); }\
  for(;_p != _p_+_n_;_p++) DI(0);\
}

uint8_t  bitdi8( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  mindelta,x,*_p; BITDIE(in, n); return mindelta; }
uint16_t bitdi16(uint16_t *in, unsigned n, uint16_t start) { uint16_t mindelta,x,*_p; BITDIE(in, n); return mindelta; }
uint32_t bitdi32(uint32_t *in, unsigned n, uint32_t start) { uint32_t mindelta,x,*_p; BITDIE(in, n); return mindelta; }
uint64_t bitdi64(uint64_t *in, unsigned n, uint64_t start) { uint64_t mindelta,x,*_p; BITDIE(in, n); return mindelta; }

unsigned bitdienc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta) { uint8_t  b = 0,*op = out,x; BITDE(uint8_t,  in, n, mindelta, b |= x;*op++ = x); return bsr8(b);}
unsigned bitdienc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta) { uint16_t b = 0,*op = out,x; BITDE(uint16_t, in, n, mindelta, b |= x;*op++ = x); return bsr16(b);}
unsigned bitdienc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta) { uint64_t b = 0,*op = out,x; BITDE(uint64_t, in, n, mindelta, b |= x;*op++ = x); return bsr64(b);}
unsigned bitdienc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta) {
    #if defined(__SSE2__) && defined(USE_SSE)
  unsigned *ip,b,*op = out; 
  __m128i bv = _mm_setzero_si128(), sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(mindelta), dv;
  for(ip = in; ip != in+(n&~(4-1)); ip += 4,op += 4) { 
    __m128i iv = _mm_loadu_si128((__m128i *)ip); 
	bv = _mm_or_si128(bv, dv = _mm_sub_epi32(DELTA128x32(iv,sv),cv)); 
	sv = iv; 
	_mm_storeu_si128((__m128i *)op, dv); 
  }
  start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
  HOR128x32(bv, b);
  while(ip != in+n) { 
    unsigned x = *ip-start-mindelta; 
	start = *ip++;
	b    |= x; 
	*op++ = x;
  }
    #else
  uint32_t b = 0,*op = out, x, *_p;
  BITDE(uint32_t, in, n, mindelta, b |= x; *op++ = x);
    #endif
  return bsr32(b);
}
 
void bitdidec8(  uint8_t  *p, unsigned n, uint8_t  start, uint8_t  mindelta) { BITDD(uint8_t,  p, n, mindelta); }
void bitdidec16( uint16_t *p, unsigned n, uint16_t start, uint16_t mindelta) { BITDD(uint16_t, p, n, mindelta); }
void bitdidec32( uint32_t *p, unsigned n, uint32_t start, uint32_t mindelta) { BITDD(uint32_t, p, n, mindelta); }
void bitdidec64( uint64_t *p, unsigned n, uint64_t start, uint64_t mindelta) { BITDD(uint64_t, p, n, mindelta); }

//------------------- For ------------------------------
unsigned bitf8(   uint8_t  *in, unsigned n, uint8_t  start) { return n?bsr8( in[n-1] - start    ):0; }
unsigned bitf18(  uint8_t  *in, unsigned n, uint8_t  start) { return n?bsr8( in[n-1] - start - n):0; }
unsigned bitf16(  uint16_t *in, unsigned n, uint16_t start) { return n?bsr16(in[n-1] - start    ):0; }
unsigned bitf116( uint16_t *in, unsigned n, uint16_t start) { return n?bsr16(in[n-1] - start - n):0; }
unsigned bitf32(  uint32_t *in, unsigned n, uint32_t start) { return n?bsr32(in[n-1] - start    ):0; }
unsigned bitf132( uint32_t *in, unsigned n, uint32_t start) { return n?bsr32(in[n-1] - start - n):0; }
unsigned bitf64(  uint64_t *in, unsigned n, uint64_t start) { return n?bsr64(in[n-1] - start    ):0; }
unsigned bitf164( uint64_t *in, unsigned n, uint64_t start) { return n?bsr64(in[n-1] - start - n):0; }

//------------------- Zigzag ---------------------------
#define ZE(i,_it_,_usize_) x = TEMPLATE2(zigzagenc, _usize_)((_it_)_p[i]-(_it_)start); start = _p[i]
#define BITZENC(_ut_, _it_, _usize_, _p_,_n_, _act_) { _ut_ *_p;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) { ZE(0,_it_,_usize_);_act_; ZE(1,_it_,_usize_);_act_; ZE(2,_it_,_usize_);_act_; ZE(3,_it_,_usize_);_act_; }\
  for(;_p != _p_+_n_; _p++) { ZE(0,_it_,_usize_); _act_; }\
}

// max. bits for zigzag encoding
unsigned bitz8( uint8_t  *in, unsigned n, uint8_t  start) { uint8_t  b = 0, x; BITZENC(uint8_t,  int8_t, 8, in, n, b |= x); return bsr8( b); }
unsigned bitz16(uint16_t *in, unsigned n, uint16_t start) { uint16_t b = 0, x; BITZENC(uint16_t, int16_t,16,in, n, b |= x); return bsr16(b); }
unsigned bitz64(uint64_t *in, unsigned n, uint64_t start) { uint64_t b = 0, x; BITZENC(uint64_t, int64_t,64,in, n, b |= x); return bsr64(b); }
unsigned bitz32(unsigned *in, unsigned n, unsigned start) { 
    #if defined(__SSE2__) && defined(USE_SSE)
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
  uint32_t b = 0, x; 
  BITZENC(uint32_t, int32_t, 32, in, n, b |= x);
    #endif
  return bsr32(b);
}

unsigned bitzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta) { uint8_t  b = 0,*op = out;uint8_t  x; BITZENC(uint8_t,  int8_t,  8,in, n, b |= x; *op++ = x); return bsr8(b);  }
unsigned bitzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta) { uint16_t b = 0,*op = out;uint16_t x; BITZENC(uint16_t, int16_t,16,in, n, b |= x; *op++ = x); return bsr16(b); }
unsigned bitzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta) { uint64_t b = 0,*op = out;uint64_t x; BITZENC(uint64_t, int64_t,64,in, n, b |= x; *op++ = x); return bsr64(b); }
unsigned bitzenc32(unsigned *in, unsigned n, unsigned *out, unsigned start, uint32_t mindelta) {
    #if defined(__SSE2__) && defined(USE_SSE)
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
  uint32_t b = 0, *op = out,x; 
  BITZENC(uint32_t, int32_t, 32,in, n, b |= x; *op++ = x);
    #endif
  return bsr32(b);
}

#define ZD(_t_, _usize_, i) { _t_ _z = _p[i]; _p[i] = (start += TEMPLATE2(zigzagdec, _usize_)(_z)); }
#define BITZDEC(_t_, _usize_, _p_, _n_) { _t_ *_p;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) { ZD(_t_, _usize_, 0); ZD(_t_, _usize_, 1); ZD(_t_, _usize_, 2); ZD(_t_, _usize_, 3); }\
  for(;_p != _p_+_n_;_p++) ZD(_t_, _usize_, 0);\
}

void bitzdec8( uint8_t  *p, unsigned n, uint8_t  start) { BITZDEC(uint8_t,  8, p, n); }
void bitzdec64(uint64_t *p, unsigned n, uint64_t start) { BITZDEC(uint64_t, 64,p, n); }

void bitzdec16(uint16_t *p, unsigned n, uint16_t start) { 
    #if defined(__SSSE3__) && defined(USE_SSE)
  __m128i sv = _mm_set1_epi16(start); //, c1 = _mm_set1_epi32(1), cz = _mm_setzero_si128();
  uint16_t *ip;
  for(ip = p; ip != p+(n&~(8-1)); ip += 8) {
    __m128i iv =  _mm_loadu_si128((__m128i *)ip); 
    iv = UNZIGZAG128x16(iv); 
	SCAN128x16(iv, sv);
	_mm_storeu_si128((__m128i *)ip, sv); 
  }
  start = (uint16_t)_mm_cvtsi128_si32(_mm_srli_si128(sv,14));
  while(ip != p+n) {
    uint16_t z = *ip; 
	*ip++ = (start += (z >> 1 ^ -(z & 1))); 
  }
    #else
  BITZDEC(uint16_t, 16, p, n);
    #endif
}

void bitzdec32(unsigned *p, unsigned n, unsigned start) { 
    #if defined(__AVX2__) && defined(USE_AVX2)
  __m256i sv = _mm256_set1_epi32(start), zv = _mm256_setzero_si256(); //, c1 = _mm_set1_epi32(1), cz = _mm_setzero_si128();
  unsigned *ip;
  for(ip = p; ip != p+(n&~(8-1)); ip += 8) {
    __m256i iv =  _mm256_loadu_si256((__m256i *)ip); 
    iv = UNZIGZAG256x32(iv); 
	SCAN256x32(iv, sv);
	_mm256_storeu_si256((__m256i *)ip, sv); 
  }
  start = (unsigned)_mm256_extract_epi32(_mm256_srli_si256(sv,12), 4);
  while(ip != p+n) { 
    unsigned z = *ip; 
	*ip++ = (start += (z >> 1 ^ -(z & 1))); 
  }
    #elif defined(__SSE2__) && defined(USE_SSE)
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
	*ip++ = (start += zigzagdec32(z)); 
  }
    #else
  BITZDEC(uint32_t, 32, p, n);
    #endif
}

//----------------------- XOR ---------------------------------
#define XE(i) x = _p[i] ^ start; start = _p[i]
#define BITXENC(_t_, _p_, _n_, _act_) { _t_ *_p;\
  for(_p = _p_; _p != _p_+(_n_&~(4-1)); _p += 4) { XE(0);_act_; XE(1);_act_; XE(2);_act_; XE(3);_act_; }\
  for(        ; _p != _p_+ _n_;         _p++   ) { XE(0);_act_; }\
}
unsigned bitxenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start) { uint8_t  b = 0,*op = out,x; BITXENC(uint8_t,  in, n, b |= x; *op++ = x); return bsr8(b);  }
unsigned bitxenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start) { uint16_t b = 0,*op = out,x; BITXENC(uint16_t, in, n, b |= x; *op++ = x); return bsr16(b); }
unsigned bitxenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start) { uint32_t b = 0,*op = out,x; BITXENC(uint32_t, in, n, b |= x; *op++ = x); return bsr32(b); }
unsigned bitxenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start) { uint64_t b = 0,*op = out,x; BITXENC(uint64_t, in, n, b |= x; *op++ = x); return bsr64(b); }

#define XD(i) _p[i] = (start ^= _p[i])
#define BITXDEC(_t_, _p_, _n_) { _t_ *_p, _x;\
  for(_p = _p_;_p != _p_+(_n_&~(4-1)); _p += 4) { XD(0); XD(1); XD(2); XD(3); }\
  for(        ;_p != _p_+ _n_        ; _p++   )   XD(0);\
}

void bitxdec8( uint8_t  *p, unsigned n, uint8_t  start) { BITXDEC(uint8_t,  p, n); }
void bitxdec16(uint16_t *p, unsigned n, uint16_t start) { BITXDEC(uint16_t, p, n); }
void bitxdec32(uint32_t *p, unsigned n, uint32_t start) { BITXDEC(uint32_t, p, n); }
void bitxdec64(uint64_t *p, unsigned n, uint64_t start) { BITXDEC(uint64_t, p, n); }

//-------------- For : calc max. bits, min,max value ------------------------
#define FM(i) mi = _p[i] < mi?_p[i]:mi; mx = _p[i] > mx?_p[i]:mx
#define BITFM(_t_, _p_,_n_) { _t_ *_p; \
  for(_p = _p_, mi = mx = *_p; _p != _p_+(_n_&~(4-1)); _p += 4) { FM(0); FM(1); FM(2); FM(3); }\
  for(;_p != _p_+_n_; _p++) FM(0);\
}

unsigned bitfm8( uint8_t  *in, unsigned n, uint8_t  *pmin) { uint8_t  mi,mx; BITFM(uint8_t,  in, n); *pmin = mi; return bsr8( mx - mi); }
unsigned bitfm16(uint16_t *in, unsigned n, uint16_t *pmin) { uint16_t mi,mx; BITFM(uint16_t, in, n); *pmin = mi; return bsr16(mx - mi); }
unsigned bitfm32(uint32_t *in, unsigned n, uint32_t *pmin) { uint32_t mi,mx; BITFM(uint32_t, in, n); *pmin = mi; return bsr32(mx - mi); }
unsigned bitfm64(uint64_t *in, unsigned n, uint64_t *pmin) { uint64_t mi,mx; BITFM(uint64_t, in, n); *pmin = mi; return bsr64(mx - mi); }


//----------- Lossy floating point conversion: pad the trailing mantissa bits with zeros according to the error e (ex. 0.00001)  -----------------------------------
static inline double efloat64(double d, double e, int lg2e) {
  uint64_t u, du = ctou64(&d);
  int      v = (du>>52 & 0x7ff)-0x3fe;
  if((v = 54 - v - lg2e) <= 0) return d;
  v = v > 52?52:v;
  do u = du & (~((1ull<<(--v))-1)); while(fabs((ctof64(&u) - d)/d) > e);
  return ctof64(&u);
}

void padfloat64(double *in, size_t n, double *out, double e) { int lg2e = -log(e)/log(2.0); double *ip; for(ip = in; ip < in+n; ip++,out++) *out = efloat64(*ip, e, lg2e); }

static inline float efloat32(float d, float e, int lg2e) {
  uint32_t u, du = ctou32(&d);
  int      v = (du>>23 & 0xff)-0x7e;
  if((v = 25 - v - lg2e) <= 0) return d;
  v = v > 23?23:v;
  do u = du & (~((1u<<(--v))-1)); while(fabsf((ctof32(&u) - d)/d) > e);
  return ctof32(&u);
}

void padfloat32(float *in, size_t n, float *out, float e) { int lg2e = -log(e)/log(2.0); float *ip; for(ip = in; ip < in+n; ip++,out++) *out = efloat32(*ip, e, lg2e); }

