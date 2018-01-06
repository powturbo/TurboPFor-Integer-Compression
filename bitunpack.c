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
//   "Integer Compression" Bit Packing 
#include "conf.h" 
#include "bitutil.h" 
#include "bitpack.h"
#include "vint.h"
#define PAD8(_x_) (((_x_)+7)/8)

#pragma warning( disable : 4005) 
#pragma warning( disable : 4090) 
#pragma warning( disable : 4068) 

#pragma GCC push_options
#pragma GCC optimize ("align-functions=16")
#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wunsequenced"

typedef unsigned char *(*BITUNPACK_F8)( const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out);
typedef unsigned char *(*BITUNPACK_D8)( const unsigned char *__restrict in, unsigned n, uint8_t  *__restrict out, uint8_t start);
typedef unsigned char *(*BITUNPACK_F16)(const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out);
typedef unsigned char *(*BITUNPACK_D16)(const unsigned char *__restrict in, unsigned n, uint16_t *__restrict out, uint16_t start);
typedef unsigned char *(*BITUNPACK_F32)(const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out);
typedef unsigned char *(*BITUNPACK_D32)(const unsigned char *__restrict in, unsigned n, uint32_t *__restrict out, uint32_t start);
typedef unsigned char *(*BITUNPACK_F64)(const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out);
typedef unsigned char *(*BITUNPACK_D64)(const unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start);
   
#define PREFETCH(_ip_) __builtin_prefetch(_ip_+512,0)//#define PREFETCH(ip)

#if !defined(SSE2_ON) && !defined(AVX2_ON)

  #if 0
#define OP(_op_, _x_) *_op_++
#define OPX(_op_)  
  #else
#define OP(_op_, _x_) _op_[_x_]
#define OPX(_op_)     _op_ += 32
  #endif 

#define OPI(_op_,_parm_) OPX(_op_)
#define OUT( _op_, _x_, _w_, _parm_) OP(_op_,_x_) = _w_ 
#define _BITUNPACK_ bitunpack 
#include "bitunpack_.h"

#define DELTA

#define OUT( _op_, _x_, _w_, _parm_) OP(_op_,_x_) = (_parm_ += (_w_))
#define _BITUNPACK_ bitdunpack  // delta + 0
#include "bitunpack_.h"
    
#define OUT( _op_, _x_, _w_, _parm_) OP(_op_,_x_) = (_parm_ += TEMPLATE2(zigzagdec, USIZE)(_w_))
#define _BITUNPACK_ bitzunpack  // zigzag
#include "bitunpack_.h"
  
#define OUT( _op_, _x_, _w_, _parm_) OP(_op_,_x_) = (_parm_ + (_w_))
#define _BITUNPACK_ bitfunpack  // for
#include "bitunpack_.h"

#define OPI(_op_,_parm_) OPX(_op_); _parm_ += 32
#define OUT( _op_, _x_, _w_, _parm_) OP(_op_,_x_) = (_parm_ += (_w_)) + (_x_+1)  
#define _BITUNPACK_ bitd1unpack  // delta + 1
#include "bitunpack_.h"

#define OUT( _op_, _x_, _w_, _parm_) OP(_op_,_x_) = _parm_ + (_w_)+(_x_+1)
#define _BITUNPACK_ bitf1unpack  // for + 1
#include "bitunpack_.h"
#undef OPI

#define BITNUNPACK(in, n, out, csize, usize) {\
  unsigned char *ip = in;\
  for(op = out,out+=n; op < out;) { unsigned oplen = out - op,b; if(oplen > csize) oplen = csize;		PREFETCH(in+512,0);\
    b = *ip++; ip = TEMPLATE2(bitunpacka, usize)[b](ip, oplen, op);\
	op += oplen;\
  } \
  return ip - in;\
}

#define BITNDUNPACK(in, n, out, csize, usize, _bitunpacka_) {\
  unsigned char *ip = in;\
  if(!n) return 0;\
  TEMPLATE2(vbxget, usize)(ip, start); \
  *out++ = start;\
  for(--n,op = out,out+=n; op < out;) { unsigned oplen = out - op,b; if(oplen > csize) oplen = csize;		PREFETCH(ip+512);\
    b = *ip++; ip = TEMPLATE2(_bitunpacka_, usize)[b](ip, oplen, op, start);\
	op += oplen;\
    start = op[-1];\
  } return ip - in;\
}
size_t bitnunpack8(   unsigned char *__restrict in, size_t n, uint8_t  *__restrict out) { uint8_t  *op; BITNUNPACK(in, n, out, 128,  8); } 
size_t bitnunpack16(  unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op; BITNUNPACK(in, n, out, 128, 16); } 
size_t bitnunpack32(  unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op; BITNUNPACK(in, n, out, 128, 32); } 
size_t bitnunpack64(  unsigned char *__restrict in, size_t n, uint64_t *__restrict out) { uint64_t *op; BITNUNPACK(in, n, out, 128, 64); } 

