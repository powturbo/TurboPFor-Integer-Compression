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

    bitpack64_.h - "Integer Compression" binary packing 
**/

#define BITBLK32_1(ip, i, op, parm) { ; register uint32_t w;;\
  IPPB(ip, i*32+ 0, parm); w  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); w |= (uint32_t)SRC(ip, i*32+ 1) <<  1;\
  IPPB(ip, i*32+ 2, parm); w |= (uint32_t)SRC(ip, i*32+ 2) <<  2;\
  IPPB(ip, i*32+ 3, parm); w |= (uint32_t)SRC(ip, i*32+ 3) <<  3;\
  IPPB(ip, i*32+ 4, parm); w |= (uint32_t)SRC(ip, i*32+ 4) <<  4;\
  IPPB(ip, i*32+ 5, parm); w |= (uint32_t)SRC(ip, i*32+ 5) <<  5;\
  IPPB(ip, i*32+ 6, parm); w |= (uint32_t)SRC(ip, i*32+ 6) <<  6;\
  IPPB(ip, i*32+ 7, parm); w |= (uint32_t)SRC(ip, i*32+ 7) <<  7;\
  IPPB(ip, i*32+ 8, parm); w |= (uint32_t)SRC(ip, i*32+ 8) <<  8;\
  IPPB(ip, i*32+ 9, parm); w |= (uint32_t)SRC(ip, i*32+ 9) <<  9;\
  IPPB(ip, i*32+10, parm); w |= (uint32_t)SRC(ip, i*32+10) << 10;\
  IPPB(ip, i*32+11, parm); w |= (uint32_t)SRC(ip, i*32+11) << 11;\
  IPPB(ip, i*32+12, parm); w |= (uint32_t)SRC(ip, i*32+12) << 12;\
  IPPB(ip, i*32+13, parm); w |= (uint32_t)SRC(ip, i*32+13) << 13;\
  IPPB(ip, i*32+14, parm); w |= (uint32_t)SRC(ip, i*32+14) << 14;\
  IPPB(ip, i*32+15, parm); w |= (uint32_t)SRC(ip, i*32+15) << 15;\
  IPPB(ip, i*32+16, parm); w |= (uint32_t)SRC(ip, i*32+16) << 16;\
  IPPB(ip, i*32+17, parm); w |= (uint32_t)SRC(ip, i*32+17) << 17;\
  IPPB(ip, i*32+18, parm); w |= (uint32_t)SRC(ip, i*32+18) << 18;\
  IPPB(ip, i*32+19, parm); w |= (uint32_t)SRC(ip, i*32+19) << 19;\
  IPPB(ip, i*32+20, parm); w |= (uint32_t)SRC(ip, i*32+20) << 20;\
  IPPB(ip, i*32+21, parm); w |= (uint32_t)SRC(ip, i*32+21) << 21;\
  IPPB(ip, i*32+22, parm); w |= (uint32_t)SRC(ip, i*32+22) << 22;\
  IPPB(ip, i*32+23, parm); w |= (uint32_t)SRC(ip, i*32+23) << 23;\
  IPPB(ip, i*32+24, parm); w |= (uint32_t)SRC(ip, i*32+24) << 24;\
  IPPB(ip, i*32+25, parm); w |= (uint32_t)SRC(ip, i*32+25) << 25;\
  IPPB(ip, i*32+26, parm); w |= (uint32_t)SRC(ip, i*32+26) << 26;\
  IPPB(ip, i*32+27, parm); w |= (uint32_t)SRC(ip, i*32+27) << 27;\
  IPPB(ip, i*32+28, parm); w |= (uint32_t)SRC(ip, i*32+28) << 28;\
  IPPB(ip, i*32+29, parm); w |= (uint32_t)SRC(ip, i*32+29) << 29;\
  IPPB(ip, i*32+30, parm); w |= (uint32_t)SRC(ip, i*32+30) << 30;\
  IPPB(ip, i*32+31, parm); w |= (uint32_t)SRC(ip, i*32+31) << 31;*((uint32_t *)op+i*1+ 0) = w;;\
}

#define BITPACK64_1(ip,  op, parm) { \
  BITBLK32_1(ip, 0, op, parm);  SRCI(ip); op += 1*4/sizeof(op[0]);\
}

#define BITBLK64_2(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*32+ 0, parm); w  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); w |= (uint32_t)SRC(ip, i*32+ 1) <<  2;\
  IPPB(ip, i*32+ 2, parm); w |= (uint32_t)SRC(ip, i*32+ 2) <<  4;\
  IPPB(ip, i*32+ 3, parm); w |= (uint32_t)SRC(ip, i*32+ 3) <<  6;\
  IPPB(ip, i*32+ 4, parm); w |= (uint32_t)SRC(ip, i*32+ 4) <<  8;\
  IPPB(ip, i*32+ 5, parm); w |= (uint32_t)SRC(ip, i*32+ 5) << 10;\
  IPPB(ip, i*32+ 6, parm); w |= (uint32_t)SRC(ip, i*32+ 6) << 12;\
  IPPB(ip, i*32+ 7, parm); w |= (uint32_t)SRC(ip, i*32+ 7) << 14;\
  IPPB(ip, i*32+ 8, parm); w |= (uint32_t)SRC(ip, i*32+ 8) << 16;\
  IPPB(ip, i*32+ 9, parm); w |= (uint32_t)SRC(ip, i*32+ 9) << 18;\
  IPPB(ip, i*32+10, parm); w |= (uint32_t)SRC(ip, i*32+10) << 20;\
  IPPB(ip, i*32+11, parm); w |= (uint32_t)SRC(ip, i*32+11) << 22;\
  IPPB(ip, i*32+12, parm); w |= (uint32_t)SRC(ip, i*32+12) << 24;\
  IPPB(ip, i*32+13, parm); w |= (uint32_t)SRC(ip, i*32+13) << 26;\
  IPPB(ip, i*32+14, parm); w |= (uint32_t)SRC(ip, i*32+14) << 28;\
  IPPB(ip, i*32+15, parm); w |= (uint32_t)SRC(ip, i*32+15) << 30;\
  IPPB(ip, i*32+16, parm); w |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); w |= (uint64_t)SRC(ip, i*32+17) << 34;\
  IPPB(ip, i*32+18, parm); w |= (uint64_t)SRC(ip, i*32+18) << 36;\
  IPPB(ip, i*32+19, parm); w |= (uint64_t)SRC(ip, i*32+19) << 38;\
  IPPB(ip, i*32+20, parm); w |= (uint64_t)SRC(ip, i*32+20) << 40;\
  IPPB(ip, i*32+21, parm); w |= (uint64_t)SRC(ip, i*32+21) << 42;\
  IPPB(ip, i*32+22, parm); w |= (uint64_t)SRC(ip, i*32+22) << 44;\
  IPPB(ip, i*32+23, parm); w |= (uint64_t)SRC(ip, i*32+23) << 46;\
  IPPB(ip, i*32+24, parm); w |= (uint64_t)SRC(ip, i*32+24) << 48;\
  IPPB(ip, i*32+25, parm); w |= (uint64_t)SRC(ip, i*32+25) << 50;\
  IPPB(ip, i*32+26, parm); w |= (uint64_t)SRC(ip, i*32+26) << 52;\
  IPPB(ip, i*32+27, parm); w |= (uint64_t)SRC(ip, i*32+27) << 54;\
  IPPB(ip, i*32+28, parm); w |= (uint64_t)SRC(ip, i*32+28) << 56;\
  IPPB(ip, i*32+29, parm); w |= (uint64_t)SRC(ip, i*32+29) << 58;\
  IPPB(ip, i*32+30, parm); w |= (uint64_t)SRC(ip, i*32+30) << 60;\
  IPPB(ip, i*32+31, parm); w |= (uint64_t)SRC(ip, i*32+31) << 62;*((uint64_t *)op+i*1+ 0) = w;;\
}

#define BITPACK64_2(ip,  op, parm) { \
  BITBLK64_2(ip, 0, op, parm);  SRCI(ip); op += 2*4/sizeof(op[0]);\
}

#define BITBLK64_3(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*64+ 0, parm); w  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); w |= (uint32_t)SRC(ip, i*64+ 1) <<  3;\
  IPPB(ip, i*64+ 2, parm); w |= (uint32_t)SRC(ip, i*64+ 2) <<  6;\
  IPPB(ip, i*64+ 3, parm); w |= (uint32_t)SRC(ip, i*64+ 3) <<  9;\
  IPPB(ip, i*64+ 4, parm); w |= (uint32_t)SRC(ip, i*64+ 4) << 12;\
  IPPB(ip, i*64+ 5, parm); w |= (uint32_t)SRC(ip, i*64+ 5) << 15;\
  IPPB(ip, i*64+ 6, parm); w |= (uint32_t)SRC(ip, i*64+ 6) << 18;\
  IPPB(ip, i*64+ 7, parm); w |= (uint32_t)SRC(ip, i*64+ 7) << 21;\
  IPPB(ip, i*64+ 8, parm); w |= (uint32_t)SRC(ip, i*64+ 8) << 24;\
  IPPB(ip, i*64+ 9, parm); w |= (uint32_t)SRC(ip, i*64+ 9) << 27;\
  IPPB(ip, i*64+10, parm); w |= (uint64_t)SRC(ip, i*64+10) << 30;\
  IPPB(ip, i*64+11, parm); w |= (uint64_t)SRC(ip, i*64+11) << 33;\
  IPPB(ip, i*64+12, parm); w |= (uint64_t)SRC(ip, i*64+12) << 36;\
  IPPB(ip, i*64+13, parm); w |= (uint64_t)SRC(ip, i*64+13) << 39;\
  IPPB(ip, i*64+14, parm); w |= (uint64_t)SRC(ip, i*64+14) << 42;\
  IPPB(ip, i*64+15, parm); w |= (uint64_t)SRC(ip, i*64+15) << 45;\
  IPPB(ip, i*64+16, parm); w |= (uint64_t)SRC(ip, i*64+16) << 48;\
  IPPB(ip, i*64+17, parm); w |= (uint64_t)SRC(ip, i*64+17) << 51;\
  IPPB(ip, i*64+18, parm); w |= (uint64_t)SRC(ip, i*64+18) << 54;\
  IPPB(ip, i*64+19, parm); w |= (uint64_t)SRC(ip, i*64+19) << 57;\
  IPPB(ip, i*64+20, parm); w |= (uint64_t)SRC(ip, i*64+20) << 60 | (uint64_t)SRC1(ip, i*64+21) << 63;*((uint64_t *)op+i*3+ 0) = w;\
  IPPB(ip, i*64+21, parm); w  = (uint32_t)SRC(ip, i*64+21) >>  1;\
  IPPB(ip, i*64+22, parm); w |= (uint32_t)SRC(ip, i*64+22) <<  2;\
  IPPB(ip, i*64+23, parm); w |= (uint32_t)SRC(ip, i*64+23) <<  5;\
  IPPB(ip, i*64+24, parm); w |= (uint32_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); w |= (uint32_t)SRC(ip, i*64+25) << 11;\
  IPPB(ip, i*64+26, parm); w |= (uint32_t)SRC(ip, i*64+26) << 14;\
  IPPB(ip, i*64+27, parm); w |= (uint32_t)SRC(ip, i*64+27) << 17;\
  IPPB(ip, i*64+28, parm); w |= (uint32_t)SRC(ip, i*64+28) << 20;\
  IPPB(ip, i*64+29, parm); w |= (uint32_t)SRC(ip, i*64+29) << 23;\
  IPPB(ip, i*64+30, parm); w |= (uint32_t)SRC(ip, i*64+30) << 26;\
  IPPB(ip, i*64+31, parm); w |= (uint32_t)SRC(ip, i*64+31) << 29;*((uint64_t *)op+i*3+ 1) = w;;\
}

