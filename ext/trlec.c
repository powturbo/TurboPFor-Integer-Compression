/**
    Copyright (C) powturbo 2015-2018
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

    - email    : powturbo [AT] gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    TurboRLE - "Most efficient and fastest Run Length Encoding"
**/
  #ifndef USIZE
#include <string.h> 
    #ifdef __SSE__
#include <emmintrin.h>
    #endif

#include "trle_.h"
#include "trle.h"

//------------------------------------- Histogram ---------------------------------------------------------
static inline unsigned hist(const unsigned char *__restrict in, unsigned inlen, unsigned *cc) { // Optimized for x86
  unsigned c0[256+8]={0},c1[256+8]={0},c2[256+8]={0},c3[256+8]={0},c4[256+8]={0},c5[256+8]={0},c6[256+8]={0},c7[256+8]={0}; 

  const unsigned char *ip;
  unsigned 					  cp = *(unsigned *)in,a;  
  int i;
  for(ip = in; ip != in+(inlen&~(16-1));) {  
    unsigned 	c = cp,	d = *(unsigned *)(ip+=4); cp = *(unsigned *)(ip+=4);    
    c0[(unsigned char) c    ]++;
    c1[(unsigned char) d    ]++;
    c2[(unsigned char)(c>>8)]++; c>>=16;
    c3[(unsigned char)(d>>8)]++; d>>=16; 
    c4[(unsigned char) c    ]++;
    c5[(unsigned char) d    ]++;
    c6[ 	           c>>8 ]++;
    c7[ 	           d>>8 ]++; 
    
    		c = cp;	d = *(unsigned *)(ip+=4); cp = *(unsigned *)(ip+=4);    
    c0[(unsigned char) c    ]++;
    c1[(unsigned char) d    ]++;
    c2[(unsigned char)(c>>8)]++; c>>=16;
    c3[(unsigned char)(d>>8)]++; d>>=16;
    c4[(unsigned char) c    ]++;
    c5[(unsigned char) d    ]++;
    c6[ 	           c>>8 ]++;
    c7[ 	           d>>8 ]++; 
  }
  while(ip < in+inlen) c0[*ip++]++; 

  for(i = 0; i < 256; i++) 
    cc[i] += c0[i]+c1[i]+c2[i]+c3[i]+c4[i]+c5[i]+c6[i]+c7[i];
  a = 256; 
  while(a > 1 && !cc[a-1]) a--; 
  return a;
}
//------------------------------------- RLE with Escape char ------------------------------------------------------------------
#define SRLE8 32
#define USIZE 8
#include "trlec.c"

  #if SRLE8
#define SRLEC8(pp, ip, op, e) do {\
  unsigned i = ip - pp;\
  if(i > 3) { *op++ = e; i -= 3; vbput32(op, i); *op++ = c; }\
  else if(c == e) {\
	while(i--) { *op++ = e; vbput32(op, 0); }\
  } else while(i--) *op++ = c;\
} while(0)