size_t bitndunpack8(  unsigned char *__restrict in, size_t n, uint8_t  *__restrict out) { uint8_t  *op,start; BITNDUNPACK(in, n, out, 128,  8, bitdunpacka); } 
size_t bitndunpack16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; BITNDUNPACK(in, n, out, 128, 16, bitdunpacka); } 
size_t bitndunpack32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; BITNDUNPACK(in, n, out, 128, 32, bitdunpacka); } 
size_t bitndunpack64( unsigned char *__restrict in, size_t n, uint64_t *__restrict out) { uint64_t *op,start; BITNDUNPACK(in, n, out, 128, 64, bitdunpacka); } 

size_t bitnd1unpack8( unsigned char *__restrict in, size_t n, uint8_t  *__restrict out) { uint8_t  *op,start; BITNDUNPACK(in, n, out, 128,  8, bitd1unpacka); } 
size_t bitnd1unpack16(unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; BITNDUNPACK(in, n, out, 128, 16, bitd1unpacka); } 
size_t bitnd1unpack32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; BITNDUNPACK(in, n, out, 128, 32, bitd1unpacka); } 
size_t bitnd1unpack64(unsigned char *__restrict in, size_t n, uint64_t *__restrict out) { uint64_t *op,start; BITNDUNPACK(in, n, out, 128, 64, bitd1unpacka); } 

size_t bitnzunpack8(  unsigned char *__restrict in, size_t n, uint8_t  *__restrict out) { uint8_t  *op,start; BITNDUNPACK(in, n, out, 128,  8, bitzunpacka); } 
size_t bitnzunpack16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; BITNDUNPACK(in, n, out, 128, 16, bitzunpacka); } 
size_t bitnzunpack32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; BITNDUNPACK(in, n, out, 128, 32, bitzunpacka); } 
size_t bitnzunpack64( unsigned char *__restrict in, size_t n, uint64_t *__restrict out) { uint64_t *op,start; BITNDUNPACK(in, n, out, 128, 64, bitzunpacka); } 
#endif

#if defined(__SSE2__) && defined(SSE2_ON)  
#include <emmintrin.h>

#define VSTO( _op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, ov)
#define VSTO0(_op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, _parm_)
#include "bitunpack_.h"

#define BITUNBLK128V32_0(ip, _i_, _op_, _parm_) {__m128i ov;\
  VSTO0(_op_,  0, ov, _parm_);\
  VSTO0(_op_,  1, ov, _parm_);\
  VSTO0(_op_,  2, ov, _parm_);\
  VSTO0(_op_,  3, ov, _parm_);\
  VSTO0(_op_,  4, ov, _parm_);\
  VSTO0(_op_,  5, ov, _parm_);\
  VSTO0(_op_,  6, ov, _parm_);\
  VSTO0(_op_,  7, ov, _parm_);\
  VSTO0(_op_,  8, ov, _parm_);\
  VSTO0(_op_,  9, ov, _parm_);\
  VSTO0(_op_, 10, ov, _parm_);\
  VSTO0(_op_, 11, ov, _parm_);\
  VSTO0(_op_, 12, ov, _parm_);\
  VSTO0(_op_, 13, ov, _parm_);\
  VSTO0(_op_, 14, ov, _parm_);\
  VSTO0(_op_, 15, ov, _parm_);\
  VSTO0(_op_, 16, ov, _parm_);\
  VSTO0(_op_, 17, ov, _parm_);\
  VSTO0(_op_, 18, ov, _parm_);\
  VSTO0(_op_, 19, ov, _parm_);\
  VSTO0(_op_, 20, ov, _parm_);\
  VSTO0(_op_, 21, ov, _parm_);\
  VSTO0(_op_, 22, ov, _parm_);\
  VSTO0(_op_, 23, ov, _parm_);\
  VSTO0(_op_, 24, ov, _parm_);\
  VSTO0(_op_, 25, ov, _parm_);\
  VSTO0(_op_, 26, ov, _parm_);\
  VSTO0(_op_, 27, ov, _parm_);\
  VSTO0(_op_, 28, ov, _parm_);\
  VSTO0(_op_, 29, ov, _parm_);\
  VSTO0(_op_, 30, ov, _parm_);\
  VSTO0(_op_, 31, ov, _parm_);\
}
#define BITUNPACK0(_parm_) _parm_ = _mm_setzero_si128()

