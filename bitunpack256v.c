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
//  "Integer Compression" AVX2 Bit Packing
  #ifndef VSTO  
#include <stdio.h>
  
#include <strings.h>
#include <immintrin.h>
#include "conf.h"       
#include "bitutil.h"
#include "bitunpack.h"

  #ifdef __AVX512F__
#include <immintrin.h>
#define mm256_maskz_expand_epi32(_m_,_v_) _mm256_maskz_expand_epi32(_m_,_v_)
#define mm256_maskz_loadu_epi32( _m_,_v_) _mm256_maskz_loadu_epi32( _m_,_v_)
  #else
#include <immintrin.h>
static unsigned char shuffles[256][8] __attribute__((aligned(32))) = {
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
#define mm256_maskz_expand_epi32(_m_, _v_) _mm256_permutevar8x32_epi32(_v_,  _mm256_cvtepu8_epi32(_mm_cvtsi64_si128(ctou64(shuffles[_m_]))) )
#define mm256_maskz_loadu_epi32(_m_,_v_)   _mm256_blendv_epi8(zv, mm256_maskz_expand_epi32(xm, _mm256_loadu_si256((__m256i*)pex)), u2vmask(xm,tv))
  #endif

#define PAD8(__x) (((__x)+7)/8) 
              
//-----------------------------------------------------------------------------
#define VSTO( _op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, ov)
#define VSTO0(_op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, _parm_)
#include "bitunpack256v.c"

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
#undef VSTO
#undef VSTO0
#undef BITUNPACK0 

//--------------------------------------- zeromask unpack for TurboPFor vp4d.c --------------------------------------
#define VSTO(_op_, _i_, _ov_, _parm_)  xm = *bb++; _mm256_storeu_si256(_op_++, _mm256_add_epi32(_ov_, _mm256_slli_epi32(mm256_maskz_loadu_epi32(xm,(__m256i*)pex), b) )); pex += popcnt32(xm)
#define VSTO0(_op_, _i_, _ov_, _parm_) xm = *bb++; _mm256_storeu_si256(_op_++,                                          mm256_maskz_loadu_epi32(xm,(__m256i*)pex) );      pex += popcnt32(xm)
#define BITUNPACK0(_parm_)
#include "bitunpack256v.c" 

unsigned char *_bitunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned *__restrict pex, unsigned char *bb) {
  const unsigned char *ip = in+PAD8(256*b); unsigned xm; __m256i sv, zv = _mm256_setzero_si256(), tv = _mm256_set_epi32(0,1,2,3,4,5,6,7);
  BITUNPACK256V32(in, b, out, sv); 
  return (unsigned char *)ip; 
}     
#undef VSTO  
#undef VSTO0 
#undef BITUNPACK0 
//--------------------------------
#define VSTO0(_op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, _parm_)
#define VSTO(__op, i, __ov, __sv) __ov = UNZIGZAG256x32(__ov); SCAN256x32(__ov,__sv); _mm256_storeu_si256(__op++, __sv)
#include "bitunpack256v.c"
  
#define BITUNPACK0(_parm_)
 
unsigned char *bitzunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(256*b); 
  __m256i sv = _mm256_set1_epi32(start), zv = _mm256_setzero_si256(); 
  BITUNPACK256V32(in, b, out, sv); 
  return (unsigned char *)ip; 
} 
#undef VSTO
#undef BITUNPACK0
 
//-----------------------------------------------------------------------------
#define VSTO(__op, i, __ov, __sv) SCAN256x32(__ov,__sv); _mm256_storeu_si256(__op++, __sv)
#include "bitunpack256v.c"

#define BITUNPACK0(_parm_)

unsigned char *bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
  const unsigned char *ip = in+PAD8(256*b); 
  __m256i sv = _mm256_set1_epi32(start), zv = _mm256_setzero_si256();
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}
#undef VSTO 
#undef VSTO0
#undef BITUNPACK0

//-----------------------------------------------------------------------------
#define VEXP(_i_, _ov_)  xm = *bb++; _ov_ = _mm256_add_epi32(_ov_, _mm256_slli_epi32(mm256_maskz_loadu_epi32(xm,(__m256i*)pex), b) ); pex += popcnt32(xm)
#define VEXP0(_i_, _ov_) xm = *bb++; _ov_ =                                          mm256_maskz_loadu_epi32(xm,(__m256i*)pex);       pex += popcnt32(xm)
 
