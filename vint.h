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
//    vint.h - "Integer Compression" variable byte include header 

#ifndef VINT_H
#define VINT_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------- variable byte : 32 bits ----------------------------------------------------------------
#define vbvlen(__x) vtab[(__x)&0xf]

#define vbputa(__op, __x, __act) {\
       if(likely(__x < (1<< 7))) {		   		   *__op++ = __x << 1; 			 		                      __act;}\
  else if(likely(__x < (1<<14))) { *(unsigned short *)__op = __x << 2 | 0x01; __op += 2; 		              __act;}\
  else if(likely(__x < (1<<21))) { *(unsigned short *)__op = __x << 3 | 0x03; __op += 2; *__op++ = __x >> 13; __act;}\
  else if(likely(__x < (1<<28))) { *(unsigned       *)__op = __x << 4 | 0x07; __op += 4; 		              __act;}\
  else { 		                   *(unsigned       *)__op = __x << 4 | 0x0f; __op += 4; *__op++ = __x >> 28; __act;}\
}

#define vbputa64(__op, __x, __act) {\
       if(__x < 1   << 7) {		   		        *__op++ = __x << 1; 			 		                                                                   __act;}\
  else if(__x < 1   <<14) { *(unsigned short     *)__op = __x << 2 | 0x01; __op += 2; 		             	                                               __act;}\
  else if(__x < 1   <<21) { *(unsigned short     *)__op = __x << 3 | 0x03; __op += 2;                 *__op++ = __x >> 13;                                 __act;}\
  else if(__x < 1   <<28) { *(unsigned           *)__op = __x << 4 | 0x07; __op += 4; 		                                                               __act;}\
  else if(__x < 1ull<<35) { *(unsigned           *)__op = __x << 5 | 0x0f; __op += 4;                 *__op++ = __x >> 27;                                 __act;}\
  else if(__x < 1ull<<42) { *(unsigned           *)__op = __x << 6 | 0x1f; __op += 4; *(unsigned short *)__op = __x >> 26;                        __op+=2; __act;}\
  else if(__x < 1ull<<49) { *(unsigned           *)__op = __x << 7 | 0x3f; __op += 4; *(unsigned short *)__op = __x >> 25; *(__op+2) = __x >> 41; __op+=3; __act;}\
  else if(__x < 1ull<<56) { *(unsigned long long *)__op = __x << 8 | 0x7f; __op += 8;                                                                      __act;}\
  else {                                          *__op++ =          0xff;        *(unsigned long long *)__op = __x;                              __op+=8; __act;}\
}

#define vbgeta64(__ip, __x, __act) do {\
  if(!((__x = *__ip) & 1<<0)) {  __ip++; __x	                    >>= 1; 		                                                                __act;}\
  else if(!(__x      & 1<<1)) { __x = (*(unsigned short     *)__ip) >>  2;		                                                     __ip += 2; __act;}\
  else if(!(__x      & 1<<2)) { __x = (*(unsigned short     *)__ip) >>  3 | *(__ip+2) << 13;                                         __ip += 3; __act;}\
  else if(!(__x      & 1<<3)) { __x = (*(unsigned           *)__ip) >>  4; 		      	                                             __ip += 4; __act;}\
  else if(!(__x      & 1<<4)) { __x = (*(unsigned           *)__ip) >>  5 | (unsigned long long)(*(unsigned char  *)(__ip+4)) << 27; __ip += 5; __act;}\
  else if(!(__x      & 1<<5)) { __x = (*(unsigned           *)__ip) >>  6 | (unsigned long long)(*(unsigned short *)(__ip+4)) << 26; __ip += 6; __act;}\
  else if(!(__x      & 1<<6)) { __x = (*(unsigned           *)__ip) >>  7 | (unsigned long long)(*(unsigned short *)(__ip+4)) << 25 | (unsigned long long)(*(__ip+6)) << 41; __ip += 7; __act;}\
  else if(!(__x      & 1<<7)) { __x = (*(unsigned long long *)__ip) >>  8; 		                                                     __ip += 8; __act;}\
  else 			   	          { __x =  *(unsigned long long *)(__ip+1);                                                              __ip += 9; __act;}\
} while(0)

//#define __AVX2__VINT
  
  #if defined(__AVX2__) && defined(__AVX2__VINT)
#include <immintrin.h>

extern unsigned long long mtab[];

#define vbgeta(__ip, __x, __act) do { unsigned _vdx=(*__ip)&0xf; __x = _pext_u64(*(unsigned long long *)__ip, mtab[_vdx]); __ip+=vtab[_vdx]; __act; } while(0)
  #else
    #if 1
