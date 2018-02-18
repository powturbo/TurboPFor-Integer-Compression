/**
    Copyright (C) powturbo 2013-2018
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
//    "Floating Point/Integer Compression" 
  #ifndef USIZE
#pragma warning( disable : 4005) 
#pragma warning( disable : 4090) 
#pragma warning( disable : 4068) 

#include "conf.h"  
#include "vp4.h"
#include "bitutil.h"
#include "fp.h"
// ------------------ bitio compression ---------------------------
#define bitdef(     _bw_,_br_)           uint64_t _bw_=0; unsigned _br_=0
#define bitini(     _bw_,_br_)           _bw_=_br_=0

#define bitput(     _bw_,_br_,_nb_,__x)  _bw_ += (uint64_t)(__x) << _br_, _br_ += (_nb_)
#define bitenorm(   _bw_,_br_,_op_)      ctou64(_op_) = _bw_; _op_ += (_br_>>3), _bw_ >>=(_br_&~7), _br_ &= 7
#define bitflush(   _bw_,_br_,_op_)      ctou64(_op_) = _bw_, _op_ += (_br_+7)>>3, _bw_=_br_=0

  #ifdef __AVX2__
#include <x86intrin.h>
  #else
#define _bzhi_u64(_u_, _b_)              ((_u_) & ((1ull<<(_b_))-1))
#define _bzhi_u32(_u_, _b_)              ((_u_) & ((1u  <<(_b_))-1))
  #endif

#define bitpeek32(  _bw_,_br_,_nb_)      _bzhi_u32(_bw_>>_br_, _nb_)
#define bitpeek64(  _bw_,_br_,_nb_)      _bzhi_u64(_bw_>>_br_, _nb_)
#define bitpeek(    _bw_,_br_)           (_bw_>>_br_)
#define bitrmv(     _bw_,_br_,_nb_)      _br_ += _nb_
#define bitget(     _bw_,_br_,_nb_,_x_)  _x_ = bitpeek64(_bw_, _br_, _nb_), bitrmv(_bw_, _br_, _nb_)

#define bitdnorm(   _bw_,_br_,_ip_)      _bw_ = ctou64(_ip_ += (_br_>>3)), _br_ &= 7
#define bitalign(   _bw_,_br_,_ip_)      (_ip_ += (_br_+7)>>3)

#define bitput8( bw,br,_b_,_x_,_op_) bitput(bw,br,_b_,_x_)
#define bitput16(bw,br,_b_,_x_,_op_) bitput(bw,br,_b_,_x_)
#define bitput32(bw,br,_b_,_x_,_op_) bitput(bw,br,_b_,_x_)
#define bitput64(bw,br,_b_,_x_,_op_) if((_b_)>45) { bitput(bw,br,(_b_)-32, (_x_)>>32); bitenorm(bw,br,_op_); bitput(bw,br,32,(unsigned)(_x_)); } else bitput(bw,br,_b_,_x_)

#define bitget8( bw,br,_b_,_x_,_ip_) bitget(bw,br,_b_,_x_)
#define bitget16(bw,br,_b_,_x_,_ip_) bitget(bw,br,_b_,_x_)
#define bitget32(bw,br,_b_,_x_,_ip_) bitget(bw,br,_b_,_x_)
#define bitget64(bw,br,_b_,_x_,_ip_) if((_b_)>45) { unsigned _v; bitget(bw,br,(_b_)-32,_x_); bitdnorm(bw,br,_ip_); bitget(bw,br,32,_v); _x_ = _x_<<32|_v; } else bitget(bw,br,_b_,_x_)
//-------------------------------
#define VSIZE 128

#define P4ENC p4enc
#define P4DEC p4dec

#define NL 18
#define N4 17 // must be > 16 

#define N2  3
#define N3  5
#define USIZE 8
#include "fp.c"

#define N2   6
#define N3  12
#define USIZE 16
#include "fp.c"

#define N2  6 // for seconds time series
#define N3 10
#define USIZE 32
#include "fp.c"

#define N2  6    // for seconds/milliseconds,... time series
#define N3 12
#define N4 20 	 // must be > 16
#define USIZE 64
#include "fp.c"

  #else
// Unlike almost floating point compressors, we are using the better zigzag encoding instead the XOR technique.
//#define ENCX(u,h,_usize_) ((u)^(h))
//#define DECX(u,h,_usize_) ((u)^(h))
#define ENCX(_u_ , _h_, _usize_)  TEMPLATE2(zigzagenc,_usize_)((_u_)-(_h_))
#define DECX(_u_ , _h_, _usize_) (TEMPLATE2(zigzagdec,_usize_)(_u_)+(_h_))

#define uint_t TEMPLATE3(uint, USIZE, _t)

//---- Last value Predictor  
size_t TEMPLATE2(fppenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t         _p[VSIZE+32], *ip, *p;
  unsigned char *op = out;

  #define FE(i,_usize_) { TEMPLATE3(uint, _usize_, _t) u = ip[i]; p[i] = ENCX(u, start,_usize_); start = u; } 
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }  
	op = TEMPLATE2(P4ENC,USIZE)(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = (in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) FE(0,USIZE);
    op = TEMPLATE2(P4ENC,USIZE)(_p, n, op);
  }
  return op - out;
}

size_t TEMPLATE2(fppdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t        *op, _p[VSIZE+32],*p; 
  unsigned char *ip = in;

  #define FD(i,_usize_) { TEMPLATE3(uint, USIZE, _t) u = DECX(p[i], start,_usize_); op[i] = u; start = u; }
  for(op = out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if(n = (out+n) - op)
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  return ip - in;
}

// delta of delta 
size_t TEMPLATE2(fpddenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t       _p[VSIZE+32], *ip, *p, pd = 0;
  unsigned char *op = out;
  
  #define FE(i,_usize_) { TEMPLATE3(uint, USIZE, _t) u = ip[i]; start = u-start; p[i] = ENCX(start,pd,_usize_); pd = start; start = u; }
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }  
	op = TEMPLATE2(P4ENC,USIZE)(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = (in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) FE(0,USIZE);
    op = TEMPLATE2(P4ENC,USIZE)(_p, n, op);
  }
  return op - out;
}

size_t TEMPLATE2(fpdddec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t       _p[VSIZE+32],*p, *op, pd=0; 
  unsigned char *ip = in;

  #define FD(i,_usize_) { TEMPLATE3(uint, USIZE, _t) u = DECX(p[i],start+pd,_usize_); op[i] = u; pd = u - start; start = u; }
  for(op = out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if(n = (out+n) - op)
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  return ip - in;
}

#define HBITS 13 //15
#define HASH64(_h_,_u_) (((_h_)<<5 ^ (_u_)>>50) & ((1u<<HBITS)-1))
#define HASH32(_h_,_u_) (((_h_)<<4 ^ (_u_)>>23) & ((1u<<HBITS)-1))
#define HASH16(_h_,_u_) (((_h_)<<3 ^ (_u_)>>12) & ((1u<<HBITS)-1))
#define HASH8( _h_,_u_) (((_h_)<<2 ^ (_u_)>> 5) & ((1u<<HBITS)-1))
//---- FCM: Finite Context Method Predictor 
size_t TEMPLATE2(fpfcmenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t       htab[1<<HBITS] = {0}, _p[VSIZE+32], *ip, h = 0, *p;
  unsigned char *op = out;
  
  #define FE(i,_usize_) { TEMPLATE3(uint, _usize_, _t) u = ip[i]; p[i] = ENCX(u, htab[h],_usize_); htab[h] = u; h = TEMPLATE2(HASH,_usize_)(h,u); } 
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {   		
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }  
	op = TEMPLATE2(P4ENC,USIZE)(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = (in+n)-ip) {
    for(p = _p; p != &_p[n]; p++,ip++) FE(0,USIZE);
    op = TEMPLATE2(P4ENC,USIZE)(_p, n, op);
  }
  return op - out;
}

size_t TEMPLATE2(fpfcmdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t *op, htab[1<<HBITS] = {0}, h = 0, _p[VSIZE+32],*p; 
  unsigned char *ip = in;

  #define FD(i,_usize_) { TEMPLATE3(uint, _usize_, _t) u = DECX(p[i], htab[h],_usize_); op[i] = u; htab[h] = u; h = TEMPLATE2(HASH,_usize_)(h,u); }
  for(op = (uint_t*)out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if(n = ((uint_t *)out+n) - op)
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  return ip - in;
}

// DFCM: Differential Finite Context Method Predictor 
size_t TEMPLATE2(fpdfcmenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t *ip, _p[VSIZE+32], h = 0, *p, htab[1<<HBITS] = {0};
  unsigned char *op = out;
  
  #define FE(i,_usize_) { TEMPLATE3(uint, _usize_, _t) u = ip[i]; p[i] = ENCX(u, (htab[h]+start),_usize_); htab[h] = start = u - start; h = TEMPLATE2(HASH,_usize_)(h,start); start = u; } 
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {  		
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }  
	op = TEMPLATE2(P4ENC,USIZE)(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }  
  if(n = (in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) FE(0,USIZE);
    op = TEMPLATE2(P4ENC,USIZE)(_p, n, op);
  }																	
  return op - out;
}  

size_t TEMPLATE2(fpdfcmdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) {
  uint_t      _p[VSIZE+32], *op, h = 0, *p, htab[1<<HBITS] = {0}; 
  unsigned char *ip = in;

  #define FD(i,_usize_) { TEMPLATE3(uint, _usize_, _t) u = DECX(p[i], (htab[h]+start),_usize_); op[i] = u; htab[h] = start = u-start; h = TEMPLATE2(HASH,_usize_)(h,start); start = u; }
  for(op = (uint_t*)out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); }
  }
  if(n = ((uint_t *)out+n) - op)
	for(ip = TEMPLATE2(P4DEC,USIZE)(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD(0,USIZE);
  return ip - in;
}

// Fastest Gorilla (see Facebook paper) Floating point/Integer compression implementation, using zigzag encoding instead of XOR. Compression, Decompression: > 10 GB/s
size_t TEMPLATE2(fpgenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip;
  unsigned       ol = 0,ot = 0;
  unsigned char *op = out;
  bitdef(bw,br); 
  
  #define FE(i,_usize_) { TEMPLATE3(uint, _usize_, _t) z = ENCX(ip[i], start,_usize_); start = ip[i];\
    if(likely(!z))             bitput( bw,br, 1, 1);\
    else { unsigned t = TEMPLATE2(ctz,_usize_)(z), l = TEMPLATE2(clz,_usize_)(z); l = l>31?31:l;\
      if(l >= ol && t >= ot) { bitput( bw,br, 2, 2);               l = _usize_ - ol - ot; z>>=ot; TEMPLATE2(bitput,_usize_)(bw,br,  l, z,op); }\
      else {                   bitput( bw,br, 2+6+5, (t-1)<<5|l); ol = _usize_ -  l -  t; z>>= t; TEMPLATE2(bitput,_usize_)(bw,br, ol, z,op); ol = l; ot = t; } \
    } bitenorm(bw,br,op);\
  }
  for(ip = in; ip != in + (n&~(4-1)); ip+=4) { __builtin_prefetch(ip+512, 0); FE(0,USIZE); FE(1,USIZE); FE(2,USIZE); FE(3,USIZE); }
  for(       ; ip != in +  n        ; ip++ ) FE(0,USIZE); 
  bitflush(bw,br,op);
  return op - out;
}

size_t TEMPLATE2(fpgdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) { if(!n) return 0;
  uint_t        *op;
  unsigned       ol = 0,ot = 0,x;
  unsigned char *ip = in;
  bitdef(bw,br);
  
  #define FD(i,_usize_) { TEMPLATE3(uint, _usize_, _t) z=0; unsigned _x; bitget(bw,br,1,_x); \
    if(likely(!_x)) { bitget(bw,br,1,_x);\
	  if(!_x) { bitget(bw,br,11,_x); ot = (_x>>5)+1; ol = _x & 31; } TEMPLATE2(bitget,_usize_)(bw,br,_usize_ - ol - ot,z,ip); z<<=ot;\
    } op[i] = start = DECX(z, start,_usize_); bitdnorm(bw,br,ip);\
  }
  for(bitdnorm(bw,br,ip),op = out; op != out+(n&~(4-1)); op+=4) { FD(0,USIZE); FD(1,USIZE); FD(2,USIZE); FD(3,USIZE); __builtin_prefetch(ip+512, 0); }
  for(        ; op != out+n; op++) FD(0,USIZE);
  bitalign(bw,br,ip);
  return ip - in;
}

// Improved Gorilla style compression with sliding double delta+zigzag encoding+RLE for timestamps in time series.
// Up to 300 times better compression and several times faster
#define OVERFLOW if(op >= out_) { *out++ = 1<<4; /*bitini(bw,br); bitput(bw,br,4+3,1<<4); bitflush(bw,br,out);*/ memcpy(out,in,n*sizeof(in[0])); return 1+n*sizeof(in[0]); }