unsigned char *bitunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b) {
  const unsigned char *ip = in+PAD8(128*b);
  __m128i sv; 
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip;
}
unsigned char *bitunpack256w32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b) {
  const unsigned char *_in=in; unsigned *_out=out;
  __m128i sv; 
  BITUNPACK128V32(in, b, out, sv); out = _out+128; in=_in+PAD8(128*b);
  BITUNPACK128V32(in, b, out, sv); 
  return (unsigned char *)_in+PAD8(256*b);
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0

//------------------------------SSE -----------------------------------------------
  #ifdef __SSSE3__
#include <tmmintrin.h>
static char shuffles[16][16] = {
  #define _ 0x80
        { _,_,_,_, _,_,_,_, _,_, _, _,  _, _, _,_  },
        { 0,1,2,3, _,_,_,_, _,_, _, _,  _, _, _,_  },
        { _,_,_,_, 0,1,2,3, _,_, _, _,  _, _, _,_  },
        { 0,1,2,3, 4,5,6,7, _,_, _, _,  _, _, _,_  },
        { _,_,_,_, _,_,_,_, 0,1, 2, 3,  _, _, _,_  },
        { 0,1,2,3, _,_,_,_, 4,5, 6, 7,  _, _, _,_  },
        { _,_,_,_, 0,1,2,3, 4,5, 6, 7,  _, _, _,_  },
        { 0,1,2,3, 4,5,6,7, 8,9,10,11,  _, _, _,_  },
        { _,_,_,_, _,_,_,_, _,_,_,_,    0, 1, 2, 3 },
        { 0,1,2,3, _,_,_,_, _,_,_,  _,  4, 5, 6, 7 },
        { _,_,_,_, 0,1,2,3, _,_,_,  _,  4, 5, 6, 7 },
        { 0,1,2,3, 4,5,6,7, _,_, _, _,  8, 9,10,11 },
        { _,_,_,_, _,_,_,_, 0,1, 2, 3,  4, 5, 6, 7 },
        { 0,1,2,3, _,_,_,_, 4,5, 6, 7,  8, 9,10,11 },
        { _,_,_,_, 0,1,2,3, 4,5, 6, 7,  8, 9,10,11 },
        { 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15 },
  #undef _
};
  #endif
  
#define VSTO( _op_, _i_, _ov_, _parm_) if((_i_) & 1) m = (*bb++) >> 4; else m = (*bb) & 0xf; _mm_storeu_si128(_op_++, _mm_add_epi32(_ov_, _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) )); pex += popcnt32(m)
#define VSTO0(_op_, _i_, ov, _parm_)   if((_i_) & 1) m = (*bb++) >> 4; else m = (*bb) & 0xf; _mm_storeu_si128(_op_++,                     _mm_shuffle_epi8(               _mm_loadu_si128((__m128i*)pex),     _mm_load_si128((__m128i*)shuffles[m]) ) );  pex += popcnt32(m)
#define BITUNPACK0(_parm_) //_parm_ = _mm_setzero_si128()
#include "bitunpack_.h"

unsigned char *_bitunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb) {
  const unsigned char *ip = in+PAD8(128*b); unsigned m;
  __m128i sv; 
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip; 
}
unsigned char *_bitunpack256w32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb) {
  const unsigned char *_in=in; unsigned *_out=out;
  unsigned m;
  __m128i sv; 
  BITUNPACK128V32(in, b, out, sv); out = _out+128; in=_in+PAD8(128*b);
  BITUNPACK128V32(in, b, out, sv); 
  return (unsigned char *)_in+PAD8(256*b);
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0

//-------------------------------------------------------------------
#define VSTO0(_op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, _parm_)
#define VSTO(_op_, i, _ov_, __sv) _ov_ = UNZIGZAG128x32(_ov_); SCAN128x32(_ov_,__sv); _mm_storeu_si128(_op_++, __sv)
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *bitzunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(128*b); 
  __m128i sv = _mm_set1_epi32(start); 
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

#define VSTO(_op_, i, _ov_, _sv_) SCAN128x32(_ov_,_sv_); _mm_storeu_si128(_op_++, _sv_)
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *bitdunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(128*b); 
  __m128i sv = _mm_set1_epi32(start);
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

#define VSTO( _op_, _i_, _ov_, _parm_) _mm_storeu_si128(_op_++, _mm_add_epi32(_ov_, sv))
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *bitfunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(128*b); 
  __m128i sv = _mm_set1_epi32(start);
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

//---------------------------- SSE ----------------------------------------------
  #ifdef __SSSE3__
#define VEXP(_i_, _ov_)         if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_add_epi32(_ov_, _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) ); pex += popcnt32(m)
#define VSTO( _op_, _i_, _ov_, _sv_)   VEXP( _i_, _ov_); SCAN128x32(_ov_,_sv_); _mm_storeu_si128(_op_++, _sv_);
#define VEXP0(_i_, _ov_)        if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_shuffle_epi8(_mm_loadu_si128((__m128i*)pex),_mm_load_si128((__m128i*)shuffles[m]) ); pex += popcnt32(m)
#define VSTO0(_op_, _i_, _ov_, _sv_)   VEXP0( _i_, _ov_); SCAN128x32(_ov_,_sv_); _mm_storeu_si128(_op_++, _sv_);
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *_bitdunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) { 
  const unsigned char *ip = in+PAD8(128*b); unsigned m;
  __m128i sv = _mm_set1_epi32(start);
  BITUNPACK128V32(in, b, out, sv); 
  return (unsigned char *)ip; 
}

