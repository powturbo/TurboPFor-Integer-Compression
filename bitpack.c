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
#define PAD8(_x_) ( (((_x_)+8-1)/8) )

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
   
#define PREFETCH(_ip_) __builtin_prefetch(_ip_+768)//#define PREFETCH(ip)

#if 0
#define IP( _ip_,_x_) *_ip_++
#define IPI(_ip_)
#else
#define IP( _ip_,_x_) _ip_[_x_]
#define IPI(_ip_) _ip_ += 32
#endif

#define IPB(_ip_,_x_, _parm_)
#define IPW(_ip_,_x_) 			v

#define IPV(_ip_,_x_) 			IP(_ip_,_x_)
#define IPX(_ip_,_x_)  		   (v = IP(_ip_,_x_))
#define IPP(_ip_,_x_, _parm_)
#define _BITPACK_ bitpack
#include "bitpack_.h"

#define DELTA
 
#define IPB(_ip_,_x_, _parm_)   v = IP(_ip_,_x_) - start; start = IP(_ip_,_x_)
#define IPV(_ip_,_x_) 		    v
#define IPX(_ip_,_x_) 		   (v = IP(_ip_,_x_) - start)
#define IPP(_ip_,_x_, _parm_)   start = IP(_ip_,_x_) 	
#define _BITPACK_ bitdpack
#include "bitpack_.h"

#define IPB(_ip_,_x_, _parm_)    
#define IPV(_ip_,_x_) 			IP(_ip_,_x_) - start
#define IPX(_ip_,_x_)  		    (v = IP(_ip_,_x_) - start)
#define IPP(_ip_,_x_, _parm_)
#define _BITPACK_ bitfpack
#include "bitpack_.h"

#define IPB( _ip_,_x_, _parm_) 	v = IP(_ip_,_x_) - start - 1; start = IP(_ip_,_x_)
#define IPV( _ip_,_x_) 			v
#define IPX(_ip_,_x_) 		   (v = IP(_ip_,_x_) - start - 1)
#define IPP(_ip_,_x_, _parm_)   start = IP(_ip_,_x_)
#define _BITPACK_ bitd1pack
#include "bitpack_.h"

#define IPB(_ip_,_x_, _parm_) 	v = zigzagenc32(IP(_ip_,_x_) - start)
#define IPV(_ip_,_x_) 			v
#define IPX(_ip_,_x_) 		   (v = zigzagenc32(IP(_ip_,_x_) - start))
#define IPP(_ip_,_x_, _parm_) 	start = IP(_ip_,_x_)
#define _BITPACK_ bitzpack
#include "bitpack_.h"

#define IPI(_ip_) _ip_ += 32; start += 32
#define IPB(_ip_,_x_, _parm_)    
#define IPV(_ip_,_x_) 			(IP(_ip_,_x_) - start - (_x_) - 1)
#define IPX(_ip_,_x_)  		    (v = IP(_ip_,_x_) - start - (_x_) - 1)
#define IPP(_ip_,_x_, _parm_)
#define _BITPACK_ bitf1pack
#include "bitpack_.h"
#undef IPI

#define BITNPACK(in, n, out, csize, usize) { ip=in;\
  /*if(usize <= 32)\
    for(; ip < in+(n&~(csize*4-1)); ) { __builtin_prefetch(ip+512); unsigned char *p=ip; unsigned u,b;\
      TEMPLATE2(BITSIZE,usize)(ip, csize, b); u  = b;     out = TEMPLATE2(bitpacka, usize)[b](ip, csize, out); ip+=csize;\
      TEMPLATE2(BITSIZE,usize)(ip, csize, b); u |= b<<6;  out = TEMPLATE2(bitpacka, usize)[b](ip, csize, out); ip+=csize;\
      TEMPLATE2(BITSIZE,usize)(ip, csize, b); u |= b<<12; out = TEMPLATE2(bitpacka, usize)[b](ip, csize, out); ip+=csize;\
      TEMPLATE2(BITSIZE,usize)(ip, csize, b); u |= b<<18; out = TEMPLATE2(bitpacka, usize)[b](ip, csize, out); ip+=csize;\
      ctou32(p) = p[3]<<24 | u&0xffffff;\
    }*/\
  for(in+=n; ip < in;) { unsigned iplen = in - ip; if(iplen > csize) iplen = csize;		__builtin_prefetch(ip+512);\
    unsigned b; TEMPLATE2(BITSIZE,usize)(ip, csize, b); *out++ = b; out = TEMPLATE2(bitpacka, usize)[b](ip, csize, out); \
	ip += csize;\
  } return out;\
}

#define BITNDPACK(in, n, out, csize, usize, _start_, _bitd_, _bitpacka_) {\
  for(ip = in,in+=n; ip < in;) { unsigned iplen = in - ip; if(iplen > csize) iplen = csize;		__builtin_prefetch(ip+512);\
    typeof(in[0]) _in[csize+8];\
    unsigned b = TEMPLATE2(_bitd_, usize)(ip, csize, _start_);\
    *out++ = b; out = TEMPLATE2(_bitpacka_, usize)[b](ip, csize, out, _start_);\
	ip += csize;\
    start = ip[-1];\
  } return out;\
}

