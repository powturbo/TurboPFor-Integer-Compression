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
#include <emmintrin.h>
#include "bitpack.h"
#include "bitutil.h"

#define OPPE(__op)
#define IPPE(__op)

#define PAD8(__x) (((__x)+8-1)/8)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm_loadu_si128(ip++)
#include "bitpack128v_.h" 
  
unsigned char *bitpack128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(128*b); BITPACK128V32(in, b, out, 0); return pout; }
#undef VSTI 
#undef IPP

//------------------------------------------------------------------------------------------------------------------------------
#define VSTI(__ip, __i, __iv, __sv) v = _mm_loadu_si128(__ip++); __iv = DELTA128x32(v,__sv); __sv = v
#define IPP(ip, i, __iv) __iv
#include "bitpack128v_.h" 

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

#ifdef __AVX2__
#include <immintrin.h>
#include <strings.h>
#include <tmmintrin.h>

#define OPPE(__op)
#define IPPE(__op)

#define PAD8(__x) (((__x)+8-1)/8)
#define OPPE(__op)
#define IPPE(__op)

#define VSTI(ip, i, iv, parm)
#define IPP(ip, i, iv) _mm256_loadu_si256(ip++)
#include "bitpack256v_.h" 
//#include "bitpack.h"
//#include "bitutil.h"
 
unsigned char *bitpack256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b) { unsigned char *pout = out+PAD8(256*b); BITPACK256V32(in, b, out, 0); return pout; }
#undef VSTI 
#undef IPP

//------------------------------------------------------------------------------------------------------------------------------
#if 0
#define VSTI(__ip, __i, __iv, __sv) v = _mm256_loadu_si256(__ip++); DELTA256x32(v,__sv, __iv) //__sv = v
#define IPP(ip, i, __iv) __iv
#include "bitpack256v_.h"  

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

