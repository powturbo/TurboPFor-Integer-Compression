/**
    Copyright (C) powturbo 2013-2014
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

    - email    : powturbo@gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    vint.h - "Integer Compression" variable byte 
**/

#ifndef VINT_H
#define VINT_H
#include "conf.h"
//-------------------------------------- variable byte : 32 bits ----------------------------------------------------------------
			     //0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111 		
static unsigned char vtab[]= {    1,   2,   1,   3,   1,   2,   1,   4,   1,   2,   1,   3,   1,   2,   1,   5 };
#define vbvlen(__x) vtab[(__x)&0xf]

#define vbputa(__op, __x, __act) {\
       if(likely(__x < (1<< 7))) {		   *__op++ = __x << 1; 			 		      __act;}\
  else if(likely(__x < (1<<14))) { *(unsigned short *)__op = __x << 2 | 0x01; __op += 2; 		      __act;}\
  else if(likely(__x < (1<<21))) { *(unsigned short *)__op = __x << 3 | 0x03; __op += 2; *__op++ = __x >> 13; __act;}\
  else if(likely(__x < (1<<28))) { *(unsigned       *)__op = __x << 4 | 0x07; __op += 4; 		      __act;}\
  else { 		           *(unsigned       *)__op = __x << 4 | 0x0f; __op += 4; *__op++ = __x >> 28; __act;}\
}

#define vbgeta(__ip, __x, __act) do { __x = *__ip;\
       if(!(__x & (1<<0))) { __x			     >>= 1; 		      __ip++;    __act;}\
  else if(!(__x & (1<<1))) { __x = (*(unsigned short *)__ip) >>  2;		      __ip += 2; __act;}\
  else if(!(__x & (1<<2))) { __x = (*(unsigned short *)__ip) >>  3 | *(__ip+2) << 13; __ip += 3; __act;}\
  else if(!(__x & (1<<3))) { __x = (*(unsigned       *)__ip) >>  4; 		      __ip += 4; __act;}\
  else 			   	   { __x = (*(unsigned       *)__ip) >>  4 | *(__ip+4) << 28; __ip += 5; __act;}\
} while(0)

#define vblen(_x_) ({ unsigned __x = _x_; __x > 0x7f?(__x > 0x3fff?(__x > 0x1fffff?(__x > 0x0fffffff?5:4):3):2):1; })
#define vbput(__op, __x) { unsigned _x__ = __x; vbputa(__op, _x__, ;); }
#define vbget(__ip) ({     unsigned _x_;        vbgeta(__ip, _x_, ;); _x_; })

static inline unsigned char *vbenc (unsigned       *__restrict__ in, int n, unsigned char  *__restrict__ out) { unsigned      *in_  = in +n;   while(in  <  in_) vbput(out, *in++);         return out;}
static inline unsigned char *vbdec (unsigned char  *__restrict__ in, int n, unsigned       *__restrict__ out) { unsigned      *out_ = out+n,x; while(out < out_) vbgeta(in, x, *out++ = x); return in;}

//--------------------------------------- variable byte : 15 bits -------------------------------------------------------------------
#define vblen16(__x) ((__x) > 0x7f?2:1)
#define vbput16(__op, __x) do { unsigned _x = __x; if(likely(_x < 0x80)) *__op++ = _x; else { *__op++ = (_x) >> 8 | 0x80; *__op++ = _x; } } while(0)
#define vbgeta16(__ip,__x, __act) do { if((__x = *__ip++) > 0x7f) __x = (__x & 0x7f) << 8 | *__ip++; __act; } while(0)
#define vbget16(__ip) ({ unsigned _x; vbgeta16(__ip, _x, ;); _x; })

static inline unsigned char *vbenc16(unsigned short *__restrict__ in, int n, unsigned char  *__restrict__ out) { unsigned short *in_  = in +n;   while(in  <  in_) vbput16(out, *in++);        return out;}
static inline unsigned char *vbdec16(unsigned char  *__restrict__ in, int n, unsigned short *__restrict__ out) { unsigned short *out_ = out+n,x; while(out < out_) vgeta16(in, x, *out++ = x); return in; }

#endif



