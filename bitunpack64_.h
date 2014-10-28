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

    bitunpack64_.c - "Integer Compression" binary packing 
**/

#define BITUNBLK32_0(ip, i, op, parm) { \
  DST(op,i*0+ 0, 0, parm);\
  DST(op,i*0+ 1, 0, parm);\
  DST(op,i*0+ 2, 0, parm);\
  DST(op,i*0+ 3, 0, parm);\
  DST(op,i*0+ 4, 0, parm);\
  DST(op,i*0+ 5, 0, parm);\
  DST(op,i*0+ 6, 0, parm);\
  DST(op,i*0+ 7, 0, parm);\
  DST(op,i*0+ 8, 0, parm);\
  DST(op,i*0+ 9, 0, parm);\
  DST(op,i*0+10, 0, parm);\
  DST(op,i*0+11, 0, parm);\
  DST(op,i*0+12, 0, parm);\
  DST(op,i*0+13, 0, parm);\
  DST(op,i*0+14, 0, parm);\
  DST(op,i*0+15, 0, parm);\
  DST(op,i*0+16, 0, parm);\
  DST(op,i*0+17, 0, parm);\
  DST(op,i*0+18, 0, parm);\
  DST(op,i*0+19, 0, parm);\
  DST(op,i*0+20, 0, parm);\
  DST(op,i*0+21, 0, parm);\
  DST(op,i*0+22, 0, parm);\
  DST(op,i*0+23, 0, parm);\
  DST(op,i*0+24, 0, parm);\
  DST(op,i*0+25, 0, parm);\
  DST(op,i*0+26, 0, parm);\
  DST(op,i*0+27, 0, parm);\
  DST(op,i*0+28, 0, parm);\
  DST(op,i*0+29, 0, parm);\
  DST(op,i*0+30, 0, parm);\
  DST(op,i*0+31, 0, parm);;\
}

#define BITUNPACK64_0(ip,  op, parm) { \
  BITUNBLK32_0(ip, 0, op, parm);  DSTI(op);\
}

#define BITUNBLK32_1(ip, i, op, parm) {   register uint32_t w0 = *(uint32_t *)(ip+(i*1+0)*4/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x1, parm);\
  DST(op,i*32+ 1, (w0 >>  1) & 0x1, parm);\
  DST(op,i*32+ 2, (w0 >>  2) & 0x1, parm);\
  DST(op,i*32+ 3, (w0 >>  3) & 0x1, parm);\
  DST(op,i*32+ 4, (w0 >>  4) & 0x1, parm);\
  DST(op,i*32+ 5, (w0 >>  5) & 0x1, parm);\
  DST(op,i*32+ 6, (w0 >>  6) & 0x1, parm);\
  DST(op,i*32+ 7, (w0 >>  7) & 0x1, parm);\
  DST(op,i*32+ 8, (w0 >>  8) & 0x1, parm);\
  DST(op,i*32+ 9, (w0 >>  9) & 0x1, parm);\
  DST(op,i*32+10, (w0 >> 10) & 0x1, parm);\
  DST(op,i*32+11, (w0 >> 11) & 0x1, parm);\
  DST(op,i*32+12, (w0 >> 12) & 0x1, parm);\
  DST(op,i*32+13, (w0 >> 13) & 0x1, parm);\
  DST(op,i*32+14, (w0 >> 14) & 0x1, parm);\
  DST(op,i*32+15, (w0 >> 15) & 0x1, parm);\
  DST(op,i*32+16, (w0 >> 16) & 0x1, parm);\
  DST(op,i*32+17, (w0 >> 17) & 0x1, parm);\
  DST(op,i*32+18, (w0 >> 18) & 0x1, parm);\
  DST(op,i*32+19, (w0 >> 19) & 0x1, parm);\
  DST(op,i*32+20, (w0 >> 20) & 0x1, parm);\
  DST(op,i*32+21, (w0 >> 21) & 0x1, parm);\
  DST(op,i*32+22, (w0 >> 22) & 0x1, parm);\
  DST(op,i*32+23, (w0 >> 23) & 0x1, parm);\
  DST(op,i*32+24, (w0 >> 24) & 0x1, parm);\
  DST(op,i*32+25, (w0 >> 25) & 0x1, parm);\
  DST(op,i*32+26, (w0 >> 26) & 0x1, parm);\
  DST(op,i*32+27, (w0 >> 27) & 0x1, parm);\
  DST(op,i*32+28, (w0 >> 28) & 0x1, parm);\
  DST(op,i*32+29, (w0 >> 29) & 0x1, parm);\
  DST(op,i*32+30, (w0 >> 30) & 0x1, parm);\
  DST(op,i*32+31, (w0 >> 31) , parm);;\
}

#define BITUNPACK64_1(ip,  op, parm) { \
  BITUNBLK32_1(ip, 0, op, parm);  DSTI(op); ip += 1*4/sizeof(ip[0]);\
}

#define BITUNBLK64_2(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*1+0)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3, parm);\
  DST(op,i*32+ 1, (w0 >>  2) & 0x3, parm);\
  DST(op,i*32+ 2, (w0 >>  4) & 0x3, parm);\
  DST(op,i*32+ 3, (w0 >>  6) & 0x3, parm);\
  DST(op,i*32+ 4, (w0 >>  8) & 0x3, parm);\
  DST(op,i*32+ 5, (w0 >> 10) & 0x3, parm);\
  DST(op,i*32+ 6, (w0 >> 12) & 0x3, parm);\
  DST(op,i*32+ 7, (w0 >> 14) & 0x3, parm);\
  DST(op,i*32+ 8, (w0 >> 16) & 0x3, parm);\
  DST(op,i*32+ 9, (w0 >> 18) & 0x3, parm);\
  DST(op,i*32+10, (w0 >> 20) & 0x3, parm);\
  DST(op,i*32+11, (w0 >> 22) & 0x3, parm);\
  DST(op,i*32+12, (w0 >> 24) & 0x3, parm);\
  DST(op,i*32+13, (w0 >> 26) & 0x3, parm);\
  DST(op,i*32+14, (w0 >> 28) & 0x3, parm);\
  DST(op,i*32+15, (w0 >> 30) & 0x3, parm);\
  DST(op,i*32+16, (w0 >> 32) & 0x3, parm);\
  DST(op,i*32+17, (w0 >> 34) & 0x3, parm);\
  DST(op,i*32+18, (w0 >> 36) & 0x3, parm);\
  DST(op,i*32+19, (w0 >> 38) & 0x3, parm);\
  DST(op,i*32+20, (w0 >> 40) & 0x3, parm);\
  DST(op,i*32+21, (w0 >> 42) & 0x3, parm);\
  DST(op,i*32+22, (w0 >> 44) & 0x3, parm);\
  DST(op,i*32+23, (w0 >> 46) & 0x3, parm);\
  DST(op,i*32+24, (w0 >> 48) & 0x3, parm);\
  DST(op,i*32+25, (w0 >> 50) & 0x3, parm);\
  DST(op,i*32+26, (w0 >> 52) & 0x3, parm);\
  DST(op,i*32+27, (w0 >> 54) & 0x3, parm);\
  DST(op,i*32+28, (w0 >> 56) & 0x3, parm);\
  DST(op,i*32+29, (w0 >> 58) & 0x3, parm);\
  DST(op,i*32+30, (w0 >> 60) & 0x3, parm);\
  DST(op,i*32+31, (w0 >> 62) , parm);;\
}

#define BITUNPACK64_2(ip,  op, parm) { \
  BITUNBLK64_2(ip, 0, op, parm);  DSTI(op); ip += 2*4/sizeof(ip[0]);\
}

#define BITUNBLK64_3(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*3+0)*8/sizeof(ip[0]));register uint32_t w1 = *(uint32_t *)(ip+(i*3+1)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7, parm);\
  DST(op,i*64+ 1, (w0 >>  3) & 0x7, parm);\
  DST(op,i*64+ 2, (w0 >>  6) & 0x7, parm);\
  DST(op,i*64+ 3, (w0 >>  9) & 0x7, parm);\
  DST(op,i*64+ 4, (w0 >> 12) & 0x7, parm);\
  DST(op,i*64+ 5, (w0 >> 15) & 0x7, parm);\
  DST(op,i*64+ 6, (w0 >> 18) & 0x7, parm);\
  DST(op,i*64+ 7, (w0 >> 21) & 0x7, parm);\
  DST(op,i*64+ 8, (w0 >> 24) & 0x7, parm);\
  DST(op,i*64+ 9, (w0 >> 27) & 0x7, parm);\
  DST(op,i*64+10, (w0 >> 30) & 0x7, parm);\
  DST(op,i*64+11, (w0 >> 33) & 0x7, parm);\
  DST(op,i*64+12, (w0 >> 36) & 0x7, parm);\
  DST(op,i*64+13, (w0 >> 39) & 0x7, parm);\
  DST(op,i*64+14, (w0 >> 42) & 0x7, parm);\
  DST(op,i*64+15, (w0 >> 45) & 0x7, parm);\
  DST(op,i*64+16, (w0 >> 48) & 0x7, parm);\
  DST(op,i*64+17, (w0 >> 51) & 0x7, parm);\
  DST(op,i*64+18, (w0 >> 54) & 0x7, parm);\
  DST(op,i*64+19, (w0 >> 57) & 0x7, parm);\
  DST(op,i*64+20, (w0 >> 60) & 0x7, parm);  \
\
  DST(op,i*64+21, (w0 >> 63) | (w1 <<  1) & 0x7, parm);\
  DST(op,i*64+22, (w1 >>  2) & 0x7, parm);\
  DST(op,i*64+23, (w1 >>  5) & 0x7, parm);\
  DST(op,i*64+24, (w1 >>  8) & 0x7, parm);\
  DST(op,i*64+25, (w1 >> 11) & 0x7, parm);\
  DST(op,i*64+26, (w1 >> 14) & 0x7, parm);\
  DST(op,i*64+27, (w1 >> 17) & 0x7, parm);\
  DST(op,i*64+28, (w1 >> 20) & 0x7, parm);\
  DST(op,i*64+29, (w1 >> 23) & 0x7, parm);\
  DST(op,i*64+30, (w1 >> 26) & 0x7, parm);\
  DST(op,i*64+31, (w1 >> 29) & 0x7, parm);;\
}

#define BITUNPACK64_3(ip,  op, parm) { \
  BITUNBLK64_3(ip, 0, op, parm);  DSTI(op); ip += 3*4/sizeof(ip[0]);\
}

#define BITUNBLK64_4(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip/*+(i*1+0)*8/sizeof(ip[0])*/);ip += 8/sizeof(ip[0]);\
  DST(op,i*16+ 0, (w0 ) & 0xf, parm);\
  DST(op,i*16+ 1, (w0 >>  4) & 0xf, parm);\
  DST(op,i*16+ 2, (w0 >>  8) & 0xf, parm);\
  DST(op,i*16+ 3, (w0 >> 12) & 0xf, parm);\
  DST(op,i*16+ 4, (w0 >> 16) & 0xf, parm);\
  DST(op,i*16+ 5, (w0 >> 20) & 0xf, parm);\
  DST(op,i*16+ 6, (w0 >> 24) & 0xf, parm);\
  DST(op,i*16+ 7, (w0 >> 28) & 0xf, parm);\
  DST(op,i*16+ 8, (w0 >> 32) & 0xf, parm);\
  DST(op,i*16+ 9, (w0 >> 36) & 0xf, parm);\
  DST(op,i*16+10, (w0 >> 40) & 0xf, parm);\
  DST(op,i*16+11, (w0 >> 44) & 0xf, parm);\
  DST(op,i*16+12, (w0 >> 48) & 0xf, parm);\
  DST(op,i*16+13, (w0 >> 52) & 0xf, parm);\
  DST(op,i*16+14, (w0 >> 56) & 0xf, parm);\
  DST(op,i*16+15, (w0 >> 60), parm);;\
}

