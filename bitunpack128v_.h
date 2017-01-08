/**
  Copyright (C) powturbo 2013-2017
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
//  TurboPFor: Integer Compression SIMD bit unpacking
#define BITUNPACK128V32_0(ip,  op, parm) {\
  BITUNBLK128V32_0(ip, 0, op, parm);\
}

#define BITUNBLK128V32_1(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 25),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 27),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 28),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 29),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 30),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_1(ip,  op, parm) {\
  BITUNBLK128V32_1(ip, 0, op, parm);\
}

#define BITUNBLK128V32_2(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*16+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*16+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*16+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*16+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*16+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*16+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*16+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*16+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*16+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22),mv);                                                                                                                 VSTO(op,i*16+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24),mv);                                                                                                                 VSTO(op,i*16+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26),mv);                                                                                                                 VSTO(op,i*16+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 28),mv);                                                                                                                 VSTO(op,i*16+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_2(ip,  op, parm) {\
  BITUNBLK128V32_2(ip, 0, op, parm);\
  BITUNBLK128V32_2(ip, 1, op, parm);\
}

#define BITUNBLK128V32_3(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 27),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 25),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 28),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_3(ip,  op, parm) {\
  BITUNBLK128V32_3(ip, 0, op, parm);\
}

#define BITUNBLK128V32_4(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*8+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*8+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*8+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*8+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*8+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*8+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24),mv);                                                                                                                 VSTO(op,i*8+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);                                                                                                                     VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACK128V32_4(ip,  op, parm) {\
  BITUNBLK128V32_4(ip, 0, op, parm);\
  BITUNBLK128V32_4(ip, 1, op, parm);\
  BITUNBLK128V32_4(ip, 2, op, parm);\
  BITUNBLK128V32_4(ip, 3, op, parm);\
}

#define BITUNBLK128V32_5(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 25),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 26),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+25,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_5(ip,  op, parm) {\
  BITUNBLK128V32_5(ip, 0, op, parm);\
}

#define BITUNBLK128V32_6(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*16+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*16+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*16+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24),mv);                                                                                                                 VSTO(op,i*16+ 4,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*16+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*16+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22),mv);                                                                                                                 VSTO(op,i*16+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*16+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*16+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*16+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*16+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_6(ip,  op, parm) {\
  BITUNBLK128V32_6(ip, 0, op, parm);\
  BITUNBLK128V32_6(ip, 1, op, parm);\
}

#define BITUNBLK128V32_7(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 24),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 23),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_7(ip,  op, parm) {\
  BITUNBLK128V32_7(ip, 0, op, parm);\
}

#define BITUNBLK128V32_8(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*4+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*4+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*4+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);                                                                                                                     VSTO(op,i*4+ 3,ov,parm); ;\
}

#define BITUNPACK128V32_8(ip,  op, parm) {\
  BITUNBLK128V32_8(ip, 0, op, parm);\
  BITUNBLK128V32_8(ip, 1, op, parm);\
  BITUNBLK128V32_8(ip, 2, op, parm);\
  BITUNBLK128V32_8(ip, 3, op, parm);\
  BITUNBLK128V32_8(ip, 4, op, parm);\
  BITUNBLK128V32_8(ip, 5, op, parm);\
  BITUNBLK128V32_8(ip, 6, op, parm);\
  BITUNBLK128V32_8(ip, 7, op, parm);\
}

#define BITUNBLK128V32_9(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 22),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 21),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_9(ip,  op, parm) {\
  BITUNBLK128V32_9(ip, 0, op, parm);\
}

#define BITUNBLK128V32_10(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*16+ 1,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*16+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*16+ 4,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*16+ 5,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*16+ 8,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*16+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*16+11,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*16+13,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*16+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_10(ip,  op, parm) {\
  BITUNBLK128V32_10(ip, 0, op, parm);\
  BITUNBLK128V32_10(ip, 1, op, parm);\
}

#define BITUNBLK128V32_11(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 19),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 20),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+29,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 21);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_11(ip,  op, parm) {\
  BITUNBLK128V32_11(ip, 0, op, parm);\
}

#define BITUNBLK128V32_12(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*8+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*8+ 1,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*8+ 3,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*8+ 4,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*8+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);                                                                                                                     VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACK128V32_12(ip,  op, parm) {\
  BITUNBLK128V32_12(ip, 0, op, parm);\
  BITUNBLK128V32_12(ip, 1, op, parm);\
  BITUNBLK128V32_12(ip, 2, op, parm);\
  BITUNBLK128V32_12(ip, 3, op, parm);\
}

#define BITUNBLK128V32_13(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 17),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 18),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+29,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 19);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_13(ip,  op, parm) {\
  BITUNBLK128V32_13(ip, 0, op, parm);\
}

#define BITUNBLK128V32_14(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*16+ 1,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*16+ 3,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*16+ 5,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*16+ 7,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*16+ 8,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*16+10,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*16+12,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*16+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 18);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_14(ip,  op, parm) {\
  BITUNBLK128V32_14(ip, 0, op, parm);\
  BITUNBLK128V32_14(ip, 1, op, parm);\
}

#define BITUNBLK128V32_15(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 15),mv);                                                                                                                 VSTO(op,i*32+ 1,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+14,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+15,ov,parm); \
  ov = _mm_and_si128(_mm_srli_epi32(iv, 16),mv);                                                                                                                 VSTO(op,i*32+16,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+25,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+29,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+30,ov,parm); \
  ov =                _mm_srli_epi32(iv, 17);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_15(ip,  op, parm) {\
  BITUNBLK128V32_15(ip, 0, op, parm);\
}

#define BITUNBLK128V32_16(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*2+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 16);                                                                                                                     VSTO(op,i*2+ 1,ov,parm); ;\
}

#define BITUNPACK128V32_16(ip,  op, parm) {\
  BITUNBLK128V32_16(ip, 0, op, parm);\
  BITUNBLK128V32_16(ip, 1, op, parm);\
  BITUNBLK128V32_16(ip, 2, op, parm);\
  BITUNBLK128V32_16(ip, 3, op, parm);\
  BITUNBLK128V32_16(ip, 4, op, parm);\
  BITUNBLK128V32_16(ip, 5, op, parm);\
  BITUNBLK128V32_16(ip, 6, op, parm);\
  BITUNBLK128V32_16(ip, 7, op, parm);\
  BITUNBLK128V32_16(ip, 8, op, parm);\
  BITUNBLK128V32_16(ip, 9, op, parm);\
  BITUNBLK128V32_16(ip, 10, op, parm);\
  BITUNBLK128V32_16(ip, 11, op, parm);\
  BITUNBLK128V32_16(ip, 12, op, parm);\
  BITUNBLK128V32_16(ip, 13, op, parm);\
  BITUNBLK128V32_16(ip, 14, op, parm);\
  BITUNBLK128V32_16(ip, 15, op, parm);\
}

#define BITUNBLK128V32_17(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 14),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 13),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_17(ip,  op, parm) {\
  BITUNBLK128V32_17(ip, 0, op, parm);\
}

#define BITUNBLK128V32_18(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*16+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*16+ 4,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*16+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*16+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*16+11,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*16+13,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_18(ip,  op, parm) {\
  BITUNBLK128V32_18(ip, 0, op, parm);\
  BITUNBLK128V32_18(ip, 1, op, parm);\
}

#define BITUNBLK128V32_19(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 12),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 11),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_19(ip,  op, parm) {\
  BITUNBLK128V32_19(ip, 0, op, parm);\
}

#define BITUNBLK128V32_20(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*8+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*8+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*8+ 5,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);                                                                                                                     VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACK128V32_20(ip,  op, parm) {\
  BITUNBLK128V32_20(ip, 0, op, parm);\
  BITUNBLK128V32_20(ip, 1, op, parm);\
  BITUNBLK128V32_20(ip, 2, op, parm);\
  BITUNBLK128V32_20(ip, 3, op, parm);\
}

#define BITUNBLK128V32_21(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv, 10),mv);                                                                                                                 VSTO(op,i*32+ 2,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  9),mv);                                                                                                                 VSTO(op,i*32+ 5,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+ 8,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+10,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+11,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+19,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+20,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+22,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+23,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+25,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+26,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+29,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_21(ip,  op, parm) {\
  BITUNBLK128V32_21(ip, 0, op, parm);\
}

#define BITUNBLK128V32_22(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*16+ 3,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*16+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*16+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*16+12,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_22(ip,  op, parm) {\
  BITUNBLK128V32_22(ip, 0, op, parm);\
  BITUNBLK128V32_22(ip, 1, op, parm);\
}

#define BITUNBLK128V32_23(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+ 3,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+ 7,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+13,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+14,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  7),mv);                                                                                                                 VSTO(op,i*32+17,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+23,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  8),mv);                                                                                                                 VSTO(op,i*32+24,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+27,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+28,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_23(ip,  op, parm) {\
  BITUNBLK128V32_23(ip, 0, op, parm);\
}

#define BITUNBLK128V32_24(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*4+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*4+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*4+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);                                                                                                                     VSTO(op,i*4+ 3,ov,parm); ;\
}

#define BITUNPACK128V32_24(ip,  op, parm) {\
  BITUNBLK128V32_24(ip, 0, op, parm);\
  BITUNBLK128V32_24(ip, 1, op, parm);\
  BITUNBLK128V32_24(ip, 2, op, parm);\
  BITUNBLK128V32_24(ip, 3, op, parm);\
  BITUNBLK128V32_24(ip, 4, op, parm);\
  BITUNBLK128V32_24(ip, 5, op, parm);\
  BITUNBLK128V32_24(ip, 6, op, parm);\
  BITUNBLK128V32_24(ip, 7, op, parm);\
}

#define BITUNBLK128V32_25(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+ 4,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+ 9,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+12,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  5),mv);                                                                                                                 VSTO(op,i*32+13,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+17,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+18,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+21,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  6),mv);                                                                                                                 VSTO(op,i*32+22,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+26,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+27,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_25(ip,  op, parm) {\
  BITUNBLK128V32_25(ip, 0, op, parm);\
}

#define BITUNBLK128V32_26(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*16+ 5,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*16+10,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_26(ip,  op, parm) {\
  BITUNBLK128V32_26(ip, 0, op, parm);\
  BITUNBLK128V32_26(ip, 1, op, parm);\
}

#define BITUNBLK128V32_27(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+ 6,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+11,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  4),mv);                                                                                                                 VSTO(op,i*32+12,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+18,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+19,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  3),mv);                                                                                                                 VSTO(op,i*32+25,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_27(ip,  op, parm) {\
  BITUNBLK128V32_27(ip, 0, op, parm);\
}

#define BITUNBLK128V32_28(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*8+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);                                                                                                                     VSTO(op,i*8+ 7,ov,parm); ;\
}

#define BITUNPACK128V32_28(ip,  op, parm) {\
  BITUNBLK128V32_28(ip, 0, op, parm);\
  BITUNBLK128V32_28(ip, 1, op, parm);\
  BITUNBLK128V32_28(ip, 2, op, parm);\
  BITUNBLK128V32_28(ip, 3, op, parm);\
}

#define BITUNBLK128V32_29(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  2),mv);                                                                                                                 VSTO(op,i*32+10,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+20,ov,parm);\
  ov = _mm_and_si128(_mm_srli_epi32(iv,  1),mv);                                                                                                                 VSTO(op,i*32+21,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_29(ip,  op, parm) {\
  BITUNBLK128V32_29(ip, 0, op, parm);\
}

#define BITUNBLK128V32_30(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*16+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);                                                                                                                     VSTO(op,i*16+15,ov,parm); ;\
}

#define BITUNPACK128V32_30(ip,  op, parm) {\
  BITUNBLK128V32_30(ip, 0, op, parm);\
  BITUNBLK128V32_30(ip, 1, op, parm);\
}

#define BITUNBLK128V32_31(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*32+ 0,ov,parm); \
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);                                                                                                                     VSTO(op,i*32+31,ov,parm); ;\
}

#define BITUNPACK128V32_31(ip,  op, parm) {\
  BITUNBLK128V32_31(ip, 0, op, parm);\
}

#define BITUNBLK128V32_32(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov = _mm_and_si128(               iv     ,mv);                                                                                                                  VSTO(op,i*1+ 0,ov,parm); ;\
}

#define BITUNPACK128V32_32(ip,  op, parm) {\
  BITUNBLK128V32_32(ip, 0, op, parm);\
  BITUNBLK128V32_32(ip, 1, op, parm);\
  BITUNBLK128V32_32(ip, 2, op, parm);\
  BITUNBLK128V32_32(ip, 3, op, parm);\
  BITUNBLK128V32_32(ip, 4, op, parm);\
  BITUNBLK128V32_32(ip, 5, op, parm);\
  BITUNBLK128V32_32(ip, 6, op, parm);\
  BITUNBLK128V32_32(ip, 7, op, parm);\
  BITUNBLK128V32_32(ip, 8, op, parm);\
  BITUNBLK128V32_32(ip, 9, op, parm);\
  BITUNBLK128V32_32(ip, 10, op, parm);\
  BITUNBLK128V32_32(ip, 11, op, parm);\
  BITUNBLK128V32_32(ip, 12, op, parm);\
  BITUNBLK128V32_32(ip, 13, op, parm);\
  BITUNBLK128V32_32(ip, 14, op, parm);\
  BITUNBLK128V32_32(ip, 15, op, parm);\
  BITUNBLK128V32_32(ip, 16, op, parm);\
  BITUNBLK128V32_32(ip, 17, op, parm);\
  BITUNBLK128V32_32(ip, 18, op, parm);\
  BITUNBLK128V32_32(ip, 19, op, parm);\
  BITUNBLK128V32_32(ip, 20, op, parm);\
  BITUNBLK128V32_32(ip, 21, op, parm);\
  BITUNBLK128V32_32(ip, 22, op, parm);\
  BITUNBLK128V32_32(ip, 23, op, parm);\
  BITUNBLK128V32_32(ip, 24, op, parm);\
  BITUNBLK128V32_32(ip, 25, op, parm);\
  BITUNBLK128V32_32(ip, 26, op, parm);\
  BITUNBLK128V32_32(ip, 27, op, parm);\
  BITUNBLK128V32_32(ip, 28, op, parm);\
  BITUNBLK128V32_32(ip, 29, op, parm);\
  BITUNBLK128V32_32(ip, 30, op, parm);\
  BITUNBLK128V32_32(ip, 31, op, parm);\
}

#define BITUNBLK128V32_33(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_33(ip,  op, parm) {\
  BITUNBLK128V32_33(ip, 0, op, parm);\
}

#define BITUNBLK128V32_34(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_34(ip,  op, parm) {\
  BITUNBLK128V32_34(ip, 0, op, parm);\
  BITUNBLK128V32_34(ip, 1, op, parm);\
}

#define BITUNBLK128V32_35(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_35(ip,  op, parm) {\
  BITUNBLK128V32_35(ip, 0, op, parm);\
}

#define BITUNBLK128V32_36(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*8+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 7,ov,parm);;\
}

#define BITUNPACK128V32_36(ip,  op, parm) {\
  BITUNBLK128V32_36(ip, 0, op, parm);\
  BITUNBLK128V32_36(ip, 1, op, parm);\
  BITUNBLK128V32_36(ip, 2, op, parm);\
  BITUNBLK128V32_36(ip, 3, op, parm);\
}

#define BITUNBLK128V32_37(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_37(ip,  op, parm) {\
  BITUNBLK128V32_37(ip, 0, op, parm);\
}

#define BITUNBLK128V32_38(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_38(ip,  op, parm) {\
  BITUNBLK128V32_38(ip, 0, op, parm);\
  BITUNBLK128V32_38(ip, 1, op, parm);\
}

#define BITUNBLK128V32_39(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_39(ip,  op, parm) {\
  BITUNBLK128V32_39(ip, 0, op, parm);\
}

#define BITUNBLK128V32_40(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*4+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*4+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*4+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*4+ 3,ov,parm);;\
}

#define BITUNPACK128V32_40(ip,  op, parm) {\
  BITUNBLK128V32_40(ip, 0, op, parm);\
  BITUNBLK128V32_40(ip, 1, op, parm);\
  BITUNBLK128V32_40(ip, 2, op, parm);\
  BITUNBLK128V32_40(ip, 3, op, parm);\
  BITUNBLK128V32_40(ip, 4, op, parm);\
  BITUNBLK128V32_40(ip, 5, op, parm);\
  BITUNBLK128V32_40(ip, 6, op, parm);\
  BITUNBLK128V32_40(ip, 7, op, parm);\
}

#define BITUNBLK128V32_41(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_41(ip,  op, parm) {\
  BITUNBLK128V32_41(ip, 0, op, parm);\
}

#define BITUNBLK128V32_42(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_42(ip,  op, parm) {\
  BITUNBLK128V32_42(ip, 0, op, parm);\
  BITUNBLK128V32_42(ip, 1, op, parm);\
}

#define BITUNBLK128V32_43(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_43(ip,  op, parm) {\
  BITUNBLK128V32_43(ip, 0, op, parm);\
}

#define BITUNBLK128V32_44(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*8+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 7,ov,parm);;\
}

#define BITUNPACK128V32_44(ip,  op, parm) {\
  BITUNBLK128V32_44(ip, 0, op, parm);\
  BITUNBLK128V32_44(ip, 1, op, parm);\
  BITUNBLK128V32_44(ip, 2, op, parm);\
  BITUNBLK128V32_44(ip, 3, op, parm);\
}

#define BITUNBLK128V32_45(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_45(ip,  op, parm) {\
  BITUNBLK128V32_45(ip, 0, op, parm);\
}

#define BITUNBLK128V32_46(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_46(ip,  op, parm) {\
  BITUNBLK128V32_46(ip, 0, op, parm);\
  BITUNBLK128V32_46(ip, 1, op, parm);\
}

#define BITUNBLK128V32_47(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_47(ip,  op, parm) {\
  BITUNBLK128V32_47(ip, 0, op, parm);\
}

#define BITUNBLK128V32_48(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*2+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*2+ 1,ov,parm);;\
}

#define BITUNPACK128V32_48(ip,  op, parm) {\
  BITUNBLK128V32_48(ip, 0, op, parm);\
  BITUNBLK128V32_48(ip, 1, op, parm);\
  BITUNBLK128V32_48(ip, 2, op, parm);\
  BITUNBLK128V32_48(ip, 3, op, parm);\
  BITUNBLK128V32_48(ip, 4, op, parm);\
  BITUNBLK128V32_48(ip, 5, op, parm);\
  BITUNBLK128V32_48(ip, 6, op, parm);\
  BITUNBLK128V32_48(ip, 7, op, parm);\
  BITUNBLK128V32_48(ip, 8, op, parm);\
  BITUNBLK128V32_48(ip, 9, op, parm);\
  BITUNBLK128V32_48(ip, 10, op, parm);\
  BITUNBLK128V32_48(ip, 11, op, parm);\
  BITUNBLK128V32_48(ip, 12, op, parm);\
  BITUNBLK128V32_48(ip, 13, op, parm);\
  BITUNBLK128V32_48(ip, 14, op, parm);\
  BITUNBLK128V32_48(ip, 15, op, parm);\
}

#define BITUNBLK128V32_49(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_49(ip,  op, parm) {\
  BITUNBLK128V32_49(ip, 0, op, parm);\
}

#define BITUNBLK128V32_50(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_50(ip,  op, parm) {\
  BITUNBLK128V32_50(ip, 0, op, parm);\
  BITUNBLK128V32_50(ip, 1, op, parm);\
}

#define BITUNBLK128V32_51(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_51(ip,  op, parm) {\
  BITUNBLK128V32_51(ip, 0, op, parm);\
}

#define BITUNBLK128V32_52(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*8+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*8+ 7,ov,parm);;\
}

#define BITUNPACK128V32_52(ip,  op, parm) {\
  BITUNBLK128V32_52(ip, 0, op, parm);\
  BITUNBLK128V32_52(ip, 1, op, parm);\
  BITUNBLK128V32_52(ip, 2, op, parm);\
  BITUNBLK128V32_52(ip, 3, op, parm);\
}

#define BITUNBLK128V32_53(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_53(ip,  op, parm) {\
  BITUNBLK128V32_53(ip, 0, op, parm);\
}

#define BITUNBLK128V32_54(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_54(ip,  op, parm) {\
  BITUNBLK128V32_54(ip, 0, op, parm);\
  BITUNBLK128V32_54(ip, 1, op, parm);\
}

#define BITUNBLK128V32_55(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_55(ip,  op, parm) {\
  BITUNBLK128V32_55(ip, 0, op, parm);\
}

#define BITUNBLK128V32_56(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*4+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*4+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*4+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*4+ 3,ov,parm);;\
}

#define BITUNPACK128V32_56(ip,  op, parm) {\
  BITUNBLK128V32_56(ip, 0, op, parm);\
  BITUNBLK128V32_56(ip, 1, op, parm);\
  BITUNBLK128V32_56(ip, 2, op, parm);\
  BITUNBLK128V32_56(ip, 3, op, parm);\
  BITUNBLK128V32_56(ip, 4, op, parm);\
  BITUNBLK128V32_56(ip, 5, op, parm);\
  BITUNBLK128V32_56(ip, 6, op, parm);\
  BITUNBLK128V32_56(ip, 7, op, parm);\
}

#define BITUNBLK128V32_57(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_57(ip,  op, parm) {\
  BITUNBLK128V32_57(ip, 0, op, parm);\
}

#define BITUNBLK128V32_58(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_58(ip,  op, parm) {\
  BITUNBLK128V32_58(ip, 0, op, parm);\
  BITUNBLK128V32_58(ip, 1, op, parm);\
}

#define BITUNBLK128V32_59(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_59(ip,  op, parm) {\
  BITUNBLK128V32_59(ip, 0, op, parm);\
}

#define BITUNBLK128V32_60(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*8+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*8+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*8+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*8+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*8+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*8+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*8+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*8+ 7,ov,parm);;\
}

#define BITUNPACK128V32_60(ip,  op, parm) {\
  BITUNBLK128V32_60(ip, 0, op, parm);\
  BITUNBLK128V32_60(ip, 1, op, parm);\
  BITUNBLK128V32_60(ip, 2, op, parm);\
  BITUNBLK128V32_60(ip, 3, op, parm);\
}

#define BITUNBLK128V32_61(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_61(ip,  op, parm) {\
  BITUNBLK128V32_61(ip, 0, op, parm);\
}

#define BITUNBLK128V32_62(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*16+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*16+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*16+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*16+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*16+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*16+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*16+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*16+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*16+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*16+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*16+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*16+11,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*16+12,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*16+13,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*16+14,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*16+15,ov,parm);;\
}

#define BITUNPACK128V32_62(ip,  op, parm) {\
  BITUNBLK128V32_62(ip, 0, op, parm);\
  BITUNBLK128V32_62(ip, 1, op, parm);\
}

#define BITUNBLK128V32_63(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*32+ 0,ov,parm);\
  ov =                _mm_srli_epi32(iv, 31);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  1), mv)); VSTO(op,i*32+ 1,ov,parm);\
  ov =                _mm_srli_epi32(iv, 30);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  2), mv)); VSTO(op,i*32+ 2,ov,parm);\
  ov =                _mm_srli_epi32(iv, 29);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  3), mv)); VSTO(op,i*32+ 3,ov,parm);\
  ov =                _mm_srli_epi32(iv, 28);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  4), mv)); VSTO(op,i*32+ 4,ov,parm);\
  ov =                _mm_srli_epi32(iv, 27);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  5), mv)); VSTO(op,i*32+ 5,ov,parm);\
  ov =                _mm_srli_epi32(iv, 26);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  6), mv)); VSTO(op,i*32+ 6,ov,parm);\
  ov =                _mm_srli_epi32(iv, 25);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  7), mv)); VSTO(op,i*32+ 7,ov,parm);\
  ov =                _mm_srli_epi32(iv, 24);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  8), mv)); VSTO(op,i*32+ 8,ov,parm);\
  ov =                _mm_srli_epi32(iv, 23);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv,  9), mv)); VSTO(op,i*32+ 9,ov,parm);\
  ov =                _mm_srli_epi32(iv, 22);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 10), mv)); VSTO(op,i*32+10,ov,parm);\
  ov =                _mm_srli_epi32(iv, 21);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 11), mv)); VSTO(op,i*32+11,ov,parm);\
  ov =                _mm_srli_epi32(iv, 20);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 12), mv)); VSTO(op,i*32+12,ov,parm);\
  ov =                _mm_srli_epi32(iv, 19);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 13), mv)); VSTO(op,i*32+13,ov,parm);\
  ov =                _mm_srli_epi32(iv, 18);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 14), mv)); VSTO(op,i*32+14,ov,parm);\
  ov =                _mm_srli_epi32(iv, 17);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 15), mv)); VSTO(op,i*32+15,ov,parm);\
  ov =                _mm_srli_epi32(iv, 16);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 16), mv)); VSTO(op,i*32+16,ov,parm);\
  ov =                _mm_srli_epi32(iv, 15);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 17), mv)); VSTO(op,i*32+17,ov,parm);\
  ov =                _mm_srli_epi32(iv, 14);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 18), mv)); VSTO(op,i*32+18,ov,parm);\
  ov =                _mm_srli_epi32(iv, 13);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 19), mv)); VSTO(op,i*32+19,ov,parm);\
  ov =                _mm_srli_epi32(iv, 12);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 20), mv)); VSTO(op,i*32+20,ov,parm);\
  ov =                _mm_srli_epi32(iv, 11);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 21), mv)); VSTO(op,i*32+21,ov,parm);\
  ov =                _mm_srli_epi32(iv, 10);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 22), mv)); VSTO(op,i*32+22,ov,parm);\
  ov =                _mm_srli_epi32(iv,  9);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 23), mv)); VSTO(op,i*32+23,ov,parm);\
  ov =                _mm_srli_epi32(iv,  8);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 24), mv)); VSTO(op,i*32+24,ov,parm);\
  ov =                _mm_srli_epi32(iv,  7);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 25), mv)); VSTO(op,i*32+25,ov,parm);\
  ov =                _mm_srli_epi32(iv,  6);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 26), mv)); VSTO(op,i*32+26,ov,parm);\
  ov =                _mm_srli_epi32(iv,  5);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 27), mv)); VSTO(op,i*32+27,ov,parm);\
  ov =                _mm_srli_epi32(iv,  4);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 28), mv)); VSTO(op,i*32+28,ov,parm);\
  ov =                _mm_srli_epi32(iv,  3);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 29), mv)); VSTO(op,i*32+29,ov,parm);\
  ov =                _mm_srli_epi32(iv,  2);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 30), mv)); VSTO(op,i*32+30,ov,parm);\
  ov =                _mm_srli_epi32(iv,  1);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 31), mv)); VSTO(op,i*32+31,ov,parm);;\
}

#define BITUNPACK128V32_63(ip,  op, parm) {\
  BITUNBLK128V32_63(ip, 0, op, parm);\
}

#define BITUNBLK128V32_64(ip, i, op, parm) { 								__m128i ov,iv = _mm_loadu_si128((__m128i *)ip++);\
  ov =                _mm_srli_epi32(iv,  0);      iv = _mm_loadu_si128((__m128i *)ip++); ov = _mm_or_si128(ov, _mm_and_si128(_mm_slli_epi32(iv, 32), mv)); VSTO(op,i*1+ 0,ov,parm);;\
}

#define BITUNPACK128V32_64(ip,  op, parm) {\
  BITUNBLK128V32_64(ip, 0, op, parm);\
  BITUNBLK128V32_64(ip, 1, op, parm);\
  BITUNBLK128V32_64(ip, 2, op, parm);\
  BITUNBLK128V32_64(ip, 3, op, parm);\
  BITUNBLK128V32_64(ip, 4, op, parm);\
  BITUNBLK128V32_64(ip, 5, op, parm);\
  BITUNBLK128V32_64(ip, 6, op, parm);\
  BITUNBLK128V32_64(ip, 7, op, parm);\
  BITUNBLK128V32_64(ip, 8, op, parm);\
  BITUNBLK128V32_64(ip, 9, op, parm);\
  BITUNBLK128V32_64(ip, 10, op, parm);\
  BITUNBLK128V32_64(ip, 11, op, parm);\
  BITUNBLK128V32_64(ip, 12, op, parm);\
  BITUNBLK128V32_64(ip, 13, op, parm);\
  BITUNBLK128V32_64(ip, 14, op, parm);\
  BITUNBLK128V32_64(ip, 15, op, parm);\
  BITUNBLK128V32_64(ip, 16, op, parm);\
  BITUNBLK128V32_64(ip, 17, op, parm);\
  BITUNBLK128V32_64(ip, 18, op, parm);\
  BITUNBLK128V32_64(ip, 19, op, parm);\
  BITUNBLK128V32_64(ip, 20, op, parm);\
  BITUNBLK128V32_64(ip, 21, op, parm);\
  BITUNBLK128V32_64(ip, 22, op, parm);\
  BITUNBLK128V32_64(ip, 23, op, parm);\
  BITUNBLK128V32_64(ip, 24, op, parm);\
  BITUNBLK128V32_64(ip, 25, op, parm);\
  BITUNBLK128V32_64(ip, 26, op, parm);\
  BITUNBLK128V32_64(ip, 27, op, parm);\
  BITUNBLK128V32_64(ip, 28, op, parm);\
  BITUNBLK128V32_64(ip, 29, op, parm);\
  BITUNBLK128V32_64(ip, 30, op, parm);\
  BITUNBLK128V32_64(ip, 31, op, parm);\
}

#define BITUNPACK128V32(__ip, __nbits, __op, _parm_) { __m128i mv,*_ov=(__m128i *)__op,*_iv=(__m128i *)__ip; \
  switch(__nbits&0x3f) {\
    case  0: BITUNPACK0(_parm_);               BITUNPACK128V32_0( _iv, _ov, _parm_); break;\
    case  1: mv = _mm_set1_epi32((1u<< 1)-1);  BITUNPACK128V32_1( _iv, _ov, _parm_); break;\
    case  2: mv = _mm_set1_epi32((1u<< 2)-1);  BITUNPACK128V32_2( _iv, _ov, _parm_); break;\
    case  3: mv = _mm_set1_epi32((1u<< 3)-1);  BITUNPACK128V32_3( _iv, _ov, _parm_); break;\
    case  4: mv = _mm_set1_epi32((1u<< 4)-1);  BITUNPACK128V32_4( _iv, _ov, _parm_); break;\
    case  5: mv = _mm_set1_epi32((1u<< 5)-1);  BITUNPACK128V32_5( _iv, _ov, _parm_); break;\
    case  6: mv = _mm_set1_epi32((1u<< 6)-1);  BITUNPACK128V32_6( _iv, _ov, _parm_); break;\
    case  7: mv = _mm_set1_epi32((1u<< 7)-1);  BITUNPACK128V32_7( _iv, _ov, _parm_); break;\
    case  8: mv = _mm_set1_epi32((1u<< 8)-1);  BITUNPACK128V32_8( _iv, _ov, _parm_); break;\
    case  9: mv = _mm_set1_epi32((1u<< 9)-1);  BITUNPACK128V32_9( _iv, _ov, _parm_); break;\
    case 10: mv = _mm_set1_epi32((1u<<10)-1);  BITUNPACK128V32_10(_iv, _ov, _parm_); break;\
    case 11: mv = _mm_set1_epi32((1u<<11)-1);  BITUNPACK128V32_11(_iv, _ov, _parm_); break;\
    case 12: mv = _mm_set1_epi32((1u<<12)-1);  BITUNPACK128V32_12(_iv, _ov, _parm_); break;\
    case 13: mv = _mm_set1_epi32((1u<<13)-1);  BITUNPACK128V32_13(_iv, _ov, _parm_); break;\
    case 14: mv = _mm_set1_epi32((1u<<14)-1);  BITUNPACK128V32_14(_iv, _ov, _parm_); break;\
    case 15: mv = _mm_set1_epi32((1u<<15)-1);  BITUNPACK128V32_15(_iv, _ov, _parm_); break;\
    case 16: mv = _mm_set1_epi32((1u<<16)-1);  BITUNPACK128V32_16(_iv, _ov, _parm_); break;\
    case 17: mv = _mm_set1_epi32((1u<<17)-1);  BITUNPACK128V32_17(_iv, _ov, _parm_); break;\
    case 18: mv = _mm_set1_epi32((1u<<18)-1);  BITUNPACK128V32_18(_iv, _ov, _parm_); break;\
    case 19: mv = _mm_set1_epi32((1u<<19)-1);  BITUNPACK128V32_19(_iv, _ov, _parm_); break;\
    case 20: mv = _mm_set1_epi32((1u<<20)-1);  BITUNPACK128V32_20(_iv, _ov, _parm_); break;\
    case 21: mv = _mm_set1_epi32((1u<<21)-1);  BITUNPACK128V32_21(_iv, _ov, _parm_); break;\
    case 22: mv = _mm_set1_epi32((1u<<22)-1);  BITUNPACK128V32_22(_iv, _ov, _parm_); break;\
    case 23: mv = _mm_set1_epi32((1u<<23)-1);  BITUNPACK128V32_23(_iv, _ov, _parm_); break;\
    case 24: mv = _mm_set1_epi32((1u<<24)-1);  BITUNPACK128V32_24(_iv, _ov, _parm_); break;\
    case 25: mv = _mm_set1_epi32((1u<<25)-1);  BITUNPACK128V32_25(_iv, _ov, _parm_); break;\
    case 26: mv = _mm_set1_epi32((1u<<26)-1);  BITUNPACK128V32_26(_iv, _ov, _parm_); break;\
    case 27: mv = _mm_set1_epi32((1u<<27)-1);  BITUNPACK128V32_27(_iv, _ov, _parm_); break;\
    case 28: mv = _mm_set1_epi32((1u<<28)-1);  BITUNPACK128V32_28(_iv, _ov, _parm_); break;\
    case 29: mv = _mm_set1_epi32((1u<<29)-1);  BITUNPACK128V32_29(_iv, _ov, _parm_); break;\
    case 30: mv = _mm_set1_epi32((1u<<30)-1);  BITUNPACK128V32_30(_iv, _ov, _parm_); break;\
    case 31: mv = _mm_set1_epi32((1u<<31)-1);  BITUNPACK128V32_31(_iv, _ov, _parm_); break;\
    case 32: mv = _mm_set1_epi32((1ull<<32)-1);BITUNPACK128V32_32(_iv, _ov, _parm_); break;\
	case 33 ... 63: break;\
  }\
}    

