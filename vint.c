/**
    Copyright (C) powturbo 2013-2015
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
//    vint.c - "Integer Compression" variable byte 
#include <stdio.h>   
     
#include "conf.h"
#include "vint.h"
#include "bitutil.h"

#define _vbputu32(__op, __x, __act) {\
       if(likely(__x < (1<< 7))) {		   		   *__op++ = __x << 1; 			 		                      __act;}\
  else if(likely(__x < (1<<14))) { *(unsigned short *)__op = __x << 2 | 0x01; __op += 2; 		              __act;}\
  else if(likely(__x < (1<<21))) { *(unsigned       *)__op = __x << 3 | 0x03; __op += 3;                      __act;}\
  else if(likely(__x < (1<<28))) { *(unsigned       *)__op = __x << 4 | 0x07; __op += 4; 		              __act;}\
  else { 		                   *(unsigned       *)__op = __x << 4 | 0x0f; __op += 4; *__op++ = __x >> 28; __act;}\
}

#define _vbgetu32(__ip, __x, __act) do {\
  if(!((__x = *__ip) & (1<<0))) {  __ip++; __x	                              >>= 1;                  __act;}\
  else if(!(__x      & (1<<1))) { __x = (*(unsigned short *)__ip)             >>  2;       __ip += 2; __act;}\
  else if(!(__x      & (1<<2))) { __x = (*(unsigned       *)__ip & 0xffffffu) >>  3;       __ip += 3; __act;}\
  else if(!(__x      & (1<<3))) { __x = (*(unsigned       *)__ip)             >>  4; 	   __ip += 4; __act;}\
  else 			   	            { __x = (unsigned long long)(*(unsigned       *)__ip) >>  4 | (unsigned long long)(__ip[4]) << 28; __ip += 5; __act;}\
} while(0)

#define vbputu32(__op, __x) { unsigned _x_ = __x; _vbputu32(__op, _x_, ;); }

//-------------------------------------- variable byte : 32 bits ----------------------------------------------------------------
  #if defined(__AVX2__) && defined(__AVX2__VINT)
#include <immintrin.h>
#define M1         0xfeull //7
#define M2       0xfffcull //14
#define M3     0xfffff8ull //21
#define M4   0xfffffff0ull //28
#define M5 0xfffffffff0ull //36 

			                 //0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111 					 
unsigned long long mtab[] = {    M1,  M2,  M1,  M3,  M1,  M2,  M1,  M4,  M1,  M2,  M1,  M3,  M1,  M2,  M1,  M5 };
  #endif
//------------------------------------------------------------------------------------------------------------------------
			                //0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111 		
unsigned char vtab[] =      {    1,   2,   1,   3,   1,   2,   1,   4,   1,   2,   1,   3,   1,   2,   1,   5 };

// decompress buffer into an array of n unsigned values. Return value = end of decompressed buffer in
unsigned char *vbdec32(unsigned char  *__restrict in, unsigned n, unsigned *__restrict out) { unsigned x,*op;
  for(op = out; op != out+(n&~(4-1)); op += 4) { 
    _vbgetu32(in, x, op[0] = x);             
    _vbgetu32(in, x, op[1] = x); 
    _vbgetu32(in, x, op[2] = x); 
    _vbgetu32(in, x, op[3] = x); 
  }
  while(op != out+n) { _vbgetu32(in, x, ; ); *op++ = x; }
  return in;
}

// encode array with n unsigned (32 bits in[n]) values to the buffer out. Return value = end of compressed buffer out
unsigned char *vbenc32(unsigned *__restrict in, unsigned n, unsigned char *__restrict out) { unsigned *ip;
  for(ip = in; ip != in+(n&~(4-1)); ) { 
    vbputu32(out, *ip++);
    vbputu32(out, *ip++);
    vbputu32(out, *ip++);
    vbputu32(out, *ip++);  
  }
  while(ip !=  in+n) vbputu32(out, *ip++);
  return out;
}

unsigned char *vbdec64(unsigned char  *__restrict in, unsigned n, uint64_t *__restrict out) { uint64_t x,*op;
  for(op = out; op != out+(n&~(4-1)); op += 4) {
    _vbget64(in, x, op[0] = x);             
    _vbget64(in, x, op[1] = x); 
    _vbget64(in, x, op[2] = x); 
    _vbget64(in, x, op[3] = x); 
  }
  while(op != out+n) { _vbget64(in, x, ; ); *op++ = x; }
  return in;
}

// encode array with n unsigned (32 bits in[n]) values to the buffer out. Return value = end of compressed buffer out
unsigned char *vbenc64(uint64_t *__restrict in, unsigned n, unsigned char *__restrict out) { uint64_t *ip;
  for(ip = in; ip != in+(n&~(4-1)); ) {
    vbput64(out, *ip++);
    vbput64(out, *ip++);
    vbput64(out, *ip++);
    vbput64(out, *ip++);  
  }
  while(ip !=  in+n) vbput64(out, *ip++);
  return out;
}

//---------------------------------- increasing integer lists ----------------------------------------------------------------------------------------------------
unsigned char *vbdenc32(unsigned *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start) { 
  unsigned *ip,v;
  for(ip = in; ip != in+(n&~(4-1)); ) {
    v = (*ip)-start; start=*ip++; _vbputu32(out, v, ;);
    v = (*ip)-start; start=*ip++; _vbputu32(out, v, ;);
    v = (*ip)-start; start=*ip++; _vbputu32(out, v, ;);
    v = (*ip)-start; start=*ip++; _vbputu32(out, v, ;);
  }
  while(ip <  in+n) { v = (*ip)-start; start = *ip++; _vbputu32(out, v, ;); }
  return out;
}  

unsigned char *vbddec32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start) { 
  unsigned x,*op;
  for(op = out; op != out+(n&~(8-1)); ) {
    _vbgetu32(in, x, ;); *op++ = (start += x);
    _vbgetu32(in, x, ;); *op++ = (start += x);
    _vbgetu32(in, x, ;); *op++ = (start += x);
    _vbgetu32(in, x, ;); *op++ = (start += x);
    _vbgetu32(in, x, ;); *op++ = (start += x);
    _vbgetu32(in, x, ;); *op++ = (start += x);
    _vbgetu32(in, x, ;); *op++ = (start += x);
    _vbgetu32(in, x, ;); *op++ = (start += x);
  }
  while(op != out+n) _vbgetu32(in, x, *op++ = (start += x));
  return in;
}

//----------------------------------strictly increasing (never remaining constant or decreasing) integer lists---------------------------------------------------------
#define VINT_Z
unsigned char *vbd1enc32(unsigned *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start) {
  unsigned *ip, v, b = 0; 
  unsigned char *op = out;
    #ifdef VINT_Z
  v = in[0] - start - 1; 
  unsigned long long u = (unsigned long long)v<<1; 
  if(n == 1) u |= 1;
  _vbputu32(op, u, ;);
  if(!--n) return op;
  start = *in++;
    #endif
  for(ip = in; ip != in + (n&~(4-1)); ) {
    v = (*ip)-start-1; start = *ip++; _vbputu32(op, v, ;); b |= v;
    v = (*ip)-start-1; start = *ip++; _vbputu32(op, v, ;); b |= v;
    v = (*ip)-start-1; start = *ip++; _vbputu32(op, v, ;); b |= v;
    v = (*ip)-start-1; start = *ip++; _vbputu32(op, v, ;); b |= v;
  }
  while(ip != in+n) { v = (*ip)-start-1; start = *ip++; _vbputu32(op, v, ;); b |= v; } 
    #ifdef VINT_Z
  if(!b) { 
    u = (unsigned long long)in[-1] << 1 | 1;
	_vbputu32(out, u, ;); 
	return out; 
  }
    #endif
  return op;
}

unsigned char *vbd1dec32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start) { 
  unsigned x,*op;
    #ifdef VINT_Z
  unsigned long long u; _vbgetu32(in, u, ;); x = u>>1; *out = (start += x+1);
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
    _vbgetu32(in, x, ++x); *op++ = (start += x);
    _vbgetu32(in, x, ++x); *op++ = (start += x);
    _vbgetu32(in, x, ++x); *op++ = (start += x);
    _vbgetu32(in, x, ++x); *op++ = (start += x);
    _vbgetu32(in, x, ++x); *op++ = (start += x);
    _vbgetu32(in, x, ++x); *op++ = (start += x);
    _vbgetu32(in, x, ++x); *op++ = (start += x);
    _vbgetu32(in, x, ++x); *op++ = (start += x);
  }
  while(op != out+n) { _vbgetu32(in, x, ++x); *op++ = (start += x); }
  return in;
}

//--------------------------------------- variable byte : 16/15 bits -------------------------------------------------------------------
unsigned char *vbenc15(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out) { unsigned short *in_  = in +n;   while(in  <  in_) vbput15(out, *in++);        return out;}
unsigned char *vbdec15(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out) { unsigned short *out_ = out+n,x; while(out < out_) _vbget15(in, x, *out++ = x); return in; }

unsigned char *vbenc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out) { unsigned short *in_  = in +n;   while(in  <  in_) vbput16(out, *in++);        return out;}
unsigned char *vbdec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out) { unsigned short *out_ = out+n,x; while(out < out_) _vbget16(in, x, *out++ = x); return in; }

//---------------------------------- Zigzag encoding ------------------------------------------------------------------------------------
unsigned char *vbzenc32(unsigned *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start) { 
  unsigned *ip,v;
  for(ip = in; ip != in+(n&~(4-1)); ) {
    v = zigzagenc32((*ip)-start); start=*ip++; _vbputu32(out, v, ;);
    v = zigzagenc32((*ip)-start); start=*ip++; _vbputu32(out, v, ;);
    v = zigzagenc32((*ip)-start); start=*ip++; _vbputu32(out, v, ;);
    v = zigzagenc32((*ip)-start); start=*ip++; _vbputu32(out, v, ;);
  }
  while(ip <  in+n) { v = zigzagenc32((*ip)-start); start = *ip++; _vbputu32(out, v, ;); }
  return out;
}  

unsigned char *vbzdec32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start) { 
  unsigned x,*op;
  for(op = out; op != out+(n&~(8-1)); ) {
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbgetu32(in, x, ;); *op++ = (start += zigzagdec32(x));
  }
  while(op != out+n) _vbgetu32(in, x, *op++ = (start += zigzagdec32(x)));
  return in;
}
