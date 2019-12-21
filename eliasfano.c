/**
    Copyright (C) powturbo 2013-2019
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

//    eliasfano.c - "Integer Compression" Elias Fano
#ifndef USIZE
#include <string.h>
#pragma warning( disable : 4005)
#pragma warning( disable : 4090)
#pragma warning( disable : 4068)

#include "conf.h"
#include "bitpack.h"
#define BITUTIL_IN
#include "bitutil.h"
#include "eliasfano.h"

#define PAD8(__x) ( (((__x)+8-1)/8) )

  #ifdef __SSE42__
#include <nmmintrin.h>
#define bslr32(x) _blsr_u32(x)
#define bslr64(x) _blsr_u64(x)
  #else
//static inline unsigned long long blsr(unsigned long long x) { return x & (x - 1); }
#define blsr32(_x_) ((_x_) & ((_x_) - 1))
#define blsr64(_x_) ((_x_) & ((_x_) - 1))
  #endif
#define blsr8(_x_)  blsr32(_x_)
#define blsr16(_x_) blsr32(_x_)

#define EFE(__x,__i,__start) ((__x[__i] - __start)-(__i)*EF_INC)

#define BITPACK bitpack
#define BITUNPACK bitunpack
#define EF_INC 1
#define EFANOENC efano1enc
#define EFANODEC efano1dec

#define USIZE 32
#include "eliasfano.c"
#undef USIZE

/*#define USIZE 16
#include "eliasfano.c"
#undef USIZE*/

#undef EF_INC
#undef EFANOENC
#undef EFANODEC

//----------
#define EF_INC 0
#define EFANOENC efanoenc
#define EFANODEC efanodec

#define USIZE 32
#include "eliasfano.c"
#undef USIZE

#define USIZE 64
#include "eliasfano.c"
#undef USIZE

/*#define USIZE 16
#include "eliasfano.c"
#undef USIZE*/

#undef BITPACK
#undef BITUNPACK

#undef EF_INC
#undef EFANOENC
#undef EFANODEC

//----------------------
  #if defined(__SSE2__) || defined(__ARM_NEON)
#define VSIZE 128

#define BITPACK   bitpack128v
#define BITUNPACK bitunpack128v
#define EF_INC 1
#define EFANOENC  efano1enc128v
#define EFANODEC  efano1dec128v

#define USIZE 32
#include "eliasfano.c"

#define EF_INC 0
#define EFANOENC  efanoenc128v
#define EFANODEC  efanodec128v

#include "eliasfano.c"
  #endif

  #ifdef __AVX2__
#define VSIZE 256
#define BITPACK bitpack256v
#define BITUNPACK bitunpack256v
#define EF_INC 1
#define EFANOENC efano1enc256v
#define EFANODEC efano1dec256v
#include "eliasfano.c"

#define EF_INC 0
#define EFANOENC efanoenc256v
#define EFANODEC efanodec256v
#include "eliasfano.c"
  #endif

#else //--------------------------------------------- implementation ---------------------------------------------------------------
#define uint_t TEMPLATE3(uint, USIZE, _t)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wparentheses"

unsigned char *TEMPLATE2(EFANOENC, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out, uint_t start) {
  uint_t *ip, e,x,hl,i;
  unsigned char *op;
  unsigned lb;
  uint_t _pa[1024+64],*pa=_pa;
  if(!n) return out;
  if(n > 1024) pa = malloc(sizeof(pa[0])*(n+64));    if(!pa) die("efanoenc:malloc error size=%d ", n);
  e = EFE(in,n-1,start);
  if(!e) { out[0] = 0; if(pa != _pa) free(pa);return out+1; }

  lb = TEMPLATE2(bsr, USIZE)(e/n);
  x = ((uint_t)1 << lb)-1; hl = PAD8((e>>lb)+n);

  for(i = 0; i != n&~3;) {
    pa[i] = EFE(in,i,start) & x; ++i;
    pa[i] = EFE(in,i,start) & x; ++i;
    pa[i] = EFE(in,i,start) & x; ++i;
    pa[i] = EFE(in,i,start) & x; ++i;
  }
  while(i < n) pa[i] = EFE(in,i,start) & x, ++i;
  *out = lb+1;
  op = TEMPLATE2(BITPACK,USIZE)(pa, n, out+1, lb);

  memset(op, 0, hl);
  for(i = 0; i != n&~3; ) {
    x = i + (EFE(in,i,start) >> lb), op[x >> 3] |= (uint_t)1 << (x & 7); ++i;
    x = i + (EFE(in,i,start) >> lb), op[x >> 3] |= (uint_t)1 << (x & 7); ++i;
    x = i + (EFE(in,i,start) >> lb), op[x >> 3] |= (uint_t)1 << (x & 7); ++i;
    x = i + (EFE(in,i,start) >> lb), op[x >> 3] |= (uint_t)1 << (x & 7); ++i;
  }
  while(i < n) x = i + (EFE(in,i,start) >> lb), op[x >> 3] |= (uint_t)1 << (x & 7),++i;
  if(pa != _pa) free(pa);
  return op+hl;
}

unsigned char *TEMPLATE2(EFANODEC, USIZE)(unsigned char *__restrict in, unsigned n, uint_t *__restrict out, uint_t start) {
  unsigned char *ip = in;
  uint_t        i,j,lb = *ip++;
  uint64_t      b,x;
  if(!n)
    return in;

  if(!lb) {
      #if (defined(__SSE2__) || defined(__ARM_NEON)) && USIZE == 32
        #if EF_INC == 1
    BITFORZERO32(out, n, start, 1);
        #else
    BITZERO32( out, n, start);
        #endif
      #else
    BITFORSET_(out, n, start, EF_INC);
      #endif
    return ip;
  }

  ip = TEMPLATE2(BITUNPACK,USIZE)(ip, n, out, --lb);
  #define EFD(i) if(!b) break; out[i] += ((uint_t)(j+ctz64(b)-i) << lb) + start+i*EF_INC; b = blsr64(b); ++i;

  for(i=j=0;; j += sizeof(uint64_t)*8) {                                            //PREFETCH(ip+256,0);
    for(b = ctou64(ip+(j>>3)); ; ) {
      EFD(i); EFD(i); EFD(i); EFD(i);
      if(!b) break; out[i] += ((uint_t)(j+ctz64(b)-i) << lb) + start+i*EF_INC;
      if(unlikely(++i >= n))
        goto e;
      b = blsr64(b);
    }
  }
  e:return ip + PAD8((EFE(out,n-1,start)>>lb)+n);
}

#pragma clang diagnostic pop
#endif