size_t TEMPLATE2(bitgenc,USIZE)(uint_t *in, size_t n, unsigned char *out, uint_t start) {
  uint_t        *ip = in, pd = 0, *pp = in,dd;
  unsigned char *op = out, *out_ = out+n*sizeof(in[0]);

  bitdef(bw,br);
  #define FE(_pp_, _ip_, _d_, _op_,_usize_) do {\
    uint64_t _r = _ip_ - _pp_;\
    if(_r > NL) { _r -= NL; unsigned _b = (bsr64(_r)+7)>>3; bitput(bw,br,4+3+3,(_b-1)<<(4+3)); bitput64(bw,br,_b<<3, _r, _op_); bitenorm(bw,br,_op_); }\
    else while(_r--) { bitput(bw,br,1,1); bitenorm(bw,br,_op_); }\
    _d_ = TEMPLATE2(zigzagenc,_usize_)(_d_);\
         if(!_d_)                bitput(bw,br,    1,       1);\
    else if(_d_ <  (1<< (N2-1))) bitput(bw,br, N2+2,_d_<<2|2);\
    else if(_d_ <  (1<< (N3-1))) bitput(bw,br, N3+3,_d_<<3|4);\
    else if(_d_ <  (1<< (N4-1))) bitput(bw,br, N4+4,_d_<<4|8);\
    else { unsigned _b = (TEMPLATE2(bsr,_usize_)(_d_)+7)>>3; bitput(bw,br,4+3,(_b-1)<<4); TEMPLATE2(bitput,_usize_)(bw,br, _b<<3, _d_,_op_); }\
    bitenorm(bw,br,_op_);\
  } while(0)

  if(n > 4)
    for(; ip < in+(n-1-4);) { 
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++; 	__builtin_prefetch(ip+256, 0);
      continue;
      a:;
      FE(pp,ip, dd, op,USIZE); 
	  pp = ++ip;		OVERFLOW;						
    }

  for(;ip < in+n;) {
    start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto b; ip++;
    continue;
    b:;
    FE(pp,ip, dd, op,USIZE); 
	pp = ++ip; OVERFLOW;								
  }
  if(ip > pp) {
    start = ip[0] - start; dd = start-pd;
    FE(pp, ip, dd, op, USIZE); OVERFLOW;
  }
  bitflush(bw,br,op);
  return op - out;
}