#define BITUNPACK64_4(ip,  op, parm) { \
  BITUNBLK64_4(ip, 0, op, parm);\
  BITUNBLK64_4(ip, 1, op, parm);  DSTI(op); /*ip += 4*4/sizeof(ip[0]);*/\
}

#define BITUNBLK64_5(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*5+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x1f, parm);\
  DST(op,i*64+ 1, (w0 >>  5) & 0x1f, parm);\
  DST(op,i*64+ 2, (w0 >> 10) & 0x1f, parm);\
  DST(op,i*64+ 3, (w0 >> 15) & 0x1f, parm);\
  DST(op,i*64+ 4, (w0 >> 20) & 0x1f, parm);\
  DST(op,i*64+ 5, (w0 >> 25) & 0x1f, parm);\
  DST(op,i*64+ 6, (w0 >> 30) & 0x1f, parm);\
  DST(op,i*64+ 7, (w0 >> 35) & 0x1f, parm);\
  DST(op,i*64+ 8, (w0 >> 40) & 0x1f, parm);\
  DST(op,i*64+ 9, (w0 >> 45) & 0x1f, parm);\
  DST(op,i*64+10, (w0 >> 50) & 0x1f, parm);\
  DST(op,i*64+11, (w0 >> 55) & 0x1f, parm); register uint64_t w1 = *(uint64_t *)(ip+(i*5+1)*8/sizeof(ip[0])); \
\
  DST(op,i*64+12, (w0 >> 60) | (w1 <<  4) & 0x1f, parm);\
  DST(op,i*64+13, (w1 >>  1) & 0x1f, parm);\
  DST(op,i*64+14, (w1 >>  6) & 0x1f, parm);\
  DST(op,i*64+15, (w1 >> 11) & 0x1f, parm);\
  DST(op,i*64+16, (w1 >> 16) & 0x1f, parm);\
  DST(op,i*64+17, (w1 >> 21) & 0x1f, parm);\
  DST(op,i*64+18, (w1 >> 26) & 0x1f, parm);\
  DST(op,i*64+19, (w1 >> 31) & 0x1f, parm);\
  DST(op,i*64+20, (w1 >> 36) & 0x1f, parm);\
  DST(op,i*64+21, (w1 >> 41) & 0x1f, parm);\
  DST(op,i*64+22, (w1 >> 46) & 0x1f, parm);\
  DST(op,i*64+23, (w1 >> 51) & 0x1f, parm);\
  DST(op,i*64+24, (w1 >> 56) & 0x1f, parm);  register uint32_t w2 = *(uint32_t *)(ip+(i*5+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+25, (w1 >> 61) | (w2 <<  3) & 0x1f, parm);\
  DST(op,i*64+26, (w2 >>  2) & 0x1f, parm);\
  DST(op,i*64+27, (w2 >>  7) & 0x1f, parm);\
  DST(op,i*64+28, (w2 >> 12) & 0x1f, parm);\
  DST(op,i*64+29, (w2 >> 17) & 0x1f, parm);\
  DST(op,i*64+30, (w2 >> 22) & 0x1f, parm);\
  DST(op,i*64+31, (w2 >> 27) & 0x1f, parm);;\
}

#define BITUNPACK64_5(ip,  op, parm) { \
  BITUNBLK64_5(ip, 0, op, parm);  DSTI(op); ip += 5*4/sizeof(ip[0]);\
}

#define BITUNBLK64_6(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*3+0)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3f, parm);\
  DST(op,i*32+ 1, (w0 >>  6) & 0x3f, parm);\
  DST(op,i*32+ 2, (w0 >> 12) & 0x3f, parm);\
  DST(op,i*32+ 3, (w0 >> 18) & 0x3f, parm);\
  DST(op,i*32+ 4, (w0 >> 24) & 0x3f, parm);\
  DST(op,i*32+ 5, (w0 >> 30) & 0x3f, parm);\
  DST(op,i*32+ 6, (w0 >> 36) & 0x3f, parm);\
  DST(op,i*32+ 7, (w0 >> 42) & 0x3f, parm);\
  DST(op,i*32+ 8, (w0 >> 48) & 0x3f, parm);\
  DST(op,i*32+ 9, (w0 >> 54) & 0x3f, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*3+1)*8/sizeof(ip[0]));\
\
  DST(op,i*32+10, (w0 >> 60) | (w1 <<  4) & 0x3f, parm);\
  DST(op,i*32+11, (w1 >>  2) & 0x3f, parm);\
  DST(op,i*32+12, (w1 >>  8) & 0x3f, parm);\
  DST(op,i*32+13, (w1 >> 14) & 0x3f, parm);\
  DST(op,i*32+14, (w1 >> 20) & 0x3f, parm);\
  DST(op,i*32+15, (w1 >> 26) & 0x3f, parm);\
  DST(op,i*32+16, (w1 >> 32) & 0x3f, parm);\
  DST(op,i*32+17, (w1 >> 38) & 0x3f, parm);\
  DST(op,i*32+18, (w1 >> 44) & 0x3f, parm);\
  DST(op,i*32+19, (w1 >> 50) & 0x3f, parm);\
  DST(op,i*32+20, (w1 >> 56) & 0x3f, parm); register uint64_t w2 = *(uint64_t *)(ip+(i*3+2)*8/sizeof(ip[0]));\
\
  DST(op,i*32+21, (w1 >> 62) | (w2 <<  2) & 0x3f, parm);\
  DST(op,i*32+22, (w2 >>  4) & 0x3f, parm);\
  DST(op,i*32+23, (w2 >> 10) & 0x3f, parm);\
  DST(op,i*32+24, (w2 >> 16) & 0x3f, parm);\
  DST(op,i*32+25, (w2 >> 22) & 0x3f, parm);\
  DST(op,i*32+26, (w2 >> 28) & 0x3f, parm);\
  DST(op,i*32+27, (w2 >> 34) & 0x3f, parm);\
  DST(op,i*32+28, (w2 >> 40) & 0x3f, parm);\
  DST(op,i*32+29, (w2 >> 46) & 0x3f, parm);\
  DST(op,i*32+30, (w2 >> 52) & 0x3f, parm);\
  DST(op,i*32+31, (w2 >> 58) , parm);;\
}

#define BITUNPACK64_6(ip,  op, parm) { \
  BITUNBLK64_6(ip, 0, op, parm);  DSTI(op); ip += 6*4/sizeof(ip[0]);\
}

#define BITUNBLK64_7(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*7+0)*8/sizeof(ip[0]));register uint64_t w1 = *(uint64_t *)(ip+(i*7+1)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7f, parm);\
  DST(op,i*64+ 1, (w0 >>  7) & 0x7f, parm);\
  DST(op,i*64+ 2, (w0 >> 14) & 0x7f, parm);\
  DST(op,i*64+ 3, (w0 >> 21) & 0x7f, parm);\
  DST(op,i*64+ 4, (w0 >> 28) & 0x7f, parm);\
  DST(op,i*64+ 5, (w0 >> 35) & 0x7f, parm);\
  DST(op,i*64+ 6, (w0 >> 42) & 0x7f, parm);\
  DST(op,i*64+ 7, (w0 >> 49) & 0x7f, parm);\
  DST(op,i*64+ 8, (w0 >> 56) & 0x7f, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*7+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 9, (w0 >> 63) | (w1 <<  1) & 0x7f, parm);\
  DST(op,i*64+10, (w1 >>  6) & 0x7f, parm);\
  DST(op,i*64+11, (w1 >> 13) & 0x7f, parm);\
  DST(op,i*64+12, (w1 >> 20) & 0x7f, parm);\
  DST(op,i*64+13, (w1 >> 27) & 0x7f, parm);\
  DST(op,i*64+14, (w1 >> 34) & 0x7f, parm);\
  DST(op,i*64+15, (w1 >> 41) & 0x7f, parm);\
  DST(op,i*64+16, (w1 >> 48) & 0x7f, parm);\
  DST(op,i*64+17, (w1 >> 55) & 0x7f, parm);  register uint32_t w3 = *(uint32_t *)(ip+(i*7+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+18, (w1 >> 62) | (w2 <<  2) & 0x7f, parm);\
  DST(op,i*64+19, (w2 >>  5) & 0x7f, parm);\
  DST(op,i*64+20, (w2 >> 12) & 0x7f, parm);\
  DST(op,i*64+21, (w2 >> 19) & 0x7f, parm);\
  DST(op,i*64+22, (w2 >> 26) & 0x7f, parm);\
  DST(op,i*64+23, (w2 >> 33) & 0x7f, parm);\
  DST(op,i*64+24, (w2 >> 40) & 0x7f, parm);\
  DST(op,i*64+25, (w2 >> 47) & 0x7f, parm);\
  DST(op,i*64+26, (w2 >> 54) & 0x7f, parm);  \
\
  DST(op,i*64+27, (w2 >> 61) | (w3 <<  3) & 0x7f, parm);\
  DST(op,i*64+28, (w3 >>  4) & 0x7f, parm);\
  DST(op,i*64+29, (w3 >> 11) & 0x7f, parm);\
  DST(op,i*64+30, (w3 >> 18) & 0x7f, parm);\
  DST(op,i*64+31, (w3 >> 25) & 0x7f, parm);;\
}

#define BITUNPACK64_7(ip,  op, parm) { \
  BITUNBLK64_7(ip, 0, op, parm);  DSTI(op); ip += 7*4/sizeof(ip[0]);\
}

#define BITUNBLK64_8(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*1+0)*8/sizeof(ip[0]));\
  DST(op,i*8+ 0, (w0 ) & 0xff, parm);\
  DST(op,i*8+ 1, (w0 >>  8) & 0xff, parm);\
  DST(op,i*8+ 2, (w0 >> 16) & 0xff, parm);\
  DST(op,i*8+ 3, (w0 >> 24) & 0xff, parm);\
  DST(op,i*8+ 4, (w0 >> 32) & 0xff, parm);\
  DST(op,i*8+ 5, (w0 >> 40) & 0xff, parm);\
  DST(op,i*8+ 6, (w0 >> 48) & 0xff, parm);\
  DST(op,i*8+ 7, (w0 >> 56) , parm);;\
}

#define BITUNPACK64_8(ip,  op, parm) { \
  BITUNBLK64_8(ip, 0, op, parm);\
  BITUNBLK64_8(ip, 1, op, parm);\
  BITUNBLK64_8(ip, 2, op, parm);\
  BITUNBLK64_8(ip, 3, op, parm);  DSTI(op); ip += 8*4/sizeof(ip[0]);\
}

