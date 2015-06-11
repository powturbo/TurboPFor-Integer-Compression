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
/*#define BITUNBLKV32_0(ip, i, op, parm) {\
  OPU(op,i*0+ 0, 0, parm);\
  OPU(op,i*0+ 1, 0, parm);\
  OPU(op,i*0+ 2, 0, parm);\
  OPU(op,i*0+ 3, 0, parm);\
  OPU(op,i*0+ 4, 0, parm);\
  OPU(op,i*0+ 5, 0, parm);\
  OPU(op,i*0+ 6, 0, parm);\
  OPU(op,i*0+ 7, 0, parm);\
  OPU(op,i*0+ 8, 0, parm);\
  OPU(op,i*0+ 9, 0, parm);\
  OPU(op,i*0+10, 0, parm);\
  OPU(op,i*0+11, 0, parm);\
  OPU(op,i*0+12, 0, parm);\
  OPU(op,i*0+13, 0, parm);\
  OPU(op,i*0+14, 0, parm);\
  OPU(op,i*0+15, 0, parm);\
  OPU(op,i*0+16, 0, parm);\
  OPU(op,i*0+17, 0, parm);\
  OPU(op,i*0+18, 0, parm);\
  OPU(op,i*0+19, 0, parm);\
  OPU(op,i*0+20, 0, parm);\
  OPU(op,i*0+21, 0, parm);\
  OPU(op,i*0+22, 0, parm);\
  OPU(op,i*0+23, 0, parm);\
  OPU(op,i*0+24, 0, parm);\
  OPU(op,i*0+25, 0, parm);\
  OPU(op,i*0+26, 0, parm);\
  OPU(op,i*0+27, 0, parm);\
  OPU(op,i*0+28, 0, parm);\
  OPU(op,i*0+29, 0, parm);\
  OPU(op,i*0+30, 0, parm);\
  OPU(op,i*0+31, 0, parm);;\
}*/

#define BITUNPACKV32_0(ip,  op, parm) {\
  BITUNBLKV32_0(ip, 0, op, parm);\
}

#define BITUNBLKV32_1(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 25), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 27), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 28), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 29), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 30), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_1(ip,  op, parm) {\
  BITUNBLKV32_1(ip, 0, op, parm);\
}

#define BITUNBLKV32_2(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*16+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*16+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*16+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*16+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*16+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*16+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*16+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*16+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*16+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22), mv);                                                                                                             VSTO(op,i*16+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24), mv);                                                                                                             VSTO(op,i*16+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26), mv);                                                                                                             VSTO(op,i*16+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 28), mv);                                                                                                             VSTO(op,i*16+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_2(ip,  op, parm) {\
  BITUNBLKV32_2(ip, 0, op, parm);\
  BITUNBLKV32_2(ip, 1, op, parm);\
}

#define BITUNBLKV32_3(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 27), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 25), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 28), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_3(ip,  op, parm) {\
  BITUNBLKV32_3(ip, 0, op, parm);\
}

#define BITUNBLKV32_4(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*8+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*8+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*8+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*8+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*8+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*8+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24), mv);                                                                                                             VSTO(op,i*8+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);                                                                                                                  VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACKV32_4(ip,  op, parm) {\
  BITUNBLKV32_4(ip, 0, op, parm);\
  BITUNBLKV32_4(ip, 1, op, parm);\
  BITUNBLKV32_4(ip, 2, op, parm);\
  BITUNBLKV32_4(ip, 3, op, parm);\
}

#define BITUNBLKV32_5(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 25), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+25,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_5(ip,  op, parm) {\
  BITUNBLKV32_5(ip, 0, op, parm);\
}

#define BITUNBLKV32_6(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*16+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*16+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*16+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24), mv);                                                                                                             VSTO(op,i*16+ 4,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*16+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*16+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22), mv);                                                                                                             VSTO(op,i*16+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*16+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*16+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*16+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*16+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_6(ip,  op, parm) {\
  BITUNBLKV32_6(ip, 0, op, parm);\
  BITUNBLKV32_6(ip, 1, op, parm);\
}

#define BITUNBLKV32_7(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_7(ip,  op, parm) {\
  BITUNBLKV32_7(ip, 0, op, parm);\
}

#define BITUNBLKV32_8(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*4+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*4+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*4+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);                                                                                                                  VSTO(op,i*4+ 3,ov,parm); ;\
}

