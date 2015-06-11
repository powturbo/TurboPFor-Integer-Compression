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
//     bitpackv32.h - "Integer Compression" simd bit packing 
#define BITBLKV32_1(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv),  1));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv),  2));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv),  3));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv),  4));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv),  5));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv),  6));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv),  7));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv),  8));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv),  9));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv), 10));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv), 11));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv), 12));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv), 13));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv), 14));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv), 15));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv), 17));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv), 18));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv), 19));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv), 20));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv), 21));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv), 22));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv), 23));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv), 24));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv), 25));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv), 26));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv), 27));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv), 28));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv), 29));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv), 30));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 31)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_1(ip,  op, parm) {\
  BITBLKV32_1(ip, 0, op, parm);  IPPE(ip); OPPE(op += 1*4/sizeof(op[0]));\
}

#define BITBLKV32_2(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 1, iv),  2));\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 2, iv),  4));\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 3, iv),  6));\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 4, iv),  8));\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 5, iv), 10));\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 6, iv), 12));\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 7, iv), 14));\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 8, iv), 16));\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 9, iv), 18));\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+10, iv), 20));\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+11, iv), 22));\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+12, iv), 24));\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+13, iv), 26));\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+14, iv), 28));\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv), 30)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_2(ip,  op, parm) {\
  BITBLKV32_2(ip, 0, op, parm);\
  BITBLKV32_2(ip, 1, op, parm);  IPPE(ip); OPPE(op += 2*4/sizeof(op[0]));\
}

#define BITBLKV32_3(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv),  3));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv),  6));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv),  9));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv), 12));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv), 15));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv), 18));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv), 21));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv), 24));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv), 27));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv),  1));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv),  4));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv),  7));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv), 10));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv), 13));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv), 19));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv), 22));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv), 25));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv), 28));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv),  2));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv),  5));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv),  8));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv), 11));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv), 14));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv), 17));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv), 20));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv), 23));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv), 26));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 29)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_3(ip,  op, parm) {\
  BITBLKV32_3(ip, 0, op, parm);  IPPE(ip); OPPE(op += 3*4/sizeof(op[0]));\
}

#define BITBLKV32_4(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*8+ 0, iv, parm); ov =                                      IPP(ip, i*8+ 0, iv);\
  VSTI(ip, i*8+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 1, iv),  4));\
  VSTI(ip, i*8+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 2, iv),  8));\
  VSTI(ip, i*8+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 3, iv), 12));\
  VSTI(ip, i*8+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 4, iv), 16));\
  VSTI(ip, i*8+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 5, iv), 20));\
  VSTI(ip, i*8+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 6, iv), 24));\
  VSTI(ip, i*8+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 7, iv), 28)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_4(ip,  op, parm) {\
  BITBLKV32_4(ip, 0, op, parm);\
  BITBLKV32_4(ip, 1, op, parm);\
  BITBLKV32_4(ip, 2, op, parm);\
  BITBLKV32_4(ip, 3, op, parm);  IPPE(ip); OPPE(op += 4*4/sizeof(op[0]));\
}

#define BITBLKV32_5(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv),  5));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv), 10));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv), 15));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv), 20));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv), 25));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv),  3));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv),  8));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv), 13));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv), 18));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv), 23));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv),  1));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv),  6));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv), 11));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv), 21));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv), 26));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv),  4));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv),  9));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv), 14));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv), 19));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv), 24));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv),  2));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv),  7));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv), 12));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv), 17));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv), 22));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 27)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_5(ip,  op, parm) {\
  BITBLKV32_5(ip, 0, op, parm);  IPPE(ip); OPPE(op += 5*4/sizeof(op[0]));\
}

#define BITBLKV32_6(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 1, iv),  6));\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 2, iv), 12));\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 3, iv), 18));\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 4, iv), 24));\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 5, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 6, iv),  4));\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 7, iv), 10));\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 8, iv), 16));\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 9, iv), 22));\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+10, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+11, iv),  2));\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+12, iv),  8));\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+13, iv), 14));\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+14, iv), 20));\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv), 26)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_6(ip,  op, parm) {\
  BITBLKV32_6(ip, 0, op, parm);\
  BITBLKV32_6(ip, 1, op, parm);  IPPE(ip); OPPE(op += 6*4/sizeof(op[0]));\
}

