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
//  "Integer Compression" bit packing 

#include <stdio.h>
#include "bitpack.h"
#include "bitutil.h" 
#include "vint.h"
#define PAD8(_x_) ( (((_x_)+8-1)/8) )

#pragma warning( disable : 4005) 
#pragma warning( disable : 4090) 
#pragma warning( disable : 4068) 
#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wunsequenced"

typedef unsigned char *(*BITPACK_F8)( uint8_t  *__restrict out, unsigned n, const unsigned char *__restrict in);
typedef unsigned char *(*BITPACK_D8)( uint8_t  *__restrict out, unsigned n, const unsigned char *__restrict in, uint8_t start);
typedef unsigned char *(*BITPACK_F16)(uint16_t *__restrict out, unsigned n, const unsigned char *__restrict in);
typedef unsigned char *(*BITPACK_D16)(uint16_t *__restrict out, unsigned n, const unsigned char *__restrict in, uint16_t start);
typedef unsigned char *(*BITPACK_F32)(uint32_t *__restrict out, unsigned n, const unsigned char *__restrict in);
typedef unsigned char *(*BITPACK_D32)(uint32_t *__restrict out, unsigned n, const unsigned char *__restrict in, uint32_t start);
typedef unsigned char *(*BITPACK_F64)(uint64_t *__restrict out, unsigned n, const unsigned char *__restrict in);
typedef unsigned char *(*BITPACK_D64)(uint64_t *__restrict out, unsigned n, const unsigned char *__restrict in, uint64_t start);
   
#define PREFETCH(_ip_) __builtin_prefetch(_ip_+768,0)//#define PREFETCH(ip)

#ifdef _MSC_VER
#define VX (v=x)
#define V  x 
#else
#define VX v
#define V  v 	
#endif

#if !defined(SSE2_ON) && !defined(AVX2_ON)
#if 0
#define IP0(_ip_,_x_) *_ip_
#define IP( _ip_,_x_) *_ip_++
#define IPI(_ip_)
#else
#define IP0(_ip_,_x_) _ip_[_x_]
#define IP( _ip_,_x_) _ip_[_x_]
#define IPI(_ip_) _ip_ += 32
#endif

#define IPB(_ip_,_x_, _parm_)
#define IPW(_ip_,_x_) 		   	VX
#define IPX(_ip_,_x_)  		   	(V = IP(_ip_,_x_))

#define IPV(_ip_,_x_) 			IP(_ip_,_x_)
#define IPP(_ip_,_x_, _parm_)
#define _BITPACK_ bitpack
#include "bitpack_.h"
#undef IPB
#undef IPV
#undef IPX
#undef IPP

#define DELTA
 
#define IPB(_ip_,_x_, _parm_)   V = IP0(_ip_,_x_) - start; start = IP(_ip_,_x_)
#define IPV(_ip_,_x_) 		    VX
#define IPX(_ip_,_x_) 		   (V = IP(_ip_,_x_) - start)
#define IPP(_ip_,_x_, _parm_)   start = IP(_ip_,_x_) 	
#define _BITPACK_ bitdpack
#include "bitpack_.h"
#undef IPB
#undef IPV
#undef IPX
#undef IPP

#define IPB(_ip_,_x_, _parm_)    
#define IPV(_ip_,_x_) 			IP(_ip_,_x_) - start
#define IPX(_ip_,_x_)  		    (V = IP(_ip_,_x_) - start)
#define IPP(_ip_,_x_, _parm_)
#define _BITPACK_ bitfpack
#include "bitpack_.h"
#undef IPB
#undef IPV
#undef IPX
#undef IPP

#define IPB( _ip_,_x_, _parm_) 	V = IP0(_ip_,_x_) - start - 1; start = IP(_ip_,_x_)
#define IPV( _ip_,_x_) 			VX
#define IPX(_ip_,_x_) 		   (V = IP(_ip_,_x_) - start - 1)
#define IPP(_ip_,_x_, _parm_)   start = IP(_ip_,_x_)
#define _BITPACK_ bitd1pack
#include "bitpack_.h"
#undef IPB
#undef IPV
#undef IPX
#undef IPP

/*#define IPB( _ip_,_x_, _parm_) 	v = IP(_ip_,_x_) - start - mdelta; start = IP(_ip_,_x_)
#define IPV( _ip_,_x_) 			v
#define IPX(_ip_,_x_) 		   (v = IP(_ip_,_x_) - start - mdelta)
#define IPP(_ip_,_x_, _parm_)   start = IP(_ip_,_x_)
#define _BITPACK_ bitepack
#include "bitpack_.h"*/

#define IPB(_ip_,_x_, _parm_) 	V = zigzagenc32(IP(_ip_,_x_) - start); start = IP(_ip_,_x_)
#define IPV(_ip_,_x_) 			VX
#define IPX(_ip_,_x_) 		   (V = zigzagenc32(IP(_ip_,_x_) - start))
#define IPP(_ip_,_x_, _parm_) 	start = IP(_ip_,_x_)
#define _BITPACK_ bitzpack
#include "bitpack_.h"
#undef IPB
#undef IPV
#undef IPX
#undef IPP