#define BITUNBLK64_9(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*9+0)*8/sizeof(ip[0]));register uint64_t w1 = *(uint64_t *)(ip+(i*9+1)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x1ff, parm);\
  DST(op,i*64+ 1, (w0 >>  9) & 0x1ff, parm);\
  DST(op,i*64+ 2, (w0 >> 18) & 0x1ff, parm);\
  DST(op,i*64+ 3, (w0 >> 27) & 0x1ff, parm);\
  DST(op,i*64+ 4, (w0 >> 36) & 0x1ff, parm);\
  DST(op,i*64+ 5, (w0 >> 45) & 0x1ff, parm);\
  DST(op,i*64+ 6, (w0 >> 54) & 0x1ff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*9+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 7, (w0 >> 63) | (w1 <<  1) & 0x1ff, parm);\
  DST(op,i*64+ 8, (w1 >>  8) & 0x1ff, parm);\
  DST(op,i*64+ 9, (w1 >> 17) & 0x1ff, parm);\
  DST(op,i*64+10, (w1 >> 26) & 0x1ff, parm);\
  DST(op,i*64+11, (w1 >> 35) & 0x1ff, parm);\
  DST(op,i*64+12, (w1 >> 44) & 0x1ff, parm);\
  DST(op,i*64+13, (w1 >> 53) & 0x1ff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*9+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+14, (w1 >> 62) | (w2 <<  2) & 0x1ff, parm);\
  DST(op,i*64+15, (w2 >>  7) & 0x1ff, parm);\
  DST(op,i*64+16, (w2 >> 16) & 0x1ff, parm);\
  DST(op,i*64+17, (w2 >> 25) & 0x1ff, parm);\
  DST(op,i*64+18, (w2 >> 34) & 0x1ff, parm);\
  DST(op,i*64+19, (w2 >> 43) & 0x1ff, parm);\
  DST(op,i*64+20, (w2 >> 52) & 0x1ff, parm);  register uint32_t w4 = *(uint32_t *)(ip+(i*9+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+21, (w2 >> 61) | (w3 <<  3) & 0x1ff, parm);\
  DST(op,i*64+22, (w3 >>  6) & 0x1ff, parm);\
  DST(op,i*64+23, (w3 >> 15) & 0x1ff, parm);\
  DST(op,i*64+24, (w3 >> 24) & 0x1ff, parm);\
  DST(op,i*64+25, (w3 >> 33) & 0x1ff, parm);\
  DST(op,i*64+26, (w3 >> 42) & 0x1ff, parm);\
  DST(op,i*64+27, (w3 >> 51) & 0x1ff, parm);  \
\
  DST(op,i*64+28, (w3 >> 60) | (w4 <<  4) & 0x1ff, parm);\
  DST(op,i*64+29, (w4 >>  5) & 0x1ff, parm);\
  DST(op,i*64+30, (w4 >> 14) & 0x1ff, parm);\
  DST(op,i*64+31, (w4 >> 23) & 0x1ff, parm);;\
}

#define BITUNPACK64_9(ip,  op, parm) { \
  BITUNBLK64_9(ip, 0, op, parm);  DSTI(op); ip += 9*4/sizeof(ip[0]);\
}

#define BITUNBLK64_10(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*5+0)*8/sizeof(ip[0]));register uint64_t w1 = *(uint64_t *)(ip+(i*5+1)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3ff, parm);\
  DST(op,i*32+ 1, (w0 >> 10) & 0x3ff, parm);\
  DST(op,i*32+ 2, (w0 >> 20) & 0x3ff, parm);\
  DST(op,i*32+ 3, (w0 >> 30) & 0x3ff, parm);\
  DST(op,i*32+ 4, (w0 >> 40) & 0x3ff, parm);\
  DST(op,i*32+ 5, (w0 >> 50) & 0x3ff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*5+2)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 6, (w0 >> 60) | (w1 <<  4) & 0x3ff, parm);\
  DST(op,i*32+ 7, (w1 >>  6) & 0x3ff, parm);\
  DST(op,i*32+ 8, (w1 >> 16) & 0x3ff, parm);\
  DST(op,i*32+ 9, (w1 >> 26) & 0x3ff, parm);\
  DST(op,i*32+10, (w1 >> 36) & 0x3ff, parm);\
  DST(op,i*32+11, (w1 >> 46) & 0x3ff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*5+3)*8/sizeof(ip[0]));\
\
  DST(op,i*32+12, (w1 >> 56) | (w2 <<  8) & 0x3ff, parm);\
  DST(op,i*32+13, (w2 >>  2) & 0x3ff, parm);\
  DST(op,i*32+14, (w2 >> 12) & 0x3ff, parm);\
  DST(op,i*32+15, (w2 >> 22) & 0x3ff, parm);\
  DST(op,i*32+16, (w2 >> 32) & 0x3ff, parm);\
  DST(op,i*32+17, (w2 >> 42) & 0x3ff, parm);\
  DST(op,i*32+18, (w2 >> 52) & 0x3ff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*5+4)*8/sizeof(ip[0]));\
\
  DST(op,i*32+19, (w2 >> 62) | (w3 <<  2) & 0x3ff, parm);\
  DST(op,i*32+20, (w3 >>  8) & 0x3ff, parm);\
  DST(op,i*32+21, (w3 >> 18) & 0x3ff, parm);\
  DST(op,i*32+22, (w3 >> 28) & 0x3ff, parm);\
  DST(op,i*32+23, (w3 >> 38) & 0x3ff, parm);\
  DST(op,i*32+24, (w3 >> 48) & 0x3ff, parm);  \
\
  DST(op,i*32+25, (w3 >> 58) | (w4 <<  6) & 0x3ff, parm);\
  DST(op,i*32+26, (w4 >>  4) & 0x3ff, parm);\
  DST(op,i*32+27, (w4 >> 14) & 0x3ff, parm);\
  DST(op,i*32+28, (w4 >> 24) & 0x3ff, parm);\
  DST(op,i*32+29, (w4 >> 34) & 0x3ff, parm);\
  DST(op,i*32+30, (w4 >> 44) & 0x3ff, parm);\
  DST(op,i*32+31, (w4 >> 54) , parm);;\
}

#define BITUNPACK64_10(ip,  op, parm) { \
  BITUNBLK64_10(ip, 0, op, parm);  DSTI(op); ip += 10*4/sizeof(ip[0]);\
}

#define BITUNBLK64_11(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*11+0)*8/sizeof(ip[0]));register uint64_t w1 = *(uint64_t *)(ip+(i*11+1)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7ff, parm);\
  DST(op,i*64+ 1, (w0 >> 11) & 0x7ff, parm);\
  DST(op,i*64+ 2, (w0 >> 22) & 0x7ff, parm);\
  DST(op,i*64+ 3, (w0 >> 33) & 0x7ff, parm);\
  DST(op,i*64+ 4, (w0 >> 44) & 0x7ff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*11+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 5, (w0 >> 55) | (w1 <<  9) & 0x7ff, parm);\
  DST(op,i*64+ 6, (w1 >>  2) & 0x7ff, parm);\
  DST(op,i*64+ 7, (w1 >> 13) & 0x7ff, parm);\
  DST(op,i*64+ 8, (w1 >> 24) & 0x7ff, parm);\
  DST(op,i*64+ 9, (w1 >> 35) & 0x7ff, parm);\
  DST(op,i*64+10, (w1 >> 46) & 0x7ff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*11+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+11, (w1 >> 57) | (w2 <<  7) & 0x7ff, parm);\
  DST(op,i*64+12, (w2 >>  4) & 0x7ff, parm);\
  DST(op,i*64+13, (w2 >> 15) & 0x7ff, parm);\
  DST(op,i*64+14, (w2 >> 26) & 0x7ff, parm);\
  DST(op,i*64+15, (w2 >> 37) & 0x7ff, parm);\
  DST(op,i*64+16, (w2 >> 48) & 0x7ff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*11+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+17, (w2 >> 59) | (w3 <<  5) & 0x7ff, parm);\
  DST(op,i*64+18, (w3 >>  6) & 0x7ff, parm);\
  DST(op,i*64+19, (w3 >> 17) & 0x7ff, parm);\
  DST(op,i*64+20, (w3 >> 28) & 0x7ff, parm);\
  DST(op,i*64+21, (w3 >> 39) & 0x7ff, parm);\
  DST(op,i*64+22, (w3 >> 50) & 0x7ff, parm);  register uint32_t w5 = *(uint32_t *)(ip+(i*11+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+23, (w3 >> 61) | (w4 <<  3) & 0x7ff, parm);\
  DST(op,i*64+24, (w4 >>  8) & 0x7ff, parm);\
  DST(op,i*64+25, (w4 >> 19) & 0x7ff, parm);\
  DST(op,i*64+26, (w4 >> 30) & 0x7ff, parm);\
  DST(op,i*64+27, (w4 >> 41) & 0x7ff, parm);\
  DST(op,i*64+28, (w4 >> 52) & 0x7ff, parm);  \
\
  DST(op,i*64+29, (w4 >> 63) | (w5 <<  1) & 0x7ff, parm);\
  DST(op,i*64+30, (w5 >> 10) & 0x7ff, parm);\
  DST(op,i*64+31, (w5 >> 21) & 0x7ff, parm);;\
}

#define BITUNPACK64_11(ip,  op, parm) { \
  BITUNBLK64_11(ip, 0, op, parm);  DSTI(op); ip += 11*4/sizeof(ip[0]);\
}

#define BITUNBLK64_12(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*3+0)*8/sizeof(ip[0]));register uint64_t w1 = *(uint64_t *)(ip+(i*3+1)*8/sizeof(ip[0]));\
  DST(op,i*16+ 0, (w0 ) & 0xfff, parm);\
  DST(op,i*16+ 1, (w0 >> 12) & 0xfff, parm);\
  DST(op,i*16+ 2, (w0 >> 24) & 0xfff, parm);\
  DST(op,i*16+ 3, (w0 >> 36) & 0xfff, parm);\
  DST(op,i*16+ 4, (w0 >> 48) & 0xfff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*3+2)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 5, (w0 >> 60) | (w1 <<  4) & 0xfff, parm);\
  DST(op,i*16+ 6, (w1 >>  8) & 0xfff, parm);\
  DST(op,i*16+ 7, (w1 >> 20) & 0xfff, parm);\
  DST(op,i*16+ 8, (w1 >> 32) & 0xfff, parm);\
  DST(op,i*16+ 9, (w1 >> 44) & 0xfff, parm);  \
\
  DST(op,i*16+10, (w1 >> 56) | (w2 <<  8) & 0xfff, parm);\
  DST(op,i*16+11, (w2 >>  4) & 0xfff, parm);\
  DST(op,i*16+12, (w2 >> 16) & 0xfff, parm);\
  DST(op,i*16+13, (w2 >> 28) & 0xfff, parm);\
  DST(op,i*16+14, (w2 >> 40) & 0xfff, parm);\
  DST(op,i*16+15, (w2 >> 52) , parm);;\
}

#define BITUNPACK64_12(ip,  op, parm) { \
  BITUNBLK64_12(ip, 0, op, parm);\
  BITUNBLK64_12(ip, 1, op, parm);  DSTI(op); ip += 12*4/sizeof(ip[0]);\
}