#define BITBLKV32_7(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv),  7));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv), 14));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv), 21));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv),  3));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv), 10));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv), 17));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv), 24));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv),  6));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv), 13));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv), 20));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv),  2));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv),  9));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv), 23));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv),  5));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv), 12));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv), 19));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv),  1));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv),  8));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv), 15));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv), 22));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv),  4));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv), 11));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv), 18));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 25)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_7(ip,  op, parm) {\
  BITBLKV32_7(ip, 0, op, parm);  IPPE(ip); OPPE(op += 7*4/sizeof(op[0]));\
}

#define BITBLKV32_8(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*4+ 0, iv, parm); ov =                                      IPP(ip, i*4+ 0, iv);\
  VSTI(ip, i*4+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*4+ 1, iv),  8));\
  VSTI(ip, i*4+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*4+ 2, iv), 16));\
  VSTI(ip, i*4+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*4+ 3, iv), 24)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_8(ip,  op, parm) {\
  BITBLKV32_8(ip, 0, op, parm);\
  BITBLKV32_8(ip, 1, op, parm);\
  BITBLKV32_8(ip, 2, op, parm);\
  BITBLKV32_8(ip, 3, op, parm);\
  BITBLKV32_8(ip, 4, op, parm);\
  BITBLKV32_8(ip, 5, op, parm);\
  BITBLKV32_8(ip, 6, op, parm);\
  BITBLKV32_8(ip, 7, op, parm);  IPPE(ip); OPPE(op += 8*4/sizeof(op[0]));\
}

#define BITBLKV32_9(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv),  9));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv), 18));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv),  4));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv), 13));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv), 22));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv),  8));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv), 17));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv),  3));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv), 12));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv), 21));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv),  7));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv),  2));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv), 11));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv), 20));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv),  6));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv), 15));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv),  1));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv), 10));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv), 19));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv),  5));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv), 14));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 23)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_9(ip,  op, parm) {\
  BITBLKV32_9(ip, 0, op, parm);  IPPE(ip); OPPE(op += 9*4/sizeof(op[0]));\
}

#define BITBLKV32_10(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 1, iv), 10));\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 2, iv), 20));\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 3, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 4, iv),  8));\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 5, iv), 18));\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 6, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 7, iv),  6));\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 8, iv), 16));\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 9, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+10, iv),  4));\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+11, iv), 14));\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+12, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+13, iv),  2));\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+14, iv), 12));\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv), 22)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_10(ip,  op, parm) {\
  BITBLKV32_10(ip, 0, op, parm);\
  BITBLKV32_10(ip, 1, op, parm);  IPPE(ip); OPPE(op += 10*4/sizeof(op[0]));\
}

#define BITBLKV32_11(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv), 11));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv),  1));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv), 12));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv),  2));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv), 13));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv),  3));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv), 14));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv),  4));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv), 15));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv),  5));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv),  6));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv), 17));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv),  7));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv), 18));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv),  8));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv), 19));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv),  9));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv), 20));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv), 10));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 21)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_11(ip,  op, parm) {\
  BITBLKV32_11(ip, 0, op, parm);  IPPE(ip); OPPE(op += 11*4/sizeof(op[0]));\
}

#define BITBLKV32_12(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*8+ 0, iv, parm); ov =                                      IPP(ip, i*8+ 0, iv);\
  VSTI(ip, i*8+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 1, iv), 12));\
  VSTI(ip, i*8+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 2, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*8+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 3, iv),  4));\
  VSTI(ip, i*8+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 4, iv), 16));\
  VSTI(ip, i*8+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 5, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*8+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 6, iv),  8));\
  VSTI(ip, i*8+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 7, iv), 20)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_12(ip,  op, parm) {\
  BITBLKV32_12(ip, 0, op, parm);\
  BITBLKV32_12(ip, 1, op, parm);\
  BITBLKV32_12(ip, 2, op, parm);\
  BITBLKV32_12(ip, 3, op, parm);  IPPE(ip); OPPE(op += 12*4/sizeof(op[0]));\
}