#define IPI(_ip_) _ip_ += 32; start += 32
#define IPB(_ip_,_x_, _parm_)    
#define IPV(_ip_,_x_) 			(IP(_ip_,_x_) - start - (_x_) - 1)
#define IPX(_ip_,_x_)  		    (V = IP(_ip_,_x_) - start - (_x_) - 1)
#define IPP(_ip_,_x_, _parm_)
#define _BITPACK_ bitf1pack
#include "bitpack_.h"
#undef IPI
#undef IPB
#undef IPV
#undef IPX
#undef IPP

#define BITNPACK(in, n, out, csize, usize) { unsigned char *op = out;\
  for(ip = in, in += n; ip < in;) { \
    unsigned iplen = in - ip,b;\
    if(iplen > csize) iplen = csize;							PREFETCH(ip+512);\
    TEMPLATE2(BITSIZE,usize)(ip, iplen, b); /*if(b!=usize) printf("$%d ", b);*/\
	*op++ = b; \
	op = TEMPLATE2(bitpacka, usize)[b](ip, csize, op); \
	ip += iplen;\
  } \
  return op - out;\
}

#define BITNDPACK(in, n, out, csize, usize, _bitd_, _bitpacka_) {\
  unsigned char *op = out; \
  if(!n) return 0;\
  start = *in++; \
  TEMPLATE2(vbxput, usize)(op, start);\
  for(ip = in,--n, in += n; ip < in;) { \
    unsigned iplen = in - ip,b;\
    TEMPLATE3(uint, usize, _t) _in[csize+8];\
	if(iplen > csize) iplen = csize;								PREFETCH(ip+512);\
    b = TEMPLATE2(_bitd_, usize)(ip, iplen, start);\
    *op++ = b; op = TEMPLATE2(_bitpacka_, usize)[b](ip, csize, op, start);\
	ip += iplen;\
    start = ip[-1];\
  } \
  return op - out;\
}

size_t bitnpack8(   uint8_t  *__restrict in, size_t n, unsigned char *__restrict out) { uint8_t  *ip,start; BITNPACK(in, n, out, 128,  8); } 
size_t bitnpack16(  uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start; BITNPACK(in, n, out, 128, 16); } 
size_t bitnpack32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start; BITNPACK(in, n, out, 128, 32); } 
size_t bitnpack64(  uint64_t *__restrict in, size_t n, unsigned char *__restrict out) { uint64_t *ip,start; BITNPACK(in, n, out, 128, 64); }

size_t bitndpack8(  uint8_t  *__restrict in, size_t n, unsigned char *__restrict out) { uint8_t  *ip,start; BITNDPACK(in, n, out, 128,  8, bitd, bitdpacka); } 
size_t bitndpack16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start; BITNDPACK(in, n, out, 128, 16, bitd, bitdpacka); } 
size_t bitndpack32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start; BITNDPACK(in, n, out, 128, 32, bitd, bitdpacka); } 
size_t bitndpack64( uint64_t *__restrict in, size_t n, unsigned char *__restrict out) { uint64_t *ip,start; BITNDPACK(in, n, out, 128, 64, bitd, bitdpacka); }

size_t bitnd1pack8( uint8_t  *__restrict in, size_t n, unsigned char *__restrict out) { uint8_t  *ip,start; BITNDPACK(in, n, out, 128,  8, bitd1, bitd1packa); } 
size_t bitnd1pack16(uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start; BITNDPACK(in, n, out, 128, 16, bitd1, bitd1packa); } 
size_t bitnd1pack32(uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start; BITNDPACK(in, n, out, 128, 32, bitd1, bitd1packa); } 
size_t bitnd1pack64(uint64_t *__restrict in, size_t n, unsigned char *__restrict out) { uint64_t *ip,start; BITNDPACK(in, n, out, 128, 64, bitd1, bitd1packa); }

size_t bitnzpack8(  uint8_t  *__restrict in, size_t n, unsigned char *__restrict out) { uint8_t  *ip,start; BITNDPACK(in, n, out, 128,  8, bitz, bitzpacka); } 
size_t bitnzpack16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start; BITNDPACK(in, n, out, 128, 16, bitz, bitzpacka); } 
size_t bitnzpack32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start; BITNDPACK(in, n, out, 128, 32, bitz, bitzpacka); } 
size_t bitnzpack64( uint64_t *__restrict in, size_t n, unsigned char *__restrict out) { uint64_t *ip,start; BITNDPACK(in, n, out, 128, 64, bitz, bitzpacka); }
#endif

#if defined(__SSE2__) && defined(SSE2_ON)
#include <emmintrin.h>

#define OPPE(__op)
#define IPPE(__op)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm_loadu_si128(ip++)
#include "bitpack_.h" 
unsigned char *bitpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(128*b); BITPACK128V32(in, b, out, 0); return pout; }
unsigned char *bitpack256w32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *_out=out; unsigned *_in=in; 
BITPACK128V32(in, b, out, 0); in = _in+128; out = _out+PAD8(128*b); BITPACK128V32(in, b, out, 0); return _out+PAD8(256*b); }

