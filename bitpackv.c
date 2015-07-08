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
//    bitpackv.c - "Integer Compression" SIMD bit packing 
  #ifndef VSTI
#include <emmintrin.h>
#include "bitpack.h"
#include "bitutil.h"

#define PAD8(__x) (((__x)+8-1)/8)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm_loadu_si128(ip++)
#include __FILE__ 
  
unsigned char *bitpackv32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(n*b); BITPACKV32(in, n, b, out, 0); return pout; }
unsigned char *bitpackv16(unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(n*b); BITPACKV32(in, n, b, out, 0); return pout; }
#undef VSTI 
#undef IPP

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = DELTA128_32(v,__sv); __sv = v
#define IPP(ip, i, __iv) __iv
#include __FILE__ 

unsigned char *bitdpackv32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(n*b);
  __m128i v,sv = _mm_set1_epi32(start);
  BITPACKV32(in, n, b, out, sv); return pout; 
}
#undef VSTI

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = _mm_sub_epi32(DELTA128_32(v,__sv),cv); __sv = v

unsigned char *bitd1packv32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(n*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACKV32(in, n, b, out, sv); return pout; 
}
#undef VSTI
//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = DELTA128_32(v,__sv); __sv = v; __iv = ZIGZAG128_32(__iv)

unsigned char *bitzpackv32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start, unsigned b) { unsigned char *pout = out+PAD8(n*b);
  __m128i v, sv = _mm_set1_epi32(start), cv = _mm_set1_epi32(1);
  BITPACKV32(in, n, b, out, sv); return pout; 
}
#undef VSTI
  #else
#include <strings.h>
#include <emmintrin.h>

#define OPPE(__op)
#define IPPE(__op)

#include "bitpackv32_.h"
 
#define BITPACKV32(__pip, __n, __nbits, __pop, __parm) { __m128i *__ip=(__m128i *)__pip,*__op=(__m128i *)__pop; typeof(__ip[0]) *__ipe=(__ip)+(__n);\
  switch(__nbits) {\
    case  0:  												    		    break;\
    case  1:{  BITPACKV32_1( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case  2:{  BITPACKV32_2( __ip, __op, __parm); /*while(__ip < __ipe)*/;} break;\
    case  3:{  BITPACKV32_3( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case  4:{  BITPACKV32_4( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case  5:{  BITPACKV32_5( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case  6:{  BITPACKV32_6( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case  7:{  BITPACKV32_7( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case  8:{  BITPACKV32_8( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case  9:{  BITPACKV32_9( __ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 10:{  BITPACKV32_10(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 11:{  BITPACKV32_11(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 12:{  BITPACKV32_12(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 13:{  BITPACKV32_13(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 14:{  BITPACKV32_14(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 15:{  BITPACKV32_15(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 16:{  BITPACKV32_16(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 17:{  BITPACKV32_17(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 18:{  BITPACKV32_18(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 19:{  BITPACKV32_19(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 20:{  BITPACKV32_20(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 21:{  BITPACKV32_21(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 22:{  BITPACKV32_22(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 23:{  BITPACKV32_23(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 24:{  BITPACKV32_24(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 25:{  BITPACKV32_25(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 26:{  BITPACKV32_26(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 27:{  BITPACKV32_27(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 28:{  BITPACKV32_28(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 29:{  BITPACKV32_29(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 30:{  BITPACKV32_30(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 31:{  BITPACKV32_31(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
    case 32:{  BITPACKV32_32(__ip, __op, __parm); /*while(__ip < __ipe);*/} break;\
  }\
}
  #endif