#define BITUNBLK64_13(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*13+0)*8/sizeof(ip[0]));register uint64_t w1 = *(uint64_t *)(ip+(i*13+1)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x1fff, parm);\
  DST(op,i*64+ 1, (w0 >> 13) & 0x1fff, parm);\
  DST(op,i*64+ 2, (w0 >> 26) & 0x1fff, parm);\
  DST(op,i*64+ 3, (w0 >> 39) & 0x1fff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*13+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 4, (w0 >> 52) | (w1 << 12) & 0x1fff, parm);\
  DST(op,i*64+ 5, (w1 >>  1) & 0x1fff, parm);\
  DST(op,i*64+ 6, (w1 >> 14) & 0x1fff, parm);\
  DST(op,i*64+ 7, (w1 >> 27) & 0x1fff, parm);\
  DST(op,i*64+ 8, (w1 >> 40) & 0x1fff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*13+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 9, (w1 >> 53) | (w2 << 11) & 0x1fff, parm);\
  DST(op,i*64+10, (w2 >>  2) & 0x1fff, parm);\
  DST(op,i*64+11, (w2 >> 15) & 0x1fff, parm);\
  DST(op,i*64+12, (w2 >> 28) & 0x1fff, parm);\
  DST(op,i*64+13, (w2 >> 41) & 0x1fff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*13+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+14, (w2 >> 54) | (w3 << 10) & 0x1fff, parm);\
  DST(op,i*64+15, (w3 >>  3) & 0x1fff, parm);\
  DST(op,i*64+16, (w3 >> 16) & 0x1fff, parm);\
  DST(op,i*64+17, (w3 >> 29) & 0x1fff, parm);\
  DST(op,i*64+18, (w3 >> 42) & 0x1fff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*13+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+19, (w3 >> 55) | (w4 <<  9) & 0x1fff, parm);\
  DST(op,i*64+20, (w4 >>  4) & 0x1fff, parm);\
  DST(op,i*64+21, (w4 >> 17) & 0x1fff, parm);\
  DST(op,i*64+22, (w4 >> 30) & 0x1fff, parm);\
  DST(op,i*64+23, (w4 >> 43) & 0x1fff, parm);  register uint32_t w6 = *(uint32_t *)(ip+(i*13+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+24, (w4 >> 56) | (w5 <<  8) & 0x1fff, parm);\
  DST(op,i*64+25, (w5 >>  5) & 0x1fff, parm);\
  DST(op,i*64+26, (w5 >> 18) & 0x1fff, parm);\
  DST(op,i*64+27, (w5 >> 31) & 0x1fff, parm);\
  DST(op,i*64+28, (w5 >> 44) & 0x1fff, parm);  \
\
  DST(op,i*64+29, (w5 >> 57) | (w6 <<  7) & 0x1fff, parm);\
  DST(op,i*64+30, (w6 >>  6) & 0x1fff, parm);\
  DST(op,i*64+31, (w6 >> 19) & 0x1fff, parm);;\
}

#define BITUNPACK64_13(ip,  op, parm) { \
  BITUNBLK64_13(ip, 0, op, parm);  DSTI(op); ip += 13*4/sizeof(ip[0]);\
}

#define BITUNBLK64_14(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*7+0)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3fff, parm);\
  DST(op,i*32+ 1, (w0 >> 14) & 0x3fff, parm);\
  DST(op,i*32+ 2, (w0 >> 28) & 0x3fff, parm);\
  DST(op,i*32+ 3, (w0 >> 42) & 0x3fff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*7+1)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 4, (w0 >> 56) | (w1 <<  8) & 0x3fff, parm);\
  DST(op,i*32+ 5, (w1 >>  6) & 0x3fff, parm);\
  DST(op,i*32+ 6, (w1 >> 20) & 0x3fff, parm);\
  DST(op,i*32+ 7, (w1 >> 34) & 0x3fff, parm);\
  DST(op,i*32+ 8, (w1 >> 48) & 0x3fff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*7+2)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 9, (w1 >> 62) | (w2 <<  2) & 0x3fff, parm);\
  DST(op,i*32+10, (w2 >> 12) & 0x3fff, parm);\
  DST(op,i*32+11, (w2 >> 26) & 0x3fff, parm);\
  DST(op,i*32+12, (w2 >> 40) & 0x3fff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*7+3)*8/sizeof(ip[0]));\
\
  DST(op,i*32+13, (w2 >> 54) | (w3 << 10) & 0x3fff, parm);\
  DST(op,i*32+14, (w3 >>  4) & 0x3fff, parm);\
  DST(op,i*32+15, (w3 >> 18) & 0x3fff, parm);\
  DST(op,i*32+16, (w3 >> 32) & 0x3fff, parm);\
  DST(op,i*32+17, (w3 >> 46) & 0x3fff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*7+4)*8/sizeof(ip[0]));\
\
  DST(op,i*32+18, (w3 >> 60) | (w4 <<  4) & 0x3fff, parm);\
  DST(op,i*32+19, (w4 >> 10) & 0x3fff, parm);\
  DST(op,i*32+20, (w4 >> 24) & 0x3fff, parm);\
  DST(op,i*32+21, (w4 >> 38) & 0x3fff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*7+5)*8/sizeof(ip[0]));\
\
  DST(op,i*32+22, (w4 >> 52) | (w5 << 12) & 0x3fff, parm);\
  DST(op,i*32+23, (w5 >>  2) & 0x3fff, parm);\
  DST(op,i*32+24, (w5 >> 16) & 0x3fff, parm);\
  DST(op,i*32+25, (w5 >> 30) & 0x3fff, parm);\
  DST(op,i*32+26, (w5 >> 44) & 0x3fff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*7+6)*8/sizeof(ip[0]));\
\
  DST(op,i*32+27, (w5 >> 58) | (w6 <<  6) & 0x3fff, parm);\
  DST(op,i*32+28, (w6 >>  8) & 0x3fff, parm);\
  DST(op,i*32+29, (w6 >> 22) & 0x3fff, parm);\
  DST(op,i*32+30, (w6 >> 36) & 0x3fff, parm);\
  DST(op,i*32+31, (w6 >> 50) , parm);;\
}

#define BITUNPACK64_14(ip,  op, parm) { \
  BITUNBLK64_14(ip, 0, op, parm);  DSTI(op); ip += 14*4/sizeof(ip[0]);\
}

#define BITUNBLK64_15(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*15+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7fff, parm);\
  DST(op,i*64+ 1, (w0 >> 15) & 0x7fff, parm);\
  DST(op,i*64+ 2, (w0 >> 30) & 0x7fff, parm);\
  DST(op,i*64+ 3, (w0 >> 45) & 0x7fff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*15+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 4, (w0 >> 60) | (w1 <<  4) & 0x7fff, parm);\
  DST(op,i*64+ 5, (w1 >> 11) & 0x7fff, parm);\
  DST(op,i*64+ 6, (w1 >> 26) & 0x7fff, parm);\
  DST(op,i*64+ 7, (w1 >> 41) & 0x7fff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*15+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 8, (w1 >> 56) | (w2 <<  8) & 0x7fff, parm);\
  DST(op,i*64+ 9, (w2 >>  7) & 0x7fff, parm);\
  DST(op,i*64+10, (w2 >> 22) & 0x7fff, parm);\
  DST(op,i*64+11, (w2 >> 37) & 0x7fff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*15+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+12, (w2 >> 52) | (w3 << 12) & 0x7fff, parm);\
  DST(op,i*64+13, (w3 >>  3) & 0x7fff, parm);\
  DST(op,i*64+14, (w3 >> 18) & 0x7fff, parm);\
  DST(op,i*64+15, (w3 >> 33) & 0x7fff, parm);\
  DST(op,i*64+16, (w3 >> 48) & 0x7fff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*15+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+17, (w3 >> 63) | (w4 <<  1) & 0x7fff, parm);\
  DST(op,i*64+18, (w4 >> 14) & 0x7fff, parm);\
  DST(op,i*64+19, (w4 >> 29) & 0x7fff, parm);\
  DST(op,i*64+20, (w4 >> 44) & 0x7fff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*15+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+21, (w4 >> 59) | (w5 <<  5) & 0x7fff, parm);\
  DST(op,i*64+22, (w5 >> 10) & 0x7fff, parm);\
  DST(op,i*64+23, (w5 >> 25) & 0x7fff, parm);\
  DST(op,i*64+24, (w5 >> 40) & 0x7fff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*15+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+25, (w5 >> 55) | (w6 <<  9) & 0x7fff, parm);\
  DST(op,i*64+26, (w6 >>  6) & 0x7fff, parm);\
  DST(op,i*64+27, (w6 >> 21) & 0x7fff, parm);\
  DST(op,i*64+28, (w6 >> 36) & 0x7fff, parm);  register uint32_t w7 = *(uint32_t *)(ip+(i*15+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+29, (w6 >> 51) | (w7 << 13) & 0x7fff, parm);\
  DST(op,i*64+30, (w7 >>  2) & 0x7fff, parm);\
  DST(op,i*64+31, (w7 >> 17) & 0x7fff, parm);;\
}

#define BITUNPACK64_15(ip,  op, parm) { \
  BITUNBLK64_15(ip, 0, op, parm);  DSTI(op); ip += 15*4/sizeof(ip[0]);\
}

#define BITUNBLK64_16(ip, i, op, parm) { \
  DST(op,i*4+ 0, *(uint16_t *)(ip+i*8+ 0), parm);\
  DST(op,i*4+ 1, *(uint16_t *)(ip+i*8+ 2), parm);\
  DST(op,i*4+ 2, *(uint16_t *)(ip+i*8+ 4), parm);\
  DST(op,i*4+ 3, *(uint16_t *)(ip+i*8+ 6), parm);;\
}

#define BITUNPACK64_16(ip,  op, parm) { \
  BITUNBLK64_16(ip, 0, op, parm);\
  BITUNBLK64_16(ip, 1, op, parm);\
  BITUNBLK64_16(ip, 2, op, parm);\
  BITUNBLK64_16(ip, 3, op, parm);\
  BITUNBLK64_16(ip, 4, op, parm);\
  BITUNBLK64_16(ip, 5, op, parm);\
  BITUNBLK64_16(ip, 6, op, parm);\
  BITUNBLK64_16(ip, 7, op, parm);  DSTI(op); ip += 16*4/sizeof(ip[0]);\
}

#define BITUNBLK64_17(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*17+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x1ffff, parm);\
  DST(op,i*64+ 1, (w0 >> 17) & 0x1ffff, parm);\
  DST(op,i*64+ 2, (w0 >> 34) & 0x1ffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*17+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 3, (w0 >> 51) | (w1 << 13) & 0x1ffff, parm);\
  DST(op,i*64+ 4, (w1 >>  4) & 0x1ffff, parm);\
  DST(op,i*64+ 5, (w1 >> 21) & 0x1ffff, parm);\
  DST(op,i*64+ 6, (w1 >> 38) & 0x1ffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*17+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 7, (w1 >> 55) | (w2 <<  9) & 0x1ffff, parm);\
  DST(op,i*64+ 8, (w2 >>  8) & 0x1ffff, parm);\
  DST(op,i*64+ 9, (w2 >> 25) & 0x1ffff, parm);\
  DST(op,i*64+10, (w2 >> 42) & 0x1ffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*17+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+11, (w2 >> 59) | (w3 <<  5) & 0x1ffff, parm);\
  DST(op,i*64+12, (w3 >> 12) & 0x1ffff, parm);\
  DST(op,i*64+13, (w3 >> 29) & 0x1ffff, parm);\
  DST(op,i*64+14, (w3 >> 46) & 0x1ffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*17+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+15, (w3 >> 63) | (w4 <<  1) & 0x1ffff, parm);\
  DST(op,i*64+16, (w4 >> 16) & 0x1ffff, parm);\
  DST(op,i*64+17, (w4 >> 33) & 0x1ffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*17+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+18, (w4 >> 50) | (w5 << 14) & 0x1ffff, parm);\
  DST(op,i*64+19, (w5 >>  3) & 0x1ffff, parm);\
  DST(op,i*64+20, (w5 >> 20) & 0x1ffff, parm);\
  DST(op,i*64+21, (w5 >> 37) & 0x1ffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*17+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+22, (w5 >> 54) | (w6 << 10) & 0x1ffff, parm);\
  DST(op,i*64+23, (w6 >>  7) & 0x1ffff, parm);\
  DST(op,i*64+24, (w6 >> 24) & 0x1ffff, parm);\
  DST(op,i*64+25, (w6 >> 41) & 0x1ffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*17+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+26, (w6 >> 58) | (w7 <<  6) & 0x1ffff, parm);\
  DST(op,i*64+27, (w7 >> 11) & 0x1ffff, parm);\
  DST(op,i*64+28, (w7 >> 28) & 0x1ffff, parm);\
  DST(op,i*64+29, (w7 >> 45) & 0x1ffff, parm);  register uint32_t w8 = *(uint32_t *)(ip+(i*17+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w7 >> 62) | (w8 <<  2) & 0x1ffff, parm);\
  DST(op,i*64+31, (w8 >> 15) & 0x1ffff, parm);;\
}

