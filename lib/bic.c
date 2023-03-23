/**
    Copyright (C) powturbo 2019-2023
    SPDX-License-Identifier: GPL v2 License

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

    - email    : powturbo [AT] gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo
**/
// Binary Interpolative Coding
// Reference: "On Implementing the Binary Interpolative Coding Algorithm" GIULIO ERMANNO PIBIRI, ISTI-CNS http://pages.di.unipi.it/pibiri/papers/BIC.pdf
//            "Techniques for Inverted Index Compression" GIULIO ERMANNO PIBIRI, ROSSANO VENTURINI, University of Pisa https://arxiv.org/abs/1908.10598

#ifndef USIZE //---------- implementation --------------------------------------------------------------------------------------------------------------------------------------
#include "include_/conf.h"
#include "include_/bic.h"

#include "include_/bitutil_.h"

static ALWAYS_INLINE unsigned pow2next(unsigned x) { return x<2?1:(1ull << (__bsr32((x)-1)+1)); }

size_t bicbound16(size_t n) { return n*2+4; }
size_t bicbound32(size_t n) { return n*4+4; }
//-- Simple binary ----------------------------------------------------------------------
#define bicput(bw,br, _u_, _x_, _usize_) bitput(  bw,br, T2(__bsr,_usize_)(_u_) + 1, _x_)   /*AS(_u_ > 0, "Fatal bicput"); AS(_x_ <= _u_, "Fatal bicput2");*/
#define bicget(bw,br, _u_, _x_, _usize_) bitget57(bw,br, T2(__bsr,_usize_)(_u_) + 1, _x_)

//------------------------------------------
#define BICENC_ bicbenc_
#define BICDEC_ bicbdec_
#define BICENC  bicbenc
#define BICDEC  bicbdec

//---- 16 bits ----------
#define USIZE 16
#define uint_t  uint16_t
#include "bic.c"

//---- 32 bits ----------
#define USIZE 32
#define uint_t  uint32_t
#include "bic.c"
#undef bicput
#undef bicget
#undef BICENC_
#undef BICDEC_
#undef BICENC
#undef BICDEC

// -- Leftmost minimal ---------------------------------------------------------------------
#define bicput(bw,br, _u_, _x_, _usize_) { \
  unsigned _x = _x_, _u = _u_, _b = T2(__bsr,_usize_)(_u), hi = (1ull << (_b + 1)) - _u - 1;\
  if(_x < hi)      bitput(bw,br, _b,   _x);\
  else { _x += hi; bitput(bw,br, _b+1, (_x&1)<<_b | _x >> 1); }\
}

#define bicget(bw,br, _u_, _x_, _usize_) {\
  unsigned _u = _u_;\
  unsigned _b  = T2(__bsr,_usize_)(_u);\
  uint_t   _hi = (1ull << (_b + 1)) - _u - 1;\
  if((_x_ = bitpeek57(bw,br,_b)) < _hi) bitrmv(bw,br,_b);\
  else { \
    unsigned _y = (bitbw(bw,br)>>_b)&1;\
    bitrmv(bw,br,_b+1);\
	_x_= (_x_<<1) + _y - _hi;\
  }\
}

//--------------------------------------------
#define BICENC_ bicenc_
#define BICDEC_ bicdec_
#define BICENC  bicenc
#define BICDEC  bicdec

//---- 16 bits ----------
#define USIZE 16
#define uint_t  uint16_t
#include "bic.c"

//---- 32 bits ----------
#define USIZE 32
#define uint_t  uint32_t
#include "bic.c"
#undef bicput
#undef bicget
#undef BICENC_
#undef BICDEC_
#undef BICENC
#undef BICDEC

//-- Center Minimal -----------------------------------------------------
#define bicput(bw,br, _u_, _x_, _usize_) { \
  unsigned _x = _x_, _u = _u_, _b = T2(__bsr,_usize_)(_u); \
  uint64_t _c = (1ull << (_b + 1)) - _u - 1; \
  unsigned _c2 = _c >> 1, _r2 = _u >> 1, _lo = _r2-_c2, _hi = _r2+_c2+1;\
  if(!(_u & 1)) _lo -= 1; \
  _b += (_x <= _lo || _x >= _hi);\
  bitput(bw,br, _b, _x);\
}