#define BITUNPACKV32_8(ip,  op, parm) {\
  BITUNBLKV32_8(ip, 0, op, parm);\
  BITUNBLKV32_8(ip, 1, op, parm);\
  BITUNBLKV32_8(ip, 2, op, parm);\
  BITUNBLKV32_8(ip, 3, op, parm);\
  BITUNBLKV32_8(ip, 4, op, parm);\
  BITUNBLKV32_8(ip, 5, op, parm);\
  BITUNBLKV32_8(ip, 6, op, parm);\
  BITUNBLKV32_8(ip, 7, op, parm);\
}

#define BITUNBLKV32_9(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_9(ip,  op, parm) {\
  BITUNBLKV32_9(ip, 0, op, parm);\
}

#define BITUNBLKV32_10(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*16+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*16+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*16+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*16+ 5,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*16+ 8,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*16+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*16+11,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*16+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*16+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_10(ip,  op, parm) {\
  BITUNBLKV32_10(ip, 0, op, parm);\
  BITUNBLKV32_10(ip, 1, op, parm);\
}

#define BITUNBLKV32_11(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+29,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 21);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_11(ip,  op, parm) {\
  BITUNBLKV32_11(ip, 0, op, parm);\
}

#define BITUNBLKV32_12(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*8+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*8+ 1,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*8+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*8+ 4,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*8+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);                                                                                                                  VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACKV32_12(ip,  op, parm) {\
  BITUNBLKV32_12(ip, 0, op, parm);\
  BITUNBLKV32_12(ip, 1, op, parm);\
  BITUNBLKV32_12(ip, 2, op, parm);\
  BITUNBLKV32_12(ip, 3, op, parm);\
}

#define BITUNBLKV32_13(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+29,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 19);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_13(ip,  op, parm) {\
  BITUNBLKV32_13(ip, 0, op, parm);\
}

#define BITUNBLKV32_14(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*16+ 1,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*16+ 3,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*16+ 5,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*16+ 8,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*16+10,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*16+12,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*16+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 18);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_14(ip,  op, parm) {\
  BITUNBLKV32_14(ip, 0, op, parm);\
  BITUNBLKV32_14(ip, 1, op, parm);\
}

#define BITUNBLKV32_15(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15), mv);                                                                                                             VSTO(op,i*32+ 1,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16), mv);                                                                                                             VSTO(op,i*32+16,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+25,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+29,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+30,ov,parm); \
  ov =               _mm_srli_epi32(iv, 17);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_15(ip,  op, parm) {\
  BITUNBLKV32_15(ip, 0, op, parm);\
}

#define BITUNBLKV32_16(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*2+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 16);                                                                                                                  VSTO(op,i*2+ 1,ov,parm); ;\
}

#define BITUNPACKV32_16(ip,  op, parm) {\
  BITUNBLKV32_16(ip, 0, op, parm);\
  BITUNBLKV32_16(ip, 1, op, parm);\
  BITUNBLKV32_16(ip, 2, op, parm);\
  BITUNBLKV32_16(ip, 3, op, parm);\
  BITUNBLKV32_16(ip, 4, op, parm);\
  BITUNBLKV32_16(ip, 5, op, parm);\
  BITUNBLKV32_16(ip, 6, op, parm);\
  BITUNBLKV32_16(ip, 7, op, parm);\
  BITUNBLKV32_16(ip, 8, op, parm);\
  BITUNBLKV32_16(ip, 9, op, parm);\
  BITUNBLKV32_16(ip, 10, op, parm);\
  BITUNBLKV32_16(ip, 11, op, parm);\
  BITUNBLKV32_16(ip, 12, op, parm);\
  BITUNBLKV32_16(ip, 13, op, parm);\
  BITUNBLKV32_16(ip, 14, op, parm);\
  BITUNBLKV32_16(ip, 15, op, parm);\
}

#define BITUNBLKV32_17(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_17(ip,  op, parm) {\
  BITUNBLKV32_17(ip, 0, op, parm);\
}

#define BITUNBLKV32_18(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*16+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*16+ 4,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*16+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*16+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*16+11,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*16+13,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_18(ip,  op, parm) {\
  BITUNBLKV32_18(ip, 0, op, parm);\
  BITUNBLKV32_18(ip, 1, op, parm);\
}

#define BITUNBLKV32_19(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv, 13);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_19(ip,  op, parm) {\
  BITUNBLKV32_19(ip, 0, op, parm);\
}

