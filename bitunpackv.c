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
//  bitunpack_.h - "Integer Compression" Bit Packing
  #ifndef VSTO
#include "conf.h"       
#include "bitutil.h"
 
#include "bitunpack.h"

#define PAD8(__x) (((__x)+7)/8) 
              
//-----------------------------------------------------------------------------------------------------------------
#define VSTO(op, i, ov, parm) _mm_storeu_si128(op++, ov)

#include __FILE__
#define BITUNBLKV32_0(ip, i, __op, __parm) {\
  _mm_storeu_si128(__op++, __parm);\
  _mm_storeu_si128(__op++, __parm);\
  _mm_storeu_si128(__op++, __parm);\
  _mm_storeu_si128(__op++, __parm);\
  _mm_storeu_si128(__op++, __parm);\
  _mm_storeu_si128(__op++, __parm);\
  _mm_storeu_si128(__op++, __parm);\
  _mm_storeu_si128(__op++, __parm);\
}
#define BITUNPACK0(__parm) __parm = _mm_setzero_si128()

unsigned char *bitunpackv32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b) { unsigned char *ip = in+PAD8(n*b); __m128i sv; BITUNPACKV32(in, n, b, out, sv); return ip; }
#undef VSTO
#undef BITUNPACK0

//------------------------------------------------------																
#define VSTO(__op, i, __ov, __sv) __ov = UNZIGZAG128_32(__ov); SCAN128_32(__ov,__sv); _mm_storeu_si128(__op++, __sv)

#include __FILE__
#define BITUNPACK0(__parm)

unsigned char *bitzunpackv32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); 
  __m128i sv = _mm_set1_epi32(start); 
  BITUNPACKV32(in, n, b, out, sv); return ip; 
}
#undef VSTO
#undef BITUNPACK0

//------------------------------------------------------																
#define VSTO(__op, i, __ov, __sv) SCAN128_32(__ov,__sv); _mm_storeu_si128(__op++, __sv)

#include __FILE__
#define BITUNPACK0(__parm)

unsigned char *bitdunpackv32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); 
  __m128i sv = _mm_set1_epi32(start);
  BITUNPACKV32(in, n, b, out, sv); return ip; 
}
#undef VSTO
#undef BITUNBLKV32_0
#undef BITUNPACK0

//--------------------------------------------------------------------------------------------------------------------------------------------- 
#define VSTO(__op, i, __ov, __sv) SCANI128_32(__ov,__sv,cv); _mm_storeu_si128(__op++, __sv);

#include __FILE__
#define BITUNBLKV32_0(ip, i, __op, __parm) {\
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
  _mm_storeu_si128(__op++, __parm); __parm = _mm_add_epi32(__parm, cv); \
}
#define BITUNPACK0(__parm) __parm = _mm_add_epi32(__parm, cv); cv = _mm_set1_epi32(4)

unsigned char *bitd1unpackv32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b);
  __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4,3,2,1);
  BITUNPACKV32(in, n, b, out, sv); return ip; 
}
#undef VSTO
#undef BITUNBLKV32_0
#undef BITUNPACK0

  #else
#include <strings.h>
#include <emmintrin.h>

#include "bitunpackv32_.h"