unsigned _srlec8(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint8_t e) {
  const uint8_t *ip = in, *pp = in - 1;
  uint8_t *op = out,c;

  if(inlen > SRLE8)
    while(ip <  in+(inlen-1-SRLE8)) {	
        #if 0 //def __SSE__  // SSE slower than scalar 
	  __m128i cv = _mm_set1_epi8(*ip);
	  unsigned mask = _mm_movemask_epi8(_mm_cmpeq_epi8(_mm_loadu_si128((const __m128i*)(ip+1)), cv)); if(mask != 0xffffu) goto a; ip += 16;
	           mask = _mm_movemask_epi8(_mm_cmpeq_epi8(_mm_loadu_si128((const __m128i*)(ip+1)), cv)); if(mask != 0xffffu) goto a; ip += 16;
      continue;
      a: c = *ip; 
      ip += __builtin_ctz((unsigned short)(~mask));
      SRLEC8(pp, ip, op, e);
	  pp = ip++;
        #elif __WORDSIZE == 64						
		{unsigned long long z;
	  if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;
	  if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;
          #if SRLE8 >= 32
	  if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;
	  if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;	
          #endif
                                                            __builtin_prefetch(ip +256, 0);
      continue;
      a: c = *ip; 
      ip += ctz64(z)>>3; 				       
      SRLEC8(pp, ip, op, e);
	  pp = ip++;
		}
        #else
	  { unsigned z;
	  if((z = (ctou32(ip) ^ ctou32(ip+1)))) goto a; ip += 4;
	  if((z = (ctou32(ip) ^ ctou32(ip+1)))) goto a; ip += 4;
          #if SRLE8 >= 16
	  if((z = (ctou32(ip) ^ ctou32(ip+1)))) goto a; ip += 4;
	  if((z = (ctou32(ip) ^ ctou32(ip+1)))) goto a; ip += 4;	
          #endif
                                                            __builtin_prefetch(ip +256, 0);
      continue;
      a: c = *ip; 
      ip += ctz32(z)>>3; 				       
      SRLEC8(pp, ip, op, e);
	  pp = ip++;	
	  }	  
        #endif
    }

  for(;ip < in+inlen; ip++) 
    if(*ip != ip[1]) {
      c = *ip;
	  SRLEC8(pp,ip, op, e);
	  pp = ip;
	}
  c = *ip; 
  SRLEC8(pp, ip, op, e);
  return op - out;
}
#endif

unsigned srlec(const unsigned char *__restrict in,  unsigned inlen, unsigned char *__restrict out) {
  unsigned m = 0xffffffffu, mi = 0, i, b[256] = {0},a; 
  size_t l;
  if(inlen < 1) return 0;

  a = hist(in,inlen,b);  		
  if(b[a-1] == inlen) {
    *out = *in;
    return 1;
  }
  
  for(i = 0; i < 256; i++) 
    if(b[i] <= m) 
	  m = b[i],mi = i;
  *out = mi;                                  
  l = _srlec8(in, inlen, out+1, mi)+1;
  if(l < inlen) 
    return l;
  memcpy(out, in, inlen);
  return inlen;
}

//------------------------------------------------- TurboRLE ------------------------------------------
struct u { unsigned c,i; };										

#define PUTC(op, x) *op++ = x
#define TRLEC(pp, ip, op, _goto_) do {\
  unsigned _i = ip - pp;\
  if(_i >= TMIN) {\
    unsigned char *q = op; \
    vbzput(op, _i-TMIN, m, rmap); \
    if((op-q) + 1 < _i) { *op++ = c; _goto_; } op=q;\
  } while(_i--) PUTC(op,c);\
} while(0)
  
#define TRLEC0(pp, ip, op, _goto_) do { unsigned _i = ip - pp;\
  if(_i >= TMIN) { vbzput(op, _i-TMIN, m, rmap); *op++ = c; } else while(_i--) PUTC(op,c);\
} while(0)