#define VEXP(_i_, _ov_)         if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_add_epi32(_ov_, _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) ); pex += popcnt32(m)
#define VSTO( _op_, _i_, _ov_, _sv_)   VEXP( _i_, _ov_); _ov_ = UNZIGZAG128x32(_ov_); SCAN128x32(_ov_,_sv_); _mm_storeu_si128(_op_++, _sv_);
#define VEXP0(_i_, _ov_)        if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_shuffle_epi8(_mm_loadu_si128((__m128i*)pex),_mm_load_si128((__m128i*)shuffles[m]) ); pex += popcnt32(m)
#define VSTO0(_op_, _i_, _ov_, _sv_)   VEXP0( _i_, _ov_); _ov_ = UNZIGZAG128x32(_ov_); SCAN128x32(_ov_,_sv_); _mm_storeu_si128(_op_++, _sv_);
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *_bitzunpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) { 
  const unsigned char *ip = in+PAD8(128*b); unsigned m;
  __m128i sv = _mm_set1_epi32(start);
  BITUNPACK128V32(in, b, out, sv); 
  return (unsigned char *)ip; 
}
  #endif

#define VSTO(_op_, i, _ov_, __sv) SCANI128x32(_ov_,__sv,cv); _mm_storeu_si128(_op_++, __sv);
#define VSTO0(_op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, _parm_); _parm_ = _mm_add_epi32(_parm_, cv)
#include "bitunpack_.h"
#define BITUNPACK0(_parm_) _parm_ = _mm_add_epi32(_parm_, cv); cv = _mm_set1_epi32(4)
unsigned char *bitd1unpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(128*b);
  __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4,3,2,1);
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

#define VSTO( _op_, _i_, _ov_, _sv_) _mm_storeu_si128(_op_++, _mm_add_epi32(_ov_, _sv_)); _sv_ = _mm_add_epi32(_sv_, cv)
#define VSTO0(_op_, _i_, ov, _sv_)   _mm_storeu_si128(_op_++, _sv_);                      _sv_ = _mm_add_epi32(_sv_, cv); 
#include "bitunpack_.h"
#define BITUNPACK0(_parm_) 
unsigned char *bitf1unpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(128*b); 
  __m128i sv = _mm_set_epi32(start+4,start+3,start+2,start+1), cv = _mm_set1_epi32(4);
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

  #ifdef __SSSE3__
#define VEXP(_i_, _ov_)         if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_add_epi32(_ov_, _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) ); pex += popcnt32(m)
#define VSTO( _op_, _i_, _ov_, _sv_)   VEXP( _i_, _ov_); SCANI128x32(_ov_,_sv_,cv); _mm_storeu_si128(_op_++, _sv_);

#define VEXP0(_i_, _ov_)        if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_shuffle_epi8(_mm_loadu_si128((__m128i*)pex),_mm_load_si128((__m128i*)shuffles[m]) ); pex += popcnt32(m)
#define VSTO0(_op_, _i_, _ov_, _sv_)   VEXP0( _i_, _ov_); SCANI128x32(_ov_,_sv_,cv); _mm_storeu_si128(_op_++, _sv_);

#include "bitunpack_.h"

#define BITUNPACK0(_parm_) mv = _mm_set1_epi32(0) //_parm_ = _mm_setzero_si128()

unsigned char *_bitd1unpack128v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) {
  const unsigned char *ip = in+PAD8(128*b); unsigned m;
  __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4,3,2,1);
  BITUNPACK128V32(in, b, out, sv);
  return (unsigned char *)ip; 
}
  #endif
#endif // __SSE2__
 
#if defined(__AVX2__) && defined(AVX2_ON)  
#include <immintrin.h>

  #ifdef __AVX512F__
#define mm256_maskz_expand_epi32(_m_,_v_) _mm256_maskz_expand_epi32(_m_,_v_)
#define mm256_maskz_loadu_epi32( _m_,_v_) _mm256_maskz_loadu_epi32( _m_,_v_)
  #else