#define BITPACK64_3(ip,  op, parm) { \
  BITBLK64_3(ip, 0, op, parm);  SRCI(ip); op += 3*4/sizeof(op[0]);\
}

#define BITBLK64_4(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*16+ 0, parm); w  = (uint32_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); w |= (uint32_t)SRC(ip, i*16+ 1) <<  4;\
  IPPB(ip, i*16+ 2, parm); w |= (uint32_t)SRC(ip, i*16+ 2) <<  8;\
  IPPB(ip, i*16+ 3, parm); w |= (uint32_t)SRC(ip, i*16+ 3) << 12;\
  IPPB(ip, i*16+ 4, parm); w |= (uint32_t)SRC(ip, i*16+ 4) << 16;\
  IPPB(ip, i*16+ 5, parm); w |= (uint32_t)SRC(ip, i*16+ 5) << 20;\
  IPPB(ip, i*16+ 6, parm); w |= (uint32_t)SRC(ip, i*16+ 6) << 24;\
  IPPB(ip, i*16+ 7, parm); w |= (uint32_t)SRC(ip, i*16+ 7) << 28;\
  IPPB(ip, i*16+ 8, parm); w |= (uint64_t)SRC(ip, i*16+ 8) << 32;\
  IPPB(ip, i*16+ 9, parm); w |= (uint64_t)SRC(ip, i*16+ 9) << 36;\
  IPPB(ip, i*16+10, parm); w |= (uint64_t)SRC(ip, i*16+10) << 40;\
  IPPB(ip, i*16+11, parm); w |= (uint64_t)SRC(ip, i*16+11) << 44;\
  IPPB(ip, i*16+12, parm); w |= (uint64_t)SRC(ip, i*16+12) << 48;\
  IPPB(ip, i*16+13, parm); w |= (uint64_t)SRC(ip, i*16+13) << 52;\
  IPPB(ip, i*16+14, parm); w |= (uint64_t)SRC(ip, i*16+14) << 56;\
  IPPB(ip, i*16+15, parm); w |= (uint64_t)SRC(ip, i*16+15) << 60;*((uint64_t *)op+i*1+ 0) = w;;\
}

#define BITPACK64_4(ip,  op, parm) { \
  BITBLK64_4(ip, 0, op, parm);\
  BITBLK64_4(ip, 1, op, parm);  SRCI(ip); op += 4*4/sizeof(op[0]);\
}

#define BITBLK64_5(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*64+ 0, parm); w  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); w |= (uint32_t)SRC(ip, i*64+ 1) <<  5;\
  IPPB(ip, i*64+ 2, parm); w |= (uint32_t)SRC(ip, i*64+ 2) << 10;\
  IPPB(ip, i*64+ 3, parm); w |= (uint32_t)SRC(ip, i*64+ 3) << 15;\
  IPPB(ip, i*64+ 4, parm); w |= (uint32_t)SRC(ip, i*64+ 4) << 20;\
  IPPB(ip, i*64+ 5, parm); w |= (uint32_t)SRC(ip, i*64+ 5) << 25;\
  IPPB(ip, i*64+ 6, parm); w |= (uint64_t)SRC(ip, i*64+ 6) << 30;\
  IPPB(ip, i*64+ 7, parm); w |= (uint64_t)SRC(ip, i*64+ 7) << 35;\
  IPPB(ip, i*64+ 8, parm); w |= (uint64_t)SRC(ip, i*64+ 8) << 40;\
  IPPB(ip, i*64+ 9, parm); w |= (uint64_t)SRC(ip, i*64+ 9) << 45;\
  IPPB(ip, i*64+10, parm); w |= (uint64_t)SRC(ip, i*64+10) << 50;\
  IPPB(ip, i*64+11, parm); w |= (uint64_t)SRC(ip, i*64+11) << 55 | (uint64_t)SRC1(ip, i*64+12) << 60;*((uint64_t *)op+i*5+ 0) = w;\
  IPPB(ip, i*64+12, parm); w  = (uint32_t)SRC(ip, i*64+12) >>  4;\
  IPPB(ip, i*64+13, parm); w |= (uint32_t)SRC(ip, i*64+13) <<  1;\
  IPPB(ip, i*64+14, parm); w |= (uint32_t)SRC(ip, i*64+14) <<  6;\
  IPPB(ip, i*64+15, parm); w |= (uint32_t)SRC(ip, i*64+15) << 11;\
  IPPB(ip, i*64+16, parm); w |= (uint32_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); w |= (uint32_t)SRC(ip, i*64+17) << 21;\
  IPPB(ip, i*64+18, parm); w |= (uint32_t)SRC(ip, i*64+18) << 26;\
  IPPB(ip, i*64+19, parm); w |= (uint64_t)SRC(ip, i*64+19) << 31;\
  IPPB(ip, i*64+20, parm); w |= (uint64_t)SRC(ip, i*64+20) << 36;\
  IPPB(ip, i*64+21, parm); w |= (uint64_t)SRC(ip, i*64+21) << 41;\
  IPPB(ip, i*64+22, parm); w |= (uint64_t)SRC(ip, i*64+22) << 46;\
  IPPB(ip, i*64+23, parm); w |= (uint64_t)SRC(ip, i*64+23) << 51;\
  IPPB(ip, i*64+24, parm); w |= (uint64_t)SRC(ip, i*64+24) << 56 | (uint64_t)SRC1(ip, i*64+25) << 61;*((uint64_t *)op+i*5+ 1) = w;\
  IPPB(ip, i*64+25, parm); w  = (uint32_t)SRC(ip, i*64+25) >>  3;\
  IPPB(ip, i*64+26, parm); w |= (uint32_t)SRC(ip, i*64+26) <<  2;\
  IPPB(ip, i*64+27, parm); w |= (uint32_t)SRC(ip, i*64+27) <<  7;\
  IPPB(ip, i*64+28, parm); w |= (uint32_t)SRC(ip, i*64+28) << 12;\
  IPPB(ip, i*64+29, parm); w |= (uint32_t)SRC(ip, i*64+29) << 17;\
  IPPB(ip, i*64+30, parm); w |= (uint32_t)SRC(ip, i*64+30) << 22;\
  IPPB(ip, i*64+31, parm); w |= (uint32_t)SRC(ip, i*64+31) << 27;*((uint64_t *)op+i*5+ 2) = w;;\
}

#define BITPACK64_5(ip,  op, parm) { \
  BITBLK64_5(ip, 0, op, parm);  SRCI(ip); op += 5*4/sizeof(op[0]);\
}

#define BITBLK64_6(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*32+ 0, parm); w  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); w |= (uint32_t)SRC(ip, i*32+ 1) <<  6;\
  IPPB(ip, i*32+ 2, parm); w |= (uint32_t)SRC(ip, i*32+ 2) << 12;\
  IPPB(ip, i*32+ 3, parm); w |= (uint32_t)SRC(ip, i*32+ 3) << 18;\
  IPPB(ip, i*32+ 4, parm); w |= (uint32_t)SRC(ip, i*32+ 4) << 24;\
  IPPB(ip, i*32+ 5, parm); w |= (uint64_t)SRC(ip, i*32+ 5) << 30;\
  IPPB(ip, i*32+ 6, parm); w |= (uint64_t)SRC(ip, i*32+ 6) << 36;\
  IPPB(ip, i*32+ 7, parm); w |= (uint64_t)SRC(ip, i*32+ 7) << 42;\
  IPPB(ip, i*32+ 8, parm); w |= (uint64_t)SRC(ip, i*32+ 8) << 48;\
  IPPB(ip, i*32+ 9, parm); w |= (uint64_t)SRC(ip, i*32+ 9) << 54 | (uint64_t)SRC1(ip, i*32+10) << 60;*((uint64_t *)op+i*3+ 0) = w;\
  IPPB(ip, i*32+10, parm); w  = (uint32_t)SRC(ip, i*32+10) >>  4;\
  IPPB(ip, i*32+11, parm); w |= (uint32_t)SRC(ip, i*32+11) <<  2;\
  IPPB(ip, i*32+12, parm); w |= (uint32_t)SRC(ip, i*32+12) <<  8;\
  IPPB(ip, i*32+13, parm); w |= (uint32_t)SRC(ip, i*32+13) << 14;\
  IPPB(ip, i*32+14, parm); w |= (uint32_t)SRC(ip, i*32+14) << 20;\
  IPPB(ip, i*32+15, parm); w |= (uint32_t)SRC(ip, i*32+15) << 26;\
  IPPB(ip, i*32+16, parm); w |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); w |= (uint64_t)SRC(ip, i*32+17) << 38;\
  IPPB(ip, i*32+18, parm); w |= (uint64_t)SRC(ip, i*32+18) << 44;\
  IPPB(ip, i*32+19, parm); w |= (uint64_t)SRC(ip, i*32+19) << 50;\
  IPPB(ip, i*32+20, parm); w |= (uint64_t)SRC(ip, i*32+20) << 56 | (uint64_t)SRC1(ip, i*32+21) << 62;*((uint64_t *)op+i*3+ 1) = w;\
  IPPB(ip, i*32+21, parm); w  = (uint32_t)SRC(ip, i*32+21) >>  2;\
  IPPB(ip, i*32+22, parm); w |= (uint32_t)SRC(ip, i*32+22) <<  4;\
  IPPB(ip, i*32+23, parm); w |= (uint32_t)SRC(ip, i*32+23) << 10;\
  IPPB(ip, i*32+24, parm); w |= (uint32_t)SRC(ip, i*32+24) << 16;\
  IPPB(ip, i*32+25, parm); w |= (uint32_t)SRC(ip, i*32+25) << 22;\
  IPPB(ip, i*32+26, parm); w |= (uint64_t)SRC(ip, i*32+26) << 28;\
  IPPB(ip, i*32+27, parm); w |= (uint64_t)SRC(ip, i*32+27) << 34;\
  IPPB(ip, i*32+28, parm); w |= (uint64_t)SRC(ip, i*32+28) << 40;\
  IPPB(ip, i*32+29, parm); w |= (uint64_t)SRC(ip, i*32+29) << 46;\
  IPPB(ip, i*32+30, parm); w |= (uint64_t)SRC(ip, i*32+30) << 52;\
  IPPB(ip, i*32+31, parm); w |= (uint64_t)SRC(ip, i*32+31) << 58;*((uint64_t *)op+i*3+ 2) = w;;\
}

#define BITPACK64_6(ip,  op, parm) { \
  BITBLK64_6(ip, 0, op, parm);  SRCI(ip); op += 6*4/sizeof(op[0]);\
}

