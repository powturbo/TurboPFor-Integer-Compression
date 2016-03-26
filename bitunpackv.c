/**
    Copyright (C) powturbo 2013-2016
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
//  bitunpackv.c - "Integer Compression" SIMD Bit Packing
  #ifndef VSTO
#include <stdio.h>
  
#include <strings.h>
#include <emmintrin.h>
#include "conf.h"       
#include "bitutil.h"
#include "bitunpack.h"

#define PAD8(__x) (((__x)+7)/8) 
              
//-----------------------------------------------------------------------------
#define VSTO( _op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, ov)
#define VSTO0(_op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, _parm_)
#include "bitunpackv.c"

#define BITUNBLKV32_0(ip, _i_, _op_, _parm_) {__m128i ov;\
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

unsigned char *bitunpackv32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b) { 
  const unsigned char *ip = in+PAD8(n*b); 
  __m128i sv; 
  BITUNPACKV32(in, n, b, out, sv); 
  return ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0
//-----------------------------------------------------------------------------
  #ifdef __SSSE3__
#include <tmmintrin.h>
static ALIGNED(char, shuffles[16][16], 16) = {
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

#define VSTO( _op_, _i_, _ov_, _parm_) if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _mm_storeu_si128(_op_++, _mm_add_epi32(_ov_, _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) )); pex += popcnt32(m)
#define VSTO0(_op_, _i_, ov, _parm_)   if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _mm_storeu_si128(_op_++,                     _mm_shuffle_epi8(               _mm_loadu_si128((__m128i*)pex),     _mm_load_si128((__m128i*)shuffles[m]) ) );  pex += popcnt32(m)
#define BITUNPACK0(_parm_) //_parm_ = _mm_setzero_si128()
#include "bitunpackv.c"

unsigned char *_bitunpackv32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb) { 
  const unsigned char *ip = in+PAD8(n*b); unsigned m;
  __m128i sv; 
  BITUNPACKV32(in, n, b, out, sv);
  return ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0
  #endif

//-----------------------------------------------------------------------------
#define VSTO0(_op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, _parm_)
#define VSTO(__op, i, __ov, __sv) __ov = UNZIGZAG128_32(__ov); SCAN128_32(__ov,__sv); _mm_storeu_si128(__op++, __sv)
#include "bitunpackv.c"

#define BITUNPACK0(_parm_)

unsigned char *bitzunpackv32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(n*b); 
  __m128i sv = _mm_set1_epi32(start); 
  BITUNPACKV32(in, n, b, out, sv); 
  return ip; 
}
#undef VSTO
#undef BITUNPACK0

//-----------------------------------------------------------------------------
#define VSTO(__op, i, __ov, __sv) SCAN128_32(__ov,__sv); _mm_storeu_si128(__op++, __sv)
#include "bitunpackv.c"

#define BITUNPACK0(_parm_)

unsigned char *bitdunpackv32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(n*b); 
  __m128i sv = _mm_set1_epi32(start);
  BITUNPACKV32(in, n, b, out, sv); 
  return ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0

//-----------------------------------------------------------------------------
  #ifdef __SSSE3__
#define VEXP(_i_, _ov_)         if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_add_epi32(_ov_, _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) ); pex += popcnt32(m)
#define VSTO( _op_, _i_, _ov_, _sv_)   VEXP( _i_, _ov_); SCAN128_32(_ov_,_sv_); _mm_storeu_si128(_op_++, _sv_);

#define VEXP0(_i_, _ov_)        if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_shuffle_epi8(_mm_loadu_si128((__m128i*)pex),_mm_load_si128((__m128i*)shuffles[m]) ); pex += popcnt32(m)
#define VSTO0(_op_, _i_, _ov_, _sv_)   VEXP0( _i_, _ov_); SCAN128_32(_ov_,_sv_); _mm_storeu_si128(_op_++, _sv_);

#include "bitunpackv.c"

#define BITUNPACK0(_parm_)

unsigned char *_bitdunpackv32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) { 
  const unsigned char *ip = in+PAD8(n*b); unsigned m;
  __m128i sv = _mm_set1_epi32(start);
  BITUNPACKV32(in, n, b, out, sv); 
  return ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0
  #endif
//-----------------------------------------------------------------------------
#define VSTO(__op, i, __ov, __sv) SCANI128_32(__ov,__sv,cv); _mm_storeu_si128(__op++, __sv);
#define VSTO0(_op_, _i_, ov, _parm_) _mm_storeu_si128(_op_++, _parm_); _parm_ = _mm_add_epi32(_parm_, cv)
#include "bitunpackv.c"

#define BITUNPACK0(_parm_) _parm_ = _mm_add_epi32(_parm_, cv); cv = _mm_set1_epi32(4)

unsigned char *bitd1unpackv32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(n*b);
  __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4,3,2,1);
  BITUNPACKV32(in, n, b, out, sv); 
  return ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0
//-----------------------------------------------------------------------------
  #ifdef __SSSE3__
#define VEXP(_i_, _ov_)         if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_add_epi32(_ov_, _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) ); pex += popcnt32(m)
#define VSTO( _op_, _i_, _ov_, _sv_)   VEXP( _i_, _ov_); SCANI128_32(_ov_,_sv_,cv); _mm_storeu_si128(_op_++, _sv_);

#define VEXP0(_i_, _ov_)        if(!((_i_) & 1)) m = (*bb) & 0xf;else m = (*bb++) >> 4; _ov_ = _mm_shuffle_epi8(_mm_loadu_si128((__m128i*)pex),_mm_load_si128((__m128i*)shuffles[m]) ); pex += popcnt32(m)
#define VSTO0(_op_, _i_, _ov_, _sv_)   VEXP0( _i_, _ov_); SCANI128_32(_ov_,_sv_,cv); _mm_storeu_si128(_op_++, _sv_);

#include "bitunpackv.c"

#define BITUNPACK0(_parm_) mv = _mm_set1_epi32(0) //_parm_ = _mm_setzero_si128()

unsigned char *_bitd1unpackv32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) { 
  const unsigned char *ip = in+PAD8(n*b); unsigned m;
  __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4,3,2,1);
  BITUNPACKV32(in, n, b, out, sv); 
  return ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0
  #endif

  #else
#include "bitunpackv32_.h"

#define BITUNPACKV32(__ip, __n, __nbits, __op, _parm_) { __m128i mv,*_ov=(__m128i *)__op,*_iv=(__m128i *)__ip; \
  switch(__nbits&0x3f) {\
    case  0: BITUNPACK0(_parm_);               BITUNPACKV32_0( _iv, _ov, _parm_); break;\
    case  1: mv = _mm_set1_epi32((1u<< 1)-1);  BITUNPACKV32_1( _iv, _ov, _parm_); break;\
    case  2: mv = _mm_set1_epi32((1u<< 2)-1);  BITUNPACKV32_2( _iv, _ov, _parm_); break;\
    case  3: mv = _mm_set1_epi32((1u<< 3)-1);  BITUNPACKV32_3( _iv, _ov, _parm_); break;\
    case  4: mv = _mm_set1_epi32((1u<< 4)-1);  BITUNPACKV32_4( _iv, _ov, _parm_); break;\
    case  5: mv = _mm_set1_epi32((1u<< 5)-1);  BITUNPACKV32_5( _iv, _ov, _parm_); break;\
    case  6: mv = _mm_set1_epi32((1u<< 6)-1);  BITUNPACKV32_6( _iv, _ov, _parm_); break;\
    case  7: mv = _mm_set1_epi32((1u<< 7)-1);  BITUNPACKV32_7( _iv, _ov, _parm_); break;\
    case  8: mv = _mm_set1_epi32((1u<< 8)-1);  BITUNPACKV32_8( _iv, _ov, _parm_); break;\
    case  9: mv = _mm_set1_epi32((1u<< 9)-1);  BITUNPACKV32_9( _iv, _ov, _parm_); break;\
    case 10: mv = _mm_set1_epi32((1u<<10)-1);  BITUNPACKV32_10(_iv, _ov, _parm_); break;\
    case 11: mv = _mm_set1_epi32((1u<<11)-1);  BITUNPACKV32_11(_iv, _ov, _parm_); break;\
    case 12: mv = _mm_set1_epi32((1u<<12)-1);  BITUNPACKV32_12(_iv, _ov, _parm_); break;\
    case 13: mv = _mm_set1_epi32((1u<<13)-1);  BITUNPACKV32_13(_iv, _ov, _parm_); break;\
    case 14: mv = _mm_set1_epi32((1u<<14)-1);  BITUNPACKV32_14(_iv, _ov, _parm_); break;\
    case 15: mv = _mm_set1_epi32((1u<<15)-1);  BITUNPACKV32_15(_iv, _ov, _parm_); break;\
    case 16: mv = _mm_set1_epi32((1u<<16)-1);  BITUNPACKV32_16(_iv, _ov, _parm_); break;\
    case 17: mv = _mm_set1_epi32((1u<<17)-1);  BITUNPACKV32_17(_iv, _ov, _parm_); break;\
    case 18: mv = _mm_set1_epi32((1u<<18)-1);  BITUNPACKV32_18(_iv, _ov, _parm_); break;\
    case 19: mv = _mm_set1_epi32((1u<<19)-1);  BITUNPACKV32_19(_iv, _ov, _parm_); break;\
    case 20: mv = _mm_set1_epi32((1u<<20)-1);  BITUNPACKV32_20(_iv, _ov, _parm_); break;\
    case 21: mv = _mm_set1_epi32((1u<<21)-1);  BITUNPACKV32_21(_iv, _ov, _parm_); break;\
    case 22: mv = _mm_set1_epi32((1u<<22)-1);  BITUNPACKV32_22(_iv, _ov, _parm_); break;\
    case 23: mv = _mm_set1_epi32((1u<<23)-1);  BITUNPACKV32_23(_iv, _ov, _parm_); break;\
    case 24: mv = _mm_set1_epi32((1u<<24)-1);  BITUNPACKV32_24(_iv, _ov, _parm_); break;\
    case 25: mv = _mm_set1_epi32((1u<<25)-1);  BITUNPACKV32_25(_iv, _ov, _parm_); break;\
    case 26: mv = _mm_set1_epi32((1u<<26)-1);  BITUNPACKV32_26(_iv, _ov, _parm_); break;\
    case 27: mv = _mm_set1_epi32((1u<<27)-1);  BITUNPACKV32_27(_iv, _ov, _parm_); break;\
    case 28: mv = _mm_set1_epi32((1u<<28)-1);  BITUNPACKV32_28(_iv, _ov, _parm_); break;\
    case 29: mv = _mm_set1_epi32((1u<<29)-1);  BITUNPACKV32_29(_iv, _ov, _parm_); break;\
    case 30: mv = _mm_set1_epi32((1u<<30)-1);  BITUNPACKV32_30(_iv, _ov, _parm_); break;\
    case 31: mv = _mm_set1_epi32((1u<<31)-1);  BITUNPACKV32_31(_iv, _ov, _parm_); break;\
    case 32: mv = _mm_set1_epi32((1ull<<32)-1);BITUNPACKV32_32(_iv, _ov, _parm_); break;\
	case 33 ... 63: break;\
  }\
}
  #endif