static unsigned char permv[256][8] __attribute__((aligned(32))) = {
0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,1,
1,0,1,1,1,1,1,1,
0,1,2,2,2,2,2,2,
1,1,0,1,1,1,1,1,
0,2,1,2,2,2,2,2,
2,0,1,2,2,2,2,2,
0,1,2,3,3,3,3,3,
1,1,1,0,1,1,1,1,
0,2,2,1,2,2,2,2,
2,0,2,1,2,2,2,2,
0,1,3,2,3,3,3,3,
2,2,0,1,2,2,2,2,
0,3,1,2,3,3,3,3,
3,0,1,2,3,3,3,3,
0,1,2,3,4,4,4,4,
1,1,1,1,0,1,1,1,
0,2,2,2,1,2,2,2,
2,0,2,2,1,2,2,2,
0,1,3,3,2,3,3,3,
2,2,0,2,1,2,2,2,
0,3,1,3,2,3,3,3,
3,0,1,3,2,3,3,3,
0,1,2,4,3,4,4,4,
2,2,2,0,1,2,2,2,
0,3,3,1,2,3,3,3,
3,0,3,1,2,3,3,3,
0,1,4,2,3,4,4,4,
3,3,0,1,2,3,3,3,
0,4,1,2,3,4,4,4,
4,0,1,2,3,4,4,4,
0,1,2,3,4,5,5,5,
1,1,1,1,1,0,1,1,
0,2,2,2,2,1,2,2,
2,0,2,2,2,1,2,2,
0,1,3,3,3,2,3,3,
2,2,0,2,2,1,2,2,
0,3,1,3,3,2,3,3,
3,0,1,3,3,2,3,3,
0,1,2,4,4,3,4,4,
2,2,2,0,2,1,2,2,
0,3,3,1,3,2,3,3,
3,0,3,1,3,2,3,3,
0,1,4,2,4,3,4,4,
3,3,0,1,3,2,3,3,
0,4,1,2,4,3,4,4,
4,0,1,2,4,3,4,4,
0,1,2,3,5,4,5,5,
2,2,2,2,0,1,2,2,
0,3,3,3,1,2,3,3,
3,0,3,3,1,2,3,3,
0,1,4,4,2,3,4,4,
3,3,0,3,1,2,3,3,
0,4,1,4,2,3,4,4,
4,0,1,4,2,3,4,4,
0,1,2,5,3,4,5,5,
3,3,3,0,1,2,3,3,
0,4,4,1,2,3,4,4,
4,0,4,1,2,3,4,4,
0,1,5,2,3,4,5,5,
4,4,0,1,2,3,4,4,
0,5,1,2,3,4,5,5,
5,0,1,2,3,4,5,5,
0,1,2,3,4,5,6,6,
1,1,1,1,1,1,0,1,
0,2,2,2,2,2,1,2,
2,0,2,2,2,2,1,2,
0,1,3,3,3,3,2,3,
2,2,0,2,2,2,1,2,
0,3,1,3,3,3,2,3,
3,0,1,3,3,3,2,3,
0,1,2,4,4,4,3,4,
2,2,2,0,2,2,1,2,
0,3,3,1,3,3,2,3,
3,0,3,1,3,3,2,3,
0,1,4,2,4,4,3,4,
3,3,0,1,3,3,2,3,
0,4,1,2,4,4,3,4,
4,0,1,2,4,4,3,4,
0,1,2,3,5,5,4,5,
2,2,2,2,0,2,1,2,
0,3,3,3,1,3,2,3,
3,0,3,3,1,3,2,3,
0,1,4,4,2,4,3,4,
3,3,0,3,1,3,2,3,
0,4,1,4,2,4,3,4,
4,0,1,4,2,4,3,4,
0,1,2,5,3,5,4,5,
3,3,3,0,1,3,2,3,
0,4,4,1,2,4,3,4,
4,0,4,1,2,4,3,4,
0,1,5,2,3,5,4,5,
4,4,0,1,2,4,3,4,
0,5,1,2,3,5,4,5,
5,0,1,2,3,5,4,5,
0,1,2,3,4,6,5,6,
2,2,2,2,2,0,1,2,
0,3,3,3,3,1,2,3,
3,0,3,3,3,1,2,3,
0,1,4,4,4,2,3,4,
3,3,0,3,3,1,2,3,
0,4,1,4,4,2,3,4,
4,0,1,4,4,2,3,4,
0,1,2,5,5,3,4,5,
3,3,3,0,3,1,2,3,
0,4,4,1,4,2,3,4,
4,0,4,1,4,2,3,4,
0,1,5,2,5,3,4,5,
4,4,0,1,4,2,3,4,
0,5,1,2,5,3,4,5,
5,0,1,2,5,3,4,5,
0,1,2,3,6,4,5,6,
3,3,3,3,0,1,2,3,
0,4,4,4,1,2,3,4,
4,0,4,4,1,2,3,4,
0,1,5,5,2,3,4,5,
4,4,0,4,1,2,3,4,
0,5,1,5,2,3,4,5,
5,0,1,5,2,3,4,5,
0,1,2,6,3,4,5,6,
4,4,4,0,1,2,3,4,
0,5,5,1,2,3,4,5,
5,0,5,1,2,3,4,5,
0,1,6,2,3,4,5,6,
5,5,0,1,2,3,4,5,
0,6,1,2,3,4,5,6,
6,0,1,2,3,4,5,6,
0,1,2,3,4,5,6,7,
1,1,1,1,1,1,1,0,
0,2,2,2,2,2,2,1,
2,0,2,2,2,2,2,1,
0,1,3,3,3,3,3,2,
2,2,0,2,2,2,2,1,
0,3,1,3,3,3,3,2,
3,0,1,3,3,3,3,2,
0,1,2,4,4,4,4,3,
2,2,2,0,2,2,2,1,
0,3,3,1,3,3,3,2,
3,0,3,1,3,3,3,2,
0,1,4,2,4,4,4,3,
3,3,0,1,3,3,3,2,
0,4,1,2,4,4,4,3,
4,0,1,2,4,4,4,3,
0,1,2,3,5,5,5,4,
2,2,2,2,0,2,2,1,
0,3,3,3,1,3,3,2,
3,0,3,3,1,3,3,2,
0,1,4,4,2,4,4,3,
3,3,0,3,1,3,3,2,
0,4,1,4,2,4,4,3,
4,0,1,4,2,4,4,3,
0,1,2,5,3,5,5,4,
3,3,3,0,1,3,3,2,
0,4,4,1,2,4,4,3,
4,0,4,1,2,4,4,3,
0,1,5,2,3,5,5,4,
4,4,0,1,2,4,4,3,
0,5,1,2,3,5,5,4,
5,0,1,2,3,5,5,4,
0,1,2,3,4,6,6,5,
2,2,2,2,2,0,2,1,
0,3,3,3,3,1,3,2,
3,0,3,3,3,1,3,2,
0,1,4,4,4,2,4,3,
3,3,0,3,3,1,3,2,
0,4,1,4,4,2,4,3,
4,0,1,4,4,2,4,3,
0,1,2,5,5,3,5,4,
3,3,3,0,3,1,3,2,
0,4,4,1,4,2,4,3,
4,0,4,1,4,2,4,3,
0,1,5,2,5,3,5,4,
4,4,0,1,4,2,4,3,
0,5,1,2,5,3,5,4,
5,0,1,2,5,3,5,4,
0,1,2,3,6,4,6,5,
3,3,3,3,0,1,3,2,
0,4,4,4,1,2,4,3,
4,0,4,4,1,2,4,3,
0,1,5,5,2,3,5,4,
4,4,0,4,1,2,4,3,
0,5,1,5,2,3,5,4,
5,0,1,5,2,3,5,4,
0,1,2,6,3,4,6,5,
4,4,4,0,1,2,4,3,
0,5,5,1,2,3,5,4,
5,0,5,1,2,3,5,4,
0,1,6,2,3,4,6,5,
5,5,0,1,2,3,5,4,
0,6,1,2,3,4,6,5,
6,0,1,2,3,4,6,5,
0,1,2,3,4,5,7,6,
2,2,2,2,2,2,0,1,
0,3,3,3,3,3,1,2,
3,0,3,3,3,3,1,2,
0,1,4,4,4,4,2,3,
3,3,0,3,3,3,1,2,
0,4,1,4,4,4,2,3,
4,0,1,4,4,4,2,3,
0,1,2,5,5,5,3,4,
3,3,3,0,3,3,1,2,
0,4,4,1,4,4,2,3,
4,0,4,1,4,4,2,3,
0,1,5,2,5,5,3,4,
4,4,0,1,4,4,2,3,
0,5,1,2,5,5,3,4,
5,0,1,2,5,5,3,4,
0,1,2,3,6,6,4,5,
3,3,3,3,0,3,1,2,
0,4,4,4,1,4,2,3,
4,0,4,4,1,4,2,3,
0,1,5,5,2,5,3,4,
4,4,0,4,1,4,2,3,
0,5,1,5,2,5,3,4,
5,0,1,5,2,5,3,4,
0,1,2,6,3,6,4,5,
4,4,4,0,1,4,2,3,
0,5,5,1,2,5,3,4,
5,0,5,1,2,5,3,4,
0,1,6,2,3,6,4,5,
5,5,0,1,2,5,3,4,
0,6,1,2,3,6,4,5,
6,0,1,2,3,6,4,5,
0,1,2,3,4,7,5,6,
3,3,3,3,3,0,1,2,
0,4,4,4,4,1,2,3,
4,0,4,4,4,1,2,3,
0,1,5,5,5,2,3,4,
4,4,0,4,4,1,2,3,
0,5,1,5,5,2,3,4,
5,0,1,5,5,2,3,4,
0,1,2,6,6,3,4,5,
4,4,4,0,4,1,2,3,
0,5,5,1,5,2,3,4,
5,0,5,1,5,2,3,4,
0,1,6,2,6,3,4,5,
5,5,0,1,5,2,3,4,
0,6,1,2,6,3,4,5,
6,0,1,2,6,3,4,5,
0,1,2,3,7,4,5,6,
4,4,4,4,0,1,2,3,
0,5,5,5,1,2,3,4,
5,0,5,5,1,2,3,4,
0,1,6,6,2,3,4,5,
5,5,0,5,1,2,3,4,
0,6,1,6,2,3,4,5,
6,0,1,6,2,3,4,5,
0,1,2,7,3,4,5,6,
5,5,5,0,1,2,3,4,
0,6,6,1,2,3,4,5,
6,0,6,1,2,3,4,5,
0,1,7,2,3,4,5,6,
6,6,0,1,2,3,4,5,
0,7,1,2,3,4,5,6,
7,0,1,2,3,4,5,6,
0,1,2,3,4,5,6,7
};
#define u2vmask(_m_,_tv_)                  _mm256_sllv_epi32(_mm256_set1_epi8(_m_), _tv_)           
#define mm256_maskz_expand_epi32(_m_, _v_) _mm256_permutevar8x32_epi32(_v_,  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(ctou64(permv[_m_]))) )
#define mm256_maskz_loadu_epi32(_m_,_v_)   _mm256_blendv_epi8(zv, mm256_maskz_expand_epi32(xm, _mm256_loadu_si256((__m256i*)pex)), u2vmask(xm,tv))
  #endif