unsigned trlec(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out) {
  int m,i;
  unsigned b[256] = {0}, rmap[256],a;
  struct u u[256],*v; // sort
  unsigned char *op;
  const unsigned char *ip,*pp;
  uint8_t c;
  if(inlen < 1) return 0;

  a = hist(in,inlen,b);  		
  if(b[a-1] == inlen) {
    *out = *in;
    return 1;
  }
  
  for(i = 0; i < 256; i++) u[i].c = b[i], u[i].i = i,b[i]=0;  		
  for(v = u + 1; v < u + 256; ++v)
    if(v->c < v[-1].c) { 
	  struct u *w, tmp = *v;
      for(w = v; w > u && tmp.c < w[-1].c; --w) *w = w[-1];
      *w = tmp;
    }  															
  																												
  for(m = -1,i = 0; i < 256 && !u[i].c; i++) 
    b[u[i].i]++, ++m;

  op = out;

  if(m < 0) { // no unused bytes found
    size_t l;
    *op++ = 0; 
	*op++ = u[0].i; 
    l = _srlec8(in, inlen, op, u[0].i)+2;
    if(l < inlen) return l;
    memcpy(out, in, inlen);
    return inlen;
  } 																		
  
  *op++ = 1;
  memset(op, 0, 32);
  for(m = -1,i = 0; i < 256; i++) 
    if(b[i]) { 
      op[i>>3] |= 1<<(i&7); 
      rmap[++m] = i; 
    } 
  op += 32;

  ip = in; pp=in-1;
  if(inlen > SRLE8)
    while(ip < in+(inlen-1-SRLE8)) {
      unsigned long long z;
      if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;
	  if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;
        #if SRLE8 >= 32
	  if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;
	  if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;	
        #endif
                                                            __builtin_prefetch(ip +256, 0);
      continue;
      a: c = *ip; 
      ip += ctz64(z)>>3; 				       
      TRLEC(pp, ip, op, goto laba);
	  laba:pp = ip++;
    }
  
  for(;ip < in+inlen; ip++) {
    if(*ip != *(ip+1)) {
      c = *ip; 
	  TRLEC(pp, ip, op, goto labb);
	  labb:pp = ip;
	}
  }

  c = *ip; 
  TRLEC(pp,ip, op, goto labc);
  labc:
  if(op - out < inlen) 
    return op - out;
  memcpy(out, in, inlen); 
  return inlen; 
}

#undef USIZE
#undef SRLE8

#define USIZE 16
#include "trlec.c"
#undef USIZE

#define USIZE 32
#include "trlec.c"
#undef USIZE

#define USIZE 64
#include "trlec.c"
#undef USIZE

#else
#define uint_t TEMPLATE3(uint, USIZE, _t)

#define SRLEC(pp, ip, op, e) do {\
  unsigned i = ip - pp;\
  if(i > 3) { *(uint_t *)op = e; op+=sizeof(uint_t); i -= 3; vbput32(op, i); *(uint_t *)op = c; op+=sizeof(uint_t); }\
  else if(c == e) {\
	while(i--) { *(uint_t *)op = e; op+=sizeof(uint_t); vbput32(op, 0); }\
  } else while(i--) { *(uint_t *)op = c; op+=sizeof(uint_t); }\
} while(0)

  #if !SRLE8
unsigned TEMPLATE2(_srlec, USIZE)(const unsigned char *__restrict cin, unsigned inlen, unsigned char *__restrict out, uint_t e) {
  unsigned char *op = out;
  uint_t *in = (uint_t *)cin, *pp = in-1, *ip=in,c; 
  unsigned n = inlen/sizeof(uint_t);
  unsigned char *p;
  if(n > 4)
    for(; ip < in+(n-1-4);) {
        #if 0 
      if(*  ip == ip[1])  
        if(*++ip == ip[1]) 
          if(*++ip == ip[1])
            if(*++ip == ip[1]) { 
              ip++; 									__builtin_prefetch(ip +256, 0);
              continue;
            }
        #else 
      if(*ip != ip[1]) goto a; ++ip;
      if(*ip != ip[1]) goto a; ++ip;
      if(*ip != ip[1]) goto a; ++ip;
      if(*ip != ip[1]) goto a; ++ip; 					__builtin_prefetch(ip +256, 0);
      continue;
      a:;
        #endif  
      c = *ip;
      SRLEC(pp,ip, op, e);
	  pp = ip++;								
    }

  for(;ip < in+n; ip++) 
    if(*ip != ip[1]) {
      c = *ip;
	  SRLEC(pp,ip, op, e);
	  pp = ip;
	}
  c = *ip; 
  SRLEC(pp, ip, op, e);
  
    #if USIZE > 8
  p = (unsigned char *)ip; 
  while(p < cin+inlen) 
	*op++ = *p++; 
    #endif
  return op - out;
}
 #endif
#undef SRLEC

unsigned TEMPLATE2(srlec, USIZE)(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint_t e) {
  size_t l = TEMPLATE2(_srlec, USIZE)(in, inlen, out, e);

  if(l < inlen) 
    return l;
  memcpy(out, in, inlen);
  return inlen;
}
#endif