#define BITBLK64_7(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*64+ 0, parm); w  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); w |= (uint32_t)SRC(ip, i*64+ 1) <<  7;\
  IPPB(ip, i*64+ 2, parm); w |= (uint32_t)SRC(ip, i*64+ 2) << 14;\
  IPPB(ip, i*64+ 3, parm); w |= (uint32_t)SRC(ip, i*64+ 3) << 21;\
  IPPB(ip, i*64+ 4, parm); w |= (uint64_t)SRC(ip, i*64+ 4) << 28;\
  IPPB(ip, i*64+ 5, parm); w |= (uint64_t)SRC(ip, i*64+ 5) << 35;\
  IPPB(ip, i*64+ 6, parm); w |= (uint64_t)SRC(ip, i*64+ 6) << 42;\
  IPPB(ip, i*64+ 7, parm); w |= (uint64_t)SRC(ip, i*64+ 7) << 49;\
  IPPB(ip, i*64+ 8, parm); w |= (uint64_t)SRC(ip, i*64+ 8) << 56 | (uint64_t)SRC1(ip, i*64+9) << 63;*((uint64_t *)op+i*7+ 0) = w;\
  IPPB(ip, i*64+ 9, parm); w  = (uint32_t)SRC(ip, i*64+ 9) >>  1;\
  IPPB(ip, i*64+10, parm); w |= (uint32_t)SRC(ip, i*64+10) <<  6;\
  IPPB(ip, i*64+11, parm); w |= (uint32_t)SRC(ip, i*64+11) << 13;\
  IPPB(ip, i*64+12, parm); w |= (uint32_t)SRC(ip, i*64+12) << 20;\
  IPPB(ip, i*64+13, parm); w |= (uint64_t)SRC(ip, i*64+13) << 27;\
  IPPB(ip, i*64+14, parm); w |= (uint64_t)SRC(ip, i*64+14) << 34;\
  IPPB(ip, i*64+15, parm); w |= (uint64_t)SRC(ip, i*64+15) << 41;\
  IPPB(ip, i*64+16, parm); w |= (uint64_t)SRC(ip, i*64+16) << 48;\
  IPPB(ip, i*64+17, parm); w |= (uint64_t)SRC(ip, i*64+17) << 55 | (uint64_t)SRC1(ip, i*64+18) << 62;*((uint64_t *)op+i*7+ 1) = w;\
  IPPB(ip, i*64+18, parm); w  = (uint32_t)SRC(ip, i*64+18) >>  2;\
  IPPB(ip, i*64+19, parm); w |= (uint32_t)SRC(ip, i*64+19) <<  5;\
  IPPB(ip, i*64+20, parm); w |= (uint32_t)SRC(ip, i*64+20) << 12;\
  IPPB(ip, i*64+21, parm); w |= (uint32_t)SRC(ip, i*64+21) << 19;\
  IPPB(ip, i*64+22, parm); w |= (uint64_t)SRC(ip, i*64+22) << 26;\
  IPPB(ip, i*64+23, parm); w |= (uint64_t)SRC(ip, i*64+23) << 33;\
  IPPB(ip, i*64+24, parm); w |= (uint64_t)SRC(ip, i*64+24) << 40;\
  IPPB(ip, i*64+25, parm); w |= (uint64_t)SRC(ip, i*64+25) << 47;\
  IPPB(ip, i*64+26, parm); w |= (uint64_t)SRC(ip, i*64+26) << 54 | (uint64_t)SRC1(ip, i*64+27) << 61;*((uint64_t *)op+i*7+ 2) = w;\
  IPPB(ip, i*64+27, parm); w  = (uint32_t)SRC(ip, i*64+27) >>  3;\
  IPPB(ip, i*64+28, parm); w |= (uint32_t)SRC(ip, i*64+28) <<  4;\
  IPPB(ip, i*64+29, parm); w |= (uint32_t)SRC(ip, i*64+29) << 11;\
  IPPB(ip, i*64+30, parm); w |= (uint32_t)SRC(ip, i*64+30) << 18;\
  IPPB(ip, i*64+31, parm); w |= (uint32_t)SRC(ip, i*64+31) << 25;*((uint64_t *)op+i*7+ 3) = w;;\
}

#define BITPACK64_7(ip,  op, parm) { \
  BITBLK64_7(ip, 0, op, parm);  SRCI(ip); op += 7*4/sizeof(op[0]);\
}

#define BITBLK64_8(ip, i, op, parm) { ;\
  IPPB(ip, i*8+ 0, parm); *((uint64_t *)op+i*1+ 0)  = (uint32_t)SRC(ip, i*8+ 0)      ;\
  IPPB(ip, i*8+ 1, parm); *((uint64_t *)op+i*1+ 0) |= (uint32_t)SRC(ip, i*8+ 1) <<  8;\
  IPPB(ip, i*8+ 2, parm); *((uint64_t *)op+i*1+ 0) |= (uint32_t)SRC(ip, i*8+ 2) << 16;\
  IPPB(ip, i*8+ 3, parm); *((uint64_t *)op+i*1+ 0) |= (uint32_t)SRC(ip, i*8+ 3) << 24;\
  IPPB(ip, i*8+ 4, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 4) << 32;\
  IPPB(ip, i*8+ 5, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 5) << 40;\
  IPPB(ip, i*8+ 6, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 6) << 48;\
  IPPB(ip, i*8+ 7, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 7) << 56;\
}

#define BITPACK64_8(ip,  op, parm) { \
  BITBLK64_8(ip, 0, op, parm);\
  BITBLK64_8(ip, 1, op, parm);\
  BITBLK64_8(ip, 2, op, parm);\
  BITBLK64_8(ip, 3, op, parm);  SRCI(ip); op += 8*4/sizeof(op[0]);\
}

#define BITBLK64_9(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*9+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*9+ 0) |= (uint32_t)SRC(ip, i*64+ 1) <<  9;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*9+ 0) |= (uint32_t)SRC(ip, i*64+ 2) << 18;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 27;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 4) << 36;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 5) << 45;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 6) << 54 | (uint64_t)SRC1(ip, i*64+7) << 63;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*9+ 1)  = (uint32_t)SRC(ip, i*64+ 7) >>  1;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*9+ 1) |= (uint32_t)SRC(ip, i*64+ 8) <<  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*9+ 1) |= (uint32_t)SRC(ip, i*64+ 9) << 17;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+10) << 26;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+11) << 35;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+12) << 44;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+13) << 53 | (uint64_t)SRC1(ip, i*64+14) << 62;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*9+ 2)  = (uint32_t)SRC(ip, i*64+14) >>  2;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*9+ 2) |= (uint32_t)SRC(ip, i*64+15) <<  7;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*9+ 2) |= (uint32_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+17) << 25;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+18) << 34;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+19) << 43;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+20) << 52 | (uint64_t)SRC1(ip, i*64+21) << 61;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*9+ 3)  = (uint32_t)SRC(ip, i*64+21) >>  3;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*9+ 3) |= (uint32_t)SRC(ip, i*64+22) <<  6;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*9+ 3) |= (uint32_t)SRC(ip, i*64+23) << 15;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+24) << 24;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+25) << 33;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+26) << 42;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+27) << 51 | (uint64_t)SRC1(ip, i*64+28) << 60;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*9+ 4)  = (uint32_t)SRC(ip, i*64+28) >>  4;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*9+ 4) |= (uint32_t)SRC(ip, i*64+29) <<  5;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*9+ 4) |= (uint32_t)SRC(ip, i*64+30) << 14;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*9+ 4) |= (uint32_t)SRC(ip, i*64+31) << 23;\
}

#define BITPACK64_9(ip,  op, parm) { \
  BITBLK64_9(ip, 0, op, parm);  SRCI(ip); op += 9*4/sizeof(op[0]);\
}

#define BITBLK64_10(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*5+ 0)  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*5+ 0) |= (uint32_t)SRC(ip, i*32+ 1) << 10;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*5+ 0) |= (uint32_t)SRC(ip, i*32+ 2) << 20;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 3) << 30;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 4) << 40;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 5) << 50 | (uint64_t)SRC1(ip, i*32+6) << 60;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*5+ 1)  = (uint32_t)SRC(ip, i*32+ 6) >>  4;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*5+ 1) |= (uint32_t)SRC(ip, i*32+ 7) <<  6;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*5+ 1) |= (uint32_t)SRC(ip, i*32+ 8) << 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+ 9) << 26;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+10) << 36;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+11) << 46 | (uint64_t)SRC1(ip, i*32+12) << 56;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*5+ 2)  = (uint32_t)SRC(ip, i*32+12) >>  8;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*5+ 2) |= (uint32_t)SRC(ip, i*32+13) <<  2;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*5+ 2) |= (uint32_t)SRC(ip, i*32+14) << 12;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*5+ 2) |= (uint32_t)SRC(ip, i*32+15) << 22;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+17) << 42;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+18) << 52 | (uint64_t)SRC1(ip, i*32+19) << 62;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*5+ 3)  = (uint32_t)SRC(ip, i*32+19) >>  2;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*5+ 3) |= (uint32_t)SRC(ip, i*32+20) <<  8;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*5+ 3) |= (uint32_t)SRC(ip, i*32+21) << 18;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+22) << 28;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+23) << 38;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+24) << 48 | (uint64_t)SRC1(ip, i*32+25) << 58;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*5+ 4)  = (uint32_t)SRC(ip, i*32+25) >>  6;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*5+ 4) |= (uint32_t)SRC(ip, i*32+26) <<  4;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*5+ 4) |= (uint32_t)SRC(ip, i*32+27) << 14;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+28) << 24;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+29) << 34;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+30) << 44;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+31) << 54;\
}

#define BITPACK64_10(ip,  op, parm) { \
  BITBLK64_10(ip, 0, op, parm);  SRCI(ip); op += 10*4/sizeof(op[0]);\
}

#define BITBLK64_11(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*11+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*11+ 0) |= (uint32_t)SRC(ip, i*64+ 1) << 11;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 22;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 33;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*64+ 4) << 44 | (uint64_t)SRC1(ip, i*64+5) << 55;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*11+ 1)  = (uint32_t)SRC(ip, i*64+ 5) >>  9;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*11+ 1) |= (uint32_t)SRC(ip, i*64+ 6) <<  2;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*11+ 1) |= (uint32_t)SRC(ip, i*64+ 7) << 13;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+ 8) << 24;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+ 9) << 35;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+10) << 46 | (uint64_t)SRC1(ip, i*64+11) << 57;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*11+ 2)  = (uint32_t)SRC(ip, i*64+11) >>  7;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*11+ 2) |= (uint32_t)SRC(ip, i*64+12) <<  4;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*11+ 2) |= (uint32_t)SRC(ip, i*64+13) << 15;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+14) << 26;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+15) << 37;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+16) << 48 | (uint64_t)SRC1(ip, i*64+17) << 59;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*11+ 3)  = (uint32_t)SRC(ip, i*64+17) >>  5;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*11+ 3) |= (uint32_t)SRC(ip, i*64+18) <<  6;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*11+ 3) |= (uint32_t)SRC(ip, i*64+19) << 17;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+20) << 28;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+21) << 39;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+22) << 50 | (uint64_t)SRC1(ip, i*64+23) << 61;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*11+ 4)  = (uint32_t)SRC(ip, i*64+23) >>  3;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*11+ 4) |= (uint32_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*11+ 4) |= (uint32_t)SRC(ip, i*64+25) << 19;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+26) << 30;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+27) << 41;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+28) << 52 | (uint64_t)SRC1(ip, i*64+29) << 63;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*11+ 5)  = (uint32_t)SRC(ip, i*64+29) >>  1;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*11+ 5) |= (uint32_t)SRC(ip, i*64+30) << 10;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*11+ 5) |= (uint32_t)SRC(ip, i*64+31) << 21;\
}

