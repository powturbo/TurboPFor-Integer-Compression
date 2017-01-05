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
//   "Integer Compression" Turbo PFor/PforDelta 
#ifndef USIZE
#include <stdint.h>
 
#include "conf.h"
#include "bitpack.h"
#include "vint.h"		//#include "vsimple.h"
#include "vp4c.h"

#define PAD8(_x_) ( (((_x_)+8-1)/8) )
//------------------------------------------
#define EXCEP 1 // Hybrid TurboPFor : 1=Variable byte (2=Vsimple, not used)
#define _P4BITS _p4bits
//-- Scalar
#define _P4ENC  _p4enc
#define  P4ENC   p4enc
//#define  P4NENC  p4dnenc
#define  BITPACK  bitpack

#define USIZE 16
#include __FILE__

#define USIZE 32
#include __FILE__

#define USIZE 64
#include __FILE__

#define EXCEP 0             // Direct access
#define _P4BITS _p4bitsx
#define _P4ENC  _p4encx
#define  P4ENC   p4encx

#define USIZE 16
#include __FILE__

#define USIZE 32
#include __FILE__

#undef _P4ENC
#undef  P4ENC
#undef  BITPACK 

#undef _P4BITS

  #ifndef NSIMD
#define EXCEP 1 // 
    #ifdef __SSE2__
//-- SIMD: Vertical bitpacking
#define VSIZE 128 
#define _P4ENC    _p4enc128v
#define  P4ENC     p4enc128v
//#define  P4NENC   p4dnencv
#define  BITPACK   bitpack128v
#define USIZE 32
#include __FILE__
#undef  _P4ENC    
#undef   P4ENC    
#undef   BITPACK
    #endif

    #ifdef __AVX2__
#define VSIZE 256
#define _P4ENC    _p4enc256v
#define  P4ENC     p4enc256v
#define  BITPACK   bitpack256v
#define USIZE 32
#include __FILE__
#undef  _P4ENC    
#undef   P4ENC    
#undef   BITPACK
    #endif
  #endif

#undef USIZE

#else
#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wparentheses"

#define uint_t TEMPLATE3(uint, USIZE, _t)

#define VSC(a)
  #ifdef _P4BITS 							
unsigned TEMPLATE2(_P4BITS, USIZE)(uint_t *__restrict in, unsigned n, unsigned *pbx) {
  uint_t *ip; int b=0,r; int i,ml,l; 
  unsigned x, bx, cnt[USIZE+1] = {0}, _vb[USIZE*2+5] = {0}, *vb=&_vb[USIZE],fx=0, b64=(n+7)/8;
    #if EXCEP >= 2
  unsigned long long smap[USIZE+1][P4D_MAX/64]={0},xmap[P4D_MAX/64]={0}; unsigned c; 
    #endif
	
  #define CNTE { ++cnt[r=TEMPLATE2(bsr, USIZE)(*ip)]; VSC(c = ip-in; smap[r][c>>6] |= (1ull << (c&0x3f))); b |= *ip++; }
  for(ip = in; ip != in+(n&~3); ) { CNTE; CNTE; CNTE; CNTE; }
  while(ip != in+n) CNTE;

  b  = TEMPLATE2(bsr, USIZE)(b);
  bx = b; 
  ml = PAD8(n*b)+1; x = cnt[b];
 
    #if EXCEP == 1
#define VBB(_x_,_b_) vb[_b_-7]+=_x_; vb[_b_-15]+=_x_*2; vb[_b_-19]+=_x_*3; vb[_b_-25]+=_x_*4;
  int vv = x; VBB(x,b); 
    #elif EXCEP == 2
  for(c = 0; c < (n+63)/64;c++) xmap[c] = smap[b][c];
    #else
  ml -= 2+b64;
	#endif
  for(i = b-1; i >= 0; --i) { 
      #if EXCEP == 1
        l = PAD8(n*i) + 2+b64 + PAD8(x*(bx-i)); 
    int v = PAD8(n*i) + 2 + x + vv, vx = 0; 
	x += cnt[i]; vv+=cnt[i]+vb[i]; VBB(cnt[i],i);
    if(v < l) l=v,vx=1; if(unlikely(l < ml)) ml=l,b=i,fx=vx; 
	  #elif EXCEP == 2
	uint_t tin[P4D_MAX],*tp=tin;
	for(ip=in,c = 0; c < (n+63)/64;c++,ip+=64) {
      unsigned long long z = xmap[c];
      while(z) { unsigned x = ctz64(z); *tp++ = ip[x]; z ^= (1ull<<x); }
	}
	unsigned char out[2048],*op = TEMPLATE2(vsenc, USIZE)(tin, n, out); unsigned vv = op - out;
        l = PAD8(n*i) + PAD8(x*(bx-i))+2+b64; 
    int v = PAD8(n*i) + 2 + x + vv, vx = 0; 
    if(v < l) l=v,vx=1; if(unlikely(l < ml)) ml=l,b=i,fx=vx; 
	x += cnt[i]; 
    for(c = 0; c < (n+63)/64;c++) xmap[c] |= smap[i][c];
      #else 
    l = PAD8(n*i) + PAD8(x*(bx-i)); x += cnt[i]; unlikely(l < ml)?(ml=l,b=i):(ml=ml,b=b);
	  #endif
  } 																						//fx = 0;
    #if EXCEP > 0
  *pbx = fx?(USIZE+1):(bx - b);
    #else
  *pbx = bx - b;
    #endif
  return b;
} 
  #endif
 
