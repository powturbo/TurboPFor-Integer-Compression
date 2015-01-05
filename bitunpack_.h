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

    - email    : powturbo [AT] gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    bitunpack_.h - "Integer Compression" binary packing 
**/
#include <stdint.h>
#define DST( __op,__x, __w, __parm) *__op++ = BPI(__w, __op, __parm)  	//__op[__x] = BPI(__w,__parm) // 
#define DSTI(__op) 						//__op += 32 // 

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