#define VSTO( _op_, _i_, _ov_, _sv_) VEXP( _i_, _ov_); SCAN256x32(_ov_,_sv_); _mm256_storeu_si256(_op_++, _sv_);
#define VSTO0(_op_, _i_, _ov_, _sv_) VEXP0(_i_, _ov_); SCAN256x32(_ov_,_sv_); _mm256_storeu_si256(_op_++, _sv_);

#include "bitunpack256v.c"
 
#define BITUNPACK0(_parm_)
 
unsigned char *_bitdunpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) { 
  const unsigned char *ip = in+PAD8(256*b); unsigned xm;
  __m256i sv = _mm256_set1_epi32(start),zv = _mm256_setzero_si256(), tv = _mm256_set_epi32(0,1,2,3,4,5,6,7);
  BITUNPACK256V32(in, b, out, sv); 
  return (unsigned char *)ip; 
} 
#undef VSTO
#undef VSTO0
#undef BITUNPACK0

//-----------------------------------------------------------------------------
#define VSTO(__op, i, __ov, __sv)    SCANI256x32(__ov,__sv,cv); _mm256_storeu_si256(__op++, __sv);
#define VSTO0(_op_, _i_, ov, _parm_) _mm256_storeu_si256(_op_++, _parm_); _parm_ = _mm256_add_epi32(_parm_, cv)
#include "bitunpack256v.c"

#define BITUNPACK0(_parm_) _parm_ = _mm256_add_epi32(_parm_, cv); cv = _mm256_set1_epi32(8)