unsigned char *TEMPLATE2(_P4ENC, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx) {
  if(!bx) 
    return TEMPLATE2(BITPACK, USIZE)(in, n,	out, b);

  uint_t             msk = (1ull << b)-1,_in[P4D_MAX], inx[P4D_MAX*2];
  unsigned long long xmap[P4D_MAX/64] = { 0 };  
  unsigned           miss[P4D_MAX],i, xn, c;
  #define MISS { miss[xn] = i; xn += in[i] > msk; _in[i] = in[i] & msk; i++; }
  for(xn = i = 0; i != n&~3; ) { MISS; MISS; MISS; MISS; }
  while(i != n) MISS;
  
  for(i = 0; i != xn; ++i) {
    c           = miss[i]; 
    xmap[c>>6] |= (1ull << (c&0x3f)); 
    inx[i]      = in[c] >> b; 
  }	
  unsigned char *_out = out;	  
    #if EXCEP > 0
  if(bx <= USIZE) { 														
    #endif
    for(i = 0; i < (n+63)/64; i++) ctou64(out+i*8) = xmap[i]; out += PAD8(n); 	
    out = TEMPLATE2(bitpack, USIZE)(inx, xn, out, bx);  	 	        					
    out = TEMPLATE2(BITPACK, USIZE)(_in, n,  out, b);  
    #if EXCEP > 0
  } 
  else { 															
    *out++ = xn; 
    out = TEMPLATE2(BITPACK, USIZE)(_in, n, out,  b);
    out = TEMPLATE2(vbenc, USIZE)(inx, xn, out); 				
    for(i = 0; i != xn; ++i) *out++ = miss[i]; 							
  }	  
    #endif
  return out;
}

unsigned char *TEMPLATE2(P4ENC, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out) {  
  unsigned bx, b = TEMPLATE2(_p4bits, USIZE)(in, n, &bx); 									
    #if EXCEP > 0
  if(bx <= USIZE) { P4SAVE(out, b, bx); } else *out++= 0x80|b<<1;									
    #else
  P4SAVE(out, b, bx); 
    #endif
  return TEMPLATE2(_P4ENC, USIZE)(in, n, out, b, bx);
}

/*unsigned char *TEMPLATE2(P4NENC, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out) {
  uint_t *ip;
  for(ip = in; ip < in+n; ip += P4D_MAX) { unsigned l = (in+n) - ip; l = min(l, P4D_MAX);      
    out = TEMPLATE2(P4ENC, USIZE)(ip, l, out);;                           
  }
}*/
#pragma clang diagnostic pop
#endif
/*
111 : 32 bits = bitpack            b=32   if bits=0x1f EOB
000 : bitpack = bitpack no exp     b=0..5
001 : vbyte                        b=0..5
010 : exp equ.                     b=0..5 b,bitpack
011 : bp. equal                    b=0..5 b,exp.
1XX : EOB
*/