#define BITUNPACKV32(__ip, __n, __nbits, __op, __parm) { __m128i mv,*__ov=(__m128i *)__op, *__ove = (__m128i *)(__op + __n),*__iv=(__m128i *)__ip; \
  switch(__nbits&0x3f) {\
    case  0: {  BITUNPACK0(__parm); do BITUNPACKV32_0( __iv, __ov, __parm) while(__ov<__ove); } break;\
    case  1: mv = _mm_set1_epi32((1u<< 1)-1);  BITUNPACKV32_1( __iv, __ov, __parm); break;\
    case  2: mv = _mm_set1_epi32((1u<< 2)-1);  BITUNPACKV32_2( __iv, __ov, __parm); break;\
    case  3: mv = _mm_set1_epi32((1u<< 3)-1);  BITUNPACKV32_3( __iv, __ov, __parm); break;\
    case  4: mv = _mm_set1_epi32((1u<< 4)-1);  BITUNPACKV32_4( __iv, __ov, __parm); break;\
    case  5: mv = _mm_set1_epi32((1u<< 5)-1);  BITUNPACKV32_5( __iv, __ov, __parm); break;\
    case  6: mv = _mm_set1_epi32((1u<< 6)-1);  BITUNPACKV32_6( __iv, __ov, __parm); break;\
    case  7: mv = _mm_set1_epi32((1u<< 7)-1);  BITUNPACKV32_7( __iv, __ov, __parm); break;\
    case  8: mv = _mm_set1_epi32((1u<< 8)-1);  BITUNPACKV32_8( __iv, __ov, __parm); break;\
    case  9: mv = _mm_set1_epi32((1u<< 9)-1);  BITUNPACKV32_9( __iv, __ov, __parm); break;\
    case 10: mv = _mm_set1_epi32((1u<<10)-1);  BITUNPACKV32_10(__iv, __ov, __parm); break;\
    case 11: mv = _mm_set1_epi32((1u<<11)-1);  BITUNPACKV32_11(__iv, __ov, __parm); break;\
    case 12: mv = _mm_set1_epi32((1u<<12)-1);  BITUNPACKV32_12(__iv, __ov, __parm); break;\
    case 13: mv = _mm_set1_epi32((1u<<13)-1);  BITUNPACKV32_13(__iv, __ov, __parm); break;\
    case 14: mv = _mm_set1_epi32((1u<<14)-1);  BITUNPACKV32_14(__iv, __ov, __parm); break;\
    case 15: mv = _mm_set1_epi32((1u<<15)-1);  BITUNPACKV32_15(__iv, __ov, __parm); break;\
    case 16: mv = _mm_set1_epi32((1u<<16)-1);  BITUNPACKV32_16(__iv, __ov, __parm); break;\
    case 17: mv = _mm_set1_epi32((1u<<17)-1);  BITUNPACKV32_17(__iv, __ov, __parm); break;\
    case 18: mv = _mm_set1_epi32((1u<<18)-1);  BITUNPACKV32_18(__iv, __ov, __parm); break;\
    case 19: mv = _mm_set1_epi32((1u<<19)-1);  BITUNPACKV32_19(__iv, __ov, __parm); break;\
    case 20: mv = _mm_set1_epi32((1u<<20)-1);  BITUNPACKV32_20(__iv, __ov, __parm); break;\
    case 21: mv = _mm_set1_epi32((1u<<21)-1);  BITUNPACKV32_21(__iv, __ov, __parm); break;\
    case 22: mv = _mm_set1_epi32((1u<<22)-1);  BITUNPACKV32_22(__iv, __ov, __parm); break;\
    case 23: mv = _mm_set1_epi32((1u<<23)-1);  BITUNPACKV32_23(__iv, __ov, __parm); break;\
    case 24: mv = _mm_set1_epi32((1u<<24)-1);  BITUNPACKV32_24(__iv, __ov, __parm); break;\
    case 25: mv = _mm_set1_epi32((1u<<25)-1);  BITUNPACKV32_25(__iv, __ov, __parm); break;\
    case 26: mv = _mm_set1_epi32((1u<<26)-1);  BITUNPACKV32_26(__iv, __ov, __parm); break;\
    case 27: mv = _mm_set1_epi32((1u<<27)-1);  BITUNPACKV32_27(__iv, __ov, __parm); break;\
    case 28: mv = _mm_set1_epi32((1u<<28)-1);  BITUNPACKV32_28(__iv, __ov, __parm); break;\
    case 29: mv = _mm_set1_epi32((1u<<29)-1);  BITUNPACKV32_29(__iv, __ov, __parm); break;\
    case 30: mv = _mm_set1_epi32((1u<<30)-1);  BITUNPACKV32_30(__iv, __ov, __parm); break;\
    case 31: mv = _mm_set1_epi32((1u<<31)-1);  BITUNPACKV32_31(__iv, __ov, __parm); break;\
    case 32: mv = _mm_set1_epi32((1ull<<32)-1);BITUNPACKV32_32(__iv, __ov, __parm); break;\
	case 33 ... 63: break;\
  }\
}
  #endif