#define BITUNBLKV32_20(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*8+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*8+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*8+ 5,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);                                                                                                                  VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACKV32_20(ip,  op, parm) {\
  BITUNBLKV32_20(ip, 0, op, parm);\
  BITUNBLKV32_20(ip, 1, op, parm);\
  BITUNBLKV32_20(ip, 2, op, parm);\
  BITUNBLKV32_20(ip, 3, op, parm);\
}

#define BITUNBLKV32_21(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10), mv);                                                                                                             VSTO(op,i*32+ 2,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9), mv);                                                                                                             VSTO(op,i*32+ 5,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+ 8,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+11,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+20,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+23,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =               _mm_srli_epi32(iv, 13);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+25,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+26,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+29,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv, 11);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_21(ip,  op, parm) {\
  BITUNBLKV32_21(ip, 0, op, parm);\
}

#define BITUNBLKV32_22(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*16+ 3,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*16+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*16+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*16+12,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_22(ip,  op, parm) {\
  BITUNBLKV32_22(ip, 0, op, parm);\
  BITUNBLKV32_22(ip, 1, op, parm);\
}

#define BITUNBLKV32_23(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+ 3,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+ 7,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =               _mm_srli_epi32(iv, 11);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+14,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7), mv);                                                                                                             VSTO(op,i*32+17,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8), mv);                                                                                                             VSTO(op,i*32+24,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =               _mm_srli_epi32(iv, 13);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+28,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv,  9);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_23(ip,  op, parm) {\
  BITUNBLKV32_23(ip, 0, op, parm);\
}

#define BITUNBLKV32_24(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*4+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*4+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*4+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);                                                                                                                  VSTO(op,i*4+ 3,ov,parm); ;\
}

#define BITUNPACKV32_24(ip,  op, parm) {\
  BITUNBLKV32_24(ip, 0, op, parm);\
  BITUNBLKV32_24(ip, 1, op, parm);\
  BITUNBLKV32_24(ip, 2, op, parm);\
  BITUNBLKV32_24(ip, 3, op, parm);\
  BITUNBLKV32_24(ip, 4, op, parm);\
  BITUNBLKV32_24(ip, 5, op, parm);\
  BITUNBLKV32_24(ip, 6, op, parm);\
  BITUNBLKV32_24(ip, 7, op, parm);\
}

#define BITUNBLKV32_25(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv, 11);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+ 4,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+ 9,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5), mv);                                                                                                             VSTO(op,i*32+13,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =               _mm_srli_epi32(iv,  9);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+18,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =               _mm_srli_epi32(iv, 13);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6), mv);                                                                                                             VSTO(op,i*32+22,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+27,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv,  7);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_25(ip,  op, parm) {\
  BITUNBLKV32_25(ip, 0, op, parm);\
}

#define BITUNBLKV32_26(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*16+ 5,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*16+10,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =               _mm_srli_epi32(iv,  6);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_26(ip,  op, parm) {\
  BITUNBLKV32_26(ip, 0, op, parm);\
  BITUNBLKV32_26(ip, 1, op, parm);\
}

#define BITUNBLKV32_27(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =               _mm_srli_epi32(iv,  7);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+ 6,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =               _mm_srli_epi32(iv,  9);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4), mv);                                                                                                             VSTO(op,i*32+12,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =               _mm_srli_epi32(iv, 11);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =               _mm_srli_epi32(iv,  6);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+19,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =               _mm_srli_epi32(iv, 13);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3), mv);                                                                                                             VSTO(op,i*32+25,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv,  5);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_27(ip,  op, parm) {\
  BITUNBLKV32_27(ip, 0, op, parm);\
}

#define BITUNBLKV32_28(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*8+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv,  4);                                                                                                                  VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACKV32_28(ip,  op, parm) {\
  BITUNBLKV32_28(ip, 0, op, parm);\
  BITUNBLKV32_28(ip, 1, op, parm);\
  BITUNBLKV32_28(ip, 2, op, parm);\
  BITUNBLKV32_28(ip, 3, op, parm);\
}

#define BITUNBLKV32_29(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv, 11);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =               _mm_srli_epi32(iv,  5);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2), mv);                                                                                                             VSTO(op,i*32+10,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =               _mm_srli_epi32(iv, 13);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =               _mm_srli_epi32(iv,  7);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =               _mm_srli_epi32(iv,  4);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1), mv);                                                                                                             VSTO(op,i*32+21,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =               _mm_srli_epi32(iv,  9);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =               _mm_srli_epi32(iv,  6);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv,  3);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_29(ip,  op, parm) {\
  BITUNBLKV32_29(ip, 0, op, parm);\
}