#define BITPACK64_11(ip,  op, parm) { \
  BITBLK64_11(ip, 0, op, parm);  SRCI(ip); op += 11*4/sizeof(op[0]);\
}

#define BITBLK64_12(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*3+ 0)  = (uint32_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*3+ 0) |= (uint32_t)SRC(ip, i*16+ 1) << 12;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*16+ 2) << 24;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*16+ 3) << 36;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*16+ 4) << 48 | (uint64_t)SRC1(ip, i*16+5) << 60;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*3+ 1)  = (uint32_t)SRC(ip, i*16+ 5) >>  4;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*3+ 1) |= (uint32_t)SRC(ip, i*16+ 6) <<  8;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*3+ 1) |= (uint32_t)SRC(ip, i*16+ 7) << 20;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*16+ 8) << 32;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*16+ 9) << 44 | (uint64_t)SRC1(ip, i*16+10) << 56;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*3+ 2)  = (uint32_t)SRC(ip, i*16+10) >>  8;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*3+ 2) |= (uint32_t)SRC(ip, i*16+11) <<  4;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*3+ 2) |= (uint32_t)SRC(ip, i*16+12) << 16;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+13) << 28;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+14) << 40;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+15) << 52;\
}

#define BITPACK64_12(ip,  op, parm) { \
  BITBLK64_12(ip, 0, op, parm);\
  BITBLK64_12(ip, 1, op, parm);  SRCI(ip); op += 12*4/sizeof(op[0]);\
}

#define BITBLK64_13(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*13+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*13+ 0) |= (uint32_t)SRC(ip, i*64+ 1) << 13;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*13+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 26;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*13+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 39 | (uint64_t)SRC1(ip, i*64+4) << 52;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*13+ 1)  = (uint32_t)SRC(ip, i*64+ 4) >> 12;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*13+ 1) |= (uint32_t)SRC(ip, i*64+ 5) <<  1;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*13+ 1) |= (uint32_t)SRC(ip, i*64+ 6) << 14;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*64+ 7) << 27;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*64+ 8) << 40 | (uint64_t)SRC1(ip, i*64+9) << 53;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*13+ 2)  = (uint32_t)SRC(ip, i*64+ 9) >> 11;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*13+ 2) |= (uint32_t)SRC(ip, i*64+10) <<  2;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*13+ 2) |= (uint32_t)SRC(ip, i*64+11) << 15;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*64+12) << 28;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*64+13) << 41 | (uint64_t)SRC1(ip, i*64+14) << 54;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*13+ 3)  = (uint32_t)SRC(ip, i*64+14) >> 10;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*13+ 3) |= (uint32_t)SRC(ip, i*64+15) <<  3;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*13+ 3) |= (uint32_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*64+17) << 29;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*64+18) << 42 | (uint64_t)SRC1(ip, i*64+19) << 55;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*13+ 4)  = (uint32_t)SRC(ip, i*64+19) >>  9;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*13+ 4) |= (uint32_t)SRC(ip, i*64+20) <<  4;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*13+ 4) |= (uint32_t)SRC(ip, i*64+21) << 17;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*64+22) << 30;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*64+23) << 43 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*13+ 5)  = (uint32_t)SRC(ip, i*64+24) >>  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*13+ 5) |= (uint32_t)SRC(ip, i*64+25) <<  5;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*13+ 5) |= (uint32_t)SRC(ip, i*64+26) << 18;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*64+27) << 31;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*64+28) << 44 | (uint64_t)SRC1(ip, i*64+29) << 57;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*13+ 6)  = (uint32_t)SRC(ip, i*64+29) >>  7;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*13+ 6) |= (uint32_t)SRC(ip, i*64+30) <<  6;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*13+ 6) |= (uint32_t)SRC(ip, i*64+31) << 19;\
}

#define BITPACK64_13(ip,  op, parm) { \
  BITBLK64_13(ip, 0, op, parm);  SRCI(ip); op += 13*4/sizeof(op[0]);\
}

#define BITBLK64_14(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*7+ 0)  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*7+ 0) |= (uint32_t)SRC(ip, i*32+ 1) << 14;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*7+ 0) |= (uint64_t)SRC(ip, i*32+ 2) << 28;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*7+ 0) |= (uint64_t)SRC(ip, i*32+ 3) << 42 | (uint64_t)SRC1(ip, i*32+4) << 56;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*7+ 1)  = (uint32_t)SRC(ip, i*32+ 4) >>  8;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*7+ 1) |= (uint32_t)SRC(ip, i*32+ 5) <<  6;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*32+ 6) << 20;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*32+ 7) << 34;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*32+ 8) << 48 | (uint64_t)SRC1(ip, i*32+9) << 62;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*7+ 2)  = (uint32_t)SRC(ip, i*32+ 9) >>  2;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*7+ 2) |= (uint32_t)SRC(ip, i*32+10) << 12;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*7+ 2) |= (uint64_t)SRC(ip, i*32+11) << 26;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*7+ 2) |= (uint64_t)SRC(ip, i*32+12) << 40 | (uint64_t)SRC1(ip, i*32+13) << 54;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*7+ 3)  = (uint32_t)SRC(ip, i*32+13) >> 10;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*7+ 3) |= (uint32_t)SRC(ip, i*32+14) <<  4;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*7+ 3) |= (uint32_t)SRC(ip, i*32+15) << 18;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*32+17) << 46 | (uint64_t)SRC1(ip, i*32+18) << 60;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*7+ 4)  = (uint32_t)SRC(ip, i*32+18) >>  4;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*7+ 4) |= (uint32_t)SRC(ip, i*32+19) << 10;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*7+ 4) |= (uint64_t)SRC(ip, i*32+20) << 24;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*7+ 4) |= (uint64_t)SRC(ip, i*32+21) << 38 | (uint64_t)SRC1(ip, i*32+22) << 52;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*7+ 5)  = (uint32_t)SRC(ip, i*32+22) >> 12;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*7+ 5) |= (uint32_t)SRC(ip, i*32+23) <<  2;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*7+ 5) |= (uint32_t)SRC(ip, i*32+24) << 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*32+25) << 30;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*32+26) << 44 | (uint64_t)SRC1(ip, i*32+27) << 58;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*7+ 6)  = (uint32_t)SRC(ip, i*32+27) >>  6;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*7+ 6) |= (uint32_t)SRC(ip, i*32+28) <<  8;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*32+29) << 22;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*32+30) << 36;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*32+31) << 50;\
}

#define BITPACK64_14(ip,  op, parm) { \
  BITBLK64_14(ip, 0, op, parm);  SRCI(ip); op += 14*4/sizeof(op[0]);\
}

#define BITBLK64_15(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*15+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*15+ 0) |= (uint32_t)SRC(ip, i*64+ 1) << 15;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*15+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 30;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*15+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 45 | (uint64_t)SRC1(ip, i*64+4) << 60;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*15+ 1)  = (uint32_t)SRC(ip, i*64+ 4) >>  4;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*15+ 1) |= (uint32_t)SRC(ip, i*64+ 5) << 11;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*15+ 1) |= (uint64_t)SRC(ip, i*64+ 6) << 26;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*15+ 1) |= (uint64_t)SRC(ip, i*64+ 7) << 41 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*15+ 2)  = (uint32_t)SRC(ip, i*64+ 8) >>  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*15+ 2) |= (uint32_t)SRC(ip, i*64+ 9) <<  7;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*15+ 2) |= (uint64_t)SRC(ip, i*64+10) << 22;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*15+ 2) |= (uint64_t)SRC(ip, i*64+11) << 37 | (uint64_t)SRC1(ip, i*64+12) << 52;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*15+ 3)  = (uint32_t)SRC(ip, i*64+12) >> 12;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*15+ 3) |= (uint32_t)SRC(ip, i*64+13) <<  3;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*64+14) << 18;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*64+15) << 33;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*64+16) << 48 | (uint64_t)SRC1(ip, i*64+17) << 63;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*15+ 4)  = (uint32_t)SRC(ip, i*64+17) >>  1;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*15+ 4) |= (uint32_t)SRC(ip, i*64+18) << 14;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*15+ 4) |= (uint64_t)SRC(ip, i*64+19) << 29;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*15+ 4) |= (uint64_t)SRC(ip, i*64+20) << 44 | (uint64_t)SRC1(ip, i*64+21) << 59;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*15+ 5)  = (uint32_t)SRC(ip, i*64+21) >>  5;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*15+ 5) |= (uint32_t)SRC(ip, i*64+22) << 10;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*15+ 5) |= (uint64_t)SRC(ip, i*64+23) << 25;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*15+ 5) |= (uint64_t)SRC(ip, i*64+24) << 40 | (uint64_t)SRC1(ip, i*64+25) << 55;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*15+ 6)  = (uint32_t)SRC(ip, i*64+25) >>  9;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*15+ 6) |= (uint32_t)SRC(ip, i*64+26) <<  6;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*15+ 6) |= (uint64_t)SRC(ip, i*64+27) << 21;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*15+ 6) |= (uint64_t)SRC(ip, i*64+28) << 36 | (uint64_t)SRC1(ip, i*64+29) << 51;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*15+ 7)  = (uint32_t)SRC(ip, i*64+29) >> 13;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*15+ 7) |= (uint32_t)SRC(ip, i*64+30) <<  2;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*15+ 7) |= (uint32_t)SRC(ip, i*64+31) << 17;\
}

#define BITPACK64_15(ip,  op, parm) { \
  BITBLK64_15(ip, 0, op, parm);  SRCI(ip); op += 15*4/sizeof(op[0]);\
}

#define BITBLK64_16(ip, i, op, parm) { \
  IPPB(ip, i*4+ 0, parm); *(uint16_t *)(op+i*8+ 0) = SRC(ip, i*4+ 0);\
  IPPB(ip, i*4+ 1, parm); *(uint16_t *)(op+i*8+ 2) = SRC(ip, i*4+ 1);\
  IPPB(ip, i*4+ 2, parm); *(uint16_t *)(op+i*8+ 4) = SRC(ip, i*4+ 2);\
  IPPB(ip, i*4+ 3, parm); *(uint16_t *)(op+i*8+ 6) = SRC(ip, i*4+ 3);;\
}

#define BITPACK64_16(ip,  op, parm) { \
  BITBLK64_16(ip, 0, op, parm);\
  BITBLK64_16(ip, 1, op, parm);\
  BITBLK64_16(ip, 2, op, parm);\
  BITBLK64_16(ip, 3, op, parm);\
  BITBLK64_16(ip, 4, op, parm);\
  BITBLK64_16(ip, 5, op, parm);\
  BITBLK64_16(ip, 6, op, parm);\
  BITBLK64_16(ip, 7, op, parm);  SRCI(ip); op += 16*4/sizeof(op[0]);\
}