#define BITBLKV32_13(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv), 13));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv),  7));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv),  1));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv), 14));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv),  8));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv),  2));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv), 15));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv),  9));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv),  3));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv), 10));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv),  4));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv), 17));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv), 11));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv),  5));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv), 18));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv), 12));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv),  6));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 19)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_13(ip,  op, parm) {\
  BITBLKV32_13(ip, 0, op, parm);  IPPE(ip); OPPE(op += 13*4/sizeof(op[0]));\
}

#define BITBLKV32_14(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 1, iv), 14));\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 2, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 3, iv), 10));\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 4, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 5, iv),  6));\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 6, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 7, iv),  2));\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 8, iv), 16));\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 9, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+10, iv), 12));\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+11, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+12, iv),  8));\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+13, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+14, iv),  4));\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv), 18)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_14(ip,  op, parm) {\
  BITBLKV32_14(ip, 0, op, parm);\
  BITBLKV32_14(ip, 1, op, parm);  IPPE(ip); OPPE(op += 14*4/sizeof(op[0]));\
}

#define BITBLKV32_15(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 1, iv), 15));\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv), 13));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv), 11));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv),  9));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv),  7));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv),  5));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv),  3));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+15, iv),  1));\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+16, iv), 16));\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv), 14));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv), 12));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv), 10));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv),  8));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv),  6));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv),  4));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+30, iv),  2));\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 17)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_15(ip,  op, parm) {\
  BITBLKV32_15(ip, 0, op, parm);  IPPE(ip); OPPE(op += 15*4/sizeof(op[0]));\
}

#define BITBLKV32_16(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*2+ 0, iv, parm); ov =                                      IPP(ip, i*2+ 0, iv);\
  VSTI(ip, i*2+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*2+ 1, iv), 16)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_16(ip,  op, parm) {\
  BITBLKV32_16(ip, 0, op, parm);\
  BITBLKV32_16(ip, 1, op, parm);\
  BITBLKV32_16(ip, 2, op, parm);\
  BITBLKV32_16(ip, 3, op, parm);\
  BITBLKV32_16(ip, 4, op, parm);\
  BITBLKV32_16(ip, 5, op, parm);\
  BITBLKV32_16(ip, 6, op, parm);\
  BITBLKV32_16(ip, 7, op, parm);\
  BITBLKV32_16(ip, 8, op, parm);\
  BITBLKV32_16(ip, 9, op, parm);\
  BITBLKV32_16(ip, 10, op, parm);\
  BITBLKV32_16(ip, 11, op, parm);\
  BITBLKV32_16(ip, 12, op, parm);\
  BITBLKV32_16(ip, 13, op, parm);\
  BITBLKV32_16(ip, 14, op, parm);\
  BITBLKV32_16(ip, 15, op, parm);  IPPE(ip); OPPE(op += 16*4/sizeof(op[0]));\
}

#define BITBLKV32_17(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv),  2));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv),  4));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv),  6));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv),  8));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv), 10));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv), 12));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv), 14));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv),  1));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv),  3));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv),  5));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv),  7));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv),  9));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv), 11));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv), 13));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 15)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_17(ip,  op, parm) {\
  BITBLKV32_17(ip, 0, op, parm);  IPPE(ip); OPPE(op += 17*4/sizeof(op[0]));\
}

#define BITBLKV32_18(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 1, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 2, iv),  4));\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 3, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 4, iv),  8));\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 5, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 6, iv), 12));\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 7, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 8, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 9, iv),  2));\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+10, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+11, iv),  6));\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+12, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+13, iv), 10));\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+14, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv), 14)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_18(ip,  op, parm) {\
  BITBLKV32_18(ip, 0, op, parm);\
  BITBLKV32_18(ip, 1, op, parm);  IPPE(ip); OPPE(op += 18*4/sizeof(op[0]));\
}

#define BITBLKV32_19(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv),  6));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv), 12));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv),  5));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv), 11));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv),  4));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv), 10));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv),  3));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv),  9));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 15)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 17);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv),  2));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv),  8));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv),  1));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv),  7));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 13)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_19(ip,  op, parm) {\
  BITBLKV32_19(ip, 0, op, parm);  IPPE(ip); OPPE(op += 19*4/sizeof(op[0]));\
}