#define BITUNBLKV32_30(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*16+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =               _mm_srli_epi32(iv,  6);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =               _mm_srli_epi32(iv,  4);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =               _mm_srli_epi32(iv,  2);                                                                                                                  VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACKV32_30(ip,  op, parm) {\
  BITUNBLKV32_30(ip, 0, op, parm);\
  BITUNBLKV32_30(ip, 1, op, parm);\
}

#define BITUNBLKV32_31(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*32+ 0,ov,parm); \
  ov =               _mm_srli_epi32(iv, 31);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =               _mm_srli_epi32(iv, 30);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =               _mm_srli_epi32(iv, 29);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =               _mm_srli_epi32(iv, 28);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =               _mm_srli_epi32(iv, 27);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =               _mm_srli_epi32(iv, 26);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =               _mm_srli_epi32(iv, 25);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =               _mm_srli_epi32(iv, 24);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =               _mm_srli_epi32(iv, 23);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =               _mm_srli_epi32(iv, 22);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =               _mm_srli_epi32(iv, 21);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =               _mm_srli_epi32(iv, 20);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =               _mm_srli_epi32(iv, 19);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =               _mm_srli_epi32(iv, 18);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =               _mm_srli_epi32(iv, 17);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =               _mm_srli_epi32(iv, 16);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =               _mm_srli_epi32(iv, 15);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =               _mm_srli_epi32(iv, 14);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =               _mm_srli_epi32(iv, 13);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =               _mm_srli_epi32(iv, 12);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =               _mm_srli_epi32(iv, 11);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =               _mm_srli_epi32(iv, 10);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =               _mm_srli_epi32(iv,  9);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =               _mm_srli_epi32(iv,  8);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =               _mm_srli_epi32(iv,  7);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =               _mm_srli_epi32(iv,  6);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =               _mm_srli_epi32(iv,  5);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =               _mm_srli_epi32(iv,  4);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =               _mm_srli_epi32(iv,  3);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =               _mm_srli_epi32(iv,  2);         iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =               _mm_srli_epi32(iv,  1);                                                                                                                  VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACKV32_31(ip,  op, parm) {\
  BITUNBLKV32_31(ip, 0, op, parm);\
}

#define BITUNBLKV32_32(ip, i, op, parm) { __m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     , mv);                                                                                                             VSTO(op,i*1+ 0,ov,parm); ;\
}

#define BITUNPACKV32_32(ip,  op, parm) {\
  BITUNBLKV32_32(ip, 0, op, parm);\
  BITUNBLKV32_32(ip, 1, op, parm);\
  BITUNBLKV32_32(ip, 2, op, parm);\
  BITUNBLKV32_32(ip, 3, op, parm);\
  BITUNBLKV32_32(ip, 4, op, parm);\
  BITUNBLKV32_32(ip, 5, op, parm);\
  BITUNBLKV32_32(ip, 6, op, parm);\
  BITUNBLKV32_32(ip, 7, op, parm);\
  BITUNBLKV32_32(ip, 8, op, parm);\
  BITUNBLKV32_32(ip, 9, op, parm);\
  BITUNBLKV32_32(ip, 10, op, parm);\
  BITUNBLKV32_32(ip, 11, op, parm);\
  BITUNBLKV32_32(ip, 12, op, parm);\
  BITUNBLKV32_32(ip, 13, op, parm);\
  BITUNBLKV32_32(ip, 14, op, parm);\
  BITUNBLKV32_32(ip, 15, op, parm);\
  BITUNBLKV32_32(ip, 16, op, parm);\
  BITUNBLKV32_32(ip, 17, op, parm);\
  BITUNBLKV32_32(ip, 18, op, parm);\
  BITUNBLKV32_32(ip, 19, op, parm);\
  BITUNBLKV32_32(ip, 20, op, parm);\
  BITUNBLKV32_32(ip, 21, op, parm);\
  BITUNBLKV32_32(ip, 22, op, parm);\
  BITUNBLKV32_32(ip, 23, op, parm);\
  BITUNBLKV32_32(ip, 24, op, parm);\
  BITUNBLKV32_32(ip, 25, op, parm);\
  BITUNBLKV32_32(ip, 26, op, parm);\
  BITUNBLKV32_32(ip, 27, op, parm);\
  BITUNBLKV32_32(ip, 28, op, parm);\
  BITUNBLKV32_32(ip, 29, op, parm);\
  BITUNBLKV32_32(ip, 30, op, parm);\
  BITUNBLKV32_32(ip, 31, op, parm);\
}