#define BITBLK64_17(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*17+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*17+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 17;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*17+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 34 | (uint64_t)SRC1(ip, i*64+3) << 51;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*17+ 1)  = (uint32_t)SRC(ip, i*64+ 3) >> 13;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*17+ 1) |= (uint32_t)SRC(ip, i*64+ 4) <<  4;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*17+ 1) |= (uint64_t)SRC(ip, i*64+ 5) << 21;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*17+ 1) |= (uint64_t)SRC(ip, i*64+ 6) << 38 | (uint64_t)SRC1(ip, i*64+7) << 55;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*17+ 2)  = (uint32_t)SRC(ip, i*64+ 7) >>  9;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*17+ 2) |= (uint32_t)SRC(ip, i*64+ 8) <<  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*17+ 2) |= (uint64_t)SRC(ip, i*64+ 9) << 25;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*17+ 2) |= (uint64_t)SRC(ip, i*64+10) << 42 | (uint64_t)SRC1(ip, i*64+11) << 59;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*17+ 3)  = (uint32_t)SRC(ip, i*64+11) >>  5;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*17+ 3) |= (uint32_t)SRC(ip, i*64+12) << 12;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*17+ 3) |= (uint64_t)SRC(ip, i*64+13) << 29;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*17+ 3) |= (uint64_t)SRC(ip, i*64+14) << 46 | (uint64_t)SRC1(ip, i*64+15) << 63;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*17+ 4)  = (uint32_t)SRC(ip, i*64+15) >>  1;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*17+ 4) |= (uint64_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*17+ 4) |= (uint64_t)SRC(ip, i*64+17) << 33 | (uint64_t)SRC1(ip, i*64+18) << 50;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*17+ 5)  = (uint32_t)SRC(ip, i*64+18) >> 14;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*17+ 5) |= (uint32_t)SRC(ip, i*64+19) <<  3;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*17+ 5) |= (uint64_t)SRC(ip, i*64+20) << 20;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*17+ 5) |= (uint64_t)SRC(ip, i*64+21) << 37 | (uint64_t)SRC1(ip, i*64+22) << 54;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*17+ 6)  = (uint32_t)SRC(ip, i*64+22) >> 10;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*17+ 6) |= (uint32_t)SRC(ip, i*64+23) <<  7;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*17+ 6) |= (uint64_t)SRC(ip, i*64+24) << 24;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*17+ 6) |= (uint64_t)SRC(ip, i*64+25) << 41 | (uint64_t)SRC1(ip, i*64+26) << 58;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*17+ 7)  = (uint32_t)SRC(ip, i*64+26) >>  6;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*17+ 7) |= (uint32_t)SRC(ip, i*64+27) << 11;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*17+ 7) |= (uint64_t)SRC(ip, i*64+28) << 28;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*17+ 7) |= (uint64_t)SRC(ip, i*64+29) << 45 | (uint64_t)SRC1(ip, i*64+30) << 62;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*17+ 8)  = (uint32_t)SRC(ip, i*64+30) >>  2;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*17+ 8) |= (uint32_t)SRC(ip, i*64+31) << 15;\
}

#define BITPACK64_17(ip,  op, parm) { \
  BITBLK64_17(ip, 0, op, parm);  SRCI(ip); op += 17*4/sizeof(op[0]);\
}

#define BITBLK64_18(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*9+ 0)  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 18;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*32+ 2) << 36 | (uint64_t)SRC1(ip, i*32+3) << 54;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*9+ 1)  = (uint32_t)SRC(ip, i*32+ 3) >> 10;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*9+ 1) |= (uint32_t)SRC(ip, i*32+ 4) <<  8;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*32+ 5) << 26;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*32+ 6) << 44 | (uint64_t)SRC1(ip, i*32+7) << 62;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*9+ 2)  = (uint32_t)SRC(ip, i*32+ 7) >>  2;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*32+ 8) << 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*32+ 9) << 34 | (uint64_t)SRC1(ip, i*32+10) << 52;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*9+ 3)  = (uint32_t)SRC(ip, i*32+10) >> 12;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*9+ 3) |= (uint32_t)SRC(ip, i*32+11) <<  6;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*32+12) << 24;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*32+13) << 42 | (uint64_t)SRC1(ip, i*32+14) << 60;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*9+ 4)  = (uint32_t)SRC(ip, i*32+14) >>  4;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*9+ 4) |= (uint32_t)SRC(ip, i*32+15) << 14;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*9+ 4) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 50;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*9+ 5)  = (uint32_t)SRC(ip, i*32+17) >> 14;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*9+ 5) |= (uint32_t)SRC(ip, i*32+18) <<  4;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*9+ 5) |= (uint64_t)SRC(ip, i*32+19) << 22;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*9+ 5) |= (uint64_t)SRC(ip, i*32+20) << 40 | (uint64_t)SRC1(ip, i*32+21) << 58;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*9+ 6)  = (uint32_t)SRC(ip, i*32+21) >>  6;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*9+ 6) |= (uint32_t)SRC(ip, i*32+22) << 12;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*9+ 6) |= (uint64_t)SRC(ip, i*32+23) << 30 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*9+ 7)  = (uint32_t)SRC(ip, i*32+24) >> 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*9+ 7) |= (uint32_t)SRC(ip, i*32+25) <<  2;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*9+ 7) |= (uint64_t)SRC(ip, i*32+26) << 20;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*9+ 7) |= (uint64_t)SRC(ip, i*32+27) << 38 | (uint64_t)SRC1(ip, i*32+28) << 56;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*9+ 8)  = (uint32_t)SRC(ip, i*32+28) >>  8;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*9+ 8) |= (uint32_t)SRC(ip, i*32+29) << 10;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*9+ 8) |= (uint64_t)SRC(ip, i*32+30) << 28;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*9+ 8) |= (uint64_t)SRC(ip, i*32+31) << 46;\
}

#define BITPACK64_18(ip,  op, parm) { \
  BITBLK64_18(ip, 0, op, parm);  SRCI(ip); op += 18*4/sizeof(op[0]);\
}

#define BITBLK64_19(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*19+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*19+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 19;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*19+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 38 | (uint64_t)SRC1(ip, i*64+3) << 57;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*19+ 1)  = (uint32_t)SRC(ip, i*64+ 3) >>  7;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*19+ 1) |= (uint32_t)SRC(ip, i*64+ 4) << 12;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*19+ 1) |= (uint64_t)SRC(ip, i*64+ 5) << 31 | (uint64_t)SRC1(ip, i*64+6) << 50;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*19+ 2)  = (uint32_t)SRC(ip, i*64+ 6) >> 14;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*19+ 2) |= (uint32_t)SRC(ip, i*64+ 7) <<  5;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*19+ 2) |= (uint64_t)SRC(ip, i*64+ 8) << 24;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*19+ 2) |= (uint64_t)SRC(ip, i*64+ 9) << 43 | (uint64_t)SRC1(ip, i*64+10) << 62;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*19+ 3)  = (uint32_t)SRC(ip, i*64+10) >>  2;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*19+ 3) |= (uint64_t)SRC(ip, i*64+11) << 17;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*19+ 3) |= (uint64_t)SRC(ip, i*64+12) << 36 | (uint64_t)SRC1(ip, i*64+13) << 55;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*19+ 4)  = (uint32_t)SRC(ip, i*64+13) >>  9;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*19+ 4) |= (uint32_t)SRC(ip, i*64+14) << 10;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*19+ 4) |= (uint64_t)SRC(ip, i*64+15) << 29 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*19+ 5)  = (uint32_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*19+ 5) |= (uint32_t)SRC(ip, i*64+17) <<  3;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*19+ 5) |= (uint64_t)SRC(ip, i*64+18) << 22;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*19+ 5) |= (uint64_t)SRC(ip, i*64+19) << 41 | (uint64_t)SRC1(ip, i*64+20) << 60;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*19+ 6)  = (uint32_t)SRC(ip, i*64+20) >>  4;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*19+ 6) |= (uint64_t)SRC(ip, i*64+21) << 15;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*19+ 6) |= (uint64_t)SRC(ip, i*64+22) << 34 | (uint64_t)SRC1(ip, i*64+23) << 53;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*19+ 7)  = (uint32_t)SRC(ip, i*64+23) >> 11;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*19+ 7) |= (uint32_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*19+ 7) |= (uint64_t)SRC(ip, i*64+25) << 27 | (uint64_t)SRC1(ip, i*64+26) << 46;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*19+ 8)  = (uint32_t)SRC(ip, i*64+26) >> 18;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*19+ 8) |= (uint32_t)SRC(ip, i*64+27) <<  1;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*19+ 8) |= (uint64_t)SRC(ip, i*64+28) << 20;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*19+ 8) |= (uint64_t)SRC(ip, i*64+29) << 39 | (uint64_t)SRC1(ip, i*64+30) << 58;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*19+ 9)  = (uint32_t)SRC(ip, i*64+30) >>  6;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*19+ 9) |= (uint32_t)SRC(ip, i*64+31) << 13;\
}

#define BITPACK64_19(ip,  op, parm) { \
  BITBLK64_19(ip, 0, op, parm);  SRCI(ip); op += 19*4/sizeof(op[0]);\
}

#define BITBLK64_20(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*5+ 0)  = (uint32_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*16+ 1) << 20;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*16+ 2) << 40 | (uint64_t)SRC1(ip, i*16+3) << 60;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*5+ 1)  = (uint32_t)SRC(ip, i*16+ 3) >>  4;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*16+ 4) << 16;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*16+ 5) << 36 | (uint64_t)SRC1(ip, i*16+6) << 56;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*5+ 2)  = (uint32_t)SRC(ip, i*16+ 6) >>  8;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*5+ 2) |= (uint32_t)SRC(ip, i*16+ 7) << 12;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*16+ 8) << 32 | (uint64_t)SRC1(ip, i*16+9) << 52;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*5+ 3)  = (uint32_t)SRC(ip, i*16+ 9) >> 12;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*5+ 3) |= (uint32_t)SRC(ip, i*16+10) <<  8;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*16+11) << 28 | (uint64_t)SRC1(ip, i*16+12) << 48;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*5+ 4)  = (uint32_t)SRC(ip, i*16+12) >> 16;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*5+ 4) |= (uint32_t)SRC(ip, i*16+13) <<  4;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*16+14) << 24;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*16+15) << 44;\
}

#define BITPACK64_20(ip,  op, parm) { \
  BITBLK64_20(ip, 0, op, parm);\
  BITBLK64_20(ip, 1, op, parm);  SRCI(ip); op += 20*4/sizeof(op[0]);\
}