#define BITUNPACK64_17(ip,  op, parm) { \
  BITUNBLK64_17(ip, 0, op, parm);  DSTI(op); ip += 17*4/sizeof(ip[0]);\
}

#define BITUNBLK64_18(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*9+0)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3ffff, parm);\
  DST(op,i*32+ 1, (w0 >> 18) & 0x3ffff, parm);\
  DST(op,i*32+ 2, (w0 >> 36) & 0x3ffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*9+1)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 3, (w0 >> 54) | (w1 << 10) & 0x3ffff, parm);\
  DST(op,i*32+ 4, (w1 >>  8) & 0x3ffff, parm);\
  DST(op,i*32+ 5, (w1 >> 26) & 0x3ffff, parm);\
  DST(op,i*32+ 6, (w1 >> 44) & 0x3ffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*9+2)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 7, (w1 >> 62) | (w2 <<  2) & 0x3ffff, parm);\
  DST(op,i*32+ 8, (w2 >> 16) & 0x3ffff, parm);\
  DST(op,i*32+ 9, (w2 >> 34) & 0x3ffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*9+3)*8/sizeof(ip[0]));\
\
  DST(op,i*32+10, (w2 >> 52) | (w3 << 12) & 0x3ffff, parm);\
  DST(op,i*32+11, (w3 >>  6) & 0x3ffff, parm);\
  DST(op,i*32+12, (w3 >> 24) & 0x3ffff, parm);\
  DST(op,i*32+13, (w3 >> 42) & 0x3ffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*9+4)*8/sizeof(ip[0]));\
\
  DST(op,i*32+14, (w3 >> 60) | (w4 <<  4) & 0x3ffff, parm);\
  DST(op,i*32+15, (w4 >> 14) & 0x3ffff, parm);\
  DST(op,i*32+16, (w4 >> 32) & 0x3ffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*9+5)*8/sizeof(ip[0]));\
\
  DST(op,i*32+17, (w4 >> 50) | (w5 << 14) & 0x3ffff, parm);\
  DST(op,i*32+18, (w5 >>  4) & 0x3ffff, parm);\
  DST(op,i*32+19, (w5 >> 22) & 0x3ffff, parm);\
  DST(op,i*32+20, (w5 >> 40) & 0x3ffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*9+6)*8/sizeof(ip[0]));\
\
  DST(op,i*32+21, (w5 >> 58) | (w6 <<  6) & 0x3ffff, parm);\
  DST(op,i*32+22, (w6 >> 12) & 0x3ffff, parm);\
  DST(op,i*32+23, (w6 >> 30) & 0x3ffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*9+7)*8/sizeof(ip[0]));\
\
  DST(op,i*32+24, (w6 >> 48) | (w7 << 16) & 0x3ffff, parm);\
  DST(op,i*32+25, (w7 >>  2) & 0x3ffff, parm);\
  DST(op,i*32+26, (w7 >> 20) & 0x3ffff, parm);\
  DST(op,i*32+27, (w7 >> 38) & 0x3ffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*9+8)*8/sizeof(ip[0]));\
\
  DST(op,i*32+28, (w7 >> 56) | (w8 <<  8) & 0x3ffff, parm);\
  DST(op,i*32+29, (w8 >> 10) & 0x3ffff, parm);\
  DST(op,i*32+30, (w8 >> 28) & 0x3ffff, parm);\
  DST(op,i*32+31, (w8 >> 46) , parm);;\
}

#define BITUNPACK64_18(ip,  op, parm) { \
  BITUNBLK64_18(ip, 0, op, parm);  DSTI(op); ip += 18*4/sizeof(ip[0]);\
}

#define BITUNBLK64_19(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*19+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7ffff, parm);\
  DST(op,i*64+ 1, (w0 >> 19) & 0x7ffff, parm);\
  DST(op,i*64+ 2, (w0 >> 38) & 0x7ffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*19+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 3, (w0 >> 57) | (w1 <<  7) & 0x7ffff, parm);\
  DST(op,i*64+ 4, (w1 >> 12) & 0x7ffff, parm);\
  DST(op,i*64+ 5, (w1 >> 31) & 0x7ffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*19+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 6, (w1 >> 50) | (w2 << 14) & 0x7ffff, parm);\
  DST(op,i*64+ 7, (w2 >>  5) & 0x7ffff, parm);\
  DST(op,i*64+ 8, (w2 >> 24) & 0x7ffff, parm);\
  DST(op,i*64+ 9, (w2 >> 43) & 0x7ffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*19+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+10, (w2 >> 62) | (w3 <<  2) & 0x7ffff, parm);\
  DST(op,i*64+11, (w3 >> 17) & 0x7ffff, parm);\
  DST(op,i*64+12, (w3 >> 36) & 0x7ffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*19+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+13, (w3 >> 55) | (w4 <<  9) & 0x7ffff, parm);\
  DST(op,i*64+14, (w4 >> 10) & 0x7ffff, parm);\
  DST(op,i*64+15, (w4 >> 29) & 0x7ffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*19+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+16, (w4 >> 48) | (w5 << 16) & 0x7ffff, parm);\
  DST(op,i*64+17, (w5 >>  3) & 0x7ffff, parm);\
  DST(op,i*64+18, (w5 >> 22) & 0x7ffff, parm);\
  DST(op,i*64+19, (w5 >> 41) & 0x7ffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*19+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+20, (w5 >> 60) | (w6 <<  4) & 0x7ffff, parm);\
  DST(op,i*64+21, (w6 >> 15) & 0x7ffff, parm);\
  DST(op,i*64+22, (w6 >> 34) & 0x7ffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*19+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+23, (w6 >> 53) | (w7 << 11) & 0x7ffff, parm);\
  DST(op,i*64+24, (w7 >>  8) & 0x7ffff, parm);\
  DST(op,i*64+25, (w7 >> 27) & 0x7ffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*19+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+26, (w7 >> 46) | (w8 << 18) & 0x7ffff, parm);\
  DST(op,i*64+27, (w8 >>  1) & 0x7ffff, parm);\
  DST(op,i*64+28, (w8 >> 20) & 0x7ffff, parm);\
  DST(op,i*64+29, (w8 >> 39) & 0x7ffff, parm);  register uint32_t w9 = *(uint32_t *)(ip+(i*19+9)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w8 >> 58) | (w9 <<  6) & 0x7ffff, parm);\
  DST(op,i*64+31, (w9 >> 13) & 0x7ffff, parm);;\
}

#define BITUNPACK64_19(ip,  op, parm) { \
  BITUNBLK64_19(ip, 0, op, parm);  DSTI(op); ip += 19*4/sizeof(ip[0]);\
}

#define BITUNBLK64_20(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*5+0)*8/sizeof(ip[0]));\
  DST(op,i*16+ 0, (w0 ) & 0xfffff, parm);\
  DST(op,i*16+ 1, (w0 >> 20) & 0xfffff, parm);\
  DST(op,i*16+ 2, (w0 >> 40) & 0xfffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*5+1)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 3, (w0 >> 60) | (w1 <<  4) & 0xfffff, parm);\
  DST(op,i*16+ 4, (w1 >> 16) & 0xfffff, parm);\
  DST(op,i*16+ 5, (w1 >> 36) & 0xfffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*5+2)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 6, (w1 >> 56) | (w2 <<  8) & 0xfffff, parm);\
  DST(op,i*16+ 7, (w2 >> 12) & 0xfffff, parm);\
  DST(op,i*16+ 8, (w2 >> 32) & 0xfffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*5+3)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 9, (w2 >> 52) | (w3 << 12) & 0xfffff, parm);\
  DST(op,i*16+10, (w3 >>  8) & 0xfffff, parm);\
  DST(op,i*16+11, (w3 >> 28) & 0xfffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*5+4)*8/sizeof(ip[0]));\
\
  DST(op,i*16+12, (w3 >> 48) | (w4 << 16) & 0xfffff, parm);\
  DST(op,i*16+13, (w4 >>  4) & 0xfffff, parm);\
  DST(op,i*16+14, (w4 >> 24) & 0xfffff, parm);\
  DST(op,i*16+15, (w4 >> 44) , parm);;\
}

#define BITUNPACK64_20(ip,  op, parm) { \
  BITUNBLK64_20(ip, 0, op, parm);\
  BITUNBLK64_20(ip, 1, op, parm);  DSTI(op); ip += 20*4/sizeof(ip[0]);\
}

#define BITUNBLK64_21(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*21+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x1fffff, parm);\
  DST(op,i*64+ 1, (w0 >> 21) & 0x1fffff, parm);\
  DST(op,i*64+ 2, (w0 >> 42) & 0x1fffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*21+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 3, (w0 >> 63) | (w1 <<  1) & 0x1fffff, parm);\
  DST(op,i*64+ 4, (w1 >> 20) & 0x1fffff, parm);\
  DST(op,i*64+ 5, (w1 >> 41) & 0x1fffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*21+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 6, (w1 >> 62) | (w2 <<  2) & 0x1fffff, parm);\
  DST(op,i*64+ 7, (w2 >> 19) & 0x1fffff, parm);\
  DST(op,i*64+ 8, (w2 >> 40) & 0x1fffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*21+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 9, (w2 >> 61) | (w3 <<  3) & 0x1fffff, parm);\
  DST(op,i*64+10, (w3 >> 18) & 0x1fffff, parm);\
  DST(op,i*64+11, (w3 >> 39) & 0x1fffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*21+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+12, (w3 >> 60) | (w4 <<  4) & 0x1fffff, parm);\
  DST(op,i*64+13, (w4 >> 17) & 0x1fffff, parm);\
  DST(op,i*64+14, (w4 >> 38) & 0x1fffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*21+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+15, (w4 >> 59) | (w5 <<  5) & 0x1fffff, parm);\
  DST(op,i*64+16, (w5 >> 16) & 0x1fffff, parm);\
  DST(op,i*64+17, (w5 >> 37) & 0x1fffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*21+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+18, (w5 >> 58) | (w6 <<  6) & 0x1fffff, parm);\
  DST(op,i*64+19, (w6 >> 15) & 0x1fffff, parm);\
  DST(op,i*64+20, (w6 >> 36) & 0x1fffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*21+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+21, (w6 >> 57) | (w7 <<  7) & 0x1fffff, parm);\
  DST(op,i*64+22, (w7 >> 14) & 0x1fffff, parm);\
  DST(op,i*64+23, (w7 >> 35) & 0x1fffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*21+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+24, (w7 >> 56) | (w8 <<  8) & 0x1fffff, parm);\
  DST(op,i*64+25, (w8 >> 13) & 0x1fffff, parm);\
  DST(op,i*64+26, (w8 >> 34) & 0x1fffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*21+9)*8/sizeof(ip[0]));\
\
  DST(op,i*64+27, (w8 >> 55) | (w9 <<  9) & 0x1fffff, parm);\
  DST(op,i*64+28, (w9 >> 12) & 0x1fffff, parm);\
  DST(op,i*64+29, (w9 >> 33) & 0x1fffff, parm);  register uint32_t w10 = *(uint32_t *)(ip+(i*21+10)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w9 >> 54) | (w10 << 10) & 0x1fffff, parm);\
  DST(op,i*64+31, (w10 >> 11) & 0x1fffff, parm);;\
}