#define BITBLKV32_20(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*8+ 0, iv, parm); ov =                                      IPP(ip, i*8+ 0, iv);\
  VSTI(ip, i*8+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 1, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*8+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 2, iv),  8));\
  VSTI(ip, i*8+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 3, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*8+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 4, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*8+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 5, iv),  4));\
  VSTI(ip, i*8+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 6, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*8+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 7, iv), 12)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_20(ip,  op, parm) {\
  BITBLKV32_20(ip, 0, op, parm);\
  BITBLKV32_20(ip, 1, op, parm);\
  BITBLKV32_20(ip, 2, op, parm);\
  BITBLKV32_20(ip, 3, op, parm);  IPPE(ip); OPPE(op += 20*4/sizeof(op[0]));\
}

#define BITBLKV32_21(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 2, iv), 10));\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 5, iv),  9));\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 8, iv),  8));\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+11, iv),  7));\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv),  6));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv),  5));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 15)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 17);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+20, iv),  4));\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+23, iv),  3));\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 13)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 19);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+26, iv),  2));\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+29, iv),  1));\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv), 11)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_21(ip,  op, parm) {\
  BITBLKV32_21(ip, 0, op, parm);  IPPE(ip); OPPE(op += 21*4/sizeof(op[0]));\
}

#define BITBLKV32_22(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 1, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 2, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 3, iv),  2));\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 4, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 5, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 6, iv),  4));\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 7, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 8, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 9, iv),  6));\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+10, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+11, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+12, iv),  8));\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+13, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+14, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv), 10)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_22(ip,  op, parm) {\
  BITBLKV32_22(ip, 0, op, parm);\
  BITBLKV32_22(ip, 1, op, parm);  IPPE(ip); OPPE(op += 22*4/sizeof(op[0]));\
}

#define BITBLKV32_23(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 3, iv),  5));\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 10)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 22);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 7, iv),  1));\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 15)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 17);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv),  6));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 11)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 21);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+14, iv),  2));\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+17, iv),  7));\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv),  3));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+24, iv),  8));\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 13)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 19);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+28, iv),  4));\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv),  9)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_23(ip,  op, parm) {\
  BITBLKV32_23(ip, 0, op, parm);  IPPE(ip); OPPE(op += 23*4/sizeof(op[0]));\
}

#define BITBLKV32_24(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*4+ 0, iv, parm); ov =                                      IPP(ip, i*4+ 0, iv);\
  VSTI(ip, i*4+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*4+ 1, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*4+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*4+ 2, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*4+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*4+ 3, iv),  8)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_24(ip,  op, parm) {\
  BITBLKV32_24(ip, 0, op, parm);\
  BITBLKV32_24(ip, 1, op, parm);\
  BITBLKV32_24(ip, 2, op, parm);\
  BITBLKV32_24(ip, 3, op, parm);\
  BITBLKV32_24(ip, 4, op, parm);\
  BITBLKV32_24(ip, 5, op, parm);\
  BITBLKV32_24(ip, 6, op, parm);\
  BITBLKV32_24(ip, 7, op, parm);  IPPE(ip); OPPE(op += 24*4/sizeof(op[0]));\
}

#define BITBLKV32_25(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 11)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 21);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 4, iv),  4));\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 15)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 17);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 8)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 24);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 9, iv),  1));\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+13, iv),  5));\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 9)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 23);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+18, iv),  2));\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 13)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 19);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+22, iv),  6));\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 10)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 22);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+27, iv),  3));\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv),  7)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_25(ip,  op, parm) {\
  BITBLKV32_25(ip, 0, op, parm);  IPPE(ip); OPPE(op += 25*4/sizeof(op[0]));\
}

#define BITBLKV32_26(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 1, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 2, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 3, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 4, iv), 8)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 24);\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+ 5, iv),  2));\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 6, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 7, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 8, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 9, iv), 10)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 22);\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+10, iv),  4));\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+11, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+12, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+13, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+14, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv),  6)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_26(ip,  op, parm) {\
  BITBLKV32_26(ip, 0, op, parm);\
  BITBLKV32_26(ip, 1, op, parm);  IPPE(ip); OPPE(op += 26*4/sizeof(op[0]));\
}