#define BITBLK64_21(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*21+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*21+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 21;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*21+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 42 | (uint64_t)SRC1(ip, i*64+3) << 63;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*21+ 1)  = (uint32_t)SRC(ip, i*64+ 3) >>  1;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*21+ 1) |= (uint64_t)SRC(ip, i*64+ 4) << 20;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*21+ 1) |= (uint64_t)SRC(ip, i*64+ 5) << 41 | (uint64_t)SRC1(ip, i*64+6) << 62;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*21+ 2)  = (uint32_t)SRC(ip, i*64+ 6) >>  2;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*21+ 2) |= (uint64_t)SRC(ip, i*64+ 7) << 19;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*21+ 2) |= (uint64_t)SRC(ip, i*64+ 8) << 40 | (uint64_t)SRC1(ip, i*64+9) << 61;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*21+ 3)  = (uint32_t)SRC(ip, i*64+ 9) >>  3;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*21+ 3) |= (uint64_t)SRC(ip, i*64+10) << 18;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*21+ 3) |= (uint64_t)SRC(ip, i*64+11) << 39 | (uint64_t)SRC1(ip, i*64+12) << 60;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*21+ 4)  = (uint32_t)SRC(ip, i*64+12) >>  4;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*21+ 4) |= (uint64_t)SRC(ip, i*64+13) << 17;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*21+ 4) |= (uint64_t)SRC(ip, i*64+14) << 38 | (uint64_t)SRC1(ip, i*64+15) << 59;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*21+ 5)  = (uint32_t)SRC(ip, i*64+15) >>  5;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*21+ 5) |= (uint64_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*21+ 5) |= (uint64_t)SRC(ip, i*64+17) << 37 | (uint64_t)SRC1(ip, i*64+18) << 58;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*21+ 6)  = (uint32_t)SRC(ip, i*64+18) >>  6;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*21+ 6) |= (uint64_t)SRC(ip, i*64+19) << 15;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*21+ 6) |= (uint64_t)SRC(ip, i*64+20) << 36 | (uint64_t)SRC1(ip, i*64+21) << 57;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*21+ 7)  = (uint32_t)SRC(ip, i*64+21) >>  7;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*21+ 7) |= (uint64_t)SRC(ip, i*64+22) << 14;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*21+ 7) |= (uint64_t)SRC(ip, i*64+23) << 35 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*21+ 8)  = (uint32_t)SRC(ip, i*64+24) >>  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*21+ 8) |= (uint64_t)SRC(ip, i*64+25) << 13;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*21+ 8) |= (uint64_t)SRC(ip, i*64+26) << 34 | (uint64_t)SRC1(ip, i*64+27) << 55;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*21+ 9)  = (uint32_t)SRC(ip, i*64+27) >>  9;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*21+ 9) |= (uint64_t)SRC(ip, i*64+28) << 12;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*21+ 9) |= (uint64_t)SRC(ip, i*64+29) << 33 | (uint64_t)SRC1(ip, i*64+30) << 54;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*21+10)  = (uint32_t)SRC(ip, i*64+30) >> 10;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*21+10) |= (uint32_t)SRC(ip, i*64+31) << 11;\
}

#define BITPACK64_21(ip,  op, parm) { \
  BITBLK64_21(ip, 0, op, parm);  SRCI(ip); op += 21*4/sizeof(op[0]);\
}

#define BITBLK64_22(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*11+ 0)  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 22 | (uint64_t)SRC1(ip, i*32+2) << 44;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*11+ 1)  = (uint32_t)SRC(ip, i*32+ 2) >> 20;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*11+ 1) |= (uint32_t)SRC(ip, i*32+ 3) <<  2;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*32+ 4) << 24 | (uint64_t)SRC1(ip, i*32+5) << 46;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*11+ 2)  = (uint32_t)SRC(ip, i*32+ 5) >> 18;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*11+ 2) |= (uint32_t)SRC(ip, i*32+ 6) <<  4;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*32+ 7) << 26 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*11+ 3)  = (uint32_t)SRC(ip, i*32+ 8) >> 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*11+ 3) |= (uint32_t)SRC(ip, i*32+ 9) <<  6;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*32+10) << 28 | (uint64_t)SRC1(ip, i*32+11) << 50;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*11+ 4)  = (uint32_t)SRC(ip, i*32+11) >> 14;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*11+ 4) |= (uint32_t)SRC(ip, i*32+12) <<  8;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*32+13) << 30 | (uint64_t)SRC1(ip, i*32+14) << 52;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*11+ 5)  = (uint32_t)SRC(ip, i*32+14) >> 12;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*11+ 5) |= (uint32_t)SRC(ip, i*32+15) << 10;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*11+ 5) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 54;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*11+ 6)  = (uint32_t)SRC(ip, i*32+17) >> 10;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*11+ 6) |= (uint64_t)SRC(ip, i*32+18) << 12;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*11+ 6) |= (uint64_t)SRC(ip, i*32+19) << 34 | (uint64_t)SRC1(ip, i*32+20) << 56;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*11+ 7)  = (uint32_t)SRC(ip, i*32+20) >>  8;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*11+ 7) |= (uint64_t)SRC(ip, i*32+21) << 14;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*11+ 7) |= (uint64_t)SRC(ip, i*32+22) << 36 | (uint64_t)SRC1(ip, i*32+23) << 58;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*11+ 8)  = (uint32_t)SRC(ip, i*32+23) >>  6;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*11+ 8) |= (uint64_t)SRC(ip, i*32+24) << 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*11+ 8) |= (uint64_t)SRC(ip, i*32+25) << 38 | (uint64_t)SRC1(ip, i*32+26) << 60;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*11+ 9)  = (uint32_t)SRC(ip, i*32+26) >>  4;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*11+ 9) |= (uint64_t)SRC(ip, i*32+27) << 18;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*11+ 9) |= (uint64_t)SRC(ip, i*32+28) << 40 | (uint64_t)SRC1(ip, i*32+29) << 62;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*11+10)  = (uint32_t)SRC(ip, i*32+29) >>  2;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*11+10) |= (uint64_t)SRC(ip, i*32+30) << 20;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*11+10) |= (uint64_t)SRC(ip, i*32+31) << 42;\
}

#define BITPACK64_22(ip,  op, parm) { \
  BITBLK64_22(ip, 0, op, parm);  SRCI(ip); op += 22*4/sizeof(op[0]);\
}

#define BITBLK64_23(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*23+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*23+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 23 | (uint64_t)SRC1(ip, i*64+2) << 46;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*23+ 1)  = (uint32_t)SRC(ip, i*64+ 2) >> 18;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*23+ 1) |= (uint32_t)SRC(ip, i*64+ 3) <<  5;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*23+ 1) |= (uint64_t)SRC(ip, i*64+ 4) << 28 | (uint64_t)SRC1(ip, i*64+5) << 51;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*23+ 2)  = (uint32_t)SRC(ip, i*64+ 5) >> 13;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*23+ 2) |= (uint64_t)SRC(ip, i*64+ 6) << 10;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*23+ 2) |= (uint64_t)SRC(ip, i*64+ 7) << 33 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*23+ 3)  = (uint32_t)SRC(ip, i*64+ 8) >>  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*23+ 3) |= (uint64_t)SRC(ip, i*64+ 9) << 15;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*23+ 3) |= (uint64_t)SRC(ip, i*64+10) << 38 | (uint64_t)SRC1(ip, i*64+11) << 61;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*23+ 4)  = (uint32_t)SRC(ip, i*64+11) >>  3;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*23+ 4) |= (uint64_t)SRC(ip, i*64+12) << 20 | (uint64_t)SRC1(ip, i*64+13) << 43;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*23+ 5)  = (uint32_t)SRC(ip, i*64+13) >> 21;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*23+ 5) |= (uint32_t)SRC(ip, i*64+14) <<  2;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*23+ 5) |= (uint64_t)SRC(ip, i*64+15) << 25 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*23+ 6)  = (uint32_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*23+ 6) |= (uint32_t)SRC(ip, i*64+17) <<  7;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*23+ 6) |= (uint64_t)SRC(ip, i*64+18) << 30 | (uint64_t)SRC1(ip, i*64+19) << 53;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*23+ 7)  = (uint32_t)SRC(ip, i*64+19) >> 11;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*23+ 7) |= (uint64_t)SRC(ip, i*64+20) << 12;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*23+ 7) |= (uint64_t)SRC(ip, i*64+21) << 35 | (uint64_t)SRC1(ip, i*64+22) << 58;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*23+ 8)  = (uint32_t)SRC(ip, i*64+22) >>  6;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*23+ 8) |= (uint64_t)SRC(ip, i*64+23) << 17;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*23+ 8) |= (uint64_t)SRC(ip, i*64+24) << 40 | (uint64_t)SRC1(ip, i*64+25) << 63;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*23+ 9)  = (uint32_t)SRC(ip, i*64+25) >>  1;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*23+ 9) |= (uint64_t)SRC(ip, i*64+26) << 22 | (uint64_t)SRC1(ip, i*64+27) << 45;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*23+10)  = (uint32_t)SRC(ip, i*64+27) >> 19;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*23+10) |= (uint32_t)SRC(ip, i*64+28) <<  4;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*23+10) |= (uint64_t)SRC(ip, i*64+29) << 27 | (uint64_t)SRC1(ip, i*64+30) << 50;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*23+11)  = (uint32_t)SRC(ip, i*64+30) >> 14;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*23+11) |= (uint32_t)SRC(ip, i*64+31) <<  9;\
}

#define BITPACK64_23(ip,  op, parm) { \
  BITBLK64_23(ip, 0, op, parm);  SRCI(ip); op += 23*4/sizeof(op[0]);\
}

#define BITBLK64_24(ip, i, op, parm) { ;\
  IPPB(ip, i*8+ 0, parm); *((uint64_t *)op+i*3+ 0)  = (uint32_t)SRC(ip, i*8+ 0)      ;\
  IPPB(ip, i*8+ 1, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*8+ 1) << 24 | (uint64_t)SRC1(ip, i*8+2) << 48;\
  IPPB(ip, i*8+ 2, parm); *((uint64_t *)op+i*3+ 1)  = (uint32_t)SRC(ip, i*8+ 2) >> 16;\
  IPPB(ip, i*8+ 3, parm); *((uint64_t *)op+i*3+ 1) |= (uint32_t)SRC(ip, i*8+ 3) <<  8;\
  IPPB(ip, i*8+ 4, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*8+ 4) << 32 | (uint64_t)SRC1(ip, i*8+5) << 56;\
  IPPB(ip, i*8+ 5, parm); *((uint64_t *)op+i*3+ 2)  = (uint32_t)SRC(ip, i*8+ 5) >>  8;\
  IPPB(ip, i*8+ 6, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*8+ 6) << 16;\
  IPPB(ip, i*8+ 7, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*8+ 7) << 40;\
}

#define BITPACK64_24(ip,  op, parm) { \
  BITBLK64_24(ip, 0, op, parm);\
  BITBLK64_24(ip, 1, op, parm);\
  BITBLK64_24(ip, 2, op, parm);\
  BITBLK64_24(ip, 3, op, parm);  SRCI(ip); op += 24*4/sizeof(op[0]);\
}