#define BITUNPACK64_21(ip,  op, parm) { \
  BITUNBLK64_21(ip, 0, op, parm);  DSTI(op); ip += 21*4/sizeof(ip[0]);\
}

#define BITUNBLK64_22(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*11+0)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3fffff, parm);\
  DST(op,i*32+ 1, (w0 >> 22) & 0x3fffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*11+1)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 2, (w0 >> 44) | (w1 << 20) & 0x3fffff, parm);\
  DST(op,i*32+ 3, (w1 >>  2) & 0x3fffff, parm);\
  DST(op,i*32+ 4, (w1 >> 24) & 0x3fffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*11+2)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 5, (w1 >> 46) | (w2 << 18) & 0x3fffff, parm);\
  DST(op,i*32+ 6, (w2 >>  4) & 0x3fffff, parm);\
  DST(op,i*32+ 7, (w2 >> 26) & 0x3fffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*11+3)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 8, (w2 >> 48) | (w3 << 16) & 0x3fffff, parm);\
  DST(op,i*32+ 9, (w3 >>  6) & 0x3fffff, parm);\
  DST(op,i*32+10, (w3 >> 28) & 0x3fffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*11+4)*8/sizeof(ip[0]));\
\
  DST(op,i*32+11, (w3 >> 50) | (w4 << 14) & 0x3fffff, parm);\
  DST(op,i*32+12, (w4 >>  8) & 0x3fffff, parm);\
  DST(op,i*32+13, (w4 >> 30) & 0x3fffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*11+5)*8/sizeof(ip[0]));\
\
  DST(op,i*32+14, (w4 >> 52) | (w5 << 12) & 0x3fffff, parm);\
  DST(op,i*32+15, (w5 >> 10) & 0x3fffff, parm);\
  DST(op,i*32+16, (w5 >> 32) & 0x3fffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*11+6)*8/sizeof(ip[0]));\
\
  DST(op,i*32+17, (w5 >> 54) | (w6 << 10) & 0x3fffff, parm);\
  DST(op,i*32+18, (w6 >> 12) & 0x3fffff, parm);\
  DST(op,i*32+19, (w6 >> 34) & 0x3fffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*11+7)*8/sizeof(ip[0]));\
\
  DST(op,i*32+20, (w6 >> 56) | (w7 <<  8) & 0x3fffff, parm);\
  DST(op,i*32+21, (w7 >> 14) & 0x3fffff, parm);\
  DST(op,i*32+22, (w7 >> 36) & 0x3fffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*11+8)*8/sizeof(ip[0]));\
\
  DST(op,i*32+23, (w7 >> 58) | (w8 <<  6) & 0x3fffff, parm);\
  DST(op,i*32+24, (w8 >> 16) & 0x3fffff, parm);\
  DST(op,i*32+25, (w8 >> 38) & 0x3fffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*11+9)*8/sizeof(ip[0]));\
\
  DST(op,i*32+26, (w8 >> 60) | (w9 <<  4) & 0x3fffff, parm);\
  DST(op,i*32+27, (w9 >> 18) & 0x3fffff, parm);\
  DST(op,i*32+28, (w9 >> 40) & 0x3fffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*11+10)*8/sizeof(ip[0]));\
\
  DST(op,i*32+29, (w9 >> 62) | (w10 <<  2) & 0x3fffff, parm);\
  DST(op,i*32+30, (w10 >> 20) & 0x3fffff, parm);\
  DST(op,i*32+31, (w10 >> 42) , parm);;\
}

#define BITUNPACK64_22(ip,  op, parm) { \
  BITUNBLK64_22(ip, 0, op, parm);  DSTI(op); ip += 22*4/sizeof(ip[0]);\
}

#define BITUNBLK64_23(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*23+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7fffff, parm);\
  DST(op,i*64+ 1, (w0 >> 23) & 0x7fffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*23+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 2, (w0 >> 46) | (w1 << 18) & 0x7fffff, parm);\
  DST(op,i*64+ 3, (w1 >>  5) & 0x7fffff, parm);\
  DST(op,i*64+ 4, (w1 >> 28) & 0x7fffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*23+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 5, (w1 >> 51) | (w2 << 13) & 0x7fffff, parm);\
  DST(op,i*64+ 6, (w2 >> 10) & 0x7fffff, parm);\
  DST(op,i*64+ 7, (w2 >> 33) & 0x7fffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*23+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 8, (w2 >> 56) | (w3 <<  8) & 0x7fffff, parm);\
  DST(op,i*64+ 9, (w3 >> 15) & 0x7fffff, parm);\
  DST(op,i*64+10, (w3 >> 38) & 0x7fffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*23+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+11, (w3 >> 61) | (w4 <<  3) & 0x7fffff, parm);\
  DST(op,i*64+12, (w4 >> 20) & 0x7fffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*23+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+13, (w4 >> 43) | (w5 << 21) & 0x7fffff, parm);\
  DST(op,i*64+14, (w5 >>  2) & 0x7fffff, parm);\
  DST(op,i*64+15, (w5 >> 25) & 0x7fffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*23+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+16, (w5 >> 48) | (w6 << 16) & 0x7fffff, parm);\
  DST(op,i*64+17, (w6 >>  7) & 0x7fffff, parm);\
  DST(op,i*64+18, (w6 >> 30) & 0x7fffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*23+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+19, (w6 >> 53) | (w7 << 11) & 0x7fffff, parm);\
  DST(op,i*64+20, (w7 >> 12) & 0x7fffff, parm);\
  DST(op,i*64+21, (w7 >> 35) & 0x7fffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*23+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+22, (w7 >> 58) | (w8 <<  6) & 0x7fffff, parm);\
  DST(op,i*64+23, (w8 >> 17) & 0x7fffff, parm);\
  DST(op,i*64+24, (w8 >> 40) & 0x7fffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*23+9)*8/sizeof(ip[0]));\
\
  DST(op,i*64+25, (w8 >> 63) | (w9 <<  1) & 0x7fffff, parm);\
  DST(op,i*64+26, (w9 >> 22) & 0x7fffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*23+10)*8/sizeof(ip[0]));\
\
  DST(op,i*64+27, (w9 >> 45) | (w10 << 19) & 0x7fffff, parm);\
  DST(op,i*64+28, (w10 >>  4) & 0x7fffff, parm);\
  DST(op,i*64+29, (w10 >> 27) & 0x7fffff, parm);  register uint32_t w11 = *(uint32_t *)(ip+(i*23+11)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w10 >> 50) | (w11 << 14) & 0x7fffff, parm);\
  DST(op,i*64+31, (w11 >>  9) & 0x7fffff, parm);;\
}

#define BITUNPACK64_23(ip,  op, parm) { \
  BITUNBLK64_23(ip, 0, op, parm);  DSTI(op); ip += 23*4/sizeof(ip[0]);\
}

#define BITUNBLK64_24(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*3+0)*8/sizeof(ip[0]));\
  DST(op,i*8+ 0, (w0 ) & 0xffffff, parm);\
  DST(op,i*8+ 1, (w0 >> 24) & 0xffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*3+1)*8/sizeof(ip[0]));\
\
  DST(op,i*8+ 2, (w0 >> 48) | (w1 << 16) & 0xffffff, parm);\
  DST(op,i*8+ 3, (w1 >>  8) & 0xffffff, parm);\
  DST(op,i*8+ 4, (w1 >> 32) & 0xffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*3+2)*8/sizeof(ip[0]));\
\
  DST(op,i*8+ 5, (w1 >> 56) | (w2 <<  8) & 0xffffff, parm);\
  DST(op,i*8+ 6, (w2 >> 16) & 0xffffff, parm);\
  DST(op,i*8+ 7, (w2 >> 40) , parm);;\
}

#define BITUNPACK64_24(ip,  op, parm) { \
  BITUNBLK64_24(ip, 0, op, parm);\
  BITUNBLK64_24(ip, 1, op, parm);\
  BITUNBLK64_24(ip, 2, op, parm);\
  BITUNBLK64_24(ip, 3, op, parm);  DSTI(op); ip += 24*4/sizeof(ip[0]);\
}

#define BITUNBLK64_25(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*25+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x1ffffff, parm);\
  DST(op,i*64+ 1, (w0 >> 25) & 0x1ffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*25+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 2, (w0 >> 50) | (w1 << 14) & 0x1ffffff, parm);\
  DST(op,i*64+ 3, (w1 >> 11) & 0x1ffffff, parm);\
  DST(op,i*64+ 4, (w1 >> 36) & 0x1ffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*25+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 5, (w1 >> 61) | (w2 <<  3) & 0x1ffffff, parm);\
  DST(op,i*64+ 6, (w2 >> 22) & 0x1ffffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*25+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 7, (w2 >> 47) | (w3 << 17) & 0x1ffffff, parm);\
  DST(op,i*64+ 8, (w3 >>  8) & 0x1ffffff, parm);\
  DST(op,i*64+ 9, (w3 >> 33) & 0x1ffffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*25+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+10, (w3 >> 58) | (w4 <<  6) & 0x1ffffff, parm);\
  DST(op,i*64+11, (w4 >> 19) & 0x1ffffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*25+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+12, (w4 >> 44) | (w5 << 20) & 0x1ffffff, parm);\
  DST(op,i*64+13, (w5 >>  5) & 0x1ffffff, parm);\
  DST(op,i*64+14, (w5 >> 30) & 0x1ffffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*25+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+15, (w5 >> 55) | (w6 <<  9) & 0x1ffffff, parm);\
  DST(op,i*64+16, (w6 >> 16) & 0x1ffffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*25+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+17, (w6 >> 41) | (w7 << 23) & 0x1ffffff, parm);\
  DST(op,i*64+18, (w7 >>  2) & 0x1ffffff, parm);\
  DST(op,i*64+19, (w7 >> 27) & 0x1ffffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*25+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+20, (w7 >> 52) | (w8 << 12) & 0x1ffffff, parm);\
  DST(op,i*64+21, (w8 >> 13) & 0x1ffffff, parm);\
  DST(op,i*64+22, (w8 >> 38) & 0x1ffffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*25+9)*8/sizeof(ip[0]));\
\
  DST(op,i*64+23, (w8 >> 63) | (w9 <<  1) & 0x1ffffff, parm);\
  DST(op,i*64+24, (w9 >> 24) & 0x1ffffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*25+10)*8/sizeof(ip[0]));\
\
  DST(op,i*64+25, (w9 >> 49) | (w10 << 15) & 0x1ffffff, parm);\
  DST(op,i*64+26, (w10 >> 10) & 0x1ffffff, parm);\
  DST(op,i*64+27, (w10 >> 35) & 0x1ffffff, parm);  register uint64_t w11 = *(uint64_t *)(ip+(i*25+11)*8/sizeof(ip[0]));\
\
  DST(op,i*64+28, (w10 >> 60) | (w11 <<  4) & 0x1ffffff, parm);\
  DST(op,i*64+29, (w11 >> 21) & 0x1ffffff, parm);  register uint32_t w12 = *(uint32_t *)(ip+(i*25+12)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w11 >> 46) | (w12 << 18) & 0x1ffffff, parm);\
  DST(op,i*64+31, (w12 >>  7) & 0x1ffffff, parm);;\
}

#define BITUNPACK64_25(ip,  op, parm) { \
  BITUNBLK64_25(ip, 0, op, parm);  DSTI(op); ip += 25*4/sizeof(ip[0]);\
}

