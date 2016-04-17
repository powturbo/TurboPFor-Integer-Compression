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
//    vint.c - "Integer Compression" variable byte 
#include <stdio.h>
     
#include "conf.h"
#include "vint.h"
#include "bitutil.h"

//-------------------------------------- variable byte : 32 bits ----------------------------------------------------------------
			                //0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111 		
unsigned char vtab[] =      {    1,   1,   1,   1,   1,   1,   1,   1,   5,   4,   3,   3,   2,   2,   2,   2 };

// decompress buffer into an array of n unsigned values. Return value = end of decompressed buffer in
unsigned char *vbdec32(unsigned char  *__restrict in, unsigned n, unsigned *__restrict out) { register unsigned x, *op; 
  for(op = out; op != out+(n&~(8-1)); op += 8) {
    _vbget32(in, x, op[0] = x);
    _vbget32(in, x, op[1] = x);
    _vbget32(in, x, op[2] = x);
    _vbget32(in, x, op[3] = x); __builtin_prefetch(in+256, 0);
    _vbget32(in, x, op[4] = x);
    _vbget32(in, x, op[5] = x);
    _vbget32(in, x, op[6] = x);
    _vbget32(in, x, op[7] = x);
  }
  while(op != out+n) _vbget32(in, x, *op++ = x );
  return in;
}

// encode array with n unsigned (32 bits in[n]) values to the buffer out. Return value = end of compressed buffer out
unsigned char *vbenc32(unsigned *__restrict in, unsigned n, unsigned char *__restrict out) { register unsigned x, *ip;
  for(ip = in; ip != in+(n&~(8-1)); ip += 8) {  __builtin_prefetch(ip+128, 0);
    x = ip[0]; _vbput32(out, x, ;);
    x = ip[1]; _vbput32(out, x, ;);
    x = ip[2]; _vbput32(out, x, ;);
    x = ip[3]; _vbput32(out, x, ;);
    x = ip[4]; _vbput32(out, x, ;);
    x = ip[5]; _vbput32(out, x, ;);
    x = ip[6]; _vbput32(out, x, ;);
    x = ip[7]; _vbput32(out, x, ;);
  }
  while(ip !=  in+n) { x = *ip++; _vbput32(out, x, ;); }
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
    v = (*ip)-start; start=*ip++; _vbput32(out, v, ;);
    v = (*ip)-start; start=*ip++; _vbput32(out, v, ;);
    v = (*ip)-start; start=*ip++; _vbput32(out, v, ;);
    v = (*ip)-start; start=*ip++; _vbput32(out, v, ;);
  }
  while(ip <  in+n) { v = (*ip)-start; start = *ip++; _vbput32(out, v, ;); }
  return out;
}  

unsigned char *vbddec32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start) { 
  unsigned x,*op;
  for(op = out; op != out+(n&~(8-1)); ) {
    _vbget32(in, x, ;); *op++ = (start += x);
    _vbget32(in, x, ;); *op++ = (start += x);
    _vbget32(in, x, ;); *op++ = (start += x);
    _vbget32(in, x, ;); *op++ = (start += x);
    _vbget32(in, x, ;); *op++ = (start += x);
    _vbget32(in, x, ;); *op++ = (start += x);
    _vbget32(in, x, ;); *op++ = (start += x);
    _vbget32(in, x, ;); *op++ = (start += x);
  }
  while(op != out+n) _vbget32(in, x, *op++ = (start += x));
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
  _vbput32(op, u, ;);
  if(!--n) return op;
  start = *in++;
    #endif
  for(ip = in; ip != in + (n&~(4-1)); ) {
    v = (*ip)-start-1; start = *ip++; _vbput32(op, v, ;); b |= v;
    v = (*ip)-start-1; start = *ip++; _vbput32(op, v, ;); b |= v;
    v = (*ip)-start-1; start = *ip++; _vbput32(op, v, ;); b |= v;
    v = (*ip)-start-1; start = *ip++; _vbput32(op, v, ;); b |= v;
  }
  while(ip != in+n) { v = (*ip)-start-1; start = *ip++; _vbput32(op, v, ;); b |= v; } 
    #ifdef VINT_Z
  if(!b) { 
    u = (unsigned long long)in[-1] << 1 | 1;
	_vbput32(out, u, ;); 
	return out; 
  }
    #endif
  return op;
}

unsigned char *vbd1dec32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start) { 
  unsigned x,*op;
    #ifdef VINT_Z
  unsigned long long u; _vbget32(in, u, ;); x = u>>1; *out = (start += x+1);
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
    _vbget32(in, x, ++x); *op++ = (start += x);
    _vbget32(in, x, ++x); *op++ = (start += x);
    _vbget32(in, x, ++x); *op++ = (start += x);
    _vbget32(in, x, ++x); *op++ = (start += x);
    _vbget32(in, x, ++x); *op++ = (start += x);
    _vbget32(in, x, ++x); *op++ = (start += x);
    _vbget32(in, x, ++x); *op++ = (start += x);
    _vbget32(in, x, ++x); *op++ = (start += x);
  }
  while(op != out+n) { _vbget32(in, x, ++x); *op++ = (start += x); }
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
    v = zigzagenc32((*ip)-start); start=*ip++; _vbput32(out, v, ;);
    v = zigzagenc32((*ip)-start); start=*ip++; _vbput32(out, v, ;);
    v = zigzagenc32((*ip)-start); start=*ip++; _vbput32(out, v, ;);
    v = zigzagenc32((*ip)-start); start=*ip++; _vbput32(out, v, ;);
  }
  while(ip <  in+n) { v = zigzagenc32((*ip)-start); start = *ip++; _vbput32(out, v, ;); }
  return out;
}  

unsigned char *vbzdec32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start) { 
  unsigned x,*op;
  for(op = out; op != out+(n&~(8-1)); ) {
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
    _vbget32(in, x, ;); *op++ = (start += zigzagdec32(x));
  }
  while(op != out+n) _vbget32(in, x, *op++ = (start += zigzagdec32(x)));
  return in;
}

unsigned char *vbzenc64(uint64_t *__restrict in, unsigned n, unsigned char *__restrict out, uint64_t start) { 
  uint64_t *ip,v;
  for(ip = in; ip != in+(n&~(4-1)); ) {
    v = zigzagenc64((*ip)-start); start=*ip++; _vbput64(out, v, ;);
    v = zigzagenc64((*ip)-start); start=*ip++; _vbput64(out, v, ;);
    v = zigzagenc64((*ip)-start); start=*ip++; _vbput64(out, v, ;);
    v = zigzagenc64((*ip)-start); start=*ip++; _vbput64(out, v, ;);
  }
  while(ip <  in+n) { v = zigzagenc64((*ip)-start); start = *ip++; _vbput64(out, v, ;); }
  return out;
}  

unsigned char *vbzdec64(unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, uint64_t start) {
  uint64_t x,*op;
  for(op = out; op != out+(n&~(4-1)); ) {
    _vbget64(in, x, ;); *op++ = (start += zigzagdec64(x));
    _vbget64(in, x, ;); *op++ = (start += zigzagdec64(x));
    _vbget64(in, x, ;); *op++ = (start += zigzagdec64(x));
    _vbget64(in, x, ;); *op++ = (start += zigzagdec64(x));
  }
  while(op != out+n) _vbget64(in, x, *op++ = (start += zigzagdec64(x)));
  return in;
}