size_t TEMPLATE2(bitgdec,USIZE)(unsigned char *in, size_t n, uint_t *out, uint_t start) { if(!n) return 0;
  uint_t *op = out, pd = 0;
  unsigned char *ip = in;

  bitdef(bw,br);
  for(bitdnorm(bw,br,ip); op < out+n; ) { 															__builtin_prefetch(ip+384, 0);
     #if USIZE == 64
    uint_t dd = bitpeek(bw,br);
	 #else
    uint32_t dd = bitpeek(bw,br);
	 #endif
         if(dd & 1) bitrmv(bw,br, 0+1), dd = 0;
	else if(dd & 2) bitrmv(bw,br,N2+2), dd = BZHI32(dd>>2, N2);
	else if(dd & 4) bitrmv(bw,br,N3+3), dd = BZHI32(dd>>3, N3);
	else if(dd & 8) bitrmv(bw,br,N4+4), dd = BZHI32(dd>>4, N4);
	else {
	  unsigned b; uint_t *_op; uint64_t r; 
	  bitget(bw,br, 4+3, b);
      if((b>>=4) <= 1) { 
        if(b==1) { 															 // No compression, because of overflow
          memcpy(out,in+1, n*sizeof(out[0])); 
          return 1+n*sizeof(out[0]); 
        }
		bitget(bw,br,3,b); bitget64(bw,br,(b+1)<<3,r,ip); bitdnorm(bw,br,ip);//RLE     //r+=NL; while(r--) *op++=(start+=pd);
		  #if defined(__SSE2__) && USIZE == 32
        __m128i sv = _mm_set1_epi32(start), cv = _mm_set_epi32(4*pd,3*pd,2*pd,1*pd); 
		for(r += NL, _op = op; op != _op+(r&~7);) {
		  sv = _mm_add_epi32(sv,cv); _mm_storeu_si128(op, sv); sv = _mm_shuffle_epi32(sv, _MM_SHUFFLE(3, 3, 3, 3)); op += 4; 
		  sv = _mm_add_epi32(sv,cv); _mm_storeu_si128(op, sv); sv = _mm_shuffle_epi32(sv, _MM_SHUFFLE(3, 3, 3, 3)); op += 4; 
		}
        start = (unsigned)_mm_cvtsi128_si32(_mm_srli_si128(sv,12));
		  #else
		for(r+=NL, _op = op; op != _op+(r&~7); op += 8) 
		  op[0]=(start+=pd),
	      op[1]=(start+=pd),
		  op[2]=(start+=pd),
		  op[3]=(start+=pd),
		  op[4]=(start+=pd),
		  op[5]=(start+=pd),
		  op[6]=(start+=pd),
		  op[7]=(start+=pd);
		  #endif
		for(; op != _op+r; op++) 
		  *op = (start+=pd);
		continue;
      } 
      TEMPLATE2(bitget,USIZE)(bw,br,(b+1)<<3,dd,ip);
    }
	pd += TEMPLATE2(zigzagdec,USIZE)(dd); 
	*op++ = (start += pd); 
	bitdnorm(bw,br,ip);
  }
  bitalign(bw,br,ip);
  return ip - in;
}
#undef USIZE
  #endif
  