#define BITUNBLK64_26(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*13+0)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3ffffff, parm);\
  DST(op,i*32+ 1, (w0 >> 26) & 0x3ffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*13+1)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 2, (w0 >> 52) | (w1 << 12) & 0x3ffffff, parm);\
  DST(op,i*32+ 3, (w1 >> 14) & 0x3ffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*13+2)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 4, (w1 >> 40) | (w2 << 24) & 0x3ffffff, parm);\
  DST(op,i*32+ 5, (w2 >>  2) & 0x3ffffff, parm);\
  DST(op,i*32+ 6, (w2 >> 28) & 0x3ffffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*13+3)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 7, (w2 >> 54) | (w3 << 10) & 0x3ffffff, parm);\
  DST(op,i*32+ 8, (w3 >> 16) & 0x3ffffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*13+4)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 9, (w3 >> 42) | (w4 << 22) & 0x3ffffff, parm);\
  DST(op,i*32+10, (w4 >>  4) & 0x3ffffff, parm);\
  DST(op,i*32+11, (w4 >> 30) & 0x3ffffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*13+5)*8/sizeof(ip[0]));\
\
  DST(op,i*32+12, (w4 >> 56) | (w5 <<  8) & 0x3ffffff, parm);\
  DST(op,i*32+13, (w5 >> 18) & 0x3ffffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*13+6)*8/sizeof(ip[0]));\
\
  DST(op,i*32+14, (w5 >> 44) | (w6 << 20) & 0x3ffffff, parm);\
  DST(op,i*32+15, (w6 >>  6) & 0x3ffffff, parm);\
  DST(op,i*32+16, (w6 >> 32) & 0x3ffffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*13+7)*8/sizeof(ip[0]));\
\
  DST(op,i*32+17, (w6 >> 58) | (w7 <<  6) & 0x3ffffff, parm);\
  DST(op,i*32+18, (w7 >> 20) & 0x3ffffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*13+8)*8/sizeof(ip[0]));\
\
  DST(op,i*32+19, (w7 >> 46) | (w8 << 18) & 0x3ffffff, parm);\
  DST(op,i*32+20, (w8 >>  8) & 0x3ffffff, parm);\
  DST(op,i*32+21, (w8 >> 34) & 0x3ffffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*13+9)*8/sizeof(ip[0]));\
\
  DST(op,i*32+22, (w8 >> 60) | (w9 <<  4) & 0x3ffffff, parm);\
  DST(op,i*32+23, (w9 >> 22) & 0x3ffffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*13+10)*8/sizeof(ip[0]));\
\
  DST(op,i*32+24, (w9 >> 48) | (w10 << 16) & 0x3ffffff, parm);\
  DST(op,i*32+25, (w10 >> 10) & 0x3ffffff, parm);\
  DST(op,i*32+26, (w10 >> 36) & 0x3ffffff, parm);  register uint64_t w11 = *(uint64_t *)(ip+(i*13+11)*8/sizeof(ip[0]));\
\
  DST(op,i*32+27, (w10 >> 62) | (w11 <<  2) & 0x3ffffff, parm);\
  DST(op,i*32+28, (w11 >> 24) & 0x3ffffff, parm);  register uint64_t w12 = *(uint64_t *)(ip+(i*13+12)*8/sizeof(ip[0]));\
\
  DST(op,i*32+29, (w11 >> 50) | (w12 << 14) & 0x3ffffff, parm);\
  DST(op,i*32+30, (w12 >> 12) & 0x3ffffff, parm);\
  DST(op,i*32+31, (w12 >> 38) , parm);;\
}

#define BITUNPACK64_26(ip,  op, parm) { \
  BITUNBLK64_26(ip, 0, op, parm);  DSTI(op); ip += 26*4/sizeof(ip[0]);\
}

#define BITUNBLK64_27(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*27+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7ffffff, parm);\
  DST(op,i*64+ 1, (w0 >> 27) & 0x7ffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*27+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 2, (w0 >> 54) | (w1 << 10) & 0x7ffffff, parm);\
  DST(op,i*64+ 3, (w1 >> 17) & 0x7ffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*27+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 4, (w1 >> 44) | (w2 << 20) & 0x7ffffff, parm);\
  DST(op,i*64+ 5, (w2 >>  7) & 0x7ffffff, parm);\
  DST(op,i*64+ 6, (w2 >> 34) & 0x7ffffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*27+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 7, (w2 >> 61) | (w3 <<  3) & 0x7ffffff, parm);\
  DST(op,i*64+ 8, (w3 >> 24) & 0x7ffffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*27+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 9, (w3 >> 51) | (w4 << 13) & 0x7ffffff, parm);\
  DST(op,i*64+10, (w4 >> 14) & 0x7ffffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*27+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+11, (w4 >> 41) | (w5 << 23) & 0x7ffffff, parm);\
  DST(op,i*64+12, (w5 >>  4) & 0x7ffffff, parm);\
  DST(op,i*64+13, (w5 >> 31) & 0x7ffffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*27+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+14, (w5 >> 58) | (w6 <<  6) & 0x7ffffff, parm);\
  DST(op,i*64+15, (w6 >> 21) & 0x7ffffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*27+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+16, (w6 >> 48) | (w7 << 16) & 0x7ffffff, parm);\
  DST(op,i*64+17, (w7 >> 11) & 0x7ffffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*27+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+18, (w7 >> 38) | (w8 << 26) & 0x7ffffff, parm);\
  DST(op,i*64+19, (w8 >>  1) & 0x7ffffff, parm);\
  DST(op,i*64+20, (w8 >> 28) & 0x7ffffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*27+9)*8/sizeof(ip[0]));\
\
  DST(op,i*64+21, (w8 >> 55) | (w9 <<  9) & 0x7ffffff, parm);\
  DST(op,i*64+22, (w9 >> 18) & 0x7ffffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*27+10)*8/sizeof(ip[0]));\
\
  DST(op,i*64+23, (w9 >> 45) | (w10 << 19) & 0x7ffffff, parm);\
  DST(op,i*64+24, (w10 >>  8) & 0x7ffffff, parm);\
  DST(op,i*64+25, (w10 >> 35) & 0x7ffffff, parm);  register uint64_t w11 = *(uint64_t *)(ip+(i*27+11)*8/sizeof(ip[0]));\
\
  DST(op,i*64+26, (w10 >> 62) | (w11 <<  2) & 0x7ffffff, parm);\
  DST(op,i*64+27, (w11 >> 25) & 0x7ffffff, parm);  register uint64_t w12 = *(uint64_t *)(ip+(i*27+12)*8/sizeof(ip[0]));\
\
  DST(op,i*64+28, (w11 >> 52) | (w12 << 12) & 0x7ffffff, parm);\
  DST(op,i*64+29, (w12 >> 15) & 0x7ffffff, parm);  register uint32_t w13 = *(uint32_t *)(ip+(i*27+13)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w12 >> 42) | (w13 << 22) & 0x7ffffff, parm);\
  DST(op,i*64+31, (w13 >>  5) & 0x7ffffff, parm);;\
}

#define BITUNPACK64_27(ip,  op, parm) { \
  BITUNBLK64_27(ip, 0, op, parm);  DSTI(op); ip += 27*4/sizeof(ip[0]);\
}

#define BITUNBLK64_28(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*7+0)*8/sizeof(ip[0]));\
  DST(op,i*16+ 0, (w0 ) & 0xfffffff, parm);\
  DST(op,i*16+ 1, (w0 >> 28) & 0xfffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*7+1)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 2, (w0 >> 56) | (w1 <<  8) & 0xfffffff, parm);\
  DST(op,i*16+ 3, (w1 >> 20) & 0xfffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*7+2)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 4, (w1 >> 48) | (w2 << 16) & 0xfffffff, parm);\
  DST(op,i*16+ 5, (w2 >> 12) & 0xfffffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*7+3)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 6, (w2 >> 40) | (w3 << 24) & 0xfffffff, parm);\
  DST(op,i*16+ 7, (w3 >>  4) & 0xfffffff, parm);\
  DST(op,i*16+ 8, (w3 >> 32) & 0xfffffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*7+4)*8/sizeof(ip[0]));\
\
  DST(op,i*16+ 9, (w3 >> 60) | (w4 <<  4) & 0xfffffff, parm);\
  DST(op,i*16+10, (w4 >> 24) & 0xfffffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*7+5)*8/sizeof(ip[0]));\
\
  DST(op,i*16+11, (w4 >> 52) | (w5 << 12) & 0xfffffff, parm);\
  DST(op,i*16+12, (w5 >> 16) & 0xfffffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*7+6)*8/sizeof(ip[0]));\
\
  DST(op,i*16+13, (w5 >> 44) | (w6 << 20) & 0xfffffff, parm);\
  DST(op,i*16+14, (w6 >>  8) & 0xfffffff, parm);\
  DST(op,i*16+15, (w6 >> 36) , parm);;\
}

#define BITUNPACK64_28(ip,  op, parm) { \
  BITUNBLK64_28(ip, 0, op, parm);\
  BITUNBLK64_28(ip, 1, op, parm);  DSTI(op); ip += 28*4/sizeof(ip[0]);\
}

#define BITUNBLK64_29(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*29+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x1fffffff, parm);\
  DST(op,i*64+ 1, (w0 >> 29) & 0x1fffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*29+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 2, (w0 >> 58) | (w1 <<  6) & 0x1fffffff, parm);\
  DST(op,i*64+ 3, (w1 >> 23) & 0x1fffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*29+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 4, (w1 >> 52) | (w2 << 12) & 0x1fffffff, parm);\
  DST(op,i*64+ 5, (w2 >> 17) & 0x1fffffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*29+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 6, (w2 >> 46) | (w3 << 18) & 0x1fffffff, parm);\
  DST(op,i*64+ 7, (w3 >> 11) & 0x1fffffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*29+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 8, (w3 >> 40) | (w4 << 24) & 0x1fffffff, parm);\
  DST(op,i*64+ 9, (w4 >>  5) & 0x1fffffff, parm);\
  DST(op,i*64+10, (w4 >> 34) & 0x1fffffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*29+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+11, (w4 >> 63) | (w5 <<  1) & 0x1fffffff, parm);\
  DST(op,i*64+12, (w5 >> 28) & 0x1fffffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*29+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+13, (w5 >> 57) | (w6 <<  7) & 0x1fffffff, parm);\
  DST(op,i*64+14, (w6 >> 22) & 0x1fffffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*29+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+15, (w6 >> 51) | (w7 << 13) & 0x1fffffff, parm);\
  DST(op,i*64+16, (w7 >> 16) & 0x1fffffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*29+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+17, (w7 >> 45) | (w8 << 19) & 0x1fffffff, parm);\
  DST(op,i*64+18, (w8 >> 10) & 0x1fffffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*29+9)*8/sizeof(ip[0]));\
\
  DST(op,i*64+19, (w8 >> 39) | (w9 << 25) & 0x1fffffff, parm);\
  DST(op,i*64+20, (w9 >>  4) & 0x1fffffff, parm);\
  DST(op,i*64+21, (w9 >> 33) & 0x1fffffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*29+10)*8/sizeof(ip[0]));\
\
  DST(op,i*64+22, (w9 >> 62) | (w10 <<  2) & 0x1fffffff, parm);\
  DST(op,i*64+23, (w10 >> 27) & 0x1fffffff, parm);  register uint64_t w11 = *(uint64_t *)(ip+(i*29+11)*8/sizeof(ip[0]));\
\
  DST(op,i*64+24, (w10 >> 56) | (w11 <<  8) & 0x1fffffff, parm);\
  DST(op,i*64+25, (w11 >> 21) & 0x1fffffff, parm);  register uint64_t w12 = *(uint64_t *)(ip+(i*29+12)*8/sizeof(ip[0]));\