#define VSTI(_ip_, _i_, _iv_, _sv_) v = _mm_loadu_si128(_ip_++); _iv_ = DELTA128x32(v,_sv_); _sv_ = v
#define IPP(ip, i, _iv_) _iv_
#include "bitpack_.h" 
unsigned char *bitdpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v,sv = _mm_set1_epi32(start);
  BITPACK128V32(in, b, out, sv); 
  return pout;
}

#define VSTI(_ip_, _i_, _iv_, _sv_) 
#define IPP(_ip_, i, _iv_) 			_mm_sub_epi32(_mm_loadu_si128(_ip_++),sv)
#include "bitpack_.h" 
unsigned char *bitfpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set1_epi32(start);
  BITPACK128V32(in, b, out, sv); 
  return pout;
}

#define VSTI(_ip_, _i_, _iv_, _sv_) v = _mm_loadu_si128(_ip_++); _iv_ = _mm_sub_epi32(DELTA128x32(v,_sv_),cv); _sv_ = v
#define IPP(ip, i, _iv_) _iv_
unsigned char *bitd1pack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACK128V32(in, b, out, sv); return pout; 
}

#define VSTI(_ip_, _i_, _iv_, _sv_) _iv_ = _mm_sub_epi32(_mm_loadu_si128(_ip_++),_sv_); _sv_ = _mm_add_epi32(_sv_,cv); 
#define IPP(ip, i, _iv_) _iv_
unsigned char *bitf1pack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set_epi32(start+4,start+3,start+2,start+1), cv = _mm_set1_epi32(4);
  BITPACK128V32(in, b, out, sv); return pout; 
}

#define VSTI(_ip_, _i_, _iv_, _sv_) v = _mm_loadu_si128(_ip_++); _iv_ = DELTA128x32(v,_sv_); _sv_ = v; _iv_ = ZIGZAG128x32(_iv_)
unsigned char *bitzpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACK128V32(in, b, out, sv); 
  return pout; 
}
#endif

#if defined(__AVX2__) && defined(AVX2_ON)
#include <immintrin.h>

#define OPPE(__op)
#define IPPE(__op)

#define PAD8(__x) (((__x)+8-1)/8)
#define OPPE(__op)
#define IPPE(__op)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm256_loadu_si256(ip++)
#include "bitpack_.h" 
 
unsigned char *bitpack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(256*b); BITPACK256V32(in, b, out, 0); return pout; }
#undef VSTI 
#undef IPP


#define VSTI(_ip_, _i_, _iv_, _sv_) _iv_ = _mm256_sub_epi32(_mm256_loadu_si256(_ip_++),sv)
#define IPP(_ip_, i, _iv_) 			_iv_
#include "bitpack_.h"  
unsigned char *bitfpack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v, sv = _mm256_set1_epi32(start);
  BITPACK256V32(in, b, out, sv); 
  return pout;
}

#define VSTI(_ip_, _i_, _iv_, _sv_) _iv_ = _mm256_sub_epi32(_mm256_loadu_si256(_ip_++),_sv_); _sv_ = _mm256_add_epi32(_sv_,cv); 
#define IPP(ip, i, _iv_) _iv_
unsigned char *bitf1pack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v, sv = _mm256_set_epi32(start+8,start+7,start+6,start+5,start+4,start+3,start+2,start+1), cv = _mm256_set1_epi32(8);
  BITPACK256V32(in, b, out, sv); return pout; 
}

#define VSTI(_ip_, _i_, _iv_, _sv_) v = _mm256_loadu_si256(_ip_++); _iv_ = DELTA256x32(v,_sv_); _sv_ = v
#define IPP(ip, i, _iv_) _iv_
#include "bitpack_.h" 
unsigned char *bitdpack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v,sv = _mm256_set1_epi32(start);
  BITPACK256V32(in, b, out, sv);
  return pout;
}

#define VSTI(_ip_, _i_, _iv_, _sv_) v = _mm256_loadu_si256(_ip_++); _iv_ = _mm256_sub_epi32(DELTA256x32(v,_sv_),cv); _sv_ = v
unsigned char *bitd1pack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v, sv = _mm256_set1_epi32(start), cv = _mm256_set1_epi32(1);
  BITPACK256V32(in, b, out, sv); 
  return pout; 
}

#define VSTI(_ip_, _i_, _iv_, _sv_) v = _mm256_loadu_si256(_ip_++); _iv_ = DELTA256x32(v,_sv_); _sv_ = v; _iv_ = ZIGZAG256x32(_iv_)
unsigned char *bitzpack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v, sv = _mm256_set1_epi32(start), cv = _mm256_set1_epi32(1);
  BITPACK256V32(in, b, out, sv); 
  return pout; 
}
#endif

#pragma clang diagnostic pop
