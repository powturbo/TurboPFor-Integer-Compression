/**
    Copyright (C) powturbo 2013-2016
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
//   vint.c - "Integer Compression" variable byte 
#include <stdio.h>
     
  #ifndef USIZE
#include "conf.h"
#include "vint.h"
#include "bitutil.h"

//------------- 32 bits ----------------------------------------------------------------
			                //0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111 		
unsigned char vtab[] =      {    1,   1,   1,   1,   1,   1,   1,   1,   5,   4,   3,   3,   2,   2,   2,   2 };

#define UN 8

#define USIZE 32
#include __FILE__
#undef USIZE

#define USIZE 64
#include __FILE__
#undef USIZE

#define USIZE 16
#include __FILE__
#undef USIZE

#define USIZE 15
#include __FILE__
#undef USIZE

#else
  #if USIZE == 15
#define uint_t uint16_t
#define zigzagenc15 zigzagenc16
#define zigzagdec15 zigzagdec16
  #else
#define uint_t TEMPLATE3(uint, USIZE, _t)
  #endif
  
unsigned char *TEMPLATE2(vbdec, USIZE)(unsigned char  *__restrict in, unsigned n, uint_t *__restrict out) { 
  register uint_t x, *op; 
  for(op = out; op != out+(n&~(UN-1)); op += UN) {
    TEMPLATE2(_vbget, USIZE)(in, x, op[0] = x);
    TEMPLATE2(_vbget, USIZE)(in, x, op[1] = x);
    TEMPLATE2(_vbget, USIZE)(in, x, op[2] = x);
    TEMPLATE2(_vbget, USIZE)(in, x, op[3] = x); __builtin_prefetch(in+8*USIZE, 0);
	  #if UN > 4
    TEMPLATE2(_vbget, USIZE)(in, x, op[4] = x);
    TEMPLATE2(_vbget, USIZE)(in, x, op[5] = x);
    TEMPLATE2(_vbget, USIZE)(in, x, op[6] = x);
    TEMPLATE2(_vbget, USIZE)(in, x, op[7] = x);
	  #endif
  }
  while(op != out+n) 
	TEMPLATE2(_vbget, USIZE)(in, x, *op++ = x );
  return in;
}

unsigned char *TEMPLATE2(vbenc, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out) { 
  register uint_t x, *ip;
  for(ip = in; ip != in+(n&~(UN-1)); ip += UN) {  __builtin_prefetch(ip+USIZE*8, 0);
    x = ip[0]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
    x = ip[1]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
    x = ip[2]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
    x = ip[3]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
	  #if UN > 4
    x = ip[4]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
    x = ip[5]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
    x = ip[6]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
    x = ip[7]; TEMPLATE2(_vbput, USIZE)(out, x, ;);
	  #endif
  }
  while(ip != in+n) { 
    x = *ip++; TEMPLATE2(_vbput, USIZE)(out, x, ;); 
  }
  return out;
}

unsigned char *TEMPLATE2(vbdenc, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out, uint_t start) { 
  uint_t *ip,v;
  for(ip = in; ip != in+(n&~(4-1)); ) {
    v = (*ip)-start; start = *ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
    v = (*ip)-start; start = *ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
    v = (*ip)-start; start = *ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
    v = (*ip)-start; start = *ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
  }
  while(ip != in+n) { 
    v = (*ip)-start; start = *ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
  }
  return out;
}  

unsigned char *TEMPLATE2(vbddec, USIZE)(unsigned char *__restrict in, unsigned n, uint_t *__restrict out, uint_t start) { 
  uint_t x,*op;
  for(op = out; op != out+(n&~(UN-1)); ) {
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
	  #if UN > 4
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += x);
	  #endif
  }
  while(op != out+n) _vbget32(in, x, *op++ = (start += x));
  return in;
}

#define VINT_Z 32
unsigned char *TEMPLATE2(vbd1enc, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out, uint_t start) {
  uint_t *ip, v; 
  unsigned char *op = out;
  
    #if VINT_Z == USIZE
  #define VINTZ(x) x 
  uint_t b = 0; 
  v = in[0] - start - 1; 
  unsigned long long u = (unsigned long long)v<<1; 
  if(n == 1) u |= 1;
  TEMPLATE2(_vbput, USIZE)(op, u, ;);
  if(!--n) return op;
  start = *in++;
    #else
  #define VINTZ(x) 
    #endif
	
  for(ip = in; ip != in + (n&~(4-1)); ) {
    v = (*ip)-start-1; start = *ip++; TEMPLATE2(_vbput, USIZE)(op, v, ;); VINTZ(b |= v);
    v = (*ip)-start-1; start = *ip++; TEMPLATE2(_vbput, USIZE)(op, v, ;); VINTZ(b |= v);
    v = (*ip)-start-1; start = *ip++; TEMPLATE2(_vbput, USIZE)(op, v, ;); VINTZ(b |= v);
    v = (*ip)-start-1; start = *ip++; TEMPLATE2(_vbput, USIZE)(op, v, ;); VINTZ(b |= v);
  }
  while(ip != in+n) { 
    v = (*ip)-start-1; start = *ip++; TEMPLATE2(_vbput, USIZE)(op, v, ;); VINTZ(b |= v); 
  } 
  
    #if VINT_Z == USIZE
  if(!b) { 
    u = (unsigned long long)in[-1] << 1 | 1;
	TEMPLATE2(_vbput, USIZE)(out, u, ;); 
	return out; 
  }
    #endif
  return op;
}
 
unsigned char *TEMPLATE2(vbd1dec, USIZE)(unsigned char *__restrict in, unsigned n, uint_t *__restrict out, uint_t start) { 
  uint_t x,*op;
    #if VINT_Z == USIZE
  unsigned long long u; TEMPLATE2(_vbget, USIZE)(in, u, ;); x = u>>1; *out = (start += x+1);
  if(u & 1) { 
      #ifdef __SSE2__
	out++; --n; BITDIZERO32(out, n, start, 1);
	  #else 
    for(x = 1; x < n; x++) out[x] = start+x;
      #endif
	return in; 
  } 
  out++; --n;
    #endif

  for(op = out; op != out+(n&~(8-1)); ) {
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
	  #if UN > 4
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
	  #endif
  }
  while(op != out+n) { 
    TEMPLATE2(_vbget, USIZE)(in, x, ++x); *op++ = (start += x);
  }
  return in;
}
#undef VINT_Z

unsigned char *TEMPLATE2(vbzenc, USIZE)(uint_t *__restrict in, unsigned n, unsigned char *__restrict out, uint_t start) { 
  uint_t *ip,v;
  for(ip = in; ip != in+(n&~(4-1)); ) {
    v = TEMPLATE2(zigzagenc, USIZE)((*ip)-start); start=*ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
    v = TEMPLATE2(zigzagenc, USIZE)((*ip)-start); start=*ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
    v = TEMPLATE2(zigzagenc, USIZE)((*ip)-start); start=*ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
    v = TEMPLATE2(zigzagenc, USIZE)((*ip)-start); start=*ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
  }
  while(ip <  in+n) { 
    v = TEMPLATE2(zigzagenc, USIZE)((*ip)-start); start=*ip++; TEMPLATE2(_vbput, USIZE)(out, v, ;);
  }
  return out;
}  

unsigned char *TEMPLATE2(vbzdec, USIZE)(unsigned char *__restrict in, unsigned n, uint_t *__restrict out, uint_t start) { 
  uint_t x,*op;
  for(op = out; op != out+(n&~(UN-1)); ) {
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
	  #if UN > 4
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
    TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
      #endif
  }
  while(op != out+n) {
	TEMPLATE2(_vbget, USIZE)(in, x, ;); *op++ = (start += TEMPLATE2(zigzagdec, USIZE)(x));
  }
  return in;
}
#endif