//-----------------------------------------------------------------------------
#define VSTO( _op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, ov)
#define VSTO0(_op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, _parm_)
#include "bitunpack_.h"

#define BITUNBLK256V32_0(ip, _i_, _op_, _parm_) {__m256i ov;\
  VSTO0(_op_,  0, ov, _parm_);\
  VSTO0(_op_,  1, ov, _parm_);\
  VSTO0(_op_,  2, ov, _parm_);\
  VSTO0(_op_,  3, ov, _parm_);\
  VSTO0(_op_,  4, ov, _parm_);\
  VSTO0(_op_,  5, ov, _parm_);\
  VSTO0(_op_,  6, ov, _parm_);\
  VSTO0(_op_,  7, ov, _parm_);\
  VSTO0(_op_,  8, ov, _parm_);\
  VSTO0(_op_,  9, ov, _parm_);\
  VSTO0(_op_, 10, ov, _parm_);\
  VSTO0(_op_, 11, ov, _parm_);\
  VSTO0(_op_, 12, ov, _parm_);\
  VSTO0(_op_, 13, ov, _parm_);\
  VSTO0(_op_, 14, ov, _parm_);\
  VSTO0(_op_, 15, ov, _parm_);\
  VSTO0(_op_, 16, ov, _parm_);\
  VSTO0(_op_, 17, ov, _parm_);\
  VSTO0(_op_, 18, ov, _parm_);\
  VSTO0(_op_, 19, ov, _parm_);\
  VSTO0(_op_, 20, ov, _parm_);\
  VSTO0(_op_, 21, ov, _parm_);\
  VSTO0(_op_, 22, ov, _parm_);\
  VSTO0(_op_, 23, ov, _parm_);\
  VSTO0(_op_, 24, ov, _parm_);\
  VSTO0(_op_, 25, ov, _parm_);\
  VSTO0(_op_, 26, ov, _parm_);\
  VSTO0(_op_, 27, ov, _parm_);\
  VSTO0(_op_, 28, ov, _parm_);\
  VSTO0(_op_, 29, ov, _parm_);\
  VSTO0(_op_, 30, ov, _parm_);\
  VSTO0(_op_, 31, ov, _parm_);\
}  
#define BITUNPACK0(_parm_) _parm_ = _mm256_setzero_si256()