#define BITBLK64_25(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*25+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*25+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 25 | (uint64_t)SRC1(ip, i*64+2) << 50;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*25+ 1)  = (uint32_t)SRC(ip, i*64+ 2) >> 14;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*25+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 11;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*25+ 1) |= (uint64_t)SRC(ip, i*64+ 4) << 36 | (uint64_t)SRC1(ip, i*64+5) << 61;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*25+ 2)  = (uint32_t)SRC(ip, i*64+ 5) >>  3;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*25+ 2) |= (uint64_t)SRC(ip, i*64+ 6) << 22 | (uint64_t)SRC1(ip, i*64+7) << 47;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*25+ 3)  = (uint32_t)SRC(ip, i*64+ 7) >> 17;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*25+ 3) |= (uint64_t)SRC(ip, i*64+ 8) <<  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*25+ 3) |= (uint64_t)SRC(ip, i*64+ 9) << 33 | (uint64_t)SRC1(ip, i*64+10) << 58;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*25+ 4)  = (uint32_t)SRC(ip, i*64+10) >>  6;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*25+ 4) |= (uint64_t)SRC(ip, i*64+11) << 19 | (uint64_t)SRC1(ip, i*64+12) << 44;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*25+ 5)  = (uint32_t)SRC(ip, i*64+12) >> 20;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*25+ 5) |= (uint32_t)SRC(ip, i*64+13) <<  5;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*25+ 5) |= (uint64_t)SRC(ip, i*64+14) << 30 | (uint64_t)SRC1(ip, i*64+15) << 55;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*25+ 6)  = (uint32_t)SRC(ip, i*64+15) >>  9;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*25+ 6) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 41;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*25+ 7)  = (uint32_t)SRC(ip, i*64+17) >> 23;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*25+ 7) |= (uint32_t)SRC(ip, i*64+18) <<  2;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*25+ 7) |= (uint64_t)SRC(ip, i*64+19) << 27 | (uint64_t)SRC1(ip, i*64+20) << 52;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*25+ 8)  = (uint32_t)SRC(ip, i*64+20) >> 12;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*25+ 8) |= (uint64_t)SRC(ip, i*64+21) << 13;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*25+ 8) |= (uint64_t)SRC(ip, i*64+22) << 38 | (uint64_t)SRC1(ip, i*64+23) << 63;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*25+ 9)  = (uint32_t)SRC(ip, i*64+23) >>  1;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*25+ 9) |= (uint64_t)SRC(ip, i*64+24) << 24 | (uint64_t)SRC1(ip, i*64+25) << 49;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*25+10)  = (uint32_t)SRC(ip, i*64+25) >> 15;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*25+10) |= (uint64_t)SRC(ip, i*64+26) << 10;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*25+10) |= (uint64_t)SRC(ip, i*64+27) << 35 | (uint64_t)SRC1(ip, i*64+28) << 60;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*25+11)  = (uint32_t)SRC(ip, i*64+28) >>  4;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*25+11) |= (uint64_t)SRC(ip, i*64+29) << 21 | (uint64_t)SRC1(ip, i*64+30) << 46;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*25+12)  = (uint32_t)SRC(ip, i*64+30) >> 18;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*25+12) |= (uint32_t)SRC(ip, i*64+31) <<  7;\
}

#define BITPACK64_25(ip,  op, parm) { \
  BITBLK64_25(ip, 0, op, parm);  SRCI(ip); op += 25*4/sizeof(op[0]);\
}

#define BITBLK64_26(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*13+ 0)  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*13+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 26 | (uint64_t)SRC1(ip, i*32+2) << 52;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*13+ 1)  = (uint32_t)SRC(ip, i*32+ 2) >> 12;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*32+ 3) << 14 | (uint64_t)SRC1(ip, i*32+4) << 40;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*13+ 2)  = (uint32_t)SRC(ip, i*32+ 4) >> 24;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*13+ 2) |= (uint32_t)SRC(ip, i*32+ 5) <<  2;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*32+ 6) << 28 | (uint64_t)SRC1(ip, i*32+7) << 54;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*13+ 3)  = (uint32_t)SRC(ip, i*32+ 7) >> 10;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*32+ 8) << 16 | (uint64_t)SRC1(ip, i*32+9) << 42;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*13+ 4)  = (uint32_t)SRC(ip, i*32+ 9) >> 22;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*13+ 4) |= (uint32_t)SRC(ip, i*32+10) <<  4;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*32+11) << 30 | (uint64_t)SRC1(ip, i*32+12) << 56;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*13+ 5)  = (uint32_t)SRC(ip, i*32+12) >>  8;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*32+13) << 18 | (uint64_t)SRC1(ip, i*32+14) << 44;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*13+ 6)  = (uint32_t)SRC(ip, i*32+14) >> 20;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*13+ 6) |= (uint32_t)SRC(ip, i*32+15) <<  6;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*13+ 6) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 58;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*13+ 7)  = (uint32_t)SRC(ip, i*32+17) >>  6;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*13+ 7) |= (uint64_t)SRC(ip, i*32+18) << 20 | (uint64_t)SRC1(ip, i*32+19) << 46;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*13+ 8)  = (uint32_t)SRC(ip, i*32+19) >> 18;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*13+ 8) |= (uint64_t)SRC(ip, i*32+20) <<  8;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*13+ 8) |= (uint64_t)SRC(ip, i*32+21) << 34 | (uint64_t)SRC1(ip, i*32+22) << 60;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*13+ 9)  = (uint32_t)SRC(ip, i*32+22) >>  4;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*13+ 9) |= (uint64_t)SRC(ip, i*32+23) << 22 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*13+10)  = (uint32_t)SRC(ip, i*32+24) >> 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*13+10) |= (uint64_t)SRC(ip, i*32+25) << 10;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*13+10) |= (uint64_t)SRC(ip, i*32+26) << 36 | (uint64_t)SRC1(ip, i*32+27) << 62;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*13+11)  = (uint32_t)SRC(ip, i*32+27) >>  2;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*13+11) |= (uint64_t)SRC(ip, i*32+28) << 24 | (uint64_t)SRC1(ip, i*32+29) << 50;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*13+12)  = (uint32_t)SRC(ip, i*32+29) >> 14;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*13+12) |= (uint64_t)SRC(ip, i*32+30) << 12;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*13+12) |= (uint64_t)SRC(ip, i*32+31) << 38;\
}

#define BITPACK64_26(ip,  op, parm) { \
  BITBLK64_26(ip, 0, op, parm);  SRCI(ip); op += 26*4/sizeof(op[0]);\
}

#define BITBLK64_27(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*27+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*27+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 27 | (uint64_t)SRC1(ip, i*64+2) << 54;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*27+ 1)  = (uint32_t)SRC(ip, i*64+ 2) >> 10;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*27+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 17 | (uint64_t)SRC1(ip, i*64+4) << 44;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*27+ 2)  = (uint32_t)SRC(ip, i*64+ 4) >> 20;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*27+ 2) |= (uint64_t)SRC(ip, i*64+ 5) <<  7;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*27+ 2) |= (uint64_t)SRC(ip, i*64+ 6) << 34 | (uint64_t)SRC1(ip, i*64+7) << 61;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*27+ 3)  = (uint32_t)SRC(ip, i*64+ 7) >>  3;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*27+ 3) |= (uint64_t)SRC(ip, i*64+ 8) << 24 | (uint64_t)SRC1(ip, i*64+9) << 51;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*27+ 4)  = (uint32_t)SRC(ip, i*64+ 9) >> 13;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*27+ 4) |= (uint64_t)SRC(ip, i*64+10) << 14 | (uint64_t)SRC1(ip, i*64+11) << 41;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*27+ 5)  = (uint32_t)SRC(ip, i*64+11) >> 23;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*27+ 5) |= (uint32_t)SRC(ip, i*64+12) <<  4;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*27+ 5) |= (uint64_t)SRC(ip, i*64+13) << 31 | (uint64_t)SRC1(ip, i*64+14) << 58;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*27+ 6)  = (uint32_t)SRC(ip, i*64+14) >>  6;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*27+ 6) |= (uint64_t)SRC(ip, i*64+15) << 21 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*27+ 7)  = (uint32_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*27+ 7) |= (uint64_t)SRC(ip, i*64+17) << 11 | (uint64_t)SRC1(ip, i*64+18) << 38;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*27+ 8)  = (uint32_t)SRC(ip, i*64+18) >> 26;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*27+ 8) |= (uint32_t)SRC(ip, i*64+19) <<  1;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*27+ 8) |= (uint64_t)SRC(ip, i*64+20) << 28 | (uint64_t)SRC1(ip, i*64+21) << 55;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*27+ 9)  = (uint32_t)SRC(ip, i*64+21) >>  9;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*27+ 9) |= (uint64_t)SRC(ip, i*64+22) << 18 | (uint64_t)SRC1(ip, i*64+23) << 45;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*27+10)  = (uint32_t)SRC(ip, i*64+23) >> 19;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*27+10) |= (uint64_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*27+10) |= (uint64_t)SRC(ip, i*64+25) << 35 | (uint64_t)SRC1(ip, i*64+26) << 62;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*27+11)  = (uint32_t)SRC(ip, i*64+26) >>  2;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*27+11) |= (uint64_t)SRC(ip, i*64+27) << 25 | (uint64_t)SRC1(ip, i*64+28) << 52;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*27+12)  = (uint32_t)SRC(ip, i*64+28) >> 12;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*27+12) |= (uint64_t)SRC(ip, i*64+29) << 15 | (uint64_t)SRC1(ip, i*64+30) << 42;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*27+13)  = (uint32_t)SRC(ip, i*64+30) >> 22;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*27+13) |= (uint32_t)SRC(ip, i*64+31) <<  5;\
}

#define BITPACK64_27(ip,  op, parm) { \
  BITBLK64_27(ip, 0, op, parm);  SRCI(ip); op += 27*4/sizeof(op[0]);\
}

#define BITBLK64_28(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*7+ 0)  = (uint32_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*7+ 0) |= (uint64_t)SRC(ip, i*16+ 1) << 28 | (uint64_t)SRC1(ip, i*16+2) << 56;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*7+ 1)  = (uint32_t)SRC(ip, i*16+ 2) >>  8;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*16+ 3) << 20 | (uint64_t)SRC1(ip, i*16+4) << 48;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*7+ 2)  = (uint32_t)SRC(ip, i*16+ 4) >> 16;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*7+ 2) |= (uint64_t)SRC(ip, i*16+ 5) << 12 | (uint64_t)SRC1(ip, i*16+6) << 40;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*7+ 3)  = (uint32_t)SRC(ip, i*16+ 6) >> 24;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*7+ 3) |= (uint32_t)SRC(ip, i*16+ 7) <<  4;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*16+ 8) << 32 | (uint64_t)SRC1(ip, i*16+9) << 60;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*7+ 4)  = (uint32_t)SRC(ip, i*16+ 9) >>  4;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*7+ 4) |= (uint64_t)SRC(ip, i*16+10) << 24 | (uint64_t)SRC1(ip, i*16+11) << 52;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*7+ 5)  = (uint32_t)SRC(ip, i*16+11) >> 12;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*16+12) << 16 | (uint64_t)SRC1(ip, i*16+13) << 44;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*7+ 6)  = (uint32_t)SRC(ip, i*16+13) >> 20;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*16+14) <<  8;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*16+15) << 36;\
}

#define BITPACK64_28(ip,  op, parm) { \
  BITBLK64_28(ip, 0, op, parm);\
  BITBLK64_28(ip, 1, op, parm);  SRCI(ip); op += 28*4/sizeof(op[0]);\
}

