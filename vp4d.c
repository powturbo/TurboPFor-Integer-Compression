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
//   "Integer Compression" TurboPFor - Pfor/PforDelta 
#ifndef USIZE 
#include <stdint.h>

#include "conf.h"
#include "bitunpack.h"
#include "bitutil.h"
#include "vp4d.h"
#include "vint.h"				//#include "vsimple.h"

#define PAD8(__x) ( (((__x)+8-1)/8) )

  #if 0 //defined(__AVX_2__) 
#include "avx2.h"
  #elif defined(__SSSE3__)
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
  #endif
 
#define P4DELTA(a)
#define _P4DEC      _p4dec
#define  P4DEC       p4dec
#define  BITUNPACK   bitunpack  // unpack only
#define  BITUNPACKD  bitunpack  // integrated unpack
#define _BITUNPACKD  bitunpack  // integrated pfor

#define USIZE 64
#include "vp4d.c"

#define  P4DECX  // direct access

#define USIZE 16
#include "vp4d.c"

#define USIZE 32
#include "vp4d.c"
#undef  P4DECX

#define P4DELTA(a) ,a
#define _P4DEC      _p4ddec			//delta0
#define  P4DEC       p4ddec
#define  BITUNPACKD  bitdunpack
#define _BITUNPACKD _bitdunpack
#define  BITUNDD     bitund
#include "vp4d.c"

#define _P4DEC      _p4d1dec        //delta1
#define  P4DEC       p4d1dec
#define  BITUNPACKD  bitd1unpack
#define _BITUNPACKD  bitd1unpack
#define  BITUNDD     bitund1
#include "vp4d.c"
                                 
#undef _P4DEC
#undef  P4DEC
#undef  BITUNPACK
#undef  BITUNDD
#undef  P4DELTA

// SIMD -------------
  #ifndef NSIMD
    #ifdef __SSSE3__
#define P4DELTA(a) 
#define  VSIZE 128
#define _P4DEC        _p4dec128v
#define  P4DEC         p4dec128v
#define  BITUNPACK     bitunpack128v
#define  BITUNPACKD    bitunpack128v
#define  _BITUNPACKD  _bitunpack128v
#include "vp4d.c"

#define P4DELTA(a) ,a
#define _P4DEC        _p4ddec128v
#define  P4DEC         p4ddec128v
#define  BITUNPACKD    bitdunpack128v
#define  _BITUNPACKD  _bitdunpack128v
#define  BITUNDD       bitund
#include "vp4d.c"

#define _P4DEC        _p4d1dec128v
#define  P4DEC         p4d1dec128v
#define  BITUNPACKD    bitd1unpack128v
#define  _BITUNPACKD  _bitd1unpack128v
#define  BITUNDD       bitund1
#include "vp4d.c"
#undef  BITUNDD
#undef  P4DELTA
    #endif

    #ifdef __AVX2__
#define  VSIZE 256
#define _P4DEC        _p4dec256v
#define  P4DEC         p4dec256v
#define  BITUNPACK     bitunpack256v
#define  BITUNPACKD    bitunpack256v
#define  _BITUNPACKD  _bitunpack256v
#include "vp4d.c"

#define P4DELTA 
#define _P4DEC        _p4ddec256v
#define  P4DEC         p4ddec256v
#define  BITUNPACKD    bitdunpack256v
#define  _BITUNPACKD  _bitdunpack256v
#define  BITUNDD       bitund
#include "vp4d.c"

#define _P4DEC        _p4d1dec256v
#define  P4DEC         p4d1dec256v
#define  BITUNPACKD    bitd1unpack256v
#define  _BITUNPACKD  _bitd1unpack256v
#define  BITUNDD       bitund1
#include "vp4d.c"
#undef  BITUNDD
    #endif  
  #endif  

#undef USIZE
#else
#define uint_t TEMPLATE3(uint, USIZE, _t)

