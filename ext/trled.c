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

#include "trle.h"
#include "trle_.h"
         
//------------------------------------- RLE with Escape char ------------------------------------------------------------------
//#define MEMSAFE
#define SRLE8 32 // 16//
#define USIZE 8
#include "trled.c"

  #if SRLE8
unsigned _srled8(const unsigned char *__restrict in, unsigned char *__restrict out, unsigned outlen, unsigned char e) { 
  const uint8_t *ip = in; 
  uint8_t *op = out, c; 
  uint32_t i;
    #ifdef __SSE__    
  __m128i ev = _mm_set1_epi8(e);
    #endif 
  if(outlen >= SRLE8)
    while(op < out+(outlen-SRLE8)) {
		
        #ifdef __SSE__ // TODO: test _mm_cmpestrm/_mm_cmpestri on sse4
      uint32_t mask;
      __m128i u,v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, ev)); if(mask) goto a; op += 16; ip += 16;
        #if SRLE8 >= 32
              u = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, u); mask = _mm_movemask_epi8(_mm_cmpeq_epi8(u, ev)); if(mask) goto a; op += 16; ip += 16;
        #endif
	  											__builtin_prefetch(ip+512, 0);
      continue;
      a: i = ctz32(mask);
      op += i; ip += i+1;
	  {
        #else
      if(likely((c = *(uint8_t *)ip) != e)) {
	    ip++;
	    *op++ = c; 
	  } else {  
        #endif     
        vbget32(ip, i);
        if(likely(i)) { 
	      uint8_t c = *ip++; 
	      i  += TMIN; 
	      rmemset(op, c, i);
	    } else 
	      *op++ = e;												      
	  }
    }

  #define rmemset8(_op_, _c_, _i_) while(_i_--) *_op_++ = _c_
  while(op < out+outlen) 
    if(likely((c = *ip) != e)) {
	  ip++;
	  *op++ = c; 
	} else { 
	  int i; 
	  ip++;
	  vbget32(ip, i);
	  if(likely(i)) { 
	    c   = *ip++;  
		i  += TMIN; 
		rmemset8(op, c, i);
	  } else 
	    *op++ = e;
    }
  return ip - in;
} 
  #endif

unsigned _srled(const unsigned char *__restrict in, unsigned char *__restrict out, unsigned outlen) {
  return _srled8(in+1, out, outlen, *in); 
}
  
unsigned srled(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen) {
  if(inlen == outlen) 
    memcpy(out, in, outlen); 
  else if(inlen == 1) 
    memset(out, in[0], outlen);
  else 
    return _srled8(in+1, out, outlen, *in);
  return inlen;
}
//------------------------------------- TurboRLE ------------------------------------------
unsigned _trled(const unsigned char *__restrict in, unsigned char *__restrict out, unsigned outlen) {
        uint8_t b[256] = {0},*op = out;
  const uint8_t *ip;
  int m = -1, i, c; 

  if(outlen < 1) 
    return 0;

  if(!*in++) 
    return _srled8(in+1, out, outlen, *in)+2;

  for(ip = in; ip < in+32; ip++)
    for(i = 0; i < 8; ++i) 
	  if(((*ip) >> i) & 1) 
	    b[(ip-in)<<3 | i] = ++m+1; 		
  
  if(outlen >= 32)
  while(op < out+(outlen-32)) {				
    if(b[*ip]) goto a; *op++ = *ip++; 						
    if(b[*ip]) goto a; *op++ = *ip++; 						
    if(b[*ip]) goto a; *op++ = *ip++; 						
    if(b[*ip]) goto a; *op++ = *ip++; 		
    if(b[*ip]) goto a; *op++ = *ip++; 						
    if(b[*ip]) goto a; *op++ = *ip++; 						
    if(b[*ip]) goto a; *op++ = *ip++; 						
    if(b[*ip]) goto a; *op++ = *ip++; 		
											__builtin_prefetch(ip+256, 0);						
    continue;
    a:
    c = b[*ip++]; 
	vbzget(ip, i, m, c-1);
	c  = *ip++; 
	i += 3; 
	rmemset(op,c,i); 													
  }
  while(op < out+outlen) {				
    if(likely(!(c = b[*ip]))) 
	  *op++ = *ip++; 						
	else { 
	  ip++; 
	  vbzget(ip, i, m, c-1);
	  c  = *ip++; 
	  i += 3; 
	  rmemset8(op,c,i); 					
    }								
  }
  return ip - in;
}

