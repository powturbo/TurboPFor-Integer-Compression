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
//      bitunpack_.h - "Integer Compression" Bit Packing 

  #ifndef BPI
#include "conf.h"
#include "bitutil.h"
#include "bitunpack.h"
#define PAD8(__x) (((__x)+7)/8)
#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wunsequenced"

//-----------------------------------------------------------------------------------------------------------------
#define DSTI(__op)
#define BPI(__w, __x, __parm) __w
#include __FILE__
unsigned char *bitunpack32(  unsigned char *__restrict in, unsigned n, unsigned           *__restrict out				 , unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out,    0); return ip; }
unsigned char *bitunpack16(  unsigned char *__restrict in, unsigned n, unsigned short     *__restrict out				 , unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out,    0); return ip; }
unsigned char *bitunpack64(  unsigned char *__restrict in, unsigned n, uint64_t           *__restrict out				 , unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK64(in, n, b, out,    0); return ip; }
#undef BPI
#undef DSTI
//-----------------------------------------------------------------------------------------------------------------
#define DSTI(__op)
#define BPI(__w, __x, __parm) (__parm += (__w) + 1)
#include __FILE__
unsigned char *bitd1unpack32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitd1unpack16(unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI
#undef DSTI

//------------------------------------------------------------------------------------------
#define DSTI(__op)
#define BPI(__w, __x, __parm) (__parm += (__w))
#include __FILE__
unsigned char *bitdunpack32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitdunpack16( unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI
#undef DSTI


//------------------------------------------------------------------------------------------
#define DSTI(__op)
#define BPI(__w, __x, __parm) (__parm += zigzagdec32(__w))
#include __FILE__
unsigned char *bitzunpack32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
//unsigned char *bitzunpack16( unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI
#undef DSTI

//------------------------------------------------------------------------------------------
#define DSTI(__op)
#define BPI(__w, __x, __parm) (__parm + (__w))
#include __FILE__

unsigned char *bitfunpack32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitfunpack16( unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI
#undef DSTI

//------------------------------------------------------------------------------------------
#define DSTI(__op) start += 32
#define BPI(__w, __x, __parm) (__parm + (__w)+__x+1)
#include __FILE__
unsigned char *bitf1unpack32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitf1unpack16(unsigned char *__restrict in, unsigned n, unsigned short *__restrict out, unsigned start, unsigned b) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI
#undef DSTI


#pragma clang diagnostic pop

  #else
#include <stdint.h>
#define DST( __op, __x, __w, __parm) *__op++ = BPI(__w, __x, __parm)  	//__op[__x] = BPI(__w,__x,__parm) // 

#define USE_BITUNPACK 64

  #if USE_BITUNPACK == 64
#include "bitunpack64_.h"
#define BITUNPACK32(__ip, __n, __nbits, __op, __parm) { typeof(__op[0]) *__ope = __op + __n,*_op=__op;\
  switch(__nbits) {\
    case  0: do BITUNPACK64_0( __ip, __op, __parm) while(__op<__ope); break;\
    case  1: do BITUNPACK64_1( __ip, __op, __parm) while(__op<__ope); break;\
    case  2: do BITUNPACK64_2( __ip, __op, __parm) while(__op<__ope); break;\
    case  3: do BITUNPACK64_3( __ip, __op, __parm) while(__op<__ope); break;\
    case  4: do BITUNPACK64_4( __ip, __op, __parm) while(__op<__ope); break;\
    case  5: do BITUNPACK64_5( __ip, __op, __parm) while(__op<__ope); break;\
    case  6: do BITUNPACK64_6( __ip, __op, __parm) while(__op<__ope); break;\
    case  7: do BITUNPACK64_7( __ip, __op, __parm) while(__op<__ope); break;\
    case  8: do BITUNPACK64_8( __ip, __op, __parm) while(__op<__ope); break;\
    case  9: do BITUNPACK64_9( __ip, __op, __parm) while(__op<__ope); break;\
    case 10: do BITUNPACK64_10(__ip, __op, __parm) while(__op<__ope); break;\
    case 11: do BITUNPACK64_11(__ip, __op, __parm) while(__op<__ope); break;\
    case 12: do BITUNPACK64_12(__ip, __op, __parm) while(__op<__ope); break;\
    case 13: do BITUNPACK64_13(__ip, __op, __parm) while(__op<__ope); break;\
    case 14: do BITUNPACK64_14(__ip, __op, __parm) while(__op<__ope); break;\
    case 15: do BITUNPACK64_15(__ip, __op, __parm) while(__op<__ope); break;\
    case 16: do BITUNPACK64_16(__ip, __op, __parm) while(__op<__ope); break;\
    case 17: do BITUNPACK64_17(__ip, __op, __parm) while(__op<__ope); break;\
    case 18: do BITUNPACK64_18(__ip, __op, __parm) while(__op<__ope); break;\
    case 19: do BITUNPACK64_19(__ip, __op, __parm) while(__op<__ope); break;\
    case 20: do BITUNPACK64_20(__ip, __op, __parm) while(__op<__ope); break;\
    case 21: do BITUNPACK64_21(__ip, __op, __parm) while(__op<__ope); break;\
    case 22: do BITUNPACK64_22(__ip, __op, __parm) while(__op<__ope); break;\
    case 23: do BITUNPACK64_23(__ip, __op, __parm) while(__op<__ope); break;\
    case 24: do BITUNPACK64_24(__ip, __op, __parm) while(__op<__ope); break;\
    case 25: do BITUNPACK64_25(__ip, __op, __parm) while(__op<__ope); break;\
    case 26: do BITUNPACK64_26(__ip, __op, __parm) while(__op<__ope); break;\
    case 27: do BITUNPACK64_27(__ip, __op, __parm) while(__op<__ope); break;\
    case 28: do BITUNPACK64_28(__ip, __op, __parm) while(__op<__ope); break;\
    case 29: do BITUNPACK64_29(__ip, __op, __parm) while(__op<__ope); break;\
    case 30: do BITUNPACK64_30(__ip, __op, __parm) while(__op<__ope); break;\
    case 31: do BITUNPACK64_31(__ip, __op, __parm) while(__op<__ope); break;\
    case 32: do BITUNPACK64_32(__ip, __op, __parm) while(__op<__ope); break;\
  }\
} 

#define BITUNPACK64(__ip, __n, __nbits, __op, __parm) { typeof(__op[0]) *__ope = __op + __n,*_op=__op;\
  switch(__nbits) {\
    case  0: do BITUNPACK64_0( __ip, __op, __parm) while(__op<__ope); break;\
    case  1: do BITUNPACK64_1( __ip, __op, __parm) while(__op<__ope); break;\
    case  2: do BITUNPACK64_2( __ip, __op, __parm) while(__op<__ope); break;\
    case  3: do BITUNPACK64_3( __ip, __op, __parm) while(__op<__ope); break;\
    case  4: do BITUNPACK64_4( __ip, __op, __parm) while(__op<__ope); break;\
    case  5: do BITUNPACK64_5( __ip, __op, __parm) while(__op<__ope); break;\
    case  6: do BITUNPACK64_6( __ip, __op, __parm) while(__op<__ope); break;\
    case  7: do BITUNPACK64_7( __ip, __op, __parm) while(__op<__ope); break;\
    case  8: do BITUNPACK64_8( __ip, __op, __parm) while(__op<__ope); break;\
    case  9: do BITUNPACK64_9( __ip, __op, __parm) while(__op<__ope); break;\
    case 10: do BITUNPACK64_10(__ip, __op, __parm) while(__op<__ope); break;\
    case 11: do BITUNPACK64_11(__ip, __op, __parm) while(__op<__ope); break;\
    case 12: do BITUNPACK64_12(__ip, __op, __parm) while(__op<__ope); break;\
    case 13: do BITUNPACK64_13(__ip, __op, __parm) while(__op<__ope); break;\
    case 14: do BITUNPACK64_14(__ip, __op, __parm) while(__op<__ope); break;\
    case 15: do BITUNPACK64_15(__ip, __op, __parm) while(__op<__ope); break;\
    case 16: do BITUNPACK64_16(__ip, __op, __parm) while(__op<__ope); break;\
    case 17: do BITUNPACK64_17(__ip, __op, __parm) while(__op<__ope); break;\
    case 18: do BITUNPACK64_18(__ip, __op, __parm) while(__op<__ope); break;\
    case 19: do BITUNPACK64_19(__ip, __op, __parm) while(__op<__ope); break;\
    case 20: do BITUNPACK64_20(__ip, __op, __parm) while(__op<__ope); break;\
    case 21: do BITUNPACK64_21(__ip, __op, __parm) while(__op<__ope); break;\
    case 22: do BITUNPACK64_22(__ip, __op, __parm) while(__op<__ope); break;\
    case 23: do BITUNPACK64_23(__ip, __op, __parm) while(__op<__ope); break;\
    case 24: do BITUNPACK64_24(__ip, __op, __parm) while(__op<__ope); break;\
    case 25: do BITUNPACK64_25(__ip, __op, __parm) while(__op<__ope); break;\
    case 26: do BITUNPACK64_26(__ip, __op, __parm) while(__op<__ope); break;\
    case 27: do BITUNPACK64_27(__ip, __op, __parm) while(__op<__ope); break;\
    case 28: do BITUNPACK64_28(__ip, __op, __parm) while(__op<__ope); break;\
    case 29: do BITUNPACK64_29(__ip, __op, __parm) while(__op<__ope); break;\
    case 30: do BITUNPACK64_30(__ip, __op, __parm) while(__op<__ope); break;\
    case 31: do BITUNPACK64_31(__ip, __op, __parm) while(__op<__ope); break;\
    case 32: do BITUNPACK64_32(__ip, __op, __parm) while(__op<__ope); break;\
    case 33: do BITUNPACK64_33(__ip, __op, __parm) while(__op<__ope); break;\
    case 34: do BITUNPACK64_34(__ip, __op, __parm) while(__op<__ope); break;\
    case 35: do BITUNPACK64_35(__ip, __op, __parm) while(__op<__ope); break;\
    case 36: do BITUNPACK64_36(__ip, __op, __parm) while(__op<__ope); break;\
    case 37: do BITUNPACK64_37(__ip, __op, __parm) while(__op<__ope); break;\
    case 38: do BITUNPACK64_38(__ip, __op, __parm) while(__op<__ope); break;\
    case 39: do BITUNPACK64_39(__ip, __op, __parm) while(__op<__ope); break;\
    case 40: do BITUNPACK64_40(__ip, __op, __parm) while(__op<__ope); break;\
    case 41: do BITUNPACK64_41(__ip, __op, __parm) while(__op<__ope); break;\
    case 42: do BITUNPACK64_42(__ip, __op, __parm) while(__op<__ope); break;\
    case 43: do BITUNPACK64_43(__ip, __op, __parm) while(__op<__ope); break;\
    case 44: do BITUNPACK64_44(__ip, __op, __parm) while(__op<__ope); break;\
    case 45: do BITUNPACK64_45(__ip, __op, __parm) while(__op<__ope); break;\
    case 46: do BITUNPACK64_46(__ip, __op, __parm) while(__op<__ope); break;\
    case 47: do BITUNPACK64_47(__ip, __op, __parm) while(__op<__ope); break;\
    case 48: do BITUNPACK64_48(__ip, __op, __parm) while(__op<__ope); break;\
    case 49: do BITUNPACK64_49(__ip, __op, __parm) while(__op<__ope); break;\
    case 50: do BITUNPACK64_50(__ip, __op, __parm) while(__op<__ope); break;\
    case 51: do BITUNPACK64_51(__ip, __op, __parm) while(__op<__ope); break;\
    case 52: do BITUNPACK64_52(__ip, __op, __parm) while(__op<__ope); break;\
    case 53: do BITUNPACK64_53(__ip, __op, __parm) while(__op<__ope); break;\
    case 54: do BITUNPACK64_54(__ip, __op, __parm) while(__op<__ope); break;\
    case 55: do BITUNPACK64_55(__ip, __op, __parm) while(__op<__ope); break;\
    case 56: do BITUNPACK64_56(__ip, __op, __parm) while(__op<__ope); break;\
    case 57: do BITUNPACK64_57(__ip, __op, __parm) while(__op<__ope); break;\
    case 58: do BITUNPACK64_58(__ip, __op, __parm) while(__op<__ope); break;\
    case 59: do BITUNPACK64_59(__ip, __op, __parm) while(__op<__ope); break;\
    case 60: do BITUNPACK64_60(__ip, __op, __parm) while(__op<__ope); break;\
    case 61: do BITUNPACK64_61(__ip, __op, __parm) while(__op<__ope); break;\
    case 62: do BITUNPACK64_62(__ip, __op, __parm) while(__op<__ope); break;\
    case 63: do BITUNPACK64_63(__ip, __op, __parm) while(__op<__ope); break;\
    case 64: do BITUNPACK64_64(__ip, __op, __parm) while(__op<__ope); break;\
  }\
} 

  #elif USE_BITUNPACK == 32
#include "bitunpack32_.h" // Not included in the github package
#define BITUNPACK32(__ip, __n, __nbits, __op, __parm) { typeof(__op[0]) *__ope = __op + __n;\
  switch(__nbits) {\
    case  0: do BITUNPACK32_0( __ip, __op, __parm) while(__op<__ope); break;\
    case  1: do BITUNPACK32_1( __ip, __op, __parm) while(__op<__ope); break;\
    case  2: do BITUNPACK32_2( __ip, __op, __parm) while(__op<__ope); break;\
    case  3: do BITUNPACK32_3( __ip, __op, __parm) while(__op<__ope); break;\
    case  4: do BITUNPACK32_4( __ip, __op, __parm) while(__op<__ope); break;\
    case  5: do BITUNPACK32_5( __ip, __op, __parm) while(__op<__ope); break;\
    case  6: do BITUNPACK32_6( __ip, __op, __parm) while(__op<__ope); break;\
    case  7: do BITUNPACK32_7( __ip, __op, __parm) while(__op<__ope); break;\
    case  8: do BITUNPACK32_8( __ip, __op, __parm) while(__op<__ope); break;\
    case  9: do BITUNPACK32_9( __ip, __op, __parm) while(__op<__ope); break;\
    case 10: do BITUNPACK32_10(__ip, __op, __parm) while(__op<__ope); break;\
    case 11: do BITUNPACK32_11(__ip, __op, __parm) while(__op<__ope); break;\
    case 12: do BITUNPACK32_12(__ip, __op, __parm) while(__op<__ope); break;\
    case 13: do BITUNPACK32_13(__ip, __op, __parm) while(__op<__ope); break;\
    case 14: do BITUNPACK32_14(__ip, __op, __parm) while(__op<__ope); break;\
    case 15: do BITUNPACK32_15(__ip, __op, __parm) while(__op<__ope); break;\
    case 16: do BITUNPACK32_16(__ip, __op, __parm) while(__op<__ope); break;\
    case 17: do BITUNPACK32_17(__ip, __op, __parm) while(__op<__ope); break;\
    case 18: do BITUNPACK32_18(__ip, __op, __parm) while(__op<__ope); break;\
    case 19: do BITUNPACK32_19(__ip, __op, __parm) while(__op<__ope); break;\
    case 20: do BITUNPACK32_20(__ip, __op, __parm) while(__op<__ope); break;\
    case 21: do BITUNPACK32_21(__ip, __op, __parm) while(__op<__ope); break;\
    case 22: do BITUNPACK32_22(__ip, __op, __parm) while(__op<__ope); break;\
    case 23: do BITUNPACK32_23(__ip, __op, __parm) while(__op<__ope); break;\
    case 24: do BITUNPACK32_24(__ip, __op, __parm) while(__op<__ope); break;\
    case 25: do BITUNPACK32_25(__ip, __op, __parm) while(__op<__ope); break;\
    case 26: do BITUNPACK32_26(__ip, __op, __parm) while(__op<__ope); break;\
    case 27: do BITUNPACK32_27(__ip, __op, __parm) while(__op<__ope); break;\
    case 28: do BITUNPACK32_28(__ip, __op, __parm) while(__op<__ope); break;\
    case 29: do BITUNPACK32_29(__ip, __op, __parm) while(__op<__ope); break;\
    case 30: do BITUNPACK32_30(__ip, __op, __parm) while(__op<__ope); break;\
    case 31: do BITUNPACK32_31(__ip, __op, __parm) while(__op<__ope); break;\
    case 32: do BITUNPACK32_32(__ip, __op, __parm) while(__op<__ope); break;\
  }\
} 
  #endif
  #endif