unsigned char *bitnpack8(   uint8_t  *__restrict in, size_t n, unsigned char *__restrict out) { uint8_t  *ip; BITNPACK(in, n, out, 128,  8); } 
unsigned char *bitnpack16(  uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip; BITNPACK(in, n, out, 128, 16); } 
unsigned char *bitnpack32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip; BITNPACK(in, n, out, 128, 32); } 
unsigned char *bitnpack64(  uint64_t *__restrict in, size_t n, unsigned char *__restrict out) { uint64_t *ip; BITNPACK(in, n, out, 128, 64); }

unsigned char *bitndpack8(  uint8_t  *__restrict in, size_t n, unsigned char *__restrict out, uint8_t  start) { uint8_t  *ip; BITNDPACK(in, n, out, 128,  8, start, bitd, bitdpacka); } 
unsigned char *bitndpack16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out, uint16_t start) { uint16_t *ip; BITNDPACK(in, n, out, 128, 16, start, bitd, bitdpacka); } 
unsigned char *bitndpack32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out, uint32_t start) { uint32_t *ip; BITNDPACK(in, n, out, 128, 32, start, bitd, bitdpacka); } 
unsigned char *bitndpack64( uint64_t *__restrict in, size_t n, unsigned char *__restrict out, uint64_t start) { uint64_t *ip; BITNDPACK(in, n, out, 128, 64, start, bitd, bitdpacka); }

unsigned char *bitnd1pack8( uint8_t  *__restrict in, size_t n, unsigned char *__restrict out, uint8_t  start) { uint8_t  *ip; BITNDPACK(in, n, out, 128,  8, start, bitd1, bitd1packa); } 
unsigned char *bitnd1pack16(uint16_t *__restrict in, size_t n, unsigned char *__restrict out, uint16_t start) { uint16_t *ip; BITNDPACK(in, n, out, 128, 16, start, bitd1, bitd1packa); } 
unsigned char *bitnd1pack32(uint32_t *__restrict in, size_t n, unsigned char *__restrict out, uint32_t start) { uint32_t *ip; BITNDPACK(in, n, out, 128, 32, start, bitd1, bitd1packa); } 
unsigned char *bitnd1pack64(uint64_t *__restrict in, size_t n, unsigned char *__restrict out, uint64_t start) { uint64_t *ip; BITNDPACK(in, n, out, 128, 64, start, bitd1, bitd1packa); }

//----------------------------------------------------------------------------------------------------------------------------------
#ifdef __SSE2__
#include <emmintrin.h>

#define OPPE(__op)
#define IPPE(__op)

#define PAD8(__x) (((__x)+8-1)/8)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm_loadu_si128(ip++)
#include "bitpack_.h" 
  
unsigned char *bitpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(128*b); BITPACK128V32(in, b, out, 0); return pout; }
#undef VSTI 
#undef IPP

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = DELTA128x32(v,__sv); __sv = v
#define IPP(ip, i, __iv) __iv
#include "bitpack_.h" 

unsigned char *bitdpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v,sv = _mm_set1_epi32(start);
  BITPACK128V32(in, b, out, sv); 
  return pout;
}
#undef VSTI

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = _mm_sub_epi32(DELTA128x32(v,__sv),cv); __sv = v

unsigned char *bitd1pack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACK128V32(in, b, out, sv); return pout; 
}
#undef VSTI
//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = DELTA128x32(v,__sv); __sv = v; __iv = ZIGZAG128x32(__iv)

unsigned char *bitzpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACK128V32(in, b, out, sv); 
  return pout; 
}
#undef VSTI
#undef IPP
#endif

#ifdef __AVX2__
#include <immintrin.h>

#define OPPE(__op)
#define IPPE(__op)

#define PAD8(__x) (((__x)+8-1)/8)
#define OPPE(__op)
#define IPPE(__op)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm256_loadu_si256(ip++)
#include "bitpack_.h" 
//#include "bitpack.h"
//#include "bitutil.h"
 
unsigned char *bitpack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(256*b); BITPACK256V32(in, b, out, 0); return pout; }
#undef VSTI 
#undef IPP

//------------------------------------------------------------------------------------------------------------------------------
#if 0
#define VSTI(__ip, __i, __iv, __sv) v = _mm256_loadu_si256(__ip++); DELTA256x32(v,__sv, __iv) //__sv = v
#define IPP(ip, i, __iv) __iv
#include "bitpack_.h"  

unsigned char *bitdpack256v32(unsigned       *__restrict in, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v; //,sv = _mm256_set1_epi32(start),zv = _mm256_setzero_si256();
  __m128i sv = _mm_set1_epi32(start);
  BITPACK256V32(in, b, out, sv);
  return pout;
}
#undef VSTI

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm256_loadu_si256(__ip++); __iv = _mm256_sub_epi32(DELTA256x32(v,__sv),cv); __sv = v

unsigned char *bitd1pack256v32(unsigned       *__restrict in, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v, sv = _mm256_set1_epi32(start), cv = _mm256_set1_epi32(1);
  //BITPACK256V32(in, b, out, sv); return pout; 
}
#undef VSTI
//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm256_loadu_si256(__ip++); __iv = DELTA256x32(v,__sv); __sv = v; __iv = ZIGZAG256x32(__iv)

unsigned char *bitzpack256v32(unsigned       *__restrict in, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(256*b);
  __m256i v, sv = _mm256_set1_epi32(start), cv = _mm256_set1_epi32(1);
  //BITPACK256V32(in, b, out, sv); 
  return pout; 
}
#endif
#undef VSTI
#endif

#pragma clang diagnostic pop