unsigned char *bitunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b) {
  const unsigned char *ip = in+PAD8(256*b);
  __m256i sv; 
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}    

//--------------------------------------- zeromask unpack for TurboPFor vp4d.c --------------------------------------
#define VSTO(_op_, _i_, _ov_, _parm_)  xm = *bb++; _mm256_storeu_si256(_op_++, _mm256_add_epi32(_ov_, _mm256_slli_epi32(mm256_maskz_loadu_epi32(xm,(__m256i*)pex), b) )); pex += popcnt32(xm)
#define VSTO0(_op_, _i_, _ov_, _parm_) xm = *bb++; _mm256_storeu_si256(_op_++,                                          mm256_maskz_loadu_epi32(xm,(__m256i*)pex) );      pex += popcnt32(xm)
#define BITUNPACK0(_parm_)
#include "bitunpack_.h" 
unsigned char *_bitunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb) {
  const unsigned char *ip = in+PAD8(256*b); unsigned xm; __m256i sv, zv = _mm256_setzero_si256(), tv = _mm256_set_epi32(0,1,2,3,4,5,6,7);
  BITUNPACK256V32(in, b, out, sv); 
  return (unsigned char *)ip; 
}     

#define VSTO0(_op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, _parm_)
#define VSTO(_op_, i, _ov_, __sv) _ov_ = UNZIGZAG256x32(_ov_); SCAN256x32(_ov_,__sv); _mm256_storeu_si256(_op_++, __sv)
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *bitzunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(256*b); 
  __m256i sv = _mm256_set1_epi32(start), zv = _mm256_setzero_si256(); 
  BITUNPACK256V32(in, b, out, sv); 
  return (unsigned char *)ip; 
} 
 