\
  DST(op,i*64+26, (w11 >> 50) | (w12 << 14) & 0x1fffffff, parm);\
  DST(op,i*64+27, (w12 >> 15) & 0x1fffffff, parm);  register uint64_t w13 = *(uint64_t *)(ip+(i*29+13)*8/sizeof(ip[0]));\
\
  DST(op,i*64+28, (w12 >> 44) | (w13 << 20) & 0x1fffffff, parm);\
  DST(op,i*64+29, (w13 >>  9) & 0x1fffffff, parm);  register uint32_t w14 = *(uint32_t *)(ip+(i*29+14)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w13 >> 38) | (w14 << 26) & 0x1fffffff, parm);\
  DST(op,i*64+31, (w14 >>  3) & 0x1fffffff, parm);;\
}

#define BITUNPACK64_29(ip,  op, parm) { \
  BITUNBLK64_29(ip, 0, op, parm);  DSTI(op); ip += 29*4/sizeof(ip[0]);\
}

#define BITUNBLK64_30(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*15+0)*8/sizeof(ip[0]));\
  DST(op,i*32+ 0, (w0 ) & 0x3fffffff, parm);\
  DST(op,i*32+ 1, (w0 >> 30) & 0x3fffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*15+1)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 2, (w0 >> 60) | (w1 <<  4) & 0x3fffffff, parm);\
  DST(op,i*32+ 3, (w1 >> 26) & 0x3fffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*15+2)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 4, (w1 >> 56) | (w2 <<  8) & 0x3fffffff, parm);\
  DST(op,i*32+ 5, (w2 >> 22) & 0x3fffffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*15+3)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 6, (w2 >> 52) | (w3 << 12) & 0x3fffffff, parm);\
  DST(op,i*32+ 7, (w3 >> 18) & 0x3fffffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*15+4)*8/sizeof(ip[0]));\
\
  DST(op,i*32+ 8, (w3 >> 48) | (w4 << 16) & 0x3fffffff, parm);\
  DST(op,i*32+ 9, (w4 >> 14) & 0x3fffffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*15+5)*8/sizeof(ip[0]));\
\
  DST(op,i*32+10, (w4 >> 44) | (w5 << 20) & 0x3fffffff, parm);\
  DST(op,i*32+11, (w5 >> 10) & 0x3fffffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*15+6)*8/sizeof(ip[0]));\
\
  DST(op,i*32+12, (w5 >> 40) | (w6 << 24) & 0x3fffffff, parm);\
  DST(op,i*32+13, (w6 >>  6) & 0x3fffffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*15+7)*8/sizeof(ip[0]));\
\
  DST(op,i*32+14, (w6 >> 36) | (w7 << 28) & 0x3fffffff, parm);\
  DST(op,i*32+15, (w7 >>  2) & 0x3fffffff, parm);\
  DST(op,i*32+16, (w7 >> 32) & 0x3fffffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*15+8)*8/sizeof(ip[0]));\
\
  DST(op,i*32+17, (w7 >> 62) | (w8 <<  2) & 0x3fffffff, parm);\
  DST(op,i*32+18, (w8 >> 28) & 0x3fffffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*15+9)*8/sizeof(ip[0]));\
\
  DST(op,i*32+19, (w8 >> 58) | (w9 <<  6) & 0x3fffffff, parm);\
  DST(op,i*32+20, (w9 >> 24) & 0x3fffffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*15+10)*8/sizeof(ip[0]));\
\
  DST(op,i*32+21, (w9 >> 54) | (w10 << 10) & 0x3fffffff, parm);\
  DST(op,i*32+22, (w10 >> 20) & 0x3fffffff, parm);  register uint64_t w11 = *(uint64_t *)(ip+(i*15+11)*8/sizeof(ip[0]));\
\
  DST(op,i*32+23, (w10 >> 50) | (w11 << 14) & 0x3fffffff, parm);\
  DST(op,i*32+24, (w11 >> 16) & 0x3fffffff, parm);  register uint64_t w12 = *(uint64_t *)(ip+(i*15+12)*8/sizeof(ip[0]));\
\
  DST(op,i*32+25, (w11 >> 46) | (w12 << 18) & 0x3fffffff, parm);\
  DST(op,i*32+26, (w12 >> 12) & 0x3fffffff, parm);  register uint64_t w13 = *(uint64_t *)(ip+(i*15+13)*8/sizeof(ip[0]));\
\
  DST(op,i*32+27, (w12 >> 42) | (w13 << 22) & 0x3fffffff, parm);\
  DST(op,i*32+28, (w13 >>  8) & 0x3fffffff, parm);  register uint64_t w14 = *(uint64_t *)(ip+(i*15+14)*8/sizeof(ip[0]));\
\
  DST(op,i*32+29, (w13 >> 38) | (w14 << 26) & 0x3fffffff, parm);\
  DST(op,i*32+30, (w14 >>  4) & 0x3fffffff, parm);\
  DST(op,i*32+31, (w14 >> 34) , parm);;\
}

#define BITUNPACK64_30(ip,  op, parm) { \
  BITUNBLK64_30(ip, 0, op, parm);  DSTI(op); ip += 30*4/sizeof(ip[0]);\
}

#define BITUNBLK64_31(ip, i, op, parm) {   register uint64_t w0 = *(uint64_t *)(ip+(i*31+0)*8/sizeof(ip[0]));\
  DST(op,i*64+ 0, (w0 ) & 0x7fffffff, parm);\
  DST(op,i*64+ 1, (w0 >> 31) & 0x7fffffff, parm);  register uint64_t w1 = *(uint64_t *)(ip+(i*31+1)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 2, (w0 >> 62) | (w1 <<  2) & 0x7fffffff, parm);\
  DST(op,i*64+ 3, (w1 >> 29) & 0x7fffffff, parm);  register uint64_t w2 = *(uint64_t *)(ip+(i*31+2)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 4, (w1 >> 60) | (w2 <<  4) & 0x7fffffff, parm);\
  DST(op,i*64+ 5, (w2 >> 27) & 0x7fffffff, parm);  register uint64_t w3 = *(uint64_t *)(ip+(i*31+3)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 6, (w2 >> 58) | (w3 <<  6) & 0x7fffffff, parm);\
  DST(op,i*64+ 7, (w3 >> 25) & 0x7fffffff, parm);  register uint64_t w4 = *(uint64_t *)(ip+(i*31+4)*8/sizeof(ip[0]));\
\
  DST(op,i*64+ 8, (w3 >> 56) | (w4 <<  8) & 0x7fffffff, parm);\
  DST(op,i*64+ 9, (w4 >> 23) & 0x7fffffff, parm);  register uint64_t w5 = *(uint64_t *)(ip+(i*31+5)*8/sizeof(ip[0]));\
\
  DST(op,i*64+10, (w4 >> 54) | (w5 << 10) & 0x7fffffff, parm);\
  DST(op,i*64+11, (w5 >> 21) & 0x7fffffff, parm);  register uint64_t w6 = *(uint64_t *)(ip+(i*31+6)*8/sizeof(ip[0]));\
\
  DST(op,i*64+12, (w5 >> 52) | (w6 << 12) & 0x7fffffff, parm);\
  DST(op,i*64+13, (w6 >> 19) & 0x7fffffff, parm);  register uint64_t w7 = *(uint64_t *)(ip+(i*31+7)*8/sizeof(ip[0]));\
\
  DST(op,i*64+14, (w6 >> 50) | (w7 << 14) & 0x7fffffff, parm);\
  DST(op,i*64+15, (w7 >> 17) & 0x7fffffff, parm);  register uint64_t w8 = *(uint64_t *)(ip+(i*31+8)*8/sizeof(ip[0]));\
\
  DST(op,i*64+16, (w7 >> 48) | (w8 << 16) & 0x7fffffff, parm);\
  DST(op,i*64+17, (w8 >> 15) & 0x7fffffff, parm);  register uint64_t w9 = *(uint64_t *)(ip+(i*31+9)*8/sizeof(ip[0]));\
\
  DST(op,i*64+18, (w8 >> 46) | (w9 << 18) & 0x7fffffff, parm);\
  DST(op,i*64+19, (w9 >> 13) & 0x7fffffff, parm);  register uint64_t w10 = *(uint64_t *)(ip+(i*31+10)*8/sizeof(ip[0]));\
\
  DST(op,i*64+20, (w9 >> 44) | (w10 << 20) & 0x7fffffff, parm);\
  DST(op,i*64+21, (w10 >> 11) & 0x7fffffff, parm);  register uint64_t w11 = *(uint64_t *)(ip+(i*31+11)*8/sizeof(ip[0]));\
\
  DST(op,i*64+22, (w10 >> 42) | (w11 << 22) & 0x7fffffff, parm);\
  DST(op,i*64+23, (w11 >>  9) & 0x7fffffff, parm);  register uint64_t w12 = *(uint64_t *)(ip+(i*31+12)*8/sizeof(ip[0]));\
\
  DST(op,i*64+24, (w11 >> 40) | (w12 << 24) & 0x7fffffff, parm);\
  DST(op,i*64+25, (w12 >>  7) & 0x7fffffff, parm);  register uint64_t w13 = *(uint64_t *)(ip+(i*31+13)*8/sizeof(ip[0]));\
\
  DST(op,i*64+26, (w12 >> 38) | (w13 << 26) & 0x7fffffff, parm);\
  DST(op,i*64+27, (w13 >>  5) & 0x7fffffff, parm);  register uint64_t w14 = *(uint64_t *)(ip+(i*31+14)*8/sizeof(ip[0]));\
\
  DST(op,i*64+28, (w13 >> 36) | (w14 << 28) & 0x7fffffff, parm);\
  DST(op,i*64+29, (w14 >>  3) & 0x7fffffff, parm);  register uint32_t w15 = *(uint32_t *)(ip+(i*31+15)*8/sizeof(ip[0]));\
\
  DST(op,i*64+30, (w14 >> 34) | (w15 << 30) & 0x7fffffff, parm);\
  DST(op,i*64+31, (w15 >>  1) & 0x7fffffff, parm);;\
}

#define BITUNPACK64_31(ip,  op, parm) { \
  BITUNBLK64_31(ip, 0, op, parm);  DSTI(op); ip += 31*4/sizeof(ip[0]);\
}

#define BITUNBLK64_32(ip, i, op, parm) { \
  DST(op,i*2+ 0, *(uint32_t *)(ip+i*8+ 0), parm);\
  DST(op,i*2+ 1, *(uint32_t *)(ip+i*8+ 4), parm);;\
}

#define BITUNPACK64_32(ip,  op, parm) { \
  BITUNBLK64_32(ip, 0, op, parm);\
  BITUNBLK64_32(ip, 1, op, parm);\
  BITUNBLK64_32(ip, 2, op, parm);\
  BITUNBLK64_32(ip, 3, op, parm);\
  BITUNBLK64_32(ip, 4, op, parm);\
  BITUNBLK64_32(ip, 5, op, parm);\
  BITUNBLK64_32(ip, 6, op, parm);\
  BITUNBLK64_32(ip, 7, op, parm);\
  BITUNBLK64_32(ip, 8, op, parm);\
  BITUNBLK64_32(ip, 9, op, parm);\
  BITUNBLK64_32(ip, 10, op, parm);\
  BITUNBLK64_32(ip, 11, op, parm);\
  BITUNBLK64_32(ip, 12, op, parm);\
  BITUNBLK64_32(ip, 13, op, parm);\
  BITUNBLK64_32(ip, 14, op, parm);\
  BITUNBLK64_32(ip, 15, op, parm);  DSTI(op); ip += 32*4/sizeof(ip[0]);\
}