#define vbgeta(__ip, __x, __act) do {\
  if(!((__x = *__ip) & (1<<0))) {  __ip++; __x	                 >>= 1; 		                      __act;}\
  else if(!(__x      & (1<<1))) { __x = (*(unsigned short *)__ip) >>  2;		           __ip += 2; __act;}\
  else if(!(__x      & (1<<2))) { __x = (*(unsigned short *)__ip) >>  3 | *(__ip+2) << 13; __ip += 3; __act;}\
  else if(!(__x      & (1<<3))) { __x = (*(unsigned       *)__ip) >>  4; 		      	   __ip += 4; __act;}\
  else 			   	            { __x = (*(unsigned       *)__ip) >>  4 | *(__ip+4) << 28; __ip += 5; __act;}\
} while(0)
    #else
#define vbgeta(__ip, __x, __act) do {\
       if(!((__x = *__ip)                   & (1<<0))) { __ip++;          __x >>= 1; 		            	    __act;}\
  else if(!((__x = *(unsigned short *)__ip) & (1<<1))) { __ip += 2;       __x >>= 2;						    __act;}\
  else if(!( __x                            & (1<<2))) { __ip += 3; __x = __x >>  3 | (unsigned)__ip[-1] << 13; __act;}\
  else if(!((__x = *(unsigned       *)__ip) & (1<<3))) { __ip += 4;       __x >>= 4; 					 		__act;}\
  else 			   	                                   { __ip += 5; __x = __x >>  4 | (unsigned)__ip[-1] << 28;	__act;}\
} while(0)
    #endif	
  #endif
//------------------------------------------------------------------------------------------------------------------------
extern unsigned char vtab[];
// Length of uncompress value. Input __x is the compressed buffer start

// Length in bytes of compressed "__x" when using variable byte
#define vblen(__x) ({ unsigned _x = __x; _x > 0x7f?(_x > 0x3fff?(_x > 0x1fffff?(_x > 0x0fffffff?5:4):3):2):1; })

// encode/decode single value
#define vbput(__op, __x) { unsigned _x_ = __x; vbputa(__op, _x_, ;); }
#define vbget(__ip) ({     unsigned _x_;       vbgeta(__ip, _x_, ;); _x_; })
#define vbput32(__op, __x) vbput(__op, __x)
#define vbget32(__ip) vbget(__ip)

#define vbput64(__op, __x) { unsigned long long _x_ = __x; vbputa64(__op, _x_, ;); }
#define vbget64(__ip) ({     unsigned long long _x_;       vbgeta64(__ip, _x_, ;); _x_; })

// encode array with n unsigned (32 bits in[n]) values to the buffer out. Return value = end of compressed buffer out
unsigned char *vbenc   (unsigned      *__restrict in, unsigned n, 				  unsigned char  *__restrict out);
unsigned char *vbenc16(unsigned short *__restrict in, unsigned n, 				  unsigned char  *__restrict out);
unsigned char *vbenc64(uint64_t       *__restrict in, unsigned n, 			      unsigned char  *__restrict out);
#define vbenc32(in,n,out) vbenc(in,n,out)
#define vbdec32(in,n,out) vbdec(in,n,out)

// decompress buffer into an array of n unsigned values. Return value = end of decompressed buffer in
unsigned char *vbdec   (unsigned char *__restrict in, unsigned n, 				  unsigned       *__restrict out);
unsigned char *vbdec16(unsigned char  *__restrict in, unsigned n, 				  unsigned short *__restrict out);
unsigned char *vbdec64(unsigned char  *__restrict in, unsigned n, 			      uint64_t       *__restrict out);
//---------------------------------- increasing integer lists -------------------------------------------------------------
unsigned char *vbdenc  (unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *vbddec  (unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

//---------------------------------- strictly increasing (never remaining constant or decreasing) integer lists------------
unsigned char *vbd1enc (unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *vbd1dec (unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

//---------------------------------- variable byte : 15 bits integer lists ------------------------------------------------
#define vbput16(__op, __x) do { unsigned _x = __x; if(likely(_x < 0x80)) *__op++ = _x; else { *__op++ = (_x) >> 8 | 0x80; *__op++ = _x; } } while(0)
#define vbgeta16(__ip,__x, __act) do { if((__x = *__ip++) > 0x7f) __x = (__x & 0x7f) << 8 | *__ip++; __act; } while(0)

#define vblen16(__x) ((__x) > 0x7f?2:1)
#define vbget16(__ip) ({ unsigned _x; vbgeta16(__ip, _x, ;); _x; })

#ifdef __cplusplus
}
#endif

#endif