unsigned trled(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen) {
  if(inlen == outlen) 
    memcpy(out, in, outlen); 
  else if(inlen == 1) 
    memset(out, in[0], outlen);
  else 
    return _trled(in, out, outlen);
  return inlen;
}

#undef USIZE
#undef rmemset
#undef SRLE8

#define USIZE 16
#include "trled.c"
#undef rmemset
#undef USIZE
#undef runcpy

#define USIZE 32
#include "trled.c"
#undef rmemset
#undef USIZE
#undef runcpy

#define USIZE 64
#include "trled.c"
#undef rmemset
#undef USIZE

 #else
  #ifdef MEMSAFE
#define rmemset(_op_, _c_, _i_) while(_i_--) *_op_++ = _c_
  #elif defined(__SSE__) && USIZE < 64
#define rmemset(_op_, _c_, _i_) do { \
  __m128i *_up = (__m128i *)_op_, cv = TEMPLATE2(_mm_set1_epi, USIZE)(_c_);\
  _op_ += _i_;\
  do { _mm_storeu_si128(  _up, cv); _mm_storeu_si128(_up+1, cv); _up+=2; } while(_up < (__m128i *)_op_);\
} while(0)
  #else
#define _cset64(_cc,_c_) _cc = _c_
#define _cset32(_cc,_c_) _cc = _c_; _cc = _cc<<32|_cc
#define _cset16(_cc,_c_) _cc = _c_; _cc = _cc<<48|_cc<<32|_cc<<16|_cc
#define _cset8( _cc,_c_) _cc = (uint32_t)_c_<<24 | (uint32_t)_c_<<16 | (uint32_t)_c_<<8 | (uint32_t)_c_; _cc = _cc<<32|_cc

#define rmemset(_op_, _c_, _i_) do {  uint64_t _cc; uint8_t *_up = (uint8_t *)_op_; _op_ +=_i_;\
 TEMPLATE2(_cset, USIZE)(_cc,_c_);\
  do {\
    TEMPLATE2(ctou, USIZE)(_up) = _c_; _up += USIZE/8;\
    TEMPLATE2(ctou, USIZE)(_up) = _c_; _up += USIZE/8;\
  } while(_up < (uint8_t *)_op_);\
} while(0)
  #endif 

#define uint_t TEMPLATE3(uint, USIZE, _t)
  
  #if !SRLE8
unsigned TEMPLATE2(_srled, USIZE)(const unsigned char *__restrict in, unsigned char *__restrict cout, unsigned outlen, uint_t e) {
  uint_t *out = (uint_t *)cout, *op = out, c; 
  const unsigned char *ip = in;
  
  while(op < out+outlen/sizeof(uint_t)) { 	__builtin_prefetch(ip +384, 0);
    if(likely((c = *(uint_t *)ip) != e)) { 
	  ip   += sizeof(uint_t);
	  *op++ = c;								 
	} else { 				
	  int i; 
	  ip += sizeof(uint_t);			
      vbget32(ip, i);
	  if(likely(i)) { 
	    c   = *(uint_t *)ip; 
		ip += sizeof(uint_t); 		
		i  += 3; 
		rmemset(op, c, i);			
	  } else 
	    *op++ = e;
    } 							
  }	
    #if USIZE > 8
  { unsigned char *p = (unsigned char *)op; 
    while(p < cout+outlen) *p++ = *ip++; 
  }
    #endif
  return ip - in;
}
  #endif

unsigned TEMPLATE2(srled, USIZE)(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint_t e) {
  if(inlen == outlen) 
    memcpy(out, in, outlen); 
  else if(inlen == 1) 
    memset(out, in[0], outlen);
  else 
    return TEMPLATE2(_srled, USIZE)(in, out, outlen, e);
  return inlen;
}
 #endif