#define BITBLK64_29(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*29+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*29+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 29 | (uint64_t)SRC1(ip, i*64+2) << 58;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*29+ 1)  = (uint32_t)SRC(ip, i*64+ 2) >>  6;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*29+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 23 | (uint64_t)SRC1(ip, i*64+4) << 52;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*29+ 2)  = (uint32_t)SRC(ip, i*64+ 4) >> 12;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*29+ 2) |= (uint64_t)SRC(ip, i*64+ 5) << 17 | (uint64_t)SRC1(ip, i*64+6) << 46;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*29+ 3)  = (uint32_t)SRC(ip, i*64+ 6) >> 18;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*29+ 3) |= (uint64_t)SRC(ip, i*64+ 7) << 11 | (uint64_t)SRC1(ip, i*64+8) << 40;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*29+ 4)  = (uint32_t)SRC(ip, i*64+ 8) >> 24;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*29+ 4) |= (uint64_t)SRC(ip, i*64+ 9) <<  5;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*29+ 4) |= (uint64_t)SRC(ip, i*64+10) << 34 | (uint64_t)SRC1(ip, i*64+11) << 63;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*29+ 5)  = (uint32_t)SRC(ip, i*64+11) >>  1;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*29+ 5) |= (uint64_t)SRC(ip, i*64+12) << 28 | (uint64_t)SRC1(ip, i*64+13) << 57;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*29+ 6)  = (uint32_t)SRC(ip, i*64+13) >>  7;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*29+ 6) |= (uint64_t)SRC(ip, i*64+14) << 22 | (uint64_t)SRC1(ip, i*64+15) << 51;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*29+ 7)  = (uint32_t)SRC(ip, i*64+15) >> 13;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*29+ 7) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 45;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*29+ 8)  = (uint32_t)SRC(ip, i*64+17) >> 19;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*29+ 8) |= (uint64_t)SRC(ip, i*64+18) << 10 | (uint64_t)SRC1(ip, i*64+19) << 39;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*29+ 9)  = (uint32_t)SRC(ip, i*64+19) >> 25;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*29+ 9) |= (uint64_t)SRC(ip, i*64+20) <<  4;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*29+ 9) |= (uint64_t)SRC(ip, i*64+21) << 33 | (uint64_t)SRC1(ip, i*64+22) << 62;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*29+10)  = (uint32_t)SRC(ip, i*64+22) >>  2;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*29+10) |= (uint64_t)SRC(ip, i*64+23) << 27 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*29+11)  = (uint32_t)SRC(ip, i*64+24) >>  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*29+11) |= (uint64_t)SRC(ip, i*64+25) << 21 | (uint64_t)SRC1(ip, i*64+26) << 50;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*29+12)  = (uint32_t)SRC(ip, i*64+26) >> 14;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*29+12) |= (uint64_t)SRC(ip, i*64+27) << 15 | (uint64_t)SRC1(ip, i*64+28) << 44;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*29+13)  = (uint32_t)SRC(ip, i*64+28) >> 20;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*29+13) |= (uint64_t)SRC(ip, i*64+29) <<  9 | (uint64_t)SRC1(ip, i*64+30) << 38;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*29+14)  = (uint32_t)SRC(ip, i*64+30) >> 26;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*29+14) |= (uint32_t)SRC(ip, i*64+31) <<  3;\
}

#define BITPACK64_29(ip,  op, parm) { \
  BITBLK64_29(ip, 0, op, parm);  SRCI(ip); op += 29*4/sizeof(op[0]);\
}

#define BITBLK64_30(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*15+ 0)  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*15+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 30 | (uint64_t)SRC1(ip, i*32+2) << 60;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*15+ 1)  = (uint32_t)SRC(ip, i*32+ 2) >>  4;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*15+ 1) |= (uint64_t)SRC(ip, i*32+ 3) << 26 | (uint64_t)SRC1(ip, i*32+4) << 56;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*15+ 2)  = (uint32_t)SRC(ip, i*32+ 4) >>  8;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*15+ 2) |= (uint64_t)SRC(ip, i*32+ 5) << 22 | (uint64_t)SRC1(ip, i*32+6) << 52;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*15+ 3)  = (uint32_t)SRC(ip, i*32+ 6) >> 12;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*32+ 7) << 18 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*15+ 4)  = (uint32_t)SRC(ip, i*32+ 8) >> 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*15+ 4) |= (uint64_t)SRC(ip, i*32+ 9) << 14 | (uint64_t)SRC1(ip, i*32+10) << 44;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*15+ 5)  = (uint32_t)SRC(ip, i*32+10) >> 20;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*15+ 5) |= (uint64_t)SRC(ip, i*32+11) << 10 | (uint64_t)SRC1(ip, i*32+12) << 40;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*15+ 6)  = (uint32_t)SRC(ip, i*32+12) >> 24;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*15+ 6) |= (uint64_t)SRC(ip, i*32+13) <<  6 | (uint64_t)SRC1(ip, i*32+14) << 36;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*15+ 7)  = (uint32_t)SRC(ip, i*32+14) >> 28;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*15+ 7) |= (uint32_t)SRC(ip, i*32+15) <<  2;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*15+ 7) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 62;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*15+ 8)  = (uint32_t)SRC(ip, i*32+17) >>  2;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*15+ 8) |= (uint64_t)SRC(ip, i*32+18) << 28 | (uint64_t)SRC1(ip, i*32+19) << 58;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*15+ 9)  = (uint32_t)SRC(ip, i*32+19) >>  6;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*15+ 9) |= (uint64_t)SRC(ip, i*32+20) << 24 | (uint64_t)SRC1(ip, i*32+21) << 54;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*15+10)  = (uint32_t)SRC(ip, i*32+21) >> 10;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*15+10) |= (uint64_t)SRC(ip, i*32+22) << 20 | (uint64_t)SRC1(ip, i*32+23) << 50;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*15+11)  = (uint32_t)SRC(ip, i*32+23) >> 14;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*15+11) |= (uint64_t)SRC(ip, i*32+24) << 16 | (uint64_t)SRC1(ip, i*32+25) << 46;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*15+12)  = (uint32_t)SRC(ip, i*32+25) >> 18;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*15+12) |= (uint64_t)SRC(ip, i*32+26) << 12 | (uint64_t)SRC1(ip, i*32+27) << 42;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*15+13)  = (uint32_t)SRC(ip, i*32+27) >> 22;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*15+13) |= (uint64_t)SRC(ip, i*32+28) <<  8 | (uint64_t)SRC1(ip, i*32+29) << 38;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*15+14)  = (uint32_t)SRC(ip, i*32+29) >> 26;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*15+14) |= (uint64_t)SRC(ip, i*32+30) <<  4;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*15+14) |= (uint64_t)SRC(ip, i*32+31) << 34;\
}

#define BITPACK64_30(ip,  op, parm) { \
  BITBLK64_30(ip, 0, op, parm);  SRCI(ip); op += 30*4/sizeof(op[0]);\
}

#define BITBLK64_31(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*31+ 0)  = (uint32_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*31+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 31 | (uint64_t)SRC1(ip, i*64+2) << 62;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*31+ 1)  = (uint32_t)SRC(ip, i*64+ 2) >>  2;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*31+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 29 | (uint64_t)SRC1(ip, i*64+4) << 60;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*31+ 2)  = (uint32_t)SRC(ip, i*64+ 4) >>  4;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*31+ 2) |= (uint64_t)SRC(ip, i*64+ 5) << 27 | (uint64_t)SRC1(ip, i*64+6) << 58;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*31+ 3)  = (uint32_t)SRC(ip, i*64+ 6) >>  6;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*31+ 3) |= (uint64_t)SRC(ip, i*64+ 7) << 25 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*31+ 4)  = (uint32_t)SRC(ip, i*64+ 8) >>  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*31+ 4) |= (uint64_t)SRC(ip, i*64+ 9) << 23 | (uint64_t)SRC1(ip, i*64+10) << 54;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*31+ 5)  = (uint32_t)SRC(ip, i*64+10) >> 10;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*31+ 5) |= (uint64_t)SRC(ip, i*64+11) << 21 | (uint64_t)SRC1(ip, i*64+12) << 52;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*31+ 6)  = (uint32_t)SRC(ip, i*64+12) >> 12;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*31+ 6) |= (uint64_t)SRC(ip, i*64+13) << 19 | (uint64_t)SRC1(ip, i*64+14) << 50;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*31+ 7)  = (uint32_t)SRC(ip, i*64+14) >> 14;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*31+ 7) |= (uint64_t)SRC(ip, i*64+15) << 17 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*31+ 8)  = (uint32_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*31+ 8) |= (uint64_t)SRC(ip, i*64+17) << 15 | (uint64_t)SRC1(ip, i*64+18) << 46;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*31+ 9)  = (uint32_t)SRC(ip, i*64+18) >> 18;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*31+ 9) |= (uint64_t)SRC(ip, i*64+19) << 13 | (uint64_t)SRC1(ip, i*64+20) << 44;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*31+10)  = (uint32_t)SRC(ip, i*64+20) >> 20;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*31+10) |= (uint64_t)SRC(ip, i*64+21) << 11 | (uint64_t)SRC1(ip, i*64+22) << 42;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*31+11)  = (uint32_t)SRC(ip, i*64+22) >> 22;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*31+11) |= (uint64_t)SRC(ip, i*64+23) <<  9 | (uint64_t)SRC1(ip, i*64+24) << 40;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*31+12)  = (uint32_t)SRC(ip, i*64+24) >> 24;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*31+12) |= (uint64_t)SRC(ip, i*64+25) <<  7 | (uint64_t)SRC1(ip, i*64+26) << 38;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*31+13)  = (uint32_t)SRC(ip, i*64+26) >> 26;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*31+13) |= (uint64_t)SRC(ip, i*64+27) <<  5 | (uint64_t)SRC1(ip, i*64+28) << 36;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*31+14)  = (uint32_t)SRC(ip, i*64+28) >> 28;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*31+14) |= (uint64_t)SRC(ip, i*64+29) <<  3 | (uint64_t)SRC1(ip, i*64+30) << 34;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*31+15)  = (uint32_t)SRC(ip, i*64+30) >> 30;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*31+15) |= (uint32_t)SRC(ip, i*64+31) <<  1;\
}

#define BITPACK64_31(ip,  op, parm) { \
  BITBLK64_31(ip, 0, op, parm);  SRCI(ip); op += 31*4/sizeof(op[0]);\
}

#define BITBLK64_32(ip, i, op, parm) { \
  IPPB(ip, i*2+ 0, parm); *(uint32_t *)(op+i*8+ 0) = SRC(ip, i*2+ 0);\
  IPPB(ip, i*2+ 1, parm); *(uint32_t *)(op+i*8+ 4) = SRC(ip, i*2+ 1);;\
}

#define BITPACK64_32(ip,  op, parm) { \
  BITBLK64_32(ip, 0, op, parm);\
  BITBLK64_32(ip, 1, op, parm);\
  BITBLK64_32(ip, 2, op, parm);\
  BITBLK64_32(ip, 3, op, parm);\
  BITBLK64_32(ip, 4, op, parm);\
  BITBLK64_32(ip, 5, op, parm);\
  BITBLK64_32(ip, 6, op, parm);\
  BITBLK64_32(ip, 7, op, parm);\
  BITBLK64_32(ip, 8, op, parm);\
  BITBLK64_32(ip, 9, op, parm);\
  BITBLK64_32(ip, 10, op, parm);\
  BITBLK64_32(ip, 11, op, parm);\
  BITBLK64_32(ip, 12, op, parm);\
  BITBLK64_32(ip, 13, op, parm);\
  BITBLK64_32(ip, 14, op, parm);\
  BITBLK64_32(ip, 15, op, parm);  SRCI(ip); op += 32*4/sizeof(op[0]);\
}