unsigned char *bitd1unpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) {
  const unsigned char *ip = in+PAD8(256*b);
  __m256i sv = _mm256_set1_epi32(start), cv = _mm256_set_epi32(8,7,6,5,4,3,2,1),zv = _mm256_setzero_si256();
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0
//-----------------------------------------------------------------------------
#define VSTO( _op_, _i_, _ov_, _sv_)   VEXP( _i_, _ov_); SCANI256x32(_ov_,_sv_,cv); _mm256_storeu_si256(_op_++, _sv_);
#define VSTO0(_op_, _i_, _ov_, _sv_)   VEXP0(_i_, _ov_); SCANI256x32(_ov_,_sv_,cv); _mm256_storeu_si256(_op_++, _sv_);

#include "bitunpack256v.c"

#define BITUNPACK0(_parm_) mv = _mm256_set1_epi32(0) //_parm_ = _mm_setzero_si128()

unsigned char *_bitd1unpack256v32( const unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned *__restrict pex, unsigned char *bb) {
  const unsigned char *ip = in+PAD8(256*b); unsigned xm;
  __m256i sv = _mm256_set1_epi32(start), cv = _mm256_set_epi32(8,7,6,5,4,3,2,1),zv = _mm256_setzero_si256(),tv = _mm256_set_epi32(0,1,2,3,4,5,6,7);
  BITUNPACK256V32(in, b, out, sv);
  return (unsigned char *)ip; 
}
#undef VSTO
#undef VSTO0
#undef BITUNPACK0

  #else
#include "bitunpack256v_.h"

#define BITUNPACK256V32(__ip, __nbits, __op, _parm_) { __m256i mv,*_ov=(__m256i *)__op,*_iv=(__m256i *)__ip; \
  switch(__nbits&0x3f) {\
    case  0: BITUNPACK0(_parm_);               BITUNPACK256V32_0( _iv, _ov, _parm_); break;\
    case  1: mv = _mm256_set1_epi32((1u<< 1)-1);  BITUNPACK256V32_1( _iv, _ov, _parm_); break;\
    case  2: mv = _mm256_set1_epi32((1u<< 2)-1);  BITUNPACK256V32_2( _iv, _ov, _parm_); break;\
    case  3: mv = _mm256_set1_epi32((1u<< 3)-1);  BITUNPACK256V32_3( _iv, _ov, _parm_); break;\
    case  4: mv = _mm256_set1_epi32((1u<< 4)-1);  BITUNPACK256V32_4( _iv, _ov, _parm_); break;\
    case  5: mv = _mm256_set1_epi32((1u<< 5)-1);  BITUNPACK256V32_5( _iv, _ov, _parm_); break;\
    case  6: mv = _mm256_set1_epi32((1u<< 6)-1);  BITUNPACK256V32_6( _iv, _ov, _parm_); break;\
    case  7: mv = _mm256_set1_epi32((1u<< 7)-1);  BITUNPACK256V32_7( _iv, _ov, _parm_); break;\
    case  8: mv = _mm256_set1_epi32((1u<< 8)-1);  BITUNPACK256V32_8( _iv, _ov, _parm_); break;\
    case  9: mv = _mm256_set1_epi32((1u<< 9)-1);  BITUNPACK256V32_9( _iv, _ov, _parm_); break;\
    case 10: mv = _mm256_set1_epi32((1u<<10)-1);  BITUNPACK256V32_10(_iv, _ov, _parm_); break;\
    case 11: mv = _mm256_set1_epi32((1u<<11)-1);  BITUNPACK256V32_11(_iv, _ov, _parm_); break;\
    case 12: mv = _mm256_set1_epi32((1u<<12)-1);  BITUNPACK256V32_12(_iv, _ov, _parm_); break;\
    case 13: mv = _mm256_set1_epi32((1u<<13)-1);  BITUNPACK256V32_13(_iv, _ov, _parm_); break;\
    case 14: mv = _mm256_set1_epi32((1u<<14)-1);  BITUNPACK256V32_14(_iv, _ov, _parm_); break;\
    case 15: mv = _mm256_set1_epi32((1u<<15)-1);  BITUNPACK256V32_15(_iv, _ov, _parm_); break;\
    case 16: mv = _mm256_set1_epi32((1u<<16)-1);  BITUNPACK256V32_16(_iv, _ov, _parm_); break;\
    case 17: mv = _mm256_set1_epi32((1u<<17)-1);  BITUNPACK256V32_17(_iv, _ov, _parm_); break;\
    case 18: mv = _mm256_set1_epi32((1u<<18)-1);  BITUNPACK256V32_18(_iv, _ov, _parm_); break;\
    case 19: mv = _mm256_set1_epi32((1u<<19)-1);  BITUNPACK256V32_19(_iv, _ov, _parm_); break;\
    case 20: mv = _mm256_set1_epi32((1u<<20)-1);  BITUNPACK256V32_20(_iv, _ov, _parm_); break;\
    case 21: mv = _mm256_set1_epi32((1u<<21)-1);  BITUNPACK256V32_21(_iv, _ov, _parm_); break;\
    case 22: mv = _mm256_set1_epi32((1u<<22)-1);  BITUNPACK256V32_22(_iv, _ov, _parm_); break;\
    case 23: mv = _mm256_set1_epi32((1u<<23)-1);  BITUNPACK256V32_23(_iv, _ov, _parm_); break;\
    case 24: mv = _mm256_set1_epi32((1u<<24)-1);  BITUNPACK256V32_24(_iv, _ov, _parm_); break;\
    case 25: mv = _mm256_set1_epi32((1u<<25)-1);  BITUNPACK256V32_25(_iv, _ov, _parm_); break;\
    case 26: mv = _mm256_set1_epi32((1u<<26)-1);  BITUNPACK256V32_26(_iv, _ov, _parm_); break;\
    case 27: mv = _mm256_set1_epi32((1u<<27)-1);  BITUNPACK256V32_27(_iv, _ov, _parm_); break;\
    case 28: mv = _mm256_set1_epi32((1u<<28)-1);  BITUNPACK256V32_28(_iv, _ov, _parm_); break;\
    case 29: mv = _mm256_set1_epi32((1u<<29)-1);  BITUNPACK256V32_29(_iv, _ov, _parm_); break;\
    case 30: mv = _mm256_set1_epi32((1u<<30)-1);  BITUNPACK256V32_30(_iv, _ov, _parm_); break;\
    case 31: mv = _mm256_set1_epi32((1u<<31)-1);  BITUNPACK256V32_31(_iv, _ov, _parm_); break;\
    case 32: mv = _mm256_set1_epi32((1ull<<32)-1);BITUNPACK256V32_32(_iv, _ov, _parm_); break;\
	case 33 ... 63: break;\
  }\
}    
  #endif


