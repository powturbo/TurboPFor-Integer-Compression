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
//    "Integer Compression" SIMD bit packing 
  #ifndef VSTI
#include <emmintrin.h>
#include "bitpack.h"
#include "bitutil.h"

#define PAD8(__x) (((__x)+8-1)/8)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm_loadu_si128(ip++)
#include __FILE__ 
  
unsigned char *bitpack128v32(unsigned       *__restrict in, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(128*b); BITPACK128V32(in, b, out, 0); return pout; }
//unsigned char *bitpack128v16(unsigned short *__restrict in, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(n*b); BITPACK128V32(in, n, b, out, 0); return pout; }
#undef VSTI 
#undef IPP

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = DELTA128x32(v,__sv); __sv = v
#define IPP(ip, i, __iv) __iv
#include __FILE__ 

unsigned char *bitdpack128v32(unsigned       *__restrict in, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v,sv = _mm_set1_epi32(start);
  BITPACK128V32(in, b, out, sv); 
  return pout;
}
#undef VSTI

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = _mm_sub_epi32(DELTA128x32(v,__sv),cv); __sv = v

unsigned char *bitd1pack128v32(unsigned       *__restrict in, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACK128V32(in, b, out, sv); return pout; 
}
#undef VSTI
//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = DELTA128x32(v,__sv); __sv = v; __iv = ZIGZAG128x32(__iv)

unsigned char *bitzpack128v32(unsigned       *__restrict in, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(128*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACK128V32(in, b, out, sv); 
  return pout; 
}
#undef VSTI
  #else
#include <strings.h>
#include <emmintrin.h>

#define OPPE(__op)
#define IPPE(__op)

#include "bitpack128v_.h"
 
#define BITPACK128V32(__pip, __nbits, __pop, __parm) { __m128i *__ip=(__m128i *)__pip,*__op=(__m128i *)__pop;\
  switch(__nbits) {\
    case  0:  										   break;\
    case  1:{  BITPACK128V32_1( __ip, __op, __parm); } break;\
    case  2:{  BITPACK128V32_2( __ip, __op, __parm); } break;\
    case  3:{  BITPACK128V32_3( __ip, __op, __parm); } break;\
    case  4:{  BITPACK128V32_4( __ip, __op, __parm); } break;\
    case  5:{  BITPACK128V32_5( __ip, __op, __parm); } break;\
    case  6:{  BITPACK128V32_6( __ip, __op, __parm); } break;\
    case  7:{  BITPACK128V32_7( __ip, __op, __parm); } break;\
    case  8:{  BITPACK128V32_8( __ip, __op, __parm); } break;\
    case  9:{  BITPACK128V32_9( __ip, __op, __parm); } break;\
    case 10:{  BITPACK128V32_10(__ip, __op, __parm); } break;\
    case 11:{  BITPACK128V32_11(__ip, __op, __parm); } break;\
    case 12:{  BITPACK128V32_12(__ip, __op, __parm); } break;\
    case 13:{  BITPACK128V32_13(__ip, __op, __parm); } break;\
    case 14:{  BITPACK128V32_14(__ip, __op, __parm); } break;\
    case 15:{  BITPACK128V32_15(__ip, __op, __parm); } break;\
    case 16:{  BITPACK128V32_16(__ip, __op, __parm); } break;\
    case 17:{  BITPACK128V32_17(__ip, __op, __parm); } break;\
    case 18:{  BITPACK128V32_18(__ip, __op, __parm); } break;\
    case 19:{  BITPACK128V32_19(__ip, __op, __parm); } break;\
    case 20:{  BITPACK128V32_20(__ip, __op, __parm); } break;\
    case 21:{  BITPACK128V32_21(__ip, __op, __parm); } break;\
    case 22:{  BITPACK128V32_22(__ip, __op, __parm); } break;\
    case 23:{  BITPACK128V32_23(__ip, __op, __parm); } break;\
    case 24:{  BITPACK128V32_24(__ip, __op, __parm); } break;\
    case 25:{  BITPACK128V32_25(__ip, __op, __parm); } break;\
    case 26:{  BITPACK128V32_26(__ip, __op, __parm); } break;\
    case 27:{  BITPACK128V32_27(__ip, __op, __parm); } break;\
    case 28:{  BITPACK128V32_28(__ip, __op, __parm); } break;\
    case 29:{  BITPACK128V32_29(__ip, __op, __parm); } break;\
    case 30:{  BITPACK128V32_30(__ip, __op, __parm); } break;\
    case 31:{  BITPACK128V32_31(__ip, __op, __parm); } break;\
    case 32:{  BITPACK128V32_32(__ip, __op, __parm); } break;\
  }\
}
  #endif
