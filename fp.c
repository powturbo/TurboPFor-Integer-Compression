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
#pragma warning( disable : 4005) 
#pragma warning( disable : 4090) 
#pragma warning( disable : 4068) 

#include "conf.h"  
#include "vp4.h"
#include "fp.h"
#include "bitutil.h"
#define VSIZE 128

// Unlike almost floating point compressors, we are using the better zigzag encoding instead the XOR technique.
//#define ENC64(u,h) ((u)^(h))
//#define DEC64(u,h) ((u)^(h))
#define ENC64(u,h) zigzagenc64((int64_t)u-(int64_t)h)
#define DEC64(u,h) zigzagdec64(u)+(int64_t)h

//---- Last value Predictor 
size_t fppenc64(uint64_t *in, size_t n, unsigned char *out, uint64_t start) {
  uint64_t       _p[VSIZE+32], *ip, *p;
  unsigned char *op = out;

  #define FE64(i) { uint64_t u = ip[i]; p[i] = ENC64(u, start); start = u; } 
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE64(0); FE64(1); FE64(2); FE64(3); }  
	op = p4enc64(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = (in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) FE64(0);
    op = p4enc64(_p, n, op);
  }
  return op - out;
}

size_t fppdec64(unsigned char *in, size_t n, uint64_t *out, uint64_t start) {
  uint64_t      *op, _p[VSIZE+32],*p; 
  unsigned char *ip = in;

  #define FD64(i) { uint64_t u = DEC64(p[i], start); op[i] = u; start = u; }
  for(op = out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = p4dec64(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD64(0); FD64(1); FD64(2); FD64(3); }
  }
  if(n = (out+n) - op)
	for(ip = p4dec64(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD64(0);
  return ip - in;
}

// delta of delta 
size_t fpddenc64(uint64_t *in, size_t n, unsigned char *out, uint64_t start) {
  uint64_t       _p[VSIZE+32], *ip, *p, pd = 0;
  unsigned char *op = out;
  
  #define FE64(i) { uint64_t u = ip[i]; start = u-start; p[i] = ENC64(start,pd); pd = start; start = u; }
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE64(0); FE64(1); FE64(2); FE64(3); }  
	op = p4enc64(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = (in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) FE64(0);
    op = p4enc64(_p, n, op);
  }
  return op - out;
}

size_t fpdddec64(unsigned char *in, size_t n, uint64_t *out, uint64_t start) {
  uint64_t       _p[VSIZE+32],*p, *op, pd=0; 
  unsigned char *ip = in;

  #define FD64(i) { uint64_t u = DEC64(p[i],start+pd); op[i] = u; pd = u - start; start = u; }
  for(op = out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = p4dec64(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD64(0); FD64(1); FD64(2); FD64(3); }
  }
  if(n = (out+n) - op)
	for(ip = p4dec64(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD64(0);
  return ip - in;
}

#define HBITS 13 //15
#define HASH64(_h_,_u_) (((_h_)<<5 ^ (_u_)>>50) & ((1u<<HBITS)-1))
//---- FCM: Finite Context Method Predictor 
size_t fpfcmenc64(uint64_t *in, size_t n, unsigned char *out, uint64_t start) {
  uint64_t       htab[1<<HBITS] = {0}, _p[VSIZE+32], *ip, h = 0, *p;
  unsigned char *op = out;
  
  #define FE64(i) { uint64_t u = ip[i]; p[i] = ENC64(u, htab[h]); htab[h] = u; h = HASH64(h,u); } 
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {   		
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE64(0); FE64(1); FE64(2); FE64(3); }  
	op = p4enc64(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = (in+n)-ip) {
    for(p = _p; p != &_p[n]; p++,ip++) FE64(0);
    op = p4enc64(_p, n, op);
  }
  return op - out;
}

size_t fpfcmdec64(unsigned char *in, size_t n, uint64_t *out, uint64_t start) {
  uint64_t *op, htab[1<<HBITS] = {0}, h = 0, _p[VSIZE+32],*p; 
  unsigned char *ip = in;

  #define FD64(i) { uint64_t u = DEC64(p[i], htab[h]); op[i] = u; htab[h] = u; h = HASH64(h,u); }
  for(op = (uint64_t*)out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = p4dec64(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD64(0); FD64(1); FD64(2); FD64(3); }
  }
  if(n = ((uint64_t *)out+n) - op)
	for(ip = p4dec64(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD64(0);
  return ip - in;
}

// DFCM: Differential Finite Context Method Predictor 
size_t fpdfcmenc64(uint64_t *in, size_t n, unsigned char *out, uint64_t start) {
  uint64_t *ip, _p[VSIZE+32], h = 0, *p, htab[1<<HBITS] = {0};
  unsigned char *op = out;
  
  #define DE64(i) { uint64_t u = ip[i]; p[i] = ENC64(u, (htab[h]+start)); htab[h] = start = u - start; h = HASH64(h,start); start = u; } 
  for(ip = in; ip != in + (n&~(VSIZE-1)); ) {  		
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { DE64(0); DE64(1); DE64(2); DE64(3); }  
	op = p4enc64(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }  
  if(n = (in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) DE64(0);
    op = p4enc64(_p, n, op);
  }																	
  return op - out;
}  

size_t fpdfcmdec64(unsigned char *in, size_t n, uint64_t *out, uint64_t start) {
  uint64_t      _p[VSIZE+32], *op, h = 0, *p, htab[1<<HBITS] = {0}; 
  unsigned char *ip = in;

  #define DD64(i) { uint64_t u = DEC64(p[i], (htab[h]+start)); op[i] = u; htab[h] = start = u-start; h = HASH64(h,start); start = u; }
  for(op = (uint64_t*)out; op != out+(n&~(VSIZE-1)); ) { 							__builtin_prefetch(ip+512, 0);
	for(ip = p4dec64(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { DD64(0); DD64(1); DD64(2); DD64(3); }
  }
  if(n = ((uint64_t *)out+n) - op)
	for(ip = p4dec64(ip, n, _p), p = _p; p != &_p[n]; p++,op++) DD64(0);
  return ip - in;
}

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

#define bitput64(bw,br,_b_,_x_,_op_) if((_b_)>45) {              bitput(bw,br,(_b_)-32, (_x_)>>32); bitenorm(bw,br,_op_); bitput(bw,br,32,(unsigned)(_x_));      } else bitput(bw,br,_b_,_x_);
#define bitget64(bw,br,_b_,_x_,_ip_) if((_b_)>45) { unsigned _v; bitget(bw,br,(_b_)-32,_x_);        bitdnorm(bw,br,_ip_); bitget(bw,br,32,_v); _x_ = _x_<<32|_v; } else bitget(bw,br,_b_,_x_); 
	
// Fastest Gorilla (see Facebook paper) Floating point/Integer compression implementation using zigzag encoding instead of XOR. Compression 5 GB/s, Decompression: 10 GB/s
size_t fpgenc64(uint64_t *in, size_t n, unsigned char *out, uint64_t start) {
  uint64_t      *ip;
  unsigned       ol = 0,ot = 0;
  unsigned char *op = out;
  bitdef(bw,br); 
  #define FE64(i) { uint64_t z = ENC64(ip[i], start); start = ip[i];\
    if(likely(!z))             bitput( bw,br, 1, 1);\
    else { unsigned t = ctz64(z), l = clz64(z); l = l>31?31:l;\
      if(l >= ol && t >= ot) { bitput( bw,br, 2, 2);               l = 64 - ol - ot; z>>=ot; bitput64(bw,br,  l, z,op); }\
      else {                   bitput( bw,br, 2+6+5, (t-1)<<5|l); ol = 64 -  l -  t; z>>= t; bitput64(bw,br, ol, z,op); ol = l; ot = t; } \
    } bitenorm(bw,br,op);\
  }
  for(ip = in; ip != in + (n&~(4-1)); ip+=4) { __builtin_prefetch(ip+512, 0); FE64(0); FE64(1); FE64(2); FE64(3); }
  for(       ; ip != in +  n        ; ip++ ) FE64(0); 
  bitflush(bw,br,op);
  return op - out;
}

size_t fpgdec64(unsigned char *in, size_t n, uint64_t *out, uint64_t start) { if(!n) return 0;
  uint64_t      *op;
  unsigned       ol = 0,ot = 0,x;
  unsigned char *ip = in;
  bitdef(bw,br);
  
  #define FD64(i) { uint64_t z=0; unsigned _x; bitget(bw,br,1,_x); \
    if(likely(!_x)) { bitget(bw,br,1,_x);\
	  if(!_x) { bitget(bw,br,11,_x); ot = (_x>>5)+1; ol = _x & 31; } bitget64(bw,br,64 - ol - ot,z,ip); z<<=ot;\
    } op[i] = start = DEC64(z, start); bitdnorm(bw,br,ip);\
  }
  for(bitdnorm(bw,br,ip),op = out; op != out+(n&~(4-1)); op+=4) { FD64(0); FD64(1); FD64(2); FD64(3); __builtin_prefetch(ip+512, 0); }
  for(        ; op != out+n; op++) FD64(0);
  bitalign(bw,br,ip);
  return ip - in;
}

// Improved Gorilla style compression with sliding double delta for timestamps in time series.
// Up to 300 times better compression and several times faster
#define N2  6 // for seconds time series
#define N3 10
#define N4 17 // must be > 16 
#define NL 18

#define ENC32(_pp_, _ip_, _d_, _op_) do {\
  size_t _r = _ip_ - _pp_;\
  if(_r > NL) { _r -= NL; unsigned _b = (bsr32(_r)+7)>>3; bitput(bw,br,4+3+3,(_b-1)<<(4+3)); bitput64(bw,br,_b<<3, _r, _op_); bitenorm(bw,br,_op_); }\
  else while(_r--) { bitput(bw,br,1,1); bitenorm(bw,br,_op_); }\
  _d_ = zigzagenc32(_d_);\
       if(!_d_)                bitput(bw,br,    1,      1);\
  else if(_d_ <  (1<< (N2-1))) bitput(bw,br, N2+2,_d_<<2|2);\
  else if(_d_ <  (1<< (N3-1))) bitput(bw,br, N3+3,_d_<<3|4);\
  else if(_d_ <  (1<< (N4-1))) bitput(bw,br, N4+4,_d_<<4|8);\
  else { unsigned _b = (bsr32(_d_)+7)>>3; bitput(bw,br,4+3,(_b-1)<<4); bitput(bw,br, _b<<3, _d_); }\
  bitenorm(bw,br,_op_);\
} while(0)

#define OVERFLOW if(op >= out_) { *out++ = 1<<4; /*bitini(bw,br); bitput(bw,br,4+3,1<<4); bitflush(bw,br,out);*/ memcpy(out,in,n*sizeof(in[0])); return 1+n*sizeof(in[0]); }

size_t bitgenc32(uint32_t *in, size_t n, unsigned char *out, uint32_t start) {
  uint32_t *ip = in, pd = 0, *pp = in,dd;
  unsigned char *op = out, *out_ = out+n*sizeof(in[0]);

  bitdef(bw,br);
  if(n > 4)
    for(; ip < in+(n-1-4);) { 
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++; 	__builtin_prefetch(ip+256, 0);
      continue;
      a:;
      ENC32(pp,ip, dd, op); OVERFLOW;
	  pp = ++ip;								
    }

  for(;ip < in+n;) {
    start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto b; ip++;
    continue;
    b:;
    ENC32(pp,ip, dd, op); OVERFLOW;
	pp = ++ip;								
  }
  if(ip > pp) {
    start = ip[0] - start; dd = start-pd;
    ENC32(pp, ip, dd, op); OVERFLOW;
  }
  bitflush(bw,br,op);
  return op - out;
}

size_t bitgdec32(unsigned char *in, size_t n, uint32_t *out, uint32_t start) { if(!n) return 0;
  uint32_t *op = out, pd = 0;
  unsigned char *ip = in;
  bitdef(bw,br);

  for(bitdnorm(bw,br,ip); op < out+n; ) { 															__builtin_prefetch(ip+384, 0);
    uint32_t dd = bitpeek(bw,br);
         if(dd & 1) bitrmv(bw,br, 0+1), dd = 0;						
	else if(dd & 2) bitrmv(bw,br,N2+2), dd = _bzhi_u32(dd>>2, N2);
	else if(dd & 4) bitrmv(bw,br,N3+3), dd = _bzhi_u32(dd>>3, N3);
	else if(dd & 8) bitrmv(bw,br,N4+4), dd = _bzhi_u32(dd>>4, N4);
	else { 								
	  unsigned b,*_op; uint64_t r; 
	  bitget(bw,br, 4+3, b);
      if((b>>=4) <= 1) { 
        if(b==1) { 								// No compression, because of overflow
		  memcpy(out,in+1, n*sizeof(out[0])); 
		  return 1+n*sizeof(out[0]); 
		}
		bitget(bw,br,3,b); bitget64(bw,br,(b+1)<<3,r,ip); bitdnorm(bw,br,ip); // RLE
		  #ifdef __SSE2__
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
      bitget(bw,br,(b+1)<<3,dd);
    }
	pd += zigzagdec32(dd); 
	*op++ = (start += pd); 
	bitdnorm(bw,br,ip);
  }	
  bitalign(bw,br,ip);
  return ip - in;
}

#define N2  6 // for seconds/milliseconds,... time series
#define N3 12
#define N4 20 // must be > 16

#define ENC64(_pp_, _ip_, _d_, _op_) do {\
  uint64_t _r = _ip_ - _pp_;\
  if(_r > NL) { _r -= NL; unsigned _b = (bsr64(_r)+7)>>3; bitput(bw,br,4+3+3,(_b-1)<<(4+3)); bitput64(bw,br,_b<<3, _r, _op_); bitenorm(bw,br,_op_); }\
  else while(_r--) { bitput(bw,br,1,1); bitenorm(bw,br,_op_); }\
  _d_ = zigzagenc64(_d_);\
       if(!_d_)                bitput(bw,br,    1,      1);\
  else if(_d_ <  (1<< (N2-1))) bitput(bw,br, N2+2,_d_<<2|2);\
  else if(_d_ <  (1<< (N3-1))) bitput(bw,br, N3+3,_d_<<3|4);\
  else if(_d_ <  (1<< (N4-1))) bitput(bw,br, N4+4,_d_<<4|8);\
  else { unsigned _b = (bsr64(_d_)+7)>>3; bitput(bw,br,4+3,(_b-1)<<4); bitput64(bw,br, _b<<3, _d_,_op_); }\
  bitenorm(bw,br,_op_);\
} while(0)

size_t bitgenc64(uint64_t *in, size_t n, unsigned char *out, uint64_t start) {
  uint64_t *ip = in, pd = 0, *pp = in,dd;
  unsigned char *op = out, *out_ = out+n*sizeof(in[0]);

  bitdef(bw,br);
  if(n > 4)
    for(; ip < in+(n-1-4);) { 
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++;
      start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto a; ip++; 	__builtin_prefetch(ip+256, 0);
      continue;
      a:;
      ENC64(pp,ip, dd, op); OVERFLOW;
	  pp = ++ip;								
    }

  for(;ip < in+n;) {
    start = ip[0] - start; dd = start-pd; pd = start; start = ip[0]; if(dd) goto b; ip++;
    continue;
    b:;
    ENC64(pp,ip, dd, op); OVERFLOW;
	pp = ++ip;								
  }
  if(ip > pp) {
    start = ip[0] - start; dd = start-pd;
    ENC64(pp, ip, dd, op); OVERFLOW;
  }
  bitflush(bw,br,op);
  return op - out;
}

size_t bitgdec64(unsigned char *in, size_t n, uint64_t *out, uint64_t start) { if(!n) return 0;
  uint64_t *op = out, pd = 0;
  unsigned char *ip = in;

  bitdef(bw,br);
  for(bitdnorm(bw,br,ip); op < out+n; ) { 															__builtin_prefetch(ip+384, 0);
    uint64_t dd = bitpeek(bw,br);
         if(dd & 1) bitrmv(bw,br, 0+1), dd = 0;
	else if(dd & 2) bitrmv(bw,br,N2+2), dd = _bzhi_u64(dd>>2, N2);
	else if(dd & 4) bitrmv(bw,br,N3+3), dd = _bzhi_u64(dd>>3, N3);
	else if(dd & 8) bitrmv(bw,br,N4+4), dd = _bzhi_u64(dd>>4, N4);
	else { 
	  unsigned b; uint64_t r,*_op; 
	  bitget(bw,br, 4+3, b);
      if((b>>=4) <= 1) { // No compression, because of overflow
        if(b==1) { 
          memcpy(out,in+1, n*sizeof(out[0])); 
          return 1+n*sizeof(out[0]); 
        }
		bitget(bw,br,3,b); bitget64(bw,br,(b+1)*8,r,ip); bitdnorm(bw,br,ip); //RLE     //r+=NL; while(r--) *op++=(start+=pd);
		for(r+=NL, _op = op; op != _op+(r&~7); op += 8) 
		  op[0]=(start+=pd),
	      op[1]=(start+=pd),
		  op[2]=(start+=pd),
		  op[3]=(start+=pd),
		  op[4]=(start+=pd),
		  op[5]=(start+=pd),
		  op[6]=(start+=pd),
		  op[7]=(start+=pd);
		for(; op != _op+r; op++) 
		  *op = (start+=pd);
		continue;
      } 
      bitget64(bw,br,(b+1)<<3,dd,ip);
    }
	pd += zigzagdec64(dd); 
	*op++ = (start += pd); 
	bitdnorm(bw,br,ip);
  }	
  bitalign(bw,br,ip);
  return ip - in;
}

#if 0
// Initial implementation without RLE
#define N2  7 // for seconds time series
#define N3  9
#define N4 12  
size_t bitg0enc32(uint32_t *in, size_t n, unsigned char *out, uint32_t start) {
  uint32_t      *ip, pd = 0;
  unsigned char *op = out;
  bitdef(bw,br);
 
  #define FE32(i) { uint32_t dd; start = ip[i] - start; dd = start-pd; pd = start; dd = zigzagenc32(dd); start = ip[i];\
         if(!dd)                bitput(bw,br,    1,      1);\
    else if(dd <  (1<< (N2-1))) bitput(bw,br, N2+2,dd<<2|2);\
    else if(dd <  (1<< (N3-1))) bitput(bw,br, N3+3,dd<<3|4);\
	else if(dd <  (1<< (N4-1))) bitput(bw,br, N4+4,dd<<4|8);\
	else { unsigned _b = (bsr32(dd)+7)>>3; bitput(bw,br,4+2,(_b-1)<<4); bitput(bw,br, _b<<3, dd); }\
	bitenorm(bw,br,op);\
  }
  for(ip = in; ip != in + (n&~(4-1)); ip+=4) { __builtin_prefetch(ip+512, 0); FE32(0); FE32(1); FE32(2); FE32(3); }
  for(       ; ip != in +  n        ; ip++ ) FE32(0); 
  bitflush(bw,br,op);
  return op - out;
}

size_t bitg0dec32(unsigned char *in, size_t n, uint32_t *out, uint32_t start) { if(!n) return 0;
  uint32_t      *op, pd = 0; 
  unsigned char *ip = in;

  bitdef(bw,br);
  
  #define FD32(i) { uint32_t dd = bitpeek(bw,br);\
         if(dd & 1) bitrmv(bw,br, 1+0), dd = 0;\
	else if(dd & 2) bitrmv(bw,br,N2+2), dd = _bzhi_u32(dd>>2, N2);\
	else if(dd & 4) bitrmv(bw,br,N3+3), dd = _bzhi_u32(dd>>3, N3);\
	else if(dd & 8) bitrmv(bw,br,N4+4), dd = _bzhi_u32(dd>>4, N4);\
	else { unsigned _b; bitget(bw,br,4+2,_b); bitget(bw,br,((_b>>4)+1)*8,dd); }\
	pd += zigzagdec32(dd); op[i] = (start += pd); bitdnorm(bw,br,ip);\
  }
  for(bitdnorm(bw,br,ip),op = out; op != out+(n&~(4-1)); op+=4) { FD32(0); FD32(1); FD32(2); FD32(3); __builtin_prefetch(ip+512, 0); }
  for(; op != out+n; op++) FD32(0);
  bitalign(bw,br,ip);
  return ip - in;
}

#define N2  6 // for seconds/milliseconds,... time series
#define N3 12
#define N4 20
size_t bitg0enc64(uint64_t *in, size_t n, unsigned char *out, uint64_t start) {
  uint64_t      *ip, pd = 0;
  unsigned char *op = out;
  bitdef(bw,br);
  
  #define FE64(i) { uint64_t dd; start = (int64_t)ip[i] - (int64_t)start; dd = (int64_t)start-(int64_t)pd; pd = start; dd = zigzagenc64(dd); start = ip[i];\
         if(!dd)                bitput(bw,br,    1,      1);\
    else if(dd <  (1<< (N2-1))) bitput(bw,br, N2+2,dd<<2|2);\
    else if(dd <  (1<< (N3-1))) bitput(bw,br, N3+3,dd<<3|4);\
	else if(dd <  (1<< (N4-1))) bitput(bw,br, N4+4,dd<<4|8);\
	else { unsigned _b = (bsr64(dd)+7)>>3; bitput(bw,br,3+4,(_b-1)<<4); bitput64(bw,br, _b<<3, dd, op); }\
	bitenorm(bw,br,op);\
  }
  for(ip = in; ip != in + (n&~(4-1)); ip+=4) { __builtin_prefetch(ip+512, 0); FE64(0); FE64(1); FE64(2); FE64(3); }
  for(       ; ip != in +  n        ; ip++ ) FE64(0); 
  bitflush(bw,br,op);
  return op - out;
}

size_t bitg0dec64(unsigned char *in, size_t n, uint64_t *out, uint64_t start) { if(!n) return 0;
  uint64_t *op, pd = 0; 
  unsigned char *ip = in;
  bitdef(bw,br);
  
  #define FD64(i) { uint64_t dd = bitpeek(bw,br);\
         if(dd & 1) bitrmv(bw,br, 1+0), dd = 0;\
	else if(dd & 2) bitrmv(bw,br,N2+2), dd = _bzhi_u64(dd>>2, N2);\
	else if(dd & 4) bitrmv(bw,br,N3+3), dd = _bzhi_u64(dd>>3, N3);\
	else if(dd & 8) bitrmv(bw,br,N4+4), dd = _bzhi_u64(dd>>4, N4);\
	else { unsigned _b; bitget(bw,br,4+3,_b); _b = ((_b>>4)+1)*8; bitget64(bw,br,_b,dd,ip); }\
	pd += zigzagdec64(dd); start += pd; op[i] = start; bitdnorm(bw,br,ip);\
  }
  for(bitdnorm(bw,br,ip),op = out; op != out+(n&~(4-1)); op+=4) { FD64(0); FD64(1); FD64(2); FD64(3); __builtin_prefetch(ip+512, 0); }
  for(; op != out+n; op++) FD64(0);
  bitalign(bw,br,ip);
  return ip - in;
}
#endif