#define VSTO(_op_, i, _ov_, __sv) SCAN256x32(_ov_,__sv); _mm256_storeu_si256(_op_++, __sv)
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(256*b); 
  __m256i sv = _mm256_set1_epi32(start), zv = _mm256_setzero_si256();
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

#define VSTO( _op_, _i_, _ov_, _parm_) _mm256_storeu_si256(_op_++, _mm256_add_epi32(_ov_, sv))
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *bitfunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(256*b); 
  __m256i sv = _mm256_set1_epi32(start);
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

//-----------------------------------------------------------------------------
#define VEXP(_i_, _ov_)  xm = *bb++; _ov_ = _mm256_add_epi32(_ov_, _mm256_slli_epi32(mm256_maskz_loadu_epi32(xm,(__m256i*)pex), b) ); pex += popcnt32(xm)
#define VEXP0(_i_, _ov_) xm = *bb++; _ov_ =                                          mm256_maskz_loadu_epi32(xm,(__m256i*)pex);       pex += popcnt32(xm)
 
#define VSTO( _op_, _i_, _ov_, _sv_) VEXP( _i_, _ov_); SCAN256x32(_ov_,_sv_); _mm256_storeu_si256(_op_++, _sv_);
#define VSTO0(_op_, _i_, _ov_, _sv_) VEXP0(_i_, _ov_); SCAN256x32(_ov_,_sv_); _mm256_storeu_si256(_op_++, _sv_);
#include "bitunpack_.h"
#define BITUNPACK0(_parm_)
unsigned char *_bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) { 
  const unsigned char *ip = in+PAD8(256*b); unsigned xm;
  __m256i sv = _mm256_set1_epi32(start),zv = _mm256_setzero_si256(), tv = _mm256_set_epi32(0,1,2,3,4,5,6,7);
  BITUNPACK256V32(in, b, out, sv); 
  return (unsigned char *)ip; 
} 

#define VSTO(_op_, i, _ov_, __sv)    SCANI256x32(_ov_,__sv,cv); _mm256_storeu_si256(_op_++, __sv);
#define VSTO0(_op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, _parm_); _parm_ = _mm256_add_epi32(_parm_, cv)
#include "bitunpack_.h"
#define BITUNPACK0(_parm_) _parm_ = _mm256_add_epi32(_parm_, cv); cv = _mm256_set1_epi32(8)
unsigned char *bitd1unpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(256*b);
  __m256i sv = _mm256_set1_epi32(start), cv = _mm256_set_epi32(8,7,6,5,4,3,2,1),zv = _mm256_setzero_si256();
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

#define VSTO( _op_, _i_, _ov_, _sv_) _mm256_storeu_si256(_op_++, _mm256_add_epi32(_ov_, _sv_)); _sv_ = _mm256_add_epi32(_sv_, cv)
#define VSTO0(_op_, _i_, ov, _sv_)   _mm256_storeu_si256(_op_++, _sv_);                         _sv_ = _mm256_add_epi32(_sv_, cv); 
#include "bitunpack_.h"
#define BITUNPACK0(_parm_) 
unsigned char *bitf1unpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(256*b); 
  __m256i sv = _mm256_set_epi32(start+8,start+7,start+6,start+5,start+4,start+3,start+2,start+1), cv = _mm256_set1_epi32(8);
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}

#define VSTO( _op_, _i_, _ov_, _sv_)   VEXP( _i_, _ov_); SCANI256x32(_ov_,_sv_,cv); _mm256_storeu_si256(_op_++, _sv_);
#define VSTO0(_op_, _i_, _ov_, _sv_)   VEXP0(_i_, _ov_); SCANI256x32(_ov_,_sv_,cv); _mm256_storeu_si256(_op_++, _sv_);
#include "bitunpack_.h"
#define BITUNPACK0(_parm_) mv = _mm256_set1_epi32(0) //_parm_ = _mm_setzero_si128()
unsigned char *_bitd1unpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) {
  const unsigned char *ip = in+PAD8(256*b); unsigned xm;
  __m256i sv = _mm256_set1_epi32(start), cv = _mm256_set_epi32(8,7,6,5,4,3,2,1),zv = _mm256_setzero_si256(),tv = _mm256_set_epi32(0,1,2,3,4,5,6,7);
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}
#endif

#pragma clang diagnostic pop
#pragma GCC pop_options