#define BITBLKV32_27(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 7)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 25);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+ 6, iv),  2));\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 9)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 23);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+12, iv),  4));\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 11)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 21);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 6)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 26);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+19, iv),  1));\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 13)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 19);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 8)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 24);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+25, iv),  3));\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 15)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 17);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 10)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 22);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv),  5)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_27(ip,  op, parm) {\
  BITBLKV32_27(ip, 0, op, parm);  IPPE(ip); OPPE(op += 27*4/sizeof(op[0]));\
}

#define BITBLKV32_28(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*8+ 0, iv, parm); ov =                                      IPP(ip, i*8+ 0, iv);\
  VSTI(ip, i*8+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 1, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*8+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 2, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*8+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 3, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*8+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 4, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*8+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 5, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*8+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*8+ 6, iv), 8)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 24);\
  VSTI(ip, i*8+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*8+ 7, iv),  4)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_28(ip,  op, parm) {\
  BITBLKV32_28(ip, 0, op, parm);\
  BITBLKV32_28(ip, 1, op, parm);\
  BITBLKV32_28(ip, 2, op, parm);\
  BITBLKV32_28(ip, 3, op, parm);  IPPE(ip); OPPE(op += 28*4/sizeof(op[0]));\
}

#define BITBLKV32_29(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 11)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 21);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 8)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 24);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 5)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 27);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+10, iv),  2));\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 13)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 19);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 10)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 22);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 7)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 25);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 4)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 28);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+21, iv),  1));\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 15)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 17);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 9)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 23);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 6)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 26);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv),  3)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_29(ip,  op, parm) {\
  BITBLKV32_29(ip, 0, op, parm);  IPPE(ip); OPPE(op += 29*4/sizeof(op[0]));\
}

#define BITBLKV32_30(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*16+ 0, iv, parm); ov =                                      IPP(ip, i*16+ 0, iv);\
  VSTI(ip, i*16+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 1, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*16+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 2, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*16+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 3, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*16+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 4, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*16+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 5, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*16+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 6, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*16+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 7, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*16+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 8, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*16+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+ 9, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*16+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+10, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*16+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+11, iv), 10)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 22);\
  VSTI(ip, i*16+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+12, iv), 8)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 24);\
  VSTI(ip, i*16+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+13, iv), 6)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 26);\
  VSTI(ip, i*16+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*16+14, iv), 4)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 28);\
  VSTI(ip, i*16+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*16+15, iv),  2)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_30(ip,  op, parm) {\
  BITBLKV32_30(ip, 0, op, parm);\
  BITBLKV32_30(ip, 1, op, parm);  IPPE(ip); OPPE(op += 30*4/sizeof(op[0]));\
}