#define bicget(bw,br, _u_, _x_, _usize_) { \
  unsigned _u = _u_, _b = T2(__bsr,_usize_)(_u);\
  uint64_t  _c = (1ull << (_b + 1)) - _u - 1;\
  unsigned _c2 = _c>>1, _r2 = _u>>1, _lo = _r2 - _c2;\
  _lo -= ((_u & 1) == 0);\
  if((_x_ = bitpeek57(bw,br,_b)) > _lo) bitrmv(bw,br,_b);\
  else bitget57(bw,br, _b+1, _x_);\
}

//--------------------------------------------
#define BICENC_ bicmenc_
#define BICDEC_ bicmdec_
#define BICENC  bicmenc
#define BICDEC  bicmdec

//---- 16 bits ----------
#define USIZE 16
#define uint_t  uint16_t
#include "bic.c"

//---- 32 bits ----------
#define USIZE 32
#define uint_t  uint32_t
#include "bic.c"


#else //-------------------- Template functions ----------------------------------------------------------------------------------------------------------

static void T2(BICENC_,USIZE)(uint_t *in, unsigned n, unsigned char **_op, unsigned lo, unsigned hi, unsigned h, uint64_t *bw, unsigned *br) {
  while(n)
    if(hi - lo + 1 != n) { 												//AC(lo <= hi,"bicenc fatal lo=%d>hi=%d n=%d\n", lo, hi, n); AS(hi - lo >= n - 1, "bicenc_32 fatal hi-lo>n-1\n");
      unsigned x = in[h];
	  bicput(*bw, *br, hi-n-lo+1, x-lo-h, USIZE);  bitenorm(*bw,*br,*_op);
      T2(BICENC_,USIZE)( in, h, _op, lo, x-1, h>>1, bw,br);
      in += h+1; n -= h+1; lo = x+1; h = n >> 1;
	} else break;
}

#define RE(a) //a  // recursion : RE(a) a
#define RD(a) a    // recursion : RD(a)
static void T2(BICDEC_,USIZE)(unsigned char **_ip, unsigned n, uint_t *out, unsigned lo, unsigned hi, unsigned h, uint64_t *bw, unsigned *br) {
  RE(if(!n) return);
  RD(do) {
    if(likely(hi - lo + 1 != n)) {						    //AS(lo <= hi, "bicdec fatal");
      unsigned x;
	  bitdnorm(*bw,*br,*_ip); bicget(*bw,*br, hi-lo+1-n, x, USIZE);
      out[h] = (x += lo + h);
      if(n != 1) {
           T2(BICDEC_,USIZE)(_ip,   h,   out,         lo,  x-1,       h>>1, bw,br);
        RE(T2(BICDEC_,USIZE)(_ip,n- h-1, out+ h+1,    x+1, hi,  (n-h-1)>>1, bw,br));
		RD(                      n-=h+1; out+=h+1; lo=x+1;        h = n>>1);
	  } RD(else break);
    } else {
	  BITFORSET_(out, n, lo, 1); 					//for(unsigned i = 0; i != n; ++i) out[i] = lo+i; //
	  RD(break);
    }
  } RD(while(n));
}

unsigned T2(BICENC,USIZE)(uint_t *in, unsigned n, unsigned char *out) {
  if(!n) return 0; 						//for(unsigned i = 1; i < n; i++) { AC(in[i]>in[i-1], "bicenc32: Not sorted at=%u,count=%d\n", i, n);  } //printf("n=%u ", n);printf("%u,", in[i]);
  bitdef(bw,br);
  unsigned char *op = out;
  unsigned      x = in[n-1];

  ctou32(op) = x; op += 4;
  T2(BICENC_,USIZE)(in, n-1, &op, 0, x, pow2next(n)>>1, &bw,&br);
  bitflush(bw,br,op);
  return op - out;
}

unsigned T2(BICDEC,USIZE)(unsigned char *in, unsigned n, uint_t *out) {
  if(!n) return 0;
  bitdef(bw,br);
  unsigned char *ip = in;
  unsigned      x = ctou32(ip);

  ip       += 4;
  out[n-1]  = x;
  T2(BICDEC_,USIZE)(&ip, n-1, out, 0, x, pow2next(n)>>1, &bw,&br);
  bitalign(bw,br,ip);
  return ip - in;
}

#undef USIZE
#undef uint_t
#endif