unsigned char *TEMPLATE2(_P4DEC, USIZE)(unsigned char *__restrict in, unsigned n, uint_t *__restrict out P4DELTA(uint_t start), unsigned b, unsigned bx ) {
  uint_t ex[P4D_MAX+8];
  if(!(b & 1)) return TEMPLATE2(BITUNPACKD, USIZE)(in, n, out P4DELTA(start), b>>1);

  b >>= 1;
    #ifdef VSIZE 
  unsigned char *pb = in;	
      #if VSIZE == 128  
  in = TEMPLATE2(bitunpack, USIZE)(in+16, popcnt64(ctou64(in)) + popcnt64(ctou64(in+8)), ex, bx); 
      #else
  in = TEMPLATE2(bitunpack, USIZE)(in+32, popcnt64(ctou64(in)) + popcnt64(ctou64(in+8)) + popcnt64(ctou64(in+16)) + popcnt64(ctou64(in+24)), ex, bx); 
      #endif
  return TEMPLATE2(_BITUNPACKD, USIZE)(in, n, out P4DELTA(start), b, ex, pb);	  
    #else
  unsigned long long bb[P4D_MAX/64]; unsigned num=0,i,p4dn = (n+63)/64;
  for(i = 0; i < n/64; i++) { bb[i] = ctou64(in+i*8); num += popcnt64(bb[i]); }
  if(n & 0x3f) { bb[i] = ctou64(in+i*8) & ((1ull<<(n&0x3f))-1); num += popcnt64(bb[i]); }
  in = TEMPLATE2(bitunpack, USIZE)(in+PAD8(n), num, ex, bx);
  in = TEMPLATE2(bitunpack, USIZE)(in, n, out, b);
      #if 0 //defined(AVX_2__)
  uint_t *op,*pex = ex;
  for(i = 0; i < p4dn; i++) {
    for(op = out;    bb[i]; bb[i] >>= 8,op += 8) { unsigned m = (unsigned char)bb[i], mc=popcnt32(m), s = pex[mc]; pex[mc]=0;		
    _mm256_storeu_si256((__m256i *)op, _mm256_add_epi32(_mm256_loadu_si256((const __m256i*)op), mm256_maskz_expand_epi32(m,_mm256_slli_epi32(_mm256_load_si256((const __m256i*)pex), b)))); pex += mc; *pex=s;
    } //out += 64; 
  }
      #elif defined(__SSSE3__) && USIZE == 32
  uint_t *_op=out,*op,*pex = ex; 	
  for(i = 0; i < p4dn; i++) {
    for(op=_op;  bb[i]; bb[i] >>= 4,op+=4) { const unsigned m = bb[i]&0xf; 
      _mm_storeu_si128((__m128i *)op, _mm_add_epi32(_mm_loadu_si128((__m128i*)op), _mm_shuffle_epi8(_mm_slli_epi32(_mm_loadu_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) )); pex += popcnt32(m);
    } _op+=64;
  }
      #else
  unsigned k = 0; uint_t *_op=out,*op;
  for(op=_op,i = 0; i < p4dn; i++) {
	while(bb[i]) { unsigned x = ctz64(bb[i]); op[x] += ex[k++]<<b; bb[i] ^= (1ull<<x); } _op += 8;
  } 				
      #endif
      #ifdef BITUNDD
  TEMPLATE2(BITUNDD, USIZE)(out, n, start);
	  #endif
  return in;
    #endif
}

unsigned char *TEMPLATE2(P4DEC, USIZE)(unsigned char *__restrict in, unsigned n, uint_t *__restrict out P4DELTA(uint_t start) ) {  
  unsigned b = *in++,bx,i;
  
  if(likely(!(b & 0x80))) {
    if(b & 1)
	  bx = *in++;
    return TEMPLATE2(_P4DEC, USIZE)(in, n, out P4DELTA(start), b, bx );
  } else {  
    uint_t ex[P4D_MAX+8]; 
	b  = (b & 0x7f)>>1;
    bx = *in++;
    in = TEMPLATE2(BITUNPACK, USIZE)(in, n, out, b); 
    in = TEMPLATE2(vbdec,     USIZE)(in, bx, ex);
    for(i = 0; i != (bx & ~3); i += 4) { 
	  out[in[i  ]] |= ex[i  ] << b;
	  out[in[i+1]] |= ex[i+1] << b;
	  out[in[i+2]] |= ex[i+2] << b;
	  out[in[i+3]] |= ex[i+3] << b;
	}
	for(;i < bx; i++) 
	  out[in[i]] |= ex[i] << b;
    in += bx;
      #ifdef BITUNDD
  TEMPLATE2(BITUNDD, USIZE)(out, n, start);
	  #endif
    return in;
  }
}

    #ifdef P4DECX  
unsigned char *TEMPLATE2(p4decx, USIZE)(unsigned char *in, unsigned n, uint_t *__restrict out) {
  unsigned b,i; 
  struct p4 p4; 
  p4ini(&p4, &in, n, &b);

  if(unlikely(p4.i & 1)) {
    for(i = 0; i != n&~3; i+=4) {
      out[i  ] = TEMPLATE2(p4getx, USIZE)(&p4, in, i  , b);  
      out[i+1] = TEMPLATE2(p4getx, USIZE)(&p4, in, i+1, b);  
      out[i+2] = TEMPLATE2(p4getx, USIZE)(&p4, in, i+2, b);  
      out[i+3] = TEMPLATE2(p4getx, USIZE)(&p4, in, i+3, b);  
    }
    for(     ; i != n; i++) 
      out[i  ] = TEMPLATE2(p4getx, USIZE)(&p4, in, i  , b);  
  } else {
    for(i = 0; i != n&~3; i+=4) {
      out[i  ] = TEMPLATE2(_bitgetx, USIZE)(in,  i   *b, b); 
      out[i+1] = TEMPLATE2(_bitgetx, USIZE)(in, (i+1)*b, b); 
      out[i+2] = TEMPLATE2(_bitgetx, USIZE)(in, (i+2)*b, b); 
      out[i+3] = TEMPLATE2(_bitgetx, USIZE)(in, (i+3)*b, b); 
    }
    for(     ; i != n; i++) 
      out[i  ] = TEMPLATE2(_bitgetx, USIZE)(in,  i   *b, b); 
  }
  return in + PAD8(n*b);
}

unsigned char *TEMPLATE2(p4fdecx, USIZE)(unsigned char *in, unsigned n, uint_t *__restrict out, unsigned start) {
  unsigned b,i; 
  struct p4 p4; 
  p4ini(&p4, &in, n, &b);

  if(unlikely(p4.i & 1)) {
    for(i = 0; i < n; i++) 
      out[i] = TEMPLATE2(p4getx, USIZE)(&p4, in, i, b)+start+i+1;  
  } else for(i = 0; i < n; i++) out[i] = TEMPLATE2(_bitgetx, USIZE)(in, i*b, b)+start+i+1;
  return in + PAD8(n*b);
}

unsigned char *TEMPLATE2(p4f0decx, USIZE)(unsigned char *in, unsigned n, uint_t *__restrict out, unsigned start) {
  unsigned b,i; 
  struct p4 p4; 
  p4ini(&p4, &in, n, &b);

  if(unlikely(p4.i & 1)) {
    for(i = 0; i < n; i++) 
      out[i] = TEMPLATE2(p4getx, USIZE)(&p4, in, i, b)+start;//   return p4.ex + PAD8((p4.cum[P4DN-1] + popcnt64(p4.xmap[P4DN-1]))*p4.bx);
  } else for(i = 0; i < n; i++) out[i] = TEMPLATE2(_bitgetx, USIZE)(in, i*b, b)+start;
  return in + PAD8(n*b);
}
    #endif

#endif