#define BITBLKV32_31(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*32+ 0, iv, parm); ov =                                      IPP(ip, i*32+ 0, iv);\
  VSTI(ip, i*32+ 1, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 1, iv), 31)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 1);\
  VSTI(ip, i*32+ 2, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 2, iv), 30)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 2);\
  VSTI(ip, i*32+ 3, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 3, iv), 29)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 3);\
  VSTI(ip, i*32+ 4, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 4, iv), 28)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 4);\
  VSTI(ip, i*32+ 5, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 5, iv), 27)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 5);\
  VSTI(ip, i*32+ 6, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 6, iv), 26)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 6);\
  VSTI(ip, i*32+ 7, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 7, iv), 25)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 7);\
  VSTI(ip, i*32+ 8, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 8, iv), 24)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 8);\
  VSTI(ip, i*32+ 9, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+ 9, iv), 23)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 9);\
  VSTI(ip, i*32+10, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+10, iv), 22)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 10);\
  VSTI(ip, i*32+11, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+11, iv), 21)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 11);\
  VSTI(ip, i*32+12, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+12, iv), 20)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 12);\
  VSTI(ip, i*32+13, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+13, iv), 19)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 13);\
  VSTI(ip, i*32+14, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+14, iv), 18)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 14);\
  VSTI(ip, i*32+15, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+15, iv), 17)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 15);\
  VSTI(ip, i*32+16, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+16, iv), 16)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 16);\
  VSTI(ip, i*32+17, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+17, iv), 15)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 17);\
  VSTI(ip, i*32+18, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+18, iv), 14)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 18);\
  VSTI(ip, i*32+19, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+19, iv), 13)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 19);\
  VSTI(ip, i*32+20, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+20, iv), 12)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 20);\
  VSTI(ip, i*32+21, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+21, iv), 11)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 21);\
  VSTI(ip, i*32+22, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+22, iv), 10)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 22);\
  VSTI(ip, i*32+23, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+23, iv), 9)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 23);\
  VSTI(ip, i*32+24, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+24, iv), 8)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 24);\
  VSTI(ip, i*32+25, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+25, iv), 7)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 25);\
  VSTI(ip, i*32+26, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+26, iv), 6)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 26);\
  VSTI(ip, i*32+27, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+27, iv), 5)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 27);\
  VSTI(ip, i*32+28, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+28, iv), 4)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 28);\
  VSTI(ip, i*32+29, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+29, iv), 3)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 29);\
  VSTI(ip, i*32+30, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(iv = IPP(ip, i*32+30, iv), 2)); _mm_storeu_si128(op++, ov); ov = _mm_srli_epi32(iv, 30);\
  VSTI(ip, i*32+31, iv, parm); ov = _mm_or_si128(ov, _mm_slli_epi32(     IPP(ip, i*32+31, iv),  1)); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_31(ip,  op, parm) {\
  BITBLKV32_31(ip, 0, op, parm);  IPPE(ip); OPPE(op += 31*4/sizeof(op[0]));\
}

#define BITBLKV32_32(ip, i, op, parm) { __m128i ov,iv;\
  VSTI(ip, i*1+ 0, iv, parm); ov =                                      IPP(ip, i*1+ 0, iv); _mm_storeu_si128((__m128i *)op++, ov);\
}

#define BITPACKV32_32(ip,  op, parm) {\
  BITBLKV32_32(ip, 0, op, parm);\
  BITBLKV32_32(ip, 1, op, parm);\
  BITBLKV32_32(ip, 2, op, parm);\
  BITBLKV32_32(ip, 3, op, parm);\
  BITBLKV32_32(ip, 4, op, parm);\
  BITBLKV32_32(ip, 5, op, parm);\
  BITBLKV32_32(ip, 6, op, parm);\
  BITBLKV32_32(ip, 7, op, parm);\
  BITBLKV32_32(ip, 8, op, parm);\
  BITBLKV32_32(ip, 9, op, parm);\
  BITBLKV32_32(ip, 10, op, parm);\
  BITBLKV32_32(ip, 11, op, parm);\
  BITBLKV32_32(ip, 12, op, parm);\
  BITBLKV32_32(ip, 13, op, parm);\
  BITBLKV32_32(ip, 14, op, parm);\
  BITBLKV32_32(ip, 15, op, parm);\
  BITBLKV32_32(ip, 16, op, parm);\
  BITBLKV32_32(ip, 17, op, parm);\
  BITBLKV32_32(ip, 18, op, parm);\
  BITBLKV32_32(ip, 19, op, parm);\
  BITBLKV32_32(ip, 20, op, parm);\
  BITBLKV32_32(ip, 21, op, parm);\
  BITBLKV32_32(ip, 22, op, parm);\
  BITBLKV32_32(ip, 23, op, parm);\
  BITBLKV32_32(ip, 24, op, parm);\
  BITBLKV32_32(ip, 25, op, parm);\
  BITBLKV32_32(ip, 26, op, parm);\
  BITBLKV32_32(ip, 27, op, parm);\
  BITBLKV32_32(ip, 28, op, parm);\
  BITBLKV32_32(ip, 29, op, parm);\
  BITBLKV32_32(ip, 30, op, parm);\
  BITBLKV32_32(ip, 31, op, parm);  IPPE(ip); OPPE(op += 32*4/sizeof(op[0]));\
}

