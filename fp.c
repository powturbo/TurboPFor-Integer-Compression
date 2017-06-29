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
//    "Floating Point Compression" 
#pragma warning( disable : 4005) 
#pragma warning( disable : 4090) 
#pragma warning( disable : 4068) 

#include "conf.h"  
#include "vp4.h"
#include "fp.h"
#include "bitutil.h"
#define VSIZE 128

//#define ENC64(u,h) (u^h)
//#define DEC64(u,h) (u^h)
#define ENC64(u,h) zigzagenc64(u-h)
#define DEC64(u,h) zigzagdec64(u)+h

//---- Last value Predictor 
unsigned char *fppenc64(uint64_t *in, unsigned n, unsigned char *out, uint64_t start) {
  uint64_t      *ip, h = 0, _p[VSIZE], *p;
  unsigned char *op = out;
  
  #define FE64(i) { uint64_t u = ip[i]; p[i] = ENC64(u, h); h = u; } 
  for(ip = (uint64_t *)in; ip != in + (n&~(VSIZE-1)); ) {   		
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE64(0); FE64(1); FE64(2); FE64(3); }  
	op = p4enc64(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = ((uint64_t *)in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) FE64(0);
    op = p4enc64(_p, n, op);
  }																	
  return op;
}

unsigned char *fppdec64(unsigned char *in, unsigned n, uint64_t *out, uint64_t start) {
  uint64_t      *op, h = 0, _p[VSIZE+32],*p; 
  unsigned char *ip = in;

  #define FD64(i) { uint64_t u = DEC64(p[i],h); op[i] = u; h = u; }
  for(op = (uint64_t*)out; op != out+(n&~(VSIZE-1)); ) { __builtin_prefetch(ip+512, 0);
	for(ip = p4dec64(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD64(0); FD64(1); FD64(2); FD64(3); }
  }
  if(n = ((uint64_t *)out+n) - op)
	for(ip = p4dec64(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD64(0);
  return ip;
}

#define HBITS 13 //15
#define HASH64(_h_,_u_) (((_h_)<<5 ^ (_u_)>>50) & ((1u<<HBITS)-1))

//---- FCM: Finite Context Method Predictor 
unsigned char *fpfcmenc64(uint64_t *in, unsigned n, unsigned char *out, uint64_t start) {
  uint64_t      *ip, htab[1<<HBITS] = {0}, h = 0, _p[VSIZE], *p;
  unsigned char *op = out;
  
  #define FE64(i) { uint64_t u = ip[i]; p[i] = ENC64(u, htab[h]); htab[h] = u; h = HASH64(h,u); } 
  for(ip = (uint64_t *)in; ip != in + (n&~(VSIZE-1)); ) {   		
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { FE64(0); FE64(1); FE64(2); FE64(3); }  
	op = p4enc64(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  }   
  if(n = ((uint64_t *)in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) FE64(0);
    op = p4enc64(_p, n, op);
  }																	
  return op;
}

unsigned char *fpfcmdec64(unsigned char *in, unsigned n, uint64_t *out, uint64_t start) {
  uint64_t      *op, htab[1<<HBITS] = {0}, h = 0, _p[VSIZE+32],*p; 
  unsigned char *ip = in;

  #define FD64(i) { uint64_t u = DEC64(p[i], htab[h]); op[i] = u; htab[h] = u; h = HASH64(h,u); }
  for(op = (uint64_t*)out; op != out+(n&~(VSIZE-1)); ) { __builtin_prefetch(ip+512, 0);
	for(ip = p4dec64(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { FD64(0); FD64(1); FD64(2); FD64(3); }
  }
  if(n = ((uint64_t *)out+n) - op)
	for(ip = p4dec64(ip, n, _p), p = _p; p != &_p[n]; p++,op++) FD64(0);
  return ip;
}

// DFCM: Differential Finite Context Method Predictor 
unsigned char *fpdfcmenc64(uint64_t *in, unsigned n, unsigned char *out, uint64_t start) {
  unsigned char *op = out;
  uint64_t       _p[VSIZE+32], *ip, h = 0, *p, htab[1<<HBITS] = {0}; htab[0] = start ;
  
  #define DE64(i) { uint64_t u = ip[i]; p[i] = ENC64(u, (htab[h]+start)); htab[h] = start = u - start; h = HASH64(h,start); start = u; } 
  for(ip = (uint64_t *)in; ip != in + (n&~(VSIZE-1)); ) {  		
    for(p = _p; p != &_p[VSIZE]; p+=4,ip+=4) { DE64(0); DE64(1); DE64(2); DE64(3); }  
	op = p4enc64(_p, VSIZE, op); 													__builtin_prefetch(ip+512, 0);
  } 
  
  if(n = ((uint64_t *)in+n)-ip) { 					
    for(p = _p; p != &_p[n]; p++,ip++) DE64(0);
    op = p4enc64(_p, n, op);
  }																	
  return op;
}

unsigned char *fpdfcmdec64(unsigned char *in, unsigned n, uint64_t *out, uint64_t start) {
  unsigned char *ip = in;
  uint64_t      _p[VSIZE+32], *op, h = 0, *p, htab[1<<HBITS] = {0}; htab[0] = start;

  #define DD64(i) { uint64_t u = DEC64(p[i], (htab[h]+start)); op[i] = u; htab[h] = start = u-start; h = HASH64(h,start); start = u; }
  for(op = (uint64_t*)out; op != out+(n&~(VSIZE-1)); ) { __builtin_prefetch(ip+512, 0);
	for(ip = p4dec64(ip, VSIZE, _p), p = _p; p != &_p[VSIZE]; p+=4,op+=4) { DD64(0); DD64(1); DD64(2); DD64(3); }
  }
  if(n = ((uint64_t *)out+n) - op)
	for(ip = p4dec64(ip, n, _p), p = _p; p != &_p[n]; p++,op++) DD64(0);
  return ip;
}
